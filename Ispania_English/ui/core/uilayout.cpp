#include <windows.h>
#include <math.h>
#include "uilayout.h"
#include "ui.h"

// ----------------------------------------------------------------------------------
CUiLayout::CUiLayout(const char* name, CUiLayout* pParent, CUi* pUi)
{
	m_uiType				= eUiType_Layout;
	m_pUi					= pUi;
	m_name					= NULL;
	m_pParent				= pParent;
	m_isVisible				= true;
	m_color.m_r				= 1.0f;
	m_color.m_g				= 1.0f;
	m_color.m_b				= 1.0f;
	m_color.m_a				= 1.0f;
	m_anchorSize.m_x		= -1.0f;
	m_anchorSize.m_y		= -1.0f;
	m_scale					= 1.0f;	
	
	m_scrollParent			= NULL;
	m_scrollOffset.m_x		= 0.0f;
	m_scrollOffset.m_y		= 0.0f;

	m_relative.clear();
	m_anchors.clear();	
	
	if (name)
	{
		if (m_pUi)
		{
			// 檢測名稱是否重復
			static char tmpStr[MAX_PATH];

			strcpy(tmpStr, name);
			int count = 1;
			while(m_pUi->FindObject(tmpStr) != NULL)
				sprintf(tmpStr, "%s%d", name, count++ );

			name = tmpStr;			
		}

		m_name = new char[strlen(name)+1];
		strcpy(m_name, name);

		if (m_pUi)
			m_pUi->InsertObject(this);
	}
}

// ----------------------------------------------------------------------------------
CUiLayout::~CUiLayout()
{	
	if (m_pUi)
		m_pUi->DeleteObject(this);

	UI_SAFE_DELETE_ARRAY(m_name);

	// 解除被自己關連元件
	for ( vector<CUiAnchor>::iterator iter = m_anchors.begin(); iter != m_anchors.end(); iter++ )
	{
		if ( (*iter).m_relativeTo )
			(*iter).m_relativeTo->DissolveRelative(this);
	}

	// 解除關連自己元件
	for ( set<CUiLayout*>::iterator iter = m_relative.begin(); iter != m_relative.end(); iter++ )
	{
		(*iter)->RemoveAnchorRelative(this);
	}

	m_scrollParent = NULL;
	m_relative.clear();
}

// ----------------------------------------------------------------------------------
void CUiLayout::SetScale(float scale)
{
	if ( m_scale != scale )
	{
		m_scale = max(scale, 0.0f);
		ResetAnchors();
	}
}

// ----------------------------------------------------------------------------------
float CUiLayout::GetRealScale()
{
	CUiLayout* parent = m_pParent;
	float scale = m_scale;
	while ( parent )
	{
		scale *= parent->m_scale;
		parent = parent->m_pParent;
	}
	return scale;
}

// ----------------------------------------------------------------------------------
void CUiLayout::SetSize(CUiSize size, bool isReAnchor)
{
	if (size != m_size)
	{
		m_size = size;		
		
		if (isReAnchor)
		{
			// 重新定位
			ResetAnchors();
		}
	}
}

// ----------------------------------------------------------------------------------
void CUiLayout::SetWidth(float width)
{
	SetSize( CUiSize(width, m_size.m_y) );
}

// ----------------------------------------------------------------------------------
void CUiLayout::SetHeight(float height)
{
	SetSize( CUiSize(m_size.m_x, height) );
}

// ----------------------------------------------------------------------------------
void CUiLayout::SetPos(CUiPoint& pos)
{
	if (pos != m_position)
	{
		m_position = pos;

		// 通知所有底下元件重新設定位置
		for ( set<CUiLayout*>::iterator iter = m_relative.begin(); iter != m_relative.end(); iter++ )
		{
			(*iter)->ResetSize();
			(*iter)->ResetAnchors();
		}
	}
}

// ----------------------------------------------------------------------------------
void CUiLayout::SetAnchorSize(CUiSize size)
{
	if ( m_anchorSize != size )
	{
		m_anchorSize = size;
		if ( m_anchorSize.m_x == 0.0f && m_anchorSize.m_y == 0.0f && m_pParent )
			SetSize(m_pParent->m_size);
		else
			SetSize(m_anchorSize);
	}
}

