#include "../../netwaker_member/NetWakerClientInc.h"
#include "RoleData/ObjectDataClass.h"
#include "../../mainproc/GameMain.h"
#include "../ItemLink/ItemLink.h"
#include "LoginParent.h"
#include "CharacterSelect.h"
#include "CharacterCreate.h"

#include "AccountLogin.h"

CCharacterCreate*		g_pCharacterCreate = NULL;

int						g_RaceJob[3][10] = { 
											{EM_Vocation_Warrior, EM_Vocation_Ranger, EM_Vocation_Rogue, EM_Vocation_Wizard, EM_Vocation_Priest, EM_Vocation_Knight, EM_Vocation_Warrior, EM_Vocation_Warrior, EM_Vocation_Warrior, EM_Vocation_Warrior},
											{EM_Vocation_Warrior, EM_Vocation_Ranger, EM_Vocation_Rogue, EM_Vocation_Wizard, EM_Vocation_RuneDancer, EM_Vocation_Druid, EM_Vocation_Warrior, EM_Vocation_Warrior, EM_Vocation_Warrior, EM_Vocation_Warrior},
//											{EM_Vocation_Warrior, EM_Vocation_Rogue, EM_Vocation_Wizard, EM_Vocation_Priest, EM_Vocation_Harpsyn, EM_Vocation_Psyron, EM_Vocation_Warrior, EM_Vocation_Warrior, EM_Vocation_Warrior, EM_Vocation_Warrior},
											};

int	ChangeSkinColor(Race_ENUM race, int id);
int	ChangeHiarColor(Race_ENUM race, int id);
int	ChangeFaceID(int id);
int ChangeHairID(int id);
Voc_ENUM ChangeClassEnum(int id);
Sex_ENUM ChangeSexEnum(int id);
Race_ENUM ChangeRaceEnum(int id);

int LuaFunc_CreateCharacter(lua_State *L);
int LuaFunc_GetCharacterCreateClassInfo(lua_State *L);
int LuaFunc_GetCharacterCreateNumSkinColors(lua_State *L);
int LuaFunc_GetCharacterCreateSkinColorInfo(lua_State *L);
int LuaFunc_GetCharacterCreateNumHairColors(lua_State *L);
int LuaFunc_GetCharacterCreateHairColorInfo(lua_State *L);
int LuaFunc_GetCharacterCreateFigureInfo(lua_State* L);
int LuaFunc_GetCharacterCreateNumHairs(lua_State* L);
int LuaFunc_GetCharacterCreateNumFaces(lua_State* L);

int LuaFunc_OpenCharacterCreateModel(lua_State* L);
int LuaFunc_CloseCharacterCreateModel(lua_State* L);
int LuaFunc_RotateCharacterCreateModel(lua_State* L);
int LuaFunc_UpdateCharacterCreateModel(lua_State* L);
int LuaFunc_UpdateCharacterCreateModelBoneScale(lua_State* L);
int LuaFunc_SetCharacterCreateFacing	(lua_State *L);
int LuaFunc_GetCharacterCreateFacing	(lua_State *L);

int LuaFunc_CharacterCreate_SetLookAtFace(lua_State *L);

int LuaFunc_SetCharacterName			(lua_State* L);			// 重新設定角色名稱
int LuaFunc_GetCharacterRace(lua_State* L);
int LuaFunc_GetCharacterSex(lua_State* L);
int LuaFunc_GetCharacterVocation(lua_State* L);
int LuaFunc_GetCharacterFace(lua_State* L);
int LuaFunc_GetCharacterHair(lua_State* L);
int LuaFunc_GetCharacterSkinIndex(lua_State* L);
int LuaFunc_GetCharacterFaceColor(lua_State* L);
int LuaFunc_GetCharacterHairColor(lua_State* L);

int LuaFunc_CheckCharacterCreateRace	(lua_State* L);
int LuaFunc_RandomCharacterCreateHair	(lua_State* L);
int LuaFunc_RandomCharacterCreateFace	(lua_State* L);

int LuaFunc_GetMaxCharacterCreate		(lua_State* L);

int LuaFunc_GetReserveNumCharacters		(lua_State* L);
int LuaFunc_GetReserveCharacterInfo		(lua_State* L);

// --------------------------------------------------------------------------------------
CCharacterCreate::CCharacterCreate(CInterface* object)
	: CInterfaceBase(object)
{
	g_pCharacterCreate = this;	

	m_sprite		= NULL;	
	//m_delta			= 0.0f;	

	m_race			= EM_Race1;
	m_sex			= EM_Sex_Boy;
	m_vocation		= EM_Vocation_Warrior;
	m_face			= 0;
	m_hair			= 0;
	m_skinColor		= 0;
	m_hairColor		= 0;

	m_bLookAtFace = false;

	m_maxCreateRoleCount = 8;

	m_reserveCharacter.clear();
}

// --------------------------------------------------------------------------------------
CCharacterCreate::~CCharacterCreate()
{
	UI_SAFE_DELETE(m_sprite);

	g_pCharacterCreate = NULL;
}

/*
// --------------------------------------------------------------------------------------
void CCharacterCreate::Update(float elapsedTime)
{
	m_delta += elapsedTime * 90.0f;
	if ( m_delta > 360 )
		m_delta = 0.0f;

	if ( m_sprite )
	{
		float angle = m_delta * PI / 180.0f;
		m_sprite->SetDirection(CRuVector3(cos(angle), 0.0f, -sin(angle)));
	}
}
*/

// --------------------------------------------------------------------------------------
void CCharacterCreate::RegisterFunc()
{
	lua_State* L = GetLuaState();
	if (L)
	{
		lua_register(L, "CreateCharacter",						LuaFunc_CreateCharacter);
		lua_register(L, "OpenCharacterCreateModel",				LuaFunc_OpenCharacterCreateModel);
		lua_register(L, "CloseCharacterCreateModel",			LuaFunc_CloseCharacterCreateModel);
		lua_register(L, "RotateCharacterCreateModel",			LuaFunc_RotateCharacterCreateModel);
		lua_register(L, "UpdateCharacterCreateModel",			LuaFunc_UpdateCharacterCreateModel);
		lua_register(L, "UpdateCharacterCreateModelBoneScale",	LuaFunc_UpdateCharacterCreateModelBoneScale);

		lua_register(L, "GetCharacterCreateClassInfo",			LuaFunc_GetCharacterCreateClassInfo);
		lua_register(L, "GetCharacterCreateNumSkinColors",		LuaFunc_GetCharacterCreateNumSkinColors);
		lua_register(L, "GetCharacterCreateSkinColorInfo",		LuaFunc_GetCharacterCreateSkinColorInfo);
		lua_register(L, "GetCharacterCreateNumHairColors",		LuaFunc_GetCharacterCreateNumHairColors);
		lua_register(L, "GetCharacterCreateHairColorInfo",		LuaFunc_GetCharacterCreateHairColorInfo);
		lua_register(L, "GetCharacterCreateFigureInfo",			LuaFunc_GetCharacterCreateFigureInfo);
		lua_register(L, "GetCharacterCreateNumHairs",			LuaFunc_GetCharacterCreateNumHairs);
		lua_register(L, "GetCharacterCreateNumFaces",			LuaFunc_GetCharacterCreateNumFaces);
		lua_register(L, "SetCharacterCreateFacing",				LuaFunc_SetCharacterCreateFacing);
		lua_register(L, "GetCharacterCreateFacing",				LuaFunc_GetCharacterCreateFacing);	

 		lua_register(L, "CharacterCreate_SetLookAtFace",		LuaFunc_CharacterCreate_SetLookAtFace);	

		lua_register(L, "SetCharacterName",						LuaFunc_SetCharacterName);

		lua_register(L, "GetCharacterRace",						LuaFunc_GetCharacterRace);
		lua_register(L, "GetCharacterSex",						LuaFunc_GetCharacterSex);
		lua_register(L, "GetCharacterVocation",					LuaFunc_GetCharacterVocation);
		lua_register(L, "GetCharacterFace",						LuaFunc_GetCharacterFace);
		lua_register(L, "GetCharacterHair",						LuaFunc_GetCharacterHair);
		lua_register(L, "GetCharacterSkinIndex",				LuaFunc_GetCharacterSkinIndex);
		lua_register(L, "GetCharacterFaceColor",				LuaFunc_GetCharacterFaceColor);
		lua_register(L, "GetCharacterHairColor",				LuaFunc_GetCharacterHairColor);

		lua_register(L, "CheckCharacterCreateRace",				LuaFunc_CheckCharacterCreateRace);
		lua_register(L, "RandomCharacterCreateHair",			LuaFunc_RandomCharacterCreateHair);
		lua_register(L, "RandomCharacterCreateFace",			LuaFunc_RandomCharacterCreateFace);

		lua_register(L, "GetMaxCharacterCreate",				LuaFunc_GetMaxCharacterCreate);

		lua_register(L, "GetReserveNumCharacters",				LuaFunc_GetReserveNumCharacters);
		lua_register(L, "GetReserveCharacterInfo",				LuaFunc_GetReserveCharacterInfo);
	}
}

