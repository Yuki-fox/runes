#include <windows.h>
#include "../uitexture.h"

int Texture_SetFile(lua_State* L);
int	Texture_SetRotate(lua_State* L);
int Texture_SetMaskFile(lua_State* L);
int Texture_SetTexture(lua_State* L);
int Texture_SetAlphaMode(lua_State* L);
int Texture_SetLuminance(lua_State* L);
int Texture_SetTexCoord(lua_State* L);
int Texture_SetCooldown(lua_State* L);
int Texture_SetAnimation(lua_State* L);
int Texture_PlayAnimation(lua_State* L);
int Texture_GetAnimationProgress(lua_State* L);
int Texture_Reload(lua_State* L);

// ----------------------------------------------------------------------------------
static const struct luaL_reg g_textureTable [] = {	
	{"SetFile",					Texture_SetFile},
	{"SetRotate",				Texture_SetRotate},
	{"SetMaskFile",				Texture_SetMaskFile},
	{"SetTexture",				Texture_SetTexture},
	{"SetAlphaMode",			Texture_SetAlphaMode},
	{"SetLuminance",			Texture_SetLuminance},
	{"SetTexCoord",				Texture_SetTexCoord},
	{"SetCooldown",				Texture_SetCooldown},
	{"SetAnimation",			Texture_SetAnimation},
	{"PlayAnimation",			Texture_PlayAnimation},
	{"GetAnimationProgress",	Texture_GetAnimationProgress},
	{"Reload",					Texture_Reload},

	{NULL, NULL}
};

// ----------------------------------------------------------------------------------
void CUiLuaTexture::OpenLib(lua_State* L)
{
	luaL_newmetatable(L, UI_LUA_TYPE_TEXTURE);			// create new metatable
	luaL_getmetatable(L, UI_LUA_TYPE_LAYOUT);			// inherit layout
	lua_setmetatable(L, -2);
	lua_pushstring(L, "__index");
	lua_pushvalue(L, -2);
	lua_settable(L, -3);
	luaL_openlib(L, NULL, g_textureTable, 0);
}

// ----------------------------------------------------------------------------------
int	CUiLuaTexture::SetMetaTable(lua_State* L)
{
	if (lua_istable(L, -1))
	{
		luaL_getmetatable(L, UI_LUA_TYPE_TEXTURE);
		lua_setmetatable(L, -2);
		return 1;
	}
	return 0;
}

// ----------------------------------------------------------------------------------
int Texture_SetFile(lua_State* L)
{
	CUiTexture* texture = dynamic_cast<CUiTexture*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (texture == NULL)
		lua_error(L);

	if (lua_isstring(L, 2))
		texture->SetFile(lua_tostring(L, 2));
	return 0;
}

// ----------------------------------------------------------------------------------
int Texture_SetRotate(lua_State* L)
{
	CUiTexture* texture = dynamic_cast<CUiTexture*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (texture == NULL)
		lua_error(L);

	if ( lua_isnumber(L, 2) )
	{
		texture->SetRotate((float)lua_tonumber(L, 2));
	}
	return 0;
}

// ----------------------------------------------------------------------------------
int Texture_SetMaskFile(lua_State* L)
{
	CUiTexture* texture = dynamic_cast<CUiTexture*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (texture == NULL)
		lua_error(L);

	if (lua_isstring(L, 2))
		texture->SetMaskFile(lua_tostring(L, 2));
	return 0;
}

// ----------------------------------------------------------------------------------
int Texture_SetTexture(lua_State* L)
{
	CUiTexture* texture = dynamic_cast<CUiTexture*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if ( texture )
	{		
		switch( lua_type(L, 2) )
		{
		case LUA_TTABLE:
			texture->SetTexture( dynamic_cast<CUiTexture*>(CUiLuaLayout::CheckLayoutTable(L, 2)) );
			return 0;

		case LUA_TSTRING:
			texture->SetFile( lua_tostring(L, 2) );
			return 0;

		default:
			texture->SetFile("");
			return 0;
		}
	}
	lua_error(L);
	return 0;
}

