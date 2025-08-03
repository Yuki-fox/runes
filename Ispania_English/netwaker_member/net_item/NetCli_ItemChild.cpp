#include "../mainproc/GameMain.h"
#include "../Interface/WorldFrames/BagFrame.h"
#include "../Interface/WorldFrames/ChatFrame.h"
#include "../Interface/WorldFrames/BankFrame.h"
#include "../Interface/WorldFrames/ActionFrame.h"
#include "../Interface/WorldFrames/ItemClipboard.h"
#include "../Interface/WorldFrames/CharacterFrame.h"
#include "../Interface/WorldFrames/DrawRuneFrame.h"
#include "../Interface/WorldFrames/ClearAttribStoneFrame.h"
#include "../Interface/WorldFrames/AccountBagFrame.h"
#include "../Interface/WorldFrames/PhantomFrame.h"
#include "../Interface/itemlink/ItemLink.h"
#include "../interface/quest/UI_QuestDetail.h"
#include "../Interface//CardBook/UI_CardBook.h"
#include "../Interface/SkillBook/UI_SkillBook.h"
#include "../Interface/ItemLink/ItemLink.h"
#include "../Interface/FusionStoneTradeFrame/FusionStoneTradeFrame.h"
#include "../tools/Common/Com_Function.h"

#include "../Interface/WorldFrames/PartnerFrame.h"
#include "../Interface/WorldFrames/RuneExchange.h"
#include "../Interface/interface.h"
#include "NetCli_ItemChild.h"

bool						NetCli_ItemChild::s_isLockQueue = false;
float						NetCli_ItemChild::s_updateTime = 0.0f;
int							NetCli_ItemChild::s_npcID;
int							NetCli_ItemChild::s_accountBagItems;
vector<NetAccountBagItem>	NetCli_ItemChild::s_accountBagList;


//-----------------------------------------------------------------------------
void NetCli_ItemChild::ReplaceMoney(string description, string money)
{
	size_t begin;
	while ( (begin = description.find("<MONEY>")) != string::npos )
		description.replace(begin, 7, money);

	g_pChatFrame->SendMsgEvent(CHAT_MSG_SYSTEM_GET, description.c_str(), "");
}

//-----------------------------------------------------------------------------
bool NetCli_ItemChild::_Init()
{
	if (This)
		return false;
	
	s_npcID = 0;
	s_accountBagItems = 0;
	s_accountBagList.clear();
	This = new NetCli_ItemChild;	
	return NetCli_Item::_Init();
}

//-----------------------------------------------------------------------------
bool NetCli_ItemChild::_Release()
{
	NetCli_Item::_Release();
	if (This)
	{
		delete This;
		This = NULL;
	}
	return true;
}

//-----------------------------------------------------------------------------
void NetCli_ItemChild::_Update(float elapsed)
{
	if ( RoleData()->PlayerBaseData->ItemTemp.IsEmpty() || s_isLockQueue )
		return;

	ItemFieldStruct*	item	= RoleData()->PlayerBaseData->ItemTemp.Begin();
	GameObjDbStructEx*	itemDB	= GetObj(item->OrgObjID);
	if ( itemDB == NULL )
		return;

	int pos = RoleData()->SearchBodyFirstFit(item);
	if (pos != - 1)
	{
		ItemFieldStruct* bagItem = RoleData()->GetBodyItem(pos);
		if ( bagItem == NULL || bagItem->Pos != EM_ItemState_NONE )
			return;

		s_isLockQueue = true;
		bagItem->Pos = EM_ItemState_CliDisabled;
		int Count = __min( item->Count , itemDB->MaxHeap - bagItem->Count );
		NetCli_Item::_GetBufferItem( *item, pos, Count );

		s_updateTime = 0.0f;
	}
	else if (s_updateTime > 10.0f)
	{
		// g_pChatFrame->SendMsgEvent(CHAT_MSG_SYSTEM, "警告! 背包物品已滿", "");
		if ( RoleData()->PlayerBaseData->ItemTemp.Size() >= 7 )
		{
			s_isLockQueue = true;
			char tmp[MAX_PATH];
			sprintf( tmp, "丟棄暫存區域物品 %s(%d)", itemDB->GetName(), item->OrgObjID );
			g_pChatFrame->SendMsgEvent(CHAT_MSG_SYSTEM, tmp, "");
			NetCli_Item::_DelBufferItem(*item);
			//RoleData()->PlayerBaseData->ItemTemp.Pop_Front();
		}
		s_updateTime = 0.0f;
	}
	else
		s_updateTime += elapsed;
}

//-----------------------------------------------------------------------------
void NetCli_ItemChild::R_GetItemOK()
{
	int a = 0;
}

//-----------------------------------------------------------------------------
void NetCli_ItemChild::R_GetGroundItemFailed()
{
	int a = 0;
	//_RequestBufferInfo();
}

//-----------------------------------------------------------------------------
void NetCli_ItemChild::R_GetBufferItemFailed()
{
	int a = 0;
	// g_pBagFrame->ReflashItemQueueTime();
	//_RequestBufferInfo();
}

//-----------------------------------------------------------------------------
void NetCli_ItemChild::R_ClsItemInBuffer()
{
	RoleData()->PlayerBaseData->ItemTemp.Clear();

	// 更新界面
	g_pBagFrame->ClearAllQueueItem();
}

//-----------------------------------------------------------------------------
void NetCli_ItemChild::R_NewItemInBuffer(ItemFieldStruct& Item)
{
	RoleData()->AddItemtoBuf( Item );	

	// 撥放音效
	GameObjDbStructEx* itemDB = g_ObjectData->GetObj(Item.OrgObjID);
	if ( itemDB ) {
		if ( g_ObjectData->GetItemQuality(itemDB, &Item) > 1 ) {
			g_pRuGlobalEventManager->PlaySoundByFile("sound\\interface\\ui_backpack_loot02.mp3", false, 0.0f, ruSOUNDTYPE_INTERFACE);
		} else {
			g_pRuGlobalEventManager->PlaySoundByFile("sound\\interface\\ui_backpack_loot01.mp3", false, 0.0f, ruSOUNDTYPE_INTERFACE);
		}
		g_pBagFrame->InsertItemQueueItem();

	} else {
		_DelBufferItem(Item);
	}	

	/*
	char itemName[MAX_PATH];
	if ( RoleDataEx::GetItemFieldName(&Item, itemName, MAX_PATH) )
	{
		char tmp[MAX_PATH];
		sprintf( tmp, g_ObjectData->GetString("SYS_GIVE_ITEM"), itemName );
		g_pChatFrame->AddMessage( 0, tmp );
	}
	*/
}

//-----------------------------------------------------------------------------
void NetCli_ItemChild::R_DeleteItemOK_Body( ItemFieldStruct& Item  )
{
	DeleteItemMessage(Item);
}

//-----------------------------------------------------------------------------
void NetCli_ItemChild::R_DeleteItemOK_Bank( ItemFieldStruct& Item )
{
	DeleteItemMessage(Item);
}

//-----------------------------------------------------------------------------
void NetCli_ItemChild::R_DeleteItemOK_EQ( ItemFieldStruct& Item )
{
	DeleteItemMessage(Item);
}

//-----------------------------------------------------------------------------
void NetCli_ItemChild::R_DeleteItemFailed()
{
	int a = 0;
}

