#include "../../mainproc/GameMain.h"
#include "../../Interface/WorldFrames/CharacterFrame.h"
#include "../../Interface/WorldFrames/ChatFrame.h"
#include "NetCli_DepartmentStoreChild.h"

#include "../../Interface/WorldFrames/MerchantFrame.h"
#include "../../tools/Common/Com_Function.h"

bool NetCli_DepartmentStoreChild::g_OnTimeWebBagRequest = false;

//-----------------------------------------------------------------------------
bool NetCli_DepartmentStoreChild::_Init()
{
	if (This)
		return false;

	This = new NetCli_DepartmentStoreChild;

	CNetGlobal::Schedular()->Push(OnTimeWebBagRequest, 3000, NULL, NULL);

	return NetCli_DepartmentStore::_Init();
}

//-----------------------------------------------------------------------------
bool NetCli_DepartmentStoreChild::_Release()
{
	NetCli_DepartmentStore::_Release();
	if (This)
	{
		delete This;
		This = NULL;
	}

	return true;
}

//-----------------------------------------------------------------------------
void NetCli_DepartmentStoreChild::RL_ShopFunctionResult	( int FunctionID , bool Result  )
{

}
void NetCli_DepartmentStoreChild::RD_SellType			( int Count , int SellType[50] )
{
	g_pMerchantFrame->ItemType(Count,SellType);
}
void NetCli_DepartmentStoreChild::RD_ShopInfo			( int ItemCount , DepartmentStoreBaseInfoStruct Item[500] )
{
	g_pMerchantFrame->ShopInfo(ItemCount,Item);
}
void NetCli_DepartmentStoreChild::RL_BuyItemResult		( DepartmentStoreBuyItemResultENUM Result )
{
	g_pMerchantFrame->BuyItemResult(Result);
}
void NetCli_DepartmentStoreChild::RL_ImportAccountMoneyResult( int Money_Account , int Money_Bonus )
{
	g_pMerchantFrame->ImportAccountMoneyResult(Money_Account);

}
void NetCli_DepartmentStoreChild::RL_ExchangeItemResult		( ItemFieldStruct& Item , int Money , int MoneyAccount , ItemExchangeResultENUM Result )
{
	switch( Result )
	{
	case EM_ItemExchangeResult_Depleted:
		g_pGameMain->SendWarningMsg( g_ObjectData->GetString( "ITEMEXCHANGE_DEPLETED" ) );
		break;
	case EM_ItemExchangeResult_RoleLimited:
		g_pGameMain->SendWarningMsg( g_ObjectData->GetString( "ITEMEXCHANGE_ROLE_LIMITED" ) );
		break;
	case EM_ItemExchangeResult_AccountLimited:
		g_pGameMain->SendWarningMsg( g_ObjectData->GetString( "ITEMEXCHANGE_ACCOUNT_LIMITED" ) );
		break;
	case EM_ItemExchangeResult_DataError:
		g_pGameMain->SendWarningMsg( g_ObjectData->GetString( "ITEMEXCHANGE_DATA_ERROR" ) );
		break;
	case EM_ItemExchangeResult_WorldError:
		g_pGameMain->SendWarningMsg( g_ObjectData->GetString( "ITEMEXCHANGE_WORLD_ERROR" ) );
		break;
	case EM_ItemExchangeResult_ItemUsed:		//物品已被使用
		g_pGameMain->SendWarningMsg( g_ObjectData->GetString( "ITEMEXCHANGE_ITEM_USED" ) );
		break;
	case EM_ItemExchangeResult_Failed:			//失敗，無此物件
		g_pGameMain->SendWarningMsg( g_ObjectData->GetString( "ITEMEXCHANGE_NOT_FIND" ) );
		break;
	case EM_ItemExchangeResult_OK:			//失敗，無此物件
		g_pGameMain->SendWarningMsg( g_ObjectData->GetString( "ITEMEXCHANGE_OK" ) );
		break;

	}
}

