#include "NetCli_MoveChild.h"
#include "../mainproc/GameMain.h"
#include "../net_party/NetCli_PartyChild.h"
#include "../Net_Script/NetCli_Script_Child.h"
#include "../Net_Guild/NetCli_GuildChild.h"
#include "../Net_Other/NetCli_OtherChild.h"
#include "../../Interface/Login/AccountLogin.h"
#include "../../Interface/WorldFrames/ChatFrame.h"
#include "../../Interface/Debug/DebugMessage.h"
#include "../../interface/map/Minimap.h"
#include "../../Interface/WorldFrames/ScrollCombatTextFrame.h"
#include "../../Interface/WorldFrames/GuildFrame.h"
#include "../../Interface/GuildHouses/GuildHousesFrame.h"
#include "../../Interface/Quest/UI_QuestBook.h"
#include "../../Interface/WorldFrames/GameControl.h"
#include "../../Interface/WorldFrames/ActionFrame.h"
#include "../../Interface/GuildHouses/GuildHouseStatus.h"

map<int, MagicColdownStruct>		NetCli_MoveChild::s_magicColdown;

void Func_MoveChild_SetPlayerTitleID( int GItemID , bool IsShowTitle , int TitleID , const char* TitleStr );

//-----------------------------------------------------------------------------
bool NetCli_MoveChild::_Init()
{
	if (This)
		return false;

	This = new NetCli_MoveChild;
	return Net_Move::_Init();
}

//-----------------------------------------------------------------------------
bool NetCli_MoveChild::_Release()
{
	Net_Move::_Release();

	if (This)
	{
		delete This;
		This = NULL;
	}
	return true;
}

//-----------------------------------------------------------------------------
void NetCli_MoveChild::AnchorPosition( int GItemID, RolePosStruct* Pos )
{
	CRoleSprite* sprite = g_pGameMain->FindSpriteByID( GItemID );

	if ( sprite == NULL ) {
		g_pDebugMessage->DebugOutput( 0, "[SetPos] Can't Find then GItemID(%d)", GItemID );
		return;
	} else {
		CRoleSprite* spriteControlProxy = sprite->GetProxyControlSprite();
		CRuVector3 anchorPosition(Pos->X, Pos->Y, Pos->Z);
		float angle = Pos->Dir * PI / 180.0f;

		if ( sprite->GetGravityMode() )
		{
			// 重新定位
			CRuOptimalTerrain* pRuTerrain = g_pGameMain->GetRuTerrain();
			if ( pRuTerrain )
			{
				CRuVector3 position = anchorPosition;
				if ( sprite->GetObjectType() == eSpriteObject_Player )
				{
					float height = 0.0f;
					position.m_y += 20.0f;

					if ( !IBaseSprite::TerrainTestHight(position, height, 50.0f) )
						position.m_y = anchorPosition.m_y;
					else
						position.m_y = height;
				}
				else
				{
					position.m_y = RuEntity_FindRestingHeight(position, pRuTerrain, 50.0f);
				}			

				anchorPosition.m_y = position.m_y;
			}
		}

		spriteControlProxy->SetPos(anchorPosition);
		spriteControlProxy->SetDirection(CRuVector3(cos(angle), 0.0f, -sin(angle)), true);		
	}
}

//-----------------------------------------------------------------------------
void NetCli_MoveChild::SetEquipmentInfo( void* data, LookEQInfoStruct& EQLook, ShowEQStruct ShowEQ )
{
	if ( data == NULL )
		return;

	CRoleSprite* sprite = (CRoleSprite*)data;
	if (sprite)
	{
		int color[2];

		color[0] = EQLook.MainColor.Head;
		color[1] = EQLook.OffColor.Head;
		ObjectDataClass::SetCharacterEquipColor(g_ObjectData->GetObj(EQLook.EQ.Head), EM_GameObjectImageArmorPos_Head, color);
		sprite->SetPaperdollPart(PAPERDOLL_PART_NAME_HELMET, (ShowEQ.Head) ? EQLook.EQ.Head : 0);
		sprite->SetPaperdollColor(PAPERDOLL_PART_NAME_HELMET, color[0], color[1]);

		color[0] = EQLook.MainColor.Gloves;
		color[1] = EQLook.OffColor.Gloves;
		ObjectDataClass::SetCharacterEquipColor(g_ObjectData->GetObj(EQLook.EQ.Gloves), EM_GameObjectImageArmorPos_Gloves, color);		
		sprite->SetPaperdollPart(PAPERDOLL_PART_NAME_HAND, (ShowEQ.Gloves) ? EQLook.EQ.Gloves : 0);
		sprite->SetPaperdollColor(PAPERDOLL_PART_NAME_HAND, color[0], color[1]);		

		color[0] = EQLook.MainColor.Shoulder;
		color[1] = EQLook.OffColor.Shoulder;
		ObjectDataClass::SetCharacterEquipColor(g_ObjectData->GetObj(EQLook.EQ.Shoulder), EM_GameObjectImageArmorPos_Shoulder, color);
		sprite->SetPaperdollPart(PAPERDOLL_PART_NAME_SHOULDER, (ShowEQ.Shoulder) ? EQLook.EQ.Shoulder : 0);
		sprite->SetPaperdollColor(PAPERDOLL_PART_NAME_SHOULDER, color[0], color[1]);		

		color[0] = EQLook.MainColor.Clothes;
		color[1] = EQLook.OffColor.Clothes;
		ObjectDataClass::SetCharacterEquipColor(g_ObjectData->GetObj(EQLook.EQ.Clothes), EM_GameObjectImageArmorPos_Clothes, color);
		sprite->SetPaperdollPart(PAPERDOLL_PART_NAME_TORSO, (ShowEQ.Clothes) ? EQLook.EQ.Clothes : 0);
		sprite->SetPaperdollColor(PAPERDOLL_PART_NAME_TORSO, color[0], color[1]);

		color[0] = EQLook.MainColor.Pants;
		color[1] = EQLook.OffColor.Pants;
		ObjectDataClass::SetCharacterEquipColor(g_ObjectData->GetObj(EQLook.EQ.Pants), EM_GameObjectImageArmorPos_Pants, color);
		sprite->SetPaperdollPart(PAPERDOLL_PART_NAME_LEG, (ShowEQ.Pants) ? EQLook.EQ.Pants : 0);
		sprite->SetPaperdollColor(PAPERDOLL_PART_NAME_LEG, color[0], color[1]);		

		color[0] = EQLook.MainColor.Belt;
		color[1] = EQLook.OffColor.Belt;
		ObjectDataClass::SetCharacterEquipColor(g_ObjectData->GetObj(EQLook.EQ.Belt), EM_GameObjectImageArmorPos_Belt, color);
		sprite->SetPaperdollPart(PAPERDOLL_PART_NAME_BELT, (ShowEQ.Belt) ? EQLook.EQ.Belt : 0);
		sprite->SetPaperdollColor(PAPERDOLL_PART_NAME_BELT, color[0], color[1]);		

		color[0] = EQLook.MainColor.Shoes;
		color[1] = EQLook.OffColor.Shoes;
		ObjectDataClass::SetCharacterEquipColor(g_ObjectData->GetObj(EQLook.EQ.Shoes), EM_GameObjectImageArmorPos_Shoes, color);
		sprite->SetPaperdollPart(PAPERDOLL_PART_NAME_FOOT, (ShowEQ.Shoes) ? EQLook.EQ.Shoes : 0);
		sprite->SetPaperdollColor( PAPERDOLL_PART_NAME_FOOT, color[0], color[1]);		

		color[0] = EQLook.MainColor.Back;
		color[1] = EQLook.OffColor.Back;
		ObjectDataClass::SetCharacterEquipColor(g_ObjectData->GetObj(EQLook.EQ.Back), EM_GameObjectImageArmorPos_Back, color);
		sprite->SetPaperdollPart(PAPERDOLL_PART_NAME_BACK, (ShowEQ.Back) ? EQLook.EQ.Back : 0);
		sprite->SetPaperdollColor(PAPERDOLL_PART_NAME_BACK, color[0], color[1]);

		sprite->SetTorsoID(EQLook.EQ.Clothes);
		sprite->PaperdollBuild();

		sprite->SetHandsObjectId(eRoleHands_MainHand, EQLook.EQ.MainHand, EQLook.Level.MainHand);
		sprite->SetHandsObjectId(eRoleHands_OffHand, EQLook.EQ.SecondHand, EQLook.Level.SecondHand);
		sprite->SetHandsObjectId(eRoleHands_Ranged, EQLook.EQ.Bow, EQLook.Level.Bow, false);
		//sprite->SetHandsObjectId(eRoleHands_Crafting, EQLook.EQ.Manufacture);
		sprite->SetWeaponPosition(sprite->GetUnholsterState());

		// 裝飾品
		{
			const char* insigniaType = NULL;
			const char* bannerType = NULL;
			int insigniaColor[2] = { 0, 0 };
			int bannerColor[2] = { 0, 0 };
			if ( EQLook.EQ.Ornament > 0 )
			{
				// 公會設定資料
				GuildFlagStruct* guildFlag = g_pGuildFrame->GetGuildFlagStruct(sprite->GetGuildID());
				if ( guildFlag != NULL )
				{
					insigniaType		= g_ObjectData->GetGuildFlagInsigniaType(guildFlag->InsigniaType);
					bannerType			= g_ObjectData->GetGuildFlagInsigniaType(guildFlag->BannerType);
					insigniaColor[0]	= guildFlag->InsigniaMainColor;
					insigniaColor[1]	= guildFlag->InsigniaOffColor;
					bannerColor[0]		= guildFlag->BannerMainColor;
					bannerColor[1]		= guildFlag->BannerOffColor;
				}
			}
			sprite->CreateOrnamentEntity((ShowEQ.Ornament) ? EQLook.EQ.Ornament : 0, insigniaType, insigniaColor[0], insigniaColor[1], bannerType, bannerColor[0], bannerColor[1]);
		}
	}
}

//-----------------------------------------------------------------------------
void NetCli_MoveChild::ClearImplementColdown()
{
	s_magicColdown.clear();
}

//-----------------------------------------------------------------------------
void NetCli_MoveChild::SetImplementMagicColdown	( int GItemID, int magicID )
{
	map<int, MagicColdownStruct>::iterator iter = s_magicColdown.find(GItemID);
	if ( iter != s_magicColdown.end() )
	{
		GameObjDbStructEx* itemDB = g_ObjectData->GetObj(magicID);
		if ( itemDB && itemDB->Classification == EM_ObjectClass_MagicCollect )
		{
			int Pos = -1;
			MagicColdownStruct& coldown = iter->second;

			coldown.AllMagic = itemDB->MagicCol.Coldown.TimeAllMagic * 10;

			switch ( itemDB->MagicCol.Coldown.Class )
			{
			case EM_MagicColdownClass_None	:		//沒有
				return;

			case EM_MagicColdownClass_Job	:		//職業類型
				Pos = DEF_COLDOWN_JOB_POS + itemDB->MagicCol.Coldown.Type;
				break;

			case EM_MagicColdownClass_EQ	:		//裝備類型
				Pos = DEF_COLDOWN_EQ_POS + itemDB->MagicCol.Coldown.Type;
				break;

			case EM_MagicColdownClass_Item	:		//物品類型
				Pos = DEF_COLDOWN_ITEM_POS + itemDB->MagicCol.Coldown.Type;
				break;
			}

			if( Pos == -1 || Pos >= DEF_MAX_COLDOWN_COUNT_ALL )
				return;

			coldown.All[ Pos ] = itemDB->MagicCol.Coldown.Time * 10;
		}
	}

	g_pActionFrame->SendWorldEvent("IMPLEMENT_ACTIONBAR_UPDATE_COOLDOWN");
}

//-----------------------------------------------------------------------------
MagicColdownStruct* NetCli_MoveChild::GetImplementMagicColdown( int GItemID )
{
	map<int, MagicColdownStruct>::iterator iter = s_magicColdown.find(GItemID);
	if ( iter != s_magicColdown.end() )
		return &(iter->second);
	return NULL;
}

