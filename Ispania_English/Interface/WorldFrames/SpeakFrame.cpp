#define _WIN32_WINNT 0x0500
#define _WIN32_WINDOWS 0x0400

#include <windows.h>
#include "../../errorlist/errorlist.h"
#include "../../mainproc/GameMain.h"
#include "../../tools/getfontnamefromfile/getfontnamefromfile.h"
#include "SpeakFrame.h"

CSpeakFrame*				g_pSpeakFrame = NULL;

// ----------------------------------------------------------------------------
CSpeakFrame::CSpeakFrame(CInterface* object)
	: CInterfaceBase(object)
{
	m_fontType			= NULL;
	m_fontShadow.color	= 0xFF000000;
	m_fontShadow.x		= 1;
	m_fontShadow.y		= 1;

	m_data.clear();

	g_pSpeakFrame		= this;
}

// ----------------------------------------------------------------------------
CSpeakFrame::~CSpeakFrame()
{
	DestroyFrames();
}

// ----------------------------------------------------------------------------
void CSpeakFrame::BuildFrames()
{
	if ( m_interface && m_interface->GetUiWorld() )
	{
		string resource = m_interface->GetResourcePath();
		string fullname = resource + g_pGameMain->GetDefaultFontPath();
		AddFontResourceEx( fullname.c_str(), FR_PRIVATE, 0 );

		string fontname = GetFontNameFromFile( fullname.c_str() );
		if ( !fontname.empty() ) {
			m_fontType = CRuFontType::GetFontType(fontname.c_str(), 16, ruFONTWEIGHT_BOLD, ruFONTOUTLINE_NORMAL);
		}
	}
}

// ----------------------------------------------------------------------------
void CSpeakFrame::DestroyFrames()
{
	for ( list<SpeakInfo>::iterator iter = m_data.begin(); iter != m_data.end(); iter++ )
		UI_SAFE_DELETE(iter->frame);
	m_data.clear();
}

// ----------------------------------------------------------------------------
void CSpeakFrame::Update(float elapsedTime)
{
	list<SpeakInfo>::iterator iter = m_data.begin();
	while ( iter != m_data.end() )
	{
		iter->time -= elapsedTime;
		if ( iter->time < 0.0f )
		{
			CRoleSprite* sprite = g_pGameMain->FindSpriteByID(iter->id);
			if ( sprite )
			{
				g_pGameMain->ResetSpriteNameVisible(sprite);
			}			
			UI_SAFE_DELETE(iter->frame);
			iter = m_data.erase(iter);
		} else {
			iter++;
		}
	}
}

// ----------------------------------------------------------------------------
void CSpeakFrame::Render()
{
	CRuCamera* pCamera = g_pGameMain->GetCamera();
	for ( list<SpeakInfo>::iterator iter = m_data.begin(); iter != m_data.end(); iter++ ) {
		CRoleSprite* sprite = g_pGameMain->FindSpriteByID(iter->id);

		if ( sprite == NULL )
			continue;
		else if ( pCamera ) {
			CRuVector3 cameraPos, spritePos;

			sprite->GetAttachPos(eAttach_Name_Title, spritePos);
			cameraPos = spritePos;
			pCamera->GetCameraTransform().TransformPoint(cameraPos);
			if ( cameraPos.m_z > 0.0f ) {
				CUiPoint point;

				// Z軸為距離深度,可用以做縮放比例

				// 轉換為2D座標
				g_pGameMain->WorldToViewport(spritePos);
				point.m_x = spritePos.m_x - (iter->frame->GetWidth() / 2.0f);
				point.m_y = spritePos.m_y - iter->frame->GetHeight();
				iter->frame->SetPos(point);
				iter->frame->SetVisible(true);

				if ( iter->time < 1.0f )
					iter->frame->SetAlpha(iter->time);
			} else {
				// 超出畫面
				iter->frame->SetVisible(false);
			}
		}
	}
}

