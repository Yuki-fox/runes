
#include "UI_TeachingFrame.h"
#include "..\tools\Cini\Ini.h"
#include "..\mainproc\GameMain.h"
#include "RoleData\ObjectDataClass.h"
#include "..\mainproc\NetGlobal.h"
#include "../../interface/ObjectBloodBar/ObjectBloodBar.h"

#include "..\NetWaker_member\Net_Other\NetCli_Other.h"
#include "../WorldFrames/BagFrame.h"
#include "../WorldFrames/ItemClipboard.h"
#include "../WorldFrames/ItemUtil.h"
#include "../Login/AccountLogin.h"

//#include "TransportBook\TransportBook.h"
#include <vector>
using namespace std;

char *g_BoneName[]=
{


 BONE_SCALE_NAME_HEAD	,//				"Head"
 BONE_SCALE_NAME_BREASTS,//				"Breasts"

 BONE_SCALE_NAME_BODY	,//			"Body"
 BONE_SCALE_NAME_CHEST	,//				"Chest"

 BONE_SCALE_NAME_ARM	,//					"Arms"
BONE_SCALE_NAME_FOREARM	,//				"Forearm"
 BONE_SCALE_NAME_HAND	,//				"Hand"
 BONE_SCALE_NAME_PELVIS	,//				"Pelvis"
 BONE_SCALE_NAME_LEGS	,//				"Legs"
 BONE_SCALE_NAME_CALF	,//				"Calf"
 BONE_SCALE_NAME_FOOT	,//				"Foot"

};

CUI_TeachingFrame*		g_pTeachingFrame=NULL;
int			LuaFunc_TeachingFrame_GetTeachtCount		( lua_State *L );
int			LuaFunc_GetTeachInfo		( lua_State *L );
int			LuaFunc_GetTextureUV		( lua_State *L );

int			LuaFunc_GC_GetMouseMoveEnable		( lua_State *L );
int			LuaFunc_GC_SetMouseMoveEnable		( lua_State *L );

int			LuaFunc_GC_SetLButtonCancelTarget	( lua_State *L );
int			LuaFunc_GC_GetLButtonCancelTarget	( lua_State *L );
int			LuaFunc_GC_SetRButtonCancelTarget	( lua_State *L );
int			LuaFunc_GC_GetRButtonCancelTarget	( lua_State *L );
int			LuaFunc_GC_SetLButtonCameraRotateEnable	( lua_State *L );
int			LuaFunc_GC_GetLButtonCameraRotateEnable	( lua_State *L );
int			LuaFunc_GC_SetCameraFollowEnable	( lua_State *L );
int			LuaFunc_GC_GetCameraFollowEnable	( lua_State *L );

int			LuaFunc_GC_SetDisableDisplayNPCTalk	( lua_State *L );
int			LuaFunc_GC_GetDisableDisplayNPCTalk	( lua_State *L );

int			LuaFunc_GC_SetCameraSelectTarget	( lua_State *L );
int			LuaFunc_GC_GetCameraSelectTarget	( lua_State *L );

int			LuaFunc_GC_GetCameraReverseEnable	( lua_State *L );
int			LuaFunc_GC_SetCameraReverseEnable	( lua_State *L );

//int			LuaFunc_GC_GetDisableTitleHide		( lua_State *L );
//int			LuaFunc_GC_SetDisableTitleHide		( lua_State *L );

int			LuaFunc_GC_GetAllSCTVisible	( lua_State *L );
int			LuaFunc_GC_SetAllSCTVisible	( lua_State *L );

int			LuaFunc_GC_GetShowGemePromrt	( lua_State *L );
int			LuaFunc_GC_SetShowGemePromrt	( lua_State *L );

int			LuaFunc_GC_GetPlayerTitleVisible	( lua_State *L );
int			LuaFunc_GC_SetPlayerTitleVisible	( lua_State *L );
int			LuaFunc_GC_GetTitleIconVisible	( lua_State *L );
int			LuaFunc_GC_SetTitleIconVisible	( lua_State *L );

int			LuaFunc_GC_GetNPCTitleVisible	( lua_State *L );
int			LuaFunc_GC_SetNPCTitleVisible	( lua_State *L );


int			LuaFunc_GC_GetSelfTitleVisible	( lua_State *L );
int			LuaFunc_GC_SetSelfTitleVisible	( lua_State *L );


int			LuaFunc_GC_GetSelfCastEnable	( lua_State *L );
int			LuaFunc_GC_SetSelfCastEnable	( lua_State *L );

int			LuaFunc_GC_SetTitleVisible		( lua_State *L );
int			LuaFunc_GC_GetTitleVisible		( lua_State *L );

int			LuaFunc_GC_SetGuildVisible		( lua_State *L );
int			LuaFunc_GC_GetGuildVisible		( lua_State *L );


int			LuaFunc_GC_GetBloodBar	( lua_State *L );
int			LuaFunc_GC_SetBloodBar	( lua_State *L );

int			LuaFunc_GC_GetNpcBloodBar	( lua_State *L );
int			LuaFunc_GC_SetNpcBloodBar	( lua_State *L );
int			LuaFunc_GC_GetOBTitleVisible	( lua_State *L );
int			LuaFunc_GC_SetOBTitleVisible	( lua_State *L );
int			LuaFunc_GC_GetPCBloodBar	( lua_State *L );
int			LuaFunc_GC_SetPCBloodBar	( lua_State *L );
int			LuaFunc_GC_GetMOBBloodBar	( lua_State *L );
int			LuaFunc_GC_SetMOBBloodBar	( lua_State *L );
int			LuaFunc_GC_GetSelfBloodBar	( lua_State *L );
int			LuaFunc_GC_SetSelfBloodBar	( lua_State *L );
int			LuaFunc_GC_SetBloodBarDistance	( lua_State *L );
int			LuaFunc_GC_GetBloodBarDistance	( lua_State *L );


int			LuaFunc_GC_Save	( lua_State *L );
int			LuaFunc_GC_Load	( lua_State *L );
int			LuaFunc_GC_GetGlobalPath(lua_State *L );
int			LuaFunc_GC_GetLocalPath(lua_State *L );
int			LuaFunc_GC_GetAddOnsPath(lua_State *L );

int			LuaFunc_GC_SetServerSaveClientData(lua_State *L );
int			LuaFunc_GC_GetServerSaveClientData(lua_State *L );


int			LuaFunc_BS_GetEquipmentInfo(lua_State *L);
int			LuaFunc_BS_GetEquipmentMainColor(lua_State *L);
int			LuaFunc_BS_GetEquipmentSubColor(lua_State *L);

int			LuaFunc_BS_GetColorCost(lua_State *L);
int			LuaFunc_BS_GetColorTableCount(lua_State *L);

int			LuaFunc_BS_GetFaceCount(lua_State *L);
int			LuaFunc_BS_GetHairCount(lua_State *L);


int			LuaFunc_BS_GetColorTable(lua_State *L);

int			LuaFunc_BS_SetEquipMainColor(lua_State *L);
int			LuaFunc_BS_SetEquipSubColor(lua_State *L);
int			LuaFunc_BS_SetVehicle(lua_State *L);
int			LuaFunc_BS_SetPlayer(lua_State *L);
//int			LuaFunc_BS_ClosePaperDoll(lua_State *L);
int			LuaFunc_BS_SetHairStyle(lua_State *L);
int			LuaFunc_BS_SetFaceStyle(lua_State *L);
int			LuaFunc_BS_RestoreColor(lua_State *L);
int			LuaFunc_BS_RestoreSkinColor(lua_State *L);
int			LuaFunc_BS_RestoreHairColor(lua_State *L);
int			LuaFunc_BS_RestoreHairStyle(lua_State *L);
int			LuaFunc_BS_RestoreFaceStyle(lua_State *L);
int			LuaFunc_BS_GetHairName(lua_State *L);
int			LuaFunc_BS_GetHairStyle(lua_State *L);
int			LuaFunc_BS_GetFaceName(lua_State *L);
int			LuaFunc_BS_GetFaceStyle(lua_State *L);
int			LuaFunc_BS_SetSkinColor(lua_State *L);
int			LuaFunc_BS_SetHairColor(lua_State *L);
int			LuaFunc_BS_GetSkinColor(lua_State *L);
int			LuaFunc_BS_GetHairColor(lua_State *L);
int			LuaFunc_BS_SetHairSwitch(lua_State *L);
int			LuaFunc_BS_Apply(lua_State *L);

int			LuaFunc_BS_GetVehicleItem(lua_State *L);//取得坐騎物品
int			LuaFunc_BS_RemoveVehicleItem(lua_State *L);//移走坐騎物品
int			LuaFunc_BS_VehicleItemColorApply(lua_State *L);
int			LuaFunc_BS_GetVehicleItemColor(lua_State *L);


int			LuaFunc_BSF_number2RGBA(lua_State *L);
int			LuaFunc_BSF_RGBA2number(lua_State *L);
int			LuaFunc_BS_GetVehicleInfo(lua_State *L);

int			LuaFunc_BS_GetPlayerBoneScale(lua_State *L);
int			LuaFunc_BS_SetPlayerBoneScale(lua_State *L);
int			LuaFunc_BS_GetPlayerBoneScaleMaxMin(lua_State *L);
int			LuaFunc_BS_ApplyBodySetting(lua_State *L);

//跑Server劇情
int	LuaFunc_RunGlobalePlot	(lua_State *L) ;


int LuaFunc_GC_OpenWebRadio(lua_State *L) ;



CUI_TeachingFrame::CUI_TeachingFrame(CInterface* object ) : CInterfaceBase( object )
{
	g_pTeachingFrame = this;
	m_runPlot=0;
	m_player=0;
	m_vehicle=0;
	m_beuatyStdio=new ColoringStruct;
	memset(m_beuatyStdio,0,sizeof(ColoringStruct));


	m_beuatyStdio->HairStyle=-1;
	m_beuatyStdio->FaceStyle=-1;
}
/*
CUiPlayerModel * CUI_TeachingFrame::GetModelPlayer(void)
{

	return m_player;
}
*/
CUiModel*   CUI_TeachingFrame::GetModelPlayer(void)
{

	return m_player;
}
	
CUI_TeachingFrame::~CUI_TeachingFrame(void)
{
	if (m_beuatyStdio)
		delete m_beuatyStdio;
	m_beuatyStdio=NULL;

	g_pTeachingFrame = NULL;
}
void CUI_TeachingFrame::RunGlobalPlot(int index)
{
	if (m_runPlot==0)
	{ 
		NetCli_Other::S_RunGlobalPlotRequest(index);
		m_runPlot=1;
	}

}
void CUI_TeachingFrame::EndRunGlobalPlot(int Result)
{

	m_runPlot=0;

}