void NetCli_DepartmentStoreChild::RD_RunningMessage			( vector<string>& MsgList )
{
	
	if (g_pMerchantFrame)
	g_pMerchantFrame->RunningMessage(MsgList);
}

void NetCli_DepartmentStoreChild::RL_MailGiftResult			( CheckMailGiftResultTypeENUM Result )
{

	if (g_pMerchantFrame)
		g_pMerchantFrame->MailGiftResult(Result);


}
//通知所有人 老虎機資料有改變
void NetCli_DepartmentStoreChild::RL_SlotMachineInfoChange(  )
{
	if (g_pMerchantFrame)
		g_pMerchantFrame->SlotMachineInfoChange();
}
//通知老虎機資料
void NetCli_DepartmentStoreChild::RL_SlotMachineInfo( int itemID[] , int itemCount[] )
{
	if (g_pMerchantFrame)
		g_pMerchantFrame->SlotMachineInfo(itemID,itemCount);
}
//結果
void NetCli_DepartmentStoreChild::RL_SlotMachinePlayResult( SlotMachinePlayResultENUM result , int itemID , int itemCount )
{

	if (g_pMerchantFrame)
		g_pMerchantFrame->SlotMachinePlayResult(result,itemID,itemCount);
}
void NetCli_DepartmentStoreChild::RL_WebBagResult( int OrgObjID , int Count )
{
	std::string Msg = "";

	switch(OrgObjID)
	{
	case -1:
		{
			Msg = g_ObjectData->GetString("SYS_ACCOUNT_ERR1_1");
			g_pGameMain->SendWarningMsg(Msg.c_str());
			g_pGameMain->SendSystemChatMsg(Msg.c_str());
		}
		break;
	case -2:
		{
			Msg = g_ObjectData->GetString("SYS_ACCOUNT_ERR1_2");
			g_pGameMain->SendWarningMsg(Msg.c_str());
			g_pGameMain->SendSystemChatMsg(Msg.c_str());
		}
		break;
	case -3:
		{
			Msg = g_ObjectData->GetString("SYS_ACCOUNT_ERR1_3");
			g_pGameMain->SendWarningMsg(Msg.c_str());
			g_pGameMain->SendSystemChatMsg(Msg.c_str());
		}
		break;
	case -4:
		{
			Msg = g_ObjectData->GetString("SYS_ACCOUNT_ERR_3");
			g_pGameMain->SendWarningMsg(Msg.c_str());
			g_pGameMain->SendSystemChatMsg(Msg.c_str());
		}
		break;
	default:
		{
			GameObjDbStructEx* pObj = g_ObjectData->GetObj( OrgObjID );

			if( pObj == NULL )
				return;

			int	Quality = g_ObjectData->GetItemQuality(pObj);
			std::string Color = g_ObjectData->GetItemQualityTextColor(Quality);

			char ItemName[256];
			sprintf(ItemName, "%s%s|r", Color.c_str(), pObj->GetName());			

			if ( Count > 1 )
			{
				Msg = ReplaceKeyString(g_ObjectData->GetString("SYS_GIVE_ITEMS"), "<ITEMCOUNT>", Count);
				Msg = ReplaceKeyString(Msg, "<ITEM>", ItemName);
			}
			else
			{
				Msg = ReplaceKeyString(g_ObjectData->GetString("SYS_GIVE_ITEM"), "<ITEM>", ItemName);
			}

			g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM_GET, "", Msg.c_str());
		}
	}
}

int NetCli_DepartmentStoreChild::OnTimeWebBagRequest(SchedularInfo* obj, void* inputclass)
{
	if (g_OnTimeWebBagRequest == true)
	{
		SL_WebBagRequest();
	}

	CNetGlobal::Schedular()->Push(OnTimeWebBagRequest, 3000, NULL, NULL);

	return 0;
}