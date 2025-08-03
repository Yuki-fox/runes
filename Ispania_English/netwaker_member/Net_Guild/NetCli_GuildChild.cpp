#include "../../mainproc/GameMain.h"
#include "NetCli_GuildChild.h"
#include "../net_other/NetCli_OtherChild.h"
#include "../interface/debug/DebugMessage.h"
#include "../interface/WorldFrames/ChatFrame.h"
#include "../Interface/WorldFrames/GuildFrame.h"
#include "../Interface/GuildHouses/GuildHousesFrame.h"







bool								NetCli_GuildChild::m_bGuildBoardRequesting = false;

int									NetCli_GuildChild::m_iBiddingCrystalID		=0;
int									NetCli_GuildChild::m_iBiddingScore			=0;

int									NetCli_GuildChild::m_iGuildFightInfoStep	=0;

vector<GuildDeclareWarInfo>			NetCli_GuildChild::s_declareWarInfo;


//-----------------------------------------------------------------------------
bool NetCli_GuildChild::_Init()
{
	
	if (This)
		return false;

	
	This = new NetCli_GuildChild;

	return NetCli_Guild::_Init();
}

//-----------------------------------------------------------------------------
bool NetCli_GuildChild::_Release()
{
	NetCli_Guild::_Release();
	if (This)
	{
		delete This;
		This = NULL;
	}
	return true;

}
//-----------------------------------------------------------------------------
void NetCli_GuildChild::RL_CreateGuildResult( int GuildID , char* GuildName , CreateGuildResultENUM Result )
{
	char tmp[MAX_PATH];
	sprintf(tmp, "�إ߳s�p���|(%s=%d) %d",GuildName,GuildID, Result );
	g_pGuildFrame->CreateGuildResult(GuildID,GuildName,Result);
	g_pDebugMessage->DebugOutput( 2 , tmp);

}
//-----------------------------------------------------------------------------
void NetCli_GuildChild::RL_NewGuildInvite( int GuildID, char* LeaderName, int LeaderDBID )
{
	char tmp[MAX_PATH];
	sprintf(tmp, "�ܽХ[�J�s�p���|(%d) �n�ФH(%s) DBID(%d)", GuildID, LeaderName, LeaderDBID);
	g_pGuildFrame->NewGuildInvite(GuildID,LeaderName,LeaderDBID);
	g_pDebugMessage->DebugOutput( 2 , tmp);
}
//-----------------------------------------------------------------------------
void NetCli_GuildChild::RL_NewGuildInviteResult	( char* PlayerName , NewGuildInviteResultENUM	Result )
{
	char tmp[MAX_PATH]="";
	const char * pStr;

	pStr=g_ObjectData->GetString("GUILD_MSG_MEMBERFULLORFAIL");
	sprintf(tmp, pStr, PlayerName);
	switch( Result )
	{
	case EM_NewGuildInviteResult_OK			:	//�n�Ц�
		pStr=g_ObjectData->GetString("GUILD_MSG_ADDMEMBER");
			
		sprintf(tmp, pStr, PlayerName);
	//	g_pChatFrame->SendMsgEvent("CHAT_MSG_SYSTEM", tmp, "");

		// ���ƭn�D���,�Ϊ̥u�[�J�浧���a���
		break;

	case EM_NewGuildInviteResult_Failed:
		pStr=g_ObjectData->GetString("GUILD_MSG_MEMBERFULLORFAIL");
		sprintf(tmp, pStr, PlayerName);
		break;

	case EM_NewGuildInviteResult_NotFind	:	//�䤣�� �Q�ܽЪ�
		pStr=g_ObjectData->GetString("GUILD_MSG_NOTFIND");
		sprintf(tmp, pStr, PlayerName);
		
		//sprintf(tmp, "%s �[�J�s�p���� (�䤣�� �Q�ܽЪ�)", PlayerName );
		//g_pDebugMessage->DebugOutput( 2 , ");
	
		break;

	case EM_NewGuildInviteResult_HasGuild	:	//�Q�ܽЪ� �w�g�����|

		pStr=g_ObjectData->GetString("GUILD_MSG_PLAYERHAVEGUILD");
		sprintf(tmp, pStr, PlayerName);
		//sprintf(tmp,  "%s �[�J�s�p���� (�Q�ܽЪ� �w�g�����|)", PlayerName);
	//	g_pDebugMessage->DebugOutput( 2 , "%s �[�J�s�p���� (�Q�ܽЪ� �w�g�����|)", PlayerName );
		break;

	case EM_NewGuildInviteResult_Disagree	:	//�Q�ܽЪ� ���P�N
		pStr=g_ObjectData->GetString("GUILD_MSG_PLAYER_SAYNO");
		sprintf(tmp, pStr, PlayerName);
		
	//	sprintf(tmp,  "%s �[�J�s�p���� (�Q�ܽЪ� ���P�N)", PlayerName );
	//	g_pDebugMessage->DebugOutput( 2 , "%s �[�J�s�p���� (�Q�ܽЪ� ���P�N)", PlayerName );
		break;
	}
	g_pChatFrame->SendMsgEvent("CHAT_MSG_SYSTEM", tmp, "");
}

