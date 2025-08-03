#include <windows.h>
#include "uieditbox.h"
#include "ui.h"

// ----------------------------------------------------------------------------------
CUiEditBox::CUiEditBox(const char* name, CUiLayout* pParent, CUi* pUi)
	: CUiFrame(name, pParent, pUi)
{
	m_uiType			= eUiType_EditBox;

	m_state.isMouseEnable	= true;

	m_history.clear();
	m_historyLast		= 0;
	m_historyLines		= 0;
	m_fontString		= NULL;
	m_text				= NULL;
	m_blockColor		= 0x7F7F7F7F;
	m_caretColor		= 0xFFFFFFFF;
	m_textSize			= 0;
	m_letters			= 0;
	m_multiMode			= false;
	m_isNumeric			= false;
	m_isPassword		= false;
	m_isInsertMode		= false;
	m_isPaser			= true;
	m_isKeyboardInput	= true;

	m_blinkSpeed		= 0.5f;
	m_lastBlinkTime		= 0.0f;
	m_firstVisible		= 0;
	m_selStart			= 0;
	m_caretPos			= 0;
	m_caretBool			= false;	

	m_firstLine			= 0;
	m_displayMaxLine	= 0;
	m_ln				= 0;
	m_ch				= 0;
	m_isRTL				= false;

	SetLetters(255);
}

// ----------------------------------------------------------------------------------
CUiEditBox::~CUiEditBox()
{
	UI_SAFE_DELETE(m_fontString);
	UI_SAFE_DELETE_ARRAY(m_text);

	for ( list<WCHAR*>::iterator iter = m_history.begin(); iter != m_history.end(); iter++ )
		UI_SAFE_DELETE_ARRAY( *iter );
	m_history.clear();
}

// ----------------------------------------------------------------------------------
void CUiEditBox::SetFontString(CUiFontString* fontString)
{
	UI_SAFE_DELETE(m_fontString);
	m_fontString = fontString;
	if ( m_fontString )
		m_fontString->SetBytes(m_letters);
}

// ----------------------------------------------------------------------------------
void CUiEditBox::SetLetters(int letters)
{	
	if ( letters > 0 && m_letters != letters )
	{
		WCHAR *pTmp;
		m_letters = letters;

		pTmp = new WCHAR[m_letters + 1];
		memset( pTmp, 0, sizeof(WCHAR) * (m_letters + 1) );

		if ( m_text )
		{
			memcpy( pTmp, m_text, sizeof(WCHAR) * min(m_textSize, m_letters) );
			delete [] m_text;
		}

		m_text = pTmp;
		if ( m_fontString )
			m_fontString->SetBytes(letters);
	}
}

// ----------------------------------------------------------------------------------
void CUiEditBox::SetText(const WCHAR* text)
{
	if (text)
	{
		int length = 0;

		if ( m_multiMode )
			length = (int)wcslen(text);
		else
		{
			while ( text[length] >= 0x20 )
				length++;
		}
		length = min(m_letters, length);
		
		m_caretPos = 0;
		m_selStart = 0;
		m_firstVisible = 0;

		if (m_textSize == length && wcsncmp(m_text, text, max(length, 1)) == 0)
			return;

		memcpy(m_text, text, sizeof(WCHAR) * length);
		m_text[length] = 0;
		m_textSize = length;

		OnTextChanged();
	}
}

// ----------------------------------------------------------------------------------
void CUiEditBox::SetTextA(const char* text)
{
	
	WCHAR* tmp = MultiByteToWChar(text);
	SetText(tmp);
	UI_SAFE_DELETE_ARRAY(tmp);
}

// ----------------------------------------------------------------------------------
void CUiEditBox::SetHistoryLines(int value)
{
	if ( value >= 0 && value != m_historyLines )
	{
		m_historyLines = value;
		AdjustHistory();
	}
}

// ----------------------------------------------------------------------------------
void CUiEditBox::AddHistoryLine(const WCHAR* text)
{
	if ( m_historyLines <= 0 )
		return;

	WCHAR* tmp = new WCHAR[wcslen(text) + 1];
	wcscpy( tmp, text );
	m_history.push_back( tmp );

	AdjustHistory();
}

// ----------------------------------------------------------------------------------
void CUiEditBox::AddHistoryLine(const char* text)
{
	WCHAR* tmp = MultiByteToWChar(text);
	AddHistoryLine(tmp);
	UI_SAFE_DELETE_ARRAY(tmp);
}

// ----------------------------------------------------------------------------------
const WCHAR* CUiEditBox::GetHistoryLine(int line)
{
	if ( m_history.empty() )
		return NULL;

	if ( line >= 0 && line < (int)m_history.size() )
	{
		list<WCHAR*>::iterator iter = m_history.begin();
		for ( int i = 0; i < line; i++ )
			iter++;
		m_historyLast = line;
		return *iter;
	}
	return NULL;
}

// ----------------------------------------------------------------------------------
bool CUiEditBox::AddText(const WCHAR* text)
{
	if ( text == NULL )
		return false;

	int len = (int)wcslen(text);
	int caretEnd = m_textSize - m_caretPos;
	if ( m_textSize + len > m_letters )
		return false;

	if ( InsertString(m_caretPos, text, len) > 0 )
	{
		OnTextChanged();
		if ( caretEnd == 0 )
		{
			PlaceCaret( m_textSize );
		}		
		m_selStart = m_caretPos;
		return true;
	}
	return false;	
}

