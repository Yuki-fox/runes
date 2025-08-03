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
	EM_PlaceingMode_New    , //�s�[�J�í�
	EM_PlaceingMode_Move   , //����
	EM_PlaceingMode_Rorare , //����
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

	//bool IsMouseEnter; //�ƹ��Ҧb
	//bool IsFocus;      //�O���O�J�I
	bool bLoaded;        //�O���O���qserver�n�L���
	bool bOpenFlag;      //�e���� �w�o�X�ʥ]���ݥ��}�X��
	bool bOpened;        //�e���� �w���}
	bool bChanged;		 //�e���� �ݭn��s
};




//Servant Work
//0 �o�b
//1 ����¤l �� ���a��
//2 �ѥ� �� Ū��
//3 ���� �� ���a
//4 �٥� �� ��a
//5 �x���� �� ��@�ش�
//6 �m�ߥΪZ�� �� �m�߾԰�
//7 �R�c �� ���R
//8 �p���^ �� �t��
//100 �@�� ��w
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

	bool isSummon; //�۳�X�� �ݻ��@����

	int random;
//	int worldGUID;

	int isWorking;
	int workBeginTime;

	int mode;													

	int step;

	CRuVector3 dirHead; //�Y ���骺��V
	CRuVector3 dirFoot; //�}����V LoadRoleSpriteServant�ɧ�s


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
	virtual void DestroyFrames(); // UI ROOT �N�Q�R��
	virtual	void RegisterFunc();

	virtual void ReloadFrames(); // �ɭ����s�Q���J��Ĳ�o

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
	//�ץ����a��m
	void FixedPlayerPos();
public:
	void SetPlaceFurnishingMode( bool bMode );

	void PickupItem( int ParentID, int Index, bool Reverse = false, bool IsBagCalled = false );

	void DrawItem( int ParentID, int Index );

	//�]�]���k�� �h��Ыήe��
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
	bool m_bLoadedRoleSpriteHouse; //�i��|�b ���J�eSprite �N�o�֮ͬ��s �y�����

	bool m_bIsOwner;
	bool m_bIsFriend;

	EHousesState m_eHousesState;

	HouseBaseDBStruct m_HouseBaseInfo;

	//�w�o�쪺�Ы�
	struct SHouseTypeInfo
	{
		int houseType;
		int keyItem;
		bool geted;
	};
	std::vector<SHouseTypeInfo> m_HouseTypeInfoTable;

	//�Ыήįq
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
	//CRoleSprite* m_pSpriteHouse;    //�Ы�

	bool m_PlaceFurnishingMode;

	int m_HouseRoleSpriteID;      //�Ы�WorldGUID

	int m_FocusFurnishingID;      //�J�I���í� DBID
 	int m_MouseEnterFurnishingID; //�ƹ����쪺HouseItem DBID

	bool m_bFurnishingChange;

	EPlaceingMode m_ePlaceingMode;  //��m�Ҧ�
	int m_PlaceStep;     //��m�Ҧ��B�J 1���� 2����

	EPlaceingMode m_ePlaceingModeTemp;  //��m�Ҧ�

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
	//�۶ҦC��^��
	virtual	void RL_ServantHireList( int ID , bool End , HouseServantStruct& Servant );
	//�۶Ҥk�����G
	virtual	void RL_ServantHireResult( bool Result , int ID , int Pos , HouseServantStruct& Servant );
	//�k���R�����G
	virtual	void RL_ServantDelResult( int ServantID , bool Result );
	//�k�����ʨƥ�q��
	virtual	void RL_ServantEvent( int ServantID , int EventID );
	//�k���R�W���G
	virtual	void RL_ServantRenameResult( const char* Name , bool Result );
	//�ץ��k���ƭ�
	virtual void RL_FixServantValue( int ServantID , HouseServantValueTypeENUM Type , int Value );
	
	//RL_LuaClientEvent lua Ĳ�o��client �ݨƥ�
	virtual	void Servant_LuaClientEvent( int GItemID , int EventType , const char* Content );

public:

	struct SServantHireList
	{
		int ID;
		HouseServantStruct Servant;
	};

	std::vector< SServantHireList >	m_ServantHireList;

	//�ޮa
	int m_MaidRoleSpriteID;
	void LoadRoleSpriteMaid();	

	bool m_bMouseEnterMaid;

	int m_FocusServantPos;       //�J�I���ޮa Pos
	int m_FocusServantDBID;      //�J�I���ޮa DBID

	int m_MouseEnterServantDBID; //�ƹ����쪺HouseItem DBID

	CRuVector3 m_ServantLookAt;

	//��UI�ϥΪ�index
	int m_UIServantIndex[6];
	int m_ServantCount;

	void UpdateUIServantIndex();

	//�۶ҨӪ��k��
	void LoadRoleSpriteServant( int index );

	string GetServantName( HouseServantStruct& rServant );		   
	void GetServantBirthday( HouseServantStruct& rServant, int& outMonth, int& outDay, string& outHoroscope );
	Race_ENUM GetServantRace( HouseServantStruct& rServant ); // �ر�	   
