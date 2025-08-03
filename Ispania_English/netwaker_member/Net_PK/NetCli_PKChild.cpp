#include "NetCli_PKChild.h"
#include "../../mainproc/GameMain.h"
#include "../Interface/WorldFrames/ChatFrame.h"
#include "../Interface/WorldFrames/GameControl.h"
//-----------------------------------------------------------------------------
bool NetCli_PKChild::_Init()
{
	if (This)
		return false;

	This = new NetCli_PKChild;
	return NetCli_PK::_Init();
}

//-----------------------------------------------------------------------------
bool NetCli_PKChild::_Release()
{
	NetCli_PK::_Release();
	if (This)
	{
		delete This;
		This = NULL;
	}
	return true;
}
//-----------------------------------------------------------------------------
void NetCli_PKChild::R_PKInviteNotify	( int TargetID , int PKScore  )
{
	/*
	char Buf[256];
	sprintf( Buf , "PK­n½Ð TargetID = %d PKScore = %d" , TargetID , PKScore  );
	g_pChatFrame->AddMessage(5, Buf );
	*/

	// g_pError->AddMessage(0, 0, "[R_PKInviteNotify] %d %d", TargetID, PKScore);

	g_pGameControl->SetDuelStep(eDuelStep_Receive, TargetID);
}
void NetCli_PKChild::R_PKInviteResult	( int TargetID , bool Result )
{
	/*
	char Buf[256];
	sprintf( Buf , "PK­n½Ðµ²ªG TargetID = %d Result = %d" , TargetID , Result  );
	g_pChatFrame->AddMessage(5, Buf);
	*/

	// g_pError->AddMessage(0, 0, "[R_PKInviteResult] %d %d", TargetID, Result);

	DuelInfo info = g_pGameControl->GetDuelInfo();
	if ( info.target == TargetID )
	{
		if ( Result )
		{
			// ¦P·N¶}©l¨M°«
		}
		else
		{						
			// ¹ï¤è©Ú“F¥æ©ö
			CRoleSprite* target = g_pGameMain->FindSpriteByID(TargetID);
			if ( target ) {
				g_pChatFrame->SendChatMessageEx(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_DUEL_DISAGREE"), target->GetName());
			}
			g_pGameControl->SetDuelStep(eDuelStep_Refuse, TargetID);
		}
	}
}
void NetCli_PKChild::R_PKPrepare		(  int Player1ID , int Player2ID , int Player1Score , int Player2Score 
										 , int X , int Y , int Z , int Range , int Time)
{
	/*
	char Buf[256];
	sprintf( Buf , "PK·Ç³Æ¶}©l Player1ID = %d Player2ID = %d" , Player1ID , Player2ID  );
	g_pChatFrame->AddMessage(5, Buf );
	*/

	g_pGameControl->StartDuel((float)X, (float)Y, (float)Z, (float)Range);
	g_pGameControl->SetDuelStep(eDuelStep_Ready, 0);

}
void NetCli_PKChild::R_PKBegin			( int Time )
{
	//g_pChatFrame->AddMessage(5, "PK¶}©l" );

	g_pGameControl->SetDuelStep(eDuelStep_Start, 0);
}
void NetCli_PKChild::R_PKEnd			( int WinnerID , int LoserID , bool IsEven )
{
	//char Buf[256];
	const char* winner = g_ObjectData->GetString("SYS_YOU");
	const char* loser = g_ObjectData->GetString("SYS_YOU");

	DuelInfo info = g_pGameControl->GetDuelInfo();
	if ( g_pGameMain->GetGItemID() == WinnerID ) {
		loser = info.targetName.c_str();
		winner = g_pGameMain->GetPlayerName();
	} else {
		winner = info.targetName.c_str();
		loser = g_pGameMain->GetPlayerName();
	}

	if ( IsEven )
		g_pChatFrame->SendChatMessageEx(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_DUEL_RESULT_DRAW"), winner, loser);
	else {

		// »{¿é°Ê§@
		CRoleSprite* sprite = g_pGameMain->FindSpriteByID(LoserID);
		if ( sprite )
		{
			sprite->SetActorState(ruFUSION_ACTORSTATE_EMOTE_BEG);
		}
		g_pChatFrame->SendChatMessageEx(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_DUEL_RESULT_WIN"), winner, loser);
	}


	/*
	CRoleSprite* player = g_pGameMain->GetPlayer();
	if ( player )
	{
		if ( IsEven )
		{
			g_pChatFrame->SendChatMessageEx(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_DUEL_RESULT_WIN"), loser);
		}

		

		if ( player->GetIdentify() == WinnerID ) {
			g_pChatFrame->SendChatMessageEx(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_DUEL_RESULT_WIN"), loser);
		} else if ( player->GetIdentify() == LoserID ) {
			player->SetActorState(ruFUSION_ACTORSTATE_EMOTE_BEG);
			g_pChatFrame->SendChatMessageEx(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_DUEL_RESULT_LOSE"), winner);			
		}
	}
	*/

	g_pGameControl->SetDuelStep(eDuelStep_Finish, 0);
}
void NetCli_PKChild::R_PKCancel			( )
{
	DuelInfo info = g_pGameControl->GetDuelInfo();
	switch ( info.step )
	{
	case eDuelStep_None:
	case eDuelStep_Cancel:
		return;
	}

	// ¹ï¤è¨ú®ø¨M°«
	g_pChatFrame->SendChatMessageEx(CHAT_MSG_SYSTEM, "", "%s %s", info.targetName.c_str(), g_ObjectData->GetString("SYS_DUEL_CANCEL"));
	g_pGameControl->SetDuelStep(eDuelStep_Cancel, 0);
}
//-----------------------------------------------------------------------------