#ifndef __Net_Move_H__
#define __Net_Move_H__
#include "PG/PG_Move.h"

#include "../../MainProc/NetGlobal.h"

#pragma unmanaged

enum MoveAttachType
{
	eMoveAttach_None			= 0,					// 沒有類型
	eMoveAttach_Point			= 1,					// 連結到參考點
	eMoveAttach_Mount			= 2,					// 坐騎
	eMoveAttach_Implement		= 3,					// 載具
	eMoveAttach_Point2			= 4,					// 連結到參考點2
};

class Net_Move : public CNetGlobal
{
protected:
    //-------------------------------------------------------------------
    static Net_Move* This;
    static bool _Init();
    static bool _Release();


    static void _PG_Move_LtoC_AddToPartition			( int Sockid , int Size , void* Data );
    static void _PG_Move_LtoC_DelFromPartition			( int Sockid , int Size , void* Data );
    static void _PG_Move_LtoC_SetPostion				( int Sockid , int Size , void* Data );
    static void _PG_Move_LtoC_ObjectMove				( int Sockid , int Size , void* Data );
    static void _PG_Move_LtoC_DoAction					( int Sockid , int Size , void* Data );
    static void _PG_Move_LtoC_MoveSpeed					( int Sockid , int Size , void* Data );
    static void _PG_Move_LtoC_MoveInfo					( int Sockid , int Size , void* Data );
    static void _PG_Move_LtoC_ModelRate				( int Sockid , int Size , void* Data );

    static void _PG_Move_LtoC_EQInfo					( int Sockid , int Size , void* Data );
    static void _PG_Move_LtoC_FixEQInfo					( int Sockid , int Size , void* Data );
    static void _PG_Move_LtoC_LookFace					( int Sockid , int Size , void* Data );
    static void _PG_Move_LtoC_ClientAutoPlot			( int Sockid , int Size , void* Data );
    static void _PG_Move_LtoC_ActionType				( int Sockid , int Size , void* Data );

    static void _PG_Move_LtoC_JumpBegin					( int Sockid , int Size , void* Data );
//    static void _PG_Move_LtoC_JumpEnd					( int Sockid , int Size , void* Data );
	static void _PG_Move_LtoC_Relation					( int Sockid , int Size , void* Data );

	static void _PG_Move_LtoC_PartyMemberMove			( int Sockid , int Size , void* Data );
	static void	_PG_Move_LtoC_PlayMotion				( int Sockid , int Size , void* Data );

	static void	_PG_Move_LtoC_SpecialMove				( int Sockid , int Size , void* Data );

	static void	_PG_Move_LtoC_Ping						( int Sockid , int Size , void* Data );
	static void	_PG_Move_LtoC_ClientLoading				( int Sockid , int Size , void* Data );

	static void	_PG_Move_LtoC_SetMinimapIcon			( int Sockid , int Size , void* Data );
	static void	_PG_Move_LtoC_SpecialEfficacy			( int Sockid , int Size , void* Data );
	static void	_PG_Move_LtoC_SetDir					( int Sockid , int Size , void* Data );
	static void	_PG_Move_LtoC_SetIdleMotion				( int Sockid , int Size , void* Data );

	static void	_PG_Move_LtoC_SetGuildID				( int Sockid , int Size , void* Data );
	static void	_PG_Move_LtoC_SetTitleID				( int Sockid , int Size , void* Data );

	static void	_PG_Move_LtoC_PlayerDropDownDamage		( int Sockid , int Size , void* Data );

	static void	_PG_Move_LtoC_ObjGuildBuildingInfo		( int Sockid , int Size , void* Data );
	static void	_PG_Move_LtoC_ObjGuildFurnitureInfo		( int Sockid , int Size , void* Data );
	static void	_PG_Move_LtoC_HorseColor				( int Sockid , int Size , void* Data );
	static void	_PG_Move_LtoC_RoomID					( int Sockid , int Size , void* Data );
	static void	_PG_Move_LtoC_PingLog					( int Sockid , int Size , void* Data );
	static void	_PG_Move_LtoC_DeadZone					( int Sockid , int Size , void* Data );

	static void	_PG_Move_LtoC_AttachObj					( int Sockid , int Size , void* Data );
	static void	_PG_Move_LtoC_DetachObj					( int Sockid , int Size , void* Data );

