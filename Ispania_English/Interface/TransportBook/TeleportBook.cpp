#include "TeleportBook.h"
#include "..\..\mainproc\NetGlobal.h"
#include "..\..\mainproc\GameMain.h"
#include "../WorldFrames/ItemClipboard.h"
#include "..\map\MapCommon.h"
#include "..\NetWaker_member\net_teleport\NetCli_Teleport.h"
#include "..\NetWaker_member\net_Channel\NetCli_Channel.h"
#include "../WorldFrames/ChatFrame.h"
#include "../WorldFrames/ChatFrame.h"
#include "../../tools/Common/Com_Function.h"
#include "../WorldFrames/ItemUTIL.h"
#include "../WorldFrames/CastingBarFrame.h"

CUI_TeleportBook*		g_pTeleportBook=NULL;
int LuaFunc_TB_GetTeleportInfo( lua_State *L);
int LuaFunc_TB_SetBookMark( lua_State *L);


int LuaFunc_TB_EditNote( lua_State *L);
int LuaFunc_TB_Teleport( lua_State *L);
int LuaFunc_TB_DeleteTeleport( lua_State *L);
int LuaFunc_TB_GetTeleportItem( lua_State *L);
int LuaFunc_TB_ChangeSlot( lua_State *L);
int LuaFunc_TB_PickupItem( lua_State *L);
int LuaFunc_TB_GetItemName( lua_State *L);



int LuaFunc_CRG_InviteListOK(lua_State *L);
int LuaFunc_CRG_InviteChannel(lua_State *L);
int LuaFunc_CRG_ListSetToggle( lua_State *L);
int LuaFunc_CRG_ListGetInfo( lua_State *L);
int LuaFunc_CRG_ListGetCount( lua_State *L);
int LuaFunc_CRG_ListReflash( lua_State *L);

int LuaFunc_CRG_MemberGetCount( lua_State *L);
int LuaFunc_CRG_MemberGetName( lua_State *L);
int LuaFunc_CRG_GetChannelOwner( lua_State *L);
int LuaFunc_CRG_GetChannelName( lua_State *L);
int LuaFunc_CRG_GetChannelPass( lua_State *L);


int LuaFunc_CRG_SetMsnFontSize( lua_State *L);

int LuaFunc_CRG_GetMsnFontSize( lua_State *L);
int LuaFunc_CRG_SetAutoCheck( lua_State *L);


bool isVipTime(void );
char* zone_cityname[]={
	"VARANAS"
	//"HINTERVALE"
};

int comp_CityName(char *Name)
{
	int index=-1;
	for(int i=0;i<sizeof(zone_cityname)/sizeof(char *);i++)
	{
		if (!strcmp(Name,zone_cityname[i]))
			index=i;
	}

	return index;

}
void CUI_TeleportBook::GetItemName(LocationSaveStruct* posTab,char *ItemName)
{

	if(posTab&&posTab->IconType!=0)
	{		
		int ZoneID=posTab->ZoneID;
		int x=(int)posTab->X;
		int y=(int)posTab->Y;
		int z=(int)posTab->Z;		

		float CoordX;
		float CoordY;
		int DBZoneID = ZoneID% _DEF_ZONE_BASE_COUNT_ + Def_ObjectClass_Zone ;
		GameObjDbStructEx* zoneDB = CNetGlobal::GetObj( DBZoneID );
		if (zoneDB)
		{

			const WorldMapInfoStruct* pWorldMapInfo = g_ObjectData->GetWorldMapInfoByID( ZoneID % _DEF_ZONE_BASE_COUNT_ );
			if (pWorldMapInfo)
			{



				SWorldCoordinate WorldCoord;
				SMapCoordinate MapCoord;


				WorldCoord.wdbFile = pWorldMapInfo->wdbFile;
				WorldCoord.PosX =(float) x;
				WorldCoord.PosZ = (float)z;

				MapCoord.MapID = pWorldMapInfo->MapID;



				if( Func_TransformCoordinate_WorldToMap( WorldCoord, MapCoord ) )
				{
					CoordX = MapCoord.PosX;
					CoordY = MapCoord.PosZ;
				}
				const char *cZoneName=	Func_GetLocalMapName(ZoneID % _DEF_ZONE_BASE_COUNT_);
			


			
				const char *ZoneName=zoneDB->GetName();

				if (cZoneName)
					ZoneName=cZoneName;

				sprintf(ItemName,"%s(%3.1f,%3.1f)",ZoneName,(CoordX*100),(CoordY*100));
			}
		}
	}

}

CUI_TeleportBook::CUI_TeleportBook(CInterface* object): CInterfaceBase( object )
{
	m_flag=TB_ENABLE_BATTLE | TB_ENABLE_COOLDOWN ;
	m_CoolDown=0;
	g_pTeleportBook=this;
	m_state=0;

}