// ----------------------------------------------------------------------------------
bool CUiEditBox::AddTextA(const char* text)
{
	WCHAR* tmp = MultiByteToWChar(text);
	bool resultBool = AddText(tmp);
	UI_SAFE_DELETE_ARRAY(tmp);
	return resultBool;
}

// ----------------------------------------------------------------------------------
CUiFrame* CUiEditBox::GetFrameAtPoint(CUiPoint pt, CUiFrame* pExclude)
{
	if ( !m_isVisible || !m_isEnable || !static_cast<CUiFrame*>(m_pParent)->IsEnable() )
		return NULL;

	return CUiFrame::GetFrameAtPoint(pt, pExclude);
}

// ----------------------------------------------------------------------------------
void CUiEditBox::AdjustHistory()
{
	if ( m_historyLines < (int)m_history.size() )
	{
		for ( int i = 0; i < (int)m_history.size() - m_historyLines; i++ )
		{
			list<WCHAR*>::iterator iter = m_history.begin();
			if ( *iter ) delete [] (*iter);
			m_history.erase( iter );
		}
	}
	m_historyLast = (int)m_history.size();
}

// ----------------------------------------------------------------------------------
void CUiEditBox::UpdateLine()
{
	m_multiLine.clear();
	if ( m_fontString == NULL || m_fontString->GetFontType() == NULL )
		return;

	float scale = GetRealScale();

	CUiRectangle rect;
	GetRectangle(&rect);

	rect.m_left		+= m_textInset.m_left * scale;
	rect.m_top		+= m_textInset.m_top * scale;
	rect.m_right	-= m_textInset.m_right * scale;
	rect.m_bottom	-= m_textInset.m_bottom * scale;

	float rectWidth		= rect.RectWidth();	
	float rectHeight	= rect.RectHeight();
	float fontHeight	= m_fontString->GetFontType()->GetHeight() * scale;
	if ( m_multiMode )
	{
		MultiLineInfo line;	

		line.start = 0;
		line.count = 0;
		line.width = 0.0f;

		int i = 0;
		while ( i < m_textSize )
		{
			int ln = 1;

			// 換行
			if ( m_text[i] == L'\n' )
			{
				line.count++;
				m_multiLine.push_back(line);
				line.start = i + 1;
				line.count = 0;
				line.width = 0.0f;
			}
			else
			{
				CRuFontString* fontString = NULL;
				float width = 0.0f;

				if ( m_text[i] >= L' ' )
				{
					// 下一個字元位置
					const wchar_t* charNext = CRuFontString::CharNext(&m_text[i], m_textSize - i);

					ln = int(charNext - &m_text[i]);

					fontString = CRuFontString::GetFontString(m_fontString->GetFontType(), m_text + i, ln);

					width = fontString->GetXSize() * scale;
				}

				if ( line.count > 0 && line.width + width > rectWidth )
				{
					m_multiLine.push_back(line);
					line.start = i;
					line.count = ln;
					line.width = width;
				}
				else
				{
					line.width += width;
					line.count += ln;
				}
			}

			i += ln;
		}

		/*
		for ( int i = 0; i < m_textSize; i++ )
		{
			// 換行
			if ( m_text[i] == L'\n' )
			{
				line.count++;
				m_multiLine.push_back(line);
				line.start = i + 1;
				line.count = 0;
				line.width = 0.0f;
			}
			else
			{
				CRuFontString* fontString = NULL;
				float width = 0.0f;

				if ( m_text[i] >= L' ' )
				{
					fontString = CRuFontString::GetFontString(m_fontString->GetFontType(), m_text + i, 1, TRUE);
					width = fontString->GetXSize() * scale;
				}
				if ( line.count > 0 && line.width + width > rectWidth )
				{
					m_multiLine.push_back(line);
					line.start = i;
					line.count = 1;
					line.width = width;
				}
				else
				{
					line.width += width;
					line.count++;
				}
			}
		}
		*/

		if ( line.count > 0 )
			m_multiLine.push_back(line);
	}

	m_displayMaxLine = min((int)m_multiLine.size(), (int)(rectHeight/fontHeight));
}

// ----------------------------------------------------------------------------------
// 單行(字串資料)游標轉換為雙行(拆解後的行數資料)游標值
void CUiEditBox::CaretToPoint(int caret, int &ln, int &ch)
{	
	ln = 0;
	ch = 0;
	if ( caret == 0 )
		return;

	for ( vector<MultiLineInfo>::iterator iter = m_multiLine.begin(); iter != m_multiLine.end(); iter++ )
	{
		MultiLineInfo &line = *iter;
		int index = caret - line.start;
		if ( index < 0 )
			break;
		else if ( index <= line.count )
		{
			if ( m_text[caret-1] == L'\n' || (index == line.count && m_text[caret] >= L' ') )
			{
				ln++;
				ch = 0;
			}
			else
				ch = index;
			break;
		}
		ln++;
	}
}

