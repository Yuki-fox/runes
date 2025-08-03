#include <windows.h>
#include "sprite.h"
#include "../ErrorList/errorlist.h"

//-----------------------------------------------------------------------------
CTextSprite::CTextSprite()
{
	m_classType			= eSpriteClass_Text;

	m_pRuWidget = NULL;
	m_updateTime = 0.0f;
}

//-----------------------------------------------------------------------------
CTextSprite::~CTextSprite()
{
	ruSAFE_RELEASE( m_pRuWidget );
}

//-----------------------------------------------------------------------------
void CTextSprite::Update(float elapsedTime)
{
	if ( m_updateTime < 1.0f )
	{
	}
	else if ( m_updateTime < 4.0f )
	{
		SetPos(m_nowPos + CRuVector3(0.0f, 3.0f, 0.0f) * elapsedTime);
	}
	else
	{
		if ( !m_isDestruct )
			SetDestructTime( 1.0f );
	}

	if ( m_pRuWidget )
	{
		m_pRuWidget->SetTranslation( m_nowPos );
		m_pRuWidget->Update( elapsedTime );
	}
	m_updateTime += elapsedTime;	
	IBaseSprite::Update( elapsedTime );
}

//-----------------------------------------------------------------------------
void CTextSprite::Render()
{
	if ( m_pRuWidget )
		g_ruDefaultRenderContext->Submit( m_pRuWidget );
}

//-----------------------------------------------------------------------------
void CTextSprite::ReceiveDestruct( IBaseSprite* pSprite )
{
}

//-----------------------------------------------------------------------------
void CTextSprite::SetText(const char* text, float scale, int color)
{
	if (text == NULL)
		return;

	ruSAFE_RELEASE( m_pRuWidget );
	if ( (m_pRuWidget = ruNEW CRuEntity_Widget_Label()) != NULL )
	{
		WCHAR* tmp;
		vector<CRuRectangle> stock;

		tmp = TransToWChar(text);		

		vector<wstring> strList;
		CRuFontString::SplitString(tmp, (int)wcslen(tmp), strList);

		for ( vector<wstring>::iterator iter = strList.begin(); iter != strList.end(); iter++ )
		{
			CRuFontString* fontString = CRuFontString::GetFontString(s_pFontType, iter->c_str(), (int)iter->length());
			if ( fontString )
			{
				CRuFontTexture2* fontTexture = fontString->GetFontTexture();
				if ( fontTexture )
				{
					stock.push_back( fontTexture->GetRectangle() );
				}			
			}
		}

		if ( !stock.empty() )
		{
			INT32 row = m_pRuWidget->GetRowCount();
			m_pRuWidget->InsertGlyph(row, (INT32)stock.size(), &stock[0], scale, color, CRuFontTexture2::GetRuTextureD3D());
		}

		delete [] tmp;
	}
}