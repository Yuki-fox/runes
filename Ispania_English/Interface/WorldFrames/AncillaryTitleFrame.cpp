#include "AncillaryTitleFrame.h"
#include "../../mainproc/GameMain.h"
#include "../../netwaker_member/net_item/NetCli_Item.h"

#include "BagFrame.h"
#include "../CraftFrame/UI_CraftFrame.h"
#include "../../netwaker_member/net_other/NetCli_OtherChild.h"

#include "ItemClipboard.h"

//****************************************************************************
CAncillaryTitleFrame*	g_pAncillaryTitleFrame = NULL;

int		LuaFunc_ATF_Open						(lua_State* L);
int		LuaFunc_ATF_Close						(lua_State* L);
	
int		LuaFunc_GetAncillaryTitleInfo			(lua_State* L);
int		LuaFunc_GetATF_Title					(lua_State* L);

int		LuaFunc_SetATF_Title					(lua_State* L);

int		LuaFunc_ATFUseItem_Open					(lua_State* L);
int		LuaFunc_AddATF_UseItem				 	(lua_State* L);
int		LuaFunc_ClearATF_UseItem				(lua_State* L);
int		LuaFunc_OkayATF_UseItem					(lua_State* L);
int		LuaFunc_UpdateATF_UseItem				(lua_State* L);

int		LuaFunc_GetATF_UseInfo					(lua_State* L);
int		LuaFunc_Remove_AncillaryTitle			(lua_State* L);


int		LuaFunc_ATF_FameButton_OnClick			(lua_State* L);
int		LuaFunc_ATF_IsTitleVisible				(lua_State* L);
int		LuaFunc_ATF_IsFrameShow						(lua_State* L);
		

