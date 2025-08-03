// ³o¬O¤@­Ó¤ñ¸û¯S§Oªº¬É­±,¦b¨ä¥L¬É­±²£¥Í«e·|Àu¥ý²£¥Í


#include "../mainproc/GameMain.h"
#include "../../tools/getfontnamefromfile/getfontnamefromfile.h"
#include "../Login/AccountLogin.h"
#include "LoadingFrame.h"


CLoadingFrame*				g_pLoadingFrame = NULL;

// --------------------------------------------------------------------------------------
CLoadingFrame::CLoadingFrame(CInterface* object)
	: CInterfaceBase(object)
{
	m_isLoading				= false;
	m_delayEndTime			= 0.0f;
	m_fadeDuration			= 0.0f;

	m_loadingFrame			= NULL;	
	m_loadingScreen			= NULL;
	m_loadingNameLogo		= NULL;
	m_loadingMoliyoLogo		= NULL;
	m_loadingCompanyLogo1	= NULL;
	m_loadingCompanyLogo2	= NULL;
	m_koreanImage1			= NULL;
	m_koreanImage2			= NULL;
	m_koreanText			= NULL;

	m_loadingBar			= NULL;
	m_loadingTipBG			= NULL;
	m_loadingTipText		= NULL;
	g_pLoadingFrame		= this;
}

// --------------------------------------------------------------------------------------
CLoadingFrame::~CLoadingFrame()
{
	g_pLoadingFrame = NULL;
}

// --------------------------------------------------------------------------------------
void CLoadingFrame::RegisterFunc()
{
	lua_State* L = GetLuaState();
	if (L)
	{
	}
}