// --------------------------------------------------------------------------------------
void CCharacterCreate::CreateRoleSprite()
{
	g_ruEngineSettings->GetSetting("paperdoll:max mip level", m_paperdollTextureDetail);
	g_ruEngineSettings->GetSetting("texture:max detail level", m_textureDetail);	
	g_ruEngineSettings->SetSetting("paperdoll:max mip level", 0);
	g_ruEngineSettings->SetSetting("texture:max detail level", 4);


	if ( m_sprite == NULL )
	{
		int seed;

		m_sprite = new CRoleSprite();
  		m_sprite->SetDirection( g_pAccountLogin->m_PlayerDirection );
  
		// 種族
		seed = rand() % 2;
		switch (seed)
		{
		case EM_Race1:
			break;

		case EM_Race2:
			if ( g_ObjectData->GetObj(1002) == NULL )
				seed = 0;
			break;

/*		case EM_Race3:
			if ( g_ObjectData->GetObj(1004) == NULL )
				seed = 0;
			break;*/
		}
		m_race = seed;

		// 性別
/*		if ( m_race == EM_Race3 )
			m_sex = 0;
		else
			m_sex = rand() % 2;*/

		// 職業
		seed = g_RaceJob[m_race][(rand() % 6)];

		switch (seed)
		{
		case EM_Vocation_Warrior:				// 戰士
		case EM_Vocation_Ranger:				// 遊俠
		case EM_Vocation_Rogue:					// 盜賊
		case EM_Vocation_Wizard:				// 法師
			break;

		case EM_Vocation_Priest:				// 僧侶
			if (m_race == EM_Race2)
				seed = EM_Vocation_RuneDancer;
			break;

		case EM_Vocation_Knight:				// 騎士
			if (m_race == EM_Race2)
				seed = EM_Vocation_Druid;
			break;

		case EM_Vocation_RuneDancer:			// 言靈使
			if (m_race == EM_Race1)
				seed = EM_Vocation_Priest;
			break;

		case EM_Vocation_Druid:					// 德魯伊
			if (m_race == EM_Race1)
				seed = EM_Vocation_Knight;
			break;

		case EM_Vocation_Harpsyn:				// 靈能者
		case EM_Vocation_Psyron:				// 符文工匠
			break;
		}
		m_vocation = seed;

		vector<int>& face = g_ObjectData->GetCreateRoleFace((Race_ENUM)m_race, (Sex_ENUM)m_sex);
		vector<int>& hair = g_ObjectData->GetCreateRoleHair((Race_ENUM)m_race, (Sex_ENUM)m_sex);
		CreateCharacterColor& skinColor = g_ObjectData->GetCreateSkinColor((Race_ENUM)m_race);
		CreateCharacterColor& hairColor = g_ObjectData->GetCreateHairColor((Race_ENUM)m_race);
		
		m_face			= int(rand() % max(face.size(), 1));
		m_hair			= int(rand() % max(hair.size(), 1));
		m_skinColor		= skinColor.color[rand() % skinColor.numColors].color;
		m_hairColor		= hairColor.color[rand() % hairColor.numColors].color;

		/*
		int r, g, b;
		r = rand() % 255;
		g = rand() % 255;
		b = rand() % 255;
		if ( r < 32 && g < 32 && b < 32 ) {
			r += 32;
		}
		m_hairColor		= 0xFF000000 | (r << 24) | (g << 16) | b;
		*/
	}
}

// --------------------------------------------------------------------------------------
void CCharacterCreate::DeleteRoleSprite()
{
	if ( m_sprite )
	{
		g_ruEngineSettings->SetSetting("paperdoll:max mip level", m_paperdollTextureDetail);
		g_ruEngineSettings->SetSetting("texture:max detail level", m_textureDetail);
	}

	UI_SAFE_DELETE(m_sprite);	
}