// ----------------------------------------------------------------------------------
int Texture_SetAlphaMode(lua_State* L)
{
	CUiTexture* texture = dynamic_cast<CUiTexture*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (texture == NULL)
		lua_error(L);

	const char* alphaMode = luaL_checkstring(L, 2);
	if ( strcmp(alphaMode, "DISABLE") == 0 )
		texture->SetAlphaMode(ALPHA_MODE_DISABLE);
	else if ( strcmp(alphaMode, "BLEND") == 0 )
		texture->SetAlphaMode(ALPHA_MODE_BLEND);
	else if ( strcmp(alphaMode, "ADD") == 0 )
		texture->SetAlphaMode(ALPHA_MODE_ADD);
	else if ( strcmp(alphaMode, "LUMINANCE") == 0 )
		texture->SetAlphaMode(ALPHA_MODE_LUMINANCE);
	return 0;
}

// ----------------------------------------------------------------------------------
int Texture_SetLuminance(lua_State* L)
{
	CUiTexture* texture = dynamic_cast<CUiTexture*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (texture == NULL)
		lua_error(L);

	if (lua_toboolean(L, 2) != 0)
		texture->SetAlphaMode( ALPHA_MODE_LUMINANCE );
	else
		texture->SetAlphaMode( ALPHA_MODE_BLEND );
	return 0;
}

// ----------------------------------------------------------------------------------
int Texture_SetTexCoord(lua_State* L)
{
	CUiTexture* texture = dynamic_cast<CUiTexture*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (texture == NULL)
		lua_error(L);

	CRuRectangle rect(0.0f, 0.0f, 1.0f, 1.0f);
	for ( int i = 2; i <= lua_gettop(L); i++ )
	{
		if ( !lua_isnumber(L, i) )
			continue;

		switch (i)
		{
		case 2:
			rect.m_left = (float)luaL_checknumber(L, i);
			break;

		case 3:
			rect.m_right = (float)luaL_checknumber(L, i);
			break;

		case 4:
			rect.m_top = (float)luaL_checknumber(L, i);
			break;

		case 5:
			rect.m_bottom = (float)luaL_checknumber(L, i);
			break;
		}
	}
	texture->SetTexCoords(1, &rect);
	return 0;
}

// ----------------------------------------------------------------------------------
int Texture_SetCooldown(lua_State* L)
{
	CUiTexture* texture = dynamic_cast<CUiTexture*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (texture == NULL)
		lua_error(L);

	texture->StartCooldown((float)luaL_checknumber(L, 2), (float)luaL_checknumber(L, 3));
	return 0;
}
//-------------------------------------------------
// (file ,speed, dism,index,count)
int Texture_SetAnimation(lua_State* L)
{

	CUiTexture* texture = dynamic_cast<CUiTexture*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (texture == NULL)
		lua_error(L);
	const char *file=luaL_checkstring(L,2);
	float speed=(float)luaL_checknumber(L, 3);
	int disw=(int)luaL_checknumber(L, 4);
	int index=(int)luaL_checknumber(L, 5);
	int count=(int)luaL_checknumber(L, 6);

	bool shrink=false;
	int counts=lua_gettop(L);

	if ( counts>=7&&lua_isboolean(L, 7) )
        shrink = lua_toboolean(L, 7) != 0;
	
	


	CRuRectangle *pArray=new CRuRectangle[count];

	texture->SetFile(file);

	texture->ClearTexCoords();

	int i;
	
	float x=1.0f/disw;
	int  s=index;
	float dx=0;
	if (shrink)
	dx=1.0/256.0f;

	for( i=0;i<count;i++)
	{
		float u=x*(s/disw);
		float v=x*(s%disw);
		pArray[i].m_top=u+dx;
		pArray[i].m_left=v+dx;
		
		pArray[i].m_bottom=u+x-dx;
		pArray[i].m_right=v+x-dx;
		s++;
	}

	texture->SetTexCoords(count, pArray);


	texture->SetAnimationSpeed(speed);




	delete []pArray;



return 0;
}

// ----------------------------------------------------------------------------------
int Texture_PlayAnimation(lua_State* L)
{
	CUiTexture* texture = dynamic_cast<CUiTexture*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (texture == NULL)
		lua_error(L);

	texture->PlayAnimation();
	return 0;
}

// ----------------------------------------------------------------------------------
int Texture_GetAnimationProgress(lua_State* L)
{
	CUiTexture* texture = dynamic_cast<CUiTexture*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (texture == NULL)
		lua_error(L);

	lua_pushnumber(L, texture->GetAnimationProgress());
	return 1;
}

// ----------------------------------------------------------------------------------
int Texture_Reload(lua_State* L)
{
	CUiTexture* texture = dynamic_cast<CUiTexture*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (texture == NULL)
		lua_error(L);

	texture->ReloadFile();
	return 0;
}