// --------------------------------------------------------------------------------------
void CLoadingFrame::BuildFrames()
{
	char temp[512];
	CUi* pUi = m_interface->GetUiWorld();
	CUiTexture* pTexture;
	CUiAnchor anchor;

	CRuFontType* fontType = NULL;
	CRuFontType* fontType2 = NULL;
	string outputFilename;
	string resource = m_interface->GetResourcePath();
	string fullname = resource + g_pGameMain->GetDefaultFontPath();
	AddFontResourceEx( fullname.c_str(), FR_PRIVATE, 0 );
	string fontname = GetFontNameFromFile( fullname.c_str() );
	if ( !fontname.empty() ) {
		fontType = CRuFontType::GetFontType(fontname.c_str(), 16, ruFONTWEIGHT_BOLD, ruFONTOUTLINE_NORMAL);
		fontType2 = CRuFontType::GetFontType(fontname.c_str(), 28, ruFONTWEIGHT_BOLD, ruFONTOUTLINE_NORMAL);
	}

	// ©³ªO
	m_loadingFrame = (CUiFrame*)pUi->CreateObject(CUiLayout::eUiType_Frame, "LoadingFrame", pUi->GetRoot());
	assert(m_loadingFrame);
	m_loadingFrame->SetMouseEnable(true);
	m_loadingFrame->SetKeyboardEnable(true);
	m_loadingFrame->SetFrameStrata(eUIFrame_Strata_High);	
	m_loadingFrame->SetFrameLevel(10000);
	m_loadingFrame->SetTopLevel(true);
	m_loadingFrame->SetAnchorSize(CUiSize(0.0f, 0.0f));
	m_loadingFrame->SetAnchor(CUiAnchor());
	m_loadingFrame->SetVisible(false);

	// ¶Â¦â­I´º¹Ï§Î
	pTexture = (CUiTexture*)pUi->CreateObject(CUiLayout::eUiType_Texture, "", m_loadingFrame);
	assert(pTexture);
	pTexture->SetAnchorSize(CUiSize(0.0f, 0.0f));
	pTexture->SetAnchor(CUiAnchor());
	pTexture->SetFile("Interface\\Common\\Block-Fill");
	pTexture->SetColor(0.0f, 0.0f, 0.0f);
	m_loadingFrame->SetLayers(CUiFrame::eLayerType_Background, pTexture);

	// Loading ­I´º¹Ï§Î
	m_loadingScreen = (CUiTexture*)pUi->CreateObject(CUiLayout::eUiType_Texture, "", m_loadingFrame);
	assert(m_loadingScreen);
	m_loadingScreen->SetAnchorSize(CUiSize(0.0f, 0.0f));
	anchor.Clear();
	anchor.m_point = CUiAnchor::eAnchor_Center;
	m_loadingScreen->SetAnchor(anchor);
	m_loadingFrame->SetLayers(CUiFrame::eLayerType_Border, m_loadingScreen);

	// LOGO
	m_loadingNameLogo = (CUiTexture*)pUi->CreateObject(CUiLayout::eUiType_Texture, "", m_loadingFrame);
	assert(m_loadingNameLogo);
	m_loadingNameLogo->SetAnchorSize(CUiSize(320.0f, 160.0f));
	anchor.Clear();
	anchor.m_relativeTo = m_loadingScreen;
	anchor.m_point = CUiAnchor::eAnchor_TopLeft;
	sprintf_s(temp, 512, "Interface\\Login\\logo\\%s\\RA_LOGO", g_pGameMain->GetImageLocation("logo"));
	m_loadingNameLogo->SetAnchor(anchor);
	m_loadingNameLogo->SetFile(temp);
	m_loadingFrame->SetLayers(CUiFrame::eLayerType_Artwork, m_loadingNameLogo);	

	// New LOGO	
	sprintf_s(temp, 512, "Interface\\Login\\logo\\%s\\new_forgster_logo", g_pGameMain->GetImageLocation("logo"));
	if ( FileExist(temp, outputFilename) )
	{
		// left bottom logo
		m_loadingMoliyoLogo = (CUiTexture*)pUi->CreateObject(CUiLayout::eUiType_Texture, "", m_loadingFrame);
		assert(m_loadingMoliyoLogo);
		m_loadingMoliyoLogo->SetAnchorSize(CUiSize(512.0f, 64.0f));
		anchor.Clear();
		anchor.m_relativeTo = m_loadingScreen;
		anchor.m_point = CUiAnchor::eAnchor_BottomLeft;
		m_loadingMoliyoLogo->SetAnchor(anchor);
		m_loadingMoliyoLogo->SetFile(outputFilename.c_str());
		m_loadingFrame->SetLayers(CUiFrame::eLayerType_Artwork, m_loadingMoliyoLogo);
		
		m_loadingCompanyLogo1 = m_loadingCompanyLogo2 = NULL;		
	}
	else
	{
		// moliyo_logo
		m_loadingMoliyoLogo = (CUiTexture*)pUi->CreateObject(CUiLayout::eUiType_Texture, "", m_loadingFrame);
		assert(m_loadingMoliyoLogo);
		m_loadingMoliyoLogo->SetAnchorSize(CUiSize(176.0f, 88.0f));
		anchor.Clear();
		anchor.m_relativeTo = m_loadingScreen;
		anchor.m_point = CUiAnchor::eAnchor_BottomLeft;
		anchor.m_offset.m_x = 15.0f;
		anchor.m_offset.m_y = 22.0f;
		m_loadingMoliyoLogo->SetAnchor(anchor);
		sprintf_s(temp, 512, "Interface\\Login\\logo\\%s\\runewaker_logo", g_pGameMain->GetImageLocation("logo"));
		m_loadingMoliyoLogo->SetFile(temp);
		m_loadingFrame->SetLayers(CUiFrame::eLayerType_Artwork, m_loadingMoliyoLogo);		

		m_loadingCompanyLogo1 = (CUiTexture*)pUi->CreateObject(CUiLayout::eUiType_Texture, "", m_loadingFrame);
		assert(m_loadingCompanyLogo1);
		m_loadingCompanyLogo1->SetAnchorSize(CUiSize(256, 64.0f));
		anchor.Clear();
		anchor.m_relativeTo = m_loadingMoliyoLogo;
		anchor.m_point = CUiAnchor::eAnchor_BottomLeft;
		anchor.m_relativePoint = CUiAnchor::eAnchor_BottomRight;
		anchor.m_offset.m_x = 12.0f;
		anchor.m_offset.m_y = -6.0f;
		m_loadingCompanyLogo1->SetAnchor(anchor);
		//if ( strcmp(g_pGameMain->GetLanguage(), "ENUS") == 0 || strcmp(g_pGameMain->GetLanguage(), "DE") == 0 )
		sprintf_s(temp, 512, "Interface\\Login\\logo\\%s\\frogster_logo", g_pGameMain->GetImageLocation("logo"));
		m_loadingCompanyLogo1->SetFile(temp);
		m_loadingFrame->SetLayers(CUiFrame::eLayerType_Artwork, m_loadingCompanyLogo1);

		m_loadingCompanyLogo2 = (CUiTexture*)pUi->CreateObject(CUiLayout::eUiType_Texture, "", m_loadingFrame);
		assert(m_loadingCompanyLogo2);
		m_loadingCompanyLogo2->SetAnchorSize(CUiSize(256, 64.0f));
		anchor.Clear();
		anchor.m_relativeTo = m_loadingCompanyLogo1;
		anchor.m_point = CUiAnchor::eAnchor_TopLeft;
		anchor.m_relativePoint = CUiAnchor::eAnchor_TopRight;
		anchor.m_offset.m_x = 0.0f;
		anchor.m_offset.m_y = 0.0f;
		m_loadingCompanyLogo2->SetAnchor(anchor);
		//if ( strcmp(g_pGameMain->GetLanguage(), "ENUS") == 0 || strcmp(g_pGameMain->GetLanguage(), "DE") == 0 )
		sprintf_s(temp, 512, "Interface\\Login\\logo\\%s\\frogster_logo2", g_pGameMain->GetImageLocation("logo"));
		m_loadingCompanyLogo2->SetFile(temp);
		m_loadingFrame->SetLayers(CUiFrame::eLayerType_Artwork, m_loadingCompanyLogo2);
	}

	// Copy Right - Right Bottom	
	sprintf_s(temp, 512, "Interface\\Login\\logo\\%s\\new_forgster_copyright", g_pGameMain->GetImageLocation("logo"));
	if ( FileExist(temp, outputFilename) )
	{
		pTexture = (CUiTexture*)pUi->CreateObject(CUiLayout::eUiType_Texture, "", m_loadingFrame);
		if ( pTexture )
		{
			pTexture->SetAnchorSize(CUiSize(512.0f, 64.0f));
			anchor.Clear();
			anchor.m_relativeTo = m_loadingScreen;
			anchor.m_point = CUiAnchor::eAnchor_BottomRight;
			pTexture->SetAnchor(anchor);
			pTexture->SetFile(outputFilename.c_str());
			m_loadingFrame->SetLayers(CUiFrame::eLayerType_Artwork, pTexture);
		}
	}	

	// Korean Image
	if ( strcmp( g_pGameMain->GetCountry(), LANGUAGE_CODE_KR ) == 0 )
	{
		m_koreanImage1 = (CUiTexture*)pUi->CreateObject(CUiLayout::eUiType_Texture, "", m_loadingFrame);
		assert(m_koreanImage1);
		m_koreanImage1->SetAnchorSize(CUiSize(152.0f, 152.0f));
		anchor.Clear();
		anchor.m_relativeTo = m_loadingFrame;
		anchor.m_point = CUiAnchor::eAnchor_TopRight;
		anchor.m_offset.m_x = -120.0f;
		anchor.m_offset.m_y = 0.0f;
		m_koreanImage1->SetAnchor(anchor);

		if ( g_pAccountLogin->GetServerAge() >= 18 )
		{
			sprintf_s(temp, 512, "Interface\\Common\\Korean18");
		}
		else
		{
			sprintf_s(temp, 512, "Interface\\Common\\Korean15");
		}

		m_koreanImage1->SetFile(temp);
		m_loadingFrame->SetLayers(CUiFrame::eLayerType_Artwork, m_koreanImage1);

		m_koreanImage2 = (CUiTexture*)pUi->CreateObject(CUiLayout::eUiType_Texture, "", m_loadingFrame);
		assert(m_koreanImage1);
		m_koreanImage2->SetAnchorSize(CUiSize(152.0f, 152.0f));
		anchor.Clear();
		anchor.m_relativeTo = m_loadingFrame;
		anchor.m_point = CUiAnchor::eAnchor_TopRight;
		anchor.m_offset.m_x = 10.0f;
		anchor.m_offset.m_y = 0.0f;
		m_koreanImage2->SetAnchor(anchor);
		m_koreanImage2->SetFile("Interface\\Common\\KoreanVoilence");
		m_loadingFrame->SetLayers(CUiFrame::eLayerType_Artwork, m_koreanImage2);

		if ( g_pAccountLogin->GetServerAge() >= 18 )
		{		
			m_koreanText = (CUiFontString*)pUi->CreateObject(CUiLayout::eUiType_FontString, "", m_loadingFrame);
			m_koreanText->SetAnchorSize(CUiSize(512.0f, 0.0f));
			anchor.Clear();
			anchor.m_relativeTo = m_loadingFrame;
			anchor.m_point = CUiAnchor::eAnchor_TopRight;
			anchor.m_offset.m_x = -6.0f;
			anchor.m_offset.m_y = 156.0f;
			m_koreanText->SetAnchor(anchor);
			m_koreanText->SetFontType(fontType2);
			m_koreanText->SetSpacing(1.0f);
			m_koreanText->SetJustifyHType(CUiFontString::eJustifyH_Left);
			m_koreanText->SetTextA(g_ObjectData->GetString("SYS_KOREA_SERVER_18"));			
			m_loadingFrame->SetLayers(CUiFrame::eLayerType_Artwork, m_koreanText);
		}
		else
		{
			m_koreanText = (CUiFontString*)pUi->CreateObject(CUiLayout::eUiType_FontString, "", m_loadingFrame);
			m_koreanText->SetAnchorSize(CUiSize(512.0f, 0.0f));
			anchor.Clear();
			anchor.m_relativeTo = m_loadingFrame;
			anchor.m_point = CUiAnchor::eAnchor_TopRight;
			anchor.m_offset.m_x = -6.0f;
			anchor.m_offset.m_y = 156.0f;
			m_koreanText->SetAnchor(anchor);
			m_koreanText->SetFontType(fontType2);
			m_koreanText->SetSpacing(1.0f);
			m_koreanText->SetJustifyHType(CUiFontString::eJustifyH_Left);
			m_koreanText->SetTextA(g_ObjectData->GetString("SYS_KOREA_SERVER_15"));
			m_loadingFrame->SetLayers(CUiFrame::eLayerType_Artwork, m_koreanText);
		}
	}

	// ¶i«×±ø
	m_loadingBar = (CUiStatusBar*)pUi->CreateObject(CUiLayout::eUiType_StatusBar, "", m_loadingFrame);
	assert(m_loadingBar);
	m_loadingBar->SetAnchorSize(CUiSize(452, 24));
	m_loadingBar->SetMinValue(0.0f);
	m_loadingBar->SetMaxValue(1.0f);

	anchor.Clear();
	anchor.m_point = CUiAnchor::eAnchor_Bottom;
	anchor.m_offset.m_x = 0;
	anchor.m_offset.m_y = -80;
	m_loadingBar->SetAnchor(anchor);	

	// ¶i«×±ø¥~®Ø¹Ï§Î
	pTexture = (CUiTexture*)pUi->CreateObject(CUiLayout::eUiType_Texture, "", m_loadingBar);
	pTexture->SetFile("Interface\\Login\\LoadingBar\\Loading-BarBorder");
	pTexture->SetAnchorSize(CUiSize(512, 32));
	anchor.Clear();
	anchor.m_point = CUiAnchor::eAnchor_Center;
	pTexture->SetAnchor(anchor);
	m_loadingBar->SetLayers(CUiFrame::eLayerType_Overlay, pTexture);

	// Bar Texture
	pTexture = (CUiTexture*)pUi->CreateObject(CUiLayout::eUiType_Texture, "", m_loadingBar);
	pTexture->SetFile("Interface\\Login\\LoadingBar\\Loading-BarFill");
	m_loadingBar->SetBarTexture(pTexture);	

	// Tips ¤å¦r
	m_loadingTipText = (CUiFontString*)pUi->CreateObject(CUiLayout::eUiType_FontString, "", m_loadingBar);
	m_loadingTipText->SetAnchorSize(CUiSize(458.0f, 0.0f));
	anchor.Clear();
	anchor.m_point = CUiAnchor::eAnchor_Bottom;
	anchor.m_relativePoint = CUiAnchor::eAnchor_Top;
	anchor.m_offset.m_x = 0.0f;
	anchor.m_offset.m_y = -12.0f;
	m_loadingTipText->SetSpacing(1.0f);
	m_loadingTipText->SetAnchor(anchor);
	m_loadingTipText->SetFontType(fontType);
	m_loadingTipText->SetColor(1.0f, 0.8f, 0.0f);
	m_loadingBar->SetLayers(CUiFrame::eLayerType_Artwork, m_loadingTipText);	

	m_loadingTipBG = (CUiTexture*)pUi->CreateObject(CUiLayout::eUiType_Texture, "", m_loadingBar);
	m_loadingTipBG->SetFile("Interface\\Common\\Block-Fill");
	m_loadingTipBG->SetAnchorSize(CUiSize(458.0f, 24.0f));
	anchor.Clear();
	anchor.m_point = CUiAnchor::eAnchor_TopLeft;
	anchor.m_relativePoint = CUiAnchor::eAnchor_TopLeft;
	anchor.m_relativeTo = m_loadingTipText;
	anchor.m_offset.m_x = -4.0f;
	anchor.m_offset.m_y = -4.0f;
	m_loadingTipBG->SetAnchor(anchor);
	m_loadingTipBG->SetColor(0.12f, 0.11f, 0.10f);
	m_loadingTipBG->SetAlpha(0.4f);
	m_loadingBar->SetLayers(CUiFrame::eLayerType_Background, m_loadingTipBG);

	if ( m_isLoading ) {
		LoadingStart(true);
	}
}