// --------------------------------------------------------------------------------------
void CCharacterCreate::UpdateRoleSprite(int race, int sex, int vocation, int face, int hair, int skinColor, int hairColor)
{
	CUiRGBA color;

	// 檢測髮色
	color.m_value = hairColor;
	if ( color.m_r < 60 && color.m_g < 60 && color.m_b < 60 )
	{
		color.m_r = 0x4C;
		color.m_g = 0x4C;
		color.m_b = 0x4C;
		hairColor = color.m_value;
	}

	// 檢查是否可以使用精靈
	switch (race)
	{
	case EM_Race1:
		break;

	case EM_Race2:
		if ( g_ObjectData->GetObj(1002) == NULL || g_ObjectData->GetObj(1003) == NULL )
			race = EM_Race1;
		break;

/*	case EM_Race3:
		if ( g_ObjectData->GetObj(1004) == NULL )
			race = EM_Race1;
		break;*/
	}	

	m_race			= race;
	m_sex			= sex;
	m_vocation		= vocation;
	m_face			= face;
	m_hair			= hair;
	m_skinColor		= skinColor;
	m_hairColor		= hairColor;
	
	int faceObjectId = ChangeFaceID(m_face);
	int hairObjectId = ChangeHairID(m_hair);
	if ( m_sprite )
	{		
		CRuVector3 dir = m_sprite->GetDirection();

		switch ( m_race )
		{
		case EM_Race1:
			if ( m_sex == EM_Sex_Boy )
				m_sprite->SetObjectId(1000, false);
			else
				m_sprite->SetObjectId(1001, false);
			break;

		case EM_Race2:
			if ( m_sex == EM_Sex_Boy )
				m_sprite->SetObjectId(1002, false);
			else
				m_sprite->SetObjectId(1003, false);
			break;

/*		case EM_Race3:
			m_sprite->SetObjectId(1004, false);
			break;*/
		}

		if ( m_sprite->GetRuActor() )
			m_sprite->GetRuActor()->EnableTorsoTurn(false);

		m_sprite->SetPaperdollColor(PAPERDOLL_PART_NAME_HEAD, m_skinColor, 0);
		m_sprite->SetPaperdollColor(PAPERDOLL_PART_NAME_HAIR, m_hairColor, 0);
		m_sprite->SetPaperdollPart(PAPERDOLL_PART_NAME_HEAD, faceObjectId);
		m_sprite->SetPaperdollPart(PAPERDOLL_PART_NAME_HAIR, hairObjectId);

		// 設定職業基本裝備
		int color[2];
		m_sprite->SetPaperdollPart(PAPERDOLL_PART_NAME_HELMET, g_ObjectData->GetCreateBaseEQ((Race_ENUM)race, (Voc_ENUM)vocation, EM_EQWearPos_Head, color));
		m_sprite->SetPaperdollColor(PAPERDOLL_PART_NAME_HELMET, color[0], color[1]);
		m_sprite->SetPaperdollPart(PAPERDOLL_PART_NAME_HAND, g_ObjectData->GetCreateBaseEQ((Race_ENUM)race, (Voc_ENUM)vocation, EM_EQWearPos_Gloves, color));
		m_sprite->SetPaperdollColor(PAPERDOLL_PART_NAME_HAND, color[0], color[1]);
		m_sprite->SetPaperdollPart(PAPERDOLL_PART_NAME_FOOT, g_ObjectData->GetCreateBaseEQ((Race_ENUM)race, (Voc_ENUM)vocation, EM_EQWearPos_Shoes, color));
		m_sprite->SetPaperdollColor(PAPERDOLL_PART_NAME_FOOT, color[0], color[1]);
		m_sprite->SetPaperdollPart(PAPERDOLL_PART_NAME_TORSO, g_ObjectData->GetCreateBaseEQ((Race_ENUM)race, (Voc_ENUM)vocation, EM_EQWearPos_Clothes, color));
		m_sprite->SetPaperdollColor(PAPERDOLL_PART_NAME_TORSO, color[0], color[1]);
		m_sprite->SetPaperdollPart(PAPERDOLL_PART_NAME_LEG, g_ObjectData->GetCreateBaseEQ((Race_ENUM)race, (Voc_ENUM)vocation, EM_EQWearPos_Pants, color));
		m_sprite->SetPaperdollColor(PAPERDOLL_PART_NAME_LEG, color[0], color[1]);
		m_sprite->SetPaperdollPart(PAPERDOLL_PART_NAME_BACK, g_ObjectData->GetCreateBaseEQ((Race_ENUM)race, (Voc_ENUM)vocation, EM_EQWearPos_Back, color));
		m_sprite->SetPaperdollColor(PAPERDOLL_PART_NAME_BACK, color[0], color[1]);
		m_sprite->SetPaperdollPart(PAPERDOLL_PART_NAME_BELT, g_ObjectData->GetCreateBaseEQ((Race_ENUM)race, (Voc_ENUM)vocation, EM_EQWearPos_Belt, color));
		m_sprite->SetPaperdollColor(PAPERDOLL_PART_NAME_BELT, color[0], color[1]);
		m_sprite->SetPaperdollPart(PAPERDOLL_PART_NAME_SHOULDER, g_ObjectData->GetCreateBaseEQ((Race_ENUM)race, (Voc_ENUM)vocation, EM_EQWearPos_Shoulder, color));
		m_sprite->SetPaperdollColor(PAPERDOLL_PART_NAME_SHOULDER, color[0], color[1]);
		m_sprite->SetHandsObjectId(eRoleHands_MainHand, g_ObjectData->GetCreateBaseEQ((Race_ENUM)race, (Voc_ENUM)vocation, EM_EQWearPos_MainHand, color));
		m_sprite->SetHandsObjectId(eRoleHands_OffHand, g_ObjectData->GetCreateBaseEQ((Race_ENUM)race, (Voc_ENUM)vocation, EM_EQWearPos_SecondHand, color));
		m_sprite->SetWeaponPosition(true);
#ifdef KALYDO
		// Delay load the paperdoll so the needed assets for every character can be downloaded.
		m_sprite->PaperdollBuild(true);		
#else
		m_sprite->PaperdollBuild(false);		
#endif // KALYDO

		CRuCamera* camera = g_pGameMain->GetCamera();
		if ( camera )
		{
			//CRuVector3 distOffsetVec = camera->GetCameraPosition() - camera->GetTargetPosition();
			//distOffsetVec.m_y = 0.0f;
			//distOffsetVec.Normalize();

			//m_sprite->SetPos(camera->GetTargetPosition() + CRuVector3(0.0f, -14.25f, 0.0f) + distOffsetVec * 53.0f);
			m_sprite->SetDirection(dir);

			//sCameraLayout CameraLayout;
			//g_pAccountLogin->GetCameraLayout( CameraLayout );


			//CRuVector3 Position( 1585.31, 30, 2995.15 );


			CRuVector3 Position = g_pAccountLogin->GetAccountLoginCameraLayoutEnd().pos;

			//static float temp = 280.0f;
			//Position.m_y = temp;

			//Position.m_y = 266.25455f;
			Position.m_y = g_pAccountLogin->m_fPlayerHeight;

			m_sprite->SetPos( Position );
			//m_sprite->SetDirection( g_pAccountLogin->m_PlayerDirection );

			//m_sprite->AnchorSprite( 20.0f );

			/*
			sCameraLayout CameraLayout;
			g_pAccountLogin->GetCameraLayout( CameraLayout );

			CRuVector3 TargetPosition = CameraLayout.pos;
			TargetPosition.y = 35;
			m_sprite->SetPos( TargetPosition );
			m_sprite->AnchorSprite();

			CameraLayout.pos = m_sprite->GetPos();
			CameraLayout.pos.y += 10.0f;

			CameraLayout.time = 1.0f;
			g_pAccountLogin->AddControlCamera( CameraLayout );
			*/

			/*
			m_sprite->SetDirection(camera->GetCameraPosition() - camera->GetTargetPosition());
			m_sprite->SetPos(camera->GetTargetPosition() + CRuVector3(0.0f, -9.0f, 0.0f));
			m_sprite->SetDirection(camera->GetCameraPosition() - camera->GetTargetPosition());
			*/
		}
	}
}


// --------------------------------------------------------------------------------------
void CCharacterCreate::UpdateRoleSpriteBoneScale(float* boneScale)
{
	if ( boneScale )
	{
		m_headScale		= boneScale[0];
		m_breastsScale	= boneScale[1];
		m_bodyScale		= boneScale[2];
		m_chestScale	= boneScale[3];
		m_pelvisScale	= boneScale[4];
		m_armScale		= boneScale[5];
		m_forearmScale	= boneScale[6];
		m_handScale		= boneScale[7];
		m_legScale		= boneScale[8];
		m_calfScale		= boneScale[9];
		m_footScale		= boneScale[10];

		if ( m_sprite )
		{
			m_sprite->SetBoneScale(BONE_SCALE_NAME_HEAD,		m_headScale);
			m_sprite->SetBoneScale(BONE_SCALE_NAME_BREASTS,		m_breastsScale);
			m_sprite->SetBoneScale(BONE_SCALE_NAME_BODY,		m_bodyScale);
			m_sprite->SetBoneScale(BONE_SCALE_NAME_CHEST,		m_chestScale);
			m_sprite->SetBoneScale(BONE_SCALE_NAME_PELVIS,		m_pelvisScale);
			m_sprite->SetBoneScale(BONE_SCALE_NAME_ARM,			m_armScale);
			m_sprite->SetBoneScale(BONE_SCALE_NAME_FOREARM,		m_forearmScale);
			m_sprite->SetBoneScale(BONE_SCALE_NAME_HAND,		m_handScale);
			m_sprite->SetBoneScale(BONE_SCALE_NAME_LEGS,		m_legScale);
			m_sprite->SetBoneScale(BONE_SCALE_NAME_CALF,		m_calfScale);
			m_sprite->SetBoneScale(BONE_SCALE_NAME_FOOT,		m_footScale);
			m_sprite->Update( 0.1f );
		}
	}

	SetLookAtFace( m_bLookAtFace , true );
}