//****************************************************************************
CAncillaryTitleFrame::CAncillaryTitleFrame(CInterface* object): CInterfaceBase(object)
{
	g_pAncillaryTitleFrame = this;
	m_IsVisible = false;
}
//****************************************************************************
CAncillaryTitleFrame::~CAncillaryTitleFrame()
{
	g_pAncillaryTitleFrame = NULL;
}
//****************************************************************************
void CAncillaryTitleFrame::RegisterFunc()
{
	lua_State* L = GetLuaStateWorld();
	if (L)
	{	
		lua_register(L, "ATF_Open",					LuaFunc_ATF_Open					);
		lua_register(L, "ATF_Close",				LuaFunc_ATF_Close					);
		lua_register(L, "GetAncillaryTitleInfo",	LuaFunc_GetAncillaryTitleInfo		);
		lua_register(L, "GetATF_Title",				LuaFunc_GetATF_Title				);
		lua_register(L, "SetATF_Title",				LuaFunc_SetATF_Title				);

		lua_register(L, "ATFUseItem_Open",			LuaFunc_ATFUseItem_Open				);
		lua_register(L, "AddATF_UseItem",			LuaFunc_AddATF_UseItem				);
		lua_register(L, "ClearATF_UseItem",			LuaFunc_ClearATF_UseItem			);
		lua_register(L, "OkayATF_UseItem",			LuaFunc_OkayATF_UseItem				);
		lua_register(L, "GetATF_UseInfo",			LuaFunc_GetATF_UseInfo				);
		lua_register(L, "Remove_AncillaryTitle",	LuaFunc_Remove_AncillaryTitle		);
		lua_register(L, "ATF_FameButton_OnClick",	LuaFunc_ATF_FameButton_OnClick		);
		lua_register(L, "UpdateATF_UseItem",		LuaFunc_UpdateATF_UseItem			);
		lua_register(L, "ATF_IsTitleVisible",		LuaFunc_ATF_IsTitleVisible			);
		lua_register(L, "ATF_IsFrameShow",			LuaFunc_ATF_IsFrameShow			);
		
	}
}
//****************************************************************************
void CAncillaryTitleFrame::Update(float elapsedTime)
{

}
//****************************************************************************
void CAncillaryTitleFrame::Open()
{
	m_IsVisible = true;
	SetATFItem();
	// 開啟界面
	SendWorldEvent("ANCILLARY_TITLE_FRAME_OPEN");
}
//****************************************************************************
void CAncillaryTitleFrame::Close()
{
	m_IsVisible = false;
	SendWorldEvent("ANCILLARY_TITLE_FRAME_CLOSE");	
}
//****************************************************************************
void CAncillaryTitleFrame::SetATFItem()
{
	RoleDataEx* Owner = CNetGlobal::RoleData();
	for( int i = 0 ; i < 4 ; i++ )
	{
		m_TitleID[i] = 0; 
	}
	for( int i = 0 ; i < 4 ; i++ )
	{	 
		m_TitleID[i] = Owner->PlayerBaseData->TitleSys.TitleID[i];
	}		
}
//****************************************************************************
void CAncillaryTitleFrame::R_SetNewTitleSysResult( bool Result )
{
	if( Result )
	{
		SetATFItem();
		SendWorldEvent("ATF_UPDATE");
	}	
}
//****************************************************************************
void CAncillaryTitleFrame::UpdateATF_TitleFrame()
{
	SendWorldEvent("ATF_UPDATE");	
}
//****************************************************************************
int LuaFunc_ATF_Open(lua_State* L)
{
	g_pAncillaryTitleFrame->Open();
	return 0;
}
//****************************************************************************
int LuaFunc_ATF_Close(lua_State* L)
{
	g_pAncillaryTitleFrame->Close();
	return 0;
}
//****************************************************************************
int LuaFunc_GetAncillaryTitleInfo(lua_State* L)
{
	 int Lv = 0;  
	 int Exp = 0; 
	 float Famous = 0; 
	 RoleDataEx* Owner = CNetGlobal::RoleData();
	 
	 if(Owner)
	 {
		 Lv = Owner->PlayerBaseData->TitleSys.Lv;
		 Exp = Owner->PlayerBaseData->TitleSys.Exp;
		 Famous = Owner->PlayerBaseData->TitleSys.Famous;
	 }

	 int Exp_Max = Exp;
	 if( Lv > 0 && Lv <=8 )	
	 {
		 char tt[32];
		 sprintf( tt, "TitleSys_LvExp_0%d" , Lv );
		 Exp_Max = g_ObjectData->GetSysKeyValue( tt );
	 }
	 
	 lua_pushnumber( L, Lv	);		//稀有度
	 lua_pushnumber( L, Exp		);		//完成經驗
	 lua_pushnumber( L, Famous	);		//名聲

	 lua_pushnumber( L, Exp_Max	);		//經驗最大直
	 return 4;
}
//****************************************************************************
int LuaFunc_GetATF_Title(lua_State* L)
{
	int Idx		= luaL_checkint( L, 1 )-1;
	GameObjDbStructEx* TitleObjDB = g_ObjectData->GetObj( g_pAncillaryTitleFrame->GetATFItem(Idx)  );
	int rare;

	if( TitleObjDB )
	{		
		rare = TitleObjDB->Rare;
		GameObjDbStruct* imageDB = CNetGlobal::GetObj( TitleObjDB->ImageID );
		if( imageDB )
		{
			lua_pushstring( L, imageDB->Image.ACTField );	 //稱號Icon
		}
		lua_pushstring( L, TitleObjDB->Name	);	//稱號名稱
		lua_pushnumber( L, rare		);			//稱號稀有度
		return 3;
	}
	else
	{
		lua_pushstring( L, "" );	 //稱號Icon
		lua_pushstring( L, "? ? ?"	);	//稱號名稱
		lua_pushnumber( L, 0		);	
		return 3;
	}
	return 0;
}
//****************************************************************************
int LuaFunc_SetATF_Title(lua_State* L)
{
	int TitleID	= luaL_checkint( L, 1 );
	GameObjDbStructEx* TitleObjDB = g_ObjectData->GetObj( TitleID );

	bool repeat = false;
	if( TitleObjDB )
	{
		for( int i = 0 ; i < 4 ; i++)
		{
			if( TitleID == g_pAncillaryTitleFrame->GetATFItem(i) )
			{
				repeat = true;		
			}
		}

		if( repeat )
		{
			g_pInterface->SendWorldEvent("ATF_Titlt_REPEAT");	
		}
		else
		{
			switch( TitleObjDB->Title.Classify[0] )
			{
			case 0:
				NetCli_OtherChild::SL_SetNewTitleSys( EM_NewTitleSysPos_Quest , TitleID);
				break;
			case 1:
				NetCli_OtherChild::SL_SetNewTitleSys( EM_NewTitleSysPos_Challenge , TitleID);
				break;
			case 3:
				NetCli_OtherChild::SL_SetNewTitleSys( EM_NewTitleSysPos_Sys , TitleID);
				break;
			case 4:
				NetCli_OtherChild::SL_SetNewTitleSys( EM_NewTitleSysPos_Activitie , TitleID);
				break;
			}
		}
	}	
	return 0;
}
//****************************************************************************
//****************************************************************************
//****************************************************************************
//****************************************************************************
//****************************************************************************
//使用物品的介面**************************************************************
//****************************************************************************
void CAncillaryTitleFrame::R_NewTitleSys_UseItemResult( NewTitleSysUseItemResultENUM Result )
{
	switch(Result)   
	{
	case EM_NewTitleSysUseItemResult_OK:
		//g_pGameMain->SendWarningMsg(g_ObjectData->GetString("ATF_USE_ITEM_WARNING01"));
		SetATF_UseItem();
		SendWorldEvent("EM_NewTitleSysUseItemResult_OK");
		break;
	case EM_NewTitleSysUseItemResult_ItemNotFind:
		//g_pGameMain->SendWarningMsg(g_ObjectData->GetString("ATF_USE_ITEM_WARNING02"));
		 SendWorldEvent("EM_NewTitleSysUseItemResult_ItemNotFind");
		break;
	case EM_NewTitleSysUseItemResult_NoEffect:
		//g_pGameMain->SendWarningMsg(g_ObjectData->GetString("ATF_USE_ITEM_WARNING03"));
		SendWorldEvent("EM_NewTitleSysUseItemResult_NoEffect");
		break;
	}
}
//整理符合的背包物品資料
void CAncillaryTitleFrame::SetATF_UseItem()
{
	BodyItemStruct &bodyItems = CNetGlobal::RoleData()->PlayerBaseData->Body;
	GameObjDbStruct* itemDB = g_ObjectData->GetObj(m_ATFUseItem[0].ID);
	if( itemDB ) 
	{
		ItemFieldStruct  &iItemBase = bodyItems.ItemBase[m_ATFUseItem[0].pos - 60];	
		m_ATFUseItem[0].count = iItemBase.Count;
		if( iItemBase.Count <= 0 )
		{
			g_pBagFrame->Unlocked(m_ATFUseItem[0].pos);
			m_ATFUseItem[0].ID = -1;
			m_ATFUseItem[0].pos = -1;
			m_ATFUseItem[0].count = 0;
		}
	}
	else
	{
		g_pBagFrame->Unlocked(m_ATFUseItem[0].pos);
		m_ATFUseItem[0].ID = -1;
		m_ATFUseItem[0].pos = -1;
		m_ATFUseItem[0].count = 0;
	}

	int count = 1;
	for( int i = 1 ; i < 10 ; i++ )
	{
		g_pAncillaryTitleFrame->m_ATFUseItem[i].ID = -1;	
		g_pAncillaryTitleFrame->m_ATFUseItem[i].pos = -1;	
		//m_ATFUseItem[0].count = 0;
	}
	
	for( int i = 0 ; i < _MAX_BODY_COUNT_ - _MAX_BODY_EX_COUNT_ - _MAX_BODY_MAGIC_BOX_COUNT_ ; i++ )
	{
		ItemFieldStruct  &iItemBase = bodyItems.ItemBase[i];
		if(iItemBase.OrgObjID != 0 )
		{
			itemDB = g_ObjectData->GetObj(iItemBase.OrgObjID);
			if ( itemDB != NULL && itemDB->Item.ItemType == EM_ItemType_TitleItem )
			{
				if( count > 9)
				{
					break;
				}
				g_pAncillaryTitleFrame->m_ATFUseItem[count].ID = iItemBase.OrgObjID;
				g_pAncillaryTitleFrame->m_ATFUseItem[count].count = iItemBase.Count;
				g_pAncillaryTitleFrame->m_ATFUseItem[count].pos = i + 60;
				count++;
			}
		}
	}
	UpdateATF_UseItem();
}		 