void CUI_TeleportBook::RegisterFunc(void)
{
	lua_State* l = GetLuaStateWorld();
	if (l)
	{
		lua_register( l, "TB_GetTeleportInfo",			LuaFunc_TB_GetTeleportInfo		);
		lua_register( l, "TB_SetBookMark",				LuaFunc_TB_SetBookMark		);
		lua_register( l, "TB_EditNote",					LuaFunc_TB_EditNote		);
		lua_register( l, "TB_ChangeSlot",				LuaFunc_TB_ChangeSlot		);
		lua_register( l, "TB_Teleport",					LuaFunc_TB_Teleport		);
		lua_register( l, "TB_DeleteTeleport",			LuaFunc_TB_DeleteTeleport	);
		lua_register( l, "TB_GetTeleportItem",			LuaFunc_TB_GetTeleportItem		);
		lua_register( l, "TB_PickupItem",				LuaFunc_TB_PickupItem );
		lua_register( l, "TB_GetItemName",				LuaFunc_TB_GetItemName );

		
		

		lua_register( l, "CRG_InviteListOK",				LuaFunc_CRG_InviteListOK );
	
		lua_register( l, "CRG_ListSetToggle",				LuaFunc_CRG_ListSetToggle );
		lua_register( l, "CRG_ListGetInfo",					LuaFunc_CRG_ListGetInfo );
		lua_register( l, "CRG_ListGetCount",				LuaFunc_CRG_ListGetCount );
		lua_register( l, "CRG_ListReflash",					LuaFunc_CRG_ListReflash );


		lua_register( l, "CRG_MemberGetCount",				LuaFunc_CRG_MemberGetCount );
		lua_register( l, "CRG_MemberGetName",				LuaFunc_CRG_MemberGetName );
		lua_register( l, "CRG_GetChannelOwner",				LuaFunc_CRG_GetChannelOwner );
		lua_register( l, "CRG_GetChannelName",				LuaFunc_CRG_GetChannelName );
		lua_register( l, "CRG_GetChannelPass",				LuaFunc_CRG_GetChannelPass );

	
		lua_register( l, "CRG_InviteChannel",				LuaFunc_CRG_InviteChannel );
		lua_register( l, "CRG_SetMsnFontSize",				LuaFunc_CRG_SetMsnFontSize );
		lua_register( l, "CRG_GetMsnFontSize",				LuaFunc_CRG_GetMsnFontSize );
		lua_register( l, "CRG_SetAutoCheck",				LuaFunc_CRG_SetAutoCheck );


	
		
	}

}
LocationSaveStruct* CUI_TeleportBook::GetItem(int index)
{
	RoleDataEx* role = CNetGlobal::RoleData();

	LocationSaveStruct* posTab=NULL;
	if  (index>=0&&index<_MAX_LOCATION_SAVE_COUNT_)
	{
		
		posTab=	&role->PlayerBaseData->LocationSave[index];
	

	}
	return posTab;

}
void CUI_TeleportBook::GetItemIcon(int icon,char * file)
{
	int city=(icon&0x0ff);

	int iconx=(icon>>8)%1000;

	if (city)
		sprintf(file,"interface/icons/map_zone%d_city",iconx);
	else	
		sprintf(file,"interface/icons/map_zone%d",iconx);
	
	
}
void CUI_TeleportBook::PickupItem(int index)
{

	LocationSaveStruct* item = GetItem( index );
	if ( item )
	{
		ClipboardItemInfo itemInfo = g_pItemClipboard->GetItemInfo();

		int destIndex;
		switch ( g_pItemClipboard->GetType() )
		{
		case eClipboardItem_Type_Teleport:			

			destIndex = itemInfo.teleport.pos;
			if (destIndex!=index)
			{
				if ( destIndex >= 0 && destIndex < _MAX_LOCATION_SAVE_COUNT_ )
				{
					ChangeSlot(index,destIndex);
				}
				g_pItemClipboard->Clear();			// 清除資料
				SendUpdateEvent();
			}
			break;

		default:
			if ( item->IconType )
			{
				g_pItemClipboard->Clear();			// 清除資料
				g_pItemClipboard->SetName(item->Note);
				{
					char temp[MAX_PATH];
					GetItemIcon(item->IconType,temp);
					g_pItemClipboard->SetFile(temp);
				}
				g_pItemClipboard->PickupTeleport(index);
				break;
			}
		}		
	}
}

bool isVipTime(void )
{

	time_t timeZone= g_pGameMain->GetServerTimeZone() ;

	
	time_t newtime=g_pGameMain->GetGameTime();

	RoleDataEx* pRole=CNetGlobal::RoleData();
	float time=pRole->PlayerBaseData->VipMember.Term;
	time_t time2=TimeExchangeFloatToInt(time);

	time2-=timeZone*60*60;

	int df=(int)difftime(time2,newtime);

	struct tm *gmt,nowtm;
	
	
	gmt = localtime( &newtime );	
	nowtm=*gmt;

	gmt = localtime( &time2 );

	return df>0;
}

