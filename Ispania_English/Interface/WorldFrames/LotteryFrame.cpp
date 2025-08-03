
#include "../mainproc/GameMain.h"
#include "LotteryFrame.h"
#include "ItemClipboard.h"
#include "../../netwaker_member/net_lottery/NetCli_LotteryChild.h"
#include "../../netwaker_member/net_item/NetCli_Item.h"


CLotteryFrame* g_pLotteryFrame = NULL;


int LuaFunc_Lottery_GetCurrentVersion  ( lua_State* L );         
int LuaFunc_Lottery_GetMaxPrizeMoney   ( lua_State* L );
int LuaFunc_Lottery_GetLastInfo        ( lua_State* L );
int LuaFunc_Lottery_CloseExchangePrize ( lua_State* L );
int LuaFunc_Lottery_CloseBuyLottery    ( lua_State* L );


int luaFunc_Lottery_NumberClear        ( lua_State* L );
int luaFunc_Lottery_NumberSelect       ( lua_State* L );
int luaFunc_Lottery_NumberCancel       ( lua_State* L );
int luaFunc_Lottery_NumberCount        ( lua_State* L );
int luaFunc_Lottery_GetCombinationCount( lua_State* L );
int luaFunc_Lottery_GetCost            ( lua_State* L );

int LuaFunc_Lottery_BuyLottery         ( lua_State* L );

int LuaFunc_Lottery_ExchangeLottery    ( lua_State* L );
int LuaFunc_Lottery_ConfirmExchange    ( lua_State* L );
int LuaFunc_Lottery_CancelExchange     ( lua_State* L );
int LuaFunc_Lottery_DeleteLottery      ( lua_State* L );

int LuaFunc_Lottery_GetExchangeSlotInfo( lua_State* L );



int SortCallback_CompareINT( const void *arg1, const void *arg2 )
{
	/* Compare all of both strings: */
	int n1 = *(int*)arg1;
	int n2 = *(int*)arg2;
	if( n1 > n2 )
		return 1;
	if( n1 < n2 )
		return -1;
	return 0;
}


// ----------------------------------------------------------------------------
CLotteryFrame::CLotteryFrame(CInterface* object) : CInterfaceBase(object)
{
	g_pLotteryFrame = this;

	m_BuyLottery_Mode = LFM_CLOSE;
	m_BuyLottery_NPCID = 0;
	m_BuyLottery_Version = 0;
	m_BuyLottery_MaxPrizeMoney = 0;

	m_ExchangePrize_Mode = LFM_CLOSE;
	m_ExchangePrize_NPCID = 0;
	ZeroMemory( m_ExchangePrize_LastInfo , sizeof( m_ExchangePrize_LastInfo ) );
	ZeroMemory( m_Number , sizeof( m_Number ) );

	//m_itemPickupExchange = NULL;
	m_bagpos = -1;
}

// ----------------------------------------------------------------------------
CLotteryFrame::~CLotteryFrame()
{
	g_pLotteryFrame = NULL;
}

// ----------------------------------------------------------------------------
void CLotteryFrame::RegisterFunc()
{
	lua_State* L = GetLuaStateWorld();
	if (L)
	{
		lua_register( L, "Lottery_GetCurrentVersion"   , LuaFunc_Lottery_GetCurrentVersion   );
		lua_register( L, "Lottery_GetMaxPrizeMoney"    , LuaFunc_Lottery_GetMaxPrizeMoney    );
		lua_register( L, "Lottery_GetLastInfo"         , LuaFunc_Lottery_GetLastInfo         );
		lua_register( L, "Lottery_CloseExchangePrize"  , LuaFunc_Lottery_CloseExchangePrize  );
		lua_register( L, "Lottery_CloseBuyLottery"     , LuaFunc_Lottery_CloseBuyLottery     );

		lua_register( L, "Lottery_NumberClear"         , luaFunc_Lottery_NumberClear         );
		lua_register( L, "Lottery_NumberSelect"        , luaFunc_Lottery_NumberSelect        );
		lua_register( L, "Lottery_NumberCancel"        , luaFunc_Lottery_NumberCancel        );
		lua_register( L, "Lottery_NumberCount"         , luaFunc_Lottery_NumberCount         );
		lua_register( L, "Lottery_GetCombinationCount" , luaFunc_Lottery_GetCombinationCount );
		lua_register( L, "Lottery_GetCost"             , luaFunc_Lottery_GetCost             );

		lua_register( L, "Lottery_BuyLottery"          , LuaFunc_Lottery_BuyLottery          );

		lua_register( L, "Lottery_ExchangeLottery"     , LuaFunc_Lottery_ExchangeLottery     );
		lua_register( L, "Lottery_ConfirmExchange"     , LuaFunc_Lottery_ConfirmExchange     );
		lua_register( L, "Lottery_CancelExchange"      , LuaFunc_Lottery_CancelExchange      );
		lua_register( L, "Lottery_DeleteLottery"       , LuaFunc_Lottery_DeleteLottery       );
		lua_register( L, "Lottery_GetExchangeSlotInfo" , LuaFunc_Lottery_GetExchangeSlotInfo );

	}
}