void CUI_TeachingFrame::RegisterFunc(void)
{
	lua_State* l = GetLuaStateWorld();
	if (l)
	{
		lua_register( l, "TeachingFrame_GetTeachCount",			LuaFunc_TeachingFrame_GetTeachtCount		);
		lua_register( l, "GetTeachInfo",						LuaFunc_GetTeachInfo						);
		lua_register( l, "GetTextureUV",						LuaFunc_GetTextureUV						);

		lua_register( l, "GC_GetMouseMoveEnable",				LuaFunc_GC_GetMouseMoveEnable				);
		lua_register( l, "GC_SetMouseMoveEnable",				LuaFunc_GC_SetMouseMoveEnable				);

		lua_register( l, "GC_SetLButtonCancelTarget",			LuaFunc_GC_SetLButtonCancelTarget			);
		lua_register( l, "GC_GetLButtonCancelTarget",			LuaFunc_GC_GetLButtonCancelTarget			);
		lua_register( l, "GC_SetRButtonCancelTarget",			LuaFunc_GC_SetRButtonCancelTarget			);
		lua_register( l, "GC_GetRButtonCancelTarget",			LuaFunc_GC_GetRButtonCancelTarget			);
		lua_register( l, "GC_SetLButtonCameraRotateEnable",		LuaFunc_GC_SetLButtonCameraRotateEnable		);
		lua_register( l, "GC_GetLButtonCameraRotateEnable",		LuaFunc_GC_GetLButtonCameraRotateEnable		);
		lua_register( l, "GC_SetCameraFollowEnable",			LuaFunc_GC_SetCameraFollowEnable			);
		lua_register( l, "GC_GetCameraFollowEnable",			LuaFunc_GC_GetCameraFollowEnable			);

		lua_register( l, "GC_SetDisableDisplayNPCTalk",			LuaFunc_GC_SetDisableDisplayNPCTalk			);
		lua_register( l, "GC_GetDisableDisplayNPCTalk",			LuaFunc_GC_GetDisableDisplayNPCTalk			);

		lua_register( l, "GC_SetCameraSelectTarget",			LuaFunc_GC_SetCameraSelectTarget			);
		lua_register( l, "GC_GetCameraSelectTarget",			LuaFunc_GC_GetCameraSelectTarget			);

		lua_register( l, "GC_GetCameraReverseEnable",			LuaFunc_GC_GetCameraReverseEnable			);
		lua_register( l, "GC_SetCameraReverseEnable",			LuaFunc_GC_SetCameraReverseEnable			);

	//	lua_register( l, "GC_GetDisableTitleHide",				LuaFunc_GC_GetDisableTitleHide				);
	//	lua_register( l, "GC_SetDisableTitleHide",				LuaFunc_GC_SetDisableTitleHide				);

		lua_register( l, "GC_GetAllSCTVisible",					LuaFunc_GC_GetAllSCTVisible					);
		lua_register( l, "GC_SetAllSCTVisible",					LuaFunc_GC_SetAllSCTVisible					);

		lua_register( l, "GC_GetShowGemePromrt",				LuaFunc_GC_GetShowGemePromrt				);
		lua_register( l, "GC_SetShowGemePromrt",				LuaFunc_GC_SetShowGemePromrt				);
	  
		lua_register( l, "GC_GetPlayerTitleVisible",			LuaFunc_GC_GetPlayerTitleVisible			);
		lua_register( l, "GC_SetPlayerTitleVisible",			LuaFunc_GC_SetPlayerTitleVisible			);
		lua_register( l, "GC_GetTitleIconVisible",				LuaFunc_GC_GetTitleIconVisible				);
		lua_register( l, "GC_SetTitleIconVisible",				LuaFunc_GC_SetTitleIconVisible				);	

		lua_register( l, "GC_GetNPCTitleVisible",				LuaFunc_GC_GetNPCTitleVisible				);
		lua_register( l, "GC_SetNPCTitleVisible",				LuaFunc_GC_SetNPCTitleVisible				);

		lua_register( l, "GC_GetSelfTitleVisible",				LuaFunc_GC_GetSelfTitleVisible				);
		lua_register( l, "GC_SetSelfTitleVisible",				LuaFunc_GC_SetSelfTitleVisible				);

		lua_register( l, "GC_GetSelfCastEnable",				LuaFunc_GC_GetSelfCastEnable				);
		lua_register( l, "GC_SetSelfCastEnable",				LuaFunc_GC_SetSelfCastEnable				);

		lua_register( l, "GC_SetTitleVisible",					LuaFunc_GC_SetTitleVisible					);
		lua_register( l, "GC_GetTitleVisible",					LuaFunc_GC_GetTitleVisible					);

		lua_register( l, "GC_SetGuildVisible",					LuaFunc_GC_SetGuildVisible					);
		lua_register( l, "GC_GetGuildVisible",					LuaFunc_GC_GetGuildVisible					);


		lua_register( l, "GC_SetBloodBar",				LuaFunc_GC_SetBloodBar				);
		lua_register( l, "GC_GetBloodBar",				LuaFunc_GC_GetBloodBar				);
		lua_register( l, "GC_SetNpcBloodBar",				LuaFunc_GC_SetNpcBloodBar				);
		lua_register( l, "GC_GetNpcBloodBar",				LuaFunc_GC_GetNpcBloodBar				);
		lua_register( l, "GC_SetOBTitleVisible",				LuaFunc_GC_SetOBTitleVisible				);
		lua_register( l, "GC_GetOBTitleVisible",				LuaFunc_GC_GetOBTitleVisible				);


		lua_register( l, "GC_SetPCBloodBar",				LuaFunc_GC_SetPCBloodBar				);
		lua_register( l, "GC_GetPCBloodBar",				LuaFunc_GC_GetPCBloodBar				);
		lua_register( l, "GC_SetMOBBloodBar",				LuaFunc_GC_SetMOBBloodBar				);
		lua_register( l, "GC_GetMOBBloodBar",				LuaFunc_GC_GetMOBBloodBar				);
		lua_register( l, "GC_SetSelfBloodBar",				LuaFunc_GC_SetSelfBloodBar				);
		lua_register( l, "GC_GetSelfBloodBar",				LuaFunc_GC_GetSelfBloodBar				);

		lua_register( l, "GC_SetBloodBarDistance",				LuaFunc_GC_SetBloodBarDistance				);
		lua_register( l, "GC_GetBloodBarDistance",				LuaFunc_GC_GetBloodBarDistance				);


		
		lua_register( l, "GC_Save",				LuaFunc_GC_Save				);
		lua_register( l, "GC_Load",				LuaFunc_GC_Load				);

		lua_register( l, "GC_GetGlobalPath",			LuaFunc_GC_GetGlobalPath				);
		lua_register( l, "GC_GetLocalPath",				LuaFunc_GC_GetLocalPath			);
		lua_register( l, "GC_GetAddOnsPath",				LuaFunc_GC_GetAddOnsPath			);

		lua_register( l, "GC_SetServerSaveClientData",				LuaFunc_GC_SetServerSaveClientData			);
		lua_register( l, "GC_GetServerSaveClientData",				LuaFunc_GC_GetServerSaveClientData			);





		lua_register( l, "BS_SetPlayer",				LuaFunc_BS_SetPlayer		);

		lua_register( l, "BS_SetVehicle",				LuaFunc_BS_SetVehicle		);
		
	//	lua_register( l, "BS_ClosePaperDoll",				LuaFunc_BS_ClosePaperDoll		);
		lua_register( l, "BS_GetEquipmentInfo",				LuaFunc_BS_GetEquipmentInfo		);
		lua_register( l, "BS_GetEquipmentMainColor",		LuaFunc_BS_GetEquipmentMainColor		);
		lua_register( l, "BS_GetEquipmentSubColor",			LuaFunc_BS_GetEquipmentSubColor		);
		lua_register( l, "BS_SetEquipMainColor",			LuaFunc_BS_SetEquipMainColor		);
		lua_register( l, "BS_SetEquipSubColor",				LuaFunc_BS_SetEquipSubColor		);

		lua_register( l, "BS_GetColorTableCount",			LuaFunc_BS_GetColorTableCount	);
		lua_register( l, "BS_GetFaceCount",					LuaFunc_BS_GetFaceCount	);
		lua_register( l, "BS_GetHairCount",					LuaFunc_BS_GetHairCount	);


		

		lua_register( l, "BS_GetColorCost",			LuaFunc_BS_GetColorCost	);
		lua_register( l, "BS_GetColorTable",				LuaFunc_BS_GetColorTable	);

		lua_register( l, "BS_GetSkinColor",				LuaFunc_BS_GetSkinColor	);
		lua_register( l, "BS_GetHairColor",				LuaFunc_BS_GetHairColor	);
		
		//lua_register( l, "BS_GetColorTable",				LuaFunc_BS_GetColorTable	);

		lua_register( l, "BS_SetHairStyle",				LuaFunc_BS_SetHairStyle	);
		lua_register( l, "BS_SetFaceStyle",				LuaFunc_BS_SetFaceStyle	);
		lua_register( l, "BS_RestoreHairStyle",				LuaFunc_BS_RestoreHairStyle	);
		lua_register( l, "BS_RestoreFaceStyle",				LuaFunc_BS_RestoreFaceStyle	);
		lua_register( l, "BS_RestoreColor",				LuaFunc_BS_RestoreColor	);
		lua_register( l, "BS_RestoreSkinColor",				LuaFunc_BS_RestoreSkinColor	);
		lua_register( l, "BS_RestoreHairColor",				LuaFunc_BS_RestoreHairColor	);


		
		lua_register( l, "BS_GetHairStyle",			LuaFunc_BS_GetHairStyle	);
		lua_register( l, "BS_GetFaceStyle",			LuaFunc_BS_GetFaceStyle	);
		lua_register( l, "BS_GetHairName",			LuaFunc_BS_GetHairName	);
		lua_register( l, "BS_GetFaceName",			LuaFunc_BS_GetFaceName	);
		lua_register( l, "BS_SetSkinColor",			LuaFunc_BS_SetSkinColor	);
		lua_register( l, "BS_SetHairColor",			LuaFunc_BS_SetHairColor	);
		lua_register( l, "BS_ApplySetting",			LuaFunc_BS_Apply	);
		lua_register( l, "BS_SetHairSwitch",		LuaFunc_BS_SetHairSwitch	);
		lua_register( l, "BS_GetVehicleItem",		LuaFunc_BS_GetVehicleItem	);
		lua_register( l, "BS_RemoveVehicleItem",	LuaFunc_BS_RemoveVehicleItem);


	


		lua_register( l, "RunGlobalePlot",			LuaFunc_RunGlobalePlot	);
		lua_register( l, "GC_OpenWebRadio",			LuaFunc_GC_OpenWebRadio	);
		lua_register( l, "BS_VehicleItemColorApply",			LuaFunc_BS_VehicleItemColorApply	);
		lua_register( l, "BS_GetVehicleItemColor",	LuaFunc_BS_GetVehicleItemColor	);
		lua_register( l, "BSF_number2RGBA",	LuaFunc_BSF_number2RGBA	);
		lua_register( l, "BS_GetVehicleInfo",	LuaFunc_BS_GetVehicleInfo	);
		lua_register( l, "BSF_RGBA2number",	LuaFunc_BSF_RGBA2number	);
		lua_register( l, "BS_GetPlayerBoneScale",	LuaFunc_BS_GetPlayerBoneScale	);
		lua_register( l, "BS_SetPlayerBoneScale",	LuaFunc_BS_SetPlayerBoneScale	);
		lua_register( l, "BS_GetPlayerBoneScaleMaxMin",	LuaFunc_BS_GetPlayerBoneScaleMaxMin	);
		lua_register( l, "BS_ApplyBodySetting",	LuaFunc_BS_ApplyBodySetting	);
	

		
	}
}



int CUI_TeachingFrame::GetTeachtCount(void)
{
	if (g_ObjectData)
	{

		vector< TeachTableStruct>&vecTeach=g_ObjectData->GetTeachInfo();

		return (int)vecTeach.size();
	}
	return 0;

}
TeachTableStruct*  CUI_TeachingFrame::GetTeachInfo(unsigned int index)
{
	if (g_ObjectData)
	{

		vector< TeachTableStruct>&vecTeach=g_ObjectData->GetTeachInfo();

		if (index>=0&&index<vecTeach.size())
			return &vecTeach[index];

	}
	return 0;

}
/*
ColoringShopStruct* CUI_TeachingFrame::GetColoringShopStructByID(int id)
{

	vector<ColoringShopStruct>& ColorTable = g_ObjectData->ColoringShop();

	for(int i=0;i<ColorTable.size();i++)
	{
		if (ColorTable[i].ID==id)
			return &ColorTable[i];


	}

	return NULL;
}*/

/*!
 * \brief
 * Write brief comment for Load here.
 * 
 * \throws <exception class>
 * Description of criteria for throwing this exception.
 * 
 * Write detailed description for Load here.
 * 
 * \remarks
 * Write remarks for Load here.
 * 
 * \see
 * Separate items with the '|' character.
 */
void CUI_TeachingFrame::Load()
{	
}
void CUI_TeachingFrame::Save()
{
}
void CUI_TeachingFrame::getGlobalPath(const char* file,char *buffer,int size)
{
	GetGlobalPath(file,buffer,size);
}
void CUI_TeachingFrame::getLocalPath(const char* file,char *buffer,int size)
{
	GetLocalPath(file,buffer,size);
}

