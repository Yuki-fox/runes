#include <windows.h>
#include "ui.h"
//#include "uitooltip.h"

#define	MAX_TOOLTIP_LINES				6

// ----------------------------------------------------------------------------------
CUiTooltip::CUiTooltip(const char* name, CUiLayout* pParent, CUi* pUi)
	: CUiFrame(name, pParent, pUi)
{
	m_uiType			= eUiType_Tooltip;

	m_pOwner			= NULL;
	m_lines				= 0;
	m_isRTL				= false;
	m_maxWidth			= 0.0f;

	m_separator.clear();
	memset(m_warp, 0, sizeof(m_warp));
}

// ----------------------------------------------------------------------------------
CUiTooltip::~CUiTooltip()
{
}

// ----------------------------------------------------------------------------------
void CUiTooltip::ClearLines()
{
	CUiFontString *str;
	char tmp[256];

	for ( int i = 0; i < m_lines; i++ )
	{
		sprintf( tmp, "%sTextLeft%d", m_name, i+1 );
		str = (CUiFontString*)m_pUi->FindObject(tmp);
		if ( str )
			str->SetVisible(false);

		sprintf( tmp, "%sTextRight%d", m_name, i+1 );
		str = (CUiFontString*)m_pUi->FindObject(tmp);
		if ( str )
			str->SetVisible(false);
	}

	CUiTexture* separator;
	for ( int i = 0; i < MAX_TOOLTIP_LINES; i++ )
	{
		sprintf( tmp, "%sSeparator%d", m_name, i+1 );
		separator = (CUiTexture*)m_pUi->FindObject(tmp);
		if ( separator )
			separator->SetVisible(false);
	}

	CUiTexture* texture;
	sprintf( tmp, "%sTierBackground", m_name );
	texture = (CUiTexture*)m_pUi->FindObject(tmp);
	if ( texture )
		texture->SetVisible(false);
	sprintf( tmp, "%sTierBarFill", m_name );
	texture = (CUiTexture*)m_pUi->FindObject(tmp);
	if ( texture )
		texture->SetVisible(false);

	m_maxWidth = 0.0f;
	m_lines = 0;
	m_separator.clear();
	memset(m_warp, 0, sizeof(m_warp));
}

// ----------------------------------------------------------------------------------
void CUiTooltip::AddDoubleLine(const wchar_t* textL, const wchar_t* textR, float rL, float gL, float bL, float rR, float gR, float bR )
{
	CUiFontString *strL, *strR;
	char tmp[256];

	if ( textR == NULL || textR[0] == 0 )
	{
		AddLine(textL, rL, gL, bL);
		return;
	}

	sprintf( tmp, "%sTextLeft%d", m_name, m_lines+1 );
	strL = (CUiFontString*)m_pUi->FindObject(tmp);
	sprintf( tmp, "%sTextRight%d", m_name, m_lines+1 );
	strR = (CUiFontString*)m_pUi->FindObject(tmp);
	if ( strL && strR )
	{		
		float width = 40.0f;
		strL->SetAnchorSize(CUiSize());
		strR->SetAnchorSize(CUiSize());

		Anchor(strL, m_lines);

		m_lines++;
		strL->SetColor(rL, gL, bL);
		strL->SetText(textL);
		strL->SetVisible(true);
		strR->SetColor(rR, gR, bR);
		strR->SetText(textR);
		strR->SetVisible(true);		
		width += strL->GetWidth() + strR->GetWidth();

		if ( width <= m_maxWidth )
		{
			CUiAnchor anchor;
			anchor.m_relativeTo		= strL;

			if ( m_isRTL )
			{
				anchor.m_point			= CUiAnchor::eAnchor_TopRight;
				anchor.m_relativePoint	= CUiAnchor::eAnchor_TopLeft;
				anchor.m_offset.m_y		= 0.0f;
				anchor.m_offset.m_x		= -(m_maxWidth - (width - 40.0f));
			}
			else
			{
				anchor.m_point			= CUiAnchor::eAnchor_TopLeft;
				anchor.m_relativePoint	= CUiAnchor::eAnchor_TopRight;
				anchor.m_offset.m_y		= 0.0f;
				anchor.m_offset.m_x		= m_maxWidth - (width - 40.0f);
			}
			
			strR->ClearAllAnchors();
			strR->SetAnchor(anchor);
		}
		else
		{
			if ( m_isRTL )
			{
				CUiAnchor anchor;
				anchor.m_relativeTo = strL;
				anchor.m_offset.m_x = -40;
				anchor.m_offset.m_y = 0;
				anchor.m_relativePoint = CUiAnchor::eAnchor_TopRight;
				anchor.m_relativePoint = CUiAnchor::eAnchor_TopLeft;
				strR->ClearAllAnchors();
				strR->SetAnchor(anchor);
			}
			else
			{
				CUiAnchor anchor;
				anchor.m_relativeTo = strL;
				anchor.m_offset.m_x = -40;
				anchor.m_offset.m_y = 0;
				anchor.m_relativePoint = CUiAnchor::eAnchor_TopLeft;
				anchor.m_relativePoint = CUiAnchor::eAnchor_TopRight;
				strR->ClearAllAnchors();
				strR->SetAnchor(anchor);
			}
		}
		
		TooltipUpdate(width);
	}	
}