void NetCli_GuildChild::RL_JoindNewGuild			( int GuildID , char* LeaderName )
{
	g_pGuildFrame->JoinNewGuild(GuildID,LeaderName);
	const char * pStr;
	pStr=GetGuildNameByID(GuildID);

	g_pDebugMessage->DebugOutput( 2 , "�[�J%s ��%s�s�p���|" ,LeaderName, pStr );
}

void NetCli_GuildChild::RL_LeaveNewGuildResult( char* KickPlayerName , bool Result )
{
	g_pGuildFrame->LeaveNewGuildResult(KickPlayerName,Result);

	if( Result )
	{	
		g_pDebugMessage->DebugOutput( 2 , "�n�D %s ���}�s�p���| " , KickPlayerName );
	}
	else
	{
		g_pDebugMessage->DebugOutput( 2 ,"�n�D %s ���}�s�p���| ����" , KickPlayerName );
	}

}
//�|���M��
void NetCli_GuildChild::RL_DelNewGuildResult		( bool Result )
{
	if( Result )
	{
		_RoleData.BaseData.GuildID = 0;
		g_pDebugMessage->DebugOutput( 2 , "�R���s�p���| " );
	}
	else
	{
		g_pDebugMessage->DebugOutput( 2 , "�R���s�p���| ����" );
	}
}

//////////////////////////////////////////////////////////////////////////
void NetCli_GuildChild::RL_GuildInvite		( int GuildID , char* LeaderName , int LeaderDBID )
{
	g_pDebugMessage->DebugOutput( 2 , "�ܽХ[�J���|(%s) �n�ФH(%s) DBID(%d)" ,GetGuildNameByID( GuildID) , LeaderName , LeaderDBID);

	//g_pGuildFrame->PetitionSignature(GuildID, LeaderName, LeaderDBID);
	g_pGuildFrame->GuildInviteRequest(GuildID, LeaderName, LeaderDBID);
}
void NetCli_GuildChild::RL_GuildInviteResult( char* PlayerName , GuildInviteResultENUM	Result )
{
	g_pGuildFrame->GuildInviteResult(PlayerName,Result);
	g_pDebugMessage->DebugOutput( 2 , "�ܽ� %s Result=%d" ,PlayerName, Result );
}
void NetCli_GuildChild::RL_JoindGuild		( int GuildID , char* LeaderName )
{
	g_pGuildFrame->JoinGuild(GuildID,LeaderName);
	const char * pStr;
	pStr=GetGuildNameByID(GuildID);

	g_pDebugMessage->DebugOutput( 2 , "�[�J%s ��%s���|" ,LeaderName, pStr );
	
}
void NetCli_GuildChild::RL_LeaveGuildResult	( char* KickPlayerName , bool Result )
{
	g_pGuildFrame->LeaveGuildResult(KickPlayerName,Result);

	if( Result )
	{	
		g_pDebugMessage->DebugOutput( 2 , "�n�D %s ���}���| " , KickPlayerName );
	}
	else
	{
		g_pDebugMessage->DebugOutput( 2 ,"�n�D %s ���}���| ����" , KickPlayerName );
	}
	
}


