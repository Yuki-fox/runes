#include "../../netwaker_member/NetWakerClientInc.h"
#include "../mainproc/GameMain.h"
#include "ItemClipboard.h"
#include "BagFrame.h"

#include "ItemUTIL.h"
#include "../ItemLink/ItemLink.h"

#include "BankFrame.h"



CBankFrame*			g_pBankFrame = NULL;

int	LuaFunc_GetBankItemInfo			(lua_State* L);
int	LuaFunc_GetBankNumItems			(lua_State* L);
int	LuaFunc_PickupBankItem			(lua_State* L);
int LuaFunc_DrawBankItem			(lua_State* L);
int	LuaFunc_CloseBank				(lua_State* L);

int LuaFunc_OpenBank   				(lua_State* L);
int LuaFunc_OpenAuction			 	(lua_State* L);
int LuaFunc_OpenMail				(lua_State* L);

int luaFunc_TimeLet_GetLetTime		(lua_State* L); 
int luaFunc_TimeLet_StoreUp			(lua_State* L); 
int luaFunc_TimeLet_GetLetInfo		(lua_State* L); 
int LuaFunc_RefreshBank				(lua_State* L);

//ItemLink
int LuaFunc_GetBankItemLink			(lua_State* L);

// ----------------------------------------------------------------------------
CBankFrame::CBankFrame(CInterface* object)
	: CInterfaceBase(object)
{
	m_bankManage		= -1;
	m_pickupItemIndex	= -1;
	m_sortLastTime		= 0.0f;

	g_pBankFrame		= this;	
}

// ----------------------------------------------------------------------------
CBankFrame::~CBankFrame()
{
	g_pBankFrame	= NULL;
}

// ----------------------------------------------------------------------------
void CBankFrame::RegisterFunc()
{
	lua_State* L = GetLuaStateWorld();
	if (L)
	{
		lua_register(L, "GetBankNumItems",		LuaFunc_GetBankNumItems);
		lua_register(L, "GetBankItemInfo",		LuaFunc_GetBankItemInfo);
		lua_register(L, "PickupBankItem",		LuaFunc_PickupBankItem);
		lua_register(L, "DrawBankItem",			LuaFunc_DrawBankItem);
		lua_register(L, "CloseBank",			LuaFunc_CloseBank);

		lua_register(L, "OpenBank",				LuaFunc_OpenBank   			);
		lua_register(L, "OpenAuction",			LuaFunc_OpenAuction			);
		lua_register(L, "OpenMail",				LuaFunc_OpenMail			);


 		lua_register(L, "TimeLet_GetLetTime",	luaFunc_TimeLet_GetLetTime	);
		lua_register(L, "TimeLet_StoreUp",		luaFunc_TimeLet_StoreUp		);
		lua_register(L, "TimeLet_GetLetInfo",	luaFunc_TimeLet_GetLetInfo	); 

  		lua_register(L, "GetBankItemLink",		LuaFunc_GetBankItemLink	); 

		lua_register(L, "RefreshBank",			LuaFunc_RefreshBank	); 
	}
}

// ----------------------------------------------------------------------------
void CBankFrame::Update(float elapsedTime)
{
	static float lastTime = 0.0f;

	if ( m_sortLastTime > 0.01f )
		m_sortLastTime -= elapsedTime;

	lastTime += elapsedTime;
	if ( lastTime < 0.2f )
		return;	

	lastTime = 0.0f;
	if ( m_bankManage >= 0 )
	{
		if (g_pGameMain->GetDistanceWithPlayer(m_bankManage) > _MAX_ROLE_TOUCH_MAXRANGE_)
		{
			SendWorldEvent( UiEvent_BankClose );
		}

		// ��s�������~���A
		if ( m_pickupItemIndex != -1 )
		{
			ItemFieldStruct* item = CNetGlobal::RoleData()->GetBankItem(m_pickupItemIndex);
			if ( item )
			{
				if ( item->Pos == EM_ItemState_CliDisabled )
				{
					ClipboardItemInfo itemInfo = g_pItemClipboard->GetItemInfo();
					if ( g_pItemClipboard->GetType() != eClipboardItem_Type_Bank || itemInfo.bank.pos != m_pickupItemIndex )
					{
						item->Pos = EM_ItemState_NONE;
						m_pickupItemIndex = -1;
					}
				}
			}
			else
				m_pickupItemIndex = -1;
		}
	}
}