//-----------------------------------------------------------------------------
void NetCli_ItemChild::DeleteItemMessage( ItemFieldStruct& Item )
{
	g_pRuGlobalEventManager->PlaySoundByFile("sound\\interface\\ui_icon_delete.mp3", false, 0.0f, ruSOUNDTYPE_INTERFACE);

	// 訊息提示
	char temp[512];
	string itemName;
	if ( ItemField_To_ItemLink(&Item, itemName) == false )
		itemName = g_ObjectData->GetItemColorName(&Item);

	int count = Item.GetCount();
	
	if ( count > 1 )
	{
		sprintf_s(temp, 512, "%s x %d", itemName.c_str(), count);		
	}
	else
	{
		sprintf_s(temp, 512, "%s", itemName.c_str());
	}

	string msg = ReplaceKeyString(g_ObjectData->GetString("SYS_DESTROY_ITEM"), "<ITEM>", temp);	
	g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM_GET, "", msg.c_str() );	
}

//-----------------------------------------------------------------------------
void NetCli_ItemChild::R_ExchangeFieldOK_Body( int ClientID )
{
	/*
	// 更新物品索引表
	int value = s_exchangeBody.front();
	int np1 = 0;
	int np2 = 0;
	while ( np1 < _MAX_BODY_COUNT_ )
	{
		if ( RoleData()->PlayerBaseData->ClientData[np1] == (value & 0xFFFF) )
			break;
		np1++;
	}
	while ( np2 < _MAX_BODY_COUNT_ )
	{
		if ( RoleData()->PlayerBaseData->ClientData[np2] == value >> 16 )
			break;
		np2++;
	}

	int temp = RoleData()->PlayerBaseData->ClientData[np1];
	RoleData()->PlayerBaseData->ClientData[np1] = RoleData()->PlayerBaseData->ClientData[np2];
	RoleData()->PlayerBaseData->ClientData[np2] = temp;
	*/
	
	// g_pError->AddMessage(0, 0, "R_ExchangeFieldOK_Body");
}

//-----------------------------------------------------------------------------
void NetCli_ItemChild::R_ExchangeFieldOK_Bank( int ClientID )
{
	int a = 0;
}

//-----------------------------------------------------------------------------
void NetCli_ItemChild::R_ExchangeFieldOK_BodyBank( int ClientID )
{
	g_pGameMain->ExcuteItemCommand(ClientID);
}

//-----------------------------------------------------------------------------
void NetCli_ItemChild::R_ExchangeFieldFailed_Body( int ClientID )
{
	int a = 0;
	// g_pError->AddMessage(0, 0, "R_ExchangeFieldFailed_Body");
}

//-----------------------------------------------------------------------------
void NetCli_ItemChild::R_ExchangeFieldFailed_Bank( int ClientID )
{
	int a = 0;
}

//-----------------------------------------------------------------------------
void NetCli_ItemChild::R_ExchangeFieldFailed_BodyBank( int ClientID )
{
	g_pGameMain->RemoveItemCommand(ClientID);
}

//-----------------------------------------------------------------------------
void NetCli_ItemChild::R_BodyBankMoneyExchangeOK()
{
	int a = 0;
}

//-----------------------------------------------------------------------------
void NetCli_ItemChild::R_BodyBankMoneyExchangeFalse()
{
	int a = 0;
}

//-----------------------------------------------------------------------------
void NetCli_ItemChild::R_FixItemInfo_Body(ItemFieldStruct& Item , int Pos)
{
	/*
	// 刪除物品
	if ( Item.IsEmpty() )
	{
		g_pBagFrame->DeleteSortItem(Pos);
	}
	else
	{
		ItemFieldStruct* item = CNetGlobal::RoleData()->GetBodyVirtualItem(Pos);
		if ( item->IsEmpty() )
		{
			g_pBagFrame->InsertSortItem(Pos);
		}
	}
	*/

	RegisterItemFieldChecksum( &Item );

	RoleData()->SetItemInfo_Body( Item , Pos );

	g_pBagFrame->UpdateItem( Pos );
	g_pBagFrame->SendWorldEvent("PLAYER_BAG_CHANGED");

	// g_pError->AddMessage(0, 0, "R_FixItemInfo_Body Pos[%d] ItemType[%d]", Pos, Item.Pos);

	if (g_pPhantomFrame != NULL)
	{
		g_pPhantomFrame->R_FixItemInfo_Body();
	}
}

//-----------------------------------------------------------------------------
void NetCli_ItemChild::R_FixItemInfo_Bank(ItemFieldStruct& Item , int Pos)
{
	RegisterItemFieldChecksum( &Item );

	RoleData()->SetItemInfo_Bank( Item , Pos );
}

//-----------------------------------------------------------------------------
void NetCli_ItemChild::R_FixItemInfo_EQ(ItemFieldStruct& Item , int Pos)
{
	RegisterItemFieldChecksum( &Item );

	// 即將損壞警告
	ItemFieldStruct* itemData = CNetGlobal::RoleData()->GetEqItem(Pos);
	if ( itemData && itemData->OrgObjID == Item.OrgObjID ) {
		float oldDurable = 0.0f;
		float durable = 0.0f;

		GameObjDbStruct* itemDB = CNetGlobal::GetObj(itemData->OrgObjID);
		if ( itemDB ) {
			oldDurable = (float)itemData->Durable / (float)(itemDB->Item.Durable * itemData->Quality);
			durable = (float)Item.Durable / (float)(itemDB->Item.Durable * Item.Quality);
		}

		if ( oldDurable > 0.3f && durable < 0.3f ) {
			string itemName;
			if ( ItemField_To_ItemLink(&Item, itemName) == false )
				itemName = g_ObjectData->GetItemColorName(&Item);
			g_pGameMain->SendWarningMsgEx(g_ObjectData->GetString("SYS_ITEM_BROKEN_WARNING"), itemName.c_str());
		}

		/*
		g_pError->AddMessage(0, 0, "(%d/%d)(%d/%d) (%.4f/%.4f)",
			Item.Durable,
			itemDB->Item.Durable * Item.Quality,
			itemData->Durable,
			itemDB->Item.Durable * itemData->Quality,
			durable,
			oldDurable);
		*/
	}

	RoleData()->SetItemInfo_EQ( Item , Pos );

	// 發出事件通知 UI 更新狀態
	lua_State* L = g_pInterface->GetLuaState();
	if ( L ) 
	{
		lua_pushnumber(L, Pos );
		lua_setglobal(L, "arg1");
		g_pInterface->SendWorldEvent(UiEvent_RoleEquipChange);
	}
}

//-----------------------------------------------------------------------------
void NetCli_ItemChild::R_FixAllItemInfo(BodyItemStruct& Body , BankItemStruct& Bank , EQStruct& EQ)
{
	RoleData()->PlayerBaseData->Body = Body;
	RoleData()->PlayerBaseData->Bank = Bank;
	RoleData()->BaseData.EQ   = EQ;
	RoleData()->TempData.UpdateInfo.Bank = true;
	RoleData()->TempData.UpdateInfo.Body = true;
	RoleData()->TempData.UpdateInfo.Eq  = true;
	// g_pError->AddMessage(0, 0, "R_FixAllItemInfo");
}