// ----------------------------------------------------------------------------------
int CUiEditBox::PointToCaret(int ln, int ch)
{
	int caret;
	if ( ln < 0 )
		caret = 0;
	else if ( ln < (int)m_multiLine.size() )
	{
		ch = max(min(ch, m_multiLine[ln].count), 0);
		caret = m_multiLine[ln].start + ch;
		if ( ch == m_multiLine[ln].count && m_text[caret-1] == L'\n' )
			caret--;
	}
	else
		caret = m_textSize;
	return caret;
}

// ----------------------------------------------------------------------------------
void CUiEditBox::PlacePoint(int ln, int ch)
{
	m_ln = 0;
	m_ch = 0;
	if ( !m_multiLine.empty() )
	{
		m_ln = max(min(ln, (int)m_multiLine.size() - 1), 0);
		m_ch = max(min(ch, m_multiLine[m_ln].count), 0);
	}
}

// ----------------------------------------------------------------------------------
// 二維游標值轉換為滑鼠座標值
void CUiEditBox::PointToPos(int ln, int ch, float &x, float &y)
{
	// 元件顯示座標值
	CUiPoint pos = GetPaintPos();

	// 縮放值
	float scale = GetRealScale();

	x = pos.m_x;
	y = pos.m_y;
	if ( m_fontString )
	{
		x += m_textInset.m_left * scale;
		y += (m_textInset.m_top + m_fontString->GetFontType()->GetHeight() * (ln - m_firstLine)) * scale;
		if ( ln < (int)m_multiLine.size() )
		{
			int start = m_multiLine[ln].start;
			for ( int i = 0; i < ch; i++ )
			{
				if ( m_text[start+i] >= L' ' )
				{
					const WCHAR* nextChar = CRuFontString::CharNext(&m_text[start+i]);
					int ln = int(nextChar - &m_text[start+i]);
					CRuFontString* fontString = CRuFontString::GetFontString(m_fontString->GetFontType(), &m_text[start+i], ln, TRUE);

					if ( m_isRTL )
					{
						x += fontString->GetXSize() * scale;
					}
					else
					{
						x += fontString->GetXSize() * scale;
					}
					
					i += (ln - 1);
				}
			}
		}
	}
}

// ----------------------------------------------------------------------------------
// 滑鼠座標值轉換為二維游標值
void CUiEditBox::PosToPoint(float x, float y, int &ln, int &ch)
{
	ln = 0;
	ch = 0;
	if ( m_fontString == NULL || m_multiLine.empty() )
		return;

	CUiRectangle rect;
	GetRectangle(&rect);
	float scale = GetRealScale();
	float fontHeight = m_fontString->GetFontType()->GetHeight() * scale;
	rect.m_left		+= m_textInset.m_left * scale;
	rect.m_top		+= m_textInset.m_top * scale;
	rect.m_right	-= m_textInset.m_right * scale;
	rect.m_bottom	-= m_textInset.m_bottom * scale;

	ln = max((int)((y - rect.m_top) / fontHeight), 0) + m_firstLine;
	if ( ln >= (int)m_multiLine.size() )
		ln = (int)m_multiLine.size() - 1;
	MultiLineInfo &line = m_multiLine[ln];
	x -= rect.m_left;
	for ( int i = 0; i < line.count; i++ )
	{
		float fontWidth = 0.0f;
		if ( m_text[line.start + i] >= L' ' )
		{
			CRuFontString* fontString = CRuFontString::GetFontString(m_fontString->GetFontType(), &m_text[line.start + i], 1, TRUE);
			fontWidth = fontString->GetXSize() * scale;
		}
		if ( x < fontWidth * 0.5f )
			break;
		x -= fontWidth;
		ch++;
	}
}

// ----------------------------------------------------------------------------------
void CUiEditBox::SetTextColor(CUiColor& color)
{
	if ( m_fontString )
		m_fontString->SetColor(color.m_r, color.m_g, color.m_b);
}

// ----------------------------------------------------------------------------------
void CUiEditBox::HighlightText(int start, int end)
{
	if ( start >= 0 && start <= m_textSize )
		m_selStart = start;
	if ( end == -1 )
		end = m_textSize;
	PlaceCaret(end);
}

// ----------------------------------------------------------------------------------
int CUiEditBox::GetNumMultiLines()
{
	if ( m_fontString == NULL )
		return 0;

	float scale = GetRealScale();
	CUiRectangle rect;
	GetRectangle(&rect);
	rect.m_top		+= m_textInset.m_top * scale;
	rect.m_bottom	-= m_textInset.m_bottom * scale;
	return (int)(rect.RectHeight() / m_fontString->GetFontType()->GetHeight() * scale);
}

// ----------------------------------------------------------------------------------
void CUiEditBox::InsertKeyboardChar(const wchar_t* ws, int len)
{
	if (len == -1)
		len = (int)wcslen(ws);

	for ( int i = 0; i < len; i++ ) {
		HandleKeyboard(WM_CHAR, ws[i], 0);
	}
}

// ----------------------------------------------------------------------------------
void CUiEditBox::InsertKeyboardChar(const char* s, int len)
{
	wchar_t* ws = MultiByteToWChar(s);
	InsertKeyboardChar(ws, len);
	delete [] ws;
}

