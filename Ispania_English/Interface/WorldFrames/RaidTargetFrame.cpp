
#include "../../mainproc/GameMain.h"
//#include "../../netwaker_member/NetWakerClientInc.h"
//#include "../WorldFrames/ItemClipboard.h"
//#include "../WorldFrames/ChatEdit.h"
//#include "../WorldFrames/ChatFrame.h"



#include "RaidTargetFrame.h"

#include "../../netwaker_member/NetWakerClientInc.h"


CRaidTargetFrame* g_pRaidTargetFrame = NULL;


int LuaFunc_InitializeRaidTargetFrame( lua_State* L ); 


int LuaFunc_GetRaidTargetIndex	(lua_State* L);
int LuaFunc_SetRaidTarget		(lua_State* L);


// ----------------------------------------------------------------------------
CRaidTargetFrame::CRaidTargetFrame(CInterface* object)
	: CInterfaceBase(object)
{
	g_pRaidTargetFrame = this;	

	m_RaidTargeinfoCount = 0;

	m_RaidTargeinfoList = NULL;

}

// ----------------------------------------------------------------------------
CRaidTargetFrame::~CRaidTargetFrame()
{
	g_pRaidTargetFrame = NULL;

	if( m_RaidTargeinfoList )
	{
		delete [] m_RaidTargeinfoList;	
	}

}

// ----------------------------------------------------------------------------
void CRaidTargetFrame::RegisterFunc()
{
	lua_State* L = GetLuaStateWorld();
	if (L)
	{
		lua_register(L, "InitializeRaidTargetFrame"  , LuaFunc_InitializeRaidTargetFrame );
		lua_register(L, "GetRaidTargetIndex"         , LuaFunc_GetRaidTargetIndex        );
		lua_register(L, "SetRaidTarget"              , LuaFunc_SetRaidTarget             );
	}
}

// ----------------------------------------------------------------------------
void CRaidTargetFrame::DestroyFrames()
{
	m_RaidTargeinfoCount = 0;	
	UI_SAFE_DELETE_ARRAY(m_RaidTargeinfoList);
}

bool IsCanSeeYou(CRoleSprite* sprite ,float& z)
{
	bool visible = !sprite->GetSneakHiddenState();
	CRuVector3 position = sprite->GetPos();
	CRuCamera* cam=g_pGameMain->GetCamera();
	cam->GetCameraTransform().TransformPoint(position);
	if (position.m_z > 0 && visible)
	{
		z=position.m_z ;
		return true;
	}
	return false;
}

