#include <windows.h>
#include "../uimodel.h"

int Model_SetTargetBone(lua_State* L);
int Model_SetTargetRotate(lua_State* L);
int Model_SetModelOffset(lua_State* L);
int Model_SetAlignmentFrames(lua_State* L);
int Model_SetMaskFile(lua_State* L);
int Model_SetCameraPosition(lua_State* L);
int Model_SetDefaultTexture(lua_State* L);

int Model_SetComponent(lua_State* L);
int Model_SetComponentColors(lua_State* L);

int Model_SetDyeFXColors(lua_State* L);
int Model_GetMaterialCount(lua_State* L);
int Model_CanDyeFXColors(lua_State* L);


int Model_Build(lua_State* L);
int Model_PlayMotion(lua_State* L);

int Model_IsPaperdoll(lua_State* L);

int Model_InserLinkFrame(lua_State* L);


extern int Model_SetHanger(lua_State* L); //︾琜	
extern int Model_SetServantHire(lua_State* L); // ┷兑共	

extern int Model_SetServant(lua_State* L); // ┬共	


extern int Model_SetItemLink(lua_State* L);
extern int Model_TakeOffAll(lua_State* L);
extern int Model_TakeOffWeapon(lua_State* L);

extern int Model_SetDummyMale(lua_State* L);
extern int Model_SetDummyFemale(lua_State* L);

extern int Model_ReleaseModel(lua_State* L);





int Model_SetModel(lua_State* L);

extern int Model_SetPlayerModel(lua_State* L); //陪ボ竤ざ 狟ね家

extern int Model_SetGuildHouseBuilding(lua_State* L); //陪ボそ穦縱家

// ----------------------------------------------------------------------------------
static const struct luaL_reg g_modelTable [] = {
	{"SetUnit",				Model_SetUnit},
	{"SetHanger",			Model_SetHanger}, //︾琜
 	{"SetServantHire",      Model_SetServantHire}, //┷兑共
    {"SetServant",          Model_SetServant}, //┬共


	{"SetPet",				Model_SetPet},
   	{"SetTargetBone",		Model_SetTargetBone},
	{"SetTargetRotate",		Model_SetTargetRotate},
	{"SetModelOffset",		Model_SetModelOffset},
	{"SetAlignmentFrames",	Model_SetAlignmentFrames},
	{"SetMaskFile",			Model_SetMaskFile},
	{"SetCameraPosition",	Model_SetCameraPosition},
	{"SetDefaultTexture",	Model_SetDefaultTexture},
	{"SetComponent",        Model_SetComponent},
 	{"SetComponentColors",	Model_SetComponentColors},
	{"SetDyeFXColors",		Model_SetDyeFXColors},
	{"GetMaterialCount",	Model_GetMaterialCount},
	{"CanDyeFXColors",		Model_CanDyeFXColors},



 	{"Build",				Model_Build},
  	{"PlayMotion",			Model_PlayMotion},

	{"SetItemLink",			Model_SetItemLink},
 	{"TakeOffAll",			Model_TakeOffAll},
  	{"TakeOffWeapon",		Model_TakeOffWeapon},
	




	{"SetModel",			Model_SetModel},

   	{"SetPlayerModel",		Model_SetPlayerModel},
   	{"SetGuildHouseBuilding", Model_SetGuildHouseBuilding},


	{"IsPaperdoll",			Model_IsPaperdoll},
  	{"InserLinkFrame",		Model_InserLinkFrame	},


 	{"SetDummyMale",		Model_SetDummyMale		},
 	{"SetDummyFemale",		Model_SetDummyFemale	},

	{"ReleaseModel",		Model_ReleaseModel	},
    
	{NULL, NULL}
};

// ----------------------------------------------------------------------------------
void CUiLuaModel::OpenLib(lua_State* L)
{
	luaL_newmetatable(L, UI_LUA_TYPE_MODEL);			// create new metatable
	luaL_getmetatable(L, UI_LUA_TYPE_FRAME);			// inherit base table
	lua_setmetatable(L, -2);
	lua_pushstring(L, "__index");
	lua_pushvalue(L, -2);
	lua_settable(L, -3);
	luaL_openlib(L, NULL, g_modelTable, 0);
}

// ----------------------------------------------------------------------------------
int	CUiLuaModel::SetMetaTable(lua_State* L)
{
	if (lua_istable(L, -1))
	{
		luaL_getmetatable(L, UI_LUA_TYPE_MODEL);
		lua_setmetatable(L, -2);
		return 1;
	}
	return 0;
}