// ----------------------------------------------------------------------------
void CBankFrame::Open(int bankManage)
{
	m_bankManage = bankManage;

	// �}�Ҭɭ�
	SendWorldEvent( UiEvent_BankOpen );
}

// ----------------------------------------------------------------------------
void CBankFrame::Close()
{
	switch(g_pItemClipboard->GetType())
	{
	case eClipboardItem_Type_Bank:
		g_pItemClipboard->Clear();
		break;
	}

	if ( m_bankManage >= 0 )
	{
		m_bankManage = -1;

		// �q���Ȧ�����
		NetCli_Item::_CloseBank();
	}
}

// ----------------------------------------------------------------------------
bool CBankFrame::Cut(int index)
{	
	ItemFieldStruct* item = CNetGlobal::RoleData()->GetBankItem(index);
	if ( item && item->Pos == EM_ItemState_NONE )
	{
		GameObjDbStruct* itemDB = CNetGlobal::GetObj( item->OrgObjID );
		if ( itemDB )
		{
			item->Pos = EM_ItemState_CliDisabled;
			//g_pItemClipboard->SetFile(itemDB->ImageObj->ACTField);
			g_pItemClipboard->SetFile( ItemUTIL::GetItemIcon( item ) );
			g_pItemClipboard->PickupBank(item->OrgObjID, index);
			m_pickupItemIndex = index;
			return true;
		}
	}
	return false;
}

// ----------------------------------------------------------------------------
void CBankFrame::Paste(int index)
{
	ItemFieldStruct* destItem = CNetGlobal::RoleData()->GetBankItem(index);
	ClipboardItemInfo itemInfo = g_pItemClipboard->GetItemInfo();

	// �����P���Ϊ��~�ۦP
	if (g_pItemClipboard->GetType() == eClipboardItem_Type_Bank && itemInfo.bank.pos == index)
	{
		g_pItemClipboard->Clear();
		return;
	}

	if ( destItem == NULL || destItem->Pos != EM_ItemState_NONE )
		return;

	// �s�b���~�磌�~�ϥΤ�
	int useIndex = g_pBagFrame->GetUseItemIndex();
	if ( useIndex != -1 )
	{
		if ( !g_pBagFrame->UseItemToItem(2, index) )
			g_pBagFrame->UseItemCancel();
		return;
	}


	if ( CNetGlobal::RoleData()->CheckBankItemEnable(index)	== false )
	{
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_BANK_RENT_ERROR"));
		return;
	}	

	switch(g_pItemClipboard->GetType())
	{	
	case eClipboardItem_Type_None:				// �������~
		Cut(index);
		return;

	case eClipboardItem_Type_Bag:				// �I�] -> �Ȧ�
		Deposit(index, eBank_Type_Bag, itemInfo.bag.pos);
		break;

	case eClipboardItem_Type_Bank:				// �Ȧ� -> �Ȧ�
		Deposit(index, eBank_Type_Bank, itemInfo.bank.pos);
		break;

	case eClipboardItem_Type_Equipment:			// �˳� -> �Ȧ�
		Deposit(index, eBank_Type_Equipment, itemInfo.equipment.pos);
		break;

	case eClipboardItem_Type_Trade:
	case eClipboardItem_Type_Action:
	case eClipboardItem_Type_Skill:
	case eClipboardItem_Type_Shop:
	case eClipboardItem_Type_Macro:
	case eClipboardItem_Type_Emote:
	case eClipboardItem_Type_House:
		break;
	}

	// clear item clipboard data
	g_pItemClipboard->Clear();
}

