
#include "../../mainproc/GameMain.h"
#include "../../netwaker_member/NetWakerClientInc.h"
#include "../WorldFrames/ItemClipboard.h"
#include "../WorldFrames/ChatEdit.h"
#include "../WorldFrames/ChatFrame.h"
#include "CombatMeter.h"


CCombatMeter* g_pCombatMeter = NULL;



#define UiEvent_CombatMeterDamage "COMBATMETER_DAMAGE"
#define UiEvent_CombatMeterHeal   "COMBATMETER_HEAL"

//int LuaFunc_GetEmoteInfoCount(lua_State* L); //
//int LuaFunc_CombatMeter_Output(lua_State* L);      //
//
//
//int LuaFunc_CombatMeter_GetUint(lua_State* L);
//int LuaFunc_CombatMeter_GetSkill(lua_State* L);
//int LuaFunc_CombatMeter_GetType(lua_State* L);



// ----------------------------------------------------------------------------
CCombatMeter::CCombatMeter(CInterface* object)
	: CInterfaceBase(object)
{
	g_pCombatMeter = this;	



}

// ----------------------------------------------------------------------------
CCombatMeter::~CCombatMeter()
{
	g_pCombatMeter = NULL;


}


// ----------------------------------------------------------------------------
void CCombatMeter::RegisterFunc()
{
	lua_State* L = GetLuaStateWorld();
	if (L)
	{

		//lua_register( L, "CombatMeter_Output"  , LuaFunc_CombatMeter_Output );
		//lua_register( L, "GetEmoteInfoCount" , LuaFunc_GetEmoteInfoCount );
	}
}

// ----------------------------------------------------------------------------
void CCombatMeter::AddDamage( const char* source, const char* target, const char* skill, const char* type, int damage )
{
	//assert( name );
	//assert( skill );
	//assert( type );
	//if( !name || !skill || !type )
	//{
	//	return;
	//}
	//CCombatMeterUnit& r = m_UnitDamages[name];
	//r.AddValue( skill,  type, damage );

	lua_State* L = m_interface->GetLuaState();
	if ( L ) {
		lua_pushstring( L, source );
		lua_setglobal( L, "_source" );

		lua_pushstring( L, target );
		lua_setglobal( L, "_target" );

		lua_pushstring( L, skill );
		lua_setglobal( L, "_skill" );

		lua_pushstring( L, type );
		lua_setglobal( L, "_type" );

		lua_pushnumber( L, damage );
		lua_setglobal( L, "_damage" );

		SendWorldEvent( UiEvent_CombatMeterDamage );
	}
}

// ----------------------------------------------------------------------------
void CCombatMeter::AddHeal( const char* source, const char* target, const char* skill, const char* type, int heal )
{
	lua_State* L = m_interface->GetUi()->GetLuaState();

	lua_pushstring( L, source );
	lua_setglobal( L, "_source" );

	lua_pushstring( L, target );
	lua_setglobal( L, "_target" );

	lua_pushstring( L, skill );
	lua_setglobal( L, "_skill" );

	lua_pushstring( L, type );
	lua_setglobal( L, "_type" );

	lua_pushnumber( L, heal );
	lua_setglobal( L, "_heal" );

	SendWorldEvent( UiEvent_CombatMeterHeal );
}


//// ----------------------------------------------------------------------------
////
//// ----------------------------------------------------------------------------
//int LuaFunc_GetEmoteInfo(lua_State* L)
//{
//
//	int index = luaL_checkint(L, 1);
//
//	SEmoteItem* pItem = g_pCombatMeter->GetEmoteItemByIndex( index );
//	if( !pItem )
//		return 0;
//	
//	lua_pushnumber( L, pItem->ID );
//	lua_pushstring( L, pItem->pName );	
//	lua_pushstring( L, pItem->pToken );	
//	lua_pushstring( L, g_pCombatMeter->GetEmoteIcon( pItem ) );	
//
//	return 4;
//}













