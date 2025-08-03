#define _WIN32_WINNT 0x0501
#define _WIN32_WINDOWS 0x0400

#include <windows.h>
#include <mmsystem.h>
#include <Imm.h>
#include "ui.h"
#include "../xml/xmlui.h"
#include "../tools/getfontnamefromfile/getfontnamefromfile.h"


int	GetXmlType(int type);
int	GetXmlAnchorPoint(int point);
int	GetXmlLayer(int layer);
RuFontWeight GetXmlRuFontWeight(int weight);
RuFontOutline GetXmlRuFontOutline(int outline);
int	GetXmlFontJustifyVType(int type);
int	GetXmlFontJustifyHType(int type);
TextureAlphaMode GetXmlTextureAlphaMode(int alphaMode);

// IME±±¨î
HIMC		g_hImc						= NULL;

// ----------------------------------------------------------------------------------
CUi::CUi(lua_State* L, HWND hWnd)
{
	m_lock.clear();

	m_hWnd				= hWnd;
	m_luaState			= L;
	m_pRootFrame		= NULL;
	m_pCapture			= NULL;
	m_pKeyDownFrame		= NULL;
	m_pKeyboardFocus	= NULL;
	m_pKeyCapture		= NULL;
	m_pMouseOver		= NULL;
	m_pDragFrame		= NULL;
	m_pMouseFocus		= NULL;
	m_pMouseExclude		= NULL;

	for ( int i = 0; i < MAX_MOUSE_BUTTON_NUM; i++ )
	{
		m_mouseDown[i] = NULL;
		m_lastmouseDown[i] = NULL;
		m_mouseDownTime[i] = 0.0f;
		m_lastmouseDownTime[i] = 0.0f;
	}

	m_captureKey		= 0;
	m_elapsedTime		= 0.0f;
	m_absoluteTime		= 0.0f;
	m_scale				= 1.0f;
	m_cursorPos.m_x		= 0.0f;
	m_cursorPos.m_y		= 0.0f;
	m_cursorOffset.m_x	= 0.0f;
	m_cursorOffset.m_y	= 0.0f;
	m_capturePoint.m_x	= 0.0f;
	m_capturePoint.m_y	= 0.0f;
	m_caretPoint.m_x	= 0.0f;
	m_caretPoint.m_y	= 0.0f;
	m_CurrentIMESize	= 0;
	m_eventLockedCount	= 0;

	m_object.clear();
	m_userEvent.clear();

	m_xmlUi				= NULL;	
	m_pRootFrame		= new CUiFrame("ui_system_root_frame", NULL, this);
	m_pRootFrame->SetMouseEnable(true);
	m_pRootFrame->RegisterForClicks(0x0F);


	m_hkl = NULL;
	m_ImeProp = 0;
	ZeroMemory(&m_csInfo, sizeof(m_csInfo));
	m_CurCP = 0;
	m_IsComStr = false;

#ifdef UNICODE
#else	
	ZeroMemory(m_asbText, sizeof(m_asbText));
	m_asbTextLen = 0;;
#endif

	//m_pRootFrame->SetKeyboardEnable(true);
	
	HINSTANCE hInstance = (HINSTANCE)GetWindowLong(hWnd, -6);

	
}

// ----------------------------------------------------------------------------------
CUi::~CUi()
{
	UI_SAFE_DELETE(m_pRootFrame);
	UI_SAFE_DELETE(m_xmlUi);

	
	/*
	if (m_luaState)
	lua_close(m_luaState);
	*/
	
}

// ----------------------------------------------------------------------------------
bool CUi::MsgProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	/*
	switch (uMsg)
	{
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYUP:
	case WM_MOUSEMOVE:
		break;

	default:
		{
			char buf[512];
			sprintf(buf, "Msg:%x wParam:%x lParam:%x", uMsg, wParam, lParam);
			UiOuputError(buf);
		}
		break;
	}
	*/
	
	bool boResult = false;

	switch ( uMsg )
	{
	case WM_CHAR:
		{
			wchar_t chBuf = (wchar_t)wParam;
			if ( wParam <= L' ' || wParam > 255) {
				if ( OnChar(&chBuf, 1) ) {
					return true;
				}
			} else {				
#ifdef UNICODE
				OnChar(&chBuf, 1);
#else
				asbAddByte((BYTE)wParam);
				return true;
#endif
			}
		}
		break;

	case WM_INPUTLANGCHANGE:
		//TRACE("WM_INPUTLANGCHANGE\n");
		{
			// IME info
			m_hkl = (HKL)lParam;
			m_ImeProp = ImmGetProperty(m_hkl, IGP_PROPERTY);
#ifdef UNICODE
			m_ImeProp = m_ImeProp | IME_PROP_UNICODE;
#endif
			// Charset info
			TranslateCharsetInfo((DWORD*)wParam, &m_csInfo, TCI_SRCCHARSET);
			m_CurCP = m_csInfo.ciACP;
			//TRACE("CP: %d\n", m_CurCP);

			// ¤w…î¤Á„ã¤F‰r¤Jªk¡CÉO­ìƒSªº‡ÛÕu¦AÆÓ‹×¨t
			asbSubmit();

			// HKL hkl = GetKeyboardLayout(0);

			// Korean IME always inserts on keystroke.  Other IMEs do not.
			if ( PRIMARYLANGID( LOWORD( m_hkl ) ) == LANG_KOREAN )
			{
				m_IsComStr = true;
			}
			else
			{
				m_IsComStr = false;
			}
		}
		return true;		
		
	case WM_IME_STARTCOMPOSITION:
		{
			COMPOSITIONFORM composition;
			HIMC hIMC = ImmGetContext(m_hWnd);
			if ( hIMC != NULL )
			{
				if ( m_pKeyboardFocus )
				{
					RECT rect;

  					GetClientRect(m_hWnd, &rect);
 					CUiPoint caretPoint = m_caretPoint;
					CUiPoint point = m_pKeyboardFocus->GetPos();
					CUiPoint size = m_pRootFrame->GetRealSize();

					point = caretPoint;
					point.m_x *= (rect.right - rect.left) / size.m_x;
					point.m_y *= (rect.bottom - rect.top) / size.m_y;;

					composition.dwStyle = CFS_POINT;
					composition.ptCurrentPos.x = (LONG)point.m_x;
					composition.ptCurrentPos.y = (LONG)point.m_y;
					ImmSetCompositionWindow(hIMC, &composition);
					m_CurrentIMESize = 0;	// scott
				}		

				ImmReleaseContext(m_hWnd, hIMC);
			}
		}
		break;

	case WM_IME_ENDCOMPOSITION:
		ClearCurrentIme();
		break;
	
	/*
	
	case WM_IME_NOTIFY:		
		switch ( wParam )
		{
		case IMN_SETOPENSTATUS:
			if ( m_pKeyboardFocus && m_pKeyboardFocus->m_uiType == CUiLayout::eUiType_EditBox ) {
				EnableIME();
				// ImmSetOpenStatus(ImmGetContext(m_hWnd), TRUE);
			} else {
				DisableIME();
				//ImmSetOpenStatus(ImmGetContext(m_hWnd), FALSE);
			}
			break;
		}
		break;
		//return true;
	*/

	case WM_IME_COMPOSITION:
		if (lParam & GCS_RESULTSTR) {
			HIMC hIMC = NULL;			
			LPBYTE lpBuf = NULL;
			LONG cchBuf = 0;

			hIMC = ImmGetContext(m_hWnd);
			if (hIMC != NULL) {
				if (m_ImeProp & IME_PROP_UNICODE) {
					// ¨ú±o¿é¤Jªk¼È¦s¤º¦r¦ê
					cchBuf = ImmGetCompositionStringW(hIMC, GCS_RESULTSTR, NULL, 0);
					if (cchBuf > 0) {
						lpBuf = (LPBYTE)malloc(cchBuf);

						if (lpBuf != NULL) {
							cchBuf = ImmGetCompositionStringW(hIMC, GCS_RESULTSTR, lpBuf, cchBuf);
							cchBuf = cchBuf / sizeof(wchar_t);
						}
						else {
							cchBuf = 0;
						}
					}
				}
				else {
					LPBYTE lpStr = NULL;
					LONG cchStr = 0;

					// ¨ú±o¿é¤Jªk¼È¦s¤º¦r¦ê
					cchStr = ImmGetCompositionStringA(hIMC, GCS_RESULTSTR, NULL, 0);
					if (cchStr > 0) {
						lpStr = (LPBYTE)malloc(cchStr);

						if (lpStr != NULL) {
							cchStr = ImmGetCompositionStringA(hIMC, GCS_RESULTSTR, lpStr, cchStr);
						}
						else {
							cchStr = 0;
						}
					}

					// Âà´«¦¨Unicode
					if (cchStr > 0) {
						cchBuf = MultiByteToWideChar(CP_ACP, 0, (LPSTR)lpStr, cchStr, NULL, 0);
						if (cchBuf>0) {
							lpBuf = (LPBYTE)malloc(cchBuf * sizeof(wchar_t));
							if (lpStr != NULL) {
								cchBuf = MultiByteToWideChar(CP_ACP, 0, (LPSTR)lpStr, cchStr, (LPWSTR)lpBuf, cchBuf);
							}
							else {
								cchBuf = 0;
							}
						}
					}

					// ÄÀ©ñ
					if (lpStr != NULL) free(lpStr);

				}
				ImmReleaseContext(m_hWnd, hIMC);
			}
			ClearCurrentIme();	// scott
			m_CurrentIMESize = 0;

			if (cchBuf > 0) {
				OnChar((LPWSTR)lpBuf, cchBuf);
			}

			if (lpBuf != NULL) free(lpBuf);

			if (cchBuf > 0) {
				return true;
			}

		} else if ( m_IsComStr && lParam & GCS_COMPSTR ) {
			HIMC hIMC;
			LPBYTE lpBuf = NULL;
			LONG cchBuf = 0;

			hIMC = ImmGetContext(m_hWnd);
			if (hIMC != NULL) {
 				if (m_ImeProp & IME_PROP_UNICODE) {
					// ¨ú±o¿é¤Jªk¼È¦s¤º¦r¦ê
					cchBuf = ImmGetCompositionStringW(hIMC, GCS_COMPSTR, NULL, 0);
					if (cchBuf > 0) {
						lpBuf = (LPBYTE)malloc(cchBuf);

						if (lpBuf != NULL) {
							cchBuf = ImmGetCompositionStringW(hIMC, GCS_COMPSTR, lpBuf, cchBuf);
							cchBuf = cchBuf / sizeof(wchar_t);
						}
						else {
							cchBuf = 0;
						}
					}
				}
				else {
					LPBYTE lpStr = NULL;
					LONG cchStr = 0;

					// ¨ú±o¿é¤Jªk¼È¦s¤º¦r¦ê
					cchStr = ImmGetCompositionStringA(hIMC, GCS_COMPSTR, NULL, 0);
					if (cchStr > 0) {
						lpStr = (LPBYTE)malloc(cchStr);

						if (lpStr != NULL) {
							cchStr = ImmGetCompositionStringA(hIMC, GCS_COMPSTR, lpStr, cchStr);
						}
						else {
							cchStr = 0;
						}
					}

					// Âà´«¦¨Unicode
					if (cchStr > 0) {
						cchBuf = MultiByteToWideChar(CP_ACP, 0, (LPSTR)lpStr, cchStr, NULL, 0);
						if (cchBuf>0) {
							lpBuf = (LPBYTE)malloc(cchBuf * sizeof(wchar_t));
							if (lpStr != NULL) {
								cchBuf = MultiByteToWideChar(CP_ACP, 0, (LPSTR)lpStr, cchStr, (LPWSTR)lpBuf, cchBuf);
							}
							else {
								cchBuf = 0;
							}
						}
					}

					// ÄÀ©ñ
					if (lpStr != NULL) free(lpStr);

				}				
				
				/*
				if ( lpBuf && lpBuf[0] != 0 ) {
					ImmSetCompositionString(hIMC, SCS_SETSTR, 0, 0, 0, 0);
				}
				*/

				ImmReleaseContext(m_hWnd, hIMC);
			}
			
			ClearCurrentIme();	// scott
			m_CurrentIMESize = cchBuf;			

			if (cchBuf > 0) {
				OnChar((LPWSTR)lpBuf, cchBuf);
			}

			if (lpBuf != NULL) free(lpBuf);

			if (cchBuf > 0) {
				return true;
			}
		}
		break;
	
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		{
			m_isUpdate = false;

			m_pKeyDownFrame = NULL;
			if ( !m_lock.empty() ) {
				m_pKeyDownFrame = (m_lock.back())->GetFrameAtKeyboard(NULL);
			} else if ( m_pKeyboardFocus && m_pKeyboardFocus->GetKeyboardState() ) {
				m_pKeyDownFrame = m_pKeyboardFocus;
			} else if ( m_pRootFrame ) {
				m_pKeyDownFrame = m_pRootFrame->GetFrameAtKeyboard(NULL);
			}

			if ( m_pKeyDownFrame ) {
				m_pKeyDownFrame->HandleKeyboard(uMsg, wParam, lParam);
				boResult = true;
			}

			m_isUpdate = true;
		}
		break;

	case WM_KEYUP:
	case WM_SYSKEYUP:
		{
			m_isUpdate = false;

			if ( m_pKeyDownFrame && m_pKeyDownFrame->IsVisible() ) {
				m_pKeyDownFrame->HandleKeyboard(uMsg, wParam, lParam);
				boResult = true;
			} else {
				m_pKeyDownFrame = NULL;
			}

			m_isUpdate = true;
		}
		
		break;
	}

	return boResult;
}

