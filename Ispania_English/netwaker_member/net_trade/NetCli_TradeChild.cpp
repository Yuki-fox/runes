#include "../../mainproc/GameMain.h"
#include "../../Interface/WorldFrames/ChatFrame.h"
#include "../../Interface/WorldFrames/TradeFrame.h"
#include "NetCli_TradeChild.h"

ItemFieldStruct		NetCli_TradeChild::s_tradeItem[PLAYER_TRADE_MAX_ITEM];

//-----------------------------------------------------------------------------
bool NetCli_TradeChild::_Init()
{
	if (This)
		return false;

	This = new NetCli_TradeChild;

	memset(s_tradeItem, 0, sizeof(s_tradeItem));

	return NetCli_Trade::_Init();
}

//-----------------------------------------------------------------------------
bool NetCli_TradeChild::_Release()
{
	NetCli_Trade::_Release();
	if (This)
	{
		delete This;
		This = NULL;
	}
	return true;
}

//-----------------------------------------------------------------------------
void NetCli_TradeChild::R_RequestTrade( int TargetID )
{	
	g_pTradeFrame->SetProcess(eTradeState_Request_Receive, TargetID);
}

//-----------------------------------------------------------------------------
void NetCli_TradeChild::R_OpenTrade(char* Name , int TargetID)
{
	// g_pError->AddMessage(0 , 0, "R_OpenTrade Name[%s] TargetID[%d]", Name, TargetID);

	CRoleSprite* sprite = g_pGameMain->FindSpriteByID(TargetID);
	if ( sprite )
	{
		/*
		char tmp[MAX_PATH];
		sprintf(tmp, "[Trade]%s¦P·N»P%s¶i¦æ¥æ©ö", sprite->GetName(), Name);
		g_pChatFrame->AddMessage( 0, tmp );
		*/
		g_pTradeFrame->SetProcess(eTradeState_Traded, g_pTradeFrame->GetTradeTarget());
	}
}

//-----------------------------------------------------------------------------
void NetCli_TradeChild::R_TargetPutItem(int Pos , ItemFieldStruct& Item)
{
	int index = Pos - EM_TRADE_ITEM1;
	if (index >= 0 && index < PLAYER_TRADE_MAX_ITEM)
	{		
		s_tradeItem[index] = Item;
		//g_pTradeFrame->SetTargetAcceptState(false);
	}
}

//-----------------------------------------------------------------------------
void NetCli_TradeChild::R_TargetPutMoney(int Money , int Money_Account )
{
	g_pTradeFrame->SetTradeTargetMoney(Money,Money_Account );
}

//-----------------------------------------------------------------------------
void NetCli_TradeChild::R_OwnerPutMoney( int Money , int Money_Account )
{
	g_pTradeFrame->SetTradeMoney(Money, Money_Account);
}
//-----------------------------------------------------------------------------
void NetCli_TradeChild::R_ClearTradeItem( )
{
	int count = 0;
	for ( int i = 0; i < _MAX_BODY_COUNT_; i++ )
	{
		switch ( CNetGlobal::RoleData()->PlayerBaseData->Body.Item[i].Pos )
		{
		case EM_TRADE_ITEM1:
		case EM_TRADE_ITEM2:
		case EM_TRADE_ITEM3:
		case EM_TRADE_ITEM4:
		case EM_TRADE_ITEM5:
		case EM_TRADE_ITEM6:
		case EM_TRADE_ITEM7:
		case EM_TRADE_ITEM8:
		case EM_TRADE_ITEM9:
		case EM_TRADE_ITEM10:
		case EM_TRADE_ITEM11:
		case EM_TRADE_ITEM12:
		case EM_TRADE_ITEM13:
		case EM_TRADE_ITEM14:
		case EM_TRADE_ITEM15:
		case EM_TRADE_ITEM16:
		case EM_TRADE_ITEM17:
		case EM_TRADE_ITEM18:
		case EM_TRADE_ITEM19:
		case EM_TRADE_ITEM20:
		case EM_TRADE_ITEM21:
		case EM_TRADE_ITEM22:
		case EM_TRADE_ITEM23:
		case EM_TRADE_ITEM24:
		case EM_TRADE_ITEM25:
			CNetGlobal::RoleData()->PlayerBaseData->Body.Item[i].Init();
			count++;
			break;
		}
	}

	if ( count > 0 )
	{
		CNetGlobal::RoleData()->TempData.UpdateInfo.Body = true;
	}
}