//-----------------------------------------------------------------------------
void NetCli_ItemChild::R_FixMoney(int BodyMoney , int BankMoney , int BodyMoney_Account , int Money_Bonus )
{
	char money[512];	
	int D_BodyMoney = BodyMoney - RoleData()->PlayerBaseData->Body.Money;
	int D_MoneyBonus = Money_Bonus - RoleData()->PlayerBaseData->Money_Bonus;
	RoleData()->PlayerBaseData->Money_Bonus = Money_Bonus;
	RoleData()->PlayerBaseData->Body.Money = BodyMoney;
	RoleData()->PlayerBaseData->Bank.Money = BankMoney;
	//RoleData()->TempData.UpdateInfo.Money = true;	
	
	if( D_BodyMoney != 0 )
	{
		const char* text;
		if ( D_BodyMoney > 0 )
		{
			sprintf_s(money, 512, "%d %s", D_BodyMoney, g_ObjectData->GetString("SYS_GOLD"));
			text = g_ObjectData->GetString("SYS_GIVE_MONEY_1");
		}
		else
		{
			sprintf_s(money, 512, "%d %s", -D_BodyMoney, g_ObjectData->GetString("SYS_GOLD"));
			text = g_ObjectData->GetString("SYS_PAY_MONEY_1");
		}

		string temp = ReplaceKeyString(text, "<MONEY>", money);
		g_pChatFrame->SendMsgEvent(CHAT_MSG_SYSTEM_GET, temp.c_str(), "");
	}

	if( BodyMoney_Account != -1 )
	{		
		int D_BodyMoney_Account = BodyMoney_Account - RoleData()->PlayerBaseData->Body.Money_Account;
		RoleData()->PlayerBaseData->Body.Money_Account  = BodyMoney_Account;

		if ( D_BodyMoney_Account != 0 )
		{
			const char* text;
			if ( D_BodyMoney_Account > 0 )
			{
				sprintf_s(money, 512, "%d %s", D_BodyMoney_Account, g_ObjectData->GetString("SYS_MONEY_RUNE"));
				text = g_ObjectData->GetString("SYS_GIVE_MONEY_1");
			}
			else
			{
				sprintf_s(money, 512, "%d %s", -D_BodyMoney_Account, g_ObjectData->GetString("SYS_MONEY_RUNE"));
				text = g_ObjectData->GetString("SYS_PAY_MONEY_1");
			}

			string temp = ReplaceKeyString(text, "<MONEY>", money);
			g_pChatFrame->SendMsgEvent(CHAT_MSG_SYSTEM_GET, temp.c_str(), "");
		}
	}

	if( D_MoneyBonus != 0 )
	{
		const char* text;
		if ( D_MoneyBonus > 0 )
		{
			sprintf_s(money, 512, "%d %s", D_MoneyBonus, g_ObjectData->GetString("SYS_RUBY"));
			text = g_ObjectData->GetString("SYS_GIVE_MONEY_1");
		}
		else
		{
			sprintf_s(money, 512, "%d %s", -D_MoneyBonus, g_ObjectData->GetString("SYS_RUBY"));
			text = g_ObjectData->GetString("SYS_PAY_MONEY_1");
		}

		string temp = ReplaceKeyString(text, "<MONEY>", money);
		g_pChatFrame->SendMsgEvent(CHAT_MSG_SYSTEM_GET, temp.c_str(), "");
	}

	// 通知角色金錢數值更改
	g_pBagFrame->SendWorldEvent(UiEvent_Role_Money_Change);
}

//-----------------------------------------------------------------------------
void NetCli_ItemChild::R_MaxItemCount(int MaxBodyItem , int MaxBankItem)
{
	RoleData()->PlayerBaseData->Body.Count = MaxBodyItem;
	RoleData()->PlayerBaseData->Bank.Count = MaxBankItem;
}

//-----------------------------------------------------------------------------
void NetCli_ItemChild::R_OpenBank( int GItemID )
{
	g_pBankFrame->Open( GItemID );
}
//-----------------------------------------------------------------------------
void NetCli_ItemChild::R_CloseBank( )
{
	// 關閉界面
	g_pBankFrame->SendWorldEvent("BANKFRAME_CLOSEED");
}
//-----------------------------------------------------------------------------
void NetCli_ItemChild::R_UseItemOK(int Type ,int Pos)
{
	CRoleSprite* player = g_pGameMain->GetPlayer();
	ItemFieldStruct* item = NULL;

	switch (Type)
	{
	// 背包
	case 0:
		item = CNetGlobal::RoleData()->GetBodyItem(Pos);
		break;
	// 銀行
	case 1:
		item = CNetGlobal::RoleData()->GetBankItem(Pos);
		break;
	// 裝備
	case 2:
		item = CNetGlobal::RoleData()->GetEqItem(Pos);
		break;
	}

	if ( item == NULL || item->IsEmpty() )
		return;
	
	CEffectSprite* pEffect = NULL;
	GameObjDbStructEx* itemDB = g_ObjectData->GetObj(item->OrgObjID);
	if ( itemDB )
	{
		// 取得物品名稱加上顏色控制碼
		string itemName;
		if ( ItemField_To_ItemLink(item, itemName) == false )
			itemName = g_ObjectData->GetItemColorName(item);
		
		const char* actFilename = NULL;
		int point = eAttach_Null;
		bool showUseMsg = true;

		switch (itemDB->Classification)
		{
		case EM_ObjectClass_Item:
			if ( itemDB->Item.IncMagic_Onuse != 0 )					// 檢查物品是否有法術效果,觸發法術特效
			{
				GameObjDbStructEx* magicCol = g_ObjectData->GetObj(itemDB->Item.IncMagic_Onuse);
				if ( magicCol && player )
				{
					GameObjDbStructEx* imageDB = g_ObjectData->GetObj(magicCol->ImageID);
					if ( imageDB )
					{
						player->SetSpellMotion(imageDB->Image.GatherMotion);
					}					
				}
			}

			switch (itemDB->Item.Class)
			{
			// 武器
			case EM_Item_Weapon:
				break;

			//防具
			case EM_Item_Armor:
				/*
				switch ( itemDB->Item.ArmorPos )
				{
				// 裝飾品
				case EM_ArmorPos_Ornament:
					player->PlayMotionOrnament("explode");
					break;
				}
				*/
				break;

			// 符文
			case EM_Item_Rune:
				showUseMsg = false;
				g_pRuGlobalEventManager->PlaySoundByFile("sound\\interface\\add_rune.mp3", false, 0.0f, ruSOUNDTYPE_INTERFACE);
				g_pChatFrame->SendChatMessageEx(CHAT_MSG_SYSTEM_GET, "", g_ObjectData->GetString("SYS_ADDRUNES_SUCCESS"), itemName.c_str());
				break;

			default:
				// 沒有法術效果檢查物品使用類型撥放音效
				switch (itemDB->Item.UseType)
				{
				case EM_ItemUseType_ClsEqSoulbound:		//清除Soulbound						
					g_pRuGlobalEventManager->PlaySoundByFile("sound\\interface\\sc_poweritem.mp3", false, 0.0f, ruSOUNDTYPE_INTERFACE);
					actFilename = g_ObjectData->GetString("FX_NAME_DESOULBOUND_SUCCESS");
					point = eAttach_Down;
					break;				
				case EM_ItemUseType_ClsTrade:		//清除Soulbound						
					g_pRuGlobalEventManager->PlaySoundByFile("sound\\interface\\sc_poweritem.mp3", false, 0.0f, ruSOUNDTYPE_INTERFACE);
					actFilename = g_ObjectData->GetString("FX_NAME_DESOULBOUND_SUCCESS");
					point = eAttach_Down;
					break;
				case EM_ItemUseType_EQClearRune:		//清除裝備符文						
					g_pRuGlobalEventManager->PlaySoundByFile("sound\\interface\\sc_poweritem.mp3", false, 0.0f, ruSOUNDTYPE_INTERFACE);
					actFilename = g_ObjectData->GetString("FX_NAME_DESOULBOUND_SUCCESS");
					point = eAttach_Down;
					break;

				case EM_ItemUseType_EqRefine:			//衝等
					// 由衝等封包決定
					break;

				case EM_ItemUseType_FixEq:				//修裝
					g_pRuGlobalEventManager->PlaySoundByFile("sound\\interface\\ui_player_repair.mp3", false, 0.0f, ruSOUNDTYPE_INTERFACE);
					break;

				case EM_ItemUseType_Dissolution:		//拆解物品
					// 由封包決定音效
					break;

				case EM_ItemUseType_Coloring:			//染色
					break;

				case EM_ItemUseType_ResetAbility:		//重設基本數值
					break;

				case EM_ItemUseType_ResetSkillPoint:	//重設技能點數
					break;

				case EM_ItemUseType_EQDigHole:			//裝備挖洞
					g_pRuGlobalEventManager->PlaySoundByFile("sound\\interface\\add_runehole.mp3", false, 0.0f, ruSOUNDTYPE_INTERFACE);
					break;
				}
			}
			break;

			// 習得配方
		case EM_ObjectClass_Recipe:
			showUseMsg = false;
			break;

			// 新增卡片
		case EM_Item_Card:
			showUseMsg = false;
			g_pRuGlobalEventManager->PlaySoundByFile("sound\\interface\\skill_learn.mp3", false, 0.0f, ruSOUNDTYPE_INTERFACE);
			g_pChatFrame->SendChatMessageEx(CHAT_MSG_SYSTEM_GET, "", g_ObjectData->GetString("SYS_ITEM_CARD_ADD"), itemName.c_str());
			break;
		}

		// 使用物品提示
		if ( showUseMsg ) {
			g_pChatFrame->SendChatMessageEx(CHAT_MSG_SYSTEM_GET, "", g_ObjectData->GetString("SYS_USE_ITEM"), itemName.c_str());
		}

		if ( player ) {
			CEffectSprite* pEffect = new CEffectSprite;
			if ( pEffect->LoadRuEntity(actFilename) ) {
				pEffect->AttachSprite(player, point);
				pEffect->SetEffectMotionStep(eEffectMotion_Step_Explode);
			} else {
				delete pEffect;
			}
		}
	}
}

