#include "../../mainproc/GameMain.h"
#include "../../netwaker_member/NetWakerClientInc.h"
#include "RoleData/ObjectDataClass.h"

#include "FusionStoneTradeFrame.h"

CFusionStoneTradeFrame*		g_pFusionStoneTradeFrame	= NULL;

//-------------------------------------------------------------------
int			LuaFunc_GetFSTOpenInfo				( lua_State *L );
int			LuaFunc_GetFSTAttrListCount			( lua_State *L );
int			LuaFunc_GetFSTAttrID				( lua_State *L );
int			LuaFunc_SendFSTResult				( lua_State *L );
int			LuaFunc_GetFSTMoneyType				( lua_State *L );
int			LuaFunc_GetFSTAttrInfo				( lua_State *L );

//-------------------------------------------------------------------
CFusionStoneTradeFrame::CFusionStoneTradeFrame( CInterface* object ) : CInterfaceBase( object )
{
	g_pFusionStoneTradeFrame = this;
	m_iStoneID		= 0;
	m_iHighestLv	= 10;
	m_iMoney[0]		= 0;
	m_iMoney[1]		= 0;
}
// --------------------------------------------------------------------------------------
CFusionStoneTradeFrame::~CFusionStoneTradeFrame(void)
{
	g_pFusionStoneTradeFrame = NULL;
}
//--------------------------------------------------------------------------------------
void CFusionStoneTradeFrame::RegisterFunc(void)
{
	lua_State* l = GetLuaStateWorld();
	if (l)
	{
		lua_register( l, "GetFSTOpenInfo",				LuaFunc_GetFSTOpenInfo			);	// 取得黑幣兌換介面開啟所需的資訊
		lua_register( l, "GetFSTAttrListCount",			LuaFunc_GetFSTAttrListCount		);	// 取得黑幣兌換介面屬性列表項目總數
		lua_register( l, "GetFSTAttrID",				LuaFunc_GetFSTAttrID			);	// 以索引取得黑幣兌換介面屬性列表屬性ID
		lua_register( l, "SendFSTResult",				LuaFunc_SendFSTResult			);	// 告訴server兌換結果
		lua_register( l, "GetFSTMoneyType",				LuaFunc_GetFSTMoneyType			);	// 取得黑幣兌換介面的兩種幣值類型
		lua_register( l, "GetFSTAttrInfo",				LuaFunc_GetFSTAttrInfo			);	// 取得黑幣兌換介面的選定之屬性的資訊
	}
}

void CFusionStoneTradeFrame::SetOpenInfo( int MagicStoneID, int Lv, int Money[2] )
{
	m_iStoneID		= MagicStoneID;
	m_iHighestLv	= Lv;
	m_iMoney[0]		= Money[0];
	m_iMoney[1]		= Money[1];
}

int LuaFunc_GetFSTOpenInfo( lua_State *L )
{
	GameObjDbStructEx* pItem = g_ObjectData->GetObj( g_pFusionStoneTradeFrame->GetStoneID() );

	if( pItem )
	{
		GameObjDbStruct* imageDB = CNetGlobal::GetObj( pItem->ImageID );

		if( imageDB )
			lua_pushstring( L, imageDB->Image.ACTField );
		else
			lua_pushnil( L );
	}

	lua_pushnumber( L, g_pFusionStoneTradeFrame->GetHighestLv() );
	lua_pushnumber( L, g_pFusionStoneTradeFrame->Get1stMoney() );
	lua_pushnumber( L, g_pFusionStoneTradeFrame->Get2ndMoney() );

	return 4;
}

int LuaFunc_GetFSTAttrListCount( lua_State *L )
{
	lua_pushnumber( L, (lua_Number)ObjectDataClass::St_SysValue()->MagicStoneShopAbilityList.size() );

	return 1;
}

int LuaFunc_GetFSTAttrID( lua_State *L )
{
	int iIndex = luaL_checkint( L, 1 ) - 1;
	int iAbilityID = ObjectDataClass::St_SysValue()->MagicStoneShopAbilityList[iIndex];

	lua_pushnumber( L, iAbilityID );

	return 1;
}

int LuaFunc_SendFSTResult( lua_State *L )
{
	static int s_SendFSTResult_LastTime = 0;

	int iAbilityID[3];
	iAbilityID[0] = luaL_checkint( L, 1 );
	iAbilityID[1] = luaL_checkint( L, 2 );
	iAbilityID[2] = luaL_checkint( L, 3 );
	bool bResult = luaL_checkint( L, 4 ) == 1 ? true : false;

	if( timeGetTime() < DWORD(s_SendFSTResult_LastTime + 3000) && bResult )
		return 0;

	s_SendFSTResult_LastTime = timeGetTime();	

	NetCli_Item::SL_MagicStoneExchangeResult( iAbilityID, bResult );

	return 0;
}

int LuaFunc_GetFSTMoneyType( lua_State *L )
{
	lua_pushnumber( L, ObjectDataClass::St_SysValue()->MagicStoneShopCostType[0] );
	lua_pushnumber( L, ObjectDataClass::St_SysValue()->MagicStoneShopCostType[1] );

	return 2;
}

int LuaFunc_GetFSTAttrInfo( lua_State *L )
{
	int iAbilityID = luaL_checkint( L, 1 );

	GameObjDbStructEx* pAbility = g_ObjectData->GetObj( iAbilityID );

	if( !pAbility )
		return 0;

	GameObjDbStruct* imageDB = CNetGlobal::GetObj( pAbility->ImageID );

	if( imageDB )
		lua_pushstring( L, imageDB->Image.ACTField );
	else
		lua_pushnil( L );

	lua_pushstring( L, pAbility->GetName() );

	char Buf[256];
	char Buf2[256];

	g_ObjectData->GetWearEqTypeString( pAbility->Attirbute.Ability.EqType[0], (float)pAbility->Attirbute.Ability.EqTypeValue[0], Buf, 256 );
	sprintf_s( Buf2, sizeof(Buf2), "|cff00ff00%s|r", Buf );

	lua_pushstring( L, Buf2 );

	return 3;
}