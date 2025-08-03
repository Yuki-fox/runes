#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "../../tools/lua/lua.h"
#include "../../tools/lua/lauxlib.h"
#include "../../tools/lua/lualib.h"

#ifdef __cplusplus
};
#endif

#define	UI_LUA_POINTER					"_uilua.lightuserdate"
#define UI_LUA_TYPE_LAYOUT				"_uiLua.Layout"
#define UI_LUA_TYPE_FONTSTRING			"_uiLua.FontString"
#define UI_LUA_TYPE_TEXTURE				"_uiLua.Texture"
#define UI_LUA_TYPE_FRAME				"_uiLua.Frame"
#define UI_LUA_TYPE_MODEL				"_uiLua.Model"
#define UI_LUA_TYPE_PLAYERMODEL			"_uiLua.PlayerModel"
#define UI_LUA_TYPE_BUTTON				"_uiLua.Button"
#define UI_LUA_TYPE_CHECKBUTTON			"_uiLua.CheckButton"
#define UI_LUA_TYPE_RADIOBUTTON			"_uiLua.RadioButton"
#define UI_LUA_TYPE_EDITBOX				"_uiLua.EditBox"
#define UI_LUA_TYPE_LISTBOX				"_uiLua.ListBox"
#define UI_LUA_TYPE_STATUSBAR			"_uiLua.StatusBar"
#define UI_LUA_TYPE_SLIDER				"_uiLua.Slider"
#define UI_LUA_TYPE_MESSAGEFRAME		"_uiLua.MessageFrame"
#define UI_LUA_TYPE_SCROLLFRAME			"_uiLua.ScrollFrame"
#define UI_LUA_TYPE_TOOLTIP				"_uiLua.Tooltip"

#define UI_SAFE_DELETE(p)			if (p) { delete p; p = NULL; }
#define UI_SAFE_DELETE_ARRAY(p)		if (p) { delete [] p; p = NULL; }

void lua_pushstring (lua_State *L, const wchar_t *s);
char* WCharToUFT8(const wchar_t *s);

class CUiSize
{
public:	
	CUiSize()						{ m_x = 0.0f; m_y = 0.0f; }
	CUiSize(float x, float y)		{ m_x = x; m_y = y; }

	CUiSize			operator+(const CUiSize& src)	{ return CUiSize(m_x+src.m_x, m_y+src.m_y); }
	CUiSize			operator-(const CUiSize& src)	{ return CUiSize(m_x-src.m_x, m_y-src.m_y); }
	CUiSize			operator*(const CUiSize& src)	{ return CUiSize(m_x*src.m_x, m_y*src.m_y); }
	CUiSize			operator*(const float f)		{ return CUiSize(m_x*f, m_y*f);	}
	CUiSize			operator/(const CUiSize& src)	{ return CUiSize(m_x/src.m_x, m_y/src.m_y); }
	CUiSize			operator/(const float f)		{ return CUiSize(m_x/f, m_y/f);	}
	
	const CUiSize&	operator+=(const CUiSize& src)	{ m_x+=src.m_x; m_y+=src.m_y; return *this; }
	const CUiSize&	operator-=(const CUiSize& src)	{ m_x-=src.m_x; m_y-=src.m_y; return *this; }
	const CUiSize&	operator*=(const CUiSize& src)	{ m_x*=src.m_x; m_y*=src.m_y; return *this; }
	const CUiSize&	operator*=(const float f)		{ m_x*=f; m_y*=f; return *this; }
	const CUiSize&	operator/=(const CUiSize& src)	{ m_x/=src.m_x; m_y/=src.m_y; return *this; }
	const CUiSize&	operator/=(const float f)		{ m_x/=f; m_y/=f; return *this; }
	bool			operator==(const CUiSize& src)	{ return (m_x == src.m_x && m_y == src.m_y); }
	bool			operator!=(const CUiSize& src)	{ return !(*this == src); }

public:
	float m_x, m_y;
};

typedef CUiSize CUiPoint;

