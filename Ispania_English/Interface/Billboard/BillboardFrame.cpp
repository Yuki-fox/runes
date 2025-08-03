#include <stdio.h>
#include "../../mainproc/GameMain.h"
#include "../../netwaker_member/NetWakerClientInc.h"
#include "../WorldFrames/ChatEdit.h"
#include "../WorldFrames/ChatFrame.h"
#include "BillboardFrame.h"


int	LuaFunc_BillboardFrameOnLoad	( lua_State* L );
int	LuaFunc_BillboardFrameOnClose	( lua_State* L );
int	LuaFunc_BillboardFrameOnShow	( lua_State* L );
int	LuaFunc_BillboardFrameAnonymous	( lua_State* L );
int	LuaFunc_BillboardFrameSearch	( lua_State* L );

int	LuaFunc_BillboardIsAnonymous	( lua_State* L );

int	LuaFunc_BillboardReadBaseInfo	( lua_State* L );
int	LuaFunc_BillboardReadItemList	( lua_State* L );

int	LuaFunc_BillboardReadBrowseFilterList( lua_State* L );

CBillboardFrame*				g_pBillboardFrame = NULL;

// ----------------------------------------------------------------------------
CBillboardFrame::CBillboardFrame(CInterface* object)
	: CInterfaceBase(object)
{
	m_NpcID = -1;

	g_pBillboardFrame = this;
	_InitFiler();
}

// ----------------------------------------------------------------------------
CBillboardFrame::~CBillboardFrame()
{
	g_pBillboardFrame = NULL;
	_ReleaseFiler();
}

// ----------------------------------------------------------------------------
void CBillboardFrame::RegisterFunc()
{
	lua_State* L = GetLuaStateWorld();
	if (L)
	{
		lua_register(L, "LuaFunc_BillboardFrameOnLoad"			,LuaFunc_BillboardFrameOnLoad		);
		lua_register(L, "LuaFunc_BillboardFrameOnClose"			,LuaFunc_BillboardFrameOnClose		);
		lua_register(L, "LuaFunc_BillboardFrameOnShow"			,LuaFunc_BillboardFrameOnShow		);
		lua_register(L, "LuaFunc_BillboardFrameAnonymous"		,LuaFunc_BillboardFrameAnonymous	);
		lua_register(L, "LuaFunc_BillboardFrameSearch"			,LuaFunc_BillboardFrameSearch		);
																 

		lua_register(L, "LuaFunc_BillboardIsAnonymous"			,LuaFunc_BillboardIsAnonymous	);

		lua_register(L, "LuaFunc_BillboardReadBaseInfo"			,LuaFunc_BillboardReadBaseInfo		);
		lua_register(L, "LuaFunc_BillboardReadItemList"			,LuaFunc_BillboardReadItemList		);

		lua_register(L, "LuaFunc_BillboardReadBrowseFilterList"	,LuaFunc_BillboardReadBrowseFilterList		);
	}
}

// ----------------------------------------------------------------------------
void CBillboardFrame::OnLoad()
{
	
}

// ----------------------------------------------------------------------------
void CBillboardFrame::Open( int NpcID )
{
	m_NpcID = NpcID;
	SendWorldEvent("BILLBOARD_OPEN");
}

// ----------------------------------------------------------------------------
void CBillboardFrame::Close()
{
	m_NpcID = -1;
	SendWorldEvent("BILLBOARD_CLOSE");
}
// ----------------------------------------------------------------------------
void CBillboardFrame::UpdateData( )
{
	SendWorldEvent("BILLBOARD_UPDATE");
}
void CBillboardFrame::Update(float elapsedTime)
{
	//SendWorldEvent("BILLBOARD_UPDATE");
	static float lastTime = 0.0f;
	lastTime += elapsedTime;
	if ( lastTime < 0.2f  || m_NpcID == -1 )
		return;

	lastTime = 0.0f;
	if ( g_pGameMain->GetDistanceWithPlayer(m_NpcID) > _MAX_ROLE_TOUCH_MAXRANGE_ )
	{
		Close();
		//SendWorldEvent("BILLBOARD_CLOSE");
	}
}
// ----------------------------------------------------------------------------
void	CBillboardFrame::_ReleaseFiler( )
{

}
void	CBillboardFrame::_InitFiler( )
{

	BillboardBrowseFilterStruct TempFilter;
	
	TempFilter.ID = EM_BillBoardMainType_Money;
	TempFilter.Name = "BILLBOARD_FORTUNE";
	m_BrowseFilter.push_back( TempFilter );

	TempFilter.ID = EM_BillBoardMainType_Title;
	TempFilter.Name = "BILLBOARD_TITLE";
	m_BrowseFilter.push_back( TempFilter );

	TempFilter.ID = EM_BillBoardMainType_PK;
	TempFilter.Name = "BILLBOARD_PK";
	m_BrowseFilter.push_back( TempFilter );


	TempFilter.ID = EM_BillBoardMainType_Job_Warrior;
	TempFilter.Name = "BILLBOARD_CLASS";
	m_BrowseFilter.push_back( TempFilter );

	TempFilter.ID = EM_BillBoardMainType_Job_Warrior;
	TempFilter.Name = "BILLBOARD_WARRIOR";
	m_BrowseFilter.back().Childs.push_back( TempFilter );

	TempFilter.ID = EM_BillBoardMainType_Job_Ranger;
	TempFilter.Name = "BILLBOARD_RANGER";
	m_BrowseFilter.back().Childs.push_back( TempFilter );

	TempFilter.ID = EM_BillBoardMainType_Job_Rogue;
	TempFilter.Name = "BILLBOARD_THIEF";
	m_BrowseFilter.back().Childs.push_back( TempFilter );

	TempFilter.ID = EM_BillBoardMainType_Job_Wizard;
	TempFilter.Name = "BILLBOARD_MAGE";
	m_BrowseFilter.back().Childs.push_back( TempFilter );

	TempFilter.ID = EM_BillBoardMainType_Job_Priest;
	TempFilter.Name = "BILLBOARD_AUGUR";
	m_BrowseFilter.back().Childs.push_back( TempFilter );

	TempFilter.ID = EM_BillBoardMainType_Job_Knight;
	TempFilter.Name = "BILLBOARD_KNIGHT";
	m_BrowseFilter.back().Childs.push_back( TempFilter );

/*	TempFilter.ID = EM_BillBoardMainType_Job_RuneDancer;
	TempFilter.Name = "BILLBOARD_WARDEN";
	m_BrowseFilter.back().Childs.push_back( TempFilter );

	TempFilter.ID = EM_BillBoardMainType_Job_Druid;
	TempFilter.Name = "BILLBOARD_DRUID";
	m_BrowseFilter.back().Childs.push_back( TempFilter );
	*/

}
// ----------------------------------------------------------------------------

