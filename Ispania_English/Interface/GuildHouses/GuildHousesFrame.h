#pragma once

#include <string>
#include <vector>
#include "../interface.h"
#include "PG/PG_GuildHouses.h"
#include "../netwaker_member/net_bg_guildwar/NetCli_BG_GuildWar.h"

#include "../../mainproc/GameMainDef.h"
#include "../Houses/HousesFrame.h"


#define DF_GuildHouses_GroundSizeX 100
#define DF_GuildHouses_GroundSizeZ 100
#define DF_GuildHouses_GroundInterval 20


#define DF_GuildHouses_GroundPosX 0
#define DF_GuildHouses_GroundPosZ 0

#define DF_GuildBankPageMax 10
#define DF_GuildBankItemMax 100



class CGuildHousesFrame;
extern CGuildHousesFrame* g_pGuildHousesFrame;


class CGuildHousesFrame : public CInterfaceBase
{
public:
	CGuildHousesFrame(CInterface* object);
	virtual ~CGuildHousesFrame();


	bool isGuildWaring();
	virtual	void BuildFrames();
	virtual void DestroyFrames(); // UI ROOT 將被刪除
	virtual	void RegisterFunc();

	virtual void ReloadFrames(); // 界面重新被載入後觸發

	virtual	void LeaveWorld();

	virtual	void Update(float elapsedTime);
	virtual void Render();

	virtual bool IsHousesLoading();
	virtual float ClientLoadingOK();

	EHousesState GetHousesState(){ return m_eHousesState; };
	CursorType GetGuildHousesCursor();

public:
	void		BuyHouseStyleResult( int Type , int Result );
	virtual void RL_BuildHouseResult	( int GuildID , bool Result );
	virtual void RL_OpenVisitHouse		( int TargetNpcID , bool IsVisitMyGuildHouse );
	virtual void RL_FindHouseResult		( bool Result );
	virtual void RL_CloseVisitHouse		( );
	virtual void RL_HouseInfoLoading	( int GuildID );
	virtual void RL_HouseInfoLoadOK		( );

	virtual void RL_CreateBuildingResult	( char* Point , int BuildingID , int BuildingDBID, int ParentBuildingDBID , bool Result );
	virtual void RL_BuildingUpgradeResult	( int GItemID , int BuildingDBID , int UpgradeBuildingID , int Result );
	virtual void RL_DeleteBuildingResult	( int BuildingDBID , int Result );
	virtual void RL_AllBuildingInfo			( int GuildID , int Count , GuildBuildingObjectInfoStruct* Building );
	virtual void RL_NewBuildingInfo			( GuildBuildingObjectInfoStruct& Building );
	virtual void RL_DelBuildingInfo			( int BuildingDBID );

	virtual void RL_HouseItemInfo			( int PageID , int MaxPage , int GetCount , int ItemCount , GuildHouseItemStruct	Item[ 100 ] );
	virtual void RL_SwapBodyHouseItemResult	( int BodyPos , int	HouseItemPageID , int HouseItemPos , int GetCount , bool	Result );
	virtual void RL_FixHouseItem			( GuildHouseItemStruct&	Item );
	virtual void RL_SwapHouseItemResult		( int HouseItemPos[2] , int HouseItemPageID[2] , bool Result );
	virtual void RL_BuyItemPageResult		( int PageID , bool Result );

	virtual void RL_FurnitureItemInfo		( GuildHouseFurnitureItemStruct& Item );
	virtual void RL_SwapBodyHouseFurnitureResult( int BodyPos , int HousePos, bool Result );
	virtual void RL_FixHouseFurniture		( GuildHouseFurnitureItemStruct& Item );
	virtual void RL_SwapHouseFurnitureResult( int Pos[2] , bool Result );
	virtual void RL_HouseItemLayoutResult	( int Pos , bool Result );

	//公會倉庫權限設定
	virtual void RL_StoreConfig( GuildStorehouseRightStruct& Config );
	virtual void RL_SetStoreConfigResult( bool Result );

	//公會倉庫Log
	virtual void RL_ItemLogList( int Count , GuildItemStoreLogStruct Log[1000] );
	virtual void RL_ItemLog( GuildItemStoreLogStruct& ItemLog );

	//
	virtual void RL_ActiveBuildingResult( int BuildDBID , bool IsActive , bool Result );
	virtual void RL_BuildingActiveType	( int BuildDBID , bool IsActive );

