
#include "../../mainproc/GameMain.h"
#include "../../netwaker_member/NetWakerClientInc.h"
#include "../WorldFrames/ItemClipboard.h"
#include "../WorldFrames/ChatEdit.h"
#include "../WorldFrames/ChatFrame.h"

#include "../debug/DebugMessage.h"
#include "../../tools/Common/Com_Function.h"


#include "../WorldFrames/GameControl.h"
#include "../WorldFrames/CharacterFrame.h"
#include "../Teach/UI_TeachingFrame.h"

#include "../WorldFrames/GuildFrame.h"


#include "../WorldFrames/BagFrame.h"

#include "../WorldFrames/ItemUTIL.h"
#include "../ItemLink/ItemLink.h"

//#include "../Map/WorldMap.h"
#include "../WorldFrames/GuildWarScore.h"
#include "../WorldFrames/GuildFrame.h"

#include "../Login/AccountLogin.h"

//#include "Rune Engine\rune\scene\container\RuContainer_Collision.h"

#include "GuildHousesFrame.h"
#include "GuildHouseStatus.h"


#define DF_ZoneGroupMax    40 //ZoneGroup最多幾個公會報名
#define DF_ZoneGroupLowest 20 //20 //ZoneGroup至少幾個公會報名 才開放下一個ZoneGroup


#define DF_FurnitureCount 100


//#define DF_GuildHousesRoleSpriteWorldGUID (-30000000)	
#define DF_GuildHousesBuildPointGItemID (-40000000)
#define DF_GuildFurnitureRoleSpriteWorldGUID (-40000001)


#define UiEvent_GuildHousesOpenVisitHouse   "GUILD_HOUSES_OPEN_VISIT_HOUSE"
#define UiEvent_GuildHousesCloseVisitHouse  "GUILD_HOUSES_CLOSE_VISIT_HOUSE"



#define UiEvent_GuildHousesFrameShow  "GUILDHOUSESFRAME_SHOW"
#define UiEvent_GuildHousesFrameHide  "GUILDHOUSESFRAME_HIDE"


#define UiEvent_GuildHousesBuildingInfoChanged  "GUILDHOUSES_BUILDINGINFO_CHANGED"
#define UiEvent_GuildHousesOpenBuildingResources  "GUILDHOUSES_OPEN_BUILDING_RESOURCES"

#define UiEvent_GuildBankFixItem "GUILDBANK_FIX_ITEM"



#define UiEvent_GuildBankPageUpdate "GUILDBANK_PAGE_UPDATE"

#define UiEvent_GuildBankClose "GUILDBANK_CLOSE"

#define UiEvent_GuildBankPageLogUpdate "GUILDBANK_PAGE_LOG_UPDATE"



#define UiEvent_GuildHouseFurnitureChanged   "GUILDHOUSE_FURNITURE_CHANGED"


#define UiEvent_GuildHouseWarInfosUpdate "GUILDHOUSEWAR_INFOS_UPDATE"
#define UiEvent_GuildHouseWarBeginWar "GUILDHOUSEWARBEGINWAR"
#define UiEvent_GuildHouseWarEndWar "GUILDHOUSEWARENDWAR"
#define UiEvent_GuildHouseWarEnterResult "GUILDHOUSEWARENTERRESULT"
#define UiEvent_GuildHouseWarScore "GUILDHOUSEWARSCORE"



#define UiEvent_GuildHouseWarStateChange "GUILDHOUSEWAR_STATE_CHANGE"




/*
struct GuildStyleInfoTableStruct
{
	const char *name;
	int ID;
	int OrgObjID;
	GuildResourceStruct res;
};


*/





CGuildHousesFrame* g_pGuildHousesFrame = NULL;

// ----------------------------------------------------------------------------
void UiCallback_GuildHousesControl_OnMouseDown  ( CUiFrame* uiFrame, INT64 lParam, INT64 rParam ){ g_pGuildHousesFrame->GuildHousesControl_OnMouseDown  ( uiFrame, lParam, rParam ); }
void UiCallback_GuildHousesControl_OnMouseUp    ( CUiFrame* uiFrame, INT64 lParam, INT64 rParam ){ g_pGuildHousesFrame->GuildHousesControl_OnMouseUp    ( uiFrame, lParam, rParam ); }
void UiCallback_GuildHousesControl_OnMouseWheel ( CUiFrame* uiFrame, INT64 lParam, INT64 rParam ){ g_pGuildHousesFrame->GuildHousesControl_OnMouseWheel ( uiFrame, lParam, rParam ); }
void UiCallback_GuildHousesControl_OnMouseMove  ( CUiFrame* uiFrame, INT64 lParam, INT64 rParam ){ g_pGuildHousesFrame->GuildHousesControl_OnMouseMove  ( uiFrame, lParam, rParam ); }
void UiCallback_GuildHousesControl_OnMouseEnter ( CUiFrame* uiFrame, INT64 lParam, INT64 rParam ){ g_pGuildHousesFrame->GuildHousesControl_OnMouseEnter ( uiFrame, lParam, rParam ); }
void UiCallback_GuildHousesControl_OnMouseLeave ( CUiFrame* uiFrame, INT64 lParam, INT64 rParam ){ g_pGuildHousesFrame->GuildHousesControl_OnMouseLeave ( uiFrame, lParam, rParam ); }
void UiCallback_GuildHousesControl_OnClick      ( CUiFrame* uiFrame, INT64 lParam, INT64 rParam ){ g_pGuildHousesFrame->GuildHousesControl_OnClick      ( uiFrame, lParam, rParam ); }
void UiCallback_GuildHousesControl_OnDoubleClick( CUiFrame* uiFrame, INT64 lParam, INT64 rParam ){ g_pGuildHousesFrame->GuildHousesControl_OnDoubleClick( uiFrame, lParam, rParam ); }
void UiCallback_GuildHousesControl_OnDragBegin  ( CUiFrame* uiFrame, INT64 lParam, INT64 rParam ){ g_pGuildHousesFrame->GuildHousesControl_OnDragBegin  ( uiFrame, lParam, rParam ); }
void UiCallback_GuildHousesControl_OnDragStop   ( CUiFrame* uiFrame, INT64 lParam, INT64 rParam ){ g_pGuildHousesFrame->GuildHousesControl_OnDragStop   ( uiFrame, lParam, rParam ); }
void UiCallback_GuildHousesControl_OnReceiveDrag( CUiFrame* uiFrame, INT64 lParam, INT64 rParam ){ g_pGuildHousesFrame->GuildHousesControl_OnReceiveDrag( uiFrame, lParam, rParam ); }



static int sGuildHouse_GetStyleID(int index,int NpcID);


// ----------------------------------------------------------------------------


int LuaFunc_GuildHouses_VisitHouseRequest		( lua_State* L );
int LuaFunc_GuildHouses_CloseVisitHouse			( lua_State* L );

int LuaFunc_GuildHouses_GetBuildingCount		( lua_State* L );
int LuaFunc_GuildHouses_GetBuilding				( lua_State* L );

int LuaFunc_GuildHouses_CreateBuilding			( lua_State* L );
int LuaFunc_GuildHouses_BuildingUpgrade			( lua_State* L );
int LuaFunc_GuildHouses_DeleteBuilding			( lua_State* L );

int LuaFunc_GuildHouses_GetBuildingResourceCount( lua_State* L );
int LuaFunc_GuildHouses_GetBuildingResource		( lua_State* L );

int LuaFunc_GuildHouses_GetStyleCount( lua_State* L );
int LuaFunc_GuildHouses_GetStyleInfo( lua_State* L );
int LuaFunc_GuildHouses_GetGuildStyle( lua_State* L );

int LuaFunc_GuildHouses_GetBuildingPointCount	( lua_State* L );
int LuaFunc_GuildHouses_GetBuildingPoint		( lua_State* L );
int LuaFunc_GuildHouses_GetBuildingPointInfo	( lua_State* L );

int LuaFunc_GuildHouses_ClearBuildPoint			( lua_State* L );
int LuaFunc_GuildHouses_CreateBuildPoint		( lua_State* L );

int LuaFunc_GuildHouses_GetBuildingPos			( lua_State* L );
int LuaFunc_GuildHouses_GetBuildingPointPos		( lua_State* L );

int LuaFunc_GuildHouses_SetBuildingActive       ( lua_State* L );

//公會銀行
int LuaFunc_GuildBank_PageRequest				( lua_State* L );

int LuaFunc_GuildBank_GetItemInfo				( lua_State* L );
int	LuaFunc_GuildBank_PickupItem				( lua_State* L );
//int LuaFunc_GuildBank_DrawItem					( lua_State* L );
int	LuaFunc_GuildBank_Close						( lua_State* L );

int LuaFunc_GuildBank_GetPageCount				( lua_State* L );
int LuaFunc_GuildBank_GetPageMax				( lua_State* L );
int LuaFunc_GuildBank_GetPageCost				( lua_State* L );
int LuaFunc_GuildBank_BuyPage					( lua_State* L );

int LuaFunc_GuildBank_SetStoreConfigBegin		( lua_State* L );
int LuaFunc_GuildBank_SetStoreConfig			( lua_State* L );
int LuaFunc_GuildBank_SetStoreConfigEnd			( lua_State* L );

int LuaFunc_GuildBank_GetStoreConfig			( lua_State* L );

int LuaFunc_GuildBank_ItemLogRequest			( lua_State* L );
int LuaFunc_GuildBank_GetItemLogCount			( lua_State* L );
int LuaFunc_GuildBank_GetItemLog				( lua_State* L );


//ItemLink
int LuaFunc_GetGuildBankItemLink			    ( lua_State* L );



//公會傢俱
int LuaFunc_GuildHouses_GetFurnitureCount		( lua_State* L );
int LuaFunc_GuildHouses_GetFurnitureInfo		( lua_State* L );
int LuaFunc_GuildHouses_PickupFurniture			( lua_State* L );




int LuaFunc_GuildHouses_SetPlaceFurnitureMode ( lua_State* L );

int LuaFunc_GuildHouses_SetFocusFurnitureID   ( lua_State* L );
int LuaFunc_GuildHouses_GetFocusFurnitureID   ( lua_State* L );


int LuaFunc_GuildHouses_FurniturePlace        ( lua_State* L );
int LuaFunc_GuildHouses_FurnitureRemove       ( lua_State* L );
int LuaFunc_GuildHouses_FurnitureMove         ( lua_State* L );
int LuaFunc_GuildHouses_FurnitureRorare       ( lua_State* L );

int LuaFunc_GuildHouses_FurnitureSendBack     ( lua_State* L );


int LuaFunc_GuildHouses_CanManageBuilding     ( lua_State* L );
int LuaFunc_GuildHouses_CanManageFurniture    ( lua_State* L );
int LuaFunc_GuildHouses_CanSetStyle    ( lua_State* L );

int LuaFunc_GuildHouses_SetMode               ( lua_State* L );

int LuaFunc_GuildHouses_SetStyleMode ( lua_State* L );

int LuaFunc_GuildHouses_ApplyStyle ( lua_State* L );

int LuaFunc_GuildHouses_IsSetStyle  ( lua_State* L ); 



//開放參觀
//int LuaFunc_GuildHouses_IsOpenVisit           ( lua_State* L );
//int LuaFunc_GuildHouses_SetOpenVisit          ( lua_State* L );

//公會戰
int LuaFunc_GuildHousesWar_OpenMenu           ( lua_State* L );
int LuaFunc_GuildHousesWar_Register           ( lua_State* L );
int LuaFunc_GuildHousesWar_MyRegisterInfo           ( lua_State* L );
int LuaFunc_GuildHousesWar_CancelRegister     ( lua_State* L );
int LuaFunc_GuildHousesWar_EnterWar           ( lua_State* L );
int LuaFunc_GuildHousesWar_LeaveWar           ( lua_State* L );
int LuaFunc_GuildHousesWar_PricesRequest      ( lua_State* L );

int LuaFunc_GuildHousesWar_GetInfo            ( lua_State* L ); 

int LuaFunc_GuildHousesWar_GetRegisterCount   ( lua_State* L ); 
int LuaFunc_GuildHousesWar_GetRegisterInfo    ( lua_State* L ); 
int LuaFunc_GuildHousesWar_GetWarHistoryCount   ( lua_State* L ); 
int LuaFunc_GuildHousesWar_GetWarHistoryInfo    ( lua_State* L ); 


int LuaFunc_GuildHousesWar_IsInBattleGround   ( lua_State* L ); 
int LuaFunc_GuildHousesWar_GetGuildWarBattleTime   ( lua_State* L ); 

int LuaFunc_GuildHousesWar_OpenLadder  ( lua_State* L ); 
int LuaFunc_GuildHousesWar_OpenLadderHistory  ( lua_State* L ); 
int LuaFunc_GuildHousesWar_GetLadderCount  ( lua_State* L );
int LuaFunc_GuildHousesWar_GetLadderHistoryCount  ( lua_State* L );
int LuaFunc_GuildHousesWar_GetLadderInfo    ( lua_State* L ); 
int LuaFunc_GuildHousesWar_GetLadderHistoryInfo    ( lua_State* L ); 
int LuaFunc_GuildHousesWar_GetMyLadderInfo    ( lua_State* L ); 
// ----------------------------------------------------------------------------
CGuildHousesFrame::CGuildHousesFrame(CInterface* object)
: CInterfaceBase(object)
{
	m_historyRefTime=-1;
	g_pGuildHousesFrame = this;

	m_pGuildHousesControlFrame = NULL;

	m_CurrentGuildID = -1;

	m_eHousesState = EHousesState_Leave;	
	m_VisitHouseNpcID = -1;

	m_clipRect = NULL;

	//m_eWorkMode = WM_None;

//	m_BuildingID = 0;
//	m_BuildingRoleSpriteID = DF_GuildHousesRoleSpriteWorldGUID;

	m_bBuildingObjectVectorDirty = true;
	m_bBuildingResourceVectorDirty = true;

	m_BuildPointCount = 0;


	m_GuildBankPageCount = 0;
	//m_pickupPageID = -1;
	//m_pickupItemPos = -1;
	m_pickupItemIndex = -1;

	m_GuildBankPageRequested = false;

	m_GuildBankPageLogRequestPageID = 0; //要求的PageID
	m_GuildBankPageLogRequested = false; //
	
 	m_GuildHouseFurnitureChanged = false;
	m_FocusFurnitureID = -1;
	m_MouseEnterFurnitureID = -1;
	m_PlaceFurnitureMode = false;

	m_bFurniturLayoutChanged = false;

	m_bGetAllBuildingInfoOK = false;
	m_FurnitureRoleSpriteID = DF_GuildFurnitureRoleSpriteWorldGUID;

	//CRuVector3 m_GuildBankPos;
	m_bGuildBankOpen = false;

	m_GuildHouseCurrentState = EM_GuildHouseWarState_None;


	m_ZoneGroupID = -1;

	m_ZoneGroupMax = 0;

	m_GuildWarMenuOpenTime = -1;
	m_GuildWarRegistering = false;
	m_OpenMenuResultTimer = -1;

	m_GuildWarStatus = EM_GuildWarStatusType_End;

	m_ladderDelayTime=60*60;

	m_histotyGuildID=0;
	m_LadderHistoryDelayTime=60*60;

	m_LadderInfoDirty=false;
	m_LadderHistoryCount=0;
	m_delayTimer=0;
	
}

// ----------------------------------------------------------------------------
CGuildHousesFrame::~CGuildHousesFrame()
{
	m_LadderInfo.clear();
	
	g_pGuildHousesFrame = NULL;

}

// ----------------------------------------------------------------------------
void CGuildHousesFrame::BuildFrames()
{
	CUi* pUi = m_interface->GetUiWorld();
	//CUiAnchor anchor;
	m_pGuildHousesControlFrame = (CUiFrame*)pUi->CreateObject(CUiLayout::eUiType_Frame, "GuildHousesControlFrame", pUi->GetRoot());
	m_pGuildHousesControlFrame->SetFrameStrata(eUIFrame_Strata_Background);
	m_pGuildHousesControlFrame->SetAnchorSize(CUiSize(0.0f, 0.0f));
	m_pGuildHousesControlFrame->SetAnchor(CUiAnchor());
	m_pGuildHousesControlFrame->SetVisible(false);

	m_pGuildHousesControlFrame->RegisterForClicks( CUiFrame::eMouse_Flag_LButton | CUiFrame::eMouse_Flag_RButton );

	m_pGuildHousesControlFrame->SetMouseEnable( true );

	m_pGuildHousesControlFrame->m_pfOnMouseDown   = UiCallback_GuildHousesControl_OnMouseDown   ;
	m_pGuildHousesControlFrame->m_pfOnMouseUp     = UiCallback_GuildHousesControl_OnMouseUp     ;
	m_pGuildHousesControlFrame->m_pfOnMouseWheel  = UiCallback_GuildHousesControl_OnMouseWheel  ;
	m_pGuildHousesControlFrame->m_pfOnMouseMove   = UiCallback_GuildHousesControl_OnMouseMove   ;
	m_pGuildHousesControlFrame->m_pfOnMouseEnter  = UiCallback_GuildHousesControl_OnMouseEnter  ;
	m_pGuildHousesControlFrame->m_pfOnMouseLeave  = UiCallback_GuildHousesControl_OnMouseLeave  ;
	m_pGuildHousesControlFrame->m_pfOnClick       = UiCallback_GuildHousesControl_OnClick       ;
	m_pGuildHousesControlFrame->m_pfOnDoubleClick = UiCallback_GuildHousesControl_OnDoubleClick ;
	m_pGuildHousesControlFrame->m_pfOnDragBegin   = UiCallback_GuildHousesControl_OnDragBegin   ;
	m_pGuildHousesControlFrame->m_pfOnDragStop    = UiCallback_GuildHousesControl_OnDragStop    ; 
	m_pGuildHousesControlFrame->m_pfOnReceiveDrag = UiCallback_GuildHousesControl_OnReceiveDrag ; 


	map< int , GuildHouseNpcReplaceCostTableStruct >::iterator it=g_ObjectData->_GuildHouseNpcReplaceCost.begin();


	for (;it!=g_ObjectData->_GuildHouseNpcReplaceCost.end();++it)
	{
		m_Styles.push_back(&it->second);
	}
}

// ----------------------------------------------------------------------------
void CGuildHousesFrame::DestroyFrames()
{
	//m_eHousesState = EHousesState_Leave;	
	m_Styles.clear();

	m_pGuildHousesControlFrame = NULL;
}

// ----------------------------------------------------------------------------
void CGuildHousesFrame::RegisterFunc()
{

	lua_State* L = GetLuaStateWorld();
	if (L)
	{
		lua_register( L, "GuildHouses_VisitHouseRequest"		, LuaFunc_GuildHouses_VisitHouseRequest			);
		lua_register( L, "GuildHouses_CloseVisitHouse"			, LuaFunc_GuildHouses_CloseVisitHouse			);

		lua_register( L, "GuildHouses_GetBuildingCount"			, LuaFunc_GuildHouses_GetBuildingCount			);
		lua_register( L, "GuildHouses_GetBuilding"				, LuaFunc_GuildHouses_GetBuilding				);

   		lua_register( L, "GuildHouses_CreateBuilding"			, LuaFunc_GuildHouses_CreateBuilding	        );
   		lua_register( L, "GuildHouses_BuildingUpgrade"			, LuaFunc_GuildHouses_BuildingUpgrade	        );

   		lua_register( L, "GuildHouses_DeleteBuilding"			, LuaFunc_GuildHouses_DeleteBuilding		    );

		lua_register( L, "GuildHouses_GetBuildingResourceCount"	, LuaFunc_GuildHouses_GetBuildingResourceCount	);
		lua_register( L, "GuildHouses_GetBuildingResource"		, LuaFunc_GuildHouses_GetBuildingResource		);

		lua_register( L, "GuildHouses_GetStyleCount"	, LuaFunc_GuildHouses_GetStyleCount	);
		lua_register( L, "GuildHouses_GetStyleInfo"		, LuaFunc_GuildHouses_GetStyleInfo		);
		lua_register( L, "GuildHouses_GetGuildStyle"		, LuaFunc_GuildHouses_GetGuildStyle		);


		lua_register( L, "GuildHouses_GetBuildingPointCount"	, LuaFunc_GuildHouses_GetBuildingPointCount		);
		lua_register( L, "GuildHouses_GetBuildingPoint"			, LuaFunc_GuildHouses_GetBuildingPoint			);
		lua_register( L, "GuildHouses_GetBuildingPointInfo"     , LuaFunc_GuildHouses_GetBuildingPointInfo      );

   		lua_register( L, "GuildHouses_ClearBuildPoint"			, LuaFunc_GuildHouses_ClearBuildPoint			);
		lua_register( L, "GuildHouses_CreateBuildPoint"			, LuaFunc_GuildHouses_CreateBuildPoint			);
	
		lua_register( L, "GuildHouses_GetBuildingPos"			, LuaFunc_GuildHouses_GetBuildingPos			);
		lua_register( L, "GuildHouses_GetBuildingPointPos"		, LuaFunc_GuildHouses_GetBuildingPointPos		);
	
		lua_register( L, "GuildHouses_SetBuildingActive"		, LuaFunc_GuildHouses_SetBuildingActive         );


		

		lua_register( L, "GuildBank_PageRequest"				, LuaFunc_GuildBank_PageRequest					);
  		lua_register( L, "GuildBank_GetItemInfo"				, LuaFunc_GuildBank_GetItemInfo					);
		lua_register( L, "GuildBank_PickupItem"					, LuaFunc_GuildBank_PickupItem					);
		lua_register( L, "GuildBank_Close"						, LuaFunc_GuildBank_Close						);

	 	lua_register( L, "GuildBank_GetPageCount"				, LuaFunc_GuildBank_GetPageCount				);
		lua_register( L, "GuildBank_GetPageMax"					, LuaFunc_GuildBank_GetPageMax					);
		lua_register( L, "GuildBank_GetPageCost"				, LuaFunc_GuildBank_GetPageCost					);
		lua_register( L, "GuildBank_BuyPage"					, LuaFunc_GuildBank_BuyPage						);
	
		lua_register( L, "GuildBank_SetStoreConfigBegin"		, LuaFunc_GuildBank_SetStoreConfigBegin			);
		lua_register( L, "GuildBank_SetStoreConfig"				, LuaFunc_GuildBank_SetStoreConfig				);
		lua_register( L, "GuildBank_SetStoreConfigEnd"			, LuaFunc_GuildBank_SetStoreConfigEnd			);
		lua_register( L, "GuildBank_GetStoreConfig"				, LuaFunc_GuildBank_GetStoreConfig				);

		lua_register( L, "GuildBank_ItemLogRequest"				, LuaFunc_GuildBank_ItemLogRequest				);
		lua_register( L, "GuildBank_GetItemLogCount"			, LuaFunc_GuildBank_GetItemLogCount				);
		lua_register( L, "GuildBank_GetItemLog"					, LuaFunc_GuildBank_GetItemLog					);

		lua_register( L, "GetGuildBankItemLink"					, LuaFunc_GetGuildBankItemLink					);

		lua_register( L, "GuildHouses_GetFurnitureCount"		, LuaFunc_GuildHouses_GetFurnitureCount			);
		lua_register( L, "GuildHouses_GetFurnitureInfo"			, LuaFunc_GuildHouses_GetFurnitureInfo			);
		lua_register( L, "GuildHouses_PickupFurniture"			, LuaFunc_GuildHouses_PickupFurniture			);
 
		lua_register( L, "GuildHouses_SetPlaceFurnitureMode"	, LuaFunc_GuildHouses_SetPlaceFurnitureMode		);
		lua_register( L, "GuildHouses_SetFocusFurnitureID"		, LuaFunc_GuildHouses_SetFocusFurnitureID		);
		lua_register( L, "GuildHouses_GetFocusFurnitureID"		, LuaFunc_GuildHouses_GetFocusFurnitureID		);
		lua_register( L, "GuildHouses_FurniturePlace"			, LuaFunc_GuildHouses_FurniturePlace			);
		lua_register( L, "GuildHouses_FurnitureRemove"			, LuaFunc_GuildHouses_FurnitureRemove			);
		lua_register( L, "GuildHouses_FurnitureMove"			, LuaFunc_GuildHouses_FurnitureMove				);
		lua_register( L, "GuildHouses_FurnitureRorare"			, LuaFunc_GuildHouses_FurnitureRorare			);

		lua_register( L, "GuildHouses_FurnitureSendBack"		, LuaFunc_GuildHouses_FurnitureSendBack			);

		lua_register( L, "GuildHouses_CanManageBuilding"		, LuaFunc_GuildHouses_CanManageBuilding			);
		lua_register( L, "GuildHouses_CanManageFurniture"		, LuaFunc_GuildHouses_CanManageFurniture		);

		lua_register( L, "GuildHouses_CanSetStyle"				, LuaFunc_GuildHouses_CanSetStyle				);
		lua_register( L, "GuildHouses_SetMode"                  , LuaFunc_GuildHouses_SetMode				    );
		lua_register( L, "GuildHouses_SetStyleMode"             , LuaFunc_GuildHouses_SetStyleMode              );
		lua_register( L, "GuildHouses_ApplyStyle"               , LuaFunc_GuildHouses_ApplyStyle                );
		lua_register( L, "GuildHouses_IsSetStyle"               , LuaFunc_GuildHouses_IsSetStyle                );

		lua_register( L, "GuildHousesWar_OpenLadder"               , LuaFunc_GuildHousesWar_OpenLadder                );
		lua_register( L, "GuildHousesWar_OpenLadderHistory"        , LuaFunc_GuildHousesWar_OpenLadderHistory         );

		lua_register( L, "GuildHousesWar_GetLadderCount"			, LuaFunc_GuildHousesWar_GetLadderCount         );
		lua_register( L, "GuildHousesWar_GetLadderHistoryCount"     , LuaFunc_GuildHousesWar_GetLadderHistoryCount  );
		lua_register( L, "GuildHousesWar_GetLadderInfo"				, LuaFunc_GuildHousesWar_GetLadderInfo         );
		lua_register( L, "GuildHousesWar_GetLadderHistoryInfo"      , LuaFunc_GuildHousesWar_GetLadderHistoryInfo  );
		

		//lua_register( L, "GuildHouses_IsOpenVisit"				, LuaFunc_GuildHouses_IsOpenVisit				);
		//lua_register( L, "GuildHouses_SetOpenVisit"				, LuaFunc_GuildHouses_SetOpenVisit				);

		lua_register( L, "GuildHousesWar_OpenMenu"         , LuaFunc_GuildHousesWar_OpenMenu              );
		lua_register( L, "GuildHousesWar_Register"         , LuaFunc_GuildHousesWar_Register              );
		lua_register( L, "GuildHousesWar_CancelRegister"   , LuaFunc_GuildHousesWar_CancelRegister        );
		lua_register( L, "GuildHousesWar_EnterWar"         , LuaFunc_GuildHousesWar_EnterWar              );
		lua_register( L, "GuildHousesWar_LeaveWar"         , LuaFunc_GuildHousesWar_LeaveWar              );
		lua_register( L, "GuildHousesWar_PricesRequest"    , LuaFunc_GuildHousesWar_PricesRequest         );
  
		
		lua_register( L, "GuildHousesWar_GetInfo"          , LuaFunc_GuildHousesWar_GetInfo               );

		lua_register( L, "GuildHousesWar_GetRegisterCount" , LuaFunc_GuildHousesWar_GetRegisterCount      );
		lua_register( L, "GuildHousesWar_GetRegisterInfo"  , LuaFunc_GuildHousesWar_GetRegisterInfo       );
		lua_register( L, "GuildHousesWar_MyRegisterInfo"  , LuaFunc_GuildHousesWar_MyRegisterInfo       );


		
		lua_register( L, "GuildHousesWar_GetWarHistoryCount" , LuaFunc_GuildHousesWar_GetWarHistoryCount      );
		lua_register( L, "GuildHousesWar_GetWarHistoryInfo"  , LuaFunc_GuildHousesWar_GetWarHistoryInfo       );

		lua_register( L, "GuildHousesWar_IsInBattleGround" , LuaFunc_GuildHousesWar_IsInBattleGround      );
		lua_register( L, "GuildHousesWar_GetGuildWarBattleTime" , LuaFunc_GuildHousesWar_GetGuildWarBattleTime      );

		lua_register( L, "GuildHousesWar_GetMyLadderInfo" , LuaFunc_GuildHousesWar_GetMyLadderInfo      );
		

	} 
}

// ----------------------------------------------------------------------------
void CGuildHousesFrame::ReloadFrames() // 界面重新被載入後觸發
{
	if( m_eHousesState == EHousesState_Enter )
	{
		if( m_CurrentGuildID == CNetGlobal::RoleData()->BaseData.GuildID )
		{
			m_pGuildHousesControlFrame->SetVisible(true);
			SendWorldEvent( UiEvent_GuildHousesFrameShow );
		}
 	}

}