void CUI_TeleportBook::Teleport(int type,int pos)
{

	LocationSaveStruct* item=GetItem(pos);
	if (item&&item->IconType!=0)
	{
		if (m_CoolDown>0)
		{
			if (!(m_flag&TB_ENABLE_COOLDOWN))
				return ;
		}
		m_CoolDown=TB_COOLDOWN_COUNT;
		bool saveItem=false;
		bool Useitem[3]={false,false,false};

		int saveitemPos=-1;
		int usePos[3]={-1,-1,-1};



		GetTeleportItem(saveItem,Useitem[0],Useitem[1],Useitem[2],saveitemPos,usePos[0],usePos[1],usePos[2]);

		bool bFlag_Teleport=CNetGlobal::RoleData()->PlayerBaseData->VipMember.Flag_Teleport;

		bool bVip=isVipTime();		


		if ((Useitem[type]&& usePos[type]!=-1)||(bFlag_Teleport&&bVip ))
		{
			NetCli_Teleport::SL_UseTeleportRequest((UseTeleportTypeENUM)type,usePos[type],pos);
		}
	}
}
bool CUI_TeleportBook::CanUseTeleport(int pos)
{
	bool ret=false;
	int retE=CanUseTeleport1(pos);
	if (retE>0)
		ret=true;
	return ret;
}
int CUI_TeleportBook::CanUseTeleport1(int pos)
{
	bool saveItem=false;
	bool Useitem1=false;
	bool Useitem2=false;
	bool Useitem3=false;
	int ItemPos=-1;
	int usePos1=-1;
	int usePos2=-1;
	int usePos3=-1;

	
	if (m_state&TB_STATE_BATTLE)//戰鬥中
	{
		if (!(m_flag&TB_ENABLE_BATTLE) )//戰鬥中可用
			return -3;
	}

	if( g_pGameMain->IsBattleGroundZone() )
	{
		return -2;
	}
	


	if (m_CoolDown<0)//CD中
	{

		if (!(m_flag&TB_ENABLE_COOLDOWN) )//CD中可用
			return -1;
	}



	if (pos>=0&&pos<_MAX_LOCATION_SAVE_COUNT_)
		GetTeleportItem(saveItem,Useitem1,Useitem2,Useitem3,ItemPos,usePos1,usePos2,usePos3);

	bool bFlag_Teleport=CNetGlobal::RoleData()->PlayerBaseData->VipMember.Flag_Teleport;

	if (bFlag_Teleport&&isVipTime())
		Useitem1=true;


	return Useitem1;


}
void			 CUI_TeleportBook::	Update(float elapsedTime)	
{

	if (m_CoolDown>0)
	{
		m_CoolDown-=elapsedTime;


		if (m_CoolDown<=0&&m_state&TB_STATE_BOOKMARK)
		{

			m_state&=~TB_STATE_BOOKMARK;

			m_CoolDown=0;

			CRoleSprite* player = g_pGameMain->GetPlayer();
			if ( player ) 
			{
				player->ClearCastingEffect();

				player->SetActorState( ruFUSION_ACTORSTATE_CAST_END);
				player->SetActorState( ruFUSION_ACTORSTATE_NORMAL,1);
				player->SetActorState(ruFUSION_ACTORSTATE_USE);
				

			//	player->BeginCastingEffect(g_ObjectData->GetString("FX_NAME_BOOKMARKING2"), "p_down", false);
			

				
				CEffectSprite* pEffect = new CEffectSprite;
				if ( pEffect->LoadRuEntity(g_ObjectData->GetString("FX_NAME_BOOKMARKING2")) )
				{
					pEffect->AttachSprite(player, eAttach_Down);
					pEffect->SetEffectMotionStep(eEffectMotion_Step_Explode);
				}
				else
					delete pEffect;

			}
			g_pCastingBarFrame->CastingStop(eCastingBar_Teleport);

			AddTeleportRequest(m_TeleportPos);
		}
		if (m_CoolDown<=0&&m_state&TB_STATE_TELEPORT)
		{

			m_state&=~TB_STATE_TELEPORT;

			m_CoolDown=0;

			CRoleSprite* player = g_pGameMain->GetPlayer();
			if ( player ) 
			{
				//player->SetActorState( ruFUSION_ACTORSTATE_NORMAL,1);
				player->SetActorState(ruFUSION_ACTORSTATE_CAST_END);
				player->SetActorState(ruFUSION_ACTORSTATE_BUFF_BEGIN );
				player->SetActorState(ruFUSION_ACTORSTATE_BUFF_INSTANT );
				player->ClearCastingEffect();

				//player->BeginCastingEffect(g_ObjectData->GetString("FX_NAME_TELEPORTING1"), "p_down", false);
				

				CEffectSprite* pEffect = new CEffectSprite;
				if ( pEffect->LoadRuEntity(g_ObjectData->GetString("FX_NAME_TELEPORTING1")) )
				{
					pEffect->AttachSprite(player, eAttach_Down);
					pEffect->SetEffectMotionStep(eEffectMotion_Step_Explode);
				}
				else
					delete pEffect;


				

				



			}
			g_pCastingBarFrame->CastingStop(eCastingBar_Teleport);
			Teleport(0,m_TeleportPos);
		}
		if (m_CoolDown<=0&&m_state&TB_STATE_OPENGATE)
		{

			m_state&=~TB_STATE_OPENGATE;

			m_CoolDown=0;

			CRoleSprite* player = g_pGameMain->GetPlayer();
			if ( player ) 
			{
				player->SetActorState(ruFUSION_ACTORSTATE_BUFF_BEGIN );
				player->SetActorState(ruFUSION_ACTORSTATE_BUFF_INSTANT );
				player->ClearCastingEffect();







			}

			g_pCastingBarFrame->CastingStop(eCastingBar_Teleport);
			Teleport(1,m_TeleportPos);
		}
		if (m_CoolDown<=0&&m_state&TB_STATE_OPENCHANNEL)
		{

			m_state&=~TB_STATE_OPENCHANNEL;

			m_CoolDown=0;

			CRoleSprite* player = g_pGameMain->GetPlayer();
			if ( player ) 
			{
				
				player->SetActorState(ruFUSION_ACTORSTATE_BUFF_BEGIN );
				player->SetActorState(ruFUSION_ACTORSTATE_BUFF_INSTANT);
				player->ClearCastingEffect();




			}

			g_pCastingBarFrame->CastingStop(eCastingBar_Teleport);
			Teleport(2,m_TeleportPos);
		}


	}



}
void CUI_TeleportBook::ChangeSlot(int pos1,int pos2)
{
	int pos[2];
	pos[0]=pos1;
	pos[1]=pos2;


	NetCli_Teleport::SL_SwapTeleportRequest(pos);
}