// ----------------------------------------------------------------------------
void CLotteryFrame::Update(float elapsedTime)
{
	//判斷物品有沒有被移動
	if( m_ExchangePrize_Mode == LFM_OPEN && m_bagpos >= 0 )
	{
		ItemFieldStruct* item = CNetGlobal::RoleData()->GetBodyItem( m_bagpos );
		if( item == NULL || 
			item->Pos != EM_ItemState_NONE || 
			*item != m_itemPickupExchange )
		{
			m_itemPickupExchange.Init();
			m_bagpos = -1;
			ExchangeSlotChanged();
		}			
	}

	static float lastTime = 0.0f;
	lastTime += elapsedTime;
	if ( lastTime < 0.2f )
		return;

	if( m_ExchangePrize_Mode != LFM_CLOSE && g_pGameMain->GetDistanceWithPlayer(m_ExchangePrize_NPCID) > _MAX_ROLE_TOUCH_MAXRANGE_ )
	{
		if( m_ExchangePrize_Mode == LFM_OPEN )
		{
			SendWorldEvent( UiEvent_CloseExchangePrize );
		}
		NetCli_Lottery::SL_CloseExchangePrize();
		m_ExchangePrize_Mode = LFM_CLOSE;
	}

	if( m_BuyLottery_Mode != LFM_CLOSE && g_pGameMain->GetDistanceWithPlayer(m_BuyLottery_NPCID) > _MAX_ROLE_TOUCH_MAXRANGE_ )
	{
		if( m_BuyLottery_Mode == LFM_OPEN )
		{
			SendWorldEvent( UiEvent_CloseExchangePrize );
		}
		NetCli_Lottery::SL_CloseBuyLottery();
		m_BuyLottery_Mode = LFM_CLOSE;
	}

	lastTime = 0.0f;
}


// ----------------------------------------------------------------------------
void CLotteryFrame::RC_OpenExchangePrize( int TargetNPCID )
{
	NetCli_Lottery::SL_LotteryHistoryInfoRequest();
	m_ExchangePrize_NPCID = TargetNPCID;
	m_ExchangePrize_Mode = LFM_UPDATE;
}

// ----------------------------------------------------------------------------
void CLotteryFrame::RC_ExchangePrizeResult( bool Result , int Money , int PrizeCount[3] )
{

}

// ----------------------------------------------------------------------------
void CLotteryFrame::RC_CloseExchangePrize( )
{
	SendWorldEvent( UiEvent_CloseExchangePrize );
	NetCli_Lottery::SL_CloseExchangePrize();
}

// ----------------------------------------------------------------------------
void CLotteryFrame::RC_OpenBuyLottery( int TargetNPCID )
{
	NetCli_Lottery::SL_LotteryInfoRequest();
	m_BuyLottery_NPCID = TargetNPCID;
	m_BuyLottery_Mode = LFM_UPDATE;
}

// ----------------------------------------------------------------------------
void CLotteryFrame::RC_BuyLotteryResult( bool Result )
{

}

// ----------------------------------------------------------------------------
void CLotteryFrame::RC_CloseBuyLottery( )
{
	SendWorldEvent( UiEvent_CloseBuyLottery );
	NetCli_Lottery::SL_CloseBuyLottery();
}

// ----------------------------------------------------------------------------
void CLotteryFrame::RD_LotteryInfo( int Version , int MaxPrizeMoney )
{
	m_BuyLottery_Version = Version;
	m_BuyLottery_MaxPrizeMoney = MaxPrizeMoney;

	if( m_BuyLottery_Mode == LFM_UPDATE )
	{
		SendWorldEvent( UiEvent_OpenBuyLottery );
		m_BuyLottery_Mode = LFM_OPEN;
	}
}

// ----------------------------------------------------------------------------
void CLotteryFrame::RD_LotteryHistoryInfo( LotteryInfoStruct LastInfo[5] )
{
	int i;
	for( i = 0 ; i < 5 ; ++i )
	{
		m_ExchangePrize_LastInfo[i] = LastInfo[i];

		qsort( m_ExchangePrize_LastInfo[i].Num , 5 , sizeof( INT ) , SortCallback_CompareINT );
	}

	if( m_ExchangePrize_Mode == LFM_UPDATE )
	{
		SendWorldEvent( UiEvent_OpenExchangePrize );
		m_ExchangePrize_Mode = LFM_OPEN;
	}
}



