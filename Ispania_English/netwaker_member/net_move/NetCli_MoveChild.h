#pragma once

#include "Net_Move.h"

class NetCli_MoveChild : public Net_Move
{
public:
	static map<int, MagicColdownStruct>		s_magicColdown;

	static bool _Init();
	static bool _Release();
	static void	AnchorPosition( int GItemID, RolePosStruct* Pos );
	static void SetEquipmentInfo( void* data, LookEQInfoStruct& EQLook, ShowEQStruct ShowEQ );
	static void ClearImplementColdown();
	static void	SetImplementMagicColdown	( int GItemID, int magicID );
	static MagicColdownStruct* GetImplementMagicColdown( int GItemID );
	static void	OnTimeProcess_HundredMS();
	

	virtual	void AddToPartition     ( int iWorldID, int GItemID , int WorldGUID , int DBID , int MasterGUID , CampID_ENUM CampID , RolePosStruct* Pos , PlayerGraphStruct* GraphObj );
	virtual	void DelFromPartition   ( int GItemID , int WorldGUID );
	virtual	void SetPos             ( int GItemID  , RolePosStruct* Pos );
	virtual	void ObjectMove         ( int GItemID  , RolePosStruct* Pos , float vX , float vY , float vZ , ClientMoveTypeENUM Type );

	virtual	void DoAction           ( int GItemID  , int ActionID , int TempActionID );

	virtual	void MoveSpeed          ( int GItemID  , float Speed );
	virtual	void MoveInfo			( int GItemID , float Gravity , float JumpRate );
	virtual	void ModelRate          ( int GItemID  , float ModelRate );

	virtual void EQInfo             ( int GItemID , LookEQInfoStruct&    EQLook , ShowEQStruct	ShowEQ );
	virtual void FixEQInfo          ( int GItemID , EQWearPosENUM  Pos , int EQ );
	virtual void LookFace           ( int GItemID , LookStruct& Look );
	virtual void ClientAutoPlot     ( int GItemID , char* AutoPlot );
	//角色行為狀態
	virtual void ActionType         ( int GItemID , RoleActionStruct	Action );

	virtual void R_JumpBegin		( int GItemID , float X , float Y , float Z , float Dir , float vX , float vY , float vZ );
	//  virtual void R_JumpEnd( int GItemID , float X , float Y , float Z , float Dir ) = 0;
	virtual void R_Relation			( int GItemID , RoleRelationStruct&   Relation );
	virtual void R_PartyMemberMove	( int DBID , int GItemID , float X , float Y , float Z );
	virtual void R_PlayMotion		( int iObjID, int iMotionID );

	virtual void R_SpecialMove		( int GItemID , SpecialMoveTypeENUM MoveType , int TargetGItemID , float X , float Y , float Z , float Dir , int MagicCollectID );
	virtual void R_Ping				( int Time );

	//Server認定 Client 需要載地圖資料，等待Client端回應再加入顯像分割區
	virtual void R_ClientLoading	( float CameraX , float CameraY , float CameraZ );

	//角色特效通知
	virtual void R_SpecialEfficacy	( int GItemID , SpecialEfficacyTypeENUM	EffectType );

	virtual void SetMinimapIcon		( int GItemID, int iIconID );
	//設定面向
	virtual void R_SetDir			( int GItemID , float Dir );
	virtual void R_SetIdleMotion	( int GitemID, int iMotionID );

	virtual void R_SetGuildID		( int GItemID , int GuildID );
	virtual void R_SetTitleID		( int GItemID , bool IsShowTitle , int TitleID , const char* TitleStr );
	virtual void R_PlayerDropDownDamage( int Damage , int Type );
	virtual void R_ObjGuildBuildingInfo( int GItemID , int BuildingParentDBID , int BuildingDBID , int BuildingID , const char* Point );
	virtual void R_ObjGuildFurnitureInfo( int GItemID , int FurnitureDBID );
	virtual void R_HorseColor		( int GItemID , int HorseColor[4] );
	virtual void R_RoomID			( int RoomID );
	virtual void R_PingLog			( int ID , int Time  );
	virtual void R_DeadZone			( ZoneTypeENUM Type , int DeadCountDown  );
	virtual void R_AttachObj		( int GItemID , int AttachItemID , int AttachType , const char* ItemPos , const char* AttachPos , int Pos );
	virtual void R_DetachObj		( int GItemID );

	virtual void R_BattleGround_AddPlayer	( AddPlayerStruct& Info );
	virtual void R_BattleGround_DelPlayer	( int GItemID );
	virtual void R_BattleGround_PlayerPos	( int GItemID , float X , float Y , float Z );
	virtual void R_BattleGround_PlayerHPMP	( int GItemID , float HP , float MP , float SP , float SP_Sub , float MaxHP , float MaxMP );

	virtual void R_UnlockRolePosOK	( bool Result );
	virtual void R_UnlockRolePosTime( int Time );
	//////////////////////////////////////////////////////////////////////////
	virtual void R_AttachObjResult( bool Result , int Pos );
	virtual void R_DetachObjResult( bool Result );
	virtual void R_ColdownInfo( int GItemID , MagicColdownStruct& Coldown );
	//////////////////////////////////////////////////////////////////////////
	//花盆資料
	virtual	void R_FlowerPotInfo	( int GItemID , ItemFieldStruct& Item );
	virtual void R_PlayerPos		( int Size , PlayerBaseInfoStruct Info[] );

	virtual void RL_AllPlayerPos	( int PlayerCount , PlayerPosSimpleStruct tempData[] );
};