#include "../../mainproc/GameMain.h"
#include "../../netwaker_member/NetWakerClientInc.h"
#include "InspectFrame.h"
#include "PartyFrame.h"
#include "UnitFrame.h"
#include "ItemUtil.h"
CUnitFrame*				g_pUnitFrame = NULL;

int LuaFunc_UnitName			(lua_State* L);
int LuaFunc_UnitGUID			(lua_State* L);
int LuaFunc_UnitSex				(lua_State* L);
int LuaFunc_UnitRace			(lua_State* L);
int LuaFunc_UnitHealth			(lua_State* L);
int LuaFunc_UnitMaxHealth		(lua_State* L);
int	LuaFunc_UnitChangeHealth	(lua_State* L);
int LuaFunc_UnitMana			(lua_State* L);
int LuaFunc_UnitMaxMana			(lua_State* L);
int LuaFunc_UnitSkill			(lua_State* L);
int LuaFunc_UnitMaxSkill		(lua_State* L);
int LuaFunc_UnitLevel			(lua_State* L);
int LuaFunc_UnitIsUnit			(lua_State* L);
int LuaFunc_UnitIsPlayer		(lua_State* L);
int LuaFunc_UnitIsImplement		(lua_State* L);
int LuaFunc_UnitIsNPC			(lua_State* L);
int LuaFunc_UnitInParty			(lua_State* L);
int LuaFunc_UnitInRaid			(lua_State* L);
int LuaFunc_UnitIsRaidAssistant	(lua_State* L);
int LuaFunc_UnitIsRaidLeader	(lua_State* L);
int LuaFunc_UnitIsRaidLeader	(lua_State* L);
int LuaFunc_UnitIsRaidMainTank	  (lua_State* L);
int LuaFunc_UnitIsRaidMainAttacker(lua_State* L);
int LuaFunc_UnitRaidState         (lua_State* L);
int LuaFunc_UnitRaidIndex         (lua_State* L); //Unit 在團隊裡的索引值

int LuaFunc_UnitIsMine			(lua_State* L);
int LuaFunc_UnitMineMsg			(lua_State* L);

int LuaFunc_UnitIsMasterLooter	(lua_State* L);

int	LuaFunc_UnitIsDeadOrGhost	(lua_State* L);
int	LuaFunc_UnitManaType		(lua_State* L);
int	LuaFunc_UnitSkillType		(lua_State* L);
int	LuaFunc_UnitBuff			(lua_State* L);
int	LuaFunc_UnitBuffLeftTime	(lua_State* L);
int	LuaFunc_UnitDebuff			(lua_State* L);
int	LuaFunc_UnitDebuffLeftTime	(lua_State* L);
int LuaFunc_UnitBuffInfo		(lua_State* L);
int	LuaFunc_UnitExists			(lua_State* L);
int LuaFunc_UnitClass			(lua_State* L);
int LuaFunc_UnitClassToken		(lua_State* L);
int LuaFunc_UnitDistance		(lua_State* L);
int LuaFunc_UnitCanAttack		(lua_State* L);
int LuaFunc_UnitTitle			(lua_State* L);
int LuaFunc_UnitWorld			(lua_State* L);
int LuaFunc_UnitMaster			(lua_State* L);
int LuaFunc_UnitQuestMsg		(lua_State* L);
int LuaFunc_UnitPKState			(lua_State* L);
int LuaFunc_UnitCastingTime		(lua_State* L);

//int LuaFunc_TargetIsMailBox		(lua_State* L);  信箱不能 target
int LuaFunc_TargetIsDecomposes	(lua_State* L);
int LuaFunc_TargetIsTransmits	(lua_State* L);

int LuaFunc_TargetIsAH	(lua_State* L);


int LuaFunc_TargetIsLottery		(lua_State* L);
// Inventory
int LuaFunc_GetInventorySlotInfo(lua_State* L);
int LuaFunc_GetInventoryItemCount(lua_State* L);
int LuaFunc_GetInventoryItemInvalid(lua_State* L);
int LuaFunc_GetInventoryItemTexture(lua_State* L);
int LuaFunc_GetInventoryItemQuality(lua_State* L);
int LuaFunc_GetInventoryItemDurable(lua_State* L);
int LuaFunc_GetInventoryItemType(lua_State* L);

int LuaFunc_TargetHateListRequest(lua_State* L);
int LuaFunc_GetTargetHateList(lua_State* L);

int LuaFunc_GetClassCount(lua_State* L);
int LuaFunc_GetClassID(lua_State* L);
int LuaFunc_GetClassInfoByID(lua_State* L);

// ----------------------------------------------------------------------------
// 取得物品詳細資料
ItemFieldStruct* GetUnitFieldStruct(const char* unit, int index)
{
	if ( unit == NULL )
		return NULL;

	if ( strcmp(unit, "player") == 0 )
		return CNetGlobal::RoleData()->GetEqItem(index);
	else if ( strcmp(unit, g_pInspectFrame->GetInspectUnit()) == 0 )
		return NetCli_OtherChild::GetInspectItemFieldStruct(index);

	return NULL;
}

// ----------------------------------------------------------------------------
// 索引編號(index)由1開始,0代表無
void CUnitFrame::UnitEvent( const char* eventStr, int type, int index )
{
	lua_State* L = g_pUnitFrame->GetLuaStateWorld();

	switch ( type )
	{
	case eUnit_Player:
		lua_pushstring(L, "player");		
		break;

	case eUnit_Target:
		lua_pushstring(L, "target");
		break;

	case eUnit_Party:
		{
			char unit[256];
			sprintf( unit, "party%d", index );
			lua_pushstring(L, unit);
		}
		break;

	case eUnit_Raid:
		{
			char unit[256];
			sprintf( unit, "raid%d", index );
			lua_pushstring(L, unit);
		}
		break;

	case eUnit_Pet:
		lua_pushstring(L, "pet");
		break;

	default:
		return;
	}

	lua_setglobal(L, "arg1");
	g_pUnitFrame->SendWorldEvent( eventStr );
}

// ----------------------------------------------------------------------------
BuffType CUnitFrame::GetBuffType(int id)
{
	GameObjDbStructEx* itemDB = g_ObjectData->GetObj(id);
	if ( itemDB && itemDB->MagicBase.MagicFunc == EM_MAGICFUN_Assist )
	{
		for ( int i = 0; i < 32; i++ )
		{
			if ( (itemDB->MagicBase.MagicGroupSet >> i) & 0x01 )
				return (BuffType)i;
		}
	}

	return eBuffType_None;
}

// ----------------------------------------------------------------------------
void CUnitFrame::Model_SetRoleSprite( CUiModel* model, CRoleSprite* sprite, bool showWeapon , bool showChangeShape )
{
	if ( model && sprite ) 
	{
		CUiTexture* texture = model->GetDefaultTexture();

		// 設定預設貼圖
		if ( texture ) {
			switch ( sprite->GetSex() )
			{
			case EM_Sex_Boy:
				texture->SetFile("Interface\\PartyFrame\\Portrait-Human-Male.tga");
				break;
			case EM_Sex_Girl:
				texture->SetFile("Interface\\PartyFrame\\Portrait-Human-Female.tga");
				break;
			}			
		}		

		CRuEntity* entity = sprite->GetRuEntity();
		CRuFusion_Mime* mime = sprite->GetRuFusionMime();
		if ( showChangeShape )   //是否顯示目前外型MODEL
		{
			if ( sprite->GetRuTempEntity() )
				entity = sprite->GetRuTempEntity();
			if ( sprite->GetRuTempFusionMime() )
				mime = sprite->GetRuTempFusionMime();
		}

		// 設定3D模型圖像
		model->CreateEntity(entity, mime);
		if ( model->GetRuEntity() ) {
			// 是否顯示武器
			if ( showWeapon ) {
				GameObjDbStructEx *itemDB, *imageDB;
				const char* mainHand = NULL;
				const char* mainPoint = NULL;
				const char* secondHand = NULL;
				const char* secondPoint = NULL;

				// 主手
				itemDB = g_ObjectData->GetObj(sprite->GetHandsObjectId(eRoleHands_MainHand));
				if (itemDB)
				{
					imageDB = g_ObjectData->GetObj(itemDB->ImageID);
					if ( imageDB )
					{
						mainHand = imageDB->Image.ACTWorld;			// ACT檔名
						mainPoint = ATTACH_POINT_RIGHT_HAND;		// 參考點位置
					}
				}

				// 副手
				itemDB = g_ObjectData->GetObj(sprite->GetHandsObjectId(eRoleHands_OffHand));
				if (itemDB)
				{
					imageDB = g_ObjectData->GetObj(itemDB->ImageID);
					if ( imageDB )
					{
						secondHand = imageDB->Image.ACTWorld;		// ACT檔名

						// 副手需要判斷類型,參考點會不同
						switch ( sprite->GetTakeItemType(itemDB->GUID) )
						{
						case eRoleTakeItem_Shield:
							secondPoint = ATTACH_POINT_HAND_SHIELD;
							break;

						default:
							secondPoint = ATTACH_POINT_LEFT_HAND;
							break;
						}
					}
				}

				model->AttachEntity(mainPoint, mainHand);
				model->AttachEntity(secondPoint, secondHand);

				// 裝飾品
				itemDB = g_ObjectData->GetObj(sprite->GetOrnamentID());
				if ( itemDB && (imageDB = g_ObjectData->GetObj(itemDB->ImageID)) != NULL )
				{
					CRuEntity* entity = NULL;
					switch (imageDB->Image.ImageType)
					{
						// 旗幟
					case EM_GameObjectImageType_Flag:
						{
						}
						break;

						// 翅膀
					case EM_GameObjectImageType_Wing:
						{
							model->AttachEntity(ATTACH_POINT_BACK_SHIELD, imageDB->Image.ACTWorld);
						}
						break;
					}
				}
				else
				{
					model->AttachEntity(ATTACH_POINT_BACK_SHIELD, NULL);
				}
			}
		}
	}
}

// ----------------------------------------------------------------------------
// 盆栽預覽用
// ----------------------------------------------------------------------------
extern void Func_Houses_CreatePlant( CRuEntity* pParent, ItemFieldStruct& rItem );