// ----------------------------------------------------------------------------
void CLotteryFrame::RD_RunLottery( LotteryInfoStruct Info )
{
	//UiEvent_ScrollBannerMessage

	//int Version;				//第幾期 ( -1 代表沒有 )
	//int	Num[5];
	//int PrizeMoney[3];			//中獎總彩金
	//int PrizePlayerCount[3];	//中獎人數
	//int	RunLotteryTime;			//開獎日
	//int TotalMoney;

	lua_State* L = m_interface->GetUi()->GetLuaState();

	qsort( Info.Num , 5 , sizeof( INT ) , SortCallback_CompareINT );

	char tt[256];
	//sprintf( tt , "第%d期樂透開獎 中獎號碼 %d %d %d %d %d" , Info.Version, Info.Num[0], Info.Num[1], Info.Num[2], Info.Num[3], Info.Num[4] ) ;
	sprintf( tt , g_ObjectData->GetString( "SYS_LOTTERY_RUN" ) , Info.Version, Info.Num[0], Info.Num[1], Info.Num[2], Info.Num[3], Info.Num[4] ) ;

	g_pGameMain->SendScrollBannerMessage( tt );
}

// ----------------------------------------------------------------------------
void CLotteryFrame::BuyLottery()
{
	int Money = 0;
	int Count = 0;
	int Number[20];	
	ZeroMemory( Number , sizeof( Number ) );

	int i;
	for( i = 0 ; i < 20 ; ++i )
	{
		if( m_Number[i] )
		{
			Number[Count++] = i + 1;	
		}
	}

	LotteryInfoTableStruct* pLotteryInfo = g_ObjectData->LotteryInfoTable( Count );

	if( !pLotteryInfo )
	{
		return ;
	}

	NetCli_Lottery::SL_BuyLottery( pLotteryInfo->CombinationCount * DEF_LOTTERY_COST , Count , Number );

}


// ----------------------------------------------------------------------------
void CLotteryFrame::ExchangeLottery()
{

	ClipboardItemInfo itemInfo = g_pItemClipboard->GetItemInfo();
	ClipboardItemType itemType = g_pItemClipboard->GetType();

	ItemFieldStruct* item = NULL;

	//if( itemType == eClipboardItem_Type_Bag || ( itemType == eClipboardItem_Type_Action && itemInfo.action.type == eActionItemType_Bag ) )
	if( itemType == eClipboardItem_Type_Bag )
	{

		g_pItemClipboard->Clear();
		if ( (item = CNetGlobal::RoleData()->GetBodyItem(itemInfo.bag.pos)) != NULL )
		{
			GameObjDbStructEx* itemDB = CNetGlobal::GetObj(item->OrgObjID);
			if( itemDB &&
				itemDB->Item.Class == EM_Item_Normal &&
				itemDB->Item.ItemType == EM_ItemType_Lottery )
			{
				m_itemPickupExchange = *item;
				m_bagpos = itemInfo.bag.pos;
				ExchangeSlotChanged();
			}
			else
			{

				return;
			}

		}


	}
	else if( itemType == eClipboardItem_Type_None )
	{


	}
	else
	{

	}

}


// ----------------------------------------------------------------------------
void CLotteryFrame::ExchangeSlotChanged()
{
	lua_State* L = GetLuaStateWorld();
	SendWorldEvent("Lottery_Exchange_SLOT_CHANGED");
}


// ----------------------------------------------------------------------------
void CLotteryFrame::ConfirmExchange()
{
	if( m_bagpos >= 0 )
	{
		NetCli_Lottery::SL_ExchangePrize( m_itemPickupExchange , m_bagpos );
		m_itemPickupExchange.Init();
		m_bagpos = -1;
		ExchangeSlotChanged();
	}
}

// ----------------------------------------------------------------------------
void CLotteryFrame::CancelExchange()
{
	m_itemPickupExchange.Init();
	m_bagpos = -1;
	ExchangeSlotChanged();
}

