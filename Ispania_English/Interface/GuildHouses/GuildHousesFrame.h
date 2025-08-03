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
	virtual void DestroyFrames(); // UI ROOT �N�Q�R��
	virtual	void RegisterFunc();

	virtual void ReloadFrames(); // �ɭ����s�Q���J��Ĳ�o

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

	//���|�ܮw�v���]�w
	virtual void RL_StoreConfig( GuildStorehouseRightStruct& Config );
	virtual void RL_SetStoreConfigResult( bool Result );

	//���|�ܮwLog
	virtual void RL_ItemLogList( int Count , GuildItemStoreLogStruct Log[1000] );
	virtual void RL_ItemLog( GuildItemStoreLogStruct& ItemLog );

	//
	virtual void RL_ActiveBuildingResult( int BuildDBID , bool IsActive , bool Result );
	virtual void RL_BuildingActiveType	( int BuildDBID , bool IsActive );

	//�֮�
	virtual void R_FlowerPotInfo( int GItemID , ItemFieldStruct& Item );


	//���|�Ȧ� �n�D��ܭ���
	void GuildBank_PageRequest( int PageID );

	//���|�Ȧ� �n�D���Log
	void GuildBank_PageLogRequest( int PageID );

	//�}�Ҥ��|�Ȧ�
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


	//�ثe���|�άO���Ӥu�|��
	int m_CurrentGuildID;

public:
	void BuildPointOnClick( int SpriteGUID );

	int GetBuildPointGItemID( int BuildingDBID, const char* PointStr );

	void ClearBuildPoint();
	void CreateBuildPoint( int BuildingDBID, const char* PointStr, const char* LocalName );

	//void CreateBuildingToGround( int BuildingID );

	//void UpdateFloors();
	//void CreateBuilding_CheckFloors( int BuildingID , int x , int z );

	//���ʫؿv��
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

	

	map<int,int> m_mapBuildingActive;  //buff�� �P��

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
	vector<GuildBuildingObjectInfoStruct*> m_vecGuildBuildingObjectInfo; //lua �� index �h�n

	bool m_bBuildingObjectVectorDirty;

	//map< int , GuildBuildingInfoTableStruct >& rGuildBuildingTable = g_ObjectData->GuildBuildingTable();
	vector<GuildBuildingInfoTableStruct*> m_vecBuildingResourceTable;
	bool m_bBuildingResourceVectorDirty;
public:
	std::set<std::string> m_BuildPoints; //�����Ҧ����ؿv�I
public:

	EHousesState m_eHousesState;

	int m_VisitHouseNpcID;

	CRuEntity_Widget_ClipRect *m_clipRect;

public:
	//�֮�
	map<int,ItemFieldStruct> m_mapFlowerPotInfo;

public:
	//���|�Ȧ�

	//�}�һȦ�ɪ��a����m
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

	int m_GuildBankPageLogRequestPageID; //�n�D��PageID
	bool m_GuildBankPageLogRequested; //

	GuildStorehouseRightStruct m_GuildBankConfigTemp; //�]�w�Ϊ�

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
	//�\��a��
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

	EPlaceingMode m_ePlaceingMode;  //��m�Ҧ�
	int m_PlaceStep;     //��m�Ҧ��B�J 1���� 2����
	EPlaceingMode m_ePlaceingModeTemp;  //��m�Ҧ�


	bool m_bFurniturLayoutChanged; //FurniturLayout �ݶǵ� ��s


	//���ʳí�
	int m_FurnitureRoleSpriteID;      //Furniture WorldGUID

	bool m_bGetAllBuildingInfoOK;










public:	
	//���|��

	//�}�ҵ��U���|�Ԥ���
	void OpenMenuRequest();
 	void RegisterRequest(bool sw=false,int index=-1);
	void CancelRegister();

	//�n�D����A�����|�W�ٵ��G
	virtual	void RBL_GuildNameResult( int WorldGuildID , const char* GuildName );

	//�}�ҵ��U���|�Ԥ���
	virtual void RL_OpenMenuResult( int PlayerDBID , int ZoneGroupID , GuildHouseWarStateENUM State ,int NextTime , int Size , GuildHouseWarInfoStruct List[1000] );

	//���U���|��( or ���� )���G
	virtual void RL_RegisterResult( GuildWarRegisterResultENUM Result , GuildHouseWarInfoStruct& Info );

	//���|�Ԫ��p( �}�l , �ǳ� , ���� )
	virtual	void RL_Status(GuildWarStatusTypeENUM Status ,int NextTime);


	//�n�D��Ծ��v
	void GuildWarHistroy( void); 
	//������Ծ��v
	void GuildWarHistroyResult(int count,GuildHouseWarHistoryStruct log[] );

	//�ѱ���
	void GuildWarLadder( void  );
	void GuildWarLadderHistory( int GuildID  );
	void GuildWarLadderResult( GuildWarRankInfoStruct& Info );
	void GuildWarLadderHisotryResult( GuildHouseWarHistoryStruct Hisotry[50] );
	int GetLadderCount();
	int GetLadderHistoryCount();
	void CalRankList();

	//�n�D�i�J���G
	virtual	void RL_EnterResult(bool Result );
	
	//�n�D���}���|�Գ����G
	virtual	void RBL_LeaveResult(bool Result );
	
	//���|���Z����T
	virtual	void RBL_GuildScore( GuildHouseWarFightStruct& Info );

	//���|�Զ}�l
	virtual	void RBL_BeginWar();

	//���|�Ե���
	virtual	void RBL_EndWar();


public:	

	//struct GuildHouseWarInfoStruct
	//{
	// int   GuildID;      //�w���Ԫ����|
	// float FightTime;    //�Ԫ��ɶ�
	// int   Score;        //�n��
	// int   EnemyGuildID; //��Ԥ��|(-1��ܨS��)
	// GuildHouseWarStateENUM State;
	//};

	GuildWarRegisterTypeENUM m_GuildWarRegisterType; //

	GuildWarStatusTypeENUM m_GuildWarStatus;

	GuildHouseWarStateENUM m_GuildHouseCurrentState;

	std::map<int,GuildHouseWarInfoStruct> m_mapGuildHouseWarInfo;
	std::vector<GuildHouseWarInfoStruct> m_vecGuildHouseWarInfo;	//�t���N���ƪ��簣

	std::map<int,GuildWarRankInfoStruct> m_LadderInfo;//�ѱ�

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

	int m_ZoneGroupID; //�ۤv���|��ZoneGroupID

	int m_ZoneGroupMax; //���X��ZoneGroup �ΨӧP�_�ʥ]�O���O�����F
	int m_OpenMenuResultTimer; //����OpenMenu�ʥ]�� �@�q�ɶ��~������U �T�O���|���е��U

	int m_GuildWarMenuOpenTime;
	bool m_GuildWarRegistering;

	float m_historyRefTime;

	int m_targetDBID;

	float m_NextTime;

	float m_delayTimer;

	void UpdateGuildHouseWarInfos();
 
	GuildHouseWarInfoStruct* GetGuildHouseWarInfoStruct( int GuildID );

};