void CUI_TeachingFrame::R_BodyShopResult(bool  Result)
{
	const char *pMsg=g_ObjectData->GetString("SYS_BODYSCALE_FAILED");
	if (Result)

		pMsg=g_ObjectData->GetString("SYS_COLORING_SUCCESS");

	g_pGameMain->SendWarningMsg(pMsg);

	SendWorldEvent("COLORING_END");

}
void CUI_TeachingFrame::R_ColoringShopResult(int  Result)
{

	const char *pMsg=g_ObjectData->GetString("SYS_COLORING_FAILED");

	switch (Result)
	{
	case EM_ColoringShopResult_OK:
		pMsg=g_ObjectData->GetString("SYS_COLORING_SUCCESS");
		break;
	case EM_ColoringShopResult_SecondPasswordError://	二次密碼有問題
		pMsg=g_ObjectData->GetString("SYS_GAMEMSGEVENT_014");
		break;
	case EM_ColoringShopResult_DataError:		//	資料有問題
		pMsg=g_ObjectData->GetString("SYS_FIXEQ_DATA_ERROR");
		break;
	case EM_ColoringShopResult_AccountMoneyError://	帳號幣不足
		pMsg=g_ObjectData->GetString("SYS_AC_NOT_ENOUGH_RUNEMONEY");
		break;

	}

	g_pGameMain->SendWarningMsg(pMsg);


	//CUiPlayerModel* sprite=g_pTeachingFrame->GetModelPlayer();

	//sprite->SetUnit(g_pGameMain->GetUnitSprite("player"));

	//CRoleSprite* player=g_pGameMain->GetPlayer();
	//sprite->CopyPaperdoll(player);

	SendWorldEvent("COLORING_END");
}

int	LuaFunc_TeachingFrame_GetTeachtCount		( lua_State *L )
{
	int iTotal=0;
	if (g_pTeachingFrame)
	{
		iTotal=g_pTeachingFrame->GetTeachtCount();

		lua_pushnumber( L, iTotal );
	}

	return 1;
}


int	LuaFunc_GetTeachInfo		( lua_State *L )
{
	if (g_pTeachingFrame)
	{
		unsigned int iIndex	= luaL_checkint( L, 1 ) - 1;
		TeachTableStruct* pTeach= g_pTeachingFrame->GetTeachInfo(iIndex);
		if (pTeach)
		{
			 lua_pushnumber( L, pTeach-> ID);
			 lua_pushnumber( L, pTeach-> Width);
			 lua_pushnumber( L, pTeach-> Height);
			 lua_pushstring( L, pTeach-> TypeStr );
			 lua_pushstring( L, pTeach-> ImageStr );
			 return 5;
		 }
		
	}
	return 0;
}
static int getTextureSize(int w)
{
	int l=1;
	if (w<0 || w>1024)
		return 256;
	while(l<w)
	{
		l=l<<1;
	}
	return l;

}
int	LuaFunc_GetTextureUV( lua_State *L )
{
	if (g_pTeachingFrame)
	{
		int srcWidth	= luaL_checkint( L, 1 ) ;
		int srcHegiht	= luaL_checkint( L, 2 );


		int texWidth=getTextureSize(srcWidth);
		int texHeight=getTextureSize(srcHegiht);
		float tu=( float)srcWidth/texWidth;
		float tv=( float)srcHegiht/texHeight;
		lua_pushnumber( L, tu );
		lua_pushnumber( L, tv );
		lua_pushnumber( L, texWidth);
		lua_pushnumber( L, texHeight);
		return 4;
	}
	return 0;
}
int	LuaFunc_GC_GetMouseMoveEnable	( lua_State *L )
{
	int ret=0;
	if( g_pGameMain )
	{
		ret=g_pGameMain->GetMouseMoveEnable();
	}
	lua_pushboolean( L, ret );
	return 1;
}

int	LuaFunc_GC_SetMouseMoveEnable		( lua_State *L )
{
	
	if( g_pGameMain )
	{
		bool eable	= lua_toboolean( L, 1 ) != 0;
		g_pGameMain->SetMouseMoveEnable(eable);
	}

	return 0;
}

int	LuaFunc_GC_SetLButtonCancelTarget	( lua_State *L )
{
	if ( g_pGameMain )
	{
        g_pGameMain->SetLButtonCancelTarget(lua_toboolean(L, 1) != 0);
	}
	return 0;
}

int LuaFunc_GC_GetLButtonCancelTarget	( lua_State *L )
{
	if ( g_pGameMain )
		lua_pushboolean(L, g_pGameMain->GetLButtonCancelTarget());
	else
		lua_pushnil(L);
	return 1;
}

int	LuaFunc_GC_SetRButtonCancelTarget	( lua_State *L )
{
	if ( g_pGameMain )
	{
		g_pGameMain->SetRButtonCancelTarget(lua_toboolean(L, 1) != 0);
	}
	return 0;
}

int	LuaFunc_GC_GetRButtonCancelTarget	( lua_State *L )
{
	if ( g_pGameMain )
		lua_pushboolean(L, g_pGameMain->GetRButtonCancelTarget());
	else
		lua_pushnil(L);
	return 1;
}

int LuaFunc_GC_SetLButtonCameraRotateEnable	(lua_State* L)
{
	if ( g_pGameMain )
	{
		g_pGameMain->SetLButtonCameraRotateEnable(lua_toboolean(L, 1) != 0);
	}
	return 1;
}

int LuaFunc_GC_GetLButtonCameraRotateEnable (lua_State* L)
{
	if ( g_pGameMain )
		lua_pushboolean(L, g_pGameMain->GetLButtonCameraRotateEnable());
	else
		lua_pushnil(L);
	return 1;
}

int LuaFunc_GC_SetCameraFollowEnable ( lua_State *L )
{
	if ( g_pGameMain )
	{
		g_pGameMain->SetCameraFollowEnable(lua_toboolean(L, 1) != 0);
	}
	return 1;
}

int	LuaFunc_GC_GetCameraFollowEnable ( lua_State *L )
{
	if ( g_pGameMain )
		lua_pushboolean(L, g_pGameMain->GetCameraFollowEnable());
	else
		lua_pushnil(L);
	return 1;
}

int LuaFunc_GC_SetDisableDisplayNPCTalk	( lua_State *L )
{
	if ( g_pGameMain )
	{
		g_pGameMain->SetDisableDisplayNPCTalk(lua_toboolean(L, 1) != 0);
	}
	return 1;
}

int	LuaFunc_GC_GetDisableDisplayNPCTalk	( lua_State *L )
{
	if ( g_pGameMain )
		lua_pushboolean(L, g_pGameMain->GetDisableDisplayNPCTalk());
	else
		lua_pushnil(L);
	return 1;
}

int LuaFunc_GC_SetCameraSelectTarget ( lua_State* L)
{
	if ( g_pGameMain )
	{
		g_pGameMain->SetCameraSelectTarget(lua_toboolean(L, 1) != 0);
	}
	return 1;
}

int LuaFunc_GC_GetCameraSelectTarget ( lua_State* L )
{
	if ( g_pGameMain )
		lua_pushboolean(L, g_pGameMain->GetCameraSelectTarget());
	else
		lua_pushnil(L);
	return 1;
}

int	LuaFunc_GC_GetCameraReverseEnable	( lua_State *L )
{
	if ( g_pGameMain )
		lua_pushboolean(L, g_pGameMain->GetCameraYReverse());
	else
		lua_pushnil(L);
	return 1;
}

int	LuaFunc_GC_SetCameraReverseEnable	( lua_State *L )
{
	if ( g_pGameMain )
	{
		g_pGameMain->SetCameraYReverse(lua_toboolean(L, 1) != 0);
	}
	return 0;
}
/*

// see GetDisableTitleHide 
// set SetDisableTitleHide
int			LuaFunc_GC_GetDisableTitleHide		( lua_State *L )
{
	int ret=0;
	if( g_pGameMain )
	{
		ret=g_pGameMain->GetDisableTitleHide();
	}
	lua_pushboolean( L, ret );
	return 1;
}
int			LuaFunc_GC_SetDisableTitleHide		( lua_State *L )
{

	if( g_pGameMain )
	{
		bool eable	= lua_toboolean( L, 1 ) != 0;
		g_pGameMain->SetDisableTitleHide(eable);
	}

	return 0;
}
*/
int			LuaFunc_GC_GetAllSCTVisible	( lua_State *L )
{
	int ret=0;
	if( g_pGameMain )
	{//GetDisplayAllSCT
		ret=g_pGameMain->GetDisplayAllSCT();
	}
	lua_pushboolean( L, ret );
	return 1;
}
int			LuaFunc_GC_SetAllSCTVisible	( lua_State *L )
{

	if( g_pGameMain )
	{
		bool eable	= lua_toboolean( L, 1 ) != 0;
				//	SetDisplayAllSCT
		g_pGameMain->SetDisplayAllSCT(eable);
	}

	return 0;
}

int LuaFunc_GC_GetShowGemePromrt( lua_State *L )
{
	int ret=0;
	if( g_pGameMain )
	{
		ret=g_pGameMain->GetShowGemePromrt();
	}
	lua_pushboolean( L, ret );
	return 1;
}

int LuaFunc_GC_SetShowGemePromrt( lua_State *L )
{
	if( g_pGameMain )
	{
		bool eable	= lua_toboolean( L, 1 ) != 0;
		g_pGameMain->SetShowGemePromrt(eable);
	}		  
	return 0;
}

int			LuaFunc_GC_GetPlayerTitleVisible	( lua_State *L )
{
	int ret=0;
	if( g_pGameMain )
	{
		ret=g_pGameMain->GetPlayerTitleVisible();
	}
	lua_pushboolean( L, ret );
	return 1;
}
int			LuaFunc_GC_SetPlayerTitleVisible	( lua_State *L )
{

	if( g_pGameMain )
	{
		bool eable	= lua_toboolean( L, 1 ) != 0;
		g_pGameMain->SetPlayerTitleVisible(eable);
	}

	return 0;
}
// ----------------------------------------------------------------------------
int			LuaFunc_GC_GetTitleIconVisible	( lua_State *L )
{
	lua_pushboolean(L, g_pGameMain->GetTitleIconVisible());
	return 1;
}
// ----------------------------------------------------------------------------
int			LuaFunc_GC_SetTitleIconVisible	( lua_State *L )
{
	g_pGameMain->SetTitleIconVisible(lua_toboolean(L, 1) != 0);
	return 0;
}

int			LuaFunc_GC_GetNPCTitleVisible	( lua_State *L )
{
	int ret=0;
	if( g_pGameMain )
	{
		ret=g_pGameMain->GetNPCTitleVisible();
	}
	lua_pushboolean( L, ret );
	return 1;
}
int			LuaFunc_GC_SetNPCTitleVisible	( lua_State *L )
{

	if( g_pGameMain )
	{
		bool eable	= lua_toboolean( L, 1 ) != 0;
		g_pGameMain->SetNPCTitleVisible(eable);
	}

	return 0;
}

int	LuaFunc_GC_Save	( lua_State *L )
{

	if (g_pTeachingFrame)
	{
		g_pTeachingFrame->Save();
	}

	return 0;
}

int			LuaFunc_GC_Load	( lua_State *L )
{

	if (g_pTeachingFrame)
	{
			g_pTeachingFrame->Load();
	}

	return 0;
}

int	LuaFunc_GC_GetGlobalPath(lua_State *L )
{


	if( g_pTeachingFrame )
	{
		const char * filename= luaL_checkstring( L, 1 ) ;
		char buffer[MAX_PATH];
		g_pTeachingFrame->getGlobalPath(filename, buffer, MAX_PATH);
		lua_pushstring( L, buffer );
	}

	return 1;
}
int	LuaFunc_GC_GetLocalPath(lua_State *L )
{
	if( g_pTeachingFrame )
	{
		const char * filename= luaL_checkstring( L, 1 ) ;
		char buffer[MAX_PATH];
		g_pTeachingFrame->getLocalPath(filename, buffer, MAX_PATH);

		lua_pushstring( L, buffer );
	}

	return 1;

}

static char *TextEquipSoltTab[]=
{
	"helmet",//EM_EQWearPos_Head 
	"hand",//EM_EQWearPos_Gloves
	"foot",//EM_EQWearPos_Shoes
	"torso",//EM_EQWearPos_Clothes
	"leg",//EM_EQWearPos_Pants
	"back",//EM_EQWearPos_Back
	"belt",//EM_EQWearPos_Belt
	"shoulder",//EM_EQWearPos_Shoulder
	"Necklace",//EM_EQWearPos_Necklace
	"Ammo",//EM_EQWearPos_Ammo
	"Ranged",//EM_EQWearPos_Bow
	"Rings",//EM_EQWearPos_Ring1
	"Rings",//EM_EQWearPos_Ring2
	"Earrings",//EM_EQWearPos_Earring1
	"Earrings",//EM_EQWearPos_Earring2
	"MainHand",//EM_EQWearPos_MainHand
	"SecondaryHand",//EM_EQWearPos_SecondHand
	"Pick"//EM_EQWearPos_Manufactory
};

static int _getEquipNameIndex(const char *equipName)
{
	int index=-1;

	for (int i=0;i<sizeof(TextEquipSoltTab)/sizeof(char *);i++)
	{
		if (!strcmp(TextEquipSoltTab[i],equipName))
		{
			index=i;
			break;
		}
	}
	return index;
}

