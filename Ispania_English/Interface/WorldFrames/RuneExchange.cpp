#include "RuneExchange.h"
#include "../../mainproc/GameMain.h"
#include "../../netwaker_member/NetWakerClientInc.h"
#include <map>
#include "ItemClipboard.h"
#include "../../netwaker_member/net_item/NetCli_Item.h"
#include "BagFrame.h"
#include "RoleData/RoleBaseDef.h"

//#define UiEvent_RuneExchangePlate  "RUNE_EXCHANGE_PLATE"  


//****************************************************************************
CRuneExchangeFrame*	g_pRuneExchangeFrame = NULL;

int LuaFunc_RuneExchangeGetListInfo(lua_State* L);
int LuaFunc_RuneExchangePushInPlate(lua_State* L);
int LuaFunc_RuneExchangeGetPlateInfo(lua_State* L);
int LuaFunc_RuneExchangeDelPlateItem(lua_State* L);
int LuaFunc_RuneExchangeOk(lua_State* L);
int LuaFunc_CloseRuneExchange(lua_State* L);

//****************************************************************************
CRuneExchangeFrame::CRuneExchangeFrame(CInterface* object): CInterfaceBase(object)
{
	int i;
	for( i = 0 ; i < EQUIP_PLATE_MAX_ITEM ; i++ )
	{
		m_RunePlateItem[i].BagPos = -1;	
		m_RunePlateItem[i].itemID = 0;	
	}
	m_ItemLvType = -1;
	m_RE_Visible = false;
	m_npcID = -1;
	g_pRuneExchangeFrame = this;
}
//****************************************************************************
CRuneExchangeFrame::~CRuneExchangeFrame()
{
	g_pRuneExchangeFrame = NULL;
}
//****************************************************************************
void CRuneExchangeFrame::RegisterFunc()
{
	lua_State* L = GetLuaStateWorld();
	if (L)
	{	
		lua_register(L, "RuneExchangeGetListInfo",		LuaFunc_RuneExchangeGetListInfo		);
		lua_register(L, "RuneExchangeGetPlateInfo",		LuaFunc_RuneExchangeGetPlateInfo	);

		lua_register(L, "RuneExchangePushInPlate",		LuaFunc_RuneExchangePushInPlate		);
		lua_register(L, "RuneExchangeDelPlateItem",		LuaFunc_RuneExchangeDelPlateItem	);
		lua_register(L, "RuneExchangeOk",				LuaFunc_RuneExchangeOk				);
		lua_register(L, "CloseRuneExchange",			LuaFunc_CloseRuneExchange			);
	}
}
// ----------------------------------------------------------------------------
void CRuneExchangeFrame::Open(int npcID)
{
	m_npcID = npcID;
	m_RE_Visible = true;
	// 開啟界面
	SendWorldEvent("RUNE_EXCHANGE_FRAME_OPEN");
}

// ----------------------------------------------------------------------------
void CRuneExchangeFrame::Close()
{
	m_RE_Visible = false;

	int i;
	for( i = 0 ; i < EQUIP_PLATE_MAX_ITEM ; i++)
	{
		if( m_RunePlateItem[i].itemID != 0 )
		{
			g_pBagFrame->Unlocked(m_RunePlateItem[i].BagPos);
		}
	}
	NetCli_Item::SL_CloseRare3EqExchangeItem();
}
void CRuneExchangeFrame::Update(float elapsedTime)
{
	if ( m_npcID && g_pGameMain->GetDistanceWithPlayer(m_npcID) > _MAX_ROLE_TOUCH_MAXRANGE_ )
	{
		SendWorldEvent("RUNE_EXCHANGE_FRAME_CLOSE");
	}
}