//-----------------------------------------------------------------------------
void NetCli_MoveChild::OnTimeProcess_HundredMS()
{
	for ( map<int, MagicColdownStruct>::iterator iter = s_magicColdown.begin(); iter != s_magicColdown.end(); iter++ )
	{
		for ( int i = 0; i < DEF_MAX_COLDOWN_COUNT_ALL; i++ )
		{
			if ( iter->second.All[i] > 0 )
				iter->second.All[i]--;
		}		
	}
}

//-----------------------------------------------------------------------------
void NetCli_MoveChild::AddToPartition( int iWorldID, int GItemID, int WorldGUID, int DBID , int MasterGUID , CampID_ENUM CampID , RolePosStruct* Pos, PlayerGraphStruct* GraphObj)
{
	//g_pDebugMessage->DebugOutput( 2, "AddToPartition GItemID(%d) WorldGUID(%d) DBID(%d) Name(%s) LV(%d) MaxHP(%d)", GItemID, WorldGUID, DBID, GraphObj->Name.Begin() , GraphObj->Level , GraphObj->MaxHP);
	//g_pError->AddMessage(0, 0, "AddToPartition GItemID(%d) WorldGUID(%d) DBID(%d) Name(%s)", GItemID, WorldGUID, DBID, GraphObj->Name.Begin());
	/*
	char Buf[256];
	sprintf( Buf , "AddToPartition GItemID(%d) WorldGUID(%d)" , GItemID, WorldGUID );
	g_pChatFrame->AddMessage( 5 , Buf );
	*/

	bool fadeIn = true;
	GameObjDbStructEx* pObj = NULL;	
	if ( (pObj = g_ObjectData->GetObj(GraphObj->OrgObjID)) == NULL )
		return;	

	GameObjDbStructEx* imageDB = g_ObjectData->GetObj(pObj->ImageID);

	SpriteShadowType shadowType = eSpriteShadowType_Projected;
	if ( imageDB && !imageDB->Image.IsShowShadow )
		shadowType = eSpriteShadowType_None;

	bool gravityMode = GraphObj->Mode.Gravity;		

	CRoleSprite* pSprite = NULL;
	int oldGItemID = GItemID;
	switch ( pObj->Classification )
	{
	case EM_ObjectClass_Player:
		pSprite = g_pGameMain->FindSpriteByWorldGUID(WorldGUID);
		if ( pSprite ) {
			oldGItemID = pSprite->GetIdentify();
		}
		break;

	default:
		DBID = -1;
		break;
	}

	//g_pError->AddMessage(0, 0, "AddToPartition[%d][%d][%d] (%.2f, %.2f, %.2f)", GItemID, DBID, WorldGUID, Pos->X, Pos->Y, Pos->Z);

	bool isNew;
	pSprite = g_pGameMain->CreateSprite(GItemID, DBID, WorldGUID, isNew);
	if ( pSprite )
	{
		char buff[1024];
		CRoleSprite* spriteControlProxy = NULL;
		CRuVector3 pos(Pos->X, Pos->Y, Pos->Z);
		CRuVector3 anchorPosition;
		int theta = (int)(min(max(Pos->Dir, 0.0f), 359.0f));
		float angle = theta * PI / 180.0f;
		
		const char* name = GraphObj->Name.Begin();
		const char* originalName = NULL;

		// 重新定位高度
		if ( g_pGameMain->GetRuTerrain() )
		{			
			anchorPosition.m_x = pos.m_x;
			anchorPosition.m_y = pos.m_y;
			anchorPosition.m_z = pos.m_z;
			anchorPosition.m_y = RuEntity_FindRestingHeight(anchorPosition, g_pGameMain->GetRuTerrain(), 100.0f);
		}		

		// 修正戰場中玩家名稱
		if ( pObj->Classification == EM_ObjectClass_Player )
		{
			// 世界戰場中
			if ( g_pGameMain->IsBattleGroundZone() && g_pGameMain->IsWorldBattleGround() )
			{
				// 取得伺服器名稱
				const char* serverName = g_pAccountLogin->FindWorldNameByID(iWorldID);
				if ( serverName != NULL )
				{
					// 玩家名稱修正為 伺服器名稱-玩家名稱
					sprintf_s(buff, 1024, "%s-%s", serverName, name);
					originalName = name;
					name = buff;
				}
			}
		}
		// 修正NPC名稱
		else
		{
			int len = (int)strlen(name);
			if ( len > 0 ) {
				name = g_ObjectData->GetSpecialString(name);
			} else {
				name = g_ObjectData->GetSpecialString(pObj->GetName() );
			}
		}

		// 種族
		Race_ENUM race = EM_Race_None;
		int originalObjectId = 0;
		int imageId = GraphObj->OrgObjID;

		switch ( pObj->Classification )
		{
		case EM_ObjectClass_Player:
			race = GraphObj->Race;

			// 調整不顯示
			if ( g_pGameMain->GetPlayerWorldGUID() != WorldGUID && isNew )
			{
#ifndef NOCHECKMEM
				bool displayDummy = false;

				g_pGameMain->CheckMemoryStatus(true);

				if ( g_pGameMain->IsDebaseTexture() && (g_pGameMain->GetMemoryLoad() > 96 || g_pGameMain->GetCurrentMemory() > REPLACE_MEMORY_SIZE) )
					displayDummy = true;
				else 
				{
					int playerCount = g_pGameMain->GetPlayerSpriteCount();
					if ( (g_pGameMain->InStressZone() && playerCount >= REPLACE_PLAYER_NUM) || playerCount >= REPLACE_MAX_PLAYER_NUM )
						displayDummy = true;
				}

				if ( displayDummy )
				{
					// 釋放坐騎資料
					/*
					pSprite->ReleaseMountSprite();
					spriteControlProxy = pSprite;
					*/

					// 設定替代物件
					originalObjectId = imageId;
					imageId = DUMMY_ID;
				}

#endif //NOCHECKMEM
			}
			break;

		case EM_ObjectClass_NPC:
			if ( pObj->NPC.HideRaceInfo )
				race = EM_Race_None;
			else
				race = (Race_ENUM)pObj->NPC.Race;
			break;
		}

		pSprite->SetOriginalObjectId(originalObjectId);
		pSprite->SetObjectId(imageId, true);

		pSprite->SetServerMove( GraphObj->Mode.Move );

		spriteControlProxy = pSprite->GetProxyControlSprite();
		switch ( pObj->Classification )
		{
		case EM_ObjectClass_Player:
			// 玩家
			// g_pError->AddMessage(0, 0, "AddToPartition GItemID[%d] WorldGUID[%d]", GItemID, WorldGUID);

			// g_pGameMain->ComparePlayerName();

			// 記錄玩家座標,當在公會城時修正座標用
			g_pGameMain->AddPartitionPosition(pSprite, pos);

			// 影子
			if ( shadowType != eSpriteShadowType_None )
				shadowType = eSpriteShadowType_Projected;

			// 是否為本地玩本
			if ( g_pGameMain->GetPlayerWorldGUID() == WorldGUID )
			{
				// 特別角度會有問題
				if ( theta == 270 )
					angle = 272.0f * PI / 180.0f;

				// 換 ZONE 時 SERVER 有可能給的座標已經穿牆內需要將角色拉回, 
				// 使用角色所在的座標為準(偏移量差距應該不應該太大)
				if ( !isNew ) {
					float distance = (pos - spriteControlProxy->GetPos()).Magnitude();

					if ( spriteControlProxy->GetMoveTime() > 0.0f && distance < 20.0f ) {
						// 移動中不移動
						pos = spriteControlProxy->GetPos();
					} else {						
						pos = anchorPosition;
					}
				}

				g_pGameMain->AddPlayerRefCount();
				g_pGameMain->InsertAnchorHeightSprite(GItemID, pos);

				//
				// g_pError->AddMessage(0, 0, "AddToPartition Player GItemID[%d] WorldGUID[%d] (%.2f, %.2f, %.2f)", GItemID, WorldGUID, Pos->X, Pos->Y, Pos->Z);

				// 本地玩家是否顯示標題
				pSprite->SetNameVisibleForce(g_pGameMain->GetSelfTitleVisible());
				pSprite->SetNameVisible(true);
				pSprite->SetCursorState(false);
				pSprite->SetEnemyState(false);
				pSprite->SetCanAttackState(false);
				g_pGameMain->SetPlayer(pSprite, pos, CRuVector3(cos(angle), 0.0f, -sin(angle)));
				Net_Move::PlayerMoveObject(GItemID, Pos->X, Pos->Y, Pos->Z, angle, 0, 0.0f, 0.0f, 0.0f, EM_ClientMoveType_Normal);

				_RoleData.BaseData.HP = (float)GraphObj->HP;
				_RoleData.BaseData.MP = (float)GraphObj->MP;
				_RoleData.BaseData.SP = (float)GraphObj->SP;
				_RoleData.BaseData.SP_Sub = (float)GraphObj->SP_Sub;
			}
			// 非本機玩家
			else
			{
				// 是否顯示名稱
				pSprite->SetNameVisibleForce(g_pGameMain->GetPlayerTitleVisible());
				pSprite->SetNameVisible(true);
			}

			// 是否在戰場中
			pSprite->SetBattleGround( g_pGameMain->IsBattleGroundZone() );

			// 是否為隊友
			pSprite->SetPartyState( NetCli_PartyChild::InParty(GraphObj->Name.Begin()) );

			// 是否在團隊內
			pSprite->SetRaidState( NetCli_PartyChild::InGroup(GraphObj->Name.Begin()) );

			// 設定公會資訊
			g_pGameMain->SetSpriteGuildID(pSprite, GraphObj->GuildID);

			// 顯示稱號
			Func_MoveChild_SetPlayerTitleID( GItemID, GraphObj->IsShowTitle, GraphObj->TitleID, "" );

			// 記錄存在的角色DBID
			if( DBID != -1 && g_pGameMain->IsBattleGroundZone() != false )
			{
				NetCli_OtherChild::InsertDBIDRoleName( DBID, GraphObj->Name.Begin() );
			}

			// 尋找所有玩家找尋寵物物件
			g_pGameMain->UpdatePetSprite(pSprite);
			break;

		case EM_ObjectClass_NPC:
		case EM_ObjectClass_QuestNPC:
			pSprite->SetAlwaysHoldWeapon(pObj->NPC.OtherFlag.ForceAttachkWeapon);
			pSprite->SetHideMasterName(!pObj->NPC.IsShowMaster);

			// 影子
			if ( shadowType != eSpriteShadowType_None )
				shadowType = eSpriteShadowType_Dot;
			
			pSprite->SetCursorState(GraphObj->Mode.Mark);
			pSprite->SetNameVisible(!GraphObj->Mode.HideName);								// 不能出現頭上字串
			pSprite->SetNameVisibleForce(g_pGameMain->GetNPCTitleVisible());				// 系統設定顯示
			pSprite->SetGuildText(g_ObjectData->GetSpecialString(pObj->NPC.TitleName));

			// 頭像框
			pSprite->SetTargetState(GraphObj->Mode.ShowRoleHead);

			if ( pObj->NPC.IsAllZoneVisible ) {
				// gravityMode = false;
				pSprite->SetStaticObjectState(true);										// 設定全區物件

				g_pGameMain->InsertGlobalObject(GItemID);
				// g_pError->AddMessage(0, 0, "AddToPartition AllZoneObject GItemID(%d) WorldGUID(%d) DBID(%d) (%.2f, %.2f, %.2f)", GItemID, WorldGUID, DBID, Pos->X, Pos->Y, Pos->Z);
			}			
			break;

		case EM_ObjectClass_Item:			
			// 沒有影子
			shadowType = eSpriteShadowType_None;

			if ( pObj->Item.ItemType == EM_ItemType_PowerLight)		// 效果光球
			{
				pSprite->SetObjectType(eSpriteObject_Effect);
				pSprite->SetCanAttackState(false);
				pSprite->SetTouchState(true);
				pSprite->SetTargetState(false);
				pSprite->SetNameVisible(false);				
			}
			break;
		case EM_ObjectClass_Mine:
			{
				// 沒有影子
				shadowType = eSpriteShadowType_None;

				pSprite->SetCursorState(true);		// 可以被游標檢測
				pSprite->SetTouchState(false);		// 可以被點選
				pSprite->SetTargetState(false);		// 不能出現目標框
				pSprite->SetNameVisible(false);		// 不能出現頭上字串

				// 檢查類型, 插入不同類別
				switch( pObj->Mine.Type )
				{
				case EM_GameObjectMineType_Mine:
					pSprite->SetMinimapIcon( 9 ); // EM_MINIMEM_MinimapOption_Mine
					pSprite->SetCheckedStatus( eChecked_Mine );  
					break;
				case EM_GameObjectMineType_Wood:
					pSprite->SetMinimapIcon( 10 ); // EM_MINIMEM_MinimapOption_Wood
					pSprite->SetCheckedStatus( eChecked_Lumber );  
					break;
				case EM_GameObjectMineType_Herb:
					pSprite->SetMinimapIcon( 11 ); // EM_MINIMEM_MinimapOption_Herb
					pSprite->SetCheckedStatus( eChecked_Herb );  
					break;
				}
				
			} break;
		case EM_ObjectClass_Flag:
			{
				// 沒有影子
				shadowType = eSpriteShadowType_None;

				pSprite->SetCheckedStatus( eChecked_Flag );
			} break;
		}

		// 名稱加上"xxx 的墓碑"
		char tempName[512];
		if ( GraphObj->OrgObjID == g_ObjectData->GetTombID() )
		{
			g_pLogList->AddMessage("[AddToPartition TOMB] questID:%d touchRange:%d rangePlot:%d", GraphObj->NpcQuestID, GraphObj->TouchPlot_Range, GraphObj->RangePlot_Range);
			g_pLogList->AddMessage("[AddToPartition TOMB] Name:%s OwnerID:%d PlayerID:%d Position:(%.2f, %.2f, %.2f)", name, MasterGUID, g_pGameMain->GetGItemID(), pos.m_x, pos.m_y, pos.m_z);

			pSprite->SetCheckedStatus( eChecked_Tomb );  
			
			sprintf_s(tempName, 512, g_ObjectData->GetString("SYS_PLAYER_TOMB"), name);
			name = tempName;			

			// 強制去除墓碑種族
			race = EM_Race_None;

			pos = anchorPosition;

			gravityMode = false;

			// 非本機玩家取消游標碰觸,並將主人ID去除
			if ( MasterGUID == g_pGameMain->GetGItemID() )
			{
				MasterGUID = 0;
				pSprite->SetCursorState(true);
			}
			else
				pSprite->SetCursorState(false);			
		}
		else if ( GraphObj->OrgObjID == g_ObjectData->GetTeleportID() )
		{
			sprintf_s(tempName, 512, g_ObjectData->GetString("SYS_PLAYER_TELEPORT"), name);
			name = tempName;
		}

		// PK掉裝袋子
		if ( GraphObj->OrgObjID == g_ObjectData->GetTreasureObjID() && isNew )
		{			
			pSprite->SetActorState(ruFUSION_ACTORSTATE_SPAWN_BEGIN);
			pSprite->SetCheckedStatus(eChecked_QuestObj);
			fadeIn = false;
		}

		pSprite->SetDisableAction(GraphObj->Mode.DisableAction);					// 不能撥放動作
		pSprite->SetDisableRotate(GraphObj->Mode.DisableRotate);					// 不能轉向
		pSprite->SetTitleIconVisible(g_pGameMain->GetTitleIconVisible());			// 設定頭上圖示
		pSprite->SetShadowType(shadowType);

		// Set fixed sprite orienting axis if the axis is valid
		if(fabs(GraphObj->vX) > ruEPSILON || fabs(GraphObj->vY) > ruEPSILON || fabs(GraphObj->vZ) > ruEPSILON)
		{
			// Set fixed orienting axis
			pSprite->SetOrientingAxis(CRuVector3(GraphObj->vX, GraphObj->vY, GraphObj->vZ));

			// Enable re-basing of direction vector around the orienting axis
			pSprite->SetRebaseDirection(true);

			// Disable orient to surface normal
			pSprite->SetOrientToSurfaceNormal(false);

			// Set orientation mode to unconstrained
			pSprite->SetOrientMode(CRuFusion_Actor::ruORIENTMODE_UNCONSTRAINED);
		}
		else
		{
			// Set sprite's upward orientation setting
			if ( imageDB )
			{
				pSprite->SetOrientToSurfaceNormal(imageDB->Image.ObjectType == EM_ObjectImage_4Foots ? true : false);
			}

			// Set orientation mode to constrained
			pSprite->SetOrientMode(CRuFusion_Actor::ruORIENTMODE_CONSTRAINED);
		}		

		// 淡入顯示
		if ( fadeIn )
			pSprite->SetFade(false, 1.0f);		

		pSprite->ClearCheckPosition();
		pSprite->SetWorldID(iWorldID);
		pSprite->SetGoodEvilType(GraphObj->GoodEvilType, pSprite->GetGoodEvilValue());		
		pSprite->SetName(name);
		pSprite->SetOriginalName(originalName);
		pSprite->SetVocation(GraphObj->Voc, GraphObj->Voc_Sub);
		pSprite->SetLevel(GraphObj->Level, GraphObj->Level_Sub);
		pSprite->SetSex(GraphObj->Sex);
		pSprite->SetRace(race);
		pSprite->SetEnemyHidden(GraphObj->Mode.EnemyHide);
		pSprite->SetBloodBarHidden(GraphObj->Mode.NotShowHPMP);

		int baseValue[2];
		g_pGameMain->TranslateBaseValue(pSprite, GraphObj->MaxMP, GraphObj->MaxSP, GraphObj->MaxSP_Sub, baseValue);
		pSprite->SetMaxHP(GraphObj->MaxHP);
		pSprite->SetMaxMP(baseValue[0]);
		pSprite->SetMaxSP(baseValue[1]);
		g_pGameMain->TranslateBaseValue(pSprite, GraphObj->MP, GraphObj->SP, GraphObj->SP_Sub, baseValue);
		pSprite->SetHP(GraphObj->HP);
		pSprite->SetMP(baseValue[0]);
		pSprite->SetSP(baseValue[1]);		

		pSprite->SetMasterID(MasterGUID);
		pSprite->SetMasterSprite(g_pGameMain->FindSpriteByID(MasterGUID), GraphObj->SysFlag.IsContorlPet);
		
		/*
		pSprite->SetGuildID(GraphObj->GuildID);
		if ( GraphObj->GuildID > 0 )
			pSprite->SetTitleText(NetCli_GuildChild::GetGuildNameByID(GraphObj->GuildID));
		*/
		
		pSprite->SetGravityMode(gravityMode);

		// 坐標位置相關
		if ( pSprite->GetAttachParentId() != 0 )
		{
			pSprite->SetDirection(CRuVector3(cos(angle), 0.0f, -sin(angle)), true, true);
			pSprite->SetMoveSpeed(GraphObj->MoveSpeed);
		}
		else
		{
			// 不是載具才改變上層元件速度
			if ( pSprite->GetImplementSprite() != spriteControlProxy )
				spriteControlProxy->SetMoveSpeed(GraphObj->MoveSpeed);

			spriteControlProxy->SetDirection(CRuVector3(cos(angle), 0.0f, -sin(angle)), true, true);			
		}

		// 更換坐標
		switch (pObj->Classification)
		{
		case EM_ObjectClass_NPC:
		case EM_ObjectClass_QuestNPC:
			// 設定 IdleMotion 如果有的話
			switch ( GraphObj->IdleMotionID )
			{
			case ruFUSION_ACTORSTATE_COMBAT_1H:
			case ruFUSION_ACTORSTATE_COMBAT_2H:
			case ruFUSION_ACTORSTATE_COMBAT_POLEARM:
			case ruFUSION_ACTORSTATE_COMBAT_UNARMED:
			case ruFUSION_ACTORSTATE_COMBAT_BOW:
			case ruFUSION_ACTORSTATE_COMBAT_GUN:
				spriteControlProxy->SetForceCombatMotion(true);
				break;

			default:
				spriteControlProxy->SetForceCombatMotion(false);
			}
			spriteControlProxy->SetActorState((CRuFusion_ActorState)GraphObj->IdleMotionID);
			if ( isNew || spriteControlProxy->GetMoveTime() < 0.01f )
				spriteControlProxy->SetPos(pos);
			else
				spriteControlProxy->SetTargetPos(pos);
			break;

		default:
			// 新物件重設坐標
			if ( isNew ) {
				spriteControlProxy->SetPos(pos);
			} else {
				if ( g_pGameMain->GetPlayerWorldGUID() == WorldGUID )
				{
					// 相同區域重新加入,或者玩家更換區域距離差距過大
					if ( GItemID == oldGItemID || (spriteControlProxy->GetPos() - pos).Magnitude() > GraphObj->MoveSpeed )
						spriteControlProxy->SetPos(pos);
				} else if ( spriteControlProxy->GetMoveTime() < 0.01f ) {
					spriteControlProxy->SetPos(pos);
				}
			}
			break;
		}

		// 重新定位角色高度,如果是新產生物件或物不在移動中
		if ( gravityMode && GraphObj->Mode.NoFix_Y == false && (isNew || spriteControlProxy->GetMoveTime() < 0.01f) )
		{
			if ( g_pGameMain->InsertAnchorHeightSprite(GItemID, pos) == false )
				spriteControlProxy->AnchorSprite(30.0f);
		}

		if( GraphObj->MinimapIcon != 0 )
			pSprite->SetMinimapIcon( GraphObj->MinimapIcon );

		pSprite->SetCursorMode( GraphObj->CursorType );

		//if( GraphObj->CursorType != 0 )

		//if( GraphObj->

		pSprite->SetObjectAttackState( GraphObj->Mode.Fight );
		pSprite->SetAFK( GraphObj->AllZoneState.AFK );
		pSprite->SetFindParty( GraphObj->AllZoneState.FindParty );
		pSprite->SetBotting( GraphObj->AllZoneState.Dig );

		// 播放指定的姿勢
		DoAction( GItemID, GraphObj->CliActionID, 0 );

		// 強制設定 PC 為顯示頭像框
		if( pObj->Classification == EM_ObjectClass_Player )
		{
			pSprite->SetTargetState(true);
		}


		// 顯示或隱藏 志偉說它做了
		//if( GraphObj->Mode.Show == false )
		//{
		//	pSprite->Hide();	// 隱藏物件
		//}		

		// 判斷物件行為. 原則是, 一個物件同時應該只能有一種行為. 要是有兩種以上, 那就代表資料設定錯誤
		//-------------------------------------------------------------------------------
		int	iSetCount = 0;

		// 給予任務 NPC 或 任務 Obj
		//if( GraphObj->NpcQuestID != 0 )
		int iQuestNpcBeginID	= Def_ObjectClass_QuestNPC;
		int iQuestNpcEndID		= Def_ObjectClass_QuestNPC + Def_ObjectClass_QuestNPC_Total;
		if( GraphObj->OrgObjID >= iQuestNpcBeginID && GraphObj->OrgObjID < iQuestNpcEndID )
		{
			// 取得 QuestNpc 物件
			GameObjectNpcStruct			*pNPC	= NULL;
			GameObjDbStructEx			*pObj	= g_ObjectData->GetObj( GraphObj->OrgObjID );
			
			if( pObj )
				pNPC	= &( g_ObjectData->GetObj( GraphObj->OrgObjID )->NPC );

			if( pNPC )
			{
				switch( pNPC->iQuestMode )
				{
				case 0:
					{
						pSprite->SetCheckedStatus( eChecked_QuestNPC );	
					} break;
				case 1:
					{
						pSprite->SetCheckedStatus( eChecked_QuestObj );
					} break;
				case 2:
					{
						pSprite->SetCheckedStatus( eChecked_QuestObj );
						pSprite->SetObjectType( eSpriteObject_BattleGroupObj );
					} break;

				}
			}
			
			iSetCount++;
		}

		/*
		// 判斷是否為商人
		//if( GraphObj->Action.OpenShop || GraphObj->Action.OpenBank || GraphObj->Action.OpenAC )
		if(		GraphObj->Action.OpenType == EM_RoleOpenMenuType_Shop 
			||	GraphObj->Action.OpenType == EM_RoleOpenMenuType_Bank 
			||	GraphObj->Action.OpenType == EM_RoleOpenMenuType_AC )			
		{
			pSprite->SetCheckedStatus( eChecked_QuestNPC );
		}

		// 判斷是否為信箱
		//if( GraphObj->Action.OpenMail )
		if( GraphObj->Action.OpenType == EM_RoleOpenMenuType_Mail )
		{
			pSprite->SetCheckedStatus( eChecked_Mail );			
		}
		*/

		// 觸碰觸發劇情 OBJ
		bool bModify = false;

		if( pSprite->GetTouchRange() != GraphObj->TouchPlot_Range || pSprite->GetAreaRange() != GraphObj->RangePlot_Range )
		{
			pSprite->SetQuestInfo(GraphObj->NpcQuestID, GraphObj->TouchPlot_Range, GraphObj->RangePlot_Range);
			CNetCli_Script_Child::OnModifyScriptState( pSprite );
		}
		else
		{
			pSprite->SetQuestInfo(GraphObj->NpcQuestID, GraphObj->TouchPlot_Range, GraphObj->RangePlot_Range);
		}

		// 設定sprite顯示
		pSprite->SetVisible(GraphObj->Mode.Show);

		// 設定sprite碰撞開關
		if ( pSprite->GetObjectType() != eSpriteObject_Player )
		{
			pSprite->EnableCollision(GraphObj->Mode.Obstruct);
			pSprite->EnableCollisionBase(GraphObj->Mode.Obstruct);
		}

		// 設定sprite碰撞劇情開關
		pSprite->EnablePlotCollisionTrigger(GraphObj->Mode.CollisionPlot);


		// 設定 Questlink
		if( pObj != NULL && CNetCli_Script_Child::CheckQuestlink( pObj ) == true )
		{
			if( pSprite->GetCheckedStatus() == eChecked_QuestObj )
			{
				pSprite->SetQuestMinimapIcon( (int)EM_MapIconType_QuestOBJ );

				if( pObj->NPC.IgnoreQuestObjEffect == false )
					pSprite->CreateCorpseEffect();
			}
			else
				pSprite->SetQuestMinimapIcon( (int)EM_MapIconType_QuestNPC );			
		}			

		/*
		if ( GraphObj->TouchPlot_Range != 0 )
		{
			pSprite->SetTouchPlotState( true );	//pSprite->SetCheckedStatus( eChecked_TouchPlot );
			CNetCli_Script_Child::OnModifyScriptState( pSprite );
		}
		else
		{
			pSprite->SetTouchPlotState( false );
		}

		// 距離觸發劇情 OBJ
		if ( GraphObj->RangePlot_Range != 0 )
		{
			pSprite->SetRangePlotState( true );				//pSprite->SetCheckedStatus( eChecked_RangePlot );
		}
		else
		{
			pSprite->SetRangePlotState( false );
		}
		*/

		// 水平移動
		if ( pSprite->GetObjectType() != eSpriteObject_Player ) {
			pSprite->SetHorizontal(GraphObj->Mode.Drag);
		}

		//
		switch ( pObj->Classification )
		{
		case EM_ObjectClass_NPC:
		case EM_ObjectClass_QuestNPC:
			if ( pObj->NPC.Prompt )
				g_pGameMain->AddEliteBell(GItemID);
			break;
		}

		if( iSetCount > 1 )
		{
			// POP Message AddToPartition 有兩個以上的行為設定.
			g_pDebugMessage->DebugOutput(0, "AddToPartition  GItemID(%d) WorldGUID(%d) Obj 有兩個以上的行為設定", GItemID, WorldGUID);
		}
		//-------------------------------------------------------------------------------
		ActionType( GItemID , GraphObj->Action );		

		// 陣營設定
		g_pGameMain->SetSpriteCampId(pSprite, CampID);

		// 更新BUFF資訊
		g_pGameMain->SpriteMsgProc(pSprite, eSpriteMsg_Buff_Changed, 0, 0);

		// 強制背旗子
		if( GraphObj->iForceFlagID != 0 )
		{
			pSprite->CreateBackEntity( GraphObj->iForceFlagID );
		}

		// 設定預設IDLE動作
		if ( GraphObj->iDefIdleMotion != 0 )
		{
			pSprite->SetDefaultIdleAction( (CRuFusion_Mime_Action) GraphObj->iDefIdleMotion );
		}

		// 執行本地顯示劇情
		if( pObj->Classification == EM_ObjectClass_QuestNPC )
		{
			if( strlen( pObj->NPC.szLuaDisplayScript ) != 0 )
			{
				if( g_pQuestBook->CheckQuestBeginLua( pObj->NPC.szLuaDisplayScript ) == true )
				{
					pSprite->Hide();
					pSprite->ScriptHide();	// 隱藏 NPC
					pSprite->EnableCollision( false );
				}
			}
		}
	}
	else
	{
		char Buf[256];
		sprintf( Buf , "AddToPartition GItemID(%d) WorldGUID(%d) 編號錯誤!!" , GItemID, WorldGUID );
		g_pChatFrame->SendMsgEvent(CHAT_MSG_SYSTEM, Buf, "");
	}			
}