// ----------------------------------------------------------------------------------
bool CUi::OnChar(wchar_t* ws, int len)
{
	CUiFrame* frame;
	bool handleResult = false;

	if ( !m_lock.empty() ) {
		frame = (m_lock.back())->GetFrameAtKeyboard(NULL);
	} else if ( m_pKeyboardFocus && m_pKeyboardFocus->GetKeyboardState() ) {
		if ( m_pKeyboardFocus != m_pKeyDownFrame )
			return true;
		frame = m_pKeyboardFocus;
	} else if ( m_pRootFrame ) {
		frame = m_pRootFrame->GetFrameAtKeyboard(NULL);
	}

	if ( frame ) {
		handleResult = true;
		for ( int i = 0; i < len; i++ )
			frame->HandleKeyboard(WM_CHAR, ws[i], 0);
	}

	return handleResult;
}

// ----------------------------------------------------------------------------------
void CUi::ClearCurrentIme()
{
	if( m_CurrentIMESize > 0 )
	{
		if ( m_pKeyboardFocus )
		{
			for( int i = 0; i < m_CurrentIMESize; i++ )
			{
				m_pKeyboardFocus->HandleKeyboard(WM_CHAR, VK_BACK, 0);
				//m_pKeyDownFrame->HandleKeyboard(WM_KEYDOWN, VK_BACK, 0);
				//m_pKeyDownFrame->HandleKeyboard(WM_KEYUP, VK_BACK, 0);
			}
		}
	}
	m_CurrentIMESize = 0;
}

// ----------------------------------------------------------------------------------
void CUi::Update(float elapsedTime, InputState &state)
{
	
	m_pMouseFocus = NULL;
	// m_pKeyDownFrame = NULL;

	// §ó·s®É¶¡
	m_elapsedTime = elapsedTime;	

	// §ó·s¿é¤Jª¬ºA
	m_cursorOffset.m_x	= ((float)state.xPos - m_cursorPos.m_x) / m_scale;
	m_cursorOffset.m_y	= ((float)state.yPos - m_cursorPos.m_y) / m_scale;
	m_cursorPos.m_x		= (float)state.xPos;
	m_cursorPos.m_y		= (float)state.yPos;
	m_input.UpdateState( state, elapsedTime );	

	// ¨ú±o²Ä¤@­Ó³B²z¤¸¥ó
	CUiFrame* first = (m_lock.empty()) ? m_pRootFrame : m_lock.back();

	// Áä½L³B²z
	/*
	if (m_pKeyCapture)
	{
	if (!m_pKeyCapture->IsParentEnable())
	LockKeyboard(NULL);
	else if (m_pKeyCapture->HandleKeyboard(m_input))
	m_pKeyboardFocus = m_pKeyCapture;
	}	
	if (m_pKeyboardFocus == NULL && first)
	{
	CUiFrame* keyboardFocus = first->GetFrameAtKeyboard(NULL);
	if (keyboardFocus && keyboardFocus->HandleKeyboard(m_input))
	m_pKeyboardFocus = keyboardFocus;
	}	
	*/

	m_isUpdate = false;	

	
	CUiFrame* mouseIn = NULL;
	if (first)
		mouseIn = first->GetFrameAtPoint(m_cursorPos, m_pMouseExclude);

	// 2006/07/07 ­×¥¿Á×§K Capture ¤¸¥ó³QÂê¦í
	// ¬O§_¦³¤¸¥óÂê¦í·Æ¹«°T®§(©ì¦²¤¤)
	if ( m_pCapture )
	{
		m_pMouseFocus = m_pCapture;
		if ( !m_pCapture->HandleMouse(m_input) )
			SetCapture(NULL, 0);
	}
	else if ( mouseIn && mouseIn->HandleMouse(m_input) )
		m_pMouseFocus = mouseIn;

	/*
	// ¬O§_¦³¤¸¥óÂê¦í·Æ¹«°T®§(©ì¦²¤¤)
	if ( m_pCapture )
	{
	m_pMouseFocus = m_pCapture;
	m_pCapture->HandleMouse(m_input);
	}
	else if ( mouseIn && mouseIn->HandleMouse(m_input) )
	m_pMouseFocus = mouseIn;
	*/	

	// ÀË¬d¬O§_¦³·Æ¹««öÁä©ñ¶}
	for ( int i = 0; i < MAX_MOUSE_BUTTON_NUM; i++ )
	{
		if ( m_input.GetButtonUp(i) )
		{
			if (m_mouseDown[i])
			{
				if ( m_mouseDown[i]->IsEnable() )
				{
					m_mouseDown[i]->OnMouseUp(m_cursorPos, i);
				}
				SetMouseDown(i, NULL);
			}
		}
	}

	m_isUpdate = true;	

	// ¥Ø«e©Ò¦b¤¸¥ó¤£¦P®É
	if (m_pMouseOver != mouseIn)
	{
		if (m_pMouseOver)
			m_pMouseOver->OnMouseLeave(m_cursorPos);

		m_pMouseOver = mouseIn;

		if (m_pMouseOver)
			m_pMouseOver->OnMouseEnter(m_cursorPos);
	}

	
	if ( m_input.GetButtonDown(MOUSE_LBUTTON) )
	{
	}	

	if (m_pRootFrame)
		m_pRootFrame->Update(elapsedTime);
	
}

// ----------------------------------------------------------------------------------
void CUi::Render()
{	
	if (m_pRootFrame)
		m_pRootFrame->Render(1.0f);	
}

// ----------------------------------------------------------------------------------
void CUi::SetScale(float scale)
{
	if ( m_scale != scale )
	{
		m_scale = scale;
		m_pRootFrame->SetScale(m_scale);
	}
}

// ----------------------------------------------------------------------------------
void CUi::SetCapture(CUiFrame* pUiFrame, int key)
{
	if (m_pCapture != pUiFrame)
	{
		m_pCapture = pUiFrame;
		m_captureKey = key;
		m_capturePoint = m_cursorPos;

		/*
		char buf[512];
		if ( m_pCapture ) {
		sprintf_s(buf, 512, "SetCapture[%s] %x", m_pCapture->GetName(), (int)m_pCapture);
		} else {

		sprintf_s(buf, 512, "SetCapture[NULL] %x", (int)m_pCapture);
		}
		UiOuputError(buf);
		*/

		// ´£°ª³B²z¶¶§Ç
		if (m_pCapture)
			m_pCapture->AdjustFrameLevel();
	}
}

// ----------------------------------------------------------------------------------
void CUi::SetKeyboardFocus(CUiFrame* pUiFrame)
{
	if ( m_pKeyboardFocus != pUiFrame ) {
		ClearCurrentIme();
		if ( m_pKeyboardFocus ) {
			m_pKeyboardFocus->OnEditFocusLost(pUiFrame);
		}
		m_pKeyboardFocus = pUiFrame;		
		if ( m_pKeyboardFocus ) {
			m_pKeyboardFocus->OnEditFocusGained(pUiFrame);
		}

		if ( m_pKeyboardFocus && m_pKeyboardFocus->m_uiType == CUiLayout::eUiType_EditBox ) {
			EnableIME();
		} else {
			DisableIME();
		}
	}	
}