void NetCli_GuildChild::RL_DelGuildResult	( bool Result )
{
	if( Result )
	{
		g_pDebugMessage->DebugOutput( 2 , "�R�����| " );
	}
	else
	{
		g_pDebugMessage->DebugOutput( 2 , "�R�����| ����" );
	}
}
/////////////////////////////////////////////////////////////////////////
//--�����줽�|��� �i��O�d�ߨ�L�u�|�� �άO�ۤv�����|
void NetCli_GuildChild::RL_GuildInfo(GuildBaseStruct& Guild , int MemberCount )
{
	g_pGuildFrame->GuildInfo(Guild,MemberCount,m_bGuildBoardRequesting);
	m_bGuildBoardRequesting=false; 	
	g_pDebugMessage->DebugOutput( 2 , "���줽�|(%s %d)��� " , Guild.GuildName.Begin() , Guild.GuildID );
}
void NetCli_GuildChild::RD_ModifySimpleGuild( SimpleGuildInfoStruct& SimpleGuildInfo )
{

	g_pGuildFrame->ModifySimpleGuild(SimpleGuildInfo);
}
void NetCli_GuildChild::RL_GuildMemberBaseInfo	( vector< GuildMemberBaseInfoStruct >& MemberList )
{
	g_pGuildFrame->GuildMemberBaseInfo(MemberList);
	
	g_pDebugMessage->DebugOutput( 2 , "���줽�|�����C��" );
	
}
void NetCli_GuildChild::RL_GuildMemberInfo		( GuildMemberStruct& Member )
{
	g_pGuildFrame->GuildMemberInfo(Member);
	
	g_pDebugMessage->DebugOutput( 2 , "���줽�|���� (%s) ��� " , Member.MemberName.Begin() );
}
void NetCli_GuildChild::RD_GuildCreate			( int GuildID )
{
	
	if (g_pGuildFrame)
		g_pGuildFrame->GuildCreate(GuildID);
	//ReloadGuildDate();

	char temp[MAX_PATH];
	const char *pGuildName=GetGuildNameByID(GuildID);

	sprintf(temp," %s�����������|", pGuildName);
	g_pDebugMessage->DebugOutput( 2 , temp  );
}
//////////////////////////////////////////////////////////////////////////
void NetCli_GuildChild::RD_BoardPostResult			( bool Result )
{
		g_pGuildFrame->RecvBoardPost(Result);
	if( Result )
		g_pDebugMessage->DebugOutput( 2 , "Post�峹"  );
	else
		g_pDebugMessage->DebugOutput( 2 , "Post�峹����"  );
}
void NetCli_GuildChild::RD_BoardListCount			( int MaxCount , int Count , int PageID )
{
	if (g_pGuildFrame)
		g_pGuildFrame->SetBBSPageCount(MaxCount,Count,PageID);

	g_pDebugMessage->DebugOutput( 2 , "BoardList MaxCount=%d Count=%d PageID=%d" , MaxCount , Count , PageID  );
}
void NetCli_GuildChild::RD_BoardListData			( int MessageGUID , int Index , char* PlayerName , int Time , char* Title , GuildBoardModeStruct Type )
{
	if (g_pGuildFrame)
		g_pGuildFrame->SetBBSPostDate(MessageGUID,Index,PlayerName,Time,Title,Type);

	g_pDebugMessage->DebugOutput( 2 , "BoardData Index=%d Title=%s" , Index , Title );
}
void NetCli_GuildChild::RD_BoardMessageResult		( GuildBoardStruct&	Message )
{
	if (g_pGuildFrame)
		g_pGuildFrame->SetBoardMessage(Message);

	g_pDebugMessage->DebugOutput( 2 , "BoardMessage Title=%s" , Message.Title.Begin() );
}
void NetCli_GuildChild::RD_BoardModifyMessageResult( bool Result )
{
	if( Result )
		g_pDebugMessage->DebugOutput( 2 , "�ק�峹"  );
	else
		g_pDebugMessage->DebugOutput( 2 , "�ק�峹����"  );

}
void NetCli_GuildChild::RD_BoardModifyModeResult	( bool Result )
{
	if( Result )
		g_pDebugMessage->DebugOutput( 2 , "�ק�峹�ݩ�"  );
	else
		g_pDebugMessage->DebugOutput( 2 , "�ק�峹�ݩʥ���"  );
}

void NetCli_GuildChild::RL_OpenGuildContribution		( int TargetNPCID)
{
	g_pGuildFrame->OpenGuildContribution(TargetNPCID);
	g_pDebugMessage->DebugOutput( 2 , "�}�_���|�^�m����"  );
}
void NetCli_GuildChild::RL_GuildContributionItemResult	( bool Result )
{


	g_pGuildFrame->GuildContributionItemResult(Result);
	
	g_pDebugMessage->DebugOutput( 2 , "�]�w�^�m���~�����G"  );
}
void NetCli_GuildChild::RL_GuildContributionClose		( )
{
	g_pGuildFrame->CloseGuildContribution();
	g_pDebugMessage->DebugOutput( 2 , "�n�D�����^�m����"  );
}
void NetCli_GuildChild::RL_OpenCreateGuild			( int TargetNPCID)
{
	g_pGuildFrame->OpenGuildCommand(TargetNPCID);
	g_pDebugMessage->DebugOutput( 2 , "�n�D�}�ҫإߤ��|����"  );
}
void NetCli_GuildChild::RL_CloseCreateGuild			( )
{
	g_pGuildFrame->CloseGuildCommand();
	g_pDebugMessage->DebugOutput( 2 , "�n�D�����إߤ��|����"  );
}

void NetCli_GuildChild::RL_AllGuildMemberCountResult( int Size , GuildMemberCountStruct MemberList[ DEF_MAX_PG_GUILD_MEMBER_COUNT_ ] )
{


}


// ���a�[�J�ۤv�����|
void NetCli_GuildChild::RD_JoinGuildMember			( GuildMemberStruct& Member )
{
	g_pGuildFrame->JoinGuildMember(Member);

	char Buf[256];
	sprintf( Buf , "�[�J�s���|���� %s" , Member.MemberName.Begin());	
	g_pDebugMessage->DebugOutput( 2 , Buf  );
}
// ���a���}�ۤv�����|
void NetCli_GuildChild::RD_LeaveGuildMember		( int LeaderDBID , int MemberDBID )
{

	g_pGuildFrame->LeaveGuildMember(LeaderDBID,MemberDBID);

	char Buf[256];

	sprintf( Buf , "���|�������}���| %d" , MemberDBID	);

	g_pDebugMessage->DebugOutput( 2 , Buf  );
}

/*
void NetCli_GuildChild::RL_GuildShopOpen( int TargetNPCID , int Score , StaticFlag<256>& Flag )
{
	g_pGuildFrame->OpenGuildShop(TargetNPCID,Score,Flag);

	g_pDebugMessage->DebugOutput( 2 , "�}�Ҥ��|�ө�" );
}
void NetCli_GuildChild::RD_GuildShopBuyResult( int FlagID , bool Result )
{
		g_pGuildFrame->GuildShopBuyResult(FlagID,Result);

		char Buf[256];

		///if ()
	g_pDebugMessage->DebugOutput( 2 , Buf );
}
void NetCli_GuildChild::RL_GuildShopClose( )
{

	g_pGuildFrame->CloseGuildShop();
	g_pDebugMessage->DebugOutput( 2 , "�n�D�������|�ө�" );
}
*/