	static void	_PG_Move_LtoC_BattleGround_AddPlayer	( int Sockid , int Size , void* Data );
	static void	_PG_Move_LtoC_BattleGround_DelPlayer	( int Sockid , int Size , void* Data );
	static void	_PG_Move_LtoC_BattleGround_PlayerPos	( int Sockid , int Size , void* Data );
	static void	_PG_Move_LtoC_BattleGround_PlayerHPMP	( int Sockid , int Size , void* Data );

	static void	_PG_Move_LtoC_UnlockRolePosOK			( int Sockid , int Size , void* Data );
	static void	_PG_Move_LtoC_AttachObjResult			( int Sockid , int Size , void* Data );
	static void	_PG_Move_LtoC_UnlockRolePosTime			( int Sockid , int Size , void* Data );
	static void	_PG_Move_LtoC_FlowerPotInfo				( int Sockid , int Size , void* Data );
	static void	_PG_Move_LtoC_ColdownInfo				( int Sockid , int Size , void* Data );
	static void	_PG_Move_LtoC_PlayerPos					( int Sockid , int Size , void* Data );
	static void	_PG_Move_LtoC_PlayerMove				( int Sockid , int Size , void* Data );
	static void	_PG_Move_LtoC_BattleGround_PlayerInfo_Zip( int Sockid , int Size , void* Data );
	static void	_PG_Move_LtoC_AllPlayerPos				( int Sockid , int Size , void* Data );
public:
    Net_Move() { This = this; }
    //-------------------------------------------------------------------
    // 資料送出
    //-------------------------------------------------------------------
    static void PlayerMoveObject( int GItemID , float X , float Y , float Z , float Dir , int AttachOjbID , float vX = 0 , float vY = 0, float vZ = 0 , ClientMoveTypeENUM Type = EM_ClientMoveType_Normal );
    static void S_DoAction( int GItemID , int ActionID , int TempActionID  );
    static void S_JumpBegin( int GItemID  , float X , float Y , float Z , float Dir , float vX , float vY , float vZ );
//    static void S_JumpEnd( int GItemID  , float X , float Y , float Z , float Dir  );

	static int	GetRespondTime();
	static void S_Ping( int Time );
	//回應地圖載入OK
	static void S_ClientLoadingOK( );

	static void S_PingLog( int ID , int Time , int ClietNetProcTime );

	static void S_UnlockRolePosRequest( float X , float Y , float Z , bool Cancel );
    //-------------------------------------------------------------------
    //  事件繼承實做
    //-------------------------------------------------------------------
    virtual	void AddToPartition     ( int iWorldID, int GItemID , int WorldGUID , int DBID , int MasterGUID , CampID_ENUM CampID , RolePosStruct* Pos , PlayerGraphStruct* GraphObj ) = 0;
    virtual	void DelFromPartition   ( int GItemID , int WorldGUID ) = 0;
    virtual	void SetPos             ( int GItemID  , RolePosStruct* Pos ) = 0;
    virtual	void ObjectMove         ( int GItemID  , RolePosStruct* Pos , float vX , float vY , float vZ , ClientMoveTypeENUM Type ) = 0;
    virtual	void DoAction           ( int GItemID  , int ActionID , int TempActionID ) = 0;

    virtual	void MoveSpeed          ( int GItemID  , float Speed ) = 0;
	virtual	void MoveInfo			( int GItemID , float Gravity , float JumpRate ) = 0;
    virtual	void ModelRate          ( int GItemID  , float ModelRate ) = 0;

    virtual void EQInfo             ( int GItemID , LookEQInfoStruct&    EQLook , ShowEQStruct	ShowEQ ) = 0;
    virtual void FixEQInfo          ( int GItemID , EQWearPosENUM  Pos , int EQ ) = 0;
    virtual void LookFace           ( int GItemID , LookStruct& Look ) = 0;
    virtual void ClientAutoPlot     ( int GItemID , char* AutoPlot ) = 0;
    //角色行為狀態
    virtual void ActionType         ( int GItemID , RoleActionStruct	Action ) = 0;

    virtual void R_JumpBegin		( int GItemID , float X , float Y , float Z , float Dir , float vX , float vY , float vZ ) = 0;
	virtual void R_Relation			( int GItemID , RoleRelationStruct&  Relation ) = 0;

