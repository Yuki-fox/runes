#define _WIN32_WINNT 0x0500
#define _WIN32_WINDOWS 0x0400

#include <windows.h>
#include "../../netwaker_member/NetWakerClientInc.h"
#include "../../tools/getfontnamefromfile/getfontnamefromfile.h"
#include "../../mainproc/GameMain.h"
#include "BotCheckFrame.h"

CBotCheckFrame*				g_pBotCheckFrame = NULL;

// ----------------------------------------------------------------------------
CBotCheckFrame::CBotCheckFrame(CInterface* object)
	: CInterfaceBase(object)
{
	m_fontType				= NULL;
	m_botCheckFrame			= NULL;
	m_explainText			= NULL;
	m_checkSerialText		= NULL;
	m_editbox				= NULL;
	m_buttonOkey			= NULL;

	g_pBotCheckFrame = this;
}

// ----------------------------------------------------------------------------
CBotCheckFrame::~CBotCheckFrame()
{
	m_fontType				= NULL;

	g_pBotCheckFrame		= NULL;
}

// ----------------------------------------------------------------------------
void CBotCheckFrame::RegisterFunc()
{
	lua_State* L = GetLuaStateWorld();
	if (L)
	{
	}
}

// ----------------------------------------------------------------------------
void CBotCheckFrame::BuildFrames()
{
	CUi* pUi = m_interface->GetUiWorld();
	CUiAnchor anchor;
	CTextureBackdrop backdrop;
	CRuRectangle rect;
	RuFontShadow fontShadow;

	fontShadow.x = (short)1.0f;
	fontShadow.y = (short)1.0f;
	fontShadow.color = 0xff000000;

	if ( pUi )
	{		
		/*
		// 取得字型類型
		string resource = m_interface->GetResourcePath();
		string fullname = resource + g_pGameMain->GetDefaultFontPath();
		AddFontResourceEx( fullname.c_str(), FR_PRIVATE, 0 );

		string fontname = GetFontNameFromFile( fullname.c_str() );
		if ( !fontname.empty() ) {
			m_fontType = CRuFontType::GetFontType(fontname.c_str(), 16, ruFONTWEIGHT_NORMAL, ruFONTOUTLINE_NORMAL);
		}

		// 底板
		m_botCheckFrame = new CUiFrame("BotCheckFrame", pUi->GetRoot(), pUi);
		backdrop.SetEdgeSize(16.0f);
		backdrop.SetTileSize(16.0f);
		backdrop.SetEdgeFileName("Interface\\Tooltips\\Tooltip-border");
		backdrop.SetTileFileName("Interface\\Tooltips\\Tooltip-background");
		backdrop.SetBackgroundOffset(CRuRectangle(4.0f, 4.0f, 4.0f, 4.0f));
		m_botCheckFrame->SetFrameLevel(-1);
		m_botCheckFrame->SetAlpha(1.0f);
		m_botCheckFrame->SetBackdrop(&backdrop);
		m_botCheckFrame->SetAnchorSize(CUiSize(380.0f, 72.0f));
		anchor.Clear();
		anchor.m_relativeTo = NULL;
		anchor.m_point = CUiAnchor::eAnchor_Center;
		anchor.m_offset.m_x = 0.0f;
		anchor.m_offset.m_y = 0.0f;
		m_botCheckFrame->SetAnchor(anchor);

		// 說明文字
		m_explainText = new CUiFontString(NULL, m_botCheckFrame, pUi);		
		m_explainText->SetBytes(511);
		m_explainText->SetJustifyVType(CUiFontString::eJustifyV_Top);
		m_explainText->SetJustifyHType(CUiFontString::eJustifyH_Center);
		m_explainText->SetFontType(m_fontType);
		m_explainText->SetFontShadow(&fontShadow);
		m_explainText->SetSpacing(0.0f);
		m_explainText->SetAnchorSize(CUiSize(340.0f, 0.0f));
		anchor.Clear();
		anchor.m_relativeTo = m_botCheckFrame;
		anchor.m_point = CUiAnchor::eAnchor_Top;
		anchor.m_offset.m_x = 0.0f;
		anchor.m_offset.m_y = 16.0f;
		m_explainText->SetAnchor(anchor);
		m_explainText->SetTextA(g_ObjectData->GetString("SYS_CHECKBOT_EXPLAIN_TEXT"));
		m_botCheckFrame->SetLayers(CUiFrame::eLayerType_Artwork, m_explainText);

		// 認証文字
		m_checkSerialText = new CUiFontString(NULL, m_botCheckFrame, pUi);
		m_checkSerialText->SetBytes(511);
		m_checkSerialText->SetJustifyVType(CUiFontString::eJustifyV_Top);
		m_checkSerialText->SetJustifyHType(CUiFontString::eJustifyH_Right);
		m_checkSerialText->SetFontType(m_fontType);
		m_checkSerialText->SetFontShadow(&fontShadow);
		m_checkSerialText->SetSpacing(0.0f);
		m_checkSerialText->SetAnchorSize(CUiSize(0.0f, 0.0f));
		anchor.Clear();
		anchor.m_relativeTo = m_botCheckFrame;
		anchor.m_point = CUiAnchor::eAnchor_BottomRight;
		anchor.m_relativePoint = CUiAnchor::eAnchor_Bottom;
		anchor.m_offset.m_x = -8.0f;
		anchor.m_offset.m_y = -48.0f;
		m_checkSerialText->SetAnchor(anchor);
		m_botCheckFrame->SetLayers(CUiFrame::eLayerType_Artwork, m_checkSerialText);

		// 輸入文字
		m_editbox = new CUiEditBox(NULL, m_botCheckFrame, pUi);		
		anchor.Clear();
		anchor.m_relativeTo = m_botCheckFrame;
		anchor.m_point = CUiAnchor::eAnchor_BottomLeft;
		anchor.m_relativePoint = CUiAnchor::eAnchor_Bottom;
		anchor.m_offset.m_x = 8.0f;
		anchor.m_offset.m_y = -44.0f;
		m_editbox->SetMouseEnable(true);
		m_editbox->SetKeyboardEnable(true);
		m_editbox->SetAnchor(anchor);
		m_editbox->SetLetters(16);
		m_editbox->SetNumeric(false);
		m_editbox->SetPassword(false);
		m_editbox->SetMultiMode(false);
		pFontString = new CUiFontString(NULL, m_editbox, pUi);
		pFontString->SetFontType(m_fontType);
		m_editbox->SetFontString(pFontString);
		m_editbox->SetAnchorSize(CUiSize(120.0f, 24.0f));		

		// 確定按鈕
		m_buttonOkey = new CUiButton(NULL, m_botCheckFrame, pUi);
		pFontString = new CUiFontString(NULL, m_buttonOkey, pUi);
		pFontString->SetFontType(m_fontType);
		pFontString->SetJustifyVType(CUiFontString::eJustifyV_Top);
		pFontString->SetJustifyHType(CUiFontString::eJustifyH_Center);
		pFontString->SetAnchorSize(CUiSize(0.0f, 0.0f));
		anchor.Clear();
		anchor.m_point = CUiAnchor::eAnchor_Center;
		anchor.m_offset.m_x = -1.0f;
		anchor.m_offset.m_y = -1.0f;
		pFontString->SetAnchor(anchor);
		m_buttonOkey->SetNormalText(pFontString);

		rect.m_top = 0.0f;
		rect.m_left = 0.0f;
		rect.m_bottom = 0.75f;
		rect.m_right = 0.6875f;

		pTexture = new CUiTexture(NULL, m_buttonOkey, pUi);
		pTexture->SetFile("Interface\\Buttons\\PanelButton-Normal");		
		pTexture->SetTexCoords(1, &rect);
		pTexture->SetAnchor(CUiAnchor());
		pTexture->SetAnchorSize(CUiSize(0.0f, 0.0f));
		m_buttonOkey->SetNormalTexture(pTexture);

		pTexture = new CUiTexture(NULL, m_buttonOkey, pUi);
		pTexture->SetFile("Interface\\Buttons\\PanelButton-Depress");
		pTexture->SetTexCoords(1, &rect);
		pTexture->SetAnchor(CUiAnchor());
		pTexture->SetAnchorSize(CUiSize(0.0f, 0.0f));
		m_buttonOkey->SetPushedTexture(pTexture);
		
		pTexture = new CUiTexture(NULL, m_buttonOkey, pUi);
		pTexture->SetFile("Interface\\Buttons\\PanelButton-Highlight");		
		pTexture->SetAnchor(CUiAnchor());
		pTexture->SetAnchorSize(CUiSize(0.0f, 0.0f));
		rect.m_right = 0.671875f;
		rect.m_bottom = 0.6875f;
		pTexture->SetTexCoords(1, &rect);
		pTexture->SetAlphaMode(ALPHA_MODE_ADD);
		m_buttonOkey->SetHighlightTexture(pTexture);

		m_buttonOkey->SetPushTextOffset(CUiPoint(1.0f, 1.0f));
		m_buttonOkey->SetAnchorSize(CUiSize(92.0f, 24.0f));
		anchor.Clear();
		anchor.m_point = CUiAnchor::eAnchor_BottomRight;
		anchor.m_offset.m_x = -8.0f;
		anchor.m_offset.m_y = -8.0f;
		m_buttonOkey->SetMouseEnable(true);
		m_buttonOkey->SetAnchor(anchor);
		m_buttonOkey->SetTextA(g_ObjectData->GetString("SYS_OKAY"));
		*/
	}
}