	//盆栽
	virtual void R_FlowerPotInfo( int GItemID , ItemFieldStruct& Item );


	//公會銀行 要求顯示頁面
	void GuildBank_PageRequest( int PageID );

	//公會銀行 要求顯示Log
	void GuildBank_PageLogRequest( int PageID );

	//開啟公會銀行
	void OpenGuildBank();

public:
	//NetCli_MoveChild
	void R_ObjGuildBuildingInfo( int GItemID , int BuildingParentDBID , int BuildingDBID , int BuildingID , const char* Point );
	void R_ObjGuildFurnitureInfo( int GItemID , int FurnitureDBID );

	map<int,int> m_mapBuildingDBIDGItemID;
	map<int,int> m_mapFurnitureDBIDGItemID;

	int GetBuildingGItemID( int BuildingDBID );
	int GetFurnitureGItemID( int FurnitureDBID );

	CRoleSprite* GetRoleSpriteByBuildingDBID( int BuildingDBID );
	CRoleSprite* GetRoleSpriteByFurnitureDBID( int FurnitureDBID );
	CRoleSprite* GetRoleSpriteByFurniturePos( int Pos );
public:
	void GuildHousesControl_OnMouseDown  ( CUiFrame* uiFrame, INT64 lParam, INT64 rParam );
	void GuildHousesControl_OnMouseUp    ( CUiFrame* uiFrame, INT64 lParam, INT64 rParam );
	void GuildHousesControl_OnMouseWheel ( CUiFrame* uiFrame, INT64 lParam, INT64 rParam );
	void GuildHousesControl_OnMouseMove  ( CUiFrame* uiFrame, INT64 lParam, INT64 rParam );
	void GuildHousesControl_OnMouseEnter ( CUiFrame* uiFrame, INT64 lParam, INT64 rParam );
	void GuildHousesControl_OnMouseLeave ( CUiFrame* uiFrame, INT64 lParam, INT64 rParam );
	void GuildHousesControl_OnClick      ( CUiFrame* uiFrame, INT64 lParam, INT64 rParam );
	void GuildHousesControl_OnDoubleClick( CUiFrame* uiFrame, INT64 lParam, INT64 rParam );
	void GuildHousesControl_OnDragBegin  ( CUiFrame* uiFrame, INT64 lParam, INT64 rParam );
	void GuildHousesControl_OnDragStop   ( CUiFrame* uiFrame, INT64 lParam, INT64 rParam );
	void GuildHousesControl_OnReceiveDrag( CUiFrame* uiFrame, INT64 lParam, INT64 rParam );

	CUiFrame* m_pGuildHousesControlFrame;


	//目前公會屋是哪個工會的
	int m_CurrentGuildID;

public:
	void BuildPointOnClick( int SpriteGUID );

	int GetBuildPointGItemID( int BuildingDBID, const char* PointStr );

	void ClearBuildPoint();
	void CreateBuildPoint( int BuildingDBID, const char* PointStr, const char* LocalName );

	//void CreateBuildingToGround( int BuildingID );

	//void UpdateFloors();
	//void CreateBuilding_CheckFloors( int BuildingID , int x , int z );

	//移動建築物
	//int m_BuildingRoleSpriteID;      //Building WorldGUID
	//int m_BuildingID;

	//enum EWorkMode
	//{
	//	WM_None,
	//	WM_CreateBuildingToGround,	
	//};

	//EWorkMode m_eWorkMode;

	int m_BuildPointCount;
	map<string,int> m_mapBuildPointGItemID;

	vector<GuildHouseNpcReplaceCostTableStruct*> m_Styles;

	

	map<int,int> m_mapBuildingActive;  //buff塔 致能

public:
	int GetGuildBuildingObjectInfoCount(){ return (int)m_mapGuildBuildingObjectInfo.size(); }



	
	GuildBuildingObjectInfoStruct* GetGuildBuildingObjectInfoByID( int ID );
	GuildBuildingObjectInfoStruct* GetGuildBuildingObjectInfoByIndex( int Index );