//****************************************************************************
int	CRuneExchangeFrame::GetRunePlateItem(int index)
{
	int isize = 0 , i;
	if( index == -1 )
	{
		for( i = 0 ; i < EQUIP_PLATE_MAX_ITEM ; i++)
		{
			if( g_pRuneExchangeFrame->m_RunePlateItem[i].itemID != 0 )
			{
				isize++;
			}
		}
		return isize;
	}
	else if( index >= 0 && index < EQUIP_PLATE_MAX_ITEM )
	{
		if( g_pRuneExchangeFrame->m_RunePlateItem[index].itemID != 0 )
		{		
			return g_pRuneExchangeFrame->m_RunePlateItem[index].itemID;
		}
	}
	return -1;
}
//****************************************************************************
void CRuneExchangeFrame::SetRunePlateItem( int index , int itemID , int ItemPos)
{
	int i ,j , k;
	k = 0 ;
	if( index >= 0 && index < EQUIP_PLATE_MAX_ITEM )
	{
		if( itemID == 0)
		{
			g_pBagFrame->Unlocked(m_RunePlateItem[index].BagPos);
			m_RunePlateItem[index].itemID = itemID;
			m_RunePlateItem[index].BagPos = ItemPos ;
		}
		else
		{
			for( j = 0 ; j < EQUIP_PLATE_MAX_ITEM ; j++ )
			{
				if( m_RunePlateItem[j].BagPos == ItemPos  )
				{
					k = 1;
				}
			}
			if( m_RunePlateItem[index].itemID != 0 )
			{
				g_pBagFrame->Unlocked(m_RunePlateItem[index].BagPos);
			}
			if( k == 0 )
			{
				m_RunePlateItem[index].itemID = itemID;
				m_RunePlateItem[index].BagPos = ItemPos;			
			}
			g_pBagFrame->Locked(ItemPos);
		}
		if( k == 1 )
		{
			g_pGameMain->SendWarningMsg(g_ObjectData->GetString("RUNE_EXCHANGE_FAILED_02"));	//裝備重複!
		}
		SendWorldEvent("RUNE_EXCHANGE_PLATE");
	}

	k = 0;
	for( i = 0 ; i < EQUIP_PLATE_MAX_ITEM ; i++ )
	{
		if( m_RunePlateItem[i].itemID != 0 )
		{
			k = 1;
			break;
		}
	}
	if( k != 1 ) { g_pRuneExchangeFrame->m_ItemLvType = -1;}
}
//****************************************************************************
bool CRuneExchangeFrame::RE_CheckEqItem( int itemID ,int  pos)
{
	GameObjDbStruct* itemObj = CNetGlobal::GetObj(itemID);
	if( itemObj ) 
	{
		if( itemObj->Rare == 3 )
		{
			switch (RoleDataEx::CheckDissolution(NULL,CNetGlobal::RoleData()->GetBodyItem(pos)))
			{
			case EM_CheckDissolutionResult_Err:			//此物品不可拆解
				g_pGameMain->SendWarningMsg(g_ObjectData->GetString("RUNE_EXCHANGE_FAILED_01"));	//請放入正確裝備!
				return false;
			case EM_CheckDissolutionResult_OK:
				switch(g_pRuneExchangeFrame->m_ItemLvType)
				{
				case -1:          //代表是第一個放入的裝備
					if( itemObj->Limit.Level >= EQUIP_PLATE_LEVEL_01_1 && itemObj->Limit.Level <= EQUIP_PLATE_LEVEL_01_2 ) //31~50
					{
						g_pRuneExchangeFrame->m_ItemLvType = 0;
						return true;
					}
					if( itemObj->Limit.Level >= EQUIP_PLATE_LEVEL_02_1 && itemObj->Limit.Level <= EQUIP_PLATE_LEVEL_02_2 )   //51~80
					{
						g_pRuneExchangeFrame->m_ItemLvType = 1;
						return true;
					}

					if( itemObj->Limit.Level >= EQUIP_PLATE_LEVEL_03_1 && itemObj->Limit.Level <= EQUIP_PLATE_LEVEL_03_2 )   //81~100
					{
						g_pRuneExchangeFrame->m_ItemLvType = 2;
						return true;
					}
					/*else
					{
						g_pGameMain->SendWarningMsg(g_ObjectData->GetString("RUNE_EXCHANGE_FAILED_01"));	//請放入等級51~70的紫色裝備		
						return false;
					}*/
					break;
				case EQUIP_PLATE_LEVEL_31_50:
					if( itemObj->Limit.Level >= EQUIP_PLATE_LEVEL_01_1 && itemObj->Limit.Level <= EQUIP_PLATE_LEVEL_01_2 )  //31~50
					{
						return true;
					}
					else if( itemObj->Limit.Level > 1)
					{
						g_pGameMain->SendWarningMsg(g_ObjectData->GetString("RUNE_EXCHANGE_FAILED_06"));	//請放入等級51~70的紫色裝備		
						return false;
					}
					break;
				case EQUIP_PLATE_LEVEL_51_80:
					if( itemObj->Limit.Level >= EQUIP_PLATE_LEVEL_02_1 && itemObj->Limit.Level <= EQUIP_PLATE_LEVEL_02_2 )   //51~80
					{
						return true;
					}
					else if( itemObj->Limit.Level > 1)
					{
						g_pGameMain->SendWarningMsg(g_ObjectData->GetString("RUNE_EXCHANGE_FAILED_12"));	//請放入等級51~70的紫色裝備		
						return false;
					}
					break;
				case EQUIP_PLATE_LEVEL_81_100:
					if( itemObj->Limit.Level >= EQUIP_PLATE_LEVEL_03_1 && itemObj->Limit.Level <= EQUIP_PLATE_LEVEL_03_2 )   //81~100
					{
						return true;
					}
					else if( itemObj->Limit.Level > 1)
					{
						g_pGameMain->SendWarningMsg(g_ObjectData->GetString("RUNE_EXCHANGE_FAILED_11"));	//請放入等級81~100的紫色裝備		
						return false;
					}
					break;
				}
				g_pGameMain->SendWarningMsg(g_ObjectData->GetString("RUNE_EXCHANGE_FAILED_01"));	//請放入正確裝備!
			}
		}
		else
		{
			g_pGameMain->SendWarningMsg(g_ObjectData->GetString("RUNE_EXCHANGE_FAILED_01"));	//請放入正確裝備!
			return false;

		}
	}
	
	return false;
}
//****************************************************************************
int LuaFunc_RuneExchangePushInPlate(lua_State* L)
{
	int index = luaL_checkint(L, 1) - 1 ;
	int itemID , temp = 0;

	switch(g_pItemClipboard->GetType())
	{
	case eClipboardItem_Type_Bag:
		ClipboardItemInfo itemInfo = g_pItemClipboard->GetItemInfo();
		itemID = itemInfo.bag.itemID;
		if( g_pRuneExchangeFrame->RE_CheckEqItem( itemID , itemInfo.bag.pos) )
		{
			g_pRuneExchangeFrame->SetRunePlateItem( index , itemID ,itemInfo.bag.pos );
		}
		g_pItemClipboard->Clear();
		temp = 1;
		break;
	}
	if( temp == 0 )
	{
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("RUNE_EXCHANGE_FAILED_10"));	
	}

	return 0;
}

