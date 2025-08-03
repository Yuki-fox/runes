#pragma once

#include "uiframe.h"

// ----------------------------------------------------------------------------------
class CUiLuaScrollFrame
{
public:
	static	void			OpenLib(lua_State* L);
	static	int				SetMetaTable(lua_State* L);
};

// ----------------------------------------------------------------------------------
class CUiScrollFrame : public CUiFrame
{
public:
	CUiScrollFrame(const char* name, CUiLayout* pParent, CUi* pUi);
	virtual ~CUiScrollFrame();
	
	virtual	void			Update(float elapsedTime);
	virtual void			Render(float alpha);
	virtual CUiFrame*		GetFrameAtPoint(CUiPoint pt, CUiFrame* pExclude);
	virtual CUiFrame*		GetFrameAtKeyboard(CUiFrame* pExclude);
	virtual	void			SetScrollChild(CUiFrame* pFrame);
	virtual	void			SetVerticalScroll(float value);
	virtual	void			SetHorizontalScroll(float value);	
	virtual	void			SetScrollRange(float x, float y, bool forceUpdate=false);
	virtual	CUiPoint		GetScrollRange()						{ return m_scrollRange; }
	virtual	void			UpdateScrollChildRect();

	virtual	void			KillFocus(int flag = -1);

	// lua functions
	virtual	void			SetLuaMetaTable(lua_State* L)			{ CUiLuaScrollFrame::SetMetaTable(L); }	

	virtual int				GetMemSize();

protected:
	virtual	void			OnLoad();
	virtual	void			InsertChild(CUiFrame *pUiFrame);

protected:
	CUiFrame*				m_pScrollChild;
	CUiPoint				m_scrollRange;
};