// ----------------------------------------------------------------------------------
void CUiTooltip::AddDoubleLine(const char* textL, const char* textR, float rL, float gL, float bL, float rR, float gR, float bR )
{
	WCHAR* tmpL = MultiByteToWChar(textL);
	WCHAR* tmpR = MultiByteToWChar(textR);
	AddDoubleLine(tmpL, tmpR, rL, gL, bL, rR, gR, bR);
	UI_SAFE_DELETE_ARRAY(tmpL);
	UI_SAFE_DELETE_ARRAY(tmpR);
}

// ----------------------------------------------------------------------------------
void CUiTooltip::AddLine(const wchar_t* text, float r, float g, float b, bool warp, int tier)
{	
	CUiFontString* strL;	
	char tmp[256];

	sprintf( tmp, "%sTextLeft%d", m_name, m_lines+1 );
	strL = (CUiFontString*)m_pUi->FindObject(tmp);
	if ( strL )
	{
		float width = 0.0f;
		float maxWidth = m_maxWidth;

		m_warp[m_lines] = warp;
		if ( warp )
			strL->SetAnchorSize(CUiSize(m_maxWidth, 0.0f));
		else
			strL->SetAnchorSize(CUiSize());

		// 重新定位
		Anchor(strL, m_lines);

		m_lines++;
		strL->SetColor(r, g, b);
		strL->SetText(text);
		strL->SetVisible(true);

		width = strL->GetWidth();		

		// 若超過合理值,則改為最大值
		if ( width > 226.0f )
		{
			width = max(m_maxWidth, 226.0f);
			strL->SetAnchorSize(CUiSize(width, 0.0f));

			// 第一行以實際顯示最大寬度
			if ( m_maxWidth == 0.0f )
			{
				float displayWidth = strL->GetDisplayWidth();
				if ( width - displayWidth > 10.0f )
				{
					width = displayWidth;
					strL->SetAnchorSize(CUiSize(width, 0.0f));
				}
			}
		}

		if ( m_lines == 1 && tier > 0 )
		{
			CUiTexture* texture;
			sprintf( tmp, "%sTierBackground", m_name );
			texture = (CUiTexture*)m_pUi->FindObject(tmp);
			if ( texture )
			{
				texture->SetVisible(true);
				if ( m_isRTL )
				{
					CUiAnchor anchor;					
					anchor.m_point = CUiAnchor::eAnchor_TopLeft;
					anchor.m_relativePoint = CUiAnchor::eAnchor_TopLeft;
					anchor.m_offset.m_x = 8;
					anchor.m_offset.m_y = 8;
					texture->ClearAllAnchors();
					texture->SetAnchor(anchor);
				}
				else 
				{
					CUiAnchor anchor;					
					anchor.m_point = CUiAnchor::eAnchor_TopRight;
					anchor.m_offset.m_x = -8;
					anchor.m_offset.m_y = 8;
					texture->ClearAllAnchors();
					texture->SetAnchor(anchor);
				}
			}

			sprintf( tmp, "%sTierBarFill", m_name );
			texture = (CUiTexture*)m_pUi->FindObject(tmp);
			if ( texture )
			{
				CRuRectangle coords;
				coords.m_top	= 0.0f;
				coords.m_bottom = 1.0f;
				coords.m_left	= 0.0f;
				coords.m_right	= ((float)tier / 15.0f) * 0.625f;
				
				texture->SetAnchorSize(CUiSize(128.0f * coords.m_right, 16.0f));
				texture->SetTexCoords(1, &coords);
				texture->SetVisible(true);
			}
			width += 40.0f + 80.0f;
		}		

		// 重新計算
		TooltipUpdate(width);
	}
}

