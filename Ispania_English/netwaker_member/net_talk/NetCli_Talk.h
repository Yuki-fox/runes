#ifndef __NETWAKER_TALK_H__
#define __NETWAKER_TALK_H__
#include "PG/PG_Talk.h"
#include "../../MainProc/NetGlobal.h"

class NetCli_Talk : public CNetGlobal
{
protected:
    //-------------------------------------------------------------------
    static NetCli_Talk* This;
    static bool _Init();
    static bool _Release();
    //-------------------------------------------------------------------

    static void _PG_Talk_TalkTo         		( int Sockid , int Size , void* Data );
    static void _PG_Talk_Range          		( int Sockid , int Size , void* Data );
    static void _PG_Talk_LtoC_SystemMsg 		( int Sockid , int Size , void* Data );
    static void _PG_GM_LtoC_SendMsg     		( int Sockid , int Size , void* Data );
	static void _PG_Talk_Channel	    		( int Sockid , int Size , void* Data );
	static void _PG_GM_LtoC_SendGameMessageEvent( int Sockid , int Size , void* Data );
	static void _PG_Talk_LtoC_ScriptMessage		( int Sockid , int Size , void* Data );
	static void _PG_GM_LtoC_GameMessageDataEvent( int Sockid , int Size , void* Data );
	static void _PG_Talk_LtoC_RunningMsg		( int Sockid , int Size , void* Data );
	static void _PG_Talk_LtoC_RunningMsgEx		( int Sockid , int Size , void* Data );
	static void _PG_Talk_DtoC_OfflineMessage	( int Sockid , int Size , void* Data );
	static void _PG_Talk_ChattoC_BillBoardInfo	( int Sockid , int Size , void* Data );
	static void _PG_Talk_ChattoC_BillBoardDelete( int Sockid , int Size , void* Data );
public:    
    NetCli_Talk() { This = this; }
	static NetCli_Talk*		GetThis()			{ return This; }
    static void TalkTo      ( const char* TargetName, const char* Content, TalkSenderInfo &SenderInfo );
    static void Range       ( const char* Content, TalkSenderInfo &SenderInfo );
    static void GMCommand   ( const char* Content );
    static void GMMsg       ( const char* Content );
	static void Channel		( GroupObjectTypeEnum Type , int ChannelID , char* Name , char* Content, TalkSenderInfo &SenderInfo );
	
    //-------------------------------------------------------------------
    //收到的事件
    //-------------------------------------------------------------------
    virtual void Rev_TalkTo      			( Voc_ENUM Job , bool IsSystem , const char* SenderName , const char* TargetName, int ContentSize , const char* Content, TalkSenderInfo& SenderInfo) = 0;
    virtual void Rev_Range       			( Voc_ENUM Job , bool IsSystem , int Sayer , const char* Name , int ContentSize , const char* Content, bool IsNPCGossip, TalkSenderInfo& SenderInfo ) = 0;
    virtual void Rev_SystemMsg   			( int ContentSize , const char* Content ) = 0;
    virtual void Rev_GmMsg       			( const char* Msg ) = 0;
	virtual void Rev_Channel	 			( Voc_ENUM Job , GroupObjectTypeEnum Type , int ChannelID , TalkChannelTypeENUM MsgType , char* Name , char* Content, TalkSenderInfo& SenderInfo ) = 0;
	virtual void Rev_GameMessageEvent		( GameMessageEventENUM Evnet ) = 0;
	virtual void Rev_GameMessageDataEvent	( GameMessageEventDataENUM Event , int iNumArg, GameMessageDataArgStruct Data[5] ) = 0;
	virtual void Rev_ScriptMessage			( int Sayer, int iTypeID, int iColorID, int iStringLen, const char* szString ) = 0;
	virtual void Rev_RunningMsg				( const char* Msg ) = 0;
	virtual void Rev_RunningMsgEx			( RunningMsgExTypeENUM Type , const char* Msg ) = 0;
	virtual void Rev_OfflineMessage			( int FromDBID , int Time , const char* FromName  , const char* ToName , const char* Content ) = 0;
    //-------------------------------------------------------------------
	//公告資料
	virtual void Rev_BillBoardInfo( int PlayerDBID , int DataSize , char* Data ) = 0;
	virtual void Rev_BillBoardDelete( int PlayerDBID ) = 0;

	static void SChat_RegisterBillBoardInfo	( int DataSize , char* Data );
	static void SChat_BillBoardLiveTime		(  int LiveTime );

	static void	S_PG_Talk_LtoC_SystemMsg	( int Sockid , int Size , void* Data )		{ _PG_Talk_LtoC_SystemMsg(Sockid, Size, Data); }
};

#endif //__NETWAKER_TALK_H__