void CUI_TeleportBook::GetTeleportItem(bool&save,bool &use1,bool &use2,bool &use3,int& savePos,int& UesPos1,int& UesPos2,int& UesPos3)
{
	ItemFieldStruct* item;

	int savenum=g_ObjectData->GetSysKeyValue("Teleport_SaveItem");
	int usenum1=g_ObjectData->GetSysKeyValue("Teleport_UseItem1");
	int usenum2=g_ObjectData->GetSysKeyValue("Teleport_UseItem2");
	int usenum3=g_ObjectData->GetSysKeyValue("Teleport_UseItem3");


	for (int i=0;i<_MAX_BODY_COUNT_;i++)
	{
		item= CNetGlobal::RoleData()->GetBodyItem(i);
		if (item)
		{
			if (item->OrgObjID!=0)
			{
				if (item->OrgObjID== savenum)
				{
					savePos=i;
					save=true;
				}
				 if (item->OrgObjID== usenum1)
				{
					UesPos1=i;
					use1=true;
				}
				 if (item->OrgObjID== usenum2)
				{
					UesPos2=i;
					use2=true;
				}
				 if (item->OrgObjID== usenum3)
				{
					UesPos3=i;
					use3=true;
				}

				if (use1&&use2&&use3&&save)
					return ;
			}
		}
	}
}
int CUI_TeleportBook::findFreeSlot()
{
	int i;
	RoleDataEx* role = CNetGlobal::RoleData();

	for (i=0;i<_MAX_LOCATION_SAVE_COUNT_;i++)
	{
		LocationSaveStruct* posTab;
		posTab=	&role->PlayerBaseData->LocationSave[i];
		if (posTab->IconType==0)
			return i;

	}
	return -1;

}
void CUI_TeleportBook::ReportNeedItem(int index)
{
	char temp[512];

	string itemName;
	GetItemName(index,itemName);


	const char *pMsg=g_ObjectData->GetString("ITEM_TRANSFER_2");
	sprintf(temp,"%s %s",pMsg,itemName.c_str());


	g_pChatFrame->SendMsgEvent(CHAT_MSG_SYSTEM, temp,"" );

	g_pGameMain->SendWarningMsg(temp);	

}
void  CUI_TeleportBook:: DoTelePortCasting(int index )
{
	if( g_pGameMain->IsBattleGroundZone() )
		return;
	g_pGameMain->CancelCastSpell();


	if (m_CoolDown<=0&& !(m_state&TB_STATE_TELEPORT))
	{

		// 角色動作與特效
		CRoleSprite* player = g_pGameMain->GetPlayer();
		if ( player ) 
		{
			player->SetActorState(ruFUSION_ACTORSTATE_CAST_BEGIN);
			player->ClearCastingEffect();

			player->BeginCastingEffect(g_ObjectData->GetString("FX_NAME_TELEPORTING"), "p_left_hand", false);
			player->BeginCastingEffect(g_ObjectData->GetString("FX_NAME_TELEPORTING"), "p_right_hand", false);
			/*
			CEffectSprite* pEffect = new CEffectSprite;
			if ( pEffect->LoadRuEntity(g_ObjectData->GetString("FX_NAME_TELEPORTING")) )
			{
				pEffect->AttachSprite(player, eAttach_Left_Hand);
				pEffect->SetEffectMotionStep(eEffectMotion_Step_Explode);
			}
			else
				delete pEffect;
			pEffect = new CEffectSprite;
			if ( pEffect->LoadRuEntity(g_ObjectData->GetString("FX_NAME_TELEPORTING")) )
			{
				pEffect->AttachSprite(player, eAttach_Right_Hand);
				pEffect->SetEffectMotionStep(eEffectMotion_Step_Explode);
			}
			else
				delete pEffect;
				*/

		}
		g_pCastingBarFrame->CastingStart(eCastingBar_Teleport, g_ObjectData->GetString("TB_TELEPORT_MSG"),TB_COOLDOWN_COUNT);

		m_TeleportPos=index;
		m_state|=TB_STATE_TELEPORT;

		m_CoolDown=TB_COOLDOWN_COUNT;
	}

}

