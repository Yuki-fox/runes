#include "NetCli_Mail.h"

//-------------------------------------------------------------------
NetCli_Mail*    NetCli_Mail::This         = NULL;

//-------------------------------------------------------------------
bool NetCli_Mail::_Init()
{
/*
	_Net.RegOnSrvPacketFunction	( EM_PG_Mail_LtoC_SendMailResult        , _PG_Mail_LtoC_SendMailResult		);
	_Net.RegOnSrvPacketFunction	( EM_PG_Mail_LtoC_GetItemResult	        , _PG_Mail_LtoC_GetItemResult		);
	_Net.RegOnSrvPacketFunction	( EM_PG_Mail_DtoC_MailListResult        , _PG_Mail_DtoC_MailListResult		);
	_Net.RegOnSrvPacketFunction	( EM_PG_Mail_DtoC_MailListCountResult   , _PG_Mail_DtoC_MailListCountResult );
	_Net.RegOnSrvPacketFunction	( EM_PG_Mail_LtoC_OpenMail	   			, _PG_Mail_LtoC_OpenMail			);
	_Net.RegOnSrvPacketFunction	( EM_PG_Mail_LtoC_CloseMail	   			, _PG_Mail_LtoC_CloseMail			);
	_Net.RegOnSrvPacketFunction	( EM_PG_Mail_DtoC_UnReadMailCount		, _PG_Mail_DtoC_UnReadMailCount );
	_Net.RegOnSrvPacketFunction	( EM_PG_Mail_DtoC_NewMail	   			, _PG_Mail_DtoC_NewMail			);
	*/

	Cli_NetReg( PG_Mail_LtoC_SendMailResult			);
	Cli_NetReg( PG_Mail_LtoC_GetItemResult	        );
	Cli_NetReg( PG_Mail_DtoC_MailListResult			);
	Cli_NetReg( PG_Mail_DtoC_MailListCountResult	);
	Cli_NetReg( PG_Mail_LtoC_OpenMail	   			);
	Cli_NetReg( PG_Mail_LtoC_CloseMail	   			);
	Cli_NetReg( PG_Mail_DtoC_UnReadMailCount		);
	Cli_NetReg( PG_Mail_DtoC_NewMail	   			);

    return true;
}
//-------------------------------------------------------------------
bool NetCli_Mail::_Release()
{
    return true;
}
//-------------------------------------------------------------------
void NetCli_Mail::_PG_Mail_DtoC_UnReadMailCount		( int Sockid , int Size , void* Data )
{
	PG_Mail_DtoC_UnReadMailCount* PG = (PG_Mail_DtoC_UnReadMailCount*) Data;
	This->RD_UnReadMailCount( PG->Count , PG->GiftCount );
}
void NetCli_Mail::_PG_Mail_DtoC_NewMail				( int Sockid , int Size , void* Data )
{
	PG_Mail_DtoC_NewMail* PG = (PG_Mail_DtoC_NewMail*) Data;
	This->RD_NewMail( PG->IsGift );
}