//-----------------------------------------------------------------------------
void NetCli_ItemChild::R_UseItemFailed(int Type ,int Pos)
{
	ItemFieldStruct* item = NULL;
	switch (Type)
	{
		// 背包
	case 0:
		item = CNetGlobal::RoleData()->GetBodyItem(Pos);
		break;
		// 銀行
	case 1:
		item = CNetGlobal::RoleData()->GetBankItem(Pos);
		break;
		// 裝備
	case 2:
		item = CNetGlobal::RoleData()->GetEqItem(Pos);
		break;
	}

	if ( item == NULL || item->IsEmpty() )
		return;

	if ( item->Pos == EM_USE_ITEM || item->Pos == EM_ItemState_CliDisabled )
	{
		item->Pos = EM_ItemState_NONE;
		RoleData()->TempData.UpdateInfo.Body = true;
	}
	g_pRuGlobalEventManager->PlaySoundByFile("sound\\interface\\sys_menu_false.mp3", false, 0.0f, ruSOUNDTYPE_INTERFACE);
}

//-----------------------------------------------------------------------------
void NetCli_ItemChild::R_DelBufferItemOK	( int ItemID , int Count )
{
	ItemFieldStruct* item = RoleData()->PlayerBaseData->ItemTemp.Begin();
	if ( item && !item->IsEmpty() )
	{
		item->Count -= Count;
		if ( item->Count <= 0 )
			RoleData()->PlayerBaseData->ItemTemp.Pop_Front();
	}

	//g_pBagFrame->ReceiveQueueItemToBag(ItemID);	
}

//-----------------------------------------------------------------------------
void NetCli_ItemChild::R_GetBufferItemOK( int ItemID , int Count )
{
	ItemFieldStruct* item = RoleData()->PlayerBaseData->ItemTemp.Begin();
	if ( item )
	{
		GameObjDbStructEx* pObj = g_ObjectData->GetObj( item->OrgObjID );
		if( pObj != NULL && !pObj->IsKeyItem() )	// 避免獲得任務物品
		{
			string itemName, msg;
			if ( ItemField_To_ItemLink(item, itemName) == false )
				itemName = g_ObjectData->GetItemColorName(item);			

			// 數量
			if ( Count > 1 && item->GetCount() > 1 )
			{
				msg = ReplaceKeyString(g_ObjectData->GetString("SYS_GIVE_ITEMS"), "<ITEMCOUNT>", Count);
				msg = ReplaceKeyString( msg, "<ITEM>", itemName);
			}
			else
			{
				msg = ReplaceKeyString(g_ObjectData->GetString("SYS_GIVE_ITEM"), "<ITEM>", itemName);
			}

			g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM_GET, "", msg.c_str());
		}

		item->Count -= Count;
		if ( item->Count <= 0 )
			RoleData()->PlayerBaseData->ItemTemp.Pop_Front();
	}
	
	// 任務完成條件判斷, 判斷新增物品是否為任務需求物品
	g_pQuestDetail->GetItemQueue( ItemID, Count );

	g_pBagFrame->ReceiveQueueItemToBag(ItemID);	
}
//-----------------------------------------------------------------------------
void NetCli_ItemChild::R_SetKeyItemFlag	( int KeyItemID , bool Value )
{
	//int				ID		= KeyItemID - Def_ObjectClass_KeyItem;
	RoleDataEx*		pRole	= CNetGlobal::RoleData();

	if( Value == true )
	{
		pRole->AddKeyItem( KeyItemID );
		//RoleData()->BaseData.KeyItem.SetFlagOn( ID );
		GameObjDbStructEx*	pObj = g_ObjectData->GetObj( KeyItemID );

		if( pObj->IsKeyItem() )
		{
			if( pObj->KeyItem.Type == EM_KeyItemType_Recipe )
			{
				g_pInterface->SendWorldEvent( UiEvent_Craft_LearnRecipe );
				g_pUi_SkillBook->SkillUpdate( 0 );	// 強制技能書第一頁更新
			}
			else if( pObj->KeyItem.Type == EM_KeyItemType_LifeSkillUnLock )
			{
				lua_State* L = g_pInterface->GetLuaState();
				if ( L ) 
				{
					lua_pushnumber(L, pObj->KeyItem.LifeSkillType-EM_SkillValueType_BlackSmith);
					lua_setglobal(L, "arg1");
				}
				g_pInterface->SendWorldEvent( "CRAFT_UNLOCK" );
			}
			else if( pObj->KeyItem.Type == EM_KeyItemType_Normal )
			{
				g_pInterface->SendWorldEvent( UiEvent_ResetQuestBook );
				g_pInterface->SendWorldEvent( UiEvent_ResetQuestTrack );

				// 檢查任務條件是否達成
				g_pQuestDetail->GetItemQueue( KeyItemID, 1 );
			}
			else if( pObj->KeyItem.Type == EM_KeyItemType_SkillBook )
			{
				//g_pInterface->SendWorldEvent( UiEvent_SkillUpdate );
				CUI_SkillBook::SkillUpdate();
			}
		}
		/*
		int id=g_ObjectData->GetSysKeyValue("Wardrobe_ItemPage3");
		if (KeyItemID>=id&&KeyItemID<(id+8))
		{
			g_pInterface->SendWorldEvent( "COOLCLOTHBAG_KEYITEM" );
		}
		else 
		{
			id=g_ObjectData->GetSysKeyValue("Wardrobe_Set6");
			if (KeyItemID>=id&&KeyItemID<(id+5))
			{
				g_pInterface->SendWorldEvent( "COOLSUIT_KEYITEM" );
			}

		}
		*/

		//魔幻寶盒
		if( KeyItemID == g_ObjectData->SysValue().MagicBoxOpenKeyItemID )
		{
			g_pInterface->SendWorldEvent( UiEvent_MagicBoxEnable );
		}
	}
	else
	{
		pRole->DelKeyItem( KeyItemID );
		//RoleData()->BaseData.KeyItem.SetFlagOff( ID );
	}
}
//-----------------------------------------------------------------------------
void NetCli_ItemChild::R_SetTitleFlag	( int TitleID , bool Value )
{
	//int				ID		= KeyItemID - Def_ObjectClass_KeyItem;
	RoleDataEx*		pRole	= CNetGlobal::RoleData();

	if( Value == true )
	{
		pRole->AddTitle( TitleID );
		//RoleData()->BaseData.KeyItem.SetFlagOn( ID );
		GameObjDbStructEx* pTitleObj = g_ObjectData->GetObj( TitleID  );

		if( !pTitleObj )
			return;

		//char msg[256];
		//sprintf( msg , g_ObjectData->GetString( "SYS_GET_TITLE" ), pTitleObj->GetName() );

		//g_pGameMain->SendSystemChatMsg( msg );

		lua_State* L = g_pInterface->GetLuaState();

		lua_pushstring( L, pTitleObj->GetName() );
		lua_setglobal( L, "arg1" );

		GameObjDbStruct* imageDB = CNetGlobal::GetObj( pTitleObj->ImageID );
		if ( imageDB )
		{
			lua_pushstring( L, imageDB->Image.ACTField ); // ICON 位置
			lua_setglobal( L, "arg2" );
		}
		else
		{
			lua_pushnil( L );
			lua_setglobal( L, "arg2" );
		}

		lua_pushnumber( L, pTitleObj->Rare );
  		lua_setglobal( L, "arg3" );

 		g_pInterface->SendWorldEvent( UiEvent_PlayerGetTitle );
	}
	else
	{
		if( pRole->BaseData.TitleID == TitleID )
		{
			pRole->BaseData.TitleID = 0;
			pRole->TempData.UpdateInfo.Recalculate_All = true;
		}

		pRole->DelTitle( TitleID );
		//RoleData()->BaseData.KeyItem.SetFlagOff( ID );
	}

	g_pInterface->SendWorldEvent( UiEvent_PlayerTitleFlagChanged );

}