//-----------------------------------------------------------------------------
void NetCli_TradeChild::R_TradeItem_OK( )
{
	// g_pError->AddMessage(0 , 0, "R_TradeItem_OK");

	// ¥æ©ö¦¨
	g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_TRADE_SUCCESS"));
	g_pTradeFrame->TradeClose();
	//g_pTradeFrame->SendWorldEvent("TRADE_CLOSED");
}

//-----------------------------------------------------------------------------
void NetCli_TradeChild::R_TradeItem_Failed( )
{
	// g_pError->AddMessage(0 , 0, "R_TradeItem_Failed");

	// ¥æ©ö¨ú®ø
	g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_TRADE_CANCEL"));
	g_pTradeFrame->TradeClose();
	//g_pTradeFrame->SendWorldEvent("TRADE_CLOSED");
}

//-----------------------------------------------------------------------------
void NetCli_TradeChild::R_PutItem_OK( )
{
	int a = 0;
}

//-----------------------------------------------------------------------------
void NetCli_TradeChild::R_PutItem_Failed( )
{
	int a = 0;
}

//-----------------------------------------------------------------------------
void NetCli_TradeChild::ClearItem()
{
	memset(s_tradeItem, 0, sizeof(s_tradeItem));
}

//-----------------------------------------------------------------------------
ItemFieldStruct* NetCli_TradeChild::GetTradeItem(int index)
{
	if (index >= 0 && index < PLAYER_TRADE_MAX_ITEM)
		return &s_tradeItem[index];
	return NULL;
}

//-----------------------------------------------------------------------------
void NetCli_TradeChild::R_RequestTradeError( )
{
	g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_TRADE_REQUEST_ERROR"));
}

//-----------------------------------------------------------------------------
void NetCli_TradeChild::R_DisagreeTrade( int ClientResult )
{
	switch (ClientResult)
	{
	// ©Ú“F¥æ©ö
	case 0:
		g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_TRADE_TARGET_DISAGREE"));
		break;

	// ¹ï¤è¦£¿ý¤¤
	case 1:
		g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_TRADE_TARGET_BUSY"));
		break;	
	}
	g_pTradeFrame->SetProcess(eTradeState_None, 0);
}

//-----------------------------------------------------------------------------
void NetCli_TradeChild::R_StopTrade			 ( int TargetID )
{
	// ¥æ©ö¨ú®ø
	g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_TRADE_CANCEL"));
	g_pTradeFrame->TradeClose();
	// g_pError->AddMessage(0 , 0, "R_StopTrade TargetID[%d]", TargetID);
}

//-----------------------------------------------------------------------------
void NetCli_TradeChild::R_TradeState ( bool TradeOK , bool FinalOK , bool TargetTradeOK , bool TargetFinalOK )
{
	int player = eTradeAcceptState_None;
	int target = eTradeAcceptState_None;

	if ( FinalOK )
		player = eTradeAcceptState_Trade;
	else if ( TradeOK )
		player = eTradeAcceptState_Ready;
	if ( TargetFinalOK )
		target = eTradeAcceptState_Trade;
	else if ( TargetTradeOK )
		target = eTradeAcceptState_Ready;

	// g_pError->AddMessage(0 , 0, "R_TradeState TradeOK[%d] FinalOK[%d] TargetTradeOK[%d] TargetFinalOK[%d]", TradeOK, FinalOK, TargetTradeOK, TargetFinalOK);
	g_pTradeFrame->TradeAcceptState(player, target);
}
//-----------------------------------------------------------------------------
void NetCli_TradeChild::R_CancelTardeRequest	 ( int TargetID )
{
	// g_pError->AddMessage(0 , 0, "CancelTardeRequest %d", TargetID);

	// ¹ï¤è¨ú®ø¥æ©öÁÜ½Ð
	if ( g_pTradeFrame->GetTradeTarget() == TargetID )
	{
		g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_TRADE_UNINVITE"));
		g_pTradeFrame->SetProcess(eTradeState_None, 0);
		g_pTradeFrame->SendWorldEvent("TRADE_REQUEST_CANCEL");
	}
	else
		g_pChatFrame->SendMsgEvent(CHAT_MSG_SYSTEM, "[R_CancelTardeRequest] ID ¿ù»~!!", "");
}
//-----------------------------------------------------------------------------
void NetCli_TradeChild::R_FinalOKFailed		 ( FinalOKFailedENUM Result )
{
	switch (Result)
	{
	case EM_FinalOKFailed_PasswordErr:
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_GAMEMSGEVENT_014"));
		break;

	case EM_FinalOKFailed_Unknow:
		break;
	}
}