//-----------------------------------------------------------------------------
void NetCli_MoveChild::DelFromPartition( int GItemID , int WorldGUID )
{
	//g_pDebugMessage->DebugOutput( 2, "DelFromPartition GItemID(%d) WorldGUID(%d)", GItemID, WorldGUID);
	//g_pError->AddMessage(0, 0, "DelFromPartition GItemID(%d) WorldGUID(%d)", GItemID, WorldGUID);

	// 當本機玩家切換不同區域時,可能照成舊SERVER 刪除比新SERVER加入還來的慢
	if ( g_pGameMain->GetPlayerWorldGUID() == WorldGUID )
	{
		g_pGameMain->DelPlayerRefCount();

		CRoleSprite* sprite = g_pGameMain->FindSpriteByWorldGUID(WorldGUID);
		if ( sprite && sprite->RefCount() < g_pGameMain->PlayerRefCount() )
		{
			return;
		}
	}

	if ( !g_pGameMain->DeleteSprite(WorldGUID) )
	{
		//char Buf[256];
		//sprintf( Buf , "DelFromPartition GItemID(%d) WorldGUID(%d) 找不到刪除編號!!" , GItemID, WorldGUID );
		//g_pChatFrame->SendMsgEvent(CHAT_MSG_SYSTEM, Buf, "");
	}
}

