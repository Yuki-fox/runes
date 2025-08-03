#include <windows.h>
#include "uilistbox.h"

// ----------------------------------------------------------------------------------
CUiListBox::CUiListBox(const char* name, CUiLayout* pParent, CUi* pUi)
	: CUiFrame(name, pParent, pUi)
{
	m_uiType			= eUiType_ListBox;

	m_list.clear();
	m_fontString		= NULL;
	m_checkedTexture	= NULL;
	m_selectedTexture	= NULL;	
	m_fadeDuration		= 0.0f;
	m_topItem			= 0;
	m_itemHeight		= 20.0f;
	m_currentItem		= -1;
}

// ----------------------------------------------------------------------------------
CUiListBox::~CUiListBox()
{	
	UI_SAFE_DELETE(m_fontString);
	UI_SAFE_DELETE(m_checkedTexture);
	UI_SAFE_DELETE(m_selectedTexture);
	ClearAllItem();
}

// ----------------------------------------------------------------------------------
void CUiListBox::SetFontString(CUiFontString* fontString)
{
	UI_SAFE_DELETE(m_fontString);
	m_fontString = fontString;
}

// ----------------------------------------------------------------------------------
void CUiListBox::SetCheckedTexture(CUiTexture* texture)
{
	UI_SAFE_DELETE(m_checkedTexture);
	m_checkedTexture = texture;	
}

// ----------------------------------------------------------------------------------
void CUiListBox::SetSelectedTexture(CUiTexture* texture)
{
	UI_SAFE_DELETE(m_selectedTexture);
	m_selectedTexture = texture;
}

// ----------------------------------------------------------------------------------
void CUiListBox::ClearAllItem()
{
	for ( list<ListItem>::iterator iter = m_list.begin(); iter != m_list.end(); iter++ )
	{
		UI_SAFE_DELETE((*iter).font);
		UI_SAFE_DELETE_ARRAY((*iter).str);
	}
	m_list.clear();
	m_topItem = 0;
	m_currentItem = -1;
}

// ----------------------------------------------------------------------------------
// 清除所有被勾選項目
void CUiListBox::ClearAllSelectedItem()
{
	for ( list<ListItem>::iterator iter = m_list.begin(); iter != m_list.end(); iter++ )
	{
		(*iter).isCheck = false;
	}
}

// ----------------------------------------------------------------------------------
bool CUiListBox::InsertItem(int pos, const WCHAR* text)
{
	pos = max(0, (DWORD)pos);
	pos = min((DWORD)m_list.size(), (DWORD)pos);

	if (text)
	{
		ListItem item;
		item.font			= NULL;
		item.isCheck		= false;
		item.time			= 0.0f;
		item.str			= new WCHAR[wcslen(text) + 1];
		wcscpy(item.str, text);

		list<ListItem>::iterator iter = m_list.begin();
		for (int i = 0; i < pos; i++)
			iter++;

		m_list.insert(iter, item);
		return true;
	}
	return false;
}

// ----------------------------------------------------------------------------------
bool CUiListBox::RemoveItem(int pos)
{
	if ( pos >= 0 && pos < (int)m_list.size() )
	{
		list<ListItem>::iterator iter = m_list.begin();
		for (int i = 0; i < pos; i++)
			iter++;

		UI_SAFE_DELETE((*iter).font);
		UI_SAFE_DELETE_ARRAY((*iter).str);
		m_list.erase(iter);
		SetTopItem(m_topItem);
		SetCurrentItem(m_currentItem);
		return true;
	}
	return false;
}

// ----------------------------------------------------------------------------------
void CUiListBox::SetTopItem(int top)
{
	m_topItem = min(max(top, -1), (int)m_list.size() - 1);
}

// ----------------------------------------------------------------------------------
void CUiListBox::SetItemChecked(int item, bool isCheck)
{
	if (item >= 0 && item < (int)m_list.size())
	{
		list<ListItem>::iterator iter = m_list.begin();
		for (int i = 0; i < item; i++)
			iter++;

		(*iter).isCheck = isCheck;
	}
}

// ----------------------------------------------------------------------------------
bool CUiListBox::IsItemChecked(int item)
{
	if (item >= 0 && item < (int)m_list.size())
	{
		list<ListItem>::iterator iter = m_list.begin();
		for (int i = 0; i < item; i++)
			iter++;

		return (*iter).isCheck;
	}
	return false;
}

