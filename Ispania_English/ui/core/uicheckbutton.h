#pragma once

#include "uibutton.h"

// --------------------------------------------------------------------------------------
class CUiLuaCheckButton
{
public:
	static	void			OpenLib(lua_State* L);
	static	int				SetMetaTable(lua_State* L);
};

// --------------------------------------------------------------------------------------
class CUiCheckButton : public CUiButton
{
public:
	CUiCheckButton(const char* name, CUiLayout* pParent, CUi* pUi);
	virtual ~CUiCheckButton();	

	virtual	void			SetScrollParent(CUiLayout* layout);
	virtual	void			SetCheckedTexture(CUiTexture* texture);
	virtual	void			SetDisabledCheckedTexture(CUiTexture* texture);
	virtual void			SetAutoChecked(bool isAuto)				{ m_isAutoCheck = isAuto; }
	virtual bool			GetAutoChecked()						{ return m_isAutoCheck; }
	virtual	void			SetChecked(bool isCheck);
	virtual	bool			IsChecked()								{ return m_isChecked; }
	virtual	CUiTexture*		GetCheckedTexture()						{ return m_checkedTexture; }
	virtual	CUiTexture*		GetDisabledCheckedTexture()				{ return m_disabledCheckedTexture; }
	virtual	void			SetLuaMetaTable(lua_State* l)			{ CUiLuaCheckButton::SetMetaTable(l); }
	virtual int				GetMemSize();

protected:	

	virtual	void			OnUpdate(float elapsedTime);
	virtual	void			OnPaint(float alpha);
	virtual	void			OnClick(CUiPoint pt, int key);

protected:
	CUiTexture*				m_checkedTexture;
	CUiTexture*				m_disabledCheckedTexture;
	bool					m_isAutoCheck;
	bool					m_isChecked;
};