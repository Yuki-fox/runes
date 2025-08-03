#include "../../netwaker_member/NetWakerClientInc.h"
#include "../../mainproc/GameMain.h"
#include "InspectFrame.h"

CInspectFrame*					g_pInspectFrame = NULL;

int LuaFunc_NotifyInspect(lua_State* L);
int LuaFunc_GetInspectIntroduce(lua_State* L);

// ----------------------------------------------------------------------------------
CInspectFrame::CInspectFrame(CInterface* object)
	: CInterfaceBase(object)
{
	m_unit.clear();

	g_pInspectFrame = this;
}

// ----------------------------------------------------------------------------------
CInspectFrame::~CInspectFrame()
{
	m_unit.clear();
}

// ----------------------------------------------------------------------------------
void CInspectFrame::RegisterFunc()
{
	lua_State* L = GetLuaState();
	if (L)
	{
		lua_register(L, "NotifyInspect",				LuaFunc_NotifyInspect);
		lua_register(L, "GetInspectIntroduce",			LuaFunc_GetInspectIntroduce);
	}
}

// ----------------------------------------------------------------------------------
void CInspectFrame::UpdateInspectItems()
{
	lua_State* L = GetLuaState();
	if ( L )
	{
		lua_pushstring(L, m_unit.c_str());
		lua_setglobal(L, "arg1");
		SendWorldEvent("UNIT_INVENTORY_CHANGED");
	}
}

// ----------------------------------------------------------------------------------
void CInspectFrame::UpdateInspectIntroduce()
{
	SendWorldEvent("INSPECT_INTRODUCE_UPDATE");
}

// ----------------------------------------------------------------------------------
void CInspectFrame::BuildFrames()
{
	m_unit.clear();
}

// ----------------------------------------------------------------------------------
void CInspectFrame::DestroyFrames()
{
	m_unit.clear();
}

// ----------------------------------------------------------------------------------
bool CInspectFrame::InspectUnit(const char* unit)
{
	if ( unit == NULL )
		return false;	
	
	CRoleSprite* sprite = g_pGameMain->GetUnitSprite(unit);
	if ( sprite )
	{
		if ( g_pGameMain->FindSpriteByID(sprite->GetIdentify()) != NULL )
		{
			m_unit = unit;
			NetCli_OtherChild::S_Inspect(sprite->GetIdentify());
			return true;
		}
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_INSPECT_OVER_DISTANCE"));
	}
	else
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_ERROR_NOTEXIST"));

	return false;
}

// ----------------------------------------------------------------------------------
int LuaFunc_NotifyInspect(lua_State* L)
{
	lua_pushboolean(L, g_pInspectFrame->InspectUnit(lua_tostring(L, 1)));
	return 1;
}

// ----------------------------------------------------------------------------------
int LuaFunc_GetInspectIntroduce(lua_State* L)
{
	NetInspectStruct inspect = NetCli_OtherChild::GetInspectInfo();
	lua_pushstring(L, inspect.note.c_str());
	return 1;
}