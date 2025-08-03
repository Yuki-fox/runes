#include "../../mainproc/GameMain.h"
#include "../../Interface/WorldFrames/StoreFrame.h"
//#include "../../Interface/WorldFrames/MerchantFrame.h"
#include "../../Interface/WorldFrames/ChatFrame.h"
#include "../tools/Common/Com_Function.h"
#include "NetCli_ShopChild.h"

//-----------------------------------------------------------------------------
bool NetCli_ShopChild::_Init()
{
	if (This)
		return false;
	This = new NetCli_ShopChild;
	return NetCli_Shop::_Init();
}

//-----------------------------------------------------------------------------
bool NetCli_ShopChild::_Release()
{
	NetCli_Shop::_Release();
	if (This)
	{
		delete This;
		This = NULL;
	}
	return true;
}

//-----------------------------------------------------------------------------
void NetCli_ShopChild::R_ShopOpen( int TraderID , int ShopObjID , int ItemCount[30] )
{
	g_pStoreFrame->Open(TraderID, ShopObjID, ItemCount);
}

//-----------------------------------------------------------------------------
void NetCli_ShopChild::R_ShopClose(  )
{
	g_pStoreFrame->ReceiveClose();
}

//-----------------------------------------------------------------------------
void NetCli_ShopChild::R_BuyItemResult( int TraderID , int OrgObjID , int Count , bool Result , PriceTypeENUM PriceType[2] , int Money[2] )
{
	// 購買了商品
	if ( Result )
	{
		char temp[512];
		const char* moneyType;
		const char* text;

		for ( int i = 0; i < 2; i ++ )
		{
			if ( PriceType[i] > EM_ACPriceType_BonusMoney && Money[i] > 0 )
			{
				sprintf_s(temp, 512, "SYS_MONEY_TYPE_%d", PriceType[i]);
				moneyType = g_ObjectData->GetString(temp);
				text = g_ObjectData->GetString("SYS_PAY_MONEY_1");

				if ( moneyType && text )
				{
					sprintf_s(temp, 512, "%d %s", Money[i], moneyType);
					string msg = ReplaceKeyString(text, "<MONEY>", temp);
					g_pChatFrame->SendMsgEvent(CHAT_MSG_SYSTEM_GET, msg.c_str(), "");
				}
			}
		}
	}

	/*
	if (Result){
		g_pDebugMessage->DebugOutput(0, "購買商品 %dx%d", OrgObjID, Count);
	} else {
		g_pDebugMessage->DebugOutput(0, "購買商品失敗");
	}
	*/
}

//-----------------------------------------------------------------------------
void NetCli_ShopChild::R_SelectSellItemResult( int TraderID , int FieldID , bool Result )
{
	// 你販賣了
	if ( Result ) {
		g_pRuGlobalEventManager->PlaySoundByFile("Sound\\Interface\\SellItem.mp3", false, 0.0f, ruSOUNDTYPE_INTERFACE);

	}
	int a = 0;
}

