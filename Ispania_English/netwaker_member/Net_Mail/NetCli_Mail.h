#pragma once

#include "PG/PG_Mail.h"
#include "../../MainProc/NetGlobal.h"

class NetCli_Mail : public CNetGlobal
{
protected:
    //-------------------------------------------------------------------
    static NetCli_Mail* This;
    static bool _Init();
    static bool _Release();
    //-------------------------------------------------------------------
	static void _PG_Mail_LtoC_SendMailResult		( int Sockid , int Size , void* Data );
	static void _PG_Mail_LtoC_GetItemResult			( int Sockid , int Size , void* Data );
	static void _PG_Mail_DtoC_MailListResult		( int Sockid , int Size , void* Data );
	static void _PG_Mail_DtoC_MailListCountResult	( int Sockid , int Size , void* Data );
	static void _PG_Mail_LtoC_OpenMail				( int Sockid , int Size , void* Data );
	static void _PG_Mail_LtoC_CloseMail				( int Sockid , int Size , void* Data );

	static void _PG_Mail_DtoC_UnReadMailCount		( int Sockid , int Size , void* Data );
	static void _PG_Mail_DtoC_NewMail				( int Sockid , int Size , void* Data );

public:    
    NetCli_Mail() { This = this; }

	//送信
	static void SL_SendMail	( MailBaseInfoStruct& MailBaseInfo , int ItemPos[10] , char* Content , char* Password );
	//要求信件列表
	static void SL_MailList	( );
	//退信
	static void SL_SendBack	( int MailDBID );
	//刪信
	static void SL_DelMail	( int MailDBID );
	//讀信
	static void SL_ReadMail	( int MailDBID );
	//取信中物品(與金錢) 或付錢 
	//Money + 為取得金錢  - 為獲到付款
	static void SL_GetItem	( int MailDBID , int Money , int Money_Account , int Money_Bonus , const char* Password );
	//通知Server已關閉Mail介面
	static void SL_CloseMail( );
	//同時送給多個玩家信件
	static void SL_SendMailList( int TargetCount , StaticString <_MAX_NAMERECORD_SIZE_>	OrgTargetName[20] , const char* Title , MailBackGroundTypeENUM	PaperType , const char* Content );


	//寄信結果
	virtual void RL_SendMailResult	( SendMailResultEnum Result , const char* TargetName ) = 0;
	//取信中物品的結果
	virtual void RL_GetItemResult	( int MailDBID , bool Result ) = 0;
	//要求開啟Mail
	virtual	void RL_OpenMail( int NpcGUID ) = 0;
	//要求關閉Mail
	virtual	void RL_CloseMail( ) = 0;


	//讀取信件列表的結果
	//	1筆1筆的回傳
	virtual void RD_MailListResult		( int MailDBID , int Index , MailBaseInfoStruct&  MailBaseInfo , char* Content ) = 0;
	virtual void RD_MailListCountResult	( int MaxCount , int Count ) = 0;

	//讀取信件內容
//	virtual void RD_ReadMailResult	( int MailDBID , MailBaseInfoStruct&  MailBaseInfo , char* Content  , bool Result ) = 0;

	virtual void RD_UnReadMailCount( int Count , int GiftCount ) = 0;
	virtual void RD_NewMail( bool IsGift ) = 0;
};

