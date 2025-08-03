
#include "../../mainproc/GameMain.h"
#include "../../netwaker_member/NetWakerClientInc.h"
#include "../WorldFrames/ItemClipboard.h"
#include "../WorldFrames/ChatEdit.h"
#include "../WorldFrames/ChatFrame.h"
#include "HousesFrame.h"
#include "../debug/DebugMessage.h"
#include "../../tools/Common/Com_Function.h"

#include "../WorldFrames/GameControl.h"

#include "../WorldFrames/CharacterFrame.h"

#include "../WorldFrames/LoadingFrame.h"

#include "../WorldFrames/ItemUTIL.h"

#include "../WorldFrames/BagFrame.h"

#include "../ItemLink/ItemLink.h"

#include "../WorldFrames/UnitFrame.h"

#include "../WorldFrames/SpeakFrame.h"



#define UiEvent_HousesOpenVisitHouse   "HOUSES_OPEN_VISIT_HOUSE"
#define UiEvent_HousesCloseVisitHouse  "HOUSES_CLOSE_VISIT_HOUSE"
#define UiEvent_HousesVisitHousePasswordError  "HOUSES_VISIT_HOUSE_PASSWORDERROR"
#define UiEvent_HousesVisitHouseNotFind        "HOUSES_VISIT_HOUSE_NOTFIND"


#define UiEvent_HousesFrameShow  "HOUSESFRAME_SHOW"
#define UiEvent_HousesFrameHide  "HOUSESFRAME_HIDE"
#define UiEvent_HousesFurnitureChanged  "HOUSES_FURNITURE_CHANGED"

#define UiEvent_HousesStorageShow       "HOUSES_STORAGE_SHOW"
#define UiEvent_HousesStorageChanged    "HOUSES_STORAGE_CHANGED"

#define UiEvent_HousesHangerShow        "HOUSES_HANGER_SHOW"
#define UiEvent_HousesHangerChanged     "HOUSES_HANGER_CHANGED"

#define DF_HousesHangerMode_Coathanger     (1)
#define DF_HousesHangerMode_WeaponSet      (2)
#define DF_HousesHangerMode_Both           (3)

//#define UiEvent_HousesStorageCloseAll    "HOUSES_STORAGE_CLOSE_ALL"
//#define UiEvent_HousesStorageChanged  "HOUSES_STORAGE_CHANGED"
#define UiEvent_HousesChangedName  "HOUSES_CHANGED_NAME"
#define UiEvent_HousesChangedPassword  "HOUSES_CHANGED_PASSWORD"

#define UiEvent_HousesMaidSpeak "HOUSES_MAID_SPEAK"

#define UiEvent_HousesFurnitureTooNearDoor "HOUSES_FURNITURE_TOO_NEAR_DOOR"	


#define UiEvent_HousesPlantOpen    "HOUSES_PLANT_OPEN"
#define UiEvent_HousesPlantClose   "HOUSES_PLANT_CLOSE"
#define UiEvent_HousesPlantChanged "HOUSES_PLANT_CHANGED"


#define UiEvent_HousesFriendChanged "HOUSES_FRIEND_CHANGED"



#define UiEvent_HousesFriendItemLogShow "HOUSES_FRIEND_ITEM_LOG_SHOW"
#define UiEvent_HousesFriendItemLogChanged "HOUSES_FRIEND_ITEM_LOG_CHANGED"



#define UiEvent_Houses_ServantHireListShow "HOUSES_SERVANT_HIRE_LIST_SHOW"
//#define UiEvent_Houses_ServantHireListChanged "HOUSES_SERVANT_HIRE_LIST_CHANGED"

#define UiEvent_Houses_ServantListChanged "HOUSES_SERVANT_LIST_CHANGED"

#define UiEvent_Houses_ServantInfoShow "HOUSES_SERVANT_INFO_SHOW"
#define UiEvent_Houses_ServantInfoUpdate "HOUSES_SERVANT_INFO_UPDATE"

#define UiEvent_Houses_ServantItemChanged "HOUSES_SERVANT_ITEM_CHANGED"

#define UiEvent_Houses_ServantHireFailed "UIEVENT_HOUSES_SERVANT_HIRE_FAILED"



#define DF_HouseFurnishingListID (-1)
#define DF_HouseServantListID (-2)

#define DF_HouseRoleSpriteWorldGUID           (-10000000)


#define DF_HouseRoleSpriteWorldGUID_House     (-20000000)	
#define DF_HouseRoleSpriteWorldGUID_Maid      (-20000001)	
//#define DF_HouseRoleSpriteWorldGUID_Servant   (-30000000)

#define DF_ServantArmorPos    0
#define DF_ServantArmorPosEnd 29
#define DF_ServantItemPos     30
#define DF_ServantItemPosEnd  39




#define DF_ServantEvent_Enter       1
//#define DF_ServantEvent_FrientEnter 2

#define DF_ServantEvent_Enter_Owner   10
#define DF_ServantEvent_Enter_Friend  11
#define DF_ServantEvent_Enter_Other   12


CHousesFrame* g_pHousesFrame = NULL;

// ----------------------------------------------------------------------------
void UiCallback_HousesControl_OnMouseDown  ( CUiFrame* uiFrame, INT64 lParam, INT64 rParam ){ g_pHousesFrame->HousesControl_OnMouseDown  ( uiFrame, lParam, rParam ); }
void UiCallback_HousesControl_OnMouseUp    ( CUiFrame* uiFrame, INT64 lParam, INT64 rParam ){ g_pHousesFrame->HousesControl_OnMouseUp    ( uiFrame, lParam, rParam ); }
void UiCallback_HousesControl_OnMouseWheel ( CUiFrame* uiFrame, INT64 lParam, INT64 rParam ){ g_pHousesFrame->HousesControl_OnMouseWheel ( uiFrame, lParam, rParam ); }
void UiCallback_HousesControl_OnMouseMove  ( CUiFrame* uiFrame, INT64 lParam, INT64 rParam ){ g_pHousesFrame->HousesControl_OnMouseMove  ( uiFrame, lParam, rParam ); }
void UiCallback_HousesControl_OnMouseEnter ( CUiFrame* uiFrame, INT64 lParam, INT64 rParam ){ g_pHousesFrame->HousesControl_OnMouseEnter ( uiFrame, lParam, rParam ); }
void UiCallback_HousesControl_OnMouseLeave ( CUiFrame* uiFrame, INT64 lParam, INT64 rParam ){ g_pHousesFrame->HousesControl_OnMouseLeave ( uiFrame, lParam, rParam ); }
void UiCallback_HousesControl_OnClick      ( CUiFrame* uiFrame, INT64 lParam, INT64 rParam ){ g_pHousesFrame->HousesControl_OnClick      ( uiFrame, lParam, rParam ); }
void UiCallback_HousesControl_OnDoubleClick( CUiFrame* uiFrame, INT64 lParam, INT64 rParam ){ g_pHousesFrame->HousesControl_OnDoubleClick( uiFrame, lParam, rParam ); }
void UiCallback_HousesControl_OnDragBegin  ( CUiFrame* uiFrame, INT64 lParam, INT64 rParam ){ g_pHousesFrame->HousesControl_OnDragBegin  ( uiFrame, lParam, rParam ); }
void UiCallback_HousesControl_OnDragStop   ( CUiFrame* uiFrame, INT64 lParam, INT64 rParam ){ g_pHousesFrame->HousesControl_OnDragStop   ( uiFrame, lParam, rParam ); }
void UiCallback_HousesControl_OnReceiveDrag( CUiFrame* uiFrame, INT64 lParam, INT64 rParam ){ g_pHousesFrame->HousesControl_OnReceiveDrag( uiFrame, lParam, rParam ); }



// ----------------------------------------------------------------------------

int LuaFunc_Houses_VisitHouseRequest      ( lua_State* L );
int LuaFunc_Houses_CloseVisitHouse        ( lua_State* L );

int LuaFunc_Houses_GetHouseInfo           ( lua_State* L );


int LuaFunc_Houses_IsOwner                ( lua_State* L );
int LuaFunc_Houses_IsFriend               ( lua_State* L );

int LuaFunc_Houses_GetTypeCount           ( lua_State* L );
int LuaFunc_Houses_GetTypeInfo            ( lua_State* L );
int LuaFunc_Houses_SetCurrentType         ( lua_State* L );

int LuaFunc_Houses_SetPlaceFurnishingMode ( lua_State* L );

int LuaFunc_Houses_GetFurnitureListID     ( lua_State* L );
int LuaFunc_Houses_GetFurnitureItemInfo   ( lua_State* L );

int LuaFunc_Houses_GetItemInfo            ( lua_State* L );

int	LuaFunc_Houses_PickupItem             ( lua_State* L );
int	LuaFunc_Houses_DrawItem               ( lua_State* L );

int LuaFunc_Houses_GetItemLink            ( lua_State* L );

int LuaFunc_Houses_SetFocusFurnishingID   ( lua_State* L );
int LuaFunc_Houses_GetFocusFurnishingID   ( lua_State* L );

int LuaFunc_Houses_FurnishingPlace        ( lua_State* L );
int LuaFunc_Houses_FurnishingRemove       ( lua_State* L );
int LuaFunc_Houses_FurnishingMove         ( lua_State* L );
int LuaFunc_Houses_FurnishingRorare       ( lua_State* L );

int LunFunc_Houses_GetEnergyCostInfo      ( lua_State* L );
int LunFunc_Houses_BuyEnergyRequest       ( lua_State* L );


int LuaFunc_Houses_GetSpaceInfo           ( lua_State* L );
int LuaFunc_House_BuySpaceRequest         ( lua_State* L );
int LuaFunc_House_TestBuySpace            ( lua_State* L );

int LuaFunc_House_CloseStorage            ( lua_State* L );

int LuaFunc_Houses_ChangedName            ( lua_State* L );
int LuaFunc_Houses_ChangedPassword        ( lua_State* L );


int LuaFunc_Houses_HangerSwap             ( lua_State* L );

// 檢查物品限制是否可以裝備(有效)
int LuaFunc_Houses_CanWearObject          ( lua_State* L );


//種植
int LuaFunc_Plant_GetInfo                 ( lua_State* L );

int	LuaFunc_Plant_PickupItem              ( lua_State* L );
int	LuaFunc_Plant_ClearItem               ( lua_State* L );

int LuaFunc_Plant_GetItmeInfo             ( lua_State* L );

int	LuaFunc_Plant_Close                   ( lua_State* L ); 

int LuaFunc_Plant_Grow                    ( lua_State* L );
int LuaFunc_Plant_Clear                   ( lua_State* L );
int LuaFunc_Plant_Lock                    ( lua_State* L );
int LuaFunc_Plant_GetProduct              ( lua_State* L );

int LuaFunc_Plant_GetPlantCount           ( lua_State* L );
int LuaFunc_Plant_GetMaxPlantCount        ( lua_State* L );



//Friend
//m_setFriendDBID
int LuaFunc_Houses_GetFriendCount         ( lua_State* L );
int LuaFunc_Houses_GetFriendInfo          ( lua_State* L );

int LuaFunc_Houses_AddFriend              ( lua_State* L );
int LuaFunc_Houses_DelFriend              ( lua_State* L );

int LuaFunc_Houses_GetFriendPut           ( lua_State* L );
int LuaFunc_Houses_GetFriendGet           ( lua_State* L );
int LuaFunc_Houses_GetFriendView          ( lua_State* L );

int LuaFunc_Houses_SetFriendPut           ( lua_State* L );
int LuaFunc_Houses_SetFriendGet           ( lua_State* L );
int LuaFunc_Houses_SetFriendView          ( lua_State* L );

int LuaFunc_Houses_GetFriendRegard        ( lua_State* L );
int LuaFunc_Houses_SetFriendRegard        ( lua_State* L );

 
int LuaFunc_Houses_GetFriendItemLog       ( lua_State* L );

//test
//int LuaFunc_Houses_SetPlant               (  lua_State* L );


//女僕招募列表
int LuaFunc_Houses_ServantHireListRequest ( lua_State* L );
int LuaFunc_Houses_ServantHireRequest     ( lua_State* L );


int LuaFunc_Houses_GetServantHireInfoCount( lua_State* L );
int LuaFunc_Houses_GetServantHireInfo     ( lua_State* L );


//玩家的女僕
int LuaFunc_Houses_GetServantCount        ( lua_State* L );
int LuaFunc_Houses_GetServantInfo         ( lua_State* L );
int LuaFunc_Houses_SummonServant          ( lua_State* L );
int LuaFunc_Houses_HideServant            ( lua_State* L );
int LuaFunc_Houses_DismissServant         ( lua_State* L );




// ----------------------------------------------------------------------------
CHousesFrame::CHousesFrame(CInterface* object)
: CInterfaceBase(object)
{
	g_pHousesFrame = this;
	m_pHousesControlFrame = NULL;

	m_eHousesState = EHousesState_Leave;	

	m_VisitHouseNpcID = -1;

	m_FocusFurnishingID = -1;        //正在放置的傢俱ID
	m_MouseEnterFurnishingID = -1;

	m_FocusServantPos = -1;
	m_FocusServantDBID = -1;      //焦點的傢俱 DBID
	m_MouseEnterServantDBID = -1; //滑鼠指到的HouseItem DBID

	m_ePlaceingMode = EM_PlaceingMode_None;     //放置模式
	m_ePlaceingModeTemp = EM_PlaceingMode_None;
	m_PlaceStep = 0;             //放置模式步驟 1移動 2旋轉

	m_PlaceFurnishingMode = false;
	m_bFurnishingChange = false;


	m_HouseRoleSpriteID = DF_HouseRoleSpriteWorldGUID_House;
	m_MaidRoleSpriteID  = DF_HouseRoleSpriteWorldGUID_Maid;

	m_bIsOwner = false;

	m_bLoadedRoleSpriteHouse = false;


	m_BuyFunction_NewOpenEnergy = 0;
	m_BuyFunction_FunctionID = -1;


	m_PlantFurnishingDBID = -1;
	m_PlantFurnishingPos  = -1;


	m_HousesSoundHandle = 0;
	m_HousesSoundFurnishingDBID = -1;
}

// ----------------------------------------------------------------------------
CHousesFrame::~CHousesFrame()
{
	g_pHousesFrame = NULL;

}

// ----------------------------------------------------------------------------
void CHousesFrame::BuildFrames()
{
	CUi* pUi = m_interface->GetUiWorld();
	//CUiAnchor anchor;
	m_pHousesControlFrame = (CUiFrame*)pUi->CreateObject(CUiLayout::eUiType_Frame, "HousesControlFrame", pUi->GetRoot());
	m_pHousesControlFrame->SetFrameStrata(eUIFrame_Strata_Background);
	m_pHousesControlFrame->SetAnchorSize(CUiSize(0.0f, 0.0f));
	m_pHousesControlFrame->SetAnchor(CUiAnchor());
	m_pHousesControlFrame->SetVisible(false);

	m_pHousesControlFrame->RegisterForClicks( CUiFrame::eMouse_Flag_LButton | CUiFrame::eMouse_Flag_RButton );

	m_pHousesControlFrame->SetMouseEnable( true );

	//root->m_pfOnMouseDown	  = GameControl_MouseDown;
	//root->m_pfOnMouseUp     = GameControl_MouseUp;
	//root->m_pfOnMouseWheel  = GameControl_MouseWheel;
	//root->m_pfOnMouseMove   = GameControl_MouseMove;
	//root->m_pfOnMouseEnter  = GameControl_MouseEnter;
	//root->m_pfOnMouseLeave  = GameControl_MouseLeave;
	//root->m_pfOnClick		  = GameControl_Click;
	//root->m_pfOnDoubleClick = GameControl_DoubleClick;
	//root->m_pfOnDragBegin	  = GameControl_DragBegin;
	//root->m_pfOnDragStop	  = GameControl_DragStop;
	//root->m_pfOnReceiveDrag = GameControl_ReceiveDrag;


	m_pHousesControlFrame->m_pfOnMouseDown   = UiCallback_HousesControl_OnMouseDown   ;
	m_pHousesControlFrame->m_pfOnMouseUp     = UiCallback_HousesControl_OnMouseUp     ;
	m_pHousesControlFrame->m_pfOnMouseWheel  = UiCallback_HousesControl_OnMouseWheel  ;
	m_pHousesControlFrame->m_pfOnMouseMove   = UiCallback_HousesControl_OnMouseMove   ;
	m_pHousesControlFrame->m_pfOnMouseEnter  = UiCallback_HousesControl_OnMouseEnter  ;
	m_pHousesControlFrame->m_pfOnMouseLeave  = UiCallback_HousesControl_OnMouseLeave  ;
	m_pHousesControlFrame->m_pfOnClick       = UiCallback_HousesControl_OnClick       ;
	m_pHousesControlFrame->m_pfOnDoubleClick = UiCallback_HousesControl_OnDoubleClick ;
	m_pHousesControlFrame->m_pfOnDragBegin   = UiCallback_HousesControl_OnDragBegin   ;
	m_pHousesControlFrame->m_pfOnDragStop    = UiCallback_HousesControl_OnDragStop    ; 
	m_pHousesControlFrame->m_pfOnReceiveDrag = UiCallback_HousesControl_OnReceiveDrag ; 
}

void CHousesFrame::DestroyFrames()
{
	m_pHousesControlFrame = NULL;
}

void CHousesFrame::ReloadFrames()
{
	if( m_eHousesState == EHousesState_Enter )
	{
		m_pHousesControlFrame->SetVisible(true);
		SendWorldEvent( UiEvent_HousesFrameShow );
	}
}

// ----------------------------------------------------------------------------
void CHousesFrame::RegisterFunc()
{
	lua_State* L = GetLuaStateWorld();
	if (L)
	{
		lua_register( L, "Houses_VisitHouseRequest"      , LuaFunc_Houses_VisitHouseRequest      );
		lua_register( L, "Houses_CloseVisitHouse"        , LuaFunc_Houses_CloseVisitHouse        );
		lua_register( L, "Houses_GetHouseInfo"           , LuaFunc_Houses_GetHouseInfo           );

   		lua_register( L, "Houses_IsOwner"                , LuaFunc_Houses_IsOwner                );
		lua_register( L, "Houses_IsFriend"               , LuaFunc_Houses_IsFriend               );

		lua_register( L, "Houses_GetTypeCount"           , LuaFunc_Houses_GetTypeCount           );
		lua_register( L, "Houses_GetTypeInfo"            , LuaFunc_Houses_GetTypeInfo            );
		lua_register( L, "Houses_SetCurrentType"         , LuaFunc_Houses_SetCurrentType         );

		lua_register( L, "Houses_SetPlaceFurnishingMode" , LuaFunc_Houses_SetPlaceFurnishingMode );
		lua_register( L, "Houses_GetFurnitureListID"     , LuaFunc_Houses_GetFurnitureListID     );
		lua_register( L, "Houses_GetFurnitureItemInfo"   , LuaFunc_Houses_GetFurnitureItemInfo   );

		lua_register( L, "Houses_GetItemInfo"            , LuaFunc_Houses_GetItemInfo            );
		lua_register( L, "Houses_PickupItem"             , LuaFunc_Houses_PickupItem             );
		lua_register( L, "Houses_DrawItem"               , LuaFunc_Houses_DrawItem               );

		lua_register( L, "Houses_GetItemLink"            , LuaFunc_Houses_GetItemLink            );


		lua_register( L, "Houses_SetFocusFurnishingID"   , LuaFunc_Houses_SetFocusFurnishingID   );
		lua_register( L, "Houses_GetFocusFurnishingID"   , LuaFunc_Houses_GetFocusFurnishingID   );

		lua_register( L, "Houses_FurnishingPlace"        , LuaFunc_Houses_FurnishingPlace        );
		lua_register( L, "Houses_FurnishingRemove"       , LuaFunc_Houses_FurnishingRemove       );
		lua_register( L, "Houses_FurnishingMove"         , LuaFunc_Houses_FurnishingMove         );
		lua_register( L, "Houses_FurnishingRorare"       , LuaFunc_Houses_FurnishingRorare       );

		lua_register( L, "Houses_GetEnergyCostInfo"      , LunFunc_Houses_GetEnergyCostInfo      );
		lua_register( L, "Houses_BuyEnergyRequest"       , LunFunc_Houses_BuyEnergyRequest       );

		lua_register( L, "Houses_GetSpaceInfo"           , LuaFunc_Houses_GetSpaceInfo           );
		lua_register( L, "House_BuySpaceRequest"         , LuaFunc_House_BuySpaceRequest         );
		lua_register( L, "House_TestBuySpace"            , LuaFunc_House_TestBuySpace            );

		lua_register( L, "House_CloseStorage"            , LuaFunc_House_CloseStorage            );


 		lua_register( L, "Houses_ChangedName"            , LuaFunc_Houses_ChangedName            );
		lua_register( L, "Houses_ChangedPassword"        , LuaFunc_Houses_ChangedPassword        );

		lua_register( L, "Houses_HangerSwap"             , LuaFunc_Houses_HangerSwap             );

   		lua_register( L, "Houses_CanWearObject"          , LuaFunc_Houses_CanWearObject          );

		lua_register( L, "Plant_GetInfo"                 , LuaFunc_Plant_GetInfo                 );

		lua_register( L, "Plant_PickupItem"		    	 , LuaFunc_Plant_PickupItem				 );
  		lua_register( L, "Plant_ClearItem"		    	 , LuaFunc_Plant_ClearItem				 );

		lua_register( L, "Plant_GetItmeInfo"		     , LuaFunc_Plant_GetItmeInfo			 );
	
		lua_register( L, "Plant_Close"                   , LuaFunc_Plant_Close                   );

		lua_register( L, "Plant_Grow"                    , LuaFunc_Plant_Grow                    );
		lua_register( L, "Plant_Clear"                   , LuaFunc_Plant_Clear                   );
		lua_register( L, "Plant_Lock"                    , LuaFunc_Plant_Lock                    );
		lua_register( L, "Plant_GetProduct"              , LuaFunc_Plant_GetProduct              );

		lua_register( L, "Plant_GetPlantCount"           , LuaFunc_Plant_GetPlantCount           );
		lua_register( L, "Plant_GetMaxPlantCount"        , LuaFunc_Plant_GetMaxPlantCount        );
	
		//lua_register( L, "Houses_SetPlant"		    	, LuaFunc_Houses_SetPlant                );

		lua_register( L, "Houses_GetFriendCount"         , LuaFunc_Houses_GetFriendCount         );
		lua_register( L, "Houses_GetFriendInfo"          , LuaFunc_Houses_GetFriendInfo          );
		lua_register( L, "Houses_AddFriend"              , LuaFunc_Houses_AddFriend              );
		lua_register( L, "Houses_DelFriend"              , LuaFunc_Houses_DelFriend              );
		lua_register( L, "Houses_GetFriendPut"           , LuaFunc_Houses_GetFriendPut           );
		lua_register( L, "Houses_GetFriendGet"           , LuaFunc_Houses_GetFriendGet           );
		lua_register( L, "Houses_GetFriendView"          , LuaFunc_Houses_GetFriendView          );
		lua_register( L, "Houses_SetFriendPut"           , LuaFunc_Houses_SetFriendPut           );
		lua_register( L, "Houses_SetFriendGet"           , LuaFunc_Houses_SetFriendGet           );
		lua_register( L, "Houses_SetFriendView"          , LuaFunc_Houses_SetFriendView          );

		lua_register( L, "Houses_GetFriendRegard"        , LuaFunc_Houses_GetFriendRegard        );
		lua_register( L, "Houses_SetFriendRegard"        , LuaFunc_Houses_SetFriendRegard        );

		lua_register( L, "Houses_GetFriendItemLog"       , LuaFunc_Houses_GetFriendItemLog       );

	
		lua_register( L, "Houses_ServantHireListRequest" , LuaFunc_Houses_ServantHireListRequest );
		lua_register( L, "Houses_ServantHireRequest"     , LuaFunc_Houses_ServantHireRequest     );

		lua_register( L, "Houses_GetServantHireInfoCount", LuaFunc_Houses_GetServantHireInfoCount);
		lua_register( L, "Houses_GetServantHireInfo"     , LuaFunc_Houses_GetServantHireInfo     );


		lua_register( L, "Houses_GetServantCount"        , LuaFunc_Houses_GetServantCount        );
		lua_register( L, "Houses_GetServantInfo"         , LuaFunc_Houses_GetServantInfo         );
		lua_register( L, "Houses_SummonServant"          , LuaFunc_Houses_SummonServant          );
 		lua_register( L, "Houses_HideServant"            , LuaFunc_Houses_HideServant            );
		lua_register( L, "Houses_DismissServant"         , LuaFunc_Houses_DismissServant         );
	
	} 
}

void CHousesFrame::LeaveWorld()
{
	if( m_eHousesState == EHousesState_Enter ) //目前狀態 在房屋中 => 離開房屋
	{
		//儲存改變
		//HouseItemLayoutChange();

		m_eHousesState = EHousesState_Leave;
		//g_pError->OutputToFile( "HousesState_Leave" );

		//清除所有資源
		g_pGameMain->DeleteSprite( m_HouseRoleSpriteID );
   		g_pGameMain->DeleteSprite( m_MaidRoleSpriteID );

	//	m_HousesContainerMap.clear();

		m_HouseItems.clear();
		m_HouseContainers.clear();

		m_MapHousePlant.clear();
		m_FriendDBIDTable.clear();
		//m_HouseFurnishingState.clear();

		m_pHousesControlFrame->SetVisible(false);
		SendWorldEvent( UiEvent_HousesFrameHide );

		m_BuyFunction_NewOpenEnergy = 0;
		m_BuyFunction_FunctionID = -1;

		m_bLoadedRoleSpriteHouse = false;

		StopHousesSound();

		m_HousesSoundHandle = 0;
		m_HousesSoundFurnishingDBID = -1;

	}
	else
	{
 		//有時 EHousesState_Loading 時 斷線到Login 狀態會是EHousesState_Loading
 		//m_eHousesState = EHousesState_Leave;
		//m_pHousesControlFrame->SetVisible(false);
	}

}

// ----------------------------------------------------------------------------
void CHousesFrame::R_BuildHouseResult( int HouseDBID , bool Result )
{
	if( Result )
	{
		PlayerBaseRoleData* pBaseData = CNetGlobal::RoleData()->PlayerBaseData;
		pBaseData->HouseDBID = HouseDBID;
	}
}

// ----------------------------------------------------------------------------
void CHousesFrame::R_OpenVisitHouse( int TargetNpcID  , bool IsVisitMyHouse )
{
	m_VisitHouseNpcID = TargetNpcID;

	if( IsVisitMyHouse )
	{
		PlayerBaseRoleData* pBaseData = CNetGlobal::RoleData()->PlayerBaseData;
		NetCli_Houses::S_VisitHouseRequest( pBaseData->HouseDBID , "" );
	}
	else
	{
		//開對話盒
		SendWorldEvent( UiEvent_HousesOpenVisitHouse );
	}
}

// ----------------------------------------------------------------------------
void CHousesFrame::R_CloseVisitHouse()
{
	m_VisitHouseNpcID = -1;
	SendWorldEvent( UiEvent_HousesCloseVisitHouse );
}

// ----------------------------------------------------------------------------
void CHousesFrame::R_FindHouseResult( FindHouseResultENUM Result )
{
	//#define UiEvent_HousesVisitHousePasswordError  "HOUSES_VISIT_HOUSE_PASSWORDERROR"
	//#define UiEvent_HousesVisitHouseNotFind        "HOUSES_VISIT_HOUSE_NOTFIND"

	if( Result == EM_FindHouseResult_OK )
	{

	}
	else if( Result == EM_FindHouseResult_PasswordError )
	{
		SendWorldEvent( UiEvent_HousesVisitHousePasswordError );
	}
	else if( Result == EM_FindHouseResult_NotFind )
	{
		SendWorldEvent( UiEvent_HousesVisitHouseNotFind );
	}
	else
	{
		//assert( false );
	}
}

// ----------------------------------------------------------------------------
void CHousesFrame::R_HouseInfoLoading()
{
	//assert( m_eHousesState == EHousesState_Leave );
	m_eHousesState = EHousesState_Loading;
	//g_pError->OutputToFile( "HousesState_Loading" );
	//g_pChatFrame->SendMsgEvent( CHAT_MSG_SYSTEM, "CHousesFrame::R_HouseInfoLoading()", "" );


	m_FocusFurnishingID = -1;        //正在放置的傢俱ID
	m_MouseEnterFurnishingID = -1;

	m_ePlaceingMode = EM_PlaceingMode_None;     //放置模式
	m_ePlaceingModeTemp = EM_PlaceingMode_None;
	m_PlaceStep = 0;             //放置模式步驟 1移動 2旋轉

	m_PlaceFurnishingMode = false;
	m_bFurnishingChange = false;
  

	m_BuyFunction_NewOpenEnergy = 0;
	m_BuyFunction_FunctionID = -1;
 
	m_PlantFurnishingDBID = -1;
	m_PlantFurnishingPos  = -1;

	m_ObjectID_Seed = -1;
//	m_ObjectID_Water = -1;
//	m_ObjectID_Topdress = -1;
//	m_ObjectID_MagicWater = -1;
}

// ----------------------------------------------------------------------------
void CHousesFrame::R_HouseBaseInfo( HouseBaseDBStruct& HouseBaseInfo )
{
	if( m_eHousesState == EHousesState_Enter )
	{
		if( m_HouseBaseInfo.HouseName != HouseBaseInfo.HouseName )
		{
			const char* HouseName = HouseBaseInfo.HouseName;
			if( HouseName[0] == '\0' )
			{
				HouseName = g_ObjectData->GetString( "ZONE_DEFAULT" );
			}

	   		lua_State* L = m_interface->GetUi()->GetLuaState();
			
			lua_pushstring( L, HouseName );
			lua_setglobal( L, "arg1" );
			lua_pushnumber( L, g_pGameMain->GetZoneID() );
			lua_setglobal( L, "arg2" );

			m_interface->SendWorldEvent(UiEvent_ZoneChanged);
		}

		if( m_HouseBaseInfo.MaxItemCount != HouseBaseInfo.MaxItemCount )
		{
			m_HouseBaseInfo = HouseBaseInfo;


			//載入傢俱
			TD_HouseItemMap &rHouseItemMap = GetHouseFurnishingList();
			TD_HouseItemMap::iterator itPos = rHouseItemMap.begin();
			TD_HouseItemMap::iterator itEnd = rHouseItemMap.end();
			for( ; itPos != itEnd ; ++itPos )
			{
				LoadRoleSpriteHouseItem( itPos->second );
			}

			UpdateHousesBenefit();
		}
	}

	m_HouseBaseInfo = HouseBaseInfo;

//	BaseRoleData* pBaseData = &CNetGlobal::RoleData()->BaseData;
	PlayerBaseRoleData* pBaseData = CNetGlobal::RoleData()->PlayerBaseData;
	m_bIsOwner = m_HouseBaseInfo.HouseDBID == pBaseData->HouseDBID ? true : false;
	
	UpdateHouseFriend();

	if( m_eHousesState == EHousesState_Enter )
	{
		SendWorldEvent( UiEvent_HousesFrameShow );
	}
}

// ----------------------------------------------------------------------------
void CHousesFrame::R_ItemInfo( int ItemDBID , int ItemCount , HouseItemDBStruct Item[_DEF_MAX_HOUSE_FURNITURE] )
{
	map< int, HouseItemDBStruct >::iterator itPos = m_HouseItems.begin();
	map< int, HouseItemDBStruct >::iterator itEnd = m_HouseItems.end();
	for( ; itPos != itEnd ; )
	{
		if( itPos->second.ParentItemDBID == ItemDBID )
		{
			m_HouseItems.erase( itPos++ );
		}
		else
		{
			++itPos;
		}
	}

	m_HouseContainers[ ItemDBID ].bDirty = true;

//	m_HousesContainerMap.erase( ItemDBID );
//	TD_HouseItemMap &rHouseItemMap = m_HousesContainerMap[ItemDBID];

	
	int i;
	for( i = 0; i < ItemCount; ++i )
	{
		RegisterItemFieldChecksum( &Item[i].Item );

		m_HouseItems[Item[i].ItemDBID] = Item[i];
	}


	//事先產生容器
	//if( ItemDBID == DF_HouseFurnishingListID || 
	//	ItemDBID == DF_HouseServantListID )
	//{
	//	GameObjDbStructEx* itemDB;
	//	for( i = 0; i < ItemCount; ++i )
	//	{
	//		itemDB = CNetGlobal::GetObj( Item[i].Item.OrgObjID );
	//		if( !( itemDB && itemDB->Item.ItemType == EM_ItemType_Furniture ) )
	//			continue;
	//		if(	itemDB->Item.emFurnitureType == EM_FurnitureType_Storage		||
	//			itemDB->Item.emFurnitureType == EM_FurnitureType_Coathanger     || 
	//			itemDB->Item.emFurnitureType == EM_FurnitureType_WeaponSet	    ||
	//			itemDB->Item.emFurnitureType == EM_FurnitureType_WeaponAndCloth )
	//		{
	//			m_HousesContainerMap[ Item[i].ItemDBID ];
	//		}
	//	}
	//}

	m_HouseContainers[ItemDBID].bLoaded = true;


	if( m_eHousesState != EHousesState_Enter )
	{
		return;
	}

	if( ItemDBID == DF_HouseFurnishingListID )
	{
		SendWorldEvent( UiEvent_HousesFurnitureChanged );
		SendWorldEvent( UiEvent_HousesFrameShow );
	}
	else
	{
		HouseItemDBStruct* pHouseItemDB = GetHousesItemDBByDBID( ItemDBID );
		//assert( pHouseItemDB );
		if( !pHouseItemDB )
			return;
		GameObjDbStructEx* itemDB = CNetGlobal::GetObj( pHouseItemDB->Item.OrgObjID );
		//assert( itemDB );
		if( !itemDB )
			return;
		//assert( itemDB->Item.ItemType == EM_ItemType_Furniture );
		if( itemDB->Item.ItemType != EM_ItemType_Furniture )
			return;

		//EM_FurnitureType_Normal		= 0,	// 一般, 不具容量
		//EM_FurnitureType_Storage	    = 1,    // 具儲存空間的傢俱
		//EM_FurnitureType_Coathanger	= 2,	// 衣架, 且可以展示衣物, 裝備, 具儲存空間的傢俱
		//EM_FurnitureType_WeaponSet	= 3,	// 武器架, 可以掛載, 單雙手武器或盾, 具儲存空間的傢俱

		
		//Servant   女僕
		if( pHouseItemDB->ParentItemDBID == DF_HouseServantListID )
		{
			if( m_HouseContainers[ItemDBID].bOpenFlag )
			{
				m_HouseContainers[ItemDBID].bOpenFlag = false;
			}

			return;	
		}

		if( itemDB->Item.emFurnitureType == EM_FurnitureType_Normal )
		{

		}
		else if( itemDB->Item.emFurnitureType == EM_FurnitureType_Storage )
		{
			//判斷是否開啟容器
			if( m_HouseContainers[ItemDBID].bOpenFlag )
			{
				m_HouseContainers[ItemDBID].bOpenFlag = false;

				lua_State* L = m_interface->GetUi()->GetLuaState();
				lua_pushnumber( L, ItemDBID );
				lua_setglobal( L, "StorageDBID" );
				SendWorldEvent( UiEvent_HousesStorageShow );

				if( m_HouseContainers[ItemDBID].bOpened == false )
				{
					if( m_bIsFriend  )
					{
						//好友問候語
						if( strlen( g_pHousesFrame->m_HouseBaseInfo.Friend_Use ) > 0 )
						{
							g_pGameMain->SendSystemChatMsg( g_pHousesFrame->m_HouseBaseInfo.Friend_Use );
						}
					}

					m_HouseContainers[ItemDBID].bOpened = true;
					CRoleSprite* pRoleSprite = GetHouseRoleSpriteByItemDBID( ItemDBID );
					//assert( pRoleSprite );
					if( pRoleSprite )
					{
						pRoleSprite->SetActorState( ruFUSION_ACTORSTATE_ACTIVATE_BEGIN );
					}
				}
			}
		}
		else if( itemDB->Item.emFurnitureType == EM_FurnitureType_Coathanger    ||
			itemDB->Item.emFurnitureType == EM_FurnitureType_WeaponSet     ||
			itemDB->Item.emFurnitureType == EM_FurnitureType_WeaponAndCloth )
		{
			lua_State* L = m_interface->GetUi()->GetLuaState();
			lua_pushnumber( L, ItemDBID );
			lua_setglobal( L, "StorageDBID" );
			SendWorldEvent( UiEvent_HousesHangerChanged );
		}
		else if( itemDB->Item.emFurnitureType == EM_FurnitureType_Maid )
		{

		}
	}

	//更新女僕
	//if( ItemDBID == DF_HouseMaidHouseItemID )
	//{
	//	LoadRoleSpriteMaid();
	//}
  
}

// ----------------------------------------------------------------------------
void CHousesFrame::R_HouseInfoLoadOK()
{

	//EHousesState_HouseInfoLoading  ,
	//EHousesState_HangerLoading     ,
	//EHousesState_RoleSpriteLoading ,

	//assert( m_eHousesState == EHousesState_Loading );
	m_eHousesState = EHousesState_LoadOK;
	//g_pError->OutputToFile( "HousesState_LoadOK" );
	//g_pChatFrame->SendMsgEvent( CHAT_MSG_SYSTEM, "CHousesFrame::R_HouseInfoLoadOK()", "" );

	//取得所有衣架

	//int i;
	//for( i = 0 ; i < (int)m_HangerDBIDTable.size() ; ++i )
	//{
	//	NetCli_Houses::S_BoxInfoRequset( m_HangerDBIDTable[i] );
	//}

}

// ----------------------------------------------------------------------------
void CHousesFrame::R_SwapBodyHouseItemResult( int HouseParentItemDBID , int HousePos , int BodyPos , bool Result )
{
	if( false == Result )
	{
		if( HouseParentItemDBID == DF_HouseFurnishingListID )
		{
			HouseItemDBStruct* pHouseItemDB = GetHousesItemDB( HouseParentItemDBID, HousePos );

			if( NULL == pHouseItemDB )
			{
				return;
			}
			
			GameObjDbStructEx* itemDB = CNetGlobal::GetObj( pHouseItemDB->Item.OrgObjID );

			if( !( itemDB && itemDB->Item.ItemType == EM_ItemType_Furniture ) )
			{	
				return;
			}

			//if( itemDB->Item.emFurnitureType == EM_FurnitureType_Normal )
			//{
				//盆栽
				if( itemDB->IsPot() )
				{
					g_pGameMain->SendSystemMsg( g_ObjectData->GetString("SYS_HOUSES_SWAP_BODY_HOUSE_ITEM_POT_FAIL") );
					g_pGameMain->SendSystemChatMsg( g_ObjectData->GetString( "SYS_HOUSES_SWAP_BODY_HOUSE_ITEM_POT_FAIL" ) );
					return;
				}
				//return;
			//}

			//容器 衣架
			if(	itemDB->Item.emFurnitureType == EM_FurnitureType_Storage       ||
				itemDB->Item.emFurnitureType == EM_FurnitureType_Coathanger    ||
				itemDB->Item.emFurnitureType == EM_FurnitureType_WeaponSet     ||
				itemDB->Item.emFurnitureType == EM_FurnitureType_WeaponAndCloth )
			{
				g_pGameMain->SendSystemMsg( g_ObjectData->GetString("SYS_HOUSES_SWAP_BODY_HOUSE_ITEM_STORAGE_FAIL") );
				g_pGameMain->SendSystemChatMsg( g_ObjectData->GetString( "SYS_HOUSES_SWAP_BODY_HOUSE_ITEM_STORAGE_FAIL" ) );
				return;
			}
   
		}

	}
}

// ----------------------------------------------------------------------------
void CHousesFrame::R_FixHouseItem( HouseItemDBStruct& HouseItem )
{
	if( HouseItem.Item.OrgObjID == 0 ) //移除
	{
		HouseItemDBStruct* pHouseItemDB = g_pHousesFrame->GetHousesItemDB( HouseItem.ParentItemDBID, HouseItem.Pos );
		if ( pHouseItemDB )
		{
			m_HouseContainers.erase( pHouseItemDB->ItemDBID );
			m_HouseItems.erase( pHouseItemDB->ItemDBID );
		}
		//m_HouseItems.erase( HouseItem.ItemDBID );	
		//m_HouseContainers.erase( HouseItem.ItemDBID );
	}
	else
	{
		//HouseItemDBStruct& old //
		map< int, HouseItemDBStruct >::iterator itPos = m_HouseItems.find( HouseItem.ItemDBID );
		if( itPos != m_HouseItems.end() )
		{
			HouseItemDBStruct& old = itPos->second;
			m_HouseContainers[ old.ParentItemDBID ].bDirty = true;
			m_HouseContainers[ old.ParentItemDBID ].bChanged = true;
		}

		RegisterItemFieldChecksum( &HouseItem.Item );
  		m_HouseItems[ HouseItem.ItemDBID ] = HouseItem;
	}

	m_HouseContainers[ HouseItem.ParentItemDBID ].bDirty = true;
	m_HouseContainers[ HouseItem.ParentItemDBID ].bChanged = true;
	
	if( m_eHousesState != EHousesState_Enter )
	{
		g_pError->OutputToFile( "CHousesFrame::R_FixHouseItem but m_eHousesState != EHousesState_Enter" );
		return;
	}

	if( HouseItem.ParentItemDBID == DF_HouseFurnishingListID )
	{
		LoadRoleSpriteHouseItem( &HouseItem );

		UpdateHousesBenefit();
		SendWorldEvent( UiEvent_HousesFurnitureChanged );
		SendWorldEvent( UiEvent_HousesFrameShow );
	}

	//女僕
	if( HouseItem.ParentItemDBID == DF_HouseServantListID )
	{
		LoadRoleSpriteServant( HouseItem.Pos );
		UpdateUIServantIndex();
	}

	//盆栽
	if( m_PlantFurnishingDBID >= 0 && HouseItem.ItemDBID == m_PlantFurnishingDBID )
	{
		SendWorldEvent( UiEvent_HousesPlantChanged );
		return;
	}

//	GameObjDbStructEx* itemDB = CNetGlobal::GetObj( HouseItem.Item.OrgObjID );
//	if( itemDB && itemDB->Item.FurnitureSoundID > 0 )
//	{
		PlayHousesSound();
//	}
}


// ----------------------------------------------------------------------------
void CHousesFrame::HouseContainerChanged( int ContainerDBID )
{
	
	//判斷是不是衣架裡的物品更新
	HouseItemDBStruct* pParentHouseItemDB = GetHousesItemDBByDBID( ContainerDBID );

	if( pParentHouseItemDB )
	{
		GameObjDbStructEx* itemDB = CNetGlobal::GetObj( pParentHouseItemDB->Item.OrgObjID );
		//assert( itemDB && itemDB->Item.ItemType == EM_ItemType_Furniture );
		if( !( itemDB && itemDB->Item.ItemType == EM_ItemType_Furniture ) )
			return;

		//EM_FurnitureType_Normal		= 0,	// 一般, 不具容量
		//EM_FurnitureType_Storage	= 1,	// 具儲存空間的傢俱
		//EM_FurnitureType_Coathanger	= 2,	// 衣架, 且可以展示衣物, 裝備, 具儲存空間的傢俱
		//EM_FurnitureType_WeaponSet	= 3,	// 武器架, 可以掛載, 單雙手武器或盾, 具儲存空間的傢俱


		//Servant   女僕
		if( pParentHouseItemDB->ParentItemDBID == DF_HouseServantListID )
		{
			// 女僕
			//更新外觀
			LoadRoleSpriteServant( pParentHouseItemDB->Pos );

			lua_State* L = m_interface->GetUi()->GetLuaState();
			lua_pushnumber( L, ContainerDBID );
			lua_setglobal( L, "ServantDBID" );
			SendWorldEvent( UiEvent_Houses_ServantItemChanged );

			return;
		}

		//	if( itemDB->Item.emFurnitureType == EM_FurnitureType_Normal )
		//	{
		//		return;
		//	}

		//容器
		if( itemDB->Item.emFurnitureType == EM_FurnitureType_Storage && 
			m_HouseContainers[ ContainerDBID ].bLoaded )
		{
			lua_State* L = m_interface->GetUi()->GetLuaState();
			lua_pushnumber( L, ContainerDBID );
			lua_setglobal( L, "StorageDBID" );
			SendWorldEvent( UiEvent_HousesStorageChanged );

			return;
		}

		//衣架
		if( itemDB->Item.emFurnitureType == EM_FurnitureType_Coathanger    ||
			itemDB->Item.emFurnitureType == EM_FurnitureType_WeaponSet     ||
			itemDB->Item.emFurnitureType == EM_FurnitureType_WeaponAndCloth )
		{
			//更新外觀
			LoadRoleSpriteHouseItem( pParentHouseItemDB );

			lua_State* L = m_interface->GetUi()->GetLuaState();
			lua_pushnumber( L, ContainerDBID );
			lua_setglobal( L, "StorageDBID" );
			SendWorldEvent( UiEvent_HousesHangerChanged );

			return;
		}
	}
}

// ----------------------------------------------------------------------------
void CHousesFrame::R_SwapHouseItemResult( int ParentItemDBID[2] , int HousePos[2] , bool Result )
{

	//HouseItemDBStruct* pHouseItemDB1 = GetHousesItemDB( ParentItemDBID[0] , HousePos[0] );
	//HouseItemDBStruct* pHouseItemDB2 = GetHousesItemDB( ParentItemDBID[1] , HousePos[1] );

	//HouseItemDBStruct HouseItemDB1;
	//HouseItemDBStruct HouseItemDB2;

	//if( pHouseItemDB1 )
	//{
	//	HouseItemDB1 = *pHouseItemDB1;
	//	}
	//HouseItemDB1.ParentItemDBID = ParentItemDBID[1];
	//HouseItemDB1.Pos = HousePos[1]; 

	//  
	//if( pHouseItemDB2 )
	//{
	//	HouseItemDB2 = *pHouseItemDB2;
	//	}
	//HouseItemDB2.ParentItemDBID = ParentItemDBID[0];
	//HouseItemDB2.Pos = HousePos[0]; 

	// 
	//R_FixHouseItem( HouseItemDB1 );
	// 	R_FixHouseItem( HouseItemDB2 );

}

// ----------------------------------------------------------------------------
void CHousesFrame::R_HouseItemLayoutResult( int HousePos , bool Result )
{

}

// ----------------------------------------------------------------------------
void CHousesFrame::R_ChangeHouseResult( int HouseType , bool Result )
{
	if( !Result )
	{
		return;
	}

	if( m_eHousesState != EHousesState_Enter ) 
	{
  		return;

	}

	m_HouseBaseInfo.HouseType = HouseType;

 
	//傢俱   不擺設
	{
		TD_HouseItemMap &rHouseItemMap = GetHouseFurnishingList();
		TD_HouseItemMap::iterator itPos = rHouseItemMap.begin();
		TD_HouseItemMap::iterator itEnd = rHouseItemMap.end();
		for( ; itPos != itEnd ; ++itPos )
		{
			(itPos->second)->Layout.IsShow = false;
		}
	}

	//管家
	{
		TD_HouseItemMap &rHouseItemMap = GetHouseContainer( DF_HouseServantListID );
		TD_HouseItemMap::iterator itPos = rHouseItemMap.begin();
		TD_HouseItemMap::iterator itEnd = rHouseItemMap.end();
		for( ; itPos != itEnd ; ++itPos )
		{
			(itPos->second)->Layout.IsShow = false;
		}
	}
	
	//再ClientLoadingOK裡 會LoadRoleSpriteHouse()
	m_eHousesState = EHousesState_LoadOK;

	g_pLoadingFrame->LoadingStart(true);

}

// ----------------------------------------------------------------------------
void CHousesFrame::R_BuyFunctionResult( int FunctionID , bool Result )
{
	if( Result && m_BuyFunction_FunctionID == FunctionID && m_BuyFunction_NewOpenEnergy > 0 )
	{
		g_pGameMain->SendSystemChatMsgEx( g_ObjectData->GetString( "UI_HOUSEFRAME_COST_ENERGY" ) , m_BuyFunction_NewOpenEnergy );	
	}
}

// ----------------------------------------------------------------------------
void CHousesFrame::R_FunctionSettingResult( int FunctionID , int Mode , bool Result )
{

}

// ----------------------------------------------------------------------------
void CHousesFrame::R_BuyEnergyResult( int Money_Account , int Energy )
{

}

// ----------------------------------------------------------------------------
void CHousesFrame::R_SetPasswordResult( bool Result )
{
	//if( Result )
	//{
	//	m_HouseBaseInfo.Password = "";
	//}
	if( Result )
	{
		g_pGameMain->SendSystemChatMsg( g_ObjectData->GetString( "SYS_HOUSE_SET_PASSWORD_SUCCEED" ) );
	}
	else
	{
		g_pGameMain->SendSystemChatMsg( g_ObjectData->GetString( "SYS_HOUSE_SET_PASSWORD_FAIL" ) );
	}
	
}




// ----------------------------------------------------------------------------
void CHousesFrame::Update( float elapsedTime )
{
	static float lastTime = 0.0f;
	lastTime += elapsedTime;
	if ( lastTime > 0.2f  && m_VisitHouseNpcID != -1 )
	{
		lastTime = 0.0f;
		if ( g_pGameMain->GetDistanceWithPlayer(m_VisitHouseNpcID) > _MAX_ROLE_TOUCH_MAXRANGE_ )
		{
			SendWorldEvent( UiEvent_HousesCloseVisitHouse );
			m_VisitHouseNpcID = -1;
		}
	}


	if( m_eHousesState != EHousesState_Enter )
	{
		return;
	}


	//修正玩家位置
	FixedPlayerPos();

	//更新寶箱
	map<int,SHouseContainer>::iterator itPos = m_HouseContainers.begin();
	map<int,SHouseContainer>::iterator itEnd = m_HouseContainers.end();
	for( ; itPos != itEnd ; ++itPos )
	{
		SHouseContainer& rContainer = itPos->second;

		if( rContainer.bChanged )
		{
			rContainer.bChanged = false;
			int DBID = itPos->first;
			//GetHouseContainer( DBID );
			HouseContainerChanged( DBID );
		}
	}


	//
	{
		CRoleSprite* pSpritePlayer = g_pGameMain->GetPlayer();
		if( !pSpritePlayer )
		{
			return;
		}
		//float _elapsedTime = elapsedTime;
		//if( _elapsedTime * 24.0f > 1.0f )
		//{
		//	_elapsedTime	= 1.0f / 24.0f;
		//}

		//float fPercentage = 1.0f - powf( 0.7 , 5.0 * _elapsedTime );

		//m_ServantLookAt += ( pSpritePlayer->GetPos() - m_ServantLookAt) * fPercentage;

		m_ServantLookAt = pSpritePlayer->GetPos();
	}

	
	


	//更新女僕
	UpdateServants( elapsedTime );

}

// ----------------------------------------------------------------------------
static bool g_bFixedPlayerPos = true;
void CHousesFrame::FixedPlayerPos()
{
	if( !g_bFixedPlayerPos )
	{
		return;
	}

 	CRoleSprite* pRoleSpriteHouse = g_pGameMain->FindSpriteByWorldGUID( m_HouseRoleSpriteID );
	if( !pRoleSpriteHouse )
	{
		return;
	}
 
	CRoleSprite* pSpritePlayer = g_pGameMain->GetPlayer();
	if( !pSpritePlayer )
	{
		return;
	}
	CRoleSprite* spriteControlProxy = pSpritePlayer->GetProxyControlSprite();
	if( !spriteControlProxy )
	{
		return;
	}

	//玩家 到 ( 0 0 0 ) 的向量
	CRuVector3 vecOrigin( 0.0f, 5.0f , 0.0f );
	CRuVector3 vecPlayer = spriteControlProxy->GetPos();

	vecPlayer.y += 5.0f; //避免只陷到地下一點點 就被拉回

	CRuVector3 vecDir = vecOrigin - vecPlayer;

	float fDistance = vecDir.Magnitude();

	if( fDistance < 5 )
	{
		return;
	}

	vecDir.Normalize();

	CRuVector3 vecSegmentOrigin = vecOrigin - ( vecDir * 1000 );

	// 建構碰撞測試結構
	CRuCollisionQuery_Segment_Nearest query;

	// 設定測試用線段及其他設定
	query.m_colSegment = CRuSegment3( vecSegmentOrigin, vecSegmentOrigin + ( vecDir * 2000.0f ) );
	query.m_ignoreBackface = FALSE;
	query.m_ignoreCollisionFlag = TRUE;

	// 測試碰撞
	pRoleSpriteHouse->GetRuEntity()->QueryCollision(&query);

	// 檢測回應
	if( query.m_positiveHit )
	{
		float fHouseDistance = ( vecOrigin - query.m_collisionPoint ).Magnitude(); 

		if( fDistance <= fHouseDistance )
		{
			//正常
			return;
		}

	}


	spriteControlProxy->SetPos( CRuVector3( 0.0f, 0.2f, 0.0f ) );
	pSpritePlayer->SetSitChairState(false);


	//CRuContainer_Collision* pCollision = IBaseSprite::GetCollisionContainer();

	//if ( pCollision )
	//{
	//	CRoleSprite* pSpritePlayer = g_pGameMain->GetPlayer();

	//	if( pSpritePlayer )
	//	{
	//		CRoleSprite* spriteControlProxy = pSpritePlayer->GetProxyControlSprite();
	//		CRuVector3 position = spriteControlProxy->GetPos();

	//		if( position.m_y < -100.0f )
	//		{
	//			//position.m_y = 20.0f;
	//			//float fHeight = RuEntity_FindRestingHeight( position, pCollision, 50.0f );
	//			//if ( abs( fHeight - position.m_y) < 50.0f )
	//			//	position.m_y = fHeight;
	//			//spriteControlProxy->SetPos( position );

	//			position.m_x = 0.0f;
	//			position.m_y = 0.0f;
	//			position.m_z = 5.0f;
	//			spriteControlProxy->SetPos( position );

	//		}
	//	}
	//}

	////玩家進入房屋可能會掉到房屋下 進入房屋後修正一次
	//if( false == m_FixedPlayerPos )
	//{
	//	CRoleSprite* pSpritePlayer = g_pGameMain->GetPlayer();
	//	CRoleSprite* pSpriteHouse  = g_pGameMain->FindSpriteByWorldGUID( m_HouseRoleSpriteID );

	//	if( pSpritePlayer && 
	//		pSpritePlayer->IsProxyLoadComplete() && 
	//		pSpriteHouse && 
	//		pSpritePlayer->IsProxyLoadComplete() )
	//	{
	//		m_FixedPlayerPos = true;

	//		CRoleSprite* spriteControlProxy = pSpritePlayer->GetProxyControlSprite();
	//		CRuVector3 position = spriteControlProxy->GetPos();

	//		position.m_y = 20.0f;

	//		float fHeight = RuEntity_FindRestingHeight( position, pCollision, 50.0f );
	//		if ( abs( fHeight - position.m_y) < 50.0f )
	//			position.m_y = fHeight;

	//		spriteControlProxy->SetPos( position );
	//	}
	//}

	//不明原因(碰撞?) 傢俱位置會偏位 暫時先一直重設
	/*
	CRoleSprite* pSprite = NULL;

	TD_HouseItemMap &rHouseItemMap = m_HousesContainerMap[DF_HouseFurnishingListID];
	TD_HouseItemMap::iterator itPos = rHouseItemMap.begin();
	TD_HouseItemMap::iterator itEnd = rHouseItemMap.end();
	for( ; itPos != itEnd ; ++itPos )
	{
	if( itPos->second.Layout.IsShow == false )
	{
	continue;
	}
	if( itPos->first == m_FocusFurnishingID )
	{
	continue;
	}
	pSprite = GetHouseRoleSpriteByItemDBID( itPos->first );
	assert( pSprite );
	if( NULL == pSprite || NULL == pSprite->GetRuEntity())
	{
	continue;
	}

	pSprite->SetPos( CRuVector3( itPos->second.Layout.X , itPos->second.Layout.Y , itPos->second.Layout.Z ) );
	pSprite->EnableCollision( true );

	}
	*/
}

// ----------------------------------------------------------------------------
bool CHousesFrame::IsHousesLoading()
{
	if( m_eHousesState == EHousesState_Leave )
	{
		return false;
	}

	if( m_eHousesState == EHousesState_Loading )
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

	m_ServantLookAt = pSpritePlayer->GetPos();
 
	return false;
}

// ----------------------------------------------------------------------------
float CHousesFrame::ClientLoadingOK()
{
	//g_pChatFrame->SendMsgEvent( CHAT_MSG_SYSTEM, "CHousesFrame::ClientLoadingOK()", "" );

	if( m_eHousesState != EHousesState_LoadOK ) //目前狀態 //準備進入 => 進入房屋
	{
		return 0.0f;
	}

	//判斷是不是主人
//	BaseRoleData* pBaseData = &CNetGlobal::RoleData()->BaseData;
	PlayerBaseRoleData* pBaseData = CNetGlobal::RoleData()->PlayerBaseData;
	m_bIsOwner = m_HouseBaseInfo.HouseDBID == pBaseData->HouseDBID ? true : false;

	const char* HouseName = m_HouseBaseInfo.HouseName;
	if( HouseName[0] == '\0' )
	{
		HouseName = g_ObjectData->GetString( "ZONE_DEFAULT" );
	}

	lua_State* L = m_interface->GetUi()->GetLuaState();

	lua_pushstring( L, HouseName );
	lua_setglobal( L, "arg1" );
	lua_pushnumber( L, g_pGameMain->GetZoneID() ); 
	lua_setglobal( L, "arg2" );
	m_interface->SendWorldEvent(UiEvent_ZoneChanged);
     
	//g_pChatFrame->SendMsgEvent( CHAT_MSG_SYSTEM, "載入房屋", "" );

	//載入房屋 // load house, this is where the house gets loaded but not all dependencies yet
	LoadRoleSpriteHouse();

	//載入管家
	LoadRoleSpriteMaid();

	//載入傢俱
	TD_HouseItemMap &rHouseItemMap = GetHouseFurnishingList();

	TD_HouseItemMap::iterator itPos = rHouseItemMap.begin();
	TD_HouseItemMap::iterator itEnd = rHouseItemMap.end();
	for( ; itPos != itEnd ; ++itPos )
	{
		LoadRoleSpriteHouseItem( itPos->second );
	}

	//載入雇用管家
	for( int i = 0 ; i < MAX_SERVANT_COUNT ; ++i )
	{
		LoadRoleSpriteServant( i );
	}

	UpdateUIServantIndex();
	
	//計算 工作效益
  	UpdateHousesBenefit();

	//檢查keyitem 有沒有寫入
	RoleDataEx* pRole = CNetGlobal::RoleData();

	if( m_HouseBaseInfo.HouseType == 575523 )
	{
		if( !pRole->CheckKeyItem( 542670 ) )
		{
			NetCli_OtherChild::S_RunGlobalPlotRequest( 7, 0 );
		}
	}
	else if( m_HouseBaseInfo.HouseType == 575524 )
	{
		if( !pRole->CheckKeyItem( 542670 ) || !pRole->CheckKeyItem( 542671 ) )
		{
			NetCli_OtherChild::S_RunGlobalPlotRequest( 7, 0 );
		}
	}


   	//g_pError->OutputToFile( "HousesState_Enter" );

	m_eHousesState = EHousesState_Enter;
	SendWorldEvent( UiEvent_HousesFrameShow );

	if( m_bIsOwner && m_HouseBaseInfo.UnCheckItemLogCount > 0 )
	{
		//有新的物品存取記錄
		g_pGameMain->SendSystemChatMsg( g_ObjectData->GetString( "SYS_HOUSE_UNCHECK_ITEM_LOG" ) );
		SendWorldEvent( UiEvent_HousesFriendItemLogShow );
	}
   
	m_pHousesControlFrame->SetVisible(true);

	m_FixedPlayerPos = false;

 	if( m_bIsFriend  )
	{
		//好友問候語
		if( strlen( g_pHousesFrame->m_HouseBaseInfo.Friend_Enter ) > 0 )
		{
			g_pGameMain->SendSystemChatMsg( g_pHousesFrame->m_HouseBaseInfo.Friend_Enter );
		}
	}

	//#define DF_ServantEvent_Enter       1
	//#define DF_ServantEvent_FrientEnter 2

	//女僕進入事件
	for( int i = 0; i < MAX_SERVANT_COUNT ; ++i )
	{
		HouseItemDBStruct* pServantItemDB = GetHousesItemDB( DF_HouseServantListID , i );
		HouseServantStruct* pServant = &m_HouseBaseInfo.Servant[i]; 
		SHousesServantWorkInfo& rWorkInfo = m_ServantWorkInfos[i];
												   
		if( NULL == pServantItemDB )
		{
			continue;
		}

		if( pServant->NPCObjID == 0 )
		{
			continue;
		}

		if( m_bIsOwner )
		{
			NetCli_Houses::SL_ServantEventRequest( pServantItemDB->ItemDBID , DF_ServantEvent_Enter , DF_ServantEvent_Enter_Owner );
		}
		else if( m_bIsFriend )
		{
			NetCli_Houses::SL_ServantEventRequest( pServantItemDB->ItemDBID , DF_ServantEvent_Enter , DF_ServantEvent_Enter_Friend );
		}
		else
		{
			NetCli_Houses::SL_ServantEventRequest( pServantItemDB->ItemDBID , DF_ServantEvent_Enter , DF_ServantEvent_Enter_Other );
		}
	}


	//背景音樂
	PlayHousesSound();

	return 2.0f;
}

//招募列表回應
void CHousesFrame::RL_ServantHireList( int ID , bool End , HouseServantStruct& Servant )
{
	SServantHireList temp;
	temp.ID = ID;
	temp.Servant = Servant; 

	m_ServantHireList.push_back( temp );

	if( End )
	{
		SendWorldEvent( UiEvent_Houses_ServantHireListShow );		
	}
}

//招募女僕結果
void CHousesFrame::RL_ServantHireResult( bool Result , int ID , int Pos , HouseServantStruct& Servant )
{
	if( Result )
	{
		if( Pos < 0 || Pos > MAX_SERVANT_COUNT )
		{
			return;
		}

		m_HouseBaseInfo.Servant[Pos] = Servant;

		LoadRoleSpriteServant( Pos );

		UpdateUIServantIndex();

		//SendWorldEvent( UiEvent_Houses_ServantHireListChanged );
		SendWorldEvent( UiEvent_Houses_ServantListChanged );
	}
	else
	{
		SendWorldEvent( UiEvent_Houses_ServantHireFailed );

		g_pGameMain->SendSystemChatMsg( g_ObjectData->GetString( "SYS_HOUSES_SERVANT_HIRE_FAILED" ) );


		//雇用失敗 更新名單
		g_pHousesFrame->m_ServantHireList.clear();
		NetCli_Houses::SL_ServantHireListRequest();
		
	}

}

//女僕刪除結果
void CHousesFrame::RL_ServantDelResult( int ServantID , bool Result )
{
	if( false == Result )
	{
		return;
	}

	if( ServantID < 0 || ServantID >= MAX_SERVANT_COUNT )
	{
		return;
	}

	m_HouseBaseInfo.Servant[ServantID].NPCObjID = 0;
	m_HouseBaseInfo.Servant[ServantID].Layout.IsShow = false;
	LoadRoleSpriteServant( ServantID );

	UpdateUIServantIndex();

	SendWorldEvent( UiEvent_Houses_ServantListChanged );
}

//女僕互動事件通知
void CHousesFrame::RL_ServantEvent( int ServantID , int EventID )
{
	//g_pGameMain->SendSystemChatMsgEx( "ServantID = %d, EventID = %d", ServantID , EventID );

	if( ServantID < 0 || ServantID >= MAX_SERVANT_COUNT )
	{
		return;
	}

	if( EventID < 0 || EventID >= (int)g_ObjectData->_ServantEventList.size() )
	{
		return;		
	}

 	//女僕
	HouseItemDBStruct* pServantItemDB = GetHousesItemDB( DF_HouseServantListID , ServantID );
	HouseServantStruct* pServant = &m_HouseBaseInfo.Servant[ServantID]; 
	SHousesServantWorkInfo& rWorkInfo = m_ServantWorkInfos[ServantID];

	if( NULL == pServantItemDB )
	{
		return;
	}

	if( pServant->NPCObjID == 0 )
	{
		return;
	}

	CRoleSprite* pRoleSprite = GetHouseRoleSpriteByItemDBID( pServantItemDB->ItemDBID );

	if( NULL == pRoleSprite || false == pRoleSprite->IsVisible() )
	{
		return;
	}


	//事件
	ServantEventTableStruct& rServantEvent = g_ObjectData->_ServantEventList[EventID];
//		rServantEvent.TalkStr;

	//說話
	if( strlen( rServantEvent.TalkStr ) != 0 )
	{
		//string strName =  GetServantName( *pServant );
		//string strTalk = g_ObjectData->GetString( rServantEvent.TalkStr );
		//g_pChatFrame->SendChatMessage( CHAT_MSG_WHISPER, strName.c_str(), strTalk.c_str() );

		ServantTalk( CHAT_MSG_WHISPER, ServantID, rServantEvent.TalkStr );
	}
	
}

//女僕命名結果
void CHousesFrame::RL_ServantRenameResult( const char* Name , bool Result )
{
	if( false == Result )
	{
		return;
	}

	//
}

//修正女僕數值
void CHousesFrame::RL_FixServantValue( int ServantID , HouseServantValueTypeENUM Type , int Value )
{
	if( ServantID < 0 || ServantID >= MAX_SERVANT_COUNT )
	{
		return;
	}

	HouseServantStruct* pServant = &m_HouseBaseInfo.Servant[ServantID]; 
	pServant->WriteValue( Type, Value );

	HouseItemDBStruct* pHouseItemDB = GetHousesItemDB( DF_HouseServantListID, ServantID );
	if ( pHouseItemDB )
	{
		lua_State* L = m_interface->GetUi()->GetLuaState();
		lua_pushnumber( L, pHouseItemDB->ItemDBID );
		lua_setglobal( L, "ServantDBID" );

		g_pHousesFrame->SendWorldEvent( UiEvent_Houses_ServantInfoUpdate );
	}	
}

//
void CHousesFrame::ServantTalk( const char* msgType, int ServantID,  const char* msg )
{
	if( ServantID < 0 || ServantID >= MAX_SERVANT_COUNT )
	{
		return;
	}



	HouseItemDBStruct* pServantItemDB = GetHousesItemDB( DF_HouseServantListID , ServantID );
	HouseServantStruct* pServant = &m_HouseBaseInfo.Servant[ServantID]; 
	SHousesServantWorkInfo& rWorkInfo = m_ServantWorkInfos[ServantID];

	CRoleSprite* pRoleSprite = GetHouseRoleSpriteByItemDBID( pServantItemDB->ItemDBID );

	if( NULL == pRoleSprite || false == pRoleSprite->IsVisible() )
	{
		return;
	}

 
	string strServantName =  GetServantName( *pServant );  
	string strMsg = g_ObjectData->GetString( msg );

	strMsg = ReplaceKeyString( strMsg, "<ServantName>", strServantName.c_str() );
	strMsg = ReplaceKeyString( strMsg, "<PlayerName>", g_pGameMain->GetPlayerName() );

	if( strcmp( msgType, "CHAT_MSG_SAY" ) == 0 )
	{
		g_pSpeakFrame->AddText( pRoleSprite->GetIdentify() , strMsg.c_str(), 1.0f, 1.0f, 1.0f );
	}

	
	g_pChatFrame->SendChatMessage( msgType, strServantName.c_str(), strMsg.c_str() );
}

//RL_LuaClientEvent lua 觸發的client 端事件
void CHousesFrame::Servant_LuaClientEvent( int GItemID , int EventType , const char* Content )
{
	//void	ClientEvent       ( int SendID , int GItemID , int EventType , const char* Content ); 
	//void	ClientEvent_Range ( int GItemID , int EventType , const char* Content ); 
	
	//
	//g_pGameMain->SendSystemChatMsg( Content );

	//分析
	//女僕ID type data


	int ServantID;
	char type[256]; type[0] = 0;
	char data[256]; data[0] = 0;

	int count = sscanf( Content, "%d %s %s", &ServantID, &type, &data );

	if( count < 2 )
	{
		return;
	}

	if( ServantID < 0 || ServantID >= MAX_SERVANT_COUNT )
	{
		return;
	}

	HouseItemDBStruct* pServantItemDB = GetHousesItemDB( DF_HouseServantListID , ServantID );
	HouseServantStruct* pServant = &m_HouseBaseInfo.Servant[ServantID]; 
	SHousesServantWorkInfo& rWorkInfo = m_ServantWorkInfos[ServantID];

	if( NULL == pServantItemDB )
	{
		return;
	}

	if( pServant->NPCObjID == 0 )
	{
		return;
	}

	//LOCK
	//UNLOCK
	//SAY
	//WHISPER
	//SYSTEM

	if( strcmp( type, "LOCK" ) == 0 )
	{
		rWorkInfo.mode = 100;
		rWorkInfo.isWorking = true;
		rWorkInfo.workBeginTime = (int)g_pGameMain->GetGameTime();
	}
	else if( strcmp( type, "UNLOCK" ) == 0 )
	{
		rWorkInfo.mode = 0;
		rWorkInfo.isWorking = false;
	}
	else if( strcmp( type, "SAY" ) == 0 )
	{
		ServantTalk( CHAT_MSG_SAY, ServantID, data );
	}
	else if( strcmp( type, "WHISPER" ) == 0 )
	{
		ServantTalk( CHAT_MSG_WHISPER, ServantID, data );
	}
	else if( strcmp( type, "SYSTEM" ) == 0 )
	{
		ServantTalk( CHAT_MSG_SYSTEM, ServantID, data );
	}
	else if( strcmp( type, "EMOTE" ) == 0 )
	{
		ServantTalk( CHAT_MSG_EMOTE, ServantID, data );
	}
	else
	{
		return;
	}

}

// ----------------------------------------------------------------------------
void CHousesFrame::LoadRoleSpriteMaid()
{
	float Pos_X = 12.0f; 
	float Pos_Y = 0.0f; 
	float Pos_Z = 5.0f; 
	float Pos_Dir = 180; 

	int OrgObjID = 110758;
	//110758

	int GItemID   = m_MaidRoleSpriteID;   
	int WorldGUID = m_MaidRoleSpriteID; 


	GameObjDbStructEx* itemDB = CNetGlobal::GetObj( OrgObjID );

	//assert(itemDB);
	if( !itemDB )
	{
		return;
	}


	int DBID = -1;

	CRoleSprite* pSprite = NULL;

	bool isNew;
	pSprite = g_pGameMain->CreateSprite( GItemID, DBID, WorldGUID, isNew );
	//assert( pSprite );
	if ( !pSprite )
	{
		return ;
	}

	CRuVector3 pos(Pos_X, Pos_Y, Pos_Z);
	float angle = Pos_Dir * PI / 180.0f;

	pSprite->SetObjectId( OrgObjID, false );
	//pSprite->SetObjectId(OrgObjID);
	//pSprite->LoadRuEntity( actName );
	pSprite->SetObjectType(eSpriteObject_Unknown);

	pSprite->SetShadowType(eSpriteShadowType_Dot);
	pSprite->SetCursorState(true);		// 可以被游標檢測
	pSprite->SetTouchState(false);		// 可以被點選
	pSprite->SetTargetState(true);		// 不能出現目標框
	pSprite->SetNameVisible(false);		// 不能出現頭上字串

	//case EM_GameObjectMineType_Mine:
	pSprite->SetMinimapIcon( 0 ); // EM_MINIMEM_MinimapOption_Mine
	pSprite->SetCheckedStatus( eChecked_Maid );  

	pSprite->SetMaxHP(100);
	pSprite->SetMaxMP(0);
	pSprite->SetMaxSP(0);
	pSprite->SetHP(100);
	pSprite->SetMP(0);
	pSprite->SetSP(0);
	pSprite->SetSex(EM_Sex_Girl);

	// Set fixed sprite orienting axis if the axis is valid

	// Set fixed orienting axis
	pSprite->SetOrientingAxis(CRuVector3(0.0, 1.0, 0.0));

	// Enable re-basing of direction vector around the orienting axis
	pSprite->SetRebaseDirection(true);

	// Disable orient to surface normal
	pSprite->SetOrientToSurfaceNormal(false);

	pSprite->SetName( itemDB->GetName() );
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


// ----------------------------------------------------------------------------
void CHousesFrame::UpdateUIServantIndex()
{
	std::multimap<int,int> _mapTemp;

	int i;
	for( i = 0 ; i < 6 ; ++i )
	{
		HouseServantStruct& rServant = m_HouseBaseInfo.Servant[i];

		if( rServant.NPCObjID == 0 )
		{
			continue;
		}

		_mapTemp.insert( make_pair( rServant.CreateTime, i) );

		//_mapTemp[ rServant.CreateTime ] = i;	

		m_UIServantIndex[0] = -1;
	}

	i = 0;

	std::multimap<int,int>::iterator itPos = _mapTemp.begin();
	std::multimap<int,int>::iterator itEnd = _mapTemp.end();

	for( ; itPos != itEnd ; ++itPos )
	{
		m_UIServantIndex[i] = itPos->second;
		++i;
	}
	m_ServantCount = i;
}

// ----------------------------------------------------------------------------
void CHousesFrame::LoadRoleSpriteServant( int index )
{
	if( index < 0 || index > MAX_SERVANT_COUNT )
	{
		return;
	}

	HouseServantStruct& rServant = m_HouseBaseInfo.Servant[index];
	HouseItemDBStruct* pHouseItemDB = g_pHousesFrame->GetHousesItemDB( DF_HouseServantListID, index );

	if( NULL == pHouseItemDB )
	{
		return;
	}

	SHousesServantWorkInfo& rWorkInfo = m_ServantWorkInfos[index];
	if( rWorkInfo.isWorking )
	{
		rWorkInfo.isWorking = false;
	}
	
	UpdateServantWorkInfo( index );

	int OrgObjID = rServant.NPCObjID; // 110758;
	
	int GItemID   = DF_HouseRoleSpriteWorldGUID + pHouseItemDB->ItemDBID;   
	int WorldGUID = DF_HouseRoleSpriteWorldGUID + pHouseItemDB->ItemDBID; 
	

	int DBID = -1;
	CRoleSprite* pSprite = NULL;

	pSprite = g_pGameMain->FindSpriteByWorldGUID( WorldGUID );

	GameObjDbStructEx* itemDB = CNetGlobal::GetObj( OrgObjID );

	//沒有東西 或 不顯示  
	//判斷是不是未開啟的空間
	if( !itemDB || false == pHouseItemDB->Layout.IsShow )
	{
		if( pSprite )
		{
			pSprite->SetVisible( false );
			pSprite->EnableCollision(FALSE);
			pSprite->SetCursorState( false ); 
		}

		return;
	}

	if( NULL == pSprite )
	{
		bool isNew;
		pSprite = g_pGameMain->CreateSprite( GItemID, DBID, WorldGUID, isNew );
		//assert( pSprite );
		if ( !pSprite )
		{
			return;
		}
	}

 	pSprite->ReleaseEntity();

	pSprite->SetObjectId( OrgObjID, false );
	//pSprite->SetObjectId(OrgObjID);
	//pSprite->LoadRuEntity( actName );
	pSprite->SetObjectType(eSpriteObject_Unknown);

	pSprite->SetShadowType(eSpriteShadowType_Dot);
	pSprite->SetCursorState(true);		// 可以被游標檢測
	pSprite->SetTouchState(false);		// 可以被點選
	pSprite->SetTargetState(true);		// 不能出現目標框
	pSprite->SetNameVisible(false);		// 不能出現頭上字串

	//case EM_GameObjectMineType_Mine:
	pSprite->SetMinimapIcon( 0 ); // EM_MINIMEM_MinimapOption_Mine
	//pSprite->SetCheckedStatus( eChecked_Maid );  
	pSprite->SetCheckedStatus( eChecked_Servant );  

	pSprite->SetMaxHP(100);
	pSprite->SetMaxMP(0);
	pSprite->SetMaxSP(0);
	pSprite->SetHP(100);
	pSprite->SetMP(0);
	pSprite->SetSP(0);
	pSprite->SetSex( EM_Sex_Girl );

	static bool s_TorsoTurn = true;
	pSprite->EnableTorsoTurn( s_TorsoTurn );

//	pSprite->EnableTorsoTurn(true);

//	if( pSprite->IsVisible() )
//	{
//		House3DLayoutStruct& rLayout = pHouseItemDB->Layout;
//		pSprite->SetTargetPos( CRuVector3( rLayout.X, rLayout.Y, rLayout.Z ) );
//	}
//	else
//	{
		House3DLayoutStruct& rLayout = pHouseItemDB->Layout;

		CRuQuaternion orientation;
		orientation.FromAngleAxis( CRuVector3(0.0, 1.0, 0.0), rLayout.Angle );
		CRuVector3 direction = CRuVector3( 0.0f, 0.0f, -1.0f ) * orientation.GetMatrix();

		direction.Normalize();

		pSprite->SetDirection( direction, true, true );
		pSprite->SetPos( CRuVector3( rLayout.X, rLayout.Y, rLayout.Z ) );
//	}

	rWorkInfo.dirHead = direction;
	rWorkInfo.dirFoot = direction;
  
	// Set fixed sprite orienting axis if the axis is valid

	// Set fixed orienting axis
	pSprite->SetOrientingAxis(CRuVector3(0.0, 1.0, 0.0));

	// Enable re-basing of direction vector around the orienting axis
	pSprite->SetRebaseDirection(true);

	// Disable orient to surface normal
	pSprite->SetOrientToSurfaceNormal(false);

  	string strName = GetServantName( rServant );
  	pSprite->SetName( strName.c_str() );

	pSprite->SetFade(false, 1.0f);
	pSprite->SetGravityMode(true);
	pSprite->SetWalkState(true);
	pSprite->SetMoveSpeed( 50.0f );
 
	// 設定sprite顯示
	//pSprite->SetVisible(GraphObj->Mode.Show);
	pSprite->SetVisible( true );

	// 設定sprite碰撞開關
	pSprite->EnableCollision(false);

	pSprite->SetCanAttackState(false);
	pSprite->SetEnemyState(false);

	{
		const char* name = NULL;
		GameObjDbStruct* pObj = CNetGlobal::GetObj( rServant.Look.FaceID );
		GameObjDbStruct* pImageDB = NULL;
		if ( pObj )
		{
			pImageDB = CNetGlobal::GetObj(pObj->ImageID);
		}	

		if ( pImageDB )
		{
			pSprite->SetPaperdollPart( PAPERDOLL_PART_NAME_HEAD, pImageDB->Image.ACTWorld );
		}
	}
	{
		const char* name = NULL;
		GameObjDbStruct* pObj = CNetGlobal::GetObj( rServant.Look.HairID );
		GameObjDbStruct* pImageDB = NULL;
		if ( pObj )
		{
			pImageDB = CNetGlobal::GetObj(pObj->ImageID);
		}	

		if ( pImageDB )
		{
			pSprite->SetPaperdollPart( PAPERDOLL_PART_NAME_HAIR, pImageDB->Image.ACTWorld );
		}
	}

	pSprite->SetPaperdollColor(PAPERDOLL_PART_NAME_HEAD, rServant.Look.BodyColor | 0xFF000000 , 0);
	pSprite->SetPaperdollColor(PAPERDOLL_PART_NAME_HAIR, rServant.Look.HairColor | 0xFF000000 , 0);

	pSprite->SetBoneScale(BONE_SCALE_NAME_HEAD,		(BYTE) rServant.Look.BoneScale[0] / 100.0f);
	pSprite->SetBoneScale(BONE_SCALE_NAME_BREASTS,	(BYTE) rServant.Look.BoneScale[1] / 100.0f);
	pSprite->SetBoneScale(BONE_SCALE_NAME_BODY,		(BYTE) rServant.Look.BoneScale[2] / 100.0f);
	pSprite->SetBoneScale(BONE_SCALE_NAME_CHEST,	(BYTE) rServant.Look.BoneScale[3] / 100.0f);
	pSprite->SetBoneScale(BONE_SCALE_NAME_PELVIS,	(BYTE) rServant.Look.BoneScale[4] / 100.0f);
	pSprite->SetBoneScale(BONE_SCALE_NAME_ARM,      (BYTE) rServant.Look.BoneScale[5] / 100.0f);
	pSprite->SetBoneScale(BONE_SCALE_NAME_FOREARM,	(BYTE) rServant.Look.BoneScale[6] / 100.0f);
	pSprite->SetBoneScale(BONE_SCALE_NAME_HAND,		(BYTE) rServant.Look.BoneScale[7] / 100.0f);
	pSprite->SetBoneScale(BONE_SCALE_NAME_LEGS,		(BYTE) rServant.Look.BoneScale[8] / 100.0f);
	pSprite->SetBoneScale(BONE_SCALE_NAME_CALF,		(BYTE) rServant.Look.BoneScale[9] / 100.0f);
	pSprite->SetBoneScale(BONE_SCALE_NAME_FOOT,		(BYTE) rServant.Look.BoneScale[10] / 100.0f);

//	GameObjDbStructEx* itemDB;
	GameObjDbStructEx* imageitemDB;

 	int EQPos;

	int color[2];
	color[0] = color[1] = 0x00000000;

	//衣架的性別
	string filename;
	//CRuEntity* pEntity = model->GetRuEntity();
	if( pSprite->GetRuEntity() )
	{
		filename = pSprite->GetRuEntity()->GetResourceName(); 
	}

	Sex_ENUM _eSex = ObjectDataClass::GetModelSex( filename.c_str() );

	TD_HouseItemMap &rHouseItemMap = GetHouseContainer( pHouseItemDB->ItemDBID );
	TD_HouseItemMap::iterator itPos = rHouseItemMap.begin();
	TD_HouseItemMap::iterator itEnd = rHouseItemMap.end();

	HouseItemDBStruct* pEqItemDB;

	for( ; itPos != itEnd ; ++itPos )
	{
		pEqItemDB = itPos->second;

		if( pEqItemDB->Pos < DF_ServantArmorPos || pEqItemDB->Pos > DF_ServantArmorPosEnd )
		{
			continue;
		}
			
		EQPos = pEqItemDB->Pos;

		ItemFieldStruct& rItemField = pEqItemDB->Item;
		itemDB = CNetGlobal::GetObj( rItemField.ImageObjectID );
		if( itemDB == NULL )
		{
			continue;
		}

		GameObjDbStructEx* imageDB = CNetGlobal::GetObj(itemDB->ImageID);
		if ( imageDB == NULL )
		{
			continue;
		}

		imageitemDB = CNetGlobal::GetObj( rItemField.ImageObjectID );
		if( imageitemDB == NULL )
		{
			imageitemDB = itemDB;
		}

		SexFlagStruct& LimitSex1 = imageitemDB->Limit.Sex;
		SexFlagStruct& LimitSex2 = imageDB->Image.LimitSex;

		bool boy = true;
		bool girl = true;
		if( LimitSex1._Sex != 0 )
		{
			if( LimitSex1.Boy == false )
			{
				boy = false;
			}
			if( LimitSex1.Girl == false )
			{
				girl = false;
			}
		}

		if( LimitSex2._Sex != 0 )
		{
			if( LimitSex2.Boy == false )
			{
				boy = false;
			}
			if( LimitSex2.Girl == false )
			{
				girl = false;
			}
		}

		if( _eSex == EM_Sex_Boy  && boy  == false || 
			_eSex == EM_Sex_Girl && girl == false )
		{
			continue;
		}


		if( rItemField.MainColor == 0 )
			color[0] = imageDB->Image.ArmorMainColor.Color;	
		else
			color[0] = rItemField.MainColor;

		if( rItemField.OffColor == 0 )
			color[1] = imageDB->Image.ArmorOffColor.Color;	
		else
			color[1] = rItemField.OffColor;

		
		switch( EQPos )
		{
		case EM_EQWearPos_Head       : pSprite->SetPaperdollPart( PAPERDOLL_PART_NAME_HELMET   , imageDB->Image.ACTWorld ); pSprite->SetPaperdollColor( PAPERDOLL_PART_NAME_HELMET  , color[0], color[1] ); break;   //
		case EM_EQWearPos_Gloves     : pSprite->SetPaperdollPart( PAPERDOLL_PART_NAME_HAND     , imageDB->Image.ACTWorld ); pSprite->SetPaperdollColor( PAPERDOLL_PART_NAME_HAND    , color[0], color[1] ); break;   //手套
		case EM_EQWearPos_Shoes      : pSprite->SetPaperdollPart( PAPERDOLL_PART_NAME_FOOT     , imageDB->Image.ACTWorld ); pSprite->SetPaperdollColor( PAPERDOLL_PART_NAME_FOOT    , color[0], color[1] ); break;   //鞋子
		case EM_EQWearPos_Clothes    : pSprite->SetPaperdollPart( PAPERDOLL_PART_NAME_TORSO    , imageDB->Image.ACTWorld ); pSprite->SetPaperdollColor( PAPERDOLL_PART_NAME_TORSO   , color[0], color[1] ); break;   //上衣
		case EM_EQWearPos_Pants      : pSprite->SetPaperdollPart( PAPERDOLL_PART_NAME_LEG      , imageDB->Image.ACTWorld ); pSprite->SetPaperdollColor( PAPERDOLL_PART_NAME_LEG     , color[0], color[1] ); break;   //褲子
		case EM_EQWearPos_Back       : pSprite->SetPaperdollPart( PAPERDOLL_PART_NAME_BACK     , imageDB->Image.ACTWorld ); pSprite->SetPaperdollColor( PAPERDOLL_PART_NAME_BACK    , color[0], color[1] ); break;   //背部
		case EM_EQWearPos_Belt       : pSprite->SetPaperdollPart( PAPERDOLL_PART_NAME_BELT     , imageDB->Image.ACTWorld ); pSprite->SetPaperdollColor( PAPERDOLL_PART_NAME_BELT    , color[0], color[1] ); break;   //腰帶
		case EM_EQWearPos_Shoulder   : pSprite->SetPaperdollPart( PAPERDOLL_PART_NAME_SHOULDER , imageDB->Image.ACTWorld ); pSprite->SetPaperdollColor( PAPERDOLL_PART_NAME_SHOULDER, color[0], color[1] ); break;   //肩甲
		case EM_EQWearPos_Ornament   :
			{
				if( imageDB->Image.ImageType == EM_GameObjectImageType_Wing )	// 翅膀
				{
					const char* insigniaType = NULL;
					const char* bannerType = NULL;
					int insigniaColor[2] = { 0, 0 };
					int bannerColor[2] = { 0, 0 };
					pSprite->CreateOrnamentEntity( rItemField.ImageObjectID, insigniaType, insigniaColor[0], insigniaColor[1], bannerType, bannerColor[0], bannerColor[1], true );
				}
			}
		
		//case EM_EQWearPos_MainHand   : pSprite->SetHandsObjectId( eRoleHands_MainHand , pEqItemDB->Item.OrgObjID ); break; 
		//case EM_EQWearPos_SecondHand : pSprite->SetHandsObjectId( eRoleHands_OffHand  , pEqItemDB->Item.OrgObjID ); break; 
		default:
			//assert( FALSE );
			break;
		}
	}

	pSprite->PaperdollBuild();

}


// ----------------------------------------------------------------------------
string CHousesFrame::GetServantName( HouseServantStruct& rServant )
{
	string Name = rServant.Name.Begin();

	if( Name.length() == 0 )
	{
		srand( (unsigned)( rServant.CreateTime / 100 ) );

		int _random = 0;

		int num = rServant.CreateTime % 100;

		do 
		{
			_random = rand();
			--num;
		} while ( num >= 0 );

		//SYS_SURNAME_00
		//SYS_MALE_NAME_00
		//SYS_FEMALE_NAME_00
 
		if( rServant.Sex == 0 )
		{
			int iSurname = _random % 50;
			int iName = ( _random / 100 ) % 50;

			const char* szSurname = g_ObjectData->GetString( Com_FormatString( "SYS_SURNAME_%02d", iSurname ) );
			const char* szName = g_ObjectData->GetString( Com_FormatString( "SYS_MALE_NAME_%02d", iName ) );

			string strTemp = g_ObjectData->GetString( "SYS_HOUSES_SERVANT_NAME_FORMAT" );
			strTemp = ReplaceKeyString( strTemp, "<name>", szName );
 			strTemp = ReplaceKeyString( strTemp, "<surname>", szSurname );

			Name = strTemp;
		}
		else if( rServant.Sex == 1 )
		{
			int iSurname = _random % 50;
			int iName = ( _random / 100 ) % 50;

			const char* szSurname = g_ObjectData->GetString( Com_FormatString( "SYS_SURNAME_%02d", iSurname ) );
			const char* szName = g_ObjectData->GetString( Com_FormatString( "SYS_FEMALE_NAME_%02d", iName ) );

			string strTemp = g_ObjectData->GetString( "SYS_HOUSES_SERVANT_NAME_FORMAT" );
			strTemp = ReplaceKeyString( strTemp, "<name>", szName );
			strTemp = ReplaceKeyString( strTemp, "<surname>", szSurname );

			Name = strTemp;
		}
		else
		{
			Name = Com_FormatString( "%d", _random );
		}
	}

	return Name;
}

// ----------------------------------------------------------------------------
void CHousesFrame::GetServantBirthday( HouseServantStruct& rServant, int& outMonth, int& outDay, string& outHoroscope )
{
	srand( (unsigned)( rServant.CreateTime / 100 ) );

	int _random = 0;

	int num = rServant.CreateTime % 100;

	do 
	{
		_random = rand();
		--num;
	} while ( num >= 0 );

	int Month = ( ( _random / 1000 ) % 12 ) + 1;

	int dayMax = 31;
	if( Month == 2 )
	{
		dayMax = 28;
	}
	else if( Month % 2 == 0 )
	{
		dayMax = 30;
	}

	int Day = _random % dayMax + 1;

	//水瓶:AQUARIUS     1/21 2/18                       
	//雙魚:PISCES       2/19 3/20                     		                             
	//牡羊:ARIES        3/21 4/20                    
	//金牛:TAURUS       4/21 5/21                    
	//雙子:GENINI       5/22 6/21                     
	//巨蟹:CANCER       6/22 7/22                     
	//獅子:LEO          7/23 8/22                 
	//處女:VIRGO        8/23 9/23                   
	//天秤:LIBRA        9/24 10/23                   
	//天蠍:SCORPIO      10/24 11/22                     
	//射手:SAGITTARIUS  11/23 12/21                          
	//山羊:CAPRICORN    12/22 1/20  

	int birthday = Month * 100 + Day;
  
	if     ( birthday >= 1222 ){ outHoroscope = "CAPRICORN"   ; }
	else if( birthday >= 1123 ){ outHoroscope = "SAGITTARIUS" ; }
	else if( birthday >= 1024 ){ outHoroscope = "SCORPIO"     ; }
	else if( birthday >=  924 ){ outHoroscope = "LIBRA"       ; }
	else if( birthday >=  823 ){ outHoroscope = "VIRGO"       ; }
	else if( birthday >=  723 ){ outHoroscope = "LEO"         ; }
	else if( birthday >=  622 ){ outHoroscope = "CANCER"      ; }
	else if( birthday >=  522 ){ outHoroscope = "GENINI"      ; }
	else if( birthday >=  421 ){ outHoroscope = "TAURUS"      ; }
	else if( birthday >=  321 ){ outHoroscope = "ARIES"       ; }
	else if( birthday >=  219 ){ outHoroscope = "PISCES"      ; }
	else if( birthday >=  121 ){ outHoroscope = "AQUARIUS"    ; }
	else                       { outHoroscope = "CAPRICORN"   ; }

	outMonth = Month;
	outDay = Day;
}

// ----------------------------------------------------------------------------
Race_ENUM CHousesFrame::GetServantRace( HouseServantStruct& rServant ) // 種族
{
	GameObjDbStructEx* NPCObjDB = g_ObjectData->GetObj( rServant.NPCObjID );
	if( NULL == NPCObjDB )
	{
		return EM_Race_None;
	}
	GameObjDbStructEx* imageDB = CNetGlobal::GetObj( NPCObjDB->ImageID );
	if( NULL == imageDB )
	{
		return EM_Race_None;
	}
	//model->CreateEntity( imageDB->Image.ACTWorld );
	const char* szFind = strstr( imageDB->Image.ACTWorld, "human" );
	if( szFind )
	{
		return EM_Race1;
	}	 

	szFind = strstr( imageDB->Image.ACTWorld, "elf" );
	if( szFind )
	{
		return EM_Race2;
	}

	return EM_Race_None;
}


// ----------------------------------------------------------------------------
void CHousesFrame::HousesControl_HouseItemMouseEnter( )
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
	m_MouseEnterFurnishingID = -1;	

	float collisionT = 100000.0f;
	CRoleSprite* pRoleSprite = NULL;

	TD_HouseItemMap& rHouseItemMap = GetHouseFurnishingList();
	TD_HouseItemMap::iterator itPos = rHouseItemMap.begin();
	TD_HouseItemMap::iterator itEnd = rHouseItemMap.end();
	for( ; itPos != itEnd ; ++itPos )
	{

		if( itPos->second->Layout.IsShow == false ||
			itPos->second->Pos >= m_HouseBaseInfo.MaxItemCount )
		{
			continue;
		}

		pRoleSprite = GetHouseRoleSpriteByItemDBID( itPos->first );

		//assert( pRoleSprite );
		if( NULL == pRoleSprite || NULL == pRoleSprite->GetRuEntity() )
		{
			continue;
		}
		pRoleSprite->GetRuEntity()->QueryCollision(&query);
		if( query.m_positiveHit )
		{
			if( query.m_collisionT < collisionT )
			{
				m_MouseEnterFurnishingID = itPos->first;				
				collisionT = query.m_collisionT;
			}
		}
	}

//	int m_FocusServantID;      //焦點的傢俱 DBID
//	int m_MouseEnterServantID; //滑鼠指到的HouseItem DBID

	m_MouseEnterServantDBID = -1;

	int index;
	{
		TD_HouseItemMap& rHouseItemMap = GetHouseContainer( DF_HouseServantListID );
		TD_HouseItemMap::iterator itPos = rHouseItemMap.begin();
		TD_HouseItemMap::iterator itEnd = rHouseItemMap.end();
		for( ; itPos != itEnd ; ++itPos )
		{
			index = itPos->second->Pos;

			if( itPos->second->Layout.IsShow == false )
			{
				continue;
			}

			if( index < 0 || index >= MAX_SERVANT_COUNT )
			{
				continue;
			}

			if( m_HouseBaseInfo.Servant[index].NPCObjID == 0 )
			{
				continue;
			}

			pRoleSprite = GetHouseRoleSpriteByItemDBID( itPos->first );

			//assert( pRoleSprite );
			if( NULL == pRoleSprite || NULL == pRoleSprite->GetRuEntity() )
			{
				continue;
			}
			pRoleSprite->GetRuEntity()->QueryCollision(&query);
			if( query.m_positiveHit )
			{
				if( query.m_collisionT < collisionT )
				{
					m_MouseEnterFurnishingID = -1;
					m_MouseEnterServantDBID = itPos->first;				
					collisionT = query.m_collisionT;
				}
			}
		}
	}

	//判斷女僕
	pRoleSprite = g_pGameMain->FindSpriteByWorldGUID( m_MaidRoleSpriteID );
	//assert( pRoleSprite );
	if( NULL == pRoleSprite || NULL == pRoleSprite->GetRuEntity())
	{
		return;
	}
	pRoleSprite->GetRuEntity()->QueryCollision(&query);
	if( query.m_positiveHit && query.m_collisionT < collisionT )
	{
		m_MouseEnterFurnishingID = -1;
		m_MouseEnterServantDBID = -1;
		m_bMouseEnterMaid  = true;
	}
	else
	{
		m_bMouseEnterMaid = false;
	}

	UpdateHighlightSprite();
}

// ----------------------------------------------------------------------------
void CHousesFrame::HousesControl_HouseItemMove( )
{
	static CRuVector3 g_StandardNormals[6] = { 
		CRuVector3(  1.0 ,  0.0 ,  0.0 ), 
		CRuVector3( -1.0 ,  0.0 ,  0.0 ),
		CRuVector3(  0.0 ,  1.0 ,  0.0 ), 
		CRuVector3(  0.0 , -1.0 ,  0.0 ),
		CRuVector3(  0.0 ,  0.0 ,  1.0 ), 
		CRuVector3(  0.0 ,  0.0 , -1.0 ),
	};

	HouseItemDBStruct* pFurnishingDB = GetHousesItemDBByDBID( m_FocusFurnishingID );
	CRoleSprite* pRoleSpriteFurnishing = GetHouseRoleSpriteByItemDBID( m_FocusFurnishingID );
	CRoleSprite* pRoleSpriteHouse = g_pGameMain->FindSpriteByWorldGUID( m_HouseRoleSpriteID );

	//assert( pFurnishingDB && pRoleSpriteFurnishing && pRoleSpriteHouse );
	if( !( pFurnishingDB && pRoleSpriteFurnishing && pRoleSpriteHouse && pRoleSpriteFurnishing->IsVisible() ) )
	{
		m_ePlaceingMode = m_ePlaceingModeTemp;
		return;
	}

	pRoleSpriteFurnishing->EnableCollision( false );

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

	// 測試碰撞
	pRoleSpriteHouse->GetRuEntity()->QueryCollision(&query);


	// 檢測回應
	if( query.m_positiveHit )
	{
		//query.m_collisionT;
		//query.m_collisionPoint;
		//query.m_collisionNormal;

		CRoleSprite* pSprite = NULL;

		TD_HouseItemMap &rHouseItemMap = GetHouseFurnishingList();
		TD_HouseItemMap::iterator itPos = rHouseItemMap.begin();
		TD_HouseItemMap::iterator itEnd = rHouseItemMap.end();
		for( ; itPos != itEnd ; ++itPos )
		{
			if( itPos->second->Layout.IsShow == false ||
				itPos->second->Pos >= m_HouseBaseInfo.MaxItemCount )
			{
				continue;
			}
			if( itPos->first == m_FocusFurnishingID )
			{
				continue;
			}
			pSprite = GetHouseRoleSpriteByItemDBID( itPos->first );
			//assert( pSprite );
			if( NULL == pSprite || NULL == pSprite->GetRuEntity())
			{
				continue;
			}
			pSprite->GetRuEntity()->QueryCollision(&query);

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
		if( query.m_collisionPoint.Magnitude() < 20 )
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

		pRoleSpriteFurnishing->SetPos( query.m_collisionPoint );
		pRoleSpriteFurnishing->SetOrientingAxis( query.m_collisionNormal );
		pRoleSpriteFurnishing->UpdateOrientation();
		pRoleSpriteFurnishing->Update(0);

		pFurnishingDB->Layout.X = query.m_collisionPoint.x;
		pFurnishingDB->Layout.Y = query.m_collisionPoint.y;
		pFurnishingDB->Layout.Z = query.m_collisionPoint.z;

		pFurnishingDB->Layout.UpX = query.m_collisionNormal.x;
		pFurnishingDB->Layout.UpY = query.m_collisionNormal.y;
		pFurnishingDB->Layout.UpZ = query.m_collisionNormal.z;

		m_bFurnishingChange = true;

	}

	
}

// ----------------------------------------------------------------------------
void CHousesFrame::HousesControl_HouseItemRorare( )
{

	HouseItemDBStruct* pFurnishingDB = GetHousesItemDBByDBID( m_FocusFurnishingID );
	CRoleSprite* pRoleSpriteFurnishing = GetHouseRoleSpriteByItemDBID( m_FocusFurnishingID );
	CRoleSprite* pRoleSpriteHouse = g_pGameMain->FindSpriteByWorldGUID( m_HouseRoleSpriteID );

	//assert( pFurnishingDB && pRoleSpriteFurnishing && pRoleSpriteHouse );
	if( !( pFurnishingDB && pRoleSpriteFurnishing && pRoleSpriteHouse && pRoleSpriteFurnishing->IsVisible() ) )
	{
		m_ePlaceingMode = m_ePlaceingModeTemp;
		return;
	}

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
	CRuVector3 spritePosition = pRoleSpriteFurnishing->GetPos();
	CRuVector3 spriteNormal = pRoleSpriteFurnishing->GetOrientingAxis();

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
			pRoleSpriteFurnishing->SetDirection( direction , false, true);


			//LOOP 會造成不能旋轉
			if( pRoleSpriteFurnishing->GetActorState() == ruFUSION_ACTORSTATE_ACTIVATE_LOOP )
			{
				pRoleSpriteFurnishing->SetActorState( ruFUSION_ACTORSTATE_ACTIVATE_END );
			}


			//
			pFurnishingDB->Layout.Angle = forwardAngleDiff;

			m_bFurnishingChange = true;

		}
	}
}


// ----------------------------------------------------------------------------
void CHousesFrame::HousesControl_OnMouseDown( CUiFrame* uiFrame, INT64 lParam, INT64 rParam )
{
	if( m_PlaceFurnishingMode && m_MouseEnterFurnishingID != -1 && lParam == MOUSE_LBUTTON )
	{

	}
	else
	{
		g_pGameControl->OnMouseDown( uiFrame, lParam, rParam );
	}
}

// ----------------------------------------------------------------------------
void CHousesFrame::HousesControl_OnMouseUp  ( CUiFrame* uiFrame, INT64 lParam, INT64 rParam )
{
	g_pGameControl->OnMouseUp( uiFrame, lParam, rParam );
}

// ----------------------------------------------------------------------------
void CHousesFrame::HousesControl_OnMouseWheel ( CUiFrame* uiFrame, INT64 lParam, INT64 rParam )
{
	g_pGameControl->OnMouseWheel( uiFrame, lParam, rParam );
}

// ----------------------------------------------------------------------------
void CHousesFrame::HousesControl_OnMouseMove(CUiFrame* uiFrame, INT64 lParam, INT64 rParam)
{
	if( m_PlaceFurnishingMode )
	{
		if( m_ePlaceingMode == EM_PlaceingMode_New ) //放置模式
		{
			if( m_PlaceStep == 0 )
			{
				HousesControl_HouseItemMove( );
			}
			else if( m_PlaceStep == 1 )
			{
				HousesControl_HouseItemRorare( );
			}
		}
		else if( m_ePlaceingMode == EM_PlaceingMode_Move )
		{
			if( m_PlaceStep == 0 )
			{
				HousesControl_HouseItemMouseEnter( );
			}
			else if( m_PlaceStep == 1 )
			{
				HousesControl_HouseItemMove( );
			}
		}
		else if( m_ePlaceingMode == EM_PlaceingMode_Rorare )
		{
			if( m_PlaceStep == 0 )
			{
				HousesControl_HouseItemMouseEnter( );
			}
			else if( m_PlaceStep == 1 )
			{
				HousesControl_HouseItemRorare( );
			}
		}
		else
		{
			HousesControl_HouseItemMouseEnter( );
		}
	}
	else
	{
		HousesControl_HouseItemMouseEnter( );
	}


}

// ----------------------------------------------------------------------------
void CHousesFrame::HousesControl_OnMouseEnter( CUiFrame* uiFrame, INT64 lParam, INT64 rParam )
{
	g_pGameControl->OnMouseEnter( uiFrame, lParam, rParam );
}

// ----------------------------------------------------------------------------
void CHousesFrame::HousesControl_OnMouseLeave( CUiFrame* uiFrame, INT64 lParam, INT64 rParam )
{
	g_pGameControl->OnMouseLeave( uiFrame, lParam, rParam );
	m_MouseEnterFurnishingID = -1;
}

// ----------------------------------------------------------------------------
void CHousesFrame::HousesControl_OnClick(CUiFrame* uiFrame, INT64 lParam, INT64 rParam)
{

	//if( lParam == MOUSE_RBUTTON )
	//{
	//	float mouseOffsetX   = g_pGameControl->GetMouseOffsetX();
	//	float mouseOffsetY   = g_pGameControl->GetMouseOffsetY();
	//	float buttonDownTime = g_pGameControl->GetButtonDownTime();

	//	float frameTime = g_pGameMain->GetFrameTime();
	//	float offset	= (float)sqrt((mouseOffsetX*mouseOffsetX)+(mouseOffsetY*mouseOffsetY));

	//	if ( frameTime - buttonDownTime > 0.6f || offset > 10.0f )
	//		return;	

	//	//女僕
	//	//判斷女僕
	//	CRoleSprite* pRoleSpriteMaid = g_pGameMain->FindSpriteByWorldGUID( m_MaidRoleSpriteID );
	//	assert( pRoleSpriteMaid );
	//	if( NULL == pRoleSpriteMaid )
	//	{
	//		return;
	//	}
 //  
	//	//if( g_pGameMain->GetMouseInPlayer() == pRoleSpriteMaid &&
	//	//	g_pGameMain->GetDistanceWithPlayer( pRoleSpriteMaid ) <= _MAX_ROLE_TOUCH_RANGE_ )
	//	if( g_pGameMain->GetMouseInPlayer() == pRoleSpriteMaid )
	//	{
	//		SetFocusFurnishingID( -1 );
	//		HousesControl_DoMaid();
	//		return;
	//	}
	//	//HousesControl_UseFurniture( m_MouseEnterFurnishingID );
	//	SetFocusFurnishingID( m_MouseEnterFurnishingID );
	//}


	if( m_PlaceFurnishingMode && ( 
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
		if( m_PlaceFurnishingMode )
		{
			if( m_ePlaceingMode == EM_PlaceingMode_New ) //放置模式
			{
				m_PlaceStep++;

				if( m_PlaceStep == 2 )
				{
					m_ePlaceingMode = m_ePlaceingModeTemp;
					m_PlaceStep = 0;
					HouseItemLayoutChange();
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

		if( m_MouseEnterFurnishingID != -1 )
		{
			SetFocusFurnishingID( m_MouseEnterFurnishingID );
			return;
		}
		//每次發生點擊 就判斷有沒有改變
		HouseItemLayoutChange();
	}

}

// ----------------------------------------------------------------------------
void CHousesFrame::HousesControl_OnDoubleClick( CUiFrame* uiFrame, INT64 lParam, INT64 rParam )
{
	g_pGameControl->OnDoubleClick( uiFrame, lParam, rParam );
	//HousesControl_UseFurniture( m_MouseEnterFurnishingID );
	SetFocusFurnishingID( m_MouseEnterFurnishingID );
}

// ----------------------------------------------------------------------------
void CHousesFrame::HousesControl_OnDragBegin( CUiFrame* uiFrame, INT64 lParam, INT64 rParam )
{
	if( m_PlaceFurnishingMode )
	{
		if( m_ePlaceingMode == EM_PlaceingMode_Move ||
			m_ePlaceingMode == EM_PlaceingMode_Rorare )
		{
			if( m_MouseEnterFurnishingID != -1 )
			{
				CRoleSprite* pRoleSprite = GetHouseRoleSpriteByItemDBID( m_MouseEnterFurnishingID );
				if( !pRoleSprite )
					return ;
				CRuVector3 vecPos = pRoleSprite->GetPos();
				g_pGameMain->WorldToScreen(vecPos);
				g_pGameMain->SetCursorPosition( (int)vecPos.x, (int)vecPos.y );
				SetFocusFurnishingID( m_MouseEnterFurnishingID );
				m_PlaceStep = 1;
			}
		}
	}

}

// ----------------------------------------------------------------------------
void CHousesFrame::HousesControl_OnDragStop ( CUiFrame* uiFrame, INT64 lParam, INT64 rParam )
{
	if( m_PlaceFurnishingMode )
	{
		if( m_ePlaceingMode == EM_PlaceingMode_Move ||
			m_ePlaceingMode == EM_PlaceingMode_Rorare )
		{
			m_PlaceStep = 0;
		}
	}
}

// ----------------------------------------------------------------------------
void CHousesFrame::HousesControl_OnReceiveDrag( CUiFrame* uiFrame, INT64 lParam, INT64 rParam )
{
	g_pGameControl->OnReceiveDrag( uiFrame, lParam, rParam );
}

// ----------------------------------------------------------------------------

#ifdef KALYDO
void CHousesFrame::KalydoLoadHouseCallback(const char* fileName, TKResult result, void* id)
{
	if (KRF_FAILURE(result))
	{
		// don't try again; this is done in the resource manager.
		//krfRequestKCPFile(fileName, &KalydoLoadHouseCallback, id);
		return;
	}

	// files are downloaded; can continue showing the house
	CHousesFrame* hf = reinterpret_cast<CHousesFrame*>(id);
	hf->ActuallyLoadHouse(fileName);
}
#endif // KALYDO

void CHousesFrame::LoadRoleSpriteHouse()
{
	GameObjDbStructEx* pDateDB = g_ObjectData->GetObj( m_HouseBaseInfo.HouseType );

	if( !pDateDB || !pDateDB->IsImage() )
	{
		return;
	}

	//const char* actName = Com_FormatString( "model\\element\\house_system\\building\\housetype_%02d_%02d.ros" , m_HouseBaseInfo.HouseType / 100 , m_HouseBaseInfo.HouseType % 100 );
	const char* actName = pDateDB->Image.ACTWorld;

#ifdef KALYDO
	// download the house and its dependencies first
	TKResult res = g_ruResourceManager->DownloadEntityAndDependencies(actName, &KalydoLoadHouseCallback, this);
#else
	// moved the code to separate function to be reused in kalydo version
	ActuallyLoadHouse(actName);
#endif // KALYDO
}

void CHousesFrame::ActuallyLoadHouse(const char* actName)
{
	int GItemID   = m_HouseRoleSpriteID;   
	int WorldGUID = m_HouseRoleSpriteID; 

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
	pSprite->LoadRuEntity( actName );

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
	pSprite->EnableCollision(true);

	pSprite->SetCanAttackState(false);
	pSprite->SetEnemyState(false);


	m_bLoadedRoleSpriteHouse = true;

}


// ----------------------------------------------------------------------------
void CHousesFrame::UpdateAlllPlantSprite()
{
	TD_HouseItemMap &rHouseItemMap = GetHouseFurnishingList();

	HouseItemDBStruct* pHouseItemDB;
	GameObjDbStructEx* pItemDB;

	TD_HouseItemMap::iterator itPos = rHouseItemMap.begin();
	TD_HouseItemMap::iterator itEnd = rHouseItemMap.end();
	for( ; itPos != itEnd ; ++itPos )
	{
		pHouseItemDB = itPos->second;
		pItemDB = CNetGlobal::GetObj( pHouseItemDB->Item.OrgObjID );
		if( pItemDB && pItemDB->Item.Plant.Pot.Mode != 0 )
		{
			LoadRoleSpriteHouseItem( pHouseItemDB );
		}
	}
}


// ----------------------------------------------------------------------------
float Func_RandomNumber( float f1  , float f2 )
{
	if (f1 == f2) return(f1);
	float fRandom = ((float)rand()) / (float)RAND_MAX;
	return ( fRandom * (f2-f1) ) + f1;
}

// ----------------------------------------------------------------------------
// GrowRate 生長	百分比
// state	fade normal bloom  狀態
// 稀有		分子
// 照顧		fade normal bloom
// SeedID   亂數種子
// tree
// branch
// leaf 
// flower
// 亂數樹

//	tree branch leaf flower
//1	0.3	 0		0	 0		  0
//2	0.4	 1		0	 0		  2.5
//3	0.5	 1		1	 0		  5
//4	0.7	 2		1	 1		  7.5
//5	1.0	 3		1	 1		  1
//
//
//

//float			EvnPoint[2];
//unsigned char	GrowRate;

//std::vector<int> g_TreeLayer;


bool CHousesFrame::UpdateHousePlant( SHousePlant& rPlant, ItemFieldStruct& rItem )
{
	
	GameObjDbStructEx* itemDB = CNetGlobal::GetObj( rItem.OrgObjID );
	if( NULL == itemDB )
	{
		return false;
	}

	//是不是盆栽
	if( itemDB->Item.Plant.Pot.Mode == 0 )
	{
		return false;
	}

	rPlant.Item = rItem;
	rPlant.strName = itemDB->GetName();

	//盆栽屬性	養分每小時下降倍率 越小越好

	rPlant.nType = 0;
	if( itemDB->Item.Plant.Pot.EvnPoint[0][0] == itemDB->Item.Plant.Pot.EvnPoint[1][0] &&
		itemDB->Item.Plant.Pot.EvnPoint[0][0] == itemDB->Item.Plant.Pot.EvnPoint[2][0] )
	{
		rPlant.nType = -1; //通用類型
	}
	else
	{
		float bestPoint = itemDB->Item.Plant.Pot.EvnPoint[ rPlant.nType ][0];
		int i;
		for( i = 1 ; i < 3 ; ++i )
		{
			if( itemDB->Item.Plant.Pot.EvnPoint[i][0] < bestPoint )
			{
				bestPoint = itemDB->Item.Plant.Pot.EvnPoint[i][0];
				rPlant.nType = i;
			}
		}
	}


	GameObjDbStructEx* itemDBSeed = NULL;

	if( rItem.Plant.SeedID > 0 )
	{
		itemDBSeed = CNetGlobal::GetObj( rItem.Plant.SeedID + Def_ObjectClass_Item );
	}

	if( NULL == itemDBSeed )
	{
		rPlant.eState = HPS_None;
		return true;
	}

	rPlant.nPlantID = itemDBSeed->Item.Plant.Seed.ModelType;
	//rPlant.strName = itemDBSeed->Name;

	rPlant.nCreateTime = rItem.Plant.CreateTime;

	rPlant.nFeedPoint = rItem.Plant.FeedPoint / 100;
	
	rPlant.nWaterPoint        = rItem.Plant.WaterPoint    / 100 ;    
	rPlant.nTopdressPoint     = rItem.Plant.TopdressPoint / 100 ;  
	rPlant.nBestWaterPoint    = itemDBSeed->Item.Plant.Seed.BestWater    ;
	rPlant.nBestTopdressPoint = itemDBSeed->Item.Plant.Seed.BestTopdress ;


	rPlant.fDecWater    = ( (float)rItem.Plant.DecWaterPoint    ) / 100 + itemDB->Item.Plant.DecWater    + itemDBSeed->Item.Plant.DecWater    ;
	rPlant.fDecTopdress = ( (float)rItem.Plant.DecTopdressPoint ) / 100 + itemDB->Item.Plant.DecTopdress + itemDBSeed->Item.Plant.DecTopdress ;

	int Seedtype = itemDBSeed->Item.Plant.Seed.Type;
	Seedtype = Seedtype - 1;
	if( Seedtype < 0 ) Seedtype = 0;
	if( Seedtype > 2 ) Seedtype = 2;
	float fEvnPoint = itemDB->Item.Plant.Pot.EvnPoint[ Seedtype ][0];

	rPlant.fDecWater    *= fEvnPoint;
	rPlant.fDecTopdress *= fEvnPoint;


	//有種子
  	double fGrowSpeed = itemDBSeed->Item.Plant.Seed.GrowSpeed;
	fGrowSpeed = fGrowSpeed / 100;
	if( fGrowSpeed < 0.000001 )
	{
		fGrowSpeed= 0.000001;
	}
  
	//GrowRate = Item.Plant.GrowRate;  //成長度

	double totalTime = ( 1 / fGrowSpeed ) * 60 * 60;   //長大需要的秒數
	//double currentTime = (double) ( max( ( g_pGameMain->GetGameTime() - rItem.Plant.CreateTime ), 0 ) );
	//double realGrowRate = currentTime / totalTime; //目前的成長率
	double realGrowRate = (double)rItem.Plant.GrowRate / 10000;
   
	if( realGrowRate > 1 )
	{
		realGrowRate = 1;
	}

	if( realGrowRate < 0.00001 )
	{
		realGrowRate = 0.00001;
	}

	rPlant.fGrowRate = (float)realGrowRate;
	rPlant.nReapTime = (int)( totalTime * ( 1 - realGrowRate ) );

//	short HealthPoint;		//健康度 (100倍)
//	short RarePoint;		//稀有度 (100倍)

	rPlant.fHealthRate = (float)((double)rItem.Plant.HealthPoint / 10000);
	rPlant.fRareRate = (float)((double)rItem.Plant.RarePoint / 10000);

	if( rPlant.fHealthRate > 1.0f ) rPlant.fHealthRate = 1.0f;
	if( rPlant.fRareRate   > 1.0f ) rPlant.fRareRate   = 1.0f;

	if( rPlant.fHealthRate < 0.0f ) rPlant.fHealthRate = 0.0f;
	if( rPlant.fRareRate   < 0.0f ) rPlant.fRareRate   = 0.0f;

   	rPlant.isLock = rItem.Plant.IsLock;
  	
	if( rItem.Plant.IsDead )
	{
 		rPlant.eState = HPS_Death;
	}
	else if( rItem.Plant.GrowRate >= 10000 )
	{
		rPlant.eState = HPS_Ripe;
	}
	else 
	{
		rPlant.eState = HPS_Grow;
	}

	return true;
}

struct SGrowRate
{
	SGrowRate():
	fScale( 0 ), nBranch( 0 ),	bLeaf( false ), bFlower( false )
	{

	}

	SGrowRate( float _fScale, int _nBranch, bool _bLeaf, bool _bFlower ):
	fScale( _fScale ), nBranch( _nBranch ),	bLeaf( _bLeaf ), bFlower( _bFlower )
	{
		
	}

	float fScale; 
	int   nBranch;   //有幾層樹枝
	bool  bLeaf;	 //有沒有樹葉
	bool  bFlower;	 //有沒有花
};

SGrowRate g_sGrowRateTable[5] = 
{   
	SGrowRate( 0.2f, 0,  true, false ),
	SGrowRate( 0.4f, 1,  true, false ),
	SGrowRate( 0.6f, 2,  true, false ),
	SGrowRate( 0.8f, 2,  true, true  ),
	SGrowRate( 1.0f, 3,  true, true  ),
};


// ----------------------------------------------------------------------------
void CHousesFrame::Houses_CreatePlantNode( CRuEntity* pParent, SHousePlant& rPlant, SHousePlantNode& rNode )
{
	static int s_OpenCount = 0; //開了幾朵
	static int s_FlowerCount = 0;  //最多幾朵


	int plantID = rPlant.nPlantID ;
	float fGrowRate   = rPlant.fGrowRate;    //
	float fHealthRate = rPlant.fHealthRate;  // <50 枯委 ,  >50 一般 ,  >90 分子
	float fRareRate   = rPlant.fRareRate;	 //開花數量

	if( rPlant.eState == HPS_Death )
	{
		fHealthRate = 0;
		fRareRate   = 0;
	}

	if( fGrowRate < 0 )
	{
		fGrowRate = 0;	
	}
	
	SGrowRate sGrowRate;

	if( fGrowRate < 0.25f )
	{
		sGrowRate = g_sGrowRateTable[0];
		sGrowRate.fScale = g_sGrowRateTable[0].fScale + ( ( g_sGrowRateTable[1].fScale - g_sGrowRateTable[0].fScale ) * fGrowRate / 0.25f );
	}
	else if( fGrowRate < 0.5f )
	{
		sGrowRate = g_sGrowRateTable[1];
		sGrowRate.fScale = g_sGrowRateTable[1].fScale + ( ( g_sGrowRateTable[2].fScale - g_sGrowRateTable[1].fScale ) * ( fGrowRate - 0.25f ) / 0.25f );
	}
	else if( fGrowRate < 0.75f )
	{
		sGrowRate = g_sGrowRateTable[2];
		sGrowRate.fScale = g_sGrowRateTable[2].fScale + ( ( g_sGrowRateTable[3].fScale - g_sGrowRateTable[2].fScale ) * ( fGrowRate - 0.5f ) / 0.25f );
	}
	else if( fGrowRate < 1.0f )
	{
		sGrowRate = g_sGrowRateTable[3];
		sGrowRate.fScale = g_sGrowRateTable[3].fScale + ( ( g_sGrowRateTable[4].fScale - g_sGrowRateTable[3].fScale ) * ( fGrowRate - 0.75f ) / 0.25f );
	}
	else
	{
		sGrowRate = g_sGrowRateTable[4];
	}

	//小時候 要可愛一點
//	if( fGrowRate < 0.25f )
//	{
//		fCareRate = 1;
//	}
	   
	char actName[256];

	//const char* actName;

  	srand( (unsigned)rNode.nRandom );
   
	if( rNode.eType == HPT_TREE )
	{
		s_OpenCount = 0; //開了幾朵
		s_FlowerCount = 0;  //最多幾朵

		string careToken;

		if( fHealthRate < 0.5 )
			careToken = "fade";
		else
			careToken = "normal";

		int random = ( rand() % 2 )  + 1;
		sprintf( actName, "model\\element\\house_system\\prop\\plant_grow\\planting%02d_tree_%s_%02d.ros", plantID, careToken.c_str(), random );
 	}
	else if( rNode.eType == HPT_BRANCH )
	{
		string careToken;

		if( fHealthRate < 0.5 )
			careToken = "fade";
		else
			careToken = "normal";

		int random = ( rand() % 2 )  + 1;
		sprintf( actName, "model\\element\\house_system\\prop\\plant_grow\\planting%02d_branch_%s_%02d.ros", plantID, careToken.c_str(), random );
  	}
	else if( rNode.eType == HPT_LEAF )
	{
		string careToken;

		if( fHealthRate < 0.5 )
			careToken = "fade";
		else
			careToken = "normal";

		int random = ( rand() % 2 )  + 1;
		sprintf( actName, "model\\element\\house_system\\prop\\plant_grow\\planting%02d_leaf_%s_%02d.ros", plantID, careToken.c_str(), random );
	}
	else if( rNode.eType == HPT_FLOWER || rNode.eType == HPT_FLOWERED )
	{
		string careToken;
		if( fHealthRate < 0.5 )
			careToken = "fade";
		else
			careToken = "normal";

		int random = ( rand() % 2 ) + 1;
		sprintf( actName, "model\\element\\house_system\\prop\\plant_grow\\planting%02d_flower_%s_%02d.ros", plantID, careToken.c_str(), random );
	}
	else if( rNode.eType == HPT_ROOTLEAF )
	{
		string careToken;

		if( fHealthRate < 0.5 )
			careToken = "fade";
		else
			careToken = "normal";

		int random = ( rand() % 2 ) + 1;
		sprintf( actName, "model\\element\\house_system\\prop\\plant_grow\\planting%02d_rootleaf_%s_%02d.ros", plantID, careToken.c_str(), random );
	}
	else
	{
		return;
	}

	//產生物件
	CRuEntity* pEntity = g_ruResourceManager->LoadEntity( actName );
	rNode.pEntity = pEntity;
	if( NULL == pEntity )
	{
		return;
	}

	int nBranchLayer = rNode.nBranchLayer;

	if( rNode.eType == HPT_TREE )
	{
		pEntity->Scale( sGrowRate.fScale );
	}
	
	if( nBranchLayer > 1 )
	{
 		//每層樹枝縮小
		static float scale = 0.75f; //0.7f;
		if( rNode.eType == HPT_BRANCH )
		{
			pEntity->Scale( scale );
		}
		else if( rNode.eType == HPT_FLOWER || rNode.eType == HPT_FLOWERED )
		{
			float temp = 1.0f;
			for( int n = 2 ; n < nBranchLayer ; ++n )
			{
				temp = temp / scale;
			}
 			pEntity->Scale( temp );
		}
	}
	
	RuEntity_Attach_BFS( pParent, pEntity, rNode.strLinkName.c_str() );

	//花要貼分子 act_planting01.ros
	if( ( rNode.eType == HPT_FLOWER || rNode.eType == HPT_FLOWERED ) && fHealthRate >= 0.9f )
	{
		sprintf( actName, "model\\element\\house_system\\prop\\plant_grow\\act_planting%02d.ros", plantID );
		CRuEntity* pEntityFX = g_ruResourceManager->LoadEntity( actName );
		if( pEntityFX )
		{
			RuACT_PlayMotionEx( pEntityFX, -1, "idle" );
   			RuEntity_Attach_BFS( pEntity, pEntityFX, "planting_fx_01" );
			pEntityFX->Update( 10.0f );
 			ruSAFE_RELEASE( pEntityFX );
		}
	}
	
	//不是樹枝
	if( rNode.eType != HPT_TREE && rNode.eType != HPT_BRANCH )
	{
		ruSAFE_RELEASE( pEntity );
		return;
	}
	
	nBranchLayer = nBranchLayer + 1;
	//static int layerMax = 5;
	//if( layerNumber >= layerMax )
	//{
	//	return ;
	//}

	static float rateBranch = 1.0f;
	static float rateLeaf = 1.0f;
	static float rateFlower = 0.5f;	//開花率
	
	//fGrowRate 超過 150% 凋謝
	if( rPlant.eState == HPS_Death )
	{
		rateFlower = 0;
	}
	else
	{
		//fRareRate 關係到花的數量
		rateFlower = (float)( 0.5 + 0.5 * ( min( fRareRate , 1.0 ) ) );
	}
	
	//產生葉子
	CRuArrayList<CRuLinkFrame *> linkFrames;
	RuEntity_EnumerateLinkFrames( pEntity, linkFrames );

	int i;
	//判斷是不是沒有下一層樹支 或 只有1花
	
	int branchNumber = 0;
	int flowerNumber = 0;
	int len;
	const char* linkName;
	//SHousePlant child;
	for( i = 0; i < linkFrames.Count(); ++i )
	{
 		SHousePlantNode child;
		child.nRandom = rand();	 //先吃掉亂數

		linkName = linkFrames[i]->GetLinkName();
		len = (int)strlen( linkName );
		if( len >= 6 && 0 == strncmp( linkName, "branch", 6 ) )
		{
			if( nBranchLayer > sGrowRate.nBranch )
			{
				continue;
			}
			child.eType = HPT_BRANCH;
			++branchNumber;			
		}
		else if( len >= 4 && 0 == strncmp( linkName, "leaf", 4 ) )
		{
			if( false == sGrowRate.bLeaf )
			{
				continue;
			}
			child.eType = HPT_LEAF;
		}
		else if( len >= 8 && 0 == strncmp( linkName, "flowered", 8 ) )
		{
			//if( false == sGrowRate.bFlower )
			//{
			//	continue;
			//}
			child.eType = HPT_FLOWERED;
			//++flowerNumber;
		}
		else if( len >= 6 && 0 == strncmp( linkName, "flower", 6 ) )
		{
			if( false == sGrowRate.bFlower )
			{
				continue;
			}
			child.eType = HPT_FLOWER;
			++flowerNumber;
		}
		else if( len >= 8 && 0 == strncmp( linkName, "rootleaf", 8 ) )
		{
			if( false == sGrowRate.bLeaf )
			{
				continue;
			}
			child.eType = HPT_ROOTLEAF;
		}
		else
		{
			continue;
		}

		child.strLinkName = linkName;
		child.nBranchLayer = nBranchLayer;
	
		rNode.Children.push_back( child );
	}
	
	bool OpenEnable = branchNumber == 0 ? true : false;	//可以開花
	bool OpenSure = flowerNumber == 1 ? true : false;	//百分百開

	int count = (int)rNode.Children.size();

	for( i = 0; i < count; ++i )
	{
		SHousePlantNode& child = rNode.Children[i];

		if( child.eType == HPT_BRANCH )
		{
			//if(  Func_RandomNumber(0.0f,1.0f) > rateBranch )
			if( ( ((float)child.nRandom) / (float)RAND_MAX ) > rateBranch )
			{
				continue;
			}
		}
		else if( child.eType == HPT_LEAF )
		{
			if( ( ((float)child.nRandom) / (float)RAND_MAX ) > rateLeaf )
			{
				continue;
			}
		}
		else if( child.eType == HPT_FLOWER )
		{
			if( OpenEnable || nBranchLayer >= sGrowRate.nBranch )
			//if( OpenEnable )
			{

			}
			else
			{
				continue ;
			}

			//只有一花　一樹枝　根樹枝同層的不開花
			if( nBranchLayer == sGrowRate.nBranch && branchNumber == 1 && flowerNumber == 1 )
			{
				continue ;
			}

			if( rateFlower == 0 )
			{
				continue ;
			}

			if( false == OpenSure )
			{

				//static int s_OpenCount = 0; //開了幾朵
				//static int s_FlowerCount = 0;  //最多幾朵

				float _rateFlower = rateFlower;

				++s_FlowerCount;

				//float nowrate = ((float)s_OpenCount) / ((float)s_FlowerCount);
				float perfect = ( rateFlower * (float)s_FlowerCount ); //理想數量

				//每+-一朵花 +-50%
				_rateFlower = _rateFlower + 0.60f * ( perfect - (float)s_OpenCount);

				//避免沒有花
				//if( s_OpenCount == 0 )
				//{
				//	_rateFlower += 0.1 * (float)s_FlowerCount;
				//}

				if( ( ((float)child.nRandom) / (float)RAND_MAX ) > _rateFlower )
				{
					continue;
				}

				++s_OpenCount;
  			}
		}
		else if( child.eType == HPT_FLOWERED )
		{

		}
		else if( child.eType == HPT_ROOTLEAF )
		{

		}
		else
		{
			continue;
		}
		
		Houses_CreatePlantNode( pEntity, rPlant, child );
	}

	ruSAFE_RELEASE( pEntity );
}

// ----------------------------------------------------------------------------
void CHousesFrame::Houses_CreatePlant( CRuEntity* pParent, ItemFieldStruct& rItem )
{
	SHousePlant sHousePlant;
	if( UpdateHousePlant( sHousePlant, rItem ) )
	{
		if( sHousePlant.nPlantID >= 0 )
		{
			sHousePlant.sTree.eType = HPT_TREE;
			sHousePlant.sTree.strLinkName = "p_plant";
			sHousePlant.sTree.nBranchLayer = 0;
			sHousePlant.sTree.nRandom = sHousePlant.nCreateTime;
			Houses_CreatePlantNode( pParent, sHousePlant, sHousePlant.sTree );
		}
	}	
}

// ----------------------------------------------------------------------------
void CHousesFrame::LoadRoleSpriteHouseItem( HouseItemDBStruct* HouseItemDB )
{
	//assert( HouseItemDB );
	if( NULL == HouseItemDB )
	{
		return;
	}

	if( false == m_bLoadedRoleSpriteHouse )
	{
 		return;
	}

	if( HouseItemDB->ParentItemDBID != DF_HouseFurnishingListID )
	{
		return;
	}

	GameObjDbStructEx* itemDB = CNetGlobal::GetObj( HouseItemDB->Item.OrgObjID );
	
	//
	CRoleSprite* pRoleSprite = GetHouseRoleSpriteByItemDBID( HouseItemDB->ItemDBID );

	//清除盆栽
	map<int,SHousePlant>::iterator itPos = m_MapHousePlant.find( HouseItemDB->ItemDBID );
	if( itPos != m_MapHousePlant.end() )
	{
		SHousePlant& rPlant = itPos->second;
		if( rPlant.sTree.pEntity && pRoleSprite )
		{
			rPlant.sTree.pEntity->DetachFromParent();
			//ruSAFE_RELEASE( rPlant.pEntity );
		}
		m_MapHousePlant.erase( itPos );
	}
   
 	
	//判斷是不是未開啟的空間
	//if( HouseItemDB->Pos >= m_HouseBaseInfo.MaxItemCount )
	//{
	//	HouseItemDB->Layout.IsShow = false;
	//}

	//沒有東西 或 不顯示  
	//判斷是不是未開啟的空間
	if( !itemDB ||
		false == HouseItemDB->Layout.IsShow ||
		HouseItemDB->Pos >= m_HouseBaseInfo.MaxItemCount )
	{

		if( pRoleSprite )  
		{
			pRoleSprite->SetVisible( false );
			pRoleSprite->EnableCollision(FALSE);
			pRoleSprite->SetCursorState( false ); 
		}

		return ;	
	}

	//int NewSpriteWorldGUID = m_FurnishingSpriteWorldGUIDUsed++; //這裡有++
	//m_FurnishingSpriteWorldGUIDMap[HouseItemDB->Pos] = NewSpriteWorldGUID;

	House3DLayoutStruct& rLayout = HouseItemDB->Layout;


	if( NULL == pRoleSprite )
	{
		int GItemID   = DF_HouseRoleSpriteWorldGUID + HouseItemDB->ItemDBID; 
		int WorldGUID = DF_HouseRoleSpriteWorldGUID + HouseItemDB->ItemDBID; 
		int DBID = -1;

		bool isNew;
		pRoleSprite = g_pGameMain->CreateSprite( GItemID, DBID , WorldGUID, isNew );
		//assert( isNew == true );
		//assert( pRoleSprite );
		if ( !pRoleSprite )
		{
			return;
		}
	}

	//CRoleSprite* pSprite = pRoleSprite;

	if( false == pRoleSprite->SetObjectId( HouseItemDB->Item.OrgObjID, false ) )
	{
		return ;
	}
	//pSprite->LoadRuEntity( actName );

	pRoleSprite->SetShadowType( eSpriteShadowType_None );		// 沒有影子
	pRoleSprite->SetCursorState( true );		// 可以被游標檢測
	pRoleSprite->SetTouchState( false );		// 可以被點選
	pRoleSprite->SetTargetState( false );		// 出現目標框
	pRoleSprite->SetNameVisible( false );		// 不能出現頭上字串

	//case EM_GameObjectMineType_Mine:
	pRoleSprite->SetMinimapIcon( 0 ); // EM_MINIMEM_MinimapOption_Mine
	//pRoleSprite->SetCheckedStatus( eChecked_Furniture );  

	//還沒打開家具擺設介面時，移動到家具可作用的家具上，滑鼠游標要變成齒輪
	if( m_bIsOwner && (
		itemDB->Item.emFurnitureType == EM_FurnitureType_Storage         || 
		itemDB->Item.emFurnitureType == EM_FurnitureType_Coathanger      ||
		itemDB->Item.emFurnitureType == EM_FurnitureType_WeaponSet       ||
		itemDB->Item.emFurnitureType == EM_FurnitureType_WeaponAndCloth  ) )
	{
		pRoleSprite->SetCheckedStatus( eChecked_FurnitureStorage );
	}
	else
	{
		pRoleSprite->SetCheckedStatus( eChecked_FurnitureNormal ); 
	}



	// Set fixed sprite orienting axis if the axis is valid

	pRoleSprite->SetName( itemDB->GetName() );

	CRuVector3 vecUP( rLayout.UpX , rLayout.UpY , rLayout.UpZ );

	// Set fixed orienting axis
	pRoleSprite->SetOrientingAxis( vecUP );

	// Enable re-basing of direction vector around the orienting axis
	pRoleSprite->SetRebaseDirection( true );

	// Disable orient to surface normal
	pRoleSprite->SetOrientToSurfaceNormal( false );

	CRuQuaternion orientation;
	orientation.FromAngleAxis( CRuVector3(0.0, 1.0, 0.0), rLayout.Angle );
	CRuVector3 direction = CRuVector3( 0.0f, 0.0f, -1.0f ) * orientation.GetMatrix();	
	pRoleSprite->SetCorrectDir(CRuVector3(0.0f, 0.0f, 0.0f));
	pRoleSprite->SetDirection( direction, true, true );

	pRoleSprite->SetPos( CRuVector3( rLayout.X, rLayout.Y, rLayout.Z ) );

	// Forcibly set actor movement type to NPC mode
	pRoleSprite->GetRuActor()->SetMovementType(ruSOJMOVETYPE_NPC);

	pRoleSprite->SetFade( false, 1.0f );
	pRoleSprite->SetGravityMode( false );
	//pRoleSprite->SetTargetState( false );		// 點擊後不會有頭像框

	// 設定sprite顯示
	//pRoleSprite->SetVisible(GraphObj->Mode.Show);
	pRoleSprite->SetVisible( true );

	// 設定sprite碰撞開關
	pRoleSprite->EnableCollision( true );

	// Set orientation mode to unconstrained
	pRoleSprite->SetOrientMode(CRuFusion_Actor::ruORIENTMODE_UNCONSTRAINED);

	pRoleSprite->SetCanAttackState(false);
	pRoleSprite->SetEnemyState(false);

	//m_RoleSpriteHouseItemMap[HouseItemDB->Pos] = pSprite;

	//g_pHousesFrame->m_pSpritePlaceing = pSprite;
	//g_pHousesFrame->m_PlaceStep = 0;
	// 設定紙娃娃部位裝備
	pRoleSprite->SetPaperdollPart( PAPERDOLL_PART_NAME_HEAD      );
	pRoleSprite->SetPaperdollPart( PAPERDOLL_PART_NAME_HAIR      );			
	pRoleSprite->SetPaperdollPart( PAPERDOLL_PART_NAME_HELMET    ); //頭盔
	pRoleSprite->SetPaperdollPart( PAPERDOLL_PART_NAME_HAND      ); //手
	pRoleSprite->SetPaperdollPart( PAPERDOLL_PART_NAME_SHOULDER  ); //肩
	pRoleSprite->SetPaperdollPart( PAPERDOLL_PART_NAME_TORSO     ); //軀幹
	pRoleSprite->SetPaperdollPart( PAPERDOLL_PART_NAME_LEG       ); //腿
	pRoleSprite->SetPaperdollPart( PAPERDOLL_PART_NAME_BELT      ); //腰帶
	pRoleSprite->SetPaperdollPart( PAPERDOLL_PART_NAME_FOOT      ); //腳
	pRoleSprite->SetPaperdollPart( PAPERDOLL_PART_NAME_BACK      ); //背部

	int color[2];
	color[0] = color[1] = 0x00000000;

	pRoleSprite->SetPaperdollColor( PAPERDOLL_PART_NAME_HEAD     , color[0], color[1] );
	pRoleSprite->SetPaperdollColor( PAPERDOLL_PART_NAME_HAIR     , color[0], color[1] );
	pRoleSprite->SetPaperdollColor( PAPERDOLL_PART_NAME_HELMET   , color[0], color[1] );
	pRoleSprite->SetPaperdollColor( PAPERDOLL_PART_NAME_HAND     , color[0], color[1] );
	pRoleSprite->SetPaperdollColor( PAPERDOLL_PART_NAME_SHOULDER , color[0], color[1] );
	pRoleSprite->SetPaperdollColor( PAPERDOLL_PART_NAME_TORSO    , color[0], color[1] );
	pRoleSprite->SetPaperdollColor( PAPERDOLL_PART_NAME_LEG      , color[0], color[1] );
	pRoleSprite->SetPaperdollColor( PAPERDOLL_PART_NAME_BELT     , color[0], color[1] );
	pRoleSprite->SetPaperdollColor( PAPERDOLL_PART_NAME_FOOT     , color[0], color[1] );
	pRoleSprite->SetPaperdollColor( PAPERDOLL_PART_NAME_BACK     , color[0], color[1] );


	pRoleSprite->SetHandsObjectId( eRoleHands_MainHand , -1 );
	pRoleSprite->SetHandsObjectId( eRoleHands_OffHand  , -1 );

	pRoleSprite->ReleaseOrnamentEntity();
	
	//處理衣架武器架
	if( itemDB->Item.emFurnitureType == EM_FurnitureType_Coathanger ||
		itemDB->Item.emFurnitureType == EM_FurnitureType_WeaponSet  ||
		itemDB->Item.emFurnitureType == EM_FurnitureType_WeaponAndCloth ||
		itemDB->Item.emFurnitureType == EM_FurnitureType_Maid )
	{
		//衣架有碰撞 會卡住
		pRoleSprite->EnableCollision( false );

		GameObjDbStructEx* itemDB;
		GameObjDbStructEx* imageitemDB;

		int EQPos;

		//衣架的性別
		string filename;
		//CRuEntity* pEntity = model->GetRuEntity();
		if( pRoleSprite->GetRuEntity() )
		{
			filename = pRoleSprite->GetRuEntity()->GetResourceName(); 
		}

		Sex_ENUM _eSex = ObjectDataClass::GetModelSex( filename.c_str() );


		TD_HouseItemMap &rHouseItemMap = GetHouseContainer( HouseItemDB->ItemDBID );
		TD_HouseItemMap::iterator itPos = rHouseItemMap.begin();
		TD_HouseItemMap::iterator itEnd = rHouseItemMap.end();

		HouseItemDBStruct* pHouseItemDB;

		for( ; itPos != itEnd ; ++itPos )
		{
			pHouseItemDB = itPos->second;
			EQPos = pHouseItemDB->Pos;

			ItemFieldStruct& rItemField = pHouseItemDB->Item;
			itemDB = CNetGlobal::GetObj( rItemField.OrgObjID );
			if( itemDB == NULL )
			{
				continue;
			}

			imageitemDB = CNetGlobal::GetObj( rItemField.ImageObjectID );
			if( imageitemDB == NULL )
			{
				imageitemDB = itemDB;
			}
	 
			GameObjDbStructEx* imageObj = CNetGlobal::GetObj( imageitemDB->ImageID );
			if ( imageObj == NULL )
			{
				continue;
			}

			SexFlagStruct& LimitSex1 = imageitemDB->Limit.Sex;
			SexFlagStruct& LimitSex2 = imageObj->Image.LimitSex;

			bool boy = true;
			bool girl = true;
			if( LimitSex1._Sex != 0 )
			{
				if( LimitSex1.Boy == false )
				{
					boy = false;
				}
				if( LimitSex1.Girl == false )
				{
					girl = false;
				}
			}

			if( LimitSex2._Sex != 0 )
			{
				if( LimitSex2.Boy == false )
				{
					boy = false;
				}
				if( LimitSex2.Girl == false )
				{
					girl = false;
				}
			}

			if( _eSex == EM_Sex_Boy  && boy  == false || 
				_eSex == EM_Sex_Girl && girl == false )
			{
				continue;
			}


			if( rItemField.MainColor == 0 )
				color[0] = imageObj->Image.ArmorMainColor.Color;	
			else
				color[0] = rItemField.MainColor;

			if( rItemField.OffColor == 0 )
				color[1] = imageObj->Image.ArmorOffColor.Color;	
			else
				color[1] = rItemField.OffColor;

			switch( EQPos )
			{
			case EM_EQWearPos_Head       : pRoleSprite->SetPaperdollPart( PAPERDOLL_PART_NAME_HELMET   , imageObj->Image.ACTWorld ); pRoleSprite->SetPaperdollColor( PAPERDOLL_PART_NAME_HELMET  , color[0], color[1] ); break;   //
			case EM_EQWearPos_Gloves     : pRoleSprite->SetPaperdollPart( PAPERDOLL_PART_NAME_HAND     , imageObj->Image.ACTWorld ); pRoleSprite->SetPaperdollColor( PAPERDOLL_PART_NAME_HAND    , color[0], color[1] ); break;   //手套
			case EM_EQWearPos_Shoes      : pRoleSprite->SetPaperdollPart( PAPERDOLL_PART_NAME_FOOT     , imageObj->Image.ACTWorld ); pRoleSprite->SetPaperdollColor( PAPERDOLL_PART_NAME_FOOT    , color[0], color[1] ); break;   //鞋子
			case EM_EQWearPos_Clothes    : pRoleSprite->SetPaperdollPart( PAPERDOLL_PART_NAME_TORSO    , imageObj->Image.ACTWorld ); pRoleSprite->SetPaperdollColor( PAPERDOLL_PART_NAME_TORSO   , color[0], color[1] ); break;   //上衣
			case EM_EQWearPos_Pants      : pRoleSprite->SetPaperdollPart( PAPERDOLL_PART_NAME_LEG      , imageObj->Image.ACTWorld ); pRoleSprite->SetPaperdollColor( PAPERDOLL_PART_NAME_LEG     , color[0], color[1] ); break;   //褲子
			case EM_EQWearPos_Back       : pRoleSprite->SetPaperdollPart( PAPERDOLL_PART_NAME_BACK     , imageObj->Image.ACTWorld ); pRoleSprite->SetPaperdollColor( PAPERDOLL_PART_NAME_BACK    , color[0], color[1] ); break;   //背部
			case EM_EQWearPos_Belt       : pRoleSprite->SetPaperdollPart( PAPERDOLL_PART_NAME_BELT     , imageObj->Image.ACTWorld ); pRoleSprite->SetPaperdollColor( PAPERDOLL_PART_NAME_BELT    , color[0], color[1] ); break;   //腰帶
			case EM_EQWearPos_Shoulder   : pRoleSprite->SetPaperdollPart( PAPERDOLL_PART_NAME_SHOULDER , imageObj->Image.ACTWorld ); pRoleSprite->SetPaperdollColor( PAPERDOLL_PART_NAME_SHOULDER, color[0], color[1] ); break;   //肩甲
			case EM_EQWearPos_MainHand   : pRoleSprite->SetHandsObjectId( eRoleHands_MainHand , rItemField.ImageObjectID ); break; 
			case EM_EQWearPos_SecondHand : pRoleSprite->SetHandsObjectId( eRoleHands_OffHand  , rItemField.ImageObjectID ); break; 
			case EM_ArmorPos_Ornament    :
				{
					if( imageObj->Image.ImageType == EM_GameObjectImageType_Wing )	// 翅膀
					{
						const char* insigniaType = NULL;
						const char* bannerType = NULL;
						int insigniaColor[2] = { 0, 0 };
						int bannerColor[2] = { 0, 0 };
						pRoleSprite->CreateOrnamentEntity( rItemField.ImageObjectID, insigniaType, insigniaColor[0], insigniaColor[1], bannerType, bannerColor[0], bannerColor[1], true );
					}
				}

			default:
				//assert( FALSE );
				break;
			}
		}
	}

	//if( itemDB->Item.emFurnitureType == EM_FurnitureType_WeaponSet || 
	//	itemDB->Item.emFurnitureType == EM_FurnitureType_WeaponAndCloth )
	//{
	//	TD_HousesContainerMap::iterator itC = m_HousesContainerMap.find( HouseItemDB->ItemDBID );
	//	if( itC != m_HousesContainerMap.end() )
	//	{
	//		TD_HouseItemMap &rHouseItemMap = itC->second;
	//		TD_HouseItemMap::iterator itI = rHouseItemMap.begin();

	//		int EQPos;

	//		for( ; itI != rHouseItemMap.end() ; ++itI )
	//		{
	//			EQPos = itI->first;

	//			switch( EQPos )
	//			{
	//			case EM_EQWearPos_MainHand     : pRoleSprite->SetHandsObjectId( eRoleHands_MainHand , itI->second.Item.OrgObjID ); break; 
	//			case EM_EQWearPos_SecondHand   : pRoleSprite->SetHandsObjectId( eRoleHands_OffHand  , itI->second.Item.OrgObjID ); break; 
	//			default:
	//				//assert( FALSE );
	//				break;
	//			}
	//		}
	//	}

	//}

	pRoleSprite->PaperdollBuild();
	

	//盆栽

	GameObjDbStructEx* pItemDB = CNetGlobal::GetObj( HouseItemDB->Item.OrgObjID );
	if( pItemDB && pItemDB->Item.Plant.Pot.Mode != 0 )
	{
		//HouseItemDB->Item.Plant

		//static int s_number = 0;
		//HouseItemDB->Item.Plant.SeedID = ( s_number % 10 ) + 1;
		//s_number = s_number + 1;
		SHousePlant sHousePlant;
		if( UpdateHousePlant( sHousePlant, HouseItemDB->Item ) )
		{
			if( sHousePlant.nPlantID >= 0 )
			{
				sHousePlant.sTree.eType = HPT_TREE;
				sHousePlant.sTree.strLinkName = "p_plant";
				sHousePlant.sTree.nBranchLayer = 0;
				sHousePlant.sTree.nRandom = sHousePlant.nCreateTime;

				Houses_CreatePlantNode( pRoleSprite->GetRuEntity(), sHousePlant, sHousePlant.sTree );

				m_MapHousePlant[ HouseItemDB->ItemDBID ] = sHousePlant;
	   		}
  		}
		
		//盆栽有碰撞 會卡住
		pRoleSprite->EnableCollision( false );
	}
		
	pRoleSprite->Update( 0 );
	pRoleSprite->SetPos( CRuVector3( rLayout.X, rLayout.Y, rLayout.Z ) );


	//留聲機
	if( itemDB->Item.FurnitureSoundID > 0 )
	{
		if( HouseItemDB->Layout.IsSound )
		{
			if( pRoleSprite->GetActorState() != ruFUSION_ACTORSTATE_ACTIVATE_LOOP && 
				pRoleSprite->GetActorState() != ruFUSION_ACTORSTATE_ACTIVATE_BEGIN )
			{
				pRoleSprite->SetActorState( ruFUSION_ACTORSTATE_ACTIVATE_BEGIN );
			}
		}
		else 
		{
			if( pRoleSprite->GetActorState() != ruFUSION_ACTORSTATE_ACTIVATE_END &&
				pRoleSprite->GetActorState() != ruFUSION_ACTORSTATE_NORMAL )
			{
				pRoleSprite->SetActorState( ruFUSION_ACTORSTATE_ACTIVATE_END );
			}
		}

		pRoleSprite->SetCheckedStatus( eChecked_FurnitureSound ); 
	}

}

// --------------------------------------------------------------------------------------
CRoleSprite* CHousesFrame::GetHouseRoleSpriteByItemDBID( int ItemDBID )
{
	CRoleSprite* pRoleSprite = g_pGameMain->FindSpriteByWorldGUID( ItemDBID + DF_HouseRoleSpriteWorldGUID );
	return pRoleSprite;
}

// --------------------------------------------------------------------------------------
TD_HouseItemMap& CHousesFrame::GetHouseContainer( int ContainerID )
{
	SHouseContainer& rHouseContainer = m_HouseContainers[ContainerID];

	if( rHouseContainer.bDirty )
	{
		rHouseContainer.Items.clear();

		map< int, HouseItemDBStruct >::iterator itPos = m_HouseItems.begin();
		map< int, HouseItemDBStruct >::iterator itEnd = m_HouseItems.end();

		for( ; itPos != itEnd ; ++itPos )
		{
			if( itPos->second.ParentItemDBID == ContainerID )
			{
				rHouseContainer.Items[ itPos->second.ItemDBID ] = &(itPos->second);
			}
		}

		rHouseContainer.bDirty = false;
	}

	return rHouseContainer.Items;
}

// --------------------------------------------------------------------------------------
TD_HouseItemMap& CHousesFrame::GetHouseFurnishingList()
{
	return GetHouseContainer( DF_HouseFurnishingListID );
}

// --------------------------------------------------------------------------------------
HouseItemDBStruct* CHousesFrame::GetHousesItemDB( int ContainerID , int Pos )
{
	TD_HouseItemMap &rHouseItemMap = GetHouseContainer( ContainerID );
	TD_HouseItemMap::iterator itPos = rHouseItemMap.begin();
	TD_HouseItemMap::iterator itEnd = rHouseItemMap.end();
	for( ; itPos != itEnd ; ++itPos )
	{
		if( itPos->second->Pos == Pos )
		{
			return itPos->second;
		}
	}

	return NULL;
}

// --------------------------------------------------------------------------------------
HouseItemDBStruct* CHousesFrame::GetHousesItemDBByDBID( int DBID )
{
	map< int, HouseItemDBStruct >::iterator itPos = m_HouseItems.find( DBID );

	if( itPos == m_HouseItems.end() )
	{
		return NULL;
	}

	return &itPos->second; 
}

// --------------------------------------------------------------------------------------
int CHousesFrame::GetContainerMaxItemCount( int ContainerID )
{
	if( ContainerID == DF_HouseFurnishingListID )
	{
		return m_HouseBaseInfo.MaxItemCount ;
	}

	map< int, HouseItemDBStruct >::iterator itPos = m_HouseItems.find( ContainerID );
	if( itPos == m_HouseItems.end() )
	{
		return 0;
	}

	GameObjDbStructEx* itemDB = CNetGlobal::GetObj( itPos->second.Item.OrgObjID );

	if( NULL == itemDB )
	{
		return 0;
	}
 
 	return itemDB->Item.iFurnitureStorageSize;
}

// --------------------------------------------------------------------------------------
int CHousesFrame::GetContainerLastPos( int ContainerID )
{
	if( ContainerID == DF_HouseFurnishingListID )
	{
		TD_HouseItemMap &rHouseItemMap = GetHouseFurnishingList();
		TD_HouseItemMap::iterator itPos = rHouseItemMap.begin();
		TD_HouseItemMap::iterator itEnd = rHouseItemMap.end();
		int lastPos = 0;

		for( ; itPos != itEnd ; ++itPos )
		{
			if( itPos->second->Pos > lastPos )
			{
				lastPos = itPos->second->Pos;
			}
		}

		return lastPos;
	}
	return GetContainerMaxItemCount( ContainerID );
}

void CHousesFrame::UpdateHighlightSprite()
{
	//m_FocusFurnishingID;    //正在放置的傢俱ID
	//m_MouseEnterFurnishingID; //滑鼠點到的傢俱ID
	CRoleSprite* pRoleSprite = NULL;

	TD_HouseItemMap &rHouseItemMap = GetHouseFurnishingList();
	TD_HouseItemMap::iterator itPos = rHouseItemMap.begin();
	TD_HouseItemMap::iterator itEnd = rHouseItemMap.end();
	for( ; itPos != itEnd ; ++itPos )
	{
		pRoleSprite = GetHouseRoleSpriteByItemDBID( itPos->second->ItemDBID );
		if( pRoleSprite )
		{
			pRoleSprite->DisableCelHighlight();
		}
	}

	//還沒打開家具擺設介面時，移動到家具上不要有描邊
	if( !m_PlaceFurnishingMode )
	{
		return;
	}

	if( m_FocusFurnishingID != -1 )
	{
		pRoleSprite = GetHouseRoleSpriteByItemDBID( m_FocusFurnishingID );
		if( pRoleSprite && pRoleSprite->IsVisible() )
			pRoleSprite->EnableCelHighlight( RUCOLOR_ARGB( 0xFF, 0xFF, 0xAF, 0x20 ) );
 	}

	if( m_MouseEnterFurnishingID != m_FocusFurnishingID &&
		m_MouseEnterFurnishingID != -1 )
	{
		pRoleSprite = GetHouseRoleSpriteByItemDBID( m_MouseEnterFurnishingID );
		if( pRoleSprite && pRoleSprite->IsVisible() )
			pRoleSprite->EnableCelHighlight( RUCOLOR_ARGB( 0xFF, 0x20, 0x20, 0xFF ) );
	}
}

// ----------------------------------------------------------------------------
void CHousesFrame::HousesControl_UseFurniture( int FurnitureID )
{
	SetFocusFurnishingID( FurnitureID );

	//使用物品
	if( m_PlaceFurnishingMode || m_FocusFurnishingID == -1 )
	{
		return;
	}

	CRoleSprite* pRoleSpriteFurnishing = GetHouseRoleSpriteByItemDBID( m_FocusFurnishingID );
	//if( g_pGameMain->GetDistanceWithPlayer( pRoleSpriteFurnishing ) > _MAX_ROLE_TOUCH_RANGE_ )
	//{
	//	return ;
	//}

	HouseItemDBStruct* pFurnishingDB = GetHousesItemDBByDBID( m_FocusFurnishingID );
	//assert( pFurnishingDB );
	if( !pFurnishingDB )
		return;
	GameObjDbStructEx* itemDB = CNetGlobal::GetObj( pFurnishingDB->Item.OrgObjID );
	//assert( itemDB );
	if( !itemDB )
		return;
	//assert( itemDB->Item.ItemType == EM_ItemType_Furniture );
	if( itemDB->Item.ItemType != EM_ItemType_Furniture )
		return;

	//EM_FurnitureType_Normal		= 0,	// 一般, 不具容量
	//EM_FurnitureType_Storage	= 1,	// 具儲存空間的傢俱
	//EM_FurnitureType_Coathanger	= 2,	// 衣架, 且可以展示衣物, 裝備, 具儲存空間的傢俱
	//EM_FurnitureType_WeaponSet	= 3,	// 武器架, 可以掛載, 單雙手武器或盾, 具儲存空間的傢俱

	if( itemDB->Item.emFurnitureType == EM_FurnitureType_Normal )
	{

	}
	else if( itemDB->Item.emFurnitureType == EM_FurnitureType_Storage )
	{
		if( m_bIsOwner )
		{

		}
		else if( m_bIsFriend && pFurnishingDB->Layout.IsFriend_Show )
		{

		}
		else
		{
			return ;
		}
 
		//DEBUG 有問題 現在先每次都要
		if( false == m_HouseContainers[pFurnishingDB->ItemDBID].bLoaded )
		{
			//發出取得BoxInfo封包
			NetCli_Houses::S_BoxInfoRequset( pFurnishingDB->ItemDBID );
			m_HouseContainers[pFurnishingDB->ItemDBID].bOpenFlag = true;
			return ;
		}

		//打開
		lua_State* L = m_interface->GetUi()->GetLuaState();
		lua_pushnumber( L, pFurnishingDB->ItemDBID );
		lua_setglobal( L, "StorageDBID" );
		SendWorldEvent( UiEvent_HousesStorageShow );

		if( m_HouseContainers[ pFurnishingDB->ItemDBID ].bOpened == false )
		{
			if( m_bIsFriend  )
			{
				//好友問候語
				if( strlen( g_pHousesFrame->m_HouseBaseInfo.Friend_Use ) > 0 )
				{
					g_pGameMain->SendSystemChatMsg( g_pHousesFrame->m_HouseBaseInfo.Friend_Use );
				}
			}

			m_HouseContainers[pFurnishingDB->ItemDBID].bOpened = true;
			CRoleSprite* pRoleSprite = GetHouseRoleSpriteByItemDBID( pFurnishingDB->ItemDBID );
			//assert( pRoleSprite );
			if( pRoleSprite )
			{
				pRoleSprite->SetActorState( ruFUSION_ACTORSTATE_ACTIVATE_BEGIN );
			}
		}

	}
	else if( m_bIsOwner && itemDB->Item.emFurnitureType == EM_FurnitureType_Coathanger )
	{
		//打開
		lua_State* L = m_interface->GetUi()->GetLuaState();
		lua_pushnumber( L, pFurnishingDB->ItemDBID );
		lua_setglobal( L, "StorageDBID" );
		lua_pushnumber( L, DF_HousesHangerMode_Coathanger );
		lua_setglobal( L, "HousesHangerMode" );
		SendWorldEvent( UiEvent_HousesHangerShow );
	}	
	else if( m_bIsOwner && itemDB->Item.emFurnitureType == EM_FurnitureType_WeaponSet )
	{
		//打開
		lua_State* L = m_interface->GetUi()->GetLuaState();
		lua_pushnumber( L, pFurnishingDB->ItemDBID );
		lua_setglobal( L, "StorageDBID" );
		lua_pushnumber( L, DF_HousesHangerMode_WeaponSet );
		lua_setglobal( L, "HousesHangerMode" );
		SendWorldEvent( UiEvent_HousesHangerShow );
	}
	else if( m_bIsOwner && itemDB->Item.emFurnitureType == EM_FurnitureType_WeaponAndCloth )
	{
		//打開
		lua_State* L = m_interface->GetUi()->GetLuaState();
		lua_pushnumber( L, pFurnishingDB->ItemDBID );
		lua_setglobal( L, "StorageDBID" );
		lua_pushnumber( L, DF_HousesHangerMode_Both );
		lua_setglobal( L, "HousesHangerMode" );
		SendWorldEvent( UiEvent_HousesHangerShow );
	}
	else
	{
		//assert( false );
	}

	
	//留聲機
	if( itemDB->Item.FurnitureSoundID > 0 )
	{
		if( m_bIsOwner )
		{
			if( pFurnishingDB->Layout.IsSound == false )
			{
				PlayFurnishingSound( pFurnishingDB->ItemDBID );
			}
			else
			{
 				StopAllFurnishingSound();
			}
		}
	}
	else if( itemDB->Item.emFurnitureType != EM_FurnitureType_Storage )
	{
		CRoleSprite* pRoleSprite = GetHouseRoleSpriteByItemDBID( pFurnishingDB->ItemDBID );
		//assert( pRoleSprite );
		if( pRoleSprite )
		{
			if( pRoleSprite->GetActorState() == ruFUSION_ACTORSTATE_ACTIVATE_LOOP )
			{
				pRoleSprite->SetActorState( ruFUSION_ACTORSTATE_ACTIVATE_END );
			}
			else
			{
				pRoleSprite->SetActorState( ruFUSION_ACTORSTATE_ACTIVATE_BEGIN );
			}
		}
	}

	//打開盆栽
	if( m_bIsOwner && itemDB->Item.Plant.Pot.Mode != 0 )
	{
		Plant_Close();
		m_PlantFurnishingDBID = m_FocusFurnishingID;
		m_PlantFurnishingPos = pFurnishingDB->Pos;
		SendWorldEvent( UiEvent_HousesPlantOpen );
	}

	//椅子
	//p_sit_chair01
	//p_sit_chair01
	
	CRuEntity* pEntity = pRoleSpriteFurnishing->GetRuEntity();
	if( pEntity )
	{
		g_pGameMain->SitToEntity( pEntity );
	}

}

// ----------------------------------------------------------------------------
void UiCallback_MaidDialog( int iID, PVOID pContext );
void UiCallback_MaidDialog_Option( int iID, PVOID pContext );
void UiCallback_MaidDialog_BackOption( int iID, PVOID pContext );


// ----------------------------------------------------------------------------
void UiCallback_MaidDialog( int iID, PVOID pContext )
{
	//vecOption.push_back( "改變密碼" );
	//vecOption.push_back( "離開房屋" );
	//vecOption.push_back( "離開對話" );

	if( g_pHousesFrame->m_bIsOwner ) 
	{
		if( iID == 0 )
		{
			NetCli_Houses::S_LeaveHouse( g_pGameMain->GetDBID() );
			NetCli_Other::This->R_CloseDialog();
		}
		else if( iID == 1 )
		{
			//改變職業
			NetCli_OtherChild::S_RunGlobalPlotRequest( 5 );
			NetCli_Other::This->R_CloseDialog();
		}
		else if( iID == 2 ) 
		{
			//開啟銀行
			NetCli_OtherChild::S_OpenBank();
			NetCli_Other::This->R_CloseDialog();
   		}
		else if( iID == 3 )
		{
			//改變房屋名稱
			lua_State* L = g_pHousesFrame->GetLuaState();
			lua_pushnumber( L, g_ObjectData->GetSysKeyValue( "House_ChangeName_Energy" ) );
			lua_setglobal( L, "arg1" );
			g_pHousesFrame->SendWorldEvent( UiEvent_HousesChangedName );
			NetCli_Other::This->R_CloseDialog();
		}
		else if( iID == 4 )
		{
			//改變密碼
			g_pHousesFrame->SendWorldEvent( UiEvent_HousesChangedPassword );
			NetCli_Other::This->R_CloseDialog();
		}
		else if( iID == 5 )
		{

			UiCallback_MaidDialog_BackOption( 0, NULL);
		}
		else if( iID == 6 )
		{

			NetCli_Other::This->R_CloseDialog();
		}
	}
	else
	{
		if( iID == 0 )
		{
			if( g_pHousesFrame->m_bIsFriend  )
			{
				//好友問候語
				if( strlen( g_pHousesFrame->m_HouseBaseInfo.Friend_Leave ) > 0 )
				{
					g_pGameMain->SendSystemChatMsg( g_pHousesFrame->m_HouseBaseInfo.Friend_Leave );
				}
			}

			NetCli_Houses::S_LeaveHouse( g_pGameMain->GetDBID() );
			NetCli_Other::This->R_CloseDialog();
		}
		else if( iID == 1 )
		{

			NetCli_Other::This->R_CloseDialog();
		}
	}

}


void UiCallback_MaidDialog_Option( int iID, PVOID pContext )
{	
	CRoleSprite* pRoleSpriteMaid = g_pGameMain->FindSpriteByWorldGUID( g_pHousesFrame->m_MaidRoleSpriteID );
	if( NULL == pRoleSpriteMaid )
	{
		return;
	}

	pRoleSpriteMaid->SetActorState( ruFUSION_ACTORSTATE_EMOTE_SPEAK );

	//選項名稱HOUSE_MAID_ENERGY_INTRO 能量補充
	//選項名稱HOUSE_MAID_SPACE_INTRO 空間擴充
	//選項名稱HOUSE_MAID_HOUSEFT_INTRO 家具擺設
	//SO_BACK 返回上一頁	
	
	if( iID == 0 )
	{
		vector<string> vecOption;
		vecOption.push_back( g_ObjectData->GetString( "SO_BACK" ) );
		vecOption.push_back( g_ObjectData->GetString( "HOUSE_MAID_LEAVE_TALK" ) );
		NetCli_Other::This->R_OpenDialog( 
			EM_LuaDialogType_Select, 
			pRoleSpriteMaid->GetName(), 
			g_ObjectData->GetString( "SO_HOMEINTRO_ENERGY"), 
			vecOption, UiCallback_MaidDialog_BackOption, NULL );
	}
	else if( iID == 1 )
	{
		vector<string> vecOption;
		vecOption.push_back( g_ObjectData->GetString( "SO_BACK" ) );
		vecOption.push_back( g_ObjectData->GetString( "HOUSE_MAID_LEAVE_TALK" ) );
		NetCli_Other::This->R_OpenDialog( 
			EM_LuaDialogType_Select, 
			pRoleSpriteMaid->GetName(), 
			g_ObjectData->GetString( "SO_HOMEINTRO_SPACE"), 
			vecOption, UiCallback_MaidDialog_BackOption, NULL );
	}
	else if( iID == 2 )
	{
		vector<string> vecOption;
		vecOption.push_back( g_ObjectData->GetString( "SO_BACK" ) );
		vecOption.push_back( g_ObjectData->GetString( "HOUSE_MAID_LEAVE_TALK" ) );
		NetCli_Other::This->R_OpenDialog( 
			EM_LuaDialogType_Select, 
			pRoleSpriteMaid->GetName(), 
			g_ObjectData->GetString( "SO_HOMEINTRO_HOMEFT"), 
			vecOption, UiCallback_MaidDialog_BackOption, NULL );
	}
	else if( iID == 3 )
	{
		g_pHousesFrame->HousesControl_DoMaid();
	}
	else if( iID == 4 )
	{

		NetCli_Other::This->R_CloseDialog();
	}
}

void UiCallback_MaidDialog_BackOption( int iID, PVOID pContext )
{
	if( iID == 0 )
	{
		CRoleSprite* pRoleSpriteMaid = g_pGameMain->FindSpriteByWorldGUID( g_pHousesFrame->m_MaidRoleSpriteID );
		if( NULL == pRoleSpriteMaid )
		{
			return;
		}

		pRoleSpriteMaid->SetActorState( ruFUSION_ACTORSTATE_EMOTE_SPEAK );

		vector<string> vecOption;	

		//選項名稱HOUSE_MAID_ENERGY_INTRO 能量補充
		//選項名稱HOUSE_MAID_SPACE_INTRO 空間擴充
		//選項名稱HOUSE_MAID_HOUSEFT_INTRO 家具擺設
		//SO_BACK 返回上一頁	 
		vecOption.push_back( g_ObjectData->GetString( "HOUSE_MAID_ENERGY_INTRO"  ) );
		vecOption.push_back( g_ObjectData->GetString( "HOUSE_MAID_SPACE_INTRO"   ) );
		vecOption.push_back( g_ObjectData->GetString( "HOUSE_MAID_HOUSEFT_INTRO" ) );
		vecOption.push_back( g_ObjectData->GetString( "SO_BACK" ) );
		vecOption.push_back( g_ObjectData->GetString( "HOUSE_MAID_LEAVE_TALK" ) );


		NetCli_Other::This->R_OpenDialog( 
			EM_LuaDialogType_Select, 
			pRoleSpriteMaid->GetName(), 
			g_ObjectData->GetString( "SO_HOMEINTRO_OPTION"), 
			vecOption, UiCallback_MaidDialog_Option, NULL );
	}
	else if( iID == 1 )
	{
		NetCli_Other::This->R_CloseDialog();
	}
}






// ----------------------------------------------------------------------------
void CHousesFrame::HousesControl_DoMaid()
{
	CRoleSprite* pRoleSpriteMaid = g_pGameMain->FindSpriteByWorldGUID( m_MaidRoleSpriteID );
	//assert( pRoleSpriteMaid );
	if( NULL == pRoleSpriteMaid )
	{
		return;
	}

	g_pHousesFrame->SendWorldEvent( UiEvent_HousesMaidSpeak );

	pRoleSpriteMaid->SetActorState( ruFUSION_ACTORSTATE_EMOTE_SPEAK );

	vector<string> vecOption;	

	vecOption.push_back( g_ObjectData->GetString( "HOUSE_MAID_LEAVE_HOUSE" ) );

	if( m_bIsOwner )
	{
		vecOption.push_back( g_ObjectData->GetString( "HOUSE_MAID_HOUSE_CHANGEJOB" ) ); //改變職業
		vecOption.push_back( g_ObjectData->GetString( "HOUSE_MAID_HOUSE_OPENBANK" ) ); //開啟銀行
		vecOption.push_back( g_ObjectData->GetString( "HOUSE_MAID_CHANGE_NAME" ) );
		vecOption.push_back( g_ObjectData->GetString( "HOUSE_MAID_CHANGE_PASSWORD" ) );
  		vecOption.push_back( g_ObjectData->GetString( "HOUSE_MAID_HOUSE_INTRO" ) ); 
 	}

	vecOption.push_back( g_ObjectData->GetString( "HOUSE_MAID_LEAVE_TALK" ) );
	NetCli_Other::This->R_OpenDialog( EM_LuaDialogType_Select, pRoleSpriteMaid->GetName(), g_ObjectData->GetString( "HOUSE_MAID_TALK_CONTENT"), vecOption, UiCallback_MaidDialog, NULL );

}


// ----------------------------------------------------------------------------
void UiCallback_ServantDialog1( int iID, PVOID pContext )
{
	NetCli_Other::This->R_CloseDialog();

	if( iID == 0 )
	{
		NetCli_Houses::SL_ServantEventRequest( g_pHousesFrame->m_FocusServantPos , 2 , 20 );
	}
	else if( iID == 1 )
	{
		NetCli_Houses::SL_ServantEventRequest( g_pHousesFrame->m_FocusServantPos , 2 , 21 );
	}
	else if( iID == 2 )
	{
		NetCli_Houses::SL_ServantEventRequest( g_pHousesFrame->m_FocusServantPos , 2 , 22 );
 	}
	else if( iID == 3 )
	{
		NetCli_Houses::SL_ServantEventRequest( g_pHousesFrame->m_FocusServantPos , 2 , 23 );
	}
	else if( iID == 4 )
	{
		NetCli_Houses::SL_ServantEventRequest( g_pHousesFrame->m_FocusServantPos , 2 , 24 );
	}
	else if( iID == 5 )
	{
		NetCli_Houses::SL_ServantEventRequest( g_pHousesFrame->m_FocusServantPos , 2 , 25 );
	}
	else if( iID == 6 )
	{

   		for( int i = 0 ; i < 6 ; ++i )
		{
			if( g_pHousesFrame->m_UIServantIndex[i] == g_pHousesFrame->m_FocusServantPos )
			{
				lua_State* L = g_pHousesFrame->GetLuaStateWorld();
				lua_pushnumber( L, i + 1 );
				lua_setglobal( L, "arg1" );
				g_pHousesFrame->SendWorldEvent( UiEvent_Houses_ServantInfoShow );
				break;
			}
		}
   
		//pRoleSprite->SetActorState( ruFUSION_ACTORSTATE_EMOTE_SPEAK );
	}

}

// ----------------------------------------------------------------------------
void UiCallback_ServantDialog2( int iID, PVOID pContext )
{
	NetCli_Other::This->R_CloseDialog();

	if( iID == 0 )
	{
			NetCli_Houses::SL_ServantEventRequest( g_pHousesFrame->m_FocusServantPos , 2 , 20 );
	}
	else if( iID == 1 )
	{

	}


}


// ----------------------------------------------------------------------------
void CHousesFrame::HousesControl_DoServant( int SpriteGUID )
{

	int ServantDBID = SpriteGUID - DF_HouseRoleSpriteWorldGUID;

	//放置家具中
	if( m_PlaceFurnishingMode )
	{
		return;
	}

	//if( ServantID < 0 || ServantID >= MAX_SERVANT_COUNT )
	//{
	//	return;
	//}

	HouseItemDBStruct* pServantItemDB = GetHousesItemDBByDBID( ServantDBID );
	if( NULL == pServantItemDB )
	{
		return;
	}

	int ServantIndex = pServantItemDB->Pos;
	if( ServantIndex < 0 || ServantIndex >= MAX_SERVANT_COUNT )
	{
		return;
	}

	HouseServantStruct* pServant = &m_HouseBaseInfo.Servant[ServantIndex]; 
	SHousesServantWorkInfo& rWorkInfo = m_ServantWorkInfos[ServantIndex];
 
	if( pServant->NPCObjID == 0 )
	{
		return;
	}

	if( rWorkInfo.isWorking )
	{												
		ServantTalk( CHAT_MSG_SYSTEM, ServantIndex, "SYS_HOUSE_SERVANT_BUSY" );
		return;
	}


	m_FocusServantDBID = ServantDBID;
	m_FocusServantPos = ServantIndex;

	CRoleSprite* pRoleSprite = GetHouseRoleSpriteByItemDBID( m_FocusServantDBID );

	if( NULL == pRoleSprite )
	{
		return;
	}

  	pRoleSprite->SetActorState( ruFUSION_ACTORSTATE_EMOTE_SPEAK );


	HouseItemDBStruct* pFurnishingDB = GetHousesItemDBByDBID( m_FocusServantDBID );
	if( !pFurnishingDB )
		return;

	//DEBUG 有問題 現在先每次都要
	if( false == m_HouseContainers[pFurnishingDB->ItemDBID].bLoaded )
	{
		//發出取得BoxInfo封包
		NetCli_Houses::S_BoxInfoRequset( pFurnishingDB->ItemDBID );
		m_HouseContainers[pFurnishingDB->ItemDBID].bOpenFlag = true;
//		return ;
	}



	vector<string> vecOption;	

	if( m_bIsOwner )
	{
		vecOption.push_back( g_ObjectData->GetString( "C_HOUSESERVANT_TALK_OPTION_1" ) );
		vecOption.push_back( g_ObjectData->GetString( "C_HOUSESERVANT_TALK_OPTION_2" ) );
		vecOption.push_back( g_ObjectData->GetString( "C_HOUSESERVANT_TALK_OPTION_3" ) );
		vecOption.push_back( g_ObjectData->GetString( "C_HOUSESERVANT_TALK_OPTION_4" ) );
		vecOption.push_back( g_ObjectData->GetString( "C_HOUSESERVANT_TALK_OPTION_5" ) );
		vecOption.push_back( g_ObjectData->GetString( "C_HOUSESERVANT_TALK_OPTION_6" ) );
		vecOption.push_back( g_ObjectData->GetString( "C_HOUSESERVANT_TALK_OPTION_VIEW" ) );
		vecOption.push_back( g_ObjectData->GetString( "HOUSE_MAID_LEAVE_TALK" ) );

		int suffix = pServant->Character % 100; //角色個性

		char talk[256];

		sprintf( talk, "C_HOUSESERVANT_INTRODUCTION_%02d", suffix );

 		NetCli_Other::This->R_OpenDialog( 
			EM_LuaDialogType_Select, 
			pRoleSprite->GetName(), 
			g_ObjectData->GetString( talk ), 
			vecOption, 
			UiCallback_ServantDialog1, 
			NULL );

	}
	else
	{
		vecOption.push_back( g_ObjectData->GetString( "C_HOUSESERVANT_TALK_OPTION_1" ) );
		vecOption.push_back( g_ObjectData->GetString( "HOUSE_MAID_LEAVE_TALK" ) );

		NetCli_Other::This->R_OpenDialog( 
			EM_LuaDialogType_Select, 
			pRoleSprite->GetName(), 
			g_ObjectData->GetString( "C_HOUSESERVANT_INTRODUCTION_100"), 
			vecOption, 
			UiCallback_ServantDialog2, 
			NULL );
	}
	
}

// ----------------------------------------------------------------------------
void CHousesFrame::UpdateHousesBenefit()
{
	m_ExpPoint          = 0;
	m_TpPoint           = 0;
	m_BlackSmithHQPoint = 0;
	m_CarpenterHQPoint  = 0;
	m_MakeArmorHQPoint  = 0;
	m_TailorHQPoint     = 0;
	m_CookHQPoint       = 0;
	m_AlchemyHQPoint    = 0;
  
	HouseItemDBStruct* pHouseItemDB = NULL;

	TD_HouseItemMap &rHouseItemMap = GetHouseFurnishingList();
	TD_HouseItemMap::iterator itPos = rHouseItemMap.begin();
	TD_HouseItemMap::iterator itEnd = rHouseItemMap.end();

	for( ; itPos != itEnd ; ++itPos )
	{
		pHouseItemDB = itPos->second;

		if( pHouseItemDB->Pos >= m_HouseBaseInfo.MaxItemCount )
		{
			continue;
		}

		GameObjDbStructEx* itemDB = CNetGlobal::GetObj( pHouseItemDB->Item.OrgObjID );
		if ( itemDB )
		{
			m_ExpPoint          += itemDB->Item.ExpPoint         ;
			m_TpPoint           += itemDB->Item.TpPoint          ;
			m_BlackSmithHQPoint += itemDB->Item.BlackSmithHQPoint;
			m_CarpenterHQPoint  += itemDB->Item.CarpenterHQPoint ;
			m_MakeArmorHQPoint  += itemDB->Item.MakeArmorHQPoint ;
			m_TailorHQPoint     += itemDB->Item.TailorHQPoint    ;
			m_CookHQPoint       += itemDB->Item.CookHQPoint      ;
			m_AlchemyHQPoint    += itemDB->Item.AlchemyHQPoint   ;
		}
 	}
}

// ----------------------------------------------------------------------------
void CHousesFrame::SetPlaceFurnishingMode( bool bMode )
{
	m_PlaceFurnishingMode = bMode;
	if( m_PlaceFurnishingMode )
	{

	}
	else
	{

	}
	UpdateHighlightSprite();
}


//void PickupItemToHouses( int ParentID, int Index )
//{
//
//}


// ----------------------------------------------------------------------------
void CHousesFrame::PickupItem( int ParentID, int Index, bool Reverse, bool IsBagCalled )
{
	if( EHousesState_Enter != m_eHousesState )
	{
		return ;
	}

	ClipboardItemInfo itemInfo = g_pItemClipboard->GetItemInfo();

	switch( g_pItemClipboard->GetType() )
	{	
	case eClipboardItem_Type_None:				// 拿取物品
		{
			HouseItemDBStruct* pHouseItemDB = GetHousesItemDB( ParentID , Index );
			if( NULL == pHouseItemDB )
			{
				break;
			}

			GameObjDbStruct* itemDB = CNetGlobal::GetObj( pHouseItemDB->Item.OrgObjID );
			if ( NULL == itemDB )
			{
				break;
			}

			if( ParentID == DF_HouseFurnishingListID )
			{
				int MaxItemCount = GetContainerMaxItemCount( ParentID );
				if( Index >= MaxItemCount )
				{
					//g_pGameMain->SendSystemMsg( g_ObjectData->GetString("SYS_HOUSES_FURNISHING_OVERDUE") );
					g_pGameMain->SendSystemChatMsg( g_ObjectData->GetString( "SYS_HOUSES_FURNISHING_OVERDUE" ) );
					return;
				}
			}
			
			//g_pItemClipboard->SetFile( itemDB->ImageObj->ACTField );
			g_pItemClipboard->SetFile( ItemUTIL::GetItemIcon( &pHouseItemDB->Item ) );
			g_pItemClipboard->PickupHouse( pHouseItemDB->Item.OrgObjID, ParentID, Index );

			return;
			break;
		}
	case eClipboardItem_Type_Bag:				// 背包 -> 
	case eClipboardItem_Type_Equipment:			// 裝備 ->
	case eClipboardItem_Type_House:             // 房屋 ->
		{
			if( ParentID == DF_HouseFurnishingListID )
			{
				int MaxItemCount = GetContainerMaxItemCount( ParentID );
				if( Index >= MaxItemCount )
				{
					//g_pGameMain->SendSystemMsg( g_ObjectData->GetString("SYS_HOUSES_FURNISHING_OVERDUE") );
					g_pGameMain->SendSystemChatMsg( g_ObjectData->GetString( "SYS_HOUSES_FURNISHING_OVERDUE" ) );
					return;
				}
			}


			bool bCanSwap= false;
			ItemFieldStruct* item = NULL;
			if( g_pItemClipboard->GetType() == eClipboardItem_Type_Bag )
			{				
				item = CNetGlobal::RoleData()->GetBodyItem(itemInfo.bag.pos);
			}
			else if( g_pItemClipboard->GetType() == eClipboardItem_Type_Equipment )
			{
				item = CNetGlobal::RoleData()->GetEqItem(itemInfo.equipment.pos);
				//   ItemFieldStruct* GetEqItem  ( int Pos );
				//destItem->Pos = srcItem->Pos = EM_ItemState_CliDisabled;
				//NetCli_EQ::S_SetEQ_Body(*destItem, index, itemInfo.equipment.pos, true);
			}
			else if( g_pItemClipboard->GetType() == eClipboardItem_Type_House )
			{
				HouseItemDBStruct* pHouseItemDB = GetHousesItemDB( itemInfo.house.parentID , itemInfo.house.pos );
				if( pHouseItemDB )
				{
					item = &pHouseItemDB->Item;
				}
			}

			if( !item )
			{
				return;
			}

			//判斷是不是未開啟的空間
   			{
   				int FurnishingPos = 0;

				if( ParentID == DF_HouseFurnishingListID )
				{
					FurnishingPos = Index;
				}
				else
				{
					HouseItemDBStruct* pParentHouseItemDB = GetHousesItemDBByDBID( ParentID );

					if( NULL == pParentHouseItemDB )
					{
						return;
					}

					FurnishingPos = pParentHouseItemDB->Pos;
				}
   		
				if( FurnishingPos >= m_HouseBaseInfo.MaxItemCount )
				{
					return;
				}
			}

   
			if( ParentID == DF_HouseFurnishingListID )
			{
				//祥人會判斷
				bCanSwap = true;
			}
			else if( ParentID == DF_HouseServantListID )
			{
				return ;
	 		}
			else
			{
				HouseItemDBStruct* pParentHouseItemDB = GetHousesItemDBByDBID( ParentID );
				//assert( pParentHouseItemDB );
				if( !pParentHouseItemDB )
					return;
				GameObjDbStructEx* pParentItemDB = CNetGlobal::GetObj( pParentHouseItemDB->Item.OrgObjID );
				//assert( pParentItemDB );
				if( !pParentItemDB )
					return;
				//assert( pParentItemDB->Item.ItemType == EM_ItemType_Furniture );
				if( pParentItemDB->Item.ItemType != EM_ItemType_Furniture )
					return;

				GameObjDbStructEx* pItemDB = CNetGlobal::GetObj( item->OrgObjID );
				//assert( pItemDB );
				if( !pItemDB )
					return;

				//EM_FurnitureType_Normal		= 0,	// 一般, 不具容量
				//EM_FurnitureType_Storage	= 1,	// 具儲存空間的傢俱
				//EM_FurnitureType_Coathanger	= 2,	// 衣架, 且可以展示衣物, 裝備, 具儲存空間的傢俱
				//EM_FurnitureType_WeaponSet	= 3,	// 武器架, 可以掛載, 單雙手武器或盾, 具儲存空間的傢俱
				
				if( pParentHouseItemDB->ParentItemDBID == DF_HouseServantListID ) //女僕
				{
	  				if( Index >= DF_ServantArmorPos && Index < DF_ServantArmorPosEnd )
					{
						int EQPos = Index;

						if( pItemDB->Classification != EM_ObjectClass_Item )
						{
							return ;
						}

						if( pItemDB->Item.Class != EM_Item_Armor )
						{
							return ;
						}

						switch( EQPos )
						{
						case EM_EQWearPos_Head     : if( pItemDB->Item.ArmorPos != EM_ArmorPos_Head     ){ return ; } break;   //
						case EM_EQWearPos_Gloves   : if( pItemDB->Item.ArmorPos != EM_ArmorPos_Gloves   ){ return ; } break;   //手套
						case EM_EQWearPos_Shoes    : if( pItemDB->Item.ArmorPos != EM_ArmorPos_Shoes    ){ return ; } break;   //鞋子
						case EM_EQWearPos_Clothes  : if( pItemDB->Item.ArmorPos != EM_ArmorPos_Clothes  ){ return ; } break;   //上衣
						case EM_EQWearPos_Pants    : if( pItemDB->Item.ArmorPos != EM_ArmorPos_Pants    ){ return ; } break;   //褲子
						case EM_EQWearPos_Back     : if( pItemDB->Item.ArmorPos != EM_ArmorPos_Back     ){ return ; } break;   //背部
						case EM_EQWearPos_Belt     : if( pItemDB->Item.ArmorPos != EM_ArmorPos_Belt     ){ return ; } break;   //腰帶
						case EM_EQWearPos_Shoulder : if( pItemDB->Item.ArmorPos != EM_ArmorPos_Shoulder ){ return ; } break;   //肩甲
						case EM_EQWearPos_Ornament : if( pItemDB->Item.ArmorPos != EM_ArmorPos_Ornament ){ return ; } break;
						default:
								return ;
						}

					}

					if( Index >= DF_ServantItemPos && Index < DF_ServantItemPosEnd )
					{

					}

					//不能放入
					if( Index > DF_ServantItemPosEnd )
					{
						return ;
					}
					
					//交換物品
					//NetCli_Houses::S_SwapBodyHouseItem( itemInfo.bag.pos , Index , ParentID , true );
					bCanSwap = true;
  				}
				else if( pParentItemDB->Item.emFurnitureType == EM_FurnitureType_Normal )
				{
					//assert( FALSE );
				}
				else if( pParentItemDB->Item.emFurnitureType == EM_FurnitureType_Storage )
				{
					//交換物品
					//NetCli_Houses::S_SwapBodyHouseItem( itemInfo.bag.pos , Index , ParentID , true );
					bCanSwap = true;
				}
				else if( pParentItemDB->Item.emFurnitureType == EM_FurnitureType_Coathanger )
				{
					if( pItemDB->Classification != EM_ObjectClass_Item )
					{
						return ;
					}

					if( pItemDB->Item.Class != EM_Item_Armor )
					{
						return ;
					}

					if( pItemDB->Item.ArmorPos != Index )
					{
						return ;
					}

					//交換物品
					//NetCli_Houses::S_SwapBodyHouseItem( itemInfo.bag.pos , Index , ParentID , true );
					bCanSwap = true;

				}
				else if( pParentItemDB->Item.emFurnitureType == EM_FurnitureType_WeaponSet )
				{
					if( pItemDB->Classification != EM_ObjectClass_Item )
					{
						return ;
					}

					//if( pItemDB->Item.Class != EM_Item_Weapon )
					//{
					//	return ;
					//}

					if( Index == EM_EQWearPos_MainHand && 
						pItemDB->Item.Class == EM_Item_Weapon &&
						( pItemDB->Item.WeaponPos == EM_WeaponPos_MainHand ||
						pItemDB->Item.WeaponPos == EM_WeaponPos_OneHand  ||
						pItemDB->Item.WeaponPos == EM_WeaponPos_TwoHand  ) )
					{   
						bCanSwap = true;
						//雙手武器需判斷副手有沒有東西
						if (pItemDB->Item.Class == EM_Item_Weapon && pItemDB->Item.WeaponPos == EM_WeaponPos_TwoHand)
						{
							//HouseItemDBStruct* mainHand = GetHousesItemDB( ParentID , EM_EQWearPos_MainHand   );
							HouseItemDBStruct* offHand  = GetHousesItemDB( ParentID , EM_EQWearPos_SecondHand );

							//if( mainHand && offHand )
							if( offHand )
							{
								RoleDataEx* pRoleData = CNetGlobal::RoleData();
								int pos = pRoleData->SearchBodyFirstFit( &offHand->Item, false );
								//ItemFieldStruct* bodyItem = pRoleData->GetBodyItem(pos);
								if ( pos != -1 )
								{
									//交換物品
									NetCli_Houses::S_SwapBodyHouseItem( pos , EM_EQWearPos_SecondHand , ParentID , true );
								}
								else
								{
									g_pGameMain->SendWarningMsg( g_ObjectData->GetString("SYS_WEAR_EQ_ERR1") );
									bCanSwap = false;
								}
							}
						}

						//交換物品
						//NetCli_Houses::S_SwapBodyHouseItem( itemInfo.bag.pos , Index , ParentID , true );
					}

					if( ( Index == EM_EQWearPos_SecondHand && pItemDB->Item.Class == EM_Item_Weapon && ( pItemDB->Item.WeaponPos == EM_WeaponPos_SecondHand || pItemDB->Item.WeaponPos == EM_WeaponPos_OneHand ) ) || //副手
						( Index == EM_EQWearPos_SecondHand && pItemDB->Item.Class == EM_Item_Armor && pItemDB->Item.WeaponPos == EM_ArmorPos_SecondHand ) ) //盾牌
					{
						bCanSwap = true;
						//判斷主手 是不是雙手武器
						HouseItemDBStruct* MainHandHouseItemDB = GetHousesItemDB( ParentID , EM_EQWearPos_MainHand );
						if( MainHandHouseItemDB )
						{
							GameObjDbStructEx* pMainHandItemDB = CNetGlobal::GetObj( MainHandHouseItemDB->Item.OrgObjID );
							if( pMainHandItemDB && pMainHandItemDB->Item.Class == EM_Item_Weapon && pMainHandItemDB->Item.WeaponPos == EM_WeaponPos_TwoHand )
							{
								RoleDataEx* pRoleData = CNetGlobal::RoleData();
								int pos = pRoleData->SearchBodyFirstFit( &MainHandHouseItemDB->Item, false );
								//ItemFieldStruct* bodyItem = pRoleData->GetBodyItem(pos);
								if ( pos != -1 )
								{
									//交換物品
									NetCli_Houses::S_SwapBodyHouseItem( pos , EM_EQWearPos_MainHand , ParentID , true );
								}
								else
								{
									g_pGameMain->SendWarningMsg( g_ObjectData->GetString("SYS_WEAR_EQ_ERR1") );
									bCanSwap = false;
								}
							}
						}
					}
	  			}
				else if( pParentItemDB->Item.emFurnitureType == EM_FurnitureType_WeaponAndCloth )
				{

				}
			}

			if( bCanSwap )
			{
				if( g_pItemClipboard->GetType() == eClipboardItem_Type_Bag )
				{
					HouseItemDBStruct* pDestHouseItemDB = GetHousesItemDB( ParentID , Index );
					if ( itemInfo.bag.pos < _MAX_BODY_EX_COUNT_ && pDestHouseItemDB != NULL )
					{
	 					GameObjDbStruct* pDestItemDB = CNetGlobal::GetObj( pDestHouseItemDB->Item.OrgObjID );
						if ( pDestItemDB && !pDestItemDB->Mode.DepartmentStore )
						{
							// 不能放回商城背包
							g_pGameMain->SendWarningMsg( g_ObjectData->GetString("SYS_HOUSE_TO_GOODS_FAILED") );
							return;
						}
					}
					if( IsBagCalled )
					{
						NetCli_Houses::S_SwapBodyHouseItem( itemInfo.bag.pos , Index , ParentID , false );
					}
					else
					{
  						NetCli_Houses::S_SwapBodyHouseItem( itemInfo.bag.pos , Index , ParentID , true );
					}
				}
				else if( g_pItemClipboard->GetType() == eClipboardItem_Type_Equipment )
				{
					//NetCli_Houses::S_ExchangeEq( ParentID , Index , (EQWearPosENUM)itemInfo.equipment.pos );
					g_pCharacterFrame->Equip( itemInfo.equipment.pos , 3 , Index , ParentID );
				}
				else if( g_pItemClipboard->GetType() == eClipboardItem_Type_House )
				{
					//NetCli_Houses::S_SwapHouseItemRequest( itemInfo.house.parentID , itemInfo.house.pos , ParentID , Index );
					//反向再判斷一次
					HouseItemDBStruct* pDestHouseItemDB = GetHousesItemDB( ParentID , Index );

					if( pDestHouseItemDB == NULL || Reverse )
					{
						NetCli_Houses::S_SwapHouseItemRequest( itemInfo.house.parentID , itemInfo.house.pos , ParentID , Index );
					}
					else
					{
						int SourParentID = itemInfo.house.parentID;
						int SourIndex = itemInfo.house.pos;
						g_pItemClipboard->Clear();
						g_pItemClipboard->PickupHouse( 0, ParentID , Index );
						g_pHousesFrame->PickupItem( SourParentID, SourIndex, true );
					}
				}
			}

			break;

		}

	case eClipboardItem_Type_Bank:				// 銀行 -> 
		//Deposit(index, eBank_Type_Bank, itemInfo.bank.pos);
		break;
	case eClipboardItem_Type_Trade:
	case eClipboardItem_Type_Action:
	case eClipboardItem_Type_Skill:
	case eClipboardItem_Type_Shop:
	case eClipboardItem_Type_Macro:
	case eClipboardItem_Type_Emote:

		break;
	}

	// clear item clipboard data
	g_pItemClipboard->Clear();
}

// ----------------------------------------------------------------------------
void CHousesFrame::DrawItem( int ParentID, int Index )
{
	if( EHousesState_Enter != m_eHousesState )
	{
		return ;
	}

	int pos = -1;
	HouseItemDBStruct* houseItem = GetHousesItemDB(ParentID, Index);
	if ( houseItem != NULL )
	{
		pos = CNetGlobal::RoleData()->SearchBodyFirstFit( &houseItem->Item, true );
	}
	else
	{
		pos = CNetGlobal::RoleData()->SearchBodyFirstFit( NULL , 0 );
	}

	if ( pos != -1 )
	{
		NetCli_Houses::S_SwapBodyHouseItem( pos , Index , ParentID , false );
		g_pRuGlobalEventManager->PlaySoundByFile("sound\\interface\\ui_icon_drop.mp3", false, 0.0f, ruSOUNDTYPE_INTERFACE);
	}
}

// ----------------------------------------------------------------------------
bool CHousesFrame::DepositFromBag( int bagindex )
{
	if( EHousesState_Enter != m_eHousesState )
	{
		return false;
	}

	map<int,SHouseContainer>::iterator itPos = m_HouseContainers.begin();
	map<int,SHouseContainer>::iterator itEnd = m_HouseContainers.end();

	bool bOpened = false;

	int DBID;
	for( ; itPos != itEnd ; ++itPos )
	{
		DBID = itPos->first;
		SHouseContainer& rHouseContainer = itPos->second;

		if( rHouseContainer.bOpened )
		{
			bOpened = true;

			//有沒有空間
			HouseItemDBStruct* pHouseItemDB = GetHousesItemDBByDBID( DBID );
			if( !pHouseItemDB )
			{
				continue;
			}
			GameObjDbStructEx* itemDB =	CNetGlobal::GetObj( pHouseItemDB->Item.OrgObjID );
			if( !itemDB )
			{
				continue;
			}

			int count = itemDB->Item.iFurnitureStorageSize;
			int i;
			for( i = 0 ; i < count ; ++i )
			{
				if( NULL == GetHousesItemDB( pHouseItemDB->ItemDBID , i ) )
				{
					NetCli_Houses::S_SwapBodyHouseItem( bagindex , i , pHouseItemDB->ItemDBID , true );						
					return true;
				}
			}
		}
	}

	if( bOpened )
	{
		g_pGameMain->SendWarningMsg( g_ObjectData->GetString("SYS_HOUSE_STORAGE_FULL") );
		return false;
	}

	return false;
}

// ----------------------------------------------------------------------------
void CHousesFrame::SetFocusFurnishingID( int ID )
{
	HouseItemLayoutChange();

	m_FocusFurnishingID = ID;

	m_ePlaceingMode = m_ePlaceingModeTemp;
	m_PlaceStep = 0;

	UpdateHighlightSprite();
	SendWorldEvent( UiEvent_HousesFurnitureChanged );
}

// ----------------------------------------------------------------------------
void CHousesFrame::FurnishingPlace()
{
	if( m_FocusFurnishingID == -1 )
	{
		return;
	}

	CRoleSprite* pRoleSpriteHouse = g_pGameMain->FindSpriteByWorldGUID( m_HouseRoleSpriteID ); //判斷房屋在不再
	HouseItemDBStruct* pFurnishingDB = GetHousesItemDBByDBID( m_FocusFurnishingID );

	//assert( pRoleSpriteHouse );
	if( !( pRoleSpriteHouse && pFurnishingDB ) )
	{
		m_ePlaceingMode = m_ePlaceingModeTemp;
		return;
	}

	//判斷是不是未開啟的空間
	if( pFurnishingDB->Pos >= m_HouseBaseInfo.MaxItemCount )
	{
		return;
	}

	//if( pFurnishingDB->Layout.IsShow == false )
	{
		pFurnishingDB->Layout.IsShow = true;

		pFurnishingDB->Layout.X = 0.0f; 
		pFurnishingDB->Layout.Y = 30.0f;
		pFurnishingDB->Layout.Z = 0.0f;     
		pFurnishingDB->Layout.UpX = 0.0f; 
		pFurnishingDB->Layout.UpY = 1.0f; 
		pFurnishingDB->Layout.UpZ = 0.0f;   
		pFurnishingDB->Layout.Angle = 0.0f; 

		LoadRoleSpriteHouseItem( pFurnishingDB );
		SetFocusFurnishingID( m_FocusFurnishingID );

		m_ePlaceingMode = EM_PlaceingMode_New;  //放置模式  
		m_PlaceStep = 0;       //放置模式步驟 1移動 2旋轉

		m_bFurnishingChange = true;
	}
}

// ----------------------------------------------------------------------------
void CHousesFrame::FurnishingRemove()
{
	if( m_FocusFurnishingID == -1 )
	{
		return;
	}

	CRoleSprite* pRoleSpriteHouse = g_pGameMain->FindSpriteByWorldGUID( m_HouseRoleSpriteID ); //判斷房屋在不再
	HouseItemDBStruct* pFurnishingDB = GetHousesItemDBByDBID( m_FocusFurnishingID );

	//assert( pRoleSpriteHouse );
	if( !( pRoleSpriteHouse && pFurnishingDB  ) )
	{
		m_ePlaceingMode = m_ePlaceingModeTemp;
		m_PlaceStep = 0;
		return;
	}

	if( pFurnishingDB->Layout.IsShow == true )
	{
		pFurnishingDB->Layout.IsShow = false;
		pFurnishingDB->Layout.X = 0.0f; 
		pFurnishingDB->Layout.Y = 30.0f;
		pFurnishingDB->Layout.Z = 0.0f;     
		pFurnishingDB->Layout.UpX = 0.0f; 
		pFurnishingDB->Layout.UpY = 1.0f; 
		pFurnishingDB->Layout.UpZ = 0.0f;   
		pFurnishingDB->Layout.Angle = 0.0f; 

		m_ePlaceingMode = m_ePlaceingModeTemp; //放置模式  
		m_PlaceStep = 0;         //放置模式步驟 1移動 2旋轉

		CRoleSprite* pRoleSprite = GetHouseRoleSpriteByItemDBID( pFurnishingDB->ItemDBID );
		if( pRoleSprite )
		{
			pRoleSprite->SetVisible( false );
			pRoleSprite->EnableCollision(FALSE);
			pRoleSprite->SetCursorState( false ); 
		}


		m_bFurnishingChange = true;
		//按下回收家具立刻反應
		HouseItemLayoutChange();
		//SetCurrentItemID( -1 );
	}
}

// ----------------------------------------------------------------------------
void CHousesFrame::FurnishingMove()
{
	m_ePlaceingMode = EM_PlaceingMode_Move; //放置模式  
	m_ePlaceingModeTemp = EM_PlaceingMode_Move;
	m_PlaceStep = 0;         //放置模式步驟 1移動 2旋轉

}

// ----------------------------------------------------------------------------
void CHousesFrame::FurnishingRorare()
{
	m_ePlaceingMode = EM_PlaceingMode_Rorare; //放置模式  
	m_ePlaceingModeTemp = EM_PlaceingMode_Rorare;
	m_PlaceStep = 0;         //放置模式步驟 1移動 2旋轉
}

// ----------------------------------------------------------------------------
void CHousesFrame::HouseItemLayoutChange()
{
	if( m_FocusFurnishingID == -1 || m_bFurnishingChange == false )
		return;

	HouseItemDBStruct* pHouseItemDB = GetHousesItemDBByDBID( m_FocusFurnishingID );

	//assert(pHouseItemDB);
	if(!pHouseItemDB)
	{
		return;
	}

	NetCli_Houses::S_HouseItemLayoutRequest( pHouseItemDB->ItemDBID, pHouseItemDB->Layout );

	//g_pChatFrame->SendMsgEvent( CHAT_MSG_SYSTEM, "CHousesFrame::HouseItemLayoutChange()", "" );

	m_bFurnishingChange = false;
}

// ----------------------------------------------------------------------------
void CHousesFrame::CloseStorage( int DBID )
{
	CRoleSprite* pRoleSprite = GetHouseRoleSpriteByItemDBID( DBID );

	//assert( pRoleSprite );
 	if( pRoleSprite )
	{
		//ruFUSION_ACTORSTATE_ACTIVATE_BEGIN
		pRoleSprite->SetActorState( ruFUSION_ACTORSTATE_ACTIVATE_END );
	}
	m_HouseContainers[DBID].bOpened = false;
}

// ----------------------------------------------------------------------------
CursorType CHousesFrame::GetHousesCursor()
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
	if( m_PlaceFurnishingMode && m_MouseEnterFurnishingID != -1 )
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

// ----------------------------------------------------------------------------
void CHousesFrame::HangerSwap( int ItemDBID, bool bAll )
{
	//判斷有無未榜定裝備
//	else if ( item->Mode.Trade && item->Mode.EQSoulBound )						// 未綁定
//	{
//		PendingEquip(slot, type, index, ContainerID);
//		return;	   
//	}
	{
		TD_HouseItemMap &rHouseItemMap = GetHouseContainer( ItemDBID );
		TD_HouseItemMap::iterator itPos = rHouseItemMap.begin();
		TD_HouseItemMap::iterator itEnd = rHouseItemMap.end();
	
		//int EQPos;
 		for( ; itPos != itEnd ; ++itPos )
		{
			//EQPos = itPos->first;
			HouseItemDBStruct* pHouseItemDB = itPos->second;
			ItemFieldStruct* item = &pHouseItemDB->Item;
			if ( item->Mode.Trade && item->Mode.EQSoulBound )
			{
				g_pGameMain->SendMessageDialog( g_ObjectData->GetString( "SYS_HOUSE_HANGER_SWAP_SOULBOUND" ) );
				return;
			}
		}
	}
	
	HouseItemDBStruct* pHouseItemDB = GetHousesItemDBByDBID( ItemDBID );
	GameObjDbStructEx* itemDB = CNetGlobal::GetObj( pHouseItemDB->Item.OrgObjID );

	if( itemDB && itemDB->Item.ItemType == EM_ItemType_Furniture )
	{

	}
	else
	{
		return;
	}

	if( bAll )
	{
		if( itemDB->Item.emFurnitureType == EM_FurnitureType_Coathanger || 
			itemDB->Item.emFurnitureType == EM_FurnitureType_WeaponAndCloth )
		{
			NetCli_Houses::S_ExchangeEq( ItemDBID , EM_EQWearPos_Head       , (EQWearPosENUM)EM_EQWearPos_Head       );
			NetCli_Houses::S_ExchangeEq( ItemDBID , EM_EQWearPos_Gloves     , (EQWearPosENUM)EM_EQWearPos_Gloves     );
			NetCli_Houses::S_ExchangeEq( ItemDBID , EM_EQWearPos_Shoes      , (EQWearPosENUM)EM_EQWearPos_Shoes      );
			NetCli_Houses::S_ExchangeEq( ItemDBID , EM_EQWearPos_Clothes    , (EQWearPosENUM)EM_EQWearPos_Clothes    );
			NetCli_Houses::S_ExchangeEq( ItemDBID , EM_EQWearPos_Pants      , (EQWearPosENUM)EM_EQWearPos_Pants      );
			NetCli_Houses::S_ExchangeEq( ItemDBID , EM_EQWearPos_Back       , (EQWearPosENUM)EM_EQWearPos_Back       );
			NetCli_Houses::S_ExchangeEq( ItemDBID , EM_EQWearPos_Belt       , (EQWearPosENUM)EM_EQWearPos_Belt       );
			NetCli_Houses::S_ExchangeEq( ItemDBID , EM_EQWearPos_Shoulder   , (EQWearPosENUM)EM_EQWearPos_Shoulder   );
			NetCli_Houses::S_ExchangeEq( ItemDBID , EM_ArmorPos_Ornament    , (EQWearPosENUM)EM_EQWearPos_Ornament   );
		}
	}
	else
	{
		vector<int> WearPosTable;
  		WearPosTable.push_back( EM_EQWearPos_Head       );
  		WearPosTable.push_back( EM_EQWearPos_Gloves     );
		WearPosTable.push_back( EM_EQWearPos_Shoes      );
		WearPosTable.push_back( EM_EQWearPos_Clothes    );
		WearPosTable.push_back( EM_EQWearPos_Pants      );
		WearPosTable.push_back( EM_EQWearPos_Back       );
		WearPosTable.push_back( EM_EQWearPos_Belt       );
		WearPosTable.push_back( EM_EQWearPos_Shoulder   );
		WearPosTable.push_back( EM_ArmorPos_Ornament    );
   		//WearPosTable.push_back( EM_EQWearPos_MainHand   );
		//WearPosTable.push_back( EM_EQWearPos_SecondHand );

		int pos;
		int i;
		for( i = 0 ; i < (int)WearPosTable.size() ; ++i )
		{
			pos = WearPosTable[i];
			if( GetHousesItemDB( ItemDBID , pos ) )
			{
				if( pos == EM_ArmorPos_Ornament )
				{
					NetCli_Houses::S_ExchangeEq( ItemDBID , pos , (EQWearPosENUM)EM_EQWearPos_Ornament );
				}
				else
				{
					NetCli_Houses::S_ExchangeEq( ItemDBID , pos , (EQWearPosENUM)pos );
				}
			}
   		}

	}


	if( itemDB->Item.emFurnitureType == EM_FurnitureType_WeaponSet ||
		itemDB->Item.emFurnitureType == EM_FurnitureType_WeaponAndCloth )
	{
		GameObjDbStructEx* HouseMainHand = NULL;
		GameObjDbStructEx* HouseOffHand = NULL;
		GameObjDbStructEx* MainHand = NULL;
		GameObjDbStructEx* OffHand = NULL;

		HouseItemDBStruct* HouseItemDBMainHand = GetHousesItemDB( ItemDBID , EM_EQWearPos_MainHand   );
		HouseItemDBStruct* HouseItemDBOffHand  = GetHousesItemDB( ItemDBID , EM_EQWearPos_SecondHand );

		if( HouseItemDBMainHand )
			HouseMainHand = CNetGlobal::GetObj( HouseItemDBMainHand->Item.OrgObjID );
		if( HouseItemDBOffHand )
			HouseOffHand = CNetGlobal::GetObj( HouseItemDBOffHand->Item.OrgObjID  );


		ItemFieldStruct* EqItemMainHand = CNetGlobal::RoleData()->GetEqItem( EM_EQWearPos_MainHand   );
		ItemFieldStruct* EqItemOffHand  = CNetGlobal::RoleData()->GetEqItem( EM_EQWearPos_SecondHand );

		if( EqItemMainHand && !EqItemMainHand->IsEmpty() )
			MainHand = CNetGlobal::GetObj( EqItemMainHand->OrgObjID );  

		if( EqItemOffHand && !EqItemOffHand->IsEmpty() )
			OffHand = CNetGlobal::GetObj( EqItemOffHand->OrgObjID );

		//雙手武器==>玩家
		if( HouseMainHand && 
			HouseMainHand->Item.Class == EM_Item_Weapon && 
			HouseMainHand->Item.WeaponPos == EM_WeaponPos_TwoHand )
		{
			if( Houses_CanWearObject( &HouseItemDBMainHand->Item , EM_EQWearPos_MainHand ) )
			{
				//先換副手 在主手
				NetCli_Houses::S_ExchangeEq( ItemDBID , EM_EQWearPos_SecondHand , EQWearPosENUM(EM_EQWearPos_SecondHand) );
				NetCli_Houses::S_ExchangeEq( ItemDBID , EM_EQWearPos_MainHand   , EQWearPosENUM(EM_EQWearPos_MainHand  ) );
			}
		}

		//雙手武器==>衣架
		else if(
			( bAll || HouseMainHand || HouseOffHand ) &&
	  		MainHand && 
			MainHand->Item.Class == EM_Item_Weapon && 
			MainHand->Item.WeaponPos == EM_WeaponPos_TwoHand  )
		{
			//衣架沒有武器 直接換
			if( !HouseMainHand && !HouseOffHand )
			{
				NetCli_Houses::S_ExchangeEq( ItemDBID , EM_EQWearPos_MainHand   , EQWearPosENUM(EM_EQWearPos_MainHand  ) );
				NetCli_Houses::S_ExchangeEq( ItemDBID , EM_EQWearPos_SecondHand , EQWearPosENUM(EM_EQWearPos_SecondHand) );
			}
			//衣架有主手 判斷玩家能不能裝備
			else if( HouseMainHand && !Houses_CanWearObject( &HouseItemDBMainHand->Item , EM_EQWearPos_MainHand ) )
			{
				//不能裝 不用換了
			}
			//衣架有副手 判斷玩家能不能裝備
			else if( HouseOffHand && !Houses_CanWearObject( &HouseItemDBOffHand->Item , EM_EQWearPos_SecondHand ) )
			{
				//副手丟到背包
				RoleDataEx* pRoleData = CNetGlobal::RoleData();
				int pos = pRoleData->SearchBodyFirstFit( &HouseItemDBOffHand->Item, false );
				//ItemFieldStruct* bodyItem = pRoleData->GetBodyItem(pos);
				if ( pos != -1 )
				{
					//交換物品
					NetCli_Houses::S_SwapBodyHouseItem( pos , EM_EQWearPos_SecondHand , ItemDBID , true );

					NetCli_Houses::S_ExchangeEq( ItemDBID , EM_EQWearPos_MainHand   , EQWearPosENUM(EM_EQWearPos_MainHand  ) );
				}
				else
				{
					g_pGameMain->SendWarningMsg( g_ObjectData->GetString("SYS_WEAR_EQ_ERR1") );
				}
			}
			else
			{
				NetCli_Houses::S_ExchangeEq( ItemDBID , EM_EQWearPos_MainHand   , EQWearPosENUM(EM_EQWearPos_MainHand  ) );
				NetCli_Houses::S_ExchangeEq( ItemDBID , EM_EQWearPos_SecondHand , EQWearPosENUM(EM_EQWearPos_SecondHand) );
			}
		}
		else if( bAll )
		{
			//全部交換
			NetCli_Houses::S_ExchangeEq( ItemDBID , EM_EQWearPos_MainHand   , EQWearPosENUM(EM_EQWearPos_MainHand  ) );
			NetCli_Houses::S_ExchangeEq( ItemDBID , EM_EQWearPos_SecondHand , EQWearPosENUM(EM_EQWearPos_SecondHand) );
		}
		else
		{
			//部分交換
			if( GetHousesItemDB( ItemDBID , EM_EQWearPos_MainHand ) )
			{
				NetCli_Houses::S_ExchangeEq( ItemDBID , EM_EQWearPos_MainHand , EQWearPosENUM(EM_EQWearPos_MainHand  ) );
			}
			if( GetHousesItemDB( ItemDBID , EM_EQWearPos_SecondHand ) )
			{
				NetCli_Houses::S_ExchangeEq( ItemDBID , EM_EQWearPos_SecondHand , EQWearPosENUM(EM_EQWearPos_SecondHand  ) );
			}
		}
  	}

}

// ----------------------------------------------------------------------------
bool CHousesFrame::Houses_CanWearObject( ItemFieldStruct* item , int eqPos )
{


	if( NULL == item )
	{
		return false;
	}

	GameObjDbStruct* itemDB = CNetGlobal::GetObj( item->OrgObjID );
	if (itemDB == NULL )
	{
		return false;
	}

	if( eqPos == EM_EQWearPos_SecondHand )
	{
		TempRoleData& TempData = CNetGlobal::RoleData()->TempData;

		if( itemDB->Item.Class == EM_Item_Weapon && !TempData.Attr.Fin.WearSkill.SecondHand )
		{
			return false;
		}
	}
 
	return CNetGlobal::RoleData()->CanWearObject( itemDB, -1, false, item->ImageObjectID );
}

// ----------------------------------------------------------------------------
void CHousesFrame::PickFurnishing( CRuVector3& point, CRuVector3& dir, CRoleSprite*& poutRoleSprite, float& outDistance )
{
	poutRoleSprite = NULL;

	// 建構碰撞測試結構
	CRuCollisionQuery_Segment_Nearest query;

	// 設定測試用線段及其他設定
	CRuVector3 pickDir = dir.Normalize();
	query.m_colSegment = CRuSegment3( point, point + ( pickDir * 1000.0f ) );
	query.m_ignoreBackface = TRUE;
	query.m_ignoreCollisionFlag = TRUE;

	float collisionT = 100000.0f;
	CRoleSprite* pRoleSprite = NULL;

	TD_HouseItemMap& rHouseItemMap = GetHouseFurnishingList();
	TD_HouseItemMap::iterator itPos = rHouseItemMap.begin();
	TD_HouseItemMap::iterator itEnd = rHouseItemMap.end();
	for( ; itPos != itEnd ; ++itPos )
	{
 		if( itPos->second->Layout.IsShow == false ||
			itPos->second->Pos >= m_HouseBaseInfo.MaxItemCount )
		{
			continue;
		}

		pRoleSprite = GetHouseRoleSpriteByItemDBID( itPos->first );

		if( NULL == pRoleSprite || NULL == pRoleSprite->GetRuEntity() )
		{
			continue;
		}

		pRoleSprite->GetRuEntity()->QueryCollision(&query);
		if( query.m_positiveHit )
		{
			if( query.m_collisionT < collisionT )
			{
 				collisionT = query.m_collisionT;

				poutRoleSprite = pRoleSprite;	
				outDistance = collisionT * 1000.0f;
			}
		}
	}

}


//SHousesServantInfo m_HousesServants[MAX_SERVANT_COUNT];
// ----------------------------------------------------------------------------
void CHousesFrame::UpdateServantWorkInfo( int index )
{
	HouseServantStruct& rServant = m_HouseBaseInfo.Servant[index]; 
	SHousesServantWorkInfo& rWorkInfo = m_ServantWorkInfos[index];
	HouseItemDBStruct* pHouseItemDB = g_pHousesFrame->GetHousesItemDB( DF_HouseServantListID, index );
	if( rServant.NPCObjID == 0 )
	{
		return;
	}

	if( NULL == pHouseItemDB )
	{
		return;
	}

	srand( (unsigned)( rServant.CreateTime / 100 ) );

	int _random = 0;

	int num = rServant.CreateTime % 100;

	do 
	{
		_random = rand();
		--num;
	} while ( num >= 0 );

	rWorkInfo.random = _random;

	//判斷有沒有道具
	ZeroMemory( rWorkInfo.item, sizeof(rWorkInfo.item) );

	TD_HouseItemMap &rHouseItemMap = GetHouseContainer( pHouseItemDB->ItemDBID );
	TD_HouseItemMap::iterator itPos = rHouseItemMap.begin();
	TD_HouseItemMap::iterator itEnd = rHouseItemMap.end();

	HouseItemDBStruct* pEqItemDB;

	for( ; itPos != itEnd ; ++itPos )
	{
		pEqItemDB = itPos->second;

		//rWorkInfo
		if( pEqItemDB->Pos < DF_ServantItemPos || pEqItemDB->Pos > DF_ServantItemPosEnd )
		{
			continue;
		}


		//207185		防身指南
		//207186		舞鞋
		//207188		雞毛撢子
		//207191		吉他
		//207193		掃把
		//207194		抹布

		if( pEqItemDB->Item.OrgObjID == 207185 )
		{
			rWorkInfo.item[6] = true;
		}
		else if( pEqItemDB->Item.OrgObjID == 207186 )
		{
			rWorkInfo.item[7] = true;
		}
		else if( pEqItemDB->Item.OrgObjID == 207188 )
		{
			rWorkInfo.item[1] = true;
		}
		else if( pEqItemDB->Item.OrgObjID == 207191 )
		{
			rWorkInfo.item[8] = true;
		}
		else if( pEqItemDB->Item.OrgObjID == 207193 )
		{
			rWorkInfo.item[3] = true;
		}
		else if( pEqItemDB->Item.OrgObjID == 207194 )
		{
			rWorkInfo.item[4] = true;
		}
	}

	
}

// ----------------------------------------------------------------------------
void CHousesFrame::UpdateServants( float elapsedTime )
{
	for( int i = 0; i < MAX_SERVANT_COUNT ; ++i )
	{
		UpdateServant( elapsedTime, i );
	}
}

// ----------------------------------------------------------------------------
void CHousesFrame::UpdateServant( float elapsedTime, int index  )
{
	if( index < 0 || index >= MAX_SERVANT_COUNT )
	{
		return;
	}

	HouseItemDBStruct* pServantItemDB = GetHousesItemDB( DF_HouseServantListID , index );
	HouseServantStruct* pServant = &m_HouseBaseInfo.Servant[index]; 
	SHousesServantWorkInfo& rWorkInfo = m_ServantWorkInfos[index];
	
	if( NULL == pServantItemDB )
	{
		return;
	}

	if( pServant->NPCObjID == 0 )
	{
		return;
	}

	CRoleSprite* pRoleSprite = GetHouseRoleSpriteByItemDBID( pServantItemDB->ItemDBID );

	if( NULL == pRoleSprite || false == pRoleSprite->IsVisible() )
	{
		if( rWorkInfo.isWorking )
		{
			rWorkInfo.isWorking = false;
		}
		return;
	}

	//招喚出來 需說一次話
	if( rWorkInfo.isSummon )
	{
		int sex = pServant->Sex;
		int suffix = pServant->Character % 100; //角色個性

		char talk[256];
		sprintf( talk, "C_HOUSESERVANT_HIRE_%d_%d", sex, suffix );

		ServantTalk( CHAT_MSG_WHISPER, index, talk );

		rWorkInfo.isSummon = false;
	}
	
	if( false == rWorkInfo.isWorking )
	{

//		int delay = ( rWorkInfo.random / 100 ) % 60;

 		int totleTime = 120 + ( ( rWorkInfo.random % 1000 ) / 100 );

		int beginTime = rWorkInfo.random % totleTime;
		int gameTime = ( (int)g_pGameMain->GetGameTime() ) % totleTime;

		
		if( gameTime == beginTime )
		{
			int mode = 0;
			//  要做什麼
			//0 發呆
			//1 抹布 → 擦家具
			//2 書本 → 讀書
			//3 掃帚 → 掃地
			//4 拖把 → 拖地
			//5 灑水器 → 灌溉種植
			//6 練習用武器 → 練習戰鬥
			//7 舞鞋 → 跳舞
			//8 小提琴 → 演奏

			gameTime = ( (int)g_pGameMain->GetGameTime() / 60 ) % 20;

			//機率
			int probability[9];

			for( int i = 0; i < 9 ; ++i )
			{
				if( rWorkInfo.item[i] )
				{
					probability[i] = 100;
				}
				else
				{
					probability[i] = 0;
				}

				if( i == 0 )
				{
					probability[i] = 200; //發呆
				}
			}

			int total = 0;
			for( int i = 0; i < 9 ; ++i )
			{
				total += probability[i];
			}

			if( total <= 0 )
			{
				return;
			}

			srand( (unsigned)( gameTime + rWorkInfo.random ) );
			int _random = 0;
			do 
			{
				_random = rand();
				--gameTime;
			} while ( gameTime >= 0 );

			_random = _random % total;

			for( int i = 0; i < 9 ; ++i )
			{
				_random -= probability[i];

				if( _random < 0 )
				{
					mode = i;
					break;
				}
			}

			if( 0 == mode )
			{

			}
			else
			{
				rWorkInfo.isWorking = true;
				rWorkInfo.workBeginTime = (int)g_pGameMain->GetGameTime();
				rWorkInfo.mode = mode;

				rWorkInfo.step = 0;

				//string strName = GetServantName( *pServant );
				//g_pGameMain->SendSystemChatMsgEx( "%s work Begin mode = %d   Time = %d", strName.c_str(), mode, rWorkInfo.workBeginTime );
			}
		}
	}

	//rWorkInfo.isWorking = false;

	if( false == rWorkInfo.isWorking )
	{
//		pRoleSprite->EnableTorsoTurn(false);

		//CRoleSprite* pPlayer = g_pGameMain->GetPlayer();
		pRoleSprite->GetRuActor()->SetMovementType( ruSOJMOVETYPE_FULL );

		CRuVector3 Dir = m_ServantLookAt - pRoleSprite->GetPos();
		Dir.Normalize();

 		static float angle = 45;
		float pi = angle * PI / 180.0f;

		CRuVector3 target; 
		//
		if( cosf(pi) < DotProduct( rWorkInfo.dirFoot , Dir ) )
		{
			target = Dir;
		}
		else
		{
 			target = rWorkInfo.dirFoot;
		}


		float fPercentage = 1.0f - powf( 0.6f , 3.0f * elapsedTime );
		rWorkInfo.dirHead += ( target - rWorkInfo.dirHead ) * fPercentage;

		pRoleSprite->SetDirection( rWorkInfo.dirHead );

		pRoleSprite->ReleaseHandsEntity( eRoleHands_Crafting );

		return ;
	}
	else
	{
		pRoleSprite->GetRuActor()->SetMovementType( ruSOJMOVETYPE_NPC );
	}

	rWorkInfo.pRoleSprite    =pRoleSprite;
	rWorkInfo.pServantItemDB = pServantItemDB; 
	rWorkInfo.pServant       = pServant;

	switch ( rWorkInfo.mode )
	{
	case 1: UpdateServantAction_1( elapsedTime, rWorkInfo ); break;
	case 2: UpdateServantAction_2( elapsedTime, rWorkInfo ); break;
	case 3: UpdateServantAction_3( elapsedTime, rWorkInfo ); break;
	case 4: UpdateServantAction_4( elapsedTime, rWorkInfo ); break;
	case 5: UpdateServantAction_5( elapsedTime, rWorkInfo ); break;
	case 6: UpdateServantAction_6( elapsedTime, rWorkInfo ); break;
	case 7: UpdateServantAction_7( elapsedTime, rWorkInfo ); break;
	case 8: UpdateServantAction_8( elapsedTime, rWorkInfo ); break;
	case 100: UpdateServantAction_100( elapsedTime, rWorkInfo ); break;
	default: 
		UpdateServantAction_0( elapsedTime, rWorkInfo );
	}

 
//	if( rWorkInfo.isWorking && g_pGameMain->GetGameTime() - rWorkInfo.workBeginTime > 30 )
//	{
//		rWorkInfo.isWorking = false;
//
//		string strName = GetServantName( rServant );
//		g_pGameMain->SendSystemChatMsgEx( "%s work end", strName.c_str() );
//	}

//	g_pGameMain->SendSystemChatMsgEx( "Time = %d", g_pGameMain->GetGameTime() );	


}


//掃把,抹布,雞毛撢子道具輸出
//T:\model\item\weapon\servant\act_wiper.ros
//T:\model\item\weapon\servant\act_duster.ros
//T:\model\item\weapon\servant\act_besom.ros
//T:\texture\item\weapon\servant\besom.dds
//T:\texture\item\weapon\servant\wiper.dds
//T:\texture\item\weapon\servant\duster.dds



// ----------------------------------------------------------------------------
//100 劇情 鎖定
void CHousesFrame::UpdateServantAction_100( float elapsedTime, SHousesServantWorkInfo& rWorkInfo )
{
	//超過2分鐘 UNLOCK

	rWorkInfo.pRoleSprite->ReleaseHandsEntity( eRoleHands_Crafting );

	if( g_pGameMain->GetGameTime() - rWorkInfo.workBeginTime > 120 )
	{
		rWorkInfo.isWorking = false;
	}	
}

// ----------------------------------------------------------------------------
//0 發呆
void CHousesFrame::UpdateServantAction_0( float elapsedTime, SHousesServantWorkInfo& rWorkInfo )
{
	if( g_pGameMain->GetGameTime() - rWorkInfo.workBeginTime > 1)
	{
		rWorkInfo.isWorking = false;

		//string strName = GetServantName( *rWorkInfo.pServant );
		//g_pGameMain->SendSystemChatMsgEx( "%s work end", strName.c_str() );
	}
}

// ----------------------------------------------------------------------------
//1 雞毛撢子 → 擦家具
void CHousesFrame::UpdateServantAction_1( float elapsedTime, SHousesServantWorkInfo& rWorkInfo )
{	
	//找家具
	if( rWorkInfo.step == 0 )
	{
		float fServantHeight = rWorkInfo.pRoleSprite->GetPos().m_y;

		vector<CRuVector3> vecTargetPoint; 
		vector<CRoleSprite*> vecSprite; 
		//
		CRoleSprite* pRoleSprite = NULL;

		TD_HouseItemMap& rHouseItemMap = GetHouseFurnishingList();
		TD_HouseItemMap::iterator itPos = rHouseItemMap.begin();
		TD_HouseItemMap::iterator itEnd = rHouseItemMap.end();
		for( ; itPos != itEnd ; ++itPos )
		{

			if( itPos->second->Layout.IsShow == false ||
				itPos->second->Pos >= m_HouseBaseInfo.MaxItemCount )
			{
				continue;
			}

			pRoleSprite = GetHouseRoleSpriteByItemDBID( itPos->first );

			if( NULL == pRoleSprite || NULL == pRoleSprite->GetRuEntity() )
			{
				continue;
			}

			//高度差太多的不要
			if( fabs( pRoleSprite->GetPos().m_y - fServantHeight ) > 2.0f  )
			{
				continue;
			}

			//能不能過去
			CRuVector3 vecPickPoint = rWorkInfo.pRoleSprite->GetPos(); 
			vecPickPoint.m_y += 0.5f;
			CRuVector3 dir = pRoleSprite->GetPos() - rWorkInfo.pRoleSprite->GetPos();
			dir.Normalize();

			CRoleSprite* poutRoleSprite;
			float outDistance;

			CRoleSprite* pRoleSpritePick = NULL;
			float fDistancePick = 0;


			for( int i = 0 ; i < 10 ; ++i )
			{
				PickFurnishing( vecPickPoint, dir, poutRoleSprite, outDistance );

				if( poutRoleSprite )
				{
					if( NULL == pRoleSpritePick || outDistance < fDistancePick )
					{
						pRoleSpritePick = poutRoleSprite;
						fDistancePick = outDistance;
					}
				}
				
				//			
				vecPickPoint.m_y += 2.0f; 
			}

			//被其他東西擋住了
			if( pRoleSpritePick != pRoleSprite )
			{
				continue;
			}

			float fDistance = ( pRoleSprite->GetPos() - rWorkInfo.pRoleSprite->GetPos() ).Magnitude();

			if( fDistancePick < fDistance )
			{
				fDistance = fDistancePick;
			}

			//要移動到的位置
			fDistance = fDistancePick - 8.0f;

			if( fDistance <= 0 )
			{
				continue;
			}
			
			CRuVector3 vecTarget = rWorkInfo.pRoleSprite->GetPos() + ( dir * fDistance ) ;

			vecTargetPoint.push_back( vecTarget );
			vecSprite.push_back( pRoleSprite );
		}

		int count = (int)vecSprite.size();
		if( count == 0 )
		{
			rWorkInfo.isWorking = false;
			return;
		}


		int index = rWorkInfo.workBeginTime % count;

		CRuVector3 vecTarget = vecTargetPoint[index];
		pRoleSprite = vecSprite[index];

		rWorkInfo.vecTargetPos = vecTarget;

		rWorkInfo.pRoleSprite->SetTargetPos( vecTarget );

		++rWorkInfo.step;
		return;
	}

	//判斷是否移動到目標了
	if( rWorkInfo.step == 1 )
	{
		float fDistance = ( rWorkInfo.vecTargetPos - rWorkInfo.pRoleSprite->GetPos() ).Magnitude();

		if( fDistance < 5.0 && rWorkInfo.pRoleSprite->GetMoveVector() == 0.0f )
		{
			++rWorkInfo.step;
			return;
		}
 	}

	if( rWorkInfo.step == 2 )
	{
		rWorkInfo.pRoleSprite->SetActorState( ruFUSION_ACTORSTATE_EMOTE_SWEEP2 );
		rWorkInfo.pRoleSprite->CreateHandsEntity( eRoleHands_Crafting, 207188, 0 );

		++rWorkInfo.step;

		rWorkInfo.workBeginTime = (int)g_pGameMain->GetGameTime();
		return;
	}
		
	if( rWorkInfo.step == 3 && g_pGameMain->GetGameTime() - rWorkInfo.workBeginTime > 30 )
	{
		rWorkInfo.pRoleSprite->SetActorState( ruFUSION_ACTORSTATE_NORMAL );
		rWorkInfo.pRoleSprite->ReleaseHandsEntity( eRoleHands_Crafting );

		++rWorkInfo.step;
		return;
	}

	if( rWorkInfo.step == 4 )
	{
		CRuFusion_ActorState ActorState = rWorkInfo.pRoleSprite->GetDisplayActorState();
 		if( ActorState == ruFUSION_ACTORSTATE_NORMAL )
		{
			rWorkInfo.vecTargetPos.x = rWorkInfo.pServantItemDB->Layout.X;
			rWorkInfo.vecTargetPos.y = rWorkInfo.pServantItemDB->Layout.Y;
			rWorkInfo.vecTargetPos.z = rWorkInfo.pServantItemDB->Layout.Z;
			rWorkInfo.pRoleSprite->SetTargetPos( rWorkInfo.vecTargetPos );

			++rWorkInfo.step;
			return;
		}
	}

	//判斷是否移動到目標了
	if( rWorkInfo.step == 5 )
	{
		float fDistance = ( rWorkInfo.vecTargetPos - rWorkInfo.pRoleSprite->GetPos() ).Magnitude();

		if( fDistance < 5.0 && rWorkInfo.pRoleSprite->GetMoveVector() == 0.0f )
		{
			rWorkInfo.isWorking = false;

			//string strName = GetServantName( *rWorkInfo.pServant );
			//g_pGameMain->SendSystemChatMsgEx( "%s work end", strName.c_str() );

			++rWorkInfo.step;
			return;
		}
	}
	
	if( g_pGameMain->GetGameTime() - rWorkInfo.workBeginTime > 60 )
	{
		rWorkInfo.pRoleSprite->SetActorState( ruFUSION_ACTORSTATE_CRAFTING_END );	

		CRuVector3 vecPos;
		vecPos.x = rWorkInfo.pServantItemDB->Layout.X;
		vecPos.y = rWorkInfo.pServantItemDB->Layout.Y;
		vecPos.z = rWorkInfo.pServantItemDB->Layout.Z;

		rWorkInfo.pRoleSprite->SetPos( vecPos );

		rWorkInfo.isWorking = false;

		//string strName = GetServantName( *rWorkInfo.pServant );
		//g_pGameMain->SendSystemChatMsgEx( "%s work end", strName.c_str() );
	}
}

// ----------------------------------------------------------------------------
//2 書本 → 讀書
void CHousesFrame::UpdateServantAction_2( float elapsedTime, SHousesServantWorkInfo& rWorkInfo )
{
	if( rWorkInfo.step == 0 )
	{
		rWorkInfo.pRoleSprite->SetActorState( ruFUSION_ACTORSTATE_READING_BEGIN );
		rWorkInfo.pRoleSprite->CreateHandsEntity( eRoleHands_Crafting, 0, 0 );
		rWorkInfo.step++;
	}

	if( g_pGameMain->GetGameTime() - rWorkInfo.workBeginTime > 30 )
	{
		rWorkInfo.pRoleSprite->SetActorState( ruFUSION_ACTORSTATE_READING_END );
		rWorkInfo.pRoleSprite->ReleaseHandsEntity( eRoleHands_Crafting );
		rWorkInfo.isWorking = false;

		//string strName = GetServantName( *rWorkInfo.pServant );
		//g_pGameMain->SendSystemChatMsgEx( "%s work end", strName.c_str() );
	}
}

// ----------------------------------------------------------------------------
//3 掃帚 → 掃地
void CHousesFrame::UpdateServantAction_3( float elapsedTime, SHousesServantWorkInfo& rWorkInfo )
{
	if( rWorkInfo.step == 0 )
	{
		rWorkInfo.pRoleSprite->SetActorState( ruFUSION_ACTORSTATE_EMOTE_SWEEP1 );
		rWorkInfo.pRoleSprite->CreateHandsEntity( eRoleHands_Crafting, 207193, 0 );
		rWorkInfo.step++;
	}

	if( g_pGameMain->GetGameTime() - rWorkInfo.workBeginTime > 30 )
	{
		rWorkInfo.pRoleSprite->SetActorState( ruFUSION_ACTORSTATE_NORMAL );
		rWorkInfo.pRoleSprite->ReleaseHandsEntity( eRoleHands_Crafting );
		rWorkInfo.isWorking = false;

		//string strName = GetServantName( *rWorkInfo.pServant );
		//g_pGameMain->SendSystemChatMsgEx( "%s work end", strName.c_str() );
	}
}

// ----------------------------------------------------------------------------
//4 拖把 → 拖地
void CHousesFrame::UpdateServantAction_4( float elapsedTime, SHousesServantWorkInfo& rWorkInfo )
{
	if( rWorkInfo.step == 0 )
	{
		rWorkInfo.pRoleSprite->SetActorState( ruFUSION_ACTORSTATE_EMOTE_SWEEP3 );
		rWorkInfo.pRoleSprite->CreateHandsEntity( eRoleHands_Crafting, 207194, 0 );
		rWorkInfo.step++;
	}

	if( g_pGameMain->GetGameTime() - rWorkInfo.workBeginTime > 30 )
	{
		rWorkInfo.pRoleSprite->SetActorState( ruFUSION_ACTORSTATE_NORMAL );
		rWorkInfo.pRoleSprite->ReleaseHandsEntity( eRoleHands_Crafting );
		rWorkInfo.isWorking = false;

		//string strName = GetServantName( *rWorkInfo.pServant );
		//g_pGameMain->SendSystemChatMsgEx( "%s work end", strName.c_str() );
	}
}

// ----------------------------------------------------------------------------
//5 灑水器 → 灌溉種植
void CHousesFrame::UpdateServantAction_5( float elapsedTime, SHousesServantWorkInfo& rWorkInfo )
{
	if( g_pGameMain->GetGameTime() - rWorkInfo.workBeginTime > 1)
	{
		rWorkInfo.isWorking = false;

		//string strName = GetServantName( *rWorkInfo.pServant );
		//g_pGameMain->SendSystemChatMsgEx( "%s work end", strName.c_str() );
	}
}

// ----------------------------------------------------------------------------
//6 練習用武器 → 練習戰鬥
void CHousesFrame::UpdateServantAction_6( float elapsedTime, SHousesServantWorkInfo& rWorkInfo )
{
	CRuFusion_ActorState ActorState = rWorkInfo.pRoleSprite->GetDisplayActorState();

	if( ActorState != ruFUSION_ACTORSTATE_ATTACK_UNARMED &&
		ActorState != ruFUSION_ACTORSTATE_ATTACK_UNARMED_SP )
	{
		if( rand() % 2 == 1 )
		{
			rWorkInfo.pRoleSprite->SetActorState( ruFUSION_ACTORSTATE_ATTACK_UNARMED );
		}
		else
		{
			rWorkInfo.pRoleSprite->SetActorState( ruFUSION_ACTORSTATE_ATTACK_UNARMED_SP );
		}
	}
 
	if( g_pGameMain->GetGameTime() - rWorkInfo.workBeginTime > 30 )
	{

		rWorkInfo.isWorking = false;

		//string strName = GetServantName( *rWorkInfo.pServant );
		//g_pGameMain->SendSystemChatMsgEx( "%s work end", strName.c_str() );
	}
}

// ----------------------------------------------------------------------------
//7 舞鞋 → 跳舞
void CHousesFrame::UpdateServantAction_7( float elapsedTime, SHousesServantWorkInfo& rWorkInfo )
{
	//if( rWorkInfo.step == 0 )
	//{
	//	rWorkInfo.pRoleSprite->SetActorState( ruFUSION_ACTORSTATE_EMOTE_DANCE );
	//	rWorkInfo.step++;
	//}

	CRuFusion_ActorState ActorState = rWorkInfo.pRoleSprite->GetDisplayActorState();

	if( ActorState != ruFUSION_ACTORSTATE_EMOTE_DANCE )
	{
		rWorkInfo.pRoleSprite->SetActorState( ruFUSION_ACTORSTATE_EMOTE_DANCE );
	}
 
	if( g_pGameMain->GetGameTime() - rWorkInfo.workBeginTime > 30 )
	{
		rWorkInfo.isWorking = false;

		//string strName = GetServantName( *rWorkInfo.pServant );
		//g_pGameMain->SendSystemChatMsgEx( "%s work end", strName.c_str() );
	}
}

// ----------------------------------------------------------------------------
//8 小提琴 → 演奏
void CHousesFrame::UpdateServantAction_8( float elapsedTime, SHousesServantWorkInfo& rWorkInfo )
{
	//if( rWorkInfo.step == 0 )
	//{
	//	rWorkInfo.pRoleSprite->SetActorState( ruFUSION_ACTORSTATE_EMOTE_PLAY_GUITAR );
	//	rWorkInfo.step++;
	//}

	CRuFusion_ActorState ActorState = rWorkInfo.pRoleSprite->GetDisplayActorState();

	if( ActorState != ruFUSION_ACTORSTATE_EMOTE_PLAY_GUITAR )
	{
		rWorkInfo.pRoleSprite->SetActorState( ruFUSION_ACTORSTATE_EMOTE_PLAY_GUITAR );
		rWorkInfo.pRoleSprite->CreateHandsEntity( eRoleHands_Crafting, 207191, 0 );
		
	}

	if( g_pGameMain->GetGameTime() - rWorkInfo.workBeginTime > 30 )
	{
		rWorkInfo.isWorking = false;

		rWorkInfo.pRoleSprite->ReleaseHandsEntity( eRoleHands_Crafting );

		//string strName = GetServantName( *rWorkInfo.pServant );
		//g_pGameMain->SendSystemChatMsgEx( "%s work end", strName.c_str() );
	}
}

// ----------------------------------------------------------------------------
void CHousesFrame::Plant_PickupItem( EPlantItemType type )
{
	ClipboardItemType itemType = g_pItemClipboard->GetType();
	ClipboardItemInfo itemInfo = g_pItemClipboard->GetItemInfo();
	g_pItemClipboard->Clear();

	if( itemType != eClipboardItem_Type_Bag )
	{
		return ;
	}

	int index = itemInfo.bag.pos;
	ItemFieldStruct* item = CNetGlobal::RoleData()->GetBodyItem( index );
	if( NULL == item )
	{
		return;
	}

	GameObjDbStructEx* pItemDB = CNetGlobal::GetObj( item->OrgObjID );
	if( NULL == pItemDB )
	{
		return;
	}

	//種植等級
	int SkillLV = (int) CNetGlobal::RoleData()->TempData.Attr.Fin.SkillValue.GetSkill( EM_SkillValueType_Plant );

	switch( type )
	{
	case PIT_Seed:
		if( !pItemDB->IsSeed() )  
		{
			g_pGameMain->SendSystemMsg( g_ObjectData->GetString("SYS_HOUSE_NOT_SEED") );
 			g_pGameMain->SendSystemChatMsg( g_ObjectData->GetString("SYS_HOUSE_NOT_SEED") );
			return;
		}
		//檢查種植等級
		if( pItemDB->Item.Plant.Level > SkillLV )
		{
 			g_pGameMain->SendSystemMsg( g_ObjectData->GetString("SYS_PLANT_LV_ERR_SEED") );
			g_pGameMain->SendSystemChatMsg( g_ObjectData->GetString("SYS_PLANT_LV_ERR_SEED") );
			return;
		}
		m_ObjectID_Seed = item->OrgObjID;
		break;
	case PIT_Water:      
		if( !pItemDB->IsManure() || pItemDB->Item.Plant.Manure.Type != EM_ManureType_Water )  
		{
			g_pGameMain->SendSystemMsg( g_ObjectData->GetString("SYS_HOUSE_NOT_WATER") );
			g_pGameMain->SendSystemChatMsg( g_ObjectData->GetString("SYS_HOUSE_NOT_WATER") );
			return;
		}
		//檢查種植等級
		if( pItemDB->Item.Plant.Level > SkillLV )
		{
			g_pGameMain->SendSystemMsg( g_ObjectData->GetString("SYS_PLANT_LV_ERR_WATER") );
			g_pGameMain->SendSystemChatMsg( g_ObjectData->GetString("SYS_PLANT_LV_ERR_WATER") );
			return;
		}
		m_ObjectID_Water = item->OrgObjID;
		break;
	case PIT_Topdress:
		if( !pItemDB->IsManure() || pItemDB->Item.Plant.Manure.Type != EM_ManureType_Topdress )  
		{
			g_pGameMain->SendSystemMsg( g_ObjectData->GetString("SYS_HOUSE_NOT_TOPDRESS") );
			g_pGameMain->SendSystemChatMsg( g_ObjectData->GetString("SYS_HOUSE_NOT_TOPDRESS") );
			return;
		}
		//檢查種植等級
		if( pItemDB->Item.Plant.Level > SkillLV )
		{
			g_pGameMain->SendSystemMsg( g_ObjectData->GetString("SYS_PLANT_LV_ERR_TOPDRESS") );
			g_pGameMain->SendSystemChatMsg( g_ObjectData->GetString("SYS_PLANT_LV_ERR_TOPDRESS") );
			return;
		}
		m_ObjectID_Topdress = item->OrgObjID;
		break;
	case PIT_MagicWater:
		if( !pItemDB->IsManure() || pItemDB->Item.Plant.Manure.Type != EM_ManureType_MagicWater )  
		{
			g_pGameMain->SendSystemMsg( g_ObjectData->GetString("SYS_HOUSE_NOT_MAGICWATER") );
			g_pGameMain->SendSystemChatMsg( g_ObjectData->GetString("SYS_HOUSE_NOT_MAGICWATER") );
			return;
		}
		//檢查種植等級
		if( pItemDB->Item.Plant.Level > SkillLV )
		{
			g_pGameMain->SendSystemMsg( g_ObjectData->GetString("SYS_PLANT_LV_ERR_MAGICWATER") );
			g_pGameMain->SendSystemChatMsg( g_ObjectData->GetString("SYS_PLANT_LV_ERR_MAGICWATER") );
			return;
		}
		m_ObjectID_MagicWater = item->OrgObjID;
		break;
	}

	SendWorldEvent( UiEvent_HousesPlantChanged );
}

// ----------------------------------------------------------------------------
void CHousesFrame::Plant_ClearItem( EPlantItemType type )
{
	switch( type )
	{
	case PIT_Seed:       m_ObjectID_Seed       = -1; break;
	case PIT_Water:      m_ObjectID_Water      = -1; break;
	case PIT_Topdress:   m_ObjectID_Topdress   = -1; break;
	case PIT_MagicWater: m_ObjectID_MagicWater = -1; break;
	}

	SendWorldEvent( UiEvent_HousesPlantChanged );
}

// ----------------------------------------------------------------------------
void CHousesFrame::Plant_Grow( EPlantItemType type )
{
	switch( type )
	{
	case PIT_Seed:
		{
			int index = g_pBagFrame->GetIndex( m_ObjectID_Seed );
			if( index >= 0 )
			{
				NetCli_Houses::S_PlantItemRequest( EM_PlantItem_Seed, m_PlantFurnishingPos, index );
			}
			m_ObjectID_Seed = -1;
		}
		break;
	case PIT_Water:
		{
			int index = g_pBagFrame->GetIndex( m_ObjectID_Water );
			if( index >= 0 )
			{
				NetCli_Houses::S_PlantItemRequest( EM_PlantItem_Water, m_PlantFurnishingPos, index );
			}

		}
		break;
	case PIT_Topdress:
		{
			int index = g_pBagFrame->GetIndex( m_ObjectID_Topdress );
			if( index >= 0 )
			{
				NetCli_Houses::S_PlantItemRequest( EM_PlantItem_Topdress, m_PlantFurnishingPos, index );
			}

		} 
		break;
	case PIT_MagicWater:
		{
			int index = g_pBagFrame->GetIndex( m_ObjectID_MagicWater );
			if( index >= 0 )
			{
				NetCli_Houses::S_PlantItemRequest( EM_PlantItem_MagicWater, m_PlantFurnishingPos, index );
			}

		}  	
		break;
	}
}

// ----------------------------------------------------------------------------
void CHousesFrame::Plant_Clear()
{
	NetCli_Houses::S_PlantClearRequest( m_PlantFurnishingPos );
}

// ----------------------------------------------------------------------------
void CHousesFrame::Plant_Lock()
{
	NetCli_Houses::S_PlantLockRequest( m_PlantFurnishingPos );
}

// ----------------------------------------------------------------------------
void CHousesFrame::Plant_GetProduct()
{
	NetCli_Houses::S_PlantGetProductRequest( m_PlantFurnishingPos );
}

// ----------------------------------------------------------------------------
void CHousesFrame::Plant_Close()
{
	m_PlantFurnishingDBID = -1;
	m_PlantFurnishingPos  = -1;
	m_ObjectID_Seed = -1;	
}

// ----------------------------------------------------------------------------
void CHousesFrame::UpdateHouseFriend()
{
	m_bIsFriend = false;
	if( !m_bIsOwner )
	{
		int playerDBID = g_pGameMain->GetDBID();

		std::vector<int>::iterator itPos = m_FriendDBIDTable.begin();
		std::vector<int>::iterator itEnd = m_FriendDBIDTable.end();

		for( ; itPos != itEnd ; ++itPos )
		{
			if( playerDBID == *itPos )
			{
				m_bIsFriend = true;
				break;
			}
		}
	}	
}

// ----------------------------------------------------------------------------
void CHousesFrame::R_FriendDBID( int HouseDBID , int FriendDBID )
{
	if( m_HouseBaseInfo.HouseDBID != HouseDBID )
	{
		return;
	}

	int i;
	for( i = 0 ; i < (int)m_FriendDBIDTable.size() ; ++i )
	{
		if( m_FriendDBIDTable[i] == FriendDBID )
		{
			return;
		}
	}

	m_FriendDBIDTable.push_back( FriendDBID );

	UpdateHouseFriend();

	SendWorldEvent( UiEvent_HousesFriendChanged );
}

// ----------------------------------------------------------------------------
void CHousesFrame::R_DeleteFriendDBID( int FriendDBID )
{
	
	std::vector<int>::iterator itPos = m_FriendDBIDTable.begin();
	std::vector<int>::iterator itEnd = m_FriendDBIDTable.end();

	for( ; itPos != itEnd ; ++itPos )
	{
		if( *itPos == FriendDBID )
		{
			m_FriendDBIDTable.erase( itPos );
			break;
		}
	}

	UpdateHouseFriend();

	SendWorldEvent( UiEvent_HousesFriendChanged );


	FindDBIDRoleNameStruct* findStruct = NetCli_OtherChild::GetFindDBIDRoleNameStruct( FriendDBID );
	if( NULL == findStruct || false == findStruct->IsFind )
	{
		return ;
	}

	g_pGameMain->SendSystemChatMsgEx( g_ObjectData->GetString( "SYS_HOUSE_DELETE_FRIEND_SUCCESS" ), findStruct->RoleName.c_str() );
}

// ----------------------------------------------------------------------------
void CHousesFrame::R_AddFriendDBID( int FriendDBID )
{
	int i;
	for( i = 0 ; i < (int)m_FriendDBIDTable.size() ; ++i )
	{
		if( m_FriendDBIDTable[i] == FriendDBID )
		{
			return;
		}
	}

	m_FriendDBIDTable.push_back( FriendDBID );

	UpdateHouseFriend();

	SendWorldEvent( UiEvent_HousesFriendChanged );
  
	FindDBIDRoleNameStruct* findStruct = NetCli_OtherChild::GetFindDBIDRoleNameStruct( FriendDBID );
	if( NULL == findStruct || false == findStruct->IsFind )
	{
		return ;
	}

  	g_pGameMain->SendSystemChatMsgEx( g_ObjectData->GetString( "SYS_HOUSE_ADD_FRIEND_SUCCESS" ), findStruct->RoleName.c_str() );
}

// ----------------------------------------------------------------------------
void CHousesFrame::R_FriendItemLog( HouseFriendItemLogStruct& Log )
{
	m_HouseBaseInfo.AddItemLog( Log );
 	SendWorldEvent( UiEvent_HousesFriendItemLogChanged );
}

// ----------------------------------------------------------------------------
void CHousesFrame::StopHousesSound()
{
	if( m_HousesSoundHandle != 0 )
	{
		//g_ruSymphony->FadeOutSound( m_HousesSoundHandle, 2000.0f );
		g_ruAudible->Stop(m_HousesSoundHandle, 0.250f);			
		m_HousesSoundHandle = 0;
		m_HousesSoundFurnishingDBID = -1;
	}
}

// ----------------------------------------------------------------------------
void CHousesFrame::PlayHousesSound()
{
	RuPropertyType propertyValue;
	REAL SoundLevel = 0;
	if( g_ruEngineSettings->GetSetting("audioFX:music volume", propertyValue) )
	{
		SoundLevel = boost::get<REAL>(propertyValue);
	}
	
	HouseItemDBStruct* pHouseItemDB = NULL;
	GameObjDbStructEx* pItemDB = NULL;

	TD_HouseItemMap &rHouseItemMap = GetHouseFurnishingList();
	TD_HouseItemMap::iterator itPos = rHouseItemMap.begin();
	TD_HouseItemMap::iterator itEnd = rHouseItemMap.end();

	int findDBID = -1;

	for( ; itPos != itEnd ; ++itPos )
	{
		pHouseItemDB = itPos->second;

		pItemDB = CNetGlobal::GetObj( pHouseItemDB->Item.OrgObjID );

		if( pHouseItemDB->Layout.IsSound )
		{
			if( m_bIsOwner )
			{
				if( false == pHouseItemDB->Layout.IsShow ||
					pItemDB->Item.FurnitureSoundID <= 0 ||
					pHouseItemDB->Pos >= m_HouseBaseInfo.MaxItemCount ||
					findDBID >= 0 )
				{
					pHouseItemDB->Layout.IsSound = false;

					NetCli_Houses::S_HouseItemLayoutRequest( pHouseItemDB->ItemDBID, pHouseItemDB->Layout );
				}
			}
			findDBID = pHouseItemDB->ItemDBID;
		}
	}

	if( m_HousesSoundFurnishingDBID != findDBID )
	{
		StopHousesSound();

		m_HousesSoundFurnishingDBID = findDBID;

		if( m_HousesSoundFurnishingDBID >= 0 )
		{
			HouseItemDBStruct* pHouseItemDB = GetHousesItemDBByDBID( m_HousesSoundFurnishingDBID );
			pItemDB = CNetGlobal::GetObj( pHouseItemDB->Item.OrgObjID );

			if( pHouseItemDB )
			{
				string strSoundFile = Com_FormatString( "music\\zone\\400\\%d.mp3", pItemDB->Item.FurnitureSoundID );
				//m_HousesSoundHandle = g_ruSymphony->PlaySoundByPath( strSoundFile.c_str() , TRUE, SoundLevel, ruSOUNDTYPE_MUSIC );
				int loopCount = (TRUE) ? 0x7FFFFFFF : 1;
				m_HousesSoundHandle = g_ruAudible->Play(strSoundFile.c_str(), (CRuAudible::AudibleType)ruSOUNDTYPE_MUSIC, (loopCount > 1) ? TRUE : FALSE);					
			}
		}
	}

	if( !m_HousesSoundHandle )
	{
		//m_HousesSoundHandle = g_ruSymphony->PlaySoundByPath( "music\\zone\\400\\background.mp3" , TRUE, SoundLevel, ruSOUNDTYPE_MUSIC );
		int loopCount = (FALSE) ? 0x7FFFFFFF : 1;
		m_HousesSoundHandle = g_ruAudible->Play("music\\zone\\400\\background.mp3", (CRuAudible::AudibleType)ruSOUNDTYPE_MUSIC, (loopCount > 1) ? TRUE : FALSE);			
	}

}

// ----------------------------------------------------------------------------
void CHousesFrame::StopAllFurnishingSound()
{
	if( !m_bIsOwner )
	{
		return ;
	}

	HouseItemDBStruct* pHouseItemDB = NULL;
	GameObjDbStructEx* pItemDB = NULL;

	TD_HouseItemMap &rHouseItemMap = GetHouseFurnishingList();
	TD_HouseItemMap::iterator itPos = rHouseItemMap.begin();
	TD_HouseItemMap::iterator itEnd = rHouseItemMap.end();

	for( ; itPos != itEnd ; ++itPos )
	{
		pHouseItemDB = itPos->second;

		pItemDB = CNetGlobal::GetObj( pHouseItemDB->Item.OrgObjID );

		if( pHouseItemDB->Layout.IsSound )
		{
			pHouseItemDB->Layout.IsSound = false;
			NetCli_Houses::S_HouseItemLayoutRequest( pHouseItemDB->ItemDBID, pHouseItemDB->Layout );
		}
	}

	PlayHousesSound();
}

// ----------------------------------------------------------------------------
void CHousesFrame::PlayFurnishingSound( int DBID )
{
	if( !m_bIsOwner )
	{
		return ;
	}

	HouseItemDBStruct* pHouseItemDB = NULL;
	GameObjDbStructEx* pItemDB = NULL;

	TD_HouseItemMap &rHouseItemMap = GetHouseFurnishingList();
	TD_HouseItemMap::iterator itPos = rHouseItemMap.begin();
	TD_HouseItemMap::iterator itEnd = rHouseItemMap.end();

	for( ; itPos != itEnd ; ++itPos )
	{
		pHouseItemDB = itPos->second;

		pItemDB = CNetGlobal::GetObj( pHouseItemDB->Item.OrgObjID );

		if( pHouseItemDB->ItemDBID == DBID )
		{
			if( !pHouseItemDB->Layout.IsSound )
			{
				pHouseItemDB->Layout.IsSound = true;
				NetCli_Houses::S_HouseItemLayoutRequest( pHouseItemDB->ItemDBID, pHouseItemDB->Layout );
			}

			continue;
		}

		if( pHouseItemDB->Layout.IsSound )
		{
			pHouseItemDB->Layout.IsSound = false;
			NetCli_Houses::S_HouseItemLayoutRequest( pHouseItemDB->ItemDBID, pHouseItemDB->Layout );
		}
	}

	PlayHousesSound();
}


// ----------------------------------------------------------------------------
int LuaFunc_Houses_VisitHouseRequest( lua_State* L )
{
	const char* HouseDBID = luaL_checkstring( L, 1 ) ;
	const char* Password  = luaL_checkstring( L, 2 ) ;


	if( 0 == strlen( HouseDBID ) )
	{
		return 0;
	}

	if( HouseDBID[0] >= '0' &&	HouseDBID[0] <= '9' )
	{
		NetCli_Houses::S_VisitHouseRequest( atoi( HouseDBID ) , (char*)Password );
	}
	else
	{
		NetCli_Houses::S_VisitHouseRequest_ByName( (char*)HouseDBID, (char*)Password );
	}

	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_Houses_CloseVisitHouse( lua_State* L )
{
	NetCli_Houses::S_CloseVisitHouse();	
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_Houses_GetHouseInfo( lua_State* L )
{
	TD_HouseItemMap &rHouseItemMap = g_pHousesFrame->GetHouseFurnishingList();

	vector<HouseShopInfoStruct>& rTable = g_ObjectData->HouseShop();

	int FunitureCount = 0;
	int	OpenEnergy = 0;
	int	KeppEnergy = 0;

	int i;
	for( i = 0 ; i < (int)rTable.size() ; ++i )
	{
		if( g_pHousesFrame->m_HouseBaseInfo.ShopFunction.GetFlag( i ) && 
			rTable[i].Type == 0 )  //( 0 表示為傢俱格子 1 表示其它類 )
		{
			FunitureCount = rTable[i].FunitureCount;
			OpenEnergy = rTable[i].OpenEnergy; 
			KeppEnergy = rTable[i].KeppEnergy;
		}
	}
	const char* HouseName = g_pHousesFrame->m_HouseBaseInfo.HouseName;
	if( HouseName[0] == '\0' )
	{
		HouseName = g_ObjectData->GetString( "ZONE_DEFAULT" );
	}

	int HouseDBID    = g_pHousesFrame->m_HouseBaseInfo.HouseDBID;
	int HouseType    = g_pHousesFrame->m_HouseBaseInfo.HouseType;
	int ItemCount    = (int)rHouseItemMap.size();
	int MaxItemCount = g_pHousesFrame->m_HouseBaseInfo.MaxItemCount;
	int PerDay       = KeppEnergy; //每日所需能量
   	int EnergyPoint  = g_pHousesFrame->m_HouseBaseInfo.EnergyPoint ; //房屋能量
	bool bIsOwner    = g_pHousesFrame->m_bIsOwner;

	int ExpPoint          = g_pHousesFrame->m_ExpPoint         ;
	int TpPoint           = g_pHousesFrame->m_TpPoint          ;
	int BlackSmithHQPoint = g_pHousesFrame->m_BlackSmithHQPoint;
	int CarpenterHQPoint  = g_pHousesFrame->m_CarpenterHQPoint ;	
	int MakeArmorHQPoint  = g_pHousesFrame->m_MakeArmorHQPoint ;	
	int TailorHQPoint     = g_pHousesFrame->m_TailorHQPoint    ;		
	int CookHQPoint       = g_pHousesFrame->m_CookHQPoint      ;		
	int AlchemyHQPoint    = g_pHousesFrame->m_AlchemyHQPoint   ;

 	lua_pushstring( L, HouseName    ); //房屋名稱
	lua_pushnumber( L, HouseDBID    ); //房屋號碼
	lua_pushnumber( L, HouseType    ); //房屋格局
	lua_pushnumber( L, ItemCount    ); //使用空間
	lua_pushnumber( L, MaxItemCount ); //最大空間
	lua_pushnumber( L, PerDay       ); //每日所需能量
  	lua_pushnumber( L, EnergyPoint  ); //房屋能量
	lua_pushboolean( L, bIsOwner    ); //
	lua_pushnumber( L, ExpPoint          ); //訓練效益
	lua_pushnumber( L, TpPoint           ); //學習效益
	lua_pushnumber( L, BlackSmithHQPoint ); //工作效益
	lua_pushnumber( L, CarpenterHQPoint  ); //工作效益
	lua_pushnumber( L, MakeArmorHQPoint  ); //工作效益
	lua_pushnumber( L, TailorHQPoint     ); //工作效益
	lua_pushnumber( L, CookHQPoint       ); //工作效益
	lua_pushnumber( L, AlchemyHQPoint    ); //工作效益

	return 16;
}

// ----------------------------------------------------------------------------
int LuaFunc_Houses_IsOwner( lua_State* L )
{
	lua_pushboolean( L, g_pHousesFrame->m_bIsOwner ); //
	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_Houses_IsFriend( lua_State* L )
{
	lua_pushboolean( L, g_pHousesFrame->m_bIsFriend ); //
	return 1;
}

// ----------------------------------------------------------------------------
void CHousesFrame::R_PlantClearResult( int HousePos , PlantClearResultTypeENUM Result )
{
	const char* szMsg = "";

	switch( Result )
	{
	case EM_PlantClearResultType_OK     : szMsg = g_ObjectData->GetString( "SYS_PLANT_CLEAR_SUCCEED" ); break;   
	case EM_PlantClearResultType_Failed : szMsg = g_ObjectData->GetString( "SYS_PLANT_CLEAR_FAILED"  ); break; 
	default:
		return;
		break;
	}

	g_pGameMain->SendSystemChatMsg( szMsg );
}

// ----------------------------------------------------------------------------
void CHousesFrame::R_PlantItemResult( PlantItemTypeENUM Type , int PotHousePos , int ItemBodyPos , PlantItemResultTypeENUM Result  )
{
	if( PotHousePos == m_PlantFurnishingPos )
	{
		SendWorldEvent( UiEvent_HousesPlantChanged );
	}

	string strMsg = "SYS_PLANT_" ;

	switch( Type )
	{
	case EM_PlantItem_Seed       : strMsg += "SEED"      ; break;   
	case EM_PlantItem_Water      : strMsg += "WATER"     ; break; 
	case EM_PlantItem_Topdress   : strMsg += "TOPDRESS"  ; break;   
	case EM_PlantItem_MagicWater : strMsg += "MAGICWATER"; break; 
	default:
		return;
		break;
	}

	if( Result == EM_PlantItemResultType_OK )
	{
		strMsg += "_SUCCEED";
	}
	else
	{
		strMsg += "_FAILED";
	}

	const char* szMsg = g_ObjectData->GetString( strMsg.c_str() );

	g_pGameMain->SendSystemChatMsg( szMsg );

}

// ----------------------------------------------------------------------------
void CHousesFrame::R_PlantLockResult( int PotHousePos , PlantLockResultTypeENUM Result )
{
	//EM_PlantLockResultType_OK		,
	//EM_PlantLockResultType_Failed	,

	const char* szMsg = "";
	if( Result == EM_PlantLockResultType_OK )
	{
		szMsg = g_ObjectData->GetString( "SYS_PLANT_LOCK_SUCCEED" );
	}
	else
	{
		szMsg = g_ObjectData->GetString( "SYS_PLANT_LOCK_FAILED" );
	}

	g_pGameMain->SendSystemChatMsg( szMsg );

}

// ----------------------------------------------------------------------------
void CHousesFrame::R_PlantGetProductResult( int PotHousePos , PlantGetProductResultTypeENUM Result )
{
	//EM_PlantGetProductResultType_OK			,
	//EM_PlantGetProductResultType_Failed		,
	//EM_PlantGetProductResultType_NoFruits	,	//沒收成

	const char* szMsg = "";
	if( Result == EM_PlantGetProductResultType_OK )
	{
		szMsg = g_ObjectData->GetString( "SYS_PLANT_GETPRODUCT_SUCCEED" );
	}
	else
	{
		szMsg = g_ObjectData->GetString( "SYS_PLANT_GETPRODUCT_FAILED" );
	}

	g_pGameMain->SendSystemChatMsg( szMsg );
}



// ----------------------------------------------------------------------------
int LuaFunc_Houses_GetTypeCount( lua_State* L )
{
	if( EHousesState_Enter != g_pHousesFrame->GetHousesState() )
	{
		return 0;
	}


	g_pHousesFrame->m_HouseTypeInfoTable.clear();

	RoleDataEx* pRole = CNetGlobal::RoleData();

	int keyItem;
	int style;

	char tt[256];

	for( int i = 0 ; i < 100 ; ++i )
	{
		sprintf( tt, "HouseType_%02d_KeyItem" , i );
		keyItem = g_ObjectData->GetSysKeyValue( tt );

		sprintf( tt, "HouseType_%02d_Style" , i );
		style = g_ObjectData->GetSysKeyValue( tt ); 

		if( g_pHousesFrame->m_HouseBaseInfo.HouseType == style )
		{
			if( false == pRole->CheckKeyItem( keyItem ) )
			{
				//keyitem 沒有寫入

			}
		}

		if( keyItem == 0 || style == 0 )
		{
			break;
		}
		CHousesFrame::SHouseTypeInfo temp;
		temp.houseType = style;
		temp.keyItem   = keyItem;
		if( keyItem <= 0 || pRole->CheckKeyItem( keyItem ) )
		{
			temp.geted = true;
		}			
		else
		{
			temp.geted = false;
		}
		g_pHousesFrame->m_HouseTypeInfoTable.push_back( temp );
	}

	lua_pushnumber( L, (lua_Number)g_pHousesFrame->m_HouseTypeInfoTable.size() ); //工作效益
	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_Houses_GetTypeInfo( lua_State* L )
{
	if( EHousesState_Enter != g_pHousesFrame->GetHousesState() )
	{
		return 0;
	}

	int index = luaL_checkint( L, 1 ) - 1 ;

	if( index < 0 || index >= (int)g_pHousesFrame->m_HouseTypeInfoTable.size() )
	{
		return 0;
	}

	int houseType = g_pHousesFrame->m_HouseTypeInfoTable[index].houseType;

	GameObjDbStructEx* pDateDB = g_ObjectData->GetObj( houseType );

	if( !pDateDB || !pDateDB->IsImage() )
	{
		return 0;
	}

 	lua_pushstring( L, pDateDB->GetName() ); //房屋名稱
  	lua_pushstring( L, pDateDB->Image.ACTField ); //房屋名稱
  	lua_pushboolean( L, g_pHousesFrame->m_HouseTypeInfoTable[index].geted ); //
	lua_pushboolean( L, houseType == g_pHousesFrame->m_HouseBaseInfo.HouseType );

	return 4;
}

// ----------------------------------------------------------------------------
int LuaFunc_Houses_SetCurrentType( lua_State* L )
{
	if( EHousesState_Enter != g_pHousesFrame->GetHousesState() )
	{
		return 0;
	}

	int index = luaL_checkint( L, 1 ) - 1 ;
	if( index < 0 || index >= (int)g_pHousesFrame->m_HouseTypeInfoTable.size() )
	{
		return 0;
	}
 	int houseType = g_pHousesFrame->m_HouseTypeInfoTable[index].houseType;

	NetCli_OtherChild::S_RunGlobalPlotRequest( 6, houseType );

	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_Houses_SetPlaceFurnishingMode( lua_State* L )
{
	if( EHousesState_Enter != g_pHousesFrame->GetHousesState() )
	{
		return 0;
	}

	bool bMode = lua_toboolean( L, 1 ) ==1 ? true:false;
	g_pHousesFrame->SetPlaceFurnishingMode( bMode );
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_Houses_GetFurnitureListID( lua_State* L )
{
	if( EHousesState_Enter != g_pHousesFrame->GetHousesState() )
	{
		return 0;
	}

	lua_pushnumber( L, DF_HouseFurnishingListID );
	return 1;
}

// ----------------------------------------------------------------------------
//	傳入 
//		參數1 容器ID
//		參數2 空格位置 傳入-1 回傳數量
// 
//	回傳 
//		參數1 enable              true = 正常  false = 租約到期
//		參數2 Item name           物品名稱 及用來判斷是否有物品    
//		參數3 item texture file
//      參數4 Placed
// ----------------------------------------------------------------------------
int LuaFunc_Houses_GetFurnitureItemInfo   ( lua_State* L )
{
	if( EHousesState_Enter != g_pHousesFrame->GetHousesState() )
	{
		return 0;
	}

	int ContainerID = DF_HouseFurnishingListID ;
	int ItemID = luaL_checkint( L, 1 ) - 1 ;

	int lastPos = g_pHousesFrame->GetContainerLastPos( ContainerID ) + 1;
	int MaxItemCount = g_pHousesFrame->GetContainerMaxItemCount( ContainerID );

	if( ItemID < 0 )
	{
		if( lastPos > MaxItemCount )
		{
			lua_pushnumber( L, lastPos );		
			return 1;
		}
		lua_pushnumber( L, MaxItemCount );		
		return 1;
	}

	HouseItemDBStruct* pHouseItemDB = g_pHousesFrame->GetHousesItemDB( ContainerID , ItemID );

	if( NULL == pHouseItemDB )
	{
		if( ItemID < MaxItemCount )
		{
			lua_pushboolean( L , true );
		}
		else
		{
			lua_pushboolean( L , false );
		}

		//lua_pushstring(L, itemDB->ImageObj->ACTField);			// push item texture file
		//lua_pushstring(L, itemDB->Name);						// push Item name
		//lua_pushnumber(L, item->Count);							// push item count
		//lua_pushboolean(L, item->Pos != EM_ItemState_NONE);		// push item is locked
		return 1;
	}

	GameObjDbStructEx* itemDB = CNetGlobal::GetObj( pHouseItemDB->Item.OrgObjID );
	if (itemDB)
	{
		if( ItemID < MaxItemCount )
		{
			lua_pushboolean( L , true );
		}
		else
		{
			lua_pushboolean( L , false );
		}

		lua_pushstring( L, itemDB->GetName() );						// push Item name
		//lua_pushstring( L, itemDB->ImageObj->ACTField );		// push item texture file
		lua_pushstring( L, ItemUTIL::GetItemIcon( &pHouseItemDB->Item ) );

		bool bIsShow = true;

		if( false == pHouseItemDB->Layout.IsShow ||
			pHouseItemDB->Pos >= g_pHousesFrame->m_HouseBaseInfo.MaxItemCount )
		{
			bIsShow = false;
		}

		lua_pushboolean( L, bIsShow ); // push Mode


		lua_pushnumber( L, pHouseItemDB->ItemDBID ); //
		return 5;
	}

	return 0;
}


// ----------------------------------------------------------------------------
int LuaFunc_Houses_GetItemInfo( lua_State* L )
{
	if( EHousesState_Enter != g_pHousesFrame->GetHousesState() )
	{
		return 0;
	}

	int ContainerID = luaL_checkint( L, 1 ) ;
	int ItemID = luaL_checkint( L, 2 ) - 1 ;

	if( ItemID < 0 )
	{
		HouseItemDBStruct* pHouseItemDB = g_pHousesFrame->GetHousesItemDBByDBID( ContainerID );
		if ( pHouseItemDB == NULL )
		{
			return 0;
		}

		GameObjDbStructEx* itemDB = CNetGlobal::GetObj( pHouseItemDB->Item.OrgObjID );
		if( NULL == itemDB )
		{
			return 0;
		}

		int FurnishingID = pHouseItemDB->Pos;
  		int MaxItemCount = itemDB->Item.iFurnitureStorageSize;
		lua_pushnumber( L, MaxItemCount );	
		lua_pushstring( L, itemDB->GetName() );
		lua_pushnumber( L, FurnishingID + 1 );
		return 3;
	}

	HouseItemDBStruct* pHouseItemDB = g_pHousesFrame->GetHousesItemDB( ContainerID , ItemID );

	if( NULL == pHouseItemDB )
	{
		return 0;
	}

	ItemFieldStruct* item = &pHouseItemDB->Item;
	if (item)
	{
		GameObjDbStructEx* itemDB = CNetGlobal::GetObj(item->OrgObjID);
		if (itemDB)
		{
			int quality = 0;
			quality = g_ObjectData->GetItemQuality(itemDB, item);


			//lua_pushstring(L, itemDB->ImageObj->ACTField);		// push item texture file
		 	lua_pushstring(L, ItemUTIL::GetItemIcon( item ) );			// push item texture file

			lua_pushstring(L, itemDB->GetName() );						// push Item name
			lua_pushnumber(L, item->GetCount());					// push item count
			lua_pushboolean(L, item->Pos != EM_ItemState_NONE);		// push item is locked
			lua_pushnumber( L, quality );
			return 5;
		}
	}

	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_Houses_SetFocusFurnishingID( lua_State* L )
{
	if( EHousesState_Enter != g_pHousesFrame->GetHousesState() )
	{
		return 0;
	}

	int ID = luaL_checkint( L, 1 );
	g_pHousesFrame->SetFocusFurnishingID( ID );

	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_Houses_GetFocusFurnishingID ( lua_State* L )
{
	if( EHousesState_Enter != g_pHousesFrame->GetHousesState() )
	{
		return 0;
	}

	lua_pushnumber( L, g_pHousesFrame->m_FocusFurnishingID );

	return 1;
}

// ----------------------------------------------------------------------------
int	LuaFunc_Houses_PickupItem(lua_State* L)
{
	if( EHousesState_Enter != g_pHousesFrame->GetHousesState() )
	{
		return 0;
	}

	int ParentID = luaL_checkint( L, 1 ) ;
	int Index = luaL_checkint( L, 2 ) - 1 ;

	g_pHousesFrame->PickupItem( ParentID, Index );
	return 0;
}

// ----------------------------------------------------------------------------
int	LuaFunc_Houses_DrawItem( lua_State* L )
{
	if( EHousesState_Enter != g_pHousesFrame->GetHousesState() )
	{
		return 0;
	}

	int ParentID = luaL_checkint( L, 1 ) ;
	int Index = luaL_checkint( L, 2 ) - 1 ;

	g_pHousesFrame->DrawItem( ParentID, Index );
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_Houses_GetItemLink( lua_State* L )
{
	if( EHousesState_Enter != g_pHousesFrame->GetHousesState() )
	{
		return 0;
	}

	int ContainerID = luaL_checkint( L, 1 ) ;
	int ItemID = luaL_checkint( L, 2 ) - 1 ;

	HouseItemDBStruct* pHouseItemDB = g_pHousesFrame->GetHousesItemDB( ContainerID , ItemID );
	if( NULL == pHouseItemDB )
	{
		return 0;
	}

	ItemFieldStruct* item = &pHouseItemDB->Item;
	if (item)
	{
		GameObjDbStruct* itemDB = CNetGlobal::GetObj( item->OrgObjID );
		if (itemDB)
		{
			string link;
			if( !ItemField_To_ItemLink( item, link, true ) )
			{
				return 0;
			}
			lua_pushstring(L, link.c_str() );
			return 1;
		}
	}
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_Houses_FurnishingPlace( lua_State* L )
{
	if( EHousesState_Enter != g_pHousesFrame->GetHousesState() )
	{
		return 0;
	}

	g_pHousesFrame->FurnishingPlace();
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_Houses_FurnishingRemove( lua_State* L )
{
	if( EHousesState_Enter != g_pHousesFrame->GetHousesState() )
	{
		return 0;
	}

	g_pHousesFrame->FurnishingRemove();
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_Houses_FurnishingMove( lua_State* L )
{
	if( EHousesState_Enter != g_pHousesFrame->GetHousesState() )
	{
		return 0;
	}

	g_pHousesFrame->FurnishingMove();
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_Houses_FurnishingRorare( lua_State* L )
{
	if( EHousesState_Enter != g_pHousesFrame->GetHousesState() )
	{
		return 0;
	}

	g_pHousesFrame->FurnishingRorare();
	return 0;
}

// ----------------------------------------------------------------------------
int LunFunc_Houses_GetEnergyCostInfo( lua_State* L )
{
	int index = luaL_checkint( L, 1 ) - 1;

	vector<HouseEnergyCostStruct>& rTable = g_ObjectData->HouseEnergyCost();

	if( index < 0 )
	{
		lua_pushnumber( L, (lua_Number)rTable.size() );	
		return 1;
	}

	if( index >= (int)rTable.size() )
	{
		//assert( false );
		return 0;
	}

	lua_pushnumber( L, rTable[index].HouseEnergy );
	lua_pushnumber( L, rTable[index].AccountMoneyCost );

	return 2;
}

// ----------------------------------------------------------------------------
int LunFunc_Houses_BuyEnergyRequest( lua_State* L )
{
	if( EHousesState_Enter != g_pHousesFrame->GetHousesState() )
	{
		return 0;
	}

	if( !g_pGameMain->getMerchant() )
	{
		return 0;
	}

	int Money = luaL_checkint( L, 1 );
	//const char* Password = luaL_checkstring( L, 2 );
	const char* Password = g_pGameMain->GetSecondPassword();
	NetCli_Houses::S_BuyEnergyRequest( Money, Password );
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_Houses_GetSpaceInfo( lua_State* L )
{
	if( EHousesState_Enter != g_pHousesFrame->GetHousesState() )
	{
		return 0;
	}

	//////////////////////////////////////////////////////////////////////////
	//struct HouseShopInfoStruct
	//{
	//	int FlagID;
	//	int FunitureCount;
	//	int	OpenEnergy;
	//	int	KeppEnergy;
	//	int Type;			//( 0 表示為傢俱格子 1 表示其它類 )
	//};

	vector<HouseShopInfoStruct>& rTable = g_ObjectData->HouseShop();

	int FunitureCount = 0;
	int	KeppEnergy = 0;
	int FunctionID = -1;

	int i;
	for( i = 0 ; i < (int)rTable.size() ; ++i )
	{
		if( g_pHousesFrame->m_HouseBaseInfo.ShopFunction.GetFlag( i ) && 
			rTable[i].Type == 0 )  //( 0 表示為傢俱格子 1 表示其它類 )
		{
			FunitureCount = rTable[i].FunitureCount;
			KeppEnergy = rTable[i].KeppEnergy;
			FunctionID = i;
		}
	}

	int current = g_pHousesFrame->m_HouseBaseInfo.MaxItemCount ;
	int min     = g_pHousesFrame->m_HouseBaseInfo.MaxItemCount - FunitureCount;
	int max     = 100;

	lua_pushnumber( L, current );
	lua_pushnumber( L, min );
	lua_pushnumber( L, max );
	//lua_pushnumber( L, KeppEnergy );
	lua_pushnumber( L, FunctionID ); 
	return 4;
}

// ----------------------------------------------------------------------------
int LuaFunc_House_BuySpaceRequest( lua_State* L )
{
	if( EHousesState_Enter != g_pHousesFrame->GetHousesState() )
	{
		return 0;
	}

	if( !g_pGameMain->getMerchant() )
	{
		return 0;
	}

	int FunctionID = luaL_checkint( L, 1 );

	if( FunctionID >= 0 )
	{
		NetCli_Houses::S_BuyFunctionRequest( FunctionID );
	}

	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_House_TestBuySpace( lua_State* L )
{
	if( EHousesState_Enter != g_pHousesFrame->GetHousesState() )
	{
		return 0;
	}

	int Request = luaL_checkint( L, 1 );
	int FunctionID = -1;

	vector<HouseShopInfoStruct>& rTable = g_ObjectData->HouseShop();

	int FunitureCount = 0;
	int	OpenEnergy = 0;
	int	KeppEnergy = 0;

	int i;
	for( i = 0 ; i < (int)rTable.size() ; ++i )
	{
		if( g_pHousesFrame->m_HouseBaseInfo.ShopFunction.GetFlag( i ) && 
			rTable[i].Type == 0 )  //( 0 表示為傢俱格子 1 表示其它類 )
		{
			FunctionID = i;
			FunitureCount = rTable[i].FunitureCount;
			OpenEnergy = rTable[i].OpenEnergy; 
			KeppEnergy = rTable[i].KeppEnergy;
		}
	}

	int current = g_pHousesFrame->m_HouseBaseInfo.MaxItemCount ;
	int mini    = g_pHousesFrame->m_HouseBaseInfo.MaxItemCount - FunitureCount;

	Request = Request - mini;

	int	NewOpenEnergy = 0;
	int	NewKeppEnergy = 0;

	for( i = 0 ; i < (int)rTable.size() ; ++i )
	{
		if( rTable[i].Type == 0 )  //( 0 表示為傢俱格子 1 表示其它類 )
		{
			if( rTable[i].FunitureCount == Request )
			{
				NewOpenEnergy = rTable[i].OpenEnergy - OpenEnergy ;
				NewKeppEnergy = rTable[i].KeppEnergy ;	
				FunctionID = i;

				if( NewOpenEnergy < 0 )
					NewOpenEnergy = 0;


				g_pHousesFrame->m_BuyFunction_NewOpenEnergy = NewOpenEnergy;
				g_pHousesFrame->m_BuyFunction_FunctionID = FunctionID;

				lua_pushnumber( L, NewOpenEnergy );
				lua_pushnumber( L, NewKeppEnergy );	
				lua_pushnumber( L, FunctionID );	
				return 3;
			}
		}
	}

	//lua_pushnumber( L, OpenEnergy );
	//lua_pushnumber( L, KeppEnergy );	
	//lua_pushnumber( L, FunctionID );

	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_House_CloseStorage( lua_State* L )
{
	if( EHousesState_Enter != g_pHousesFrame->GetHousesState() )
	{
		return 0;
	}

	int DBID = luaL_checkint( L, 1 );
	g_pHousesFrame->CloseStorage( DBID );
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_Houses_ChangedName( lua_State* L )
{
	if( EHousesState_Enter != g_pHousesFrame->GetHousesState() )
	{
		return 0;
	}

	const char* name = luaL_checkstring( L, 1 ) ;

	int len = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, name, -1, NULL, NULL) ;
	len = len - 1; //會有計算結束字元 


	if( len < 1 )
	{
		g_pGameMain->SendSystemChatMsg( g_ObjectData->GetString( "SYS_HOUSE_SET_NAME_FAIL2" ) );
		return 0;
	}

	if( len > 8 )
	{
		g_pGameMain->SendSystemChatMsg( g_ObjectData->GetString( "SYS_HOUSE_SET_NAME_FAIL3" ) );
		return 0;
	}

	if( 0 == stricmp( g_pHousesFrame->m_HouseBaseInfo.HouseName , name ) )
	{
		g_pGameMain->SendSystemChatMsg( g_ObjectData->GetString( "SYS_HOUSE_SET_NAME_FAIL6" ) );
		return 0;
	}

	if( !g_pGameMain->CheckPatternString( name ) )
	{
		g_pGameMain->SendSystemChatMsg( g_ObjectData->GetString( "SYS_HOUSE_SET_NAME_FAIL4" ) );
		return 0;
	}
	
	NetCli_Houses::S_SetHouseName( name );
	
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_Houses_ChangedPassword( lua_State* L )
{
	if( EHousesState_Enter != g_pHousesFrame->GetHousesState() )
	{
		return 0;
	}

	//captionEditBox1 = "Old Password",
	//captionEditBox2 = "New Password",
	//captionEditBox3 = "Confirm Password",

	//const char* Old = luaL_checkstring( L, 1 ) ;
	const char* New = luaL_checkstring( L, 1 ) ;
	const char* Confirm = luaL_checkstring( L, 2 ) ;

	if( !New || !Confirm )
	{
		return 0;
	}
	
	//if( g_pHousesFrame->m_HouseBaseInfo.Password != Old )
	//{
	//	g_pChatFrame->SendMsgEvent("CHAT_MSG_SYSTEM", "舊密碼確認錯誤", "" );
	//}

	if( 0 != strcmp( New, Confirm ) )
	{
		g_pGameMain->SendSystemChatMsg( g_ObjectData->GetString( "SYS_HOUSE_PASSWORD_CONFIRM_ERROR" ) );
		//g_pChatFrame->SendMsgEvent( "CHAT_MSG_SYSTEM", "新密碼確認錯誤", "" );
		return 0;
	}

	NetCli_Houses::S_SetPassword( New );

	return 0;

}


// ----------------------------------------------------------------------------
int LuaFunc_Houses_HangerSwap(lua_State* L)
{
	if( EHousesState_Enter != g_pHousesFrame->GetHousesState() )
	{
		return 0;
	}

	int ItemDBID = (int)luaL_checknumber(L, 1);
	bool bAll = (int)luaL_checknumber(L, 2) == 0 ? false : true ;
	g_pHousesFrame->HangerSwap( ItemDBID, bAll );
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_Houses_CanWearObject(  lua_State* L )
{
	if( EHousesState_Enter != g_pHousesFrame->GetHousesState() )
	{
		return 0;
	}

	int ContainerID = luaL_checkint( L, 1 ) ;
	int ItemID = luaL_checkint( L, 2 ) - 1 ;

	HouseItemDBStruct* pHouseItemDB = g_pHousesFrame->GetHousesItemDB( ContainerID , ItemID );

	if( NULL == pHouseItemDB )
	{
		return 0;
	}
 
	ItemFieldStruct* item = &pHouseItemDB->Item;
	if (item)
	{
		lua_pushboolean( L, g_pHousesFrame->Houses_CanWearObject( item , pHouseItemDB->Pos ) );
		return 1;
	}
 
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_Plant_GetInfo( lua_State* L )
{
	if( EHousesState_Enter != g_pHousesFrame->GetHousesState() )
	{
		return 0;
	}

	if( g_pHousesFrame->m_PlantFurnishingDBID < 0 )
	{
		return 0;
	}

	HouseItemDBStruct* pHouseItem = g_pHousesFrame->GetHousesItemDBByDBID( g_pHousesFrame->m_PlantFurnishingDBID );

	if( NULL == pHouseItem )
	{
		return 0;
	}

	CHousesFrame::SHousePlant Plant;
	map<int,CHousesFrame::SHousePlant>::iterator itPos = g_pHousesFrame->m_MapHousePlant.find( pHouseItem->ItemDBID );
  	if( itPos != g_pHousesFrame->m_MapHousePlant.end() )
	{
		Plant = itPos->second; 
	}
	else
	{
		if( false == g_pHousesFrame->UpdateHousePlant( Plant, pHouseItem->Item ) )
		{
			return 0;
		}
	}

	//state

	string strState;

	

	//無   none
  	//成長 grow
  	//成年 ripe
  	//鎖定 lock
  	//枯死 death

	if( Plant.eState == CHousesFrame::HPS_Death )
	{
		strState = "death";
	}
	else if( Plant.isLock )
	{
		strState = "lock";
	}
	else if( Plant.eState == CHousesFrame::HPS_None )
	{
		strState = "none";
	}
	else if( Plant.eState == CHousesFrame::HPS_Grow )
	{
 		strState = "grow";
	}
	else if( Plant.eState == CHousesFrame::HPS_Ripe )
	{
		strState = "ripe";
	}
	else
	{
		return 0;
	}

	const char* szSeedTexture = "";
	const char* szSeedName = "";
	string strLink;

	int nSeedObjectID = -1;

	if( pHouseItem->Item.Plant.OrgSeedID > 0 )
	{
		nSeedObjectID = pHouseItem->Item.Plant.OrgSeedID + Def_ObjectClass_Item;
	}
	else
	{
//		nSeedObjectID = g_pHousesFrame->m_ObjectID_Seed;
	}
	
	GameObjDbStructEx* itemDBSeed = NULL;
	itemDBSeed = CNetGlobal::GetObj( nSeedObjectID );
 	
	if( itemDBSeed )
	{
		GameObjDbStructEx* imageDB = CNetGlobal::GetObj( itemDBSeed->ImageID );
		if ( imageDB )
		{
			szSeedTexture = imageDB->Image.ACTField;
		}
		
		szSeedName = itemDBSeed->GetName();
		ItemGUID_To_ItemLink( nSeedObjectID, strLink );
	}

	if( strState == "none" )
	{
		//檢查盆栽等級
		int SkillLV = (int) CNetGlobal::RoleData()->TempData.Attr.Fin.SkillValue.GetSkill( EM_SkillValueType_Plant );
	
		GameObjDbStructEx* itemDB = CNetGlobal::GetObj( Plant.Item.OrgObjID );
		if( NULL == itemDB )
		{
			return 0;
		}

		//是不是盆栽
		if( itemDB->Item.Plant.Pot.Mode == 0 )
		{
			return 0;
		}

		if( itemDB->Item.Plant.Level > SkillLV )
		{
			strState = "skillLvErr";
		}
	}
	
	//狀態
	lua_pushstring( L, strState.c_str() );

	//名稱
	lua_pushstring( L, Plant.strName.c_str() );

	//屬性
	lua_pushnumber( L , Plant.nType + 1 );

 	//種子名稱
 	lua_pushstring( L, szSeedName );

	//種子ICON
	lua_pushstring( L, szSeedTexture );

	lua_pushstring( L, strLink.c_str() );

 	//成長狀態
	lua_pushnumber( L , min( Plant.fGrowRate, 1 ) );	
   	lua_pushnumber( L , Plant.fHealthRate );
  	//lua_pushnumber( L , Plant.fRareRate );
     
 	//下次階段
	lua_pushnumber( L , Plant.nReapTime );	
	
	lua_pushnumber( L , Plant.nFeedPoint         );	

	lua_pushnumber( L , Plant.nWaterPoint        );	
	lua_pushnumber( L , Plant.nTopdressPoint     );	
	lua_pushnumber( L , Plant.nBestWaterPoint    );	
	lua_pushnumber( L , Plant.nBestTopdressPoint );	
   
	lua_pushnumber( L , Plant.fDecWater    );	
	lua_pushnumber( L , Plant.fDecTopdress );	

//	lua_pushnumber( L , Plant.nNextTimeWater );	
//	lua_pushnumber( L , Plant.nNextTimeTopdress );	
//	lua_pushnumber( L , Plant.nNextTimeMagicWater );	

	return 16;
}

// ----------------------------------------------------------------------------
int LuaFunc_Plant_GetItmeInfo( lua_State* L )
{
	if( EHousesState_Enter != g_pHousesFrame->GetHousesState() )
	{
		return 0;
	}

	string strType = luaL_checkstring( L, 1 );
	int objectID = -1;
	if( strType == "seed" )
	{
		objectID = g_pHousesFrame->m_ObjectID_Seed;
	}
	else if( strType == "water" )
	{
		objectID = g_pHousesFrame->m_ObjectID_Water;
	}
	else if( strType == "topdress" )
	{
		objectID = g_pHousesFrame->m_ObjectID_Topdress;
	}
	else if( strType == "magicwater" )
	{
		objectID = g_pHousesFrame->m_ObjectID_MagicWater;
	}

	const char* texture = "";
	const char* name = "";
 	int count = 0;
	string strLink;


	if ( objectID > 0 )
	{
		GameObjDbStructEx* itemDB = CNetGlobal::GetObj( objectID );
		if ( itemDB )
		{
			GameObjDbStructEx* imageDB = CNetGlobal::GetObj( itemDB->ImageID );
			if ( imageDB )
			{
				texture = imageDB->Image.ACTField;
			}
			name = itemDB->GetName();

			count = CNetGlobal::RoleData()->GetBagItemCount( objectID );

			ItemGUID_To_ItemLink( objectID ,strLink );
		}
	}

	lua_pushstring( L, texture );		
	lua_pushstring( L, name );			
	lua_pushnumber( L, count );
	lua_pushstring( L, strLink.c_str() );
	return 4;
}




// ----------------------------------------------------------------------------
int LuaFunc_Plant_PickupItem( lua_State* L )
{
	if( EHousesState_Enter != g_pHousesFrame->GetHousesState() )
	{
		return 0;
	}

	string strType = luaL_checkstring( L, 1 );

	CHousesFrame::EPlantItemType type = CHousesFrame::PIT_Unknow;
	if( strType == "seed" )
	{
		type = CHousesFrame::PIT_Seed;
	}
	else if( strType == "water" )
	{
		type = CHousesFrame::PIT_Water;
	}
	else if( strType == "topdress" )
	{
		type = CHousesFrame::PIT_Topdress;
	}
	else if( strType == "magicwater" )
	{
		type = CHousesFrame::PIT_MagicWater;
	}
	else
	{
		return 0;
	}
	
	g_pHousesFrame->Plant_PickupItem( type );

	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_Plant_ClearItem( lua_State* L )
{
	if( EHousesState_Enter != g_pHousesFrame->GetHousesState() )
	{
		return 0;
	}

	string strType = luaL_checkstring( L, 1 );

	CHousesFrame::EPlantItemType type = CHousesFrame::PIT_Unknow;
	if( strType == "seed" )
	{
		type = CHousesFrame::PIT_Seed;
	}
	else if( strType == "water" )
	{
		type = CHousesFrame::PIT_Water;
	}
	else if( strType == "topdress" )
	{
		type = CHousesFrame::PIT_Topdress;
	}
	else if( strType == "magicwater" )
	{
		type = CHousesFrame::PIT_MagicWater;
	}
	else
	{
		return 0;
	}

	g_pHousesFrame->Plant_ClearItem( type );

	return 0;
}


// ----------------------------------------------------------------------------
int LuaFunc_Plant_Close( lua_State* L )
{
	if( EHousesState_Enter != g_pHousesFrame->GetHousesState() )
	{
		return 0;
	}

	g_pHousesFrame->Plant_Close();
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_Plant_Grow( lua_State* L )
{
	if( EHousesState_Enter != g_pHousesFrame->GetHousesState() )
	{
		return 0;
	}

	string strType = luaL_checkstring( L, 1 );

	CHousesFrame::EPlantItemType type = CHousesFrame::PIT_Unknow;
	if( strType == "seed" )
	{
		type = CHousesFrame::PIT_Seed;
	}
	else if( strType == "water" )
	{
		type = CHousesFrame::PIT_Water;
	}
	else if( strType == "topdress" )
	{
		type = CHousesFrame::PIT_Topdress;
	}
	else if( strType == "magicwater" )
	{
		type = CHousesFrame::PIT_MagicWater;
	}
	else
	{
		return 0;
	}

	g_pHousesFrame->Plant_Grow( type );
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_Plant_Clear( lua_State* L )
{
	if( EHousesState_Enter != g_pHousesFrame->GetHousesState() )
	{
		return 0;
	}

	g_pHousesFrame->Plant_Clear();
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_Plant_Lock( lua_State* L )
{
	if( EHousesState_Enter != g_pHousesFrame->GetHousesState() )
	{
		return 0;
	}

	g_pHousesFrame->Plant_Lock();
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_Plant_GetProduct( lua_State* L )
{
	if( EHousesState_Enter != g_pHousesFrame->GetHousesState() )
	{
		return 0;
	}

	g_pHousesFrame->Plant_GetProduct();
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_Plant_GetPlantCount( lua_State* L )
{
	if( EHousesState_Enter != g_pHousesFrame->GetHousesState() )
	{
		return 0;
	}

	int count = 0;
 
	//載入傢俱
	TD_HouseItemMap &rHouseItemMap = g_pHousesFrame->GetHouseFurnishingList();
	TD_HouseItemMap::iterator itPos = rHouseItemMap.begin();
	TD_HouseItemMap::iterator itEnd = rHouseItemMap.end();

	for( ; itPos != itEnd ; ++itPos )
	{
		//LoadRoleSpriteHouseItem( &(itPos->second) );
		HouseItemDBStruct* HouseItemDB = itPos->second;
		GameObjDbStructEx* pItemDB = CNetGlobal::GetObj( HouseItemDB->Item.OrgObjID );
		if( pItemDB && pItemDB->Item.Plant.Pot.Mode != 0 )
		{
			if( HouseItemDB->Item.Plant.OrgSeedID > 0 && 
				!HouseItemDB->Item.Plant.IsLock && 
				!HouseItemDB->Item.Plant.IsDead )
			{
				++count;
			}
		}
	}
  
	lua_pushnumber( L, count );
 	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_Plant_GetMaxPlantCount( lua_State* L )
{
	if( EHousesState_Enter != g_pHousesFrame->GetHousesState() )
	{
		return 0;
	}
	
	int count = CNetGlobal::RoleData()->PlayerBaseData->MaxPlantCount;

 	lua_pushnumber( L, count );

	return 1;
}

// ----------------------------------------------------------------------------
//int LuaFunc_Houses_SetPlant(  lua_State* L )
//{
//	if( lua_isnil( L, 1 ) )
//	{
//		s_bPlantDebug = false;
//		g_pHousesFrame->UpdateAlllPlantSprite();
//		return 0;
//	}
//
//	s_fGrowRate = (float)luaL_checknumber( L, 1 );
//	s_fCareRate = (float)luaL_checknumber( L, 2 );
//	s_fRareRate = (float)luaL_checknumber( L, 3 );
//
//	s_bPlantDebug = true;
//
//	g_pHousesFrame->UpdateAlllPlantSprite();
//	return 0;
//}

// ----------------------------------------------------------------------------
int LuaFunc_Houses_GetFriendCount( lua_State* L )
{
	if( EHousesState_Enter != g_pHousesFrame->GetHousesState() )
	{
		return 0;
	}

	int count = (int)g_pHousesFrame->m_FriendDBIDTable.size();
	lua_pushnumber( L , count );	
	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_Houses_GetFriendInfo( lua_State* L )
{
	if( EHousesState_Enter != g_pHousesFrame->GetHousesState() )
	{
		return 0;
	}

	int index = luaL_checkint( L, 1 ) - 1 ;

	if( index < 0 || index >= (int)g_pHousesFrame->m_FriendDBIDTable.size() )
	{
		return 0;
	}

	int DBID = g_pHousesFrame->m_FriendDBIDTable[index];
 
	FindDBIDRoleNameStruct* findStruct = NetCli_OtherChild::GetFindDBIDRoleNameStruct( DBID );
 
	if( NULL == findStruct || false == findStruct->IsFind )
	{
		return 0;
	}

	lua_pushstring( L, findStruct->RoleName.c_str() );
	
	return 1;
}


class FindDBIDRoleNameWork_Houses_AddFriend : public IFindDBIDRoleNameWorkBase
{
public:

	virtual void Update()
	{
		if( IsFind )
		{

			int i;
			for( i = 0 ; i < (int)g_pHousesFrame->m_FriendDBIDTable.size() ; ++i )
			{
				if( g_pHousesFrame->m_FriendDBIDTable[i] == DBID )
				{
					g_pGameMain->SendSystemChatMsgEx( g_ObjectData->GetString( "SYS_HOUSE_ADD_FRIEND_FAIL2" ), RoleName.c_str() );
					return;
				}
			}

			if( g_pGameMain->GetDBID() == DBID )
			{
				g_pGameMain->SendSystemChatMsgEx( g_ObjectData->GetString( "SYS_HOUSE_ADD_FRIEND_FAIL3" ), RoleName.c_str() );
				return;
			}

			NetCli_Houses::S_AddFriendDBID( DBID );
		}
		else
		{
			g_pGameMain->SendSystemChatMsgEx( g_ObjectData->GetString( "SYS_HOUSE_ADD_FRIEND_FAIL1" ), RoleName.c_str() );
		}
	}
};

// ----------------------------------------------------------------------------
int LuaFunc_Houses_AddFriend( lua_State* L )
{
	if( EHousesState_Enter != g_pHousesFrame->GetHousesState() )
	{
		return 0;
	}

	if( !g_pHousesFrame->m_bIsOwner )
	{
		return 0;
	}
 
	const char* cszName  = luaL_checkstring( L, 1 );
 
	//SYS_HOUSE_ADD_FRIEND_FAIL2      新增房屋好友 %s 失敗，此玩家已存在。
	//SYS_HOUSE_ADD_FRIEND_FAIL3      新增房屋好友 %s 失敗，不可設自己為房屋好友。
  

	FindDBIDRoleNameWork_Houses_AddFriend* pWork = new FindDBIDRoleNameWork_Houses_AddFriend;
	pWork->RoleName = cszName;

	NetCli_OtherChild::AddFindDBIDRoleNameWork( pWork );

	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_Houses_DelFriend( lua_State* L )
{
	if( EHousesState_Enter != g_pHousesFrame->GetHousesState() )
	{
		return 0;
	}

	if( !g_pHousesFrame->m_bIsOwner )
	{
		return 0;
	}

 	int index = luaL_checkint( L, 1 ) - 1;

	if( index < 0 || index >= (int)g_pHousesFrame->m_FriendDBIDTable.size() )
	{
		return 0;
	}

	int DBID = g_pHousesFrame->m_FriendDBIDTable[index];

	NetCli_Houses::S_DelFriendDBID( DBID );

	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_Houses_GetFriendPut( lua_State* L )
{
	if( EHousesState_Enter != g_pHousesFrame->GetHousesState() )
	{
		return 0;
	}

	int ContainerID = luaL_checkint( L, 1 ) ;

 	HouseItemDBStruct* pHouseItemDB = g_pHousesFrame->GetHousesItemDBByDBID( ContainerID );
	lua_pushboolean( L, pHouseItemDB->Layout.IsFriend_Put );

	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_Houses_GetFriendGet( lua_State* L )
{
	if( EHousesState_Enter != g_pHousesFrame->GetHousesState() )
	{
		return 0;
	}

	int ContainerID = luaL_checkint( L, 1 );

	HouseItemDBStruct* pHouseItemDB = g_pHousesFrame->GetHousesItemDBByDBID( ContainerID );
	lua_pushboolean( L, pHouseItemDB->Layout.IsFriend_Get );

	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_Houses_GetFriendView( lua_State* L )
{
	if( EHousesState_Enter != g_pHousesFrame->GetHousesState() )
	{
		return 0;
	}

	int ContainerID = luaL_checkint( L, 1 );

	HouseItemDBStruct* pHouseItemDB = g_pHousesFrame->GetHousesItemDBByDBID( ContainerID );
	lua_pushboolean( L, pHouseItemDB->Layout.IsFriend_Show );

	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_Houses_SetFriendPut( lua_State* L )
{
	if( EHousesState_Enter != g_pHousesFrame->GetHousesState() )
	{
		return 0;
	}

	if( !g_pHousesFrame->m_bIsOwner )
	{
		return 0;
	}
	
	int ContainerID = luaL_checkint( L, 1 );

	bool bIs = lua_toboolean( L, 2 ) == 1;

	HouseItemDBStruct* pHouseItemDB = g_pHousesFrame->GetHousesItemDBByDBID( ContainerID );

	pHouseItemDB->Layout.IsFriend_Put = bIs;

	NetCli_Houses::S_HouseItemLayoutRequest( pHouseItemDB->ItemDBID, pHouseItemDB->Layout );

	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_Houses_SetFriendGet( lua_State* L )
{
	if( EHousesState_Enter != g_pHousesFrame->GetHousesState() )
	{
		return 0;
	}

	if( !g_pHousesFrame->m_bIsOwner )
	{
		return 0;
	}

	int ContainerID = luaL_checkint( L, 1 ) ;

	bool bIs = lua_toboolean( L, 2 ) == 1;

	HouseItemDBStruct* pHouseItemDB = g_pHousesFrame->GetHousesItemDBByDBID( ContainerID );

	pHouseItemDB->Layout.IsFriend_Get = bIs;

	NetCli_Houses::S_HouseItemLayoutRequest( pHouseItemDB->ItemDBID, pHouseItemDB->Layout );

	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_Houses_SetFriendView( lua_State* L )
{
	if( EHousesState_Enter != g_pHousesFrame->GetHousesState() )
	{
		return 0;
	}

	if( !g_pHousesFrame->m_bIsOwner )
	{
		return 0;
	}

	int ContainerID = luaL_checkint( L, 1 ) ;

	bool bIs = lua_toboolean( L, 2 ) == 1;

	HouseItemDBStruct* pHouseItemDB = g_pHousesFrame->GetHousesItemDBByDBID( ContainerID );

	pHouseItemDB->Layout.IsFriend_Show = bIs;

	NetCli_Houses::S_HouseItemLayoutRequest( pHouseItemDB->ItemDBID, pHouseItemDB->Layout );

	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_Houses_GetFriendRegard( lua_State* L )
{
	if( EHousesState_Enter != g_pHousesFrame->GetHousesState() )
	{
		return 0;
	}

	if( !g_pHousesFrame->m_bIsOwner )
	{
		return 0;
	}

	lua_pushstring( L, g_pHousesFrame->m_HouseBaseInfo.Friend_Enter );
	lua_pushstring( L, g_pHousesFrame->m_HouseBaseInfo.Friend_Leave );
	lua_pushstring( L, g_pHousesFrame->m_HouseBaseInfo.Friend_Use   );

	return 3;
}

// ----------------------------------------------------------------------------
int LuaFunc_Houses_SetFriendRegard( lua_State* L )
{
	if( EHousesState_Enter != g_pHousesFrame->GetHousesState() )
	{
		return 0;
	}

	if( !g_pHousesFrame->m_bIsOwner )
	{
		return 0;
	}

	const char* szEnter = luaL_checkstring( L, 1 );
	const char* szLeave = luaL_checkstring( L, 2 );
	const char* szUse   = luaL_checkstring( L, 3 );

	int maxLen = MAX_HOUSE_STRING_SIZE - 1;
	if( (int)strlen( szEnter ) > maxLen ||
		(int)strlen( szLeave ) > maxLen ||
		(int)strlen( szUse   ) > maxLen ) 
	{
		g_pGameMain->SendSystemChatMsg( "SYS_HOUSES_SETFRIENDREGARD_FAIL1" );
		return 0;
	}

	if( !g_pGameMain->CheckPatternString( szEnter ) ||
		!g_pGameMain->CheckPatternString( szLeave ) ||
		!g_pGameMain->CheckPatternString( szUse   ) )
	{
		g_pGameMain->SendSystemChatMsg( g_ObjectData->GetString( "SYS_HOUSES_SETFRIENDREGARD_FAIL2" ) );
		return 0;
	}

	if( strcmp( g_pHousesFrame->m_HouseBaseInfo.Friend_Enter, szEnter  ) != 0 ||
		strcmp( g_pHousesFrame->m_HouseBaseInfo.Friend_Leave, szLeave  ) != 0 ||
		strcmp( g_pHousesFrame->m_HouseBaseInfo.Friend_Use  , szUse    ) != 0 ) 
	{

	}
	else
	{
		return 0;
	}
	
	strcpy( g_pHousesFrame->m_HouseBaseInfo.Friend_Enter, szEnter );
	strcpy( g_pHousesFrame->m_HouseBaseInfo.Friend_Leave, szLeave );
	strcpy( g_pHousesFrame->m_HouseBaseInfo.Friend_Use  , szUse   );

	NetCli_Houses::S_SetClientData( g_pHousesFrame->m_HouseBaseInfo.ClientData );

	g_pGameMain->SendSystemChatMsg( g_ObjectData->GetString( "SYS_HOUSES_SETFRIENDREGARD_SUCCESS" )  );

	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_Houses_GetFriendItemLog( lua_State* L )
{
	if( EHousesState_Enter != g_pHousesFrame->GetHousesState() )
	{
		return 0;
	}
	
	int index = luaL_checkint( L, 1 ) - 1 ;

	if( index < 0 || index >= MAX_HOUSE_ITEM_LOG_COUNT )
	{
		return 0;
	}

	HouseFriendItemLogStruct& rLog = g_pHousesFrame->m_HouseBaseInfo.ItemLog[index];


	int time = max( ( (int)g_pGameMain->GetGameTime() - rLog.Time ), 0);

	lua_pushnumber( L , time );

 	FindDBIDRoleNameStruct* findStruct = NetCli_OtherChild::GetFindDBIDRoleNameStruct( rLog.FriendDBID );
  	if( NULL == findStruct || false == findStruct->IsFind )
	{
		lua_pushstring( L, "" );
	}
	else
	{
		lua_pushstring( L, findStruct->RoleName.c_str() );
	}

	string ItemLink;
	ItemGUID_To_ItemLink( rLog.ItemID, ItemLink );

	lua_pushstring( L, ItemLink.c_str() );

	lua_pushnumber( L , rLog.Count );

	return 4;
}

// ----------------------------------------------------------------------------
int LuaFunc_Houses_ServantHireListRequest( lua_State* L )
{
	g_pHousesFrame->m_ServantHireList.clear();
	NetCli_Houses::SL_ServantHireListRequest();
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_Houses_ServantHireRequest( lua_State* L )
{
	int index = luaL_checkint( L, 1 ) - 1 ;
	if( index < 0 || index >= (int)g_pHousesFrame->m_ServantHireList.size() )
	{
		return 0;
	}

	CHousesFrame::SServantHireList& rInfo = g_pHousesFrame->m_ServantHireList[index];
	
	for( int i = 0; i < MAX_SERVANT_COUNT ; ++i )
	{
		if( g_pHousesFrame->m_HouseBaseInfo.Servant[i].NPCObjID == 0 )
		{
			NetCli_Houses::SL_ServantHireRequest( rInfo.ID , i );
			return 0;
		}
	}

	//已滿

 	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_Houses_GetServantHireInfoCount( lua_State* L )
{
	if( EHousesState_Enter != g_pHousesFrame->GetHousesState() )
	{
		return 0;
	}

	int count = (int)g_pHousesFrame->m_ServantHireList.size();
 	lua_pushnumber( L , count );
	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_Houses_GetServantHireInfo( lua_State* L )
{
	if( EHousesState_Enter != g_pHousesFrame->GetHousesState() )
	{
		return 0;
	}

	int index = luaL_checkint( L, 1 ) - 1 ;
	if( index < 0 || index >= (int)g_pHousesFrame->m_ServantHireList.size() )
	{
		return 0;
	}

	CHousesFrame::SServantHireList& rInfo = g_pHousesFrame->m_ServantHireList[index];
	HouseServantStruct& rServant = rInfo.Servant;

	string Name = g_pHousesFrame->GetServantName( rServant );
	
	lua_pushnumber( L , rInfo.ID );
	lua_pushstring( L , Name.c_str() );

	lua_pushnumber( L , rServant.Sex );    //0 男 1 女 2 怪物
	lua_pushnumber( L , rServant.Character ); //角色個性


	int Month = 1;
 	int Day = 1;
	string Horoscope;
	g_pHousesFrame->GetServantBirthday( rServant, Month, Day, Horoscope );

 	lua_pushnumber( L , Month );  
	lua_pushnumber( L , Day );  
 	lua_pushstring( L , Horoscope.c_str() );

	int nRace = (int)g_pHousesFrame->GetServantRace( rServant );
	lua_pushstring(L, g_ObjectData->GetString( Com_FormatString( "SYS_RACE_%d", nRace ) ) );
 
	lua_pushnumber( L , rServant.Value[0] );              
	lua_pushnumber( L , rServant.MaxValue[0] );          

	lua_pushnumber( L , rServant.Value[1] );              
	lua_pushnumber( L , rServant.MaxValue[1] );          

	lua_pushnumber( L , rServant.Tired );              
	lua_pushnumber( L , 100 );           

	lua_pushnumber( L , rServant.Value[3] );              
	lua_pushnumber( L , rServant.MaxValue[3] );          

	lua_pushnumber( L , rServant.Value[4] );              
	lua_pushnumber( L , rServant.MaxValue[4] );          

	lua_pushnumber( L , rServant.Value[5] );              
	lua_pushnumber( L , rServant.MaxValue[5] );          

	lua_pushnumber( L , rServant.Value[6] );              
	lua_pushnumber( L , rServant.MaxValue[6] );          

	lua_pushnumber( L , rServant.Value[7] );              
	lua_pushnumber( L , rServant.MaxValue[7] );   

	lua_pushboolean( L , rServant.IsHire ); //是否被雇用

	lua_pushnumber( L , rServant.Cost ); //費用
	
	return 26;
}

// ----------------------------------------------------------------------------
int LuaFunc_Houses_GetServantCount( lua_State* L )
{
	if( EHousesState_Enter != g_pHousesFrame->GetHousesState() )
	{
		return 0;
	}
	lua_pushnumber( L , g_pHousesFrame->m_ServantCount );
	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_Houses_GetServantInfo( lua_State* L )
{
	if( EHousesState_Enter != g_pHousesFrame->GetHousesState() )
	{
		return 0;
	}

	int index = luaL_checkint( L, 1 ) - 1 ;

 	if( index < 0 || index >= g_pHousesFrame->m_ServantCount )
	{
		return 0;		
	}

	index = g_pHousesFrame->m_UIServantIndex[index];

	if( index < 0 || index >= MAX_SERVANT_COUNT )
	{
		return 0;
	}



	if( g_pHousesFrame->m_HouseBaseInfo.Servant[index].NPCObjID == 0 )
	{
		return 0;
	}

	HouseItemDBStruct* pHouseItemDB = g_pHousesFrame->GetHousesItemDB( DF_HouseServantListID , index );

	if( NULL == pHouseItemDB )
	{
		return 0;
	}

	HouseServantStruct& rServant = g_pHousesFrame->m_HouseBaseInfo.Servant[index];

	string Name = g_pHousesFrame->GetServantName( rServant );

 	lua_pushnumber( L , pHouseItemDB->ItemDBID );
	lua_pushstring( L , Name.c_str() );

	lua_pushnumber( L , rServant.Sex );    //0 男 1 女 2 怪物
	lua_pushnumber( L , rServant.Character ); //角色個性

	int Month = 1;
	int Day = 1;
	string Horoscope;
	g_pHousesFrame->GetServantBirthday( rServant, Month, Day, Horoscope );

	lua_pushnumber( L , Month );  
	lua_pushnumber( L , Day );  
	lua_pushstring( L , Horoscope.c_str() );

	int nRace = (int)g_pHousesFrame->GetServantRace( rServant );
	lua_pushstring(L, g_ObjectData->GetString( Com_FormatString( "SYS_RACE_%d", nRace ) ) );

 	lua_pushnumber( L , rServant.Value[0] );              
	lua_pushnumber( L , rServant.MaxValue[0] );          

	lua_pushnumber( L , rServant.Value[1] );              
	lua_pushnumber( L , rServant.MaxValue[1] );          

	lua_pushnumber( L , rServant.Tired );              
	lua_pushnumber( L , 100 );          

	lua_pushnumber( L , rServant.Value[3] );              
	lua_pushnumber( L , rServant.MaxValue[3] );          

	lua_pushnumber( L , rServant.Value[4] );              
	lua_pushnumber( L , rServant.MaxValue[4] );          

	lua_pushnumber( L , rServant.Value[5] );              
	lua_pushnumber( L , rServant.MaxValue[5] );          

	lua_pushnumber( L , rServant.Value[6] );              
	lua_pushnumber( L , rServant.MaxValue[6] );          

	lua_pushnumber( L , rServant.Value[7] );              
	lua_pushnumber( L , rServant.MaxValue[7] );          
	
	
	return 24;
}

// ----------------------------------------------------------------------------
int LuaFunc_Houses_SummonServant( lua_State* L )
{
	if( EHousesState_Enter != g_pHousesFrame->GetHousesState() )
	{
		return 0;
	}

	int index = luaL_checkint( L, 1 ) - 1 ;

	if( index < 0 || index >= g_pHousesFrame->m_ServantCount )
	{
		return 0;		
	}
	index = g_pHousesFrame->m_UIServantIndex[index];

	if( index < 0 || index >= MAX_SERVANT_COUNT )
	{
		return 0;
	}

	if( g_pHousesFrame->m_HouseBaseInfo.Servant[index].NPCObjID == 0 )
	{
		return 0;
	}
 
	HouseItemDBStruct* pHouseItemDB = g_pHousesFrame->GetHousesItemDB( DF_HouseServantListID, index );

	if( NULL == pHouseItemDB )
	{
		return 0;
	}

	CRoleSprite* pPlayer = g_pGameMain->GetPlayer();

	//CRuFusion_ActorState actorState = pPlayer->GetActorState();
	//if( actorState != ruFUSION_ACTORSTATE_NORMAL )
	//{
	//	g_pGameMain->SendSystemChatMsg( "Summon Servant error, ActorState != NORMAL" );
	//	return 0;
	//}
	//if( pPlayer->GetMoveVector() != 0 )
	//{
	//	g_pGameMain->SendSystemChatMsg( "Summon Servant error, MoveVector != 0" );
	//	return 0;
	//}

	CRuVector3 Dir = pPlayer->GetDirection();
	Dir.y = 0;
	Dir.Normalize();

	CRuVector3 Pos = pPlayer->GetPos() + ( Dir * 10 );


	CRoleSprite* pRoleSpriteHouse = g_pGameMain->FindSpriteByWorldGUID( g_pHousesFrame->m_HouseRoleSpriteID );
	if( !pRoleSpriteHouse )
	{
		return 0;
	}

	//判斷會不會跑出房屋
	{
		// 建構碰撞測試結構
		CRuCollisionQuery_Segment_Nearest query;
  
		CRuVector3 playerPos = pPlayer->GetPos();
		playerPos.y += 12.0f;

		// 設定測試用線段及其他設定
		query.m_colSegment = CRuSegment3( playerPos, playerPos + ( Dir * 20.0f ) );
		query.m_ignoreBackface = FALSE;
		query.m_ignoreCollisionFlag = TRUE;

		// 測試碰撞
		pRoleSpriteHouse->GetRuEntity()->QueryCollision(&query);

		// 檢測回應
		if( query.m_positiveHit )
		{
			//g_pGameMain->SendSystemMsg( g_ObjectData->GetString( "SYS_HOUSES_SERVANT_SUMMON_FAILED" ) );
			//g_pGameMain->SendSystemChatMsg( g_ObjectData->GetString( "SYS_HOUSES_SERVANT_SUMMON_FAILED" ) );

			CRuVector3 collisionNormal = query.m_collisionNormal;
			collisionNormal.y = 0;
			collisionNormal.Normalize();

			CRuVector3 vecDir = Dir * 12.5f;

			float value = DotProduct( vecDir, collisionNormal );
			//float value1 = DotProduct( collisionNormal, vecDir );

			//value = fabs( value );
			//if( value < 2.5f )
			//{
			//	g_pGameMain->SendSystemMsg( g_ObjectData->GetString( "SYS_HOUSES_SERVANT_SUMMON_FAILED" ) );
			//	g_pGameMain->SendSystemChatMsg( g_ObjectData->GetString( "SYS_HOUSES_SERVANT_SUMMON_FAILED" ) );
			//	return 0;
			//}
			//else
			{
				//vecDir.Normalize();
				//vecDir.x = -vecDir.x;
				//vecDir.z = -vecDir.z;

				CRuVector3 TempPos = query.m_collisionPoint + ( vecDir * ( 2.5f / value ) );

				float dist = ( TempPos - playerPos ).Magnitude();
				if( dist < 10 )
				{
					Pos = TempPos;
				}
			}
		}
	}

  	//地板
	{
		Pos.y = pPlayer->GetPos().y + 17.0f;
		// 建構碰撞測試結構
		CRuCollisionQuery_Segment_Nearest query;

		//判斷會不會跑出房屋

		// 設定測試用線段及其他設定
		query.m_colSegment = CRuSegment3( Pos, Pos + CRuVector3( 0.0f, -30.0f, 0.0f ) );
		query.m_ignoreBackface = FALSE;
		query.m_ignoreCollisionFlag = TRUE;

		// 測試碰撞
		pRoleSpriteHouse->GetRuEntity()->QueryCollision(&query);

		// 檢測回應
		if( query.m_positiveHit )
		{
			Pos = query.m_collisionPoint;
		}
		else
		{
			g_pGameMain->SendSystemMsg( g_ObjectData->GetString( "SYS_HOUSES_SERVANT_SUMMON_FAILED" ) );
			g_pGameMain->SendSystemChatMsg( g_ObjectData->GetString( "SYS_HOUSES_SERVANT_SUMMON_FAILED" ) );
			return 0;
		}
	}


	Dir.x = -Dir.x;
	Dir.z = -Dir.z;

	pHouseItemDB->Layout.X = Pos.x;
	pHouseItemDB->Layout.Y = Pos.y;
	pHouseItemDB->Layout.Z = Pos.z;

	CRuVector3 entityForward = Dir;
	entityForward.Normalize();
	REAL forwardAngleDiff;
	CalculateAngleDifferential( CRuVector3(0.0f, 0.0f, -1.0f) , entityForward, CRuVector3(0.0f, 1.0f, 0.0f), forwardAngleDiff );
	pHouseItemDB->Layout.Angle = forwardAngleDiff;



	pHouseItemDB->Layout.IsShow = true;
	NetCli_Houses::S_HouseItemLayoutRequest( pHouseItemDB->ItemDBID, pHouseItemDB->Layout );


	//女僕被雇用出來的時候
	//說的話
	//C_HOUSESERVANT_HIRE_1_1
	//C_HOUSESERVANT_HIRE_1_2
	//C_HOUSESERVANT_HIRE_1_3
	//C_HOUSESERVANT_HIRE_1_4
	//C_HOUSESERVANT_HIRE_1_5

	SHousesServantWorkInfo& rWorkInfo = g_pHousesFrame->m_ServantWorkInfos[index];
	rWorkInfo.isSummon = true;

	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_Houses_HideServant( lua_State* L )
{	
	if( EHousesState_Enter != g_pHousesFrame->GetHousesState() )
	{
		return 0;
	}

	int index = luaL_checkint( L, 1 ) - 1 ;

	if( index < 0 || index >= g_pHousesFrame->m_ServantCount )
	{
		return 0;		
	}
	index = g_pHousesFrame->m_UIServantIndex[index];

	if( index < 0 || index >= MAX_SERVANT_COUNT )
	{
		return 0;
	}

	if( g_pHousesFrame->m_HouseBaseInfo.Servant[index].NPCObjID == 0 )
	{
		return 0;
	}

	HouseItemDBStruct* pHouseItemDB = g_pHousesFrame->GetHousesItemDB( DF_HouseServantListID, index );
	if( NULL == pHouseItemDB )
	{
		return 0;
	}

	pHouseItemDB->Layout.IsShow = false;
	NetCli_Houses::S_HouseItemLayoutRequest( pHouseItemDB->ItemDBID, pHouseItemDB->Layout );

	//g_pHousesFrame->LoadRoleSpriteServant( index );

	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_Houses_DismissServant( lua_State* L )
{
	if( EHousesState_Enter != g_pHousesFrame->GetHousesState() )
	{
		return 0;
	}

	int index = luaL_checkint( L, 1 ) - 1 ;

	if( index < 0 || index >= g_pHousesFrame->m_ServantCount )
	{
		return 0;		
	}
	index = g_pHousesFrame->m_UIServantIndex[index];

	if( index < 0 || index >= MAX_SERVANT_COUNT )
	{
		return 0;
	}

	if( g_pHousesFrame->m_HouseBaseInfo.Servant[index].NPCObjID == 0 )
	{
		return 0;
	}

	HouseItemDBStruct* pHouseItemDB = g_pHousesFrame->GetHousesItemDB( DF_HouseServantListID, index );


	//身上有物品 不能解雇
	TD_HouseItemMap& rHouseItemMap = g_pHousesFrame->GetHouseContainer( pHouseItemDB->ItemDBID );

	if( rHouseItemMap.size() > 0 )
	{
		g_pGameMain->SendSystemMsg( g_ObjectData->GetString( "SYS_HOUSES_SERVANT_DISMISS_FAILED" ) );
		g_pGameMain->SendSystemChatMsg( g_ObjectData->GetString( "SYS_HOUSES_SERVANT_DISMISS_FAILED" ) );
		return 0;
	}
	
 	pHouseItemDB->Layout.IsShow = false;
 	NetCli_Houses::S_HouseItemLayoutRequest( pHouseItemDB->ItemDBID, pHouseItemDB->Layout );

	NetCli_Houses::SL_ServantDelRequest( index );

	return 0;
}



// ----------------------------------------------------------------------------
int Model_SetHanger(lua_State* L)
{
	CUiModel* model = dynamic_cast<CUiModel*>(CUiLuaLayout::CheckLayoutTable(L, 1));
	if (model == NULL)
		lua_error(L);

	int ItemDBID = (int)luaL_checknumber(L, 2);
	bool bFitting = lua_toboolean(L, 3) == 0 ? false : true; 

	HouseItemDBStruct* pHouseItemDB = g_pHousesFrame->GetHousesItemDBByDBID( ItemDBID );

	if( NULL == pHouseItemDB )
	{
		return 0;
	}

	GameObjDbStructEx* HangeritemDB;
	HangeritemDB = CNetGlobal::GetObj( pHouseItemDB->Item.OrgObjID );
	if( !( HangeritemDB && HangeritemDB->Item.ItemType == EM_ItemType_Furniture ) )
	{
		return 0;
	}

	CRoleSprite* sprite =NULL; 
	CRuEntity* entity = NULL;

	bool backweapon = false; //收起狀態

	if( bFitting )
	{
		sprite = g_pGameMain->GetPlayer();
		if (sprite)
		{
			entity = sprite->GetRuEntity();
		}
		model->CreateEntity(entity, NULL);

		if( pHouseItemDB )
		{

		}
	}
	else
	{
		sprite = g_pHousesFrame->GetHouseRoleSpriteByItemDBID( ItemDBID );
		if (sprite)
		{
			entity = sprite->GetRuEntity();
		}
		model->CreateEntity(entity, NULL);
	}

	//CRuVector3 relativeCamPos( 0.0 , 0.0 , 1.0 );
	if( HangeritemDB->Item.emFurnitureType == EM_FurnitureType_WeaponSet && !bFitting )
	{
		//relativeCamPos = CRuVector3( 0.0f , -1.0f , 0.1f );
		backweapon = true;
	}
	//model->SetRelativeCameraPosition( relativeCamPos );


	//裝備
  	TD_HouseItemMap& rHouseItemMap = g_pHousesFrame->GetHouseContainer( ItemDBID );

	TD_HouseItemMap::iterator itPos = rHouseItemMap.begin();
	TD_HouseItemMap::iterator itEnd = rHouseItemMap.end();

	GameObjDbStructEx* itemDB;
	GameObjDbStructEx* imageitemDB;

	int color[2];


	int EQPos;
	for( ; itPos != itEnd ; ++itPos )
	{
		HouseItemDBStruct* pHouseItemDB = itPos->second;
		ItemFieldStruct& rItemField = pHouseItemDB->Item;
		itemDB = CNetGlobal::GetObj( rItemField.OrgObjID );
		if( itemDB == NULL )
			continue;

		imageitemDB = CNetGlobal::GetObj( rItemField.ImageObjectID );
		if( imageitemDB == NULL )
		{
			imageitemDB = itemDB;
		}

		GameObjDbStructEx* imageDB = CNetGlobal::GetObj( imageitemDB->ImageID );
		if ( imageDB == NULL )
			continue;

		itemDB = imageitemDB;

		//if( ( LimitSex1._Sex != 0 && ( ( pRole->BaseData.Sex == EM_Sex_Boy && LimitSex1.Boy == false ) || ( pRole->BaseData.Sex == EM_Sex_Girl && LimitSex1.Girl == false ) ) ) ||
		//	( LimitSex2._Sex != 0 && ( ( pRole->BaseData.Sex == EM_Sex_Boy && LimitSex2.Boy == false ) || ( pRole->BaseData.Sex == EM_Sex_Girl && LimitSex2.Girl == false ) ) ) )
		//{
		//	return 0;	
		//}
		//pSprite->SetPaperdollColor(PAPERDOLL_PART_NAME_HELMET, color[0], color[1]);	
		//  model->SetPaperdollColor( );

		//	model->AttachEntity( ATTACH_POINT_LEFT_HAND, NULL );
		//	model->AttachEntity( ATTACH_POINT_HAND_SHIELD, pItemDB->ImageObj->ACTWorld );
		//	model->SetPaperdollColor( ATTACH_POINT_HAND_SHIELD  , color[0], color[1] );
		//ImageObjectID


		if( rItemField.MainColor == 0 )
			color[0] = imageDB->Image.ArmorMainColor.Color;	
		else
			color[0] = rItemField.MainColor;

		if( rItemField.OffColor == 0 )
			color[1] = imageDB->Image.ArmorOffColor.Color;	
		else
			color[1] = rItemField.OffColor; 

		EQPos = itPos->second->Pos;
		switch(EQPos)
		{
//		case EM_EQWearPos_Head       : if( bFitting ){ model->SetPaperdollPart( PAPERDOLL_PART_NAME_HELMET  , imageDB->Image.ACTWorld ); model->SetPaperdollColor( PAPERDOLL_PART_NAME_HELMET  , color[0], color[1] ); } break;   //頭
//		case EM_EQWearPos_Gloves     : if( bFitting ){ model->SetPaperdollPart( PAPERDOLL_PART_NAME_HAND    , imageDB->Image.ACTWorld ); model->SetPaperdollColor( PAPERDOLL_PART_NAME_HAND    , color[0], color[1] ); } break;   //手套
//		case EM_EQWearPos_Shoes      : if( bFitting ){ model->SetPaperdollPart( PAPERDOLL_PART_NAME_FOOT    , imageDB->Image.ACTWorld ); model->SetPaperdollColor( PAPERDOLL_PART_NAME_FOOT    , color[0], color[1] ); } break;   //鞋子
//		case EM_EQWearPos_Clothes    : if( bFitting ){ model->SetPaperdollPart( PAPERDOLL_PART_NAME_TORSO   , imageDB->Image.ACTWorld ); model->SetPaperdollColor( PAPERDOLL_PART_NAME_TORSO   , color[0], color[1] ); } break;   //上衣
//		case EM_EQWearPos_Pants      : if( bFitting ){ model->SetPaperdollPart( PAPERDOLL_PART_NAME_LEG     , imageDB->Image.ACTWorld ); model->SetPaperdollColor( PAPERDOLL_PART_NAME_LEG     , color[0], color[1] ); } break;   //褲子
//		case EM_EQWearPos_Back       : if( bFitting ){ model->SetPaperdollPart( PAPERDOLL_PART_NAME_BACK    , imageDB->Image.ACTWorld ); model->SetPaperdollColor( PAPERDOLL_PART_NAME_BACK    , color[0], color[1] ); } break;   //背部
//		case EM_EQWearPos_Belt       : if( bFitting ){ model->SetPaperdollPart( PAPERDOLL_PART_NAME_BELT    , imageDB->Image.ACTWorld ); model->SetPaperdollColor( PAPERDOLL_PART_NAME_BELT    , color[0], color[1] ); } break;   //腰帶
//		case EM_EQWearPos_Shoulder   : if( bFitting ){ model->SetPaperdollPart( PAPERDOLL_PART_NAME_SHOULDER, imageDB->Image.ACTWorld ); model->SetPaperdollColor( PAPERDOLL_PART_NAME_SHOULDER, color[0], color[1] ); } break;   //肩甲

		case EM_EQWearPos_MainHand   : 
			{
				if( itemDB->Item.Class != EM_Item_Weapon )
					continue;

				if( backweapon )
				{
					if( itemDB->Item.WeaponPos == EM_WeaponPos_OneHand    ||
						itemDB->Item.WeaponPos == EM_WeaponPos_MainHand   ) 
					{
						model->AttachEntity( ATTACH_POINT_LEFT_WEAPON, imageDB->Image.ACTWorld );
					}
					else if( itemDB->Item.WeaponPos == EM_WeaponPos_TwoHand )
					{
						if( itemDB->Item.WeaponType == EM_Weapon_Staff )
						{
							model->AttachEntity( ATTACH_POINT_BACK_STAFF, imageDB->Image.ACTWorld );
						}
						else
						{
							model->AttachEntity( ATTACH_POINT_BACK_SWORD, imageDB->Image.ACTWorld );
						}
					}
				}
				else
				{
					if( itemDB->Item.WeaponPos == EM_WeaponPos_TwoHand  ||
						itemDB->Item.WeaponPos == EM_WeaponPos_OneHand  ||
						itemDB->Item.WeaponPos == EM_WeaponPos_MainHand   ) 
					{
						model->AttachEntity( ATTACH_POINT_RIGHT_HAND, imageDB->Image.ACTWorld );
					}
				}
				break; 
			}
		case EM_EQWearPos_SecondHand : 
			{
				if( backweapon )
				{
					if( itemDB->Item.Class == EM_Item_Weapon && (  
						itemDB->Item.WeaponPos == EM_WeaponPos_OneHand    ||
						itemDB->Item.WeaponPos == EM_WeaponPos_SecondHand )) 
					{
						model->AttachEntity( ATTACH_POINT_RIGHT_WEAPON, imageDB->Image.ACTWorld) ;
					}
					else if( itemDB->Item.Class == EM_Item_Armor && itemDB->Item.WeaponPos == EM_ArmorPos_SecondHand ) 
					{
						if( itemDB->Item.ArmorType == EM_Armor_Shield )
						{
							model->AttachEntity( ATTACH_POINT_BACK_SHIELD, imageDB->Image.ACTWorld );
						}
						else if( itemDB->Item.ArmorType == EM_Armor_Implement )
						{
							model->AttachEntity( ATTACH_POINT_RIGHT_WEAPON, imageDB->Image.ACTWorld );
						}
					}
				}
				else
				{
					if( itemDB->Item.Class == EM_Item_Weapon && (  
						itemDB->Item.WeaponPos == EM_WeaponPos_OneHand    ||
						itemDB->Item.WeaponPos == EM_WeaponPos_SecondHand )) 
					{
						model->AttachEntity( ATTACH_POINT_LEFT_HAND, imageDB->Image.ACTWorld );
					}
					else if( itemDB->Item.Class == EM_Item_Armor && itemDB->Item.WeaponPos == EM_ArmorPos_SecondHand  ) 
					{
						if( itemDB->Item.ArmorType == EM_Armor_Shield )
						{
							model->AttachEntity( ATTACH_POINT_HAND_SHIELD, imageDB->Image.ACTWorld );
						}
						else if( itemDB->Item.ArmorType == EM_Armor_Implement )
						{
							model->AttachEntity( ATTACH_POINT_LEFT_HAND, imageDB->Image.ACTWorld );
						}
					}
				}

				break;
			}
		case EM_ArmorPos_Ornament :
			{
				CUnitFrame::Model_SetItem( model, itemDB, &rItemField, true );
			}
		default:
			if( bFitting )
			{
				CUnitFrame::Model_SetItem( model, itemDB, &rItemField, true );
			}
		}									
	}

	lua_pushboolean(L, entity != NULL);

	return 1;
}

// ----------------------------------------------------------------------------
// 給裝備預覽用
// ----------------------------------------------------------------------------
void Func_Houses_CreatePlant( CRuEntity* pParent, ItemFieldStruct& rItem )
{
	g_pHousesFrame->Houses_CreatePlant( pParent, rItem );
}


// ----------------------------------------------------------------------------
// 招募女僕
// ----------------------------------------------------------------------------
int Model_SetServantHire(lua_State* L)
{
	CUiModel* model = dynamic_cast<CUiModel*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (model == NULL)
		lua_error(L);

	int index = luaL_checkint( L, 2 ) - 1 ;
	if( index < 0 || index >= (int)g_pHousesFrame->m_ServantHireList.size() )
	{
		return 0;
	}

	CHousesFrame::SServantHireList& rInfo = g_pHousesFrame->m_ServantHireList[index];
	HouseServantStruct& rServant = rInfo.Servant;
  
	GameObjDbStructEx* NPCObjDB = g_ObjectData->GetObj( rServant.NPCObjID );
	if( NULL == NPCObjDB )
	{
		return 0;
	}

	GameObjDbStructEx* imageDB = CNetGlobal::GetObj( NPCObjDB->ImageID );
	if( NULL == imageDB )
	{
		return 0;
	}

	model->CreateEntity( imageDB->Image.ACTWorld );

//	model->SetPaperdollPart( PAPERDOLL_PART_NAME_HAIR     , imageDB->Image.PaperdollHair.Name    );
//	model->SetPaperdollPart( PAPERDOLL_PART_NAME_HEAD     , imageDB->Image.PaperdollFace.Name    );
	model->SetPaperdollPart( PAPERDOLL_PART_NAME_HELMET   , imageDB->Image.PaperdollHead.Name    );
	model->SetPaperdollPart( PAPERDOLL_PART_NAME_HAND     , imageDB->Image.PaperdollGloves.Name  );
	model->SetPaperdollPart( PAPERDOLL_PART_NAME_FOOT     , imageDB->Image.PaperdollShoes.Name   );
	model->SetPaperdollPart( PAPERDOLL_PART_NAME_TORSO    , imageDB->Image.PaperdollClothes.Name );
	model->SetPaperdollPart( PAPERDOLL_PART_NAME_LEG      , imageDB->Image.PaperdollPants.Name   );
	model->SetPaperdollPart( PAPERDOLL_PART_NAME_BACK     , imageDB->Image.PaperdollBack.Name    );
	model->SetPaperdollPart( PAPERDOLL_PART_NAME_BELT     , imageDB->Image.PaperdollBelt.Name    );
	model->SetPaperdollPart( PAPERDOLL_PART_NAME_SHOULDER , imageDB->Image.PaperdollShouder.Name );

//	model->SetPaperdollColor( PAPERDOLL_PART_NAME_HAIR		, imageDB->Image.PaperdollHair.MainColor.Color   , imageDB->Image.PaperdollHair.OffColor.Color    );
//	model->SetPaperdollColor( PAPERDOLL_PART_NAME_HEAD		, imageDB->Image.PaperdollFace.MainColor.Color   , imageDB->Image.PaperdollFace.OffColor.Color    );
	model->SetPaperdollColor( PAPERDOLL_PART_NAME_HELMET	, imageDB->Image.PaperdollHead.MainColor.Color   , imageDB->Image.PaperdollHead.OffColor.Color    );
	model->SetPaperdollColor( PAPERDOLL_PART_NAME_HAND		, imageDB->Image.PaperdollGloves.MainColor.Color , imageDB->Image.PaperdollGloves.OffColor.Color  );
	model->SetPaperdollColor( PAPERDOLL_PART_NAME_FOOT		, imageDB->Image.PaperdollShoes.MainColor.Color  , imageDB->Image.PaperdollShoes.OffColor.Color   );
	model->SetPaperdollColor( PAPERDOLL_PART_NAME_TORSO		, imageDB->Image.PaperdollClothes.MainColor.Color, imageDB->Image.PaperdollClothes.OffColor.Color );
	model->SetPaperdollColor( PAPERDOLL_PART_NAME_LEG		, imageDB->Image.PaperdollPants.MainColor.Color  , imageDB->Image.PaperdollPants.OffColor.Color   );
	model->SetPaperdollColor( PAPERDOLL_PART_NAME_BACK		, imageDB->Image.PaperdollBack.MainColor.Color   , imageDB->Image.PaperdollBack.OffColor.Color    );
	model->SetPaperdollColor( PAPERDOLL_PART_NAME_BELT		, imageDB->Image.PaperdollBelt.MainColor.Color   , imageDB->Image.PaperdollBelt.OffColor.Color    );
	model->SetPaperdollColor( PAPERDOLL_PART_NAME_SHOULDER	, imageDB->Image.PaperdollShouder.MainColor.Color, imageDB->Image.PaperdollShouder.OffColor.Color );

	{
		const char* name = NULL;
		GameObjDbStruct* pObj = CNetGlobal::GetObj( rServant.Look.FaceID );
		GameObjDbStruct* pImageDB = NULL;
		if ( pObj )
		{
			pImageDB = CNetGlobal::GetObj(pObj->ImageID);
		}	

		if ( pImageDB )
		{
			model->SetPaperdollPart( PAPERDOLL_PART_NAME_HEAD, pImageDB->Image.ACTWorld );
		}
	}

	{
		const char* name = NULL;
		GameObjDbStruct* pObj = CNetGlobal::GetObj( rServant.Look.HairID );
		GameObjDbStruct* pImageDB = NULL;
		if ( pObj )
		{
			pImageDB = CNetGlobal::GetObj(pObj->ImageID);
		}	

		if ( pImageDB )
		{
			model->SetPaperdollPart( PAPERDOLL_PART_NAME_HAIR, pImageDB->Image.ACTWorld );
		}
	}
  
	model->SetPaperdollColor(PAPERDOLL_PART_NAME_HEAD, rServant.Look.BodyColor | 0xFF000000 , 0);
	model->SetPaperdollColor(PAPERDOLL_PART_NAME_HAIR, rServant.Look.HairColor | 0xFF000000 , 0);

	model->SetBoneScale(BONE_SCALE_NAME_HEAD,		(BYTE) rServant.Look.BoneScale[0] / 100.0f);
	model->SetBoneScale(BONE_SCALE_NAME_BREASTS,	(BYTE) rServant.Look.BoneScale[1] / 100.0f);
	model->SetBoneScale(BONE_SCALE_NAME_BODY,		(BYTE) rServant.Look.BoneScale[2] / 100.0f);
	model->SetBoneScale(BONE_SCALE_NAME_CHEST,		(BYTE) rServant.Look.BoneScale[3] / 100.0f);
	model->SetBoneScale(BONE_SCALE_NAME_PELVIS,	    (BYTE) rServant.Look.BoneScale[4] / 100.0f);
	model->SetBoneScale(BONE_SCALE_NAME_ARM,        (BYTE) rServant.Look.BoneScale[5] / 100.0f);
	model->SetBoneScale(BONE_SCALE_NAME_FOREARM,	(BYTE) rServant.Look.BoneScale[6] / 100.0f);
	model->SetBoneScale(BONE_SCALE_NAME_HAND,		(BYTE) rServant.Look.BoneScale[7] / 100.0f);
	model->SetBoneScale(BONE_SCALE_NAME_LEGS,		(BYTE) rServant.Look.BoneScale[8] / 100.0f);
	model->SetBoneScale(BONE_SCALE_NAME_CALF,		(BYTE) rServant.Look.BoneScale[9] / 100.0f);
	model->SetBoneScale(BONE_SCALE_NAME_FOOT,		(BYTE) rServant.Look.BoneScale[10] / 100.0f);

//	model->PaperdollBuild();


	return 0;
}



// ----------------------------------------------------------------------------
// 房屋女僕
// ----------------------------------------------------------------------------
int Model_SetServant(lua_State* L)
{
	CUiModel* model = dynamic_cast<CUiModel*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (model == NULL)
		lua_error(L);

	model->ReleaseEntity();

 	int index = (int)luaL_checknumber(L, 2) - 1;

	if( index < 0 || index >= g_pHousesFrame->m_ServantCount )
	{
		return 0;		
	}
	index = g_pHousesFrame->m_UIServantIndex[index];

	if( index < 0 || index > MAX_SERVANT_COUNT )
	{
		return 0;
	}

	HouseServantStruct& Servant = g_pHousesFrame->m_HouseBaseInfo.Servant[index];
   	if( Servant.NPCObjID == 0 )
	{
		return 0;
	}

	GameObjDbStructEx* NPCObjDB = g_ObjectData->GetObj( Servant.NPCObjID );
	if( NULL == NPCObjDB )
	{
		return 0;
	}

	GameObjDbStructEx* imageDB = CNetGlobal::GetObj( NPCObjDB->ImageID );
	if( NULL == imageDB )
	{
		return 0;
	}


	model->CreateEntity( imageDB->Image.ACTWorld );

	//	model->SetPaperdollPart( PAPERDOLL_PART_NAME_HAIR     , imageDB->Image.PaperdollHair.Name    );
	//	model->SetPaperdollPart( PAPERDOLL_PART_NAME_HEAD     , imageDB->Image.PaperdollFace.Name    );
	model->SetPaperdollPart( PAPERDOLL_PART_NAME_HELMET   , imageDB->Image.PaperdollHead.Name    );
	model->SetPaperdollPart( PAPERDOLL_PART_NAME_HAND     , imageDB->Image.PaperdollGloves.Name  );
	model->SetPaperdollPart( PAPERDOLL_PART_NAME_FOOT     , imageDB->Image.PaperdollShoes.Name   );
	model->SetPaperdollPart( PAPERDOLL_PART_NAME_TORSO    , imageDB->Image.PaperdollClothes.Name );
	model->SetPaperdollPart( PAPERDOLL_PART_NAME_LEG      , imageDB->Image.PaperdollPants.Name   );
	model->SetPaperdollPart( PAPERDOLL_PART_NAME_BACK     , imageDB->Image.PaperdollBack.Name    );
	model->SetPaperdollPart( PAPERDOLL_PART_NAME_BELT     , imageDB->Image.PaperdollBelt.Name    );
	model->SetPaperdollPart( PAPERDOLL_PART_NAME_SHOULDER , imageDB->Image.PaperdollShouder.Name );

	//	model->SetPaperdollColor( PAPERDOLL_PART_NAME_HAIR		, imageDB->Image.PaperdollHair.MainColor.Color   , imageDB->Image.PaperdollHair.OffColor.Color    );
	//	model->SetPaperdollColor( PAPERDOLL_PART_NAME_HEAD		, imageDB->Image.PaperdollFace.MainColor.Color   , imageDB->Image.PaperdollFace.OffColor.Color    );
	model->SetPaperdollColor( PAPERDOLL_PART_NAME_HELMET	, imageDB->Image.PaperdollHead.MainColor.Color   , imageDB->Image.PaperdollHead.OffColor.Color    );
	model->SetPaperdollColor( PAPERDOLL_PART_NAME_HAND		, imageDB->Image.PaperdollGloves.MainColor.Color , imageDB->Image.PaperdollGloves.OffColor.Color  );
	model->SetPaperdollColor( PAPERDOLL_PART_NAME_FOOT		, imageDB->Image.PaperdollShoes.MainColor.Color  , imageDB->Image.PaperdollShoes.OffColor.Color   );
	model->SetPaperdollColor( PAPERDOLL_PART_NAME_TORSO		, imageDB->Image.PaperdollClothes.MainColor.Color, imageDB->Image.PaperdollClothes.OffColor.Color );
	model->SetPaperdollColor( PAPERDOLL_PART_NAME_LEG		, imageDB->Image.PaperdollPants.MainColor.Color  , imageDB->Image.PaperdollPants.OffColor.Color   );
	model->SetPaperdollColor( PAPERDOLL_PART_NAME_BACK		, imageDB->Image.PaperdollBack.MainColor.Color   , imageDB->Image.PaperdollBack.OffColor.Color    );
	model->SetPaperdollColor( PAPERDOLL_PART_NAME_BELT		, imageDB->Image.PaperdollBelt.MainColor.Color   , imageDB->Image.PaperdollBelt.OffColor.Color    );
	model->SetPaperdollColor( PAPERDOLL_PART_NAME_SHOULDER	, imageDB->Image.PaperdollShouder.MainColor.Color, imageDB->Image.PaperdollShouder.OffColor.Color );

	{
		const char* name = NULL;
		GameObjDbStruct* pObj = CNetGlobal::GetObj( Servant.Look.FaceID );
		GameObjDbStruct* pImageDB = NULL;
		if ( pObj )
		{
			pImageDB = CNetGlobal::GetObj(pObj->ImageID);
		}	

		if ( pImageDB )
		{
			model->SetPaperdollPart( PAPERDOLL_PART_NAME_HEAD, pImageDB->Image.ACTWorld );
		}
	}

	{
		const char* name = NULL;
		GameObjDbStruct* pObj = CNetGlobal::GetObj( Servant.Look.HairID );
		GameObjDbStruct* pImageDB = NULL;
		if ( pObj )
		{
			pImageDB = CNetGlobal::GetObj(pObj->ImageID);
		}	

		if ( pImageDB )
		{
			model->SetPaperdollPart( PAPERDOLL_PART_NAME_HAIR, pImageDB->Image.ACTWorld );
		}
	}

	model->SetPaperdollColor(PAPERDOLL_PART_NAME_HEAD, Servant.Look.BodyColor | 0xFF000000 , 0);
	model->SetPaperdollColor(PAPERDOLL_PART_NAME_HAIR, Servant.Look.HairColor | 0xFF000000 , 0);

	model->SetBoneScale(BONE_SCALE_NAME_HEAD,		(BYTE) Servant.Look.BoneScale[0] / 100.0f);
	model->SetBoneScale(BONE_SCALE_NAME_BREASTS,	(BYTE) Servant.Look.BoneScale[1] / 100.0f);
	model->SetBoneScale(BONE_SCALE_NAME_BODY,		(BYTE) Servant.Look.BoneScale[2] / 100.0f);
	model->SetBoneScale(BONE_SCALE_NAME_CHEST,		(BYTE) Servant.Look.BoneScale[3] / 100.0f);
	model->SetBoneScale(BONE_SCALE_NAME_PELVIS,	    (BYTE) Servant.Look.BoneScale[4] / 100.0f);
	model->SetBoneScale(BONE_SCALE_NAME_ARM,        (BYTE) Servant.Look.BoneScale[5] / 100.0f);
	model->SetBoneScale(BONE_SCALE_NAME_FOREARM,	(BYTE) Servant.Look.BoneScale[6] / 100.0f);
	model->SetBoneScale(BONE_SCALE_NAME_HAND,		(BYTE) Servant.Look.BoneScale[7] / 100.0f);
	model->SetBoneScale(BONE_SCALE_NAME_LEGS,		(BYTE) Servant.Look.BoneScale[8] / 100.0f);
	model->SetBoneScale(BONE_SCALE_NAME_CALF,		(BYTE) Servant.Look.BoneScale[9] / 100.0f);
	model->SetBoneScale(BONE_SCALE_NAME_FOOT,		(BYTE) Servant.Look.BoneScale[10] / 100.0f);



	//裝備
	HouseItemDBStruct* pHouseItemDBServant = g_pHousesFrame->GetHousesItemDB( DF_HouseServantListID, index ); 

	if( NULL == pHouseItemDBServant )
	{
		return 0;
	}

	TD_HouseItemMap& rHouseItemMap = g_pHousesFrame->GetHouseContainer( pHouseItemDBServant->ItemDBID );

	TD_HouseItemMap::iterator itPos = rHouseItemMap.begin();
	TD_HouseItemMap::iterator itEnd = rHouseItemMap.end();

	GameObjDbStructEx* itemDB;

	//int EQPos;
	for( ; itPos != itEnd ; ++itPos )
	{
		HouseItemDBStruct* pHouseItemDB = itPos->second;

		if( pHouseItemDB->Pos < DF_ServantArmorPos || 
			pHouseItemDB->Pos > DF_ServantArmorPosEnd )
		{
			continue;
		}
  
		ItemFieldStruct& rItemField = pHouseItemDB->Item;

		itemDB = CNetGlobal::GetObj( rItemField.OrgObjID );
		if( itemDB == NULL )
		{
			continue;
		}

		CUnitFrame::Model_SetItem( model, itemDB, &rItemField, true );
	}

	return 0;
}











































