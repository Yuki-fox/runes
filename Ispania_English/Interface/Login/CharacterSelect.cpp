#include "../../mainproc/GameMain.h"
#include "../../netwaker_member/NetWakerClientInc.h"
#include "AccountLogin.h"
#include "CharacterSelect.h"

#include "../Map/MapCommon.h"


CCharacterSelect*		g_pCharacterSelect	= NULL;

int	LuaFunc_GetNumCharacters				(lua_State *L);
int	LuaFunc_GetCharacterInfo				(lua_State *L);
int	LuaFunc_GetSelectedItem					(lua_State *L);
int	LuaFunc_SelectCharacter					(lua_State *L);
int	LuaFunc_DeleteCharacter					(lua_State *L);
int	LuaFunc_EnterWorld						(lua_State *L);
int	LuaFunc_CancelEnterWorld				(lua_State *L);
int	LuaFunc_OpenCharacterSelect				(lua_State *L);
int	LuaFunc_CloseCharacterSelect			(lua_State *L);
int LuaFunc_RotateCharacterSelectModel		(lua_State *L);
int	LuaFunc_GetNumParalleZones				(lua_State *L);
int	LuaFunc_GetParalleZonesInfo				(lua_State *L);

int LuaFunc_SetCharacterSelectFacing		(lua_State *L);
int LuaFunc_GetCharacterSelectFacing		(lua_State *L);

int LuaFunc_RecoverDeleteCharacter			(lua_State *L);

// ----------------------------------------------------------------------------
CCharacterSelect::CCharacterSelect(CInterface* object) : CInterfaceBase(object)
{	
	g_pCharacterSelect		= this;

	m_sprite		= NULL;
	m_selected		= 0;
	m_boPrompt		= false;

	m_waitQueue		= false;
}

// ----------------------------------------------------------------------------
CCharacterSelect::~CCharacterSelect()
{
	g_pCharacterSelect = NULL;

	UI_SAFE_DELETE(m_sprite);
}

// ----------------------------------------------------------------------------
void CCharacterSelect::RegisterFunc()
{
	lua_State* L = GetLuaStateLogin();
	if (L)
	{
		lua_register(L, "GetNumCharacters",				LuaFunc_GetNumCharacters);
		lua_register(L, "GetCharacterInfo",				LuaFunc_GetCharacterInfo);
		lua_register(L, "GetSelectedItem",				LuaFunc_GetSelectedItem);
		lua_register(L, "SelectCharacter",				LuaFunc_SelectCharacter);
		lua_register(L, "DeleteCharacter",				LuaFunc_DeleteCharacter);
		lua_register(L, "EnterWorld",					LuaFunc_EnterWorld);
		lua_register(L, "CancelEnterWorld",				LuaFunc_CancelEnterWorld);
		lua_register(L, "OpenCharacterSelect",			LuaFunc_OpenCharacterSelect);
		lua_register(L, "CloseCharacterSelect",			LuaFunc_CloseCharacterSelect);
		lua_register(L, "RotateCharacterSelectModel",	LuaFunc_RotateCharacterSelectModel);
		lua_register(L, "GetNumParalleZones",			LuaFunc_GetNumParalleZones);
		lua_register(L, "GetParalleZonesInfo",			LuaFunc_GetParalleZonesInfo);

		lua_register(L, "SetCharacterSelectFacing",		LuaFunc_SetCharacterSelectFacing);
		lua_register(L, "GetCharacterSelectFacing",		LuaFunc_GetCharacterSelectFacing);

		lua_register(L, "RecoverDeleteCharacter",		LuaFunc_RecoverDeleteCharacter);		
	}
}

// --------------------------------------------------------------------------------------
void CCharacterSelect::Open()
{
	if ( m_sprite == NULL )
	{
		m_sprite = new CRoleSprite;
		//m_sprite->SetDirection(CRuVector3(0.0f, 0.0f, -1.0f));
	}

	UpdateSprite();
}

// --------------------------------------------------------------------------------------
void CCharacterSelect::Close()
{
	if ( IBaseSprite::CheckedSpriteIsLife(m_sprite) ) {
		UI_SAFE_DELETE(m_sprite);
	}
	m_sprite = NULL;
	m_selected = 0;
	m_boPrompt = false;
}