// --------------------------------------------------------------------------------------
eCREATE_CHARACTER_RESULT CCharacterCreate::CreateCharacter(const char* name)
{
	lua_State* L = GetLuaState();
	
	/*
	wstring outString;
	g_ObjectData->CheckCharacterNameRules(name, EM_LanguageType_JP, outString);
		return CREATE_CHARACTER_ERR_WRONGFUL;
	*/	

	CreateRoleData role;
	role.SampleRole.init();
	role.SampleRole.RoleName		= name;
	role.SampleRole.Race			= (Race_ENUM)m_race;
	role.SampleRole.Sex				= (Sex_ENUM)m_sex;
	role.SampleRole.Voc				= (Voc_ENUM)m_vocation;
	role.SampleRole.BoneScale[0]	= (BYTE) (m_headScale * 100.0f);
	role.SampleRole.BoneScale[1]	= (BYTE) (m_breastsScale * 100.0f);
	role.SampleRole.BoneScale[2]	= (BYTE) (m_bodyScale * 100.0f);
	role.SampleRole.BoneScale[3]	= (BYTE) (m_chestScale * 100.0f);
	role.SampleRole.BoneScale[4]	= (BYTE) (m_pelvisScale * 100.0f);
	role.SampleRole.BoneScale[5]	= (BYTE) (m_armScale * 100.0f);
	role.SampleRole.BoneScale[6]	= (BYTE) (m_forearmScale * 100.0f);
	role.SampleRole.BoneScale[7]	= (BYTE) (m_handScale * 100.0f);
	role.SampleRole.BoneScale[8]	= (BYTE) (m_legScale * 100.0f);
	role.SampleRole.BoneScale[9]	= (BYTE) (m_calfScale * 100.0f);
	role.SampleRole.BoneScale[10]	= (BYTE) (m_footScale * 100.0f);
	role.SampleRole.iFaceID			= ChangeFaceID(m_face);
	role.SampleRole.iHairID			= ChangeHairID(m_hair);
	role.SampleRole.iBodyColor		= m_skinColor;
	role.SampleRole.iHairColor		= m_hairColor;

	switch ( role.SampleRole.Race )
	{
	case EM_Race1:
		if ( role.SampleRole.Sex == EM_Sex_Boy )
			role.SampleRole.OrgObjID = 1000;
		else
			role.SampleRole.OrgObjID = 1001;
		break;

	case EM_Race2:
		if ( role.SampleRole.Sex == EM_Sex_Boy )
			role.SampleRole.OrgObjID = 1002;
		else
			role.SampleRole.OrgObjID = 1003;
		break;

/*	case EM_Race3:
		role.SampleRole.OrgObjID = 1004;
		break;*/
	}

	CNetCli_Login::CreateRole(&role);
	return CREATE_CHARACTER_SUCCESS;
}

// --------------------------------------------------------------------------------------
void CCharacterCreate::CreateResult(int emResult, int itemID[5], int itemCount[5])
{
	char tmp[1024];
	lua_State* L = GetLuaState();

#ifdef KALYDO
	static bool first = true;
#endif //KALYDO

	switch (emResult)
	{
	case EM_CreateRoleResult_OK:

		// 移除保留角色資料
		{
			vector<ReserverRoleStruct>::iterator iter = m_reserveCharacter.begin();
			while ( iter != m_reserveCharacter.end() )
			{
				bool isRemove = false;

				for ( int i = 0; i < CNetCli_Login::GetNumCharacters(); i++ )
				{
					SampleRoleData* roleData = CNetCli_Login::GetCharacterInfo(i + 1);
					if ( roleData )
					{
						if ( stricmp(roleData->RoleName.Begin(), iter->RoleName.Begin()) == 0 )
						{
							isRemove = true;
							iter = m_reserveCharacter.erase(iter);
						}
					}
				}

				if ( !isRemove )
					iter++;
			}

			// 獲得物品
			GameObjDbStructEx* itemDB[5];

			bool hasItem = false;
			for ( int i = 0; i < 5; i++ )
			{
				if ( (itemDB[i] = g_ObjectData->GetObj( itemID[i] )) != NULL )
					hasItem = true;
			}

			if ( hasItem )
			{
				char temp[4096];

				sprintf_s(temp, 4096, "%s ", g_ObjectData->GetString("SYS_GIVE_TEXT"));
				for ( int i = 0; i < 5; i++ )
				{
					if ( itemDB[i] )
					{
						string strLink;
						ItemGUID_To_ItemLink(itemDB[i]->GUID, strLink);

						strcat(temp, strLink.c_str());
					}
				}

				lua_pushstring(L, temp);
				lua_setglobal(L, "arg1");
			}
			else
			{
				lua_pushnil(L);
				lua_setglobal(L, "arg1");
			}
		}

		g_pLoginParent->SendLoginEvent("CHARACTER_CREATE_SUCCESS");
		g_pCharacterSelect->SetSelected(CNetCli_Login::GetNumCharacters());
		g_pCharacterSelect->UpdateCharacterList();

#ifdef KALYDO
		// Wallpost example: We've created a character; show it on e.g. Facebook!
		{
			std::string message("FBNAME has created a character named ");
			SampleRoleData* pRoleData = CNetCli_Login::GetCharacterInfo(CNetCli_Login::GetNumCharacters());
			message += pRoleData->RoleName.Begin();
			message += " in the awesome MMO game Runes of Magic";

			application->doWallPost("FBNAME started playing Runes of Magic", message.c_str(), "Welcome to the mystical world of Runes of Magic! LINK", "CharCreated");
		}

		// also, send a stat of successful character creation
		if(first)
		{
			TAppStatisticsPair statPair[1];
			memset(statPair, 0, sizeof(statPair));
			strcpy(statPair[0].name, "CharCreated");
			strcpy(statPair[0].value, "1");
			application->sendAppStatistics(statPair, 1, NULL);
			first = false;
		}
#endif // KALYDO
		return;

	case EM_CreateRoleResult_RoleNameExist:
		strcpy(tmp, g_ObjectData->GetString("SYS_CREATEROLE_ROLE_EXIST"));
		break;

	case EM_CreateRoleResult_SystemError:
		strcpy(tmp, g_ObjectData->GetString("SYS_CREATEROLE_UNKNOW"));
		break;

	case EM_CreateRoleResult_NameToShort:			// 名稱太短,不合法
		strcpy(tmp, g_ObjectData->GetString("SYS_CREATEROLE_ERROR_SHORT"));
		break;

	case EM_CreateRoleResult_NameToLong:			// 名稱太長,不合法
		strcpy(tmp, g_ObjectData->GetString("SYS_CREATEROLE_ERROR_LONG"));
		break;

	case EM_CreateRoleResult_NameIsWrongful:		// 有錯誤的符號或字元
		strcpy(tmp, g_ObjectData->GetString("SYS_CREATEROLE_ERROR_WRONGFUL"));
		break;

	case EM_CreateRoleResult_HasSpecialString:		// 特殊名稱不能用
		strcpy(tmp, g_ObjectData->GetString("SYS_CREATEROLE_ERROR_STRING"));
		break;

	case EM_CreateRoleResult_OnlyBasicLetters:		// 只能使用基本英文字母
		strcpy(tmp, g_ObjectData->GetString("SYS_CREATEROLE_ONLYBASICLETTERS"));
		break;

	case EM_CreateRoleResult_EndLetterError:		// 結尾字元錯誤(',")
		strcpy(tmp, g_ObjectData->GetString("SYS_CREATEROLE_ENDLETTERERROR"));
		break;

	case EM_CreateRoleResult_NewHandDisableCreateRole:		// 禁止創立新角
		strcpy(tmp, g_ObjectData->GetString("SYS_CREATEROLE_DISABLE_CREATE"));
		break;		

	default:
		sprintf(tmp, "Create role failed! reult_code(%d)", emResult);
		break;
	}
	g_pLoginParent->SendLoginEvent("CHARACTER_CREATE_FAILED");

	if ( L )
	{
		lua_pushstring(L, "OKAY");
		lua_setglobal(L, "arg1");
		lua_pushstring(L, tmp);
		lua_setglobal(L, "arg2");
		SendLoginEvent("OPEN_LOGIN_DIALOG");
	}
}