void NetCli_ItemChild::R_SetCardFlag	( int CardID , bool Value )
{
	//int				ID		= KeyItemID - Def_ObjectClass_KeyItem;
	RoleDataEx*		pRole	= CNetGlobal::RoleData();

	if( Value == true )
	{
		pRole->AddCard( CardID );
		//RoleData()->BaseData.KeyItem.SetFlagOn( ID );
	}
	else
	{
		pRole->DelCard( CardID );
		//RoleData()->BaseData.KeyItem.SetFlagOff( ID );
	}

	g_pUi_CardBook->ResetCardInfo( );
}

void NetCli_ItemChild::R_UseItemtoItemResult			( bool Result )
{
	// 判斷來源物品類型
	GameObjDbStructEx* itemDB = CNetGlobal::GetObj(s_useItemSource.OrgObjID);
	if ( itemDB == NULL )
		return;

	string itemName;
	if ( ItemField_To_ItemLink(&s_useItemTarget, itemName) == false )
		itemName = g_ObjectData->GetItemColorName(&s_useItemTarget);

	switch( itemDB->Item.Class )
	{
	case EM_Item_Rune:
		break;

	default:
		switch (itemDB->Item.UseType)
		{
		case EM_ItemUseType_Magic:
		case EM_ItemUseType_SrvScript:
		case EM_ItemUseType_CliScript:
		case EM_ItemUseType_UnPacket:
		case EM_ItemUseType_UnPacket_DropList:
		case EM_ItemUseType_Egg:
			break;

		case EM_ItemUseType_EqRefine:			//衝等
			break;

		case EM_ItemUseType_FixEq:				//修裝
			break;
		case EM_ItemUseType_Dissolution:		//拆解物品
			break;
		case EM_ItemUseType_Coloring:			//染色
			break;
		case EM_ItemUseType_ClsEqSoulbound:		//清除裝備綁定
			break;
		case EM_ItemUseType_ClsTrade:		//清除裝備綁定
			break;
		case EM_ItemUseType_ResetAbility:		//重設基本數值
			break;
		case EM_ItemUseType_ResetSkillPoint:	//重設技能點數
			break;
		case EM_ItemUseType_EQDigHole:			//裝備挖洞
			break;
		case EM_ItemUseType_EQClearRune:		//清除裝備符文
			break;

		case EM_ItemUseType_ItemLock:			//裝備鎖
			if ( Result ) {
				g_pChatFrame->SendChatMessageEx(CHAT_MSG_SYSTEM_GET, "", g_ObjectData->GetString("SYS_LOCKITEM_SUCCESS"), itemName.c_str());
			} else {
				g_pGameMain->SendWarningMsgEx(g_ObjectData->GetString("SYS_LOCKITEM_FAILED"), itemName.c_str());
			}
			break;

		case EM_ItemUseType_ItemUnLock:			//解除裝備鎖
			if ( Result ) {
				g_pChatFrame->SendChatMessageEx(CHAT_MSG_SYSTEM_GET, "", g_ObjectData->GetString("SYS_UNLOCKITEM_SUCCESS"), itemName.c_str());
			} else {
				g_pGameMain->SendWarningMsgEx(g_ObjectData->GetString("SYS_UNLOCKITEM_FAILED"), itemName.c_str());
			}
			break;

		case EM_ItemUseType_EQProtect:			//裝備保護
			if ( Result ) {
				g_pChatFrame->SendChatMessageEx(CHAT_MSG_SYSTEM_GET, "", g_ObjectData->GetString("SYS_PROTECTITEM_SUCCESS"), itemName.c_str());
			} else {
				g_pGameMain->SendWarningMsgEx(g_ObjectData->GetString("SYS_PROTECTITEM_FAILED"), itemName.c_str());
			}
			break;
		case EM_ItemUseType_EQLimitMax:
			break;
		}
	}
}
void NetCli_ItemChild::R_UsePowerLight					( int PlayerGUID , int PowerLightGUID )
{
	CRoleSprite* sprite = g_pGameMain->FindSpriteByID(PlayerGUID);	
	if ( sprite )
	{
		CRuVector3 pos;

		CRoleSprite* target = g_pGameMain->FindSpriteByID(PowerLightGUID);
		if ( target )
		{
			pos = target->GetPos();
		}
		else
		{
			pos = sprite->GetPos();
		}

		sprite->CreateCollectorFX(pos);
	}


}
void NetCli_ItemChild::R_EqRefineResult					( ItemFieldStruct&	Item , EqRefineResultENUM Result )
{
	/*
	GameObjDbStructEx* itemDB = g_ObjectData->GetObj(Item.OrgObjID);
	if ( itemDB == NULL || itemDB->Classification != EM_ObjectClass_Item )
		return;
	*/

	// 取得物品名稱
	string itemName;
	if ( ItemField_To_ItemLink(&Item, itemName) == false )
		itemName = g_ObjectData->GetItemColorName(&Item);

	// 特效名稱
	const char* effectStr = NULL;
	int point = 0;

	switch( Result )
	{
	case EM_EqRefineResult_None		:		//沒效果
		g_pRuGlobalEventManager->PlaySoundByFile("sound\\interface\\refine_fail.mp3", false, 0.0f, ruSOUNDTYPE_INTERFACE);
		g_pChatFrame->SendChatMessageEx(CHAT_MSG_SYSTEM_GET, "", g_ObjectData->GetString("SYS_REFINE_FAILED"), itemName.c_str());
		break;

	case EM_EqRefineResult_LevelUp	:		//等級上升
		g_pRuGlobalEventManager->PlaySoundByFile("sound\\interface\\refine_success.mp3", false, 0.0f, ruSOUNDTYPE_INTERFACE);
		g_pChatFrame->SendChatMessageEx(CHAT_MSG_SYSTEM_GET, "", g_ObjectData->GetString("SYS_REFINE_SUCCEEDED"), itemName.c_str());
		effectStr = g_ObjectData->GetString("FX_NAME_RUSHLEVEL_SUCCESS");
		point = eAttach_Down;
		break;

	case EM_EqRefineResult_LevelDown:		//等級下降
		g_pRuGlobalEventManager->PlaySoundByFile("sound\\interface\\refine_back.mp3", false, 0.0f, ruSOUNDTYPE_INTERFACE);
		g_pChatFrame->SendChatMessageEx(CHAT_MSG_SYSTEM_GET, "", g_ObjectData->GetString("SYS_REFINE_DESCEND"), itemName.c_str());
		effectStr = g_ObjectData->GetString("FX_NAME_RUSHLEVEL_REDUCE");
		point = eAttach_Hit_Point1;
		break;

	case EM_EqRefineResult_Broken:			//物品壞掉
		g_pRuGlobalEventManager->PlaySoundByFile("sound\\interface\\refine_breakdown.mp3", false, 0.0f, ruSOUNDTYPE_INTERFACE);
		g_pChatFrame->SendChatMessageEx(CHAT_MSG_SYSTEM_GET, "", g_ObjectData->GetString("SYS_REFINE_BROKEN"), itemName.c_str(), itemName.c_str());
		effectStr = g_ObjectData->GetString("FX_NAME_RUSHLEVEL_DESTROY");
		point = eAttach_Hit_Point1;
		break;

	case EM_EqRefineResult_Protect:			//裝備保護
		g_pChatFrame->SendChatMessageEx(CHAT_MSG_SYSTEM_GET, "", g_ObjectData->GetString("SYS_REFINE_PROTECT"), itemName.c_str());
		effectStr = g_ObjectData->GetString("FX_NAME_RUSHLEVEL_REDUCE");
		point = eAttach_Hit_Point1;
		break;
	}

	CRoleSprite* player = g_pGameMain->GetPlayer();
	if ( effectStr && player ) {
		CEffectSprite* pEffect = new CEffectSprite;
		if ( pEffect->LoadRuEntity(effectStr) )
		{
			pEffect->AttachSprite(player, point);
			pEffect->SetEffectMotionStep(eEffectMotion_Step_Explode);
		}
		else
			delete pEffect;
	}
}

