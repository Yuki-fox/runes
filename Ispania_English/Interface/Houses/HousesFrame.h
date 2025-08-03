#pragma once

#include <string>
#include <vector>
#include "../interface.h"
#include "PG/PG_Houses.h"
#include "../../mainproc/GameMainDef.h"
#include "../WorldFrames/BagFrame.h"

class CHousesFrame;
extern CHousesFrame* g_pHousesFrame;

enum EHousesState
{
	EHousesState_Loading ,
	EHousesState_LoadOK  , 
	EHousesState_Enter   ,
	EHousesState_Leave   ,
};

enum EPlaceingMode
{
	EM_PlaceingMode_None   = 0,
	EM_PlaceingMode_New    , //新加入傢俱
	EM_PlaceingMode_Move   , //移動
	EM_PlaceingMode_Rorare , //旋轉
};

typedef map<int,HouseItemDBStruct*> TD_HouseItemMap;

struct SHouseContainer
{
	SHouseContainer()
	{
		bDirty = true;
		bLoaded = false;
		bOpenFlag = false;
		bOpened = false;
		bChanged = false;
	}



	bool bDirty;
	TD_HouseItemMap Items;

	//bool IsMouseEnter; //滑鼠所在
	//bool IsFocus;      //是不是焦點
	bool bLoaded;        //是不是有從server要過資料
	bool bOpenFlag;      //容器類 已發出封包等待打開旗標
	bool bOpened;        //容器類 已打開
	bool bChanged;		 //容器類 需要更新
};




//Servant Work
//0 發呆
//1 雞毛撢子 → 擦家具
//2 書本 → 讀書
//3 掃帚 → 掃地
//4 抹布 → 拖地
//5 灑水器 → 灌溉種植
//6 練習用武器 → 練習戰鬥
//7 舞鞋 → 跳舞
//8 小提琴 → 演奏
//100 劇情 鎖定
struct SHousesServantWork
{
//	int servantDBID;
	int mode;
	float x;
	float y;
	float z;
	float angle;
};

struct SHousesServantWorkInfo
{
	SHousesServantWorkInfo()
	{
		random = -1;
//		worldGUID = -1;
		isWorking = false;
		workBeginTime = -1;

		step = 0;

		isSummon = false;

		ZeroMemory( item, sizeof(item) );
	}
//	int servantDBID;

	bool isSummon; //招喚出來 需說一次話

	int random;
//	int worldGUID;

	int isWorking;
	int workBeginTime;

	int mode;													

	int step;

	CRuVector3 dirHead; //頭 身體的方向
	CRuVector3 dirFoot; //腳的方向 LoadRoleSpriteServant時更新


	CRuVector3 vecTargetPos;

	bool item[8];

	//	int workTime;
//	std::list<SHousesServantWork> m_works;
	//
	CRoleSprite* pRoleSprite;
	HouseItemDBStruct* pServantItemDB; 
	HouseServantStruct* pServant;
};


class CHousesFrame : public CInterfaceBase
{
public:
	CHousesFrame(CInterface* object);
	virtual ~CHousesFrame();
	virtual	void BuildFrames();
	virtual void DestroyFrames(); // UI ROOT 將被刪除
	virtual	void RegisterFunc();

	virtual void ReloadFrames(); // 界面重新被載入後觸發

	virtual	void LeaveWorld();

	virtual	void Update(float elapsedTime);

	virtual bool IsHousesLoading();
	virtual float ClientLoadingOK();

public:

	virtual void R_BuildHouseResult				( int HouseDBID , bool Result );
	virtual void R_OpenVisitHouse				( int TargetNpcID , bool IsVisitMyHouse );
	virtual void R_CloseVisitHouse				();
	virtual void R_FindHouseResult				( FindHouseResultENUM Result );
	virtual void R_HouseInfoLoading				();
	virtual void R_HouseBaseInfo				( HouseBaseDBStruct& HouseBaseInfo );
	virtual void R_ItemInfo						( int ItemDBID , int ItemCount , HouseItemDBStruct Item[_DEF_MAX_HOUSE_FURNITURE] );
	virtual void R_HouseInfoLoadOK				();
	virtual void R_SwapBodyHouseItemResult		( int HouseParentItemDBID , int HousePos , int BodyPos , bool Result );
	virtual void R_FixHouseItem					( HouseItemDBStruct& HouseItem );
	virtual void R_SwapHouseItemResult			( int ParentItemDBID[2] , int HousePos[2] , bool Result );
	virtual void R_HouseItemLayoutResult		( int HousePos , bool Result );
	virtual void R_ChangeHouseResult			( int HouseType , bool Result );
	virtual void R_BuyFunctionResult			( int FunctionID , bool Result );
	virtual void R_FunctionSettingResult		( int FunctionID , int Mode , bool Result );
	virtual void R_BuyEnergyResult				( int Money_Account , int Energy );
	virtual void R_SetPasswordResult			( bool Result );


public:

	void HousesControl_OnMouseDown  ( CUiFrame* uiFrame, INT64 lParam, INT64 rParam );
	void HousesControl_OnMouseUp    ( CUiFrame* uiFrame, INT64 lParam, INT64 rParam );
	void HousesControl_OnMouseWheel ( CUiFrame* uiFrame, INT64 lParam, INT64 rParam );
	void HousesControl_OnMouseMove  ( CUiFrame* uiFrame, INT64 lParam, INT64 rParam );
	void HousesControl_OnMouseEnter ( CUiFrame* uiFrame, INT64 lParam, INT64 rParam );
	void HousesControl_OnMouseLeave ( CUiFrame* uiFrame, INT64 lParam, INT64 rParam );
	void HousesControl_OnClick      ( CUiFrame* uiFrame, INT64 lParam, INT64 rParam );
	void HousesControl_OnDoubleClick( CUiFrame* uiFrame, INT64 lParam, INT64 rParam );
	void HousesControl_OnDragBegin  ( CUiFrame* uiFrame, INT64 lParam, INT64 rParam );
	void HousesControl_OnDragStop   ( CUiFrame* uiFrame, INT64 lParam, INT64 rParam );
	void HousesControl_OnReceiveDrag( CUiFrame* uiFrame, INT64 lParam, INT64 rParam );

	void HousesControl_HouseItemMouseEnter( );
	void HousesControl_HouseItemMove( );
	void HousesControl_HouseItemRorare( );

	void HousesControl_UseFurniture( int FurnitureID );
	void HousesControl_DoMaid();
 	void HousesControl_DoServant( int SpriteGUID );
public:
	//修正玩家位置
	void FixedPlayerPos();
public:
	void SetPlaceFurnishingMode( bool bMode );

	void PickupItem( int ParentID, int Index, bool Reverse = false, bool IsBagCalled = false );

	void DrawItem( int ParentID, int Index );

	//包包按右鍵 搬到房屋容器
	bool DepositFromBag( int bagindex );

	void SetFocusFurnishingID( int ID );
	void FurnishingPlace ();
	void FurnishingRemove();
	void FurnishingMove  ();
	void FurnishingRorare();

	void HouseItemLayoutChange();

	void CloseStorage( int DBID );

	EHousesState GetHousesState(){ return m_eHousesState; };

	CursorType GetHousesCursor();

public:

	void HangerSwap( int ItemDBID, bool bAll );
public:
	bool m_FixedPlayerPos;
	bool m_bLoadedRoleSpriteHouse; //可能會在 載入前Sprite 就發生盆栽更新 造成當機

	bool m_bIsOwner;
	bool m_bIsFriend;

	EHousesState m_eHousesState;

	HouseBaseDBStruct m_HouseBaseInfo;

	//已得到的房屋
	struct SHouseTypeInfo
	{
		int houseType;
		int keyItem;
		bool geted;
	};
	std::vector<SHouseTypeInfo> m_HouseTypeInfoTable;

	//房屋效益
	int m_ExpPoint;
	int m_TpPoint;
	int m_BlackSmithHQPoint;
	int m_CarpenterHQPoint;	
	int m_MakeArmorHQPoint;	
	int m_TailorHQPoint;		
	int m_CookHQPoint;		
	int m_AlchemyHQPoint;