// ----------------------------------------------------------------------------------
int Model_SetTargetBone(lua_State* L)
{
	CUiModel* model = dynamic_cast<CUiModel*>(CUiLuaLayout::CheckLayoutTable(L, 1));
	if (model == NULL)
		lua_error(L);

	model->SetTargetBone(luaL_checkstring(L, 2));
	return 0;
}

// ----------------------------------------------------------------------------------
int Model_SetTargetRotate(lua_State* L)
{
	CUiModel* model = dynamic_cast<CUiModel*>(CUiLuaLayout::CheckLayoutTable(L, 1));
	if (model == NULL)
		lua_error(L);

	model->SetTargetRotate((float)luaL_checknumber(L, 2));
	return 0;
}

// ----------------------------------------------------------------------------------
int Model_SetModelOffset(lua_State* L)
{
	CUiModel* model = dynamic_cast<CUiModel*>(CUiLuaLayout::CheckLayoutTable(L, 1));
	if (model == NULL)
		lua_error(L);

	CRuVector3 offset;
	offset.m_x = (lua_isnumber(L, 2)) ? (float)lua_tonumber(L, 2) : 0.0f;
	offset.m_y = (lua_isnumber(L, 3)) ? (float)lua_tonumber(L, 3) : 0.0f;
	offset.m_z = (lua_isnumber(L, 4)) ? (float)lua_tonumber(L, 4) : 0.0f;
	model->SetModelOffset(offset);
	return 0;
}

// ----------------------------------------------------------------------------------
int Model_SetAlignmentFrames(lua_State* L)
{
	CUiModel* model = dynamic_cast<CUiModel*>(CUiLuaLayout::CheckLayoutTable(L, 1));
	if (model == NULL)
		lua_error(L);

	model->SetAlignmentFrames(luaL_checkstring(L, 2), luaL_checkstring(L, 3), (float)luaL_checknumber(L, 4), (float)luaL_checknumber(L, 5));
	return 0;
}

// ----------------------------------------------------------------------------------
int Model_SetMaskFile(lua_State* L)
{
	CUiModel* model = dynamic_cast<CUiModel*>(CUiLuaLayout::CheckLayoutTable(L, 1));
	if (model == NULL)
		lua_error(L);

	model->SetMaskFile(luaL_checkstring(L, 2));
	return 0;
}

// ----------------------------------------------------------------------------------
int Model_SetCameraPosition(lua_State* L)
{
	CUiModel* model = dynamic_cast<CUiModel*>(CUiLuaLayout::CheckLayoutTable(L, 1));
	if (model == NULL)
		lua_error(L);

	CRuVector3 pos;
	pos.m_x = (float)-luaL_checknumber(L, 2);
	pos.m_y = (float)-luaL_checknumber(L, 3);
	pos.m_z = (float)-luaL_checknumber(L, 4);
	model->SetRelativeCameraPosition(pos);
	return 0;
}

// ----------------------------------------------------------------------------------
int Model_SetDefaultTexture(lua_State* L)
{
	CUiModel* model = dynamic_cast<CUiModel*>(CUiLuaLayout::CheckLayoutTable(L, 1));
	if (model == NULL)
		lua_error(L);

	CUiTexture* defaultTexture = dynamic_cast<CUiTexture*>(CUiLuaLayout::CheckLayoutTable(L, 2));
	model->SetDefaultTexture(defaultTexture);
	return 0;
}


// ----------------------------------------------------------------------------------
int Model_SetComponent(lua_State* L)
{
	CUiModel* model = dynamic_cast<CUiModel*>(CUiLuaLayout::CheckLayoutTable(L, 1));
	if (model == NULL)
		lua_error(L);

   	CRuEntity* pRuEntity = model->GetRuEntity();
  
	const char *pPartName = luaL_checkstring(L, 2);
	const char *pComponentName = luaL_checkstring(L, 3);

	RuPaperdoll_SetComponent( pRuEntity, pPartName, pComponentName );

	return 0;
}

// ----------------------------------------------------------------------------------
int Model_SetComponentColors(lua_State* L)
{
	CUiModel* model = dynamic_cast<CUiModel*>(CUiLuaLayout::CheckLayoutTable(L, 1));
	if (model == NULL)
		lua_error(L);

   	CRuEntity* pRuEntity = model->GetRuEntity();

  	const char *pPartName = luaL_checkstring(L, 2);

	BOOL layerActivated[4] = {true,true,true,true};
    RUCOLOR layerColor[4];

	layerColor[0] = 0xffffffff;
 	layerColor[1] = 0xffffffff;

	{
		float r = (float)luaL_checknumber(L, 3);
		float g = (float)luaL_checknumber(L, 4);
		float b = (float)luaL_checknumber(L, 5);
		CRuColor4 rucolor(1.0f,r,g,b);
		layerColor[2] = rucolor.GetARGB();
	}
	{
		float r = (float)luaL_checknumber(L, 6);
		float g = (float)luaL_checknumber(L, 7);
		float b = (float)luaL_checknumber(L, 8);
		CRuColor4 rucolor(1.0f,r,g,b);
		layerColor[3] = rucolor.GetARGB();
	}
 
	RuPaperdoll_SetComponentColors( pRuEntity, pPartName, layerActivated, layerColor);
   
	return 0;
}