// ----------------------------------------------------------------------------------
void CUiLayout::SetAnchor(CUiAnchor anchor)
{
	if ( anchor.m_relativeTo == NULL || anchor.m_relativeTo == this )
		anchor.m_relativeTo = m_pParent;
	if ( anchor.m_relativeTo )
		anchor.m_relativeTo->EntangleRelative(this);
	m_anchors.push_back(anchor);

	// 重新定位
	ResetAnchors();
}

// ----------------------------------------------------------------------------------
void CUiLayout::ClearAllAnchors()
{
	for ( vector<CUiAnchor>::iterator iter = m_anchors.begin(); iter != m_anchors.end(); iter++ )
	{
		if ( (*iter).m_relativeTo )
			(*iter).m_relativeTo->DissolveRelative(this);
	}
	m_anchors.clear();
}

// ----------------------------------------------------------------------------------
CUiLayout* CUiLayout::FindByName(const char* name)
{
	if (m_pUi)
		return m_pUi->FindObject(name);
	return NULL;
}

// ----------------------------------------------------------------------------------
const char* CUiLayout::TransName(const char* name)
{	
	const char* key_word = "$parent";

	if (name == NULL)
		return NULL;

	if ( strncmp(name, key_word, strlen(key_word)) == 0 )
	{
		static char new_name[1024];
		const char* tmp = name + strlen(key_word);

		if (m_pParent && m_pParent->GetName())
		{
			strcpy(new_name, m_pParent->GetName());
			strcat(new_name, tmp);
		}
		else
			strcpy(new_name, tmp);
		return new_name;
	}
	return name;
}

// ----------------------------------------------------------------------------------
CUiLayout* CUiLayout::GetRelative()
{
	if ( !m_anchors.empty() && m_anchors[0].m_relativeTo )
		return m_anchors[0].m_relativeTo;
	return m_pParent;
}

// ----------------------------------------------------------------------------------
void CUiLayout::Moving(float x, float y)
{
	for ( vector<CUiAnchor>::iterator iter = m_anchors.begin(); iter != m_anchors.end(); iter++ )
	{
		CUiAnchor& anchor = *iter;
		(*iter).m_offset.m_x += x;
		(*iter).m_offset.m_y += y;
	}
	ResetAnchors();
}

// ----------------------------------------------------------------------------------
void CUiLayout::Sizing(int anchorPoint, float x, float y)
{
	CUiSize size = m_size;
	switch (anchorPoint)
	{
	case CUiAnchor::eAnchor_TopLeft:
		size.m_x -= x;
		size.m_y -= y;
		break;

	case CUiAnchor::eAnchor_Top:
		size.m_y -= y;
		break;

	case CUiAnchor::eAnchor_TopRight:
		size.m_x += x;
		size.m_y -= y;
		break;

	case CUiAnchor::eAnchor_Left:
		size.m_x -= x;
		break;

	case CUiAnchor::eAnchor_Right:
		size.m_x += x;
		break;

	case CUiAnchor::eAnchor_BottomLeft:
		size.m_x -= x;
		size.m_y += y;
		break;

	case CUiAnchor::eAnchor_Bottom:
		size.m_y += y;
		break;

	case CUiAnchor::eAnchor_BottomRight:
		size.m_x += x;
		size.m_y += y;
		break;
	}
	SetAnchorSize(size);
}

// ----------------------------------------------------------------------------------
void CUiLayout::GetRectangle(CUiRectangle* rect, bool scrollOffset)
{
	if ( rect )
	{
		CUiSize size = GetRealSize();
		if ( scrollOffset )
		{
			CUiPoint point = GetPaintPos();
			rect->m_left	= point.m_x;
			rect->m_top		= point.m_y;
		}
		else
		{
			rect->m_left	= m_position.m_x;
			rect->m_top		= m_position.m_y;
		}	
		rect->m_right	= rect->m_left + size.m_x;
		rect->m_bottom	= rect->m_top + size.m_y;
	}
}