// ----------------------------------------------------------------------------------
void CUiEditBox::PlaceCaret(int caretPos)
{
	if (caretPos >= 0 && caretPos <= m_textSize)
	{
		m_caretPos = caretPos;
		
		// 多行處理
		if ( m_multiMode )
		{
			int multiLines = GetNumMultiLines();
			CaretToPoint(m_caretPos, m_ln, m_ch);
			if ( m_multiLine.empty() )
				SetMultiFirstLine(0);
			else if ( m_ln < m_firstLine )
				SetMultiFirstLine(m_ln);
			else if ( m_ln >= m_firstLine + multiLines )
				SetMultiFirstLine(m_ln - multiLines + 1);

			OnEditCursorChanged(m_ch, m_ln);
		}
		// 單行需要判斷是否捲動
		else
		{
			if ( m_caretPos < m_firstVisible )
				m_firstVisible = m_caretPos;
			else if (m_fontString)
			{
				float scale = GetRealScale();
				float extent = m_fontString->CaretToPos(m_firstVisible, m_caretPos);
				float width = (m_size.m_x - (m_textInset.m_left + m_textInset.m_right)) * scale;
				if ( width <= 0.0f )
				{
					m_firstVisible = min(m_firstVisible + 3, m_caretPos);
				}
				else
				{
					while (extent > width)
					{
						m_firstVisible = min(m_firstVisible + 3, m_caretPos);
						extent = m_fontString->CaretToPos(m_firstVisible, m_caretPos);
					}
				}
			}

			OnEditCursorChanged(m_caretPos, 0);
		}		
	}
}

// ----------------------------------------------------------------------------------
void CUiEditBox::PasteFromClipboard()
{
	DeleteSelectionText();
	if( OpenClipboard(NULL) )
	{
		HANDLE handle = GetClipboardData(CF_UNICODETEXT);
		if( handle )
		{
			// Convert the ANSI string to Unicode, then insert to our text string.
			WCHAR *tmpTextStr = (WCHAR*)GlobalLock(handle);
			if( tmpTextStr )
			{
				// Copy all characters up to null.
				INT pasteCount;
				if ( (pasteCount = InsertString(m_caretPos, tmpTextStr)) > 0 )
				{
					OnTextChanged();
					PlaceCaret( m_caretPos + pasteCount );
				}
				m_selStart = m_caretPos;
				GlobalUnlock( handle );
			}
		}
		CloseClipboard();
	}
}

// ----------------------------------------------------------------------------------
void CUiEditBox::CopyToClipboard()
{
	// Copy the selection text to the clipboard
	if( m_caretPos != m_selStart && OpenClipboard( NULL ) )
	{
		EmptyClipboard();
		INT first = min( m_caretPos, m_selStart );
		INT last = max( m_caretPos, m_selStart );
		if ( last - first > 0 )
		{
			wstring hyperlinkName;
			CUiFontString::HyperlinkText(m_text + first, last - first, hyperlinkName);

			int size = (int)hyperlinkName.size();
			if ( size > 0 )
			{
				HGLOBAL hBlock = GlobalAlloc( GMEM_MOVEABLE, sizeof(WCHAR) * (size + 1) );
				if ( hBlock )
				{
					WCHAR *tmpTextStr = (WCHAR*)GlobalLock( hBlock );
					memcpy( tmpTextStr, hyperlinkName.c_str(), size * sizeof(WCHAR) );
					tmpTextStr[size] = L'\0';  // Terminate it
					GlobalUnlock( hBlock );
				}
				SetClipboardData( CF_UNICODETEXT, hBlock );

				// We must not free the object until CloseClipboard is called.
				if( hBlock )
					GlobalFree( hBlock );
			}
		}
		CloseClipboard();
	}
}

// ----------------------------------------------------------------------------------
void CUiEditBox::DeleteSelectionText()
{
	if ( m_caretPos != m_selStart )
	{
		int first = min( m_caretPos, m_selStart );
		int last = max( m_caretPos, m_selStart );

		// Remove the characters
		while (first < last)
		{
			int removeSize = RemoveChar(first);
			if ( removeSize > 0 )
			{
				last -= removeSize;
				if ( last < 0 )
					last = 0;
			}
			else
				break;
		}

		OnTextChanged();

		// Update caret and selection
		PlaceCaret( last );

		m_selStart = m_caretPos;
	}
}

// ----------------------------------------------------------------------------------
void CUiEditBox::ResetCaretBlink()
{
	m_caretBool			= true;
	m_lastBlinkTime		= 0.0f;
}

// ----------------------------------------------------------------------------------
bool CUiEditBox::InsertChar(int index, WCHAR ch)
{
	if (m_textSize + 1 > m_letters || index > m_textSize)
		return false;

	memmove(m_text+index+1, m_text+index, sizeof(WCHAR) * (m_textSize-index+1));
	m_text[index] = ch;
	m_textSize++;
	return true;
}

