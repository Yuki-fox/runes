#include "../../mainproc/GameMain.h"
#include "../../Interface/WorldFrames/CharacterFrame.h"
#include "../../Interface/WorldFrames/ChatFrame.h"
#include "../../Interface/WorldFrames/SuitSkillFrame.h"
#include "NetCli_EQChild.h"

//-----------------------------------------------------------------------------
bool NetCli_EQChild::_Init()
{
	if (This)
		return false;

	This = new NetCli_EQChild;
	return NetCli_EQ::_Init();
}

//-----------------------------------------------------------------------------
bool NetCli_EQChild::_Release()
{
	NetCli_EQ::_Release();
	if (This)
	{
		delete This;
		This = NULL;
	}

	return true;
}

//-----------------------------------------------------------------------------
void NetCli_EQChild::S_EQ2Body(ItemFieldStruct& Item, int ItemPos, int EqPos)
{
	GameObjDbStructEx* itemDB;
	switch (EqPos)
	{
	// 裝備副手
	case EM_EQWearPos_SecondHand:
		{
			ItemFieldStruct* mainHand = RoleData()->GetEqItem(EM_EQWearPos_MainHand);
			if ( !mainHand->IsEmpty() )
			{
				// 檢查主手是否為雙手
				if ( (itemDB = GetObj(mainHand->OrgObjID)) != NULL && itemDB->Item.Class == EM_Item_Weapon && itemDB->Item.WeaponPos == EM_WeaponPos_TwoHand )
				{
					// 將主手雙手武器拿下
					int pos = RoleData()->SearchBodyFirstFit(mainHand, false);
					ItemFieldStruct* bodyItem = RoleData()->GetBodyItem(pos);
					if ( bodyItem )
					{
						bodyItem->Pos = EM_ItemState_CliDisabled;
						NetCli_EQ::S_SetEQ_Body(*bodyItem, pos, EM_EQWearPos_MainHand, false);
					}
					else
					{
						g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_WEAR_EQ_ERR1"));
						return;
					}
				}
			}
		}
		break;

	case EM_EQWearPos_MainHand:
	case EM_EQWearPos_BK_MainHand:
		if ( (itemDB = GetObj(Item.OrgObjID)) != NULL )
		{
			GameObjDbStructEx* itemDB = GetObj(Item.OrgObjID);
			// 雙手劍需要拿下副手物品
			if (itemDB->Item.Class == EM_Item_Weapon && itemDB->Item.WeaponPos == EM_WeaponPos_TwoHand)
			{
				ItemFieldStruct* mainHand = RoleData()->GetEqItem(EM_EQWearPos_MainHand);
				ItemFieldStruct* offHand = RoleData()->GetEqItem(EM_EQWearPos_SecondHand);
				
				if (!offHand->IsEmpty())
				{
					int pos = RoleData()->SearchBodyFirstFit(offHand, false);
					ItemFieldStruct* bodyItem = RoleData()->GetBodyItem(pos);
					if ( bodyItem )
					{
						bodyItem->Pos = EM_ItemState_CliDisabled;
						NetCli_EQ::S_SetEQ_Body(*bodyItem, pos, EM_EQWearPos_SecondHand, false);
					}
					else
					{
						g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_WEAR_EQ_ERR1"));
						return;
					}
				}
				/*
				else if ( !offHand->IsEmpty() )
				{
					int pos = RoleData()->SearchBodyFirstFit(offHand, false);
					ItemFieldStruct* bodyItem = RoleData()->GetBodyItem(pos);
					if ( bodyItem )
					{
						bodyItem->Pos = EM_ItemState_CliDisabled;
						NetCli_EQ::S_SetEQ_Body(*bodyItem, pos, EM_EQWearPos_SecondHand, false);
					}
					else
					{
						g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_WEAR_EQ_ERR1"));
						return;
					}
				}
				*/
			}
		}
		break;
	}
	NetCli_EQ::S_SetEQ_Body(Item, ItemPos, EqPos, false);
}

//-----------------------------------------------------------------------------
void NetCli_EQChild::R_SetEQOK()
{
}

//-----------------------------------------------------------------------------
void NetCli_EQChild::R_SetEQFailed()
{
}
//-----------------------------------------------------------------------------
void NetCli_EQChild::R_EQBroken( EQWearPosENUM Pos )
{
	ItemFieldStruct* EqItem = CNetGlobal::RoleData()->GetEqItem(Pos);
	switch (Pos)
	{
	case EM_EQWearPos_Head:
	case EM_EQWearPos_Gloves:
	case EM_EQWearPos_Shoes:
	case EM_EQWearPos_Clothes:
	case EM_EQWearPos_Pants:
	case EM_EQWearPos_Back:
	case EM_EQWearPos_Belt:
	case EM_EQWearPos_Shoulder:
	case EM_EQWearPos_Necklace:	
	case EM_EQWearPos_Bow:
	case EM_EQWearPos_Ring1:
	case EM_EQWearPos_Ring2:
	case EM_EQWearPos_Earring1:
	case EM_EQWearPos_Earring2:
	case EM_EQWearPos_MainHand:
	case EM_EQWearPos_SecondHand:
	case EM_EQWearPos_Manufactory:
	case EM_EQWearPos_MagicTool1:
	case EM_EQWearPos_MagicTool2:
	case EM_EQWearPos_MagicTool3:
	case EM_EQWearPos_Ornament:
		{
			//g_pChatFrame->SendChatMessageEx(CHAT_);

			char msg[512];
			char itemName[256];

			CNetGlobal::RoleData()->GetItemFieldName(EqItem, itemName, 256);
			sprintf(msg, g_ObjectData->GetString("SYS_ITEM_BROKEN"), itemName);
			g_pGameMain->SendWarningMsg(msg);
			g_pChatFrame->SendMsgEvent(CHAT_MSG_SYSTEM, msg, "");
		}
		break;

	case EM_EQWearPos_Ammo:
		// 重新裝填彈藥
		for ( int i = 0; i < CNetGlobal::RoleData()->PlayerBaseData->Body.Count; i++ )
		{
			ItemFieldStruct* item = CNetGlobal::RoleData()->GetBodyItem(i);
			if ( item->OrgObjID == EqItem->OrgObjID )
			{
				g_pCharacterFrame->Equip(EM_EQWearPos_Ammo, 0, i);
				break;
			}
		}
		return;
	}
}

void NetCli_EQChild::R_SetShowEQ( int GItemID , ShowEQStruct ShowEQ )
{

}

void NetCli_EQChild::R_SwapBackupEQ( int BackupPosID , bool Result )
{
	RoleDataEx* Role = CNetGlobal::RoleData();
	if( Result )
	{
		Role->SwapEQ( BackupPosID );		
	}
	g_pCharacterFrame->SwapEquipResult(Result);
}

void NetCli_EQChild::R_LearnSuitSkill_Open( int TargetID )
{
	g_pSuitSkillFrame->R_Open(TargetID);
}

void NetCli_EQChild::R_LearnSuitSkillResult( int LearnSkillID , LearnSuitSkillResultENUM Result )
{
	g_pSuitSkillFrame->R_LearnSuitSkillResult(LearnSkillID, Result);
}

void NetCli_EQChild::R_SetSuitSkillResult( SetSuitSkillResultENUM Result )
{
	g_pSuitSkillFrame->R_SetSuitSkillResult(Result);
}

void NetCli_EQChild::R_UseSuitSkillResult( UseSuitSkillResultENUM Result )
{
	g_pSuitSkillFrame->R_UseSuitSkillResult(Result);
}