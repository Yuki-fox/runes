#pragma once

#include <string>
#include <set>
#include <map>
#include <vector>
#include "uibase.h"

using namespace std;

#define UI_LUA_ANCHOR_TOPLEFT			"TOPLEFT"
#define UI_LUA_ANCHOR_TOP				"TOP"
#define UI_LUA_ANCHOR_TOPRIGHT			"TOPRIGHT"
#define UI_LUA_ANCHOR_LEFT				"LEFT"
#define UI_LUA_ANCHOR_CENTER			"CENTER"
#define UI_LUA_ANCHOR_RIGHT				"RIGHT"
#define UI_LUA_ANCHOR_BOTTOMLEFT		"BOTTOMLEFT"
#define UI_LUA_ANCHOR_BOTTOM			"BOTTOM"
#define UI_LUA_ANCHOR_BOTTOMRIGHT		"BOTTOMRIGHT"

class CUiLayout;
// ----------------------------------------------------------------------------------
class CUiAnchor
{
public:
	CUiAnchor()					{ m_relativeTo = NULL; m_point = eAnchor_Null; m_relativePoint = eAnchor_Null; }
	~CUiAnchor()				{ m_relativeTo = NULL; m_point = eAnchor_Null; m_relativePoint = eAnchor_Null; }

	enum {
		eAnchor_Null					= 0,
		eAnchor_TopLeft					= 1,
		eAnchor_Top						= 2,
		eAnchor_TopRight				= 3,
		eAnchor_Left					= 4,
		eAnchor_Center					= 5,
		eAnchor_Right					= 6,
		eAnchor_BottomLeft				= 7,
		eAnchor_Bottom					= 8,
		eAnchor_BottomRight				= 9
	};	

	static	bool				IsPairPoint(int p1, int p2, int v1, int v2)
	{
		if ( p1 == v1 && p2 == v2 )
			return true;
		else if ( p1 == v2 && p2 == v1 )
			return true;
		return false;
	}

	static	int					GetAnchorPoint(const char* point)
	{
		if ( strcmp(point, UI_LUA_ANCHOR_TOPLEFT) == 0 )
			return eAnchor_TopLeft;
		else if ( strcmp(point, UI_LUA_ANCHOR_TOP) == 0 )
			return eAnchor_Top;
		else if ( strcmp(point, UI_LUA_ANCHOR_TOPRIGHT) == 0 )
			return eAnchor_TopRight;
		else if ( strcmp(point, UI_LUA_ANCHOR_LEFT) == 0 )
			return eAnchor_Left;
		else if ( strcmp(point, UI_LUA_ANCHOR_CENTER) == 0 )
			return eAnchor_Center;
		else if ( strcmp(point, UI_LUA_ANCHOR_RIGHT) == 0 )
			return eAnchor_Right;
		else if ( strcmp(point, UI_LUA_ANCHOR_BOTTOMLEFT) == 0 )
			return eAnchor_BottomLeft;
		else if ( strcmp(point, UI_LUA_ANCHOR_BOTTOM) == 0 )
			return eAnchor_Bottom;
		else if ( strcmp(point, UI_LUA_ANCHOR_BOTTOMRIGHT) == 0 )
			return eAnchor_BottomRight;
		return eAnchor_Null;
	}

	static const char*			GetAnchorPointStr(int point)
	{
		switch (point)
		{
		case eAnchor_TopLeft:
			return UI_LUA_ANCHOR_TOPLEFT;
		case eAnchor_Top:
			return UI_LUA_ANCHOR_TOP;
		case eAnchor_TopRight:
			return UI_LUA_ANCHOR_TOPRIGHT;
		case eAnchor_Left:
			return UI_LUA_ANCHOR_LEFT;
		case eAnchor_Center:
			return UI_LUA_ANCHOR_CENTER;
		case eAnchor_Right:
			return UI_LUA_ANCHOR_RIGHT;
		case eAnchor_BottomLeft:
			return UI_LUA_ANCHOR_BOTTOMLEFT;
		case eAnchor_Bottom:
			return UI_LUA_ANCHOR_BOTTOM;
		case eAnchor_BottomRight:
			return UI_LUA_ANCHOR_BOTTOMRIGHT;
		}
		return NULL;
	}

	int							GetPos()							{ return (m_point == eAnchor_Null) ? eAnchor_TopLeft : m_point; }
	int							GetRelativePoint()					{ return (m_relativePoint == eAnchor_Null) ? GetPos() : m_relativePoint; }	
	void						Clear()								{
		m_relativeTo = NULL;
		m_offset.m_x = 0.0f;
		m_offset.m_y = 0.0f;
		m_point = eAnchor_Null;
		m_relativePoint = eAnchor_Null;
	}

public:
	CUiLayout*		m_relativeTo;
	CUiPoint		m_offset;	
	int				m_point;
	int				m_relativePoint;
};

// ----------------------------------------------------------------------------------
// Lua functions
class CUiLuaLayout
{
public:
	static	void			OpenLib(lua_State* L);
	static	CUiLayout*		CheckLayoutTable(lua_State* L, int num);
	static	int				SetMetaTable(lua_State* L);
	static	int				NewTable(lua_State* L, CUiLayout* layout);	
	static	void			DeleteTable(lua_State* L, CUiLayout* layout);
	static	void			PushLayoutTable(lua_State* L, CUiLayout* layout);