	void UpdateHousesBenefit();

//	typedef map<int,HouseItemDBStruct> TD_HouseItemMap;	 //Pos
//	typedef map<int,TD_HouseItemMap> TD_HousesContainerMap;	//DBID
//	TD_HousesContainerMap m_HousesContainerMap;

//	typedef map<int,SHouseFurnishingState> TD_HouseFurnishingState;
//	TD_HouseFurnishingState m_HouseFurnishingState;

//
	map< int, HouseItemDBStruct > m_HouseItems;

	map<int,SHouseContainer> m_HouseContainers;

	
	//
	TD_HouseItemMap& GetHouseContainer( int ContainerID );

  	TD_HouseItemMap& GetHouseFurnishingList();

	HouseItemDBStruct* GetHousesItemDB( int ContainerID , int Pos );
	HouseItemDBStruct* GetHousesItemDBByDBID( int DBID );
	int GetContainerMaxItemCount( int ContainerID );
	int GetContainerLastPos( int ContainerID );


	void HouseContainerChanged( int ContainerDBID );
public:
	int m_VisitHouseNpcID;
	//CRoleSprite* m_pSpriteHouse;    //房屋

	bool m_PlaceFurnishingMode;

	int m_HouseRoleSpriteID;      //房屋WorldGUID

	int m_FocusFurnishingID;      //焦點的傢俱 DBID
 	int m_MouseEnterFurnishingID; //滑鼠指到的HouseItem DBID

	bool m_bFurnishingChange;

	EPlaceingMode m_ePlaceingMode;  //放置模式
	int m_PlaceStep;     //放置模式步驟 1移動 2旋轉

	EPlaceingMode m_ePlaceingModeTemp;  //放置模式

public:

	int m_BuyFunction_NewOpenEnergy;
	int m_BuyFunction_FunctionID;

#ifdef KALYDO
	static void KalydoLoadHouseCallback(const char* fileName, TKResult result, void* id);
#endif // KALYDO
	void ActuallyLoadHouse(const char* actName);

protected:
	void UpdateHighlightSprite();
	void LoadRoleSpriteHouse();	
	void LoadRoleSpriteHouseItem( HouseItemDBStruct* HouseItemDB );
public:
	void UpdateAlllPlantSprite();
public:
	CRoleSprite* GetHouseRoleSpriteByItemDBID( int ItemDBID );
public:
	CUiFrame* m_pHousesControlFrame;

public:
	//招募列表回應
	virtual	void RL_ServantHireList( int ID , bool End , HouseServantStruct& Servant );
	//招募女僕結果
	virtual	void RL_ServantHireResult( bool Result , int ID , int Pos , HouseServantStruct& Servant );
	//女僕刪除結果
	virtual	void RL_ServantDelResult( int ServantID , bool Result );
	//女僕互動事件通知
	virtual	void RL_ServantEvent( int ServantID , int EventID );
	//女僕命名結果
	virtual	void RL_ServantRenameResult( const char* Name , bool Result );
	//修正女僕數值
	virtual void RL_FixServantValue( int ServantID , HouseServantValueTypeENUM Type , int Value );
	
	//RL_LuaClientEvent lua 觸發的client 端事件
	virtual	void Servant_LuaClientEvent( int GItemID , int EventType , const char* Content );

public:

	struct SServantHireList
	{
		int ID;
		HouseServantStruct Servant;
	};

	std::vector< SServantHireList >	m_ServantHireList;

	//管家
	int m_MaidRoleSpriteID;
	void LoadRoleSpriteMaid();	

	bool m_bMouseEnterMaid;

	int m_FocusServantPos;       //焦點的管家 Pos
	int m_FocusServantDBID;      //焦點的管家 DBID

	int m_MouseEnterServantDBID; //滑鼠指到的HouseItem DBID

	CRuVector3 m_ServantLookAt;

	//給UI使用的index
	int m_UIServantIndex[6];
	int m_ServantCount;