// ----------------------------------------------------------------------------
void CGuildHousesFrame::LeaveWorld()
{
	if( m_eHousesState == EHousesState_Enter ) //目前狀態 在房屋中 => 離開房屋
	{
   		m_eHousesState = EHousesState_Leave;
 		ruSAFE_RELEASE( m_clipRect );
		
		SendWorldEvent( UiEvent_GuildHousesFrameHide );

		ClearBuildPoint();

		m_CurrentGuildID = -1;

		m_FocusFurnitureID = -1;
		m_MouseEnterFurnitureID = -1;

		m_pGuildHousesControlFrame->SetVisible(false);

		m_GuildBankPages.clear();
		m_LogTemp.clear();

		m_mapBuildingActive.clear();

		m_bGetAllBuildingInfoOK = false;
 	}

	m_GuildWarStatus = EM_GuildWarStatusType_End;
	m_GuildWarRegistering = false;
	m_OpenMenuResultTimer = -1;
}

// ----------------------------------------------------------------------------
void CGuildHousesFrame::Update(float elapsedTime)
{
	// ----------------------------------------------------------------------------
	//參觀工會屋介面
	static float lastTime = 0.0f;
	lastTime += elapsedTime;

	static float deltime=0 ;
	deltime+=elapsedTime;

	m_ladderDelayTime+=elapsedTime;
	m_LadderHistoryDelayTime+=elapsedTime;
	GuildBaseStruct* pGuildInfo =  g_pGuildFrame->GetGuildBaseInfo();
	if (pGuildInfo&&pGuildInfo->HousesWar.NextWarTime>0)
	{
		static float s_deltime=0 ;
		s_deltime+=elapsedTime;
		if (s_deltime>=1)
		{
			s_deltime=0;
			pGuildInfo->HousesWar.NextWarTime-=1;
		}
	}

	if (m_LadderInfoDirty)
	{
		m_delayTimer+=elapsedTime;
		if (m_delayTimer>5)
		{
			m_LadderInfoDirty=false;

			CalRankList();


		}
	}
#define _DEBUG_LADDER 0
#if(_DEBUG_LADDER==1)
	static bool s_bsim=true;

	if (s_bsim)
	{
		static float  s_bTime=5;
		s_bTime-=elapsedTime;
		if (s_bTime<0)
		{
			s_bTime=(rand()%10)+10;
			char temp[512];
			int guildID=0;
			int count=(rand()%10)+1;
			GuildWarRankInfoStruct info;
			for(int i=0;i<count;i++)
			{			
				int sever=(rand()%3);//* 0x1000000 ;
				int guild=(rand()%1000);
				sprintf_s(temp,512,"[Test %d:%d]",sever,guild);
				info.GuildName=temp;

				info.WinCount=rand()%100;
				info.LostCount=rand()%100;
				info.EvenCount=rand()%100;

				info.Score=info.WinCount*5+info.EvenCount*2;
				
				info.WorldGuildID=sever*0x1000000+guild;
				GuildWarLadderResult(info);
			}
		}
	}


	
#endif
	if (deltime>=60)
	{
		deltime=0;
		m_NextTime-=60;
		lua_State* L = GetLuaState();
		lua_pushnumber( L, m_NextTime );
		lua_setglobal( L, "arg1" );
		SendWorldEvent( "UPDATE_GUILDWARTIME" );
	}
	
	if ( lastTime > 0.2f  && m_VisitHouseNpcID != -1 )
	{
		lastTime = 0.0f;
		if ( g_pGameMain->GetDistanceWithPlayer(m_VisitHouseNpcID) > _MAX_ROLE_TOUCH_MAXRANGE_ )
		{
			SendWorldEvent( UiEvent_GuildHousesCloseVisitHouse );
			m_VisitHouseNpcID = -1;
		}
	}
	
	m_historyRefTime-=elapsedTime;
	if( m_eHousesState != EHousesState_Enter )
	{
		return;
	}
 
	//載入盆栽
	if( m_mapFlowerPotInfo.size() > 0 )
	{
		map<int,ItemFieldStruct>::iterator itPos = m_mapFlowerPotInfo.begin();
		map<int,ItemFieldStruct>::iterator itEnd = m_mapFlowerPotInfo.end();

		for( ; itPos != itEnd ; ++itPos )
		{
			int GItemID = itPos->first;
			ItemFieldStruct& Item = itPos->second;
				
			CRoleSprite* sprite = g_pGameMain->FindSpriteByID( GItemID );
			if ( sprite == NULL )
				continue;

			CRuEntity* pEntity = sprite->GetRuEntity();
			if ( pEntity == NULL )
				continue;

			g_pHousesFrame->Houses_CreatePlant( pEntity, Item );
		}

		m_mapFlowerPotInfo.clear();
	}

	// ----------------------------------------------------------------------------
	//公會銀行
	if ( m_pickupItemIndex != -1  )
	{
		ItemFieldStruct* item = GetGuildBankItem( m_pickupItemIndex );
		if ( item )
		{
			if ( item->Pos == EM_ItemState_CliDisabled )
			{
				ClipboardItemInfo itemInfo = g_pItemClipboard->GetItemInfo();
				if ( g_pItemClipboard->GetType() != eClipboardItem_Type_GuildBank || itemInfo.guildbank.pos != m_pickupItemIndex )
				{
					item->Pos = EM_ItemState_NONE;
					m_pickupItemIndex = -1;
				}					
			}
			else
			{
				m_pickupItemIndex = -1;
			}

		}
		else
		{
			m_pickupItemIndex = -1;
		}
	}

	//公會銀行
	if( m_bGuildBankOpen )
	{
		CRoleSprite* pPlayer = g_pGameMain->GetPlayer();
		if( NULL == pPlayer || 
			( pPlayer->GetPos() - m_GuildBankPos ).Magnitude() > _MAX_ROLE_TOUCH_MAXRANGE_ )
		{
			m_bGuildBankOpen = false;
			SendWorldEvent( UiEvent_GuildBankClose );
		}
	}


	// ----------------------------------------------------------------------------
	//家具
	if( m_GuildHouseFurnitureChanged )
	{
		m_GuildHouseFurnitureChanged = false;

		SendWorldEvent( UiEvent_GuildHouseFurnitureChanged );
	}





	// ----------------------------------------------------------------------------
	//if( m_eHousesState == EHousesState_Enter )
	//{
	//	if( m_eWorkMode == WM_CreateBuildingToGround )
	//	{
	//		CRoleSprite* pRoleSpriteBuilding = g_pGameMain->FindSpriteByWorldGUID( m_BuildingRoleSpriteID );

	//		if( !pRoleSpriteBuilding || !pRoleSpriteBuilding->IsVisible() )
	//		{
	//			m_eWorkMode = WM_None;
	//			return;
	//		}
	//		
	//		// 參數
	//		INT32 x = g_pRuneDev->GetCursorPtX();		// 滑鼠x
	//		INT32 y = g_pRuneDev->GetCursorPtY();		// 滑鼠y
	//		IRuSwapChain *swapChain = g_pRuneDev->GetRuSwapChain();		// 由CRuneDev取得IRuSwapChain
	//		CRuCamera* camera = g_pRuneDev->GetCamera();	
	//		// 將滑鼠座標轉換成世界座標
	//		CRuVector3 homPt, viewPt, worldPt;
	//		camera->Coord_ScreenToHomogeneous(x, y, swapChain->GetWidth(), swapChain->GetHeight(), homPt);
	//		camera->Coord_HomogeneousToView(homPt, viewPt);
	//		camera->Coord_ViewToWorld(viewPt, worldPt);

	//		// 建構碰撞測試結構
	//		CRuCollisionQuery_Segment_Nearest query;

	//		// 設定測試用線段及其他設定
	//		CRuVector3 pickDir = (worldPt - camera->GetCameraPosition()).Normalize();
	//		query.m_colSegment = CRuSegment3(worldPt, worldPt + (pickDir * 1000.0f));
	//		query.m_ignoreBackface = TRUE;

	//		// 測試碰撞
	//		g_pGameMain->GetRuTerrain()->QueryCollision(&query);
	//		
	//		if( query.m_positiveHit )
	//		{

	//			CRuVector3 Point = query.m_collisionPoint;

	//			Point.x = ((float)(int)( Point.x / DF_GuildHouses_GroundInterval ) ) * DF_GuildHouses_GroundInterval + DF_GuildHouses_GroundInterval / 2 ;
 //				Point.z = ((float)(int)( Point.z / DF_GuildHouses_GroundInterval ) ) * DF_GuildHouses_GroundInterval + DF_GuildHouses_GroundInterval / 2 ;

	//			pRoleSpriteBuilding->SetPos( Point );
	//			pRoleSpriteBuilding->SetOrientingAxis( query.m_collisionNormal );
	//			pRoleSpriteBuilding->UpdateOrientation();
	//			pRoleSpriteBuilding->Update(0);
	//		}

	//	}
	//}


}

// ----------------------------------------------------------------------------
void CGuildHousesFrame::Render()
{
	if( m_eHousesState != EHousesState_Enter )
	{
		return ;
	}

	if( m_clipRect )
	{
		g_ruDefaultRenderContext->Submit(m_clipRect);
	}
	
}

// ----------------------------------------------------------------------------
bool CGuildHousesFrame::IsHousesLoading()
{
	if( m_eHousesState == EHousesState_Leave )
	{
		return false;
	}

	if( m_eHousesState == EHousesState_Loading )
	{
		return true;
	}

//	if( m_eHousesState != EHousesState_LoadOK )
//	{
//		return false;
//	}

	if( false == m_bGetAllBuildingInfoOK )
	{
		return true;
	}

	//檢查物件是否都載完了
	CRoleSprite* pSpritePlayer = g_pGameMain->GetPlayer();
	
	if( NULL == pSpritePlayer )
	{
		return true;
	}

	if( pSpritePlayer->IsProxyLoadComplete() == false )
	{
		return true;
	}


	//pSprite->GetObjectType()
	set<IBaseSprite*>& rContainers = IBaseSprite::GetContainer();

	IBaseSprite* pSprite;
	int nObjectType;

	set<IBaseSprite*>::iterator itPos = rContainers.begin();
	set<IBaseSprite*>::iterator itEnd = rContainers.end();

	CRuAABB bounds(-10000.0f, -10000.0f, -10000.0f, 10000.0f, 10000.0f, 10000.0f);
	CRuOptimalTerrain* pTerrain = g_pGameMain->GetRuTerrain();
	if ( pTerrain )
	{
		bounds = pTerrain->GetEnumerationBounds();	
	}

	for( ; itPos != itEnd ; ++itPos )
	{
		pSprite = *itPos;

		if( NULL == pSprite )
		{
			continue;
		}

		nObjectType = pSprite->GetObjectType();

		if( nObjectType == eSpriteObject_NPC ||
			nObjectType == eSpriteObject_QuestNPC )
		{
			if ( pSprite->IsProxyLoadComplete() == false && bounds.Contains(pSprite->GetPos()) == TRUE )
			{
				return true;
			}
		}
 	}



	/*
	CRoleSprite* pSpriteBuilding;

	map<int,GuildBuildingObjectInfoStruct>::iterator itPos = m_mapGuildBuildingObjectInfo.begin();
	map<int,GuildBuildingObjectInfoStruct>::iterator itEnd = m_mapGuildBuildingObjectInfo.end();

	for( ; itPos != itEnd ; ++itPos )
	{
		GuildBuildingObjectInfoStruct& rInfo = itPos->second;
		pSpriteBuilding = g_pGameMain->FindSpriteByID( rInfo.GItemID );

		if( NULL == pSpriteBuilding )
		{
			continue;
		}

		if( pSpriteBuilding->IsProxyLoadComplete() == false )
		{
			return true;
		}
	}
	*/

	//CRuContainer_Collision* pCollision = IBaseSprite::GetCollisionContainer();

	//if ( NULL == pCollision )
	//{
	//	return true;
	//}
	//
	//CRuVector3 position = pSpritePlayer->GetPos();
	//position.m_y = 20.0f;

	//float fHeight = RuEntity_FindRestingHeight( position, pCollision, 50.0f );
	//if ( abs( fHeight - position.m_y) < 50.0f )
	//	position.m_y = fHeight;

	//pSpritePlayer->SetPos( position );

	return false;
}

// ----------------------------------------------------------------------------
float CGuildHousesFrame::ClientLoadingOK()
{
	if( m_eHousesState != EHousesState_LoadOK ) //目前狀態 //準備進入 => 進入房屋
	{
		return 0.0f;
	}

	/*
	// 建立碰撞用容器
	CRuContainer_Collision *colContainer;
	colContainer = ruNEW CRuContainer_Collision();

	// 將碰撞容器清除
	colContainer->ClearCollisionObjects();

	// 將要有碰撞的物件加入容器
	//colContainer->InsertCollisionObject(entity1);
	//colContainer->InsertCollisionObject(entity2);


	colContainer->InsertCollisionObject( g_pGameMain->GetRuTerrain() );

	//CRuOptimalTerrain*	GetRuTerrain()								{ return m_pRuTerrain; } 


	/*
	// 建立棋盤物件
	//CRuEntity_Widget_ClipRect *m_clipRect;
	m_clipRect = ruNEW CRuEntity_Widget_ClipRect();
   
	// 設定碰撞物件
	m_clipRect->SetTargetObject(colContainer);
   
	ruSAFE_RELEASE( colContainer );
	
	// 設定碰撞範圍
	CRuOBB obb;
	obb.Extents()[0] = DF_GuildHouses_GroundSizeX * DF_GuildHouses_GroundInterval / 2 ;
	obb.Extents()[1] = 100;
	obb.Extents()[2] = DF_GuildHouses_GroundSizeZ * DF_GuildHouses_GroundInterval / 2 ;

	m_clipRect->SetClipBox(obb);

	// 設定棋盤間隔(0 = DISABLE)
	m_clipRect->SetGridInterval( DF_GuildHouses_GroundInterval );

	// 移動棋盤位置

	CRuVector3 pos;
	pos.x = DF_GuildHouses_GroundPosX + DF_GuildHouses_GroundSizeX * DF_GuildHouses_GroundInterval / 2;
	pos.y = 0;
 	pos.z = DF_GuildHouses_GroundPosZ + DF_GuildHouses_GroundSizeZ * DF_GuildHouses_GroundInterval / 2;

	m_clipRect->Translate( pos, ruTRANSFORMOP_SET );
 
	m_clipRect->Update( 0.0 );
	//g_ruDefaultRenderContext->Submit(m_clipRect);
	*/
 	
	m_eHousesState = EHousesState_Enter;

	if( m_CurrentGuildID == CNetGlobal::RoleData()->BaseData.GuildID )
	{
 		SendWorldEvent( UiEvent_GuildHousesFrameShow );
		m_pGuildHousesControlFrame->SetVisible(true);
	}

	//把玩家拉高
	//CRoleSprite* pSpriteBuilding;

	//map<int,GuildBuildingObjectInfoStruct>::iterator itPos = m_mapGuildBuildingObjectInfo.begin();
	//map<int,GuildBuildingObjectInfoStruct>::iterator itEnd = m_mapGuildBuildingObjectInfo.end();

	//for( ; itPos != itEnd ; ++itPos )
	//{
	//	GuildBuildingObjectInfoStruct& rInfo = itPos->second;
	//	pSpriteBuilding = g_pGameMain->FindSpriteByID( rInfo.GItemID );

	//	if( NULL == pSpriteBuilding )
	//	{
	//		return true;
	//	}

	//	if( pSpriteBuilding->IsProxyLoadComplete() == false )
	//	{
	//		return true;
	//	}
	//}

	//修正所有玩家位置

	CRuContainer_Collision* pCollision = IBaseSprite::GetCollisionContainer();

	if ( pCollision )
	{
		//CRoleSprite* pSpritePlayer = g_pGameMain->GetPlayer();
		//CRoleSprite* spriteControlProxy = pSpritePlayer->GetProxyControlSprite();

		//CRuVector3 position = spriteControlProxy->GetPos();
		//position.m_y = 20.0f;

		//float fHeight = RuEntity_FindRestingHeight( position, pCollision, 50.0f );
		//if ( abs( fHeight - position.m_y) < 50.0f )
		//	position.m_y = fHeight;

		//spriteControlProxy->SetPos( position );


		set<IBaseSprite*>& rContainers = IBaseSprite::GetContainer();
		IBaseSprite* pSprite;
		int nObjectType;

		set<IBaseSprite*>::iterator itPos = rContainers.begin();
		set<IBaseSprite*>::iterator itEnd = rContainers.end();

		for( ; itPos != itEnd ; ++itPos )
		{
			pSprite = *itPos;

			if( NULL == pSprite )
			{
				continue;
			}

			nObjectType = pSprite->GetObjectType();

			if( nObjectType == eSpriteObject_Player )
			{
				CRoleSprite* pSpritePlayer = (CRoleSprite*)pSprite;
				CRoleSprite* spriteControlProxy = pSpritePlayer->GetProxyControlSprite();

				CRuVector3 position = spriteControlProxy->GetPos();

				//add Partition 時的位置
				CRuVector3* pPartitionPosition = g_pGameMain->FindPartitionPosition( spriteControlProxy );
				if( pPartitionPosition )
				{
					position = *pPartitionPosition;
				}
	
				position.m_y +=  20.0f;

				float fHeight = RuEntity_FindRestingHeight( position, pCollision, 50.0f );
				if ( abs( fHeight - position.m_y) < 50.0f )
					position.m_y = fHeight;

				spriteControlProxy->SetPos( position );
			}
		}
   
	}

	//修正所有玩家
	//CRuVector3* CGameMain::FindPartitionPosition(CRoleSprite* sprite)
  
	return 0.0;
}

// ----------------------------------------------------------------------------
CursorType CGuildHousesFrame::GetGuildHousesCursor()
{
	if( ( m_ePlaceingMode == EM_PlaceingMode_New && m_PlaceStep == 0 ) || 
		( m_ePlaceingMode == EM_PlaceingMode_Move && m_PlaceStep == 1 ) )
	{
		return eCursor_FurnitureMove;
	}
	if( ( m_ePlaceingMode == EM_PlaceingMode_New && m_PlaceStep == 1 ) || 
		( m_ePlaceingMode == EM_PlaceingMode_Rorare && m_PlaceStep == 1 ) )
	{
		return eCursor_FurnitureRorare;
	}

	//還沒打開家具擺設介面時，移動到家具可作用的家具上，滑鼠游標要變成齒輪
	//打開家具擺設介面時 強制POINT
	if( m_PlaceFurnitureMode && m_MouseEnterFurnitureID != -1 )
	{
		if( m_ePlaceingMode == EM_PlaceingMode_Move )
		{
			return eCursor_FurnitureMove;
		}

		if( m_ePlaceingMode == EM_PlaceingMode_Rorare )
		{
			return eCursor_FurnitureRorare;
		}
	}

	return eCursor_Null;
}
class CompGuildHouseWarRankInfo  
{  
public:  
	CompGuildHouseWarRankInfo(std::map<int,GuildWarRankInfoStruct>& info):_info(info)
	{

	}
	int type;

	std::map<int,GuildWarRankInfoStruct>& _info;
	// Return whether first element is less than the second  
	bool operator () (int Guild_A,int Guild_B) const  
	{  
		GuildWarRankInfoStruct &a=_info[Guild_A];
		GuildWarRankInfoStruct &b=_info[Guild_B];

		return a.Score>b.Score;  
	};  
}; 
void CGuildHousesFrame::CalRankList()
{

	int count=0;
	int size=(int)m_LadderInfo.size();
	m_ladderList.clear();
	m_ladderList.resize(size);
	std::map<int,GuildWarRankInfoStruct>::iterator it= m_LadderInfo.begin();
	for(;it!=m_LadderInfo.end();++it)
	{

		m_ladderList[count]=it->first;
		count++;

	}

	std::sort( m_ladderList.begin() , m_ladderList.end() , CompGuildHouseWarRankInfo(m_LadderInfo) );


	SendWorldEvent("GUILDHOUSEWAR_LADDER_UPDATE");
}
// ----------------------------------------------------------------------------
void CGuildHousesFrame::RL_BuildHouseResult( int GuildID , bool Result )
{
	if( Result )
	{
	//	BaseRoleData* pBaseData = &CNetGlobal::RoleData()->BaseData;
	//	pBaseData->GuildID = GuildID;
  		g_pGameMain->SendSystemChatMsg( g_ObjectData->GetString( "SYS_BUILD_HOUSE_SUCCESS" ) );
	}
	else
	{
		g_pGameMain->SendSystemChatMsg( g_ObjectData->GetString( "SYS_BUILD_HOUSE_FAIL" ) );
	}
}

// ----------------------------------------------------------------------------
void CGuildHousesFrame::RL_OpenVisitHouse( int TargetNpcID , bool IsVisitMyGuildHouse )
{
	m_VisitHouseNpcID = TargetNpcID;

	if( IsVisitMyGuildHouse )
	{
		BaseRoleData* pBaseData = &CNetGlobal::RoleData()->BaseData;

		if ( pBaseData->GuildID == 0 )
		{
			return ;
		}
		
		NetCli_GuildHouses::SL_VisitHouseRequest( pBaseData->GuildID );
	}
	else
	{
		//開對話盒
		SendWorldEvent( UiEvent_GuildHousesOpenVisitHouse );
	}	
}

// ----------------------------------------------------------------------------
void CGuildHousesFrame::RL_FindHouseResult( bool Result )
{
	
}

// ----------------------------------------------------------------------------
void CGuildHousesFrame::RL_CloseVisitHouse( )
{
	m_VisitHouseNpcID = -1;
	SendWorldEvent( UiEvent_GuildHousesCloseVisitHouse );	
}

// ----------------------------------------------------------------------------
void CGuildHousesFrame::RL_HouseInfoLoading( int GuildID )
{
	m_eHousesState = EHousesState_Loading;	

	m_mapBuildingDBIDGItemID.clear();
	m_mapFurnitureDBIDGItemID.clear();
	m_GuildHouseFurnitureList.clear();


	m_mapGuildBuildingObjectInfo.clear();
	m_vecGuildBuildingObjectInfo.clear();

	m_bBuildingObjectVectorDirty = true;

	m_mapBuildPointGItemID.clear();
	m_BuildPointCount = 0;

	m_GuildBankPageCount = 0;
	m_GuildBankPages.clear();

 	m_pickupItemIndex = -1;

 	m_GuildHouseFurnitureChanged = false;
	m_MouseEnterFurnitureID = -1;
	m_PlaceFurnitureMode = false;

	m_bFurniturLayoutChanged = false;

	m_CurrentGuildID = GuildID;
	m_guildStyle=-1;

}

// ----------------------------------------------------------------------------
void CGuildHousesFrame::RL_HouseInfoLoadOK( )
{
	m_eHousesState = EHousesState_LoadOK;	
	m_bGetAllBuildingInfoOK = false;
	NetCli_GuildHouses::SL_AllBuildingInfoRequest( CNetGlobal::RoleData()->BaseData.GuildID );

	NetCli_GuildHouses::SL_HouseStyleRequest();
}
//
static int _GetHouseStyleIndex(int style)
{

	int count=(int)g_pGuildHousesFrame->m_Styles.size();

//	GuildHouseNpcReplaceCostTableStruct** list=g_pGuildHousesFrame->m_Styles.begin();
	for (int i=0;i<count;i++)
	{
		if (g_pGuildHousesFrame->m_Styles[i]->Type==style)
			return i;

	}
	return -1;

}
  
 void CGuildHousesFrame::HouseStyle( int Type )
 {

	 m_guildStyle=_GetHouseStyleIndex(Type);

 }

// ----------------------------------------------------------------------------
void CGuildHousesFrame::RL_CreateBuildingResult( char* Point , int BuildingID , int BuildingDBID, int ParentBuildingDBID , bool Result )
{
	if( Result )
	{
		GuildBuildingInfoTableStruct* pBuildingResource = GetGuildBuildingResourceByID( BuildingID );

		if( NULL == pBuildingResource )
		{
			return ;
		}

		GameObjDbStructEx* pObjDB = g_ObjectData->GetObj( pBuildingResource->OrgObjID );
		if ( pObjDB == NULL )
			return ;
	 
		g_pGameMain->SendSystemChatMsgEx( g_ObjectData->GetString( "SYS_GUILDHOUSES_CREATE_BUILDING_SUCCESS" ), pObjDB->GetName() );
  	}

}

// ----------------------------------------------------------------------------
void CGuildHousesFrame::RL_BuildingUpgradeResult( int GItemID , int BuildingDBID , int UpgradeBuildingID , int Result )
{
	if( false == Result )
	{
		return;
	}


	map<int,GuildBuildingObjectInfoStruct>::iterator itPos = m_mapGuildBuildingObjectInfo.find( BuildingDBID );
	if( itPos == m_mapGuildBuildingObjectInfo.end() )
	{
		return;
	}

	GuildBuildingObjectInfoStruct& rInfo = itPos->second;
	rInfo.BuildingID = UpgradeBuildingID;
	rInfo.GItemID = GItemID;

	//不必更新Vector
	//m_bBuildingObjectVectorDirty = true;

   
	GuildBuildingInfoTableStruct* pBuildingResource = GetGuildBuildingResourceByID( UpgradeBuildingID );

	if( NULL == pBuildingResource )
	{
		return ;
	}

	GameObjDbStructEx* pObjDB = g_ObjectData->GetObj( pBuildingResource->OrgObjID );
	if ( pObjDB == NULL )
		return ;

	g_pGameMain->SendSystemChatMsgEx( g_ObjectData->GetString( "SYS_GUILDHOUSES_UPGRADE_BUILDING_SUCCESS" ), pObjDB->GetName() );

	SendWorldEvent( UiEvent_GuildHousesBuildingInfoChanged );

		
}

// ----------------------------------------------------------------------------
void CGuildHousesFrame::RL_DeleteBuildingResult( int BuildingDBID , int Result )
{
	GuildBuildingObjectInfoStruct* pGuildBuildingObjectInfo = GetGuildBuildingObjectInfoByID( BuildingDBID );

	if( pGuildBuildingObjectInfo && Result )
	{
		GuildBuildingInfoTableStruct* pBuildingResource = GetGuildBuildingResourceByID( pGuildBuildingObjectInfo->BuildingID );

		if( NULL == pBuildingResource )
		{
			return ;
		}

		GameObjDbStructEx* pObjDB = g_ObjectData->GetObj( pBuildingResource->OrgObjID );
		if ( pObjDB == NULL )
			return ;
 
		g_pGameMain->SendSystemChatMsgEx( g_ObjectData->GetString( "SYS_GUILDHOUSES_DELETE_BUILDING_SUCCESS" ), pObjDB->GetName() );
 	}

}

// ----------------------------------------------------------------------------
void CGuildHousesFrame::RL_AllBuildingInfo( int GuildID , int Count , GuildBuildingObjectInfoStruct* Building )
{
	m_bGetAllBuildingInfoOK = true;

	if( m_CurrentGuildID != GuildID )
	{
		return;
	}
	
	m_mapGuildBuildingObjectInfo.clear();
	m_bBuildingObjectVectorDirty = true;

	int i;
	for( i = 0 ; i < Count ; ++i )
	{
		m_mapGuildBuildingObjectInfo[Building[i].BuildingDBID] = Building[i];	
	}

   	if( m_CurrentGuildID == CNetGlobal::RoleData()->BaseData.GuildID )
	{
		SendWorldEvent( UiEvent_GuildHousesBuildingInfoChanged );
	}
  
}

// ----------------------------------------------------------------------------
void CGuildHousesFrame::RL_NewBuildingInfo( GuildBuildingObjectInfoStruct& Building )
{
	map<int,GuildBuildingObjectInfoStruct>::iterator itPos = m_mapGuildBuildingObjectInfo.find( Building.BuildingDBID );
	if( itPos != m_mapGuildBuildingObjectInfo.end() )
	{
		m_mapGuildBuildingObjectInfo.erase( itPos );
	}
   
	m_mapGuildBuildingObjectInfo[Building.BuildingDBID] = Building;

	m_bBuildingObjectVectorDirty = true;

	if( m_CurrentGuildID == CNetGlobal::RoleData()->BaseData.GuildID )
	{
		SendWorldEvent( UiEvent_GuildHousesBuildingInfoChanged );
	}
}

// ----------------------------------------------------------------------------
void CGuildHousesFrame::RL_DelBuildingInfo( int BuildingDBID )
{
	map<int,GuildBuildingObjectInfoStruct>::iterator itPos = m_mapGuildBuildingObjectInfo.find( BuildingDBID );

	if( itPos != m_mapGuildBuildingObjectInfo.end() )
	{
		m_mapGuildBuildingObjectInfo.erase( itPos );	
	}

	m_bBuildingObjectVectorDirty = true;
 
	if( m_CurrentGuildID == CNetGlobal::RoleData()->BaseData.GuildID )
	{
		SendWorldEvent( UiEvent_GuildHousesBuildingInfoChanged );
	}
}