int	LuaFunc_BS_SetVehicle(lua_State *L)
{

	//CRoleSprite* sprite=g_pGameMain->GetSampleSprite();
	//CRoleSprite* player=g_pGameMain->GetPlayer();
	//sprite->CopyPaperdoll(player);

	//CUiPlayerModel* playerModel = dynamic_cast<CUiPlayerModel*>(CUiLuaLayout::CheckLayoutTable(L, 1));
	CUiModel* pModel = dynamic_cast<CUiModel*>(CUiLuaLayout::CheckLayoutTable(L, 1));
	if (g_pTeachingFrame&&g_pGameMain->getMerchant())
		g_pTeachingFrame->m_vehicle=pModel;




	return 0;
}

static void _SetOrigModelID( CUiModel* model,int equipIndex,bool show)
{

	int eqIndex = 0;

	string PaperdollPartName;
	switch( equipIndex )
	{
	case EM_EQWearPos_Head:			// 1
		
		PaperdollPartName = PAPERDOLL_PART_NAME_HELMET;
		eqIndex = 0;
		break;

	case EM_EQWearPos_Gloves:		// 2
		
		PaperdollPartName = PAPERDOLL_PART_NAME_HAND;
		eqIndex = 1;
		break;

	case EM_EQWearPos_Shoes:		// 3
	
		PaperdollPartName = PAPERDOLL_PART_NAME_FOOT;
		eqIndex = 2;
		break;

	case EM_EQWearPos_Clothes:		// 4
		
		PaperdollPartName = PAPERDOLL_PART_NAME_TORSO;
		eqIndex = 3;
		break;

	case EM_EQWearPos_Pants:		// 5
		
		PaperdollPartName = PAPERDOLL_PART_NAME_LEG;
		eqIndex = 4;
		break;

	case EM_EQWearPos_Back:			// 6
	
		PaperdollPartName = PAPERDOLL_PART_NAME_BACK;
		eqIndex = 5;
		break;

	case EM_EQWearPos_Belt:			// 7
		
		PaperdollPartName = PAPERDOLL_PART_NAME_BELT;
		eqIndex = 6;
		break;

	case EM_EQWearPos_Shoulder:		// 8
		
		PaperdollPartName = PAPERDOLL_PART_NAME_SHOULDER;
		eqIndex = 7;
		break;

	case EM_EQWearPos_Ornament:		// 21
		
		eqIndex = 21;
		break;

	default:
		return ;
	}

	bool bShow=show;
	ItemFieldStruct* pItemField = CNetGlobal::RoleData()->GetEqItem( eqIndex );

	if( !pItemField )
	{
		bShow = false;
	}
	if( bShow == false )
	{
		if( equipIndex == EM_EQWearPos_Ornament )
		{
			model->AttachEntity( ATTACH_POINT_BACK_SHIELD, "" );
		}
		else
		{
			model->SetPaperdollPart( PaperdollPartName.c_str(), "" );
			model->SetPaperdollColor( PaperdollPartName.c_str(), 0, 0 );	
		}
	}
	else
	{

		string strACTWorld = "";
		int color[2];
		color[1] = 0;
		color[0] = 0;

		GameObjDbStruct* _itemDB = CNetGlobal::GetObj( pItemField->ImageObjectID );
		if( _itemDB )
		{
			GameObjDbStruct* _imageDB = g_ObjectData->GetObj( _itemDB->ImageID );
			if( _imageDB )
			{
				strACTWorld = _imageDB->Image.ACTWorld;

				if( pItemField && pItemField->MainColor != 0 )
					color[0] = pItemField->MainColor;
				else
					color[0] = _imageDB->Image.ArmorMainColor.Color;	

				if( pItemField && pItemField->OffColor != 0 )
					color[1] = pItemField->OffColor; 
				else
					color[1] = _imageDB->Image.ArmorOffColor.Color;
			}
		}

		if( equipIndex == EM_EQWearPos_Ornament )
		{
			model->AttachEntity( ATTACH_POINT_BACK_SHIELD, strACTWorld.c_str() );
		}
		else
		{
			model->SetPaperdollPart( PaperdollPartName.c_str(), strACTWorld.c_str() );
			model->SetPaperdollColor( PaperdollPartName.c_str(), color[0], color[1] );
		}

	}

}

static void _SetOrigModel( CUiModel* model )
{
	PlayerBaseRoleData* PlayerBaseData = CNetGlobal::RoleData()->PlayerBaseData;
	int eqIndex = 0;	
	if( model )
	{

		int table[]={EM_EQWearPos_Head,EM_EQWearPos_Gloves,EM_EQWearPos_Shoulder,EM_EQWearPos_Clothes,EM_EQWearPos_Belt,EM_EQWearPos_Pants,EM_EQWearPos_Shoes,EM_EQWearPos_Back};
		for (int i=0;i<8;i++)
		{

			_SetOrigModelID(model,table[i],true);

		}


		_SetOrigModelID(model,EM_EQWearPos_Ornament,false);

	}
}





void CUI_TeachingFrame::SetPlayer(CUiModel* player)
{
	m_player=player;


	_SetOrigModel(player);


	for (int i=0;i<11;i++)
		m_boneScale[i]=100;
	if (m_player)
	{
		for (int i=0;i<11;i++)
		{
			m_boneScale[i] = (int)((m_player->GetBoneScale(g_BoneName[i]) + 0.005f) * 100);
		}
	}
}

int	LuaFunc_BS_SetPlayer(lua_State *L)
{

	//CRoleSprite* sprite=g_pGameMain->GetSampleSprite();
	//CRoleSprite* player=g_pGameMain->GetPlayer();
	//sprite->CopyPaperdoll(player);
	
	//CUiPlayerModel* playerModel = dynamic_cast<CUiPlayerModel*>(CUiLuaLayout::CheckLayoutTable(L, 1));
	CUiModel* playerModel = dynamic_cast<CUiModel*>(CUiLuaLayout::CheckLayoutTable(L, 1));
	if (g_pTeachingFrame&&g_pGameMain->getMerchant())
	{
		g_pTeachingFrame->SetPlayer(playerModel);

		


	}



	
	return 0;
}
/*
int	LuaFunc_BS_ClosePaperDoll(lua_State *L)
{

	g_pGameMain->ReleaseSampleSprite();
	
	return 0;
}
*/
int	LuaFunc_BS_GetEquipmentInfo(lua_State* L)
{
	RoleDataEx* role = CNetGlobal::RoleData();
	int index = luaL_checkint(L, 1) - 1;	
	if (index<0|| index >=sizeof(TextEquipSoltTab)/sizeof(char *))
		return 0;

	ItemFieldStruct* item = role->GetEqItem(index);
	char *pText=NULL;
	char temp[512];
	bool bLock=false;
	bool bInvalid=true;
	


	sprintf(temp,"interface\\CharacterFrame\\Equipment-%sSlot",TextEquipSoltTab[index]);
	if ( item )
	{
		if ( !item->IsEmpty() )
		{
			GameObjDbStruct* itemDB = CNetGlobal::GetObj(item->OrgObjID);
			GameObjDbStruct* imageDB = CNetGlobal::GetObj(item->ImageObjectID);

			if (itemDB)
			{
				
				//sprintf(temp,"%s",itemDB->ImageObj->ACTField);

				sprintf(temp,"%s",ItemUTIL::GetItemIcon(item));
				
				bLock= item->Pos != EM_ItemState_NONE;		// push item is locked

				bool bColoring=true;
				if (imageDB)
					 bColoring=imageDB->Mode.Coloring;
				//檢查酷裝 可不可以染
				if(itemDB->Mode.Coloring&&bColoring)
				{
					bInvalid= !CNetGlobal::RoleData()->CanWearObject(itemDB, index);
				}
			}
		
		}

		lua_pushstring(L, temp);			// push item texture file				
		lua_pushboolean(L, bLock);		// push item is locked
		lua_pushboolean(L, bInvalid);
		
		return 3;
	}
	return 0;
}
int			LuaFunc_BS_GetEquipmentMainColor(lua_State *L)
{
	RoleDataEx* role = CNetGlobal::RoleData();
	int index = luaL_checkint(L, 1) - 1;	
	if (index<0|| index >=sizeof(TextEquipSoltTab)/sizeof(char *))
		return 0;

	ItemFieldStruct* item = role->GetEqItem(index);
	int color=item->MainColor;
	float r=((color>>16)&0xff)/255.0f;
	float g=((color>>8)&0xff)/255.0f;
	float b=((color)&0xff)/255.0f;
	float a=((color>>24)&0xff)/255.0f;

	lua_pushnumber(L, r);		// 
	lua_pushnumber(L, g);		// 
	lua_pushnumber(L, b);		// 
	lua_pushnumber(L, a);		// 
	
	return 4;
}

int	LuaFunc_BS_GetEquipmentSubColor(lua_State *L)
{
	RoleDataEx* role = CNetGlobal::RoleData();
	int index = luaL_checkint(L, 1) - 1;	
	if (index<0|| index >=sizeof(TextEquipSoltTab)/sizeof(char *))
		return 0;
	ItemFieldStruct* item = role->GetEqItem(index);
	int color=item->OffColor;
	float r=((color>>16)&0xff)/255.0f;
	float g=((color>>8)&0xff)/255.0f;
	float b=((color)&0xff)/255.0f;
	float a=((color>>24)&0xff)/255.0f;

	lua_pushnumber(L, r);		// 
	lua_pushnumber(L, g);		// 
	lua_pushnumber(L, b);		// 
	lua_pushnumber(L, a);		// 
	
	return 4;
}
//------------------------------------------------------------------
int			LuaFunc_BS_GetFaceCount(lua_State *L)
{



	RoleDataEx*		Role		= CNetGlobal::RoleData();
	int count=0;
	if (Role)	
	{
		count=(int)g_ObjectData->GetBeautyShopFace(Role->BaseData.Race,Role->BaseData.Sex).size();


	}


	lua_pushnumber(L,count);

	return 1;
}
//------------------------------------------------------------------
int			LuaFunc_BS_GetHairCount(lua_State *L)
{
	RoleDataEx*		Role		= CNetGlobal::RoleData();
	int count=0;
	if (Role)	
	{
		count=(int)g_ObjectData->GetBeautyShopHair(Role->BaseData.Race,Role->BaseData.Sex).size();
		

	}
	

	lua_pushnumber(L,count);

	
	return 1;
}

//------------------------------------------------------------------
int			LuaFunc_BS_GetPlayerBoneScaleMaxMin(lua_State *L)
{

	int Type = luaL_checkint(L, 1);
	int  minValue = 80;
	int maxValue = 120;

	switch (Type)
	{
	case 0:
		minValue = 85;
		maxValue = 120;
		break;
	case 1:
		minValue = 80;
		maxValue = 120;
		break;
	case 2:
		minValue = 75;
		maxValue = 130;
		break;
	case 3:
		minValue = 85;
		maxValue = 110;
		break;
	case 4:
		minValue = 80;
		maxValue = 150;
		break;
	case 5:
		minValue = 80;
		maxValue = 130;
		break;
	case 6:
		minValue = 70;
		maxValue = 130;
		break;
	case 7:
		minValue = 80;
		maxValue = 120;
		break;
	case 8:
		minValue = 70;
		maxValue = 120;
		break;
	case 9:
		minValue = 80;
		maxValue = 140;
		break;
	case 10:
		 minValue = 70;
		 maxValue = 130;		
		break;
	}
	lua_pushinteger(L, minValue);
	lua_pushinteger(L, maxValue);	
	
	return 2;
}

//------------------------------------------------------------------
int			LuaFunc_BS_GetPlayerBoneScale(lua_State *L)
{

	int index = luaL_checkint(L, 1);
	int  BoneScale=0;
	if (g_pTeachingFrame&&g_pGameMain->getMerchant())
	 BoneScale=g_pTeachingFrame->GetPlayerBoneScale(index);
	lua_pushinteger(L, BoneScale);	
	return 1;

}
//------------------------------------------------------------------
int			LuaFunc_BS_SetPlayerBoneScale(lua_State *L)
{
	int index = luaL_checkint(L, 1);
	int  BoneScale= (int)luaL_checknumber(L, 2);
	if (g_pTeachingFrame&&g_pGameMain->getMerchant())
	g_pTeachingFrame->SetPlayerBoneScale(index,BoneScale);


	return 0;
}
//------------------------------------------------------------------
int	LuaFunc_BS_ApplyBodySetting(lua_State *L)
{

	if (g_pTeachingFrame&&g_pGameMain->getMerchant())
	{
		g_pTeachingFrame->ApplyBodySetting();

	}
	return 0;

}