//-----------------------------------------------------------------------------
void NetCli_ShopChild::R_RepairWeaponResult( int TraderID , int Pos , bool IsWeapon , bool Result )
{
	// 修理全部裝備
	if ( Pos == -1 ) {
		if ( Result ) {
			//g_ruSymphony->PlaySoundByPath( "Sound\\Interface\\RepairEquipment.mp3", false );
			int loopCount = (FALSE) ? 0x7FFFFFFF : 1;
			g_ruAudible->Play("Sound\\Interface\\RepairEquipment.mp3", (CRuAudible::AudibleType)ruSOUNDTYPE_SFX, (loopCount > 1) ? TRUE : FALSE);					
			g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_REPAIR_ALLEQ_OK"));
		} else {
			g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_REPAIR_ALLEQ_FAILED"));
		}
	} else {		
		ItemFieldStruct* itemData = CNetGlobal::RoleData()->GetEqItem(Pos);
		if ( itemData ) {
			string itemName = g_ObjectData->GetItemColorName(itemData);
			if ( Result ) {
				// g_ruSymphony->PlaySoundByPath( "Sound\\Interface\\RepairEquipment.mp3", false );
				int loopCount = (FALSE) ? 0x7FFFFFFF : 1;
				g_ruAudible->Play("Sound\\Interface\\RepairEquipment.mp3", (CRuAudible::AudibleType)ruSOUNDTYPE_SFX, (loopCount > 1) ? TRUE : FALSE);				
				g_pChatFrame->SendChatMessageEx(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_REPAIR_EQ_SUCCESS"), itemName.c_str());
			} else {
				g_pGameMain->SendWarningMsgEx(g_ObjectData->GetString("SYS_REPAIR_EQ_FAILED"), itemName.c_str());
			}
		}
	}
}
//-----------------------------------------------------------------------------
void NetCli_ShopChild::R_AddSellItemRecord( SellItemFieldStruct& Info )
{
	_RoleData.PlayerTempData.SellItemBackup.Push_Back( Info );
	g_pStoreFrame->UpdateSell();
}

//-----------------------------------------------------------------------------
void NetCli_ShopChild::R_DelSellItemRecord(  int EarsePos )
{
	_RoleData.PlayerTempData.SellItemBackup.Erase( EarsePos );
	g_pStoreFrame->UpdateSell();
}

//-----------------------------------------------------------------------------
void NetCli_ShopChild::R_FixShopItemCount( int TraderID , int ShopObjID , int Pos , int ItemCount )
{
	g_pStoreFrame->UpdateBuy( Pos, ItemCount );
}

/*
//購買商城的結果
void NetCli_ShopChild::R_AccountMoneyShopResult( int OrgObjID , AccountMoneyShopResultENUM Result )
{
	int a = 0;

}
*/
void NetCli_ShopChild::R_AccountMoneyShopBuyListResult( AccountMoneyShopResultENUM Result )
{	
//	g_pMerchantFrame->AccountMoneyShopBuyListResult(Result);


	switch (Result)
	{
	case EM_AccountMoneyShopResult_OK:
	
		g_pStoreFrame->SendWorldEvent("BUY_ACCOUNTSHOP_SUCCEEDED");
		g_pChatFrame->SendMsgEvent(CHAT_MSG_SYSTEM, g_ObjectData->GetString("SYS_ACCOUNT_BUY_SUCCEEDED"), "");
		break;

	case EM_AccountMoneyShopResult_MoneyNotEnough:
		g_pStoreFrame->SendWorldEvent("BUY_ACCOUNTSHOP_FAILED");
		g_pGameMain->SendWarningMsg("SYS_ACCOUNT_ERR_1");
		break;

	case EM_AccountMoneyShopResult_PocketFull:
		g_pStoreFrame->SendWorldEvent("BUY_ACCOUNTSHOP_FAILED");
		g_pGameMain->SendWarningMsg("SYS_ACCOUNT_ERR_2");
		break;

	case EM_AccountMoneyShopResult_PocketPosErr:
		g_pStoreFrame->SendWorldEvent("BUY_ACCOUNTSHOP_FAILED");
		g_pGameMain->SendWarningMsg("SYS_ACCOUNT_ERR_2");
		break;

	case EM_AccountMoneyShopResult_Failed:
		g_pStoreFrame->SendWorldEvent("BUY_ACCOUNTSHOP_FAILED");
		g_pGameMain->SendWarningMsg("SYS_ACCOUNT_ERR_3");
		break;
	}
}

void NetCli_ShopChild::R_FixSellItemBackup( StaticVector< SellItemFieldStruct , _MAX_SELL_ITEM_SAVE_COUNT_ >&	SellItemBackup )
{
	_RoleData.PlayerTempData.SellItemBackup = SellItemBackup;
}