// ----------------------------------------------------------------------------
GameObjDbStructEx* _Model_SetItem_GetimageDB( GameObjDbStructEx* pItemDB, ItemFieldStruct* pItemField, bool UseImageObjectID )
{
	GameObjDbStructEx* imageDB = NULL;

	if( NULL == pItemDB ) return NULL;

	

	if( UseImageObjectID && pItemField )
	{
		GameObjDbStructEx* pimageItemDB = CNetGlobal::GetObj( pItemField->ImageObjectID );	
		if( pimageItemDB )
		{
			imageDB = CNetGlobal::GetObj( pimageItemDB->ImageID );	
		}
	}

	if( NULL == imageDB )
	{
		imageDB = CNetGlobal::GetObj( pItemDB->ImageID );
		if ( pItemDB->Item.ItemType == EM_ItemType_Pet)
		{
			GameObjDbStructEx* NPCOrgObjDB = g_ObjectData->GetObj( pItemField-> Pet.PetOrgID );//
			if( NPCOrgObjDB )
			{
				GameObjDbStructEx* imageDB1 = g_ObjectData->GetObj( NPCOrgObjDB->ImageID);//
				if( imageDB1 )
					imageDB=imageDB1;			
			}

		}
		
		
	}

	return imageDB;
}

// ----------------------------------------------------------------------------
EModelSetItemResult CUnitFrame::Model_SetItem( CUiModel* model, GameObjDbStructEx* pItemDB, ItemFieldStruct* pItemField, bool UseImageObjectID )
{
	if( !model || !pItemDB )
	{
		return MSIR_Fail;
	}

	if( pItemDB->Classification == EM_ObjectClass_Item )
	{
		if( pItemDB->Item.Class == EM_Item_Normal )
		{
			GameObjDbStructEx* imageDB = _Model_SetItem_GetimageDB( pItemDB, pItemField, UseImageObjectID );

			//打包物
			//ItemType
			if( pItemDB->Item.ItemType == EM_ItemType_Packet_DropList )
			{
				GameObjDbStructEx* pDropListPacketItemDB = g_ObjectData->GetObj( pItemDB->Item.DropListPacket );
				if( NULL == pDropListPacketItemDB )
				{
					return MSIR_Fail;
				}

				GameObjDbStructEx* pTreasureItemDB;
				EModelSetItemResult eResult;
				int i;
				for( i = 0 ; i < pDropListPacketItemDB->Treasure.ItemCount ; ++i )
				{
					pTreasureItemDB = g_ObjectData->GetObj( pDropListPacketItemDB->Treasure.Item[i].OrgID );
					if( pTreasureItemDB )
					{
						eResult = Model_SetItem( model, pTreasureItemDB, NULL, UseImageObjectID );

						//只有第一筆有問題 才回傳
						if( i == 0 && eResult != MSIR_Success )
						{
							return eResult;
						}
					}
				}

				return MSIR_Success;
			}

			if( pItemDB->Item.UseType == EM_ItemUseType_UnPacket )
			{
				if( NULL == pItemField )
				{
					return MSIR_Fail;
				}
				GameObjDbStructEx* pTreasureItemDB;
				EModelSetItemResult eResult;
				int itemID;
				int i;
				for( i = 0 ; i < 8 ; ++i )
				{
					itemID = pItemField->IntAbility[i] >> 8;
					pTreasureItemDB = g_ObjectData->GetObj( itemID );
					if( pTreasureItemDB )
					{
						eResult = Model_SetItem( model, pTreasureItemDB, NULL, UseImageObjectID );
						//只有第一筆有問題 才回傳
						if( i == 0 && eResult != MSIR_Success )
						{
							return eResult;
						}
					}
				}

				return MSIR_Success;
			}

			if ( imageDB == NULL || imageDB->Image.ACTWorld[0] == 0 )
			{
				return MSIR_Fail;
			}

			model->CreateEntity( imageDB->Image.ACTWorld );

			CRuEntity* pEntity = model->GetRuEntity();
			if( pEntity )
			{
				IRuPaperdoll* pPaperdoll = RuEntity_FindFirstPaperdoll( pEntity );
				if(  pPaperdoll )
				{
					model->SetPaperdollPart( PAPERDOLL_PART_NAME_HEAD     , "type01" );
					model->SetPaperdollPart( PAPERDOLL_PART_NAME_HELMET   , NULL );
					model->SetPaperdollPart( PAPERDOLL_PART_NAME_HAND     , "body000-001" );
					model->SetPaperdollPart( PAPERDOLL_PART_NAME_FOOT     , "body000-001" );
					model->SetPaperdollPart( PAPERDOLL_PART_NAME_TORSO    , "body000-001" );
					model->SetPaperdollPart( PAPERDOLL_PART_NAME_LEG      , "body000-001" );
					model->SetPaperdollPart( PAPERDOLL_PART_NAME_BACK     , NULL );
					model->SetPaperdollPart( PAPERDOLL_PART_NAME_BELT     , NULL );
					model->SetPaperdollPart( PAPERDOLL_PART_NAME_SHOULDER , NULL );
				}
			}

			// 坐騎物件,其他物件也可能會使用染色層.
			if ( pItemDB->Item.ItemType == EM_ItemType_SummonHorse )
			{
				int colors[4] = { 0, 0, 0, 0 };
				if ( pItemField )
					memcpy(colors, pItemField->HorseColor, sizeof(colors));
				model->SetDyeFXColors(colors);
			}

			//寵物
			if ( pItemDB->Item.ItemType == EM_ItemType_Pet)
			{
				
				
			}

			//是不是盆栽
			if( pItemDB->Item.Plant.Pot.Mode != 0 && pItemField )
			{
				Func_Houses_CreatePlant( pEntity, *pItemField );
			}

			return MSIR_Success_Not_Player;
		}
		else if( pItemDB->Item.Class == EM_Item_Card )
		{
			GameObjDbStructEx* CardOrgObjDB = g_ObjectData->GetObj( pItemDB->Item.CardOrgObjID );
			if( CardOrgObjDB == NULL )
				return MSIR_Fail;

			//GameObjDbStructEx* imageDB = _Model_SetItem_GetimageDB( pItemDB, pItemField, UseImageObjectID );
			GameObjDbStructEx* imageDB = CNetGlobal::GetObj(CardOrgObjDB->ImageID);
			if ( imageDB == NULL )
				return MSIR_Fail;

			model->CreateEntity( imageDB->Image.ACTWorld );

			model->SetPaperdollPart( PAPERDOLL_PART_NAME_HAIR     , imageDB->Image.PaperdollHair.Name    );
			model->SetPaperdollPart( PAPERDOLL_PART_NAME_HEAD     , imageDB->Image.PaperdollFace.Name    );
			model->SetPaperdollPart( PAPERDOLL_PART_NAME_HELMET   , imageDB->Image.PaperdollHead.Name    );
			model->SetPaperdollPart( PAPERDOLL_PART_NAME_HAND     , imageDB->Image.PaperdollGloves.Name  );
			model->SetPaperdollPart( PAPERDOLL_PART_NAME_FOOT     , imageDB->Image.PaperdollShoes.Name   );
			model->SetPaperdollPart( PAPERDOLL_PART_NAME_TORSO    , imageDB->Image.PaperdollClothes.Name );
			model->SetPaperdollPart( PAPERDOLL_PART_NAME_LEG      , imageDB->Image.PaperdollPants.Name   );
			model->SetPaperdollPart( PAPERDOLL_PART_NAME_BACK     , imageDB->Image.PaperdollBack.Name    );
			model->SetPaperdollPart( PAPERDOLL_PART_NAME_BELT     , imageDB->Image.PaperdollBelt.Name    );
			model->SetPaperdollPart( PAPERDOLL_PART_NAME_SHOULDER , imageDB->Image.PaperdollShouder.Name );

			model->SetPaperdollColor( PAPERDOLL_PART_NAME_HAIR		, imageDB->Image.PaperdollHair.MainColor.Color   , imageDB->Image.PaperdollHair.OffColor.Color    );
			model->SetPaperdollColor( PAPERDOLL_PART_NAME_HEAD		, imageDB->Image.PaperdollFace.MainColor.Color   , imageDB->Image.PaperdollFace.OffColor.Color    );
			model->SetPaperdollColor( PAPERDOLL_PART_NAME_HELMET	, imageDB->Image.PaperdollHead.MainColor.Color   , imageDB->Image.PaperdollHead.OffColor.Color    );
			model->SetPaperdollColor( PAPERDOLL_PART_NAME_HAND		, imageDB->Image.PaperdollGloves.MainColor.Color , imageDB->Image.PaperdollGloves.OffColor.Color  );
			model->SetPaperdollColor( PAPERDOLL_PART_NAME_FOOT		, imageDB->Image.PaperdollShoes.MainColor.Color  , imageDB->Image.PaperdollShoes.OffColor.Color   );
			model->SetPaperdollColor( PAPERDOLL_PART_NAME_TORSO		, imageDB->Image.PaperdollClothes.MainColor.Color, imageDB->Image.PaperdollClothes.OffColor.Color );
			model->SetPaperdollColor( PAPERDOLL_PART_NAME_LEG		, imageDB->Image.PaperdollPants.MainColor.Color  , imageDB->Image.PaperdollPants.OffColor.Color   );
			model->SetPaperdollColor( PAPERDOLL_PART_NAME_BACK		, imageDB->Image.PaperdollBack.MainColor.Color   , imageDB->Image.PaperdollBack.OffColor.Color    );
			model->SetPaperdollColor( PAPERDOLL_PART_NAME_BELT		, imageDB->Image.PaperdollBelt.MainColor.Color   , imageDB->Image.PaperdollBelt.OffColor.Color    );
			model->SetPaperdollColor( PAPERDOLL_PART_NAME_SHOULDER	, imageDB->Image.PaperdollShouder.MainColor.Color, imageDB->Image.PaperdollShouder.OffColor.Color );
			return MSIR_Success_Not_Player;
		}
		else if( pItemDB->Item.Class == EM_Item_Weapon )
		{
			GameObjDbStructEx* imageDB = _Model_SetItem_GetimageDB( pItemDB, pItemField, UseImageObjectID );

			if ( imageDB == NULL )
			{
				return MSIR_Fail;
			}

			CRuEntity* pEntity = model->GetRuEntity();
			if( NULL == pEntity )
			{
				return MSIR_NoPaperdoll; //讓LUA重載
			}
			IRuPaperdoll* pPaperdoll = RuEntity_FindFirstPaperdoll( pEntity );
			if( NULL == pPaperdoll )
			{

				return MSIR_NoPaperdoll; //讓LUA重載
			}

			int color[2];

			//color[0] = imageDB->Image.ArmorMainColor.Color;
			//color[1] = imageDB->Image.ArmorOffColor.Color;

			if( pItemField && pItemField->MainColor != 0 )
				color[0] = pItemField->MainColor;
			else
 				color[0] = imageDB->Image.ArmorMainColor.Color;	


			if( pItemField && pItemField->OffColor != 0 )
				color[1] = pItemField->OffColor; 
			else
				color[1] = imageDB->Image.ArmorOffColor.Color;



			switch ( pItemDB->Item.WeaponPos )
			{
			case EM_WeaponPos_MainHand:			// 主手
			case EM_WeaponPos_OneHand:			// 雙手都可以裝備
			case EM_WeaponPos_TwoHand:			// 兩手

				model->AttachEntity( ATTACH_POINT_RIGHT_HAND, imageDB->Image.ACTWorld );
				model->SetPaperdollColor( ATTACH_POINT_RIGHT_HAND  , color[0], color[1] );
				break;

			case EM_WeaponPos_Bow:				// 弓
				model->AttachEntity( ATTACH_POINT_RIGHT_HAND, imageDB->Image.ACTWorld );
				model->SetPaperdollColor( ATTACH_POINT_RIGHT_HAND  , color[0], color[1] );
				break;

			case EM_WeaponPos_SecondHand:		// 副手
				model->AttachEntity( ATTACH_POINT_LEFT_HAND, imageDB->Image.ACTWorld );
				model->SetPaperdollColor( ATTACH_POINT_LEFT_HAND  , color[0], color[1] );
				break;
			default:
				return MSIR_Fail;
			}

			return MSIR_Success;
		}
		else if( pItemDB->Item.Class == EM_Item_Armor )
		{
			GameObjDbStructEx* imageDB = _Model_SetItem_GetimageDB( pItemDB, pItemField, UseImageObjectID );

			if ( imageDB == NULL )
			{
				return MSIR_Fail;
			}

			CRuEntity* pEntity = model->GetRuEntity();
			if( NULL == pEntity )
			{
				return MSIR_NoPaperdoll; //讓LUA重載
			}
			IRuPaperdoll* pPaperdoll = RuEntity_FindFirstPaperdoll( pEntity );
			if( NULL == pPaperdoll )
			{
				return MSIR_NoPaperdoll; //讓LUA重載
			}

			RoleDataEx*	pRole = CNetGlobal::RoleData();

			SexFlagStruct& LimitSex1 = pItemDB->Limit.Sex;
			SexFlagStruct& LimitSex2 = imageDB->Image.LimitSex;

			bool boy = true;
			bool girl = true;

			if( LimitSex1._Sex != 0 && LimitSex1._Sex != -1 )
			{
				if( LimitSex1.Boy == false )
				{
					boy = false;
				}

				if( LimitSex1.Girl == false )
				{
					girl = false;
				}
			}


			if( LimitSex2._Sex != 0 && LimitSex2._Sex != -1 )
			{
				if( LimitSex2.Boy == false )
				{
					boy = false;
				}

				if( LimitSex2.Girl == false )
				{
					girl = false;
				}
			}

			if( boy == false && girl == false )
			{
				return MSIR_Fail;
			}

			string filename;
			//CRuEntity* pEntity = model->GetRuEntity();
			if( pEntity )
			{
				filename = model->GetRuEntity()->GetResourceName(); 
			}

			if( filename.length() == 0 )
			{
				return MSIR_Fail;
			}

			Sex_ENUM _eSex = ObjectDataClass::GetModelSex( filename.c_str() );

			if( _eSex == EM_Sex_Boy && boy  == false )
			{
				return MSIR_SexError_LimitGirl;
			}

			if( _eSex == EM_Sex_Girl && girl  == false )
			{
				return MSIR_SexError_LimitBoy;
			}

  			int color[2];
			if( pItemField && pItemField->MainColor != 0 )
				color[0] = pItemField->MainColor;
			else
				color[0] = imageDB->Image.ArmorMainColor.Color;	


			if( pItemField && pItemField->OffColor != 0 )
				color[1] = pItemField->OffColor; 
			else
				color[1] = imageDB->Image.ArmorOffColor.Color;

			switch (pItemDB->Item.ArmorPos)
			{
			case EM_ArmorPos_Head       : model->SetPaperdollPart( PAPERDOLL_PART_NAME_HELMET   , imageDB->Image.ACTWorld ); model->SetPaperdollColor( PAPERDOLL_PART_NAME_HELMET  , color[0], color[1] ); break;
			case EM_ArmorPos_Gloves     : model->SetPaperdollPart( PAPERDOLL_PART_NAME_HAND     , imageDB->Image.ACTWorld ); model->SetPaperdollColor( PAPERDOLL_PART_NAME_HAND    , color[0], color[1] ); break;
			case EM_ArmorPos_Shoes      : model->SetPaperdollPart( PAPERDOLL_PART_NAME_FOOT     , imageDB->Image.ACTWorld ); model->SetPaperdollColor( PAPERDOLL_PART_NAME_FOOT    , color[0], color[1] ); break;
			case EM_ArmorPos_Clothes    : model->SetPaperdollPart( PAPERDOLL_PART_NAME_TORSO    , imageDB->Image.ACTWorld ); model->SetPaperdollColor( PAPERDOLL_PART_NAME_TORSO   , color[0], color[1] ); break;
			case EM_ArmorPos_Pants      : model->SetPaperdollPart( PAPERDOLL_PART_NAME_LEG      , imageDB->Image.ACTWorld ); model->SetPaperdollColor( PAPERDOLL_PART_NAME_LEG     , color[0], color[1] ); break;
			case EM_ArmorPos_Back       : model->SetPaperdollPart( PAPERDOLL_PART_NAME_BACK     , imageDB->Image.ACTWorld ); model->SetPaperdollColor( PAPERDOLL_PART_NAME_BACK    , color[0], color[1] ); break;
			case EM_ArmorPos_Belt       : model->SetPaperdollPart( PAPERDOLL_PART_NAME_BELT     , imageDB->Image.ACTWorld ); model->SetPaperdollColor( PAPERDOLL_PART_NAME_BELT    , color[0], color[1] ); break;
			case EM_ArmorPos_Shoulder   : model->SetPaperdollPart( PAPERDOLL_PART_NAME_SHOULDER , imageDB->Image.ACTWorld ); model->SetPaperdollColor( PAPERDOLL_PART_NAME_SHOULDER, color[0], color[1] ); break;
			case EM_ArmorPos_SecondHand :
				{
					if( pItemDB->Item.ArmorType == EM_Armor_Shield )
					{
						model->AttachEntity( ATTACH_POINT_LEFT_HAND, NULL );
						model->AttachEntity( ATTACH_POINT_HAND_SHIELD, imageDB->Image.ACTWorld );
						//model->SetPaperdollColor( ATTACH_POINT_HAND_SHIELD  , color[0], color[1] );
					}
					else if( pItemDB->Item.ArmorType == EM_Armor_Implement )
					{
						model->AttachEntity( ATTACH_POINT_LEFT_HAND, imageDB->Image.ACTWorld );
						model->AttachEntity( ATTACH_POINT_HAND_SHIELD, NULL );
						//model->SetPaperdollColor( ATTACH_POINT_HAND_SHIELD  , color[0], color[1] );
					}
				}
				break;
			case EM_ArmorPos_MagicTool  : 
				{
					model->AttachEntity( ATTACH_POINT_LEFT_HAND, imageDB->Image.ACTWorld );
					model->AttachEntity( ATTACH_POINT_HAND_SHIELD, NULL );
				}  
				break;
			case EM_ArmorPos_Ornament  :
				{
					if( imageDB->Image.ImageType == EM_GameObjectImageType_Wing )	// 翅膀
					{
						model->AttachEntity( ATTACH_POINT_BACK_SHIELD, imageDB->Image.ACTWorld );
					}
				}
				break;
			default:
				return MSIR_Fail;
			}

			return MSIR_Success;
		}
		else
		{

			return MSIR_Fail;
		}
	}


	if( pItemDB->Classification == EM_ObjectClass_Recipe )
	{
		GameObjDbStructEx* recipeItemDB = g_ObjectData->GetObj( pItemDB->Recipe.ItemAll[0].OrgObjID );
		if ( recipeItemDB )
		{
			return Model_SetItem( model, recipeItemDB, NULL, UseImageObjectID );
		}

		return MSIR_Fail;
	}


	return MSIR_Fail;
}