// ----------------------------------------------------------------------------------
void CUi::OnReceiveDrag(CUiPoint& pt, CUiFrame* pExclude)
{
	CUiFrame* first = (m_lock.empty()) ? m_pRootFrame : m_lock.back();
	if (first)
	{
		CUiFrame* pUiFrame = first->GetFrameAtPoint(pt, pExclude);
		if ( pUiFrame && pUiFrame->IsEnable() )
			pUiFrame->OnReceiveDrag();
	}
}

// ----------------------------------------------------------------------------------
void CUi::SetMouseDown(int key, CUiFrame* pUiFrame)
{
	if ( key >= 0 && key < MAX_MOUSE_BUTTON_NUM )
	{
		float time = timeGetTime() / 1000.0f;
		if ( pUiFrame && m_lastmouseDown[key] == pUiFrame && (time - m_lastmouseDownTime[key]) < 0.3f )
		{			
			if ( pUiFrame->GetClicksSet() & (0x01 << key) )
				pUiFrame->OnDoubleClick(CUiPoint((float)m_input.GetXPos(), (float)m_input.GetYPos()), key);
		}

		m_lastmouseDown[key] = m_mouseDown[key];
		m_lastmouseDownTime[key] = m_mouseDownTime[key];
		m_mouseDown[key] = pUiFrame;
		m_mouseDownTime[key] = time;
	}
}

// ----------------------------------------------------------------------------------
CUiFrame* CUi::GetMouseDown(int key)
{
	if ( key >= 0 && key < MAX_MOUSE_BUTTON_NUM )
		return m_mouseDown[key];
	return NULL;
}

// ----------------------------------------------------------------------------------
bool CUi::LoadScriptFile(const char* filename)
{
	if ( m_luaState == NULL || filename == NULL || filename[0] == 0 )
		return false;

	if ( luaL_loadfile(m_luaState, filename) != 0 || lua_resume(m_luaState, 0) != 0 )
	{
		const char* err = lua_tostring(m_luaState, -1);
		UiOuputError(err);
		lua_pop(m_luaState, 1);
		return false;
	}
	return true;
}

// ----------------------------------------------------------------------------------
bool CUi::LoadScriptFromBuffer(const char* buffer, int bufferSize, const char* filename)
{
	if ( m_luaState == NULL || buffer == NULL )
		return false;

	if ( filename == NULL )
		filename = "";

	char *tempData = ruNEW char [bufferSize + 1];
	memcpy( tempData, buffer, bufferSize );
	tempData[bufferSize] = 0;	

	if ( luaL_loadbuffer(m_luaState, tempData, bufferSize, 0) != 0 || lua_resume(m_luaState, 0) != 0 )
	{
		delete [] tempData;

		char errorStr[512];
		sprintf_s(errorStr, 512, "%s %s", filename, lua_tostring(m_luaState, -1));
		lua_pop(m_luaState, 1);
		UiOuputError(errorStr);		
		return false;
	} else {
		delete [] tempData;
		return true;
	}
}

// ----------------------------------------------------------------------------------
void CUi::InitializeXmlParser(const char* resourcePathname)
{
	assert(m_xmlUi == NULL);

	CXmlUi::Initialize();

	m_resourcePath = resourcePathname;
	m_xmlUi = new CXmlUi;
}

// ----------------------------------------------------------------------------------
void CUi::ReleaseXmlParser()
{
	UI_SAFE_DELETE(m_xmlUi);

	CXmlUi::Release();
}

// ----------------------------------------------------------------------------------
bool CUi::LoadXmlFile(const char* filename)
{
	bool loadResult = false;
	if ( m_xmlUi == NULL || filename == NULL )
		return false;

	const char* resourceName;
	if ( strncmp(filename, m_resourcePath.c_str(), m_resourcePath.size()) == 0 ) {
		resourceName = filename + m_resourcePath.size();
	} else {
		resourceName = filename;
	}

	int length = (int)strlen(resourceName);
	if ( length > 4 )
	{
		// Load Stream
		IRuStream* stream = g_ruResourceManager->LoadStream(resourceName);
		if ( stream  )
		{
			// Load Lua Files
			if ( strnicmp(&resourceName[length-4], ".lua", 4) == 0 )
			{				
				loadResult = LoadScriptFromBuffer((const char*)stream->OpenStreamMapping(), stream->GetStreamSize(), resourceName);
			}
			// Load Xml Files
			else if ( strnicmp(&resourceName[length-4], ".xml", 4) == 0 )
			{
				if ( m_xmlUi->LoadXmlFromMemory(stream->OpenStreamMapping(), stream->GetStreamSize(), resourceName) ) {
					CXmlNode* pXmlNode = m_xmlUi->GetNextXmlNode();
					while ( pXmlNode ) {
						if ( pXmlNode->GetType() == eXmlUiType_Script ) {
							// Script ®æ¦¡¤£¦P
							LoadXmlScript(pXmlNode);
						} else {
							// ¨ä¥L¤¸¥ó
							CXmlLayout* pXmlLayout = (CXmlLayout*)pXmlNode;
							if (!pXmlLayout->IsVirtual()) {
								const char* parentName = (pXmlLayout->m_parent) ? pXmlLayout->m_parent->m_name.c_str() : "";
								CUiFrame* pUiFrame = LoadXmlFrames(FindObject(parentName), pXmlLayout);
								if ( pUiFrame )
									pUiFrame->OnLoad();
							}
						}

						pXmlNode = m_xmlUi->GetNextXmlNode();
					}
					loadResult = true;
				}	
			}

			delete stream;
		}
	}
	return loadResult;
}

// ----------------------------------------------------------------------------------
bool CUi::LoadXmlBinding(const char* filename)
{
	bool loadResult = false;
	if ( m_xmlUi == NULL || filename == NULL )
		return false;

	const char* resourceName;
	if ( strncmp(filename, m_resourcePath.c_str(), m_resourcePath.size()) == 0 ) {
		resourceName = filename + m_resourcePath.size();
	} else {
		resourceName = filename;
	}

	// Load Stream
	IRuStream* stream = g_ruResourceManager->LoadStream(resourceName);
	if ( stream ) {
		// Load XML
		loadResult = m_xmlUi->LoadBindingFromMemory(stream->OpenStreamMapping(), stream->GetStreamSize());

		delete stream;
	}	
	return loadResult;
}

// ----------------------------------------------------------------------------------
void CUi::OpenTocFile(const char* filename, const char* resourcePath)
{
	if (m_xmlUi == NULL)
		m_xmlUi = new CXmlUi;

	m_resourcePath.empty();
	if ( resourcePath )
		m_resourcePath = resourcePath;

	if ( m_xmlUi->Initialize() )
	{		
		m_xmlUi->OpenToc(filename);
		m_xmlUi->LoadBindingFromFile(filename);			// Binding
	}
	m_xmlUi->Release();	

	/*
	for ( vector<CXmlBinding*>::iterator iter = m_xmlUi->m_bindings.begin(); iter != m_xmlUi->m_bindings.end(); iter++ )
	{
	CUiBinding binding;
	binding.m_name = (*iter)->m_name;
	binding.m_header = (*iter)->m_header;
	binding.m_content = (*iter)->m_content;
	binding.m_runOnUp = (*iter)->m_runOnUp;
	m_bindings.push_back(binding);
	}
	*/
}

// ----------------------------------------------------------------------------------
void CUi::BuildFrames()
{
	if ( m_xmlUi == NULL )
		return;	

	// ¸ü¤J xml nodes
	for ( vector<CXmlFile*>::iterator iter = m_xmlUi->m_files.begin(); iter != m_xmlUi->m_files.end(); iter++ )
	{
		LoadXmlFile(*iter);		
	}

	for ( vector<string>::iterator iter = m_xmlUi->m_errorList.begin(); iter != m_xmlUi->m_errorList.end(); iter++ )
	{
		UiOuputError((*iter).c_str());
	}

	UI_SAFE_DELETE(m_xmlUi);
}