void NetCli_ItemChild::R_ItemDissolutionResult			( ItemFieldStruct& Item , ItemDissolutionResultENUM Result )
{
	switch (Result)
	{
	case EM_ItemDissolutionResult_OK:

		g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_DISSOLUTION_SUCCESS"));
		break;

	case EM_ItemDissolutionResult_OK_Empty:
		// 未分解出物品
		g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_DISSOLUTION_ITEM_EMPTY"));

		// 分解失敗
		g_pRuGlobalEventManager->PlaySoundByFile("sound\\interface\\dissolution_fail.mp3", false, 0.0f, ruSOUNDTYPE_INTERFACE);
		break;

	case EM_ItemDissolutionResult_Failed:
		// 未分解出物品
		g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_DISSOLUTION_ITEM_FAILED"));
		break;
	}
}

void NetCli_ItemChild::R_TakeOutCardResult				( int CardObjID , bool Result )
{
	GameObjDbStructEx* itemDB = g_ObjectData->GetObj(CardObjID);
	const char* name = (itemDB) ? itemDB->GetName() : "";
	if ( Result ) {
		g_pChatFrame->SendChatMessageEx(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_ITEM_CARD_TAKEOUT"), name);
	} else {
		g_pChatFrame->SendChatMessageEx(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_ITEM_CARD_TAKEOUT_FAILED"), name);
	}
}

void NetCli_ItemChild::R_ApartBodyItemResult		( bool	Result , short PosSrc , short PosDes )
{
	if( Result ) {
		g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM_GET, "", g_ObjectData->GetString("SYS_APARTITEM_SUCCESS"));
		
		if ( g_pItemClipboard->GetType() != eClipboardItem_Type_None ) {
			g_pItemClipboard->Unlock();
			g_pItemClipboard->Clear();
		}
		//g_pBagFrame->Cut(PosDes);
	} else {
		g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM_GET, "", g_ObjectData->GetString("SYS_APARTITEM_FAILED"));
	}
}

void NetCli_ItemChild::R_DigHoleResult					( bool Result )
{
	if ( Result ) {
		CRoleSprite* player = g_pGameMain->GetPlayer();
		if ( player ) {
			CEffectSprite* pEffect = new CEffectSprite;
			if ( pEffect->LoadRuEntity(g_ObjectData->GetString("FX_NAME_ADDRUNEHOLE_SUCCESS")) )
			{
				pEffect->AttachSprite(player, eAttach_Down);
				pEffect->SetEffectMotionStep(eEffectMotion_Step_Explode);
			}
			else
				delete pEffect;
		}
		g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_DIGHOLE_SUCCESS"));
	} else {
		g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_DIGHOLE_FAILED"));
	}
}
void NetCli_ItemChild::R_EQCombinOpen					( int TargetNPC , int ItemPos , int Type )
{
	if (g_pBagFrame)
	g_pBagFrame->OpenItemMarge(TargetNPC,ItemPos,Type);

}
void NetCli_ItemChild::R_EQCombinClose					( )
{
	if (g_pBagFrame)
		g_pBagFrame->CloseItemMarge();
}
void NetCli_ItemChild::R_EQCombinResult					( bool Result )
{
	if (g_pBagFrame)
		g_pBagFrame->ItemMargeResult(Result);

}

void NetCli_ItemChild::R_OpenAccountBag				( int TargetNPC , int MaxCount , int ItemDBID[20] , ItemFieldStruct Item[20] )
{
	s_npcID = TargetNPC;
	s_accountBagItems = MaxCount;

	NetAccountBagItem itemData;
	for ( int i = 0; i < min(MaxCount, 20); i++ )
	{
		RegisterItemFieldChecksum( &Item[i] );

		itemData.DBID = ItemDBID[i];
		itemData.item = Item[i];
		s_accountBagList.push_back(itemData);
	}

	g_pAccountBagFrame->SendWorldEvent("ACCOUNTBAG_UPDATE");
}
void NetCli_ItemChild::R_AccountBagGetItemResult	( int ItemDBID , int BodyPos , AccountBagGetItemResultENUM Result )
{
	vector<NetAccountBagItem>::iterator iter = s_accountBagList.begin();
	while ( iter != s_accountBagList.end() ) {
		if ( iter->DBID == ItemDBID ) {
			break;
		}
		iter++;
	}

	// 物品拿取成?
	if ( Result == EM_AccountBagGetItemResult_OK ) {
		// 將物品資料清除,但是包留位置
		if ( iter != s_accountBagList.end() ) {
			iter->DBID = 0;
			iter->item.Init();
			s_accountBagItems--;
		}
	} else {
		if ( iter != s_accountBagList.end() ) {
			// 清除鎖定
			iter->item.Pos = EM_ItemState_NONE;
		}
		switch (Result)
		{
		case EM_AccountBagGetItemResult_ObjDBErr:
			g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_ACCOUNTBAG_GETITEM_DB_ERROR"));
			break;

		case EM_AccountBagGetItemResult_BodyPosErr:
			g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_ACCOUNTBAG_GETITEM_POSITION_ERROR"));
			break;

		case EM_AccountBagGetItemResult_CountErr:
			g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_ACCOUNTBAG_GETITEM_COUNT_ERROR"));
			break;

		case EM_AccountBagGetItemResult_ItemDBIDErr:
			g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_ACCOUNTBAG_GETITEM_DBID_ERROR"));
			break;
		}
	}

	g_pAccountBagFrame->SendWorldEvent("ACCOUNTBAG_UPDATE");
}

