#include "..\GameMain.h"
//#include "../InterfaceMember/ChatFrame.h"

// --------------------------------------------------------------------------------------
void CGameMain::Rev_TalkTo( const char* SayerName , int ContentSize ,const char* Content )
{
	// ���
	int a = 0;
	char tmp[MAX_PATH];
	sprintf( tmp, "[%s]:%s", SayerName, Content );
	g_pChatFrame->AddMessage( 0, tmp );
}

// --------------------------------------------------------------------------------------
void CGameMain::Rev_Range( int Sayer ,const char* Name , int ContentSize ,const char* Content )
{
	// �d��
	int a = 0;

	char tmp[MAX_PATH];
	sprintf( tmp, "[%s]:%s", Name, Content );
	g_pChatFrame->AddMessage( 0, tmp );
}

// --------------------------------------------------------------------------------------
void CGameMain::Rev_Zone( int Sayer ,const char* Name , int ContentSize ,const char* Content )
{
	// ��ϰ�,���W�D	
	int a = 0;

	char tmp[MAX_PATH];
	sprintf( tmp, "[%s]:%s", Name, Content );
	g_pChatFrame->AddMessage( 0, tmp );
}

// --------------------------------------------------------------------------------------
void CGameMain::Rev_SystemMsg( int ContentSize ,const char* Content )
{
	// �t��
	int a = 0;

	char tmp[MAX_PATH];
	sprintf( tmp, "[System]:%s", Content );
	g_pChatFrame->AddMessage( 0, tmp );
}