// ----------------------------------------------------------------------------------
void CUiTooltip::AddLine(const char* text, float r, float g, float b, bool warp, int tier)
{
	WCHAR* tmp = MultiByteToWChar(text);
	AddLine(tmp, r, g, b, warp, tier);
	UI_SAFE_DELETE_ARRAY(tmp);
}

// ----------------------------------------------------------------------------------
void CUiTooltip::AddSeparator()
{
	CUiTexture* separator;
	char tmp[256];
	sprintf( tmp, "%sSeparator%d", m_name, m_separator.size() + 1 );
	separator = (CUiTexture*)m_pUi->FindObject(tmp);
	if ( separator )
	{
		Anchor(separator, m_lines);

		separator->SetVisible(true);
		separator->SetAnchorSize(CUiSize(m_maxWidth, 4.0f));
		m_separator.insert(make_pair(m_lines, m_separator.size() + 1));
		m_lines++;
	}
}

// ----------------------------------------------------------------------------------
void CUiTooltip::SetText(const wchar_t* text, float r, float g, float b, bool warp)
{
	ClearLines();
	AddLine(text, r, g, b, warp);
}

// ----------------------------------------------------------------------------------
void CUiTooltip::SetText(const char* utext, float r, float g, float b, bool warp)
{
	ClearLines();
	AddLine(utext, r, g, b, warp);
}

// ----------------------------------------------------------------------------------
void CUiTooltip::SetOwner(CUiLayout* pUiLayout, int point, float xOff, float yOff)
{
	m_pOwner = pUiLayout;
	if ( pUiLayout )
	{
		CUiAnchor anchor;
		ClearAllAnchors();
		if ( point < CUiAnchor::eAnchor_TopLeft || point > CUiAnchor::eAnchor_BottomRight )
			point = CUiAnchor::eAnchor_TopLeft;

		anchor.m_relativeTo		= pUiLayout;
		anchor.m_offset.m_x		= xOff;
		anchor.m_offset.m_y		= yOff;
		anchor.m_relativePoint	= point;
		anchor.m_point			= 10 - point;

		SetAnchor(anchor);
	}
}

// ----------------------------------------------------------------------------------
int CUiTooltip::GetMemSize()
{
	int size = CUiFrame::GetMemSize() + sizeof(CUiTooltip) - sizeof(CUiFrame);

	CUiLayout* pLayout = NULL;

	char tmp[256];
	for ( map<int, int>::iterator iter = m_separator.begin(); iter != m_separator.end(); iter++ )
	{
		sprintf( tmp, "%sSeparator%d", m_name, iter->second );
		pLayout = m_pUi->FindObject(tmp);
		if ( pLayout )
			size += pLayout->GetMemSize();
	}	

	for ( int i = 0; i < 100; i++ )
	{
		sprintf( tmp, "%sTextLeft%d", m_name, i );
		pLayout = m_pUi->FindObject(tmp);
		if ( pLayout )
			size += pLayout->GetMemSize();

		sprintf( tmp, "%sTextRight%d", m_name, i );
		pLayout = m_pUi->FindObject(tmp);
		if ( pLayout )
			size += pLayout->GetMemSize();
	}

	return size;
}

// ----------------------------------------------------------------------------------
void CUiTooltip::OnHide(lua_State* L)
{
	SetOwner(NULL, 0, 0.0f, 0.0f);

	CUiFrame::OnHide(L);
}

// ----------------------------------------------------------------------------------
CUiLayout* CUiTooltip::GetLine(int index, bool right)
{
	char tmp[256];
	map<int, int>::iterator iter = m_separator.find(index - 1);
	if ( iter != m_separator.end() )
	{
		sprintf( tmp, "%sSeparator%d", m_name, iter->second );
		return m_pUi->FindObject(tmp);
	}
	else
	{
		if ( right )
			sprintf( tmp, "%sTextRight%d", m_name, index );
		else
			sprintf( tmp, "%sTextLeft%d", m_name, index );
		return m_pUi->FindObject(tmp);
	}
}