	void UpdateUIServantIndex();

	//招募來的女僕
	void LoadRoleSpriteServant( int index );

	string GetServantName( HouseServantStruct& rServant );		   
	void GetServantBirthday( HouseServantStruct& rServant, int& outMonth, int& outDay, string& outHoroscope );
	Race_ENUM GetServantRace( HouseServantStruct& rServant ); // 種族	   
public:
	bool Houses_CanWearObject( ItemFieldStruct* item , int eqPos );

	void UpdateServants( float elapsedTime );
	void UpdateServant( float elapsedTime, int index );

	void UpdateServantAction_0( float elapsedTime, SHousesServantWorkInfo& rWorkInfo ); //0 發呆
	void UpdateServantAction_1( float elapsedTime, SHousesServantWorkInfo& rWorkInfo ); //1 雞毛撢子 → 擦家具 
	void UpdateServantAction_2( float elapsedTime, SHousesServantWorkInfo& rWorkInfo );	//2 書本 → 讀書
	void UpdateServantAction_3( float elapsedTime, SHousesServantWorkInfo& rWorkInfo ); //3 掃帚 → 掃地
	void UpdateServantAction_4( float elapsedTime, SHousesServantWorkInfo& rWorkInfo ); //4 抹布 → 拖地
	void UpdateServantAction_5( float elapsedTime, SHousesServantWorkInfo& rWorkInfo ); //5 灑水器 → 灌溉種植
	void UpdateServantAction_6( float elapsedTime, SHousesServantWorkInfo& rWorkInfo ); //6 練習用武器 → 練習戰鬥
	void UpdateServantAction_7( float elapsedTime, SHousesServantWorkInfo& rWorkInfo ); //7 舞鞋 → 跳舞
	void UpdateServantAction_8( float elapsedTime, SHousesServantWorkInfo& rWorkInfo ); //8 小提琴 → 演奏
 	void UpdateServantAction_100( float elapsedTime, SHousesServantWorkInfo& rWorkInfo ); //100 劇情 鎖定

	SHousesServantWorkInfo m_ServantWorkInfos[MAX_SERVANT_COUNT];
	void UpdateServantWorkInfo( int index );

	void ServantTalk( const char* msgType, int ServantID,  const char* msg ); 

public:

	void PickFurnishing( CRuVector3& point, CRuVector3& dir, CRoleSprite*& poutRoleSprite, float& outDistance );
protected:
	//避免同SpriteID 出現
	//每次有改變都新建一個RoleSprite   SpriteWorldGUID
	//int m_FurnishingSpriteWorldGUIDUsed;
	//typedef map<int,int> TD_FurnishingSpriteWorldGUIDMap;
	//TD_FurnishingSpriteWorldGUIDMap m_FurnishingSpriteWorldGUIDMap;

	//盆栽
	//map<CRoleSprite*,CRuEntity*> m_MapPlantEntity;

	enum EHousePlantType
	{
		HPT_NONE,
		HPT_TREE,
		HPT_BRANCH,
		HPT_LEAF,
		HPT_ROOTLEAF,
		HPT_FLOWER,
		HPT_FLOWERED,
	};

	//enum EHousePlantNodeState
	//{
	//	HPNS_CLOSE,
	//	HPNS_OPENING,
	//	HPNS_OPEN,
	//};

	struct SHousePlantNode
	{
		SHousePlantNode()
		{
			eType = HPT_NONE;
			nRandom = 0;
			nBranchLayer = 0;
			pEntity = NULL;
		}

		EHousePlantType eType;
		string strLinkName;
		int nBranchLayer; 
		int nRandom;

		//EHousePlantState eState;

		CRuEntity* pEntity;
		vector<SHousePlantNode> Children;
	};
public:
	enum EHousePlantState
	{
		HPS_None , //無  
		HPS_Grow , //成長
		HPS_Ripe , //成年
		//HPS_Lock , //鎖定
		HPS_Death , //枯死
	};