//////////////////////////////////////////////////////////////////////////
//�ήɸ�T
//////////////////////////////////////////////////////////////////////////
//���|�C���T
void NetCli_GuildChild::RL_GuildBaseInfo			( int GuildID , char* GuildName , GuildFlagStruct& Flag , bool IsReady )
{
	g_pGuildFrame->GuildBaseInfo( GuildID, GuildName, Flag, IsReady );
	g_pDebugMessage->DebugOutput( 2 , "���|�C���T%d=%s %d",GuildID,GuildName,IsReady );

}
//���|�R��
void NetCli_GuildChild::RL_GuildDestroy	( int GuildID  )
{
	//���|�˷|�F
	const char *GuildName=NULL;
	char temp[1024];
	if (g_pGuildFrame)
	{
		GuildName=g_pGuildFrame->GetGuildNameByID(GuildID);
		sprintf(temp, "���|�R��%s", GuildName);
		if (GuildName)
			g_pGuildFrame->GuildDestroy(GuildID);
	}

	g_pDebugMessage->DebugOutput( 2 ,temp);
}

//���|�����W�U�u
void NetCli_GuildChild::RD_GuildPlayerOnline		( int PlayerDBID , bool IsOnline )
{

	g_pGuildFrame->GuildPlayerOnline(PlayerDBID,IsOnline);


	
	g_pDebugMessage->DebugOutput( 2 , "RD_GuildPlayerOnline PlayerDBID=%d IsOnline=%d" , PlayerDBID , IsOnline );
}
//------------------------------------------------------------------------------
const char * NetCli_GuildChild::GetGuildNameByID(int guildID)
{

	return  g_pGuildFrame->GetGuildNameByID(guildID);
}

//------------------------------------------------------------------------------

void NetCli_GuildChild::GuildBoardRequest( int guildID )
{
	m_bGuildBoardRequesting=true;
	g_pGuildFrame->GuildBoardRequest(guildID,m_bGuildBoardRequesting);
	g_pDebugMessage->DebugOutput( 2 , "GuildBoardRequest Guild=%d" , guildID );
}