	int GetGuildBuildingResourceCount();
	GuildBuildingInfoTableStruct* GetGuildBuildingResourceByID( int ID,int type=-1 );
	GuildBuildingInfoTableStruct*  GetReviewStyleModel(int id,int index,int type);
	GuildBuildingInfoTableStruct* GetGuildBuildingResourceByIndex( int Index );

protected:
	map<int,GuildBuildingObjectInfoStruct> m_mapGuildBuildingObjectInfo;
	vector<GuildBuildingObjectInfoStruct*> m_vecGuildBuildingObjectInfo; //lua 用 index 去要

	bool m_bBuildingObjectVectorDirty;

	//map< int , GuildBuildingInfoTableStruct >& rGuildBuildingTable = g_ObjectData->GuildBuildingTable();
	vector<GuildBuildingInfoTableStruct*> m_vecBuildingResourceTable;
	bool m_bBuildingResourceVectorDirty;
public:
	std::set<std::string> m_BuildPoints; //紀錄所有的建築點
public:

	EHousesState m_eHousesState;

	int m_VisitHouseNpcID;

	CRuEntity_Widget_ClipRect *m_clipRect;

public:
	//盆栽
	map<int,ItemFieldStruct> m_mapFlowerPotInfo;

public:
	//公會銀行

	//開啟銀行時玩家的位置
	CRuVector3 m_GuildBankPos;
	bool m_bGuildBankOpen;

	//#define DF_GuildBankPageMax 10
	//#define DF_GuildBankItemMax 100

	struct GuildBankPage
	{
		GuildBankPage()
		{
			ZeroMemory( Items , sizeof( Items ) );
			//ItemUpdateTime = -10000;

			//LogCount = 0;
			//LogUpdateTime = -10000;

			bItemUpdated = false;
			bLogUpdated = false;
		};
		ItemFieldStruct Items[DF_GuildBankItemMax];
		//int ItemUpdateTime;
		bool bItemUpdated;

		std::multimap<int,GuildItemStoreLogStruct> Logs;
		//GuildItemStoreLogStruct Log[1000];
		//int LogCount;
		//int LogUpdateTime;
		bool bLogUpdated;

		GuildStorehouseRightStruct GuildBankConfig;
	};
	std::map<int,GuildBankPage> m_GuildBankPages;


	std::vector<GuildItemStoreLogStruct> m_LogTemp;

	int m_GuildBankPageCount;

	bool m_GuildBankPageRequested;

	int m_GuildBankPageLogRequestPageID; //要求的PageID
	bool m_GuildBankPageLogRequested; //

	GuildStorehouseRightStruct m_GuildBankConfigTemp; //設定用的

	////GuildItemStoreLogStruct Log[1000]
	//struct GuildBankPageLog
	//{
	//	GuildBankPageLog()
	//	{
	//		Count = 0;
	//		UpdateTime = -10000;
	//	};
	//	int Count;
	//	GuildItemStoreLogStruct Log[1000];

	//	int UpdateTime;
	//};

	// 	std::map<int,GuildBankPageLog> m_GuildBankPageLogs;



	//ItemFieldStruct m_GuildBankItems[ DF_GuildBankItemMax * DF_GuildBankPageMax ];
	//int m_GuildBankPageUpdateTime[DF_GuildBankPageMax];

	//m_pickupPageID = -1;
	//m_pickupItemPos = -1;
	int m_pickupItemIndex;



	ItemFieldStruct* GetGuildBankItem( int index );

	void GuildBank_Pickup( int index );

	int m_guildStyle;

	void HouseStyle( int Type );


public:

protected:
	void GuildHousesControl_FurnitureMouseEnter( );
	void GuildHousesControl_FurnitureMove( );
	void GuildHousesControl_FurnitureRorare( );

	void UpdateHighlightFurnitureSprite();




public:	
	//擺放家具
	void SetPlaceFurnitureMode( bool bMode );

	void PickupFurniture( int Index );

	void FurniturLayoutChange();

	void SetFocusFurnitureID( int ID );
	void FurniturePlace ();
	void FurnitureRemove();
	void FurnitureMove  ();
	void FurnitureRorare();

	std::map<int,GuildHouseFurnitureItemStruct> m_GuildHouseFurnitureList;
	bool m_GuildHouseFurnitureChanged;

	GuildHouseFurnitureItemStruct* GetGuildHouseFurnitureItem( int pos );

	int m_MouseEnterFurnitureID;
	bool m_PlaceFurnitureMode;
	int m_FocusFurnitureID; //pos