bool CUI_TeleportBook::BreakCasting(void)
{

	bool bCasting=false;
	
	if(m_state&TB_STATE_BOOKMARK ||m_state&TB_STATE_TELEPORT||m_state&TB_STATE_OPENGATE||m_state&TB_STATE_OPENCHANNEL)
	{
		m_CoolDown=0;

		g_pCastingBarFrame->CastingFailed(eCastingBar_Teleport);
		// 角色動作與特效
		CRoleSprite* player = g_pGameMain->GetPlayer();
		if ( player ) 
		{
			player->EndCastingEffect();
			player->SetActorState(ruFUSION_ACTORSTATE_CAST_END);
			//player->SetActorState(ruFUSION_ACTORSTATE_NORMAL,1);
		}

		m_state&=~(TB_STATE_BOOKMARK|TB_STATE_TELEPORT|TB_STATE_OPENGATE|TB_STATE_OPENCHANNEL);

		bCasting=true;
	}

	return bCasting;

}
void  CUI_TeleportBook::DoOpenChannelCasting(int index )
{
	if( g_pGameMain->IsBattleGroundZone() )
			return;
	g_pGameMain->CancelCastSpell();

	if (m_CoolDown<=0&& !(m_state&TB_STATE_OPENCHANNEL))
	{

		// 角色動作與特效
		CRoleSprite* player = g_pGameMain->GetPlayer();
		if ( player ) 
		{
				player->SetActorState(ruFUSION_ACTORSTATE_CAST_BEGIN);
			player->SetActorState(ruFUSION_ACTORSTATE_BUFF_INSTANT);
			
			
			player->ClearCastingEffect();

			player->BeginCastingEffect(g_ObjectData->GetString("FX_NAME_OPENGATE"), "p_left_hand", false);
			player->BeginCastingEffect(g_ObjectData->GetString("FX_NAME_OPENGATE"), "p_right_hand", false);
			/*
			CEffectSprite* pEffect = new CEffectSprite;
			if ( pEffect->LoadRuEntity(g_ObjectData->GetString("FX_NAME_OPENGATE")) )
			{
				pEffect->AttachSprite(player, eAttach_Left_Hand);
				pEffect->SetEffectMotionStep(eEffectMotion_Step_Explode);
			}
			else
				delete pEffect;
			pEffect = new CEffectSprite;
			if ( pEffect->LoadRuEntity(g_ObjectData->GetString("FX_NAME_OPENGATE")) )
			{
				pEffect->AttachSprite(player, eAttach_Right_Hand);
				pEffect->SetEffectMotionStep(eEffectMotion_Step_Explode);
			}
			else
				delete pEffect;

				*/


		}
		g_pCastingBarFrame->CastingStart(eCastingBar_Teleport, g_ObjectData->GetString("TB_OPENCHANNEL_MSG"),TB_COOLDOWN_COUNT);

		m_TeleportPos=index;
		m_state|=TB_STATE_OPENCHANNEL;

		m_CoolDown=TB_COOLDOWN_COUNT;
	}

}
void CUI_TeleportBook::DoOpenGateCasting(int index )
{

	if( g_pGameMain->IsBattleGroundZone() )
		return;
	g_pGameMain->CancelCastSpell();

	if (m_CoolDown<=0&& !(m_state&TB_STATE_OPENGATE))
	{

		// 角色動作與特效
		CRoleSprite* player = g_pGameMain->GetPlayer();
		if ( player ) 
		{
				player->SetActorState(ruFUSION_ACTORSTATE_CAST_BEGIN);
			player->SetActorState(ruFUSION_ACTORSTATE_BUFF_INSTANT);
		
			player->ClearCastingEffect();


				player->BeginCastingEffect(g_ObjectData->GetString("FX_NAME_OPENGATE"), "p_left_hand", false);
				player->BeginCastingEffect(g_ObjectData->GetString("FX_NAME_OPENGATE"), "p_right_hand", false);

				/*
				CEffectSprite* pEffect = new CEffectSprite;
				if ( pEffect->LoadRuEntity(g_ObjectData->GetString("FX_NAME_OPENGATE")) )
				{
					pEffect->AttachSprite(player, eAttach_Left_Hand);
					pEffect->SetEffectMotionStep(eEffectMotion_Step_Explode);
				}
				else
					delete pEffect;
				pEffect = new CEffectSprite;
				if ( pEffect->LoadRuEntity(g_ObjectData->GetString("FX_NAME_OPENGATE")) )
				{
					pEffect->AttachSprite(player, eAttach_Right_Hand);
					pEffect->SetEffectMotionStep(eEffectMotion_Step_Explode);
				}
				else
					delete pEffect;

		*/

		}
		g_pCastingBarFrame->CastingStart(eCastingBar_Teleport, g_ObjectData->GetString("TB_OPENGATE_MSG"),TB_COOLDOWN_COUNT);

		m_TeleportPos=index;
		m_state|=TB_STATE_OPENGATE;

		m_CoolDown=TB_COOLDOWN_COUNT;
	}


}
void CUI_TeleportBook::DoBookMarkCasting(int TeleportPos )
{
	if( g_pGameMain->IsBattleGroundZone() )
		return;
	g_pGameMain->CancelCastSpell();

	if (m_CoolDown<=0&& !(m_state&TB_STATE_BOOKMARK))
	{

		// 角色動作與特效
		CRoleSprite* player = g_pGameMain->GetPlayer();
		if ( player ) 
		{
			player->SetActorState(ruFUSION_ACTORSTATE_CRAFTING_BEGIN);
			player->ClearCastingEffect();
			player->BeginCastingEffect(g_ObjectData->GetString("FX_NAME_BOOKMARKING"),"p_left_hand",false);
			player->BeginCastingEffect(g_ObjectData->GetString("FX_NAME_BOOKMARKING"),"p_right_hand",false);

			/*
			CEffectSprite* pEffect = new CEffectSprite;
			if ( pEffect->LoadRuEntity(g_ObjectData->GetString("FX_NAME_BOOKMARKING")) )
			{
				pEffect->AttachSprite(player, eAttach_Left_Hand);
				pEffect->SetEffectMotionStep(eEffectMotion_Step_Idle);
				pEffect->P

			}
			else
				delete pEffect;
			pEffect = new CEffectSprite;
			if ( pEffect->LoadRuEntity(g_ObjectData->GetString("FX_NAME_BOOKMARKING")) )
			{
				pEffect->AttachSprite(player, eAttach_Right_Hand);
				pEffect->SetEffectMotionStep(eEffectMotion_Step_Explode);
			}
			else
				delete pEffect;
				*/

		//	player->BeginCastingEffect(g_ObjectData->GetString("FX_NAME_BOOKMARKING"), "p_left_hand", false);
		//	player->BeginCastingEffect(g_ObjectData->GetString("FX_NAME_BOOKMARKING"), "p_right_hand", false);
		}

		m_TeleportPos=TeleportPos;
		m_state|=TB_STATE_BOOKMARK;

		g_pCastingBarFrame->CastingStart(eCastingBar_Teleport, g_ObjectData->GetString("TB_RECODE_MSG"),TB_COOLDOWN_COUNT);

		m_CoolDown=TB_COOLDOWN_COUNT;
	}
}

