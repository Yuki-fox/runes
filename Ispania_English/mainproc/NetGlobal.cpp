#include "NetGlobal.h"
#include "../NetWaker_Member/NetWakerClientInc.h"
#include "../Interface/interface.h"
#include "../Interface/WorldFrames/BagFrame.h"
#include "../Interface/WorldFrames/TradeFrame.h"
#include "../Interface/SkillBook/UI_SkillBook.h"

bool						CNetGlobal::_IsInitial				= false;
CliNetWaker					CNetGlobal::_Net;
CliNetWaker					CNetGlobal::_NetBattle;

ServerID					CNetGlobal::_ServerNetID;
//ServerListCliClass			CNetGlobal::_NetSrvList;
SrvListEvent				CNetGlobal::_SrvListListener;

PlayerRoleData				CNetGlobal::_RoleData;                     //角色資料
bool						CNetGlobal::_RoleDataLoaded = false;
RoleValueData				CNetGlobal::_RoleBaseAttr;                 //角色資料

// 斷線資訊
set<int>					CNetGlobal::s_localServerList;
float						CNetGlobal::s_waitConnectTime = 0.0f;
int							CNetGlobal::s_lastNetProcTime = 0;
bool						CNetGlobal::s_boFinishUI = false;			// 界面是否載入結束
vector<CPGTempInfo>			CNetGlobal::s_PGEvent;						// 封包

bool						CNetGlobal::_InBattleGround = false;

void	SrvListEvent::OnRevServerList( int LastLoginWorldID , const char* Account , const char* Country , int IPNum , int Age , int ValidTime , int VivoxLicenseTime ,  vector<ServerListInfoStruct>& SrvList )
{
	TRACE0( "OnRevServerList" );
}
void	SrvListEvent::OnConnectFailed( )
{
	TRACE0( "OnConnectFailed" );
}
void	SrvListEvent::OnConnect( )
{
	TRACE0( "OnConnect" );
}
void	SrvListEvent::OnDisConnect( )
{
	TRACE0( "OnDisConnect" );
}
void	SrvListEvent::OnError( ServerListFailed_ENUM ErrCode )
{
	TRACE0( "OnError" );
}

void	SrvListEvent::OnErrorData( ServerListFailed_ENUM ErrCode, void* Data, unsigned long Size)
{
	TRACE0( "OnErrorData" );
}

void	SrvListEvent::OnGameRoleCount( char RoleCount[30] , int FileSize , char* FileData )
{
	//各Server人數
	TRACE0( "OnGameRoleCount" );
}


void CNetGlobal::Initial(const char* db_filename, const char* datalanguage, const char* stringlanguage )
{    
	_IsInitial = true;

	ObjectDataClass::Init(db_filename, datalanguage, stringlanguage , false );

	_Net.Init();
	_NetBattle.Init();

	Net_GSrvConnectChild::_Init();
	CNetCli_Login_Child::_Init();
	NetCli_AttackChild::_Init();	
	NetCli_EQChild::_Init();
	NetCli_ItemChild::_Init();
	NetCli_MagicChild::_Init();
	NetCli_MoveChild::_Init();
	NetCli_RoleValueChild::_Init();
	NetCli_ShopChild::_Init();
	NetCli_TalkChild::_Init();
	NetCli_TradeChild::_Init();
	CNetCli_Script_Child::Init();
	NetCli_PartyChild::_Init();
	NetCli_ChannelChild::_Init();
	NetCli_FriendListChild::_Init();
	NetCli_MailChild::_Init();
	NetCli_ACChild::_Init();
	NetCli_RecipeChild::_Init();
	NetCli_GuildChild::_Init();
	CNetCli_Gather_Child::Init();
	NetCli_BillboardChild::_Init();
	NetCli_PKChild::_Init();
	NetCli_OtherChild::_Init();
	NetCli_LotteryChild::_Init();
	NetCli_HousesChild::_Init();
	NetCli_DepartmentStoreChild::_Init();
	NetCli_ImportBoardChild::_Init();
	CNetCli_BattleGround_Child::Init();
	NetCli_TeleportChild::_Init();
	NetCli_TreasureChild::_Init();
	NetCli_GuildHousesChild::_Init();
	NetCli_CultivatePetChild::_Init();
//	NetCli_GuildHousesWarChild::_Init();
	NetCli_BG_GuildWarChild::_Init();
	CNetCli_FestivalChild::_Init();
	NetCli_InstanceChild::_Init();
	CNetCli_PublicEncounterChild::_Init();

	RoleDataEx::St_GetObjDB			= GetObj;
	RoleDataEx::St_UpDateEvent		= St_UpDateEvent;
	RoleDataEx::St_GetKeyMagicID	= GetKeyMagicID;
	RoleDataEx::St_RuneExchange		= ObjectDataClass::RuneExchange;
	RoleDataEx::St_NextRuneLv		= ObjectDataClass::NextRuneLv;
	RoleDataEx::St_SysKeyValue		= ObjectDataClass::St_SysValue;
}