//------------------------------------------------------------------
int LuaFunc_BS_GetColorTableCount(lua_State *L)
{

	vector<ColoringShopStruct>& ColorTable = g_ObjectData->ColoringShop();
	lua_pushnumber(L, (lua_Number)ColorTable.size());
	return 1;
}
//------------------------------------------------------------------
int LuaFunc_BS_GetColorCost(lua_State *L)
{

	int	ColoringCost=1;
	int VehicleCost=1;
	int ColoringHair=1;
	int ColoringBody=1;
	int HairStyle=1;
	int FaceStyle=1;
	int body=1;

	if (g_ObjectData)
	{

		ColoringCost = g_ObjectData->GetMoneyKeyValue( "ColoringCost" , 0 );
		VehicleCost=g_ObjectData->GetMoneyKeyValue("ColoringHorseCost", 0 );


		ColoringHair=g_ObjectData->GetMoneyKeyValue("ColoringHair", 0 );
		ColoringBody=g_ObjectData->GetMoneyKeyValue("ColoringBody", 0 );


		HairStyle=g_ObjectData->GetMoneyKeyValue("HairStyle", 0 );
		FaceStyle=g_ObjectData->GetMoneyKeyValue("FaceStyle", 0 );
		body=g_ObjectData->GetMoneyKeyValue("BodyShop", 0 );

	}
		
	lua_pushnumber( L, ColoringCost );
	lua_pushnumber( L, VehicleCost );
	lua_pushnumber( L, ColoringHair );
	lua_pushnumber( L, ColoringBody );
	lua_pushnumber( L, HairStyle );
	lua_pushnumber( L, FaceStyle );
	lua_pushnumber( L, body );
	

	return 7;
}
int	LuaFunc_BS_GetColorTable(lua_State *L)
{
	
	int index =luaL_checkint(L, 1) - 1;
	ColoringShopStruct* ColorTable = g_ObjectData->GetColoringShop(index);
	if (ColorTable)
	{
 		//lua_pushnumber(L, ColorTable->ID);		// 
		lua_pushnumber(L, ColorTable->Type);		// 
		lua_pushnumber(L, ColorTable->Color.R/255.0f);		//   
		lua_pushnumber(L, ColorTable->Color.G/255.0f);		// 
		lua_pushnumber(L, ColorTable->Color.B/255.0f);		// 
		lua_pushnumber(L, ColorTable->Cost);		// 
		lua_pushnumber(L, ColorTable->ObjID);		// 
	}
	return 6;
}


static const char *_GetItemColor(int index,int* color)
{

	const char *pName="";
	ItemFieldStruct* pItemField = CNetGlobal::RoleData()->GetEqItem( index );

	if (pItemField)
	{

		GameObjDbStruct* _itemDB = CNetGlobal::GetObj( pItemField->ImageObjectID );
		if( _itemDB )
		{
			GameObjDbStruct* _imageDB = g_ObjectData->GetObj( _itemDB->ImageID );
			if( _imageDB )
			{
				pName = _imageDB->Image.ACTWorld;

				if( pItemField && pItemField->MainColor != 0 )
					color[0] = pItemField->MainColor;
				else
					color[0] = _imageDB->Image.ArmorMainColor.Color;	

				if( pItemField && pItemField->OffColor != 0 )
					color[1] = pItemField->OffColor; 
				else
					color[1] = _imageDB->Image.ArmorOffColor.Color;
			}
		}
	}
	return pName;

}

int	LuaFunc_BS_SetEquipMainColor(lua_State *L)
{
	RoleDataEx* role = CNetGlobal::RoleData();
	const char* equipName = luaL_checkstring(L, 1) ;	
	
	int NameIndex=luaL_checkint(L, 2) - 1;
	int r,g,b,a;
	int bSet;

	r=(int)(luaL_checknumber(L, 3)*255 );
	g=(int)(luaL_checknumber(L, 4)*255 );
	b=(int)(luaL_checknumber(L, 5)*255 );
	a=(int)(luaL_checknumber(L, 6)*255 );

	bSet=(int)(luaL_checknumber(L, 7));
	int Color=((r&0xff)<<16)|((g&0xff)<<8)|(b&0xff)|(a&0xff)<<24;		
	
	CUiModel* sprite=g_pTeachingFrame->GetModelPlayer();
	if (sprite)
	{		
		ColoringStruct& c=*g_pTeachingFrame->m_beuatyStdio;
		c.Items[NameIndex].MainColor=Color;
		int color[2];			

	//	CRoleSprite* player=g_pGameMain->GetPlayer();

		

		int index=_getEquipNameIndex(equipName);

		
		
		if (index==-1)
			return 0;
		const char *pName=NULL;
		
		if (index!=-1)
		{

			pName=_GetItemColor(index, color);
		}


		//ItemFieldStruct* item = role->GetEqItem(index);
	//	const char *pName=sprite->GetPaperdollPart(equipName);
		//player->GetPaperdollColor(equipName, color);
		if (bSet>0)
		{
			if (color[0]!=Color)
				c.Items[NameIndex].MainColorEnabled=true;
			else
				c.Items[NameIndex].MainColorEnabled=false; 
		}	
		char temp[512];
		
		
		
		strcpy(temp,pName);

		sprite->GetPaperdollColor(equipName,color);

	
		color[0]=Color;
		sprite->SetPaperdollColor(equipName,color[0],color[1]);
		
		sprite->SetPaperdollPart(equipName,temp);
	}
	return 0;
}
int	LuaFunc_BS_SetEquipSubColor(lua_State *L)
{
	RoleDataEx* role = CNetGlobal::RoleData();

	const char* equipName  = luaL_checkstring(L, 1) ;	
	int NameIndex=luaL_checkint(L, 2) - 1;
	int r,g,b,a;
	r=(int)(luaL_checknumber(L, 3)*255 );
	g=(int)(luaL_checknumber(L, 4)*255 );
	b=(int)(luaL_checknumber(L, 5)*255 );
	a=(int)(luaL_checknumber(L, 6)*255 );
	int bSet=(int)(luaL_checknumber(L, 7));
	int Color=((r&0xff)<<16)|((g&0xff)<<8)|(b&0xff)|(a&0xff)<<24;	

	

	CUiModel* sprite=g_pTeachingFrame->GetModelPlayer();	
	if (sprite)
	{		
		int color[2];			
	//	CRoleSprite* player=g_pGameMain->GetPlayer();
		ColoringStruct& c=*g_pTeachingFrame->m_beuatyStdio;
		c.Items[NameIndex].OffColor=Color;

		const char *pName=NULL;

	int index=_getEquipNameIndex(equipName);


	

		if (index!=-1)
		{

			pName=_GetItemColor(index, color);
		}


		//player->GetPaperdollColor(equipName, color);
		if (bSet>0)
		{
			if (color[1]!=Color)
			c.Items[NameIndex].OffColorEnabled=true;
			else
				c.Items[NameIndex].OffColorEnabled=false;
		}
		

		
		
	
		

		
		char temp[512]; strcpy(temp,pName);
			
		sprite->GetPaperdollColor(equipName,color);
			
	
		color[1]=Color;
		sprite->SetPaperdollColor(equipName,color[0],color[1]);

		
		sprite->SetPaperdollPart(equipName,temp);
	}
	return 0;
}
//-----------------------------------------------------------------
/*
int	LuaFunc_BS_Rotate(lua_State *L)
{
	const char* str = luaL_checkstring(L, 1);
	CRoleSprite* sprite = g_pGameMain->GetSampleSprite();
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
*/
int	LuaFunc_BS_RestoreColor(lua_State *L)
{

	RoleDataEx* role = CNetGlobal::RoleData();

	const char* equipName  = luaL_checkstring(L, 1) ;	
	int NameIndex=luaL_checkint(L, 2) - 1;

	//CUiPlayerModel* sprite=g_pTeachingFrame->GetModelPlayer();
	CUiModel* sprite=g_pTeachingFrame->GetModelPlayer();
	if (sprite)
	{


		//CRoleSprite* sprite=g_pGameMain->GetSampleSprite();
		int color[2];
	//	CRoleSprite* player=g_pGameMain->GetPlayer();


		const char *pName=NULL;

		int index=_getEquipNameIndex(equipName);


		

		if (index!=-1)
		{

			pName=_GetItemColor(index, color);
		}


		//if( player != NULL )
		{
			ColoringStruct& c=*g_pTeachingFrame->m_beuatyStdio;

			c.Items[NameIndex].MainColor=0;
			c.Items[NameIndex].MainColorEnabled=false;
			c.Items[NameIndex].OffColor=0;
			c.Items[NameIndex].OffColorEnabled=false;

		//	const char *pName=player->GetPaperdollPart(equipName);
			if( pName != NULL )
			{
				char temp[512]; 
				strcpy(temp,pName);

			//	player->GetPaperdollColor(equipName,color);		
				sprite->SetPaperdollColor(equipName,color[0], color[1]);
				//sprite->PaperdollBuild(false);
				sprite->SetPaperdollPart(equipName,temp);
			}
		}
	}
	return 0;
}
int	LuaFunc_BS_GetFaceStyle(lua_State *L)
{
	CRoleSprite* player=g_pGameMain->GetPlayer();

	if(player)
	{
		const char *facename=player->GetPaperdollPart("head");
		lua_pushstring(L, facename);			// push item texture file		
	}

	return 1;
}
int	LuaFunc_BS_GetHairStyle(lua_State *L)
{
	CRoleSprite* player=g_pGameMain->GetPlayer();

	if(player)
	{
		const char *hairname=player->GetPaperdollPart("hair");
		lua_pushstring(L, hairname);			// push item texture file		
	}

	return 1;
}

int LuaFunc_BS_GetFaceName(lua_State *L)
{
	int id=0;
	int index		= luaL_checkint(L, 1)-1;
	RoleDataEx*		Role		= CNetGlobal::RoleData();

	if (Role)	
	{
		vector<int>& table=g_ObjectData->GetBeautyShopFace(Role->BaseData.Race,Role->BaseData.Sex);
		if (index<table.size())
		{		
			id= table[index];
			//CUiLuaPlayerModel* sprite=g_pTeachingFrame->GetModelPlayer();
			CRoleSprite* player=g_pGameMain->GetPlayer();
			const char* facename = NULL;
			GameObjDbStruct* pObj = g_ObjectData->GetObj(id);			

			if ( pObj )
			{
				GameObjDbStruct* imageDB = CNetGlobal::GetObj(pObj->ImageID);
				if ( imageDB )
				{
					facename = imageDB->Image.ACTWorld;
				}
			}
			else
			{
				pObj = g_ObjectData->GetObj(player->GetObjectId());
				if ( pObj )
				{
					GameObjDbStruct* imageDB = CNetGlobal::GetObj(pObj->ImageID);
					if ( imageDB )
					{
						facename = imageDB->Image.PaperdollHead.Name;
					}
				}
			}

			lua_pushstring(L, facename);	
			return 1;
		}
	}

	return 0;

}
int LuaFunc_BS_GetHairName(lua_State *L)
{
	int id=0;
	int index		= luaL_checkint(L, 1)-1;

	RoleDataEx*		Role		= CNetGlobal::RoleData();
	
	if (Role)	
	{
		vector<int>& table=g_ObjectData->GetBeautyShopFace(Role->BaseData.Race,Role->BaseData.Sex);
		if (index<table.size())
		{
			id= table[index];
			//CUiLuaPlayerModel* sprite=g_pTeachingFrame->GetModelPlayer();
			CRoleSprite* player=g_pGameMain->GetPlayer();
			const char* hairname = NULL;
			GameObjDbStruct* pObj = g_ObjectData->GetObj(id);
			if ( pObj )
			{
				GameObjDbStruct* imageDB = CNetGlobal::GetObj(pObj->ImageID);
				if ( imageDB )
				{
					hairname = imageDB->Image.ACTWorld;
				}
			}
			else
			{
				pObj = g_ObjectData->GetObj(player->GetObjectId());
				if ( pObj )
				{
					GameObjDbStruct* imageDB = CNetGlobal::GetObj(pObj->ImageID);
					if ( imageDB )
					{						
						hairname = imageDB->Image.PaperdollHair.Name;
					}
				}
			}

			lua_pushstring(L, hairname);	
			return 1;
		}
	}

	return 0;

}
/*
int	LuaFunc_BS_SetBodyStyle(lua_State *L)
{
	int skinColor;
	int hairColor;
	//int face;
	int hair;

	CRoleSprite* sprite=g_pGameMain->GetSampleSprite();
	if (sprite)
	{	
		sprite->SetPaperdollSkinColor(skinColor);
		sprite->SetPaperdollHairColor(hairColor);
	//	sprite->SetPaperdollPart(PAPERDOLL_PART_NAME_HEAD, face);
		sprite->SetPaperdollPart(PAPERDOLL_PART_NAME_HAIR, hair);
	}

	return 0;
}*/