void CUI_TeleportBook::AddTeleportRequest(int TeleportPos )
{

	bool saveItem=false;
	bool Useitem1=false;
	bool Useitem2=false;
	bool Useitem3=false;
	int ItemPos=-1;
	int usePos1=-1;
	int usePos2=-1;
	int usePos3=-1;

	GetTeleportItem(saveItem,Useitem1,Useitem2,Useitem3,ItemPos,usePos1,usePos2,usePos3);
	bool Flag_Teleport_Save=CNetGlobal::RoleData()->PlayerBaseData->VipMember.Flag_Teleport_Save	;

	
	bool isVip=isVipTime();


	if ((Flag_Teleport_Save&&(isVip)) || (saveItem&&ItemPos!=-1))
	{
		if (TeleportPos<0)
			TeleportPos=findFreeSlot();
		if (TeleportPos>=0&&TeleportPos<_MAX_LOCATION_SAVE_COUNT_)
		{		
			LocationSaveStruct Info;
			Info.Init();
			char temp[MAX_PATH]={0};

			Info.ZoneID= g_pGameMain->GetZoneID() ;

			int DBZoneID = Info.ZoneID % _DEF_ZONE_BASE_COUNT_ + Def_ObjectClass_Zone ;
			GameObjDbStruct* zoneDB = CNetGlobal::GetObj( DBZoneID );
			if (zoneDB)
			{

				

				CRoleSprite* pPlayer = g_pGameMain->GetPlayer();


				// 計算座標

				CRuVector3 pos = pPlayer->GetPos();

			
				{
					
					const char * zoneName=g_pGameMain->GetZoneName();
					char Cname[MAX_PATH];
					strcpy(Cname,zoneName);
					strupr(Cname);
					int index= comp_CityName(Cname);


				
					sprintf(temp, "ZONE_%s", Cname);
				

					const char *name = g_ObjectData->GetString(temp);


					const char *zname =zoneDB->Zone.MapInfoList[0].MapName;
					char Zname[MAX_PATH];
					strcpy(Zname,zname);
					strupr(Zname);

					sprintf(temp, "ZONE_%s", Zname);

					zname = g_ObjectData->GetString(temp);
				

					if (strcmp(zname,name))			
					{
						
						sprintf(temp,"%s-%s",zname,name);
						if (strlen(temp)>=64)
						strncpy(temp,name,64);

					}
					else
						sprintf(temp,"%s",zname);

					Info.IconType=Info.ZoneID<<8 |((index!=-1) ? 1: 0);

					char tempBuff[512];
					strncpy(tempBuff,temp,63);

					tempBuff[63]=0;		
					size_t destLen=63;
					const char* safetempBuff=ItemUTIL::checkUtf8SafeString(tempBuff,destLen,NULL);

					strncpy(Info.Note,safetempBuff,63);
					Info.X=pos.x;
					Info.Y=pos.y;
					Info.Z=pos.z;
					NetCli_Teleport::SL_AddTeleportRequest(ItemPos,TeleportPos,Info);
				}
			}
		}
	}


}

void CUI_TeleportBook::GetItemName(int index,string &Itemname)
{

	const char *name=NULL;
	if (index>=0&&index<4)
	{

		char* table[]={"Teleport_SaveItem","Teleport_UseItem1","Teleport_UseItem2","Teleport_UseItem3"};
		int num=g_ObjectData->GetSysKeyValue(table[index]);

		if (num!=-1)
		{
			GameObjDbStructEx*	pItemDB		= g_ObjectData->GetObj( num );
			if (pItemDB)
			{
				name=pItemDB->GetName();


				// 取得物品名稱
				
			

				// 稀有度
				int color = g_ObjectData->GetItemQualityColor( g_ObjectData->GetItemQuality( pItemDB, NULL ) );

			
				
				Itemname= Com_FormatString( "|c%08x[%s]|r", color, name ); 


			}
		}


		



	}



	





}

void CUI_TeleportBook::SendUpdateEvent(void)
{


	SendWorldEvent("TB_UPDATE");
}

