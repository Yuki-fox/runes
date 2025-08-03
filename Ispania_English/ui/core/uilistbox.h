#pragma once

#include <list>
#include "uiframe.h"

#define	UI_LIST_INSERT_HEAD			0
#define	UI_LIST_INSERT_END			-1

using namespace std;

// ----------------------------------------------------------------------------------
class CUiLuaListBox
{
public:
	static	void			OpenLib(lua_State* L);
	static	int				SetMetaTable(lua_State* L);
};

// ----------------------------------------------------------------------------------
class CUiListBox : public CUiFrame
{
public:
	CUiListBox(const char* name, CUiLayout* pParent, CUi* pUi);
	virtual ~CUiListBox();	

	virtual	void			SetFontString(CUiFontString* fontString);
	virtual	void			SetCheckedTexture(CUiTexture* texture);
	virtual	void			SetSelectedTexture(CUiTexture* texture);
	virtual	void			ClearAllItem();
	virtual	void			ClearAllSelectedItem();
	virtual	bool			InsertItem(int pos, const WCHAR* text);
	virtual	bool			RemoveItem(int pos);
	virtual	int				GetListSize()							{ return (int)m_list.size(); }
	virtual	void			SetTopItem(int top);
	virtual	int				GetTopItem()							{ return m_topItem; }
	virtual	void			SetItemChecked(int item, bool isCheck);
	virtual	bool			IsItemChecked(int item);
	virtual	int				GetCurrentItem()						{ return m_currentItem; }
	virtual	void			SetItemHeight(float height)				{ m_itemHeight = height; }
	virtual	float			GetItemHeight()							{ return m_itemHeight; }
	virtual	void			SetTextInset(CUiRectangle& inset)		{ m_textInset = inset; }
	virtual	void			SetLuaMetaTable(lua_State* l)			{ CUiLuaListBox::SetMetaTable(l); }

protected:
	virtual	void			ClearAllItemFont();
	virtual	void			ResetAllItemTime();
	virtual	void			SetCurrentItem(int item);

	virtual	void			OnResize();
	virtual	void			OnUpdate(float elapsedTime);
	virtual	void			OnPaint(float alpha);

	virtual void			OnMouseMove(CUiPoint pt);
	virtual	void			OnClick(CUiPoint pt, int key);

protected:
	struct ListItem
	{
		WCHAR*			str;
		CUiFontString*	font;
		bool			isCheck;
		float			time;
	};

	list<ListItem>			m_list;
	CUiFontString*			m_fontString;	
	CUiTexture*				m_checkedTexture;			// 勾選項目
	CUiTexture*				m_selectedTexture;			// 移到項目
	CUiRectangle			m_textInset;				// 文字顯示區域
	float					m_fadeDuration;	
	float					m_itemHeight;
	int						m_topItem;
	int						m_currentItem;
};