	struct SHousePlant
	{
		SHousePlant()
		{
			nSpriteWorldGUID = -1;
			nPlantID  = -1;

			eState = HPS_None;

			nCreateTime = 0;

			isLock = false;

			nType = 0;

			nFeedPoint = 0;

			nWaterPoint = 0;     
			nTopdressPoint = 0;  
			nBestWaterPoint = 0;
			nBestTopdressPoint = 0;

			fDecWater = 0;
			fDecTopdress = 0;

			fGrowRate = 0;
			fHealthRate = 0;
			fRareRate = 0;

			//下次階段
			nReapTime = -1;    
		}

		ItemFieldStruct Item;
		int nPlantID; //種子裡的模型ID

		string strName;

		//狀態
		EHousePlantState eState;

		//種植時間 當作亂數種子使用
		int nCreateTime;

		bool isLock;

		//屬性
		int nType;

		int nFeedPoint;

		int nWaterPoint;     //濕度 
		int nTopdressPoint;  //養分 
		int nBestWaterPoint;
		int nBestTopdressPoint;

		float fDecWater;     //水分每小時下降
		float fDecTopdress;  //養分每小時下降

		//成長狀態
		float fGrowRate;
		float fHealthRate; //健康度 
		float fRareRate;   //稀有度

		//下次階段
		int nReapTime;    //收穫的時間 -1 代表沒有種子可以種植

		int nSpriteWorldGUID;
		SHousePlantNode sTree;
	};

	map<int,SHousePlant> m_MapHousePlant;

	bool UpdateHousePlant( SHousePlant& rPlant, ItemFieldStruct& rItem );
	//void Houses_CreatePlant( ItemFieldStruct& Item, SHousePlantNode& rHousePlant );
	void Houses_CreatePlantNode( CRuEntity* pParent, SHousePlant& rPlant, SHousePlantNode& rNode );

	// ----------------------------------------------------------------------------
	// 給裝備預覽用
	// ----------------------------------------------------------------------------
	void Houses_CreatePlant( CRuEntity* pParent, ItemFieldStruct& rItem );
public:
	int m_PlantFurnishingDBID;  //盆栽的DBID
	int m_PlantFurnishingPos;   //盆栽的Pos

	//種子的ObjectID
	int m_ObjectID_Seed;

	//水的ObjectID
	int m_ObjectID_Water;

	//肥料的ObjectID
	int m_ObjectID_Topdress;

	//魔法灌溉物品的ObjectID
	int m_ObjectID_MagicWater;

	enum EPlantItemType
	{
		PIT_Unknow    ,
		PIT_Seed      ,
		PIT_Water     ,
		PIT_Topdress  ,
		PIT_MagicWater,
	};

	void Plant_PickupItem( EPlantItemType type );
	void Plant_ClearItem( EPlantItemType type );
	void Plant_Grow( EPlantItemType type );
	void Plant_Clear();
	void Plant_Lock();
	void Plant_GetProduct();
	void Plant_Close();

	virtual void R_PlantClearResult				( int HousePos , PlantClearResultTypeENUM Result );
	virtual void R_PlantItemResult				( PlantItemTypeENUM Type , int PotHousePos , int ItemBodyPos , PlantItemResultTypeENUM Result  );
	virtual void R_PlantLockResult				( int PotHousePos , PlantLockResultTypeENUM Result );
	virtual void R_PlantGetProductResult		( int PotHousePos , PlantGetProductResultTypeENUM Result );


public:
	void UpdateHouseFriend();

	virtual void R_FriendDBID				( int HouseDBID , int FriendDBID );
	virtual void R_DeleteFriendDBID			( int FriendDBID );
	virtual void R_AddFriendDBID			( int FriendDBID );

	virtual void R_FriendItemLog            ( HouseFriendItemLogStruct& Log );

	std::vector<int> m_FriendDBIDTable;


public:
	//留聲機
	PTRVALUE m_HousesSoundHandle;

	int m_HousesSoundFurnishingDBID;

 	void StopHousesSound();
	void PlayHousesSound();

	void StopAllFurnishingSound();
	void PlayFurnishingSound( int DBID );

};