void NetCli_GuildChild::RL_GuildFightAuctionOpen( int TargetNPCID , int CrystalID , int OwnerGuildID , int BitGuildID , int BitScore )
{

}
//------------------------------------------------------------------------------
void NetCli_GuildChild::RL_GuildFightAuctionClose( )
{

}
//------------------------------------------------------------------------------
void NetCli_GuildChild::RD_GuildFightAuctionBidResult( int CrystalID , int OwnerGuildID , int BitGuildID, bool Result )
{

}
//------------------------------------------------------------------------------
void NetCli_GuildChild::RD_GuildFightAuctionBidNewOne( int CrystalID , int OwnerGuildID , int BitGuildID )
{

}
//------------------------------------------------------------------------------
void NetCli_GuildChild::RD_GuildLeaderChange		  ( int NewLeaderDBID , int OldLeaderDBID )
{

	g_pGuildFrame->LeaderChangeMsg(NewLeaderDBID,OldLeaderDBID);
	g_pDebugMessage->DebugOutput( 2 , "�󴫷|��" );	
}
//------------------------------------------------------------------------------
void NetCli_GuildChild::RD_GuildMemberRankChange		( int MemberDBID , int Rank )
{
	g_pGuildFrame->GuildMemberRankChange	(MemberDBID,Rank);
	g_pDebugMessage->DebugOutput( 2 , "��令��Rank" );	
}
//------------------------------------------------------------------------------
void NetCli_GuildChild::RL_SetGuildReady		( bool Result )
{
	g_pGuildFrame->GuildReadyResult	(Result);
	char Buf[256];
	sprintf( Buf , "�s�p���|���G %s " , Result?"ture":"false"	);
	g_pDebugMessage->DebugOutput( 2 , Buf  );	

}
//------------------------------------------------------------------------------
void NetCli_GuildChild::RD_GuildFightInfo( EM_GuildFightInfo_Result emResult, int iCrystalID, int iDefenderGuildID, int iAttackerGuildID, int iHighestAmount, int iRemainTime, int iFightTime, int iOtherCrystalID, int iGuildPoint )
{
	// iDefenderGuildID		�ثe�ðê̤��|, �� 0 ��Ĥ@���v��
	// iAttackerGuildID		�ثe�X���̰������|, �γ̫�@���X�������|
	// iRemainTime			== 0 �ɪ�w�W�L�ɶ�.
	// iFightTime			���g�԰��ɶ� 60s + 60m * 60 + 24 * 60m + 7 
	//SYSTEMTIME st;
	//GetSystemTime( &st );              // gets current time
	//int iTimeNowVal			= st.wSecond + ( st.wMinute * 60 ) + ( st.wHour * 3600 ) + ( st.wDayOfWeek * 86400 );
	
	//	int					iResult;			//  �d�ߵ��G 0 ���`, 1 �d�ߪ̥����w�g�U�Ш䥦�����F, 2 �d�ߪ̥����w�g�֦��䥦�����F
	// �n�Ҽ{�����p
	// �W�L�U�Юɶ�, ��ܥثe�ðê� ( ����ت��p, ���ðê� �εL�ðê� ), �D�Ԫ�, ��ɾ԰�
	// �b�ɶ���, �X�{�U�пﶵ
	// �b�ɶ���, �����|�n������
	// �U�Ъ����| 1 �d�ߪ̥����w�g�U�Ш䥦�����F, 2 �d�ߪ̥����w�g�֦��䥦�����F

	m_iBiddingCrystalID = iCrystalID;
	m_iBiddingScore		= iHighestAmount;

	if( iRemainTime == 0  )
	{
		// �w�g�L�F�v�Юɶ�
		m_iGuildFightInfoStep = 1;

		vector<string> vecOption;		
		vecOption.push_back( "���}" );
		
		NetCli_Other::This->R_OpenDialog( EM_LuaDialogType_Select, "DAMNTITLE", "TIMEOUT", vecOption, GuildFightBidDialogCallBack, NULL );
	}
	else
	if( iAttackerGuildID == _RoleData.BaseData.GuildID && iAttackerGuildID != 0 )
	{
		// �A�����|�ثe���M���̰���
		m_iGuildFightInfoStep = 2;

		vector<string> vecOption;		
		vecOption.push_back( "���}" );
		NetCli_Other::This->R_OpenDialog( EM_LuaDialogType_Select, "DAMNTITLE", "HIGH", vecOption, GuildFightBidDialogCallBack, NULL );
	}
	else
	//if( iAttackerGuildID != _RoleData.BaseData.GuildID && )
	{
		// �A�����|�i�H�X���ѥ[�v��
		m_iGuildFightInfoStep = 3;

		vector<string> vecOption;		
		vecOption.push_back( "�U��" );
		vecOption.push_back( "���}" );
		NetCli_Other::This->R_OpenDialog( EM_LuaDialogType_Select, "DAMNTITLE", "HIGH", vecOption, GuildFightBidDialogCallBack, NULL  );
		//NetCli_Other::R_DialogSetTitle( )
	}

	//if( )
	//NetCli_Other::This->->R_OpenDialog( EM_LuaDialogType_Select, DialogContent, DialogSelectStr );





}
//------------------------------------------------------------------------------
void NetCli_GuildChild::GuildFightBidDialogCallBack( int iID, PVOID pContext )
{

	switch( m_iGuildFightInfoStep )
	{
	case 1:
	case 2:
		{
			NetCli_Other::This->R_CloseDialog();
		} break;
	case 3:
		{
			if( iID == 0 ) // �U��
			{
				SL_GuildFightAuctionBid( (int)( m_iBiddingScore * 1.5 ), m_iBiddingCrystalID  );
			}

			NetCli_Other::This->R_CloseDialog();
		} break;

	}
}
//------------------------------------------------------------------------------
void NetCli_GuildChild::RD_GuildFightAuctionEnd		( int iCrystalID, int iDefenderGuildID, int iAttackerGuildID, int iFightTime, int iEndAmount )
{
	// ���|���v�Фw�g����, �o�ʥ]�|�P�ɳq�� iDefenderGuildID and iAttackerGuildID
	char szMsg[256];
	char szCrystalID[256];
	char szCrystalName[256];
	const char* DefenderGuildName = GetGuildNameByID( iDefenderGuildID );
	const char* AttackerGuildName = GetGuildNameByID( iAttackerGuildID );

	sprintf( szCrystalID, "CRYSTAL_NAME_%02d", iCrystalID );
	sprintf( szCrystalName, g_ObjectData->GetString( szCrystalID ) );
	sprintf( szMsg, g_ObjectData->GetString( "GUILD_MSG_GUILDFIGHT_AUCTIONEND" ), szCrystalName, AttackerGuildName, DefenderGuildName );

	g_pChatFrame->SendMsgEvent("CHAT_MSG_SYSTEM", szMsg, "" );
}
//------------------------------------------------------------------------------
void NetCli_GuildChild::RD_GuildFightBeginNotify		( int iCrystalID, int iRemainTime )
{
	// ���|�ԧY�N�󤭤������}�l, �o�ʥ]�|�P�ɳq�� iDefenderGuildID and iAttackerGuildID, iRemainTime �O���T�����
	if ( iRemainTime > 60 )
		g_pChatFrame->SendMsgEvent("CHAT_MSG_SYSTEM", g_ObjectData->GetString( "GUILD_MSG_GUILDFIGHT_BEGINNOTIFY" ), "" );
	else
		g_pChatFrame->SendMsgEvent("CHAT_MSG_SYSTEM", g_ObjectData->GetString( "GUILD_MSG_GUILDFIGHT_BEGINNOTIFY_1" ), "" );
}
//------------------------------------------------------------------------------
void NetCli_GuildChild::RL_GuildFightLeaderInvite( PG_Guild_LtoC_GuildFightLeaderInvite* pPacket )
{
	// ����t��
	g_pChatFrame->SendMsgEvent("CHAT_MSG_SYSTEM", "[TEMP]���줽�|�Է|���}�l�ܽгq��" , "" );
}

