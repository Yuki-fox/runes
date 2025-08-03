#pragma once

#include "uiframe.h"

// ----------------------------------------------------------------------------------
class CUiLuaStatusBar
{
public:
	static	void			OpenLib(lua_State* L);
	static	int				SetMetaTable(lua_State* L);
};

// ----------------------------------------------------------------------------------
class CUiStatusBar : public CUiFrame
{
public:
	CUiStatusBar(const char* name, CUiLayout* pParent, CUi* pUi);
	virtual ~CUiStatusBar();

	virtual	void			SetScrollParent(CUiLayout* layout);
	virtual	void			SetMaxValue(float maxValue);
	virtual	float			GetMaxValue()							{ return m_maxValue; }
	virtual	void			SetMinValue(float minValue);
	virtual	float			GetMinValue()							{ return m_minValue; }
	virtual	void			SetStepValue(float stepValue)			{ m_stepValue = stepValue; }
	virtual	float			GetStepValue()							{ return m_stepValue; }
	virtual	void			SetValue(float value)					{ SetValueInternal(value); }
	virtual	float			GetValue()								{ return m_value; }
	virtual	void			StepIt()								{ SetValueInternal(m_value + m_stepValue); }
	virtual	void			SetBarTexture(CUiTexture* texture);
	virtual	void			SetBarColor(CUiColor& color)			{ m_barColor = color; }
	virtual	CUiColor		GetBarColor()							{ return m_barColor; }
	virtual	void			SetLuaMetaTable(lua_State* l)			{ CUiLuaStatusBar::SetMetaTable(l); }
	virtual void			SetReverse(bool reverse);
	virtual int				GetMemSize();

protected:	
	virtual	void			OnUpdate(float elapsedTime);
	virtual void			OnPaint(float alpha);

	virtual	void			SetValueInternal(float value);
	virtual void			UpdateBarTexture();

protected:
	CUiTexture*				m_pBarTexture;
	CUiColor				m_barColor;
	float					m_minValue;
	float					m_maxValue;
	float					m_stepValue;
	float					m_value;
	bool					m_reverse;
};