// ----------------------------------------------------------------------------
void CSpeakFrame::AddText(int id, const char* text, float r, float g, float b)
{
	CRoleSprite* sprite = g_pGameMain->FindSpriteByID(id);
	if ( sprite && text )
	{
		CTextureBackdrop backdrop;
		SpeakInfo info;		
		CUiAnchor anchor;
		float width;

		if ( strncmp(text, "[$MUTE]", 7) == 0 ) {
			text = text + 7;
		}

		sprite->SetNameVisibleForce(false);
		for ( list<SpeakInfo>::iterator iter = m_data.begin(); iter != m_data.end(); iter++ )
		{
			if ( iter->id == id )
			{
				iter->time = 5.0f;
				iter->font->SetAnchorSize(CUiSize(0.0f, 0.0f));
				iter->font->SetColor(r, g, b);
				iter->font->SetTextA(text);
				iter->frame->SetAlpha(1.0f);

				width = iter->font->GetWidth();
				if ( width > 240.0f )
				{
					iter->font->SetAnchorSize(CUiSize(240.0f, 0.0f));
				}

				iter->frame->SetWidth(iter->font->GetWidth() + 24.0f);
				iter->frame->SetHeight(iter->font->GetHeight() + 24.0f);
				return;
			}
		}

		CUi* ui		= m_interface->GetUiWorld();
		info.id		= id;
		info.time	= 5.0f;
		info.frame	= new CUiFrame(NULL, ui->GetRoot(), ui);

		backdrop.SetEdgeSize(16.0f);
		backdrop.SetTileSize(16.0f);
		backdrop.SetEdgeFileName("Interface\\Tooltips\\SayBubble-Border");
		backdrop.SetTileFileName("Interface\\Tooltips\\SayBubble-Background");
		backdrop.SetBackgroundOffset(CRuRectangle(3.0f, 3.0f, 3.0f, 3.0f));
		info.frame->SetFrameLevel(-1);
		info.frame->SetAlpha(1.0f);
		info.frame->SetBackdrop(&backdrop);
		info.frame->SetAnchorSize(CUiSize(100.0f, 100.0f));

		CUiTexture* texture = new CUiTexture(NULL, info.frame, ui);
		texture->SetFile("Interface\\Tooltips\\SayBubble-Tail");
		texture->SetAnchorSize(CUiSize(16.0f, 16.0f));
		anchor.m_point			= CUiAnchor::eAnchor_Top;
		anchor.m_relativePoint	= CUiAnchor::eAnchor_Bottom;
		anchor.m_relativeTo		= NULL;
		anchor.m_offset.m_x		= 0.0f;
		anchor.m_offset.m_y		= -6.0f;
		texture->SetAnchor(anchor);
		texture->SetScale(1.0f);
		info.frame->SetLayers(CUiFrame::eLayerType_Background, texture);

		info.font	= new CUiFontString(NULL, info.frame, ui);
		info.font->SetBytes(511);
		info.font->SetJustifyVType(CUiFontString::eJustifyV_Top);

		if ( strcmp(g_pGameMain->GetLanguage(), "AE") == 0  )
		{
			info.font->SetJustifyHType(CUiFontString::eJustifyH_Right);
		}
		else
		{
			info.font->SetJustifyHType(CUiFontString::eJustifyH_Left);
		}
		
		info.font->SetFontType(m_fontType);
		info.font->SetFontShadow(&m_fontShadow);
		info.font->SetSpacing(0.0f);
		info.font->SetAnchorSize(CUiSize(0.0f, 0.0f));
		anchor.m_point			= CUiAnchor::eAnchor_Bottom;
		anchor.m_relativePoint	= CUiAnchor::eAnchor_Null;
		anchor.m_relativeTo		= NULL;
		anchor.m_offset.m_x		= 0.0f;
		anchor.m_offset.m_y		= -12.0f;
		info.font->SetAnchor(anchor);
		info.font->SetColor(r, g, b);
		info.font->SetAlpha(1.0f);
		info.font->SetTextA(text);

		info.frame->SetLayers(CUiFrame::eLayerType_Artwork, info.font);
		width = info.font->GetWidth();
		if ( width > 240.0f )
		{
			info.font->SetAnchorSize(CUiSize(240.0f, 0.0f));
		}
		info.frame->SetWidth(info.font->GetWidth() + 24.0f);
		info.frame->SetHeight(info.font->GetHeight() + 24.0f);

		// 判斷座標,是否顯示
		{
			CRuCamera* pCamera = g_pGameMain->GetCamera();
			CRuVector3 cameraPos;

			sprite->GetAttachPos(eAttach_Name_Title, cameraPos);
			pCamera->GetCameraTransform().TransformPoint(cameraPos);
			if ( cameraPos.m_z > 0.0f ) {
				info.frame->SetVisible(true);
			} else {
				info.frame->SetVisible(false);
			}
		}		

		m_data.push_back(info);
	}
}

