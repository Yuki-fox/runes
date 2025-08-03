#include "NetCli_Talk.h"
#include "../../MainProc/NetGlobal.h"
//-------------------------------------------------------------------
NetCli_Talk*    NetCli_Talk::This         = NULL;

//-------------------------------------------------------------------
bool NetCli_Talk::_Init()
{
	/*
    _Net.RegOnSrvPacketFunction	( EM_PG_Talk_TalkTo         			, _PG_Talk_TalkTo           		);
    _Net.RegOnSrvPacketFunction	( EM_PG_Talk_Range          			, _PG_Talk_Range            		);
    _Net.RegOnSrvPacketFunction	( EM_PG_Talk_LtoC_SystemMsg 			, _PG_Talk_LtoC_SystemMsg   		);
    _Net.RegOnSrvPacketFunction	( EM_PG_GM_LtoC_SendMsg     			, _PG_GM_LtoC_SendMsg				);
    _Net.RegOnSrvPacketFunction	( EM_PG_Talk_Channel					, _PG_Talk_Channel					);
	_Net.RegOnSrvPacketFunction	( EM_PG_GM_LtoC_SendGameMessageEvent	, _PG_GM_LtoC_SendGameMessageEvent  );
	_Net.RegOnSrvPacketFunction	( EM_PG_Talk_LtoC_ScriptMessage			, _PG_Talk_LtoC_ScriptMessage		);
	_Net.RegOnSrvPacketFunction	( EM_PG_GM_LtoC_GameMessageDataEvent	, _PG_GM_LtoC_GameMessageDataEvent  );
	_Net.RegOnSrvPacketFunction	( EM_PG_Talk_LtoC_RunningMsg			, _PG_Talk_LtoC_RunningMsg			);
	_Net.RegOnSrvPacketFunction	( EM_PG_Talk_DtoC_OfflineMessage		, _PG_Talk_DtoC_OfflineMessage		);
*/
	Cli_NetReg( PG_Talk_TalkTo         			);
	Cli_NetReg( PG_Talk_Range          			);
	Cli_NetReg( PG_Talk_LtoC_SystemMsg 			);
	Cli_NetReg( PG_GM_LtoC_SendMsg     			);
	Cli_NetReg( PG_Talk_Channel					);
	Cli_NetReg( PG_GM_LtoC_SendGameMessageEvent	);
	Cli_NetReg( PG_Talk_LtoC_ScriptMessage		);
	Cli_NetReg( PG_GM_LtoC_GameMessageDataEvent	);
	Cli_NetReg( PG_Talk_LtoC_RunningMsg			);
	Cli_NetReg( PG_Talk_LtoC_RunningMsgEx		);
	Cli_NetReg( PG_Talk_DtoC_OfflineMessage		);

	Cli_NetReg( PG_Talk_ChattoC_BillBoardInfo	);
	Cli_NetReg( PG_Talk_ChattoC_BillBoardDelete	);

    return true;
}
//-------------------------------------------------------------------
bool NetCli_Talk::_Release()
{
    return true;
}
//-------------------------------------------------------------------
void NetCli_Talk::_PG_Talk_ChattoC_BillBoardInfo	( int Sockid , int Size , void* Data )
{
	PG_Talk_ChattoC_BillBoardInfo* PG = (PG_Talk_ChattoC_BillBoardInfo*)Data;
	This->Rev_BillBoardInfo( PG->PlayerDBID , PG->DataSize , PG->Data );
}
void NetCli_Talk::_PG_Talk_ChattoC_BillBoardDelete( int Sockid , int Size , void* Data )
{
	PG_Talk_ChattoC_BillBoardDelete* PG = (PG_Talk_ChattoC_BillBoardDelete*)Data;
	This->Rev_BillBoardDelete( PG->PlayerDBID );
}
void NetCli_Talk::_PG_Talk_DtoC_OfflineMessage	( int Sockid , int Size , void* Data )
{
	PG_Talk_DtoC_OfflineMessage* PG = (PG_Talk_DtoC_OfflineMessage*)Data;
	This->Rev_OfflineMessage( PG->FromDBID , (int)PG->Time , PG->FromName.Begin() , PG->ToName.Begin() , PG->Content.Begin() );
}