//-----------------------------------------------------------------------------
void NetCli_MoveChild::SetPos( int GItemID, RolePosStruct* Pos )
{
	AnchorPosition(GItemID, Pos);

	CRoleSprite* sprite = g_pGameMain->FindSpriteByID( GItemID );
	if ( sprite )
	{
		CRoleSprite* spriteControlProxy = sprite->GetProxyControlSprite();

		sprite->ClearCheckPosition();

		// 修正子物件坐標
		spriteControlProxy->UpdateChildSprite();
	}

	if ( GItemID == g_pGameMain->GetGItemID() )
	{
		CRuVector3 position(Pos->X, Pos->Y, Pos->Z);

		CRoleSprite* player = g_pGameMain->GetPlayer();
		if ( player )
		{
			CRoleSprite* spriteControlProxy = player->GetProxyControlSprite();

			position = spriteControlProxy->GetPos();
		}

		g_pGameMain->SetPlayerPos(position);

		// 回送座標
		Net_Move::PlayerMoveObject(GItemID, Pos->X, Pos->Y, Pos->Z, Pos->Dir, 0, 0.0f, 0.0f, 0.0f, EM_ClientMoveType_Normal);
	}
}

//-----------------------------------------------------------------------------
void NetCli_MoveChild::ObjectMove( int GItemID, RolePosStruct* Pos, float vX , float vY, float vZ, ClientMoveTypeENUM Type  )
{
	CRoleSprite* player = g_pGameMain->GetPlayer();
	CRoleSprite* sprite = g_pGameMain->FindSpriteByID( GItemID );

	if ( sprite == NULL ) {
		g_pDebugMessage->DebugOutput( 0, "[ObjectMove] Can't Find then GItemID(%d)", GItemID );
		return;
	} else {
		CRoleSprite* spriteControlProxy = sprite->GetProxyControlSprite();
		float speed = spriteControlProxy->GetMoveSpeed();

		//char buf[512];
		CRuVector3 nowPos, tarPos, forward;		
		double angle = Pos->Dir;
		if ( abs(Pos->Dir - 270.5f) < 0.51f )
			angle = 271.0;

		angle = angle / 180.0 * 3.14159265;
		float distance;

		nowPos		= spriteControlProxy->GetPos();
		tarPos.m_x	= Pos->X;
		tarPos.m_y	= Pos->Y;
		tarPos.m_z	= Pos->Z;
		forward.m_x	= (REAL)cos(angle);
		forward.m_y	= 0.0f;
		forward.m_z	= (REAL)-sin(angle);
		distance	= (nowPos - tarPos).Magnitude();

		forward.Normalize();

		switch (Type)
		{
		case EM_ClientMoveType_Normal:
		case EM_ClientMoveType_Mount:
			// 特殊移動或者參考父元件
			if ( spriteControlProxy->GetMoveType() == eRoleSpriteMove_Charge || sprite->GetAttachParentId() != 0 )
				return;

			switch (sprite->GetObjectType())
			{
			case eSpriteObject_Unknown:
				break;

			case eSpriteObject_Player:
				if ( player == sprite ) {
					return;
				} else if ( player ) {
					CRuVector3 playerPos = player->GetPos();
					int count = g_pGameMain->GetPlayerCount();
					if ( count < 20 || (count < 60 && (playerPos - tarPos).Magnitude() < 300.0f) )
					{
						AutoReportBaseInfoStruct info;
						info.GUID = GItemID;
						info.X = tarPos.m_x;
						info.Y = tarPos.m_y;
						info.Z = tarPos.m_z;

						// 記錄玩家封包內座標點
						switch ( sprite->PushCheckPosition(tarPos) )
						{
						case -1:
							NetCli_OtherChild::SL_AutoReportPossibleCheater(info, EM_AutoReportPossibleCheater_ThroughtWall, sprite->GetDBID());
							break;
						case -2:
							NetCli_OtherChild::SL_AutoReportPossibleCheater(info, EM_AutoReportPossibleCheater_Teleport, sprite->GetDBID());
							break;
						case -3:
							NetCli_OtherChild::SL_AutoReportPossibleCheater(info, EM_AutoReportPossibleCheater_Floating, sprite->GetDBID());
							break;
						}
					} else {
						sprite->ClearCheckPosition();
					}
				}				

				// if ( spriteControlProxy->IsImplementSprite() == false )
				{
					// 改變朝向
					spriteControlProxy->SetDirection(forward, true);
				}

				if ( Type == EM_ClientMoveType_Mount )
				{
					CRoleSprite* parentMount = g_pGameMain->FindSpriteByID(sprite->GetParentMountId());
					if ( parentMount )
					{
						tarPos = parentMount->GetPos() - sprite->GetParentMountOffset();
					}					
				}

				// 結束移動,向量大小為零
				if ( vX == 0.0f && vZ == 0.0f )	{
					spriteControlProxy->SetMoveVector(CRuVector3(0.0f, 0.0f, 0.0f));			// 清除移動向量

					if ( Type == EM_ClientMoveType_Mount )
					{
						spriteControlProxy->SetPos(tarPos);
					}
					else if ( !spriteControlProxy->IsTargetMove() )
					{
						// 若差距過大或過小則做座標點修正
						if ( distance > 40.0f ) {
							spriteControlProxy->SetPos(tarPos);
						} else if ( distance > 8.0f && spriteControlProxy->IsImplement() == false ) {
							CRuVector3 newVec = tarPos - nowPos;

							// 移動到達時間
							float moveTime = newVec.Magnitude() / speed;
							CRuVector3 moveVector = newVec.Normalize() * speed;

							// 將目標向量轉換為角色Z軸向量
							CRuQuaternion quaternion;
							quaternion.RotateIntoVector(CRuVector3(0.0f, 0.0f, 1.0f), forward);
							quaternion.TransformVector(moveVector);		
							spriteControlProxy->SetMoveVector(moveVector, moveTime, true);
						}
					}
				} else {			// 處理移動向量
					CRuVector3 moveVector(vX, 0.0f, vZ);		
					CRuQuaternion quaternion;

					// 若差距過大則做座標點修正
					if ( distance > 15.0f )	{
						spriteControlProxy->SetPos(tarPos);
						nowPos = tarPos;
					}

					CRuVector3 otherPos = tarPos + moveVector * 0.3f;				// 預測移動1.2秒座標位置
					CRuVector3 newVec = (otherPos - nowPos) / 0.6f;					// 取得新的每秒移動向量
					CRuVector3 tmpVec[2];
					newVec.m_y = 0.0f;

					// 朝向做偏位,將角色移動前進向量做旋轉
					CRuVector3 newForward = forward;
					tmpVec[0] = newVec;
					tmpVec[1] = moveVector;
					tmpVec[0].Normalize();
					tmpVec[1].Normalize();
					quaternion.RotateIntoVector(tmpVec[0], tmpVec[1]);
					quaternion.TransformVector(newForward);

					// 將目標向量轉換為角色Z軸向量
					quaternion.RotateIntoVector(CRuVector3(0.0f, 0.0f, 1.0f), forward);
					quaternion.TransformVector(moveVector);

					moveVector.m_y = 0.0f;
					if ( abs(moveVector.m_x) < 2.0f )
						moveVector.m_x = 0.0f;
					if ( abs(moveVector.m_z) < 2.0f )
						moveVector.m_z = 0.0f;

					// g_pChatFrame->SendChatMessageEx(CHAT_MSG_SYSTEM, "", "%f %f %f", moveVector.m_x, moveVector.m_y, moveVector.m_z);

					if ( spriteControlProxy->IsImplement() == false )
					{
						spriteControlProxy->SetForwardVector(newForward);
					}

					spriteControlProxy->SetMoveVector(moveVector, 1.0f, true);			// 角色朝目標移動1.2秒
				}
				break;

			case eSpriteObject_NPC:
			case eSpriteObject_QuestNPC:

				// 如果是載具不處理移動
				if ( spriteControlProxy->IsImplement() )
					return;

				// 設定面向
				if ( spriteControlProxy->GetDisableRotate() ) {
					spriteControlProxy->SetDirection(CRuVector3((REAL)cos(angle), 0.0f, (REAL)-sin(angle)), true, false);
				} else {
					spriteControlProxy->SetCorrectDir(CRuVector3((REAL)cos(angle), 0.0f, (REAL)-sin(angle)));
				}

				// 兩點座標差值
				if ( spriteControlProxy->IsInWater() == false )
				{
					CRuVector3 serverPos;
					serverPos.m_x = vX;
					serverPos.m_y = vY;
					serverPos.m_z = vZ;

					// 距離差距過遠
					if ( (nowPos - serverPos).Magnitude() > speed )
					{
						spriteControlProxy->SetPos(serverPos);

						// 修正子物件坐標
						spriteControlProxy->UpdateChildSprite();
					}
				}
				else
				{
					// 定位正常高度
					CRuOptimalTerrain* pRuTerrain = g_pGameMain->GetRuTerrain();

					CRuVector3 serverPos;
					serverPos.m_x = vX;
					serverPos.m_y = vY;
					serverPos.m_z = vZ;

					if ( pRuTerrain ) {
						serverPos.m_y = RuEntity_FindRestingHeight(serverPos, pRuTerrain, 150.0f);			// 定位目標座標點Y軸高度
					}

					// 距離差距過遠
					if ( (nowPos - serverPos).Magnitude() > speed )
					{
						spriteControlProxy->SetPos(serverPos);

						// 修正子物件坐標
						spriteControlProxy->UpdateChildSprite();
					}
				}

				// 有重力物件重新定位Y軸
				if ( sprite->GetGravityMode() )
				{
					// 定位正常高度
					CRuOptimalTerrain* pRuTerrain = g_pGameMain->GetRuTerrain();

					// This is unnecessary... actor takes care of anchoring as the entity is walking
					if ( pRuTerrain ) {
						CRuVector3 anchorPosition = tarPos;
						tarPos.m_y = RuEntity_FindRestingHeight(anchorPosition, pRuTerrain);			// 定位目標座標點Y軸高度
					}

					/*
					// 重新定位Y軸高度
					CRuVector3 nowPos = spriteControlProxy->GetPos();
					if ( abs(nowPos.m_y - tarPos.m_y) > 50.0f )
					{
						nowPos.m_y = tarPos.m_y;
						spriteControlProxy->SetPos(nowPos);

						// Notify actor that its position has been reset
						if(spriteControlProxy->GetRuActor())
						{
							spriteControlProxy->GetRuActor()->NotifyPositionReset();
						}
					}
					spriteControlProxy->SetAnchorHeight(tarPos.m_y);
					*/
				}
				spriteControlProxy->SetTargetPos(tarPos);
				break;
			}
			break;

		case EM_ClientMoveType_Jump:
			// 改變朝向
			spriteControlProxy->SetDirection(forward);
			break;

		case EM_ClientMoveType_SetPos:
			{
				// 重新定位
				bool isResetAnchor = true;

				sprite->ClearCheckPosition();

				// 改變朝向
				spriteControlProxy->SetDirection(forward, true, true);

				// 本機玩家重新定位
				if ( GItemID == g_pGameMain->GetGItemID() )
				{
					// 回送座標
					Net_Move::PlayerMoveObject(GItemID, Pos->X, Pos->Y, Pos->Z, Pos->Dir, 0, 0.0f, 0.0f, 0.0f, EM_ClientMoveType_Normal);

					isResetAnchor = false;
				}

				// NPC vX, vY, vZ 為目標座標
				switch (sprite->GetObjectType())
				{
				case eSpriteObject_Player:
					if ( isResetAnchor && spriteControlProxy )
						spriteControlProxy->SetPos(tarPos);
					break;

				case eSpriteObject_NPC:
				case eSpriteObject_QuestNPC:
					{
						Pos->X = vX;
						Pos->Y = vY;
						Pos->Z = vZ;
					}
					if ( isResetAnchor )
						AnchorPosition(GItemID, Pos);
					break;
				}				
			}
			break;
		}
	}
}