//****************************************************************************
int LuaFunc_RuneExchangeGetListInfo(lua_State* L)
{
	int index = luaL_checkint(L, 1) - 1 ;
	int count = 11;

	if( index == -1 )
	{
		lua_pushnumber(L, count);
		return 1;
	}
	else
	{
		lua_pushnumber(L, index);
		return 1;
	}
}
//****************************************************************************
int CRuneExchangeFrame::RE_GetCost()
{
	int cost = 0 , i , k = 0; 

	for( i = 0 ; i < EQUIP_PLATE_MAX_ITEM ; i++ )
	{
		if( m_RunePlateItem[i].itemID != 0 )
		{
			GameObjDbStruct* itemObj = CNetGlobal::GetObj(m_RunePlateItem[i].itemID);
			if( itemObj ) 
			{
				cost += (itemObj->Limit.Level / 10 )*(itemObj->Limit.Level / 10 )*1000 + 5000;
				k++;
			}
		}
	}
	if( k < 3)
	{
		cost = 0;
	}
	
	return cost;
}


int LuaFunc_RuneExchangeGetPlateInfo(lua_State* L)
{
	int index = luaL_checkint(L, 1) - 1 ;
	int itemID;
	const char* texture = "";
	int money = 0;
	if( index == -1 )
	{
		money = g_pRuneExchangeFrame->RE_GetCost();
		lua_pushnumber(L, money);	
		return 1;
	}
	else if( index >= 0 && index < EQUIP_PLATE_MAX_ITEM )
	{
		itemID = g_pRuneExchangeFrame->GetRunePlateItem(index);
		if( itemID != -1 )
		{
			GameObjDbStruct* itemObj = CNetGlobal::GetObj(itemID);
			GameObjDbStruct* itemIconObj = CNetGlobal::GetObj(itemObj->ImageID);
			texture = itemIconObj->Image.ACTField;
			if( texture )
			{
				lua_pushstring(L, texture);     //圖片    
				return 1;
			}
		}
	}
	return 0;
}
//****************************************************************************
int LuaFunc_RuneExchangeDelPlateItem(lua_State* L)
{
	int index = luaL_checkint(L, 1) - 1 ;
	if( index >= 0 && index < EQUIP_PLATE_MAX_ITEM )
	{
		g_pRuneExchangeFrame->SetRunePlateItem( index , 0, -1);
	}
	return 0;
}
//****************************************************************************
int LuaFunc_RuneExchangeOk(lua_State* L)
{
	int itype = luaL_checkint(L, 1) ;
	
	if( itype >=0 && itype <= 10)
	{
		g_pRuneExchangeFrame->CheckRE_EqItem(itype);
	}
	return 0;
}

void CRuneExchangeFrame::CheckRE_EqItem( int itype )
{
	int i ,  ipos = 0;
	vector<int>  ItemPos;

	for( i = 0 ; i < EQUIP_PLATE_MAX_ITEM ; i++ )
	{
		if( g_pRuneExchangeFrame->m_RunePlateItem[i].BagPos != -1 )
		{
			ItemPos.push_back(g_pRuneExchangeFrame->m_RunePlateItem[i].BagPos);
		}
	}
	if( ItemPos.size() >= 3 )
	{
		NetCli_Item::SL_Rare3EqExchangeItemRequest( itype , ItemPos );
	}
	else
	{
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("RUNE_EXCHANGE_FAILED_08"));	//裝備數量有誤，最少放入3件裝備

	}
}
//****************************************************************************
void CRuneExchangeFrame::SetRE_Init()
{
	int i;
	for( i = 0 ; i < EQUIP_PLATE_MAX_ITEM ; i++ )
	{
		m_RunePlateItem[i].BagPos = -1;	
		m_RunePlateItem[i].itemID = 0;	
	}
	m_ItemLvType = -1;
	g_pInterface->SendWorldEvent("RUNE_EXCHANGE_PLATE");
}
//****************************************************************************
int LuaFunc_CloseRuneExchange(lua_State* L)
{
	g_pRuneExchangeFrame->Close();
	return 0;
}