void CUI_TeleportBook::ModifyTeleportResult	( int TeleportPos , LocationSaveStruct& Info , bool Result )
{

	char tmp[MAX_PATH];
	const char *strResult;
	if (Result)
	{	
		RoleDataEx* role = CNetGlobal::RoleData();
		LocationSaveStruct* posTab;
		posTab=	&role->PlayerBaseData->LocationSave[TeleportPos];
		*posTab=Info;
		SendUpdateEvent();
		strResult=g_ObjectData->GetString("TB_TELEPORT_MODIFY_OK");
	}
	else
	{


		strResult=g_ObjectData->GetString("TB_TELEPORT_MODIFY_FAIL");


	}

	sprintf(tmp, "%s",strResult );
	g_pChatFrame->SendMsgEvent("CHAT_MSG_SYSTEM", tmp, "");

}

void CUI_TeleportBook::AddTeleportResult	( int TeleportPos , LocationSaveStruct& Info , bool Result )
{

	char tmp[MAX_PATH];
	const char *strResult;
	if (Result)
	{	
		RoleDataEx* role = CNetGlobal::RoleData();
		LocationSaveStruct* posTab;
		posTab=	&role->PlayerBaseData->LocationSave[TeleportPos];
		*posTab=Info;
		SendUpdateEvent();
		strResult=g_ObjectData->GetString("TB_ADDTELEPORT_OK");
	}
	else
	{


		strResult=g_ObjectData->GetString("TB_ADDTELEPORT_FAIL");
		

	}

	sprintf(tmp, "%s",strResult );
	g_pChatFrame->SendMsgEvent("CHAT_MSG_SYSTEM", tmp, "");

	

}
void CUI_TeleportBook::DelTeleportRequest	( int TeleportPos )
{

	if (TeleportPos>=0&&TeleportPos<_MAX_LOCATION_SAVE_COUNT_)

	NetCli_Teleport::SL_DelTeleportRequest(TeleportPos);

}
void CUI_TeleportBook::UseTeleportResult( int Type , int ItemPos , int TeleportPos , bool Result )
{
	const char *strResult=g_ObjectData->GetString("TB_TELEPORT_FAIL");
	const char *TypeMsg=g_ObjectData->GetString("TB_TELEPORT_TYPE1");
	char tmp[MAX_PATH];
		
		switch(Type)
		{
		case 0:
			TypeMsg=g_ObjectData->GetString("TB_TELEPORT_TYPE1");
			break;
		case 1:
			TypeMsg=g_ObjectData->GetString("TB_TELEPORT_TYPE2");
			break;
		case 2:
			TypeMsg=g_ObjectData->GetString("TB_TELEPORT_TYPE3");
			break;
		}
		

	
	if (Result)
		strResult=g_ObjectData->GetString("TB_TELEPORT_OK");
	else
		strResult=g_ObjectData->GetString("TB_TELEPORT_FAIL");

	SendUpdateEvent();
	sprintf(tmp, "%s%s",TypeMsg,strResult );
	g_pChatFrame->SendMsgEvent("CHAT_MSG_SYSTEM", tmp, "");

}

void CUI_TeleportBook::SwapTeleportResult( int TeleportPos[2] , bool Result )
{

	char tmp[MAX_PATH];
	const char *strResult;
	if (Result)
	{	
		RoleDataEx* role = CNetGlobal::RoleData();
		LocationSaveStruct* posTab1,*posTab2,temp;


		posTab1=	&role->PlayerBaseData->LocationSave[TeleportPos[0]];
		posTab2=	&role->PlayerBaseData->LocationSave[TeleportPos[1]];
		temp=*posTab1;

		*posTab1=*posTab2;
		*posTab2=temp;
		SendUpdateEvent();
		strResult=g_ObjectData->GetString("TB_SWAPTELEPORT_OK");
	}
	else
	{
		strResult=g_ObjectData->GetString("TB_SWAPTELEPORT_FAIL");


	}
	sprintf(tmp, "%s",strResult );
	g_pChatFrame->SendMsgEvent("CHAT_MSG_SYSTEM", tmp, "");

}
void CUI_TeleportBook::DelTeleportResult( int TeleportPos , bool Result )
{
	if (Result)
	{	
		RoleDataEx* role = CNetGlobal::RoleData();
		LocationSaveStruct* posTab,temp={0};
		posTab=	&role->PlayerBaseData->LocationSave[TeleportPos];
		*posTab=temp;
	}
	SendUpdateEvent();
}