// ----------------------------------------------------------------------------------
CUiLayout* CUi::CreateObject(int type, const char* name, CUiLayout* pParent, const char* inherit)
{
	CUiLayout* layout = NULL;

	/*
	if ( FindObject(name) != NULL )
		return 
	*/	

	if (pParent == NULL)
		pParent = m_pRootFrame;

	bool bIsWB = false;

	switch(type)
	{
	case CUiLayout::eUiType_FontString:
		layout = new CUiFontString(name, pParent, this);
		break;

	case CUiLayout::eUiType_Texture:
		layout = new CUiTexture(name, pParent, this);		
		break;

	case CUiLayout::eUiType_Frame:
		layout = new CUiFrame(name, pParent, this);		
		break;

	case CUiLayout::eUiType_Model:
		layout = new CUiModel(name, pParent, this);		
		break;

	case CUiLayout::eUiType_Button:
		layout = new CUiButton(name, pParent, this);		
		break;

	case CUiLayout::eUiType_CheckButton:
		layout = new CUiCheckButton(name, pParent, this);		
		break;

	case CUiLayout::eUiType_RadioButton:
		layout = new CUiRadioButton(name, pParent, this);		
		break;

	case CUiLayout::eUiType_EditBox:
		layout = new CUiEditBox(name, pParent, this);		
		break;

	case CUiLayout::eUiType_ListBox:
		layout = new CUiListBox(name, pParent, this);		
		break;

	case CUiLayout::eUiType_Slider:
		layout = new CUiSlider(name, pParent, this);		
		break;

	case CUiLayout::eUiType_StatusBar:
		layout = new CUiStatusBar(name, pParent, this);		
		break;

	case CUiLayout::eUiType_MessageFrame:
		layout = new CUiMessageFrame(name, pParent, this);
		break;

	case CUiLayout::eUiType_ScrollFrame:
		layout = new CUiScrollFrame(name, pParent, this);
		break;

	case CUiLayout::eUiType_Tooltip:
		layout = new CUiTooltip(name, pParent, this);
		break;
	}

	if (layout)
	{
		// «Ø¥ß Lua Table
		if (m_luaState)
		{
			int result = CUiLuaLayout::NewTable(m_luaState, layout);
			assert(result);
			layout->SetLuaMetaTable(m_luaState);
			lua_pop(m_luaState, 1);
		}

		const char* layout_name = layout->GetName();
		if (layout_name && layout_name[0])
		{
			if (m_luaState)
			{
				CUiLuaLayout::PushLayoutTable(m_luaState, layout);
				if (!lua_isnil(m_luaState, -1))
					lua_setglobal(m_luaState, layout_name);			// Set table name
				else
					lua_pop(m_luaState, 1);							// Pop nil value
			}
		}

		// ¬O§_¦³Ä~©Óª«¥ó
		if ( m_xmlUi && inherit )
		{
			CXmlLayout* layoutXml = NULL;
			CXmlLayout* parentXml = m_xmlUi->FindByName(pParent->GetName());
			switch (layout->GetUiType())
			{
			case CUiLayout::eUiType_FontString:
				layoutXml = m_xmlUi->CreateXmlLayers(L"FontString", parentXml, inherit, name);
				LoadXmlFontString(layout, layoutXml);
				break;

			case CUiLayout::eUiType_Texture:
				layoutXml = m_xmlUi->CreateXmlLayers(L"Texture", parentXml, inherit, name);
				layoutXml->m_name = name;
				LoadXmlTexture(layout, layoutXml);
				break;

			case CUiLayout::eUiType_Frame:
				layoutXml = m_xmlUi->CreateXmlFrames(L"Frame", parentXml, inherit, name);
				layoutXml->m_name = name;
				LoadXmlFrame(layout, layoutXml);
				break;

			case CUiLayout::eUiType_Model:
				layoutXml = m_xmlUi->CreateXmlFrames(L"Model", parentXml, inherit, name);
				layoutXml->m_name = name;
				LoadXmlModel(layout, layoutXml);
				break;

			case CUiLayout::eUiType_Button:
				layoutXml = m_xmlUi->CreateXmlFrames(L"Button", parentXml, inherit, name);
				layoutXml->m_name = name;
				LoadXmlButton(layout, layoutXml);
				break;

			case CUiLayout::eUiType_CheckButton:
				layoutXml = m_xmlUi->CreateXmlFrames(L"CheckButton", parentXml, inherit, name);
				LoadXmlCheckButton(layout, layoutXml);
				break;

			case CUiLayout::eUiType_RadioButton:
				layoutXml = m_xmlUi->CreateXmlFrames(L"RadioButton", parentXml, inherit, name);
				LoadXmlRadioButton(layout, layoutXml);
				break;

			case CUiLayout::eUiType_StatusBar:
				layoutXml = m_xmlUi->CreateXmlFrames(L"StatusBar", parentXml, inherit, name);
				LoadXmlStatusBar(layout, layoutXml);
				break;

			case CUiLayout::eUiType_Slider:
				layoutXml = m_xmlUi->CreateXmlFrames(L"Slider", parentXml, inherit, name);
				LoadXmlSlider(layout, layoutXml);
				break;

			case CUiLayout::eUiType_EditBox:
				layoutXml = m_xmlUi->CreateXmlFrames(L"EditBox", parentXml, inherit, name);
				LoadXmlEditBox(layout, layoutXml);
				break;

			case CUiLayout::eUiType_ListBox:
				layoutXml = m_xmlUi->CreateXmlFrames(L"ListBox", parentXml, inherit, name);
				LoadXmlListBox(layout, layoutXml);
				break;

			case CUiLayout::eUiType_MessageFrame:
				layoutXml = m_xmlUi->CreateXmlFrames(L"MessageFrame", parentXml, inherit, name);
				LoadXmlMessageFrame(layout, layoutXml);
				break;

			case CUiLayout::eUiType_ScrollFrame:
				layoutXml = m_xmlUi->CreateXmlFrames(L"ScrollFrame", parentXml, inherit, name);
				LoadXmlScrollFrame(layout, layoutXml);
				break;

			case CUiLayout::eUiType_Tooltip:
				layoutXml = m_xmlUi->CreateXmlFrames(L"GameTooltip", parentXml, inherit, name);
				LoadXmlTooltip(layout, layoutXml);
				break;
			}

			UI_SAFE_DELETE(layoutXml);
		}
	}	

	if( bIsWB )
	{

		char szMsg[1024];
		sprintf_s( szMsg, 1024, "[Info] WebBrowser This3 = %X\n", (INT64)layout );
		OutputDebugStringA( szMsg );
	}



	return layout;
}

// ----------------------------------------------------------------------------------
CUiLayout* CUi::FindObject(const char* name)
{
	if (name && name[0])
	{
		map<const char*, CUiLayout*, ltstr>::iterator iter = m_object.find(name);
		if (iter != m_object.end())
			return iter->second;
	}
	return NULL;
}

// ----------------------------------------------------------------------------------
void CUi::RegisterEvent(const char* event, CUiFrame* pUiFrame)
{
	if (event == NULL || pUiFrame == NULL)
		return;

	map< string, vector<CUiFrame*> >::iterator iter = m_userEvent.find(event);
	if (iter != m_userEvent.end())
	{
		vector<CUiFrame*>& vec = iter->second;
		for (vector<CUiFrame*>::iterator i = vec.begin(); i != vec.end(); i++ )
		{
			if (*i == pUiFrame)
				return;
		}

		vec.push_back(pUiFrame);
	}
	else
	{
		vector<CUiFrame*> vec;
		vec.push_back(pUiFrame);
		m_userEvent.insert(make_pair(event, vec));
	}
}

// ----------------------------------------------------------------------------------
void CUi::UnregisterEvent(const char* event, CUiFrame* pUiFrame)
{
	if (event == NULL || pUiFrame == NULL)
		return;

	map< string, vector<CUiFrame*> >::iterator iter = m_userEvent.find(event);
	if (iter != m_userEvent.end())
	{
		vector<CUiFrame*>& vec = iter->second;
		for (vector<CUiFrame*>::iterator i = vec.begin(); i != vec.end(); i++ )
		{
			if (*i == pUiFrame)
			{
				vec.erase(i);
				return;
			}
		}
	}
}

// ----------------------------------------------------------------------------------
bool CUi::SendEvent(const char* event)
{
	m_eventLockedCount++;
	m_isEvent = true;

	if (event)
	{
		map< string, vector<CUiFrame*> >::iterator iter = m_userEvent.find(event);
		if (iter != m_userEvent.end())
		{
			vector<CUiFrame*>& vec = iter->second;
			for (vector<CUiFrame*>::iterator i = vec.begin(); i != vec.end(); i++ )
				(*i)->OnEvent(event);

			m_eventLockedCount--;
			if ( m_eventLockedCount == 0 )
				m_isEvent = false;

			return true;
		}
	}

	m_eventLockedCount--;
	if ( m_eventLockedCount == 0 )
		m_isEvent = false;

	return false;
}

// ----------------------------------------------------------------------------------
void CUi::LockedFrame(CUiFrame* frame)
{
	if ( frame == NULL )
		return;

	UnlockedFrame(frame);
	m_lock.push_back(frame);
}

// ----------------------------------------------------------------------------------
void CUi::UnlockedFrame(CUiFrame* frame)
{
	if ( frame == NULL )
		return;

	for ( list<CUiFrame*>::iterator iter = m_lock.begin(); iter != m_lock.end(); iter++ ) {
		if ( *iter == frame ) {
			m_lock.erase(iter);
			break;
		}
	}
}

// ----------------------------------------------------------------------------------
int CUi::GetTotalMemSize()
{
	int size = 0;
	if ( m_pRootFrame )
		size += m_pRootFrame->GetMemSize();
	return size;
}

// ----------------------------------------------------------------------------------
string CUi::GetFontName(const char* fontFilename)
{
#ifdef KALYDO
	// this caused slow starting. Unneccesary; as the font needs to be loaded only once.
	string fontFileName(fontFilename);
	map< string, string >::iterator it = m_FontNames.find(fontFileName);
	if (it == m_FontNames.end())
	{
		string fullname = m_resourcePath + fontFilename;
		int lResult = AddFontResourceEx( fullname.c_str(), FR_PRIVATE, 0 );

		m_FontNames[fontFileName] = GetFontNameFromFile( fullname.c_str() );
	}
	return m_FontNames[fontFileName];
#else
	string fullname = m_resourcePath + fontFilename;
	int lResult = AddFontResourceEx( fullname.c_str(), FR_PRIVATE, 0 );

	return GetFontNameFromFile( fullname.c_str() );
#endif // KALYDO
}

// ----------------------------------------------------------------------------------
void CUi::InsertObject(CUiLayout* layout)
{
	const char* layout_name = layout->GetName();
	if (layout_name && layout_name[0])
	{
		if (m_luaState)
		{
			CUiLuaLayout::PushLayoutTable(m_luaState, layout);
			if (!lua_isnil(m_luaState, -1))
				lua_setglobal(m_luaState, layout_name);			// Set table name
			else
				lua_pop(m_luaState, 1);							// Pop nil value
		}
		m_object.insert(make_pair(layout_name, layout));
	}
}

// ----------------------------------------------------------------------------------
void CUi::DeleteObject(CUiLayout* layout)
{
	if (layout)
	{
		const char* name = layout->GetName();
		if (name)
			m_object.erase(name);

		if (m_luaState)
		{
			CUiLuaLayout::DeleteTable(m_luaState, layout);
		}

		
		map< string, vector<CUiFrame*> >::iterator iter;
		for (iter = m_userEvent.begin(); iter != m_userEvent.end(); iter++)
		{
			vector<CUiFrame*>& vec = iter->second;
			for (vector<CUiFrame*>::iterator i = vec.begin(); i != vec.end(); i++ )
			{
				if (*i == layout)
				{
					vec.erase(i);
					break;
				}
			}
		}

		// §R°£°O¿ýªº«ü¼Ð¦ì¸m
		if ( m_pRootFrame == layout ) m_pRootFrame = NULL;
		if ( m_pCapture == layout ) m_pCapture = NULL;
		if ( m_pKeyDownFrame == layout ) m_pKeyDownFrame = NULL;
		if ( m_pKeyboardFocus == layout ) m_pKeyboardFocus = NULL;
		if ( m_pMouseFocus == layout ) m_pMouseFocus = NULL;
		if ( m_pMouseOver == layout ) m_pMouseOver = NULL;
		if ( m_pDragFrame == layout ) m_pDragFrame = NULL;
		if ( m_pMouseExclude == layout ) m_pMouseExclude = NULL;

		for (int i = 0; i < MAX_MOUSE_BUTTON_NUM; i++)
		{
			if ( m_mouseDown[i] == layout )
				m_mouseDown[i] = NULL;
		}
		for (int i = 0; i < MAX_MOUSE_BUTTON_NUM; i++ )
		{
			if ( m_lastmouseDown[i] == layout )
				m_lastmouseDown[i] = NULL;
		}
	}
}

