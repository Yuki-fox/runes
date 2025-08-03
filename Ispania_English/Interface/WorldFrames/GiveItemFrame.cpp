#include <time.h>
#include "../MainProc/GameMain.h"
#include "../../netwaker_member/NetWakerClientInc.h"
#include "../Login/CharacterSelect.h"
#include "ChatFrame.h"
#include "ItemClipboard.h"
#include "BagFrame.h"
#include "GiveItemFrame.h"

CGiveItemFrame*			g_pGiveItemFrame = NULL;

int LuaFunc_GetGiveItem         (lua_State* L);
int LuaFunc_ClickGiveItemButton (lua_State* L);
int LuaFunc_GiveItemSure        (lua_State* L);
int LuaFunc_GiveItemCancel      (lua_State* L);

// ----------------------------------------------------------------------------
CGiveItemFrame::CGiveItemFrame(CInterface* object)
	: CInterfaceBase(object)
{
	m_npcID				= 0;

	g_pGiveItemFrame		= this;


	m_Count = 0;
	for( int i = 0 ; i < DF_GiveItemMax ; ++i )
	{
		m_BagItem[i] = -1;
	}
}

// ----------------------------------------------------------------------------
CGiveItemFrame::~CGiveItemFrame()
{
	g_pGiveItemFrame = NULL;
}

// ----------------------------------------------------------------------------
void CGiveItemFrame::RegisterFunc()
{
	lua_State* L = GetLuaStateWorld();
	if (L)
	{
		lua_register(L, "GetGiveItem"         , LuaFunc_GetGiveItem         );
		lua_register(L, "ClickGiveItemButton" , LuaFunc_ClickGiveItemButton );
 		lua_register(L, "GiveItemSure"        , LuaFunc_GiveItemSure        );
 		lua_register(L, "GiveItemCancel"      , LuaFunc_GiveItemCancel      );
	}
}

// ----------------------------------------------------------------------------
void CGiveItemFrame::Update(float elapsedTime)
{
	if ( m_npcID && g_pGameMain->GetDistanceWithPlayer(m_npcID) > _MAX_ROLE_TOUCH_MAXRANGE_ )
	{
		SendWorldEvent("AUCTION_CLOSE");
	}
}

// ----------------------------------------------------------------------------
void CGiveItemFrame::Pickup( int index )
{
	if( index < 0 || index >= m_Count )
	{
		return ;
	}

	int itemIndex = GetGiveItemIndex( index );	

	SetGiveItemIndex( index , -1 );

	g_pBagFrame->Cut(itemIndex);
}

// ----------------------------------------------------------------------------
void CGiveItemFrame::PutDown( int index, int BagItemIndex )
{	
	if ( GetGiveItemIndex( index ) != -1 ) {
		// 拿取上一個舊物品
		Pickup( index );
	} else {
		// 更新物品不是背包鎖定
		g_pBagFrame->ClearPickupItemIndex();
	}

	// 鎖定該物品
	g_pBagFrame->Locked( BagItemIndex );

	// 設定販賣物品
	SetGiveItemIndex( index, BagItemIndex );
}

// ----------------------------------------------------------------------------
void CGiveItemFrame::OpenGiveItemFrame(  bool IsSort , int Count , int TargetGUID  )
{
	m_IsSort = IsSort;
 	m_Count = Count;
	m_npcID = TargetGUID;

	lua_State* L = m_interface->GetUi()->GetLuaState();
  
	lua_pushnumber( L, Count );
	lua_setglobal( L, "count" );

	SendWorldEvent("GIVEITEMFRAME_OPEN");
}

// ----------------------------------------------------------------------------
void CGiveItemFrame::CloseGiveItemFrame()
{
	m_Count = 0;

	if ( m_npcID )
	{
		m_npcID = 0;

		for( int i = 0 ; i < DF_GiveItemMax ; ++i )
		{
			if( m_BagItem[i] != -1 )
			{
				g_pBagFrame->Unlocked( m_BagItem[i] );
				m_BagItem[i] = -1;
			}
		}

		NetCli_Other::S_GiveItemPlot( m_IsSort , m_BagItem );
	}
}

// ----------------------------------------------------------------------------
void CGiveItemFrame::S_GiveItem()
{
	if ( m_npcID )
	{
 		for( int i = 0 ; i < DF_GiveItemMax ; ++i )
		{
			if( m_BagItem[i] != -1 )
			{
				g_pBagFrame->Unlocked( m_BagItem[i] );
			}
		}

		NetCli_Other::S_GiveItemPlot( m_IsSort , m_BagItem );
  
		for( int i = 0 ; i < DF_GiveItemMax ; ++i )
		{
			if( m_BagItem[i] != -1 )
			{
				m_BagItem[i] = -1;
			}
		}
		m_Count = 0;
		m_npcID = 0;
 	}
}

// ----------------------------------------------------------------------------
void CGiveItemFrame::SetGiveItemIndex( int index, int BagItemIndex )
{
	if( index < 0 || index >= m_Count )
	{
		return ;
	}

	if ( BagItemIndex != m_BagItem[index] )
	{
		if( m_BagItem[index] != -1 )
			g_pBagFrame->Unlocked( m_BagItem[index] );
		m_BagItem[index] = BagItemIndex;
		SendWorldEvent("GIVEITEMFRAME_UPDATE");
	}
}

// ----------------------------------------------------------------------------
int  CGiveItemFrame::GetGiveItemIndex( int index )
{
	if( index < 0 || index >= m_Count )
	{
		return -1;
	}

	return m_BagItem[index];
}

// ----------------------------------------------------------------------------
int LuaFunc_GetGiveItem(lua_State* L)
{
	const char* name = "";
	const char* texture = "";
	int stack = 0;
	int sellPrice = 0;

  	int index = luaL_checkint(L, 1) - 1;

	int BodyItemID = g_pGiveItemFrame->GetGiveItemIndex( index );
	ItemFieldStruct* itemData = CNetGlobal::RoleData()->GetBodyItem(BodyItemID);
	if ( itemData )
	{
		GameObjDbStruct* itemDB = CNetGlobal::GetObj(itemData->OrgObjID);
		if (itemDB)
		{
			GameObjDbStruct* imageDB = CNetGlobal::GetObj(itemDB->ImageID);
			if ( imageDB )
			{
				texture = imageDB->Image.ACTField;
			}
			
			stack		= itemData->Count;
			lua_pushnumber(L, BodyItemID + 1 );
			lua_pushstring(L, texture);
			lua_pushnumber(L, max(stack, 1));
			return 3;
		}
	}	
	return 0;
}


// ----------------------------------------------------------------------------
int LuaFunc_ClickGiveItemButton(lua_State* L)
{
	int index = luaL_checkint(L, 1) - 1;

	ClipboardItemInfo itemInfo = g_pItemClipboard->GetItemInfo();

	switch ( g_pItemClipboard->GetType() )
	{
		// pickup item
	case eClipboardItem_Type_None:
		g_pGiveItemFrame->Pickup( index );
		break;

		//
	case eClipboardItem_Type_Bag:
		g_pItemClipboard->Clear();
		g_pGiveItemFrame->PutDown( index, itemInfo.bag.pos );
		break;

	default:
		break;
	}
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GiveItemSure(lua_State* L)
{
	g_pGiveItemFrame->S_GiveItem();
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GiveItemCancel(lua_State* L)
{
	g_pGiveItemFrame->CloseGiveItemFrame();
	return 0;
}