// ----------------------------------------------------------------------------
// �s�J���~
// index �Ȧ���ޭ�
// srcType �ӷ����~����
// pos ���ޭ�
// formBag �O�_�ӦۭI�]
bool CBankFrame::Deposit(int index, int srcType, int pos, bool toBag, int clientID)
{
	ItemFieldStruct* bankItem = CNetGlobal::RoleData()->GetBankItem(index);
	if ( bankItem && bankItem->Pos == EM_ItemState_NONE )
	{
		ItemFieldStruct* item = NULL;

		switch ( srcType )
		{
		case eBank_Type_Bag:
			item = CNetGlobal::RoleData()->GetBodyItem(pos);			
			if ( item )
			{
				bool ErrorForDepartmentStore=false;

				const char *pErrorID=NULL;
				
				if (index>=200&&index<_MAX_BANK_COUNT_)//�ӫ����~�Ȧ�200~300
				{
					GameObjDbStruct* BagitemDB = g_ObjectData->GetObj(item->OrgObjID);
					if (BagitemDB&& BagitemDB->Mode.DepartmentStore == false) 
					{
						// ������Ȧ�ӫ��I�]
						ErrorForDepartmentStore=true;
						pErrorID="SYS_CANNOT_MOVE_TO_ACCOUNTBANK";
					}
				}
				GameObjDbStruct* itemDB = g_ObjectData->GetObj(bankItem->OrgObjID);
				if ( pos < _MAX_BODY_EX_COUNT_ && bankItem->IsEmpty() == false && (itemDB && itemDB->Mode.DepartmentStore == false) )
				{
					// �����^�ӫ��I�]
						ErrorForDepartmentStore=true;
						pErrorID="SYS_CANNOT_MOVE_TO_ACCOUNTBAG";
				}
				if (ErrorForDepartmentStore&&pErrorID)
				{
					g_pGameMain->SendWarningMsg( g_ObjectData->GetString(pErrorID) );
					return false;
				}
				else
				{
					g_pBagFrame->ClearPickupItemIndex();
					bankItem->Pos = item->Pos = EM_ItemState_CliDisabled;
					if ( toBag )
						NetCli_Item::_ExchangeField_BankBody(*bankItem, index, *item, pos, clientID);
					else
						NetCli_Item::_ExchangeField_BodyBank(*item, pos, *bankItem, index, clientID);
				}
			}
			break;

		case eBank_Type_Bank:
			item = CNetGlobal::RoleData()->GetBankItem(pos);
			if ( item )
			{
				
				bool bDepartmentStore=false;
				if (index>=200&&index<_MAX_BANK_COUNT_)//�ӫ����~�Ȧ�200~300
				{
					GameObjDbStruct* itemDB1 = g_ObjectData->GetObj(item->OrgObjID);
					if ( itemDB1&&itemDB1->Mode.DepartmentStore == false) 
					{
						// ������Ȧ�ӫ��I�]
						bDepartmentStore=true;
						
					}
				}
				if (pos>=200&&pos<_MAX_BANK_COUNT_)//�ӫ����~�Ȧ�200~300
				{
					GameObjDbStruct* itemDB2 = g_ObjectData->GetObj(bankItem->OrgObjID);
					if ( itemDB2&&itemDB2->Mode.DepartmentStore == false) 
					{
						// ������Ȧ�ӫ��I�]
						bDepartmentStore=true;

					}
				}
				
				if (bDepartmentStore)
				{
					g_pGameMain->SendWarningMsg( g_ObjectData->GetString("SYS_CANNOT_MOVE_TO_ACCOUNTBANK") );
					return false;
				}

				else
				{
					bankItem->Pos = item->Pos = EM_ItemState_CliDisabled;
					NetCli_Item::_ExchangeField_Bank(*bankItem, index, *item, pos, clientID);
				}
			}
			break;

		case eBank_Type_Equipment:
			item = CNetGlobal::RoleData()->GetEqItem(pos);
			if ( item )
			{
				bool bDepartmentStore=false;
				if (index>=200&&index<_MAX_BANK_COUNT_)//�ӫ����~�Ȧ�200~300
				{
					GameObjDbStruct* itemDB1 = g_ObjectData->GetObj(item->OrgObjID);
					if ( itemDB1&&itemDB1->Mode.DepartmentStore == false) 
					{
						// ������Ȧ�ӫ��I�]
						bDepartmentStore=true;
					}
				}
				if (bDepartmentStore)
				{
					g_pGameMain->SendWarningMsg( g_ObjectData->GetString("SYS_CANNOT_MOVE_TO_ACCOUNTBANK") );
					return false;
				}

				else
				{
					bankItem->Pos = item->Pos = EM_ItemState_CliDisabled;
					NetCli_EQ::S_SetEQ_Bank(*bankItem, index, pos, true);
				}
				
			}
			break;
		}

		return true;
	}

	return false;
}

// ----------------------------------------------------------------------------
// ���X���~
// �Ȧ���ޭ�
void CBankFrame::Draw(int index)
{
	ItemFieldStruct* bankItem = CNetGlobal::RoleData()->GetBankItem(index);
	if ( bankItem == NULL )
		return;

	int pos = CNetGlobal::RoleData()->SearchBodyFirstFit( bankItem , true );
	if ( pos != -1 )
	{
		if ( Deposit(index, eBank_Type_Bag, pos) )
		{
			g_pRuGlobalEventManager->PlaySoundByFile("sound\\interface\\ui_icon_drop.mp3", false, 0.0f, ruSOUNDTYPE_INTERFACE);
		}
	}
}