// ----------------------------------------------------------------------------------
int CUiEditBox::RemoveChar(int index)
{	
	if (index >= m_textSize)
		return 0;
	
	int size = 1;
	int beginIndex = index;
	if ( m_fontString )
		size = min(m_fontString->GetCharSize(index, &beginIndex), m_textSize);

	memmove(m_text + beginIndex, m_text + beginIndex + size, sizeof(WCHAR) * (m_textSize - (beginIndex + size) + 1));
	m_textSize -= size;
	return size;
}

// ----------------------------------------------------------------------------------
int CUiEditBox::InsertString(int index, const WCHAR *text, int size)
{
	int count = 0;
	if (text)
	{
		if (size == -1)
			size = (int)wcslen(text);

		if ( m_multiMode == false )
		{
			size = 0;
			while ( text[size] >= 32 )
				size++;
		}

		for (int i = 0; i < size; i++)
		{
			if ( InsertChar(index+count, text[i]) )
				count++;
		}
	}
	return count;
}

// ----------------------------------------------------------------------------------
void CUiEditBox::OnUpdate(float elapsedTime)
{
	m_lastBlinkTime += elapsedTime;
	if (m_lastBlinkTime > m_blinkSpeed)
	{
		m_lastBlinkTime = 0.0f;
		m_caretBool = !m_caretBool;
	}	
	CUiFrame::OnUpdate(elapsedTime);
}