// ----------------------------------------------------------------------------
void CGuildHousesFrame::RL_HouseItemInfo( int PageID , int MaxPage , int GetCount , int ItemCount , GuildHouseItemStruct	Item[ 100 ] )
{
	if( m_GuildBankPageRequested )
	{
		m_GuildBankPageRequested = false;
	}

	if( PageID < 0 || PageID >= DF_GuildBankPageMax )
	{
		return;
	}
 
	if( ItemCount < 0 || ItemCount > 100  )
	{
		return;
	}

	m_GuildBankPageCount = MaxPage;


	 
	GuildBankPage& rPage = m_GuildBankPages[PageID];
	
	for( int i = 0 ; i < ItemCount ; ++i )
	{
		RegisterItemFieldChecksum( &Item[i].Item );
		rPage.Items[ Item[i].PosID ] = Item[i].Item;
	}

	//pPage->ItemUpdateTime = timeGetTime();
	rPage.bItemUpdated = true;

	lua_State* L = GetLuaState();
	lua_pushnumber( L, PageID + 1 );
	lua_setglobal( L, "arg1" );
	SendWorldEvent( UiEvent_GuildBankPageUpdate );
}

// ----------------------------------------------------------------------------
void CGuildHousesFrame::RL_SwapBodyHouseItemResult( int BodyPos, int HouseItemPageID, int HouseItemPos, int GetCount, bool Result )
{
	if( false == Result )
	{
		ItemFieldStruct* bagItem = CNetGlobal::RoleData()->GetBodyItem( BodyPos );
		if ( bagItem && bagItem->Pos == EM_ItemState_CliDisabled ) 
		{
			bagItem->Pos = EM_ItemState_NONE;
		}


		ItemFieldStruct* item = GetGuildBankItem( HouseItemPageID * DF_GuildBankItemMax + HouseItemPos );
		if ( item && item->Pos == EM_ItemState_CliDisabled )
		{
   			item->Pos = EM_ItemState_NONE;
		}
	}
}

// ----------------------------------------------------------------------------
void CGuildHousesFrame::RL_FixHouseItem( GuildHouseItemStruct& Item )
{
	if( m_eHousesState != EHousesState_Enter )
	{
		return ;
	}

	if( Item.PageID < 0 || Item.PageID >= DF_GuildBankPageMax )
	{
		return;
	}

	if( Item.PosID < 0 || Item.PosID >= DF_GuildBankItemMax )
	{
		return;
	}

	//std::map<int,GuildBankPage>::iterator itPos = m_GuildBankPages.find( Item.PageID );

	//if( itPos == m_GuildBankPages.end() )
	//{
	//	GuildBankPage _Page;
	//	m_GuildBankPages.insert( std::map<int,GuildBankPage>::value_type( Item.PageID , _Page ) );
	//	itPos = m_GuildBankPages.find( Item.PageID );
	//}

	//GuildBankPage* pPage = &itPos->second;
   	//pPage->Items[Item.PosID] = Item.Item;

	GuildBankPage& rPage = m_GuildBankPages[ Item.PageID ];
	if( false == rPage.bItemUpdated )
	{
		return;
	}
	rPage.Items[Item.PosID] = Item.Item;
	RegisterItemFieldChecksum( &Item.Item );

//	m_GuildBankItems[ Item.PageID * DF_GuildBankItemMax + Item.PosID ] = Item.Item;

	lua_State* L = GetLuaState();

	lua_pushnumber( L, Item.PageID + 1 );
	lua_setglobal( L, "arg1" );

	lua_pushnumber( L, Item.PosID + 1 );
	lua_setglobal( L, "arg2" );

	SendWorldEvent( UiEvent_GuildBankFixItem );
}

// ----------------------------------------------------------------------------
void CGuildHousesFrame::RL_SwapHouseItemResult( int HouseItemPos[2] , int HouseItemPageID[2] , bool Result )
{
	if( Result )
	{
		return;
	}

	//失敗解除鎖定
	if( HouseItemPageID[0] < 0 || HouseItemPageID[0] >= DF_GuildBankPageMax )
	{
		return;
	}

	if( HouseItemPos[0] < 0 || HouseItemPos[0] >= DF_GuildBankItemMax  )
	{
		return;
	}

	if( HouseItemPageID[1] < 0 || HouseItemPageID[1] >= DF_GuildBankPageMax )
	{
		return;
	}

	if( HouseItemPos[1] < 0 || HouseItemPos[1] >= DF_GuildBankItemMax  )
	{
		return;
	}

	m_GuildBankPages[ HouseItemPageID[0] ].Items[ HouseItemPos[0] ].Pos = EM_ItemState_NONE; 
	m_GuildBankPages[ HouseItemPageID[1] ].Items[ HouseItemPos[1] ].Pos = EM_ItemState_NONE; 
	 

	lua_State* L = GetLuaState();

	lua_pushnumber( L, HouseItemPageID[0] + 1 );
	lua_setglobal( L, "arg1" );

	lua_pushnumber( L, HouseItemPos[0] + 1 );
	lua_setglobal( L, "arg2" );

	SendWorldEvent( UiEvent_GuildBankFixItem );


	lua_pushnumber( L, HouseItemPageID[1] + 1 );
	lua_setglobal( L, "arg1" );

	lua_pushnumber( L, HouseItemPos[1] + 1 );
	lua_setglobal( L, "arg2" );

	SendWorldEvent( UiEvent_GuildBankFixItem );

}

// ----------------------------------------------------------------------------
void CGuildHousesFrame::RL_BuyItemPageResult( int PageID , bool Result )
{
	if( false == Result )
	{
		g_pGameMain->SendSystemChatMsg( g_ObjectData->GetString( "SYS_GUILDBANK_BUYPAGE_FAIL1" ) );
		return;
	}

	g_pGameMain->SendSystemChatMsg( g_ObjectData->GetString( "SYS_GUILDBANK_BUYPAGE_SUCCEED" ) );

	//lua_State* L = GetLuaState();
	//lua_pushnumber( L, PageID + 1 );
	//lua_setglobal( L, "arg1" );
	//SendWorldEvent( UiEvent_GuildBankPageUpdate );

	GuildBank_PageRequest( PageID );
}

// ----------------------------------------------------------------------------
void CGuildHousesFrame::RL_FurnitureItemInfo( GuildHouseFurnitureItemStruct& Item )
{
	m_GuildHouseFurnitureList[ Item.Pos ] = Item;
	m_GuildHouseFurnitureChanged = true;
}

// ----------------------------------------------------------------------------
void CGuildHousesFrame::RL_SwapBodyHouseFurnitureResult( int BodyPos , int HousePos, bool Result )
{

}

// ----------------------------------------------------------------------------
void CGuildHousesFrame::RL_FixHouseFurniture( GuildHouseFurnitureItemStruct& Item )
{
	m_GuildHouseFurnitureList[ Item.Pos ] = Item;
	m_GuildHouseFurnitureChanged = true;
}

// ----------------------------------------------------------------------------
void CGuildHousesFrame::RL_SwapHouseFurnitureResult( int Pos[2] , bool Result )
{

}

// ----------------------------------------------------------------------------
void CGuildHousesFrame::RL_HouseItemLayoutResult( int Pos , bool Result )
{

}

// ----------------------------------------------------------------------------
void CGuildHousesFrame::RL_StoreConfig( GuildStorehouseRightStruct& Config )
{
	if( Config.PageID < 0 || Config.PageID >= DF_GuildBankPageMax )
	{
		return ;
	}

	GuildBankPage& rPage = g_pGuildHousesFrame->m_GuildBankPages[Config.PageID];
	rPage.GuildBankConfig = Config;
}

// ----------------------------------------------------------------------------
void CGuildHousesFrame::RL_SetStoreConfigResult( bool Result )
{
	if( false == Result )
	{
		return;
	}

	//m_GuildBankConfigs[m_GuildBankConfigTemp.PageID] = m_GuildBankConfigTemp;
}

// ----------------------------------------------------------------------------
void CGuildHousesFrame::RL_ItemLogList( int Count , GuildItemStoreLogStruct Log[1000] )
{
	//struct GuildBankPageLog
	//{
	//	GuildBankPageLog()
	//	{
	//		Count = 0;
	//	};
	//	int Count;
	//	GuildItemStoreLogStruct Log[1000];

	//	int GuildBankPageLogUpdateTime;
	//};

	//std::map<int,GuildBankPageLog> m_GuildBankPageLogs;


	//Log[0]


	//bool m_GuildBankPageLogRequestPageID; //要求的PageID
	//bool m_GuildBankPageLogRequested; //

	GuildBankPage& rPage = m_GuildBankPages[m_GuildBankPageLogRequestPageID];
					  
	//rPage.LogCount = Count;
		
	for( int i = 0 ; i < Count ; ++i )
	{
		//rPage.Log[i] = Log[i];
		rPage.Logs.insert( std::multimap<int,GuildItemStoreLogStruct>::value_type( Log[i].Time , Log[i] ) );
	}

	rPage.bLogUpdated = true;

	m_GuildBankPageLogRequested = false;


	lua_State* L = GetLuaState();
	lua_pushnumber( L, m_GuildBankPageLogRequestPageID + 1 );
	lua_setglobal( L, "arg1" );
	SendWorldEvent( UiEvent_GuildBankPageLogUpdate );
}

// ----------------------------------------------------------------------------
void CGuildHousesFrame::RL_ItemLog( GuildItemStoreLogStruct& ItemLog )
{

	if( m_eHousesState != EHousesState_Enter )
	{
		return ;
	}

	if( ItemLog.PageID < 0 || ItemLog.PageID >= DF_GuildBankPageMax )
	{
		return;
	}

	GuildBankPage& rPage = m_GuildBankPages[ ItemLog.PageID ];


	if( false == rPage.bLogUpdated )
	{
		return;
	}

	rPage.Logs.insert( std::multimap<int,GuildItemStoreLogStruct>::value_type( ItemLog.Time , ItemLog ) );

	//單筆不更新
	//lua_State* L = GetLuaState();
	//lua_pushnumber( L, m_GuildBankPageLogRequestPageID + 1 );
	//lua_setglobal( L, "arg1" );
	//SendWorldEvent( UiEvent_GuildBankPageLogUpdate );
}


// ----------------------------------------------------------------------------
void CGuildHousesFrame::RL_ActiveBuildingResult( int BuildDBID , bool IsActive , bool Result )
{
	if( Result )
	{
		m_mapBuildingActive[BuildDBID] = IsActive;


		if( m_eHousesState == EHousesState_Enter )
		{
		//	SendWorldEvent( UiEvent_GuildHousesBuildingInfoChanged );
		}
	}
}

// ----------------------------------------------------------------------------
void CGuildHousesFrame::RL_BuildingActiveType( int BuildDBID , bool IsActive )
{
	m_mapBuildingActive[BuildDBID] = IsActive;

	if( m_eHousesState == EHousesState_Enter )
	{
		SendWorldEvent( UiEvent_GuildHousesBuildingInfoChanged );
	}
}

// ----------------------------------------------------------------------------
void CGuildHousesFrame::R_FlowerPotInfo( int GItemID , ItemFieldStruct& Item )
{
	if( m_eHousesState == EHousesState_Leave )
	{
		return;
	}
	m_mapFlowerPotInfo[GItemID] = Item;
}

// ----------------------------------------------------------------------------
void CGuildHousesFrame::GuildBank_PageRequest( int PageID )
{
	if( m_eHousesState != EHousesState_Enter )
	{
		return;
	}

	if( false == m_bGuildBankOpen )
	{
		return;
	}

	if( m_GuildBankPageRequested )
	{
		return;
	}

	if( PageID < 0 || PageID >= DF_GuildBankPageMax )
	{
		return;
	}

	GuildBankPage& rPage = m_GuildBankPages[PageID];

	//int nowtime = timeGetTime();
	//if( nowtime - pPage->ItemUpdateTime > 10000 )
	//{
	//	pPage->ItemUpdateTime = nowtime;
	//	NetCli_GuildHouses::SL_HouseItemRequest( PageID );
	//	return;
	//}

	if( false == rPage.bItemUpdated )
	{
		NetCli_GuildHouses::SL_HouseItemRequest( PageID );
		return;
	}

	lua_State* L = GetLuaState();
	lua_pushnumber( L, PageID + 1 );
	lua_setglobal( L, "arg1" );
	SendWorldEvent( UiEvent_GuildBankPageUpdate );
}

void CGuildHousesFrame::GuildBank_PageLogRequest( int PageID )
{
	//int m_GuildBankPageLogRequestPageID; //要求的PageID
	//bool m_GuildBankPageLogRequested; //

	if( m_eHousesState != EHousesState_Enter )
	{
		return;
	}

	if( false == m_bGuildBankOpen )
	{
		return;
	}

	if( m_GuildBankPageLogRequested )
	{
		return;
	}

	if( PageID < 0 || PageID >= DF_GuildBankPageMax )
	{
		return;
	}

	m_GuildBankPageLogRequestPageID = PageID;

	GuildBankPage& rPage = m_GuildBankPages[PageID];

	//int nowtime = timeGetTime();

	//if( nowtime - rPage.LogUpdateTime > 1000 ) //300000 ) //1000 * 60 * 5 5分鐘要一次
	//{
	//	rPage.LogUpdateTime = nowtime;
	//	NetCli_GuildHouses::SL_ItemLogRequest( PageID, 3 );
	//	return;
	//}

	if( false == rPage.bLogUpdated )
	{
  		NetCli_GuildHouses::SL_ItemLogRequest( PageID , 0 );
   		NetCli_GuildHouses::SL_ItemLogRequest( PageID , 1 );
		NetCli_GuildHouses::SL_ItemLogRequest( PageID , 2 );
		return;
	}

	lua_State* L = GetLuaState();
	lua_pushnumber( L, PageID + 1 );
	lua_setglobal( L, "arg1" );
	SendWorldEvent( UiEvent_GuildBankPageLogUpdate );

}

void CGuildHousesFrame::OpenGuildBank()
{
	if( m_eHousesState != EHousesState_Enter )
	{
		m_bGuildBankOpen = false;
		return;
	}
	CRoleSprite* pPlayer = g_pGameMain->GetPlayer();
	if( NULL == pPlayer )
	{
		m_bGuildBankOpen = false;
		return;
	}

	m_GuildBankPos = g_pGameMain->GetPlayer()->GetPos();

	m_bGuildBankOpen = true;

	GuildBank_PageRequest( 0 );
}

// ----------------------------------------------------------------------------
void CGuildHousesFrame::R_ObjGuildBuildingInfo( int GItemID , int BuildingParentDBID , int BuildingDBID , int BuildingID , const char* Point )
{
	m_mapBuildingDBIDGItemID[BuildingDBID] = GItemID;
}

// ----------------------------------------------------------------------------
void CGuildHousesFrame::R_ObjGuildFurnitureInfo( int GItemID , int FurnitureDBID )
{
	m_mapFurnitureDBIDGItemID[FurnitureDBID] = GItemID;
}

// ----------------------------------------------------------------------------
int CGuildHousesFrame::GetBuildingGItemID( int BuildingDBID )
{
	map<int,int>::iterator itPos = m_mapBuildingDBIDGItemID.find( BuildingDBID );

	if( itPos != m_mapBuildingDBIDGItemID.end() )
	{
		return itPos->second;
	}
	return -1;
}

// ----------------------------------------------------------------------------
int CGuildHousesFrame::GetFurnitureGItemID( int FurnitureDBID )
{
	map<int,int>::iterator itPos = m_mapFurnitureDBIDGItemID.find( FurnitureDBID );

	if( itPos != m_mapFurnitureDBIDGItemID.end() )
	{
		return itPos->second;
	}
	return -1;
}

// ----------------------------------------------------------------------------
CRoleSprite* CGuildHousesFrame::GetRoleSpriteByBuildingDBID( int BuildingDBID )
{
	int GItemID = GetBuildingGItemID( BuildingDBID );
	if( GItemID < 0 )
	{
		return NULL;
	}
	CRoleSprite* pRoleSprite = g_pGameMain->FindSpriteByID( GItemID );
	return pRoleSprite;
}

// ----------------------------------------------------------------------------
CRoleSprite* CGuildHousesFrame::GetRoleSpriteByFurnitureDBID( int FurnitureDBID )
{
	CRoleSprite* pRoleSprite = NULL;
	int GItemID = GetFurnitureGItemID( FurnitureDBID );
	if( GItemID > 0 )
	{
		pRoleSprite = g_pGameMain->FindSpriteByID( GItemID );
	}

	return pRoleSprite;
}

// ----------------------------------------------------------------------------
CRoleSprite* CGuildHousesFrame::GetRoleSpriteByFurniturePos( int Pos )
{
	GuildHouseFurnitureItemStruct* pFurnitureItem = GetGuildHouseFurnitureItem( Pos );
	if( NULL == pFurnitureItem )
	{
		return NULL;
	}

 	CRoleSprite* pRoleSprite = GetRoleSpriteByFurnitureDBID( pFurnitureItem->ItemDBID );

	if( Pos == m_FocusFurnitureID )
	{
		CRoleSprite* pRoleSpriteTemp = g_pGameMain->FindSpriteByWorldGUID( m_FurnitureRoleSpriteID );

		if( NULL == pRoleSprite )
		{
			pRoleSprite = pRoleSpriteTemp;
		}
		else
		{
			if( pRoleSpriteTemp )
			{
				pRoleSpriteTemp->SetVisible( false );
			}
		}
	}
	
	return pRoleSprite;
}
//要求天梯資料
void CGuildHousesFrame::GuildWarLadder( void  )
{
	if (m_ladderDelayTime>(60*5))
	{
		m_ladderDelayTime=0;

		NetCli_BG_GuildWar::SL_GuildWarRankInfoRequest( EM_GuildWarRankInfoType_RankList , 0);
	}
	
}
void CGuildHousesFrame::GuildWarLadderHistory( int GuildID  )
{

	bool bImm=false;
	if (m_histotyGuildID!=GuildID)
		bImm=true;

	if (bImm||(m_LadderHistoryDelayTime>(60*5)))
	{
		m_histotyGuildID=GuildID;
#if(_DEBUG_LADDER==1)
		{
			//	char temp1[512];

			//	int guild=m_histotyGuildID%0x1000000;
			//	int sever=m_histotyGuildID/0x1000000;
			//	sprintf_s(temp1,512,"[Test %d:%d]",sever,guild);
			char temp2[512];

			m_LadderHistoryCount=(rand()%40)+10;


			for (int i=0;i<m_LadderHistoryCount;i++)
			{
				GuildHouseWarHistoryStruct& info=	m_LadderHistory[i];


				info.GuildID=m_histotyGuildID;
				int sever=(rand()%3);
				int guild=rand()%1000;
				sprintf_s(temp2,512,"[Test %d:%d]",sever,guild);
				info.TargetGuildID=guild+sever*0x1000000;
				info.GuildScore=rand()%1000;
				info.TargetGuildScore=rand()%1000;
				info.TargetGuildName=temp2;
				info.Result=rand()%3;
			}
				SendWorldEvent("GUILDHOUSEWAR_LADDERHISTORY_UPDATE");
		}
#endif
		m_LadderHistoryDelayTime=0;
		NetCli_BG_GuildWar::SL_GuildWarRankInfoRequest( EM_GuildWarRankInfoType_History , GuildID);
	}
}
//歷史對戰
void CGuildHousesFrame::GuildWarLadderHisotryResult( GuildHouseWarHistoryStruct Hisotry[50] )
{
	int count=0;
	for (int i=0;i<50;i++)
	{
		if (Hisotry[i].GuildID>0)
		{
			m_LadderHistory[i]=Hisotry[i];
			count++;

		}	
		else 
			break;
	}	
	m_LadderHistoryCount=count;
	SendWorldEvent("GUILDHOUSEWAR_LADDERHISTORY_UPDATE");

}

//天梯資料
void CGuildHousesFrame::GuildWarLadderResult( GuildWarRankInfoStruct& Info )
{

	m_delayTimer=0;

	m_LadderInfoDirty=true;

	m_LadderInfo[Info.WorldGuildID]=Info;

}

// ----------------------------------------------------------------------------
void CGuildHousesFrame::GuildHousesControl_OnMouseDown  ( CUiFrame* uiFrame, INT64 lParam, INT64 rParam )
{
	if( m_PlaceFurnitureMode && m_MouseEnterFurnitureID != -1 && lParam == MOUSE_LBUTTON )
	{

	}
	else
	{
		g_pGameControl->OnMouseDown( uiFrame, lParam, rParam );
	}
}

// ----------------------------------------------------------------------------
void CGuildHousesFrame::GuildHousesControl_OnMouseUp    ( CUiFrame* uiFrame, INT64 lParam, INT64 rParam )
{
	g_pGameControl->OnMouseUp( uiFrame, lParam, rParam );
}

// ----------------------------------------------------------------------------
void CGuildHousesFrame::GuildHousesControl_OnMouseWheel ( CUiFrame* uiFrame, INT64 lParam, INT64 rParam )
{
	g_pGameControl->OnMouseWheel( uiFrame, lParam, rParam );
}

// ----------------------------------------------------------------------------
void CGuildHousesFrame::GuildHousesControl_OnMouseMove  ( CUiFrame* uiFrame, INT64 lParam, INT64 rParam )
{

	if( m_PlaceFurnitureMode )
	{
		if( m_ePlaceingMode == EM_PlaceingMode_New ) //放置模式
		{
			if( m_PlaceStep == 0 )
			{
				GuildHousesControl_FurnitureMove( );
			}
			else if( m_PlaceStep == 1 )
			{
				GuildHousesControl_FurnitureRorare( );
			}
		}
		else if( m_ePlaceingMode == EM_PlaceingMode_Move )
		{
			if( m_PlaceStep == 0 )
			{
				GuildHousesControl_FurnitureMouseEnter( );
			}
			else if( m_PlaceStep == 1 )
			{
				GuildHousesControl_FurnitureMove( );
			}
		}
		else if( m_ePlaceingMode == EM_PlaceingMode_Rorare )
		{
			if( m_PlaceStep == 0 )
			{
				GuildHousesControl_FurnitureMouseEnter( );
			}
			else if( m_PlaceStep == 1 )
			{
				GuildHousesControl_FurnitureRorare( );
			}
		}
		else
		{
			GuildHousesControl_FurnitureMouseEnter( );
		}
	}
	else
	{
		GuildHousesControl_FurnitureMouseEnter( );
	}
}

// ----------------------------------------------------------------------------
void CGuildHousesFrame::GuildHousesControl_OnMouseEnter ( CUiFrame* uiFrame, INT64 lParam, INT64 rParam )
{
	g_pGameControl->OnMouseEnter( uiFrame, lParam, rParam );
}

// ----------------------------------------------------------------------------
void CGuildHousesFrame::GuildHousesControl_OnMouseLeave ( CUiFrame* uiFrame, INT64 lParam, INT64 rParam )
{
	g_pGameControl->OnMouseLeave( uiFrame, lParam, rParam );
	m_MouseEnterFurnitureID = -1;
}

// ----------------------------------------------------------------------------
void CGuildHousesFrame::GuildHousesControl_OnClick      ( CUiFrame* uiFrame, INT64 lParam, INT64 rParam )
{
	if( m_PlaceFurnitureMode && ( 
		m_ePlaceingMode ==  EM_PlaceingMode_New ||
		( m_ePlaceingMode == EM_PlaceingMode_Move   && m_PlaceStep == 1 ) ||
		( m_ePlaceingMode == EM_PlaceingMode_Rorare && m_PlaceStep == 1 )  ) )
		//if( m_PlaceFurnishingMode && m_MouseEnterFurnishingID != -1 && lParam == MOUSE_LBUTTON )
	{

	}
	else
	{
		g_pGameControl->OnClick( uiFrame, lParam, rParam );
	}

	if( lParam == MOUSE_LBUTTON )
	{
		if( m_PlaceFurnitureMode )
		{
			if( m_ePlaceingMode == EM_PlaceingMode_New ) //放置模式
			{
				m_PlaceStep++;

				if( m_PlaceStep == 2 )
				{
					m_ePlaceingMode = m_ePlaceingModeTemp;
					m_PlaceStep = 0;
					FurniturLayoutChange();
				}
				return;
			}
			else if( m_ePlaceingMode == EM_PlaceingMode_Move )
			{
			}
			else if( m_ePlaceingMode == EM_PlaceingMode_Rorare )
			{
			}
		}

		if( m_MouseEnterFurnitureID != -1 )
		{
			SetFocusFurnitureID( m_MouseEnterFurnitureID );
			return;
		}
		//每次發生點擊 就判斷有沒有改變
		FurniturLayoutChange();
	}











	//if( m_eWorkMode == WM_CreateBuildingToGround )
	//{

	//	if( lParam == MOUSE_RBUTTON )
	//	{
	//		CRoleSprite* pRoleSpriteBuilding = g_pGameMain->FindSpriteByWorldGUID( m_BuildingRoleSpriteID );
	//		if( pRoleSpriteBuilding )
	//		{
	//			pRoleSpriteBuilding->SetVisible( false );
 //  			}
	//		m_eWorkMode = WM_None;
	//		return;
	//	}
	//	
	//	CRoleSprite* pRoleSpriteBuilding = g_pGameMain->FindSpriteByWorldGUID( m_BuildingRoleSpriteID );

	//	if( !pRoleSpriteBuilding || !pRoleSpriteBuilding->IsVisible() )
	//	{
	//		m_eWorkMode = WM_None;
	//		return;
	//	}

	//	// 參數
	//	INT32 x = g_pRuneDev->GetCursorPtX();		// 滑鼠x
	//	INT32 y = g_pRuneDev->GetCursorPtY();		// 滑鼠y
	//	IRuSwapChain *swapChain = g_pRuneDev->GetRuSwapChain();		// 由CRuneDev取得IRuSwapChain
	//	CRuCamera* camera = g_pRuneDev->GetCamera();	
	//	// 將滑鼠座標轉換成世界座標
	//	CRuVector3 homPt, viewPt, worldPt;
	//	camera->Coord_ScreenToHomogeneous(x, y, swapChain->GetWidth(), swapChain->GetHeight(), homPt);
	//	camera->Coord_HomogeneousToView(homPt, viewPt);
	//	camera->Coord_ViewToWorld(viewPt, worldPt);

	//	// 建構碰撞測試結構
	//	CRuCollisionQuery_Segment_Nearest query;

	//	// 設定測試用線段及其他設定
	//	CRuVector3 pickDir = (worldPt - camera->GetCameraPosition()).Normalize();
	//	query.m_colSegment = CRuSegment3(worldPt, worldPt + (pickDir * 1000.0f));
	//	query.m_ignoreBackface = TRUE;

	//	// 測試碰撞
	//	g_pGameMain->GetRuTerrain()->QueryCollision(&query);

	//	if( query.m_positiveHit )
	//	{

	//		CRuVector3 Point = query.m_collisionPoint;

	//		int x = (int)( Point.x / DF_GuildHouses_GroundInterval ) ;
	//		int z = (int)( Point.z / DF_GuildHouses_GroundInterval ) ;

	//		char szPoint[256];
	//		sprintf( szPoint, "ground_%d_%d" , x , z );

	//		NetCli_GuildHouses::SL_CreateBuildingRequest( szPoint , m_BuildingID , -1 , 90 );

	//		pRoleSpriteBuilding->SetVisible( false );
	//		m_eWorkMode = WM_None;
	//		return;
	//	}

	//}
	//else
	//{
	//	g_pGameControl->OnClick( uiFrame, lParam, rParam );
	//}

}

// ----------------------------------------------------------------------------
void CGuildHousesFrame::GuildHousesControl_OnDoubleClick( CUiFrame* uiFrame, INT64 lParam, INT64 rParam )
{
	g_pGameControl->OnDoubleClick( uiFrame, lParam, rParam );
}

// ----------------------------------------------------------------------------
void CGuildHousesFrame::GuildHousesControl_OnDragBegin  ( CUiFrame* uiFrame, INT64 lParam, INT64 rParam )
{
	if( m_PlaceFurnitureMode )
	{
		if( m_ePlaceingMode == EM_PlaceingMode_Move ||
			m_ePlaceingMode == EM_PlaceingMode_Rorare )
		{
			if( m_MouseEnterFurnitureID != -1 )
			{
				CRoleSprite* pRoleSprite = GetRoleSpriteByFurniturePos( m_MouseEnterFurnitureID );
				if( !pRoleSprite )
					return ;
				CRuVector3 vecPos = pRoleSprite->GetPos();
				g_pGameMain->WorldToScreen(vecPos);
				g_pGameMain->SetCursorPosition( (int)vecPos.x, (int)vecPos.y );
				SetFocusFurnitureID( m_MouseEnterFurnitureID );
				m_PlaceStep = 1;
			}
		}
	}
	else
	{
		g_pGameControl->OnDragBegin( uiFrame, lParam, rParam );
	}
}

