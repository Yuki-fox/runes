#include "../../mainproc/GameMain.h"
#include "../../Interface/WorldFrames/ChatFrame.h"
#include "../../Interface/WorldFrames/LootFrame.h"
#include "../../Interface/WorldFrames/BagFrame.h"
#include "../../Interface/ItemLink/ItemLink.h"
#include "../../Interface/Quest/UI_QuestDetail.h"
#include "../tools/Common/Com_Function.h"
#include "NetCli_TreasureChild.h"

//-----------------------------------------------------------------------------
bool NetCli_TreasureChild::_Init()
{
	if (This)
		return false;

	This = new NetCli_TreasureChild;


	return NetCli_Treasure::_Init();
}

//-----------------------------------------------------------------------------
bool NetCli_TreasureChild::_Release()
{
	NetCli_Treasure::_Release();
	if (This)
	{
		delete This;
		This = NULL;
	}
	return true;
}

void NetCli_TreasureChild::R_BaseInfo( int ItemGUID , bool IsEmpty )
{
	CRoleSprite* sprite = g_pGameMain->FindSpriteByID(ItemGUID);
	if ( sprite == NULL )
		return;

	if ( IsEmpty ) {
		s_booty.clear();
		g_pLootFrame->CloseBooty(false);
		sprite->SetCorpseState(false);
		sprite->ReleaseCorpseEffect();
	} else {
		sprite->SetCorpseState(true);
		sprite->CreateCorpseEffect();
	}
}

void NetCli_TreasureChild::R_OpenFailed( int ItemGUID )
{
	g_pLootFrame->OpenBooty(false, 0);
}

void NetCli_TreasureChild::R_Open( int ItemGUID , int MaxCount , ItemFieldStruct* Items )
{
	s_booty.clear();
	for ( int i = 0; i < MaxCount; i++ )
	{
		// 清除鎖定物品
		Items[i].Pos = EM_ItemState_NONE;

		s_booty.push_back(Items[i]);

		RegisterItemFieldChecksum( &Items[i] );
	}

	g_pLootFrame->OpenBooty(true, ItemGUID);
}

void NetCli_TreasureChild::R_GetItemResult( int BodyPos , int ItemSerial , TreasureGetItemResultENUM Result )
{
	// 清除鎖定物品
	for ( vector<ItemFieldStruct>::iterator iter = s_booty.begin(); iter != s_booty.end(); iter++ )
	{
		if ( iter->Serial == ItemSerial )
		{
			iter->Pos = EM_ItemState_NONE;
			break;
		}
	}

	switch (Result)
	{
	// 拿取失敗
	case EM_TreasureGetItemResult_MaxCountErr:
		{
			ItemFieldStruct* item = CNetGlobal::RoleData()->GetBodyItem(BodyPos);
			if ( item && item->Pos == EM_ItemState_CliDisabled )
			{
				item->Pos = EM_ItemState_NONE;
			}
		}

	case EM_TreasureGetItemResult_Faild:		
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_BOOTY_PICKUP_FAILED"));		
		break;

	case EM_TreasureGetItemResult_OK:
	case EM_TreasureGetItemResult_PartyQueue:
		for ( vector<ItemFieldStruct>::iterator iter = s_booty.begin(); iter != s_booty.end(); iter++ )
		{
			if ( iter->Serial == ItemSerial )
			{
				// 金錢
				if ( iter->OrgObjID == g_ObjectData->GetMonetItemID(EM_ACPriceType_GameMoney) ||
					iter->OrgObjID == g_ObjectData->GetMonetItemID(EM_ACPriceType_AccountMoney) )
				{

				}
				// 其他物品(個人獲得)
				else if ( Result == EM_TreasureGetItemResult_OK )
				{
					char temp[512];
					string itemName;
					if ( ItemField_To_ItemLink(&(*iter), itemName) == false )
						itemName = g_ObjectData->GetItemColorName(&(*iter));

					// 物品音效
					if ( g_ObjectData->GetItemQuality(NULL, &(*iter)) > 1 ) {
						g_pRuGlobalEventManager->PlaySoundByFile("sound\\interface\\ui_backpack_loot02.mp3", false, 0.0f, ruSOUNDTYPE_INTERFACE);
					} else {
						g_pRuGlobalEventManager->PlaySoundByFile("sound\\interface\\ui_backpack_loot01.mp3", false, 0.0f, ruSOUNDTYPE_INTERFACE);
					}

					// 物品進入特效
					g_pBagFrame->GiveNewItem();

					// 數量
					if ( iter->GetCount() > 1 )
					{
						sprintf_s(temp, 512, "%s x %d", itemName.c_str(), iter->GetCount());
					}
					else
					{
						sprintf_s(temp, 512, "%s", itemName.c_str());
					}

					string msg = ReplaceKeyString(g_ObjectData->GetString("SYS_GIVE_ITEM"), "<ITEM>", temp);
					g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM_GET, "", msg.c_str());

					// 任務完成條件判斷, 判斷新增物品是否為任務需求物品
					g_pQuestDetail->GetItemQueue( iter->OrgObjID, iter->GetCount() );
				}

				iter->Init();
				break;
			}
		}
		g_pLootFrame->UpdateBooty();
		break;
	}
}