void CNetGlobal::Release()
{
	if ( _IsInitial )
	{
		ObjectDataClass::Release();

		Net_GSrvConnectChild::_Release();
		CNetCli_Login_Child::_Release();
		NetCli_EQChild::_Release();
		NetCli_ItemChild::_Release();
		NetCli_MagicChild::_Release();
		NetCli_MoveChild::_Release();
		NetCli_RoleValueChild::_Release();
		NetCli_ShopChild::_Release();
		NetCli_TalkChild::_Release();
		NetCli_TradeChild::_Release();
		CNetCli_Script_Child::Release();
		NetCli_PartyChild::_Release();
		NetCli_ChannelChild::_Release();
		NetCli_FriendListChild::_Release();
		NetCli_MailChild::_Release();
		NetCli_ACChild::_Release();
		NetCli_RecipeChild::_Release();
		NetCli_GuildChild::_Release();
		NetCli_BillboardChild::_Release();
		NetCli_PKChild::_Release();
		NetCli_OtherChild::_Release();
		NetCli_LotteryChild::_Release();
		NetCli_HousesChild::_Release();
		NetCli_DepartmentStoreChild::_Release();
		NetCli_ImportBoardChild::_Release();
		CNetCli_BattleGround_Child::Release();
		NetCli_TeleportChild::_Release();
		NetCli_TreasureChild::_Release();
		NetCli_GuildHousesChild::_Release();
		NetCli_CultivatePetChild::_Release();
//		NetCli_GuildHousesWarChild::_Release();
		NetCli_BG_GuildWarChild::_Release();
		NetCli_InstanceChild::_Release();
//		delete _Schedular;
		_IsInitial = false;
	}
}

void CNetGlobal::Process( double frameTime, FLOAT elapsedTime )
{
	static double localTime = 0.0f;
	static double localSecond = 0.0f;
	if ( localTime == 0.0f )
		localTime = frameTime;
	if ( localSecond == 0.0f )
		localSecond = frameTime;

	CNetBridge::WaitAll( 0 );

	// 每0.1秒更新
	while (frameTime - localTime > 0.1f)
	{
		_Net.Process();

		_Net._Net->_NetBSwitch->SetOutPutCB( CliNetWakerClass::OutputNetEngineSwitch );
		_Net._Net->_NetBProxy->SetOutPutCB( CliNetWakerClass::OutputNetEngineProxy );

		_NetBattle._Net->_NetBSwitch->SetOutPutCB( CliNetWakerClass::OutputNetEngineBGProxy );
		_NetBattle._Net->_NetBProxy->SetOutPutCB( CliNetWakerClass::OutputNetEngineBGProxy );


		s_lastNetProcTime = timeGetTime();


		_NetBattle.Process();

		((RoleDataEx*)&_RoleData)->OnTimeProc_HundredMS();
		CNetCli_Login_Child::ServerListProcess();
		NetCli_MoveChild::OnTimeProcess_HundredMS();
		localTime += 0.1f;
	}	

	// 每秒更新
	while (frameTime - localSecond > 1.0)
	{
		((RoleDataEx*)&_RoleData)->OnTimeProc();
		((RoleDataEx*)&_RoleData)->OnTimeProcAll();
		localSecond += 1.0;
	}	
	CNetCli_Gather_Child::Update(elapsedTime);

	g_ObjectData->OnTimeProc( );
}
//--------------------------------------------------------------------------------------
FunctionSchedularClass* CNetGlobal::Schedular( )
{
	return _Net.Schedular();
   //return _Schedular;
}
//--------------------------------------------------------------------------------------
void CNetGlobal::NetRecvEvent(bool boEvent)
{
	_Net._Net->_NetBProxy->SetRecvEvent(boEvent);
	_NetBattle._Net->_NetBProxy->SetRecvEvent(boEvent);
}

//--------------------------------------------------------------------------------------
void CNetGlobal::ClearTempEvent()
{
	s_PGEvent.clear();
}

//--------------------------------------------------------------------------------------
void CNetGlobal::PushTempEvent(int Sockid , int Size , void* Data, PGCallback func)
{
	assert(!s_boFinishUI);

	if ( Data && func )
	{
		s_PGEvent.push_back(CPGTempInfo());
		s_PGEvent[s_PGEvent.size() - 1].SetInfo(Sockid, Size, (char*)Data, func);
	}
}