// ----------------------------------------------------------------------------
CUnitFrame::CUnitFrame(CInterface* object)
	: CInterfaceBase(object)
{
	g_pUnitFrame = this;
}

// ----------------------------------------------------------------------------
CUnitFrame::~CUnitFrame()
{
	g_pUnitFrame = NULL;
}

// ----------------------------------------------------------------------------
void CUnitFrame::RegisterFunc()
{
	lua_State* L = GetLuaStateWorld();
	if (L)
	{
		lua_register( L, "UnitName",				LuaFunc_UnitName );
		lua_register( L, "UnitGUID",				LuaFunc_UnitGUID );
 		lua_register( L, "UnitSex",					LuaFunc_UnitSex );
		lua_register( L, "UnitRace",				LuaFunc_UnitRace );
		lua_register( L, "UnitHealth",				LuaFunc_UnitHealth );
		lua_register( L, "UnitMaxHealth",			LuaFunc_UnitMaxHealth );
		lua_register( L, "UnitChangeHealth",		LuaFunc_UnitChangeHealth );
		lua_register( L, "UnitMana",				LuaFunc_UnitMana );
		lua_register( L, "UnitMaxMana",				LuaFunc_UnitMaxMana );
		lua_register( L, "UnitSkill",				LuaFunc_UnitSkill );
		lua_register( L, "UnitMaxSkill",			LuaFunc_UnitMaxSkill );
		lua_register( L, "UnitLevel",				LuaFunc_UnitLevel );
		lua_register( L, "UnitIsUnit",				LuaFunc_UnitIsUnit );
		lua_register( L, "UnitIsPlayer",			LuaFunc_UnitIsPlayer );
		lua_register( L, "UnitIsImplement",			LuaFunc_UnitIsImplement );
		lua_register( L, "UnitIsNPC",				LuaFunc_UnitIsNPC );
		lua_register( L, "UnitInParty",				LuaFunc_UnitInParty );
   		lua_register( L, "UnitInRaid",				LuaFunc_UnitInRaid );
		lua_register( L, "UnitIsRaidAssistant",		LuaFunc_UnitIsRaidAssistant );
		lua_register( L, "UnitIsRaidLeader",		LuaFunc_UnitIsRaidLeader );
		lua_register( L, "UnitIsRaidMainTank",		LuaFunc_UnitIsRaidMainTank );
		lua_register( L, "UnitIsRaidMainAttacker",	LuaFunc_UnitIsRaidMainAttacker );  
		lua_register( L, "UnitRaidState",			LuaFunc_UnitRaidState );
   		lua_register( L, "UnitRaidIndex",			LuaFunc_UnitRaidIndex );

		lua_register( L, "UnitIsMasterLooter",		LuaFunc_UnitIsMasterLooter );
  
   		lua_register( L, "UnitIsDeadOrGhost",		LuaFunc_UnitIsDeadOrGhost );
		lua_register( L, "UnitManaType",			LuaFunc_UnitManaType );
		lua_register( L, "UnitSkillType",			LuaFunc_UnitSkillType );
		lua_register( L, "UnitBuff",				LuaFunc_UnitBuff );
		lua_register( L, "UnitBuffLeftTime",		LuaFunc_UnitBuffLeftTime );
		lua_register( L, "UnitDebuff",				LuaFunc_UnitDebuff );
		lua_register( L, "UnitDebuffLeftTime",		LuaFunc_UnitDebuffLeftTime );
		lua_register( L, "UnitBuffInfo",			LuaFunc_UnitBuffInfo );
		lua_register( L, "UnitExists",				LuaFunc_UnitExists );
		lua_register( L, "UnitClass",				LuaFunc_UnitClass );
		lua_register( L, "UnitClassToken",			LuaFunc_UnitClassToken );
		lua_register( L, "UnitDistance",			LuaFunc_UnitDistance );
		lua_register( L, "UnitCanAttack",			LuaFunc_UnitCanAttack );
		lua_register( L, "UnitTitle",				LuaFunc_UnitTitle );		
		lua_register( L, "UnitWorld",				LuaFunc_UnitWorld );		
		lua_register( L, "UnitMaster",				LuaFunc_UnitMaster );
 		lua_register( L, "UnitQuestMsg",			LuaFunc_UnitQuestMsg );
		lua_register( L, "UnitPKState",				LuaFunc_UnitPKState );
		lua_register( L, "UnitCastingTime",			LuaFunc_UnitCastingTime );

		lua_register( L, "UnitIsMine",				LuaFunc_UnitIsMine );
		lua_register( L, "UnitMineMsg",				LuaFunc_UnitMineMsg );

	//	lua_register( L, "TargetIsMailBox",			LuaFunc_TargetIsMailBox);		
		lua_register( L, "TargetIsTransmits",		LuaFunc_TargetIsTransmits);	
		lua_register( L, "TargetIsDecomposes",		LuaFunc_TargetIsDecomposes);	
		lua_register( L, "TargetIsLottery",			LuaFunc_TargetIsLottery);	
		lua_register( L, "TargetIsAH",				LuaFunc_TargetIsAH);	


		lua_register(L, "GetInventorySlotInfo",		LuaFunc_GetInventorySlotInfo);
		lua_register(L, "GetInventoryItemCount",	LuaFunc_GetInventoryItemCount);
		lua_register(L, "GetInventoryItemInvalid",	LuaFunc_GetInventoryItemInvalid);
		lua_register(L, "GetInventoryItemTexture",	LuaFunc_GetInventoryItemTexture);
		lua_register(L, "GetInventoryItemQuality",	LuaFunc_GetInventoryItemQuality);
		lua_register(L, "GetInventoryItemDurable",	LuaFunc_GetInventoryItemDurable);
		lua_register(L, "GetInventoryItemType",		LuaFunc_GetInventoryItemType);
 		lua_register(L, "TargetHateListRequest",	LuaFunc_TargetHateListRequest);
   		lua_register(L, "GetTargetHateList",		LuaFunc_GetTargetHateList);
											
   		lua_register(L, "GetClassCount",			LuaFunc_GetClassCount);
   		lua_register(L, "GetClassID",				LuaFunc_GetClassID);
		lua_register(L, "GetClassInfoByID",			LuaFunc_GetClassInfoByID);

	}
}