// ----------------------------------------------------------------------------
void CGuildHousesFrame::GuildHousesControl_OnDragStop   ( CUiFrame* uiFrame, INT64 lParam, INT64 rParam )
{
	if( m_PlaceFurnitureMode )
	{
		if( m_ePlaceingMode == EM_PlaceingMode_Move ||
			m_ePlaceingMode == EM_PlaceingMode_Rorare )
		{
			m_PlaceStep = 0;
		}
	}
	else
	{
		g_pGameControl->OnDragStop( uiFrame, lParam, rParam );
	}

}

// ----------------------------------------------------------------------------
void CGuildHousesFrame::GuildHousesControl_OnReceiveDrag( CUiFrame* uiFrame, INT64 lParam, INT64 rParam )
{
	g_pGameControl->OnReceiveDrag( uiFrame, lParam, rParam );
}

// ----------------------------------------------------------------------------
void CGuildHousesFrame::GuildHousesControl_FurnitureMouseEnter( )
{
	// 參數
	INT32 x = g_pRuneDev->GetCursorPtX();		// 滑鼠x
	INT32 y = g_pRuneDev->GetCursorPtY();		// 滑鼠y
	IRuSwapChain *swapChain = g_pRuneDev->GetRuSwapChain();		// 由CRuneDev取得IRuSwapChain
	CRuCamera* camera = g_pRuneDev->GetCamera();	
	// 將滑鼠座標轉換成世界座標
	CRuVector3 homPt, viewPt, worldPt;
	camera->Coord_ScreenToHomogeneous(x, y, swapChain->GetWidth(), swapChain->GetHeight(), homPt);
	camera->Coord_HomogeneousToView(homPt, viewPt);
	camera->Coord_ViewToWorld(viewPt, worldPt);

	// 建構碰撞測試結構
	CRuCollisionQuery_Segment_Nearest query;

	// 設定測試用線段及其他設定
	CRuVector3 pickDir = (worldPt - camera->GetCameraPosition()).Normalize();
	query.m_colSegment = CRuSegment3(worldPt, worldPt + (pickDir * 1000.0f));
	query.m_ignoreBackface = TRUE;
	query.m_ignoreCollisionFlag = TRUE;

	//
	m_MouseEnterFurnitureID = -1;	

	float collisionT = 0.0f;
	CRoleSprite* pRoleSprite = NULL;

  	std::map<int,GuildHouseFurnitureItemStruct>::iterator itPos = m_GuildHouseFurnitureList.begin();
	std::map<int,GuildHouseFurnitureItemStruct>::iterator itEnd = m_GuildHouseFurnitureList.end();
	for( ; itPos != itEnd ; ++itPos )
	{
		GuildHouseFurnitureItemStruct& rFurnitureItem = itPos->second;

		//if( rFurnitureItem.Layout.IsShow == false )
		//{
		//	continue;
		//}

		pRoleSprite = GetRoleSpriteByFurnitureDBID( rFurnitureItem.ItemDBID );

		//assert( pRoleSprite );
		if( NULL == pRoleSprite || NULL == pRoleSprite->GetRuEntity() )
		{
			continue;
		}
		pRoleSprite->GetRuEntity()->QueryCollision(&query);
		if( query.m_positiveHit )
		{
			if( m_MouseEnterFurnitureID == -1 || query.m_collisionT < collisionT )
			{
				m_MouseEnterFurnitureID = itPos->first;				
				collisionT = query.m_collisionT;
			}
		}
	}

	UpdateHighlightFurnitureSprite();
}
static int  sGuildHousesFrame_GetStyleCount()
{

	

	return (int)g_pGuildHousesFrame->m_Styles.size();




}

static GuildHouseNpcReplaceCostTableStruct*  sGuildHousesFrame_GetGuildStyleInfoTableStructByIndex(int index)
{

	GuildHouseNpcReplaceCostTableStruct* ret=NULL;

	int count= (int)g_pGuildHousesFrame->m_Styles.size();
	if (index>=0&&index<count)
	{



		ret= g_pGuildHousesFrame->m_Styles[index];
	}


	return ret;
}

// ----------------------------------------------------------------------------
void CGuildHousesFrame::GuildHousesControl_FurnitureMove( )
{
	static CRuVector3 g_StandardNormals[6] = { 
		CRuVector3(  1.0 ,  0.0 ,  0.0 ), 
		CRuVector3( -1.0 ,  0.0 ,  0.0 ),
		CRuVector3(  0.0 ,  1.0 ,  0.0 ), 
		CRuVector3(  0.0 , -1.0 ,  0.0 ),
		CRuVector3(  0.0 ,  0.0 ,  1.0 ), 
		CRuVector3(  0.0 ,  0.0 , -1.0 ),
	};
  
	GuildHouseFurnitureItemStruct* pFurnitureItem = GetGuildHouseFurnitureItem( m_FocusFurnitureID );
 	CRoleSprite* pRoleSpriteFurniture = GetRoleSpriteByFurniturePos( m_FocusFurnitureID );

	if( !( pFurnitureItem && pRoleSpriteFurniture ) )
	{
		m_ePlaceingMode = m_ePlaceingModeTemp;
		return;
	}

	if( pFurnitureItem->OwnerDBID != g_pGameMain->GetDBID() )
	{
		m_ePlaceingMode = m_ePlaceingModeTemp;
		m_PlaceStep = 0;
		g_pGameMain->SendSystemMsg( g_ObjectData->GetString( "GUILDHOUSE_NOT_FURNITURE_OWNER" ) );
		return;
	}


	pRoleSpriteFurniture->SetVisible( true );
 	pRoleSpriteFurniture->EnableCollision( false );

	// 參數
	INT32 x = g_pRuneDev->GetCursorPtX();		// 滑鼠x
	INT32 y = g_pRuneDev->GetCursorPtY();		// 滑鼠y
	IRuSwapChain *swapChain = g_pRuneDev->GetRuSwapChain();		// 由CRuneDev取得IRuSwapChain
	CRuCamera* camera = g_pRuneDev->GetCamera();	
	// 將滑鼠座標轉換成世界座標
	CRuVector3 homPt, viewPt, worldPt;
	camera->Coord_ScreenToHomogeneous(x, y, swapChain->GetWidth(), swapChain->GetHeight(), homPt);
	camera->Coord_HomogeneousToView(homPt, viewPt);
	camera->Coord_ViewToWorld(viewPt, worldPt);

	// 建構碰撞測試結構
	CRuCollisionQuery_Segment_Nearest query;

	// 設定測試用線段及其他設定
	CRuVector3 pickDir = (worldPt - camera->GetCameraPosition()).Normalize();
	query.m_colSegment = CRuSegment3(worldPt, worldPt + (pickDir * 1000.0f));
	query.m_ignoreBackface = TRUE;
	query.m_ignoreCollisionFlag = TRUE;

	CRoleSprite* pSprite = NULL;

	//pick Furniture
	std::map<int,GuildHouseFurnitureItemStruct>::iterator itPos = m_GuildHouseFurnitureList.begin();
	std::map<int,GuildHouseFurnitureItemStruct>::iterator itEnd = m_GuildHouseFurnitureList.end();
	for( ; itPos != itEnd ; ++itPos )
	{
		GuildHouseFurnitureItemStruct& rFurnitureItem = itPos->second;
		//if( rFurnitureItem.Layout.IsShow == false )
		//{
		//	continue;
		//}
		if( itPos->first == m_FocusFurnitureID )
		{
			continue;
		}
		pSprite = GetRoleSpriteByFurniturePos( rFurnitureItem.Pos );
		if( NULL == pSprite || NULL == pSprite->GetRuEntity())
		{
			continue;
		}
		pSprite->GetRuEntity()->QueryCollision( &query );
	}

	//pick Building
 	GuildBuildingObjectInfoStruct* pBuildingInfo;
	int count = GetGuildBuildingObjectInfoCount();
	for( int i = 0 ; i < count ; ++i )
	{
		pBuildingInfo = GetGuildBuildingObjectInfoByIndex( i );
		if( NULL == pBuildingInfo )
		{
			continue;
		}

		pSprite = GetRoleSpriteByBuildingDBID( pBuildingInfo->BuildingDBID );
		if( NULL == pSprite || NULL == pSprite->GetRuEntity() )
		{
			continue;
		}
		pSprite->GetRuEntity()->QueryCollision( &query );

	}

	// 測試碰撞
	// 點地形
	g_pGameMain->GetRuTerrain()->QueryCollision(&query);

	if( !query.m_positiveHit )
	{
		pRoleSpriteFurniture->SetVisible( false );	
		return;
	}
 
	for( int i = 0 ; i < 6 ; ++i )
	{
		//10度
		if( 0.9848 < DotProduct( query.m_collisionNormal , g_StandardNormals[i] ) )
		{
			query.m_collisionNormal = g_StandardNormals[i];
			break;
		}
	}
	//DotProduct( query.m_collisionNormal , CRuVector3( 1.0 , 0.0 , 0.0 ) );
	//CRuVector3 vecDir( 0.0f, 0.0f, -1.0f);
	//m_pSpriteCurrentItem->SetDirection( vecDir );

	//
	if( GetKeyState(VK_SHIFT) < 0 )
	{
		//query.m_collisionPoint.x = ( ( query.m_collisionPoint.x - (float)(int)query.m_collisionPoint.x ) > 0.5f ? 1.0f : 0.0f ) + (float)(int)query.m_collisionPoint.x;
		//query.m_collisionPoint.y = ( ( query.m_collisionPoint.y - (float)(int)query.m_collisionPoint.y ) > 0.5f ? 1.0f : 0.0f ) + (float)(int)query.m_collisionPoint.y;
		//query.m_collisionPoint.z = ( ( query.m_collisionPoint.z - (float)(int)query.m_collisionPoint.z ) > 0.5f ? 1.0f : 0.0f ) + (float)(int)query.m_collisionPoint.z;

		//讓家具能貼齊 再PICK一次
		CRuPlane4 Plane( query.m_collisionNormal, query.m_collisionPoint );

		CRuVector3 Point = query.m_collisionPoint;
		Point.x = ( ( Point.x - (float)(int)Point.x ) > 0.5f ? 1.0f : 0.0f ) + (float)(int)Point.x;
		Point.y = ( ( Point.y - (float)(int)Point.y ) > 0.5f ? 1.0f : 0.0f ) + (float)(int)Point.y;
		Point.z = ( ( Point.z - (float)(int)Point.z ) > 0.5f ? 1.0f : 0.0f ) + (float)(int)Point.z;

		CRuVector3 rayOrigin = Point + query.m_collisionNormal * 10 ; 
		CRuVector3 rayDirection( -query.m_collisionNormal.x , -query.m_collisionNormal.y , -query.m_collisionNormal.z );

		CRuVector3 intPt;
		REAL intT;
		CRuRay3 pickRay( rayOrigin , rayDirection , TRUE );
		if( FindIntersection( pickRay, Plane, intPt, intT ) )
		{
			query.m_collisionPoint = intPt;
		}
	}

	static bool s_Sended = false;
	//門口附近不能放
	if( ( query.m_collisionPoint - CRuVector3( 0.0f, 5.0f, -485.0f ) ).Magnitude() < 40 )
	{
		//SendWorldEvent( UiEvent_HousesFurnitureTooNearDoor );
		if( false == s_Sended )
		{
			g_pGameMain->SendWarningMsg( g_ObjectData->GetString( "SYS_HOUSES_FURNITURE_TOO_NEAR_DOOR" ) );
			s_Sended = true;
		}
		return ;
	}
	s_Sended = false;

	pRoleSpriteFurniture->SetPos( query.m_collisionPoint );
	pRoleSpriteFurniture->SetOrientingAxis( query.m_collisionNormal );

	float angle = pFurnitureItem->Layout.Angle * PI / 180.0f;
	pRoleSpriteFurniture->SetDirection(CRuVector3(cos(angle), 0.0f, -sin(angle)), true, true);
	
	pRoleSpriteFurniture->UpdateOrientation();
	pRoleSpriteFurniture->Update(0);

//	char tt[256];
//	sprintf( tt, "Pos( %.02f, %.02f, %.02f ), Normal( %.02f, %.02f, %.02f )\n" ,
//		query.m_collisionPoint.x, query.m_collisionPoint.y, query.m_collisionPoint.z,
//		query.m_collisionNormal.x, query.m_collisionNormal.y, query.m_collisionNormal.z );
//	OutputDebugString( tt );
  
	pFurnitureItem->Layout.X = query.m_collisionPoint.x;
	pFurnitureItem->Layout.Y = query.m_collisionPoint.y;
	pFurnitureItem->Layout.Z = query.m_collisionPoint.z;

	pFurnitureItem->Layout.UpX = query.m_collisionNormal.x;
	pFurnitureItem->Layout.UpY = query.m_collisionNormal.y;
	pFurnitureItem->Layout.UpZ = query.m_collisionNormal.z;

	m_bFurniturLayoutChanged = true;

}

// ----------------------------------------------------------------------------
void CGuildHousesFrame::GuildHousesControl_FurnitureRorare( )
{
	GuildHouseFurnitureItemStruct* pFurnitureItem = GetGuildHouseFurnitureItem( m_FocusFurnitureID );
	CRoleSprite* pRoleSpriteFurniture = GetRoleSpriteByFurniturePos( m_FocusFurnitureID );

	if( !( pFurnitureItem && pRoleSpriteFurniture ) )
	{
		m_ePlaceingMode = m_ePlaceingModeTemp;
		return;
	}

	if( pFurnitureItem->OwnerDBID != g_pGameMain->GetDBID() )
	{
		m_ePlaceingMode = m_ePlaceingModeTemp;
		m_PlaceStep = 0;
		g_pGameMain->SendSystemMsg( g_ObjectData->GetString( "GUILDHOUSE_NOT_FURNITURE_OWNER" ) );
		return;
	}

	pRoleSpriteFurniture->SetVisible( true );

	// 參數
	INT32 x = g_pRuneDev->GetCursorPtX();		// 滑鼠x
	INT32 y = g_pRuneDev->GetCursorPtY();		// 滑鼠y
	IRuSwapChain *swapChain = g_pRuneDev->GetRuSwapChain();		// 由CRuneDev取得IRuSwapChain
	CRuCamera* camera = g_pRuneDev->GetCamera();	

	// 將滑鼠座標轉換成世界座標
	CRuVector3 homPt, viewPt, worldPt;
	camera->Coord_ScreenToHomogeneous(x, y, swapChain->GetWidth(), swapChain->GetHeight(), homPt);
	camera->Coord_HomogeneousToView(homPt, viewPt);
	camera->Coord_ViewToWorld(viewPt, worldPt);

	// Build an orienting plane centered on the sprite position
	CRuVector3 spritePosition = pRoleSpriteFurniture->GetPos();
	CRuVector3 spriteNormal = pRoleSpriteFurniture->GetOrientingAxis();

	CRuPlane4 orientingPlane(spriteNormal, spritePosition);

	// Test intersection against orienting plane
	CRuVector3 intPt;
	REAL intT;
	CRuRay3 pickRay(camera->GetCameraPosition(), worldPt - camera->GetCameraPosition(), TRUE);
	if(FindIntersection(pickRay, orientingPlane, intPt, intT))
	{
		// Calculate forward orientation vector for the furnishing
		CRuVector3 entityForward = intPt - spritePosition;

		if(entityForward.SquaredMagnitude() > ruEPSILON)
		{
			entityForward.Normalize();

			// Build furnishing's default orientation transform
			CRuMatrix4x4 orientTransform;
			orientTransform = RuMatrixUtility_BuildOrientationTransform(CRuVector3(0.0f, 0.0f, -1.0f), CRuVector3(0.0f, 0.0f, -1.0f), spriteNormal, 1.0f);

			// Get inverse of the orientation transform
			CRuMatrix4x4 invOrientTransform;
			invOrientTransform = orientTransform.GetInverse();

			// Transform entity forward into local space
			invOrientTransform.TransformVector(entityForward);

			// Set sprite forward direction
			//m_pSpriteCurrentItem->SetDirection(entityForward);


			REAL forwardAngleDiff;
			CalculateAngleDifferential( CRuVector3(0.0f, 0.0f, -1.0f) , entityForward, CRuVector3(0.0f, 1.0f, 0.0f), forwardAngleDiff );

			//g_pChatFrame->SendMsgEvent(CHAT_MSG_SYSTEM, Com_FormatString( "X(%0.2f) Y(%0.2f) Z(%0.2f) Angle(%0.2f)", entityForward.x, entityForward.y, entityForward.z, forwardAngleDiff ), "" );

			if( GetKeyState(VK_SHIFT) < 0 )
			{
				while( forwardAngleDiff < 0.0f )
				{
					forwardAngleDiff += PI * 2.0f ; 
				}

				while( forwardAngleDiff > PI * 2.0f )
				{
					forwardAngleDiff -= PI * 2.0f ; 
				}

				forwardAngleDiff = forwardAngleDiff *  8.0f / PI ;
				forwardAngleDiff = ( ( forwardAngleDiff - (float)(int)forwardAngleDiff ) > 0.5f ? 1.0f : 0.0f ) + (float)(int)forwardAngleDiff ;
				forwardAngleDiff = forwardAngleDiff * PI / 8.0f ;

			}

			CRuQuaternion orientation;
			orientation.FromAngleAxis(CRuVector3(0.0, 1.0, 0.0), forwardAngleDiff);
			CRuVector3 direction = CRuVector3( 0.0f, 0.0f, -1.0f) * orientation.GetMatrix();

			bool temp = pRoleSpriteFurniture->GetDisableRotate();
			pRoleSpriteFurniture->SetDisableRotate(false);

			pRoleSpriteFurniture->SetDirection( direction , false, true);

			pRoleSpriteFurniture->SetDisableRotate(temp);


			//LOOP 會造成不能旋轉
			if( pRoleSpriteFurniture->GetActorState() == ruFUSION_ACTORSTATE_ACTIVATE_LOOP )
			{
				pRoleSpriteFurniture->SetActorState( ruFUSION_ACTORSTATE_ACTIVATE_END );
			}


			//轉成祥人的角度
			float theta = acos( direction.Dot( CRuVector3(1.0f, 0.0f, 0.0f) ) );
			if ( direction.m_z > 0.0f )
				theta = PI + PI - theta;

			// 轉換為角度
			theta = theta * 180.0f / PI;
	 
			pFurnitureItem->Layout.Angle = theta;

			m_bFurniturLayoutChanged = true;

		}
	}
}

// ----------------------------------------------------------------------------
void CGuildHousesFrame::UpdateHighlightFurnitureSprite()
{
	//m_FocusFurnishingID;    //正在放置的傢俱ID
	//m_MouseEnterFurnishingID; //滑鼠點到的傢俱ID
	CRoleSprite* pRoleSprite = NULL;

	std::map<int,GuildHouseFurnitureItemStruct>::iterator itPos = m_GuildHouseFurnitureList.begin();
	std::map<int,GuildHouseFurnitureItemStruct>::iterator itEnd = m_GuildHouseFurnitureList.end();
	for( ; itPos != itEnd ; ++itPos )
	{
		pRoleSprite = GetRoleSpriteByFurnitureDBID( itPos->second.ItemDBID );
		if( pRoleSprite )
			pRoleSprite->DisableCelHighlight();
	}

	//還沒打開家具擺設介面時，移動到家具上不要有描邊
	if( !m_PlaceFurnitureMode )
	{
		return;
	}

	if( m_FocusFurnitureID != -1 )
	{	
		pRoleSprite = GetRoleSpriteByFurniturePos( m_FocusFurnitureID );
		if( pRoleSprite && pRoleSprite->IsVisible() )
			pRoleSprite->EnableCelHighlight( RUCOLOR_ARGB( 0xFF, 0xFF, 0xAF, 0x20 ) );
	}

	if( m_MouseEnterFurnitureID != m_FocusFurnitureID &&
		m_MouseEnterFurnitureID != -1 )
	{
		pRoleSprite = GetRoleSpriteByFurniturePos( m_MouseEnterFurnitureID );
		if( pRoleSprite && pRoleSprite->IsVisible() )
			pRoleSprite->EnableCelHighlight( RUCOLOR_ARGB( 0xFF, 0x20, 0x20, 0xFF ) );
	}
}

void CGuildHousesFrame::BuildPointOnClick( int SpriteGUID )
{
	const char* BuildStr = NULL;

	map<string,int>::iterator itPos = m_mapBuildPointGItemID.begin();
	map<string,int>::iterator itEnd = m_mapBuildPointGItemID.end();

	for( ; itPos != itEnd ; ++itPos )
	{
		if( itPos->second == SpriteGUID )
		{
			BuildStr = (itPos->first).c_str();
 			int BuildingDBID;
			char PointStr[256];
			if( 2 != sscanf( BuildStr , "%d_%s",  &BuildingDBID , &PointStr ) )
			{
				return;
			}

			lua_State* L = GetLuaState();

			lua_pushstring( L, PointStr );
			lua_setglobal( L, "arg1" );

			lua_pushnumber( L, BuildingDBID );
			lua_setglobal( L, "arg2" );

			SendWorldEvent( UiEvent_GuildHousesOpenBuildingResources );

			return;	
		}
	}
}

int CGuildHousesFrame::GetBuildPointGItemID( int BuildingDBID , const char* PointStr )
{
	//DF_GuildHousesBuildPointGItemID

	char tt[256];
	sprintf( tt , "%d_%s" , BuildingDBID , PointStr );

	map<string,int>::iterator itPos = m_mapBuildPointGItemID.find( tt );

	if( itPos != m_mapBuildPointGItemID.end() )
	{
		return itPos->second;
	}
	int GItemID = DF_GuildHousesBuildPointGItemID + m_BuildPointCount;
	++m_BuildPointCount;

	m_mapBuildPointGItemID[tt] = GItemID;


	return GItemID;
}

void CGuildHousesFrame::ClearBuildPoint()
{
 	map<string,int>::iterator itPos = m_mapBuildPointGItemID.begin();
  	map<string,int>::iterator itEnd = m_mapBuildPointGItemID.end();

	CRoleSprite* pSprite;

	for( ; itPos != itEnd ; ++itPos )
	{
		pSprite = g_pGameMain->FindSpriteByID( itPos->second );
		if( pSprite )
		{
			pSprite->SetVisible( false );
			pSprite->SetCursorState( false ); 
		}
 	}
}

void CGuildHousesFrame::CreateBuildPoint( int BuildingDBID, const char* PointStr, const char* LocalName )
{
	if( m_eHousesState != EHousesState_Enter )
	{
		return;
	}

	GuildBuildingObjectInfoStruct* pGuildBuildingObjectInfo = GetGuildBuildingObjectInfoByID( BuildingDBID );
	if( NULL == pGuildBuildingObjectInfo )
	{
		return ;
	}
  
	CRoleSprite* pSpriteBuilding = g_pGameMain->FindSpriteByID( pGuildBuildingObjectInfo->GItemID );

	if( NULL == pSpriteBuilding )
	{
		return ;
	}

	CRuEntity* pEntityPoint = RuEntity_FindLinkFrame( pSpriteBuilding->GetRuEntity(), PointStr );
	if( NULL == pEntityPoint )
	{
		return ;
	}

	CRuVector3 pos = pEntityPoint->GetTranslation( ruFRAMETYPE_WORLD );

	//g_pWorldMapFrame->AddMapIcon( 20090723, 401, pos.x, pos.z, LocalName , EM_MapIconType_Mine);

	//DF_GuildHousesRoleSpriteWorldGUID
	//int ObjectId = rBuildingInfo.OrgObjID;
  
	//m_BuildingRoleSpriteID 

	//const char* actName = Com_FormatString( "model\\element\\house_system\\building\\housetype_%02d_%02d.ros" , m_HouseBaseInfo.HouseType / 100 , m_HouseBaseInfo.HouseType % 100 );

	int GItemID   = GetBuildPointGItemID( BuildingDBID , PointStr );   
	int WorldGUID = GItemID; 

// 	float Pos_X = 0.0f; 
// 	float Pos_Y = 0.0f; 
// 	float Pos_Z = 0.0f; 

	int DBID = -1;

	CRoleSprite* pSprite = NULL;



	bool isNew;
	pSprite = g_pGameMain->CreateSprite( GItemID, DBID, WorldGUID, isNew );
	//assert( pSprite );
	if ( !pSprite )
	{
		return;
	}

	if( !isNew )
	{
		pSprite->SetPos(pos);
		pSprite->SetVisible( true );
		pSprite->SetCursorState(true);		// 可以被游標檢測
		return;
	}

	//CRuVector3 pos(Pos_X, Pos_Y, Pos_Z);


	//pSprite->SetObjectId(OrgObjID);
	if( false == pSprite->LoadRuEntity( "model\\item\\ui_object\\building_area\\act_building_area.ros" ) )
	{
		return;
	}

	pSprite->PlayMotion( "stand_idle01" );
	//pSprite->SetObjectId( ObjectId );

	pSprite->SetShadowType(eSpriteShadowType_None);		// 沒有影子
	pSprite->SetCursorState(true);		// 可以被游標檢測
	pSprite->SetTouchState(false);		// 可以被點選
	pSprite->SetTargetState(false);		// 不能出現目標框
	pSprite->SetNameVisible(false);		// 不能出現頭上字串

	//case EM_GameObjectMineType_Mine:
	pSprite->SetMinimapIcon( 0 ); // EM_MINIMEM_MinimapOption_Mine
	pSprite->SetCheckedStatus( eChecked_GuildHousesBuildPoint );  

	// Set fixed sprite orienting axis if the axis is valid

	// Set fixed orienting axis
	pSprite->SetOrientingAxis(CRuVector3(0.0, 1.0, 0.0));

	// Enable re-basing of direction vector around the orienting axis
	pSprite->SetRebaseDirection(true);

	// Disable orient to surface normal
	pSprite->SetOrientToSurfaceNormal(false);

	// Set orientation mode to unconstrained
	pSprite->SetOrientMode(CRuFusion_Actor::ruORIENTMODE_UNCONSTRAINED);
  
	pSprite->SetName( LocalName );

	CRuQuaternion orientation;

	float angle = 0 * PI / 180.0f;
	orientation.FromAngleAxis(CRuVector3(0.0, 1.0, 0.0), angle);
	CRuVector3 direction = CRuVector3( 0.0f, 0.0f, -1.0f) * orientation.GetMatrix();
	pSprite->SetDirection( direction , true, true);
	pSprite->SetFade(false, 1.0f);
	pSprite->SetGravityMode(false);

	pSprite->SetPos(pos);

	// 設定sprite顯示
	//pSprite->SetVisible(GraphObj->Mode.Show);
	pSprite->SetVisible( true );

	// 設定sprite碰撞開關
	pSprite->EnableCollision(false);

	pSprite->SetCanAttackState(false);
	pSprite->SetEnemyState(false);

}

/*
// ----------------------------------------------------------------------------
void CGuildHousesFrame::CreateBuildingToGround( int BuildingID )
{
	GuildBuildingInfoTableStruct* pBuildingResource = GetGuildBuildingResourceByID( BuildingID );
	if( NULL == pBuildingResource )
	{
		return;
	}
	  
	m_BuildingID = BuildingID;
	m_eWorkMode = WM_CreateBuildingToGround;

  	int ObjectId = pBuildingResource->OrgObjID;
  

	//m_BuildingRoleSpriteID 
  
	//const char* actName = Com_FormatString( "model\\element\\house_system\\building\\housetype_%02d_%02d.ros" , m_HouseBaseInfo.HouseType / 100 , m_HouseBaseInfo.HouseType % 100 );

	int GItemID   = m_BuildingRoleSpriteID;   
	int WorldGUID = m_BuildingRoleSpriteID; 

	float Pos_X = 0.0f; 
	float Pos_Y = 0.0f; 
	float Pos_Z = 0.0f; 
	float Pos_Dir = 0; 

	int DBID = -1;

	CRoleSprite* pSprite = NULL;

	bool isNew;
	pSprite = g_pGameMain->CreateSprite( GItemID, DBID, WorldGUID, isNew );
	//assert( pSprite );
	if ( !pSprite )
	{
		return;
	}

	CRuVector3 pos(Pos_X, Pos_Y, Pos_Z);
	float angle = Pos_Dir * PI / 180.0f;

	//pSprite->SetObjectId(OrgObjID);
	//pSprite->LoadRuEntity( actName );
	pSprite->SetObjectId( ObjectId );

	pSprite->SetShadowType(eSpriteShadowType_None);		// 沒有影子
	pSprite->SetCursorState(false);		// 可以被游標檢測
	pSprite->SetTouchState(false);		// 可以被點選
	pSprite->SetTargetState(false);		// 不能出現目標框
	pSprite->SetNameVisible(false);		// 不能出現頭上字串

	//case EM_GameObjectMineType_Mine:
	pSprite->SetMinimapIcon( 0 ); // EM_MINIMEM_MinimapOption_Mine
	pSprite->SetCheckedStatus( eChecked_Null );  

	// Set fixed sprite orienting axis if the axis is valid

	// Set fixed orienting axis
	pSprite->SetOrientingAxis(CRuVector3(0.0, 1.0, 0.0));

	// Enable re-basing of direction vector around the orienting axis
	pSprite->SetRebaseDirection(true);

	// Disable orient to surface normal
	pSprite->SetOrientToSurfaceNormal(false);

	pSprite->SetName( "Houses_Test" );
	CRuQuaternion orientation;
	orientation.FromAngleAxis(CRuVector3(0.0, 1.0, 0.0), angle);
	CRuVector3 direction = CRuVector3( 0.0f, 0.0f, -1.0f) * orientation.GetMatrix();
	pSprite->SetDirection( direction , true, true);
	pSprite->SetFade(false, 1.0f);
	pSprite->SetGravityMode(false);

	pSprite->SetPos(pos);

	// 設定sprite顯示
	//pSprite->SetVisible(GraphObj->Mode.Show);
	pSprite->SetVisible( true );

	// 設定sprite碰撞開關
	pSprite->EnableCollision(false);

	pSprite->SetCanAttackState(false);
	pSprite->SetEnemyState(false);

}
*/