/*
class CUiPoint
{
public:	
	CUiPoint()										{ m_x = 0.0f; m_y = 0.0f; }
	CUiPoint(float x, float y)						{ m_x = x; m_y = y; }

	CUiPoint		operator+(const CUiPoint& src)	{ return CUiPoint(m_x+src.m_x, m_y+src.m_y); }
	CUiPoint		operator-(const CUiPoint& src)	{ return CUiPoint(m_x-src.m_x, m_y-src.m_y); }
	CUiPoint		operator*(const CUiPoint& src)	{ return CUiPoint(m_x*src.m_x, m_y*src.m_y); }
	CUiPoint		operator/(const CUiPoint& src)	{ return CUiPoint(m_x/src.m_x, m_y/src.m_y); }
	const CUiPoint&	operator+=(const CUiPoint& src)	{ m_x+=src.m_x; m_y+=src.m_y; return *this; }
	const CUiPoint&	operator-=(const CUiPoint& src)	{ m_x-=src.m_x; m_y-=src.m_y; return *this; }
	const CUiPoint&	operator*=(const CUiPoint& src)	{ m_x*=src.m_x; m_y*=src.m_y; return *this; }
	const CUiPoint&	operator/=(const CUiPoint& src)	{ m_x/=src.m_x; m_y/=src.m_y; return *this; }
	bool			operator==(const CUiPoint& src)	{ return (m_x == src.m_x && m_y == src.m_y); }
	bool			operator!=(const CUiPoint& src)	{ return !(*this == src); }

public:
	float m_x, m_y;
};
*/

class CUiRectangle
{
public:
	CUiRectangle()									{ m_left = 0.0f; m_top = 0.0f; m_right = 0.0f; m_bottom = 0.0f; }
	CUiRectangle(float left, float top, float right, float bottom)	{ m_left = left; m_top = top; m_right = right; m_bottom = bottom; }	

	void			Offset(float x, float y)		{ m_left+=x; m_right+=x; m_top+=y; m_bottom+=y; }
	void			Collect(CUiRectangle &rect)
	{
		m_left		= min(m_left, rect.m_left);
		m_top		= min(m_top, rect.m_top);
		m_right		= max(m_right, rect.m_right);
		m_bottom	= max(m_bottom, rect.m_bottom);
	}
	float			RectWidth()						{ return m_right - m_left; }
	float			RectHeight()					{ return m_bottom - m_top; }

public:
	float m_left, m_top, m_right, m_bottom;
};

class CUiRGBA
{
public:
	CUiRGBA()										{ m_r = 0; m_g = 0; m_b = 0; m_a = 0; }
	CUiRGBA(DWORD color)							{ m_value = color; }
	
	void			SetColor(DWORD color)			{ m_value = color; }
	void			SetAlpha(unsigned char a)		{ m_a = a; }
	void			SetAlpha(float a)				{ m_a = (unsigned char)(255*max(min(1.0f, a), 0.0f)); }
	void			SetAlphaScale(float s)			{ SetAlpha(m_a/255.0f * s); }

	union {
		struct {
			unsigned char m_b, m_g, m_r, m_a;
		};
		DWORD m_value;
	};
};

class CUiColor
{
public:
	CUiColor()										{ m_r = 0.0f; m_g = 0.0f; m_b = 0.0f; m_a = 0.0f; }
	CUiColor(DWORD color)							{ SetColor(color); }
	CUiColor(float r, float g, float b, float a)	{ m_r = r; m_g = g; m_b = b; m_a = a; }

	void			SetColor(DWORD color)			{
		CUiRGBA c(color);
		m_b = c.m_b/255.0f;
		m_g = c.m_g/255.0f;
		m_r = c.m_r/255.0f;
		m_a = c.m_a/255.0f;
	}

	DWORD			GetColor()						{
		CUiRGBA c;
		c.m_a = (unsigned char)(max(min(m_a, 1.0f), 0.0f) * 255);
		c.m_r = (unsigned char)(max(min(m_r, 1.0f), 0.0f) * 255);
		c.m_g = (unsigned char)(max(min(m_g, 1.0f), 0.0f) * 255);
		c.m_b = (unsigned char)(max(min(m_b, 1.0f), 0.0f) * 255);
		return c.m_value;
	}

	bool			operator==(const CUiColor& src)	{
		if (m_r == src.m_r && m_g == src.m_g && m_b == src.m_b && m_a == src.m_a )
			return true;
		return false;
	}

	bool			operator!=(const CUiColor& src)	{ return !(*this == src); }

	float m_r, m_g, m_b, m_a;
};