//-----------------------------------------------------------------------------
void NetCli_MoveChild::DoAction( int GItemID  , int ActionID , int TempActionID )
{
	CRoleSprite* pSprite = g_pGameMain->FindSpriteByID( GItemID );
	if ( pSprite )
	{
		PLAYER_ACTOR_STATE actorState;
		RoleSpriteStatus spriteState;
		spriteState.value = ActionID;
		actorState.value = TempActionID;

		if ( actorState.background != ruFUSION_ACTORSTATE_NORMAL )
			pSprite->SetActorState((CRuFusion_ActorState)actorState.background, 1);

		if ( actorState.transient != ruFUSION_ACTORSTATE_NORMAL )
			pSprite->SetActorState((CRuFusion_ActorState)actorState.transient, 3);

		// 特殊類型設定
		switch (pSprite->GetObjectType())
		{
		case eSpriteObject_Player:
			pSprite->SetWalkState(spriteState.isWalk);
			pSprite->SetCombatState(spriteState.isCombat);
			break;

		case eSpriteObject_NPC:
		case eSpriteObject_Effect:
		case eSpriteObject_QuestNPC:
		case eSpriteObject_BattleGroupObj:
			// pSprite->SetForceCombatMotion(spriteState.isCombat);
			break;
		}
		
		pSprite->SetWeaponPosition(spriteState.isUnholster);

		// 永久性動作
		pSprite->SetSitState(spriteState.isSit);

		pSprite->SetSitChairState(spriteState.isSitChair);		


		/*
		if ( ActionID ) 
		{
			pSprite->Motion(TempActionID);
		}
		else
		{
			RoleSpriteStatus status;
			status.flag = ActionID ;
			int action = status.action;

			pSprite->SetFight( status.isFight );
			pSprite->SetWalk( status.isWalk );
			pSprite->SetWeaponPosition( status.isUnholster );		

			switch ( action )
			{
			case eAction_Sit:
			case eAction_Sleep:
				if ( pSprite->IsMove() )
					pSprite->SetAction( action );
				else
				{
					SpriteEventInfo event;
					event.func = CRoleSprite::DelayAction;
					event.nowTime = 0.5f;
					event.userData[0] = action;
					pSprite->PushEvent( -2, event );
				}
				break;

			default:
				if ( action != eAction_Null )
					pSprite->SetAction( action );
				break;
			}
		}
		*/
	}
	else
		g_pDebugMessage->DebugOutput(0, "[DoAction] Can't Find then GItemID(%d)", GItemID);
}

//-----------------------------------------------------------------------------
void NetCli_MoveChild::MoveSpeed( int GItemID  , float Speed )
{
	CRoleSprite* sprite = g_pGameMain->FindSpriteByID( GItemID );
	if ( sprite == NULL ) {
		g_pDebugMessage->DebugOutput(0, "[MoveSpeed] Can't Find then GItemID(%d)", GItemID);
		return;
	} else if ( sprite->GetAttachParentId() == 0 ) {
		CRoleSprite* spriteControlProxy = sprite->GetProxyControlSprite();

		sprite->SetMoveSpeed(Speed);

		// 不是載具才改變上層元件速度
		if ( sprite->GetImplementSprite() != spriteControlProxy )
			spriteControlProxy->SetMoveSpeed(Speed);
	}		
}

//-----------------------------------------------------------------------------
void NetCli_MoveChild::ModelRate( int GItemID  , float ModelRate )
{
	CRoleSprite* sprite = g_pGameMain->FindSpriteByID(GItemID);
	if ( sprite )
	{
		sprite->SetImmediateScale(ModelRate);
	}	
}

//-----------------------------------------------------------------------------
void NetCli_MoveChild::EQInfo( int GItemID , LookEQInfoStruct&    EQLook  , ShowEQStruct ShowEQ )
{
	CRoleSprite* pSprite = g_pGameMain->FindSpriteByID( GItemID );
	if ( pSprite )
	{
		if ( pSprite->GetObjectId() == DUMMY_ID )
		{
			PlayerEquipmentStruct equipInfo;
			equipInfo.EQLook = EQLook;
			equipInfo.ShowEQ = ShowEQ;
			pSprite->CreateEquipmentInfo(&equipInfo, sizeof(equipInfo));
			return;
		}

		SetEquipmentInfo(pSprite, EQLook, ShowEQ);

		if ( GItemID == g_pGameMain->GetGItemID() )
		{
			if ( g_pInterface )
			{
				lua_State* L = g_pInterface->GetLuaState();
				if ( L )
				{
					g_pInterface->SendWorldEvent("PLAYER_EQUIPMENT_UPDATE");
				}
			}
		}

		/*
		GuildFlagStruct* guildFlag = g_pGuildFrame->GetGuildFlagStruct(pSprite->GetGuildID());
		if ( guildFlag )
		{
			// Flag Object's number 220977
			pSprite->CreateFlagEntity( EQLook.EQ.Ornament,
				g_ObjectData->GetGuildFlagInsigniaType(guildFlag->InsigniaType), guildFlag->InsigniaMainColor, guildFlag->InsigniaOffColor,
				g_ObjectData->GetGuildFlagInsigniaType(guildFlag->BannerType), guildFlag->BannerMainColor, guildFlag->BannerOffColor );
		}
		*/
		
	}
	else
		g_pDebugMessage->DebugOutput( 0, "[EQInfo] Can't Find then GItemID(%d)", GItemID );
}