// --------------------------------------------------------------------------------------
void CCharacterSelect::LoadCharacterStart()
{
	lua_State* L = GetLuaStateLogin();
	if ( L )
	{
		lua_pushstring(L, "WAITING_QUEUE");
		lua_setglobal(L, "arg1");
		lua_pushstring(L, g_ObjectData->GetString("SYS_LOAD_CHARACTER_DATA"));
		lua_setglobal(L, "arg2");
		SendLoginEvent("OPEN_LOGIN_DIALOG");
	}
}

// --------------------------------------------------------------------------------------
void CCharacterSelect::LoadCharacterEnd()
{	
	lua_State* L = GetLuaStateLogin();
	if ( L )
	{
		SendLoginEvent("CLOSE_LOGIN_DIALOG");
	}
	UpdateCharacterList();	

	if( CNetCli_Login::GetNumCharacters() == 0 )
	{
		UpdateCamera();
	}
}

// --------------------------------------------------------------------------------------
void CCharacterSelect::SetSelected(int index)
{		
	index = min(max(1, index), CNetCli_Login::GetNumCharacters());
	if (m_selected != index)
	{
		lua_State* L = m_interface->GetUiLogin()->GetLuaState();
		m_selected = index;
		if (L)
		{
			lua_pushnumber(L, m_selected);
			lua_setglobal(L, "arg1");
			SendLoginEvent("UPDATE_SELECTED_CHARACTER");
		}
		UpdateSprite();
	}
}

// --------------------------------------------------------------------------------------
void CCharacterSelect::UpdateCharacterList()
{
	lua_State* L = m_interface->GetUiLogin()->GetLuaState();
	if (L)
	{
		SendLoginEvent("UPDATE_CHARACTER_SELECT_LIST");
		SetSelected(m_selected);
	}
}