void NetCli_Talk::_PG_Talk_LtoC_RunningMsg		( int Sockid , int Size , void* Data )
{
	PG_Talk_LtoC_RunningMsg* PG = (PG_Talk_LtoC_RunningMsg*)Data;
	This->Rev_RunningMsg( PG->Content );
}
void NetCli_Talk::_PG_Talk_LtoC_RunningMsgEx		( int Sockid , int Size , void* Data )
{
	PG_Talk_LtoC_RunningMsgEx* PG = (PG_Talk_LtoC_RunningMsgEx*)Data;
	This->Rev_RunningMsgEx( PG->Type , PG->Content );
}
void NetCli_Talk::_PG_GM_LtoC_GameMessageDataEvent( int Sockid , int Size , void* Data )
{
	PG_GM_LtoC_GameMessageDataEvent* PG = (PG_GM_LtoC_GameMessageDataEvent*)Data;
	This->Rev_GameMessageDataEvent( PG->Event , PG->iNumArg, PG->Data );
}

void NetCli_Talk::_PG_GM_LtoC_SendGameMessageEvent( int Sockid , int Size , void* Data )
{
	PG_GM_LtoC_SendGameMessageEvent* PG = (PG_GM_LtoC_SendGameMessageEvent*) Data;
	This->Rev_GameMessageEvent( PG->Event );
}

void NetCli_Talk::_PG_Talk_TalkTo         ( int Sockid , int Size , void* Data )
{
    PG_Talk_TalkTo* PG = (PG_Talk_TalkTo*) Data;
	This->Rev_TalkTo( PG->Job , PG->IsSystem , PG->SenderName.Begin() , PG->TargetName.Begin(), PG->ContentSize , PG->Content.Begin(), PG->SenderInfo );
}
void NetCli_Talk::_PG_Talk_Range          ( int Sockid , int Size , void* Data )
{
    PG_Talk_Range* PG = (PG_Talk_Range*) Data;
    This->Rev_Range( PG->Job , PG->IsSystem , PG->GItemID , PG->Name.Begin() , PG->ContentSize , PG->Content.Begin(), PG->IsNPCGossip, PG->SenderInfo );
}
/*
void NetCli_Talk::_PG_Talk_Zone           ( int Sockid , int Size , void* Data )
{
    PG_Talk_Zone* PG = (PG_Talk_Zone*) Data;
    This->Rev_Zone( PG->GItemID , PG->Name.Begin() , PG->ContentSize , PG->Content.Begin() );
}
*/
void NetCli_Talk::_PG_Talk_LtoC_SystemMsg ( int Sockid , int Size , void* Data )
{
    PG_Talk_LtoC_SystemMsg* PG = (PG_Talk_LtoC_SystemMsg*) Data;
    This->Rev_SystemMsg(  PG->ContentSize , PG->Content.Begin() );
}
void NetCli_Talk::_PG_GM_LtoC_SendMsg     ( int Sockid , int Size , void* Data )
{
	if ( s_boFinishUI )	{
		PG_GM_LtoC_SendMsg* PG = (PG_GM_LtoC_SendMsg*) Data;
		This->Rev_GmMsg(  PG->Content.Begin() );
	} else {
		PushTempEvent(Sockid, Size, Data, _PG_GM_LtoC_SendMsg);
	}
    
}
void NetCli_Talk::_PG_Talk_Channel	    ( int Sockid , int Size , void* Data )
{
	PG_Talk_Channel* PG = (PG_Talk_Channel*) Data;
	This->Rev_Channel	 ( PG->Job , PG->Type , PG->ChannelID , PG->MsgType , (char*)PG->Name.Begin() , (char*)PG->Content.Begin(), PG->SenderInfo );

}