int	LuaFunc_BS_SetHairStyle(lua_State *L)
{

	int hair		= luaL_checkint(L, 1) -1;
	int id=0;



	RoleDataEx*		Role		= CNetGlobal::RoleData();

	if (Role)	
	{
		vector<int>& table=g_ObjectData->GetBeautyShopHair(Role->BaseData.Race,Role->BaseData.Sex);
		if (hair<table.size())
		{
			id= table[hair];
		


			//vector<int>& hairVec = g_ObjectData->GetHairInfo();
			//if ( id >= 0 && id < (int)hairVec.size() )
			//	id= hairVec[hair];

			ColoringStruct& c=*g_pTeachingFrame->m_beuatyStdio;
			c.HairStyle=-1;
			//CUiPlayerModel* sprite=g_pTeachingFrame->GetModelPlayer();
			CUiModel* sprite=g_pTeachingFrame->GetModelPlayer();
			//CRoleSprite* sprite=g_pGameMain->GetSampleSprite();
			if (sprite)
			{
				CRoleSprite* player=g_pGameMain->GetPlayer();
				const char* hairname = NULL;
				GameObjDbStruct* pObj = g_ObjectData->GetObj(id);
				if ( pObj )
				{
					GameObjDbStruct* imageDB = CNetGlobal::GetObj(pObj->ImageID);
					if ( imageDB )
					{
						hairname = imageDB->Image.ACTWorld;
					}
				}
				else
				{
					pObj = g_ObjectData->GetObj(player->GetObjectId());
					if ( pObj )
					{
						GameObjDbStruct* imageDB = CNetGlobal::GetObj(pObj->ImageID);
						if ( imageDB )
						{
							hairname = imageDB->Image.PaperdollHair.Name;
						}
					}
				}

				const char *temp;
				temp=player->GetPaperdollPart(PAPERDOLL_PART_NAME_HAIR);
				if (strcmp(temp,hairname))
				{
					c.HairStyle=id;			
				}


				sprite->SetPaperdollPart(PAPERDOLL_PART_NAME_HAIR,hairname);
	

				//sprite->PaperdollBuild(false);
			}
		}
	}

	return 0;
}
int	LuaFunc_BS_SetFaceStyle(lua_State *L)
{

	int face		= luaL_checkint(L, 1) -1;
	int id=0;
	RoleDataEx*		Role		= CNetGlobal::RoleData();

	if (Role)	
	{
		vector<int>& table=g_ObjectData->GetBeautyShopFace(Role->BaseData.Race,Role->BaseData.Sex);
		if (face<table.size())
		{
			id= table[face];
		
	
			//vector<int>& hairVec = g_ObjectData->GetHairInfo();
			//if ( id >= 0 && id < (int)hairVec.size() )
			//	id= hairVec[hair];

			ColoringStruct& c=*g_pTeachingFrame->m_beuatyStdio;
			c.FaceStyle=-1;
			//CUiPlayerModel* sprite=g_pTeachingFrame->GetModelPlayer();
			CUiModel* sprite=g_pTeachingFrame->GetModelPlayer();
			//CRoleSprite* sprite=g_pGameMain->GetSampleSprite();
			if (sprite)
			{
				CRoleSprite* player=g_pGameMain->GetPlayer();
				const char* facename = NULL;
				GameObjDbStruct* pObj = g_ObjectData->GetObj(id);
				if ( pObj )
				{
					GameObjDbStruct* imageDB = CNetGlobal::GetObj(pObj->ImageID);
					if ( imageDB )
					{
						facename = imageDB->Image.ACTWorld;
					}
				}
				else
				{
					pObj = g_ObjectData->GetObj(player->GetObjectId());
					if ( pObj )
					{
						GameObjDbStruct* imageDB = CNetGlobal::GetObj(pObj->ImageID);
						if ( imageDB )
						{
							facename = imageDB->Image.PaperdollHead.Name;
						}
					}
				}

				const char *temp;
				temp=	player->GetPaperdollPart(PAPERDOLL_PART_NAME_HEAD);
				if (strcmp(temp,facename))
				{
					c.FaceStyle=id;			
				}

				sprite->SetPaperdollPart(PAPERDOLL_PART_NAME_HEAD,facename);
				//sprite->PaperdollBuild(false);
			}
		}
	}
	return 0;
}
int	LuaFunc_BS_SetHairColor(lua_State *L)
{

	RoleDataEx* role = CNetGlobal::RoleData();

	int r,g,b;
	r=(int)(luaL_checknumber(L, 1)*255 );
	g=(int)(luaL_checknumber(L, 2)*255 );
	b=(int)(luaL_checknumber(L, 3)*255 );
	int hairColor=((r&0xff)<<16)|((g&0xff)<<8)|(b&0xff)|0xff000000;
	int bSet=(int)(luaL_checknumber(L, 4));
	CUiModel* sprite=g_pTeachingFrame->GetModelPlayer();
	if (sprite)
	{		
		ColoringStruct& c=*g_pTeachingFrame->m_beuatyStdio;
		c.HairColor=hairColor;
		
		CRoleSprite* player=g_pGameMain->GetPlayer();
		int color[2];
		player->GetPaperdollColor(PAPERDOLL_PART_NAME_HAIR, color);

		if (bSet>0)
		{
			if (color[0]!=hairColor)
				c.HairColorEnabled=true;
			else
				c.HairColorEnabled=false;

		}
		
	
		
		//CRoleSprite* player=g_pGameMain->GetPlayer();
	
		char temp[512]; 

		sprite->GetPaperdollPart(PAPERDOLL_PART_NAME_HEAD,temp);
	
	
		color[0]=hairColor;
		
		color[1]=0;
		sprite->SetPaperdollColor(PAPERDOLL_PART_NAME_HAIR,color[0], color[1]);
	
		sprite->SetPaperdollPart(PAPERDOLL_PART_NAME_HEAD,temp);
	}
	return 0;
}

int	LuaFunc_BS_SetSkinColor(lua_State *L)
{
	RoleDataEx* role = CNetGlobal::RoleData();

	int r,g,b;
	r=(int)(luaL_checknumber(L, 1)*255 );
	g=(int)(luaL_checknumber(L, 2)*255 );
	b=(int)(luaL_checknumber(L, 3)*255 );
	int SkinColor=((r&0xff)<<16)|((g&0xff)<<8)|(b&0xff)|0xff000000;	
	int bSet=(int)(luaL_checknumber(L, 4));
	CUiModel* sprite=g_pTeachingFrame->GetModelPlayer();
	if (sprite)
	{		
		ColoringStruct& c=*g_pTeachingFrame->m_beuatyStdio;
		c.BodyColor=SkinColor;
		CRoleSprite* player=g_pGameMain->GetPlayer();
		int color[2];
		player->GetPaperdollColor(PAPERDOLL_PART_NAME_HEAD, color);
		if (bSet>0)
		{
			if (color[0]!=SkinColor)
				c.BodyColorEnabled=true;
			else
				c.BodyColorEnabled=false;
		}
		
	
		
	
		char temp[512]; 
		sprite->GetPaperdollPart(PAPERDOLL_PART_NAME_HEAD,temp);

		
		
		
		color[0]=SkinColor;
		color[1]=0;
		sprite->SetPaperdollColor(PAPERDOLL_PART_NAME_HEAD,color[0],color[1]);


		sprite->SetPaperdollPart(PAPERDOLL_PART_NAME_HEAD,temp);
		
	}
	return 0;

}


int	LuaFunc_BS_RestoreHairColor(lua_State *L)
{

	ColoringStruct& c=*g_pTeachingFrame->m_beuatyStdio;
	c.HairColorEnabled=false;
	c.HairColor=0;

	//CRoleSprite* sprite=g_pGameMain->GetSampleSprite();
	//CUiPlayerModel* sprite=g_pTeachingFrame->GetModelPlayer();
	CUiModel* sprite=g_pTeachingFrame->GetModelPlayer();
	if (sprite)
	{

	
	CRoleSprite* player=g_pGameMain->GetPlayer();

	int color[2];
	player->GetPaperdollColor(PAPERDOLL_PART_NAME_HAIR, color);
	sprite->SetPaperdollColor(PAPERDOLL_PART_NAME_HAIR, color[0], color[1]);
	

	const char *pName=player->GetPaperdollPart(PAPERDOLL_PART_NAME_HEAD);
	char temp[512]; strcpy(temp,pName);

	
	//sprite->PaperdollBuild(false);
	sprite->SetPaperdollPart(PAPERDOLL_PART_NAME_HEAD,temp);
	}
	return 0;
}
int	LuaFunc_BS_RestoreFaceStyle(lua_State *L)
{


	ColoringStruct& c=*g_pTeachingFrame->m_beuatyStdio;
	c.FaceStyle=-1;
	//	CUiPlayerModel* sprite=g_pTeachingFrame->GetModelPlayer();
	CRoleSprite* player=g_pGameMain->GetPlayer();
	CUiModel* sprite=g_pTeachingFrame->GetModelPlayer();
	//CRoleSprite* sprite=g_pGameMain->GetSampleSprite();
	if (sprite&&player)
	{
		sprite->SetPaperdollPart(PAPERDOLL_PART_NAME_HEAD,		player->GetPaperdollPart(PAPERDOLL_PART_NAME_HEAD));


		//sprite->PaperdollBuild(false);
	}

	return 0;

}
int LuaFunc_BS_SetHairSwitch(lua_State *L)
{

	int sw=luaL_checkint(L,1);
	ColoringStruct& c=*g_pTeachingFrame->m_beuatyStdio;
	c.HairStyle=-1;
	//	CUiPlayerModel* sprite=g_pTeachingFrame->GetModelPlayer();
	CRoleSprite* player=g_pGameMain->GetPlayer();
	CUiModel* sprite=g_pTeachingFrame->GetModelPlayer();
	//CRoleSprite* sprite=g_pGameMain->GetSampleSprite();
	if (sprite&&player)
	{
		const char *pName=player->GetPaperdollPart(PAPERDOLL_PART_NAME_HAIR);
		if (sw==0)
			pName=NULL;

		sprite->SetPaperdollPart(PAPERDOLL_PART_NAME_HAIR,	pName	);


		//sprite->PaperdollBuild(false);
	}

	return 0;

	

}
int	LuaFunc_BS_RestoreHairStyle(lua_State *L)
{


	ColoringStruct& c=*g_pTeachingFrame->m_beuatyStdio;
	c.HairStyle=-1;
//	CUiPlayerModel* sprite=g_pTeachingFrame->GetModelPlayer();
	CRoleSprite* player=g_pGameMain->GetPlayer();
	CUiModel* sprite=g_pTeachingFrame->GetModelPlayer();
	//CRoleSprite* sprite=g_pGameMain->GetSampleSprite();
	if (sprite&&player)
	{
		sprite->SetPaperdollPart(PAPERDOLL_PART_NAME_HAIR,		player->GetPaperdollPart(PAPERDOLL_PART_NAME_HAIR));
	

		//sprite->PaperdollBuild(false);
	}

	return 0;

}
int	LuaFunc_BS_RestoreSkinColor(lua_State *L)
{

	ColoringStruct& c=*g_pTeachingFrame->m_beuatyStdio;
	c.BodyColorEnabled=false;
	c.BodyColor=0;
	
	CUiModel* sprite=g_pTeachingFrame->GetModelPlayer();

	CRoleSprite* player=g_pGameMain->GetPlayer();
	if (player&&sprite)
	{
		int color[2];
		player->GetPaperdollColor(PAPERDOLL_PART_NAME_HEAD, color);
		sprite->SetPaperdollColor(PAPERDOLL_PART_NAME_HEAD, color[0], color[1]);

	
		const char *pName=player->GetPaperdollPart(PAPERDOLL_PART_NAME_HEAD);
		char temp[512]; strcpy(temp,pName);
		sprite->SetPaperdollPart(PAPERDOLL_PART_NAME_HEAD,temp);
		

	}	return 0;
}