public:
	bool Houses_CanWearObject( ItemFieldStruct* item , int eqPos );

	void UpdateServants( float elapsedTime );
	void UpdateServant( float elapsedTime, int index );

	void UpdateServantAction_0( float elapsedTime, SHousesServantWorkInfo& rWorkInfo ); //0 �o�b
	void UpdateServantAction_1( float elapsedTime, SHousesServantWorkInfo& rWorkInfo ); //1 ����¤l �� ���a�� 
	void UpdateServantAction_2( float elapsedTime, SHousesServantWorkInfo& rWorkInfo );	//2 �ѥ� �� Ū��
	void UpdateServantAction_3( float elapsedTime, SHousesServantWorkInfo& rWorkInfo ); //3 ���� �� ���a
	void UpdateServantAction_4( float elapsedTime, SHousesServantWorkInfo& rWorkInfo ); //4 �٥� �� ��a
	void UpdateServantAction_5( float elapsedTime, SHousesServantWorkInfo& rWorkInfo ); //5 �x���� �� ��@�ش�
	void UpdateServantAction_6( float elapsedTime, SHousesServantWorkInfo& rWorkInfo ); //6 �m�ߥΪZ�� �� �m�߾԰�
	void UpdateServantAction_7( float elapsedTime, SHousesServantWorkInfo& rWorkInfo ); //7 �R�c �� ���R
	void UpdateServantAction_8( float elapsedTime, SHousesServantWorkInfo& rWorkInfo ); //8 �p���^ �� �t��
 	void UpdateServantAction_100( float elapsedTime, SHousesServantWorkInfo& rWorkInfo ); //100 �@�� ��w

	SHousesServantWorkInfo m_ServantWorkInfos[MAX_SERVANT_COUNT];
	void UpdateServantWorkInfo( int index );

	void ServantTalk( const char* msgType, int ServantID,  const char* msg ); 

public:

	void PickFurnishing( CRuVector3& point, CRuVector3& dir, CRoleSprite*& poutRoleSprite, float& outDistance );
protected:
	//�קK�PSpriteID �X�{
	//�C�������ܳ��s�ؤ@��RoleSprite   SpriteWorldGUID
	//int m_FurnishingSpriteWorldGUIDUsed;
	//typedef map<int,int> TD_FurnishingSpriteWorldGUIDMap;
	//TD_FurnishingSpriteWorldGUIDMap m_FurnishingSpriteWorldGUIDMap;

	//�֮�
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
		HPS_None , //�L  
		HPS_Grow , //����
		HPS_Ripe , //���~
		//HPS_Lock , //��w
		HPS_Death , //�\��
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

			//�U�����q
			nReapTime = -1;    
		}

		ItemFieldStruct Item;
		int nPlantID; //�ؤl�̪��ҫ�ID

		string strName;

		//���A
		EHousePlantState eState;

		//�شӮɶ� ��@�üƺؤl�ϥ�
		int nCreateTime;

		bool isLock;

		//�ݩ�
		int nType;

		int nFeedPoint;

		int nWaterPoint;     //��� 
		int nTopdressPoint;  //�i�� 
		int nBestWaterPoint;
		int nBestTopdressPoint;

		float fDecWater;     //�����C�p�ɤU��
		float fDecTopdress;  //�i���C�p�ɤU��

		//�������A
		float fGrowRate;
		float fHealthRate; //���d�� 
		float fRareRate;   //�}����

		//�U�����q
		int nReapTime;    //��ì���ɶ� -1 �N��S���ؤl�i�H�ش�

		int nSpriteWorldGUID;
		SHousePlantNode sTree;
	};

	map<int,SHousePlant> m_MapHousePlant;

	bool UpdateHousePlant( SHousePlant& rPlant, ItemFieldStruct& rItem );
	//void Houses_CreatePlant( ItemFieldStruct& Item, SHousePlantNode& rHousePlant );
	void Houses_CreatePlantNode( CRuEntity* pParent, SHousePlant& rPlant, SHousePlantNode& rNode );

	// ----------------------------------------------------------------------------
	// ���˳ƹw����
	// ----------------------------------------------------------------------------
	void Houses_CreatePlant( CRuEntity* pParent, ItemFieldStruct& rItem );
public:
	int m_PlantFurnishingDBID;  //�֮⪺DBID
	int m_PlantFurnishingPos;   //�֮⪺Pos

	//�ؤl��ObjectID
	int m_ObjectID_Seed;

	//����ObjectID
	int m_ObjectID_Water;

	//�ήƪ�ObjectID
	int m_ObjectID_Topdress;

	//�]�k��@���~��ObjectID
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
	//�d�n��
	PTRVALUE m_HousesSoundHandle;

	int m_HousesSoundFurnishingDBID;

 	void StopHousesSound();
	void PlayHousesSound();

	void StopAllFurnishingSound();
	void PlayFurnishingSound( int DBID );

};