// --------------------------------------------------------------------------------------
void CLoadingFrame::DestroyFrames()
{
	//m_isLoading			= false;
	//m_delayEndTime		= 0.0f;
	//m_fadeDuration		= 0.0f;

	m_loadingFrame			= NULL;	
	m_loadingScreen			= NULL;
	m_loadingNameLogo		= NULL;
	m_loadingMoliyoLogo		= NULL;
	m_loadingCompanyLogo1	= NULL;
	m_loadingCompanyLogo2	= NULL;
	m_koreanImage1			= NULL;
	m_koreanImage2			= NULL;
	m_loadingBar			= NULL;
	m_loadingTipText		= NULL;
	m_loadingTipBG			= NULL;

	//m_isLoading			= false;
}

// --------------------------------------------------------------------------------------
void CLoadingFrame::Update(float elapsedTime)
{
	if ( m_isLoading ) {
		// ²H¥X
		if ( m_fadeDuration < 1.0f ) {
			m_fadeDuration += elapsedTime * 2.0f;
			if ( m_loadingFrame )
				m_loadingFrame->SetAlpha(m_fadeDuration);
		}		
	} else if ( m_delayEndTime > 0.001f ) {
		// µ²§ôÅª¨ú,¶}±Ò¹CÀ¸¬É­±
		m_delayEndTime -= elapsedTime;
		if ( m_delayEndTime < 0.001f ) {
			if ( m_loadingFrame )
				m_loadingFrame->SetVisible(false);
			SendWorldEvent("LOADING_END");
		}
	}
}

