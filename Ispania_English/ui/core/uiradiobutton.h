#pragma once

#include "uicheckbutton.h"

// ----------------------------------------------------------------------------------
class CUiLuaRadioButton
{
public:
	static	void			OpenLib(lua_State* L);
	static	int				SetMetaTable(lua_State* L);
};

// ----------------------------------------------------------------------------------
class CUiRadioButton : public CUiCheckButton
{
public:
	CUiRadioButton(const char* name, CUiLayout* pParent, CUi* pUi);
	virtual ~CUiRadioButton();

	virtual	void			SetChecked(bool isCheck);
	virtual	void			SetGroupId(int id)						{ m_groupId = id; }
	virtual	int				GetGroupId()							{ return m_groupId; }
	virtual	void			SetLuaMetaTable(lua_State* l)			{ CUiLuaRadioButton::SetMetaTable(l); }
	virtual int				GetMemSize();

protected:
	virtual	void			OnClick(CUiPoint pt, int key);

protected:
	int						m_groupId;
};