	EPlaceingMode m_ePlaceingMode;  //放置模式
	int m_PlaceStep;     //放置模式步驟 1移動 2旋轉
	EPlaceingMode m_ePlaceingModeTemp;  //放置模式


	bool m_bFurniturLayoutChanged; //FurniturLayout 需傳給 更新


	//移動傢俱
	int m_FurnitureRoleSpriteID;      //Furniture WorldGUID

	bool m_bGetAllBuildingInfoOK;










public:	
	//公會戰

	//開啟註冊公會戰介面
	void OpenMenuRequest();
 	void RegisterRequest(bool sw=false,int index=-1);
	void CancelRegister();

	//要求跨伺服器公會名稱結果
	virtual	void RBL_GuildNameResult( int WorldGuildID , const char* GuildName );

	//開啟註冊公會戰介面
	virtual void RL_OpenMenuResult( int PlayerDBID , int ZoneGroupID , GuildHouseWarStateENUM State ,int NextTime , int Size , GuildHouseWarInfoStruct List[1000] );

	//註冊公會戰( or 取消 )結果
	virtual void RL_RegisterResult( GuildWarRegisterResultENUM Result , GuildHouseWarInfoStruct& Info );

	//公會戰狀況( 開始 , 準備 , 結束 )
	virtual	void RL_Status(GuildWarStatusTypeENUM Status ,int NextTime);


	//要求對戰歷史
	void GuildWarHistroy( void); 
	//接收對戰歷史
	void GuildWarHistroyResult(int count,GuildHouseWarHistoryStruct log[] );

	//天梯資料
	void GuildWarLadder( void  );
	void GuildWarLadderHistory( int GuildID  );
	void GuildWarLadderResult( GuildWarRankInfoStruct& Info );
	void GuildWarLadderHisotryResult( GuildHouseWarHistoryStruct Hisotry[50] );
	int GetLadderCount();
	int GetLadderHistoryCount();
	void CalRankList();

	//要求進入結果
	virtual	void RL_EnterResult(bool Result );
	
	//要求離開公會戰場結果
	virtual	void RBL_LeaveResult(bool Result );
	
	//公會戰績分資訊
	virtual	void RBL_GuildScore( GuildHouseWarFightStruct& Info );

	//公會戰開始
	virtual	void RBL_BeginWar();

	//公會戰結束
	virtual	void RBL_EndWar();


public:	

	//struct GuildHouseWarInfoStruct
	//{
	// int   GuildID;      //預約戰的公會
	// float FightTime;    //戰爭時間
	// int   Score;        //積分
	// int   EnemyGuildID; //對戰公會(-1表示沒有)
	// GuildHouseWarStateENUM State;
	//};

	GuildWarRegisterTypeENUM m_GuildWarRegisterType; //

	GuildWarStatusTypeENUM m_GuildWarStatus;

	GuildHouseWarStateENUM m_GuildHouseCurrentState;

	std::map<int,GuildHouseWarInfoStruct> m_mapGuildHouseWarInfo;
	std::vector<GuildHouseWarInfoStruct> m_vecGuildHouseWarInfo;	//配對後將重複的剔除

	std::map<int,GuildWarRankInfoStruct> m_LadderInfo;//天梯

	GuildHouseWarHistoryStruct m_LadderHistory[50];
	int m_LadderHistoryCount;
	float m_ladderDelayTime;
	int m_histotyGuildID;
	float m_LadderHistoryDelayTime;
	bool m_bIsAcceptAssignment;

	bool m_LadderInfoDirty;
	
	std::vector<int> m_ladderList;

	std::vector<GuildHouseWarHistoryStruct> m_warrepots;

	std::map<int,int> m_mapZoneGroupGuildCount;

	int m_ZoneGroupID; //自己公會的ZoneGroupID

	int m_ZoneGroupMax; //有幾個ZoneGroup 用來判斷封包是不是收完了
	int m_OpenMenuResultTimer; //當收到OpenMenu封包後 一段時間才能夠註冊 確保不會重覆註冊

	int m_GuildWarMenuOpenTime;
	bool m_GuildWarRegistering;

	float m_historyRefTime;

	int m_targetDBID;

	float m_NextTime;

	float m_delayTimer;

	void UpdateGuildHouseWarInfos();
 
	GuildHouseWarInfoStruct* GetGuildHouseWarInfoStruct( int GuildID );

};