// ----------------------------------------------------------------------------------
void CUiTooltip::Anchor(CUiLayout* layout, int index)
{
	layout->ClearAllAnchors();

	CUiAnchor anchor;
	if ( index == 0 )
	{
		anchor.m_relativeTo		= this;
		if ( m_isRTL )
		{
			anchor.m_point			= CUiAnchor::eAnchor_TopRight;
			anchor.m_relativePoint	= CUiAnchor::eAnchor_TopRight;
			anchor.m_offset.m_x		= -10;
			anchor.m_offset.m_y		= 10;
		}
		else
		{
			anchor.m_point			= CUiAnchor::eAnchor_TopLeft;
			anchor.m_relativePoint	= CUiAnchor::eAnchor_TopLeft;
			anchor.m_offset.m_x		= 10;
			anchor.m_offset.m_y		= 10;
		}
	}
	else
	{
		anchor.m_relativeTo		= GetLine(index);
		anchor.m_offset.m_x		= 0;
		anchor.m_offset.m_y		= 2;

		if ( m_isRTL )
		{
			anchor.m_point			= CUiAnchor::eAnchor_TopRight;
			anchor.m_relativePoint	= CUiAnchor::eAnchor_BottomRight;
		}
		else
		{
			anchor.m_point			= CUiAnchor::eAnchor_TopLeft;
			anchor.m_relativePoint	= CUiAnchor::eAnchor_BottomLeft;
		}
	}
	layout->SetAnchor(anchor);
}

// ----------------------------------------------------------------------------------
void CUiTooltip::TooltipUpdate(float width)
{
	if ( m_lines <= 0 )
		return;

	CUiFontString *strL, *strR;
	float scale = GetRealScale();
	char tmp[256];

	if ( width > m_maxWidth )
	{
		m_maxWidth = width;

		int separatorCount = 0;
		for ( int i = 0; i < m_lines; i++ )
		{
			map<int, int>::iterator iter = m_separator.find(i);
			if ( iter != m_separator.end() )
			{
				CUiLayout* separator = GetLine(i+1);
				if ( separator )
				{
					separator->SetAnchorSize(CUiSize(m_maxWidth, 4.0f));
				}
			}
			else
			{
				sprintf( tmp, "%sTextLeft%d", m_name, i+1 );
				strL = (CUiFontString*)m_pUi->FindObject(tmp);
				sprintf( tmp, "%sTextRight%d", m_name, i+1 );
				strR = (CUiFontString*)m_pUi->FindObject(tmp);

				if ( strR->IsVisible() )
				{
					CUiAnchor anchor;
					anchor.m_relativeTo		= strL;					

					if ( m_isRTL )
					{
						anchor.m_point			= CUiAnchor::eAnchor_Right;
						anchor.m_relativePoint	= CUiAnchor::eAnchor_Left;
						anchor.m_offset.m_y		= 0.0f;
						anchor.m_offset.m_x		= -(m_maxWidth - strL->GetWidth() - strR->GetWidth());
					}
					else
					{
						anchor.m_point			= CUiAnchor::eAnchor_Left;
						anchor.m_relativePoint	= CUiAnchor::eAnchor_Right;
						anchor.m_offset.m_y		= 0.0f;
						anchor.m_offset.m_x		= m_maxWidth - strL->GetWidth() - strR->GetWidth();
					}					
					
					strR->ClearAllAnchors();
					strR->SetAnchor(anchor);
				}
				else if ( m_warp[i] )
				{
					strL->SetAnchorSize(CUiSize(m_maxWidth, 0.0f));
				}
			}
		}
	}

	float top, bottom;
	CUiLayout *line1 = GetLine(1);
	CUiLayout *line2 = GetLine(m_lines);

	top = line1->GetPos().m_y;
	bottom = line2->GetPos().m_y + line2->GetHeight() * scale;

	/*
	sprintf( tmp, "%sTextLeft%d", m_name, m_lines );
	strL = (CUiFontString*)m_pUi->FindObject(tmp);
	sprintf( tmp, "%sTextRight%d", m_name, m_lines );
	strR = (CUiFontString*)m_pUi->FindObject(tmp);
	if ( strL )
		bottom = strL->GetPos().m_y + strL->GetHeight() * scale;
	if ( strR && strR->IsVisible() )
	{
		float height = strL->GetPos().m_y + strL->GetHeight() * scale;
		if ( height > bottom )
			bottom = height;
	}
	*/

	SetSize( CUiSize(m_maxWidth + 20.0f, (bottom - top) / scale + 20.0f) );
}