// ----------------------------------------------------------------------------
GuildBuildingObjectInfoStruct* CGuildHousesFrame::GetGuildBuildingObjectInfoByID( int ID )
{

	map<int,GuildBuildingObjectInfoStruct>::iterator itPos = m_mapGuildBuildingObjectInfo.find( ID );

	if( itPos != m_mapGuildBuildingObjectInfo.end() )
	{
		return &(itPos->second);	
	}

	return NULL;
}

// ----------------------------------------------------------------------------
GuildBuildingObjectInfoStruct* CGuildHousesFrame::GetGuildBuildingObjectInfoByIndex( int index )
{
	if( m_bBuildingObjectVectorDirty )
	{
		m_vecGuildBuildingObjectInfo.clear();


		map<int,GuildBuildingObjectInfoStruct>::iterator itPos = m_mapGuildBuildingObjectInfo.begin();
		map<int,GuildBuildingObjectInfoStruct>::iterator itEnd = m_mapGuildBuildingObjectInfo.end();

		for( ; itPos != itEnd ; ++itPos )
		{
			m_vecGuildBuildingObjectInfo.push_back( &itPos->second );
		}
		m_bBuildingObjectVectorDirty = false;
	}

	if( index < 0 || index >= (int)m_vecGuildBuildingObjectInfo.size() )
	{
		return NULL;
	}

	return  m_vecGuildBuildingObjectInfo[index];
}
void CGuildHousesFrame::BuyHouseStyleResult( int Type , int Result )
{
	if (Result)
	{

		m_guildStyle=_GetHouseStyleIndex(Type);
	}
	else
	{

	}

	SendWorldEvent( "GUILDHOUSESTYLE_UPDATE" );

}
// ----------------------------------------------------------------------------
int CGuildHousesFrame::GetGuildBuildingResourceCount()
{
	return (int)g_ObjectData->GuildBuildingTable().size();
}

GuildBuildingInfoTableStruct*   CGuildHousesFrame::GetReviewStyleModel(int ID,int index,int type)
{
	static GuildBuildingInfoTableStruct s_styleObj; 

	GuildBuildingInfoTableStruct* ret=NULL;


	
	if (type==-1)
		type=m_guildStyle;


	


	map<int,GuildBuildingInfoTableStruct>& rGuildBuildingTable = g_ObjectData->GuildBuildingTable();
	map<int,GuildBuildingInfoTableStruct>::iterator itPos = rGuildBuildingTable.find( ID );

	if( itPos != rGuildBuildingTable.end() )
	{
			ret=&itPos->second;
	}	

	map< int , map< int , int > >::iterator it=g_ObjectData->_GuildHouseNpcReplace.find(type);
	if (it!=g_ObjectData->_GuildHouseNpcReplace.end())
	{

		map< int , int >::iterator sit=it->second.find(index);

		if  (sit!=it->second.end())
		{
		
			
				s_styleObj=*ret;//clone
				ret=&s_styleObj;//

				ret->OrgObjID=sit->second; //change style ID

			
		}


	}
	
	

	return ret;
}
// ----------------------------------------------------------------------------
GuildBuildingInfoTableStruct* CGuildHousesFrame::GetGuildBuildingResourceByID( int ID ,int type)
{
	static GuildBuildingInfoTableStruct s_styleObj; 

	GuildBuildingInfoTableStruct* ret=NULL;

	if (type==-1)
		type=m_guildStyle;



	map<int,GuildBuildingInfoTableStruct>& rGuildBuildingTable = g_ObjectData->GuildBuildingTable();
 	map<int,GuildBuildingInfoTableStruct>::iterator itPos = rGuildBuildingTable.find( ID );

	if( itPos != rGuildBuildingTable.end() )
	{	
		ret=&itPos->second;
		if (type!=-1)
		{
				
			int ID=sGuildHouse_GetStyleID(type,ret->OrgObjID);
			if (ID!=-1)
			{
				s_styleObj=*ret;//clone
				ret=&s_styleObj;//

				ret->OrgObjID=ID; //change style ID

			}
		}
	}
	return ret;
}

// ----------------------------------------------------------------------------
GuildBuildingInfoTableStruct* CGuildHousesFrame::GetGuildBuildingResourceByIndex( int index )
{
	if( m_bBuildingResourceVectorDirty )
	{
		m_vecBuildingResourceTable.clear();

		map<int,GuildBuildingInfoTableStruct>& rGuildBuildingTable = g_ObjectData->GuildBuildingTable();
		map<int,GuildBuildingInfoTableStruct>::iterator itPos = rGuildBuildingTable.begin();
		map<int,GuildBuildingInfoTableStruct>::iterator itEnd = rGuildBuildingTable.end();

		for( ; itPos != itEnd ; ++itPos )
		{
			m_vecBuildingResourceTable.push_back( &itPos->second );

			//紀錄所有的建築點
			m_BuildPoints.insert( itPos->second.BuildPoint );
		}
		
		m_bBuildingResourceVectorDirty = false;
	}

	if( index < 0 || index >= (int)m_vecBuildingResourceTable.size() )
	{
		return NULL;
	}

	return  m_vecBuildingResourceTable[index];
}

// ----------------------------------------------------------------------------
ItemFieldStruct* CGuildHousesFrame::GetGuildBankItem( int index )
{
	if( m_eHousesState != EHousesState_Enter )
	{
		return NULL;
	}

	if( false == m_bGuildBankOpen )
	{
		return NULL;
	}

	int pageID = index / DF_GuildBankItemMax;
	int itemPos = index % DF_GuildBankItemMax;

	if( pageID < 0 || pageID >= DF_GuildBankPageMax )
	{
		return NULL;
	}
  
	GuildBankPage& rPage = m_GuildBankPages[pageID];
	int rank = g_pGuildFrame->GetUserRank();
	if( rank < 0 || rank >= EM_GuildRank_Count )
	{
		return NULL;
	}
	if( false == rPage.GuildBankConfig.Rank[ rank ].View )
	{
		return NULL;
	}

	//GuildBankPage& rPage = m_GuildBankPages[pageID];

	return &rPage.Items[itemPos];
}

// ----------------------------------------------------------------------------
void CGuildHousesFrame::GuildBank_Pickup(int index)
{
	if( m_eHousesState != EHousesState_Enter )
	{
		return;
	}

	if( false == m_bGuildBankOpen )
	{
		return;
	}


	int pageID = index / DF_GuildBankItemMax;
	if( pageID < 0 || pageID >= DF_GuildBankPageMax )
	{
		return ;
	}
	GuildBankPage& rPage = m_GuildBankPages[pageID];
	int rank = g_pGuildFrame->GetUserRank();
	if( rank < 0 || rank >= EM_GuildRank_Count )
	{
		return ;
	}
	if( false == rPage.GuildBankConfig.Rank[ rank ].View )
	{
		return ;
	}
	
	ItemFieldStruct* destItem = GetGuildBankItem(index);
	ClipboardItemInfo itemInfo = g_pItemClipboard->GetItemInfo();

	// 拿取與做用物品相同
	if( g_pItemClipboard->GetType() == eClipboardItem_Type_GuildBank && itemInfo.guildbank.pos == index )
	{
		g_pItemClipboard->Clear();
		return;
	}

	if ( destItem == NULL || destItem->Pos != EM_ItemState_NONE )
		return;

	// 存在物品對物品使用中
	int useIndex = g_pBagFrame->GetUseItemIndex();
	if ( useIndex != -1 )
	{
		//if ( !g_pBagFrame->UseItemToItem(2, index) )
		//	g_pBagFrame->UseItemCancel();
		return;
	}

	switch( g_pItemClipboard->GetType() )
	{	
	case eClipboardItem_Type_None:				// 拿取物品
		{
			ItemFieldStruct* item = GetGuildBankItem(index);
			if ( item && item->Pos == EM_ItemState_NONE )
			{
				GameObjDbStruct* itemDB = CNetGlobal::GetObj( item->OrgObjID );
				if ( itemDB )
				{
					item->Pos = EM_ItemState_CliDisabled;
					//g_pItemClipboard->SetFile( itemDB->ImageObj->ACTField );
					g_pItemClipboard->SetFile( ItemUTIL::GetItemIcon( item ) );
					g_pItemClipboard->PickupGuildBank( item->OrgObjID, index );
					m_pickupItemIndex = index;
					return ;
				}
			}
		}
		return;

	case eClipboardItem_Type_Bag:				// 背包 -> 公會銀行
		{
			int pos = itemInfo.bag.pos;
			ItemFieldStruct* bankItem = GetGuildBankItem(index);
			ItemFieldStruct* item = CNetGlobal::RoleData()->GetBodyItem( pos );
			if ( item )
			{
				if ( pos < _MAX_BODY_EX_COUNT_ && bankItem->IsEmpty() == false )
				{
					// 不能放回商城背包
					//g_pGameMain->SendWarningMsg( g_ObjectData->GetString("SYS_BANK_TO_GOODS_FAILED") );
					return;
				}
				else
				{
					g_pBagFrame->ClearPickupItemIndex();
					bankItem->Pos = item->Pos = EM_ItemState_CliDisabled;
					//NetCli_Item::_ExchangeField_BodyBank(*item, pos, *bankItem , index);
					NetCli_GuildHouses::SL_SwapBodyHouseItem( pos , index / DF_GuildBankItemMax , index % DF_GuildBankItemMax , true );
				}
			}
		}

		break;

	case eClipboardItem_Type_GuildBank:				// 公會銀行 -> 公會銀行
		{
			int pos = itemInfo.guildbank.pos;

			if( pos == index )
			{
 				break;
			}


			ItemFieldStruct* bankItem1 = GetGuildBankItem(pos);
 			ItemFieldStruct* bankItem2 = GetGuildBankItem(index);

			bankItem1->Pos = bankItem2->Pos = EM_ItemState_CliDisabled;

			int HouseItemPos1 = pos   % DF_GuildBankItemMax; 
			int HouseItemPos2 = index % DF_GuildBankItemMax; 
			int HouseItemPageID1 = pos   / DF_GuildBankItemMax; 
			int HouseItemPageID2 = index / DF_GuildBankItemMax;
	  
			NetCli_GuildHouses::SL_SwapHouseItemRequest( HouseItemPos2, HouseItemPos1, HouseItemPageID2,  HouseItemPageID1 );
			//Deposit( index, eBank_Type_Bank, itemInfo.guildbank.pos );
  		}

		break;

	case eClipboardItem_Type_Equipment:			// 裝備 -> 銀行
	//	Deposit(index, eBank_Type_Equipment, itemInfo.equipment.pos);
	//	break;

	case eClipboardItem_Type_Trade:
	case eClipboardItem_Type_Action:
	case eClipboardItem_Type_Skill:
	case eClipboardItem_Type_Shop:
	case eClipboardItem_Type_Macro:
	case eClipboardItem_Type_Emote:
	case eClipboardItem_Type_House:
		break;
	}

	// clear item clipboard data
	g_pItemClipboard->Clear();
}


//擺放家具
// ----------------------------------------------------------------------------
void CGuildHousesFrame::SetPlaceFurnitureMode( bool bMode )
{
	if( m_eHousesState != EHousesState_Enter )
	{
		return;
	}

	m_PlaceFurnitureMode = bMode;
	UpdateHighlightFurnitureSprite();
}

// ----------------------------------------------------------------------------
void CGuildHousesFrame::PickupFurniture( int Index )
{
	if( m_eHousesState != EHousesState_Enter )
	{
		return;
	}

	ClipboardItemInfo itemInfo = g_pItemClipboard->GetItemInfo();


	switch( g_pItemClipboard->GetType() )
	{	
	case eClipboardItem_Type_None:				// 拿取物品
		{

			GuildHouseFurnitureItemStruct* pFurnitureItem = GetGuildHouseFurnitureItem( Index );

			if( NULL == pFurnitureItem )
			{
				break;
			}

			GameObjDbStruct* itemDB = CNetGlobal::GetObj( pFurnitureItem->Item.OrgObjID );
			if ( NULL == itemDB )
			{
				break;
			}

			g_pItemClipboard->SetFile( ItemUTIL::GetItemIcon( &pFurnitureItem->Item ) );
			g_pItemClipboard->PickupGuildFurniture( pFurnitureItem->Item.OrgObjID, Index );

			return;
		}
	case eClipboardItem_Type_Bag: // 背包
		{
			NetCli_GuildHouses::SL_SwapBodyHouseFurniture( itemInfo.bag.pos, Index );

			break;
		}
	case eClipboardItem_Type_GuildFurniture: // 公會家具 -> 
		{
			NetCli_GuildHouses::SL_SwapHouseFurnitureRequest( itemInfo.guildfurniture.pos , Index );

			break;
		}
	}

	g_pItemClipboard->Clear();

}
int CGuildHousesFrame::GetLadderCount()
{

	return (int)m_ladderList.size();
}
int CGuildHousesFrame::GetLadderHistoryCount()
{
	return m_LadderHistoryCount;

}
// ----------------------------------------------------------------------------
void CGuildHousesFrame::FurniturLayoutChange()
{
	if( m_eHousesState != EHousesState_Enter )
	{
		return;
	}

	CRoleSprite* pRoleSprite = g_pGameMain->FindSpriteByWorldGUID( m_FurnitureRoleSpriteID );

	if( pRoleSprite )
	{
		pRoleSprite->SetVisible( false );
	}

  	if( m_FocusFurnitureID == -1 || m_bFurniturLayoutChanged == false )
		return;

	GuildHouseFurnitureItemStruct* pFurnitureItem = GetGuildHouseFurnitureItem( m_FocusFurnitureID );
	
	if(!pFurnitureItem)
	{
		return;
	}

	//NetCli_Houses::S_HouseItemLayoutRequest( pHouseItemDB->ItemDBID, pHouseItemDB->Layout );

 	int BuildingObjID = GetFurnitureGItemID( pFurnitureItem->ItemDBID );

	//if( BuildingObjID < 0 )
	//{
	//	return;
	//}
	
	NetCli_GuildHouses::SL_HouseItemLayoutRequest( BuildingObjID , pFurnitureItem->Pos , pFurnitureItem->Layout );

	m_bFurniturLayoutChanged = false;
}


// ----------------------------------------------------------------------------
void CGuildHousesFrame::SetFocusFurnitureID( int ID )
{
	if( m_eHousesState != EHousesState_Enter )
	{
		return;
	}

	FurniturLayoutChange();

	m_FocusFurnitureID = ID;

	m_ePlaceingMode = m_ePlaceingModeTemp;
	m_PlaceStep = 0;

	UpdateHighlightFurnitureSprite();
	SendWorldEvent( UiEvent_GuildHouseFurnitureChanged );
}

// ----------------------------------------------------------------------------
void CGuildHousesFrame::FurniturePlace ()
{
	if( m_eHousesState != EHousesState_Enter )
	{
		return;
	}

	if( m_FocusFurnitureID == -1 )
	{
		return;
	}

	GuildHouseFurnitureItemStruct* pFurnitureItem = GetGuildHouseFurnitureItem( m_FocusFurnitureID );

	if( NULL == pFurnitureItem )
	{
		m_ePlaceingMode = m_ePlaceingModeTemp;
		return;
	}

	if( pFurnitureItem->OwnerDBID != g_pGameMain->GetDBID() )
	{
		m_ePlaceingMode = m_ePlaceingModeTemp;
		m_PlaceStep = 0;
		g_pGameMain->SendSystemMsg( g_ObjectData->GetString( "GUILDHOUSE_NOT_FURNITURE_OWNER" ) );
		return;
	}

	//if( pFurnishingDB->Layout.IsShow == false )
	{
		pFurnitureItem->Layout.IsShow = true;

		pFurnitureItem->Layout.X = 0.0f; 
		pFurnitureItem->Layout.Y = 0.0f;
		pFurnitureItem->Layout.Z = 0.0f;     
		pFurnitureItem->Layout.UpX = 0.0f; 
		pFurnitureItem->Layout.UpY = 1.0f; 
		pFurnitureItem->Layout.UpZ = 0.0f;   
		pFurnitureItem->Layout.Angle = 90.0f; 

		//LoadRoleSpriteHouseItem( pFurnishingDB );
		SetFocusFurnitureID( m_FocusFurnitureID );

		m_ePlaceingMode = EM_PlaceingMode_New;  //放置模式  
		m_PlaceStep = 0;       //放置模式步驟 1移動 2旋轉

		m_PlaceFurnitureMode = true;
	}
	
	int ObjectId = pFurnitureItem->Item.OrgObjID;
	  

	int GItemID   = m_FurnitureRoleSpriteID;   
	int WorldGUID = m_FurnitureRoleSpriteID; 

	float Pos_X = 0.0f; 
	float Pos_Y = 0.0f; 
	float Pos_Z = 0.0f; 
	//float Pos_Dir = 0; 

	int DBID = -1;

	CRoleSprite* pSprite = NULL;

	bool isNew;
	pSprite = g_pGameMain->CreateSprite( GItemID, DBID, WorldGUID, isNew );
	//assert( pSprite );
	if ( !pSprite )
	{
		return;
	}

	CRuVector3 pos(Pos_X, Pos_Y, Pos_Z);
	float angle = pFurnitureItem->Layout.Angle * PI / 180.0f;

	//pSprite->SetObjectId(OrgObjID);
	//pSprite->LoadRuEntity( actName );
	if( false == pSprite->SetObjectId( ObjectId ) )
	{
		return ;
	}

	pSprite->SetShadowType(eSpriteShadowType_None);		// 沒有影子
	pSprite->SetCursorState(false);		// 可以被游標檢測
	pSprite->SetTouchState(false);		// 可以被點選
	pSprite->SetTargetState(false);		// 不能出現目標框
	pSprite->SetNameVisible(false);		// 不能出現頭上字串

	//case EM_GameObjectMineType_Mine:
	pSprite->SetMinimapIcon( 0 ); // EM_MINIMEM_MinimapOption_Mine
	pSprite->SetCheckedStatus( eChecked_Null );  

	// Set fixed sprite orienting axis if the axis is valid

	// Set fixed orienting axis
	pSprite->SetOrientingAxis(CRuVector3(0.0, 1.0, 0.0));

	// Enable re-basing of direction vector around the orienting axis
	pSprite->SetRebaseDirection(true);

	// Disable orient to surface normal
	pSprite->SetOrientToSurfaceNormal(false);

	// Set orientation mode to unconstrained
	pSprite->SetOrientMode(CRuFusion_Actor::ruORIENTMODE_UNCONSTRAINED);

	pSprite->SetName( "Houses_Test" );

	pSprite->SetDirection(CRuVector3(cos(angle), 0.0f, -sin(angle)), true, true);

	pSprite->SetFade(false, 1.0f);
	pSprite->SetGravityMode(false);

	pSprite->SetPos(pos);

	// 設定sprite顯示
	//pSprite->SetVisible(GraphObj->Mode.Show);
	pSprite->SetVisible( true );

	// 設定sprite碰撞開關
	pSprite->EnableCollision(false);

	pSprite->SetCanAttackState(false);
	pSprite->SetEnemyState(false);

	
	// 設定紙娃娃部位裝備
	pSprite->SetPaperdollPart( PAPERDOLL_PART_NAME_HEAD      );
	pSprite->SetPaperdollPart( PAPERDOLL_PART_NAME_HAIR      );			
	pSprite->SetPaperdollPart( PAPERDOLL_PART_NAME_HELMET    ); //頭盔
	pSprite->SetPaperdollPart( PAPERDOLL_PART_NAME_HAND      ); //手
	pSprite->SetPaperdollPart( PAPERDOLL_PART_NAME_SHOULDER  ); //肩
	pSprite->SetPaperdollPart( PAPERDOLL_PART_NAME_TORSO     ); //軀幹
	pSprite->SetPaperdollPart( PAPERDOLL_PART_NAME_LEG       ); //腿
	pSprite->SetPaperdollPart( PAPERDOLL_PART_NAME_BELT      ); //腰帶
	pSprite->SetPaperdollPart( PAPERDOLL_PART_NAME_FOOT      ); //腳
	pSprite->SetPaperdollPart( PAPERDOLL_PART_NAME_BACK      ); //背部

}

// ----------------------------------------------------------------------------
void CGuildHousesFrame::FurnitureRemove()
{
	if( m_eHousesState != EHousesState_Enter )
	{
		return;
	}

	if( m_FocusFurnitureID == -1 )
	{
		return;
	}

	GuildHouseFurnitureItemStruct* pFurnitureItem = GetGuildHouseFurnitureItem( m_FocusFurnitureID );
  
	if( NULL == pFurnitureItem )
	{
		m_ePlaceingMode = m_ePlaceingModeTemp;
		m_PlaceStep = 0;
		return;
	}


	if( pFurnitureItem->OwnerDBID != g_pGameMain->GetDBID() )
	{
		m_ePlaceingMode = m_ePlaceingModeTemp;
		m_PlaceStep = 0;
		g_pGameMain->SendSystemMsg( g_ObjectData->GetString( "GUILDHOUSE_NOT_FURNITURE_OWNER" ) );
		return;
	}


	if( pFurnitureItem->Layout.IsShow == true )
	{
		pFurnitureItem->Layout.IsShow = false;
		pFurnitureItem->Layout.X = 0.0f; 
		pFurnitureItem->Layout.Y = 0.0f;
		pFurnitureItem->Layout.Z = 0.0f;     
		pFurnitureItem->Layout.UpX = 0.0f; 
		pFurnitureItem->Layout.UpY = 1.0f; 
		pFurnitureItem->Layout.UpZ = 0.0f;   
		pFurnitureItem->Layout.Angle = 0.0f; 

		m_ePlaceingMode = m_ePlaceingModeTemp; //放置模式  
		m_PlaceStep = 0;         //放置模式步驟 1移動 2旋轉

		//CRoleSprite* pRoleSprite = GetHouseRoleSpriteByItemDBID( pFurnishingDB->ItemDBID );
		//if( pRoleSprite )
		//{
		//	pRoleSprite->SetVisible( false );
		//	pRoleSprite->EnableCollision(FALSE);
		//	pRoleSprite->SetCursorState( false ); 
		//}

		m_bFurniturLayoutChanged = true;
		FurniturLayoutChange();
	}	
}

// ----------------------------------------------------------------------------
void CGuildHousesFrame::FurnitureMove  ()
{
	m_ePlaceingMode = EM_PlaceingMode_Move; //放置模式  
	m_ePlaceingModeTemp = EM_PlaceingMode_Move;
	m_PlaceStep = 0;         //放置模式步驟 1移動 2旋轉
}

// ----------------------------------------------------------------------------
void CGuildHousesFrame::FurnitureRorare()
{
	m_ePlaceingMode = EM_PlaceingMode_Rorare; //放置模式  
	m_ePlaceingModeTemp = EM_PlaceingMode_Rorare;
	m_PlaceStep = 0;         //放置模式步驟 1移動 2旋轉
}


// ----------------------------------------------------------------------------
GuildHouseFurnitureItemStruct* CGuildHousesFrame::GetGuildHouseFurnitureItem( int pos )
{
	std::map<int,GuildHouseFurnitureItemStruct>::iterator itPos = m_GuildHouseFurnitureList.find(pos);

	if( itPos == m_GuildHouseFurnitureList.end() )
	{
		return NULL;
	}

	return &itPos->second;
}

// ----------------------------------------------------------------------------
void CGuildHousesFrame::OpenMenuRequest()
{
	//g_pGameMain->SetGuildBattleGroundState( false );
	if( timeGetTime() - m_GuildWarMenuOpenTime < 3000 )
	{
		return;
	}
	m_GuildWarMenuOpenTime = timeGetTime();

	m_mapGuildHouseWarInfo.clear();
	m_vecGuildHouseWarInfo.clear();
	m_mapZoneGroupGuildCount.clear();
	m_ZoneGroupID = -1;



	NetCli_BG_GuildWar::SL_OpenMenuRequest(); 
}

// ----------------------------------------------------------------------------
void CGuildHousesFrame::RegisterRequest(bool sw,int index)
{
	int targetDBID=-1;

	if( index >=0 && index < g_pGuildHousesFrame->m_vecGuildHouseWarInfo.size() )
	{
		GuildHouseWarInfoStruct* pGuildInfo = &g_pGuildHousesFrame->m_vecGuildHouseWarInfo[index];
		if (pGuildInfo)
		{
			targetDBID=pGuildInfo->GuildID;

		}
		//return 0;
	}

	


	int ZoneGroupMax = g_ObjectData->GetMoneyKeyValue( "GuildHousesWarZoneGroupMax" , 0 );
	int ZoneGroupLowest = g_ObjectData->GetMoneyKeyValue( "GuildHousesWarZoneGroupLowest" , 0 );

	if( ZoneGroupMax <= 0 ) ZoneGroupMax = DF_ZoneGroupMax;
	if( ZoneGroupLowest <= 0 ) ZoneGroupLowest = DF_ZoneGroupLowest;
	
	m_GuildWarRegisterType = EM_GuildWarRegisterType_Register;

	if( m_ZoneGroupID != -1 )
	{
		return ;
	}

	if( m_mapZoneGroupGuildCount.size() == 0 )
	{
		return ;
	}

  	int ZoneGroupID = -1;


	std::map<int,int>::iterator itPos;
	std::map<int,int>::iterator itEnd;


	//找最少的奇數Group
	int mini = ZoneGroupMax;

	itPos = m_mapZoneGroupGuildCount.begin();
	itEnd = m_mapZoneGroupGuildCount.end();

	for( ; itPos != itEnd ; ++itPos )
	{
		if( itPos->second %2 == 1 )
		{
			if( itPos->second < mini )
			{
				mini = itPos->second;
				ZoneGroupID = itPos->first;	
			}
		}
	}
 	
	if( ZoneGroupID == -1 )
	{
		//先把每個Group捕到 DF_ZoneGroupLowest
		itPos = m_mapZoneGroupGuildCount.begin();
		itEnd = m_mapZoneGroupGuildCount.end();

		for( ; itPos != itEnd ; ++itPos )
		{
			if( itPos->second < ZoneGroupLowest )
			{
				ZoneGroupID = itPos->first;	
				break;
			}
		}
	}

	if( ZoneGroupID == -1 )
	{
		//找最少的Group

		int mini = ZoneGroupMax;

		itPos = m_mapZoneGroupGuildCount.begin();
		itEnd = m_mapZoneGroupGuildCount.end();

		for( ; itPos != itEnd ; ++itPos )
		{
			if( itPos->second < mini )
			{
				mini = itPos->second;
				ZoneGroupID = itPos->first;	
			}
		}
 	}

	if( ZoneGroupID == -1 )
	{
		//滿載
		string msg = g_ObjectData->GetString( "GUILDWAR_REGISTER_FAIL1" );
		g_pGameMain->SendSystemMsg( msg.c_str() );
		g_pGameMain->SendSystemChatMsg( msg.c_str() );
		return;
	}

		int count = g_pGuildHousesFrame->m_mapZoneGroupGuildCount[ZoneGroupID];
	if ( g_pGameMain->IsShowServerDebugMsg() ) 
	{
		g_pGameMain->SendSystemChatMsgEx( "[SL_RegisterRequest] ZoneGroupID = %d IsAcceptAssignment=%d,count=%d ", ZoneGroupID,sw,count);
	}

 
 	
	NetCli_BG_GuildWar::SL_RegisterRequest( ZoneGroupID, EM_GuildWarRegisterType_Register, count ,sw,targetDBID);

	m_bIsAcceptAssignment=sw;
	m_targetDBID=targetDBID;
	m_GuildWarRegistering = true;
}

// ----------------------------------------------------------------------------
void CGuildHousesFrame::CancelRegister()
{
	m_GuildWarRegisterType = EM_GuildWarRegisterType_Cancel;

	if( m_ZoneGroupID == -1 )
	{
		return ;
	}

	NetCli_BG_GuildWar::SL_RegisterRequest( m_ZoneGroupID, EM_GuildWarRegisterType_Cancel, 0 );


	//m_GuildWarRegistering = true;
}