// ----------------------------------------------------------------------------------
void CUiEditBox::OnPaint(float alpha)
{
	// 若未設定顯示文字物件,無法繪出文字
	if ( m_fontString == NULL )
		return;

	// CUiFrame::OnPaint();	

	float scale = GetRealScale();

	if ( m_multiMode )
	{
		// 取得顯示區域
		CUiRectangle rect;
		GetRectangle(&rect);
		rect.m_left		+= m_textInset.m_left * scale;
		rect.m_top		+= m_textInset.m_top * scale;
		rect.m_right	-= m_textInset.m_right * scale;
		rect.m_bottom	-= m_textInset.m_bottom * scale;

		CRuFontType* fontType = m_fontString->GetFontType();
		float fontHeight = fontType->GetHeight() * scale;
		int first = min(m_caretPos, m_selStart);
		int end = max(m_caretPos, m_selStart);
		DWORD color = 0xFFFFFFFF;

		// 繪制字串
		CRuFontString::PaintBegin();
		//CRuFontChar::PaintBegin();
		for ( int i = 0; i < m_displayMaxLine; i++ )
		{
			wchar_t temp[512];

			int line = m_firstLine + i;
			if ( line >= (int)m_multiLine.size() )
				break;
			
			float paintX = rect.m_left;
			float paintY = rect.m_top + fontHeight * i;
			
			MultiLineInfo& lineInfo = m_multiLine[line];

			if ( m_isRTL )
			{
				int maxChar = min(lineInfo.count, 511);
				wcsncpy(temp, &m_text[lineInfo.start], maxChar);
				temp[maxChar] = 0;
				m_fontString->SetSpacing(0.0f);
				m_fontString->SetJustifyHType(CUiFontString::eJustifyH_Left);
				m_fontString->SetAnchorSize(CUiSize(rect.m_right - rect.m_left, 0));
				m_fontString->SetRTL(m_isRTL);
				m_fontString->SetText(temp);
				m_fontString->Render(alpha, paintX, paintY);				
			}
			else
			{
				int n = 0;
				while ( n < m_multiLine[line].count )
				{
					int index = m_multiLine[line].start + n;

					if ( m_text[index] >= L' ' )
					{
						// 下一個字元位置
						const wchar_t* charNext = CRuFontString::CharNext(&m_text[index]);

						int ln = int(charNext - &m_text[index]);

						CRuFontString* fontString = CRuFontString::GetFontString(fontType, m_text + index, ln);					

						if ( fontString )
						{
							DWORD blockTextColor = color;
							DWORD blockColor = 0;
							if ( index >= first && index < end )
							{
								blockColor = m_blockColor;
								//blockTextColor = 0xFF000000;
							}
							fontString->Paint(paintX, paintY, scale, blockTextColor, NULL, blockColor);
							paintX += fontString->GetXSize() * scale;
						}

						n += ln;
					}
					else
					{
						n++;
					}
				}
			}

			if ( m_isRTL )
			{
				m_fontString->SetText(m_text);
			}

			

			/*
			int n = 0;
			while ( n < m_multiLine[line].count )
			{
				int index = m_multiLine[line].start + n;

				if ( m_text[index] >= L' ' )
				{
					// 下一個字元位置
					const wchar_t* charNext = CRuFontString::CharNext(&m_text[index]);

					int ln = charNext - &m_text[index];

					CRuFontString* fontString = CRuFontString::GetFontString(fontType, m_text + index, ln);					

					if ( fontString )
					{
						DWORD blockTextColor = color;
						DWORD blockColor = 0;
						if ( index >= first && index < end )
						{
							blockColor = m_blockColor;
							//blockTextColor = 0xFF000000;
						}
						fontString->Paint(paintX, paintY, scale, blockTextColor, NULL, blockColor);
						paintX += fontString->GetXSize() * scale;
					}

					n += ln;
				}
				else
				{
					n++;
				}
			}
			*/

			/*
			for ( int n = 0; n < m_multiLine[line].count; n++)
			{
				int index = m_multiLine[line].start + n;
				if ( m_text[index] >= L' ' )
				{
					CRuFontString* fontString = CRuFontString::GetFontString(fontType, &m_text[index], 1, TRUE);
					if ( fontString )
					{
						DWORD blockTextColor = color;
						DWORD blockColor = 0;
						if ( index >= first && index < end )
						{
							blockColor = m_blockColor;
							//blockTextColor = 0xFF000000;
						}
						fontString->Paint(paintX, paintY, scale, blockTextColor, NULL, blockColor);
						paintX += fontString->GetXSize() * scale;
					}
				}
			}
			*/
		}	

		// 繪出游標
		if (m_pUi->GetKeyboardFocus() == this && m_caretBool)
		{			
			CRuRectangle caretRect;
			float width = 2.0f;

			//CaretToPoint(m_caretPos, ln, ch);
			PointToPos(m_ln, m_ch, caretRect.m_left, caretRect.m_top);
			caretRect.m_bottom = caretRect.m_top + fontHeight;

			// 插入狀態
			if ( m_isInsertMode )
			{
				if ( m_text[m_caretPos] >= L' ' )
				{
					// 下一個字元位置
					const wchar_t* charNext = CRuFontString::CharNext(&m_text[m_caretPos]);

					int ln = int(charNext - &m_text[m_caretPos]);

					CRuFontString* fontString = CRuFontString::GetFontString(fontType, &m_text[m_caretPos], ln, TRUE);
					if ( fontString )
						width = fontString->GetXSize() * scale;
					else
						width = 4.0f;
				}
				else
					width = 4.0f;
			}
			caretRect.m_right = caretRect.m_left + width;

			g_ruUIRenderer->SetEffectType( ru2DFXTYPE_FLAT );
			g_ruUIRenderer->SetEffectParameter( ru2DFXPARAM_ROTATE, ru2DROTATE_NONE );
			g_ruUIRenderer->Submit(1, &caretRect, &caretRect, &m_caretColor);

			// 設定最後顯示游標位置
			if ( m_pUi )
			{
				CUiPoint point(caretRect.m_left, caretRect.m_top);
				m_pUi->SetCaretPoint(point);
			}
		}
	}
	else
	{
		CUiPoint pos = GetPaintPos();
		float sx = pos.m_x + m_textInset.m_left * scale;
		float sy = pos.m_y + m_textInset.m_top * scale;
		CUiSize size = m_size * scale;

		CRuFontType* fontType = m_fontString->GetFontType();
		float fontHeight = (fontType) ? fontType->GetHeight() * scale : 0.0f;

		bool isTrail;
		float width = size.m_x - (m_textInset.m_left + m_textInset.m_right) * scale;
		int lastVisible = m_fontString->CaretToIndex(m_fontString->PosToCaret(m_firstVisible, width, isTrail));

		if (lastVisible != m_firstVisible)
		{
			// 繪出選取字元矩形框
			if ( m_caretPos != m_selStart )
			{
				CRuRectangle rect;
				CUiRGBA color(m_blockColor);
				color.SetAlphaScale(alpha);
				int first = max( min( m_caretPos, m_selStart ), m_firstVisible );
				int last = min( max( m_caretPos, m_selStart ), lastVisible );

				rect.m_left		= sx + m_fontString->CaretToPos(m_firstVisible, first);
				rect.m_right	= sx + m_fontString->CaretToPos(m_firstVisible, last);
				rect.m_top		= sy;
				rect.m_bottom	= sy + fontHeight;

				g_ruUIRenderer->SetEffectType( ru2DFXTYPE_FLAT );
				g_ruUIRenderer->SetEffectParameter( ru2DFXPARAM_ROTATE, ru2DROTATE_NONE );
				g_ruUIRenderer->Submit(1, &rect, &rect, &color.m_value);
			}
			m_fontString->Render(alpha, sx, sy, m_firstVisible, lastVisible);
		}

		// 繪出游標
		if (m_pUi->GetKeyboardFocus() == this && m_caretBool)
		{
			CRuRectangle rect;
			CUiRGBA color(0xFFFFFFFF);
			color.SetAlphaScale(alpha);
			rect.m_left		= sx + m_fontString->CaretToPos(m_firstVisible, m_caretPos);
			rect.m_right	= rect.m_left + 2.0f;
			rect.m_top		= sy;
			rect.m_bottom	= sy + fontHeight;

			if ( m_isInsertMode )
				rect.m_right += 4.0f;

			g_ruUIRenderer->SetEffectType( ru2DFXTYPE_FLAT );
			g_ruUIRenderer->SetEffectParameter( ru2DFXPARAM_ROTATE, ru2DROTATE_NONE );
			g_ruUIRenderer->Submit(1, &rect, &rect, &m_caretColor);

			// 設定最後顯示游標位置
			if ( m_pUi )
			{
				CUiPoint point(rect.m_left, rect.m_top);
				m_pUi->SetCaretPoint(point);
			}
		}
	}
}

// ----------------------------------------------------------------------------------
void CUiEditBox::OnResize()
{
	UpdateLine();
	CUiFrame::OnResize();
}