// ----------------------------------------------------------------------------------
void CUi::LoadXmlFile(CXmlFile* pXmlFile)
{
	LoadScriptFile(pXmlFile->m_luaFilename.c_str());

	for ( vector<CXmlNode*>::iterator iter = pXmlFile->m_nodes.begin(); iter != pXmlFile->m_nodes.end(); iter++ )
	{
		CXmlNode* pXmlNode = *iter;
		if ( pXmlNode->GetType() == eXmlUiType_Script )
		{
			LoadXmlScript(pXmlNode);
		}
		else
		{
			CXmlLayout* pXmlLayout = (CXmlLayout*)pXmlNode;
			if (!pXmlLayout->IsVirtual())
			{
				const char* parentName = (pXmlLayout->m_parent) ? pXmlLayout->m_parent->m_name.c_str() : "";
				LoadXmlFrames(FindObject(parentName), pXmlLayout);
			}
		}
	}
}

// ----------------------------------------------------------------------------------
void CUi::LoadXmlScript(CXmlNode* pXmlNode)
{
	CXmlScript* pXmlScript = (CXmlScript*)pXmlNode;

	const char* filename = pXmlScript->m_file.c_str();
	if ( strncmp(filename, m_resourcePath.c_str(), m_resourcePath.size()) == 0 ) {
		filename = filename + m_resourcePath.size();
	}

	// ¸ü¤J Lua Files
	IRuStream* stream = g_ruResourceManager->LoadStream(filename);
	if ( stream ) {
		LoadScriptFromBuffer((const char*)stream->OpenStreamMapping(), stream->GetStreamSize(), filename);
		delete stream;
	}

	// Lua Content	
	LoadScriptFromBuffer(pXmlScript->m_content.c_str(), (int)pXmlScript->m_content.size());
}

// ----------------------------------------------------------------------------------
CUiFrame* CUi::LoadXmlFrames(CUiLayout* pUiParent, CXmlLayout* pXmlLayout)
{
	CUiFrame* pUiFrame = NULL;

	if (pXmlLayout == NULL)
		return NULL;

	const char* name = pXmlLayout->GetName();
	switch (pXmlLayout->GetType())
	{
	case eXmlUiType_Frame:
		pUiFrame = (CUiFrame*)CreateObject(CUiLayout::eUiType_Frame, name, pUiParent);
		LoadXmlFrame(pUiFrame, pXmlLayout);
		break;

	case eXmlUiType_Model:
		pUiFrame = (CUiFrame*)CreateObject(CUiLayout::eUiType_Model, name, pUiParent);
		LoadXmlModel(pUiFrame, pXmlLayout);
		break;

	case eXmlUiType_Button:
		pUiFrame = (CUiFrame*)CreateObject(CUiLayout::eUiType_Button, name, pUiParent);
		LoadXmlButton(pUiFrame, pXmlLayout);
		break;

	case eXmlUiType_CheckButton:
		pUiFrame = (CUiFrame*)CreateObject(CUiLayout::eUiType_CheckButton, name, pUiParent);
		LoadXmlCheckButton(pUiFrame, pXmlLayout);
		break;

	case eXmlUiType_RadioButton:
		pUiFrame = (CUiFrame*)CreateObject(CUiLayout::eUiType_RadioButton, name, pUiParent);
		LoadXmlRadioButton(pUiFrame, pXmlLayout);
		break;

	case eXmlUiType_Slider:
		pUiFrame = (CUiFrame*)CreateObject(CUiLayout::eUiType_Slider, name, pUiParent);
		LoadXmlSlider(pUiFrame, pXmlLayout);
		break;

	case eXmlUiType_StatusBar:
		pUiFrame = (CUiFrame*)CreateObject(CUiLayout::eUiType_StatusBar, name, pUiParent);
		LoadXmlStatusBar(pUiFrame, pXmlLayout);
		break;

	case eXmlUiType_EditBox:
		pUiFrame = (CUiFrame*)CreateObject(CUiLayout::eUiType_EditBox, name, pUiParent);
		LoadXmlEditBox(pUiFrame, pXmlLayout);
		break;

	case eXmlUiType_ListBox:
		pUiFrame = (CUiFrame*)CreateObject(CUiLayout::eUiType_ListBox, name, pUiParent);
		LoadXmlListBox(pUiFrame, pXmlLayout);
		break;

	case eXmlUiType_MessageFrame:
		pUiFrame = (CUiFrame*)CreateObject(CUiLayout::eUiType_MessageFrame, name, pUiParent);
		LoadXmlMessageFrame(pUiFrame, pXmlLayout);
		break;

	case eXmlUiType_ScrollFrame:
		pUiFrame = (CUiFrame*)CreateObject(CUiLayout::eUiType_ScrollFrame, name, pUiParent);
		LoadXmlScrollFrame(pUiFrame, pXmlLayout);
		break;

	case eXmlUiType_Tooltip:
		pUiFrame = (CUiFrame*)CreateObject(CUiLayout::eUiType_Tooltip, name, pUiParent);
		LoadXmlTooltip(pUiFrame, pXmlLayout);
		break;
	}

	/*
	if (pUiFrame)
		pUiFrame->OnLoad();
	*/

	return pUiFrame;
}

// ----------------------------------------------------------------------------------
CUiLayout* CUi::LoadXmlLayers(CUiLayout* pUiParent, CXmlLayout* pXmlLayout)
{
	CUiLayout* pUiLayout = NULL;

	if (pXmlLayout == NULL)
		return NULL;

	const char* name = pXmlLayout->GetName();
	switch (pXmlLayout->GetType())
	{
	case eXmlUiType_FontString:
		pUiLayout = CreateObject(CUiLayout::eUiType_FontString, name, pUiParent);
		LoadXmlFontString((CUiFontString*)pUiLayout, (CXmlFontString*)pXmlLayout);
		break;

	case eXmlUiType_Texture:
		pUiLayout = CreateObject(CUiLayout::eUiType_Texture, name, pUiParent);
		LoadXmlTexture((CUiTexture*)pUiLayout, (CXmlTexture*)pXmlLayout);
		break;
	}

	return pUiLayout;
}

// ----------------------------------------------------------------------------------
void CUi::LoadXmlLayout(CUiLayout* pUiLayout, CXmlLayout* pXmlLayout)
{
	pUiLayout->SetVisible(pXmlLayout->m_isVisible);
	// pUiLayout->SetSize(CUiSize(pXmlLayout->m_size.m_x, pXmlLayout->m_size.m_y));
	pUiLayout->SetAnchorSize(CUiSize(pXmlLayout->m_size.m_x, pXmlLayout->m_size.m_y));	

	// Set Anchors
	vector<CUiAnchor> anchors;
	vector<CXmlAnchor>& vec = pXmlLayout->m_anchors.m_vector;
	for (vector<CXmlAnchor>::iterator iter = vec.begin(); iter != vec.end(); iter++)
	{
		CUiAnchor anchor;
		anchor.m_offset.m_x		= (*iter).m_offset.m_x;
		anchor.m_offset.m_y		= (*iter).m_offset.m_y;
		anchor.m_point			= GetXmlAnchorPoint((*iter).m_point);
		anchor.m_relativePoint	= GetXmlAnchorPoint((*iter).m_relativePoint);

		if ((*iter).m_relativeTo.empty())
			anchor.m_relativeTo	= pUiLayout->GetParent();
		else
		{
			const char* name = (*iter).m_relativeTo.c_str();			
			anchor.m_relativeTo = FindObject((*iter).m_relativeTo.c_str());

			// Ãö³s¤¸¥ó¤£¦s¦b
			if( !anchor.m_relativeTo ) {
				char msg[256];
				sprintf( msg, "%s(%d) , m_relativeTo = \"%s\"" , __FILE__ , __LINE__ , name  );
				UiOuputError(msg);
			}
			//assert(anchor.m_relativeTo);
		}
		anchors.push_back(anchor);
	}

	// ³Ì¤Ö¦³¤@²ÕÃö³s(¤÷¤¸¥ó)
	if (anchors.empty())
		anchors.push_back(CUiAnchor());

	pUiLayout->ClearAllAnchors();
	for (vector<CUiAnchor>::iterator iter = anchors.begin(); iter != anchors.end(); iter++)
		pUiLayout->SetAnchor(*iter);
}

// ----------------------------------------------------------------------------------
void CUi::LoadXmlFontString(CUiLayout* pUiLayout, CXmlLayout* pXmlLayout)
{
	LoadXmlLayout(pUiLayout, pXmlLayout);

	CUiFontString* pUiFontString = dynamic_cast<CUiFontString*>(pUiLayout);
	CXmlFontString* pXmlFontString = dynamic_cast<CXmlFontString*>(pXmlLayout);

	if (pUiFontString == NULL || pXmlFontString == NULL)
		return;

	string fontname = GetFontName(pXmlFontString->m_font.c_str());
	if ( fontname.empty() )
		return;

	// 
	CRuFontType* pFontType = CRuFontType::GetFontType(fontname.c_str(),
		(int)pXmlFontString->m_height.m_val,
		GetXmlRuFontWeight(pXmlFontString->m_weight),
		GetXmlRuFontOutline(pXmlFontString->m_outline));

	// ¦r«¬¿ù»~
	assert(pFontType);
	pUiFontString->SetFontType(pFontType);

	// ÃC¦â
	pUiFontString->SetColor(pXmlFontString->m_color.m_r, pXmlFontString->m_color.m_g, pXmlFontString->m_color.m_b);
	pUiFontString->SetAlpha(pXmlFontString->m_color.m_a);
	pUiFontString->SetBytes(pXmlFontString->m_bytes);
	pUiFontString->SetJustifyVType(GetXmlFontJustifyVType(pXmlFontString->m_justifyV));
	pUiFontString->SetJustifyHType(GetXmlFontJustifyHType(pXmlFontString->m_justifyH));
	pUiFontString->SetSpacing(pXmlFontString->m_spacing);

	// ­I´º¦â
	if (pXmlFontString->m_pShadow)
	{
		RuFontShadow shadow;
		CUiColor tmpColor;
		tmpColor.m_r	= pXmlFontString->m_pShadow->m_color.m_r;
		tmpColor.m_g	= pXmlFontString->m_pShadow->m_color.m_g;
		tmpColor.m_b	= pXmlFontString->m_pShadow->m_color.m_b;
		tmpColor.m_a	= pXmlFontString->m_pShadow->m_color.m_a;
		shadow.color	= tmpColor.GetColor();
		shadow.x		= (short)pXmlFontString->m_pShadow->m_offset.m_x;
		shadow.y		= (short)pXmlFontString->m_pShadow->m_offset.m_y;
		pUiFontString->SetFontShadow(&shadow);
	}

	const char* text = pXmlFontString->m_text.c_str();
	lua_getglobal(m_luaState, text);
	if ( lua_isstring(m_luaState, -1) )
		text = lua_tostring(m_luaState, -1);
	pUiFontString->SetTextA(text);
	lua_pop(m_luaState, 1);
}