// --------------------------------------------------------------------------------------
void CLoadingFrame::LoadingStart(bool forceReset)
{
	lua_State* L = GetLuaState();

	if ( !forceReset && m_isLoading )
		return;

	m_isLoading = true;
	m_delayEndTime = 1.0f;
	m_fadeDuration = 1.0f;

	// §ä¥X¦@¥Îªº¤j¦a¹Ï¦WºÙ
	if ( m_loadingScreens.empty() )
	{
		char loadingFileName[512];
		const char* imageLocation = g_pGameMain->GetImageLocation("LoadingScreen");

		// ¤j¦a¹Ï
		for ( int i = 1; i < 99; i++ )
		{
			sprintf_s(loadingFileName, 512, "Interface\\Login\\LoginScreens\\%s\\loading_world%02d", imageLocation, i);

			// ¹w³]¹Ï§Î
			const char* list[4] = { ".dds", ".tga", ".png", ".bmp" };
			for ( int last = 0; last < 4; last++ )
			{
				string filename = loadingFileName;
				filename += list[last];

				if ( g_ruResourceManager->FileExists(filename.c_str()) )
				{
					m_loadingScreens.push_back(filename);
					break;
				}
			}
		}
	}

	if ( !forceReset ) {

		// ¨ú±o¦a¹Ï¦WºÙ
		char mapName[512];
		strcpy(mapName, g_pGameMain->GetMapName());
		int len = (int)strlen(mapName);
		if ( len > 4 ) {
			mapName[len-4] = 0;
			len -= 4;
		}
		for ( int i = len - 1; i >= 0; i-- ) {
			if ( mapName[i] == '\\' || mapName[i] == '/' ) {
				memmove(mapName, mapName+(i+1), len - i);
				break;
			}
		}
		
		// ¤j¦a¹Ï
		if ( strcmp(mapName, "world") == 0 )
		{
			if ( m_loadingScreens.size() > 0 )
			{
				m_loadingFilename = m_loadingScreens[(rand() % m_loadingScreens.size())];
			}
		}
		// °Æ¥»©Î¯S®í¦a¹Ï
		else 
		{
			char loadingFileName[512];
			memset(loadingFileName, 0, sizeof(loadingFileName));

			const char* imageLocation = g_pGameMain->GetImageLocation("LoadingScreen");			
			sprintf_s(loadingFileName, 512, "Interface\\Login\\LoginScreens\\%s\\loading_%s", imageLocation, mapName);

			// ¹w³]¹Ï§Î
			const char* list[4] = { ".dds", ".tga", ".png", ".bmp" };
			int last = 0;
			for ( ; last < 4; last++ )
			{
				string filename = loadingFileName;
				filename += list[last];

				if ( g_ruResourceManager->FileExists(filename.c_str()) ) {
					break;
				}
			}

			if ( last >= 4 && m_loadingScreens.size() > 0 ) {
				m_loadingFilename = m_loadingScreens[(rand() % m_loadingScreens.size())];
			} else {
				m_loadingFilename = loadingFileName;
			}			
		}		
	} else {
		m_fadeDuration = 1.0f;
	}
	
	// ³]©w­I´º¹Ï§Î
	if ( m_loadingScreen )
	{
		float width = g_pGameMain->GetUIWidth() / g_pInterface->GetScale();
		float height = g_pGameMain->GetUIHeight() / g_pInterface->GetScale();
		m_loadingScreen->SetAnchorSize(CUiSize(height * 1.3333f, height));
		m_loadingScreen->SetFile(m_loadingFilename.c_str());
	}

	// ³]©wTIPS¤å¦r
	if ( m_loadingTipText )
	{
		const char* tipText = g_ObjectData->GetTipText();
		m_loadingTipText->SetAnchorSize(CUiSize(458.0f, 0.0f));
		m_loadingTipText->SetTextA(tipText);

		vector<StringInfo>& stringData = m_loadingTipText->GetStringData();
		if ( stringData.size() > 1 )
		{
			// ¥­§¡¼e«×
			float width = 0.0f;
			for ( vector<StringInfo>::iterator iter = stringData.begin(); iter != stringData.end(); iter++ )
				width += iter->width;

			float averageWidth = width / stringData.size();
			averageWidth += 20.0f;

			m_loadingTipText->SetAnchorSize(CUiSize(averageWidth, 0.0f));
			m_loadingTipText->SetTextA(tipText);
		} 
		
		CUiSize textSize = m_loadingTipText->GetSize();
		textSize.m_x += 8.0f;
		textSize.m_y += 8.0f;
		if ( m_loadingTipBG )
			m_loadingTipBG->SetSize(textSize);
	}

	// ²H¤J
	if ( m_loadingFrame )
	{
		m_loadingFrame->SetAlpha(m_fadeDuration);
		m_loadingFrame->SetVisible(true);
	}	

	// ¹w³]¥Ñ¹s¶}©l
	if ( m_loadingBar )
		m_loadingBar->SetValue(0.0f);

#ifdef KALYDO
	// reset the download counter so we know what the start value was before downloading.
	g_ruResourceManager->LoadingStart();
#endif // KALYDO
	/*
	lua_State* L = GetLuaState();
	if ( L ) {
		lua_pushstring(L, loadingFileName);
		lua_setglobal(L, "arg1");
		SendLoginEvent("LOADING_START");

		lua_pushstring(L, loadingFileName);
		lua_setglobal(L, "arg1");
		SendWorldEvent("LOADING_START");
	}	
	*/
}