// ----------------------------------------------------------------------------------
CUiPoint CUiLayout::GetPaintPos()
{	
	CUiPoint point = m_position;
	if ( m_scrollParent )
	{
		CUiLayout *parent = m_scrollParent;
		while (parent)
		{
			point -= parent->m_scrollOffset * parent->GetRealScale();
			parent = parent->m_scrollParent;
		}
	}
	return point;
}

// ----------------------------------------------------------------------------------
void CUiLayout::SetScrollParent(CUiLayout* layout)
{
	if ( layout != this )
		m_scrollParent = layout;
}

// ----------------------------------------------------------------------------------
CUiPoint CUiLayout::GetCenter()
{
	return ( m_position + GetRealSize() / 2 );
}

// ----------------------------------------------------------------------------------
int CUiLayout::GetMemSize()
{
	int size = sizeof(CUiLayout);

	size += sizeof(CUiLayout*) * (int)m_relative.size();

	if ( m_name )
		size += (int)(strlen(m_name) + 1) * sizeof(char);

	size += sizeof(CUiAnchor) * (int)m_anchors.size();	

	return size;
}

// ----------------------------------------------------------------------------------
lua_State* CUiLayout::GetLuaState()
{
	if (m_pUi)
		return m_pUi->GetLuaState();
	return NULL;
}

// ----------------------------------------------------------------------------------
void CUiLayout::ResetAnchors()
{
	CUiPoint pos;	

	if ( m_pParent == NULL )
	{
		pos.m_x = 0.0f;
		pos.m_y = 0.0f;
	}
	else if ( m_anchors.empty() )
	{
		pos = m_pParent->m_position;
	}
	else
	{
		CUiAnchor anchor1, anchor2;
		float scale = GetRealScale();

		anchor1.m_point			= m_anchors[0].GetPos();
		anchor1.m_relativePoint = m_anchors[0].GetRelativePoint();
		anchor1.m_offset		= m_anchors[0].m_offset;
		anchor1.m_relativeTo	= (m_anchors[0].m_relativeTo) ? m_anchors[0].m_relativeTo : m_pParent;

		// 若定位點超過一個,代表還包含大小設定
		if ( m_anchors.size() > 1 )
		{
			CUiSize size = m_size;
			float x1, x2, y1, y2;

			anchor2.m_point			= m_anchors[1].GetPos();
			anchor2.m_relativePoint = m_anchors[1].GetRelativePoint();
			anchor2.m_offset		= m_anchors[1].m_offset;
			anchor2.m_relativeTo	= (m_anchors[1].m_relativeTo) ? m_anchors[1].m_relativeTo : m_pParent;

			anchor1.m_relativeTo->GetAnchorPointPos(anchor1.m_relativePoint, anchor1.m_offset, x1, y1);
			anchor2.m_relativeTo->GetAnchorPointPos(anchor2.m_relativePoint, anchor2.m_offset, x2, y2);

			// 調整寬度
			if ( CUiAnchor::IsPairPoint(anchor1.m_point, anchor2.m_point, CUiAnchor::eAnchor_Left, CUiAnchor::eAnchor_Right) ||
				CUiAnchor::IsPairPoint(anchor1.m_point, anchor2.m_point, CUiAnchor::eAnchor_TopLeft, CUiAnchor::eAnchor_TopRight) ||
				CUiAnchor::IsPairPoint(anchor1.m_point, anchor2.m_point, CUiAnchor::eAnchor_BottomLeft, CUiAnchor::eAnchor_BottomRight) )
			{
				if ( scale > 0.0f )
					size.m_x = abs(x1 - x2) / scale;
				else
					size.m_x = 0.0f;
			}
			// 調整高度
			else if ( CUiAnchor::IsPairPoint(anchor1.m_point, anchor2.m_point, CUiAnchor::eAnchor_Top, CUiAnchor::eAnchor_Bottom) ||
				CUiAnchor::IsPairPoint(anchor1.m_point, anchor2.m_point, CUiAnchor::eAnchor_TopLeft, CUiAnchor::eAnchor_BottomLeft) ||
				CUiAnchor::IsPairPoint(anchor1.m_point, anchor2.m_point, CUiAnchor::eAnchor_TopRight, CUiAnchor::eAnchor_BottomRight) )
			{
				if ( scale > 0.0f )
					size.m_y = abs(y1 - y2) / scale;
				else
					size.m_y = 0.0f;
			}
			// 調整寬及高度
			else if ( CUiAnchor::IsPairPoint(anchor1.m_point, anchor2.m_point, CUiAnchor::eAnchor_TopLeft, CUiAnchor::eAnchor_BottomRight) ||
				CUiAnchor::IsPairPoint(anchor1.m_point, anchor2.m_point, CUiAnchor::eAnchor_BottomRight, CUiAnchor::eAnchor_TopLeft) )
			{
				if ( scale > 0.0f )
				{
					size.m_x = abs(x1 - x2) / scale;
					size.m_y = abs(y1 - y2) / scale;
				}
				else
				{
					size.m_x = 0.0f;
					size.m_y = 0.0f;
				}
			}

			SetSize( size, false );
		}

		// 取得真實大小
		CUiSize relativeSize = anchor1.m_relativeTo->m_size * anchor1.m_relativeTo->GetRealScale();

		// 定位
		pos.m_x = ((anchor1.m_relativePoint - 1) % 3) * (relativeSize.m_x / 2) - ((anchor1.m_point - 1) % 3) * (m_size.m_x * scale / 2);
		pos.m_y = ((anchor1.m_relativePoint - 1) / 3) * (relativeSize.m_y / 2) - ((anchor1.m_point - 1) / 3) * (m_size.m_y * scale / 2);

		/*
		// 定位		
		pos.m_x = ((anchor1.m_relativePoint - 1) % 3) * (anchor1.m_relativeTo->m_size.m_x / 2) - ((anchor1.m_point - 1) % 3) * (m_size.m_x / 2);
		pos.m_y = ((anchor1.m_relativePoint - 1) / 3) * (anchor1.m_relativeTo->m_size.m_y / 2) - ((anchor1.m_point - 1) / 3) * (m_size.m_y / 2);
		*/

		//pos += (anchor1.m_offset * scale) + anchor1.m_relativeTo->m_position - (m_scrollOffset * scale);
		pos += (anchor1.m_offset * scale) + anchor1.m_relativeTo->m_position;
	}

	SetPos( pos );

	// 通知所有底下元件重新設定位置
	for ( set<CUiLayout*>::iterator iter = m_relative.begin(); iter != m_relative.end(); iter++ )
	{
		(*iter)->ResetSize();
		(*iter)->ResetAnchors();
	}
}