// ----------------------------------------------------------------------------
void CRaidTargetFrame::Render()
{
	if ( !g_pGameMain->IsEnterWorld() )
	{
		return;
	}
 
	//CRuCamera* pCamera = g_pGameMain->GetCamera();
	//CRuMatrix4x4  Billboard;
	//CRuVector3 geometryForward( 0.0f , 0.0f , 1.0f ); 
	//CRuVector3 geometryUp( 0.0f , 1.0f , 0.0f );
	//CRuVector3 vecPos1;
	//CRuVector3 vecPos2;
	//CUiAnchor anchor;

	CRoleSprite* pSprite;
  
	float fz;
	//float scale;
	for( int i = 0; i < m_RaidTargeinfoCount; ++i )
	{
		if( m_RaidTargeinfoList[i].bLock )
		{
			m_RaidTargeinfoList[i].bLock = false;
			continue;
		}

		if( !m_RaidTargeinfoList[i].pUiFrame )
		{
			continue;
		}

		m_RaidTargeinfoList[i].pUiFrame->SetVisible( false );

		if( m_RaidTargeinfoList[i].TargetID < 0 )
			continue;

		pSprite = g_pGameMain->FindSpriteByID( m_RaidTargeinfoList[i].TargetID );

		if( NULL == pSprite )
			continue;

		if( false == IsCanSeeYou( pSprite , fz ) )
		{
			continue;
		}

		pSprite->SetRaidMarkTarget( i );
  

		/*
		pSprite->GetAttachPos( eAttach_Name_Title, vecPos1 );

		pCamera->Utility_GetBillboardTransform( geometryForward, geometryUp, Billboard );

		vecPos2 = CRuVector3( 0.0f, 1.0f, 0.0f ) * Billboard + vecPos1;
		g_pGameMain->WorldToViewport(vecPos1);
		g_pGameMain->WorldToViewport(vecPos2);
		scale = fabs( vecPos2.y - vecPos1.y );

		if ( scale < 0.25f )
		{
			continue;
		}

		static float s_fH = 3;

		float fUIScale = g_pInterface->GetScale();

		anchor.m_relativeTo = m_interface->GetUiWorld()->GetRoot() ;
		anchor.m_offset.m_x = ( vecPos1.m_x ) / fUIScale ;
		anchor.m_offset.m_y = ( vecPos1.m_y - ( s_fH * scale ) ) / fUIScale ;
		anchor.m_point = CUiAnchor::eAnchor_Bottom;
		anchor.m_relativePoint = CUiAnchor::eAnchor_TopLeft;

		m_RaidTargeinfoList[i].pUiFrame->ClearAllAnchors();
		m_RaidTargeinfoList[i].pUiFrame->SetAnchor( anchor );
		m_RaidTargeinfoList[i].pUiFrame->SetVisible( true );

		m_RaidTargeinfoList[i].pUiFrame->SetScale( 1.0f );

		CUiSize size;
		size.m_x = 4 * scale ;
		size.m_y = 4 * scale ;
		m_RaidTargeinfoList[i].pUiFrame->SetSize( size );
		*/
	}
}

// ----------------------------------------------------------------------------
void CRaidTargetFrame::Initialize( const char* pFrameName, int count )
{
	if( m_RaidTargeinfoList )
	{
		delete [] m_RaidTargeinfoList;	
	}
	m_RaidTargeinfoCount = 0;

	if( !( pFrameName ) )
		return ;

 	m_RaidTargeinfoCount = count;
	m_RaidTargeinfoList = new SRaidTargeinfo[m_RaidTargeinfoCount];
   
	SRaidTargeinfo* pInfo;

	char szFrameName[MAX_PATH];
	for( int i = 0; i < m_RaidTargeinfoCount; ++i )
	{
		pInfo = &m_RaidTargeinfoList[i];
		sprintf( szFrameName, "%s%d", pFrameName , ( i + 1 ) );
		pInfo->pUiFrame = dynamic_cast<CUiFrame*>(m_interface->GetUiWorld()->FindObject( szFrameName ) );
		pInfo->TargetID = -1;
		pInfo->bLock = false;
	}

}

void CRaidTargetFrame::ClearAllRaidTarget()
{
	for( int i = 0; i < m_RaidTargeinfoCount; ++i )
	{
		if( m_RaidTargeinfoList[i].TargetID > 0 )
		{
			CRoleSprite* pSprite = g_pGameMain->FindSpriteByID( m_RaidTargeinfoList[i].TargetID );
			if( pSprite )
				pSprite->SetRaidMarkTarget( -1 );
		}

		m_RaidTargeinfoList[i].TargetID = -1;
	}
	SendWorldEvent( UiEvent_RaidTargetChanged );
}


// ----------------------------------------------------------------------------
int CRaidTargetFrame::GetRaidTargetIndex( const char* uint )
{
	CRoleSprite* sprite = g_pGameMain->GetUnitSprite( uint );

	if( !sprite )
	{
		return -1;	
	}

	int id = sprite->GetIdentify();

	for( int i = 0 ; i < m_RaidTargeinfoCount ; ++i )
	{
		if( m_RaidTargeinfoList[i].TargetID == id )
		{
			return i;
		}
	}
 
	return -1;
}