//-----------------------------------------------------------------------------
void NetCli_MoveChild::FixEQInfo( int GItemID , EQWearPosENUM  Pos , int EQ )
{
	/*
	CRoleSprite* pSprite = g_pGameMain->FindSpriteByID( GItemID );
	if ( pSprite )
	{		
		switch ( Pos )
		{
		case EM_EQWearPos_Head:
			pSprite->SetPaperdollId( ePaperdoll_Helmet, EQ );
			break;

		case EM_EQWearPos_Gloves:
			pSprite->SetPaperdollId( ePaperdoll_Hand, EQ );
			break;

		case EM_EQWearPos_Shoes:
			pSprite->SetPaperdollId( ePaperdoll_Foot, EQ );
			break;

		case EM_EQWearPos_Clothes:
			pSprite->SetPaperdollId( ePaperdoll_Torso, EQ );
			break;

		case EM_EQWearPos_Pants:
			pSprite->SetPaperdollId( ePaperdoll_Leg, EQ );
			break;

		case EM_EQWearPos_Back:
			pSprite->SetPaperdollId( ePaperdoll_Back, EQ );
			break;

		case EM_EQWearPos_Belt:
			pSprite->SetPaperdollId( ePaperdoll_Belt, EQ );
			break;

		case EM_EQWearPos_Shoulder:
			pSprite->SetPaperdollId( ePaperdoll_Shoulder, EQ );
			break;

		case EM_EQWearPos_Necklace:
			break;

		case EM_EQWearPos_Ammo:
			break;

		case EM_EQWearPos_Ring1:
			break;

		case EM_EQWearPos_Ring2:
			break;

		case EM_EQWearPos_Earring1:
			break;

		case EM_EQWearPos_Earring2:
			break;

		case EM_EQWearPos_MainHand:
			pSprite->SetLeftObjectId( EQ );			
			break;

		case EM_EQWearPos_SecondHand:
			pSprite->SetLeftObjectId( EQ );
			break;
		}

		pSprite->PaperdollBuild();
	}
	else
		g_pDebugMessage->DebugOutput(0, "[FixEQInfo] Can't Find then GItemID(%d)", GItemID);
	*/
}

//-----------------------------------------------------------------------------
void NetCli_MoveChild::LookFace( int GItemID , LookStruct& Look )
{
	CRoleSprite* pSprite = g_pGameMain->FindSpriteByID( GItemID );
	if ( pSprite )
	{
		g_pDebugMessage->DebugOutput(2, "[LookFace] GItemID(%d)", GItemID);
		pSprite->SetLock(Look);

		/*
		pSprite->SetPaperdollColor(PAPERDOLL_PART_NAME_HEAD, Look.BodyColor, 0);
		pSprite->SetPaperdollColor(PAPERDOLL_PART_NAME_HAIR, Look.HairColor, 0);
		pSprite->SetPaperdollPart(PAPERDOLL_PART_NAME_HEAD, Look.FaceID);
		pSprite->SetPaperdollPart(PAPERDOLL_PART_NAME_HAIR, Look.HairID);

		pSprite->SetBoneScale(BONE_SCALE_NAME_HEAD,		(BYTE) Look.BoneScale[0] / 100.0f);
		pSprite->SetBoneScale(BONE_SCALE_NAME_BREASTS,	(BYTE) Look.BoneScale[1] / 100.0f);
		pSprite->SetBoneScale(BONE_SCALE_NAME_BODY,		(BYTE) Look.BoneScale[2] / 100.0f);
		pSprite->SetBoneScale(BONE_SCALE_NAME_CHEST,	(BYTE) Look.BoneScale[3] / 100.0f);
		pSprite->SetBoneScale(BONE_SCALE_NAME_PELVIS,	(BYTE) Look.BoneScale[4] / 100.0f);
		pSprite->SetBoneScale(BONE_SCALE_NAME_ARM,		(BYTE) Look.BoneScale[5] / 100.0f);
		pSprite->SetBoneScale(BONE_SCALE_NAME_FOREARM,	(BYTE) Look.BoneScale[6] / 100.0f);
		pSprite->SetBoneScale(BONE_SCALE_NAME_HAND,		(BYTE) Look.BoneScale[7] / 100.0f);
		pSprite->SetBoneScale(BONE_SCALE_NAME_LEGS,		(BYTE) Look.BoneScale[8] / 100.0f);
		pSprite->SetBoneScale(BONE_SCALE_NAME_CALF,		(BYTE) Look.BoneScale[9] / 100.0f);
		pSprite->SetBoneScale(BONE_SCALE_NAME_FOOT,		(BYTE) Look.BoneScale[10] / 100.0f);
		pSprite->PaperdollBuild();		
		*/
	}
	else
		g_pDebugMessage->DebugOutput(0, "[LookFace] Can't Find then GItemID(%d)", GItemID);
}

//-----------------------------------------------------------------------------
void NetCli_MoveChild::ClientAutoPlot( int GItemID , char* AutoPlot )
{
}

//-----------------------------------------------------------------------------
void NetCli_MoveChild::ActionType(int GItemID, RoleActionStruct Action)
{
	CRoleSprite* sprite = g_pGameMain->FindSpriteByID(GItemID);
	if ( sprite )
	{
		sprite->SetDisableQuestState( Action.DisableQuest );

		// 判斷是否為商人
		//if( Action.OpenShop || Action.OpenBank || Action.OpenAC )
 		if( sprite->GetObjectType() != eSpriteObject_Player &&
			( Action.OpenType == EM_RoleOpenMenuType_Shop 
			|| Action.OpenType == EM_RoleOpenMenuType_Bank 
			|| Action.OpenType == EM_RoleOpenMenuType_AC) )
		{
			sprite->SetCheckedStatus( eChecked_Shop );
		}

		// 判斷是否為信箱
		//if( Action.OpenMail )
		if( Action.OpenType == EM_RoleOpenMenuType_Mail ) 
		{
			sprite->SetCheckedStatus( eChecked_Mail );
		}
		
		CRoleSprite* player = g_pGameMain->GetPlayer();
		if ( player == sprite ) {
			// 進入戰鬥
			g_pGameMain->SetCombatState(Action.AttackMode);
		} else if ( sprite ){
			switch (sprite->GetObjectType())
			{
			case eSpriteObject_NPC:
			case eSpriteObject_QuestNPC:
				// 攻擊狀態(由SERVER控制)
				sprite->SetCombatState(Action.Attack);

				// 走路模式(由SERVER控制)
				sprite->SetWalkState(Action.Walk);
				sprite->SetCursorEnableState( !Action.DisableClick );		
				break;
			}

			// 收拔武器
			sprite->SetWeaponPosition(Action.Unholster);

			// 死亡(所有物件共有)
			if ( Action.Dead ) {
				if ( !sprite->GetDeadState() ) {
					sprite->SetActorState(ruFUSION_ACTORSTATE_DEAD);
					sprite->SetDeadState(true);
				}
			} else if ( sprite->GetDeadState() ) {
				// 解除死亡狀態
				sprite->SetDeadState(false);
			}

			// 坐下
			if ( Action.Sit ) {
				sprite->SetActorState(ruFUSION_ACTORSTATE_SIT_BEGIN);
			}
		}
	}
}

//-----------------------------------------------------------------------------
void NetCli_MoveChild::R_JumpBegin( int GItemID , float X , float Y , float Z , float Dir , float vX , float vY , float vZ )
{
	/*
	char Buf[256];
	sprintf( Buf , "[R_JumpBegin]GItemID = %d (%.2f %.2f %.2f) (%.2f, %.2f, %.2f)", GItemID, X , Y , Z, vX, vY, vZ );
	g_pDebugMessage->DebugOutput( 2, Buf );
	*/

	CRoleSprite* sprite = g_pGameMain->FindSpriteByID( GItemID );
	if ( sprite )
	{
		CRoleSprite* spriteControlProxy = sprite->GetProxyControlSprite();
		float angle = Dir * PI / 180.0f;

		spriteControlProxy->SetPos(CRuVector3(X, Y, Z));
		spriteControlProxy->SetDirection(CRuVector3(cos(angle), 0.0f, -sin(angle)), true);

		// 當是騎馬狀態時
		if ( sprite->GetRideState() ) {
			spriteControlProxy->Jump();
		} else {
			spriteControlProxy->ForcedJump(CRuVector3(vX, vY, vZ));
		}
	}
}

//-----------------------------------------------------------------------------
void NetCli_MoveChild::R_Relation( int GItemID , RoleRelationStruct&   Relation )
{
	/*
	char Buf[256];
	sprintf( Buf , "GItemID = %d Relation=%d " , GItemID , Relation );
	g_pDebugMessage->DebugOutput( 2, Buf );
	*/

	CRoleSprite* pSprite = g_pGameMain->FindSpriteByID( GItemID );
	if ( pSprite && pSprite != g_pGameMain->GetPlayer() )
	{
		GameObjDbStructEx* NPC = g_ObjectData->GetObj( pSprite->GetObjectId() );

		switch (pSprite->GetObjectType())
		{
		//-----------------------------------------------------------------------------
		case eSpriteObject_Player:
			pSprite->SetEnemyState(Relation.Enemy);
			pSprite->SetCanAttackState(Relation.CanAttack);
			break;
		//-----------------------------------------------------------------------------
		case eSpriteObject_NPC:		
		case eSpriteObject_QuestNPC:
			if( NPC->IsNPC() && NPC->NPC.SearchRange == 0 )
				pSprite->SetEnemyState( false );
			else
				pSprite->SetEnemyState( Relation.Enemy );
			pSprite->SetTouchPlotState(Relation.TouckPlot);
			if ( pSprite->GetTouchRange() != 0 && Relation.TouckPlot == false )
				pSprite->SetCursorState(false);
			pSprite->SetCanAttackState(Relation.CanAttack);
			break;
		//-----------------------------------------------------------------------------
		case eSpriteObject_Effect:
			break;
		//-----------------------------------------------------------------------------
		case eSpriteObject_BattleGroupObj:
			{
				if( Relation.BGTouch == true )
					pSprite->SetCursorState( true );
				else
					pSprite->SetCursorState( false );
			} break;
		}
	}
}