// ----------------------------------------------------------------------------
void CBankFrame::SortItem()
{
	if ( m_sortLastTime > 0.01f || m_bankManage == -1 )
		return;

	// ���Ҧ����
	BankItemStruct& bank = CNetGlobal::RoleData()->PlayerBaseData->Bank;
	for ( int i = 0; i < bank.Count; i++ )
	{
		bank.Item[i].Pos = EM_ItemState_CliDisabled;
	}

	// �M���ŶKï�W��ICON
	m_pickupItemIndex = -1;
	if ( g_pItemClipboard->GetType() == eClipboardItem_Type_Bank )
	{
		g_pItemClipboard->Clear();
	}	

	m_sortLastTime = 1.0f;
	NetCli_Other::SL_SortBankRequest();
}

// ----------------------------------------------------------------------------
void CBankFrame::RL_SortBankEnd()
{
	// �Ѱ�Client�Ȯ�����A
	BankItemStruct& bank = CNetGlobal::RoleData()->PlayerBaseData->Bank;
	for ( int i = 0; i < bank.Count; i++ )
	{
		if ( bank.Item[i].Pos == EM_ItemState_CliDisabled )
			bank.Item[i].Pos = EM_ItemState_NONE;
	}
}

// ----------------------------------------------------------------------------
int	LuaFunc_GetBankNumItems(lua_State* L)
{
	int count=CNetGlobal::RoleData()->PlayerBaseData->Bank.Count;
	lua_pushnumber(L, count);
	return 1;
}

// ----------------------------------------------------------------------------
int	LuaFunc_GetBankItemInfo(lua_State* L)
{
	ItemFieldStruct* item = CNetGlobal::RoleData()->GetBankItem(luaL_checkint(L, 1) - 1);
	if ( item )
	{
		GameObjDbStructEx* itemDB = CNetGlobal::GetObj(item->OrgObjID);
		if (itemDB)
		{
			//lua_pushstring(L, itemDB->ImageObj->ACTField);			// push item texture file
 			lua_pushstring(L, ItemUTIL::GetItemIcon( item ) );			// push item texture file
			lua_pushstring(L, itemDB->GetName());						// push Item name
			lua_pushnumber(L, item->GetCount());					// push item count
			lua_pushboolean(L, item->Pos != EM_ItemState_NONE);		// push item is locked
			return 4;
		}
	}
	return 0;
}