//清掉主要選取目標
void CAncillaryTitleFrame::ClearATF_UseItem()
{
	g_pBagFrame->Unlocked(g_pAncillaryTitleFrame->m_ATFUseItem[0].pos);
	g_pAncillaryTitleFrame->m_ATFUseItem[0].ID = -1;
	g_pAncillaryTitleFrame->m_ATFUseItem[0].count = -1;
	g_pAncillaryTitleFrame->m_ATFUseItem[0].pos = -1;
	UpdateATF_UseItem();
}		
//加入選取目標
void CAncillaryTitleFrame::AddATF_UseItem(int Idx)
{		
	
	if( Idx == -1 )
	{
		ClipboardItemType itemType = g_pItemClipboard->GetType();
		ClipboardItemInfo itemInfo = g_pItemClipboard->GetItemInfo();	
		switch(g_pItemClipboard->GetType())
		{
		case eClipboardItem_Type_Bag:					// 背包	
			GameObjDbStruct* itemDB = g_ObjectData->GetObj( itemInfo.bag.itemID );
			if( itemDB && itemDB->Item.ItemType == EM_ItemType_TitleItem)
			{
				GameObjDbStruct* ObjDB = g_ObjectData->GetObj( m_ATFUseItem[0].ID );
				if(ObjDB)
				{
					g_pBagFrame->Unlocked(m_ATFUseItem[0].pos);
				}


				g_pAncillaryTitleFrame->m_ATFUseItem[0].ID = itemInfo.bag.itemID;
				g_pAncillaryTitleFrame->m_ATFUseItem[0].pos = itemInfo.bag.pos;	


				g_pBagFrame->Locked( itemInfo.bag.pos );
				BodyItemStruct &bodyItems = CNetGlobal::RoleData()->PlayerBaseData->Body;
				ItemFieldStruct  &iItemBase = bodyItems.ItemBase[itemInfo.bag.pos - 60];
				if( iItemBase.OrgObjID != 0 )
				{
					g_pAncillaryTitleFrame->m_ATFUseItem[0].count = iItemBase.Count;
				}				
				g_pItemClipboard->Clear();
				UpdateATF_UseItem();
			}
			break;
		}
	}
	else
	{

		GameObjDbStruct* itemDB = g_ObjectData->GetObj(g_pAncillaryTitleFrame->m_ATFUseItem[Idx].ID);
		if( itemDB )
		{
			GameObjDbStruct* ObjDB = g_ObjectData->GetObj( m_ATFUseItem[0].ID );
			if(ObjDB)
			{
				g_pBagFrame->Unlocked(m_ATFUseItem[0].pos);
			}
			g_pAncillaryTitleFrame->m_ATFUseItem[0].ID = g_pAncillaryTitleFrame->m_ATFUseItem[Idx].ID;
			g_pAncillaryTitleFrame->m_ATFUseItem[0].count = g_pAncillaryTitleFrame->m_ATFUseItem[Idx].count;
			g_pAncillaryTitleFrame->m_ATFUseItem[0].pos = g_pAncillaryTitleFrame->m_ATFUseItem[Idx].pos;	
			g_pBagFrame->Locked( g_pAncillaryTitleFrame->m_ATFUseItem[0].pos );
			UpdateATF_UseItem();
		}	
	}

} 