// ----------------------------------------------------------------------------------
int Model_Build(lua_State* L)
{
	CUiModel* model = dynamic_cast<CUiModel*>(CUiLuaLayout::CheckLayoutTable(L, 1));
	if (model == NULL)
		lua_error(L);

   	CRuEntity* pRuEntity = model->GetRuEntity();
	RuPaperdoll_Build( pRuEntity , false );

	return 0;
}

// ----------------------------------------------------------------------------------
int Model_PlayMotion(lua_State* L)
{
	CUiModel* model = dynamic_cast<CUiModel*>(CUiLuaLayout::CheckLayoutTable(L, 1));
	if (model == NULL)
		lua_error(L);

	CRuEntity* pRuEntity = model->GetRuEntity();

	const char *pMotion = luaL_checkstring(L, 2);

	RuACT_PlayMotion(pRuEntity, pMotion);

	return 0;	
}

// ----------------------------------------------------------------------------------
int Model_SetDyeFXColors(lua_State* L)
{
	CUiModel* model = dynamic_cast<CUiModel*>(CUiLuaLayout::CheckLayoutTable(L, 1));
	if (model == NULL)
		lua_error(L);

	int colors[4];
	colors[0]= luaL_checkint(L, 2);
	colors[1]= luaL_checkint(L, 3);
	colors[2]= luaL_checkint(L, 4);
	colors[3]= luaL_checkint(L, 5);

	model->SetDyeFXColors(colors);	
	return 0;	
}

// ----------------------------------------------------------------------------------
int Model_GetMaterialCount(lua_State* L)
{
	CUiModel* model = dynamic_cast<CUiModel*>(CUiLuaLayout::CheckLayoutTable(L, 1));
	if (model == NULL)
		lua_error(L);

	lua_pushnumber(L, model->GetMaterialCount());
	return 1;
}

// ----------------------------------------------------------------------------------
int Model_CanDyeFXColors(lua_State* L)
{
	CUiModel* model = dynamic_cast<CUiModel*>(CUiLuaLayout::CheckLayoutTable(L, 1));
	if (model == NULL)
		lua_error(L);
	
	bool outExists[4] = { false, false, false, false };
	if ( model->CanDyeFXColors(luaL_checkint(L, 2) - 1, outExists) == false )
		return 0;

	lua_pushboolean(L, true);
	lua_pushboolean(L, outExists[0]);
	lua_pushboolean(L, outExists[1]);
	lua_pushboolean(L, outExists[2]);
	lua_pushboolean(L, outExists[3]);
	return 5;
}

// ----------------------------------------------------------------------------------
int Model_IsPaperdoll(lua_State* L)
{
	CUiModel* model = dynamic_cast<CUiModel*>(CUiLuaLayout::CheckLayoutTable(L, 1));
	if (model == NULL)
		lua_error(L);

	CRuEntity* pEntity = model->GetRuEntity();

	if( NULL == pEntity )
	{
		return 0;
	}

	IRuPaperdoll* pPaperdoll = RuEntity_FindFirstPaperdoll( pEntity );

	if( NULL == pPaperdoll )
	{
		return 0;
	}

	lua_pushboolean( L, true);
 
	return 1;
}

// ----------------------------------------------------------------------------------
int Model_InserLinkFrame(lua_State* L)
{
	CUiModel* model = dynamic_cast<CUiModel*>(CUiLuaLayout::CheckLayoutTable(L, 1));
	if (model == NULL)
		lua_error(L);


	const char *pName = luaL_checkstring(L, 2);

	CRuVector3 point;
	point.x = (float)luaL_checknumber(L, 3);
	point.y = (float)luaL_checknumber(L, 4);
 	point.z = (float)luaL_checknumber(L, 5);

	model->InserLinkFrame( pName, point );

	return 0;
}

// ----------------------------------------------------------------------------------
int Model_ReleaseModel(lua_State* L)
{
	CUiModel* model = dynamic_cast<CUiModel*>(CUiLuaLayout::CheckLayoutTable(L, 1));
	if (model == NULL)
		lua_error(L);

	model->CreateEntity( NULL );

	return 0;
}



