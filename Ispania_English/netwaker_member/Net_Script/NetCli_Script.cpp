#include "NetCli_Script.h"

// 全域宣告
//-------------------------------------------------------------------
CNetCli_Script*		CNetCli_Script::m_pThis				= NULL;
bool				CNetCli_Script::m_bSendingOption	= false;
//-------------------------------------------------------------------
CNetCli_Script::CNetCli_Script(void)
{

}
// --------------------------------------------------------------------------------------
CNetCli_Script::~CNetCli_Script(void)
{

}
// --------------------------------------------------------------------------------------
bool CNetCli_Script::Init()
{
	Cli_NetReg( PG_Script_LtoC_QuestAccept_Result		);
	Cli_NetReg( PG_Script_LtoC_QuestComplete_Result		);
	Cli_NetReg( PG_Script_LtoC_QuestDelete_Result		);
	Cli_NetReg( PG_Script_LtoC_QuestFlag				);
	Cli_NetReg( PG_Script_LtoC_ClickQuestObjResult		);
	Cli_NetReg( PG_Script_LtoC_KillQuestObjNotify		);
	Cli_NetReg( PG_Script_LtoC_PlayMotion				);
	Cli_NetReg( PG_Script_LtoC_SpeakOption				);
	Cli_NetReg( PG_Script_LtoC_ChoiceOptionResult		);

	Cli_NetReg( PG_Script_LtoC_PlaySound				);
	Cli_NetReg( PG_Script_LtoC_Play3DSound				);
	Cli_NetReg( PG_Script_LtoC_PlayMusic				);

	Cli_NetReg( PG_Script_LtoC_SetQuestState			);
	Cli_NetReg( PG_Script_LtoC_CloseSpeak				);

	Cli_NetReg( PG_Script_LtoC_ClearBorder				);
	Cli_NetReg( PG_Script_LtoC_AddBorderPage			);
	Cli_NetReg( PG_Script_LtoC_ShowBorder				);

	Cli_NetReg( PG_Script_LtoC_ScriptCast				);
	Cli_NetReg( PG_Script_LtoC_ScriptCastResult			);
	Cli_NetReg( PG_Script_LtoC_ResetDailyQuest			);

	Cli_NetReg( PG_Script_LtoC_AttachCastMotionToolID	);
	Cli_NetReg( PG_Script_LtoC_StartClientCountDown		);
	Cli_NetReg( PG_Script_LtoC_AttachTool				);

	Cli_NetReg( PG_Script_LtoC_SetCameraFollowUnit		);
	Cli_NetReg( PG_Script_LtoC_StopSound		);
		
	return true;
}
// --------------------------------------------------------------------------------------
bool CNetCli_Script::Release()
{

	return true;
}
//--------------------------------------------------------------------------------------
void CNetCli_Script::Update				()
{
	/*
	if( m_pThis )
		m_pThis->OnUpdate();
	*/
}
//--------------------------------------------------------------------------------------
void CNetCli_Script::_PG_Script_LtoC_StopSound	( int iCliID , int iSize , PVOID pData )
{
	M_PACKET( PG_Script_LtoC_StopSound );	
	m_pThis->OnStopSound( pPacket->iObjID, pPacket->iSoundID );
}
//--------------------------------------------------------------------------------------
void CNetCli_Script::_PG_Script_LtoC_SetCameraFollowUnit		( int iCliID , int iSize , PVOID pData )
{
	M_PACKET( PG_Script_LtoC_SetCameraFollowUnit );	
	m_pThis->OnSetCameraFollowUnit( pPacket->iObjID, pPacket->offsetX, pPacket->offsetY, pPacket->offsetZ );
}
//--------------------------------------------------------------------------------------
void CNetCli_Script::_PG_Script_LtoC_QuestAccept_Result ( int iCliID , int iSize , PVOID pData )
{
	M_PACKET( PG_Script_LtoC_QuestAccept_Result );	
	m_pThis->OnQuestAcceptResult( pPacket->iQuestField, pPacket->iQuestID, pPacket->emResult );
}
//--------------------------------------------------------------------------------------
void CNetCli_Script::_PG_Script_LtoC_QuestComplete_Result ( int iCliID , int iSize , PVOID pData )
{
	M_PACKET( PG_Script_LtoC_QuestComplete_Result );
	int a = 3;
	m_pThis->OnQuestCompleteResult( pPacket->iQuestNPCID, pPacket->iQuestField, pPacket->iQuestID, pPacket->emResult );
}
//--------------------------------------------------------------------------------------
void CNetCli_Script::_PG_Script_LtoC_QuestDelete_Result ( int iCliID , int iSize , PVOID pData )
{
	M_PACKET( PG_Script_LtoC_QuestDelete_Result );	
	m_pThis->OnQuestDeleteResult( pPacket->iQuestField, pPacket->iQuestID, pPacket->emResult );
}
//--------------------------------------------------------------------------------------
void CNetCli_Script::_PG_Script_LtoC_QuestFlag ( int iCliID , int iSize , PVOID pData )
{
	M_PACKET( PG_Script_LtoC_QuestFlag );
	m_pThis->OnQuestFlag( pPacket->iQuestFlagID, pPacket->iVal );
}
//--------------------------------------------------------------------------------------
void CNetCli_Script::_PG_Script_LtoC_ClickQuestObjResult		( int iCliID , int iSize , PVOID pData )
{
	M_PACKET( PG_Script_LtoC_ClickQuestObjResult );
	m_pThis->OnClickQuestObjResult( pPacket->iResult, pPacket->iItemID, pPacket->iItemVal );
}
//--------------------------------------------------------------------------------------
void CNetCli_Script::_PG_Script_LtoC_KillQuestObjNotify		( int iCliID , int iSize , PVOID pData )
{
	M_PACKET( PG_Script_LtoC_KillQuestObjNotify );
	m_pThis->OnKillQuestObjNotify( pPacket->iQuestField, pPacket->iQuestID, pPacket->iDeadObjID );
}
//--------------------------------------------------------------------------------------
void CNetCli_Script::_PG_Script_LtoC_PlayMotion		( int iCliID , int iSize , PVOID pData )
{
	M_PACKET( PG_Script_LtoC_PlayMotion );
	m_pThis->OnPlayMotion( pPacket->iObjID, pPacket->szMotion );
}
//--------------------------------------------------------------------------------------
void CNetCli_Script::_PG_Script_LtoC_Play3DSound( int iCliID , int iSize , PVOID pData )
{
	M_PACKET( PG_Script_LtoC_Play3DSound );
	m_pThis->OnPlay3DSound( pPacket->iObjID, pPacket->sSoundPath.Begin(), pPacket->bLoop, pPacket->iSoundID );
}
//--------------------------------------------------------------------------------------
void CNetCli_Script::_PG_Script_LtoC_PlayMusic( int iCliID , int iSize , PVOID pData )
{
	M_PACKET( PG_Script_LtoC_PlayMusic );
	m_pThis->OnPlayMusic( pPacket->iObjID, pPacket->sSoundPath.Begin(), pPacket->bLoop, pPacket->iSoundID );
}
//--------------------------------------------------------------------------------------
void CNetCli_Script::_PG_Script_LtoC_PlaySound( int iCliID , int iSize , PVOID pData )
{
	M_PACKET( PG_Script_LtoC_PlaySound );
	m_pThis->OnPlaySound( pPacket->iObjID, pPacket->sSoundPath.Begin(), pPacket->bLoop, pPacket->iSoundID );
}
//--------------------------------------------------------------------------------------
void CNetCli_Script::_PG_Script_LtoC_SpeakOption( int iCliID , int iSize , PVOID pData )
{
	//M_PACKET( _PG_Script_LtoC_SpeakOption );
	m_pThis->OnSpeakOption( iCliID, iSize, pData );
}
//--------------------------------------------------------------------------------------
void CNetCli_Script::_PG_Script_LtoC_SetQuestState( int iCliID , int iSize , PVOID pData )
{
	M_PACKET( PG_Script_LtoC_SetQuestState );
	m_pThis->OnQuestState( pPacket->iQuestID, pPacket->iState );
}
//--------------------------------------------------------------------------------------
void CNetCli_Script::_PG_Script_LtoC_ChoiceOptionResult( int iCliID , int iSize , PVOID pData )
{
	//M_PACKET( _PG_Script_LtoC_SpeakOption );
	m_pThis->OnChoiceOptionResult();
}
//--------------------------------------------------------------------------------------
void CNetCli_Script::_PG_Script_LtoC_CloseSpeak( int iCliID , int iSize , PVOID pData )
{
	m_pThis->OnCloseSpeak();
}
//--------------------------------------------------------------------------------------
void CNetCli_Script::_PG_Script_LtoC_ClearBorder ( int iCliID , int iSize , PVOID pData )
{
	m_pThis->OnClearBorder();
}
//--------------------------------------------------------------------------------------
void CNetCli_Script::_PG_Script_LtoC_AddBorderPage ( int iCliID , int iSize , PVOID pData )
{
	BYTE* ptr = (BYTE*) pData;
	ptr += sizeof( GamePGCommandEnum );
	m_pThis->OnAddBorderPage( (const char*) ptr );
}
//--------------------------------------------------------------------------------------
void CNetCli_Script::_PG_Script_LtoC_ShowBorder	( int iCliID , int iSize , PVOID pData )
{
	M_PACKET( PG_Script_LtoC_ShowBorder );
	m_pThis->OnShowBorder( pPacket->iQuestID, pPacket->szTitle.Begin(), pPacket->szTexture.Begin() );
}
//--------------------------------------------------------------------------------------
void CNetCli_Script::_PG_Script_LtoC_ScriptCast	( int iCliID , int iSize , PVOID pData )
{
	M_PACKET( PG_Script_LtoC_ScriptCast );
	m_pThis->OnScriptCast( pPacket->szString.Begin(), pPacket->iTime, pPacket->iMotionID, pPacket->iMotionEndID );
}
//--------------------------------------------------------------------------------------
void CNetCli_Script::_PG_Script_LtoC_ScriptCastResult ( int iCliID , int iSize , PVOID pData )
{
	M_PACKET( PG_Script_LtoC_ScriptCastResult );
	m_pThis->OnScriptCastResult( pPacket->iResult );
}
//--------------------------------------------------------------------------------------
void CNetCli_Script::_PG_Script_LtoC_ResetDailyQuest( int iCliID , int iSize , PVOID pData )
{
	M_PACKET( PG_Script_LtoC_ResetDailyQuest );
	m_pThis->OnResetDailyQuest( pPacket->iType );
}
//--------------------------------------------------------------------------------------
void CNetCli_Script::_PG_Script_LtoC_AttachCastMotionToolID	( int iCliID , int iSize , PVOID pData )
{
	M_PACKET( PG_Script_LtoC_AttachCastMotionToolID );
	m_pThis->OnAttachCastMotionToolID( pPacket->iObjID, pPacket->iToolID );
}
//--------------------------------------------------------------------------------------
void CNetCli_Script::_PG_Script_LtoC_AttachTool	( int iCliID , int iSize , PVOID pData )
{
	M_PACKET( PG_Script_LtoC_AttachTool );
	m_pThis->OnAttachTool( pPacket->iObjID, pPacket->iToolID, pPacket->LinkID.Begin() );
}

