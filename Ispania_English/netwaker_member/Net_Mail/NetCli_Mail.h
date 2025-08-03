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

	//�e�H
	static void SL_SendMail	( MailBaseInfoStruct& MailBaseInfo , int ItemPos[10] , char* Content , char* Password );
	//�n�D�H��C��
	static void SL_MailList	( );
	//�h�H
	static void SL_SendBack	( int MailDBID );
	//�R�H
	static void SL_DelMail	( int MailDBID );
	//Ū�H
	static void SL_ReadMail	( int MailDBID );
	//���H�����~(�P����) �ΥI�� 
	//Money + �����o����  - �����I��
	static void SL_GetItem	( int MailDBID , int Money , int Money_Account , int Money_Bonus , const char* Password );
	//�q��Server�w����Mail����
	static void SL_CloseMail( );
	//�P�ɰe���h�Ӫ��a�H��
	static void SL_SendMailList( int TargetCount , StaticString <_MAX_NAMERECORD_SIZE_>	OrgTargetName[20] , const char* Title , MailBackGroundTypeENUM	PaperType , const char* Content );


	//�H�H���G
	virtual void RL_SendMailResult	( SendMailResultEnum Result , const char* TargetName ) = 0;
	//���H�����~�����G
	virtual void RL_GetItemResult	( int MailDBID , bool Result ) = 0;
	//�n�D�}��Mail
	virtual	void RL_OpenMail( int NpcGUID ) = 0;
	//�n�D����Mail
	virtual	void RL_CloseMail( ) = 0;


	//Ū���H��C�����G
	//	1��1�����^��
	virtual void RD_MailListResult		( int MailDBID , int Index , MailBaseInfoStruct&  MailBaseInfo , char* Content ) = 0;
	virtual void RD_MailListCountResult	( int MaxCount , int Count ) = 0;

	//Ū���H�󤺮e
//	virtual void RD_ReadMailResult	( int MailDBID , MailBaseInfoStruct&  MailBaseInfo , char* Content  , bool Result ) = 0;

	virtual void RD_UnReadMailCount( int Count , int GiftCount ) = 0;
	virtual void RD_NewMail( bool IsGift ) = 0;
};