// --------------------------------------------------------------------------------------
void CCharacterCreate::RenameResult(int emResult)
{	
	char tmp[MAX_PATH];	

	switch (emResult)
	{
	case EM_CreateRoleResult_OK:
		g_pCharacterSelect->SendLoginEvent("LOGINRENAME_SUCCESS");
		g_pCharacterSelect->UpdateCharacterList();
		return;

	case EM_CreateRoleResult_RoleNameExist:
		strcpy(tmp, g_ObjectData->GetString("SYS_CREATEROLE_ROLE_EXIST"));
		break;

	case EM_CreateRoleResult_SystemError:
		strcpy(tmp, g_ObjectData->GetString("SYS_CREATEROLE_UNKNOW"));
		break;

	default:
		sprintf(tmp, "Create role failed! reult_code(%d)", emResult);
		break;
	}
	g_pLoginParent->SendLoginEvent("LOGINRENAME_FAILED");

	lua_State* L = GetLuaState();
	if ( L )
	{
		lua_pushstring(L, "OKAY");
		lua_setglobal(L, "arg1");
		lua_pushstring(L, tmp);
		lua_setglobal(L, "arg2");
		SendLoginEvent("OPEN_LOGIN_DIALOG");
	}
}

// --------------------------------------------------------------------------------------
eCREATE_CHARACTER_RESULT CCharacterCreate::CheckRightfulName(const char* name)
{
	return CREATE_CHARACTER_SUCCESS;
}

// --------------------------------------------------------------------------------------
void CCharacterCreate::SetLookAtFace( bool bLookAtFace , bool bUpdateBoneScale )
{
	m_bLookAtFace = bLookAtFace;
	if( m_bLookAtFace )
	{
		sCameraLayout CameraLayoutEnd = g_pAccountLogin->GetAccountLoginCameraLayoutEnd();

		CRoleSprite* sprite = GetCharacterSprite();
		CRuVector3 posTop;
		CRuVector3 posDown;
		sprite->GetAttachPos( eAttach_Name_Title , posTop  );
		sprite->GetAttachPos( eAttach_Down       , posDown );

		posTop = ( posTop - posDown ) * 0.75 + posDown;

		sCameraLayout CameraLayout;
		CameraLayout.pos = posTop;
		CameraLayout.pitch = 0.0;
		CameraLayout.yaw = CameraLayoutEnd.yaw;
		CameraLayout.roll = 0.0;
		CameraLayout.zoom = 8;

		if( bUpdateBoneScale )
			CameraLayout.time = 0.1f;
		else
			CameraLayout.time = 0.6f;

		g_pAccountLogin->CleanControlCamera();
		g_pAccountLogin->AddControlCamera( CameraLayout );

	}
	else
	{
		sCameraLayout CameraLayout = g_pAccountLogin->GetAccountLoginCameraLayoutEnd();
		CameraLayout.time = 0.5;

		g_pAccountLogin->CleanControlCamera();
		g_pAccountLogin->AddControlCamera( CameraLayout );
	}
}

// --------------------------------------------------------------------------------------
int	CCharacterCreate::GetSkinColorIndex()
{
	CreateCharacterColor& skinColor = g_ObjectData->GetCreateSkinColor((Race_ENUM)m_race);
	for ( int i = 0; i < skinColor.numColors; i++ )
	{
		int r = (m_skinColor & 0x00FF0000) >> 16;
		int g = (m_skinColor & 0x0000FF00) >> 8;
		int b = (m_skinColor & 0x000000FF);

		if ( skinColor.color[i].r == r && skinColor.color[i].g == g && skinColor.color[i].b == b )
			return i;
	}

	/*
	CreateCharacterColor& skinColor = g_ObjectData->GetMainColor();
	for ( int i = 0; i < skinColor.numColors; i++ )
	{
		if ( skinColor.color[i].color == m_skinColor )
			return i;
	}
	*/
	return 0;
}

// --------------------------------------------------------------------------------------
void CCharacterCreate::RandomHair()
{
	vector<int>& hair = g_ObjectData->GetCreateRoleHair((Race_ENUM)m_race, (Sex_ENUM)m_sex);
	m_hair = int(rand() % max(hair.size(), 1));	
}

// --------------------------------------------------------------------------------------
void CCharacterCreate::RandomFace()
{
	vector<int>& face = g_ObjectData->GetCreateRoleFace((Race_ENUM)m_race, (Sex_ENUM)m_sex);
	m_face = int(rand() % max(face.size(), 1));
}

// --------------------------------------------------------------------------------------
void CCharacterCreate::ClearReserveRole()
{
	m_reserveCharacter.clear();
}

// --------------------------------------------------------------------------------------
void CCharacterCreate::R_ReserveRole(ReserverRoleStruct& info)
{
	m_reserveCharacter.push_back(info);
}

// --------------------------------------------------------------------------------------
int ChangeSkinColor(Race_ENUM race, int id)
{
	CreateCharacterColor& skinColor = g_ObjectData->GetCreateSkinColor(race);
	CUiRGBA color;

	if ( id >= 0 && id < skinColor.numColors )
	{
		color.m_a = 255;
		color.m_r = skinColor.color[id].r;
		color.m_g = skinColor.color[id].g;
		color.m_b = skinColor.color[id].b;
	}
	return color.m_value;
}

// --------------------------------------------------------------------------------------
int ChangeHiarColor(Race_ENUM race, int id)
{
	CreateCharacterColor& hairColor = g_ObjectData->GetCreateHairColor(race);
	CUiRGBA color;

	if ( id >= 0 && id < hairColor.numColors )
	{
		color.m_a = 255;
		color.m_r = hairColor.color[id].r;
		color.m_g = hairColor.color[id].g;
		color.m_b = hairColor.color[id].b;
	}
	return color.m_value;
}

// --------------------------------------------------------------------------------------
int ChangeFaceID(int id)
{
	Race_ENUM race = (Race_ENUM)g_pCharacterCreate->GetRace();
	Sex_ENUM sex = (Sex_ENUM)g_pCharacterCreate->GetSex();

	vector<int>& face = g_ObjectData->GetCreateRoleFace(race, sex);
	if ( id >= 0 && id < (int)face.size() )
		return face[id];
	return 0;
}