// ----------------------------------------------------------------------------
int LuaFunc_UnitName (lua_State* L)
{
	CRoleSprite* sprite = NULL;
	const char* name = NULL;
	if (lua_isstring(L, 1))
	{
		sprite = g_pGameMain->GetUnitSprite(lua_tostring(L, 1));
		if (sprite)
			name = sprite->GetName();
	}
	lua_pushstring(L, name);
	return 1;
}

int LuaFunc_UnitGUID(lua_State* L)
{
	if (lua_isstring(L, 1))
	{
		CRoleSprite* sprite = g_pGameMain->GetUnitSprite(lua_tostring(L, 1));
		if (sprite)
		{
			lua_pushnumber( L, sprite->GetIdentify() & 0xFFFF );
			return 1;
		}
	}
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_UnitSex	(lua_State* L)
{
	int sex = 0;
	if (lua_isstring(L, 1))
	{
		CRoleSprite* sprite = g_pGameMain->GetUnitSprite(lua_tostring(L, 1));
		if (sprite)
			sex = sprite->GetSex();
	}
	lua_pushnumber(L, sex);
	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_UnitRace(lua_State* L)
{
	int race = EM_Race_None;
	if (lua_isstring(L, 1))
	{
		CRoleSprite* sprite = g_pGameMain->GetUnitSprite(lua_tostring(L, 1));
		if (sprite)
			race = sprite->GetRace();
	}

	if ( race == EM_Race_None )
		return 0;

	char temp[512];
	sprintf_s(temp, 512, "SYS_RACE_%d", race);
	lua_pushstring(L, g_ObjectData->GetString(temp));
	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_UnitHealth (lua_State* L)
{
	int health = 0;
	if (lua_isstring(L, 1))
	{
		CRoleSprite* sprite = g_pGameMain->GetUnitSprite(lua_tostring(L, 1));
		if (sprite) 
		{
			health = sprite->GetHP();
			switch ( sprite->GetObjectType() )
			{
			case eSpriteObject_Player:
				break;

			default:
				// 寵物顯示實際血量
				if ( sprite->GetMasterSprite() == NULL )
				{
					int maxValue = sprite->GetMaxHP();
					if ( maxValue > 0 )
					{
						if ( health == 0 )
							health = 0;
						else {
							health = max((int)((double)health / (double)maxValue * 100), 1);
						}
					}
				}
				break;
			}
		}
	}
	lua_pushnumber(L, health);
	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_UnitMaxHealth (lua_State* L)
{
	int maxHealth = 0;
	if (lua_isstring(L, 1))
	{
		CRoleSprite* sprite = g_pGameMain->GetUnitSprite(lua_tostring(L, 1));
		if (sprite)
		{
			maxHealth = sprite->GetMaxHP();
			switch ( sprite->GetObjectType() )
			{
			case eSpriteObject_Player:
				break;

			default:
				// 寵物顯示實際血量
				if ( sprite->GetMasterSprite() == NULL )
				{
					maxHealth = 100;
				}
				break;
			}
		}
	}
	lua_pushnumber(L, maxHealth);
	return 1;
}

// ----------------------------------------------------------------------------
int	LuaFunc_UnitChangeHealth (lua_State* L)
{
	int changeHealth = 0;
	if (lua_isstring(L, 1))
	{
		CRoleSprite* sprite = g_pGameMain->GetUnitSprite(lua_tostring(L, 1));
		if (sprite)
			changeHealth = sprite->GetHP() - sprite->GetLastHP();
	}
	lua_pushnumber(L, changeHealth);
	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_UnitMana (lua_State* L)
{
	int mana = 0;
	if (lua_isstring(L, 1))
	{
		CRoleSprite* sprite = g_pGameMain->GetUnitSprite(lua_tostring(L, 1));
		if (sprite)
		{
			mana = sprite->GetMP();

			switch ( sprite->GetObjectType() )
			{
			case eSpriteObject_Player:
				break;

			default:
				if ( sprite->GetMasterSprite() == NULL )
				{
					if( sprite->GetMaxMP() > 0 )
						mana = (int)((double)mana * 100 / (double)sprite->GetMaxMP());
					else
						mana = 0;
				}
				break;				
			}
		}
	}
	lua_pushnumber(L, mana);
	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_UnitMaxMana (lua_State* L)
{
	int maxMana = 0;
	if (lua_isstring(L, 1))
	{
		CRoleSprite* sprite = g_pGameMain->GetUnitSprite(lua_tostring(L, 1));
		if (sprite)
		{
			maxMana = sprite->GetMaxMP();
			switch ( sprite->GetObjectType() )
			{
			case eSpriteObject_Player:
				break;

			default:
				if ( sprite->GetMasterSprite() == NULL )
				{
					if( maxMana > 0 )
						maxMana = 100;
					else
						maxMana = 0;
				}
				break;
			}
		}
	}
	lua_pushnumber(L, maxMana);
	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_UnitSkill (lua_State* L)
{
	int skill = 0;
	if (lua_isstring(L, 1))
	{
		CRoleSprite* sprite = g_pGameMain->GetUnitSprite(lua_tostring(L, 1));
		if (sprite)
		{
			skill = sprite->GetSP();
			switch ( sprite->GetObjectType() )
			{
			case eSpriteObject_Player:
				break;

			default:
				if ( sprite->GetMasterSprite() == NULL )
				{
					if( sprite->GetMaxSP() > 0 )
						skill = (int)((double)skill * 100 / (double)sprite->GetMaxSP());
					else
						skill = 0;
				}
				break;
			}
		}
	}
	lua_pushnumber(L, skill);
	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_UnitMaxSkill (lua_State* L)
{
	int maxSkill = 0;
	if (lua_isstring(L, 1))
	{
		CRoleSprite* sprite = g_pGameMain->GetUnitSprite(lua_tostring(L, 1));
		if (sprite)
		{
			maxSkill = sprite->GetMaxSP();
			switch ( sprite->GetObjectType() )
			{
			case eSpriteObject_Player:
				break;

			default:
				if ( sprite->GetMasterSprite() == NULL )
				{
					if( maxSkill > 0 )
						maxSkill = 100;
					else
						maxSkill = 0;
				}
				break;
			}
		}
	}
	lua_pushnumber(L, maxSkill);
	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_UnitLevel (lua_State* L)
{
	CRoleSprite* sprite = g_pGameMain->GetUnitSprite(luaL_checkstring(L, 1));
	if ( sprite )
	{
		lua_pushnumber(L, sprite->GetLevel());
		lua_pushnumber(L, sprite->GetLevelSub());
		return 2;
	}
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_UnitIsUnit (lua_State* L)
{
	CRoleSprite* src = g_pGameMain->GetUnitSprite(luaL_checkstring(L, 1));
	CRoleSprite* dest = g_pGameMain->GetUnitSprite(luaL_checkstring(L, 2));
	if (src && dest)
		lua_pushboolean(L, src == dest);
	else
		lua_pushnil(L);
	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_UnitIsMine (lua_State* L)
{
	CRoleSprite* pSprite = g_pGameMain->GetUnitSprite(luaL_checkstring(L, 1));
	if ( pSprite )
	{
		GameObjDbStructEx*	pObj	= NULL;	
		int					iObjID	= pSprite->GetObjectId();

		pObj = g_ObjectData->GetObj( iObjID );

		if( pObj != NULL )
		{
			if( pObj->Classification == EM_ObjectClass_Mine )
			{
				lua_pushboolean(L, true );
			}
			else
			{
				lua_pushboolean(L, false );
			}		
		}
		else
			lua_pushnil(L);
	}
	else
		lua_pushnil(L);

	return 1;
}
// ----------------------------------------------------------------------------
int LuaFunc_UnitIsPlayer (lua_State* L)
{
	CRoleSprite* sprite = g_pGameMain->GetUnitSprite(luaL_checkstring(L, 1));
	if ( sprite && sprite->GetObjectType() == eSpriteObject_Player)
		lua_pushboolean(L, true);
	else
		lua_pushnil(L);
	return 1;
}
// ----------------------------------------------------------------------------
// 是否為載具物件
int LuaFunc_UnitIsImplement (lua_State* L)
{
	CRoleSprite* sprite = g_pGameMain->GetUnitSprite(luaL_checkstring(L, 1));
	if ( sprite && sprite->IsImplement() )
	{
		lua_pushboolean(L, true);
		lua_pushnumber(L, sprite->GetImplementCount());			// 目前乘客數
		lua_pushnumber(L, sprite->GetImplementMaxNum());		// 最大乘客數
	}
	else
	{
		lua_pushnil(L);
		lua_pushnumber(L, 0);
		lua_pushnumber(L, 0);
	}
	return 3;
}
// ----------------------------------------------------------------------------
int LuaFunc_UnitIsNPC (lua_State* L)
{
	CRoleSprite* sprite = g_pGameMain->GetUnitSprite(luaL_checkstring(L, 1));
	if ( sprite )
	{
		switch ( sprite->GetObjectType() )
		{
		case eSpriteObject_NPC:
		case eSpriteObject_QuestNPC:
			if ( sprite->GetMasterSprite() == NULL )
			{
				lua_pushboolean(L, true);
				return 1;
			}
			break;
		}
	}

	lua_pushnil(L);
	return 1;
}
// ----------------------------------------------------------------------------
int LuaFunc_UnitInParty (lua_State* L)
{
	if( lua_isnil( L, 1 ) || lua_isnone( L, 1 ) )
	{
		return 0;
	}

	CRoleSprite* sprite = g_pGameMain->GetUnitSprite(luaL_checkstring(L, 1));
	if ( sprite )
	{
		if ( g_pGameMain->IsBattleGroundZone() )
		{
			lua_pushboolean(L, NetCli_PartyChild::InBattleParty(sprite->GetDBID()));
		}
		else
		{
			lua_pushboolean(L, NetCli_PartyChild::InParty(sprite->GetName()));
		}
	}
	else
	{
		lua_pushnil(L);
	}

	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_UnitInRaid			(lua_State* L)
{
	if( lua_isnil( L, 1 ) || lua_isnone( L, 1 ) )
	{
		return 0;
	}

	CRoleSprite* sprite = g_pGameMain->GetUnitSprite(luaL_checkstring(L, 1));
	if ( sprite )
	{
		if ( g_pGameMain->IsBattleGroundZone() )
		{
			lua_pushboolean( L, NetCli_PartyChild::InBattleGroup( sprite->GetDBID() ) );
		}
		else
		{
			lua_pushboolean( L, NetCli_PartyChild::InGroup( sprite->GetName() ) );
		}
	}
	else
	{
		lua_pushnil(L);
	}

	return 1;
}



// ----------------------------------------------------------------------------
int LuaFunc_UnitIsRaidAssistant	(lua_State* L)
{
	if( lua_isnil( L, 1 ) || lua_isnone( L, 1 ) )
	{
		return 0;
	}

	CRoleSprite* sprite = g_pGameMain->GetUnitSprite( luaL_checkstring(L, 1) );
	if ( !sprite )
	{
		return 0;
	}

	if ( g_pGameMain->IsBattleGroundZone() )
		return 0;

	PartyMemberInfo* member = NetCli_PartyChild::GetGroupMemberByDBID( sprite->GetDBID() );
  
	if ( member )
	{
		lua_pushboolean( L, member->assistant );				// 助理
		return 1;
	}	
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_UnitIsRaidLeader	(lua_State* L)
{
	if( lua_isnil( L, 1 ) || lua_isnone( L, 1 ) )
	{
		return 0;
	}

	CRoleSprite* sprite = g_pGameMain->GetUnitSprite( luaL_checkstring(L, 1) );
	if ( !sprite )
	{
		return 0;
	}

	if ( g_pGameMain->IsBattleGroundZone() )
	{
		lua_pushboolean(L, NetCli_PartyChild::s_BattleLeaderDBID == sprite->GetDBID());
		return 1;
	}
	else
	{
		const char* szLeaderName = NetCli_PartyChild::GetPartyLeaderName();

		if( NULL == szLeaderName )
		{
			return 0;
		}

		if( strcmp( szLeaderName , sprite->GetName() ) == 0 )
		{
			lua_pushboolean( L, true );
			return 1;
		}
	}	
 
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_UnitIsRaidMainTank(lua_State* L)
{
	if( lua_isnil( L, 1 ) || lua_isnone( L, 1 ) )
	{
		return 0;
	}

	CRoleSprite* sprite = g_pGameMain->GetUnitSprite( luaL_checkstring(L, 1) );
	if ( !sprite )
	{
		return 0;
	}

	if ( g_pGameMain->IsBattleGroundZone() )
		return 0;

	PartyMemberInfo* member = NetCli_PartyChild::GetGroupMemberByDBID( sprite->GetDBID() );

	if ( member )
	{
		lua_pushboolean( L, member->tank );
		return 1;
	}	
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_UnitIsRaidMainAttacker(lua_State* L)
{
	if( lua_isnil( L, 1 ) || lua_isnone( L, 1 ) )
	{
		return 0;
	}

	CRoleSprite* sprite = g_pGameMain->GetUnitSprite( luaL_checkstring(L, 1) );
	if ( !sprite )
	{
		return 0;
	}

	if ( g_pGameMain->IsBattleGroundZone() )
		return 0;

	PartyMemberInfo* member = NetCli_PartyChild::GetGroupMemberByDBID( sprite->GetDBID() );

	if ( member )
	{
		lua_pushboolean( L, member->attack );
		return 1;
	}	
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_UnitRaidState(lua_State* L)
{
	if( lua_isnil( L, 1 ) || lua_isnone( L, 1 ) )
	{
		return 0;
	}

	bool isLeader    = false;
	bool isAssistant = false;
	bool isLooter    = false;
	bool isTank      = false;
	bool isAttacker  = false;


	CRoleSprite* sprite = g_pGameMain->GetUnitSprite( luaL_checkstring(L, 1) );
	if ( !sprite )
	{
		return 0;
	}

	if ( g_pGameMain->IsBattleGroundZone() )
	{
		if ( NetCli_PartyChild::s_BattleLeaderDBID == sprite->GetDBID() )
		{
			isLeader = true;
		}
	}
	else
	{
		const char* szLeaderName = NetCli_PartyChild::GetPartyLeaderName();

		if( NULL == szLeaderName )
		{
			return 0;
		}

		if( strcmp( szLeaderName , sprite->GetName() ) == 0 )
		{
			isLeader = true;
		}

		if( NetCli_PartyChild::GetPartyInfo().ShareName == sprite->GetName() )
		{
			isLooter = true;
		}

		PartyMemberInfo* member = NetCli_PartyChild::GetGroupMemberByDBID( sprite->GetDBID() );
		if ( NULL == member )
		{
			return 0;
		}
		isAssistant = member->assistant;
		isTank = member->tank;
		isAttacker = member->attack;
	}

	lua_pushboolean( L, isLeader    );
	lua_pushboolean( L, isAssistant );
  	lua_pushboolean( L, isLooter    );
	lua_pushboolean( L, isTank      );
	lua_pushboolean( L, isAttacker  );

	return 5;
}

// ----------------------------------------------------------------------------
int LuaFunc_UnitRaidIndex(lua_State* L)
{
	if( lua_isnil( L, 1 ) || lua_isnone( L, 1 ) )
	{
		return 0;
	}

	CRoleSprite* sprite = g_pGameMain->GetUnitSprite( luaL_checkstring(L, 1) );
	if ( !sprite )
	{
		return 0;
	}

	if ( g_pGameMain->IsBattleGroundZone() )
	{
		BattleGroundPartyMember* member = NetCli_PartyChild::GetBattleGroupMemberByDBID( sprite->GetDBID() );
		if ( member )
		{
			lua_pushnumber(L, member->partyNo);
			return 1;
		}
	}
	else
	{
		PartyMemberInfo* member = NetCli_PartyChild::GetGroupMemberByDBID( sprite->GetDBID() );

		if ( member )
		{
			lua_pushnumber( L, member->raidNo + 1 );
			return 1;
		}
	}	
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_UnitIsMasterLooter(lua_State* L)
{
	if( lua_isnil( L, 1 ) || lua_isnone( L, 1 ) )
	{
		return 0;
	}

	CRoleSprite* sprite = g_pGameMain->GetUnitSprite( luaL_checkstring(L, 1) );
	if ( !sprite )
	{
		return 0;
	}

	if ( NetCli_PartyChild::GetPartyInfo().ItemShareType == EM_PartyItemShare_Assign && NetCli_PartyChild::GetPartyInfo().ShareName == sprite->GetName() )
	{
		lua_pushboolean( L, true );
		return 1;
	}

	return 0;
}


// ----------------------------------------------------------------------------
int	LuaFunc_UnitIsDeadOrGhost(lua_State* L)
{
	if( lua_isnil( L, 1 ) || lua_isnone( L, 1 ) )
	{
		return 0;
	}

	CRoleSprite* sprite = g_pGameMain->GetUnitSprite(luaL_checkstring(L, 1));
	if ( sprite )
		lua_pushboolean(L, sprite->GetDeadState());
	else
		lua_pushnil(L);
	return 1;
}

// ----------------------------------------------------------------------------
int	LuaFunc_UnitManaType(lua_State* L)
{
	CRoleSprite* sprite = g_pGameMain->GetUnitSprite(luaL_checkstring(L, 1));
	int type = (sprite) ? sprite->GetManaType() : 1;	
	lua_pushnumber(L, type);
	return 1;
}

// ----------------------------------------------------------------------------
int	LuaFunc_UnitSkillType(lua_State* L)
{
	CRoleSprite* sprite = g_pGameMain->GetUnitSprite(luaL_checkstring(L, 1));
	int type = (sprite) ? sprite->GetSkillType() : 1;
	lua_pushnumber(L, type);
	return 1;
}

// ----------------------------------------------------------------------------
int	LuaFunc_UnitBuff(lua_State* L)
{
	CRoleSprite* sprite = g_pGameMain->GetUnitSprite(luaL_checkstring(L, 1));
	if ( sprite )
	{
		int index = sprite->GetBuffHelpfulIndex(luaL_checkint(L, 2) - 1);
		if ( index != -1 )
		{
			RoleBuffInfo* buff = sprite->GetBuffInfo(index);
			if ( buff )
			{
				GameObjDbStructEx* itemDB = g_ObjectData->GetObj(buff->id);				
				if ( itemDB )
				{
					BuffType type = CUnitFrame::GetBuffType(buff->id);
					int count = 0;
					if ( itemDB->MagicBase.Setting.ShowPowerLv )
						count = buff->magicLv + 1;
					else
						count = itemDB->MagicBase.BuffCount;

					lua_pushstring(L, itemDB->GetName());
					lua_pushstring(L, ItemUTIL::GetItemIcon(itemDB, true));
					lua_pushnumber(L, count);
					lua_pushnumber(L, buff->id);

					if ( type == eBuffType_None )
						lua_pushnil(L);
					else
						lua_pushnumber(L, type);
					return 5;
				}
			}
		}

	}
	return 0;
}

// ----------------------------------------------------------------------------
int	LuaFunc_UnitBuffLeftTime(lua_State* L)
{
	CRoleSprite* sprite = g_pGameMain->GetUnitSprite(luaL_checkstring(L, 1));
	float time = -1.0f;
	if ( sprite )
	{
		int index = sprite->GetBuffHelpfulIndex(luaL_checkint(L, 2) - 1);
		if ( index != -1 )
		{
			RoleBuffInfo* buff = sprite->GetBuffInfo(index);
			if ( buff )
			{
				time = buff->time;
				if ( time > 0.0f )
				{
					GameObjDbStruct* itemDB = g_ObjectData->GetObj(buff->id);
					if ( itemDB && !itemDB->MagicBase.Setting.ShowBuffTime )
						time = -1.0f;
				}
			}
		}
	}	
	if ( time < 0.0f )
		lua_pushnil(L);
	else
		lua_pushnumber(L, time);
	return 1;
}

// ----------------------------------------------------------------------------
int	LuaFunc_UnitDebuff(lua_State* L)
{
	CRoleSprite* sprite = g_pGameMain->GetUnitSprite(luaL_checkstring(L, 1));
	if ( sprite )
	{
		int index = sprite->GetBuffHarmfulIndex(luaL_checkint(L, 2) - 1);
		if ( index != -1 )
		{
			RoleBuffInfo* buff = sprite->GetBuffInfo(index);
			if ( buff )
			{
				GameObjDbStructEx* itemDB = g_ObjectData->GetObj(buff->id);
				if ( itemDB )
				{
					BuffType type = CUnitFrame::GetBuffType(buff->id);
					int count = 0;
					if ( itemDB->MagicBase.Setting.ShowPowerLv )
						count = buff->magicLv + 1;
					else
						count = itemDB->MagicBase.BuffCount;

					lua_pushstring(L, itemDB->GetName());
					lua_pushstring(L, ItemUTIL::GetItemIcon(itemDB, true));
					lua_pushnumber(L, count);
					lua_pushnumber(L, buff->id);
					if ( type == eBuffType_None )
						lua_pushnil(L);
					else
						lua_pushnumber(L, type);
					return 5;
				}
			}
		}

	}
	return 0;
}

// ----------------------------------------------------------------------------
int	LuaFunc_UnitDebuffLeftTime(lua_State* L)
{
	CRoleSprite* sprite = g_pGameMain->GetUnitSprite(luaL_checkstring(L, 1));
	float time = -1.0f;
	if ( sprite )
	{
		int index = sprite->GetBuffHarmfulIndex(luaL_checkint(L, 2) - 1, (lua_toboolean(L, 3) != 0));
		if ( index != -1 )
		{
			RoleBuffInfo* buff = sprite->GetBuffInfo(index);
			if ( buff )
			{
				time = buff->time;
				if ( time > 0.0f )
				{
					GameObjDbStruct* itemDB = g_ObjectData->GetObj(buff->id);
					if ( itemDB && !itemDB->MagicBase.Setting.ShowBuffTime )
						time = -1.0f;
				}
			}
		}
	}	
	if ( time < 0.0f )
		lua_pushnil(L);
	else
		lua_pushnumber(L, time);
	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_UnitBuffInfo(lua_State* L)
{
	CRoleSprite* sprite = g_pGameMain->GetUnitSprite(luaL_checkstring(L, 1));
	if ( sprite )
	{
		RoleBuffInfo* buff = sprite->GetBuffInfo(luaL_checkint(L, 2) - 1);
		if ( buff )
		{
			GameObjDbStructEx* itemDB = g_ObjectData->GetObj(buff->id);
			if ( itemDB )
			{
				BuffType type = CUnitFrame::GetBuffType(buff->id);
				int count = 0;
				if ( itemDB->MagicBase.Setting.ShowPowerLv )
					count = buff->magicLv + 1;
				else
					count = itemDB->MagicBase.BuffCount;

				lua_pushstring(L, itemDB->GetName());
				lua_pushstring(L, ItemUTIL::GetItemIcon(itemDB, true));
				lua_pushnumber(L, count);
				lua_pushnumber(L, buff->id);
				if ( type == eBuffType_None )
					lua_pushnil(L);
				else
					lua_pushnumber(L, type);
				return 5;
			}
		}
	}
	return 0;
}

// ----------------------------------------------------------------------------
int	LuaFunc_UnitExists(lua_State* L)
{
	CRoleSprite* sprite = g_pGameMain->GetUnitSprite(luaL_checkstring(L, 1));
	if ( sprite )
		lua_pushboolean(L, true);
	else
		lua_pushnil(L);
	return 1;
}


// ----------------------------------------------------------------------------------
// 參數1 MODEL物件
// 參數2 Unit (player, target, party1...)
// 參數3 boolean 是否顯示武器(true 顯示)
// 參數4 boolean 是否顯示目前外型MODEL(true 變身改變後模樣)
int Model_SetUnit(lua_State* L)
{
	CUiModel* model = dynamic_cast<CUiModel*>(CUiLuaLayout::CheckLayoutTable(L, 1));
	if (model == NULL)
		lua_error(L);
	 
	CRoleSprite* sprite = g_pGameMain->GetUnitSprite(luaL_checkstring(L, 2));

	if( sprite )
	{
		bool showWeapon = (lua_toboolean(L, 3) != 0);
		bool showChangeShape = (lua_toboolean(L, 4) != 0);
		CUnitFrame::Model_SetRoleSprite( model, sprite, showWeapon, showChangeShape );
	}

	return 0;
}

// ----------------------------------------------------------------------------------
int LuaFunc_UnitClass(lua_State* L)
{
	CRoleSprite* sprite = g_pGameMain->GetUnitSprite(luaL_checkstring(L, 1));
	if ( sprite )
	{
		Voc_ENUM classType[2];
		classType[0] = sprite->GetVocation();
		classType[1] = sprite->GetVocationSub();

		if ( sprite->GetObjectType() != eSpriteObject_Player )
		{
			if ( classType[0] == EM_Vocation_GameMaster ) classType[0] = EM_Vocation_Knight;
			if ( classType[1] == EM_Vocation_GameMaster ) classType[1] = EM_Vocation_Knight;
		}		
		lua_pushstring(L, g_ObjectData->GetClassNameByID(classType[0]));
		lua_pushstring(L, g_ObjectData->GetClassNameByID(classType[1]));
		return 2;
	}
	return 0;
}

// ----------------------------------------------------------------------------------
int LuaFunc_UnitClassToken(lua_State* L)
{
	CRoleSprite* sprite = g_pGameMain->GetUnitSprite(luaL_checkstring(L, 1));
	if ( sprite )
	{
		Voc_ENUM classType[2];
		classType[0] = sprite->GetVocation();
		classType[1] = sprite->GetVocationSub();

		if ( sprite->GetObjectType() != eSpriteObject_Player )
		{
			if ( classType[0] == EM_Vocation_GameMaster ) classType[0] = EM_Vocation_Knight;
			if ( classType[1] == EM_Vocation_GameMaster ) classType[1] = EM_Vocation_Knight;
		}
		lua_pushstring(L, g_ObjectData->GetClassTokenByID(classType[0]));
		lua_pushstring(L, g_ObjectData->GetClassTokenByID(classType[1]));
		return 2;
	}
	return 0;
}

// ----------------------------------------------------------------------------------
int LuaFunc_UnitDistance(lua_State* L)
{
	CRoleSprite* sprite = g_pGameMain->GetUnitSprite(luaL_checkstring(L, 1));
	float distance = 0.0f;

	// 需要有權限
	if ( sprite && CNetGlobal::RoleData()->PlayerBaseData->ManageLV > EM_Management_Nornal )
	{
		CRoleSprite* player = g_pGameMain->GetPlayer();
		if ( player != sprite )
			distance = (player->GetPos() - sprite->GetPos()).Magnitude();
	}
	lua_pushnumber(L, distance);
	return 1;
}

// ----------------------------------------------------------------------------------
int LuaFunc_UnitCanAttack(lua_State* L)
{
	CRoleSprite* sprite1 = g_pGameMain->GetUnitSprite(luaL_checkstring(L, 1));
	CRoleSprite* sprite2 = g_pGameMain->GetUnitSprite(luaL_checkstring(L, 2));
	CRoleSprite* player = g_pGameMain->GetPlayer();

	if ( sprite1 == player ) {
		if ( sprite2 ) {
			lua_pushboolean(L, sprite2->GetCanAttackState());
		} else {
			lua_pushboolean(L, false);
		}
	}
	else if ( sprite2 == player ) {
		if ( sprite1 ) {
			lua_pushboolean(L, sprite1->GetEnemyState());
		} else {
			lua_pushboolean(L, false);
		}
	}
	else
		lua_pushnil(L);

	return 1;
}
// ----------------------------------------------------------------------------------
int LuaFunc_UnitTitle			(lua_State* L)
{
	RoleDataEx*	pRole		= CNetGlobal::RoleData();

	//CRole
	//CRoleSprite*		pSprite	= g_pGameMain->GetPlayer();
	int iTitleID = pRole->BaseData.TitleID;

	if( iTitleID == 0 )
	{
		lua_pushstring( L, "" );
	}
	else
		if( iTitleID == 1 )
		{
			lua_pushstring( L, pRole->PlayerBaseData->TitleStr.Begin() );
		}
		else
		{
			GameObjDbStructEx* pTitleObj = g_ObjectData->GetObj( iTitleID );
			if( pTitleObj )
				lua_pushstring( L, pTitleObj->GetName() );
			else
				lua_pushstring( L, "Unknow" );	
		}

	return 1;
}
// ----------------------------------------------------------------------------
int LuaFunc_GetInventorySlotInfo(lua_State* L)
{
	if ( lua_isstring(L, 1) )
	{
		const char* soltName = lua_tostring(L, 1);			

		if ( stricmp(soltName, "HeadSlot") == 0 )
		{
			lua_pushnumber(L, EM_EQWearPos_Head);
			lua_pushstring(L, "interface\\CharacterFrame\\Equipment-HeadSlot");
		}
		else if ( stricmp(soltName, "HandsSlot") == 0 )
		{
			lua_pushnumber(L, EM_EQWearPos_Gloves);
			lua_pushstring(L, "interface\\CharacterFrame\\Equipment-HandsSlot");
		}
		else if ( stricmp(soltName, "FeetSlot") == 0 )
		{
			lua_pushnumber(L, EM_EQWearPos_Shoes);
			lua_pushstring(L, "interface\\CharacterFrame\\Equipment-FeetSlot");
		}
		else if ( stricmp(soltName, "ChestSlot") == 0 )
		{
			lua_pushnumber(L, EM_EQWearPos_Clothes);
			lua_pushstring(L, "interface\\CharacterFrame\\Equipment-ChestSlot");
		}
		else if ( stricmp(soltName, "LegsSlot") == 0 )
		{
			lua_pushnumber(L, EM_EQWearPos_Pants);
			lua_pushstring(L, "interface\\CharacterFrame\\Equipment-LegsSlot");
		}
		else if ( stricmp(soltName, "BackSlot") == 0 )
		{
			lua_pushnumber(L, EM_EQWearPos_Back);
			lua_pushstring(L, "interface\\CharacterFrame\\Equipment-BackSlot");
		}
		else if ( stricmp(soltName, "WaistSlot") == 0 )
		{
			lua_pushnumber(L, EM_EQWearPos_Belt);
			lua_pushstring(L, "interface\\CharacterFrame\\Equipment-WaistSlot");
		}
		else if ( stricmp(soltName, "ShoulderSlot") == 0 )
		{
			lua_pushnumber(L, EM_EQWearPos_Shoulder);
			lua_pushstring(L, "interface\\CharacterFrame\\Equipment-ShoulderSlot");
		}
		else if ( stricmp(soltName, "NecklaceSlot") == 0 )
		{
			lua_pushnumber(L, EM_EQWearPos_Necklace);
			lua_pushstring(L, "interface\\CharacterFrame\\Equipment-NecklaceSlot");
		}
		else if ( stricmp(soltName, "AmmoSlot") == 0 )
		{
			lua_pushnumber(L, EM_EQWearPos_Ammo);
			lua_pushstring(L, "interface\\CharacterFrame\\Equipment-AmmoSlot");
		}
		else if ( stricmp(soltName, "RangedSlot") == 0 )
		{
			lua_pushnumber(L, EM_EQWearPos_Bow);
			lua_pushstring(L, "interface\\CharacterFrame\\Equipment-RangedSlot");
		}
		else if ( stricmp(soltName, "Ring0Slot") == 0 )
		{
			lua_pushnumber(L, EM_EQWearPos_Ring1);
			lua_pushstring(L, "interface\\CharacterFrame\\Equipment-RingsSlot");
		}
		else if ( stricmp(soltName, "Ring1Slot") == 0 )
		{
			lua_pushnumber(L, EM_EQWearPos_Ring2);
			lua_pushstring(L, "interface\\CharacterFrame\\Equipment-RingsSlot");
		}
		else if ( stricmp(soltName, "Earring0Slot") == 0 )
		{
			lua_pushnumber(L, EM_EQWearPos_Earring1);
			lua_pushstring(L, "interface\\CharacterFrame\\Equipment-EarringsSlot");
		}
		else if ( stricmp(soltName, "Earring1Slot") == 0 )
		{
			lua_pushnumber(L, EM_EQWearPos_Earring2);
			lua_pushstring(L, "interface\\CharacterFrame\\Equipment-EarringsSlot");
		}
		else if ( stricmp(soltName, "MainHandSlot") == 0 )
		{
			lua_pushnumber(L, EM_EQWearPos_MainHand);
			lua_pushstring(L, "interface\\CharacterFrame\\Equipment-MainHandSlot");
		}
		else if ( stricmp(soltName, "SecondaryHandSlot") == 0 )
		{
			lua_pushnumber(L, EM_EQWearPos_SecondHand);
			lua_pushstring(L, "interface\\CharacterFrame\\Equipment-SecondaryHandSlot");
		}
		else if ( stricmp(soltName, "UtensilSlot") == 0 )
		{
			lua_pushnumber(L, EM_EQWearPos_Manufactory);
			lua_pushstring(L, "interface\\CharacterFrame\\Equipment-UtensilSlot");
		}
		else if ( stricmp(soltName, "Talisman0Slot") == 0 )
		{
			lua_pushnumber(L, EM_EQWearPos_MagicTool1);
			lua_pushstring(L, "interface\\CharacterFrame\\Equipment-TalismanSlot");
		}
		else if ( stricmp(soltName, "Talisman1Slot") == 0 )
		{
			lua_pushnumber(L, EM_EQWearPos_MagicTool2);
			lua_pushstring(L, "interface\\CharacterFrame\\Equipment-TalismanSlot");
		}
		else if ( stricmp(soltName, "Talisman2Slot") == 0 )
		{
			lua_pushnumber(L, EM_EQWearPos_MagicTool3);
			lua_pushstring(L, "interface\\CharacterFrame\\Equipment-TalismanSlot");
		}
		else if ( stricmp(soltName, "AdornmentSlot") == 0 )
		{
			lua_pushnumber(L, EM_EQWearPos_Ornament);
			lua_pushstring(L, "interface\\CharacterFrame\\Equipment-AdornmentSlot");
		}
		else
		{
			lua_pushnil(L);
			lua_pushstring(L, "interface\\CharacterFrame\\Equipment-EmptySlot");
		}

		return 2;
	}
	return 0;
}
// ----------------------------------------------------------------------------
int LuaFunc_GetInventoryItemCount(lua_State* L)
{
	ItemFieldStruct* item = GetUnitFieldStruct(luaL_checkstring(L, 1), luaL_checkint(L, 2));
	if ( item && !item->IsEmpty() )
	{
		lua_pushnumber(L, item->GetCount());
		return 1;
	}
	return 0;
}
// ----------------------------------------------------------------------------
int LuaFunc_GetInventoryItemInvalid(lua_State* L)
{
	const char* unit = luaL_checkstring(L, 1);
	int index = luaL_checkint(L, 2);

	ItemFieldStruct* item = GetUnitFieldStruct(unit, index);
	if ( item && !item->IsEmpty() )
	{
		GameObjDbStruct* itemDB = CNetGlobal::GetObj(item->OrgObjID);
		if (itemDB)
		{
			lua_pushboolean(L, !CNetGlobal::RoleData()->CanWearObject(itemDB, index, item->Mode.IgnoreLimitLv));
			return 1;
		}
	}
	return 0;
}
// ----------------------------------------------------------------------------
int LuaFunc_GetInventoryItemTexture(lua_State* L)
{
	ItemFieldStruct* item = GetUnitFieldStruct(luaL_checkstring(L, 1), luaL_checkint(L, 2));
	if ( item && !item->IsEmpty() )
	{
		GameObjDbStruct* itemDB = CNetGlobal::GetObj(item->OrgObjID);
		if (itemDB)
		{
			lua_pushstring(L,ItemUTIL::GetItemIcon(item) );//itemDB->ImageObj->ACTField
			return 1;
		}
	}
	return 0;
}
// ----------------------------------------------------------------------------
int LuaFunc_GetInventoryItemQuality(lua_State* L)
{
	int quality = 0;

	ItemFieldStruct* item = GetUnitFieldStruct(luaL_checkstring(L, 1), luaL_checkint(L, 2));
	if ( item && !item->IsEmpty() )
	{
		GameObjDbStruct* itemDB = CNetGlobal::GetObj(item->OrgObjID);
		if (itemDB)
			quality = g_ObjectData->GetItemQuality(itemDB, item);
	}
	lua_pushnumber(L, quality);
	return 1;
}
// ----------------------------------------------------------------------------
int LuaFunc_GetInventoryItemDurable(lua_State* L)
{
	int durable = 0;
	int durableMax = 0;
	int durablef = 0;
	int durableMaxf = 0;
	const char *name = NULL;
	ItemFieldStruct* item = GetUnitFieldStruct(luaL_checkstring(L, 1), luaL_checkint(L, 2));
	if ( item && !item->IsEmpty() )
	{
		GameObjDbStructEx* itemDB = CNetGlobal::GetObj(item->OrgObjID);
		if (itemDB)
		{
			name=itemDB->GetName();
			durable = item->Durable / 100;
			durablef = item->Durable;
			durableMax = itemDB->Item.Durable * item->Quality / 100;
			durableMaxf = itemDB->Item.Durable * item->Quality;
		}
	}
	
	lua_pushnumber(L, durable);
	lua_pushnumber(L, durableMax);
	lua_pushstring(L, name);

	lua_pushnumber(L, durablef);
	lua_pushnumber(L, durableMaxf);
	return 5;
}
// ----------------------------------------------------------------------------
int LuaFunc_UnitWorld (lua_State* L)
{
	RoleDataEx*	pRole		= CNetGlobal::RoleData();

	//CRole
	//CRoleSprite*		pSprite	= g_pGameMain->GetPlayer();
	int WorldID = ( pRole->BaseData.ZoneID / _DEF_ZONE_BASE_COUNT_ ) + 1;
	char szWorldStr[256];
	//CGameObj
	//GameObjDbStruct* pZoneObj = g_ObjectData->GetObj( iZoneID + 750000 );

	sprintf( szWorldStr, g_ObjectData->GetString("SOCIAL_S_WORLD"), WorldID );

	lua_pushstring( L, szWorldStr );

	return 1;
}

// ----------------------------------------------------------------------------
// 取得寵物主人
int LuaFunc_UnitMaster(lua_State* L)
{
	CRoleSprite* sprite = g_pGameMain->GetUnitSprite(luaL_checkstring(L, 1));
	if ( sprite && !sprite->GetHideMasterName() ) {
		CRoleSprite* masterSprite = g_pGameMain->FindSpriteByID(sprite->GetMasterID());
		if ( masterSprite ) {
			lua_pushstring(L, masterSprite->GetName());
			return 1;
		}
	}
	return 0;
}

// ----------------------------------------------------------------------------
extern bool GetNPCAllQuestTooltipText( string& str, GameObjDbStructEx* ObjDb );
int LuaFunc_UnitQuestMsg(lua_State* L)
{
	CRoleSprite* sprite = g_pGameMain->GetUnitSprite(luaL_checkstring(L, 1));
	if( !sprite ) 
	{
		return 0;
	}

  	GameObjDbStructEx* ObjDb = g_ObjectData->GetObj( sprite->GetObjectId() );
	if( !ObjDb ) 
	{
		return 0;
	}

	string str;
	if( !GetNPCAllQuestTooltipText( str, ObjDb ) )
	{
		return 0;
	}
  
	lua_pushstring( L, str.c_str() );
	return 1;
}

int LuaFunc_UnitMineMsg(lua_State* L)
{
	CRoleSprite* sprite = g_pGameMain->GetUnitSprite(luaL_checkstring(L, 1));
	if( !sprite ) 
	{
		return 0;
	}

	GameObjDbStructEx* ObjDb = g_ObjectData->GetObj( sprite->GetObjectId() );
	if( !ObjDb ) 
	{
		return 0;
	}

	string	str;
	int		iMineType = 0;
	int		iSkillNow = 0;
	int		iReqSkill = 0;
	
	CNetCli_Gather_Child::GetMinetooltips ( sprite->GetObjectId(), str, iMineType, iSkillNow, iReqSkill );

	lua_pushstring( L, str.c_str() );
	lua_pushnumber(L, iMineType );
	lua_pushnumber(L, iSkillNow );
	lua_pushnumber(L, iReqSkill );

	return 4;
}

int LuaFunc_UnitPKState(lua_State* L)
{
	CRoleSprite* sprite = g_pGameMain->GetUnitSprite(luaL_checkstring(L, 1));
	if( !sprite )
		return 0;

	lua_pushboolean(L, g_pGameMain->GetSpritePVPState(sprite));
	return 1;
}

int LuaFunc_UnitCastingTime(lua_State* L)
{
	CRoleSprite* sprite = g_pGameMain->GetUnitSprite(luaL_checkstring(L, 1));
	if( sprite )
	{
		const char* name = NULL;
		GameObjDbStructEx* item = g_ObjectData->GetObj(sprite->GetSpellID());
		if ( item )
			name = item->GetName();
		
		lua_pushstring(L, name);
		lua_pushnumber(L, sprite->GetCastingMaxTime());
		lua_pushnumber(L, sprite->GetCastingCurrentTime());
		return 3;
	}
	return 0;
}

int LuaFunc_TargetIsDecomposes		(lua_State* L)
{
	bool ret=false;

	{
		CRoleSprite* sprite = g_pGameMain->GetUnitSprite("target");
		if (sprite)
		{
			int id=sprite->GetIdentify();

			if (id==110536)
				ret=true;

		}
	}

	lua_pushboolean(L,ret);
	return 1;
}
int LuaFunc_TargetIsTransmits		(lua_State* L)
{
	bool ret=false;
	
	{
		CRoleSprite* sprite = g_pGameMain->GetUnitSprite("target");
		if (sprite)
		{
			int id=sprite->GetIdentify();
			if (id==110755||id==110728||id==110727 ||id==110726)
				ret=true;

		}
	}

	lua_pushboolean(L,ret);
	return 1;
}
int LuaFunc_TargetIsLottery		(lua_State* L)
{
	bool ret=false;

	{
		CRoleSprite* sprite = g_pGameMain->GetUnitSprite("target");
		if (sprite)
		{
			int id=sprite->GetIdentify();
			if (id==110701 ||id==110700)
				ret=true;

		}
	}

	lua_pushboolean(L,ret);
	return 1;

}
/*
// 信箱不能target
int LuaFunc_TargetIsMailBox		(lua_State* L)
{
	bool ret=false;

	{
		CRoleSprite* sprite = g_pGameMain->GetUnitSprite("target");
		if (sprite)
		{
			int id=sprite->GetDBID();
			 if (id==110538 ||id==110694)
				 ret=true;

		}
	}

	lua_pushboolean(L,ret);
	return 1;
}

*/
int LuaFunc_TargetIsAH	(lua_State* L)
{

	bool ret=false;

	{
		CRoleSprite* sprite = g_pGameMain->GetUnitSprite("target");
		if (sprite)
		{
			int id=sprite->GetIdentify();
			if (id==110754 ||id==110580)
				ret=true;

		}
	}

	lua_pushboolean(L,ret);
	return 1;
}
int LuaFunc_GetInventoryItemType(lua_State* L)
{

	int type=-1;
	
	ItemFieldStruct* item = GetUnitFieldStruct(luaL_checkstring(L, 1), luaL_checkint(L, 2));
	if ( item && !item->IsEmpty() )
	{
		GameObjDbStruct* itemDB = CNetGlobal::GetObj(item->OrgObjID);
		if (itemDB)
		{
			if (itemDB->Classification==EM_ObjectClass_Item)
			{
				if (itemDB->Item.Class==EM_Item_Weapon)
					type=1;
				else if (itemDB->Item.Class==EM_Item_Armor)
					type=0;
			}
		
		
		}
	}

	
	lua_pushnumber(L,type);
	return 1;
}

int LuaFunc_TargetHateListRequest(lua_State* L)
{
	CRoleSprite* pSprite = g_pGameMain->GetPlayerTarget();

	if( pSprite && pSprite->GetCanAttackState() && pSprite->GetHP() > 0 )
	{
		static int s_lastTime = 0;
		int time = timeGetTime();
		if( time - s_lastTime > 2000 )
		{
			int NpcGUID = pSprite->GetIdentify(); //pSprite->GetMasterID() GetWorldGUID() GetDBID();
			NetCli_Other::S_HateListRequest( NpcGUID );
			s_lastTime = time;
		}
	}
	else
	{
		NetCli_OtherChild::s_NPCHateListGUID = -1;
	}

	/*
	else
	{
		pSprite = NULL;
		PartyMemberInfo* pInfo;
  		CRoleSprite* pPartyMember;
		CRoleSprite* pTarget;
		int i = 0;
		while(true)
		{
			pInfo = NetCli_PartyChild::GetPartyMember(i);
			i++;
		 
			if( pInfo == NULL || pInfo->sprite == NULL )
				break;

			pPartyMember = g_pGameMain->FindSpriteByDBID( pInfo->DBID );

			if( NULL == pPartyMember )
			{
				continue;
			}

			//pPartyMember = (CRoleSprite*)pInfo->sprite;

			pTarget = pPartyMember->GetTargetSprite();

			if( pTarget && pTarget->GetCanAttackState() && pTarget->GetHP() > 0 )
			{
				pSprite = pTarget;
				break;
			}
		}
  	}
	*/


	return 0;
}

int LuaFunc_GetTargetHateList(lua_State* L)
{
	//CRoleSprite* pSprite = g_pGameMain->GetPlayerTarget();

	CRoleSprite* pSprite = g_pGameMain->FindSpriteByID( NetCli_OtherChild::s_NPCHateListGUID );
 
	//NetCli_OtherChild::s_NPCHateListGUID;
	//NetCli_OtherChild::s_NPCHateList;

  	if( NULL == pSprite )
	{
		return 0;
	}

	//if( pSprite->GetIdentify() != NetCli_OtherChild::s_NPCHateListGUID )
	//{
	//	return 0;
	//}

	//if( pSprite->GetHP() == 0 )
	//{
	//	return 0;
	//}

	int index = luaL_checkint( L, 1 ) - 1;
	StaticVector< NPCHateStruct , _MAX_HATELIST_SIZE_ >& Hate = NetCli_OtherChild::s_NPCHateList.Hate;
	int count = Hate.Size();
	if( index < 0 )
	{
 		lua_pushnumber( L, count );
		lua_pushstring( L, pSprite->GetName() );
		return 2;
	}

	if( index >= count )
	{
		return 0;
	}
 
	NPCHateStruct& NPCHate = Hate[index];

	//NPCHate.DBID;
	//NPCHate.ItemID;
	//NPCHate.HitPoint;
	//NPCHate.HatePoint;
	//NPCHate.TempHatePoint;
  
	CRoleSprite* sprite = g_pGameMain->FindSpriteByDBID( NPCHate.DBID );

	if( sprite == NULL )
	{
 		return 0;
	}

  	lua_pushstring( L, sprite->GetName() );
	lua_pushnumber( L, NPCHate.TotalHatePoint() );
 	lua_pushnumber( L, NPCHate.HitPoint );
	
	return 3;
}




int LuaFunc_GetClassCount(lua_State* L);
int LuaFunc_GetClassID(lua_State* L);
int LuaFunc_GetClassInfo(lua_State* L);


static vector<int> s_ClassIDTable;
// ----------------------------------------------------------------------------
int LuaFunc_GetClassCount(lua_State* L)
{
	if( s_ClassIDTable.empty() )
	{
		for( int i = 0 ; i < 100 ; ++i )
		{
			if( i == EM_Vocation_GameMaster )
				continue;

			//判斷有沒有該職業
			if( g_ObjectData->GetObj( Def_ObjectClass_LearnMagic + i ) )
			{
				s_ClassIDTable.push_back( i );
			}
  		}
	}
 	lua_pushnumber( L, (lua_Number)s_ClassIDTable.size() );
	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_GetClassID(lua_State* L)
{
	int index = luaL_checkint( L, 1 ) - 1;
	if( index < 0 || index >= s_ClassIDTable.size() )
		return 0;

	int id = s_ClassIDTable[index];

	lua_pushnumber( L, id );
	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_GetClassInfoByID(lua_State* L)
{
	int id = luaL_checkint( L, 1 );

	string strClass = g_ObjectData->GetClassNameByID( (Voc_ENUM)id );
	string strToken = g_ObjectData->GetClassTokenByID( (Voc_ENUM)id );
	
	lua_pushstring( L, strClass.c_str() );
	lua_pushstring( L, strToken.c_str() );
	return 2;
}


