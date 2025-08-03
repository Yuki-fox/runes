#pragma once

#include "NetCli_Talk.h"

class NetCli_TalkChild : public NetCli_Talk
{
	PrintClass  PrinttoFile;

public:
	static bool _Init();
	static bool _Release();	

	virtual	void	Rev_TalkTo					( Voc_ENUM Job , bool IsSystem , const char* SenderName , const char* TargetName, int ContentSize , const char* Content, TalkSenderInfo& SenderInfo );
	virtual	void	Rev_Range					( Voc_ENUM Job , bool IsSystem , int Sayer , const char* Name , int ContentSize , const char* Content, bool IsNPCGossip, TalkSenderInfo& SenderInfo );
	virtual	void	Rev_SystemMsg				( int ContentSize , const char* Content );
	virtual	void	Rev_GmMsg					( const char* Msg );
	virtual void	Rev_Channel					( Voc_ENUM Job , GroupObjectTypeEnum Type , int ChannelID , TalkChannelTypeENUM MsgType , char* Name , char* Content, TalkSenderInfo& SenderInfo );
	virtual void	Rev_GameMessageEvent		( GameMessageEventENUM Evnet );
	virtual void	Rev_ScriptMessage			( int Sayer, int iTypeID, int iColorID, int iStringLen, const char* szString );
	virtual void	Rev_GameMessageDataEvent	( GameMessageEventDataENUM Event , int iNumArg, GameMessageDataArgStruct Data[5] );
	virtual void	Rev_RunningMsg				( const char* Msg );
	virtual void	Rev_RunningMsgEx			( RunningMsgExTypeENUM Type , const char* Msg );
	virtual void	Rev_OfflineMessage			( int FromDBID , int Time , const char* FromName  , const char* ToName , const char* Content );

	virtual void 	Rev_BillBoardInfo			( int PlayerDBID , int DataSize , char* Data );
	virtual void 	Rev_BillBoardDelete			( int PlayerDBID );
};