// ----------------------------------------------------------------------------
void CRaidTargetFrame::SetRaidTarget( const char* uint, int icon )
{
	PartyStateInfo ePSInfo = NetCli_PartyChild::GetPartyState();
	PartyMemberInfo* member = NetCli_PartyChild::GetGroupMemberByDBID( g_pGameMain->GetDBID() );

	if( ePSInfo != ePartyState_Leader && !( member && member->assistant ) )
	{
		return;
	}
	
	CRoleSprite* sprite = g_pGameMain->GetUnitSprite( uint );

	int targetID = -1;
	if( sprite )
	{
		targetID = sprite->GetIdentify();
		if( targetID <= 0 )
			targetID = -1;
	}

	//發出訊息給隊友
	if( ePSInfo == ePartyState_Leader ||
		ePSInfo == ePartyState_Member )
	{
		PartyBaseInfoStruct& rInfo = NetCli_PartyChild::GetPartyInfo();
		NetCli_Channel::S_CtoC_SetRaidTarget( 
			EM_GroupObjectType_Party, rInfo.PartyID, targetID, icon );
	}
	else
	{
		R_SetRaidTarget( targetID , icon );
	}
}

void CRaidTargetFrame::R_SetRaidTarget( int targetID , int iconID )
{
	for( int i = 0; i < m_RaidTargeinfoCount; ++i )
	{
		if( m_RaidTargeinfoList[i].TargetID > 0 )
		{
			CRoleSprite* pSprite = g_pGameMain->FindSpriteByID( m_RaidTargeinfoList[i].TargetID );
			if( pSprite )
				pSprite->SetRaidMarkTarget( -1 );
		}
		
		if( m_RaidTargeinfoList[i].TargetID == targetID )
		{
			m_RaidTargeinfoList[i].TargetID = -1;
		}
	}

	if( iconID >= 0 && iconID < m_RaidTargeinfoCount )
	{
		m_RaidTargeinfoList[iconID].TargetID = targetID;
	}

	SendWorldEvent( UiEvent_RaidTargetChanged );

}

int CRaidTargetFrame::GetIconID( int targetID )
{
	for( int i = 0 ; i < m_RaidTargeinfoCount ; ++i )
	{
		if( m_RaidTargeinfoList[i].TargetID == targetID )
		{
			return i;
		}
	}
	return -1;
}

CUiFrame* CRaidTargetFrame::GetIconFrame( int targetID )
{
	int iconID = GetIconID( targetID );

	if( iconID >= 0 && iconID < m_RaidTargeinfoCount )
	{
		m_RaidTargeinfoList[iconID].bLock = true;
		return m_RaidTargeinfoList[iconID].pUiFrame;
	}
	return NULL;
}


// ----------------------------------------------------------------------------
int LuaFunc_InitializeRaidTargetFrame( lua_State* L )
{
	const char* pFrameName = luaL_checkstring( L, 1 );
	int         count      = (int)luaL_checknumber( L, 2 );

	g_pRaidTargetFrame->Initialize( pFrameName, count );

	return 0;
}


// ----------------------------------------------------------------------------
int LuaFunc_GetRaidTargetIndex	(lua_State* L)
{
	const char* uint = luaL_checkstring( L, 1 );

	int icon = g_pRaidTargetFrame->GetRaidTargetIndex( uint );
	if( icon >= 0 )
	{
		icon = icon + 1;
		lua_pushnumber( L, icon );
		return 1;
	}
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_SetRaidTarget		(lua_State* L)
{
	const char* uint = luaL_checkstring( L, 1 );

	int icon = -1;
	if( lua_isnil(L,2) )
	{
		icon = -1;
	}
	else
	{
		icon = luaL_checkint(L,2) - 1; 

		if( icon < 0 || icon > 7 )
		{
			icon = -1;
		}
	}

	g_pRaidTargetFrame->SetRaidTarget( uint, icon );

	return 0;
}



