void NetCli_GuildChild::RL_GuildFightInviteNotify( PG_Guild_LtoC_GuildFightInviteNotify* pPacket )
{
	g_pChatFrame->SendMsgEvent("CHAT_MSG_SYSTEM", "[TEMP]�����ܽгq��" , "" );
	SL_GuildFightInviteRespond( pPacket->iSrvID, pPacket->iRoomID, pPacket->iCrystalID, _RoleData.BaseData.GuildID, _RoleData.Base.DBID, true );
}
//�Y�H���|����
void NetCli_GuildChild::RL_GuildIDChange				( int GItemID , int GuildID )
{
	CRoleSprite* sprite = g_pGameMain->FindSpriteByID( GItemID & 0xffffff);
	if ( sprite ) {
		g_pGameMain->SetSpriteGuildID(sprite, GuildID);
	}	

	/*
	CRoleSprite* player = g_pGameMain->GetPlayer();	
	if ( sprite ) {
		sprite->SetGuildID(GuildID);		
		sprite->SetTitleText(NetCli_GuildChild::GetGuildNameByID(GuildID));

		// PK State �P����,�P���|���ब��
		int PKState = g_pGameMain->GetPlayerKillState();
		if ( PKState == EM_PK_Party_Guild && player ) {
			if ( player == sprite ) {
				// �ۤv�[�J���s�]�w�d�򤺪��a
				g_pGameMain->SetPlayerKillState(PKState, false);
			} else if ( GuildID > 0 && sprite->GetGuildID() == player->GetGuildID() ) {
				// �P���|
				sprite->SetEnemyState(false);
				sprite->SetCanAttackState(false);
			} else {
				// ���P���|
				sprite->SetEnemyState(true);
				sprite->SetCanAttackState(true);
			}
		}
	}
	*/
}
static int  GetNowTime()
{
	time_t NowTime;
	time( &NowTime );
	NowTime+=5*60;
	return (int)NowTime;

}
//////////////////////////////////////////////////////////////////////////
//���|�ۥ�PK�ž�
void NetCli_GuildChild::RL_DeclareWarResult		( DeclareWarResultENUM Result , int Type , int TargetGuildID )
{
	const char* guildName = GetGuildNameByID(TargetGuildID);
	int time, money, score=0;

	g_ObjectData->GetGuildWarDeclareTypeInfo(Type, time, money );

	switch (Result)
	{
	case EM_DeclareWarResult_OK:
		{
			CRoleSprite* player = g_pGameMain->GetPlayer();
			int BeginTime=GetNowTime();


			char timeStr[256];

			int guildID = player->GetGuildID() & 0xffffff;
			GuildDeclareWarInfo info;
			info.bothID[0] = guildID;
			info.bothID[1] =TargetGuildID & 0xffffff;
			info.startTime = BeginTime;
			info.time =-1;// 35*60;//(float)Time;
			s_declareWarInfo.push_back(info);

			g_pInterface->SendWorldEvent("GUILDWAR_DECLARELIST_UPDATE");

			g_ObjectData->SecondsToTimeAbbrev(time * 60.0f, timeStr);

			g_pChatFrame->SendChatMessageEx("CHAT_MSG_SYSTEM", "", g_ObjectData->GetString("SYS_GUILDDECLARE_SUCCESS"), guildName, timeStr);
		}
		break;

	case EM_DeclareWarResult_DataErr:			//��ƿ��~	
	case EM_DeclareWarResult_OnWarErr:			//�žԤ�
		g_pChatFrame->SendChatMessage("CHAT_MSG_SYSTEM", "", g_ObjectData->GetString("SYS_GUILDDECLARE_DATA_ERROR"));
		break;
	
	case EM_DeclareWarResult_OnWarPrepareErr:	//�žԷǳƤ�
		g_pChatFrame->SendChatMessage("CHAT_MSG_SYSTEM", "", g_ObjectData->GetString("SYS_GUILDDECLARE_PREPAREERR"));
		break;

	case EM_DeclareWarResult_MonyeErr:			//��������
		g_pChatFrame->SendChatMessage("CHAT_MSG_SYSTEM", "", g_ObjectData->GetString("SYS_GUILDWAR_NOT_ENOUGH_MONEY"));
		break;

	case EM_DeclareWarResult_LeaderErr:			//���O�|��
		g_pChatFrame->SendChatMessage("CHAT_MSG_SYSTEM", "", g_ObjectData->GetString("SYS_GUILDWAR_NOT_LEADER"));
		break;
		
	case EM_DeclareWarResult_ScoreErr:			//���|��������
		g_pChatFrame->SendChatMessage("CHAT_MSG_SYSTEM", "", g_ObjectData->GetString("SYS_GUILDWAR_NOT_ENOUGH_POINT"));
		break;

	case EM_DeclareWarResult_TimeErr:			//�žԹL��
		g_pChatFrame->SendChatMessageEx("CHAT_MSG_SYSTEM", "", g_ObjectData->GetString("SYS_GUILDDECLARE_TIME_ERROR"));
		break;

	case EM_DeclareWarResult_GuildLvErr:		// ���|���Ť���
		g_pChatFrame->SendChatMessageEx("CHAT_MSG_SYSTEM", "", g_ObjectData->GetString("SYS_GUILDDECLARE_LEVEL_ERROR"));
		break;

	case EM_DeclareWarResult_TargetGuildErr:	//�ؼФ��|���s�b
		g_pChatFrame->SendChatMessageEx("CHAT_MSG_SYSTEM", "", g_ObjectData->GetString("SYS_GUILDDECLARE_TARGET_NOT_EXIST"));
		break;

	case EM_DeclareWarResult_TargetGuildLvErr:	//�ؼФ��|���Ť���
		g_pChatFrame->SendChatMessageEx("CHAT_MSG_SYSTEM", "", g_ObjectData->GetString("SYS_GUILDDECLARE_TARGET_LEVEL_ERROR"));
		break;
	}
}