// ----------------------------------------------------------------------------------
void CUi::LoadXmlTexture(CUiLayout* pUiLayout, CXmlLayout* pXmlLayout)
{
	LoadXmlLayout(pUiLayout, pXmlLayout);	

	CUiTexture* pUiTexture = dynamic_cast<CUiTexture*>(pUiLayout);
	CXmlTexture* pXmlTexture = dynamic_cast<CXmlTexture*>(pXmlLayout);

	if (pUiTexture == NULL || pXmlTexture == NULL)
		return;

	pUiTexture->SetFile(pXmlTexture->m_file.c_str());
	pUiTexture->SetColor(pXmlTexture->m_color.m_r, pXmlTexture->m_color.m_g, pXmlTexture->m_color.m_b);
	pUiTexture->SetAlpha(pXmlTexture->m_color.m_a);
	pUiTexture->SetAlphaMode(GetXmlTextureAlphaMode(pXmlTexture->m_alphaMode));
	pUiTexture->SetAnimationSpeed(pXmlTexture->m_animationSpeed);

	vector<CRuRectangle> texCoords;
	for(vector<CXmlTexCoords>::iterator iter = pXmlTexture->m_texCoords.begin(); iter != pXmlTexture->m_texCoords.end(); iter++)
	{
		CRuRectangle rc;
		rc.m_left	= (*iter).m_left;
		rc.m_right	= (*iter).m_right;
		rc.m_top	= (*iter).m_top;
		rc.m_bottom	= (*iter).m_bottom;	
		texCoords.push_back(rc);
	}

	if (texCoords.empty())
		pUiTexture->SetTexCoords(0, NULL);
	else
		pUiTexture->SetTexCoords((int)texCoords.size(), &texCoords[0]);
}

// ----------------------------------------------------------------------------------
void CUi::LoadXmlFrame(CUiLayout* pUiLayout, CXmlLayout* pXmlLayout)
{
	LoadXmlLayout(pUiLayout, pXmlLayout);

	CUiFrame* pUiFrame = dynamic_cast<CUiFrame*>(pUiLayout);
	CXmlFrame* pXmlFrame = dynamic_cast<CXmlFrame*>(pXmlLayout);

	if (pUiFrame == NULL || pXmlFrame == NULL)
		return;

	switch (pXmlFrame->m_frameStrata)
	{
	case CXmlFrame::eStrata_Background:
		pUiFrame->SetFrameStrata(eUIFrame_Strata_Background);
		break;

	case CXmlFrame::eStrata_Low:
		pUiFrame->SetFrameStrata(eUIFrame_Strata_Low);
		break;

	case CXmlFrame::eStrata_Medium:
		pUiFrame->SetFrameStrata(eUIFrame_Strata_Medium);
		break;

	case CXmlFrame::eStrata_High:
		pUiFrame->SetFrameStrata(eUIFrame_Strata_High);
		break;

	case CXmlFrame::eStrata_Dialog:
		pUiFrame->SetFrameStrata(eUIFrame_Strata_Dialog);
		break;

	case CXmlFrame::eStrata_Tooltip:
		pUiFrame->SetFrameStrata(eUIFrame_Strata_Tooltip);
		break;
	}

	pUiFrame->SetID(pXmlFrame->m_id);	
	pUiFrame->SetFrameLevel(pXmlFrame->m_frameLevel);
	pUiFrame->SetTopLevel(pXmlFrame->m_topLevel);
	pUiFrame->SetAlpha(pXmlFrame->m_alpha);
	pUiFrame->SetKeyboardEnable(pXmlFrame->m_isEnableKeyboard);
	pUiFrame->SetMouseEnable(pXmlFrame->m_isEnableMouse);
	pUiFrame->SetMovable(pXmlFrame->m_isMovable);

	CUiRectangle inset;
	inset.m_left	= pXmlFrame->m_hitRectInsets.m_left;
	inset.m_right	= pXmlFrame->m_hitRectInsets.m_right;
	inset.m_top		= pXmlFrame->m_hitRectInsets.m_top;
	inset.m_bottom	= pXmlFrame->m_hitRectInsets.m_bottom;
	pUiFrame->SetHitRectInsets(inset);

	if (pXmlFrame->m_resizeBounds.m_maxBounds)
	{
		CUiSize size;
		size.m_x = pXmlFrame->m_resizeBounds.m_maxBounds->m_x;
		size.m_y = pXmlFrame->m_resizeBounds.m_maxBounds->m_y;
		pUiFrame->SetMaxResizeBounds(size);
	}
	if (pXmlFrame->m_resizeBounds.m_minBounds)
	{
		CUiSize size;
		size.m_x = pXmlFrame->m_resizeBounds.m_minBounds->m_x;
		size.m_y = pXmlFrame->m_resizeBounds.m_minBounds->m_y;
		pUiFrame->SetMinResizeBounds(size);
	}

	if (pXmlFrame->m_pBackdrop)
	{
		CTextureBackdrop backdrop;
		CRuRectangle rc;
		rc.m_left	= pXmlFrame->m_pBackdrop->m_bgInsets.m_left;
		rc.m_right	= pXmlFrame->m_pBackdrop->m_bgInsets.m_right;
		rc.m_top	= pXmlFrame->m_pBackdrop->m_bgInsets.m_top;
		rc.m_bottom	= pXmlFrame->m_pBackdrop->m_bgInsets.m_bottom;

		backdrop.SetEdgeFileName(pXmlFrame->m_pBackdrop->m_edgeFile.c_str());
		backdrop.SetTileFileName(pXmlFrame->m_pBackdrop->m_tileFile.c_str());
		backdrop.SetEdgeSize(pXmlFrame->m_pBackdrop->m_edgeSize.m_val);
		backdrop.SetTileSize(pXmlFrame->m_pBackdrop->m_tileSize.m_val);
		backdrop.SetBackgroundOffset(rc);

		pUiFrame->SetBackdrop(&backdrop);
	}

	// Layers
	for (multimap<int, CXmlLayout*>::iterator iter = pXmlFrame->m_layers.begin(); iter != pXmlFrame->m_layers.end(); iter++)
		pUiFrame->SetLayers(GetXmlLayer(iter->first), LoadXmlLayers(pUiFrame, iter->second));

	// Frames
	for (vector<CXmlFrame*>::iterator iter = pXmlFrame->m_frames.begin(); iter != pXmlFrame->m_frames.end(); iter++)
		LoadXmlFrames(pUiFrame, *iter);	

	// Scripts	
	for (map<string, string>::iterator iter = pXmlFrame->m_scripts.begin(); iter != pXmlFrame->m_scripts.end(); iter++) {
		pUiFrame->SetScripts(pUiFrame->GetScriptEvent(iter->first.c_str()), iter->second.c_str());
	}
}

// ----------------------------------------------------------------------------------
void CUi::LoadXmlModel(CUiLayout* pUiLayout, CXmlLayout* pXmlLayout)
{
	LoadXmlFrame(pUiLayout, pXmlLayout);

	CUiModel* pUiModel = dynamic_cast<CUiModel*>(pUiLayout);
	CXmlModel* pXmlModel = dynamic_cast<CXmlModel*>(pXmlLayout);
	if (pUiModel == NULL || pXmlModel == NULL)
		return;

	if (!pXmlModel->m_file.empty())
		pUiModel->CreateEntity(pXmlModel->m_file.c_str());
	if (!pXmlModel->m_maskFile.empty())
		pUiModel->SetMaskFile(pXmlModel->m_maskFile.c_str());
	if (!pXmlModel->m_targetBone.empty())
		pUiModel->SetTargetBone(pXmlModel->m_targetBone.c_str());
}

// ----------------------------------------------------------------------------------
void CUi::LoadXmlButton(CUiLayout* pUiLayout, CXmlLayout* pXmlLayout)
{
	LoadXmlFrame(pUiLayout, pXmlLayout);

	CUiButton* pUiButton = dynamic_cast<CUiButton*>(pUiLayout);
	CXmlButton* pXmlButton = dynamic_cast<CXmlButton*>(pXmlLayout);
	if (pUiButton == NULL || pXmlButton == NULL)
		return;

	if (pXmlButton->m_normalTexture)
		pUiButton->SetNormalTexture((CUiTexture*)LoadXmlLayers(pUiButton, pXmlButton->m_normalTexture));
	if (pXmlButton->m_pushedTexture)
		pUiButton->SetPushedTexture((CUiTexture*)LoadXmlLayers(pUiButton, pXmlButton->m_pushedTexture));
	if (pXmlButton->m_disabledTexture)
		pUiButton->SetDisabledTexture((CUiTexture*)LoadXmlLayers(pUiButton, pXmlButton->m_disabledTexture));
	if (pXmlButton->m_highlightTexture)
		pUiButton->SetHighlightTexture((CUiTexture*)LoadXmlLayers(pUiButton, pXmlButton->m_highlightTexture));
	if (pXmlButton->m_normalText)
		pUiButton->SetNormalText((CUiFontString*)LoadXmlLayers(pUiButton, pXmlButton->m_normalText));
	if (pXmlButton->m_disabledText)
		pUiButton->SetDisabledText((CUiFontString*)LoadXmlLayers(pUiButton, pXmlButton->m_disabledText));
	if (pXmlButton->m_highlightText)
		pUiButton->SetHighlightText((CUiFontString*)LoadXmlLayers(pUiButton, pXmlButton->m_highlightText));

	CUiPoint offset;
	offset.m_x	= pXmlButton->m_pushedTextOffset.m_x;
	offset.m_y	= pXmlButton->m_pushedTextOffset.m_y;
	pUiButton->SetPushTextOffset(offset);

	const char* text = pXmlButton->m_text.c_str();
	lua_getglobal(m_luaState, text);
	if ( lua_isstring(m_luaState, -1) )
		text = lua_tostring(m_luaState, -1);
	pUiButton->SetTextA(text);
	lua_pop(m_luaState, 1);
}