// ----------------------------------------------------------------------------
int	LuaFunc_PickupBankItem(lua_State* L)
{
	g_pBankFrame->Paste( luaL_checkint(L, 1) - 1 );
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_DrawBankItem(lua_State* L)
{
	g_pBankFrame->Draw( luaL_checkint(L, 1) - 1 );
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_CloseBank(lua_State* L)
{
	g_pBankFrame->Close();
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_OpenBank(lua_State* L)
{
	NetCli_OtherChild::S_OpenBank();
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_OpenAuction(lua_State* L)
{
	NetCli_OtherChild::S_OpenAuction();
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_OpenMail(lua_State* L)
{
	NetCli_OtherChild::S_OpenMail();
	return 0;
}

// ----------------------------------------------------------------------------
bool GetTimeLetInfo( const char* type , int index , int& outTimeFlag, string& outKey )
{
	if( index < 0 || index >= 20 )
	{
		return false;
	}

	char _Key[256];
	if( 0 == strcmp( type , "BankBag2" ) )
	{
		outTimeFlag = EM_TimeFlagEffect_BankBag2;
		sprintf( _Key , "BankBagLet%02d" , index );
	}
	else if( 0 == strcmp( type , "BankBag3" ) )
	{
		outTimeFlag = EM_TimeFlagEffect_BankBag3;
		sprintf( _Key , "BankBagLet%02d" , index );
	}
	else if( 0 == strcmp( type , "BankBag4" ) )											  
	{
		outTimeFlag = EM_TimeFlagEffect_BankBag4;
		sprintf( _Key , "BankBagLet%02d" , index );
	}
	else if( 0 == strcmp( type , "BankBag5" ) )											  
	{
		outTimeFlag = EM_TimeFlagEffect_BankBag5;
		sprintf( _Key , "BankBagLet%02d" , index );
	}
	else if( 0 == strcmp( type , "BankLet" ) )
	{
		outTimeFlag = EM_TimeFlagEffect_HandyBank;
		sprintf( _Key , "BankLet%02d" , index );
	}
	else if( 0 == strcmp( type , "AuctionLet" ) )
	{
		outTimeFlag = EM_TimeFlagEffect_HandyAuction;
		sprintf( _Key , "AuctionLet%02d" , index );
	}
	else if( 0 == strcmp( type , "MailLet" ) )
	{
		outTimeFlag = EM_TimeFlagEffect_HandyMail;
		sprintf( _Key , "MailLet%02d" , index );
	}
	else if( 0 == strcmp( type , "BagLet3" ) )
	{
		outTimeFlag = EM_TimeFlagEffect_BagPage3;
		sprintf( _Key , "BagLet%02d" , index );
	}
	else if( 0 == strcmp( type , "BagLet4" ) )
	{
		outTimeFlag = EM_TimeFlagEffect_BagPage4;
		sprintf( _Key , "BagLet%02d" , index );
	}
	else if( 0 == strcmp( type , "BagLet5" ) )
	{
		outTimeFlag = EM_TimeFlagEffect_BagPage5;
		sprintf( _Key , "BagLet%02d" , index );
	}
	else if( 0 == strcmp( type , "BagLet6" ) )
	{
		outTimeFlag = EM_TimeFlagEffect_BagPage6;
		sprintf( _Key , "BagLet%02d" , index );
	}
	else
	{
		outTimeFlag = 0 ;
		outKey = "";
		return false;
	}

	outKey = _Key;
 
	return true;
}

// ----------------------------------------------------------------------------
int luaFunc_TimeLet_GetLetTime(lua_State* L)
{
	const char* type = luaL_checkstring(L, 1);
	int index = 0;

	int TimeFlag;
	string Key;


	if( false == GetTimeLetInfo( type , index , TimeFlag, Key ) )
	{
		return 0;
	}


	int Time = CNetGlobal::RoleData()->PlayerBaseData->TimeFlag[TimeFlag];

	lua_pushnumber( L , Time );

	return 1;
}

// ----------------------------------------------------------------------------
int luaFunc_TimeLet_StoreUp(lua_State* L)
{
	const char* type = luaL_checkstring(L, 1);
	int index = luaL_checkint(L, 2) - 1;

	if ( g_pGameMain->getMerchant() == false )
		return 0;

	int TimeFlag;
 	string Key;

	int Money;

	if( false == GetTimeLetInfo( type , index , TimeFlag, Key ) )
	{
		return 0;
	}

  	Money = g_ObjectData->GetMoneyKeyValue( Key.c_str() , 1);
 
	if( Money == 0 || Money > CNetGlobal::RoleData()->PlayerBaseData->Body.Money_Account )
	{
		return 0;
	}

	TimeFlag = TimeFlag * 100 + index;
  	NetCli_OtherChild::S_RunGlobalPlotRequest( 4, TimeFlag );	

	return 0;
}

// ----------------------------------------------------------------------------
int luaFunc_TimeLet_GetLetInfo(lua_State* L)
{
	const char* type = luaL_checkstring(L, 1);
	int index = luaL_checkint(L, 2) - 1;

	int TimeFlag;
	string Key;

	int Time;
	int Money;

	if( false == GetTimeLetInfo( type , index , TimeFlag, Key ) )
	{
		return 0;
	}

	Time = g_ObjectData->GetMoneyKeyValue( Key.c_str() , 0);
	Money = g_ObjectData->GetMoneyKeyValue( Key.c_str() , 1);

	if( Time == 0 && Money == 0 )
	{
		return 0;
	}

	lua_pushnumber( L , Time );
	lua_pushnumber( L , Money );

	return 2;
}

// ----------------------------------------------------------------------------
int LuaFunc_RefreshBank(lua_State* L)
{
	g_pBankFrame->SortItem();
	return 0;
}


int LuaFunc_GetBankItemLink(lua_State* L)
{
	int index = luaL_checkint(L, 1) - 1;
	ItemFieldStruct* item = CNetGlobal::RoleData()->GetBankItem(index);
	if (item)
	{
		GameObjDbStruct* itemDB = CNetGlobal::GetObj( item->OrgObjID );
		if (itemDB)
		{
			string link;
			if( !ItemField_To_ItemLink( item, link, true ) )
			{
				return 0;
			}
			lua_pushstring(L, link.c_str() );
			return 1;
		}
	}
	return 0;
}










