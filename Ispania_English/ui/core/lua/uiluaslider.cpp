#include <windows.h>
#include "../uislider.h"

int Slider_SetMaxValue(lua_State* L);
int Slider_GetMaxValue(lua_State* L);
int Slider_SetMinValue(lua_State* L);
int Slider_GetMinValue(lua_State* L);
int Slider_SetMinMaxValues(lua_State* L);
int Slider_GetMinMaxValues(lua_State* L);
int Slider_SetValue(lua_State* L);
int Slider_GetValue(lua_State* L);
int Slider_SetValueStepMode(lua_State* L);
int Slider_GetValueStepMode(lua_State* L);

// ----------------------------------------------------------------------------------
static const struct luaL_reg g_sliderTable [] = {
	{"SetMaxValue",			Slider_SetMaxValue},
	{"GetMaxValue",			Slider_GetMaxValue},
	{"SetMinValue",			Slider_SetMinValue},
	{"GetMinValue",			Slider_GetMinValue},
	{"SetMinMaxValues",		Slider_SetMinMaxValues},
	{"GetMinMaxValues",		Slider_GetMinMaxValues},
	{"SetValue",			Slider_SetValue},
	{"GetValue",			Slider_GetValue},
	{"SetValueStepMode",	Slider_SetValueStepMode},
	{"GetValueStepMode",	Slider_GetValueStepMode},
	{NULL, NULL}
};

// ----------------------------------------------------------------------------------
void CUiLuaSlider::OpenLib(lua_State* L)
{
	luaL_newmetatable(L, UI_LUA_TYPE_SLIDER);			// create new metatable
	luaL_getmetatable(L, UI_LUA_TYPE_FRAME);			// inherit layout
	lua_setmetatable(L, -2);
	lua_pushstring(L, "__index");
	lua_pushvalue(L, -2);
	lua_settable(L, -3);
	luaL_openlib(L, NULL, g_sliderTable, 0);
}

// ----------------------------------------------------------------------------------
int	CUiLuaSlider::SetMetaTable(lua_State* L)
{
	if (lua_istable(L, -1))
	{
		luaL_getmetatable(L, UI_LUA_TYPE_SLIDER);
		lua_setmetatable(L, -2);
		return 1;
	}
	return 0;
}

// ----------------------------------------------------------------------------------
int Slider_SetMaxValue(lua_State* L)
{
	CUiSlider* slider = dynamic_cast<CUiSlider*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (slider == NULL)
		lua_error(L);

	slider->SetMaxValue((float)luaL_checknumber(L, 2));
	return 0;
}

// ----------------------------------------------------------------------------------
int Slider_GetMaxValue(lua_State* L)
{
	CUiSlider* slider = dynamic_cast<CUiSlider*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (slider == NULL)
		lua_error(L);

	lua_pushnumber(L, slider->GetMaxValue());
	return 1;
}

// ----------------------------------------------------------------------------------
int Slider_SetMinValue(lua_State* L)
{
	CUiSlider* slider = dynamic_cast<CUiSlider*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (slider == NULL)
		lua_error(L);

	slider->SetMinValue((float)luaL_checknumber(L, 2));
	return 0;
}

// ----------------------------------------------------------------------------------
int Slider_GetMinValue(lua_State* L)
{
	CUiSlider* slider = dynamic_cast<CUiSlider*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (slider == NULL)
		lua_error(L);

	lua_pushnumber(L, slider->GetMinValue());
	return 1;
}

// ----------------------------------------------------------------------------------
int Slider_SetMinMaxValues(lua_State* L)
{
	CUiSlider* slider = dynamic_cast<CUiSlider*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (slider == NULL)
		lua_error(L);

	if ( lua_isnumber(L, 2) )
		slider->SetMinValue( (float)lua_tonumber(L, 2) );
	if ( lua_isnumber(L, 3) )
		slider->SetMaxValue( (float)lua_tonumber(L, 3) );

	return 0;
}

// ----------------------------------------------------------------------------------
int Slider_GetMinMaxValues(lua_State* L)
{
	CUiSlider* slider = dynamic_cast<CUiSlider*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (slider == NULL)
		lua_error(L);

	lua_pushnumber(L, slider->GetMinValue());
	lua_pushnumber(L, slider->GetMaxValue());
	return 2;
}

// ----------------------------------------------------------------------------------
int Slider_SetValue(lua_State* L)
{
	CUiSlider* slider = dynamic_cast<CUiSlider*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (slider == NULL)
		lua_error(L);

	slider->SetValue((float)luaL_checknumber(L, 2));
	return 0;
}

// ----------------------------------------------------------------------------------
int Slider_GetValue(lua_State* L)
{
	CUiSlider* slider = dynamic_cast<CUiSlider*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (slider == NULL)
		lua_error(L);

	lua_pushnumber(L, slider->GetValue());
	return 1;
}

// ----------------------------------------------------------------------------------
int Slider_SetValueStepMode(lua_State* L)
{
	CUiSlider* slider = dynamic_cast<CUiSlider*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (slider == NULL)
		lua_error(L);

	const char* mode = luaL_checkstring(L, 2);
	if ( strcmp(mode, "INT") == 0 )
		slider->SetValueStepMode(CUiSlider::eValueStep_Int);
	else if ( strcmp(mode, "FLOAT") == 0 )
		slider->SetValueStepMode(CUiSlider::eValueStep_Float);
	return 0;
}

// ----------------------------------------------------------------------------------
int Slider_GetValueStepMode(lua_State* L)
{
	CUiSlider* slider = dynamic_cast<CUiSlider*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (slider == NULL)
		lua_error(L);

	switch ( slider->GetValueStepMode() )
	{
	case CUiSlider::eValueStep_Int:
		lua_pushstring(L, "INT");
		break;

	case CUiSlider::eValueStep_Float:
		lua_pushstring(L, "FLOAT");
		break;

	default:
		lua_pushnil(L);
		break;
	}
	return 1;
}