void NetCli_GuildChild::RD_PrepareWar					( int GuildID , int Time )
{
	char timeStr[256];
	g_ObjectData->SecondsToTimeAbbrev((float)Time, timeStr);

	const char* guildName = GetGuildNameByID(GuildID & 0xffffff);
	g_pChatFrame->SendChatMessageEx("CHAT_MSG_SYSTEM", "", g_ObjectData->GetString("SYS_GUILDWAR_DECLARE_READY"), guildName, timeStr);
}

void NetCli_GuildChild::RL_WarInfo				( bool IsDeclareWar , int GuildID[2] , int Time , int BeginTime )
{
	GuildDeclareWarInfo info;
	info.bothID[0] = GuildID[0] & 0xffffff;
	info.bothID[1] = GuildID[1] & 0xffffff;
	info.startTime = BeginTime;
	info.time = (float)Time;

	CRoleSprite* player = g_pGameMain->GetPlayer();

	bool flash = false;
	// ���߹��
	if ( IsDeclareWar )
	{
		for ( vector<GuildDeclareWarInfo>::iterator iter = s_declareWarInfo.begin(); iter != s_declareWarInfo.end(); iter++ )
		{
			if ( memcmp(iter->bothID, info.bothID, sizeof(int)*2) == 0 )
			{
				iter->time = (float)Time;
				g_pInterface->SendWorldEvent("GUILDWAR_DECLARELIST_UPDATE");
				return ;
			}
		}

		// �W�[
		s_declareWarInfo.push_back(info);

		// ��T
		if ( player )
		{
			int guildID = player->GetGuildID();
			if ( GuildID[0] == guildID || GuildID[1] == guildID )
			{
				const char* guildName;
				if ( GuildID[0] == guildID )
					guildName = GetGuildNameByID(GuildID[1]);
				else
					guildName = GetGuildNameByID(GuildID[0]);

				char timeText[256];
				g_ObjectData->SecondsToTimeAbbrev((float)Time, timeText);
				g_pChatFrame->SendChatMessageEx("CHAT_MSG_SYSTEM", "", g_ObjectData->GetString("SYS_GUILDWAR_DECLARE"), guildName, timeText);

				flash = true;
			}			
		}
	}
	// �Ѱ����
	else
	{
		for ( vector<GuildDeclareWarInfo>::iterator iter = s_declareWarInfo.begin(); iter != s_declareWarInfo.end(); iter++ )
		{
			if ( memcmp(iter->bothID, info.bothID, sizeof(int)*2) == 0 )
			{
				s_declareWarInfo.erase(iter);
				break;
			}
		}

		// ��T
		if ( player )
		{
			int guildID = player->GetGuildID();
			if ( GuildID[0] == guildID || GuildID[1] == guildID )
			{
				const char* guildName;
				if ( GuildID[0] == guildID )
					guildName = GetGuildNameByID(GuildID[1]);
				else
					guildName = GetGuildNameByID(GuildID[0]);
				g_pChatFrame->SendChatMessageEx("CHAT_MSG_SYSTEM", "", g_ObjectData->GetString("SYS_GUILDWAR_REMOVE_DECLARE"), guildName);

				flash = true;
			}			
		}
	}

	g_pInterface->SendWorldEvent("GUILDWAR_DECLARELIST_UPDATE");

	// �ץ����a���Y
	g_pGameMain->GuildEnemyUpdate();
}

// ���|�ž����Y,�즸�����줽�|���a
void NetCli_GuildChild::RL_LoginWarInfo			( int GuildID[2] , int Time , int BeginTime )
{
	GuildDeclareWarInfo info;
	info.bothID[0] = GuildID[0];
	info.bothID[1] = GuildID[1];
	info.startTime = BeginTime;
	info.time = (float)Time;	

	for ( vector<GuildDeclareWarInfo>::iterator iter = s_declareWarInfo.begin(); iter != s_declareWarInfo.end(); iter++ )
	{
		if ( memcmp(iter->bothID, info.bothID, sizeof(int)*2) == 0 )
		{
			// ���ƭn�D
			// assert(false);
			return;
		}
	}

	s_declareWarInfo.push_back(info);
	g_pGameMain->GuildEnemyUpdate();
}