// ----------------------------------------------------------------------------
void CBotCheckFrame::DestroyFrames()
{
	m_fontType				= NULL;
	m_botCheckFrame			= NULL;
	m_explainText			= NULL;
	m_checkSerialText		= NULL;
	m_editbox				= NULL;
	m_buttonOkey			= NULL;
}

// ----------------------------------------------------------------------------
void CBotCheckFrame::Open()
{
	if ( m_botCheckFrame )
	{
		string registerText = g_ObjectData->GetString("SYS_REGISTER_TEXT");
		char checkText[10];
		float height = 84.0f;

		memset(checkText, 0, sizeof(checkText));
		for ( int i = 0; i < 6; i++ )
		{
			checkText[i] = '0' + rand() % 10;
		}
		registerText += checkText;

		m_checkSerialText->SetTextA(registerText.c_str());

		height += m_explainText->GetHeight();
		m_botCheckFrame->SetHeight(height);
		m_botCheckFrame->SetVisible(true);
	}
}

// ----------------------------------------------------------------------------
void CBotCheckFrame::Close()
{
}

// ----------------------------------------------------------------------------
void CBotCheckFrame::CheckText()
{
	if ( m_botCheckFrame == NULL )
		return;

	const wchar_t* editText = m_editbox->GetText();
	const wchar_t* fontText = m_checkSerialText->GetText();

	if ( wcscmp(editText, fontText) != 0 )
	{
		// 輸入錯誤
		return;
	}

	m_botCheckFrame->SetVisible(false);	
}