//--------------------------------------------------------------------------------------
void CNetCli_Script::_PG_Script_LtoC_StartClientCountDown	( int iCliID , int iSize , PVOID pData )
{
	M_PACKET( PG_Script_LtoC_StartClientCountDown );
	m_pThis->OnStartClientCountDown( pPacket->iStartNumber, pPacket->iEndNumber, pPacket->iOffset, pPacket->iMode, pPacket->iShowStatusBar, pPacket->szText.Begin());
}
//--------------------------------------------------------------------------------------
// 處理
//--------------------------------------------------------------------------------------
void CNetCli_Script::AcceptQuest			( int iQuestNpcID, int iQuestID )
{
	PG_Script_CtoL_QuestAccept Packet;

	Packet.iQuestNpcID	= iQuestNpcID;
	Packet.iQuestID		= iQuestID;

	SendToLocal( sizeof( Packet ) , &Packet );

}
//------------------------------------------------------------------------------
void CNetCli_Script::CompleteQuest		( int iQuestNpcID, int iQuestID, int iChoiceID )
{
	PG_Script_CtoL_QuestComplete Packet;

	Packet.iQuestNpcID	= iQuestNpcID;
	Packet.iQuestID		= iQuestID;
	Packet.iChoiceID	= iChoiceID;

	SendToLocal( sizeof( Packet ) , &Packet );
}
//--------------------------------------------------------------------------------------
void CNetCli_Script::DeleteQuest					( int iQuestField, int iQuestID )
{
	PG_Script_CtoL_QuestDelete	Packet;

	Packet.iQuestField	= iQuestField;
	Packet.iQuestID		= iQuestID;
	
	SendToLocal( sizeof( Packet ) , &Packet );
}
//--------------------------------------------------------------------------------------
void CNetCli_Script::TouchEvent			( int iObjID )
{
	PG_Script_CtoL_TouchEvent Packet;

	Packet.iEventObjID	= iObjID;

	SendToLocal( sizeof( Packet ), &Packet );
}
//--------------------------------------------------------------------------------------
void CNetCli_Script::RangeEvent			( int iObjID )
{
	PG_Script_CtoL_RangeEvent Packet;

	Packet.iEventObjID	= iObjID;

	SendToLocal( sizeof( Packet ), &Packet );
}
//--------------------------------------------------------------------------------------
void CNetCli_Script::CollisionEvent		( int iObjID )
{
	PG_Script_CtoL_CollisionEvent Packet;

	Packet.iEventObjID	= iObjID;

	SendToLocal( sizeof( Packet ), &Packet );
}
//--------------------------------------------------------------------------------------
void CNetCli_Script::ClickQuestObj ( int iQuestField, int iQuestID, int iClickObjID, int iClickObj )
{
	PG_Script_CtoL_ClickQuestObj	Packet;

	Packet.iQuestField	= iQuestField;
	Packet.iQuestID		= iQuestID;
	Packet.iClickObjID	= iClickObjID;
	Packet.iClickObj	= iClickObj;

	SendToLocal( sizeof( Packet ), &Packet );
}
//--------------------------------------------------------------------------------------
void CNetCli_Script::LoadSrvOption ( int iNPCID )
{
	PG_Script_CtoL_LoadQuestSpeakOption Packet;

	Packet.iNPCID	= iNPCID;

	SendToLocal( sizeof( Packet ), &Packet );
}
//--------------------------------------------------------------------------------------
void CNetCli_Script::ChoiceOption ( int iOptionID )
{
	if( m_bSendingOption == false )
	{
		PG_Script_CtoL_ChoiceOption Packet;


		Packet.iOptionID = iOptionID;
		m_bSendingOption = true;

		SendToLocal( sizeof( Packet ), &Packet );
	}
}
//--------------------------------------------------------------------------------------
void CNetCli_Script::DelayPatrol ( int iNPCID, int iTime )
{
	PG_Script_CtoL_DelayPatrol	Packet;

	Packet.iObjID	= iNPCID;
	Packet.iTime	= iTime;

	SendToLocal( sizeof( Packet ), &Packet );
}
//--------------------------------------------------------------------------------------
void CNetCli_Script::CloseBorder()
{
	PG_Script_CtoL_CloseBorder Packet;
	SendToLocal( sizeof( Packet ), &Packet );
}
//--------------------------------------------------------------------------------------
void CNetCli_Script::AcceptBorderQuest()
{
	PG_Script_CtoL_AcceptBorderQuest Packet;
	SendToLocal( sizeof( Packet ), &Packet );
}
//--------------------------------------------------------------------------------------