	static	CUiAnchor		GetAnchor(lua_State* L, CUiLayout* layout);
	static	CUiColor		GetColor(lua_State* L, int top);
	static	CUiRectangle	GetRectangle(lua_State* L, int top);
};

class CUi;
// ----------------------------------------------------------------------------------
class CUiLayout
{
public:
	CUiLayout(const char* name, CUiLayout* pParent, CUi* pUi);
	virtual ~CUiLayout();

	enum {
		eUiType_Layout				= 0,
		eUiType_FontString			,
		eUiType_Texture				,
		eUiType_Frame				,
		eUiType_Model				,
		eUiType_Button				,
		eUiType_CheckButton			,
		eUiType_RadioButton			,
		eUiType_StatusBar			,
		eUiType_Slider				,
		eUiType_EditBox				,
		eUiType_ListBox				,
		eUiType_MessageFrame		,
		eUiType_ScrollFrame			,
		eUiType_Tooltip,
	};	

	// virtual function	
	virtual	void			Update(float elapsedTime)				{}
	virtual	void			Render(float alpha)						{}
	virtual	int				GetUiType()								{ return m_uiType; }
	virtual	const char*		GetName()								{ return m_name; }
	virtual CUiLayout*		GetParent()								{ return m_pParent; }
	virtual void			SetVisible(bool visible)				{ m_isVisible = visible; }
	virtual bool			IsVisible(bool checkParent=false)		{ return m_isVisible; }
	virtual	void			SetScale(float scale);
	virtual	float			GetScale()								{ return m_scale; }
	virtual	float			GetRealScale();
	virtual void			SetSize(CUiSize size, bool isReAnchor=true);
	virtual	CUiSize			GetSize()								{ return m_size; }
	virtual	CUiSize			GetRealSize()							{ return m_size * GetRealScale(); }
	virtual	void			SetWidth(float width);
	virtual	float			GetWidth()								{ return m_size.m_x; }
	virtual	void			SetHeight(float height);
	virtual	float			GetHeight()								{ return m_size.m_y; }
	virtual void			SetPos(CUiPoint& pos);
	virtual	CUiPoint		GetPos()								{ return m_position; }	
	virtual	void			SetAnchorSize(CUiSize size);
	virtual CUiSize			GetAnchorSize()							{ return m_anchorSize; }
	virtual	void			SetAnchor(CUiAnchor anchor);
	virtual	void			ClearAllAnchors();
	virtual int				GetAnchorsSize()						{ return (int)m_anchors.size(); }
	virtual CUiAnchor&		GetAnchor(int index)					{ return m_anchors[index]; }		
	virtual	CUiLayout*		FindByName(const char* name);
	virtual	const char*		TransName(const char* name);
	virtual CUiLayout*		GetRelative();
	virtual	void			SetColor(float r, float g, float b)
	{ 
		m_color.m_r = max(0.0f, min(1.0f, r));
		m_color.m_g = max(0.0f, min(1.0f, g));
		m_color.m_b = max(0.0f, min(1.0f, b));
	}

	virtual	CUiColor		GetColor()								{ return m_color; }
	virtual	void			SetAlpha(float a)						{ m_color.m_a = max(0.0f, min(1.0f, a)); }
	virtual	float			GetAlpha()								{ return m_color.m_a; }
	virtual	void			Moving(float x, float y);
	virtual	void			Sizing(int anchorPoint, float x, float y);	
	virtual	void			GetRectangle(CUiRectangle* rect, bool scrollOffset=true);
	virtual	CUiPoint		GetPaintPos();
	virtual	void			SetScrollParent(CUiLayout* layout);
	virtual void			SetScrollOffset(CUiPoint& offset)		{ m_scrollOffset = offset; }
	virtual CUiPoint		GetScrollOffset()						{ return m_scrollOffset; }
	virtual	CUi*			GetUi()									{ return m_pUi; }

	virtual	CUiPoint		GetCenter();
	virtual int				GetMemSize();

	// lua
	virtual	lua_State*		GetLuaState();
	virtual	void			SetLuaMetaTable(lua_State* l)			{ CUiLuaLayout::SetMetaTable(l); }
protected:
	virtual void			ResetAnchors();
	virtual void			GetAnchorPointPos(int point, CUiPoint offset, float &x, float &y);
	virtual void			ResetSize();
	virtual void			RemoveAnchorRelative(CUiLayout* pUiLayout);
	virtual	void			DissolveRelative(CUiLayout* pUiLayout);
	virtual	void			EntangleRelative(CUiLayout* pUiLayout);

protected:	
	set<CUiLayout*>			m_relative;
	int						m_uiType;
	CUi*					m_pUi;
	char*					m_name;						// 元件名稱
	bool					m_isVisible;				// 隱藏或顯示
	CUiLayout*				m_pParent;					// 父元件	

	vector<CUiAnchor>		m_anchors;
	CUiSize					m_anchorSize;
	CUiSize					m_size;
	CUiPoint				m_position;	
	CUiColor				m_color;
	float					m_scale;					// 縮放值

	bool					m_scrollState;				// 捲動狀態(是否為捲動頁面子元件)
	CUiLayout*				m_scrollParent;				// 捲動視窗父元件
	CUiPoint				m_scrollOffset;
};