// ----------------------------------------------------------------------------------
void CUi::LoadXmlCheckButton(CUiLayout* pUiLayout, CXmlLayout* pXmlLayout)
{
	LoadXmlButton(pUiLayout, pXmlLayout);

	CUiCheckButton* pUiCheckButton = dynamic_cast<CUiCheckButton*>(pUiLayout);
	CXmlCheckButton* pXmlCheckButton = dynamic_cast<CXmlCheckButton*>(pXmlLayout);
	if (pUiCheckButton == NULL || pXmlCheckButton == NULL)
		return;

	if (pXmlCheckButton->m_checkedTexture)
		pUiCheckButton->SetCheckedTexture((CUiTexture*)LoadXmlLayers(pUiCheckButton, pXmlCheckButton->m_checkedTexture));
	if (pXmlCheckButton->m_disabledCheckedTexture)
		pUiCheckButton->SetDisabledCheckedTexture((CUiTexture*)LoadXmlLayers(pUiCheckButton, pXmlCheckButton->m_disabledCheckedTexture));

	pUiCheckButton->SetChecked(pXmlCheckButton->m_checked);
	pUiCheckButton->SetAutoChecked(pXmlCheckButton->m_autoChecked);
}

// ----------------------------------------------------------------------------------
void CUi::LoadXmlRadioButton(CUiLayout* pUiLayout, CXmlLayout* pXmlLayout)
{
	LoadXmlCheckButton(pUiLayout, pXmlLayout);

	CUiRadioButton* pUiRadioButton = dynamic_cast<CUiRadioButton*>(pUiLayout);
	CXmlRadioButton* pXmlRadioButton = dynamic_cast<CXmlRadioButton*>(pXmlLayout);
	if (pUiRadioButton == NULL || pXmlRadioButton ==NULL)
		return;

	pUiRadioButton->SetGroupId(pXmlRadioButton->m_groupId);
}

// ----------------------------------------------------------------------------------
void CUi::LoadXmlSlider(CUiLayout* pUiLayout, CXmlLayout* pXmlLayout)
{
	LoadXmlFrame(pUiLayout, pXmlLayout);

	CUiSlider* pUiSlider = dynamic_cast<CUiSlider*>(pUiLayout);
	CXmlSlider* pXmlSlider = dynamic_cast<CXmlSlider*>(pXmlLayout);
	if (pUiSlider == NULL || pXmlSlider == NULL)
		return;

	switch (pXmlSlider->m_orientation)
	{
	case CXmlSlider::eOrientation_Vertical:
		pUiSlider->SetOrientation(CUiSlider::eOrientation_Vertical);
		break;
	case CXmlSlider::eOrientation_Horizontal:
		pUiSlider->SetOrientation(CUiSlider::eOrientation_Horizontal);
		break;
	}

	pUiSlider->SetMaxValue(pXmlSlider->m_maxValue);
	pUiSlider->SetMinValue(pXmlSlider->m_minValue);
	pUiSlider->SetValue(pXmlSlider->m_defaultValue);	

	if (pXmlSlider->m_thumbTexture)
		pUiSlider->SetThumbTexture((CUiTexture*)LoadXmlLayers(pUiSlider, pXmlSlider->m_thumbTexture));
}

// ----------------------------------------------------------------------------------
void CUi::LoadXmlStatusBar(CUiLayout* pUiLayout, CXmlLayout* pXmlLayout)
{
	LoadXmlFrame(pUiLayout, pXmlLayout);

	CUiStatusBar* pUiStatusBar = dynamic_cast<CUiStatusBar*>(pUiLayout);
	CXmlStatusBar* pXmlStatusBar = dynamic_cast<CXmlStatusBar*>(pXmlLayout);
	if (pUiStatusBar == NULL || pXmlStatusBar == NULL)
		return;

	pUiStatusBar->SetMaxValue(pXmlStatusBar->m_maxValue);
	pUiStatusBar->SetMinValue(pXmlStatusBar->m_minValue);
	pUiStatusBar->SetValue(pXmlStatusBar->m_defaultValue);
	pUiStatusBar->SetReverse(pXmlStatusBar->m_reverse);

	CUiColor color;
	color.m_r = pXmlStatusBar->m_barColor.m_r;
	color.m_g = pXmlStatusBar->m_barColor.m_g;
	color.m_b = pXmlStatusBar->m_barColor.m_b;
	color.m_a = pXmlStatusBar->m_barColor.m_a;
	pUiStatusBar->SetBarColor(color);

	if (pXmlStatusBar->m_barTexture)
		pUiStatusBar->SetBarTexture((CUiTexture*)LoadXmlLayers(pUiStatusBar, pXmlStatusBar->m_barTexture));
}

// ----------------------------------------------------------------------------------
void CUi::LoadXmlEditBox(CUiLayout* pUiLayout, CXmlLayout* pXmlLayout)
{
	LoadXmlFrame(pUiLayout, pXmlLayout);

	CUiEditBox* pUiEditBox = dynamic_cast<CUiEditBox*>(pUiLayout);
	CXmlEditBox* pXmlEditBox = dynamic_cast<CXmlEditBox*>(pXmlLayout);
	if (pUiEditBox == NULL || pXmlEditBox == NULL)
		return;

	pUiEditBox->SetHistoryLines(pXmlEditBox->m_historyLines);
	pUiEditBox->SetLetters(pXmlEditBox->m_letters);
	pUiEditBox->SetNumeric(pXmlEditBox->m_isNumeric);
	pUiEditBox->SetPassword(pXmlEditBox->m_isPassword);
	pUiEditBox->SetMultiMode(pXmlEditBox->m_isMultiLine);

	CUiColor color;
	color.m_r = pXmlEditBox->m_highlightColor.m_r;
	color.m_g = pXmlEditBox->m_highlightColor.m_g;
	color.m_b = pXmlEditBox->m_highlightColor.m_b;
	color.m_a = pXmlEditBox->m_highlightColor.m_a;
	pUiEditBox->SetHighlightColor(color.GetColor());

	CUiRectangle inset;
	inset.m_left	= pXmlEditBox->m_textInsets.m_left;
	inset.m_right	= pXmlEditBox->m_textInsets.m_right;
	inset.m_top		= pXmlEditBox->m_textInsets.m_top;
	inset.m_bottom	= pXmlEditBox->m_textInsets.m_bottom;
	pUiEditBox->SetTextInset(inset);

	if (pXmlEditBox->m_fontString)
		pUiEditBox->SetFontString((CUiFontString*)LoadXmlLayers(pUiEditBox, pXmlEditBox->m_fontString));
}

// ----------------------------------------------------------------------------------
void CUi::LoadXmlListBox(CUiLayout* pUiLayout, CXmlLayout* pXmlLayout)
{
	LoadXmlFrame(pUiLayout, pXmlLayout);

	CUiListBox* pUiListBox = dynamic_cast<CUiListBox*>(pUiLayout);
	CXmlListBox* pXmlListBox = dynamic_cast<CXmlListBox*>(pXmlLayout);
	if (pUiListBox == NULL || pXmlListBox == NULL)
		return;

	CUiRectangle inset;
	inset.m_left	= pXmlListBox->m_textInsets.m_left;
	inset.m_right	= pXmlListBox->m_textInsets.m_right;
	inset.m_top		= pXmlListBox->m_textInsets.m_top;
	inset.m_bottom	= pXmlListBox->m_textInsets.m_bottom;
	pUiListBox->SetTextInset(inset);

	if (pXmlListBox->m_fontString)
		pUiListBox->SetFontString((CUiFontString*)LoadXmlLayers(pUiListBox, pXmlListBox->m_fontString));
}

// ----------------------------------------------------------------------------------
void CUi::LoadXmlMessageFrame(CUiLayout* pUiLayout, CXmlLayout* pXmlLayout)
{
	LoadXmlFrame(pUiLayout, pXmlLayout);

	CUiMessageFrame* pUiMessageFrame = dynamic_cast<CUiMessageFrame*>(pUiLayout);
	CXmlMessageFrame* pXmlMessageFrame = dynamic_cast<CXmlMessageFrame*>(pXmlLayout);
	if (pUiMessageFrame == NULL || pXmlMessageFrame == NULL)
		return;

	if (pXmlMessageFrame->m_fontString)
		pUiMessageFrame->SetFontString((CUiFontString*)LoadXmlLayers(pUiMessageFrame, pXmlMessageFrame->m_fontString));

	CUiRectangle inset;
	inset.m_left	= pXmlMessageFrame->m_textInsets.m_left;
	inset.m_right	= pXmlMessageFrame->m_textInsets.m_right;
	inset.m_top		= pXmlMessageFrame->m_textInsets.m_top;
	inset.m_bottom	= pXmlMessageFrame->m_textInsets.m_bottom;
	pUiMessageFrame->SetTextInset(inset);

	pUiMessageFrame->SetMaxLines(pXmlMessageFrame->m_maxLines);
	pUiMessageFrame->SetFadeDuration(pXmlMessageFrame->m_fadeDuration);
	pUiMessageFrame->SetDisplayDuration(pXmlMessageFrame->m_displayDuration);
}

// ----------------------------------------------------------------------------------
void CUi::LoadXmlScrollFrame(CUiLayout* pUiLayout, CXmlLayout* pXmlLayout)
{
	LoadXmlFrame(pUiLayout, pXmlLayout);

	CUiScrollFrame* pUiScrollFrame = dynamic_cast<CUiScrollFrame*>(pUiLayout);
	CXmlScrollFrame* pXmlScrollFrame = dynamic_cast<CXmlScrollFrame*>(pXmlLayout);
	if (pUiScrollFrame == NULL || pXmlScrollFrame == NULL)
		return;

	if ( pXmlScrollFrame->m_pScrollChild ) 
		pUiScrollFrame->SetScrollChild(LoadXmlFrames(pUiScrollFrame, pXmlScrollFrame->m_pScrollChild));
}