union XX
{
	struct  
	{
		unsigned char  b, g, r ,a;
	};
	int color;

};

int			LuaFunc_BS_GetHairColor(lua_State *L)
{
	CRoleSprite* player=g_pGameMain->GetPlayer();
	if (player)
	{
	int color[2];
	XX value;
	player->GetPaperdollColor(PAPERDOLL_PART_NAME_HAIR, color);
	value.color = color[0];
	lua_pushnumber(L, value.r/255.0f);		//   
	lua_pushnumber(L, value.g/255.0f);		// 
	lua_pushnumber(L, value.b/255.0f);		// 
	}
	return 3;
}

int	LuaFunc_BS_GetSkinColor(lua_State *L)
{
	CRoleSprite* player=g_pGameMain->GetPlayer();
	if (player)
	{
	int color[2];
	XX value;
	player->GetPaperdollColor(PAPERDOLL_PART_NAME_HEAD, color);
	value.color = color[0];
	lua_pushnumber(L, value.r/255.0f);		//   
	lua_pushnumber(L, value.g/255.0f);		// 
	lua_pushnumber(L, value.b/255.0f);		// 
	}
	return 3;
}


int	LuaFunc_BS_Apply(lua_State *L)
{
	//const char *password=luaL_checkstring(L,1);

	const char *password=g_pGameMain->GetSecondPassword();
	
	if (g_pTeachingFrame&&g_pGameMain->getMerchant())
	{
		ColoringStruct& c=*g_pTeachingFrame->m_beuatyStdio;

		NetCli_Other::S_ColoringShopRequest(c,password);

		memset(&c,0,sizeof(ColoringStruct));
		c.HairStyle=-1;
		c.FaceStyle=-1;
	}
	


	

	
	return 0;
}

int			LuaFunc_GC_GetSelfTitleVisible	( lua_State *L )
{

	int ret=0;
	if( g_pGameMain )
	{
		ret=g_pGameMain->GetSelfTitleVisible();
	}
	lua_pushboolean( L, ret );
	return 1;
}
int			LuaFunc_GC_SetSelfTitleVisible	( lua_State *L )
{
	if( g_pGameMain )
	{
		bool eable	= lua_toboolean( L, 1 ) != 0;
		g_pGameMain->SetSelfTitleVisible(eable);
	}

	return 0;
}


int			LuaFunc_GC_GetSelfCastEnable	( lua_State *L )
{
	int ret=0;
	if( g_pGameMain )
	{
		ret=g_pGameMain->GetSelfCastState();
	}
	lua_pushboolean( L, ret );
	return 1;

}
int			LuaFunc_GC_SetSelfCastEnable	( lua_State *L )
{
	if( g_pGameMain )
	{
		bool eable	= lua_toboolean( L, 1 ) != 0;
		g_pGameMain->SetSelfCastState(eable);
	}

	return 0;
}
int			LuaFunc_GC_SetTitleVisible		( lua_State *L )
{
	g_pGameMain->SetTitleVisible(lua_toboolean(L, 1) != 0);
	return 0;
}
int			LuaFunc_GC_GetTitleVisible		( lua_State *L )
{
	lua_pushboolean(L, g_pGameMain->GetTitleVisible());
	return 1;
}
int			LuaFunc_GC_SetGuildVisible		( lua_State *L )
{
	g_pGameMain->SetGuildVisible(lua_toboolean(L, 1) != 0);
	return 0;
}
int			LuaFunc_GC_GetGuildVisible		( lua_State *L )
{
	lua_pushboolean(L, g_pGameMain->GetGuildVisible());
	return 1;
}

int		LuaFunc_RunGlobalePlot	(lua_State *L) 
{
	int index = luaL_checkint(L, 1) ;	
	g_pTeachingFrame->RunGlobalPlot(index);		

	return 0;
}

int			LuaFunc_GC_GetBloodBar	( lua_State *L )
{
	int ret=0;
	if( g_pObjectBloodBar )
	{
		ret=g_pObjectBloodBar->GetBloodBar();
	}
	lua_pushboolean( L, ret );
	return 1;

}
int			LuaFunc_GC_SetBloodBar	( lua_State *L )
{

	if( g_pObjectBloodBar )
	{
		bool eable	= lua_toboolean( L, 1 ) != 0;
		g_pObjectBloodBar->SetBloodBar(eable);
	}
	return 0;
}

int			LuaFunc_GC_GetNpcBloodBar	( lua_State *L )
{
	int ret=0;
	if( g_pObjectBloodBar )
	{
		ret=g_pObjectBloodBar->GetVisable();
	}
	lua_pushboolean( L, ret&OBB_NPC_FLAG );
	return 1;
}
int			LuaFunc_GC_SetNpcBloodBar	( lua_State *L )
{

	if( g_pObjectBloodBar )
	{
		bool eable	= lua_toboolean( L, 1 ) != 0;
		int ret=g_pObjectBloodBar->GetVisable()&~OBB_NPC_FLAG;
		if (eable)
			ret|=OBB_NPC_FLAG;

		g_pObjectBloodBar->SetVisable(ret);
	}
	return 0;

}
int			LuaFunc_GC_SetOBTitleVisible( lua_State *L )
{

	if( g_pObjectBloodBar )
	{
		bool eable	= lua_toboolean( L, 1 ) != 0;
		int ret=g_pObjectBloodBar->GetVisable()&~OBB_TITLE_FLAG;
		if (eable)
			ret|=OBB_TITLE_FLAG;

		g_pObjectBloodBar->SetVisable(ret);
	}
	return 0;

}
int			LuaFunc_GC_GetOBTitleVisible	( lua_State *L )
{
	int ret=0;
	if( g_pObjectBloodBar )
	{
		ret=g_pObjectBloodBar->GetVisable();
	}
	lua_pushboolean( L, ret&OBB_TITLE_FLAG );
	return 1;
}
int			LuaFunc_GC_GetPCBloodBar	( lua_State *L )
{
	int ret=0;
	if( g_pObjectBloodBar )
	{
		ret=g_pObjectBloodBar->GetVisable();
	}
	lua_pushboolean( L, ret&OBB_PLAYER_FLAG );
	return 1;
}
int			LuaFunc_GC_SetPCBloodBar	( lua_State *L )
{
	if( g_pObjectBloodBar )
	{
		bool eable	= lua_toboolean( L, 1 ) != 0;
		int ret=g_pObjectBloodBar->GetVisable()&~OBB_PLAYER_FLAG;
		if (eable)
			ret|=OBB_PLAYER_FLAG;

		g_pObjectBloodBar->SetVisable(ret);
	}
	return 0;
}
int			LuaFunc_GC_GetMOBBloodBar	( lua_State *L )
{
	int ret=0;
	if( g_pObjectBloodBar )
	{
		ret=g_pObjectBloodBar->GetVisable();
		
	}
	lua_pushboolean( L, ret&OBB_EMENY_FLAG );
	return 1;

}
int			LuaFunc_GC_SetMOBBloodBar	( lua_State *L )
{

	if( g_pObjectBloodBar )
	{
		bool eable	= lua_toboolean( L, 1 ) != 0;

		int ret=g_pObjectBloodBar->GetVisable()&~OBB_EMENY_FLAG;
		if (eable)
			ret|=OBB_EMENY_FLAG;

		g_pObjectBloodBar->SetVisable(ret);
	}
	return 0;
}
int			LuaFunc_GC_GetSelfBloodBar	( lua_State *L )
{
	int ret=0;
	if( g_pObjectBloodBar )
	{
		ret=g_pObjectBloodBar->GetVisable();

	}
	lua_pushboolean( L, ret&OBB_SELF_FLAG );
	return 1;

}
int			LuaFunc_GC_SetSelfBloodBar	( lua_State *L )
{

	if( g_pObjectBloodBar )
	{
		bool eable	= lua_toboolean( L, 1 ) != 0;

		int ret=g_pObjectBloodBar->GetVisable()&~OBB_SELF_FLAG;
		if (eable)
			ret|=OBB_SELF_FLAG;

		g_pObjectBloodBar->SetVisable(ret);
	}
	return 0;
}

int			LuaFunc_GC_SetBloodBarDistance	( lua_State *L )
{
	if( g_pObjectBloodBar )
	{
		int distance = (int)lua_tonumber(L, 1);
		if (distance<=80)
			distance=80;
		if (distance>=200)
			distance=200;
		g_pObjectBloodBar->SetDistance(distance);

	}

	return 0;
}
int			LuaFunc_GC_GetBloodBarDistance	( lua_State *L )
{
	int  distance	=120;
	if( g_pObjectBloodBar )
	{
		distance=g_pObjectBloodBar->GetDistance();

		if (distance<=80)
			distance=80;
		if (distance>=200)
			distance=200;
		
	}
	lua_pushnumber( L, distance );
	return 1;

}

void CUI_TeachingFrame::getResourcePath(char *buffer,int size)
{
	const char *path=m_interface->GetResourcePath();
	strncpy(buffer,path,size);

}

int	LuaFunc_GC_GetAddOnsPath(lua_State *L )
{
	char path[MAX_PATH];
	char buffer[MAX_PATH];
	
	g_pTeachingFrame->getResourcePath(path,MAX_PATH);

	int len=(int)strlen(path);
	if (path[len-1]!='\\' &&path[len-1]!='/' )
		strcat(path,"\\");

	const char * filename= luaL_checkstring( L, 1 ) ;
		
	sprintf(buffer,"%s%s",path,filename);

	lua_pushstring( L, buffer );
	
	return 1;
}

//-----------------------------------------------------
int LuaFunc_GC_OpenWebRadio(lua_State *L) 
{
	std::string WebLink = luaL_checkstring( L, 1 ) ;
	std::string	KeyStr = "";

	char Buff[64];
	size_t found = std::string::npos;

	//-------------------------------------------------	
	if (g_pGameMain != NULL)
	{
		KeyStr = "[$ROLEDBID]";
		sprintf(Buff, "%d", g_pGameMain->GetDBID());

		found = WebLink.find(KeyStr.c_str());
		while (found != std::string::npos)
		{
			WebLink.replace(found, KeyStr.length(), Buff);
			found = WebLink.find(KeyStr.c_str());
		}
	}
	//-------------------------------------------------

	//-------------------------------------------------	
	if (g_pAccountLogin != NULL)
	{
		KeyStr = "[$WORLDID]";
		sprintf(Buff, "%d", g_pAccountLogin->GetWorldID());

		found = WebLink.find(KeyStr.c_str());
		while (found != std::string::npos)
		{
			WebLink.replace(found, KeyStr.length(), Buff);
			found = WebLink.find(KeyStr.c_str());
		}
	}
	//-------------------------------------------------

	//-------------------------------------------------
	KeyStr = "[$SESSIONTYPE]";
	sprintf(Buff, "%d", EM_SessionType_OpenItemMall);

	found = WebLink.find(KeyStr.c_str());
	while (found != std::string::npos)
	{
		WebLink.replace(found, KeyStr.length(), Buff);
		found = WebLink.find(KeyStr.c_str());
	}
	//-------------------------------------------------

	//-------------------------------------------------
	if (g_pGameMain != NULL)
	{
		KeyStr = "[$SESSIONID]";
		sprintf(Buff, "%d", g_pGameMain->GetItemMallSessionID());

		found = WebLink.find(KeyStr.c_str());
		while (found != std::string::npos)
		{
			WebLink.replace(found, KeyStr.length(), Buff);
			found = WebLink.find(KeyStr.c_str());
		}
	}

	//-------------------------------------------------

	//-------------------------------------------------
	if (g_pGameMain != NULL)
	{
		HWND hWnd=g_pGameMain->GetWnd();
		ShellExecute(hWnd, "open", WebLink.c_str(), 0, 0, SW_SHOW);
	}
	//-------------------------------------------------

	//const char * weblink= luaL_checkstring( L, 1 ) ;

	//HWND hWnd=g_pGameMain->GetWnd();

	//ShellExecute(hWnd, "open", weblink, 0, 0, SW_SHOW);

	return 0;

}

int LuaFunc_GC_SetServerSaveClientData(lua_State *L )
{
	bool bIsServerSaveClientData = lua_toboolean( L, 1 ) != 0;

	g_pGameMain->SetServerSaveClientData( bIsServerSaveClientData );

	return 0;
}