void NetCli_ItemChild::ClearAccountBag( )
{
	s_npcID = 0;
	s_accountBagItems = 0;
	s_accountBagList.clear();
}

NetAccountBagItem* NetCli_ItemChild::GetAccountBagItemInfo(int index)
{
	if ( (unsigned int)index >= (unsigned int)s_accountBagList.size() )
		return NULL;
	return &(s_accountBagList[index]);
}

void NetCli_ItemChild::R_ItemTimeout( ItemTimeoutPosENUM Pos , ItemFieldStruct& Item )
{
	// 取得物品名稱
	string itemLink;
	if ( ItemField_To_ItemLink(&Item, itemLink) == false )
		itemLink = g_ObjectData->GetItemColorName(&Item);

	switch (Pos)
	{
	case EM_ItemTimeoutPos_Body:
	case EM_ItemTimeoutPos_Bank:
	case EM_ItemTimeoutPos_EQ:
		g_pChatFrame->SendChatMessageEx(CHAT_MSG_SYSTEM_GET, "", g_ObjectData->GetString("SYS_ITEM_TIMEOUT"), itemLink.c_str());
		break;

	default:
		return;
	}
}
/*
void NetCli_ItemChild::R_SmeltEqtoMagicStoneResult	( int EQPos , int SmeltItemPos , int BoxPos , bool Result ) 
{

}
void NetCli_ItemChild::R_RefineMagicStoneResult		( int MagicStonePos[3] , int BoxPos , bool Result )
{

}
void NetCli_ItemChild::R_CombinEQandMagicStoneResult( int MagicStonePos[3] , int EQPos , int BoxPos , bool Result )  
{

}
void NetCli_ItemChild::R_RuneTransferResult		( int RunePos[2] , int BoxPos , bool Result )  
{

}
void NetCli_ItemChild::R_RefineRuneResult			( int RunePos[5] , int BoxPos , bool Result )  
{

}
*/
void NetCli_ItemChild::R_MagicBoxResult( ItemFieldStruct& Item , bool Result )
{
	if( !Result )
	{
		return;
	}

	int begin = _MAX_BODY_EX_COUNT_;
	int end = begin + _MAX_BODY_MAGIC_BOX_COUNT_;

	ItemFieldStruct* item;

	for( int i = begin + 1 ; i < end ; ++i )
	{
		item = CNetGlobal::RoleData()->GetBodyItem( i );
		item->Init();
	}
	
	item = CNetGlobal::RoleData()->GetBodyItem( begin );

	*item = Item;

	g_pInterface->SendWorldEvent( UiEvent_MagicboxChanged );
}

void NetCli_ItemChild::R_UseEq		( int PlayerGUID , int EqID )
{
	CRoleSprite* sprite = g_pGameMain->FindSpriteByID(PlayerGUID);
	if ( sprite == NULL )
		return;

	GameObjDbStruct* itemDB = g_ObjectData->GetObj(EqID);
	if ( itemDB == NULL )
		return;

	switch (itemDB->Classification)
	{
	// 物品類型
	case EM_ObjectClass_Item:
		switch (itemDB->Item.Class)
		{
		// 武器
		case EM_Item_Weapon:
			break;

		//防具
		case EM_Item_Armor:
			switch ( itemDB->Item.ArmorPos )
			{
			// 裝飾品
			case EM_ArmorPos_Ornament:
				sprite->PlayMotionOrnament("explode");
				break;
			}
			break;
		}
	}
}

void NetCli_ItemChild::R_GamebleItemStateResult( int TableID , int StatePos , int StateID , GamebleItemStateResultENUM Result )
{
	g_pBagFrame->R_GamebleItemStateResult( TableID, StatePos, StateID, Result );
}

void NetCli_ItemChild::R_GetGamebleItemStateResult( bool Result )
{
	g_pBagFrame->R_GetGamebleItemStateResult( Result );
}