// ----------------------------------------------------------------------------------
void CUiListBox::ClearAllItemFont()
{
	for ( list<ListItem>::iterator iter = m_list.begin(); iter != m_list.end(); iter++ )
	{
		UI_SAFE_DELETE((*iter).font);
	}
}

// ----------------------------------------------------------------------------------
void CUiListBox::ResetAllItemTime()
{
	for ( list<ListItem>::iterator iter = m_list.begin(); iter != m_list.end(); iter++ )
	{
		(*iter).time = 0.0f;
	}
}

// ----------------------------------------------------------------------------------
void CUiListBox::SetCurrentItem(int item)
{
	m_currentItem = min(max(item, -1), (int)m_list.size() - 1);
}

// ----------------------------------------------------------------------------------
void CUiListBox::OnResize()
{
	ClearAllItemFont();
	ResetAllItemTime();
	CUiFrame::OnResize();
}

// ----------------------------------------------------------------------------------
void CUiListBox::OnUpdate(float elapsedTime)
{
	if ( m_fadeDuration > 0.0f )
	{
		for ( list<ListItem>::iterator iter = m_list.begin(); iter != m_list.end(); iter++ )
		{
			(*iter).time += elapsedTime;
		}
	}

	if(m_checkedTexture)
		m_checkedTexture->Update(elapsedTime);	
	if (m_selectedTexture)
		m_selectedTexture->Update(elapsedTime);

	CUiFrame::OnUpdate(elapsedTime);
}

// ----------------------------------------------------------------------------------
void CUiListBox::OnPaint(float alpha)
{
	if (m_fontString)
	{
		CUiSize size;
		CUiPoint screenPt = GetPaintPos();
		float y = screenPt.m_y;
		float width = (m_checkedTexture) ? m_itemHeight : 0.0f;
		size.m_x	= m_size.m_x - m_textInset.m_left + m_textInset.m_right;
		size.m_y	= m_size.m_y - m_textInset.m_top + m_textInset.m_bottom;

		list<ListItem>::iterator iter = m_list.begin();
		for (int i = 0; i < m_topItem; i++)
			iter++;

		for(int i = 0; iter != m_list.end(); i++, iter++ )
		{
			ListItem& item = (*iter);

			if (item.time > m_fadeDuration)
				break;

			if (size.m_y < y + m_itemHeight)
				break;

			if (item.font == NULL)
			{
				CUiColor color = m_fontString->GetColor();
				item.font = new CUiFontString(NULL, NULL, NULL);
				item.font->SetFontType(m_fontString->GetFontType());
				item.font->SetFontShadow(m_fontString->GetFontShadow());
				item.font->SetJustifyHType(m_fontString->GetJustifyHType());
				item.font->SetJustifyVType(m_fontString->GetJustifyVType());
				item.font->SetSpacing(m_fontString->GetSpacing());
				item.font->SetColor(color.m_r, color.m_g, color.m_b);
				item.font->SetAnchorSize(CUiSize(1.0f, 1.0f));
				item.font->SetSize(CUiSize(size.m_x - width, m_itemHeight), false);
				item.font->SetText(item.str);
			}

			item.font->Render(alpha, screenPt.m_x + m_textInset.m_left + width, y);
			if (item.isCheck && m_checkedTexture)
			{
				m_checkedTexture->SetSize(CUiSize(m_itemHeight, m_itemHeight));
				m_checkedTexture->Render(alpha, screenPt.m_x + m_textInset.m_left, y, 1.0f, 1.0f);
			}

			if (i == m_currentItem && m_selectedTexture)
			{
				m_selectedTexture->SetSize(CUiSize(m_size.m_x, m_itemHeight));
				m_selectedTexture->Render(alpha, screenPt.m_x, y, 1.0f, 1.0f);
			}
			y += m_itemHeight;
		}
	}
}

// ----------------------------------------------------------------------------------
void CUiListBox::OnMouseMove(CUiPoint pt)
{
	if ( ContainsPoint(pt) )
	{
		CUiPoint localPt = pt - GetPaintPos();

		if (m_itemHeight != 0.0f)
			SetCurrentItem(m_topItem + (int)(localPt.m_y / m_itemHeight));
	}

	CUiFrame::OnMouseMove(pt);
}

// ----------------------------------------------------------------------------------
void CUiListBox::OnClick(CUiPoint pt, int key)
{
	CUiPoint localPt = pt - GetPaintPos();

	if (m_itemHeight != 0.0f)
	{
		int item = m_topItem + (int)(localPt.m_y / m_itemHeight);
		SetItemChecked(item, !IsItemChecked(item));
	}
	CUiFrame::OnClick(pt, key);
}