void CUI_TeleportBook::EditTeleportNoteRequest(int index,const char*note)
{
	LocationSaveStruct* posTab=NULL;
	if (index>=0&&index<_MAX_LOCATION_SAVE_COUNT_)
	{
		RoleDataEx* role = CNetGlobal::RoleData();
		posTab=	&role->PlayerBaseData->LocationSave[index];

		char tempBuff[512];
		strncpy(tempBuff,note,63);

		tempBuff[63]=0;
		
		size_t destLen=63;
		const char* safeString=ItemUTIL::checkUtf8SafeString(tempBuff,destLen,NULL);

		strncpy(posTab->Note,safeString,63);

		NetCli_Teleport::SL_ModifyTeleportRequest(index,*posTab);
	}

}
LocationSaveStruct* CUI_TeleportBook::GetTeleportInfo(int index )
{

	LocationSaveStruct* posTab=NULL;
	if (index>=0&&index<_MAX_LOCATION_SAVE_COUNT_)
	{
		RoleDataEx* role = CNetGlobal::RoleData();


		posTab=	&role->PlayerBaseData->LocationSave[index];


	

	}
	return posTab;

	
	 
}
int LuaFunc_TB_GetTeleportItem( lua_State *L)
{
	bool saveItem=false;
	bool Useitem1=false;
	bool Useitem2=false;
	bool Useitem3=false;
	int savePos=-1;
	int usePos1=-1;
	int usePos2=-1;
	int usePos3=-1;


	if (g_pTeleportBook)
	{

		g_pTeleportBook->GetTeleportItem(saveItem,Useitem1,Useitem2,Useitem3,savePos,usePos1,usePos2,usePos3);


		bool bFlag_Teleport_Save	=CNetGlobal::RoleData()->PlayerBaseData->VipMember.Flag_Teleport_Save	;
		bool bFlag_Teleport=CNetGlobal::RoleData()->PlayerBaseData->VipMember.Flag_Teleport;

		bool isVip=isVipTime();

		if (bFlag_Teleport_Save&&isVip)
			saveItem=true;
		
		if (bFlag_Teleport&&isVip)			
			Useitem1=true;
		
	}
	lua_pushboolean(L, saveItem);		// Teleports
	lua_pushboolean(L, Useitem1);		// Teleports
	lua_pushboolean(L, Useitem2);		// Teleports
	lua_pushboolean(L, Useitem3);		// Teleports

	return 4;

}
int LuaFunc_TB_Teleport( lua_State *L)
{


	int type = luaL_checkint(L, 1) ;	
	int pos = luaL_checkint(L, 2) -1;	

	if (g_pTeleportBook)
	{

		switch (type)
		{
		case 0:
			g_pTeleportBook->DoTelePortCasting(pos);
			break;
		case 1:
			g_pTeleportBook->DoOpenGateCasting(pos);
			break;
		case 2:
			g_pTeleportBook->DoOpenChannelCasting(pos);
			break;


		}

		//g_pTeleportBook->Teleport(type,pos);
	}



	return 0;
}


int LuaFunc_TB_EditNote( lua_State *L)
{
	int index = luaL_checkint(L, 1)-1 ;	
	const char* note = luaL_checkstring(L, 2) ;	

	if (g_pTeleportBook)
	{

		g_pTeleportBook->EditTeleportNoteRequest(index,note);
	}

	return 0;
}
int LuaFunc_TB_SetBookMark( lua_State *L)
{

	int pos = luaL_checkint(L, 1) -1;	

	if (g_pTeleportBook)
	{


		

		g_pTeleportBook->DoBookMarkCasting(pos);
	}


	return 0;

}
int LuaFunc_TB_DeleteTeleport( lua_State *L)
{
	int pos = luaL_checkint(L, 1) -1;	
	if (g_pTeleportBook)
	{

		g_pTeleportBook->DelTeleportRequest(pos);
	}
	return 0;
}

int LuaFunc_TB_ChangeSlot( lua_State *L)
{
	int pos1 = luaL_checkint(L, 1) -1;	
	int pos2 = luaL_checkint(L, 2) -1;	

	if (g_pTeleportBook)
	{

		g_pTeleportBook->ChangeSlot(pos1,pos2);
	}
	return 0;
}

int LuaFunc_TB_GetItemName( lua_State *L)
{
	int index=luaL_checkint(L, 1) - 1;
	string itename;
	if (g_pTeleportBook)
		g_pTeleportBook->GetItemName(index,itename);


	lua_pushstring(L,itename.c_str());

		return 1;
}

// ----------------------------------------------------------------------------
int	LuaFunc_TB_PickupItem(lua_State* L)
{
	int index=luaL_checkint(L, 1) - 1;
	if (g_pTeleportBook)
		g_pTeleportBook->PickupItem(index);
	return 0;
}
// ----------------------------------------------------------------------------
int LuaFunc_TB_GetTeleportInfo( lua_State *L)
{
	int index = luaL_checkint(L, 1) -1;	

	int IconType=0;
	const char * pNote=g_ObjectData->GetString("TB_NULLITEM_MSG");
	int ZoneID=0;
	int x,y,z;

	char ItemIcon[MAX_PATH]={0};
	char ItemName[MAX_PATH]={0};

	if (g_pTeleportBook)
	{
		LocationSaveStruct* posTab=g_pTeleportBook->GetTeleportInfo(index);	
		if(posTab&&posTab->IconType!=0)
		{
			 x=(int )posTab->X;
			 y=(int )posTab->Y;
			 z=(int )posTab->Z;		
			IconType=posTab->IconType;
			g_pTeleportBook->GetItemIcon(IconType,ItemIcon);
			pNote=posTab->Note;
			ZoneID=posTab->ZoneID;
			CUI_TeleportBook::GetItemName(posTab,ItemName);
		
		}
		
	}	

	lua_pushnumber(L,IconType );
	lua_pushstring(L,pNote);
	lua_pushnumber(L, ZoneID);

	lua_pushnumber(L, x);		// Teleports
	lua_pushnumber(L, y);		// Teleports
	lua_pushnumber(L, z);		// Teleports
	lua_pushstring(L, ItemName);		// Name
	lua_pushstring(L, ItemIcon);		// Name
	return 8;

}