// --------------------------------------------------------------------------------------
int ChangeHairID(int id)
{
	Race_ENUM race = (Race_ENUM)g_pCharacterCreate->GetRace();
	Sex_ENUM sex = (Sex_ENUM)g_pCharacterCreate->GetSex();

	vector<int>& hair = g_ObjectData->GetCreateRoleHair(race, sex);
	if ( id >= 0 && id < (int)hair.size() )
		return hair[id];
	return 0;
}

// --------------------------------------------------------------------------------------
Voc_ENUM ChangeClassEnum(int id)
{
	switch (id + 1)
	{
	case EM_Vocation_Warrior:		// 戰士
		return EM_Vocation_Warrior;
	case EM_Vocation_Ranger:		// 遊俠
		return EM_Vocation_Ranger;
	case EM_Vocation_Rogue:			// 盜賊
		return EM_Vocation_Rogue;
	case EM_Vocation_Wizard:		// 法師
		return EM_Vocation_Wizard;
	case EM_Vocation_Priest:		// 僧侶
		return EM_Vocation_Priest;
	case EM_Vocation_Knight:		// 騎士
		return EM_Vocation_Knight;
	case EM_Vocation_RuneDancer:	// 言靈使
		return EM_Vocation_RuneDancer;
	case EM_Vocation_Druid:			// 德魯伊
		return EM_Vocation_Druid;
	case EM_Vocation_Harpsyn:		// 靈能者
		return EM_Vocation_Harpsyn;
	case EM_Vocation_Psyron:		// 符文工匠
		return EM_Vocation_Psyron;
	}
	return EM_Vocation_None;

	/*
	CreateRoleInfoTableStruct* createRoleTable = g_ObjectData->CreateRoleTable();
	if ( id >= 0 && id < createRoleTable->JobCount )
		return (Voc_ENUM)createRoleTable->JobID[id];
	return EM_Vocation_None;
	*/
}

// --------------------------------------------------------------------------------------
Sex_ENUM ChangeSexEnum(int id)
{
	switch (id)
	{
	case 1:
		return EM_Sex_Boy;

	case 2:
		return EM_Sex_Girl;
	}
	return EM_Sex_BigMonster;
}

// --------------------------------------------------------------------------------------
Race_ENUM ChangeRaceEnum(int id)
{
	switch (id)
	{
	case 1:
		return EM_Race1;

	case 2:
		return EM_Race2;

//	case 3:
//		return EM_Race3;
	}
	return EM_Race1;
}

// --------------------------------------------------------------------------------------
int LuaFunc_CreateCharacter(lua_State *L)
{
	lua_pushnumber(L, g_pCharacterCreate->CreateCharacter(lua_tostring(L, 1)));
	return 1;
}

// --------------------------------------------------------------------------------------
int LuaFunc_OpenCharacterCreateModel(lua_State* L)
{
	g_pCharacterCreate->CreateRoleSprite();
	return 0;
}

// --------------------------------------------------------------------------------------
int LuaFunc_CloseCharacterCreateModel(lua_State* L)
{
	g_pCharacterCreate->DeleteRoleSprite();
	return 0;
}