int	LuaFunc_BillboardFrameOnLoad	( lua_State* L )
{
	return 0;
}
int	LuaFunc_BillboardFrameOnClose	( lua_State* L )
{
	NetCli_BillboardChild::S_CloseBillboardRequest( );
	return 0;
}
int	LuaFunc_BillboardFrameOnShow	( lua_State* L )
{
	NetCli_BillboardChild::S_RankInfoRequest( EM_BillBoardMainType_Money , 1 );
	return 0;
}
int	LuaFunc_BillboardFrameAnonymous	( lua_State* L )
{
	bool IsAnonymous = lua_toboolean( L , 1 ) != 0;
	if( CNetGlobal::RoleData()->PlayerBaseData->IsBillboardAnonymous != IsAnonymous )
	{
		NetCli_BillboardChild::S_SetAnonymous( IsAnonymous );
		CNetGlobal::RoleData()->PlayerBaseData->IsBillboardAnonymous = IsAnonymous;
	}
	return 0;
}
int	LuaFunc_BillboardFrameSearch	( lua_State* L )
{
	int Type	= (int)lua_tonumber( L , 1 );
	int Index	= (int)lua_tonumber( L , 2 ) * 20 - 19;

	if( Index <= 0 )
		Index = -1;

	NetCli_BillboardChild::S_RankInfoRequest( (BillBoardMainTypeENUM)Type , Index );
	return 0;
}

int	LuaFunc_BillboardIsAnonymous	( lua_State* L )
{
	lua_pushboolean(L, CNetGlobal::RoleData()->PlayerBaseData->IsBillboardAnonymous );
	return 1;
}

int	LuaFunc_BillboardReadBaseInfo	( lua_State* L )
{

	lua_pushboolean( L , NetCli_BillboardChild::m_Result );
	lua_pushnumber ( L , NetCli_BillboardChild::m_SearchType );
	lua_pushnumber ( L , NetCli_BillboardChild::m_RankBegin );
	lua_pushnumber ( L , NetCli_BillboardChild::m_RankEnd );
	int MaxPage = ( NetCli_BillboardChild::m_RankMax - 1 ) / 20 + 1;
	lua_pushnumber ( L , MaxPage );

	return 5;
}
int	LuaFunc_BillboardReadItemList	( lua_State* L )
{
	int ID = (int)lua_tonumber( L , 1 );

	if( (unsigned)ID >= NetCli_BillboardChild::m_BillboardInfo.size() )
	{
		return 0;
	}

	BillBoardRoleInfoStruct& Info = NetCli_BillboardChild::m_BillboardInfo[ ID ];

	if( strcmp( Info.RoleName.Begin() , CNetGlobal::RoleData()->RoleName()  ) == 0 )
		lua_pushboolean( L ,true );
	else
		lua_pushboolean( L , false );
//	lua_pushboolean( L , Info.DBID == CNetGlobal::RoleData()->DBID() );
	lua_pushstring( L , Info.RoleName.Begin()	);
	lua_pushnumber( L , Info.Rank				);
	lua_pushnumber( L , Info.MainRankInfo		);

	lua_pushstring(L, g_ObjectData->GetClassNameByID(Info.Voc));	
	if( Info.Voc_Sub > 0 && Info.Lv_Sub > 0 )
	{		
		lua_pushstring(L, g_ObjectData->GetClassNameByID(Info.Voc_Sub));
	}
	else
	{
		lua_pushstring(L, "" );
	}
	lua_pushnumber( L , Info.Lv			);
	lua_pushnumber( L , Info.Lv_Sub		);
	lua_pushnumber( L , Info.Money		);
	lua_pushnumber( L , Info.TitleCount	);
	lua_pushnumber( L , Info.PKScore	);

	return 11;
}

int	LuaFunc_BillboardReadBrowseFilterList( lua_State* L )
{	
	int Top = lua_gettop( L );
	if( Top <= 0 )
		return 0;
	int ID = (int)lua_tonumber( L , 1 );

	if( (int)g_pBillboardFrame->m_BrowseFilter.size() <= ID )
		return 0;

	BillboardBrowseFilterStruct*  BrowseFilter = &(g_pBillboardFrame->m_BrowseFilter[ID]); 

	for( int i = 2 ; i <= Top ; i++ )
	{
		ID = (int)lua_tonumber( L , i );
		
		if( (int)BrowseFilter->Childs.size() <= ID )
			return 0;

		BrowseFilter = &( BrowseFilter->Childs[ID ] );	
	}
	lua_pushnumber( L , (lua_Number)BrowseFilter->Childs.size() );
	lua_pushnumber( L , BrowseFilter->ID );
	lua_pushstring( L , BrowseFilter->Name.c_str() );

	return 3;
}