// --------------------------------------------------------------------------------------
void CCharacterSelect::UpdateSprite()
{
	SampleRoleData* pRoleData = CNetCli_Login::GetCharacterInfo(m_selected);
	if ( pRoleData && m_sprite )
	{
		int color[2];		

#ifdef KALYDO
		// Delay load the character so it can be downloaded if needed.
		m_sprite->SetObjectId(pRoleData->OrgObjID, true);
#else
		m_sprite->SetObjectId(pRoleData->OrgObjID, false);
#endif // KALYDO
		if ( m_sprite->GetRuActor() )
			m_sprite->GetRuActor()->EnableTorsoTurn(false);
		m_sprite->SetPaperdollColor(PAPERDOLL_PART_NAME_HEAD, pRoleData->iBodyColor, 0);
		m_sprite->SetPaperdollColor(PAPERDOLL_PART_NAME_HAIR, pRoleData->iHairColor, 0);
		m_sprite->SetPaperdollPart(PAPERDOLL_PART_NAME_HEAD, pRoleData->iFaceID);
		m_sprite->SetPaperdollPart(PAPERDOLL_PART_NAME_HAIR, pRoleData->iHairID);

		color[0] = pRoleData->EQ.Head.MainColor;
		color[1] = pRoleData->EQ.Head.OffColor;
		ObjectDataClass::SetCharacterEquipColor(g_ObjectData->GetObj(pRoleData->EQ.Head.ImageObjectID), EM_GameObjectImageArmorPos_Head, color);
		m_sprite->SetPaperdollPart(PAPERDOLL_PART_NAME_HELMET, pRoleData->EQ.Head.ImageObjectID);
		m_sprite->SetPaperdollColor(PAPERDOLL_PART_NAME_HELMET,	color[0], color[1]);
		

		color[0] = pRoleData->EQ.Shoulder.MainColor;
		color[1] = pRoleData->EQ.Shoulder.OffColor;
		ObjectDataClass::SetCharacterEquipColor(g_ObjectData->GetObj(pRoleData->EQ.Shoulder.ImageObjectID), EM_GameObjectImageArmorPos_Shoulder, color);
		m_sprite->SetPaperdollPart(PAPERDOLL_PART_NAME_SHOULDER, pRoleData->EQ.Shoulder.ImageObjectID);
		m_sprite->SetPaperdollColor(PAPERDOLL_PART_NAME_SHOULDER, color[0], color[1]);

		color[0] = pRoleData->EQ.Clothes.MainColor;
		color[1] = pRoleData->EQ.Clothes.OffColor;
		ObjectDataClass::SetCharacterEquipColor(g_ObjectData->GetObj(pRoleData->EQ.Clothes.ImageObjectID), EM_GameObjectImageArmorPos_Clothes, color);		
		m_sprite->SetPaperdollPart(PAPERDOLL_PART_NAME_TORSO, pRoleData->EQ.Clothes.ImageObjectID);
		m_sprite->SetPaperdollColor(PAPERDOLL_PART_NAME_TORSO, color[0], color[1]);

		color[0] = pRoleData->EQ.Gloves.MainColor;
		color[1] = pRoleData->EQ.Gloves.OffColor;
		ObjectDataClass::SetCharacterEquipColor(g_ObjectData->GetObj(pRoleData->EQ.Gloves.ImageObjectID), EM_GameObjectImageArmorPos_Gloves, color);
		m_sprite->SetPaperdollPart(PAPERDOLL_PART_NAME_HAND, pRoleData->EQ.Gloves.ImageObjectID);
		m_sprite->SetPaperdollColor(PAPERDOLL_PART_NAME_HAND, color[0], color[1]);

		color[0] = pRoleData->EQ.Belt.MainColor;
		color[1] = pRoleData->EQ.Belt.OffColor;
		ObjectDataClass::SetCharacterEquipColor(g_ObjectData->GetObj(pRoleData->EQ.Belt.ImageObjectID), EM_GameObjectImageArmorPos_Belt, color);
		m_sprite->SetPaperdollPart(PAPERDOLL_PART_NAME_BELT, pRoleData->EQ.Belt.ImageObjectID);
		m_sprite->SetPaperdollColor(PAPERDOLL_PART_NAME_BELT, color[0], color[1]);

		color[0] = pRoleData->EQ.Pants.MainColor;
		color[1] = pRoleData->EQ.Pants.OffColor;
		ObjectDataClass::SetCharacterEquipColor(g_ObjectData->GetObj(pRoleData->EQ.Pants.ImageObjectID), EM_GameObjectImageArmorPos_Pants, color);
		m_sprite->SetPaperdollPart(PAPERDOLL_PART_NAME_LEG, pRoleData->EQ.Pants.ImageObjectID);
		m_sprite->SetPaperdollColor(PAPERDOLL_PART_NAME_LEG, color[0], color[1]);

		color[0] = pRoleData->EQ.Shoes.MainColor;
		color[1] = pRoleData->EQ.Shoes.OffColor;
		ObjectDataClass::SetCharacterEquipColor(g_ObjectData->GetObj(pRoleData->EQ.Shoes.ImageObjectID), EM_GameObjectImageArmorPos_Shoes, color);
		m_sprite->SetPaperdollPart(PAPERDOLL_PART_NAME_FOOT, pRoleData->EQ.Shoes.ImageObjectID);
		m_sprite->SetPaperdollColor(PAPERDOLL_PART_NAME_FOOT, color[0], color[1]);

		color[0] = pRoleData->EQ.Back.MainColor;
		color[1] = pRoleData->EQ.Back.OffColor;
		ObjectDataClass::SetCharacterEquipColor(g_ObjectData->GetObj(pRoleData->EQ.Back.ImageObjectID), EM_GameObjectImageArmorPos_Back, color);
		m_sprite->SetPaperdollPart(PAPERDOLL_PART_NAME_BACK, pRoleData->EQ.Back.ImageObjectID);
		m_sprite->SetPaperdollColor(PAPERDOLL_PART_NAME_BACK, color[0], color[1]);

		m_sprite->SetHandsObjectId(eRoleHands_MainHand,			pRoleData->EQ.MainHand.ImageObjectID);
		m_sprite->SetHandsObjectId(eRoleHands_OffHand,			pRoleData->EQ.SecondHand.ImageObjectID);
		m_sprite->SetWeaponPosition(true);
		
		{
			const char* insigniaType = NULL;
			const char* bannerType = NULL;
			int insigniaColor[2] = { 0, 0 };
			int bannerColor[2] = { 0, 0 };
			m_sprite->CreateOrnamentEntity(pRoleData->EQ.Ornament.ImageObjectID, insigniaType, insigniaColor[0], insigniaColor[1], bannerType, bannerColor[0], bannerColor[1], true);
		}
#ifdef KALYDO
		// Delay load the paperdoll so the needed assets for every character can be downloaded.
		m_sprite->PaperdollBuild(true);		
#else
		m_sprite->PaperdollBuild(false);		
#endif // KALYDO

		m_sprite->SetBoneScale(BONE_SCALE_NAME_HEAD,		(BYTE) pRoleData->BoneScale[0] / 100.0f);
		m_sprite->SetBoneScale(BONE_SCALE_NAME_BREASTS,		(BYTE) pRoleData->BoneScale[1] / 100.0f);
		m_sprite->SetBoneScale(BONE_SCALE_NAME_BODY,		(BYTE) pRoleData->BoneScale[2] / 100.0f);
		m_sprite->SetBoneScale(BONE_SCALE_NAME_CHEST,		(BYTE) pRoleData->BoneScale[3] / 100.0f);
		m_sprite->SetBoneScale(BONE_SCALE_NAME_PELVIS,		(BYTE) pRoleData->BoneScale[4] / 100.0f);		
		m_sprite->SetBoneScale(BONE_SCALE_NAME_ARM,			(BYTE) pRoleData->BoneScale[5] / 100.0f);		
		m_sprite->SetBoneScale(BONE_SCALE_NAME_FOREARM,		(BYTE) pRoleData->BoneScale[6] / 100.0f);		
		m_sprite->SetBoneScale(BONE_SCALE_NAME_HAND,		(BYTE) pRoleData->BoneScale[7] / 100.0f);		
		m_sprite->SetBoneScale(BONE_SCALE_NAME_LEGS,		(BYTE) pRoleData->BoneScale[8] / 100.0f);		
		m_sprite->SetBoneScale(BONE_SCALE_NAME_CALF,		(BYTE) pRoleData->BoneScale[9] / 100.0f);		
		m_sprite->SetBoneScale(BONE_SCALE_NAME_FOOT,		(BYTE) pRoleData->BoneScale[10] / 100.0f);		



		CRuCamera* camera = g_pGameMain->GetCamera();
		if ( camera )
		{
			//CRuVector3 distOffsetVec = camera->GetCameraPosition() - camera->GetTargetPosition();
			//distOffsetVec.m_y = 0.0f;
			//distOffsetVec.Normalize();

			//m_sprite->SetPos(camera->GetTargetPosition() + CRuVector3(0.0f, -14.25f, 0.0f) + distOffsetVec * 53.0f);
			//m_sprite->SetDirection(camera->GetCameraPosition() - camera->GetTargetPosition());

			CRuVector3 Position = g_pAccountLogin->GetAccountLoginCameraLayoutEnd().pos;

			//static float temp = 280.0f;
			//Position.m_y = temp;

			Position.m_y = g_pAccountLogin->m_fPlayerHeight;

			m_sprite->SetPos( Position );
			m_sprite->SetDirection( g_pAccountLogin->m_PlayerDirection );
	//		m_sprite->AnchorSprite( 20.0f );

			UpdateCamera();
		}

		m_sprite->Update( 1.0f );
	}

}

