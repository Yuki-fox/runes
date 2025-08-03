#include "..\GameMain.h"
#include "../InterfaceMember/ChatFrame.h"

// --------------------------------------------------------------------------------------
void CGameMain::Rev_TalkTo( const char* SayerName , int ContentSize ,const char* Content )
{
	// 對我
	int a = 0;
	char tmp[MAX_PATH];
	sprintf( tmp, "[%s]:%s", SayerName, Content );
	g_pChatFrame->AddMessage( 0, DEF_COLOR_MAGENTA, tmp );
}

// --------------------------------------------------------------------------------------
void CGameMain::Rev_Range( int Sayer ,const char* Name , int ContentSize ,const char* Content )
{
	// 範圍
	int a = 0;

	char tmp[MAX_PATH];
	sprintf( tmp, "[%s]:%s", Name, Content );
	g_pChatFrame->AddMessage( 0, DEF_COLOR_LIGHT_GRAY, tmp );
}

// --------------------------------------------------------------------------------------
void CGameMain::Rev_Zone( int Sayer ,const char* Name , int ContentSize ,const char* Content )
{
	// 對區域,或頻道	
	int a = 0;

	char tmp[MAX_PATH];
	sprintf( tmp, "[%s]:%s", Name, Content );
	g_pChatFrame->AddMessage( 0, DEF_COLOR_ORANGE, tmp );
}

// --------------------------------------------------------------------------------------
void CGameMain::Rev_SystemMsg( int ContentSize ,const char* Content )
{
	// 系統
	int a = 0;

	char tmp[MAX_PATH];
	sprintf( tmp, "[System]:%s", Content );
	g_pChatFrame->AddMessage( 0, DEF_COLOR_RED, tmp );
}

// --------------------------------------------------------------------------------------
void CGameMain::Rev_GmMsg( const char* Msg )
{
    char tmp[MAX_PATH];
    sprintf( tmp, "[GM]%s", Msg );

    g_pChatFrame->AddMessage( 0, DEF_COLOR_RED, tmp );
}
// --------------------------------------------------------------------------------------