void NetCli_Talk::_PG_Talk_LtoC_ScriptMessage	    ( int Sockid , int Size , void* pData )
{
	char*				ptr			= (char*)pData;

	GamePGCommandEnum	Command;
	int					iLen;
	int					iSayerID;
	int					iTypeID;
	int					iColorID;
	char				szBuff[ 4096 ];

	Command			= *((GamePGCommandEnum*)ptr);	
	ptr				+= sizeof( GamePGCommandEnum );

	iSayerID		= *((int*)ptr);
	ptr				+= sizeof( int );

	iTypeID			= *((int*)ptr);
	ptr				+= sizeof( int );

	iColorID		= *((int*)ptr);
	ptr				+= sizeof( int );

	iLen			= *((int*)ptr);
	ptr				+= sizeof( int );

	if( iLen != 0 ) 
	{
		strcpy( szBuff, ptr );	
		ptr				+= iLen;
	}

	This->Rev_ScriptMessage( iSayerID, iTypeID, iColorID, iLen, szBuff );
}
//-------------------------------------------------------------------
void NetCli_Talk::TalkTo( const char* TargetName , const char* Content, TalkSenderInfo &SenderInfo )
{
    PG_Talk_TalkTo Send;
	Send.SenderInfo = SenderInfo;
    Send.ContentSize = int( min( sizeof( Send.Content) , strlen( Content ) + 1 ));
//    strncpy( Send.Content , Content , sizeof( Send.Content ) );
//    strncpy( Send.Name , Name , sizeof( Send.Name ) );
    Send.Content		= Content;
	//Send.SenderName		= SenderName;
    Send.TargetName		= TargetName;
    
    SendToLocal_Base( sizeof(Send) , &Send );
}
//-------------------------------------------------------------------
void NetCli_Talk::Range( const char* Content, TalkSenderInfo &SenderInfo   )
{
    PG_Talk_Range Send;

	Send.SenderInfo = SenderInfo;
    Send.ContentSize = int( min( sizeof( Send.Content) , strlen( Content ) + 1));
    Send.Content = Content;
    SendToLocal( sizeof(Send) , &Send );

}
//-------------------------------------------------------------------
/*
void NetCli_Talk::Zone( const char* Content )
{
    PG_Talk_Zone Send;

    Send.ContentSize = int( min( sizeof( Send.Content) , strlen( Content ) + 1 ));
//    strncpy( Send.Content , Content , sizeof( Send.Content ) );
    Send.Content = Content;

    SendToLocal( sizeof(Send) , &Send );

}
*/
//-------------------------------------------------------------------
void NetCli_Talk::GMCommand( const char* Content )
{
    PG_Talk_CtoL_GMCommand Send;

    Send.ContentSize = int( min( sizeof( Send.Content) , strlen( Content ) + 1 ));
//    strncpy( Send.Content , Content , sizeof( Send.Content ) );
    Send.Content = Content;

    SendToLocal( sizeof(Send) , &Send );
}
//-------------------------------------------------------------------
void NetCli_Talk::GMMsg       ( const char* Content )
{
    PG_GM_CtoL_SendMsg Send;
    Send.ContentSize = int( min( sizeof( Send.Content) , strlen( Content ) + 1 ));
    Send.Content = Content;

    SendToLocal( sizeof(Send) , &Send );

}
//-------------------------------------------------------------------
void NetCli_Talk::Channel( GroupObjectTypeEnum Type , int ChannelID , char* Name , char* Content, TalkSenderInfo &SenderInfo )
{
	PG_Talk_Channel Send;
	Send.Type = Type;
	Send.ChannelID = ChannelID;
	Send.Name = Name;
	Send.SenderInfo = SenderInfo;
	Send.Content = Content;
	Send.ContentSize = Send.Content.Size() + 1;

	switch( Type )
	{
	case EM_GroupObjectType_PlayerChannel:		//玩家的
	case EM_GroupObjectType_Party:				//隊伍
		SendToChat( sizeof(Send) , &Send );
		break;
	case EM_GroupObjectType_SysZoneChannel:		//每個區域都有的
		SendToLocal( sizeof(Send) , &Send );
		break;
	case EM_GroupObjectType_SysChannel:			//系統
	case EM_GroupObjectType_Guild:				//工會
		SendToLocal_Base( sizeof(Send) , &Send );
		break;
	}
	
}
void NetCli_Talk::SChat_RegisterBillBoardInfo	( int DataSize , char* Data )
{
	PG_Talk_CtoChat_RegisterBillBoardInfo Send;
	Send.DataSize = DataSize;
	memcpy( Send.Data , Data , DataSize );
	SendToChat( sizeof(Send) , &Send  );
}
void NetCli_Talk::SChat_BillBoardLiveTime		( int LiveTime )
{
	PG_Talk_CtoChat_BillBoardLiveTime Send;
	Send.LiveTime = LiveTime;
	SendToChat( sizeof(Send) , &Send  );
}