// ----------------------------------------------------------------------------------
void CUiEditBox::OnMouseDown(CUiPoint pt, int key)
{
	switch (key)
	{
	case MOUSE_LBUTTON:
		if ( m_multiMode )
		{
			int ln, ch;
			PosToPoint(pt.m_x, pt.m_y, ln, ch);
			PlaceCaret(PointToCaret(ln, ch));
			m_selStart = m_caretPos;
			ResetCaretBlink();
		}
		else
		{
			if (m_fontString)
			{
				bool isTrail;
				CUiPoint pos = GetPaintPos();
				float scale = GetRealScale();
				float offset = pt.m_x - (pos.m_x + m_textInset.m_left * scale);
				float first = m_fontString->CaretToPos(0, m_firstVisible);
				int caret = m_fontString->PosToCaret(0, first+offset, isTrail);
				if (isTrail && caret < m_textSize)
					PlaceCaret(caret + 1);
				else
					PlaceCaret(caret);

				m_selStart = m_caretPos;
				ResetCaretBlink();
			}
		}
		break;
	}
	m_pUi->SetKeyboardFocus(this);
	CUiFrame::OnMouseDown(pt, key);
}

// ----------------------------------------------------------------------------------
void CUiEditBox::OnMouseMove(CUiPoint pt)
{
	//if ( m_isPushed )
	if ( m_pUi->GetMouseDown(MOUSE_LBUTTON) == this )
	{
		if ( m_multiMode )
		{
			int ln, ch;
			PosToPoint(pt.m_x, pt.m_y, ln, ch);
			PlaceCaret(PointToCaret(ln, ch));
		}
		else if (m_fontString)
		{
			bool isTrail;
			CUiPoint pos = GetPaintPos();
			float scale = GetRealScale();
			float offset = pt.m_x - (pos.m_x + m_textInset.m_left * scale);
			float first = m_fontString->CaretToPos(0, m_firstVisible);
			int caret = m_fontString->PosToCaret(0, first+offset, isTrail);
			if (isTrail && caret < m_textSize)
				PlaceCaret(caret + 1);
			else
				PlaceCaret(caret);
		}
	}
	CUiFrame::OnMouseMove(pt);
}

// ----------------------------------------------------------------------------------
void CUiEditBox::OnChar(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case VK_BACK:
		if ( m_isKeyboardInput == false )
			break;

		if( m_caretPos != m_selStart )
			DeleteSelectionText();
		else if( m_caretPos > 0 )
		{
			int removeSize = 1;
			removeSize = RemoveChar(m_caretPos - 1);
			OnTextChanged();

			// Move the caret, then delete the char.
			PlaceCaret(m_caretPos - removeSize);
			m_selStart = m_caretPos;
		}
		ResetCaretBlink();
		break;

	case 3:				// Ctrl-C Copy
	case 24:			// Ctrl-X Cut
		CopyToClipboard();

		// If the key is Ctrl-X, delete the selection too.
		if ( wParam == 24 && m_isKeyboardInput )
			DeleteSelectionText();
		break;

	case 22:			// Ctrl-V Paste
		if ( m_isKeyboardInput )
			PasteFromClipboard();
		break;

	default:
		if ( m_isKeyboardInput == false )
			break;

		if ( m_multiMode && wParam == VK_RETURN )
		{
			wParam = L'\n';
		}
		else if ( wParam < L' ' )
			break;

		if (m_isNumeric && (wParam < L'0' || wParam > L'9'))
			break;

		// If there's a selection and the user starts to type, the selection should be deleted.
		if(m_caretPos != m_selStart)
			DeleteSelectionText();

		// If we are in overwrite mode and there is already
		// a char at the caret's position, simply replace it.
		// Otherwise, we insert the char as normal.	
		if (m_isInsertMode && m_caretPos < m_textSize)
		{
			m_text[m_caretPos] = (WCHAR)wParam;
			OnTextChanged();
			PlaceCaret( m_caretPos + 1 );
			m_selStart = m_caretPos;
		}
		// Insert the char
		else if ( InsertChar( m_caretPos, (WCHAR)wParam ) )
		{
			OnTextChanged();
			PlaceCaret( m_caretPos + 1 );
			m_selStart = m_caretPos;
		}
		ResetCaretBlink();
		break;
	}

	CUiFrame::OnChar(wParam, lParam);
}