// ----------------------------------------------------------------------------------
void CUiLayout::GetAnchorPointPos(int point, CUiPoint offset, float &x, float &y)
{
	float scale = GetRealScale();
	x = ((point - 1) % 3) * (m_size.m_x * scale / 2) + m_position.m_x + offset.m_x * scale;
	y = ((point - 1) / 3) * (m_size.m_y * scale / 2) + m_position.m_y + offset.m_y * scale;
}

// ----------------------------------------------------------------------------------
void CUiLayout::ResetSize()
{
	if ( m_anchorSize.m_x == 0.0f && m_anchorSize.m_y == 0.0f && m_pParent )
		SetSize( m_pParent->m_size, false );
}

// ----------------------------------------------------------------------------------
void CUiLayout::RemoveAnchorRelative(CUiLayout* pUiLayout)
{	
	for ( vector<CUiAnchor>::iterator iter = m_anchors.begin(); iter != m_anchors.end(); iter++ )
	{
		if ( (*iter).m_relativeTo == pUiLayout )
			(*iter).m_relativeTo = NULL;
	}
}

// ----------------------------------------------------------------------------------
void CUiLayout::DissolveRelative(CUiLayout* pUiLayout)
{
	set<CUiLayout*>::iterator iter = m_relative.find(pUiLayout);
	if ( iter != m_relative.end() )
	{
		m_relative.erase(iter);
	}
}

// ----------------------------------------------------------------------------------
void CUiLayout::EntangleRelative(CUiLayout* pUiLayout)
{
	if (pUiLayout)
	{
		m_relative.insert(pUiLayout);
	}
}