// --------------------------------------------------------------------------------------
int LuaFunc_RotateCharacterCreateModel(lua_State* L)
{
	const char* str = luaL_checkstring(L, 1);
	CRoleSprite* sprite = g_pCharacterCreate->GetRoleSprite();
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

// --------------------------------------------------------------------------------------
int LuaFunc_UpdateCharacterCreateModel(lua_State* L)
{	
	int race		= ChangeRaceEnum(luaL_checkint(L, 1));
	int sex			= ChangeSexEnum(luaL_checkint(L, 2));
	int vocation	= ChangeClassEnum(luaL_checkint(L, 3) - 1);	
	int face		= luaL_checkint(L, 4) - 1;
	int hair		= luaL_checkint(L, 5) - 1;
	int skinColor	= ChangeSkinColor((Race_ENUM)race, luaL_checkint(L, 6) - 1);
	float hairR		= (float)luaL_checknumber(L, 7);
	float hairG		= (float)luaL_checknumber(L, 8);
	float hairB		= (float)luaL_checknumber(L, 9);
	int hairColor	= 0xFF000000 | ((int)(hairR * 255)) << 16 | ((int)(hairG * 255)) << 8 | (int)(hairB * 255);

	vector<int>& faceVector = g_ObjectData->GetCreateRoleFace((Race_ENUM)race, (Sex_ENUM)sex);
	vector<int>& hairVector = g_ObjectData->GetCreateRoleHair((Race_ENUM)race, (Sex_ENUM)sex);
	if ( face < 0 || face >= faceVector.size() )
		face = 0;
	if ( hair < 0 || hair >= hairVector.size() )
		hair = 0;

	//vocation = g_RaceJob[race][vocation-1];
	
	switch (race)
	{
	case EM_Race1:
		switch ( vocation )
		{
		case EM_Vocation_RuneDancer:
		case EM_Vocation_Harpsyn:
			vocation = EM_Vocation_Priest;
			break;

		case EM_Vocation_Druid:
		case EM_Vocation_Psyron:
			vocation = EM_Vocation_Knight;
			break;
		}
		break;

	case EM_Race2:
		switch ( vocation )
		{
		case EM_Vocation_Priest:
		case EM_Vocation_Harpsyn:
			vocation = EM_Vocation_RuneDancer;
			break;

			case EM_Vocation_Psyron:
		case EM_Vocation_Knight:
			vocation = EM_Vocation_Druid;
			break;
		}
		break;

/*	case EM_Race3:
		switch ( vocation )
		{
		case EM_Vocation_Ranger:
			vocation = EM_Vocation_Rogue;
			break;
		case EM_Vocation_Knight:		
			vocation = EM_Vocation_Harpsyn;
			break;
		case EM_Vocation_Druid:
		case EM_Vocation_RuneDancer:
			vocation = EM_Vocation_Wizard;
			break;
		}
		break;*/
	}

	g_pCharacterCreate->UpdateRoleSprite(race, sex, vocation, face, hair, skinColor, hairColor);
	return 0;
}

// --------------------------------------------------------------------------------------
int LuaFunc_UpdateCharacterCreateModelBoneScale(lua_State* L)
{
	float boneScale[15] = { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f };
	int num = min(lua_gettop(L), 15);
	for ( int i = 0; i < num; i++ )
	{
		if ( lua_isnumber(L, i+1) )
			boneScale[i] = (float)lua_tonumber(L, i+1);
	}

	g_pCharacterCreate->UpdateRoleSpriteBoneScale(boneScale);
	return 0;
}

// --------------------------------------------------------------------------------------
int LuaFunc_GetCharacterCreateClassInfo(lua_State *L)
{
	/*
	Race_ENUM race = ChangeRaceEnum(luaL_checkint(L, 1));
	*/

	Race_ENUM race = (Race_ENUM)g_pCharacterCreate->GetRace();
	switch (race)
	{
	case EM_Race1:
		lua_pushnumber(L, EM_Vocation_Warrior);
		lua_pushstring(L, g_ObjectData->GetString("SYS_CLASSNAME_WARRIOR"));
		lua_pushnumber(L, EM_Vocation_Ranger);
		lua_pushstring(L, g_ObjectData->GetString("SYS_CLASSNAME_RANGER"));
		lua_pushnumber(L, EM_Vocation_Rogue);
		lua_pushstring(L, g_ObjectData->GetString("SYS_CLASSNAME_THIEF"));
		lua_pushnumber(L, EM_Vocation_Wizard);
		lua_pushstring(L, g_ObjectData->GetString("SYS_CLASSNAME_MAGE"));
		lua_pushnumber(L, EM_Vocation_Priest);
		lua_pushstring(L, g_ObjectData->GetString("SYS_CLASSNAME_AUGUR"));
		lua_pushnumber(L, EM_Vocation_Knight);
		lua_pushstring(L, g_ObjectData->GetString("SYS_CLASSNAME_KNIGHT"));
		return 12;

	case EM_Race2:
		lua_pushnumber(L, EM_Vocation_Warrior);
		lua_pushstring(L, g_ObjectData->GetString("SYS_CLASSNAME_WARRIOR"));
		lua_pushnumber(L, EM_Vocation_Ranger);
		lua_pushstring(L, g_ObjectData->GetString("SYS_CLASSNAME_RANGER"));
		lua_pushnumber(L, EM_Vocation_Rogue);
		lua_pushstring(L, g_ObjectData->GetString("SYS_CLASSNAME_THIEF"));
		lua_pushnumber(L, EM_Vocation_Wizard);
		lua_pushstring(L, g_ObjectData->GetString("SYS_CLASSNAME_MAGE"));
		lua_pushnumber(L, EM_Vocation_RuneDancer);
		lua_pushstring(L, g_ObjectData->GetString("SYS_CLASSNAME_WARDEN"));
		lua_pushnumber(L, EM_Vocation_Druid);
		lua_pushstring(L, g_ObjectData->GetString("SYS_CLASSNAME_DRUID"));
		return 12;

		/*	case EM_Race3:
		lua_pushnumber(L, EM_Vocation_Warrior);
		lua_pushstring(L, g_ObjectData->GetString("SYS_CLASSNAME_WARRIOR"));		
		lua_pushnumber(L, EM_Vocation_Rogue);
		lua_pushstring(L, g_ObjectData->GetString("SYS_CLASSNAME_THIEF"));
		lua_pushnumber(L, EM_Vocation_Wizard);
		lua_pushstring(L, g_ObjectData->GetString("SYS_CLASSNAME_MAGE"));
		lua_pushnumber(L, EM_Vocation_Priest);
		lua_pushstring(L, g_ObjectData->GetString("SYS_CLASSNAME_AUGUR"));		
		lua_pushnumber(L, EM_Vocation_Harpsyn);
		lua_pushstring(L, g_ObjectData->GetString("SYS_CLASSNAME_HARPSYN"));
		lua_pushnumber(L, EM_Vocation_Psyron);
		lua_pushstring(L, g_ObjectData->GetString("SYS_CLASSNAME_PSYRON"));
		return 12; */
	}
	return 0;
	
	/*
	CreateRoleInfoTableStruct* createRoleTable = g_ObjectData->CreateRoleTable();
	for ( int i = 0; i < createRoleTable->JobCount; i++ )
	{
		char tmp[256];
		sprintf(tmp, "SYS_CLASSNAME_%s", createRoleTable->JobKeyStr[i]);
		lua_pushstring(L, g_ObjectData->GetString(tmp));
	}
	return createRoleTable->JobCount;
	*/
}

// --------------------------------------------------------------------------------------
int LuaFunc_GetCharacterCreateNumSkinColors(lua_State *L)
{
	Race_ENUM race = (Race_ENUM)g_pCharacterCreate->GetRace();
	CreateCharacterColor& skinColor = g_ObjectData->GetCreateSkinColor(race);
	lua_pushnumber(L, skinColor.numColors);
	return 1;
}

// --------------------------------------------------------------------------------------
int LuaFunc_GetCharacterCreateSkinColorInfo(lua_State *L)
{	
	Race_ENUM race = (Race_ENUM)g_pCharacterCreate->GetRace();
	CreateCharacterColor& skinColor = g_ObjectData->GetCreateSkinColor(race);
	unsigned int index = (unsigned int)luaL_checknumber(L, 1) - 1;
	if ( index < (unsigned int)skinColor.numColors )
	{
		lua_pushnumber(L, skinColor.color[index].r / 255.0f);
		lua_pushnumber(L, skinColor.color[index].g / 255.0f);
		lua_pushnumber(L, skinColor.color[index].b / 255.0f);
		return 3;
	}
	return 0;
}

// --------------------------------------------------------------------------------------
int LuaFunc_GetCharacterCreateNumHairColors(lua_State *L)
{
	Race_ENUM race = (Race_ENUM)g_pCharacterCreate->GetRace();
	CreateCharacterColor& hairColor = g_ObjectData->GetCreateHairColor(race);
	lua_pushnumber(L, hairColor.numColors);
	return 1;
}

// --------------------------------------------------------------------------------------
int LuaFunc_GetCharacterCreateHairColorInfo(lua_State *L)
{	
	Race_ENUM race = (Race_ENUM)g_pCharacterCreate->GetRace();
	CreateCharacterColor& hairColor = g_ObjectData->GetCreateHairColor(race);
	unsigned int index = (unsigned int)luaL_checknumber(L, 1) - 1;
	if ( index < (unsigned int)hairColor.numColors )
	{
		lua_pushnumber(L, hairColor.color[index].r / 255.0f);
		lua_pushnumber(L, hairColor.color[index].g / 255.0f);
		lua_pushnumber(L, hairColor.color[index].b / 255.0f);
		return 3;
	}
	return 0;
}

// --------------------------------------------------------------------------------------
int LuaFunc_GetCharacterCreateFigureInfo(lua_State* L)
{
	const char* boneName = luaL_checkstring(L, 1);
	//float minValue = 0.25f;
	//float maxValue = 3.0f;
	//float defValue = 1.00;

	
	float minValue = 0.8f;
	float maxValue = 1.2f;
	float defValue = 1.00;
	if ( strcmp(boneName, "Head") == 0 ) {
		minValue = 0.85f;
		maxValue = 1.2f;
	} else if ( strcmp(boneName, "Breasts") == 0 ) {
		minValue = 0.8f;
		maxValue = 1.2f;
	} else if ( strcmp(boneName, "Stature") == 0 ) {
		minValue = 0.75f;
		maxValue = 1.3f;
	} else if ( strcmp(boneName, "Chest") == 0 ) {
		minValue = 0.85f;
		maxValue = 1.1f;
	} else if ( strcmp(boneName, "Figure") == 0 ) {
		minValue = 0.80f;
		maxValue = 1.2f;
	} else if ( strcmp(boneName, "Arm") == 0 ) {
		minValue = 0.80f;
		maxValue = 1.5f;
	} else if ( strcmp(boneName, "Forearm") == 0 ) {
		minValue = 0.80f;
		maxValue = 1.30f;
	} else if ( strcmp(boneName, "Hand") == 0 ) {
		minValue = 0.7f;
		maxValue = 1.3f;
	} else if ( strcmp(boneName, "Leg") == 0 ) {
		minValue = 0.7f;
		maxValue = 1.2f;
	} else if ( strcmp(boneName, "Calf") == 0 ) {
		minValue = 0.8f;
		maxValue = 1.4f;
	} else if ( strcmp(boneName, "Foot") == 0 ) {
		minValue = 0.7f;
		maxValue = 1.3f;
	}

	lua_pushnumber(L, minValue);
	lua_pushnumber(L, maxValue);	
	lua_pushnumber(L, defValue);
	return 3;
}

// --------------------------------------------------------------------------------------
int LuaFunc_GetCharacterCreateNumHairs(lua_State* L)
{
	Race_ENUM race = ChangeRaceEnum(luaL_checkint(L, 1));
	Sex_ENUM sex = ChangeSexEnum(luaL_checkint(L, 2));
	vector<int>& hair = g_ObjectData->GetCreateRoleHair(race, sex);
	lua_pushnumber(L, (lua_Number)hair.size());
	return 1;	
}

// --------------------------------------------------------------------------------------
int LuaFunc_GetCharacterCreateNumFaces(lua_State* L)
{
	Race_ENUM race = ChangeRaceEnum(luaL_checkint(L, 1));
	Sex_ENUM sex = ChangeSexEnum(luaL_checkint(L, 2));
	vector<int>& face = g_ObjectData->GetCreateRoleFace(race, sex);
	lua_pushnumber(L, (lua_Number)face.size());
	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_SetCharacterCreateFacing(lua_State* L)
{
	CRoleSprite* sprite = g_pCharacterCreate->GetCharacterSprite();
	if ( sprite && lua_isnumber(L, 1) ) {
		float angle = (float)(lua_tonumber(L, 1) * PI / 180.0f);
		sprite->SetDirection(CRuVector3(cos(angle), 0.0f, -sin(angle)));
	}
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GetCharacterCreateFacing(lua_State* L)
{	
	float theta = 0.0f;
	CRoleSprite* sprite = g_pCharacterCreate->GetCharacterSprite();
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

// ----------------------------------------------------------------------------
int LuaFunc_CharacterCreate_SetLookAtFace(lua_State *L)
{
	bool bLookAtFace = lua_toboolean(L,1) == 1 ? true : false;

	g_pCharacterCreate->SetLookAtFace(bLookAtFace);
 
	return 0;
}

// ----------------------------------------------------------------------------
// 重新設定角色名稱
int LuaFunc_SetCharacterName(lua_State* L)
{
	SampleRoleData* roleData = CNetCli_Login::GetCharacterInfo(g_pCharacterSelect->GetSelected());

	// 角色是否存在
	if ( roleData == NULL )
	{
		lua_pushnumber(L, -10);			// 需要選取角色
		return 1;
	}

	// 檢查是否有更名旗標
	if ( roleData->SysFlag.ChangeNameEnabled == false )
	{
		lua_pushnumber(L, -11);			// 沒有權力更名
		return 1;
	}

	// 檢查名稱正確性
	const char* newName = luaL_checkstring(L, 1);
	eCREATE_CHARACTER_RESULT resultEnum = g_pCharacterCreate->CheckRightfulName(newName);
	switch (resultEnum)
	{
	case CREATE_CHARACTER_SUCCESS:
		CNetCli_Login::SM_ChangeNameRequest(roleData->DBID, newName);
		break;
	case CREATE_CHARACTER_ERR_SHORT:
		break;
	case CREATE_CHARACTER_ERR_LONG:
		break;
	case CREATE_CHARACTER_ERR_WRONGFUL:
		break;
	case CREATE_CHARACTER_ERR_TOKEN:
		break;
	}
	lua_pushnumber(L, resultEnum);
	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_GetCharacterRace(lua_State* L)
{
	lua_pushnumber(L, g_pCharacterCreate->GetRace() + 1);
	return 1;
}
// ----------------------------------------------------------------------------
int LuaFunc_GetCharacterSex(lua_State* L)
{
	lua_pushnumber(L, g_pCharacterCreate->GetSex() + 1);
	return 1;
}
// ----------------------------------------------------------------------------
int LuaFunc_GetCharacterVocation(lua_State* L)
{
	lua_pushnumber(L, g_pCharacterCreate->GetVocation());
	return 1;
}
// ----------------------------------------------------------------------------
int LuaFunc_GetCharacterFace(lua_State* L)
{
	lua_pushnumber(L, g_pCharacterCreate->GetFace() + 1);
	return 1;
}
// ----------------------------------------------------------------------------
int LuaFunc_GetCharacterHair(lua_State* L)
{
	lua_pushnumber(L, g_pCharacterCreate->GetHair() + 1);
	return 1;
}
// ----------------------------------------------------------------------------
int LuaFunc_GetCharacterSkinIndex(lua_State* L)
{
	lua_pushnumber(L, g_pCharacterCreate->GetSkinColorIndex() + 1);
	return 1;
}
// ----------------------------------------------------------------------------
int LuaFunc_GetCharacterFaceColor(lua_State* L)
{
	int color = g_pCharacterCreate->GetSkinColor();
	lua_pushnumber(L, ((color & 0xFF0000) >> 16) / 255.0f);
	lua_pushnumber(L, ((color & 0xFF00) >> 8) / 255.0f);
	lua_pushnumber(L, (color & 0xFF) / 255.0f);
	return 3;
}
// ----------------------------------------------------------------------------
int LuaFunc_GetCharacterHairColor(lua_State* L)
{
	int color = g_pCharacterCreate->GetHairColor();
	lua_pushnumber(L, ((color & 0xFF0000) >> 16) / 255.0f);
	lua_pushnumber(L, ((color & 0xFF00) >> 8) / 255.0f);
	lua_pushnumber(L, (color & 0xFF) / 255.0f);
	return 3;
}
// ----------------------------------------------------------------------------
int LuaFunc_CheckCharacterCreateRace(lua_State* L)
{
	Race_ENUM race = ChangeRaceEnum(luaL_checkint(L, 1));
	switch (race)
	{
	case EM_Race1:
		lua_pushboolean(L, g_ObjectData->GetObj(1000) != NULL);
		break;

	case EM_Race2:
		lua_pushboolean(L, g_ObjectData->GetObj(1002) != NULL);
		break;

		/*	case EM_Race3:
		lua_pushboolean(L, g_ObjectData->GetObj(1004) != NULL);
		break;

	default:
		lua_pushnil(L);
		break;*/
	}

	return 1;
}
// ----------------------------------------------------------------------------
int LuaFunc_RandomCharacterCreateHair(lua_State* L)
{
	g_pCharacterCreate->RandomHair();
	return 0;
}
// ----------------------------------------------------------------------------
int LuaFunc_RandomCharacterCreateFace(lua_State* L)
{
	g_pCharacterCreate->RandomFace();
	return 0;
}
// ----------------------------------------------------------------------------
int LuaFunc_GetMaxCharacterCreate(lua_State* L)
{
	lua_pushnumber(L, g_pCharacterCreate->GetMaxCreateRoleCount());
	return 1;
}
// ----------------------------------------------------------------------------
int LuaFunc_GetReserveNumCharacters	(lua_State* L)
{
	vector<ReserverRoleStruct>& reserveCharacter = g_pCharacterCreate->GetReserveRole();
	lua_pushnumber(L, (lua_Number)reserveCharacter.size());	
	return 1;
}
// ----------------------------------------------------------------------------
int LuaFunc_GetReserveCharacterInfo	(lua_State* L)
{
	int index = luaL_checkint(L, 1) - 1;
	vector<ReserverRoleStruct>& reserveCharacter = g_pCharacterCreate->GetReserveRole();

	if ( index >= 0 && index < (int)reserveCharacter.size() )
	{
		ReserverRoleStruct& info = reserveCharacter[index];
		lua_pushstring(L, info.RoleName.Begin());				// 角色名稱
		lua_pushnumber(L, info.Race);							// 種族
		lua_pushnumber(L, info.Sex);							// 性別
		lua_pushnumber(L, info.Job);							// 職業
		return 4;
	}
	return 0;
}