void CAncillaryTitleFrame::OkayATF_UseItem(int Idx)
{					
	int GUID = g_pAncillaryTitleFrame->GetATFUseItemID(Idx);
	GameObjDbStruct* itemDB = g_ObjectData->GetObj(GUID);
	if( itemDB )
	{
		int pos = g_pAncillaryTitleFrame->GetATFUseItemPos(Idx);
		NetCli_OtherChild::SL_NewTitleSys_UseItem( 0, pos);
	}
} 
void CAncillaryTitleFrame::UpdateATF_UseItem()
{					
	g_pInterface->SendWorldEvent("ATF_USE_ITEMFRAME_UPDATE");	

} 
int LuaFunc_UpdateATF_UseItem(lua_State* L)
{
	g_pAncillaryTitleFrame->SetATF_UseItem();	
	return 0;
}

int LuaFunc_ATFUseItem_Open(lua_State* L)
{
	g_pAncillaryTitleFrame->SetATF_UseItem();	
	return 0;
}

int LuaFunc_AddATF_UseItem(lua_State* L)
{
	int Idx = luaL_checkint(L, 1) ;
	if( Idx >= -1 && Idx <= 9 )
	{
		g_pAncillaryTitleFrame->AddATF_UseItem(Idx);
	}
	return 0;
}
int LuaFunc_ClearATF_UseItem(lua_State* L)
{
	g_pAncillaryTitleFrame->ClearATF_UseItem();
	return 0;
}
int LuaFunc_OkayATF_UseItem(lua_State* L)
{
	int Idx = luaL_checkint(L, 1) ;
	g_pAncillaryTitleFrame->OkayATF_UseItem(Idx);
	return 0;
} 
int LuaFunc_GetATF_UseInfo(lua_State* L)
{
	for(int i = 1 ; i < 10 ; i++)
	{
		g_pAncillaryTitleFrame->SetATFUseItemID(i,0);
	}
	int count = 0;
	BodyItemStruct &bodyItems = CNetGlobal::RoleData()->PlayerBaseData->Body;
	for( int i = 0 ; i < _MAX_BODY_COUNT_ - _MAX_BODY_EX_COUNT_ - _MAX_BODY_MAGIC_BOX_COUNT_ ; i++ )
	{
		ItemFieldStruct  &iItemBase = bodyItems.ItemBase[i];
		if(iItemBase.OrgObjID != 0 )
		{
			GameObjDbStruct* objDB  = g_ObjectData->GetObj(iItemBase.OrgObjID);
			if ( objDB != NULL && objDB->Item.ItemType == EM_ItemType_TitleItem )
			{
				if( count > 9)
				{
					break;
				}
				g_pAncillaryTitleFrame->SetATFUseItemID(count+1,iItemBase.OrgObjID);
				g_pAncillaryTitleFrame->SetATFUseItemCount(count+1,iItemBase.Count);
				g_pAncillaryTitleFrame->SetATFUseItemPos(count+1,i + 60);
				//g_pAncillaryTitleFrame->m_ATFUseItem[count].ID = iItemBase.OrgObjID;
				//g_pAncillaryTitleFrame->m_ATFUseItem[count].count = iItemBase.Count;
				//g_pAncillaryTitleFrame->m_ATFUseItem[count].pos = i + 60;
				count++;
			}
		}
	}
	int Idx = luaL_checkint(L, 1) ;

	int Guid = g_pAncillaryTitleFrame->GetATFUseItemID(Idx);
	GameObjDbStruct* itemDB = g_ObjectData->GetObj(Guid);
	if( itemDB )
	{
		GameObjDbStruct* imageDB = CNetGlobal::GetObj( itemDB->ImageID );
		if( imageDB )
		{
			lua_pushstring( L, imageDB->Image.ACTField );	 //稱號Icon			
			lua_pushnumber( L, g_pAncillaryTitleFrame->GetATFUseItemCount(Idx));
			return 2;
		}
	}
	return 0;
} 
int LuaFunc_Remove_AncillaryTitle(lua_State* L)
{
	int Idx = luaL_checkint(L, 1) ;
	
	switch( Idx )
	{
	case 1:
		NetCli_OtherChild::SL_SetNewTitleSys( EM_NewTitleSysPos_Quest , 0);
		break;
	case 2:
		NetCli_OtherChild::SL_SetNewTitleSys( EM_NewTitleSysPos_Challenge , 0);
		break;
	case 3:
		NetCli_OtherChild::SL_SetNewTitleSys( EM_NewTitleSysPos_Sys , 0);
		break;
	case 4:
		NetCli_OtherChild::SL_SetNewTitleSys( EM_NewTitleSysPos_Activitie , 0);
		break;
	}
	return 0;
}
int LuaFunc_ATF_FameButton_OnClick(lua_State* L)
{
	RoleDataEx* Owner = CNetGlobal::RoleData();

	if( Owner->PlayerBaseData->TitleSys.Famous >= 100 )
	{
		NetCli_OtherChild::S_RunGlobalPlotRequest( 9 , 0 );
		g_pInterface->SendWorldEvent("ATF_Fame_UPDATE");
	}
	return 0;
}		   


int LuaFunc_ATF_GetATFCount(lua_State* L)
{
	int Idx = luaL_checkint(L, 1) ;
	//g_pCharacterFrame->m_TitleInfos
	return 0;
}
int LuaFunc_ATF_IsTitleVisible(lua_State* L)
{	
	for( int i = 0 ; i < 4 ; i++)
	{
		int GUID = g_pAncillaryTitleFrame->GetATFItem(i);
		GameObjDbStruct* itemDB = g_ObjectData->GetObj(GUID);
		if( itemDB )
		{
			lua_pushboolean( L, true);
			return 1;
		}
	}
	lua_pushboolean( L, false);
	return 1;
}
int LuaFunc_ATF_IsFrameShow(lua_State* L)  {
	int objectID = g_ObjectData->GetSysKeyValue( "TitleSys_OpenKey" );
	if( CNetGlobal::RoleData()->CheckKeyItem( objectID ) ) {
		lua_pushboolean( L, true);	
	}else{
		lua_pushboolean( L, false);
	}	
	return 1;
}