// --------------------------------------------------------------------------------------
void CLoadingFrame::LoadingEnd(float delayTime)
{
	if ( m_isLoading ) {
		SetLoadingStatus(1.0f);
		m_isLoading = false;
		m_delayEndTime = max(delayTime, 0.1f);
	}
}

// --------------------------------------------------------------------------------------
void CLoadingFrame::SetLoadingStatus(float value)
{
	if ( m_isLoading ) {
		if ( m_loadingBar )
			m_loadingBar->SetValue(value);
	}	

	/*
	lua_State* L = GetLuaState();
	if ( m_isLoading && L ) {
		lua_pushnumber(L, value);
		lua_setglobal(L, "arg1");
		SendLoginEvent("LOADING_PROGRESS_POSITION");

		lua_pushnumber(L, value);
		lua_setglobal(L, "arg1");
		SendWorldEvent("LOADING_PROGRESS_POSITION");
	}
	*/
}

bool CLoadingFrame::FileExist(const char* pathName, string& outputString)
{
	// ¹w³]¹Ï§Î
	const char* list[4] = { ".dds", ".tga", ".png", ".bmp" };
	for ( int i = 0; i < 4; i++ )
	{
		outputString = pathName;
		outputString += list[i];

		if ( g_ruResourceManager->FileExists(outputString.c_str()) )
		{
			return true;
		}
	}

	outputString.clear();
	return false;
}

/*
// --------------------------------------------------------------------------------------
bool CLoadingFrame::GetLoadingState()
{
	if ( m_isLoading ) {
		// ¸ü¤J¤¤
		return true;
	} else if ( m_delayEndTime > 0.0f ) {
		// µ¥«Ý©µ“rµ²§ô
		return true;
	}
	// ¤w¸ü¤Jµ²§ô
	return false;
}
*/