// ----------------------------------------------------------------------------
void CGuildHousesFrame::RBL_GuildNameResult( int WorldGuildID , const char* GuildName )
{

}

// ----------------------------------------------------------------------------
void CGuildHousesFrame::RL_OpenMenuResult( int PlayerDBID , int ZoneGroupID , GuildHouseWarStateENUM State , int NextTime ,int Size , GuildHouseWarInfoStruct List[1000] )
{
	if( Size > 1000 )
	{
		//
		return;
	}
/*
EM_GuildWarStatusType_Prepare		,
EM_GuildWarStatusType_Begin			,
EM_GuildWarStatusType_End			,
EM_GuildWarStatusType_Appointment	,
*/
	switch (State)
	{
	case EM_GuildHouseWarState_None			://	未知
	case EM_GuildHouseWarState_Appointment	://	預約
		m_GuildWarStatus=EM_GuildWarStatusType_Appointment;
		break;
	case EM_GuildHouseWarState_Prepare		://	準備		
	case EM_GuildHouseWarState_Failed		://	沒有配到隊
		m_GuildWarStatus=EM_GuildWarStatusType_Prepare;
		break;
	case EM_GuildHouseWarState_Fight		://	戰爭中
		m_GuildWarStatus=EM_GuildWarStatusType_Begin;

		break;
	case EM_GuildHouseWarState_FightEnd		://	戰爭結束

	case EM_GuildHouseWarState_FightEnd_Even://	戰爭平手
	case EM_GuildHouseWarState_FightEnd_Win	://	戰爭勝利
	case EM_GuildHouseWarState_FightEnd_Lost://	戰爭失敗
		{
		
			m_GuildWarStatus=	EM_GuildWarStatusType_End;
			
		}
	
		break;

	}
	GuildBaseStruct* pGuildInfo =  g_pGuildFrame->GetGuildBaseInfo();


	if (pGuildInfo)
	{
		NetCli_Guild::SL_GuildInfoRequet(pGuildInfo->GuildID);			
	}		
	m_NextTime=(float)NextTime;
	m_OpenMenuResultTimer = timeGetTime();

	m_GuildWarMenuOpenTime = -1;


	m_GuildHouseCurrentState = State;

//	m_mapGuildHouseWarInfo.clear();
//	m_vecGuildHouseWarInfo.clear();

	m_mapZoneGroupGuildCount[ZoneGroupID] = Size;

	int i;
	for( i = 0 ; i < Size ; ++i )
	{
		GuildHouseWarInfoStruct &ref=List[i];
		if (ref.State==EM_GuildHouseWarState_Fight)
		{
			if (ref.EnemyGuildID<=0)
				ref.State=EM_GuildHouseWarState_Failed;

		}
		m_mapGuildHouseWarInfo[ List[i].GuildID ] = List[i];
 	//	m_vecGuildHouseWarInfo.push_back( List[i] );
	}

	if (m_GuildWarRegistering&&m_targetDBID>0)
	{
		std::map<int,GuildHouseWarInfoStruct>::iterator it=m_mapGuildHouseWarInfo.find(m_targetDBID);
		if (it!=m_mapGuildHouseWarInfo.end())
		{
			if (it->second.EnemyGuildID>0)
			{
				m_GuildWarRegistering=false;
				string msg = g_ObjectData->GetString( "C_GHW_MESSAGE_02" );
				g_pGameMain->SendSystemMsg( msg.c_str() );
				g_pGameMain->SendSystemChatMsg( msg.c_str() );

				m_targetDBID=0;
			}
			
		}
		else
		{
			m_GuildWarRegistering=false;
			string msg = g_ObjectData->GetString( "C_GHW_MESSAGE_01" );
			g_pGameMain->SendSystemMsg( msg.c_str() );
			g_pGameMain->SendSystemChatMsg( msg.c_str() );

			m_targetDBID=0;
		}



	}

	if( m_mapZoneGroupGuildCount.size() >= m_ZoneGroupMax )
	{
		m_ZoneGroupMax = (int)m_mapZoneGroupGuildCount.size();
		UpdateGuildHouseWarInfos();

		if( m_GuildWarRegistering )
		{
			int index=-1;

			for(int i=0;i< m_vecGuildHouseWarInfo.size();i++)
			{
				GuildHouseWarInfoStruct& GuildInfo =m_vecGuildHouseWarInfo[i];
				if (GuildInfo.GuildID==m_targetDBID)
				{
					index=i;
					break;
				}								
			}
			
			RegisterRequest(m_bIsAcceptAssignment,index);
		}
	}
}

// ----------------------------------------------------------------------------
void CGuildHousesFrame::RL_RegisterResult( GuildWarRegisterResultENUM Result, GuildHouseWarInfoStruct& Info )
{
	//EM_GuildWarRegisterResult_OK				,
	//EM_GuildWarRegisterResult_UnknowErr		,
	//EM_GuildWarRegisterResult_Full			,
	//EM_GuildWarRegisterResult_GuildCountErr	,
	//EM_GuildWarRegisterResult_NoGuildHouse	,	//沒有公會屋
	//EM_GuildWarRegisterResult_NotLeader		,	//不是會長


	if( EM_GuildWarRegisterResult_GuildCountErr == Result &&!Info.IsAcceptAssignment)
	{

		string msg = g_ObjectData->GetString( "GUILDWAR_REGISTER_FAIL4" );
		g_pGameMain->SendSystemMsg( msg.c_str() );
		g_pGameMain->SendSystemChatMsg( msg.c_str() );

		OpenMenuRequest();
		return;
	}

	m_GuildWarRegistering = false;

	if( EM_GuildWarRegisterResult_Full == Result )
	{
		if( m_GuildWarRegisterType == EM_GuildWarRegisterType_Register )
		{
			string msg = g_ObjectData->GetString( "GUILDWAR_REGISTER_FAIL1" );
			g_pGameMain->SendSystemMsg( msg.c_str() );
			g_pGameMain->SendSystemChatMsg( msg.c_str() );
		}

		return;
	}
	if( Result == EM_GuildWarRegisterResult_OnceaDay )
	{
		string msg = g_ObjectData->GetString( "GUILDWAR_REGISTER_FAIL3" );
		g_pGameMain->SendSystemMsg( msg.c_str() );
		g_pGameMain->SendSystemChatMsg( msg.c_str() );
		return;
	}

	 
		
	if( Result != EM_GuildWarRegisterResult_OK )
	{
		
		return;
	}
	
	if( m_GuildWarRegisterType == EM_GuildWarRegisterType_Register )
	{
		//m_mapGuildHouseWarInfo[ Info.GuildID ] = Info;
		//m_vecGuildHouseWarInfo.push_back( Info );
		//UpdateGuildHouseWarInfos();

		m_targetDBID=0;
		OpenMenuRequest();
	}
	else if( m_GuildWarRegisterType == EM_GuildWarRegisterType_Cancel )
	{
		//m_mapGuildHouseWarInfo.erase( Info.GuildID );
		//
		//std::vector<GuildHouseWarInfoStruct>::iterator itPos = m_vecGuildHouseWarInfo.begin();
		//std::vector<GuildHouseWarInfoStruct>::iterator itEnd = m_vecGuildHouseWarInfo.end();

		//for( ; itPos != itEnd ; ++itPos )
		//{
		//	if( (*itPos).GuildID == Info.GuildID )
		//	{
		//		m_vecGuildHouseWarInfo.erase( itPos );
		//		break;
		//	}
		//}
		//
		//m_ZoneGroupID = -1;
		//UpdateGuildHouseWarInfos();

		OpenMenuRequest();
	}
	else if( m_GuildWarRegisterType == EM_GuildWarRegisterResult_UnknowErr )
	{
		string msg = g_ObjectData->GetString( "GUILDWAR_REGISTER_FAIL5" );
		g_pGameMain->SendSystemMsg( msg.c_str() );
		g_pGameMain->SendSystemChatMsg( msg.c_str() );

		m_targetDBID=0;
	
		OpenMenuRequest();
	}

}

// ----------------------------------------------------------------------------
void CGuildHousesFrame::RL_Status(GuildWarStatusTypeENUM Status,int NextTime )
{
	m_NextTime=(float)NextTime;
	//m_GuildHouseCurrentState = Status;
	//NetCli_BG_GuildWar::SL_OpenMenuRequest();
	if( m_GuildWarStatus == Status )
	{
		return;
	}

	m_GuildWarStatus = Status;

	if( Status == EM_GuildWarStatusType_Prepare )
	{
		string msg = g_ObjectData->GetString( "SYS_GUILDWARSTATUS_PREPARE" );
		g_pGameMain->SendScrollBannerMessage( msg.c_str() );
	}
	else if( Status == EM_GuildWarStatusType_Begin )
	{
		string msg = g_ObjectData->GetString( "SYS_GUILDWARSTATUS_BEGIN" );
		g_pGameMain->SendScrollBannerMessage( msg.c_str() );
	}
	lua_State* L = GetLuaState();
	lua_pushnumber( L, NextTime  );
	lua_setglobal( L, "arg1" );

	SendWorldEvent(UiEvent_GuildHouseWarStateChange);
}
void CGuildHousesFrame::GuildWarHistroy( )
{

	if (m_historyRefTime<0)
	{

		NetCli_Guild::SL_GuildWarHistroy();
	}

}
void CGuildHousesFrame::GuildWarHistroyResult(int count,GuildHouseWarHistoryStruct log[] )
{
	m_warrepots.clear();

	m_warrepots.resize(count);

	for(int i=0;i<count;i++)
	{
		m_warrepots[i]=log[i];

	}



	SendWorldEvent("GUILDWARREPORT_UPDATE");


	m_historyRefTime=5*60;
}
// ----------------------------------------------------------------------------
void CGuildHousesFrame::RL_EnterResult(bool Result)
{
	if(!Result){
		return;
	}
	SendWorldEvent(UiEvent_GuildHouseWarEnterResult);
}
// ----------------------------------------------------------------------------
void CGuildHousesFrame::RBL_LeaveResult(bool Result )
{

	SendWorldEvent(UiEvent_GuildHouseWarEndWar);
}

// ----------------------------------------------------------------------------
void CGuildHousesFrame::RBL_GuildScore( GuildHouseWarFightStruct& Info )
{	
	g_pGuildHouseWarStatus->GetGuildScore( Info );
	
	SendWorldEvent(UiEvent_GuildHouseWarScore);
}

// ----------------------------------------------------------------------------
void CGuildHousesFrame::RBL_BeginWar()
{
	SendWorldEvent(UiEvent_GuildHouseWarBeginWar);
}

// ----------------------------------------------------------------------------
void CGuildHousesFrame::RBL_EndWar()
{
	g_pGuildHouseWarStatus->EndWarCalc();
	SendWorldEvent(UiEvent_GuildHouseWarEndWar);
}

// --------------------------------------------------------------------------------------
bool CompGuildHouseWarInfo( const GuildHouseWarInfoStruct& Info1 , const GuildHouseWarInfoStruct& Info2 )
{
	if( Info1.Score < Info2.Score )
	{
		return false;
	}

	return true;
}


// ----------------------------------------------------------------------------
void CGuildHousesFrame::UpdateGuildHouseWarInfos()
{
	//m_vecGuildHouseWarInfo.clear();
 
	{
		m_vecGuildHouseWarInfo.clear();
		std::map<int,GuildHouseWarInfoStruct>::iterator itPos = m_mapGuildHouseWarInfo.begin();
		std::map<int,GuildHouseWarInfoStruct>::iterator itEnd = m_mapGuildHouseWarInfo.end();
	
		for( ; itPos != itEnd ; ++itPos )
		{
			m_vecGuildHouseWarInfo.push_back( itPos->second );
		}
	}

	//找出m_ZoneGroupID

	m_ZoneGroupID = -1;
	
	int worldID = g_pAccountLogin->GetWorldID();

	const char* szGuildName = g_pGuildFrame->GetGuildName();

	string GuildName;

	if( szGuildName )
	{
		GuildName = szGuildName;
	}

	//
	int findCount = 0;

	std::vector<GuildHouseWarInfoStruct>::iterator itPos = m_vecGuildHouseWarInfo.begin();
	std::vector<GuildHouseWarInfoStruct>::iterator itEnd = m_vecGuildHouseWarInfo.end();
	for( ; itPos != itEnd ; ++itPos )
	{
		GuildHouseWarInfoStruct& rInfo = *itPos;

		if( worldID == (rInfo.GuildID / 0x1000000) && GuildName == rInfo.GuildName.Begin() )
		{
			if( findCount > 0 )
			{
				m_GuildWarRegisterType = EM_GuildWarRegisterType_Cancel;
				NetCli_BG_GuildWar::SL_RegisterRequest( rInfo.ZoneGroupID, EM_GuildWarRegisterType_Cancel, 0 );
			}
			else
			{
				m_ZoneGroupID = rInfo.ZoneGroupID;
			}
			++findCount;
		}
	}

	std::sort( m_vecGuildHouseWarInfo.begin() , m_vecGuildHouseWarInfo.end() , CompGuildHouseWarInfo );

	//配對 將重複的剔除

	std::set<int> setEnemyGuildID;
	std::vector<GuildHouseWarInfoStruct> vecTemp;
	itPos = m_vecGuildHouseWarInfo.begin();
	itEnd = m_vecGuildHouseWarInfo.end();
 
	for( ; itPos != itEnd ; ++itPos )
	{
		GuildHouseWarInfoStruct& rInfo = *itPos;
		if( setEnemyGuildID.find( rInfo.GuildID ) != setEnemyGuildID.end() )
		{
			continue;
		}

		vecTemp.push_back( rInfo );
		setEnemyGuildID.insert( rInfo.EnemyGuildID );	
	}

	m_vecGuildHouseWarInfo = vecTemp;
	lua_State* L = GetLuaState();
	lua_pushnumber( L, m_NextTime );
	lua_setglobal( L, "arg1" );
	SendWorldEvent( UiEvent_GuildHouseWarInfosUpdate );
}

// ----------------------------------------------------------------------------
GuildHouseWarInfoStruct* CGuildHousesFrame::GetGuildHouseWarInfoStruct( int GuildID )
{
	std::map<int,GuildHouseWarInfoStruct>::iterator itPos = m_mapGuildHouseWarInfo.find( GuildID );

	if( itPos != m_mapGuildHouseWarInfo.end() )
	{
		return &itPos->second;
	}

	return NULL;
}