void NetCli_ItemChild::R_DelItemMsg( ItemFieldStruct& Item )
{
	DeleteItemMessage(Item);
}
void NetCli_ItemChild::R_OpenGamebleItemState( int TargetID ) 
{
	g_pBagFrame->R_OpenGamebleItemState( TargetID );
}
void NetCli_ItemChild::R_CloseGamebleItemState( )
{
	g_pBagFrame->R_CloseGamebleItemState();
}
void NetCli_ItemChild::R_OpenMagicStoneExchange	( int MagicStoneID , int Lv , int Money[2] )
{
	g_pFusionStoneTradeFrame->SetOpenInfo( MagicStoneID, Lv, Money );
	g_pFusionStoneTradeFrame->SendWorldEvent( "OPEN_FUSION_STONE_TRADE_FARME" );
}
void NetCli_ItemChild::R_MagicStoneExchangeResult	( ItemFieldStruct& Item , MagicStoneExchangeResultENUM Result , int Money[2] )
{
	switch( Result )
	{
	case EM_MagicStoneExchangeResult_MoneyErr:
		g_pFusionStoneTradeFrame->SendWorldEvent( "FUSION_STONE_TRADE_NO_MONEY" );
		break;

	case EM_MagicStoneExchangeResult_ClientDataErr:
		g_pFusionStoneTradeFrame->SendWorldEvent( "FUSION_STONE_TRADE_DATA_ERR" );
		break;

	case EM_MagicStoneExchangeResult_OK:

		{
			char temp[512];
			const char* moneyType;
			const char* text;

			for ( int i = 0; i < 2; i ++ )
			{
				if ( g_ObjectData->SysValue().MagicStoneShopCostType[i] > EM_ACPriceType_BonusMoney && Money[i] > 0 )
				{
					sprintf_s(temp, 512, "SYS_MONEY_TYPE_%d", g_ObjectData->SysValue().MagicStoneShopCostType[i]);
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

	case EM_MagicStoneExchangeResult_ClientCancel:
		g_pFusionStoneTradeFrame->SendWorldEvent( "FUSION_STONE_TRADE_CLOSE" );
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
void NetCli_ItemChild::R_DrawOutRuneResult	( DrawOutRuneResultENUM Result )
{
	if (g_pDrawRuneFrame)
	g_pDrawRuneFrame->DrawOutRuneResult((int )Result);

}

//////////////////////////////////////////////////////////////////////////
//魔法衣櫥
//////////////////////////////////////////////////////////////////////////

#define CliNetWaker_PushClientData(val) CliNetWaker::PushClientData( #val, &val , sizeof(val) );
#define CliNetWaker_PopClientData(val) if( !CliNetWaker::PopClientData( #val, &val , sizeof(val) ) ){ return };


void NetCli_ItemChild::PutCoolClothRequest( int BodyPos, int ClothPos, int ObjID )
{
	CliNetWaker::PushClientData( "ClothPos", &ClothPos , sizeof(ClothPos) );
	CliNetWaker::PushClientData( "ObjID", &ObjID , sizeof(ObjID) );

	//CliNetWaker_PushClientData(ClothPos);
 	//CliNetWaker_PushClientData(ObjID);
	NetCli_Item::SL_PutCoolClothRequest( BodyPos, ClothPos );
}

void NetCli_ItemChild::R_PutCoolClothResult		( PutCoolClothResultENUM Result )
{
	int ClothPos = 0;
	int ObjID= 0;

	//CliNetWaker_PopClientData(ClothPos);
 	//CliNetWaker_PopClientData(ObjID);

	if( !CliNetWaker::PopClientData( "ClothPos", &ClothPos , sizeof(ClothPos) ) ||
		!CliNetWaker::PopClientData( "ObjID", &ObjID , sizeof(ObjID) ) )
	{
		return;
	}

	if( EM_PutCoolClothResult_OK == Result )
	{
		PlayerBaseRoleData* PlayerBaseData = CNetGlobal::RoleData()->PlayerBaseData;

		if( ClothPos < 0 || ClothPos >= _DEF_MAX_COOL_CLOTH_COUNT_ )
		{
			return ;
		}

		PlayerBaseData->CoolClothList[ClothPos] = ObjID;
	}
	g_pBagFrame->SendWorldEvent("COOLCLOTHBAG_UPDATE");
}

void NetCli_ItemChild::DelCoolClothRequest( int Pos , int ImageObjectID )
{
	CliNetWaker::PushClientData( "Pos", &Pos , sizeof(Pos) );
	NetCli_Item::SL_DelCoolClothRequest( Pos , ImageObjectID );
}

void NetCli_ItemChild::R_DelCoolClothResult		( bool Result )
{

	if( false == Result )
	{
		return;
	}

	int Pos = 0;
	if( !CliNetWaker::PopClientData( "Pos", &Pos , sizeof(Pos) ) )
	{
		return;
	}

 	PlayerBaseRoleData* PlayerBaseData = CNetGlobal::RoleData()->PlayerBaseData;
	if( Pos < 0 || Pos >= _DEF_MAX_COOL_CLOTH_COUNT_ )
	{
		return ;
	}
	PlayerBaseData->CoolClothList[Pos] = 0;

	g_pBagFrame->SendWorldEvent("COOLCLOTHBAG_UPDATE");


}


void NetCli_ItemChild::SetCoolSuitRequest( int SuitIndexID , CoolSuitBase& Info )
{
	CliNetWaker::PushClientData( "SuitIndexID", &SuitIndexID , sizeof(SuitIndexID) );
	CliNetWaker::PushClientData( "Info", &Info , sizeof(Info) );
	NetCli_Item::SL_SetCoolSuitRequest( SuitIndexID , Info );
}

void NetCli_ItemChild::R_SetCoolSuitResult		( bool Result )
{
	if( false == Result )
	{
		return;
	}

	int SuitIndexID = 0;
	CoolSuitBase Info;
	if( !CliNetWaker::PopClientData( "SuitIndexID", &SuitIndexID , sizeof(SuitIndexID) ) ||
		!CliNetWaker::PopClientData( "Info", &Info , sizeof(Info) ) )
	{
		return;
	}

	PlayerBaseRoleData* PlayerBaseData = CNetGlobal::RoleData()->PlayerBaseData;
	if( SuitIndexID < 0 || SuitIndexID >= _DEF_MAX_COOL_SUIT_COUNT_ )
	{
		return ;
	}

	PlayerBaseData->CoolSuitList[SuitIndexID] = Info;

	g_pInterface->SendWorldEvent( UiEvent_RoleCoolSuitChange );

}

void NetCli_ItemChild::ShowCoolSuitIndexRequest( int CoolSuitIndexID )
{
	CliNetWaker::PushClientData( "CoolSuitIndexID", &CoolSuitIndexID , sizeof(CoolSuitIndexID) );
	NetCli_Item::SL_ShowCoolSuitIndexRequest( CoolSuitIndexID );
}

void NetCli_ItemChild::R_ShowCoolSuitIndexResult( bool Result )
{
	if( false == Result )
	{
		return;
	}

	int CoolSuitIndexID = 0;
	if( !CliNetWaker::PopClientData( "CoolSuitIndexID", &CoolSuitIndexID , sizeof(CoolSuitIndexID) ) )
	{
		return;
	}

	PlayerBaseRoleData* PlayerBaseData = CNetGlobal::RoleData()->PlayerBaseData;
	if( CoolSuitIndexID < 0 || CoolSuitIndexID >= _DEF_MAX_COOL_SUIT_COUNT_ )
	{
		return ;
	}

	PlayerBaseData->CoolSuitID = CoolSuitIndexID;

	g_pInterface->SendWorldEvent( UiEvent_RoleCoolSuitChange );
}

void NetCli_ItemChild::SwapCoolClothRequest( int Pos1 , int Pos2 )
{
	CliNetWaker::PushClientData( "Pos1", &Pos1 , sizeof(Pos1) );
	CliNetWaker::PushClientData( "Pos2", &Pos2 , sizeof(Pos2) );
	NetCli_Item::SL_SwapCoolClothRequest( Pos1, Pos2 );
}

void NetCli_ItemChild::R_SwapCoolClothResult	( bool Result )
{
	if( false == Result )
	{
		return;
	}

	int Pos1 = 0;
	int Pos2 = 0;

	if( !CliNetWaker::PopClientData( "Pos1", &Pos1 , sizeof(Pos1) ) ||
		!CliNetWaker::PopClientData( "Pos2", &Pos2 , sizeof(Pos2) ) )
	{
		return;
	}

	if( Pos1 < 0 || Pos1 >= _DEF_MAX_COOL_CLOTH_COUNT_ )
	{
		return ;
	}

	if( Pos1 < 0 || Pos1 >= _DEF_MAX_COOL_CLOTH_COUNT_ )
	{
		return ;
	}

	PlayerBaseRoleData* PlayerBaseData = CNetGlobal::RoleData()->PlayerBaseData;

	int ObjID1 = PlayerBaseData->CoolClothList[Pos1];
	int ObjID2 = PlayerBaseData->CoolClothList[Pos2];

	PlayerBaseData->CoolClothList[Pos1] = ObjID2;
	PlayerBaseData->CoolClothList[Pos2] = ObjID1;

	g_pBagFrame->SendWorldEvent("COOLCLOTHBAG_UPDATE");

}

//////////////////////////////////////////////////////////////////////////
//特殊背包
//////////////////////////////////////////////////////////////////////////
void NetCli_ItemChild::R_PutInExBagResult( PutInExBagResultENUM Result )
{
	if (g_pPartnerFrame)
		return g_pPartnerFrame->PutInExBagResult((int) Result);

}
void NetCli_ItemChild::R_PopExBagResult( PopExBagResultENUM Result )
{
	if (g_pPartnerFrame)
		return g_pPartnerFrame->PopExBagResult( Result);
}

void NetCli_ItemChild::R_UseExBagResult( bool Result )
{
	if (g_pPartnerFrame)
		return g_pPartnerFrame->UseExBagResult( Result);
}

//////////////////////////////////////////////////////////////////////////
//稀有度3的裝備兌換
//////////////////////////////////////////////////////////////////////////
void NetCli_ItemChild::RL_OpenRare3EqExchangeItem	( int TargetNPCID )
{
	g_pRuneExchangeFrame->Open(TargetNPCID);
}
void NetCli_ItemChild::RL_Rare3EqExchangeItemResult( Rare3EqExchangeItemRequestENUM Result )
{
	switch (Result)
	{	
	case EM_Rare3EqExchangeItem_OK:		
		g_pRuneExchangeFrame->SetRE_Init();
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("RUNE_EXCHANGE_SUCCES_01"));	//兌換成！
		break;
	case EM_Rare3EqExchangeItem_MoneyError:	
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("RUNE_EXCHANGE_FAILED_04"));	//身上金額不足!
		break;
	case EM_Rare3EqExchangeItem_EQErr:	
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("RUNE_EXCHANGE_FAILED_05"));	//擺放裝備錯誤!
		break;
	case EM_Rare3EqExchangeItem_LuaErr:	
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("RUNE_EXCHANGE_FAILED_09"));	//擺放裝備錯誤!
		break;
	}
}
void NetCli_ItemChild::R_ClsAttributeResult( bool Result )
{
	g_pClearAttribStoneFrame->ClearAttribStoneResult( Result );
}
void NetCli_ItemChild::R_MagicBoxAttrDrawOffResult( bool Result )
{

	g_pBagFrame->MagicBoxAttrDrawOffResult( Result );
}