	virtual void R_PartyMemberMove	( int DBID , int GItemID , float X , float Y , float Z ) = 0;

	virtual void R_PlayMotion		( int iObjID, int iMotionID ) = 0;
  //  virtual void R_JumpEnd( int GItemID , float X , float Y , float Z , float Dir ) = 0;

	virtual void R_SpecialMove		( int GItemID , SpecialMoveTypeENUM MoveType , int TargetGItemID , float X , float Y , float Z , float Dir , int MagicCollectID ) = 0;

	virtual void R_Ping				( int Time ) = 0;

	//Server認定 Client 需要載地圖資料，等待Client端回應再加入顯像分割區
	virtual void R_ClientLoading	( float CameraX , float CameraY , float CameraZ ) = 0;
	//角色特效通知
	virtual void R_SpecialEfficacy	( int GItemID , SpecialEfficacyTypeENUM	EffectType ) = 0;
	//設定物件在小地圖上的 ICON 類型
	virtual void SetMinimapIcon		( int GItemID, int iIconID ) = 0;
	virtual void R_SetDir			( int GItemID , float Dir ) = 0;
	virtual void R_SetIdleMotion	( int GitemID, int iMotionID ) = 0;

	virtual void R_SetGuildID		( int GItemID , int GuildID ) = 0;
	virtual void R_SetTitleID		( int GItemID , bool IsShowTitle , int TitleID , const char* TitleStr ) = 0;
	virtual void R_PlayerDropDownDamage( int Damage , int Type ) = 0;
	virtual void R_ObjGuildBuildingInfo( int GItemID , int BuildingParentDBID , int BuildingDBID , int BuildingID , const char* Point ) = 0;
	virtual void R_ObjGuildFurnitureInfo( int GItemID , int FurnitureDBID ) = 0;
	virtual void R_HorseColor		( int GItemID , int HorseColor[4] ) = 0;
	virtual void R_RoomID			( int RoomID ) = 0;

	virtual void R_PingLog			( int ID , int Time  ) = 0;
	virtual void R_DeadZone			( ZoneTypeENUM Type , int DeadCountDown  ) = 0;

	virtual void R_AttachObj		( int GItemID , int AttachItemID , int AttachType , const char* ItemPos , const char* AttachPos , int Pos ) = 0;
	virtual void R_DetachObj		( int GItemID ) = 0;

	virtual void R_BattleGround_AddPlayer	( AddPlayerStruct& Info ) = 0;
	virtual void R_BattleGround_DelPlayer	( int GItemID ) = 0;
	virtual void R_BattleGround_PlayerPos	( int GItemID , float X , float Y , float Z ) = 0;
	virtual void R_BattleGround_PlayerHPMP	( int GItemID , float HP , float MP , float SP , float SP_Sub , float MaxHP , float MaxMP ) = 0;

	virtual void R_UnlockRolePosOK	( bool Result ) = 0;
	virtual void R_UnlockRolePosTime( int Time ) = 0;
	//////////////////////////////////////////////////////////////////////////
	//載具
	virtual void R_AttachObjResult( bool Result , int Pos ) = 0;
	virtual void R_DetachObjResult( bool Result ) = 0;
	static void S_AttachObjRequest( int GItemID , int WagonItemID , const char* ItemPos , const char* WagonPos , int AttachType );
	static void S_DetachObjRequest( int GItemID , int WagonItemID  );

	virtual void R_ColdownInfo( int GItemID , MagicColdownStruct& Coldown ) = 0;
	//////////////////////////////////////////////////////////////////////////
	//花盆資料
	virtual	void R_FlowerPotInfo	( int GItemID , ItemFieldStruct& Item ) = 0;
	//////////////////////////////////////////////////////////////////////////
	virtual void R_PlayerPos		( int Size , PlayerBaseInfoStruct Info[] ) = 0;
	//////////////////////////////////////////////////////////////////////////
	//設定顯示分割區
	static void S_SetPartition		( float X , float Y , float Z );

	virtual void RL_AllPlayerPos	( int PlayerCount , PlayerPosSimpleStruct tempData[] ) = 0;
};

#pragma managed
#endif //__Net_Move_H__