void NetCli_GuildChild::RD_DeclareWarFinalResult		( int GuildID[2] , int Score[2] , int KillCount[2] )
{
	char temp[256];
	int winner[3];
	int loser[3];

	if ( Score[0] > Score[1] )
	{
		winner[0] = GuildID[0];
		winner[1] = Score[0];
		winner[2] = KillCount[0];

		loser[0] = GuildID[1];
		loser[1] = Score[1];
		loser[2] = KillCount[1];
	}
	else
	{
		winner[0] = GuildID[1];
		winner[1] = Score[1];
		winner[2] = KillCount[1];

		loser[0] = GuildID[0];
		loser[1] = Score[0];
		loser[2] = KillCount[0];
	}

	// �B�z�ۤv���|
	GuildBaseStruct* info = g_pGuildFrame->GetGuildBaseInfo();
	if ( info ) {
		if ( info->GuildID == winner[0] ) {
			if ( winner[1] == loser[1] ) {
				info->WarEvenCount++;
			} else {
				info->WarWinCount++;
			}
		} else if ( info->GuildID == loser[0] ) {
			if ( winner[1] == loser[1] ) {
				info->WarEvenCount++;
			} else {
				info->WarLostCount++;
			}
		}
	}

	const char* winnerName = g_pGuildFrame->GetGuildNameByID(winner[0]);
	const char* loserName = g_pGuildFrame->GetGuildNameByID(loser[0]);

	if ( winnerName == NULL )
		winnerName = "unknown";
	if ( loserName == NULL )
		loserName = "unknown";

	if ( winner[1] == loser[1] )
		sprintf_s(temp, 256, g_ObjectData->GetString("SYS_GUILDWAR_RESULT_DRAW"), winnerName, loserName, winner[1], loser[1], winner[2], loser[2]);
	else
		sprintf_s(temp, 256, g_ObjectData->GetString("SYS_GUILDWAR_RESULT"), winnerName, loserName, winner[1], loser[1], winner[2], loser[2]);		

	g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", temp);
}

void NetCli_GuildChild::RX_SetGuildFlagInfoResult		( SetGuildFlagInfoResultENUM Result )
{

	switch(Result)
	{
	case EM_SetGuildFlagInfoResult_OK :
		g_pGameMain->SendSystemMsg( g_ObjectData->GetString("SYS_SETGUILDFLAG_OK") );
		break;
	case EM_SetGuildFlagInfoResult_GuildError :
		g_pGameMain->SendSystemMsg( g_ObjectData->GetString("SYS_SETGUILDFLAG_GUILDERROR") );
		break;
	case EM_SetGuildFlagInfoResult_LeaderError :
		g_pGameMain->SendSystemMsg( g_ObjectData->GetString("SYS_SETGUILDFLAG_LEADERERROR") );
		break;
	case EM_SetGuildFlagInfoResult_ScoreError :
		g_pGameMain->SendSystemMsg( g_ObjectData->GetString("SYS_SETGUILDFLAG_SCOREERROR") );
		break;
	}

}

void NetCli_GuildChild::RD_ChangeGuildNameResult		( ChangeGuildNameResultENUM Result )
{
	if (g_pGuildFrame)
		g_pGuildFrame->ChangeGuildNameResult(Result);
}
void NetCli_GuildChild::RD_ChangeGuildName				( int GuildID , char* GuildName )
{
	if (g_pGuildFrame)
		g_pGuildFrame->ChangeGuildName(GuildID,GuildName);

	g_pGameMain->ChangeGuildName(GuildID);
}


void NetCli_GuildChild::RD_GuildUpgradeResult			( int GuildLv , bool Result )
{
	if (g_pGuildFrame)
		g_pGuildFrame->GuildUpgradeResult(GuildLv,Result);
}

void NetCli_GuildChild::RD_SelectContributionLogResult	( int Count , ContributionInfoLogStruct* Log )
{
	if (g_pGuildFrame)
		g_pGuildFrame->SetDayLogTable(Count,Log);

}

void NetCli_GuildChild::RD_GuildBaseInfoUpdate(  )
{

	if (g_pGuildFrame)
		g_pGuildFrame->GuildBaseInfoUpdate();
}

void NetCli_GuildChild::RD_Guild_LevelUp				( int GuildID , int Level )
{
	if (g_pGuildFrame)
		g_pGuildFrame->Guild_LevelUp(GuildID,Level);
}

void NetCli_GuildChild::RD_GuildInfoChange				( GuildInfoChangeStruct& ChangeMode )
{

	if (g_pGuildFrame)
		g_pGuildFrame->GuildInfoChange(ChangeMode);

}

//�n�D���|�Ծ��v��Ƶ��G
void NetCli_GuildChild::RD_GuildWarHistroyResult( int count , GuildHouseWarHistoryStruct log[] )
{

	if (g_pGuildHousesFrame)
		g_pGuildHousesFrame->GuildWarHistroyResult(count,log);
}