//--------------------------------------------------------------------------------------
void CNetGlobal::SetUIFinish(bool boEnd)
{
	if ( s_boFinishUI != boEnd )
	{
		s_boFinishUI = boEnd;
		if ( s_boFinishUI )
		{
			for ( vector<CPGTempInfo>::iterator iter = s_PGEvent.begin(); iter != s_PGEvent.end(); iter++ )  
			{
				iter->Call();
			}

			s_PGEvent.clear();
		}
	}
}

//--------------------------------------------------------------------------------------
void CNetGlobal::St_UpDateEvent(RoleDataEx *Role , RoleUpdateEvent Event)
{
	if( Event.Recalculate_Buff || Event.LvChange || Event.Recalculate_All )
	{
		((RoleDataEx*)&_RoleData)->ReCalculateEQNoBuff();		
		_RoleBaseAttr = _RoleData.TempData.Attr;
		_RoleBaseAttr.Calculate();
		//((RoleDataEx*)&_RoleData)->ReCalculateMagicAndEQ();		
		((RoleDataEx*)&_RoleData)->ReCalculateBuff();
		_RoleData.TempData.Attr.Calculate();
	}
	
	/*
	if ( Event.Money )
	{
		ui->SendEvent( UiEvent_Role_Money_Change );
	}
	*/

	if( Event.Bank)
	{
		//g_pInterface->GetUi()->SendEvent()
		//CFrame::SendEventString( _Def_UIEvent_OnBankUpDate_, 0, 0 );
	}
	if( Event.Body )
	{
		g_pInterface->SendWorldEvent("PLAYER_BAG_CHANGED");
		g_pInterface->SendWorldEvent("RESET_QUESTTRACK");
		g_pInterface->SendWorldEvent("RESET_QUESTBOOK");
		if ( g_pBagFrame )
			g_pBagFrame->RefreshCoin();

		if ( g_pTradeFrame )
			g_pTradeFrame->Refresh();
		/*
		CFrame::SendEventString( _Def_UIEvent_OnItemUpDate_, 0, 0 );
		if( CUI_Trade::IsVisible()  ) 
		{
			CUI_Trade::UpDateMyTardeItemInfo( );
		}
		*/
	}

	if( Event.Eq )
	{
		//CFrame::SendEventString( _Def_UIEvent_OnEQUpDate_, 0, 0 );
	}

	if( Event.Trade || Event.Body )
	{
		/*
		if( CUI_Trade::IsVisible() )
		{
			CFrame::SendEventString( _Def_UIEvent_OnTradeUpDate_, 0, 0 );
		} 
		*/
	}
	if (Event.VocChange)
	{
		Event.Recalculate_All = true;
		Event.SkillValueChange = true;
		Event.ExpChange = true;
	}

	if( Event.LvChange )
	{
		//ui->SendEvent( UiEvent_RoleLVChange );
		Event.ExpChange = true;
		Event.Recalculate_All = true;
		Event.SkillValueChange = true;
	}

	if( Event.HPMPSP )
	{
		//ui->SendEvent( UiEvent_RoleStatusChange );
	}
	if( Event.ExpChange )
	{
		g_pInterface->SendWorldEvent("PLAYER_EXP_CHANGED");
	}	
	if( Event.ReSetTalbe )
	{
		g_pInterface->SendWorldEvent("PLAYER_ABILITY_CHANGED");
  	//	g_pInterface->SendWorldEvent(UiEvent_RoleTableChange);
		CUI_SkillBook::SkillUpdate();
	}	
	if( Event.Recalculate_All || Event.Recalculate_Buff )
	{
		g_pInterface->SendWorldEvent("PLAYER_ABILITY_CHANGED");

		// 通知裝備做更新
		lua_State* L = g_pInterface->GetLuaState();
		if ( L )
		{
			for ( int i = EM_EQWearPos_Head; i < EM_EQWearPos_MaxCount; i++ )
			{
				lua_pushnumber(L, i);
				lua_setglobal(L, "arg1");
				g_pInterface->SendWorldEvent(UiEvent_RoleEquipChange);
			}
		}
	}
	if (Event.SkillValueChange)
	{
		g_pInterface->SendWorldEvent("PLAYER_SKILLED_CHANGED");
	}

	// 技能書更新
	if( Event.LvChange || Event.SkillKeyItemChange || Event.VocChange )
	{
		//g_pInterface->SendWorldEvent(UiEvent_SkillUpdate);
		CUI_SkillBook::SkillUpdate();
	}
}
//----------------------------------------------------------------------------	----------
GameObjDbStructEx* CNetGlobal::GetObj(int ObjID)
{
	if ( g_ObjectData == NULL )
		return NULL;
	return g_ObjectData->GetObj(ObjID);
}
//----------------------------------------------------------------------------	----------
bool   CNetGlobal::SendToLocal( int Size , void* Data )
{
	if( _ServerNetID.m_iLocal_Battle != -1 )
	{
		_NetBattle.SendToSrv( _ServerNetID.m_iLocal_Battle , Size , Data );
	}
    else if( _ServerNetID.m_iLocal_Base != -1 )
	{
		_Net.SendToSrv( _ServerNetID.m_iLocal_Base , Size , Data );
	}
	else
		return false;

    return true;
}
bool CNetGlobal::SendToLocalPublic( int Size , void* Data )
{
	return SendToLocal(Size, Data);
}
bool   CNetGlobal::SendToLocal_Battle	( int Size , void* Data		)
{
	if( _ServerNetID.m_iLocal_Battle != -1 )
	{
		_NetBattle.SendToSrv( _ServerNetID.m_iLocal_Battle , Size , Data );
	}
	else
		return false;

	return true;
}
bool   CNetGlobal::SendToLocal_Base		( int Size , void* Data		)
{

	if( _ServerNetID.m_iLocal_Base != -1 )
	{
		_Net.SendToSrv( _ServerNetID.m_iLocal_Base , Size , Data );
	}
	else
		return false;

	return true;
}
//--------------------------------------------------------------------------------------
bool   CNetGlobal::SendToMaster( int iSize , void* pData )
{
    if( _ServerNetID.m_iMaster == -1 ) 
        return false;

    return _Net.SendToSrv( _ServerNetID.m_iMaster , iSize, pData );
}
//--------------------------------------------------------------------------------------
bool   CNetGlobal::SendToChat( int iSize , void* pData )
{
	if( _ServerNetID.m_iChat == -1 ) 
		return false;

	return _Net.SendToSrv( _ServerNetID.m_iChat , iSize, pData );
}
//--------------------------------------------------------------------------------------
bool CNetGlobal::SendToServer	( int iSrvID, int Size , void* Data )
{
	return _Net.SendToSrv( iSrvID , Size , Data );
}
//--------------------------------------------------------------------------------------
void CNetGlobal::SetSrvID_Local_Base	( int iServerID )
{	
	// 連線成
	s_waitConnectTime = 0.0f;
	s_localServerList.insert(iServerID);
	_ServerNetID.m_iLocal_Base = iServerID;
}
void CNetGlobal::SetSrvID_Local_Battle	( int iServerID )
{
	_ServerNetID.m_iLocal_Battle = iServerID;
}
//--------------------------------------------------------------------------------------
bool     CNetGlobal::NetConnect( char* IP , int Port , char* Account , char* Password, const char* pszMacAddress, bool HashedPassword )
{
    _Net.Connect( IP , Port , Account , Password, pszMacAddress, HashedPassword );

    return true;
}
/*
bool     CNetGlobal::NetConnectSrvList( char* IP , int Port , char* Account , char* Password )
{
	_NetSrvList.Connect( IP , Port , Account , Password , &_SrvListListener );

	return true;	
}
*/
/*
void     CNetGlobal::Print( int iLV, char* format,... )
{
	static char buf[1024];
	va_list args;
	va_start( args, format );
	vsprintf( buf, format, args );
	va_end( args);
	OutputDebugString( buf);
	printf("%s", buf);
}
*/
//--------------------------------------------------------------------------------------
bool     CNetGlobal::Logout( ClientNetCloseTypeENUM Type )
{
    _Net.Logout( Type );
	_NetBattle.Logout( Type );
    return true;
}
//--------------------------------------------------------------------------------------
void CNetGlobal::InitializeBaseData()
{
	_RoleDataLoaded = true;
	if ( _RoleData.BaseData.GuildID != 0 )
	{
		NetCli_Guild::SL_GuildInfoRequet(_RoleData.BaseData.GuildID);
		NetCli_Guild::SL_GuildMemberListRequet(_RoleData.BaseData.GuildID);
		NetCli_Treasure::ClearBooty();
	}
}
//--------------------------------------------------------------------------------------
void CNetGlobal::ReleaseBaseData()
{
	_RoleDataLoaded = false;
}

int	CNetGlobal::GetRespondTime()
{
	return _Net.GetRespondTime();
}