//-----------------------------------------------------------------------------
void NetCli_MoveChild::R_PartyMemberMove	( int DBID , int GItemID , float X , float Y , float Z )
{
	/*
	char Buf[256];
	sprintf( Buf , "[Party成員移動資訊]DBID = %d ZoneID=%d X=%f Y=%f Z=%f" , DBID , ZoneID , X , Y , Z );
	g_pDebugMessage->DebugOutput( 2, Buf );
	*/

	/*
	// 角色若是在範圍內則由一般更新封包做用
	CRoleSprite* sprite = g_pGameMain->FindSpriteByDBID(DBID);	
	if ( sprite == NULL ) {		
	}
	*/

	// g_pError->AddMessage(0, 0, "[R_PartyMemberMove] DBID=%d ZoneID=%d (%.2f,%.2f,%.2f)", DBID, ZoneID, X, Y, Z);

	PartyMemberInfo* info = NetCli_PartyChild::GetGroupMemberByDBID(DBID);
	if ( info ) {
		CRoleSprite* sprite = (CRoleSprite*)(info->sprite);

		sprite->SetIdentify(GItemID);

		info->GItemID = GItemID;

		// 更換位置
		sprite->SetPos(CRuVector3(X, Y, Z));
	}
}
//-----------------------------------------------------------------------------
void NetCli_MoveChild::R_PlayMotion			( int iObjID, int iMotionID )
{
	CRoleSprite* sprite = g_pGameMain->FindSpriteByID( iObjID );
	if ( sprite == NULL )
		return;

	CRoleSprite* spriteControlProxy = sprite->GetProxyControlSprite();
	spriteControlProxy->SetActorState((CRuFusion_ActorState)iMotionID);
}
//-----------------------------------------------------------------------------
void NetCli_MoveChild::R_SpecialMove		( int GItemID , SpecialMoveTypeENUM MoveType , int TargetGItemID , float X , float Y , float Z , float Dir , int MagicCollectID )
{
	CRoleSprite* sprite = g_pGameMain->FindSpriteByID(GItemID);	
	if ( sprite )
	{
		CRoleSprite *spriteControlProxy = sprite->GetProxyControlSprite();
		CRuVector3 targetPos = CRuVector3(X, Y, Z);
		CRuVector3 direction = CRuVector3(cos(Dir*PI/180.0f), 0.0f, -sin(Dir*PI/180.0f));
		CRuVector3 spritePos = spriteControlProxy->GetPos();		

		switch(MoveType)
		{
		case EM_SpecialMoveType_Dash:
			//衝擊
			{
				CRoleSprite *target = g_pGameMain->FindSpriteByID(TargetGItemID);
				if ( target )
				{
					CRuVector3 moveVector = spritePos - target->GetPos();
					moveVector.m_y = 0.0f;

					float attackRange = max(target->GetAttackRange() + spriteControlProxy->GetAttackRange() + 4.0f, 20.0f);
					if ( moveVector.Magnitude() > attackRange )
						targetPos = target->GetPos() + (moveVector.Normalize() * attackRange);
					else
						targetPos = spritePos;
				}

				if ( targetPos != spritePos )
					spriteControlProxy->ChargeToPos(targetPos);

				/*
				{
					CRuVector3 pos = spriteControlProxy->GetPos();
					CRuVector3 moveVector(targetPos.m_x - pos.m_x, targetPos.m_y - pos.m_y, targetPos.m_z - pos.m_z);
					float speed = 150.0f;

					// 朝正Z軸移動
					// g_pError->AddMessage(0, 0, "Charge (%.2f, %.2f, %.2f)->(%.2f, %.2f, %.2f) Time[%.2f]", pos.m_x, pos.m_y, pos.m_z, targetPos.m_x, targetPos.m_y, targetPos.m_z, moveVector.Magnitude()/speed);
				}
				*/
			}
			break;

		case EM_SpecialMoveType_StrikeBack:
			//震退
			spriteControlProxy->StrikeBack(targetPos);
			break;

		case EM_SpecialMoveType_StrikeAway:
			//震飛
			break;
			
		case EM_SpecialMoveType_Bomb:
			// 爆炸
			if ( spriteControlProxy )
			{
				spriteControlProxy->JumpEx();
			}
			break;
		}

		if ( sprite == g_pGameMain->GetPlayer() )
		{
			Net_Move::PlayerMoveObject(sprite->GetIdentify(), X, Y, Z, Dir, 0, 0.0f, 0.0f, 0.0f, EM_ClientMoveType_Normal);
		}
	}
}
//-----------------------------------------------------------------------------
void NetCli_MoveChild::R_Ping				( int Time )
{
	g_pGameMain->ReceivePing( Time );
}

//Server認定 Client 需要載地圖資料，等待Client端回應再加入顯像分割區
void NetCli_MoveChild::R_ClientLoading	( float CameraX , float CameraY , float CameraZ )
{
	g_pGameMain->SetPlayerCameraPos(CRuVector3(CameraX, CameraY, CameraZ));
}

void NetCli_MoveChild::SetMinimapIcon( int iObjID, int iIconID )
{
	CRoleSprite* pObj = g_pGameMain->FindSpriteByID( iObjID );
	if ( pObj )
		pObj->SetMinimapIcon( iIconID );
}

//角色特效通知
void NetCli_MoveChild::R_SpecialEfficacy	( int GItemID , SpecialEfficacyTypeENUM	EffectType )
{
	CRoleSprite* sprite = g_pGameMain->FindSpriteByID(GItemID);
	if ( sprite == NULL )
		return;

	switch ( EffectType )
	{
	case EM_SpecialEfficacyType_ChangeJob:
		{
			CEffectSprite* pEffect;

			// down
			pEffect = new CEffectSprite;
			if ( pEffect->LoadRuEntity("model\\fx\\transfer\\act_transfer_t_down.ros") ) {
				pEffect->AttachSprite(sprite, eAttach_Down);
				pEffect->SetEffectMotionStep(eEffectMotion_Step_Explode);
			} else
				delete pEffect;

			// up
			pEffect = new CEffectSprite;
			if ( pEffect->LoadRuEntity("model\\fx\\transfer\\act_transfer_t_top.ros") )	{
				pEffect->AttachSprite(sprite, eAttach_Top);
				pEffect->SetEffectMotionStep(eEffectMotion_Step_Explode);
			} else
				delete pEffect;
		}
		break;
	}
}

//設定面向
void NetCli_MoveChild::R_SetDir			( int GItemID , float Dir )
{
	char Buf[256];
	sprintf( Buf , "R_SetDir 設定面向 GItemID=%d  Dir=%f" , GItemID , Dir );
	g_pDebugMessage->DebugOutput( 2, Buf );

	CRoleSprite* sprite = g_pGameMain->FindSpriteByID(GItemID);
	if ( sprite )
	{
		float angle = Dir * PI / 180.0f;
		sprite->SetDirection(CRuVector3(cos(angle), 0.0f, -sin(angle)), true);
	}
}

void NetCli_MoveChild::R_SetIdleMotion	( int GItemID, int iMotionID )
{
	CRoleSprite* sprite = g_pGameMain->FindSpriteByID( GItemID );
	if( sprite )
	{
		sprite->SetDefaultIdleAction( (CRuFusion_Mime_Action) iMotionID );
	}
}

void NetCli_MoveChild::R_SetGuildID		( int GItemID , int GuildID )
{
	CRoleSprite* sprite = g_pGameMain->FindSpriteByID(GItemID);
	if ( sprite )
	{
		g_pGameMain->SetSpriteGuildID(sprite, GuildID);

		/*
		sprite->SetGuildID(GuildID);
		if ( GuildID > 0 )
			sprite->SetTitleText(NetCli_GuildChild::GetGuildNameByID(GuildID));
		*/
	}
}

void Func_MoveChild_SetPlayerTitleID( int GItemID , bool IsShowTitle , int TitleID , const char* TitleStr )
{
	CRoleSprite* sprite = g_pGameMain->FindSpriteByID( GItemID );
	if ( sprite )
	{
		const char* text = NULL;

		// 是否顯示頭銜
		if ( IsShowTitle && TitleID != 0 ) {
			if ( TitleID == -1 ) {
				// 自定稱號
				text = TitleStr;
			} else {
				// 取得稱號資料
				GameObjDbStructEx* itemDB = g_ObjectData->GetObj(TitleID);
				if ( itemDB ) {
					text = itemDB->GetName();
				}
			}
		}

		switch (sprite->GetObjectType())
		{
		case eSpriteObject_Player:
			// 是否顯示玩家稱號
			sprite->SetTitleVisible( g_pGameMain->GetTitleVisible() );
			sprite->SetTitleText(text);
			break;

		default:
			// 是否顯示NPC頭銜
			sprite->SetGuildVisible( g_pGameMain->GetGuildVisible() );			

			// 非玩家顯示在[公會資訊內]
			sprite->SetGuildText(text);
			break;
		}
	}
}

//
// IsShowTitle	顯示或不顯示
// TitleID		物件稱號 0無 -1自定稱號 其他為稱號名稱
// TiTleStr		自定稱號
void NetCli_MoveChild::R_SetTitleID		( int GItemID , bool IsShowTitle , int TitleID , const char* TitleStr )
{
	if ( GItemID == g_pGameMain->GetGItemID() ) {
		RoleDataEx* Player = CNetGlobal::RoleData();
		Player->BaseData.IsShowTitle = IsShowTitle;
		Player->BaseData.TitleID = TitleID;
		Player->PlayerBaseData->TitleStr = TitleStr;		
		Player->TempData.UpdateInfo.Recalculate_All = true;
	}
	Func_MoveChild_SetPlayerTitleID( GItemID , IsShowTitle , TitleID , TitleStr );
}

void NetCli_MoveChild::R_PlayerDropDownDamage( int Damage , int Type )
{
	CRoleSprite* player = g_pGameMain->GetPlayer();
	if ( player == NULL )
		return;

	SCTInfo sctInfo;
	sctInfo.source = player->GetIdentify();
	sctInfo.target = player->GetIdentify();

	// 傷害數值顯示
	sctInfo.type = eSCT_Normal_Damage;
	sctInfo.hp = Damage;
	g_pScrollCombatTextFrame->PushSCTInfo(sctInfo);
}

void NetCli_MoveChild::R_ObjGuildBuildingInfo( int GItemID , int BuildingParentDBID , int BuildingDBID , int BuildingID , const char* Point )
{
	g_pGuildHousesFrame->R_ObjGuildBuildingInfo( GItemID, BuildingParentDBID, BuildingDBID, BuildingID, Point );
}

void NetCli_MoveChild::R_ObjGuildFurnitureInfo( int GItemID , int FurnitureDBID )
{
	g_pGuildHousesFrame->R_ObjGuildFurnitureInfo( GItemID, FurnitureDBID );	
}

void NetCli_MoveChild::R_HorseColor		( int GItemID , int HorseColor[4] )
{
	CRoleSprite* sprite = g_pGameMain->FindSpriteByID( GItemID );
	if ( sprite )
	{
		sprite->SetDyeFXColors(HorseColor);
	}
}
void NetCli_MoveChild::MoveInfo			( int GItemID , float Gravity , float JumpRate )
{
	CRoleSprite* sprite = g_pGameMain->FindSpriteByID( GItemID );
	if ( sprite )
	{
		sprite->SetRisingimpulse(JumpRate);
		sprite->SetGravityAcceleration(Gravity);
	}
}

void NetCli_MoveChild::R_RoomID			( int RoomID )
{
	g_pGameMain->SetRoomID(RoomID);
}

void NetCli_MoveChild::R_PingLog			( int ID , int Time  )
{
	int ClietNetProcTime = timeGetTime() - s_lastNetProcTime;
	S_PingLog( ID , Time , ClietNetProcTime );
}
void NetCli_MoveChild::R_DeadZone			( ZoneTypeENUM Type , int DeadCountDown  )
{
	switch (Type)
	{
	case EM_ZoneType_NORMAL:
		g_pInterface->SendWorldEvent("WARNING_STOP");
		break;

	case EM_ZoneType_DEADZONE:
		if ( g_pInterface )
		{
			lua_State* L = g_pInterface->GetLuaState();
			if ( L )
			{
				lua_pushstring(L, g_ObjectData->GetString("SYS_WARNING_DEAD_ZONE"));
				lua_setglobal(L, "arg1");
				lua_pushnumber(L, DeadCountDown);
				lua_setglobal(L, "arg2");
				g_pInterface->SendWorldEvent("WARNING_START");
			}
		}
		break;
	case EM_ZoneType_DAMAGEZONE:
		break;
	case EM_ZoneType_IMMEDIATEDEATHZONE:
		break;
	}	
}