// ----------------------------------------------------------------------------------
void CUi::LoadXmlTooltip(CUiLayout* pUiLayout, CXmlLayout* pXmlLayout)
{
	LoadXmlFrame(pUiLayout, pXmlLayout);
}

// ----------------------------------------------------------------------------------
void CUi::DisableIME()
{
	if (m_hWnd == NULL)
		return;

	g_hImc = ImmGetContext(m_hWnd);
	if ( g_hImc ) {
		ImmAssociateContext(m_hWnd, NULL);
	}
	ImmReleaseContext(m_hWnd, g_hImc);
}

// ----------------------------------------------------------------------------------
void CUi::EnableIME()
{
	if (m_hWnd == NULL)
		return;

	if (g_hImc) {
		ImmAssociateContext(m_hWnd, g_hImc);   
		g_hImc = NULL;
	}
}

// ----------------------------------------------------------------------------------
int	GetXmlType(int type)
{
	switch (type)
	{
	case eXmlUiType_Layout:
		return CUiLayout::eUiType_Layout;

	case eXmlUiType_FontString:
		return CUiLayout::eUiType_FontString;

	case eXmlUiType_Texture:
		return CUiLayout::eUiType_Texture;

	case eXmlUiType_Frame:
		return CUiLayout::eUiType_Frame;

	case eXmlUiType_Model:
		return CUiLayout::eUiType_Model;

	case eXmlUiType_Button:
		return CUiLayout::eUiType_Button;

	case eXmlUiType_CheckButton:
		return CUiLayout::eUiType_CheckButton;

	case eXmlUiType_RadioButton:
		return CUiLayout::eUiType_RadioButton;

	case eXmlUiType_StatusBar:
		return CUiLayout::eUiType_StatusBar;

	case eXmlUiType_Slider:
		return CUiLayout::eUiType_Slider;

	case eXmlUiType_EditBox:
		return CUiLayout::eUiType_EditBox;

	case eXmlUiType_ListBox:
		return CUiLayout::eUiType_ListBox;

	case eXmlUiType_MessageFrame:
		return CUiLayout::eUiType_MessageFrame;

	case eXmlUiType_ScrollFrame:
		return CUiLayout::eUiType_ScrollFrame;
	}

	return -1;
}

// ----------------------------------------------------------------------------------
int	GetXmlAnchorPoint(int point)
{
	switch (point)
	{
	case CXmlAnchor::eAnchor_TopLeft:
		return CUiAnchor::eAnchor_TopLeft;

	case CXmlAnchor::eAnchor_Top:
		return CUiAnchor::eAnchor_Top;

	case CXmlAnchor::eAnchor_TopRight:
		return CUiAnchor::eAnchor_TopRight;

	case CXmlAnchor::eAnchor_Left:
		return CUiAnchor::eAnchor_Left;

	case CXmlAnchor::eAnchor_Center:
		return CUiAnchor::eAnchor_Center;

	case CXmlAnchor::eAnchor_Right:
		return CUiAnchor::eAnchor_Right;

	case CXmlAnchor::eAnchor_BottomLeft:
		return CUiAnchor::eAnchor_BottomLeft;

	case CXmlAnchor::eAnchor_Bottom:
		return CUiAnchor::eAnchor_Bottom;

	case CXmlAnchor::eAnchor_BottomRight:
		return CUiAnchor::eAnchor_BottomRight;
	}

	return CUiAnchor::eAnchor_Null;
}

// ----------------------------------------------------------------------------------
int	GetXmlLayer(int layer)
{
	switch(layer)
	{
	case CXmlFrame::eLayer_Background:
		return CUiFrame::eLayerType_Background;
	case CXmlFrame::eLayer_Border:
		return CUiFrame::eLayerType_Border;
	case CXmlFrame::eLayer_Artwork:
		return CUiFrame::eLayerType_Artwork;
	case CXmlFrame::eLayer_Overlay:
		return CUiFrame::eLayerType_Overlay;
	case CXmlFrame::eLayer_Highlight:
		return CUiFrame::eLayerType_Highlight;
	}
	return -1;
}

// ----------------------------------------------------------------------------------
RuFontWeight GetXmlRuFontWeight(int weight)
{
	switch(weight)
	{
	case CXmlFontString::eFontWeight_Thin:
		return ruFONTWEIGHT_THIN;
	case CXmlFontString::eFontWeight_Normal:
		return ruFONTWEIGHT_NORMAL;
	case CXmlFontString::eFontWeight_Bold:
		return ruFONTWEIGHT_BOLD;
	}
	return ruFONTWEIGHT_NORMAL;
}

// ----------------------------------------------------------------------------------
RuFontOutline GetXmlRuFontOutline(int weight)
{
	switch(weight)
	{
	case CXmlFontString::eOutline_None:
		return ruFONTOUTLINE_NONE;
	case CXmlFontString::eOutline_Normal:
		return ruFONTOUTLINE_NORMAL;
	case CXmlFontString::eOutline_Thick:
		return ruFONTOUTLINE_THICK;
	}
	return ruFONTOUTLINE_NORMAL;
}

// ----------------------------------------------------------------------------------
int	GetXmlFontJustifyVType(int type)
{
	switch(type)
	{
	case CXmlFontString::eJustifyVType_Top:
		return CUiFontString::eJustifyV_Top;
	case CXmlFontString::eJustifyVType_Middle:
		return CUiFontString::eJustifyV_Middle;
	case CXmlFontString::eJustifyVType_Bottom:
		return CUiFontString::eJustifyV_Bottom;
	}
	return CUiFontString::eJustifyV_Top;
}

// ----------------------------------------------------------------------------------
int	GetXmlFontJustifyHType(int type)
{
	switch(type)
	{
	case CXmlFontString::eJustifyHType_Left:
		return CUiFontString::eJustifyH_Left;
	case CXmlFontString::eJustifyHType_Center:
		return CUiFontString::eJustifyH_Center;
	case CXmlFontString::eJustifyHType_Right:
		return CUiFontString::eJustifyH_Right;
	}
	return CUiFontString::eJustifyH_Left;
}

// ----------------------------------------------------------------------------------
TextureAlphaMode GetXmlTextureAlphaMode(int alphaMode)
{
	switch(alphaMode)
	{
	case CXmlTexture::eAlphaMode_Disable:
		return ALPHA_MODE_DISABLE;
	case CXmlTexture::eAlphaMode_Blend:
		return ALPHA_MODE_BLEND;
	case CXmlTexture::eAlphaMode_Add:
		return ALPHA_MODE_ADD;
	}

	return ALPHA_MODE_BLEND;
}

/////////////////////////////////////////////////////////////////////////////
// ANSI string buffer function
#ifdef UNICODE
#else

// ¼W¥[¤@­Ó¦r¤¸
// Return: ¬O§_´£¥æ¤F¦r¤¸¡C
BOOL CUi::asbAddByte(BYTE by)
{
	// ²K¥[¸Ó¦r¤¸
	m_asbText[m_asbTextLen++] = by;
	if (m_asbTextLen == MAXANSIBUF){ // ¦pªG½w½Ä°Ï¤wº¡,´£¥X¦r¦ê
		return asbSubmit();
	}else{

		m_asbText[m_asbTextLen] = '\0';
	}

	wchar_t wsBuf[MAXANSIBUF];
	int cchBuf;
	char* p0;
	char* p1;
	char* pMax;

	// ¤ÀªR½w½Ä°Ï¤º¦³¦h¤Ö¦r¤¸
	p0 = p1 = m_asbText;
	pMax = m_asbText + m_asbTextLen;
	while(1){
		p1 = CharNextExA(m_CurCP, p0, 0);
		if((*p1 == '\0')||(p1 >= pMax)||(p1 == p0)||(p1==NULL)) break;
	}

	// ´£¥æ«e­±ªº¦r¤¸
	if(p0 != m_asbText){
		// Âà¬°Unicode
		cchBuf = MultiByteToWideChar(m_CurCP, 0, m_asbText, int(p0 - m_asbText), wsBuf, MAXANSIBUF);

		// ´£¥æ¦r¦r¦ê
		OnChar(wsBuf, cchBuf);
	}

	// ¹Á¸ÕÂà´«³Ì«á¤@­Ó¦r¤¸
	cchBuf = MultiByteToWideChar(m_CurCP, MB_ERR_INVALID_CHARS, p0, int(p1 - p0), wsBuf, MAXANSIBUF);
	if(cchBuf > 0){ // Âà´«¦¨¥\
		// ¿é¥X¦r¤¸
		OnChar(wsBuf, cchBuf);

		// ²MªÅ½w¨R°Ï
		asbClear();
	}else{ // Âà´«¥¢±Ñ
		// ¥Ñ©ó«e­±ªº¦r¦ê¤w´£¥æ¡A©Ò¥H±N³Ì«á¨º¨Ç¦r¤¸²¾°Ê¨ì«e­±¨Ó
		m_asbTextLen = int(p1 - p0);
		MoveMemory(m_asbText, p0, m_asbTextLen);
		m_asbText[m_asbTextLen] = '\0';
	}

	return (cchBuf > 0)||(p0 != m_asbText);
}

// ¿é¥X¼ÆÕu
//Return: ¦³¼ÆÕu´N´£¥æ¡Aªð¦^«D0¡F§_«hªð¦^0
BOOL CUi::asbSubmit(void)
{
	wchar_t wsBuf[MAXANSIBUF];
	int cchBuf;

	if (0==m_asbTextLen) return FALSE;

	// Âà´«¬°Unicode 16
	cchBuf = MultiByteToWideChar(m_CurCP, 0, m_asbText, m_asbTextLen, wsBuf, MAXANSIBUF);

	// ´£¥æ¦r¦ê
	OnChar(wsBuf, cchBuf);

	// ²MªÅ½w¨R°Ï
	asbClear();

	return TRUE;
}

// ²MªÅ½w¨R°Ï
//Return: ¦³¦r¤¸´N²MªÅ¡Aªð¦^«D0¡F§_«hªð¦^0
BOOL CUi::asbClear(void)
{
	if (0==m_asbTextLen) return FALSE;
	m_asbTextLen = 0;
	ZeroMemory(m_asbText, sizeof(m_asbText));
	return TRUE;
}

#endif