// ----------------------------------------------------------------------------
int LuaFunc_GuildHouses_VisitHouseRequest( lua_State* L )
{
	int GuildID = -1;

	if( lua_isnumber( L, 1 ) )
	{
		GuildID = (int)luaL_checknumber( L, 1 ) ;
	}
	else if( lua_isstring( L, 1 ) )
	{
		const char* GuildName = luaL_checkstring( L, 1 ) ;
		GuildID = g_pGuildFrame->GetGuildIDByName( GuildName );
	}

	if( GuildID > 0 )
	{
		NetCli_GuildHouses::SL_VisitHouseRequest( GuildID );
	}
	
	return 0;
}
// ----------------------------------------------------------------------------
int LuaFunc_GuildHouses_CloseVisitHouse( lua_State* L )
{
	NetCli_GuildHouses::SL_CloseVisitHouse();	
	return 0;
}
// ----------------------------------------------------------------------------
int LuaFunc_GuildHouses_CreateBuilding( lua_State* L )
{
	const char* Point = luaL_checkstring( L, 1 );
 	int BuildingID = luaL_checkint( L, 2 );
 	int ParentBuildingDBID = luaL_checkint( L, 3 );

	//if( strcmp( Point , "ground" ) == 0 )
	//{
	//	g_pGuildHousesFrame->CreateBuildingToGround( BuildingID );
	//	return 0;
	//}
   
	/*
	GuildBuildingObjectInfoStruct* pBuildingObject = g_pGuildHousesFrame->GetGuildBuildingObjectInfoByID( ParentBuildingDBID );

	if( NULL == pBuildingObject )
	{
		return 0;
	}
  
	CRoleSprite* pSprite = g_pGameMain->FindSpriteByID( pBuildingObject->GItemID );
	
	if( NULL == pSprite )
	{
		return 0;
	}

	CRuEntity* pEntity = pSprite->GetRuEntity();

	if( NULL == pEntity )
	{
		return 0;
	}

	CRuEntity* pBuildingPoint = RuEntity_FindLinkFrame( pEntity, Point, TRUE );
	if( NULL == pBuildingPoint )
	{
		return 0;
	}

	const CRuQuaternion& rRotation = pBuildingPoint->GetRotation( ruFRAMETYPE_WORLD );

	CRuVector3 vecDir( 0, 0, 1 );
	vecDir = vecDir * rRotation.GetMatrix();

	// 取得角色所朝方向
	float theta = acos( vecDir.Dot( CRuVector3(1.0f, 0.0f, 0.0f) ) );
	if ( vecDir.m_z > 0.0f )
		theta = PI + PI - theta;

	// 轉換為角度
	theta = theta * 180.0f / PI;

	
 	NetCli_GuildHouses::SL_CreateBuildingRequest( (char*)Point , BuildingID , ParentBuildingDBID , theta );

	*/
	NetCli_GuildHouses::SL_CreateBuildingRequest( (char*)Point , BuildingID , ParentBuildingDBID , 0 );
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GuildHouses_BuildingUpgrade( lua_State* L )
{
	int BuildingDBID = luaL_checkint( L, 1 );
	int UpgradeBuildingID = luaL_checkint( L, 2 );
	NetCli_GuildHouses::SL_BuildingUpgradeRequest( BuildingDBID , UpgradeBuildingID );
	return 0;
}


// ----------------------------------------------------------------------------
int LuaFunc_GuildHouses_DeleteBuilding( lua_State* L )
{
	int BuildingDBID = luaL_checkint( L, 1 ) ;

	NetCli_GuildHouses::SL_DeleteBuildingRequest( BuildingDBID );
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GuildHouses_GetBuildingCount( lua_State* L )
{
	int count = g_pGuildHousesFrame->GetGuildBuildingObjectInfoCount();
   	lua_pushnumber( L, count );	
	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_GuildHouses_GetBuilding( lua_State* L )
{
	int index = luaL_checkint( L, 1 ) - 1;

	GuildBuildingObjectInfoStruct* pGuildBuildingObjectInfo = g_pGuildHousesFrame->GetGuildBuildingObjectInfoByIndex( index );

	if( NULL == pGuildBuildingObjectInfo )
	{
		return 0;
	}
 
	////int					GItemID;
	//int					BuildingParentDBID;
	//int					BuildingDBID;
	//int					BuildingID;
	////int					Dir;
	//char				PointStr[32];

	map<int,int>::iterator itPos = g_pGuildHousesFrame->m_mapBuildingActive.find( pGuildBuildingObjectInfo->BuildingDBID );

	bool isActive = false;
	if( itPos == g_pGuildHousesFrame->m_mapBuildingActive.end() || itPos->second == 0 )
	{

	}
	else
	{
		isActive = true;
	}

	lua_pushnumber( L, pGuildBuildingObjectInfo->BuildingParentDBID );	
	lua_pushnumber( L, pGuildBuildingObjectInfo->BuildingDBID       );	
	lua_pushnumber( L, pGuildBuildingObjectInfo->BuildingID         );	
	lua_pushstring( L, pGuildBuildingObjectInfo->PointStr           );
	lua_pushboolean( L, isActive );

	return 5;
}

// ----------------------------------------------------------------------------
int LuaFunc_GuildHouses_GetBuildingResourceCount( lua_State* L )
{
	int count = g_pGuildHousesFrame->GetGuildBuildingResourceCount();
	lua_pushnumber( L, count );	
	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_GuildHouses_GetStyleCount( lua_State* L )
{
	int count = sGuildHousesFrame_GetStyleCount();
	lua_pushnumber( L, count );	
	return 1;
}
// ----------------------------------------------------------------------------
int LuaFunc_GuildHouses_GetStyleInfo( lua_State* L )
{
	int index = luaL_checkint( L, 1 ) - 1 ;

	GuildHouseNpcReplaceCostTableStruct* pBuildingResource = sGuildHousesFrame_GetGuildStyleInfoTableStructByIndex( index );
	if (pBuildingResource)
	{
		int type=pBuildingResource->Type;
		bool upgrap=false;
		GuildBuildingObjectInfoStruct* pGuildBuildingObject = g_pGuildHousesFrame->GetGuildBuildingObjectInfoByIndex( 0 );
		GuildBuildingObjectInfoStruct* pGuildBuildingObject1 = g_pGuildHousesFrame->GetGuildBuildingObjectInfoByIndex( 1);
		if(  pGuildBuildingObject &&pGuildBuildingObject1)
		{

			GuildBuildingInfoTableStruct* pBuildingResource = g_pGuildHousesFrame->GetGuildBuildingResourceByID( pGuildBuildingObject->BuildingID,type );
			GuildBuildingInfoTableStruct* pBuildingResource1 = g_pGuildHousesFrame->GetGuildBuildingResourceByID( pGuildBuildingObject1->BuildingID ,type);
			if(  pBuildingResource&&pBuildingResource1 )
			{
				upgrap=true;

			}

		
		}

	
		lua_pushnumber( L, pBuildingResource->Type );
		lua_pushnumber( L, pBuildingResource->GuildLv );
		lua_pushnumber( L, pBuildingResource->Cost._Value[0] );
		lua_pushnumber( L, pBuildingResource->Cost._Value[1] );
		lua_pushnumber( L, pBuildingResource->Cost._Value[2] );
		lua_pushnumber( L, pBuildingResource->Cost._Value[3] );
		lua_pushnumber( L, pBuildingResource->Cost._Value[4] );
		lua_pushnumber( L, pBuildingResource->Cost._Value[5] );
		lua_pushnumber( L, pBuildingResource->Cost._Value[6] );		
		lua_pushboolean( L, upgrap);		//可否升級





		return 10;
	}
	return 0;
}
//---------------------------------------------------------------------------
int LuaFunc_GuildHouses_GetGuildStyle( lua_State* L )
{

	lua_pushnumber( L,g_pGuildHousesFrame->m_guildStyle);

	return 1;
}
// ----------------------------------------------------------------------------
int LuaFunc_GuildHouses_GetBuildingResource( lua_State* L )
{
	int index = luaL_checkint( L, 1 ) - 1 ;
  
	GuildBuildingInfoTableStruct* pBuildingResource = g_pGuildHousesFrame->GetGuildBuildingResourceByIndex( index );

	if( NULL == pBuildingResource )
	{
		return 0;
	}

	GameObjDbStructEx* pObjDB = g_ObjectData->GetObj( pBuildingResource->OrgObjID );
	if ( pObjDB == NULL )
		return 0;

  	lua_pushnumber( L, pBuildingResource->ID );

  	lua_pushstring( L, pObjDB->GetName() );

 	lua_pushstring( L, pBuildingResource->BuildPoint );	
	lua_pushstring( L, pBuildingResource->TypeStr );	
  	lua_pushnumber( L, pBuildingResource->Lv );	

  	lua_pushnumber( L, pBuildingResource->SrcBuildingID );

	lua_pushnumber( L, pBuildingResource->Resource.Gold       );	
	lua_pushnumber( L, pBuildingResource->Resource.BonusGold  );	
	lua_pushnumber( L, pBuildingResource->Resource.Mine       );	
	lua_pushnumber( L, pBuildingResource->Resource.Wood       );	
	lua_pushnumber( L, pBuildingResource->Resource.Herb       );	
	lua_pushnumber( L, pBuildingResource->Resource.GuildRune  );	
	lua_pushnumber( L, pBuildingResource->Resource.GuildStone );	

  	lua_pushstring( L, pBuildingResource->NeedTypeStr1 );	
  	lua_pushnumber( L, pBuildingResource->NeedLv1      );	
  	lua_pushstring( L, pBuildingResource->NeedTypeStr2 );	
  	lua_pushnumber( L, pBuildingResource->NeedLv2      );	

  	lua_pushnumber( L, pBuildingResource->NeedGuildLv  );	

//	lua_pushnumber( L, pBuildingResource->BlockSizeX );	
//	lua_pushnumber( L, pBuildingResource->BlockSizeY );	

	lua_pushnumber( L, pBuildingResource->Mode.Remove );


	string strShortNote = CNetCli_Script_Child::ReplaceSystemKeyword( pObjDB->ShortNote );

	lua_pushstring( L, strShortNote.c_str() );

 	lua_pushstring( L, pBuildingResource->IconPath );

  	lua_pushnumber( L, pBuildingResource->MaintenanceCharge  );	

  	lua_pushnumber( L, pBuildingResource->SortID  );	

	return 23;

}

vector<string>* _GetBuildingPointArrayList( int ID )
{
	static vector<string> s_PointStrList;

	static int s_ID = -1;

	if( s_ID != ID )
	{
		s_PointStrList.clear();

		GuildBuildingObjectInfoStruct* pGuildBuildingObjectInfo = g_pGuildHousesFrame->GetGuildBuildingObjectInfoByID( ID );

		if( NULL == pGuildBuildingObjectInfo )
		{
			return NULL;
		}

		int BuildingID = pGuildBuildingObjectInfo->BuildingID;
		GuildBuildingInfoTableStruct* pBuildingResource = g_pGuildHousesFrame->GetGuildBuildingResourceByID( BuildingID );
		if( NULL == pBuildingResource )
		{
			return NULL;
		}

		GameObjDbStructEx* pObjDB = g_ObjectData->GetObj( pBuildingResource->OrgObjID );
		if( !pObjDB )
		{
			return NULL;
		}

		GameObjDbStructEx* imageDB = CNetGlobal::GetObj( pObjDB->ImageID );
		if ( imageDB == NULL || imageDB->Image.ACTWorld[0] == 0 )
		{
			return NULL;
		}
		
		CRuEntity* pEntity = g_ruResourceManager->LoadEntity( imageDB->Image.ACTWorld );

		if( NULL == pEntity )
		{
			return NULL;
		}
		
		CRuArrayList<CRuLinkFrame *> linkFrames;
		RuEntity_EnumerateLinkFrames( pEntity, linkFrames );

		for( int i = 0; i < linkFrames.Count(); ++i )
		{
			s_PointStrList.push_back( linkFrames[i]->GetLinkName() );
		}

		pEntity->Release();

		s_ID = ID;	
	}
	
	return &s_PointStrList;
}

// ----------------------------------------------------------------------------
int LuaFunc_GuildHouses_GetBuildingPointCount( lua_State* L )
{

	// 抓出所有參考點
	//CRuArrayList<CRuLinkFrame *> linkFrames;
	//RuEntity_EnumerateLinkFrames(rootEntity, linkFrames);
	//for(INT32 i = 0; i < linkFrames.Count(); ++i)
	//{
	//	const char *linkName = linkFrames[i]->GetLinkName();
	//}
	int ID = luaL_checkint( L, 1 );

	vector<string>* pPointStrList = _GetBuildingPointArrayList( ID );

	if( NULL == pPointStrList )
	{
		return 0;
	}

   	lua_pushnumber( L, (lua_Number)pPointStrList->size() );

	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_GuildHouses_GetBuildingPoint( lua_State* L )
{
	int ID = luaL_checkint( L, 1 );
 	int index = luaL_checkint( L, 2 ) - 1;

	vector<string>* pPointStrList = _GetBuildingPointArrayList( ID );

	if( NULL == pPointStrList )
	{
		return 0;
	}

	if( index >= (int)pPointStrList->size() )
	{
		return 0;
	}

	const char *linkName = (*pPointStrList)[index].c_str();
	
	if( NULL == linkName )
	{
		return 0;
	}

	lua_pushstring( L, linkName );

	return 1;
}

int LuaFunc_GuildHouses_GetBuildingPointInfo( lua_State* L )
{
	int ID = luaL_checkint( L, 1 );
	int index = luaL_checkint( L, 2 ) - 1;

	vector<string>* pPointStrList = _GetBuildingPointArrayList( ID );

	if( NULL == pPointStrList )
	{
		return 0;
	}

	if( index >= (int)pPointStrList->size() )
	{
		return 0;
	}

	const char *linkName = (*pPointStrList)[index].c_str();

	if( NULL == linkName )
	{
		return 0;
	}

	//

	GuildBuildingObjectInfoStruct* pGuildBuildingObjectInfo = g_pGuildHousesFrame->GetGuildBuildingObjectInfoByID( ID );

	if( NULL == pGuildBuildingObjectInfo )
	{
		return 0;
	}

	int BuildingID = pGuildBuildingObjectInfo->BuildingID;
	GuildBuildingInfoTableStruct* pBuildingResource = g_pGuildHousesFrame->GetGuildBuildingResourceByID( BuildingID );
	if( NULL == pBuildingResource )
	{
		return 0;
	}

	GameObjDbStructEx* pObjDB = g_ObjectData->GetObj( pBuildingResource->OrgObjID );
	if( pObjDB == NULL )
	{
		return 0;
	}

	GameObjDbStructEx* imageDB = CNetGlobal::GetObj(pObjDB->ImageID);
	if ( imageDB == NULL || imageDB->Image.ACTWorld[0] == 0 )
	{
		return 0;
	}

	CRuEntity* pEntity = g_ruResourceManager->LoadEntity( imageDB->Image.ACTWorld );

	if( NULL == pEntity )
	{
		return 0;
	}

	CRuEntity* pBuildingPoint = RuEntity_FindLinkFrame( pEntity, linkName, TRUE );
	if( NULL == pBuildingPoint )
	{
		return 0;
	}


	const CRuVector3& rPosition = pBuildingPoint->GetTranslation( ruFRAMETYPE_LOCAL );
	const CRuQuaternion& rRotation = pBuildingPoint->GetRotation( ruFRAMETYPE_LOCAL );

	CRuVector3 vecDir( 0, 0, 1 );
	vecDir = vecDir * rRotation.GetMatrix();

	lua_pushstring( L, linkName );
	lua_pushnumber( L, rPosition.x );
	lua_pushnumber( L, rPosition.y );
	lua_pushnumber( L, rPosition.z );
	lua_pushnumber( L, vecDir.x );
	lua_pushnumber( L, vecDir.y );
	lua_pushnumber( L, vecDir.z );

	pEntity->Release();

	return 7;
}

int LuaFunc_GuildHouses_ClearBuildPoint( lua_State* L )
{
	g_pGuildHousesFrame->ClearBuildPoint();
	return 0;
}

int LuaFunc_GuildHouses_CreateBuildPoint( lua_State* L )
{
	int BuildingDBID = luaL_checkint( L, 1 );
	const char* PointStr = luaL_checkstring( L, 2 );
 	const char* LocalName = luaL_checkstring( L, 3 );
 	g_pGuildHousesFrame->CreateBuildPoint( BuildingDBID, PointStr, LocalName );

	return 0;
}


int LuaFunc_GuildHouses_GetBuildingPos( lua_State* L )
{
	int BuildingDBID = luaL_checkint( L, 1 );

	GuildBuildingObjectInfoStruct* pGuildBuildingObjectInfo = g_pGuildHousesFrame->GetGuildBuildingObjectInfoByID( BuildingDBID );
	if( NULL == pGuildBuildingObjectInfo )
	{
		return 0;
	}

	CRoleSprite* pSpriteBuilding = g_pGameMain->FindSpriteByID( pGuildBuildingObjectInfo->GItemID );

	if( NULL == pSpriteBuilding )
	{
		return 0;
	}

	CRuVector3 pos = pSpriteBuilding->GetRuEntity()->GetTranslation( ruFRAMETYPE_WORLD );

	const CRuSphere& rSphere = pSpriteBuilding->GetRuEntity()->GetWorldBounds();

  	lua_pushnumber( L, pos.x );
	lua_pushnumber( L, pos.z );
	//lua_pushnumber( L, rSphere.GetCenter().x );
	//lua_pushnumber( L, rSphere.GetCenter().z );
  	lua_pushnumber( L, rSphere.GetRadius() );

	return 3;
}

int LuaFunc_GuildHouses_SetBuildingActive( lua_State* L )
{
	int BuildingDBID = luaL_checkint( L, 1 );
	bool isActive = luaL_checkint( L, 2 ) == 1 ? true : false;

	NetCli_GuildHouses::SL_ActiveBuildingRequest( BuildingDBID, isActive );

	return 0;
}

int LuaFunc_GuildHouses_GetBuildingPointPos( lua_State* L )
{
	int BuildingDBID = luaL_checkint( L, 1 );
	const char* PointStr = luaL_checkstring( L, 2 );

	GuildBuildingObjectInfoStruct* pGuildBuildingObjectInfo = g_pGuildHousesFrame->GetGuildBuildingObjectInfoByID( BuildingDBID );
	if( NULL == pGuildBuildingObjectInfo )
	{
		return 0;
	}

	CRoleSprite* pSpriteBuilding = g_pGameMain->FindSpriteByID( pGuildBuildingObjectInfo->GItemID );

	if( NULL == pSpriteBuilding )
	{
		return 0;
	}

	CRuEntity* pEntityPoint = RuEntity_FindLinkFrame( pSpriteBuilding->GetRuEntity(), PointStr );
	if( NULL == pEntityPoint )
	{
		return 0;
	}

	CRuVector3 pos = pEntityPoint->GetTranslation( ruFRAMETYPE_WORLD );


	lua_pushnumber( L, pos.x );
	lua_pushnumber( L, pos.z );

	return 2;
}


// ----------------------------------------------------------------------------------
#include "../../ui/core/uimodel.h"
// ----------------------------------------------------------------------------------
//顯示社公會屋建築物模型
int Model_SetGuildHouseBuilding(lua_State* L)
{
	CUiModel* model = dynamic_cast<CUiModel*>(CUiLuaLayout::CheckLayoutTable(L, 1));
	if (model == NULL)
		lua_error(L);

	int n = lua_gettop(L);

	int DBID=0;
	

	int id = luaL_checkint( L, 2 ) ;
	GuildBuildingInfoTableStruct* pBuildingResource = g_pGuildHousesFrame->GetGuildBuildingResourceByID( id );
	if( NULL == pBuildingResource )
	{
		return NULL;
	}
	DBID= pBuildingResource->OrgObjID ;
	
	
	//int		OrgObjID;				//npc 參考物件ID
	GameObjDbStructEx* pObjDB = g_ObjectData->GetObj(DBID);
	if ( pObjDB == NULL )
		return 0;

	GameObjDbStructEx* imageDB = CNetGlobal::GetObj(pObjDB->ImageID);
	if ( imageDB == NULL || imageDB->Image.ACTWorld[0] == 0 )
		return 0;

	model->CreateEntity( imageDB->Image.ACTWorld );

	CRuEntity* pEntity = model->GetRuEntity();

	if( NULL == pEntity )
	{
		return 0;
	}

	CRuArrayList<CRuLinkFrame *> linkFrames;
	RuEntity_EnumerateLinkFrames( pEntity, linkFrames );
	int i;
	vector<string> PointStrList;
	for( i = 0; i < linkFrames.Count(); ++i )
	{
		PointStrList.push_back( linkFrames[i]->GetLinkName() );
	}

	std::set<std::string>::iterator itPos;

	int pos;
	string strType;
   
	CRuEntity* pEntityPoint;
	for( i = 0; i < PointStrList.size() ; ++i )
	{

		strType = PointStrList[i].c_str();

		pos = (int)strType.find( '_' );
		if( pos == string::npos )  //沒有關鍵字
		{
  			continue;		
		}

		strType.erase( pos );

			itPos = g_pGuildHousesFrame->m_BuildPoints.find( strType );

		if( itPos == g_pGuildHousesFrame->m_BuildPoints.end() )
		{
			continue;
		}
   
		pEntityPoint = g_ruResourceManager->LoadEntity( "model\\item\\ui_object\\building_area\\act_building_area.ros" );

		if( NULL == pEntityPoint )
		{
			continue;
		}
   
		RuEntity_Attach_BFS( pEntity, pEntityPoint, PointStrList[i].c_str() );
		RuACT_PlayMotionEx( pEntityPoint, -1, "stand_idle01" );

		ruSAFE_RELEASE( pEntityPoint );
	}


	return 0;
}


// ----------------------------------------------------------------------------------
int LuaFunc_GuildBank_PageRequest( lua_State* L )
{
	if( false == g_pGuildHousesFrame->m_bGuildBankOpen )
	{
		return 0;
	}

	//	static void SL_HouseItemRequest			( int PageID );
	//	static void SL_SwapBodyHouseItem		( int BodyPos , int HouseItemPageID , int HouseItemPos , bool IsFromBody );
	//	static void SL_SwapHouseItemRequest		( int HouseItemPos1 , int HouseItemPos2 , int HouseItemPageID1 , int HouseItemPageID2 );

	int PageID = luaL_checkint( L, 1 ) - 1;
	g_pGuildHousesFrame->GuildBank_PageRequest( PageID );

	return  0;
}


// ----------------------------------------------------------------------------------
int LuaFunc_GuildBank_GetItemInfo( lua_State* L )
{
	if( false == g_pGuildHousesFrame->m_bGuildBankOpen )
	{
		return 0;
	}

	//int PageID = luaL_checkint( L, 1 ) - 1;
	//int PosID  = luaL_checkint( L, 2 ) - 1;

	//if( PageID < 0 || PageID >= DF_GuildBankPageMax )
	//{
	//	return 0;
	//}

	//if( PosID < 0 || PosID >= DF_GuildBankItemMax )
	//{
	//	return 0;
	//}

	//ItemFieldStruct* item = &g_pGuildHousesFrame->m_GuildBankItems[ PageID * DF_GuildBankItemMax + PosID ];


	int PosID  = luaL_checkint( L, 1 ) - 1;

	if( PosID < 0 || PosID >= DF_GuildBankItemMax * DF_GuildBankPageMax )
	{
		return 0;
	}
	
	ItemFieldStruct* item = g_pGuildHousesFrame->GetGuildBankItem( PosID );
  
	if ( item )
	{
		GameObjDbStructEx* itemDB = CNetGlobal::GetObj(item->OrgObjID);
		if (itemDB)
		{
			//lua_pushstring(L, itemDB->ImageObj->ACTField);			// push item texture file

			lua_pushstring(L, ItemUTIL::GetItemIcon(item) );			// push item texture file
			lua_pushstring(L, itemDB->GetName());						// push Item name
			lua_pushnumber(L, item->GetCount());					// push item count
			lua_pushboolean(L, item->Pos != EM_ItemState_NONE);		// push item is locked
			return 4;
		}
	}

	return 0;
}


// ----------------------------------------------------------------------------------
int	LuaFunc_GuildBank_PickupItem( lua_State* L )
{
	if( false == g_pGuildHousesFrame->m_bGuildBankOpen )
	{
		return 0;
	}

	g_pGuildHousesFrame->GuildBank_Pickup( luaL_checkint(L, 1) - 1 );
	return 0;
}


// ----------------------------------------------------------------------------------
int	LuaFunc_GuildBank_Close( lua_State* L )
{
	//g_pGuildHousesFrame->Close();
	g_pGuildHousesFrame->m_bGuildBankOpen = false;
	return 0;	
}

// ----------------------------------------------------------------------------------
int LuaFunc_GuildBank_GetPageCount( lua_State* L )
{
	if( false == g_pGuildHousesFrame->m_bGuildBankOpen )
	{
		return 0;
	}
	lua_pushnumber( L, g_pGuildHousesFrame->m_GuildBankPageCount );
	return 1;
}

// ----------------------------------------------------------------------------------
int LuaFunc_GuildBank_GetPageMax( lua_State* L )
{
	if( false == g_pGuildHousesFrame->m_bGuildBankOpen )
	{
		return 0;
	}

	vector<GuildHouseItemPageCostTableStruct>& rGuildHousePageCostTable = g_ObjectData->GetGuildHousePageCostTable();
	//lua_pushnumber( L, rGuildHousePageCostTable.size() );

	int count = (int)rGuildHousePageCostTable.size();
	
	//目前最多只能有10頁
	count = min( count , DF_GuildBankPageMax );

	lua_pushnumber( L, DF_GuildBankPageMax );
	return 1;
}

// ----------------------------------------------------------------------------------
int LuaFunc_GuildBank_GetPageCost( lua_State* L )
{
	if( false == g_pGuildHousesFrame->m_bGuildBankOpen )
	{
		return 0;
	}

	vector<GuildHouseItemPageCostTableStruct>& rGuildHousePageCostTable = g_ObjectData->GetGuildHousePageCostTable();

	int pageCount = g_pGuildHousesFrame->m_GuildBankPageCount;
	int pageMax = (int)rGuildHousePageCostTable.size();

	if( pageCount >= pageMax )
	{
		return 0;
	}

   GuildHouseItemPageCostTableStruct& rPageCostTable = rGuildHousePageCostTable[pageCount];

   lua_pushnumber( L, rPageCostTable.Resource.Gold       );	
   lua_pushnumber( L, rPageCostTable.Resource.BonusGold  );	
   lua_pushnumber( L, rPageCostTable.Resource.Mine       );	
   lua_pushnumber( L, rPageCostTable.Resource.Wood       );	
   lua_pushnumber( L, rPageCostTable.Resource.Herb       );	
   lua_pushnumber( L, rPageCostTable.Resource.GuildRune  );	
   lua_pushnumber( L, rPageCostTable.Resource.GuildStone );

   return 7;
}

// ----------------------------------------------------------------------------------
int LuaFunc_GuildBank_BuyPage( lua_State* L )
{
	if( false == g_pGuildHousesFrame->m_bGuildBankOpen )
	{
		return 0;
	}

	NetCli_GuildHouses::SL_BuyItemPageRequest( g_pGuildHousesFrame->m_GuildBankPageCount );
	return 0;
}


// ----------------------------------------------------------------------------------
int LuaFunc_GuildBank_SetStoreConfigBegin( lua_State* L )
{
	if( false == g_pGuildHousesFrame->m_bGuildBankOpen )
	{
		return 0;
	}

	int PageID = luaL_checkint( L, 1 ) - 1;

	if( PageID < 0 || PageID >= DF_GuildBankPageMax )
	{
		return 0;
	}

	CGuildHousesFrame::GuildBankPage& rPage = g_pGuildHousesFrame->m_GuildBankPages[PageID];

	g_pGuildHousesFrame->m_GuildBankConfigTemp = rPage.GuildBankConfig;
	return 0;
}

// ----------------------------------------------------------------------------------
int LuaFunc_GuildBank_SetStoreConfig( lua_State* L )
{
	if( false == g_pGuildHousesFrame->m_bGuildBankOpen )
	{
		return 0;
	}

	//int PageID = luaL_checkint( L, 1 ) - 1;

	int rank     = luaL_checkint( L, 1 ) - 1;
	bool view    = luaL_checkint( L, 2 ) == 1 ? true : false;
	bool put     = luaL_checkint( L, 3 ) == 1 ? true : false;
	//bool get     = luaL_checkint( L, 4 ) == 1 ? true : false;
	int getCount = luaL_checkint( L, 4 );

	bool get = false;

	if( rank < 0 || rank >= EM_GuildRank_Count )
	{
		return 0;
	}

	if( getCount > 99 )
	{
		getCount = 99;
	}

	if( getCount < 0 )
	{
		getCount = 0;
	}

	if( getCount > 0 )
	{
		get = true;
	}
	
	g_pGuildHousesFrame->m_GuildBankConfigTemp.Rank[rank].View = view;
 	g_pGuildHousesFrame->m_GuildBankConfigTemp.Rank[rank].Put = put;
	g_pGuildHousesFrame->m_GuildBankConfigTemp.Rank[rank].Get = get;
	g_pGuildHousesFrame->m_GuildBankConfigTemp.Rank[rank].GetCount = getCount;
	
	return 0;
}

// ----------------------------------------------------------------------------------
int LuaFunc_GuildBank_SetStoreConfigEnd( lua_State* L )
{
	if( false == g_pGuildHousesFrame->m_bGuildBankOpen )
	{
		return 0;
	}

	NetCli_GuildHouses::SL_SetStoreConfig( g_pGuildHousesFrame->m_GuildBankConfigTemp );
	return 0;
}

// ----------------------------------------------------------------------------------
int LuaFunc_GuildBank_GetStoreConfig( lua_State* L )
{
	if( false == g_pGuildHousesFrame->m_bGuildBankOpen )
	{
		return 0;
	}

	int pageID = luaL_checkint( L, 1 ) - 1;
	int rank   = luaL_checkint( L, 2 ) - 1;
	//int rank = g_pGuildFrame->GetUserRank();
	if( pageID < 0 || pageID >= DF_GuildBankPageMax )
	{
		return 0;
	}

	if( rank < 0 || rank >= EM_GuildRank_Count )
	{
		return 0;
	}

	CGuildHousesFrame::GuildBankPage& rPage = g_pGuildHousesFrame->m_GuildBankPages[pageID];

	lua_pushnumber( L, rPage.GuildBankConfig.Rank[rank].View  );
	lua_pushnumber( L, rPage.GuildBankConfig.Rank[rank].Put   );
	//lua_pushnumber( L, rPage.GuildBankConfig.Rank[rank].Get   );
 	lua_pushnumber( L, rPage.GuildBankConfig.Rank[rank].GetCount   );

	return 3;
}

// ----------------------------------------------------------------------------------
int LuaFunc_GuildBank_ItemLogRequest( lua_State* L )
{
	if( false == g_pGuildHousesFrame->m_bGuildBankOpen )
	{
		return 0;
	}

	int pageID = luaL_checkint( L, 1 ) - 1;
	if( pageID < 0 || pageID >= DF_GuildBankPageMax )
	{
		return 0;
	}
 	g_pGuildHousesFrame->GuildBank_PageLogRequest( pageID );
	return 0;
}

// ----------------------------------------------------------------------------------

int LuaFunc_GuildBank_GetItemLogCount( lua_State* L )
{
	if( false == g_pGuildHousesFrame->m_bGuildBankOpen )
	{
		return 0;
	}

	int pageID = luaL_checkint( L, 1 ) - 1;


	if( pageID < 0 || pageID >= DF_GuildBankPageMax )
	{
		return 0;
	}

	CGuildHousesFrame::GuildBankPage& rPage = g_pGuildHousesFrame->m_GuildBankPages[pageID];


	g_pGuildHousesFrame->m_LogTemp.clear();

	std::multimap<int,GuildItemStoreLogStruct>::reverse_iterator itPos = rPage.Logs.rbegin();
	std::multimap<int,GuildItemStoreLogStruct>::reverse_iterator itEnd = rPage.Logs.rend();

	for( ; itPos != itEnd ; ++itPos )
	{
		g_pGuildHousesFrame->m_LogTemp.push_back( itPos->second );
 	}
   
	lua_pushnumber( L, (lua_Number)g_pGuildHousesFrame->m_LogTemp.size() );
	return 1;
}

// ----------------------------------------------------------------------------------
int LuaFunc_GuildBank_GetItemLog( lua_State* L )
{
	if( false == g_pGuildHousesFrame->m_bGuildBankOpen )
	{
		return 0;
	}

	int pageID = luaL_checkint( L, 1 ) - 1;
	int index   = luaL_checkint( L, 2 ) - 1;

	if( pageID < 0 || pageID >= DF_GuildBankPageMax )
	{
		return 0;
	}
	
//	CGuildHousesFrame::GuildBankPage& rPage = g_pGuildHousesFrame->m_GuildBankPages[pageID];
	
	if( index < 0 || index >= (int)g_pGuildHousesFrame->m_LogTemp.size() )
	{
		return 0;
	}

	GuildItemStoreLogStruct& rLog = g_pGuildHousesFrame->m_LogTemp[index];

	//-------------------------------------------------------------------------
	//enum GuildHouseLootTypeENUM
	//{
	//	EM_GuildHouseLootType_Put	,		
	//	EM_GuildHouseLootType_Get	,		
	//};
	////公會倉庫存取Log
	//struct GuildItemStoreLogStruct
	//{
	//	int GuildID;
	//	int PageID;
	//	int	Time;
	//	GuildHouseLootTypeENUM Type;
	//	int	PlayerDBID;
	//	int	ItemOrgID;
	//	int	ItemCount;
	//};

	std::string strPlayerName = "";

  	std::string strLootType = "";

	std::string strItemLink = "";

	GuildMemberStruct* pMember = g_pGuildFrame->GetGuildMemberStruct( rLog.PlayerDBID );
	
	if( pMember )
	{
		strPlayerName = pMember->MemberName;
	}
	else
	{
		FindDBIDRoleNameStruct* pFindStruct = NetCli_OtherChild::GetFindDBIDRoleNameStruct( rLog.PlayerDBID );

		if( pFindStruct && pFindStruct->IsFind )
		{
			strPlayerName = pFindStruct->RoleName;
		}
	}

	int fromPageID = 0;
	int toPageID = 0;


	if( rLog.Type == EM_GuildHouseLootType_Put )
	{
		strLootType = "put";
	}
	else if( rLog.Type == EM_GuildHouseLootType_Get )
	{
  		strLootType = "get";
	}
	else if( rLog.Type == EM_GuildHouseLootType_Move )
	{
  		strLootType = "move";
		fromPageID = rLog.FromPageID + 1;
		toPageID = rLog.ToPageID + 1;
	}
	else
	{
		return 0;
	}
 
	if( false == ItemGUID_To_ItemLink( rLog.ItemOrgID, strItemLink ) )
	{
		strItemLink = "";
	}

	int time = max( ( (int)g_pGameMain->GetGameTime() - rLog.Time ), 0);

	lua_pushstring( L, strPlayerName.c_str() );
 	lua_pushstring( L, strLootType.c_str() );
	lua_pushstring( L, strItemLink.c_str() );
   	lua_pushnumber( L, rLog.ItemCount );
  	lua_pushnumber( L, time );
	lua_pushnumber( L, fromPageID );
	lua_pushnumber( L, toPageID );

	return 7;
}



// ----------------------------------------------------------------------------
int LuaFunc_GetGuildBankItemLink( lua_State* L )
{
	if( false == g_pGuildHousesFrame->m_bGuildBankOpen )
	{
		return 0;
	}

	//bool ItemField_To_ItemLink( ItemFieldStruct* item, string& link );
	int PosID  = luaL_checkint( L, 1 ) - 1;

	if( PosID < 0 || PosID >= DF_GuildBankItemMax * DF_GuildBankPageMax )
	{
		return 0;
	}

	string link;
	ItemFieldStruct* item = g_pGuildHousesFrame->GetGuildBankItem( PosID );

	if ( NULL == item )
	{
		return 0;
	}

	if( false == ItemField_To_ItemLink( item, link, true ) )
	{
  		return 0;
	}

	lua_pushstring( L, link.c_str() );
	
	return 1;
}



// ----------------------------------------------------------------------------------
int LuaFunc_GuildHouses_GetFurnitureCount( lua_State* L )
{
	int FurnitureCount = DF_FurnitureCount;
	lua_pushnumber( L, FurnitureCount );					// push item count
	return 1;
}

// ----------------------------------------------------------------------------------
int LuaFunc_GuildHouses_GetFurnitureInfo( lua_State* L )
{
	//HouseItemDBStruct* pHouseItemDB = g_pHousesFrame->GetHousesItemDB( ContainerID , ItemID );

	int index = luaL_checkint( L, 1 ) - 1;
	GuildHouseFurnitureItemStruct* pFurnitureItem = g_pGuildHousesFrame->GetGuildHouseFurnitureItem( index );

	if( NULL == pFurnitureItem )
	{
		return 0;
	}
	
	GameObjDbStructEx* itemDB = CNetGlobal::GetObj( pFurnitureItem->Item.OrgObjID );
	if ( NULL == itemDB)
	{
 		return 0;
	}

	lua_pushstring( L, itemDB->GetName() );						// push Item name
	lua_pushstring( L, ItemUTIL::GetItemIcon( &pFurnitureItem->Item ) );

	bool bIsShow = true;

	if( false == pFurnitureItem->Layout.IsShow ||
		pFurnitureItem->Pos >= DF_FurnitureCount )
	{
		bIsShow = false;
	}

	lua_pushboolean( L, bIsShow );		// push Mode


	FindDBIDRoleNameStruct* pFindStruct = NetCli_OtherChild::GetFindDBIDRoleNameStruct( pFurnitureItem->OwnerDBID );

	string strOwnerName;
	if( pFindStruct && pFindStruct->IsFind )
	{
		strOwnerName = pFindStruct->RoleName;
	}

	lua_pushstring( L, strOwnerName.c_str() );

	return 4;

}



// ----------------------------------------------------------------------------
int	LuaFunc_GuildHouses_PickupFurniture(lua_State* L)
{
	//Houses_PickupItem( this.parentID  , this.index );

	int Index = luaL_checkint( L, 1 ) - 1 ;

	g_pGuildHousesFrame->PickupFurniture( Index );
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GuildHouses_SetPlaceFurnitureMode( lua_State* L )
{
	bool bMode = lua_toboolean( L, 1 ) ==1 ? true:false;
	g_pGuildHousesFrame->SetPlaceFurnitureMode( bMode );
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GuildHouses_SetFocusFurnitureID( lua_State* L )
{
	int ID = luaL_checkint( L, 1 ) - 1;
	g_pGuildHousesFrame->SetFocusFurnitureID( ID );

	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GuildHouses_GetFocusFurnitureID( lua_State* L )
{
	lua_pushnumber( L, g_pGuildHousesFrame->m_FocusFurnitureID + 1);

	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_GuildHouses_FurniturePlace( lua_State* L )
{
	g_pGuildHousesFrame->FurniturePlace();
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GuildHouses_FurnitureRemove( lua_State* L )
{
	g_pGuildHousesFrame->FurnitureRemove();
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GuildHouses_FurnitureMove( lua_State* L )
{
	g_pGuildHousesFrame->FurnitureMove();
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GuildHouses_FurnitureRorare( lua_State* L )
{
	g_pGuildHousesFrame->FurnitureRorare();
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GuildHouses_FurnitureSendBack( lua_State* L )
{
	int Index = luaL_checkint( L, 1 ) - 1 ;

	//g_pGuildHousesFrame->PickupFurniture( Index );
	NetCli_GuildHouses::SL_FurnitureSendBack( Index );
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GuildHouses_CanManageBuilding( lua_State* L )
{
	GuildRankBaseInfoStruct* pInfo = g_pGuildFrame->GetUserRankInfo();
	if( NULL == pInfo )
	{
		return 0;
	}
	lua_pushboolean( L, pInfo->Setting.Building );
	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_GuildHouses_CanManageFurniture( lua_State* L )
{
	GuildRankBaseInfoStruct* pInfo = g_pGuildFrame->GetUserRankInfo();
	if( NULL == pInfo )
	{
		return 0;
	}
	lua_pushboolean( L, pInfo->Setting.Furniture );
	return 1;
}
int LuaFunc_GuildHouses_CanSetStyle    ( lua_State* L )
{
	bool right=g_pGuildFrame->IsLeader();

	GuildRankBaseInfoStruct* pInfo = g_pGuildFrame->GetUserRankInfo();
	if( !right&& pInfo )
	{
		right= pInfo->Setting.SetGuildHouseStyle;

	}

	
	lua_pushboolean( L,right );
	return 1;
}

// ----------------------------------------------------------------------------
static int sModel_SetGuildHouseModel(CUiModel* pModel,int BaseOrgObjID,int OrgObjID2,const char *point_str)
{
	//int		OrgObjID;				//npc 參考物件ID
	GameObjDbStructEx* pBaseObjDB = g_ObjectData->GetObj( BaseOrgObjID );
	if ( pBaseObjDB == NULL )
		return 0;

	GameObjDbStructEx* pBaseImageDB = CNetGlobal::GetObj(pBaseObjDB->ImageID);
	if ( pBaseImageDB == NULL || pBaseImageDB->Image.ACTWorld[0] == 0 )
		return 0;

	pModel->CreateEntity( pBaseImageDB->Image.ACTWorld );

	CRuEntity* pEntity = pModel->GetRuEntity();

	if( NULL == pEntity )
	{
		return 0;
	}

	//	CRuSphere Sphere = pEntity->GetLocalBounds();

	// Create link frames
	CRuLinkFrame *linkFrame;

	CRuLinkFrame * linkTemp = ruNEW CRuLinkFrame();
	linkTemp->SetLinkName("linkTemp");

	linkFrame = ruNEW CRuLinkFrame();
	linkFrame->SetLinkName("p_down");

	linkFrame->SetTranslation( CRuVector3( 0, 0, 0 ) );
	linkTemp->AddChild(linkFrame);
	ruSAFE_RELEASE(linkFrame);

	linkFrame = ruNEW CRuLinkFrame();
	linkFrame->SetLinkName("p_top");
	linkFrame->SetTranslation( CRuVector3( 0, 600, 0 ) );

	linkTemp->AddChild(linkFrame);
	ruSAFE_RELEASE(linkFrame);

	pEntity->AddChild(linkTemp);
	ruSAFE_RELEASE(linkTemp);


	


	GameObjDbStructEx *pObjDB = g_ObjectData->GetObj(OrgObjID2 );
	if ( pObjDB == NULL )
		return 0;

	GameObjDbStructEx *imageDB = CNetGlobal::GetObj(pObjDB->ImageID);
	if ( imageDB == NULL || imageDB->Image.ACTWorld[0] == 0 )
	{
		return 0;
	}

	CRuEntity*	pEntityBaseCastle = g_ruResourceManager->LoadEntity( imageDB->Image.ACTWorld );

	if( NULL == pEntityBaseCastle )
	{
		return 0;
	}

	RuEntity_Attach_BFS( pEntity, pEntityBaseCastle, point_str );
	RuACT_PlayMotionEx( pEntityBaseCastle, -1, "stand_idle01" );

	ruSAFE_RELEASE( pEntityBaseCastle );
	return 0;
}
// NpcID圖像ID

static int sGuildHouse_GetStyleID(int index,int NpcID)
{
	int id=-1;
	if (index!=-1)
	{
		int count=(int)g_pGuildHousesFrame->m_Styles.size();
		if (index>=0&&index<count)
		{

			GuildHouseNpcReplaceCostTableStruct* StyleV=g_pGuildHousesFrame->m_Styles[index];
			if (StyleV->Type)
			{
				map< int , map< int , int > >::iterator it=g_ObjectData->_GuildHouseNpcReplace.find(StyleV->Type);
				if (it!=g_ObjectData->_GuildHouseNpcReplace.end())
				{
					map< int , int >::iterator sit=it->second.find(NpcID);
					if (sit!=it->second.end())
						id=sit->second;

				}

			}

		}

		

	}
	return id;
}
//--------------------------------------------------------

int LuaFunc_GuildHouses_ApplyStyle ( lua_State* L )
{
	int index = luaL_checkint(L, 1)-1;

	int count=(int)g_pGuildHousesFrame->m_Styles.size();
	if (index>=0&&index<count)
	{


		GuildHouseNpcReplaceCostTableStruct* StyleV=g_pGuildHousesFrame->m_Styles[index];


		NetCli_GuildHouses::SL_BuyHouseStyle(StyleV->Type);

	}

	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GuildHouses_SetStyleMode( lua_State* L )
{
	const char* szModelName = luaL_checkstring(L, 1);
	int index = luaL_checkint(L, 2);
	CUiModel* pModel = dynamic_cast<CUiModel*>( g_pGameMain->GetInterface()->GetUiWorld()->FindObject( szModelName ) );	
	if (pModel)
	{
		GuildBuildingObjectInfoStruct* pGuildBuildingObject1 = g_pGuildHousesFrame->GetGuildBuildingObjectInfoByIndex( 0 );
		GuildBuildingObjectInfoStruct* pGuildBuildingObject2 = g_pGuildHousesFrame->GetGuildBuildingObjectInfoByIndex( 1 );


		int type=1;
		if (index>=0&&index<g_pGuildHousesFrame->m_Styles.size())
		type=g_pGuildHousesFrame->m_Styles[index]->Type;
		if(pGuildBuildingObject2&&pGuildBuildingObject1)
		{

			int BaseOrgID=0;
			int BaseOrgID2=0;
			GuildBuildingInfoTableStruct* pGuildObject1=g_pGuildHousesFrame->GetReviewStyleModel(pGuildBuildingObject1->BuildingID, 0,type );
			if (pGuildObject1)

			BaseOrgID=pGuildObject1->OrgObjID;
			
			GuildBuildingInfoTableStruct* pGuildObject2=g_pGuildHousesFrame->GetReviewStyleModel( pGuildBuildingObject2->BuildingID,1,type );	
			if (pGuildObject2)
				BaseOrgID2=pGuildObject2->OrgObjID;

			if (BaseOrgID!=0&&BaseOrgID2!=0)
				sModel_SetGuildHouseModel(pModel,BaseOrgID,BaseOrgID2,pGuildBuildingObject2->PointStr);		
			
		}

		
			
	

	}
	return 0;

}
// ----------------------------------------------------------------------------
int LuaFunc_GuildHouses_SetMode( lua_State* L )
{
	const char* szModelName = luaL_checkstring(L, 1);
	CUiModel* pModel = dynamic_cast<CUiModel*>( g_pGameMain->GetInterface()->GetUiWorld()->FindObject( szModelName ) );	
	if( NULL == pModel )
	{
		return 0;
	}

	GuildBuildingObjectInfoStruct* pGuildBuildingObject = g_pGuildHousesFrame->GetGuildBuildingObjectInfoByIndex( 0 );
	if( NULL == pGuildBuildingObject )
	{
		return 0;
	}

	GuildBuildingInfoTableStruct* pBuildingResource = g_pGuildHousesFrame->GetGuildBuildingResourceByID( pGuildBuildingObject->BuildingID );
	if( NULL == pBuildingResource )
	{
		return 0;
	}
	int BaseOrgID=pBuildingResource->OrgObjID;

	
	pGuildBuildingObject = g_pGuildHousesFrame->GetGuildBuildingObjectInfoByIndex( 1 );
	if( NULL == pGuildBuildingObject )
	{
		return 0;
	}


	pBuildingResource = g_pGuildHousesFrame->GetGuildBuildingResourceByID( pGuildBuildingObject->BuildingID );
	if( NULL == pBuildingResource )
	{
		return 0;
	}

	int BaseOrgID2=pBuildingResource->OrgObjID;


	sModel_SetGuildHouseModel(pModel,BaseOrgID,BaseOrgID2,pGuildBuildingObject->PointStr);


	//CRuEntity* pEntity = model->GetRuEntity();

	//if( NULL == pEntity )
	//{
	//	return 0;
	//}

	//CRuArrayList<CRuLinkFrame *> linkFrames;
	//RuEntity_EnumerateLinkFrames( pEntity, linkFrames );
	//int i;
	//vector<string> PointStrList;
	//for( i = 0; i < linkFrames.Count(); ++i )
	//{
	//	PointStrList.push_back( linkFrames[i]->GetLinkName() );
	//}

	//std::set<std::string>::iterator itPos;

	//int pos;
	//string strType;

	//CRuEntity* pEntityPoint;
	//for( i = 0; i < PointStrList.size() ; ++i )
	//{

	//	strType = PointStrList[i].c_str();

	//	pos = strType.find( '_' );
	//	if( pos == string::npos )  //沒有關鍵字
	//	{
	//		continue;		
	//	}

	//	strType.erase( pos );

	//	itPos = g_pGuildHousesFrame->m_BuildPoints.find( strType );

	//	if( itPos == g_pGuildHousesFrame->m_BuildPoints.end() )
	//	{
	//		continue;
	//	}

	//	pEntityPoint = g_ruResourceManager->LoadEntity( "model\\item\\ui_object\\building_area\\act_building_area.ros" );

	//	if( NULL == pEntityPoint )
	//	{
	//		continue;
	//	}

	//	RuEntity_Attach_BFS( pEntity, pEntityPoint, PointStrList[i].c_str() );
	//	RuACT_PlayMotionEx( pEntityPoint, -1, "stand_idle01" );

	//	ruSAFE_RELEASE( pEntityPoint );
	//}


	return 0;
}

/*
// ----------------------------------------------------------------------------
int LuaFunc_GuildHouses_IsOpenVisit( lua_State* L )
{

	GuildBaseStruct* pInfo = g_pGuildFrame->GetGuildBaseInfo();

	bool bOpen = !pInfo->IsLockGuildHouse;

	lua_pushboolean( L, bOpen );
	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_GuildHouses_SetOpenVisit( lua_State* L )
{
	bool bOpen = (bool)lua_toboolean( L, 1 );
	bool IsLock = !bOpen;

	GuildBaseStruct* pInfo = g_pGuildFrame->GetGuildBaseInfo();
	if( pInfo->IsLockGuildHouse == IsLock )
	{
		return 0;	
	}

	pInfo->IsLockGuildHouse = IsLock;
	g_pGuildFrame->GuildInfoApply();

	return 0;
}
*/

// ----------------------------------------------------------------------------
int LuaFunc_GuildHousesWar_OpenMenu( lua_State* L )
{
	if( false == g_pGameMain->IsGuildBattleGroundEnable() )
	{
		
		lua_pushnumber( L, 0 );
		lua_setglobal( L, "arg1" );
		g_pGuildHousesFrame->SendWorldEvent( UiEvent_GuildHouseWarInfosUpdate );
		return 0;
	}

	if( g_pGuildHousesFrame->m_GuildWarRegistering )
	{
		return 0;
	}

	g_pGuildHousesFrame->GuildWarHistroy();
	g_pGuildHousesFrame->OpenMenuRequest();
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GuildHousesWar_Register( lua_State* L )
{
	GuildBaseStruct* pGuildInfo =  g_pGuildFrame->GetGuildBaseInfo();


	bool sw = lua_toboolean( L , 1 ) != 0;
	int index = luaL_checkint( L, 2 ) - 1 ;
	if( false == pGuildInfo->IsOwnHouse )
	{
		string msg = g_ObjectData->GetString( "GUILDWAR_REGISTER_FAIL2" );
		g_pGameMain->SendSystemMsg( msg.c_str() );
		g_pGameMain->SendSystemChatMsg( msg.c_str() );
		return 0;
	}

	if( false == g_pGameMain->IsGuildBattleGroundEnable() )
	{
		return 0;
	}

	if( g_pGuildHousesFrame->m_GuildWarRegistering )
	{
		return 0;
	}

	if( ( timeGetTime() - g_pGuildHousesFrame->m_OpenMenuResultTimer ) < 1000 )
	{
		return 0;
	}

	g_pGuildHousesFrame->RegisterRequest(sw,index);
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GuildHousesWar_CancelRegister( lua_State* L )
{
	if( g_pGuildHousesFrame->m_GuildWarRegistering )
	{
		return 0;
	}
 
	g_pGuildHousesFrame->CancelRegister();
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GuildHousesWar_EnterWar( lua_State* L )
{

	if( false == g_pGameMain->IsGuildBattleGroundEnable() )
	{
		return 0;
	}


	int zoneID = g_pGameMain->GetZoneID() % _DEF_ZONE_BASE_COUNT_ ;

	if( zoneID >= 100 )
	{
		g_pGameMain->SendMessageDialog( g_ObjectData->GetString( "BG_MSG_IN_INDEPENDENT_ZONE_NOT_ALLOW_ENTER" ) );
  		return 0;
	}

	if( g_pGuildHousesFrame->m_ZoneGroupID == -1 )
	{
		return 0;
	}

	NetCli_BG_GuildWar::SL_EnterRequest( g_pGuildHousesFrame->m_ZoneGroupID );
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GuildHousesWar_LeaveWar( lua_State* L )
{
	NetCli_BG_GuildWar::SBL_LeaveRequest();
	g_pGuildHouseWarStatus->Reset();
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GuildHousesWar_PricesRequest( lua_State* L )
{
	//X_ERR
	//NetCli_BG_GuildWar::SL_PricesRequest(); 
	return 0;
}


void func_GetGuildHouseWarStateString( GuildHouseWarStateENUM state ,string& strState )
{
	switch( state )
	{
	case EM_GuildHouseWarState_Appointment:
		strState = "Appointment";
		break;
	case EM_GuildHouseWarState_Prepare:
		strState = "Prepare";
		break;
	case EM_GuildHouseWarState_Failed:
		strState = "Failed";
		break;
	case EM_GuildHouseWarState_Fight:
		strState = "Fight";
		break;
  
	case EM_GuildHouseWarState_FightEnd_Even:
		strState = "FightEnd_Even";
		break;

	case EM_GuildHouseWarState_FightEnd_Win:
		strState = "FightEnd_Win";
		break;

	case EM_GuildHouseWarState_FightEnd_Lost:
		strState = "FightEnd_Lost";
		break;
	default:
		strState = "None";
		break;
	}
}
bool CGuildHousesFrame::isGuildWaring()
{
	bool ret=false;
	int count=(int)g_pGuildHousesFrame->m_vecGuildHouseWarInfo.size() ;

//string strState2;


	int GuildID=CNetGlobal::RoleData()->BaseData.GuildID | (g_pAccountLogin->GetWorldID()*0x1000000);

	for (int i=0;i<count;i++)
	{
		GuildHouseWarInfoStruct* pGuildInfo = &g_pGuildHousesFrame->m_vecGuildHouseWarInfo[i];

		if (GuildID ==	pGuildInfo->GuildID||GuildID==pGuildInfo ->EnemyGuildID)
		{

			ret=true;
			switch (pGuildInfo->State)
			{
				case EM_GuildHouseWarState_None:
				case EM_GuildHouseWarState_Failed:				
				case EM_GuildHouseWarState_Appointment:
				case EM_GuildHouseWarState_FightEnd_Lost:
				case EM_GuildHouseWarState_FightEnd_Win:
				case EM_GuildHouseWarState_FightEnd_Even:
					ret=false ;
				break;
			}
			

			break;
		}


	}
	return ret;
}
// ----------------------------------------------------------------------------
int LuaFunc_GuildHousesWar_GetInfo( lua_State* L )
{
	if( false == g_pGameMain->IsGuildBattleGroundEnable() )
	{
		lua_pushstring( L, "Disable" );
		return 1;
	}
	
	string strState;
	func_GetGuildHouseWarStateString( g_pGuildHousesFrame->m_GuildHouseCurrentState, strState );

  	lua_pushstring( L, strState.c_str() );

	int count=(int)g_pGuildHousesFrame->m_vecGuildHouseWarInfo.size() ;

	string strState2="Disable";
	int GuildID=CNetGlobal::RoleData()->BaseData.GuildID | (g_pAccountLogin->GetWorldID()*0x1000000);

	for (int i=0;i<count;i++)
	{
			GuildHouseWarInfoStruct* pGuildInfo = &g_pGuildHousesFrame->m_vecGuildHouseWarInfo[i];

			if (GuildID ==	pGuildInfo->GuildID||GuildID ==	pGuildInfo->EnemyGuildID)
			{
				func_GetGuildHouseWarStateString( pGuildInfo->State, strState2 );
				break;
			}
		

	}

	lua_pushstring( L, strState2.c_str() );

	return 2;
}

// ----------------------------------------------------------------------------
int LuaFunc_GuildHousesWar_GetRegisterCount( lua_State* L )
{
	int count = (int)g_pGuildHousesFrame->m_vecGuildHouseWarInfo.size();
	lua_pushnumber( L, count ); 

	lua_pushnumber( L,g_pGuildHousesFrame->m_GuildWarStatus );
	return 2;
}
// ----------------------------------------------------------------------------
int LuaFunc_GuildHousesWar_GetWarHistoryCount( lua_State* L )
{
	int count = (int)g_pGuildHousesFrame->m_warrepots.size();
	lua_pushnumber( L, count ); 
	return 1;
}
//----------------------------------------------------------------------
static void func_GetGameResultString(int state,string& strState)
{
	switch( state )
	{
	case EM_GameResult_Win:
		strState = "FightEnd_Win";
		break;
	case EM_GameResult_Lost:
		strState = "FightEnd_Lost";
		break;
	case EM_GameResult_Even:
		strState = "FightEnd_Even";
		break;
		default:
		strState = "None";
		break;
	}
}
// ----------------------------------------------------------------------------
int LuaFunc_GuildHousesWar_GetWarHistoryInfo( lua_State* L )
{

	int index = luaL_checkint( L, 1 ) - 1 ;
	int count=(int)g_pGuildHousesFrame->m_warrepots.size();
	if( index < 0 || index >= count )
	{
		return 0;
	}

	GuildHouseWarHistoryStruct* pWarHistory = &g_pGuildHousesFrame->m_warrepots[(count-1)-index];
	const char *szEnemyServer =NULL;
	const char *szEnemyName=NULL;
	int szEnemyScore=0;
	const char *szTargetName =NULL;
	const char *szTargetServer=NULL;
	int szTargetScore=0;
	char szDate[512];
	//int GuildID=-1;
	if (pWarHistory)
	{
		szTargetServer = g_pAccountLogin->FindWorldNameByID( pWarHistory->GuildID / 0x1000000 );

		GuildBaseStruct* pGuildInfo =  g_pGuildFrame->GetGuildBaseInfo();

		if (pGuildInfo)
		{
			szTargetName=pGuildInfo->GuildName;
		}

		szTargetScore=pWarHistory->GuildScore;

		szEnemyServer = g_pAccountLogin->FindWorldNameByID( pWarHistory->TargetGuildID / 0x1000000 );
		szEnemyName=pWarHistory->TargetGuildName;
		szEnemyScore=pWarHistory->TargetGuildScore;


		time_t	nTime = TimeExchangeFloatToInt( pWarHistory->EndFightTime );
		struct tm* tmnow = localtime(&nTime);

		sprintf_s( szDate ,512, "%02d/%02d/%02d %02d:%02d" ,  1 + tmnow->tm_mon , tmnow->tm_mday,  tmnow->tm_year -100, tmnow->tm_hour , tmnow->tm_min);


		//	GuildID= pWarHistory->GuildID;
	}
	string strState;

	func_GetGameResultString( pWarHistory->Result, strState );



	lua_pushstring( L, szTargetName );
	lua_pushstring( L, szTargetServer );
	lua_pushnumber( L, szTargetScore );

	lua_pushstring( L, strState.c_str() );
	lua_pushstring( L, szEnemyName );
	lua_pushstring( L, szEnemyServer );
	lua_pushnumber( L, szEnemyScore );

	lua_pushstring( L, szDate );


	return 8;

	
}
//----------------------------------------------------------------------------
int LuaFunc_GuildHousesWar_MyRegisterInfo( lua_State* L )
{

	int count=(int)g_pGuildHousesFrame->m_vecGuildHouseWarInfo.size();

	
	GuildBaseStruct* info = g_pGuildFrame->GetGuildBaseInfo();
	if (info)
	{
		int GuildID=info->GuildID | (g_pAccountLogin->GetWorldID()*0x1000000);
	
		for (int i=0;i<count;i++)
		{
			GuildHouseWarInfoStruct& pGuildInfo = g_pGuildHousesFrame->m_vecGuildHouseWarInfo[i];
			if (pGuildInfo.GuildID==GuildID ||pGuildInfo.EnemyGuildID==GuildID)
			{
				lua_pushnumber( L, i +1);
				return 1;
			}


		}

	}
	
	lua_pushnumber( L, -1 );
	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_GuildHousesWar_GetRegisterInfo( lua_State* L )
{
	int index = luaL_checkint( L, 1 ) - 1 ;
	int count=(int)g_pGuildHousesFrame->m_vecGuildHouseWarInfo.size();
	if( index < 0 || index >= count )
	{
		return 0;
	}

	GuildHouseWarInfoStruct* pGuildInfo = &g_pGuildHousesFrame->m_vecGuildHouseWarInfo[index];
	GuildHouseWarInfoStruct* pEnemyGuildInfo = g_pGuildHousesFrame->GetGuildHouseWarInfoStruct( pGuildInfo->EnemyGuildID );


	//int		GuildID;			//預約戰的公會
	//float	FightTime;			//戰爭時間
	//int		Score;				//積分
	//int		EnemyGuildID;		//對戰公會(-1表示沒有)
	//GuildHouseWarStateENUM State;

	//EM_GuildHouseWarState_Appointment	,	//預約
	//EM_GuildHouseWarState_Prepare		,	//準備
	//EM_GuildHouseWarState_Failed		,	//沒有配到隊
	//EM_GuildHouseWarState_Fight		,	//戰爭中
	//EM_GuildHouseWarState_FightEnd	,	//戰爭結束

	//const char* szGuildName = g_pGuildFrame->GetGuildNameByID( rinfo.GuildID );
	const char* szGuildName = pGuildInfo->GuildName;
	const char* szGuildServer = g_pAccountLogin->FindWorldNameByID( pGuildInfo->GuildID / 0x1000000 );
 	const char* szEnemyGuildName = "";
   	const char* szEnemyServer = "";

	if( NULL == szGuildName )
	{
		return 0;
	}

	if( 0 == pGuildInfo->GuildID )
	{
		return 0;
	}

	if( pEnemyGuildInfo )
	{
		szEnemyGuildName = pEnemyGuildInfo->GuildName;
		szEnemyServer = g_pAccountLogin->FindWorldNameByID( pEnemyGuildInfo->GuildID / 0x1000000 );
	}

	if( NULL == szGuildServer )
	{
		szGuildServer = "";
	}

	if( NULL == szEnemyServer )
	{
		szEnemyServer = "";
	}

	int	nTime = 0;
	//int	nTime = TimeExchangeFloatToInt( pGuildInfo->FightTime );
	//nTime = nTime - g_pGameMain->GetGameTime();

	string strState;



	func_GetGuildHouseWarStateString( pGuildInfo->State, strState );

	int GuildScore = pGuildInfo->Score;
	int EnemyGuildScore = -1;

	if( pEnemyGuildInfo )
	{
		EnemyGuildScore = pEnemyGuildInfo->Score;
  	}

	lua_pushstring( L, szGuildName );
	lua_pushstring( L, szGuildServer );
   	lua_pushnumber( L, GuildScore );

	lua_pushstring( L, szEnemyGuildName );
 	lua_pushstring( L, szEnemyServer );
   	lua_pushnumber( L, EnemyGuildScore );

   	lua_pushnumber( L, nTime );
	lua_pushstring( L, strState.c_str() );

	lua_pushboolean( L, pGuildInfo->IsAcceptAssignment );

	//lua_pushnumber( L,g_pGuildHousesFrame->m_GuildWarStatus );


	return 9;
}


// ----------------------------------------------------------------------------
int LuaFunc_GuildHousesWar_IsInBattleGround( lua_State* L )
{

	int ZoneID = g_pGameMain->GetZoneID() % _DEF_ZONE_BASE_COUNT_ ;

	if( ZoneID == 402 )
	{
	   	lua_pushboolean( L, true );
	}
	else
	{
		lua_pushboolean( L, false );
	}

	return 1;
}


//----------------------------------------

int  LuaFunc_GuildHouses_IsSetStyle  ( lua_State* L )
{

	bool ret=false;
	int level=	g_pGuildFrame->GetGuildLevel();
	if (level>=10)
	{
		GuildBuildingObjectInfoStruct* pGuildBuildingObject = g_pGuildHousesFrame->GetGuildBuildingObjectInfoByIndex( 0 );
		GuildBuildingObjectInfoStruct* pGuildBuildingObjecWall = g_pGuildHousesFrame->GetGuildBuildingObjectInfoByIndex( 1 );
		if (pGuildBuildingObject&&pGuildBuildingObjecWall)
		{
			GuildBuildingInfoTableStruct* pBuildingResource = g_pGuildHousesFrame->GetGuildBuildingResourceByID( pGuildBuildingObject->BuildingID );
			GuildBuildingInfoTableStruct* pBuildingResource1 = g_pGuildHousesFrame->GetGuildBuildingResourceByID( pGuildBuildingObjecWall->BuildingID );
			if (pBuildingResource->Lv>=3&&pBuildingResource1->Lv>=3)
			{
				ret=true ;
			}
		}		
	}

	lua_pushboolean( L, ret );
	return 1;
}


int LuaFunc_GuildHousesWar_OpenLadder  ( lua_State* L )
{
	g_pGuildHousesFrame->GuildWarLadder();
	return 0;
}
int LuaFunc_GuildHousesWar_GetLadderCount  ( lua_State* L )
{
	int count=g_pGuildHousesFrame->GetLadderCount();
	lua_pushnumber( L, count);
	return 1;
}
static void __GetLadderinfo(lua_State* L,GuildWarRankInfoStruct&info)
{

	//GuildWarRankInfoStruct& info=itPos->second;

	const char *szTargetName =info.GuildName;
	const char *szTargetServer=g_pAccountLogin->FindWorldNameByID( info.WorldGuildID / 0x1000000 );
	int szTargetScore=info.Score;
	int Win=info.WinCount;
	int Lost=info.LostCount;
	int Even=info.EvenCount;
	int GuildID=info.WorldGuildID;


	lua_pushstring( L, szTargetName );
	lua_pushstring( L, szTargetServer );
	lua_pushnumber( L, szTargetScore );
	lua_pushnumber( L, Win );
	lua_pushnumber( L, Lost );
	lua_pushnumber( L, Even );

	lua_pushnumber( L, GuildID );
	//return 7;
}
int LuaFunc_GuildHousesWar_GetMyLadderInfo( lua_State* L )
{
	GuildBaseStruct* info = g_pGuildFrame->GetGuildBaseInfo();
	if (info)
	{
		int GuildID=info->GuildID | (g_pAccountLogin->GetWorldID()*0x1000000);
		std::map<int,GuildWarRankInfoStruct>& mapLadder=g_pGuildHousesFrame-> m_LadderInfo;

		std::map<int,GuildWarRankInfoStruct>::iterator itPos =mapLadder.find(GuildID);
		if (itPos!=mapLadder.end())
		{
			__GetLadderinfo(L,itPos->second);

			int count=(int)g_pGuildHousesFrame->m_ladderList.size();
			for (int index=0;index<count;index++)
			{
				if (g_pGuildHousesFrame->m_ladderList[index]==GuildID)
				{
					lua_pushnumber(L,index);

					return 8;
				}

			}
			return 7;
		}
	}
	return 0;
}
int LuaFunc_GuildHousesWar_GetLadderHistoryInfo( lua_State* L )
{

	int index = luaL_checkint( L, 1 ) - 1 ;
	int count=g_pGuildHousesFrame->m_LadderHistoryCount ;
	if( index < 0 || index >= count)
	{
		return 0;
	}


	GuildHouseWarHistoryStruct* pWarHistory = &g_pGuildHousesFrame->m_LadderHistory[(count-1)-index];
	const char *szEnemyServer =NULL;
	const char *szEnemyName=NULL;
	int szEnemyScore=0;
	const char *szTargetName =NULL;
	const char *szTargetServer=NULL;
	int szTargetScore=0;
	char szDate[512];
	//int GuildID=-1;
	if (pWarHistory)
	{
		szTargetServer = g_pAccountLogin->FindWorldNameByID( pWarHistory->GuildID / 0x1000000 );

		std::map<int,GuildWarRankInfoStruct>::iterator itPos =g_pGuildHousesFrame-> m_LadderInfo.find( pWarHistory->GuildID );

		if (itPos!=g_pGuildHousesFrame->m_LadderInfo.end())
		{
			szTargetName=itPos->second.GuildName;
		}
		
		szTargetScore=pWarHistory->GuildScore;

		szEnemyServer = g_pAccountLogin->FindWorldNameByID( pWarHistory->TargetGuildID / 0x1000000 );
		szEnemyName=pWarHistory->TargetGuildName;
		szEnemyScore=pWarHistory->TargetGuildScore;


		time_t	nTime = TimeExchangeFloatToInt( pWarHistory->EndFightTime );
		struct tm* tmnow = localtime(&nTime);

		sprintf_s( szDate ,512, "%02d/%02d/%02d %02d:%02d" ,  1 + tmnow->tm_mon , tmnow->tm_mday,  tmnow->tm_year -100, tmnow->tm_hour , tmnow->tm_min);


	//	GuildID= pWarHistory->GuildID;
	}
	string strState;

	func_GetGameResultString( pWarHistory->Result, strState );



	lua_pushstring( L, szTargetName );
	lua_pushstring( L, szTargetServer );
	lua_pushnumber( L, szTargetScore );
	
	lua_pushstring( L, strState.c_str() );
	lua_pushstring( L, szEnemyName );
	lua_pushstring( L, szEnemyServer );
	lua_pushnumber( L, szEnemyScore );

	lua_pushstring( L, szDate );


	return 8;

	
}
int LuaFunc_GuildHousesWar_GetLadderInfo( lua_State* L )
{

	std::map<int,GuildWarRankInfoStruct>& mapLadder=g_pGuildHousesFrame-> m_LadderInfo;


	int index = luaL_checkint( L, 1 ) - 1 ;

	if (index>=0&&index<g_pGuildHousesFrame->m_ladderList.size())
	{
		int GuildID=g_pGuildHousesFrame->m_ladderList[index];
		std::map<int,GuildWarRankInfoStruct>::iterator itPos =mapLadder.find(GuildID);
		if (itPos!=mapLadder.end())
		{
			

			 __GetLadderinfo(L,itPos->second);
			 return 7;

		}
	}

	




	return 0;
}
int LuaFunc_GuildHousesWar_GetLadderHistoryCount  ( lua_State* L )
{
	int count=g_pGuildHousesFrame->GetLadderHistoryCount();
	lua_pushnumber( L, count);
	return 1;
}
int LuaFunc_GuildHousesWar_OpenLadderHistory  ( lua_State* L )
{

	int GuildID = -1;
	int top=lua_gettop(L);
	if (top>=1)
	{
		GuildID=luaL_checkint( L, 1 )  ;
	}
	if (GuildID==-1)
	{

		GuildBaseStruct* info = g_pGuildFrame->GetGuildBaseInfo();
		if (info)
			GuildID=info->GuildID | (g_pAccountLogin->GetWorldID()*0x1000000);
	}
	if (GuildID!=-1)
	g_pGuildHousesFrame->GuildWarLadderHistory(GuildID);
	return 0;
}
//----------------------------------------

int LuaFunc_GuildHousesWar_GetGuildWarBattleTime   ( lua_State* L )
{

	GuildBaseStruct* pGuildInfo =  g_pGuildFrame->GetGuildBaseInfo();
	int time=-1;
	if (pGuildInfo)
		time=pGuildInfo->HousesWar.NextWarTime;

	lua_pushnumber(L,time);



	return 1;

}

