// ----------------------------------------------------------------------------------
void CUiEditBox::OnKeyDown(WPARAM wParam, LPARAM lParam)
{
	const WCHAR* history = NULL;
	switch ( wParam )
	{
	case VK_DELETE:
		if ( m_isKeyboardInput == false )
			break;

		// Check if there is a text selection.
		if( m_caretPos != m_selStart )
			DeleteSelectionText();
		// Deleting one character
		else if ( RemoveChar( m_caretPos ) )
			OnTextChanged();

		ResetCaretBlink();
		break;

	case VK_INSERT:
		if( GetKeyState( VK_CONTROL ) < 0 )
		{
			// Control Insert. Copy to clipboard
			CopyToClipboard();
		}
		else if( GetKeyState( VK_SHIFT ) < 0 )
		{
			// Shift Insert. Paste from clipboard
			PasteFromClipboard();
		}
		else
		{
			// Toggle caret insert mode
			m_isInsertMode = !m_isInsertMode;
		}
		break;

	case VK_LEFT:
		if ( m_caretPos > 0 )
		{
			int size = 1;
			if ( m_fontString )
				size = m_fontString->GetCharSize(m_caretPos - 1);
			PlaceCaret( m_caretPos - size );
		}

		// Shift is not down
		if( GetKeyState( VK_SHIFT ) >= 0 )
			m_selStart = m_caretPos;

		ResetCaretBlink();
		break;

	case VK_RIGHT:
		if ( m_caretPos < m_textSize )
		{
			int size = 1;
			if ( m_fontString )
				size = m_fontString->GetCharSize(m_caretPos);
			PlaceCaret( m_caretPos + size );
		}

		// Shift is not down
		if( GetKeyState( VK_SHIFT ) >= 0 )
			m_selStart = m_caretPos;

		ResetCaretBlink();
		break;

	case VK_HOME:
		if ( m_multiMode )
		{
			//int ln, ch;
			//CaretToPoint(m_caretPos, ln, ch);
			PlaceCaret(PointToCaret(m_ln, 0));
		}
		else
			PlaceCaret( 0 );

		// Shift is not down
		if( GetKeyState( VK_SHIFT ) >= 0 )
			m_selStart = m_caretPos;
		ResetCaretBlink();
		break;

	case VK_END:
		if ( m_multiMode )
		{
			//int ln, ch;
			//CaretToPoint(m_caretPos, ln, ch);
			PlaceCaret(PointToCaret(m_ln, 0xFFFF));
		}
		else
			PlaceCaret( m_textSize );

		// Shift is not down
		if( GetKeyState( VK_SHIFT ) >= 0 )
			m_selStart = m_caretPos;
		ResetCaretBlink();
		break;

	case VK_UP:
		if ( m_multiMode && m_fontString )
		{
			if ( m_ln > 0 )
				PlaceCaret(PointToCaret(m_ln - 1, m_ch));
		}
		else
		{
			history = GetHistoryLine(m_historyLast - 1);
			if ( history )
			{			
				SetText(history);
				m_selStart = 0;
				PlaceCaret(m_textSize);
			}
		}

		// Shift is not down
		if( GetKeyState( VK_SHIFT ) >= 0 )
			m_selStart = m_caretPos;
		ResetCaretBlink();
		break;

	case VK_DOWN:
		if ( m_multiMode && m_fontString )
		{
			if ( m_ln + 1 < (int)m_multiLine.size() )
				PlaceCaret(PointToCaret(m_ln + 1, m_ch));
		}
		else
		{
			history = GetHistoryLine(m_historyLast + 1);
			if ( history )
			{
				SetText(history);
				m_selStart = 0;
				PlaceCaret(m_textSize);
			}
		}

		// Shift is not down
		if( GetKeyState( VK_SHIFT ) >= 0 )
			m_selStart = m_caretPos;
		ResetCaretBlink();
		break;
	}
	CUiFrame::OnKeyDown(wParam, lParam);
}
// ----------------------------------------------------------------------------------
void CUiEditBox::OnTextChanged()
{
	CUiFrame::OnTextChanged();

	UpdateLine();

	if (m_fontString)
	{
		if ( m_isPassword )
		{
			wstring str;
			for ( int i = 0; i < m_textSize; i++ ) {
				str += L"*";
			}
			m_fontString->SetText(str.c_str());
		}
		else
		{
			m_fontString->SetText(m_text);
		}
	}
}
// ----------------------------------------------------------------------------------
void		CUiEditBox::SetPlaceCaret(int caretPos)
{
	if (caretPos<=0)
		caretPos=m_textSize;

	if (caretPos>=m_textSize)
		caretPos=m_textSize;

	//int len;
	//int ch;

	//CaretToPoint(caretPos,len,ch);
	PlaceCaret(caretPos);
	m_selStart = caretPos;
	ResetCaretBlink();

}

// ----------------------------------------------------------------------------------
void CUiEditBox::SetMultiFirstLine(int line)
{
	if ( line < 0 || (int)line > m_multiLine.size() )
		return;

	m_firstLine = line;
	if ( m_ln < m_firstLine )
		PlacePoint(m_firstLine, m_ch);
	else if ( m_ln >= m_firstLine + m_displayMaxLine )
		PlacePoint(m_firstLine + m_displayMaxLine - 1, m_ch);
}

// ----------------------------------------------------------------------------------
int CUiEditBox::GetMemSize()
{
	int size = CUiFrame::GetMemSize() + sizeof(CUiEditBox) - sizeof(CUiFrame);

	for ( list<WCHAR*>::iterator iter = m_history.begin(); iter != m_history.end(); iter++ )
		size += (int)(wcslen(*iter) + 1) * sizeof(WCHAR);

	if ( m_text )
		size += (int)(wcslen(m_text) + 1) * sizeof(WCHAR);

	if ( m_fontString )
		size += m_fontString->GetMemSize();

	size += (int)m_multiLine.size() * sizeof(MultiLineInfo);

	return size;
}