#pragma once

#include "uiframe.h"

// ----------------------------------------------------------------------------------
class CUiLuaSlider
{
public:
	static	void			OpenLib(lua_State* L);
	static	int				SetMetaTable(lua_State* L);
};

// ----------------------------------------------------------------------------------
class CUiSlider : public CUiFrame
{
public:
	CUiSlider(const char* name, CUiLayout* pParent, CUi* pUi);
	virtual ~CUiSlider();

	enum Orientation {
		eOrientation_Vertical	= 1,
		eOrientation_Horizontal	= 2,
	};

	enum ValueStepMode {
		eValueStep_Int			= 1,
		eValueStep_Float		= 2,
	};

	virtual void			SetMouseEnable(bool isEnable)			{ m_state.isMouseEnable = true; }
	virtual	void			SetOrientation(Orientation orientation)	{ m_orientation = orientation; }
	virtual	Orientation		GetOrientation()						{ return m_orientation; }
	virtual	void			SetThumbTexture(CUiTexture* texture);
	virtual	void			SetMaxValue(float maxValue);
	virtual	float			GetMaxValue()							{ return m_maxValue; }
	virtual	void			SetMinValue(float minValue);
	virtual	float			GetMinValue()							{ return m_minValue; }
	virtual	void			SetValue(float value)					{ SetValueInternal(value); }
	virtual	float			GetValue()								{ return m_value; }
	virtual	void			SetValueStepMode(ValueStepMode mode)	{ m_valueStepMode = mode; }
	virtual	ValueStepMode	GetValueStepMode()						{ return m_valueStepMode; }
	virtual	void			SetLuaMetaTable(lua_State* l)			{ CUiLuaSlider::SetMetaTable(l); }
	virtual int				GetMemSize();

protected:
	virtual	void			OnUpdate(float elapsedTime);
	virtual void			OnPaint(float alpha);
	virtual	void			OnMouseDown(CUiPoint pt, int key);
	virtual	void			OnMouseUp(CUiPoint pt, int key);
	virtual	void			OnMouseMove(CUiPoint pt);

	virtual	void			SetValueInternal(float value, bool force=false);
	virtual	float			ValueFromPos(CUiPoint& pt);
	virtual	bool			GetThumbRect(CUiRectangle &rect);

protected:
	CUiTexture*				m_pThumbTexture;
	Orientation				m_orientation;
	ValueStepMode			m_valueStepMode;
	float					m_value;	
	float					m_maxValue;
	float					m_minValue;

	CUiPoint				m_pressedOffset;
	bool					m_isThumbPressed;
};