int LuaFunc_GC_GetServerSaveClientData(lua_State *L )
{
	bool bIsServerSaveClientData = g_pGameMain->GetServerSaveClientData();

	lua_pushboolean( L, bIsServerSaveClientData );

	return 1;
}

void CUI_TeachingFrame::ApplyBodySetting(void)
{
	char BoneScale[15];
	for (int i=0;i<15;i++)
		BoneScale[i]=100;



	BoneScale[0]	= (BYTE) (m_boneScale[0] );
	BoneScale[1]	= (BYTE) (m_boneScale[1] );
	BoneScale[2]	= (BYTE) (m_boneScale[2] );
	BoneScale[3]	= (BYTE) (m_boneScale[3] );
	
	BoneScale[4]	= (BYTE) (m_boneScale[7] );
	BoneScale[5]	= (BYTE) (m_boneScale[4] );
	BoneScale[6]	= (BYTE) ( m_boneScale[5] );
	BoneScale[7]	= (BYTE) (m_boneScale[6] );
	BoneScale[8]	= (BYTE) (m_boneScale[8] );
	BoneScale[9]	= (BYTE) (m_boneScale[9]  );
	BoneScale[10]	= (BYTE) (m_boneScale[10] );


	NetCli_Other::SL_BodyShop(BoneScale);
}

int  CUI_TeachingFrame::GetClipboardVehicleItem(void)
{

	lua_State* L=GetLuaState();

	ClipboardItemInfo itemInfo = g_pItemClipboard->GetItemInfo();
	ClipboardItemType itemType = g_pItemClipboard->GetType();
	ItemFieldStruct* item = NULL;

	//背包來者
	if( itemType == eClipboardItem_Type_Bag )
	{
		//背包位置
		int index=itemInfo.bag.pos;

		//有物品嗎
		item = CNetGlobal::RoleData()->GetBodyItem(index);
		if (item)
		{
			//拿出資料庫物件
			GameObjDbStruct* itemDB = CNetGlobal::GetObj(item->OrgObjID);	
			if (itemDB&& itemDB->Classification==EM_ObjectClass_Item &&(itemDB->Item.ItemType== EM_ItemType_Normal  ||itemDB->Item.ItemType==EM_ItemType_SummonHorse) &&itemDB->Item.UseType==EM_ItemUseType_Magic)
			{	
				int imageObj=itemDB->Item.IncMagic_Onuse;
				
				GameObjDbStruct* MagicItem = CNetGlobal::GetObj(imageObj);	
				if (MagicItem&&MagicItem->Classification==EM_ObjectClass_MagicCollect)
				{
					
					GameObjDbStruct* MagicBaseDB = CNetGlobal::GetObj(MagicItem->MagicCol.Magic_List[0].MagicBaseID);
					if (MagicBaseDB)
					{
						const char *TextureName=NULL;

						if (MagicBaseDB->MagicBase.MagicFunc==EM_MAGICFUN_Assist)
						{

							if (MagicBaseDB->MagicBase.Effect.Ride==true)
							{
								int imagedb=MagicBaseDB->MagicBase.RideID;

								GameObjDbStruct* imageDB = CNetGlobal::GetObj(itemDB->ImageID);
								if ( imageDB )					
									TextureName = imageDB->Image.ACTField;
					
								g_pItemClipboard->Clear();
								item->Pos=EM_USE_GUILDCONTRIBUTION;
								g_pBagFrame->ClearPickupItemIndex();

								RemoveVehicleItem();
								m_ItemPos=index;
								lua_pushnumber(L, index+1);			
								lua_pushstring(L,TextureName);			// push item texture file
								lua_pushnumber(L, imagedb);	

								lua_pushnumber(L,item-> HorseColor[0]);	
								lua_pushnumber(L, item->HorseColor[1]);	
								lua_pushnumber(L, item->HorseColor[2]);	
								lua_pushnumber(L,item-> HorseColor[3]);								

								
								return 7;
							}


						}

					}


				}
			
				
			}
			else
			{

				const char* tempStr;

				tempStr=g_ObjectData->GetString("MSG_BSF_ERRORITEM");

				g_pGameMain->SendWarningMsg(tempStr);

				

			}
		}
	}

return 0;

}

void CUI_TeachingFrame::GetVehicleInfo(bool& lock,bool& invalid)
{

	ItemFieldStruct* item = NULL;
	//有物品嗎
	item = CNetGlobal::RoleData()->GetBodyItem(m_ItemPos);
	if (item)
	{

		//拿出資料庫物件
		GameObjDbStruct* itemDB = CNetGlobal::GetObj(item->OrgObjID);	
		if (itemDB&& 
			itemDB->Classification==EM_ObjectClass_Item &&
			(itemDB->Item.ItemType== EM_ItemType_Normal ||itemDB->Item.ItemType==EM_ItemType_SummonHorse) &&
			itemDB->Item.UseType==EM_ItemUseType_Magic)
		{	
			int imageObj=itemDB->Item.IncMagic_Onuse;

			GameObjDbStruct* MagicItem = CNetGlobal::GetObj(imageObj);	
			if (MagicItem&&MagicItem->Classification==EM_ObjectClass_MagicCollect)
			{

				GameObjDbStruct* MagicBaseDB = CNetGlobal::GetObj(MagicItem->MagicCol.Magic_List[0].MagicBaseID);
				if (MagicBaseDB)
				{


					if (MagicBaseDB->MagicBase.MagicFunc==EM_MAGICFUN_Assist)
					{

						if (MagicBaseDB->MagicBase.Effect.Ride==true)
						{
							lock=false;
							invalid=false;
						}
					}
				}
			}
		}
	}

}
 void CUI_TeachingFrame::SetPlayerBoneScale(int index,int scale)
{


	if (index>=0&&index<11)
	{
		if (m_player)
		{
			m_player->SetBoneScale(g_BoneName[index], scale * 0.01f);
		}

		m_boneScale[index]=scale;
	}

	



}

int  CUI_TeachingFrame::GetPlayerBoneScale(int index)
{


	if (index>=0&&index<11)
	return m_boneScale[index];

	return 100;

	

}
void CUI_TeachingFrame::GetVehicleItemColor(int* color)
{
	ItemFieldStruct* item = NULL;
	//有物品嗎
	item = CNetGlobal::RoleData()->GetBodyItem(m_ItemPos);
	if (item)
	{
		//拿出資料庫物件
		GameObjDbStruct* itemDB = CNetGlobal::GetObj(item->OrgObjID);	
		if (itemDB&& itemDB->Classification==EM_ObjectClass_Item &&(itemDB->Item.ItemType== EM_ItemType_Normal ||itemDB->Item.ItemType== EM_ItemType_SummonHorse )&&itemDB->Item.UseType==EM_ItemUseType_Magic)
		{	
			int imageObj=itemDB->Item.IncMagic_Onuse;

			GameObjDbStruct* MagicItem = CNetGlobal::GetObj(imageObj);	
			if (MagicItem&&MagicItem->Classification==EM_ObjectClass_MagicCollect)
			{

				GameObjDbStruct* MagicBaseDB = CNetGlobal::GetObj(MagicItem->MagicCol.Magic_List[0].MagicBaseID);
				if (MagicBaseDB)
				{
					

					if (MagicBaseDB->MagicBase.MagicFunc==EM_MAGICFUN_Assist)
					{

						if (MagicBaseDB->MagicBase.Effect.Ride==true)
						{
							color[0]=item-> HorseColor[0];		
							color[1]=item-> HorseColor[1];					
							color[2]=item-> HorseColor[2];					
							color[3]=item-> HorseColor[3];					
							
						}
					}
				}
			}
		}


	}

}

void CUI_TeachingFrame::VehicleItemColorApply(int* color)
{

	char tempp[128];

	const char *password=g_pGameMain->GetSecondPassword();
	strcpy(tempp,password);
	
	NetCli_Other::S_ColoringHorse( color, tempp, m_ItemPos );

}
void CUI_TeachingFrame::BuildFrames(void)
{
}
void CUI_TeachingFrame::DestroyFrames(void)
{
m_vehicle=NULL;
m_player=NULL;

}

int   CUI_TeachingFrame::RemoveVehicleItem(void)
{

	int index=m_ItemPos;
	if (index!=-1)
	{

	
		ItemFieldStruct* item = CNetGlobal::RoleData()->GetBodyItem(index);
		if (item)
		{
			GameObjDbStruct* itemDB = CNetGlobal::GetObj(item->OrgObjID);		
			item->Pos=EM_ItemState_NONE;

		}
	}

	if (m_vehicle)
	{

		m_vehicle->CreateEntity(NULL);

		m_vehicle=NULL;
	}

	m_ItemPos=-1;

return 0;
}

//移走坐騎物品
int			LuaFunc_BS_RemoveVehicleItem(lua_State *L)
{
	if (g_pTeachingFrame)
	{
		return g_pTeachingFrame->RemoveVehicleItem();
	}
	return 0;
}
//取得坐騎物品
int LuaFunc_BS_GetVehicleItem(lua_State *L)
{

	if (g_pTeachingFrame)
	{
		return g_pTeachingFrame->GetClipboardVehicleItem();

	}
	return 0;

}
//-----------------------------------------------------------------------
//坐騎染色
int	LuaFunc_BS_VehicleItemColorApply(lua_State *L)
{

	
	int colorold[4];
	int colornew[4];
	colornew[0]=luaL_checkint(L, 1) ;	
	colornew[1]=luaL_checkint(L, 2) ;	
	colornew[2]=luaL_checkint(L, 3) ;	
	colornew[3]=luaL_checkint(L, 4) ;	

	

	if (g_pTeachingFrame&&g_pGameMain->getMerchant())
	{
		g_pTeachingFrame->GetVehicleItemColor(colorold);


		int i;
		int  change=0;
		for (i=0;i<4;i++)
		{
			if (colornew[i]!=colorold[i])
			{
				if (colornew[i]==-1 )
					colornew[i]=colorold[i];
				//else
				

				
				change++;
				//break;
			}
		}
		if (change>0)
		 g_pTeachingFrame->VehicleItemColorApply(colornew);

	}
	return 0;
}
//-----------------------------------------------------------------------
//取得坐騎的顏色資訊
int			LuaFunc_BS_GetVehicleItemColor(lua_State *L)
{

	int color[4];

	 color[0]=RGB(142,142,142)|0xff000000;
	 color[1]=RGB(142,142,142)|0xff000000;
	 color[2]=RGB(142,142,142)|0xff000000;
	 color[3]=RGB(142,142,142)|0xff000000;
	if (g_pTeachingFrame&&g_pGameMain->getMerchant())
		g_pTeachingFrame->GetVehicleItemColor(color);
	lua_pushnumber(L,color[0]);	
	lua_pushnumber(L, color[1]);	
	lua_pushnumber(L, color[2]);	
	lua_pushnumber(L,color[3]);								



	return 4;
}


//-----------------------------------------------------------------------
//INT分解成RGB

int			LuaFunc_BSF_number2RGBA(lua_State *L)
{

	int color = luaL_checkint(L, 1) ;	


	float r,g,b,a;

	r=((color>>16)&0xff)/255.0f;;
	g=((color>>8)&0xff)/255.0f;;
	b=((color)&0xff)/255.0f;;
	a=((color>>24)&0xff)/255.0f;;
	lua_pushnumber(L,r);	
	lua_pushnumber(L, g);	
	lua_pushnumber(L, b);	
	lua_pushnumber(L, a);	
	return 4;
}
//-----------------------------------------------------------------------
//取得坐騎的資訊
int			LuaFunc_BS_GetVehicleInfo(lua_State *L)
{

	bool lock=true,invalid=true;
	if (g_pTeachingFrame&&g_pGameMain->getMerchant())
		g_pTeachingFrame->GetVehicleInfo(lock,invalid);
	lua_pushboolean(L, lock);	
	lua_pushboolean(L,invalid);	
return 2;
}
//-----------------------------------------------------------------------
//RGB 合成INT
int	LuaFunc_BSF_RGBA2number(lua_State *L)
{
	int r = luaL_checkint(L, 1) ;	
	int g = luaL_checkint(L, 2) ;	
	int b = luaL_checkint(L, 3) ;	
	int a = luaL_checkint(L, 4) ;	
	int color=(a<<24)|(r<<16)|(g<<8)|b;

	lua_pushnumber(L,color );	
	return 1;
}
//static void NetCli_Other::S_ColoringHorse( int Color[4], char* Password , int ItemPos );
//virtual void NetCli_Other::R_ColoringHorseResult( ColoringShopResultENUM Result ) = 0;