void NetCli_MoveChild::R_AttachObj		( int GItemID , int AttachItemID , int AttachType , const char* ItemPos , const char* AttachPos , int Pos )
{
	//g_pError->AddMessage(0, 0, "R_AttachObj GItemID(%d) AttachItemID(%d)", GItemID, AttachItemID);

	CRoleSprite* sprite = g_pGameMain->FindSpriteByID( GItemID );
	CRoleSprite* target = g_pGameMain->FindSpriteByID( AttachItemID );

	switch (AttachType)
	{
	// 載具
	case eMoveAttach_Implement:
		if ( sprite && target )
		{
			/*
			SpriteAttachPoint point = eAttach_Down;
			if ( AttachPos && AttachPos[0] ) {
				point = (SpriteAttachPoint)IBaseSprite::GetAttachPointEnum(AttachPos);
			}
			*/
			sprite->AttachImplement(target, eAttach_Down, ruFUSION_MIME_RIDE_IDLE, Pos + 1);
		}
		else
		{
			char temp[256];
			sprintf_s(temp, "%d", Pos + 1);
			g_pGameMain->AddAttachObj(GItemID, AttachItemID, AttachType, ItemPos, temp);
		}
		break;

	// 玩家連接
	case eMoveAttach_Mount:
		{
			char attachPoint[256];
			sprintf_s(attachPoint, 256, "p_mount%02d", (Pos+2));

			// 判斷物件是否已經存在
			if ( sprite && target && target->GetMountSprite() )
			{
				sprite->AttachMountSprite(target, attachPoint);
			}
			else
			{
				g_pGameMain->AddAttachObj(GItemID, AttachItemID, AttachType, ItemPos, attachPoint);
			}
		}
		break;

	case eMoveAttach_Point:
		if ( sprite && target )
		{
			sprite->SetAttachParent(target, IBaseSprite::GetAttachPointEnum(AttachPos), false);
		}
		else
		{
			g_pGameMain->AddAttachObj(GItemID, AttachItemID, AttachType, ItemPos, AttachPos);
		}
		break;

	case eMoveAttach_Point2:
		if ( sprite && target )
		{
			sprite->SetAttachParent(target, IBaseSprite::GetAttachPointEnum(AttachPos), true);
		}
		else
		{
			g_pGameMain->AddAttachObj(GItemID, AttachItemID, AttachType, ItemPos, AttachPos);
		}
		break;

	// NPC串接
	default:
		// 判斷物件是否已經存在
		if ( sprite && target && sprite->IsProxyLoadComplete() && target->IsProxyLoadComplete() )
		{
			sprite->FollowSprite(target, AttachPos, ItemPos);
		}
		else
		{
			g_pGameMain->AddAttachObj(GItemID, AttachItemID, AttachType, ItemPos, AttachPos);
		}
		break;
	}
}
void NetCli_MoveChild::R_DetachObj		( int GItemID )
{
	//g_pError->AddMessage(0, 0, "R_DetachObj GItemID(%d)", GItemID);

	g_pGameMain->DeleteAttachObj(GItemID);

	CRoleSprite* sprite = g_pGameMain->FindSpriteByID(GItemID);
	if ( sprite )
	{
		sprite->FollowSprite(NULL);
		sprite->DetachMountSprite();
		sprite->DetachImplement();
		sprite->SetAttachParent(NULL, 0);
	}
}

void NetCli_MoveChild::R_BattleGround_AddPlayer	( AddPlayerStruct& Info )
{
	CRoleSprite* sprite = g_pGameMain->FindBettleMemberByGItemID(Info.GItemID);
	if ( sprite == NULL )
	{
		CRoleSprite* sprite = new CRoleSprite(false);
		sprite->SetDBID(Info.DBID);
		sprite->SetIdentify(Info.GItemID);
		sprite->SetWorldID(Info.WorldID);
		sprite->SetVocation(Info.Job, Info.Job_Sub);
		sprite->SetName(Info.RoleName.Begin());
		sprite->SetRace(Info.Race);
		sprite->SetPos(CRuVector3(Info.X, Info.Y, Info.Z));

		int baseValue[2];
		g_pGameMain->TranslateBaseValue(sprite, Info.MaxMP, 100, 100, baseValue);
		sprite->SetMaxHP(Info.MaxHP);
		sprite->SetMaxMP(baseValue[0]);
		sprite->SetMaxSP(baseValue[1]);
		g_pGameMain->TranslateBaseValue(sprite, (int)Info.MP, (int)Info.SP, (int)Info.SP_Sub, baseValue);
		sprite->SetHP((int)Info.HP);
		sprite->SetMP(baseValue[0]);
		sprite->SetSP(baseValue[1]);

		if ( g_pGameMain->AddBattleMember(Info.GItemID, sprite) == false )
		{
			delete sprite;
			return;
		}

		// 通知有新角色加入
		NetCli_PartyChild::RefreshBattleMember(sprite->GetDBID());
	}
}
void NetCli_MoveChild::R_BattleGround_DelPlayer	( int GItemID )
{
	bool deleteReault = g_pGameMain->DeleteBettleMember(GItemID);
}
void NetCli_MoveChild::R_BattleGround_PlayerPos	( int GItemID , float X , float Y , float Z )
{
	CRoleSprite* sprite = g_pGameMain->FindBettleMemberByGItemID(GItemID);
	if ( sprite )
	{
		sprite->SetPos(CRuVector3(X, Y, Z));
	}
}

void NetCli_MoveChild::R_BattleGround_PlayerHPMP	( int GItemID , float HP , float MP , float SP , float SP_Sub , float MaxHP , float MaxMP )
{
	CRoleSprite* sprite = g_pGameMain->FindBettleMemberByGItemID(GItemID);
	if ( sprite == NULL )
		return;

	int baseValue[2];
	g_pGameMain->TranslateBaseValue(sprite, (int)MaxHP, 100, 100, baseValue);
	sprite->SetMaxHP((int)MaxHP);
	sprite->SetMaxMP(baseValue[0]);
	sprite->SetMaxSP(baseValue[1]);
	g_pGameMain->TranslateBaseValue(sprite, (int)MP, (int)SP, (int)SP_Sub, baseValue);
	sprite->SetHP((int)HP);
	sprite->SetMP(baseValue[0]);
	sprite->SetSP(baseValue[1]);
}

void NetCli_MoveChild::R_UnlockRolePosOK	( bool Result )
{
	g_pGameControl->R_UnlockRolePosOK(Result);
}
void NetCli_MoveChild::R_UnlockRolePosTime( int Time )
{
	lua_State* L = g_pInterface->GetLuaState();
	if ( L ) {
		lua_pushstring(L, "WEDGE");
		lua_setglobal(L, "arg1");
		lua_pushnumber(L, Time);
		lua_setglobal(L, "arg2");
		lua_pushstring(L, "");
		lua_setglobal(L, "arg3");
		g_pInterface->SendWorldEvent("FORCE_DIALOG_SHOW");
	}
}
void NetCli_MoveChild::R_AttachObjResult( bool Result , int Pos )
{
	int a = 0;
}
void NetCli_MoveChild::R_DetachObjResult( bool Result )
{
	int a = 0;
}
void NetCli_MoveChild::R_ColdownInfo( int GItemID , MagicColdownStruct& Coldown )
{
	// 清除所有冷卻時間
	s_magicColdown.clear();

	s_magicColdown[GItemID] = Coldown;
}
//////////////////////////////////////////////////////////////////////////
//花盆資料
void NetCli_MoveChild::R_FlowerPotInfo	( int GItemID , ItemFieldStruct& Item )
{
	g_pGuildHousesFrame->R_FlowerPotInfo( GItemID, Item );
}

#define DF_AddIconPlayerPos 20100226

void NetCli_MoveChild::R_PlayerPos		( int Size , PlayerBaseInfoStruct Info[] )
{
	g_pWorldMapFrame->ClearMapIcon( DF_AddIconPlayerPos );
	g_pWorldMapFrame->ClearMapIconWhenChangeZone( DF_AddIconPlayerPos );

	int WorldMapID = g_pGameMain->GetZoneID();

	//GuildMemberBaseInfoStruct* pGuildMemberInfo;

	int MyTeamColor = g_pGuildHouseWarStatus->GetMyTeamColor();

	vector<SGuildHouseWarPlayerInfo>& rPlayerScoresTable = g_pGuildHouseWarStatus->GetPlayerScoresTable();

	bool needUpdate = false;
	const char* szName;
	int num;
	bool bShow;
	bool bFind;
	SGuildHouseWarPlayerInfo* pPlayerInfo;
	for( int i = 0 ; i < Size ; ++i )
	{
		if( Info[i].DBID == g_pGameMain->GetZoneDBID() )
		{
			continue;
		}

		num = (int)rPlayerScoresTable.size();
		bShow = false;
		bFind = false;

		for( int n = 0 ; n < num ; ++n )
		{
			pPlayerInfo = &rPlayerScoresTable[n];

			if( NULL == pPlayerInfo )
			{
				continue;
			}

			if( pPlayerInfo->Status.PlayerDBID == Info[i].DBID )
			{
				szName = pPlayerInfo->Status.RoleName;
				bFind = true;
				if( pPlayerInfo->iTeamColor == MyTeamColor )
				{
					bShow = true;
				}
				break;
			}
		}

		if( bFind == false )
		{
			needUpdate = true;
		}

		if( false == bShow )
		{
			continue;
		}
		
		if( Info[i].IsAttack )
		{
			g_pWorldMapFrame->AddMapIcon( DF_AddIconPlayerPos, WorldMapID, (float)Info[i].X, (float)Info[i].Z, szName, EM_MapIconType_Fighting, 1.0f, 1.0f, 1.0f, false, Info[i].DBID );
		}
		else
		{
			g_pWorldMapFrame->AddMapIcon( DF_AddIconPlayerPos, WorldMapID, (float)Info[i].X, (float)Info[i].Z, szName, EM_MapIconType_Player, 0.2f, 1.0f, 0.2f, false, Info[i].DBID );
		}
		
	}

	if( needUpdate )
	{
		//NetCli_BG_GuildWar::SBL_PlayerInfoRequest();
	}


	/*
	for( int i = 0 ; i < Size ; ++i )
	{
		if( Info[i].DBID == g_pGameMain->GetDBID() )
		{
			continue;
		}

 		pGuildMemberInfo = g_pGuildFrame->GetMemberBaseInfo( Info[i].DBID );
		if( NULL == pGuildMemberInfo )
		{
			continue;
		}

		//const char* szName = NetCli_OtherChild::GetRoleName( DBID );
		const char* szName = pGuildMemberInfo->MemberName;

		if( Info[i].IsAttack )
		{
			r = 1.0f;
			g = 0.1f;
			b = 0.1f;
		}
		else
		{
			r = 0.2f;
			g = 1.0f;
			b = 0.2f;
		}
		g_pWorldMapFrame->AddMapIcon( DF_AddIconPlayerPos, WorldMapID, (float)Info[i].X, (float)Info[i].Z, szName, EM_MapIconType_Player, r, g, b, false, Info[i].DBID );
	}
	*/
}

void NetCli_MoveChild::RL_AllPlayerPos	( int PlayerCount , PlayerPosSimpleStruct tempData[] )
{
	CRoleSprite* player = g_pGameMain->GetPlayer();
	for ( int i = 0; i < PlayerCount; i++ )
	{
		PlayerPosSimpleStruct& info = tempData[i];
		CRoleSprite* sprite = g_pGameMain->FindSpriteByID(info.GUID);

		if ( sprite && player != sprite && (sprite->GetMoveTime() < 0.1f || sprite->IsTargetMove()) )
		{
			CRoleSprite* spriteControlProxy = sprite->GetProxyControlSprite();
			CRuVector3 tarPos((REAL)info.X, (REAL)info.Y, (REAL)info.Z);
			CRuVector3 nowPos = spriteControlProxy->GetPos();
			float distance = (nowPos - tarPos).Magnitude();	

			if ( distance > 8.0f && spriteControlProxy->IsImplement() == false )
			{
				float angle = info.Dir * PI / 180.0f;
				float speed = spriteControlProxy->GetMoveSpeed();
				CRuVector3 forward(cos(angle), 0.0f, -sin(angle));
				CRuVector3 newVec = tarPos - nowPos;
				
				// 改變面向
				spriteControlProxy->SetDirection(forward, true);

				// 移動到達時間
				float moveTime = newVec.Magnitude() / speed;
				CRuVector3 moveVector = newVec.Normalize() * speed;

				// 將目標向量轉換為角色Z軸向量
				CRuQuaternion quaternion;
				quaternion.RotateIntoVector(CRuVector3(0.0f, 0.0f, 1.0f), forward);
				quaternion.TransformVector(moveVector);		
				spriteControlProxy->SetMoveVector(moveVector, moveTime, true, true);
			}
		}
	}
}