// --------------------------------------------------------------------------------------
void CCharacterSelect::UpdateParalleZone()
{
	lua_State* L = GetLuaState();
	if ( L )
	{
		lua_pushnumber(L, g_pAccountLogin->GetSelectedRegion() + 1);
		lua_setglobal(L, "arg1");
		SendLoginEvent("UPDATE_CHARACTER_PARALLEZONE");
	}
}


// --------------------------------------------------------------------------------------
void CCharacterSelect::UpdateCamera()
{
	CRuCamera* camera = g_pGameMain->GetCamera();
	if ( camera )
	{
		static bool s_FirstMove = true;
		if( s_FirstMove )
		{
			g_pAccountLogin->DoCameraMotion();
			s_FirstMove = false;
		}
		else
		{
			sCameraLayout CameraLayout = g_pAccountLogin->GetAccountLoginCameraLayoutEnd();
			CameraLayout.time = 0.2f;

			//g_pAccountLogin->CleanControlCamera();
			g_pAccountLogin->AddControlCamera( CameraLayout );
		}
	}
}

// --------------------------------------------------------------------------------------
int LuaFunc_GetNumCharacters(lua_State *L)
{
	lua_pushnumber(L, CNetCli_Login::GetNumCharacters());
	return 1;
}

// --------------------------------------------------------------------------------------
int LuaFunc_GetCharacterInfo(lua_State *L)
{	
	SampleRoleData* pRoleData = CNetCli_Login::GetCharacterInfo(luaL_checkint(L, 1));
	if ( pRoleData )
	{
		lua_pushstring(L, pRoleData->RoleName.Begin());			// Name
		lua_pushnumber(L, pRoleData->Sex);						// Sex

		lua_pushnumber(L, pRoleData->Level);					// Level
		lua_pushstring(L, g_ObjectData->GetClassNameByID(pRoleData->Voc));		// Voc
		lua_pushnumber(L, pRoleData->Level_Sub);				// Level		
		lua_pushstring(L, g_ObjectData->GetClassNameByID(pRoleData->Voc_Sub));	// Voc Sub		

		// Zone Name
		string zoneName;
		if ( pRoleData->ZoneName.Size() > 0 ) {

			int MapID = atoi( pRoleData->ZoneName.Begin() );
			zoneName =  Func_GetLocalMapName( MapID );
		}
		lua_pushstring(L, zoneName.c_str());

		if ( pRoleData->DestroyTime > 0 ) {
			lua_pushnumber(L, pRoleData->DestroyTime);			// 剩餘刪除時間
		} else {
			lua_pushnil(L);
		}
		return 8;
	}
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GetSelectedItem(lua_State *L)
{
	lua_pushnumber(L, g_pCharacterSelect->GetSelected());
	return 1;	
}

// ----------------------------------------------------------------------------
int LuaFunc_SelectCharacter(lua_State *L)
{
	g_pCharacterSelect->SetSelected(luaL_checkint(L, 1));
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_DeleteCharacter(lua_State *L)
{
	CNetCli_Login::DeleteCharacter(luaL_checkint(L, 1), luaL_checkstring(L, 2));
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_RecoverDeleteCharacter(lua_State *L)
{
	CNetCli_Login::RecoverCharacter(luaL_checkint(L, 1));
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_EnterWorld(lua_State *L)
{
	const UpdateInfInfo& info = g_pGameMain->GetUpdateInfo();

	// 如果有提示公告說明內容
	if ( info.enterWorldPrompt != 0 && !g_pCharacterSelect->GetPromptState() )
	{
		g_pCharacterSelect->SetPromptState(true);

		lua_pushstring(L, "CONFIRM_ENTER_WORLD");
		lua_setglobal(L, "arg1");
		lua_pushstring(L, g_ObjectData->GetString("ENTER_WORLD_PROMPT"));
		lua_setglobal(L, "arg2");
		g_pCharacterSelect->SendLoginEvent("OPEN_LOGIN_DIALOG");
	}
	else
	{
		int ParalleZoneID = luaL_checkint(L, 1) - 1;
		if ( ParalleZoneID < 0 ) ParalleZoneID = 0;
		if ( ParalleZoneID >= CNetCli_Login::GetNumParalleZones() ) ParalleZoneID = CNetCli_Login::GetNumParalleZones() - 1;
		g_pAccountLogin->SetParalleZoneID(ParalleZoneID);
		CNetCli_Login::EnterWorld(g_pCharacterSelect->GetSelected() , ParalleZoneID );

		g_pCharacterSelect->SetPromptState(false);
	}
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_CancelEnterWorld(lua_State *L)
{
	CNetCli_Login::s_enterZone1 = false;
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_OpenCharacterSelect(lua_State *L)
{
	g_pCharacterSelect->Open();
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_CloseCharacterSelect(lua_State *L)
{
	g_pCharacterSelect->Close();
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_RotateCharacterSelectModel(lua_State *L)
{
	const char* str = luaL_checkstring(L, 1);
	CRoleSprite* sprite = g_pCharacterSelect->GetRoleSprite();
	if ( sprite )
	{
		CRuVector3 dir = sprite->GetDirection();
		float theta, angle;

		// 取得角色所朝方向
		theta = acos( dir.Dot( CRuVector3(1.0f, 0.0f, 0.0f) ) );
		if ( dir.m_z > 0.0f )
			theta = PI + PI - theta;

		// 轉換為角度
		theta = theta * 180.0f / PI;

		if ( strcmp(str, "LEFT") == 0 )
			theta += 5.0f;
		else if ( strcmp(str, "RIGHT") == 0 )
			theta -= 5.0f;

		angle = theta * PI / 180.0f;

		sprite->SetDirection(CRuVector3(cos(angle), 0.0f, -sin(angle)));
	}
	return 0;
}
// ----------------------------------------------------------------------------
int	LuaFunc_GetNumParalleZones(lua_State *L)
{
	lua_pushnumber(L, CNetCli_Login::GetNumParalleZones());
	return 1;
}
// ----------------------------------------------------------------------------
int	LuaFunc_GetParalleZonesInfo(lua_State *L)
{	
	char name[256];
	int loadType = 1;
	
	memset(name, 0, sizeof(name));

	int index = luaL_checkint(L, 1) - 1;
	int maxZones = CNetCli_Login::GetNumParalleZones();
	if ( index < maxZones ) {
		string realName = g_pAccountLogin->GetRealmName();
		float zoneLoad[256];

		// 取得分流負載量
		memset(zoneLoad, 0, sizeof(zoneLoad));
		int size = CNetCli_Login_Child::GetParalleZonesLoad(realName.c_str(), zoneLoad, 256);
		
		// 名稱
		sprintf_s(name, 256, "%s %d", realName.c_str(), index + 1);

		
		// 壓力量
		if ( zoneLoad[index] < 0.4 ) {
			loadType = 1;
		} else if ( zoneLoad[index] < 0.85 ) {
			loadType = 2;
		} else {
			loadType = 3;
		}		
	}

	// 名稱
	lua_pushstring(L, name);

	// 壓力量
	lua_pushnumber(L, loadType);

	return 2;
}
// ----------------------------------------------------------------------------
void CCharacterSelect::LoadCharacterEvent( int iEvent, int iValue )
{
	lua_State* L = GetLuaState();
	EM_LoadDataEvent emEvent = (EM_LoadDataEvent)iEvent;

	switch( emEvent )
	{
	case EM_LoadDataEvent_WaitingQueue:
		m_waitQueue = true;
		if ( L )
		{
			char temp[512];
			sprintf_s(temp, 512, g_ObjectData->GetString("SYS_LOGIN_WAITINGQUEUE"), iValue);

			lua_pushstring(L, "WAITING_QUEUE");
			lua_setglobal(L, "arg1");
			lua_pushstring(L, temp);
			lua_setglobal(L, "arg2");
			SendLoginEvent("OPEN_LOGIN_DIALOG");
			//char szMsg[256];
			//sprintf( szMsg, "排隊中, 前面還有 [ %d ] 個人", iValue );
			//MessageBox( NULL, szMsg, "Message", MB_OK );
		} break;

	case EM_LoadDataEvent_StartLoad:		// 開始讀取角色資料
		if ( m_waitQueue )
		{
			m_waitQueue = false;
			SendLoginEvent("CLOSE_LOGIN_DIALOG");
		} break;

	case EM_LoadDataEvent_SPWCheckFailed:
		{
			g_pAccountLogin->SetPasswordErrorCount(iValue);
		} break;

	case EM_LoadDataEvent_Lock:
		{
			// iValue = 還要多少秒才能再次登入遊戲
			// 按下確定後關畢閉遊細或回到登入話面
			lua_pushnumber(L, iValue);
			lua_setglobal(L, "arg1");
			SendLoginEvent("LOGIN_LOCK_CHARACTER");
		} break;

	case EM_LoadDataEvent_RoleExist:
		// 斷線
		g_pAccountLogin->Logout(EM_ClientNetCloseType_ReturnAccount);

		// 角色已在遊戲內
		if ( L )
		{
			lua_pushstring(L, "OKAY");
			lua_setglobal(L, "arg1");
			lua_pushstring(L, g_ObjectData->GetString("SYS_LOGIN_ROLEEXIST"));
			lua_setglobal(L, "arg2");
			SendLoginEvent("OPEN_LOGIN_DIALOG");
		} break;
	}

}

// ----------------------------------------------------------------------------
int LuaFunc_SetCharacterSelectFacing(lua_State* L)
{
	CRoleSprite* sprite = g_pCharacterSelect->GetCharacterSprite();	
	if ( sprite && lua_isnumber(L, 1) ) {
		float angle = (float)(lua_tonumber(L, 1) * PI / 180.0f);
		sprite->SetDirection(CRuVector3(cos(angle), 0.0f, -sin(angle)));
	}
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GetCharacterSelectFacing(lua_State* L)
{	
	float theta = 0.0f;
	CRoleSprite* sprite = g_pCharacterSelect->GetCharacterSprite();
	if ( sprite ) {
		CRuVector3 dir = sprite->GetDirection();		

		// 取得角色所朝方向
		theta = acos( dir.Dot( CRuVector3(1.0f, 0.0f, 0.0f) ) );
		if ( dir.m_z > 0.0f )
			theta = PI + PI - theta;

		// 轉換為角度
		theta = theta * 180.0f / PI;
	}
	lua_pushnumber(L, theta);
	return 1;
}