void NetCli_Mail::_PG_Mail_LtoC_OpenMail	( int Sockid , int Size , void* Data )
{
	PG_Mail_LtoC_OpenMail* PG = (PG_Mail_LtoC_OpenMail*) Data;
	This->RL_OpenMail( PG->NpcGUID );
}
void NetCli_Mail::_PG_Mail_LtoC_CloseMail	( int Sockid , int Size , void* Data )
{
	PG_Mail_LtoC_CloseMail* PG = (PG_Mail_LtoC_CloseMail*) Data;
	This->RL_CloseMail( );
}
void NetCli_Mail::_PG_Mail_LtoC_SendMailResult	( int Sockid , int Size , void* Data )
{
	PG_Mail_LtoC_SendMailResult* PG = (PG_Mail_LtoC_SendMailResult*) Data;
	This->RL_SendMailResult( PG->Result , PG->TargetName.Begin() );
}
void NetCli_Mail::_PG_Mail_LtoC_GetItemResult	( int Sockid , int Size , void* Data )
{
	PG_Mail_LtoC_GetItemResult* PG = (PG_Mail_LtoC_GetItemResult*) Data;
	This->RL_GetItemResult( PG->MailDBID , PG->Result );
}
void NetCli_Mail::_PG_Mail_DtoC_MailListResult	( int Sockid , int Size , void* Data )
{
	PG_Mail_DtoC_MailListResult* PG = (PG_Mail_DtoC_MailListResult*) Data;

	This->RD_MailListResult( PG->MailDBID , PG->Index , PG->MailBaseInfo , (char*)PG->Content.Begin() );

}
void NetCli_Mail::_PG_Mail_DtoC_MailListCountResult	( int Sockid , int Size , void* Data )
{
	PG_Mail_DtoC_MailListCountResult* PG = (PG_Mail_DtoC_MailListCountResult*) Data;

	This->RD_MailListCountResult( PG->MaxCount , PG->Count);

}
/*
void NetCli_Mail::_PG_Mail_DtoC_ReadMailResult	( int Sockid , int Size , void* Data )
{
	PG_Mail_DtoC_ReadMailResult* PG = (PG_Mail_DtoC_ReadMailResult*) Data;

	This->RD_ReadMailResult( PG->MailDBID , PG->MailBaseInfo , (char*)PG->Content.Begin() , PG->Result );
}
*/
//-------------------------------------------------------------------
void NetCli_Mail::SL_SendMail	( MailBaseInfoStruct& MailBaseInfo , int ItemPos[10] , char* Content , char* Password )
{
	PG_Mail_CtoL_SendMail Send;
	Send.MailBaseInfo = MailBaseInfo;
	memcpy( Send.ItemPos , ItemPos , sizeof(Send.ItemPos) );
	//Send.ItemPos = ItemPos;
	Send.Content = Content;
	Send.ContentSize = Send.Content.Size()+1;
	Send.Password = Password;

	SendToLocal( Send.PGSize() , &Send );
}
void NetCli_Mail::SL_MailList	(  )
{
	PG_Mail_CtoL_MailList Send;

	SendToLocal( sizeof(Send) , &Send );
}
void NetCli_Mail::SL_SendBack	( int MailDBID )
{
	PG_Mail_CtoL_SendBack Send;
	Send.MailDBID = MailDBID;

	SendToLocal( sizeof(Send) , &Send );
}
void NetCli_Mail::SL_DelMail	( int MailDBID )
{
	PG_Mail_CtoL_DelMail Send;
	Send.MailDBID = MailDBID;

	SendToLocal( sizeof(Send) , &Send );
}
void NetCli_Mail::SL_ReadMail	( int MailDBID )
{
	PG_Mail_CtoL_ReadMail Send;
	Send.MailDBID = MailDBID;

	SendToLocal( sizeof(Send) , &Send );
}
void NetCli_Mail::SL_GetItem	( int MailDBID , int Money , int Money_Account , int Money_Bonus , const char* Password )
{
	PG_Mail_CtoL_GetItem Send;
	Send.MailDBID = MailDBID;
	Send.Money = Money;
	Send.Money_Account = Money_Account;
	Send.Money_Bonus = Money_Bonus;
	Send.Password = Password;

	SendToLocal( sizeof(Send) , &Send );
}
//通知Server已關閉Mail介面
void NetCli_Mail::SL_CloseMail( )
{
	PG_Mail_CtoL_CloseMail Send;

	SendToLocal( sizeof(Send) , &Send );
}
//同時送給多個玩家信件
void NetCli_Mail::SL_SendMailList( int TargetCount , StaticString <_MAX_NAMERECORD_SIZE_>	OrgTargetName[20] , const char* Title , MailBackGroundTypeENUM	PaperType , const char* Content )
{
	if( (unsigned)TargetCount > 20 )
		return ;

	PG_Mail_CtoL_SendMailList Send;
	Send.TargetCount = TargetCount;
	for( int i = 0 ; i < TargetCount ; i++ )
		Send.OrgTargetName[i] = OrgTargetName[i];

	Send.Title = Title;
	Send.PaperType = PaperType;
	Send.Content = Content;
	Send.ContentSize = Send.Content.Size();
	SendToLocal( Send.PGSize() , &Send );
}

//-------------------------------------------------------------------