// ----------------------------------------------------------------------------
int LuaFunc_Lottery_GetCurrentVersion( lua_State* L )
{
	lua_pushnumber( L, g_pLotteryFrame->m_BuyLottery_Version );
	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_Lottery_GetMaxPrizeMoney( lua_State* L )
{
	lua_pushnumber( L, g_pLotteryFrame->m_BuyLottery_MaxPrizeMoney );
	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_Lottery_GetLastInfo( lua_State* L )
{
	//struct LotteryInfoStruct
	//{
	//	int Version;				//第幾期 ( -1 代表沒有 )
	//	int	Num[5];
	//	int PrizeMoney[3];			//中獎總彩金
	//	int PrizePlayerCount[3];	//中獎人數
	//	int	RunLotteryTime;			//開獎日
	//	int TotalMoney;
	//};

	int index = (int)luaL_checknumber( L, 1 ) - 1;

	if( index >= 0 && index < 5 )
	{
		if( g_pLotteryFrame->m_ExchangePrize_LastInfo[index].Version == -1 )
		{
			return 0;
		}

		lua_pushnumber( L, g_pLotteryFrame->m_ExchangePrize_LastInfo[index].Version );
		lua_pushnumber( L, g_pLotteryFrame->m_ExchangePrize_LastInfo[index].Num[0]  );
		lua_pushnumber( L, g_pLotteryFrame->m_ExchangePrize_LastInfo[index].Num[1]  );
		lua_pushnumber( L, g_pLotteryFrame->m_ExchangePrize_LastInfo[index].Num[2]  );
		lua_pushnumber( L, g_pLotteryFrame->m_ExchangePrize_LastInfo[index].Num[3]  );
		lua_pushnumber( L, g_pLotteryFrame->m_ExchangePrize_LastInfo[index].Num[4]  );
		lua_pushnumber( L, g_pLotteryFrame->m_ExchangePrize_LastInfo[index].PrizeMoney[0]  );
		lua_pushnumber( L, g_pLotteryFrame->m_ExchangePrize_LastInfo[index].PrizeMoney[1]  );
		lua_pushnumber( L, g_pLotteryFrame->m_ExchangePrize_LastInfo[index].PrizeMoney[2]  );
		lua_pushnumber( L, g_pLotteryFrame->m_ExchangePrize_LastInfo[index].PrizePlayerCount[0]  );
		lua_pushnumber( L, g_pLotteryFrame->m_ExchangePrize_LastInfo[index].PrizePlayerCount[1]  );
		lua_pushnumber( L, g_pLotteryFrame->m_ExchangePrize_LastInfo[index].PrizePlayerCount[2]  );

		return 12;
	}
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_Lottery_CloseExchangePrize( lua_State* L )
{
	NetCli_Lottery::SL_CloseExchangePrize();
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_Lottery_CloseBuyLottery( lua_State* L )
{
	NetCli_Lottery::SL_CloseBuyLottery();
	return 0;
}

// ----------------------------------------------------------------------------
int luaFunc_Lottery_NumberClear( lua_State* L )
{
	ZeroMemory( g_pLotteryFrame->m_Number , sizeof( g_pLotteryFrame->m_Number ) );

	return 0;
}

// ----------------------------------------------------------------------------
int luaFunc_Lottery_NumberSelect( lua_State* L )
{
	int index = (int)luaL_checknumber( L, 1 ) - 1;

	if( index >= 0 && index < 20 )
	{
		g_pLotteryFrame->m_Number[index] = 1;
	}

	return 0;
}

// ----------------------------------------------------------------------------
int luaFunc_Lottery_NumberCancel( lua_State* L )
{
	int index = (int)luaL_checknumber( L, 1 ) - 1;

	if( index >= 0 && index < 20 )
	{
		g_pLotteryFrame->m_Number[index] = 0;
	}

	return 0;
}

// ----------------------------------------------------------------------------
int luaFunc_Lottery_NumberCount( lua_State* L )
{
	int count = 0;
	int i;
	for( i = 0 ; i < 20 ; ++i )
	{
		if( g_pLotteryFrame->m_Number[i] )
		{
			++count;
		}
	}
	lua_pushnumber( L, count );
	return 1;
}

// ----------------------------------------------------------------------------
int luaFunc_Lottery_GetCombinationCount( lua_State* L )
{
	int count = 0;
	int i;
	for( i = 0 ; i < 20 ; ++i )
	{
		if( g_pLotteryFrame->m_Number[i] )
		{
			++count;
		}
	}

	LotteryInfoTableStruct* pLotteryInfo = g_ObjectData->LotteryInfoTable( count );

	if( !pLotteryInfo )
	{
		lua_pushnumber( L, 0 );
		return 1;
	}

	lua_pushnumber( L, pLotteryInfo->CombinationCount );
	return 1;
}

// ----------------------------------------------------------------------------
int luaFunc_Lottery_GetCost( lua_State* L )
{
	lua_pushnumber( L, DEF_LOTTERY_COST );
	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_Lottery_BuyLottery( lua_State* L )
{
	g_pLotteryFrame->BuyLottery();
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_Lottery_ExchangeLottery( lua_State* L )
{
	g_pLotteryFrame->ExchangeLottery();

	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_Lottery_ConfirmExchange( lua_State* L )
{
	g_pLotteryFrame->ConfirmExchange();
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_Lottery_CancelExchange( lua_State* L )
{
	g_pLotteryFrame->CancelExchange();
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_Lottery_DeleteLottery( lua_State* L )
{
	if( g_pLotteryFrame->m_bagpos >= 0 )
	{
		GameObjDbStructEx* itemDB = CNetGlobal::GetObj( g_pLotteryFrame->m_itemPickupExchange.OrgObjID );
		if( itemDB &&
			itemDB->Item.Class == EM_Item_Normal &&
			itemDB->Item.ItemType == EM_ItemType_Lottery )
		{
			NetCli_Item::_DeleteItem_Body( g_pLotteryFrame->m_itemPickupExchange , g_pLotteryFrame->m_bagpos );
			g_pGameMain->SendSystemMsg( g_ObjectData->GetString( "SYS_LOTTERY_DELETE" ) );
		}
	}

	g_pLotteryFrame->CancelExchange();
	return 0;
}

#define DF_Lottery_NotRunLottery "NOT_RUN_LOTTERY"
#define DF_Lottery_Overdue       "OVERDUE"
#define DF_Lottery_NotWinLotter  "NOT_WIN_LOTTER"
#define DF_Lottery_WinLotter     "WIN_LOTTER"

int LuaFunc_Lottery_GetExchangeSlotInfo( lua_State* L )
{
	ItemFieldStruct* item = &g_pLotteryFrame->m_itemPickupExchange;
	GameObjDbStruct* itemDB = CNetGlobal::GetObj(item->OrgObjID);
	if( NULL == itemDB)
	{
		return 0;
	}


	lua_pushnumber(L, g_pLotteryFrame->m_bagpos + 1);
	lua_pushnumber(L, item->Lottery.Index);

	GameObjDbStruct* imageDB = CNetGlobal::GetObj(itemDB->ImageID);
	if ( imageDB )
	{
		lua_pushstring(L, imageDB->Image.ACTField);
	}
	else
	{		
		lua_pushnil(L);
	}	

	//對獎
	int index = g_pLotteryFrame->m_ExchangePrize_LastInfo[0].Version - item->Lottery.Index;

	//未開獎
	//not run lottery
	if( index < 0 )
	{
		lua_pushstring(L, DF_Lottery_NotRunLottery);
		return 4;
	}

	//過期
	//overdue
	if( index > 4 )
	{
		lua_pushstring(L, DF_Lottery_Overdue);
		return 4;
	}

	LotteryInfoStruct& LastInfo = g_pLotteryFrame->m_ExchangePrize_LastInfo[index];

	int winNumbers = 0;

	int i,n;
	for( i = 0 ; i < 5 ; ++i )
	{
		for( n = 0 ; n < item->Lottery.Count ; ++n )
		{
			if( LastInfo.Num[i] == item->Lottery.SelectID[n] )
			{
				++winNumbers;
				break;
			}
		}
	}

	LotteryInfoTableStruct* pLotteryInfo = g_ObjectData->LotteryInfoTable( item->Lottery.Count );

	//沒中獎
	//not win lotter
	if( pLotteryInfo->SelectPrize[winNumbers][0] == 0 &&
		pLotteryInfo->SelectPrize[winNumbers][1] == 0 &&
		pLotteryInfo->SelectPrize[winNumbers][2] == 0 )
	{
		lua_pushstring(L, DF_Lottery_NotWinLotter);
		return 4;
	}

	//中獎
	//win lotter
	else
	{
		lua_pushstring(L, DF_Lottery_WinLotter);

		lua_pushnumber( L, pLotteryInfo->SelectPrize[winNumbers][0] );
		lua_pushnumber( L, pLotteryInfo->SelectPrize[winNumbers][1] );
		lua_pushnumber( L, pLotteryInfo->SelectPrize[winNumbers][2] );
		lua_pushnumber( L, LastInfo.PrizeMoney[0] );
		lua_pushnumber( L, LastInfo.PrizeMoney[1] );
		lua_pushnumber( L, LastInfo.PrizeMoney[2] );

		return 10;
	}

}



//static void SL_ExchangePrize			( ItemFieldStruct&	Item , int Pos );
//static void SL_BuyLottery( int Money , int Count , int Number[20] );


//g_ObjectData























