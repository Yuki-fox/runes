#include "NetCli_TalkChild.h"
#include "../net_channel/NetCli_ChannelChild.h"
#include "../../mainproc/GameMain.h"
#include "../../Interface/WorldFrames/ChatEdit.h"
#include "../../Interface/WorldFrames/ChatFrame.h"
#include "../../Interface/WorldFrames/SpeakFrame.h"
#include "../../Interface/Debug/DebugMessage.h"
#include "smallobj/SmallObj.h"
#include "../Net_Script/NetCli_Script_Child.h"
#include "../../Interface/PartyBoard/PartyBoardFrame.h"
#include "../../Interface/WorldFrames/GuildFrame.h"
//#include "../../Interface/WorldFrames/ChatMsn.h"

//-----------------------------------------------------------------------------
bool NetCli_TalkChild::_Init()
{
	if (This)
		return false;
	This = new NetCli_TalkChild;

	((NetCli_TalkChild*)This)->PrinttoFile.SetOutputFileName( "Chat.Log" );
	((NetCli_TalkChild*)This)->PrinttoFile.SetOutputFieldName( "Log" );

	return NetCli_Talk::_Init();
}

//-----------------------------------------------------------------------------
bool NetCli_TalkChild::_Release()
{
	NetCli_Talk::_Release();
	if (This)
	{
		delete This;
		This = NULL;
	}
	return true;
}

//-----------------------------------------------------------------------------
void NetCli_TalkChild::Rev_TalkTo( Voc_ENUM Job , bool IsSystem , const char* SenderName , const char* TargetName, int ContentSize , const char* Content, TalkSenderInfo& SenderInfo )
{
	char szReplaceString[4096];

	//SenderInfo.vocation[0] = Job;

	if( IsSystem == true )
	{
		string sRangeString = CNetCli_Script_Child::ReplaceSystemKeyword( Content );

		ContentSize = (int)sRangeString.size();
		Content		= sRangeString.c_str();

		ZeroMemory( szReplaceString, sizeof( szReplaceString ) );
		memcpy( szReplaceString, Content, ContentSize );

		Content		= szReplaceString;
		ContentSize	= ContentSize + 1;

		// �]���y�t���]��, Server �o�e�L�Ӫ� NPC �W��, �|�H�Ҫ����W�ٰe�L��, ��軡 Sys111282_name
		SenderName = g_ObjectData->GetSpecialString( SenderName );
	}

	// �t�εo�X
	if ( IsSystem ) {
		char senderName[MAX_PATH];
		senderName[0] = NPC_HEADER;
		strcpy(senderName+1, SenderName);
		g_pChatFrame->SendChatMessage(CHAT_MSG_WHISPER, senderName, Content);
	} else {
		// �ഫ�r��
		g_pGameMain->MatchPatternString((BYTE*)Content);

		// ��ۤv����
		if ( strcmp(SenderName, g_pGameMain->GetPlayerName()) == 0 )
		{
			g_pChatFrame->SendChatMessage(CHAT_MSG_WHISPER_INFORM, TargetName, Content);
			g_pChatFrame->HaveNewWhisper(TargetName, Content, 1);
		}
		// �����K�y,�¦W�氣�~
		else if ( !RoleData()->CheckIgnoreNameList(SenderName) && !g_pChatEdit->IsUndesirable(SenderName) )
		{
			// g_ruSymphony->PlaySoundByPath( "Sound\\Interface\\whisper.mp3", false );
			int loopCount = (FALSE) ? 0x7FFFFFFF : 1;
			g_ruAudible->Play("Sound\\Interface\\whisper.mp3", (CRuAudible::AudibleType)ruSOUNDTYPE_SFX, (loopCount > 1) ? TRUE : FALSE);						
			g_pChatFrame->SendChatMessage(CHAT_MSG_WHISPER, SenderName, Content, SenderInfo.vocation[0]);
			g_pChatFrame->HaveNewWhisper(SenderName, Content, 0);
		}
	}

	if( g_pGameMain->IsChatLog()  )
	{
		char tmp[512];
		//g_pDebugMessage->DebugOutput(0, "[Tell]%s->%s:%s", SenderName , TargetName ,Content);
		sprintf( tmp ,"[Tell]%s->%s:%s", SenderName , TargetName ,Content );
		PrinttoFile.OutputToFile( tmp );
	}
}

//-----------------------------------------------------------------------------
void NetCli_TalkChild::Rev_Range( Voc_ENUM Job , bool IsSystem , int Sayer , const char* Name , int ContentSize , const char* Content, bool IsNPCGossip, TalkSenderInfo& SenderInfo )
{
	char szReplaceString[4096];

	// �����t�Φr��
	CRoleSprite* sprite = g_pGameMain->FindSpriteByID(Sayer);
	if (sprite && sprite->IsVisible() == false)
		return;	

	//SenderInfo.vocation[0] = Job;

	if( IsSystem == true )
	{
		string sRangeString = CNetCli_Script_Child::ReplaceSystemKeyword( Content );

		ContentSize = (int)sRangeString.size();
		Content		= sRangeString.c_str();

		ZeroMemory( szReplaceString, sizeof( szReplaceString ) );
		memcpy( szReplaceString, Content, ContentSize );

		Content		= szReplaceString;
		ContentSize	= ContentSize + 1;

		// �]���y�t���]��, Server �o�e�L�Ӫ� NPC �W��, �|�H�Ҫ����W�ٰe�L��, ��軡 Sys111282_name
		Name = g_ObjectData->GetSpecialString( Name );
	}

	std::string szContent = Content;
	const char* key = "[$HIDECHAT]";
	int len = (int)strlen(key);
	size_t begin;
	while ( (begin = szContent.find(key)) != string::npos )
		szContent.replace(begin, len, "");

	if ( IsSystem )	{
		g_pSpeakFrame->AddText(Sayer, szContent.c_str(), 1.0f, 1.0f, 1.0f);

		char senderName[MAX_PATH];
		senderName[0] = NPC_HEADER;
		strcpy(senderName+1, Name);

		if ( IsNPCGossip && g_pGameMain->GetDisableDisplayNPCTalk() )
		{
			// �ثe���ᤣ���
		}
		else
		{
			if ( strstr(Content, "[$HIDECHAT]") == NULL )
			{
				g_pChatFrame->SendChatMessage(CHAT_MSG_SAY, senderName, szContent.c_str());
			}
		}		
	} else if ( !RoleData()->CheckIgnoreNameList(Name) && !g_pChatEdit->IsUndesirable(Name) ) {
		// �ഫ�r��
		g_pGameMain->MatchPatternString((BYTE*)szContent.c_str());

		g_pSpeakFrame->AddText(Sayer, szContent.c_str(), 1.0f, 1.0f, 1.0f);

		if ( IsNPCGossip && g_pGameMain->GetDisableDisplayNPCTalk() )
		{
			// �ثe���ᤣ���
		}
		else
		{
			g_pChatFrame->SendChatMessage(CHAT_MSG_SAY, Name, szContent.c_str(), SenderInfo.vocation[0]);
		}
	}

	if( g_pGameMain->IsChatLog()  )
	{
		char tmp[512];
		//sprintf( tmp ,CHAT_MSG_SAY ,Content, Name );
		sprintf( tmp ,"[Range]%s:%s" ,Name , Content);

		PrinttoFile.OutputToFile( tmp );
	}
}

//-----------------------------------------------------------------------------
/*
void NetCli_TalkChild::Rev_Zone( int Sayer , const char* Name , int ContentSize , const char* Content )
{
	char tmp[512];
	sprintf( tmp, "[%s]:%s", Name, Content );
	g_pChatFrame->AddMessage(eMessage_Channel_Zone, tmp);
}
*/
//-----------------------------------------------------------------------------
void NetCli_TalkChild::Rev_SystemMsg( int ContentSize , const char* Content )
{
	if ( g_pGameMain->IsShowServerDebugMsg() ) {
		g_pChatFrame->SendMsgEvent(CHAT_MSG_SYSTEM, Content, "");
	}
}

//-----------------------------------------------------------------------------
void NetCli_TalkChild::Rev_GmMsg( const char* Msg )
{
	string sString = CNetCli_Script_Child::ReplaceSystemKeyword( Msg );
	g_pChatFrame->SendChatMessageEx(CHAT_MSG_GM_TALK, "", "[GM]%s", sString.c_str());
}
//-----------------------------------------------------------------------------
void NetCli_TalkChild::Rev_Channel	( Voc_ENUM Job , GroupObjectTypeEnum Type , int ChannelID , TalkChannelTypeENUM MsgType , char* Name , char* Content, TalkSenderInfo& SenderInfo )
{
	char tmp[512];
	const char *ChannelName=NULL;

	if ( RoleData()->CheckIgnoreNameList(Name) == true || g_pChatEdit->IsUndesirable(Name) == true )
		return;	

	//SenderInfo.vocation[0] = Job;
	g_pGameMain->MatchPatternString((BYTE*)Content);

	switch( Type )
	{
	case EM_GroupObjectType_SysChannel:

		g_pChatFrame->SendChatMessage(CHAT_MSG_YELL, Name, Content, SenderInfo.vocation[0]);

		//sprintf( tmp, "[SysChannel %d][%s]:%s", ChannelID ,  Name, Content );
		//g_pChatFrame->AddMessage(0, tmp);
		break;
	case EM_GroupObjectType_SysZoneChannel:
	
		g_pChatFrame->SendChatMessage(CHAT_MSG_ZONE, Name, Content, SenderInfo.vocation[0]);		

		/*
		g_pChatFrame->SendChatMessageEx(CHAT_MSG_CHANNEL, Name, "[%d.%s]%s", ChannelID, g_pGameMain->GetZoneName(), Content);
		sprintf( tmp, "[ZoneChannel %d][%s]:%s", ChannelID ,  Name, Content );
		*/
		break;

	case EM_GroupObjectType_PlayerChannel:
		{
			const char* channelName = NetCli_ChannelChild::GetChannelNameByID(ChannelID);
		
			
			if ( channelName != NULL )
			{				
				sprintf_s(tmp, "%s%s", CHAT_MSG_CHANNEL, channelName);
				
				g_pChatFrame->SendChatMessage(tmp, Name, Content, SenderInfo.vocation[0]);
			}
		}
		break;
	case EM_GroupObjectType_Party:
		
	
		g_pChatFrame->SendChatMessage(CHAT_MSG_PARTY, Name, Content, SenderInfo.vocation[0]);
		break;
	case EM_GroupObjectType_Guild:
		
		g_pChatFrame->SendChatMessage(CHAT_MSG_GUILD, Name, Content, SenderInfo.vocation[0]);
		break;
	}

	int isSelf=0;

	// �ۤv�o��
	if ( strcmp(Name, g_pGameMain->GetPlayerName()) == 0 )
		isSelf=1;


	{
	 	//ChannelName=g_ObjectData->GetString(ChannelName);

		g_pChatFrame->HaveNewChannel(Type,ChannelID, Name, Content, isSelf);

	}
	

	if( g_pGameMain->IsChatLog()  )
		PrinttoFile.OutputToFile( tmp );
}
//-----------------------------------------------------------------------------
void NetCli_TalkChild::Rev_GameMessageDataEvent	( GameMessageEventDataENUM Event , int iNumArg, GameMessageDataArgStruct Data[5] )
{
	char KeyStr[256];
	sprintf( KeyStr , "SYS_GAMEMSGEVENTDATA_%03d" , Event );
	const char* MsgStr = g_ObjectData->GetString( KeyStr );

	char ArgStr[5][64];

	for( int i = 0 ; i < iNumArg ; i++ )
	{
		switch( Data[i].Type )
		{
		case EM_GameMessageDataType_None:
			ArgStr[i][0] = 0;
			break;
		case EM_GameMessageDataType_Int:
			if(strcmp(KeyStr,"SYS_GAMEMSGEVENTDATA_004") == 0)
			{
				sprintf( ArgStr[i] , "%02d" , Data[i].Value );
				g_pGuildFrame->GuildCanNotJoin();
			}
			else
			{
				sprintf( ArgStr[i] , "%d" , Data[i].Value );
			}
			break;
		case EM_GameMessageDataType_ObjID_Name:
			GameObjDbStructEx* ObjDB = g_ObjectData->GetObj( Data[i].Value );
			if( ObjDB == NULL )
				ArgStr[i][0] = 0;
			else
				sprintf( ArgStr[i] , "%s" , ObjDB->GetName() );
			break;
		}
	}
	
	char Content[4096];
	
	switch( iNumArg )
	{
		case 1:	sprintf( Content ,  MsgStr , ArgStr[0] );	break;
		case 2:	sprintf( Content ,  MsgStr , ArgStr[0], ArgStr[1] );	break;
		case 3:	sprintf( Content ,  MsgStr , ArgStr[0], ArgStr[1], ArgStr[2] );	break;
		case 4:	sprintf( Content ,  MsgStr , ArgStr[0], ArgStr[1], ArgStr[2], ArgStr[3] );	break;
		case 5:	sprintf( Content ,  MsgStr , ArgStr[0], ArgStr[1], ArgStr[2], ArgStr[3], ArgStr[4] );	break;
	}

	//sprintf( Content ,  MsgStr , ArgStr[0] , ArgStr[1], ArgStr[2], ArgStr[3], ArgStr[4] );
	g_pChatFrame->SendMsgEvent(CHAT_MSG_SYSTEM, Content, "" );
	g_pGameMain->SendWarningMsg(Content);
}
//-----------------------------------------------------------------------------
void NetCli_TalkChild::Rev_GameMessageEvent	( GameMessageEventENUM Evnet )
{
	float frameTime = (float)g_pGameMain->GetFrameTime();

	char KeyStr[256];
	sprintf( KeyStr , "SYS_GAMEMSGEVENT_%03d" , Evnet );
	const char* MsgStr = g_ObjectData->GetString( KeyStr );

	switch(Evnet)
	{
	case EM_GameMessageEvent_Role_Right_NoSpeak:			//GM��A �ä[�T��
	case EM_GameMessageEvent_Role_Right_Speak:				//GM��A �Ѱ��T��
	case EM_GameMessageEvent_Role_Right_NoTrade:			//GM��A �ä[�T����
	case EM_GameMessageEvent_Role_Right_Trade:				//GM��A �Ѱ��T����
	case EM_GameMessageEvent_TargetHasParty:
		g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", MsgStr);
		break;

	case EM_GameMessageEvent_AttackError_TargetTooFar:		//�����ؼйL��
	case EM_GameMessageEvent_AttackError_Dir:				//�������V�����D
		{
			static float time = 0.0f;
			if ( frameTime - time > 5.0f )
			{
				time = frameTime;
				g_pGameMain->SendWarningMsg(MsgStr);
			}
		}
		break;

	case EM_GameMessageEvent_MagicError_MPNotEnough:			//MP����
	case EM_GameMessageEvent_MagicError_WarriorSPNotEnough:		//�ԤhSP����
	case EM_GameMessageEvent_MagicError_RangerSPNotEnough:		//�C�LSP����
	case EM_GameMessageEvent_MagicError_ThiefSPNotEnough:		//�s��SP����
		g_pChatFrame->SendWorldEvent("UPDATE_MOUSE_LIGHT_NotEnough");// �e�X�ƥ�LUA

	default:
		g_pGameMain->SendWarningMsg(MsgStr);
		break;
		/*
	case EM_GameMessageEvent_AttackError_TargetDead:		//�ؼЦ��`
		g_pGameMain->SendWarningMsg("�ؼЦ��`");
		break;
	case EM_GameMessageEvent_AttackError_Target:			//�ؼФ��i����
		g_pGameMain->SendWarningMsg("�ؼФ��i����");
		break;
	case EM_GameMessageEvent_MagicError_OnSpell:			//�w�b�I�k��
		g_pGameMain->SendWarningMsg("�w�b�I�k��");
		break;
	case EM_GameMessageEvent_MagicError_TargetTooFar:		//�I�k�Z���L��
		g_pGameMain->SendWarningMsg("�I�k�Z���L��");
		break;
	case EM_GameMessageEvent_MagicError_Request:			//�k�N�ݨD�����D
		g_pGameMain->SendWarningMsg("�k�N�ݨD�����D");
		break;
	case EM_GameMessageEvent_MagicError_Target:				//�k�N�ؼЦ����D
		g_pGameMain->SendWarningMsg("�k�N�ؼЦ����D");
		break;
	case EM_GameMessageEvent_MagicError_Back:				//�n�b�ؼЭI��~�i�I�i
		g_pGameMain->SendWarningMsg("�n�b�ؼЭI��~�i�I�i");
		break;
	case EM_GameMessageEvent_MagicError_Front:				//�n�b�ؼЫe���~�i�I�i
		g_pGameMain->SendWarningMsg("�n�b�ؼЫe���~�i�I�i");
		break;
	case EM_GameMessageEvent_MagicError_LookAt:				//�n�ݵۥؼФ~�i�I�i
		g_pGameMain->SendWarningMsg("�n�ݵۥؼФ~�i�I�i");
		break;
	case EM_GameMessageEvent_MagicError_Coldown:			//�N�o��
		g_pGameMain->SendWarningMsg("�N�o��");
		break;
	case	EM_GameMessageEvent_OK								:	//�S�����D
		g_pGameMain->SendWarningMsg("�N�o��");
		break;
	case	EM_GameMessageEvent_Failed							:	//�@�륢��
		g_pGameMain->SendWarningMsg("�N�o��");
		break;
	case	EM_GameMessageEvent_FightNoLogout					:	//�԰������i�n�X
		g_pGameMain->SendWarningMsg("�N�o��");
		break;
	case	EM_GameMessageEvent_ChanageZoneError_PraviteZoneFull:	//�W�߰ϰ캡�F
		g_pGameMain->SendWarningMsg("�N�o��");
		break;
	case	EM_GameMessageEvent_ChanageZoneError_Failed			:	//���ϥ���
		g_pGameMain->SendWarningMsg("�N�o��");
		break;
	case	EM_GameMessageEvent_MoveSpeedDataError				:	//���ʸ�Ʀ����D
		g_pGameMain->SendWarningMsg("�N�o��");
		break;
	case	EM_GameMessageEvent_MoveSpeedToFastError			:	//���ʥ[�t??
		g_pGameMain->SendWarningMsg("�N�o��");
		break;
	case	EM_GameMessageEvent_TotalMoveSpeedToFastError		:	//�����`�q�[�t??
		g_pGameMain->SendWarningMsg("�N�o��");
		break;
	case	EM_GameMessageEvent_MoveVectorError					:	//���ʦV�q�L�j
		g_pGameMain->SendWarningMsg("�N�o��");
		break;
	case	EM_GameMessageEvent_NoMove							:	//�ثe�Q�I�i�w���k�N�A���i����
		g_pGameMain->SendWarningMsg("�N�o��");
		break;
	case	EM_GameMessageEvent_CharacterNotFound				:	//���⤣�s�b
		g_pGameMain->SendWarningMsg("�N�o��");
		break;
	case	EM_GameMessageEvent_AttackError_PhyDisable			:	//���i���z����
		g_pGameMain->SendWarningMsg("�N�o��");
		break;
	case	EM_GameMessageEvent_AttackError_MagicDisable		:	//���i�k�N����
		g_pGameMain->SendWarningMsg("�N�o��");
		break;
	case	EM_GameMessageEvent_MagicError_MagicIDError			:	//MagicID�����D
		g_pGameMain->SendWarningMsg("�N�o��");
		break;
	case	EM_GameMessageEvent_MagicError_CreateObjectError	:	//��ߪ��󥢱�
		g_pGameMain->SendWarningMsg("�N�o��");
		break;
	case	EM_GameMessageEvent_MagicError_HPNotEnough			:	//HP����
		g_pGameMain->SendWarningMsg("�N�o��");
		break;
	case	EM_GameMessageEvent_MagicError_MPNotEnough			:	//MP����
		g_pGameMain->SendWarningMsg("�N�o��");
		break;
	case	EM_GameMessageEvent_MagicError_WarriorSPNotEnough	:	//�ԤhSP����
		g_pGameMain->SendWarningMsg("�N�o��");
		break;
	case	EM_GameMessageEvent_MagicError_RangerSPNotEnough	:	//�C�LSP����
		g_pGameMain->SendWarningMsg("�N�o��");
		break;
	case	EM_GameMessageEvent_MagicError_ThiefSPNotEnough		:	//�s��SP����
		g_pGameMain->SendWarningMsg("�N�o��");
		break;
	case	EM_GameMessageEvent_MagicError_StomachFull			:	//�Y���U
		g_pGameMain->SendWarningMsg("�N�o��");
		break;
	case	EM_GameMessageEvent_MagicError_CostItemError		:	//���Ӫ��~�����D
		g_pGameMain->SendWarningMsg("�N�o��");
		break;
	case	EM_GameMessageEvent_MagicError_NoGun				:	//�S�j
		g_pGameMain->SendWarningMsg("�N�o��");
		break;
	case	EM_GameMessageEvent_MagicError_NoBow				:	//�S�}
		g_pGameMain->SendWarningMsg("�N�o��");
		break;
	case	EM_GameMessageEvent_MagicError_NoCossBow			:	//�S�Q�r�}
		g_pGameMain->SendWarningMsg("�N�o��");
		break;
	case	EM_GameMessageEvent_MagicError_NoGunAmmo			:	//�S�l�u
		g_pGameMain->SendWarningMsg("�N�o��");
		break;
	case	EM_GameMessageEvent_MagicError_NoBowAmmo			:	//�S�b
		g_pGameMain->SendWarningMsg("�N�o��");
		break;
	case	EM_GameMessageEvent_MagicError_NoCossBowAmmo		:	//�S��
		g_pGameMain->SendWarningMsg("�N�o��");
		break;
	case	EM_GameMessageEvent_MagicError_NoThrowAmmo			:	//�S���Y��
		g_pGameMain->SendWarningMsg("�N�o��");
		break;
	case	EM_GameMessageEvent_MagicError_NoAllLongWeapon		:	//�S���{�Z��
		g_pGameMain->SendWarningMsg("�N�o��");
		break;
	case	EM_GameMessageEvent_MagicError_NoAmmo				:	//�S�u��
		g_pGameMain->SendWarningMsg("�N�o��");
		break;
	case	EM_GameMessageEvent_MagicError_NeedWeaponError		:	//�Z���L�k�I�i���ۦ�
		g_pGameMain->SendWarningMsg("�N�o��");
		break;
	case	EM_GameMessageEvent_MagicError_NeedBuffError		:	//�ʤ֫e�m���k�NBuf
		g_pGameMain->SendWarningMsg("�N�o��");
		break;
	case	EM_GameMessageEvent_MagicError_NeedNoBuffError		:	//��Buff�٨S������
		g_pGameMain->SendWarningMsg("�N�o��");
		break;
	case	EM_GameMessageEvent_MagicError_NeedTargetNoBuffError:	//�ؼЧ�Buff�٨S������
	case	EM_GameMessageEvent_MagicError_NeedUnarmed			:	//�ݭn�Ť�~�i�I�i
	case	EM_GameMessageEvent_MagicError_NeedBlade			:	//�ݭn�D�⮳���C�~�i�I�i
	case	EM_GameMessageEvent_MagicError_NeedDagger			:	//�ݭn�D�⮳�P���~�i�I�i
	case	EM_GameMessageEvent_MagicError_NeedWand				:	//�ݭn�D�⮳�v���~�i�I�i
	case	EM_GameMessageEvent_MagicError_NeedAxe				:	//�ݭn�D�⮳����~�i�I�i
	case	EM_GameMessageEvent_MagicError_NeedBludgeon			:	//�ݭn���ҴΤ~�i�I�i
	case	EM_GameMessageEvent_MagicError_NeedClaymore			:	//�ݭn������C�~�i�I�i
	case	EM_GameMessageEvent_MagicError_NeedStaff			:	//�ݭn��������~�i�I�i
	case	EM_GameMessageEvent_MagicError_Need2HAxe			:	//�ݭn�������~�i�I�i
	case	EM_GameMessageEvent_MagicError_Need2HHammer			:	//�ݭn��������~�i�I�i
	case	EM_GameMessageEvent_MagicError_NeedPolearm			:	//�ݭn�����٤~�i�I�i
	case	EM_GameMessageEvent_MagicError_NeedSwordType		:	//�ݭn���C���~�i�I�i
	case	EM_GameMessageEvent_MagicError_NeedAxeType			:	//�ݭn����~�i�I�i
	case	EM_GameMessageEvent_MagicError_NeedShield			:	//�ݭn���ޤ~�i�I�i
	case	EM_GameMessageEvent_MagicError_NeedDistance			:	//�ݭn�Z������
	case	EM_GameMessageEvent_MagicError_NeedNoAttack			:	//�ݭn�ۤv�D�԰�
	case	EM_GameMessageEvent_MagicError_NeedAttack			:	//�ݭn�԰��~�i�ϥ�
	case	EM_GameMessageEvent_MagicError_NeedCritical			:	//�z����~�i�ϥ�
	case	EM_GameMessageEvent_MagicError_NeedBeCritical		:	//�Q�z����~�i�ϥ�
	case	EM_GameMessageEvent_MagicError_NeedDodge			:	//�{�׫�~�i�ϥ�
	case	EM_GameMessageEvent_MagicError_NeedMiss				:	//Miss��~�i�ϥ�
	case	EM_GameMessageEvent_MagicError_NeedParry			:	//���ɫ�~�i�ϥ�
	case	EM_GameMessageEvent_MagicError_NeedBeParry			:	//�Q���ɫ�~�i�ϥ�
	case	EM_GameMessageEvent_MagicError_NeedTargetNoAttack	:	//�ؼЫD�԰��~�i�ϥ�
	case	EM_GameMessageEvent_MagicError_NeedTargetAttack		:	//�ؼо԰��~�i�ϥ�
	case	EM_GameMessageEvent_MagicError_NeedBeDodge			:	//�Q�{�פ~�i�ϥ�
	case	EM_GameMessageEvent_MagicError_NeedHPSmallerPercent	:	//HP�L�h�L�k�ϥ�
	case	EM_GameMessageEvent_MagicError_NeedHPGreaterPercent	:	//HP�L�ֵL�k�ϥ�
	case	EM_GameMessageEvent_MagicError_NeedJob				:	//��¾�~�L�k�I�i
	case	EM_GameMessageEvent_MagicError_NeedLongDistWeapon	:	//�ݭn�����Z���Z���~�i�ϥ�
	case	EM_GameMessageEvent_MagicError_NeedHammerType		:	//�ݭn�������Z���~�i�ϥ�
	case	EM_GameMessageEvent_MagicError_Need2HWeapon			:	//�ݭn������Z���~�i�ϥ�
	case	EM_GameMessageEvent_MagicError_PetNotFound			:	//�䤣���d��
	case	EM_GameMessageEvent_Shop_NotCloseYet				:	//������A�S�Ѱ�
	case	EM_GameMessageEvent_Shop_ACOpenYet					:	//����ҥ��b�}�Ҥ�
	case	EM_GameMessageEvent_Shop_MailOpenYet				:	//�l�󥿦b�}�Ҥ�
	case	EM_GameMessageEvent_Shop_ShopOpenYet				:	//�ө����b�}�Ҥ�
	case	EM_GameMessageEvent_Shop_BankOpenYet				:	//�Ȧ楿�b�}�Ҥ�
	case	EM_GameMessageEvent_Shop_ChangeJobOpenYet			:	//�����������b�}�Ҥ�
	case	EM_GameMessageEvent_Shop_BillBoardOpenYet			:	//�Ʀ�]�������b�}�Ҥ�
	case	EM_GameMessageEvent_Shop_ACNoOpen					:	//������٨S�}��
	case	EM_GameMessageEvent_Shop_MailNoOpen					:	//�l���٨S�}��
	case	EM_GameMessageEvent_Shop_ShopNoOpen					:	//�ө��٨S�}��
	case	EM_GameMessageEvent_Shop_BankNoOpen					:	//�Ȧ��٨S�}��
	case	EM_GameMessageEvent_Shop_ChangeJobNoOpen			:	//�����٨S�}��
	case	EM_GameMessageEvent_Shop_BillBoardNoOpen			:	//�Ʀ�]�٨S�}��
	case	EM_GameMessageEvent_Shop_ItemNotSell				:	//�S�榹���~
	case	EM_GameMessageEvent_Shop_CountTooMany				:	//�ʶR�L�h	
	case	EM_GameMessageEvent_Shop_ItemNotExist				:	//�����~���s�b
	case	EM_GameMessageEvent_Shop_CountError					:	//�ʶR�ƶq�����D
	case	EM_GameMessageEvent_Shop_NoSellAccountItem			:	//���i�ʶR�I�ưӫ~
	case	EM_GameMessageEvent_Shop_MomeyNotEnought			:	//���B����
	case	EM_GameMessageEvent_Shop_BagFull					:	//�]�q���F
	case	EM_GameMessageEvent_Shop_SellDataError				:	//���~��Ƥ���
	case	EM_GameMessageEvent_Shop_SellItemOnUse				:	//���~�٦b�ϥΤ�
	case	EM_GameMessageEvent_Shop_CanNotSell					:	//�����~���i��
	case	EM_GameMessageEvent_Shop_PepairDataError			:	//���~��Ƥ���
	case	EM_GameMessageEvent_EQError_WearPos_Body			:	//�˳Ʀ�m�����D
	case	EM_GameMessageEvent_EQError_DataInfo_Body			:	//�˳Ƹ�Ʀ����D
	case	EM_GameMessageEvent_EQError_OrgObjID_Body			:	//�˳ƪ��󦳰��D
	case	EM_GameMessageEvent_EQError_WearPos_Bank			:	//�˳Ʀ�m�����D
	case	EM_GameMessageEvent_EQError_DataInfo_Bank			:	//�˳Ƹ�Ʀ����D
	case	EM_GameMessageEvent_EQError_OrgObjID_Bank			:	//�˳ƪ��󦳰��D
	case	EM_GameMessageEvent_EQError_WearPos_EQ				:	//�˳Ʀ�m�����D
	case	EM_GameMessageEvent_EQError_DataInfo_EQ				:	//�˳Ƹ�Ʀ����D
	case	EM_GameMessageEvent_EQError_OrgObjID_EQ				:	//�˳ƪ��󦳰��D
	case	EM_GameMessageEvent_FriendList_DelError				:	//�R���n�ͦ����D
	case	EM_GameMessageEvent_Guild_GuildNoFound				:	//�ۤv�S�����|
	case	EM_GameMessageEvent_Guild_NoGuild					:	//�S�����|
	case	EM_GameMessageEvent_Guild_GuildIsReady				:	//�w�g�O�������|
	case	EM_GameMessageEvent_Guild_NotLeader					:	//�D���|�|��
	case	EM_GameMessageEvent_Guild_TargetNotOnline			:	//���H���b�u�W
	case	EM_GameMessageEvent_Guild_LeaderNotOnline			:	//���|�|�����b�u�W
	case	EM_GameMessageEvent_Guild_GuildIsNotReady			:	//�٦b�s�p��
	case	EM_GameMessageEvent_Guild_NoInviteRight				:	//�S���ܽХ[�J���v��
	case	EM_GameMessageEvent_Guild_InviterNotOnline			:	//�ܽЪ̤��b�u�W
	case	EM_GameMessageEvent_Item_OrgObjIDError				:	//�����Ʀ����D
	case	EM_GameMessageEvent_Item_NotItem					:	//�D���~
	case	EM_GameMessageEvent_Item_PosError					:	//�n�D�񪺦�m�����D
	case	EM_GameMessageEvent_Item_Queue_PosError				:	//�n�D�񪺦�m�����D
	case	EM_GameMessageEvent_Item_Queue_BufferEmpty			:	//���~�ǦC���O�Ū�
	case	EM_GameMessageEvent_Item_Queue_DataError			:	//���~��ƿ��~
	case	EM_GameMessageEvent_Item_Queue_CountError			:	//���~�ƶq���~
	case	EM_GameMessageEvent_Item_DelError					:	//��ƧR�����~
	case	EM_GameMessageEvent_Item_ExchangeError				:	//���~�洫�����D
	case	EM_GameMessageEvent_Item_BankOpenError				:	//�Ȧ�}�Ҫ��p�����D
	case	EM_GameMessageEvent_Item_BankTooFar					:	//�Ȧ�Z���L��
	case	EM_GameMessageEvent_Item_ExchangeMoneyError			:	//�����洫�����D
	case	EM_GameMessageEvent_Item_CountError					:	//�ƶq�����D
	case	EM_GameMessageEvent_PK_PKError						:	//PK�B�z�����D
	case	EM_GameMessageEvent_PK_InviteError					:	//�w�n�Ш�L�ؼ�
	case	EM_GameMessageEvent_PK_Prepar						:	//�w�bPK�ǳƤ�
	case	EM_GameMessageEvent_PK_Begin						:	//�w�bPK��
	case	EM_GameMessageEvent_PK_TargetNotExist				:	//�ؼФ��s�b
	case	EM_GameMessageEvent_PK_TargetTooFar					:	//�ؼйL��
	case	EM_GameMessageEvent_PK_TargetInviteOther			:	//�ؼЭn�Ш�L�H
	case	EM_GameMessageEvent_PK_TargetPK						:	//�ؼ�PK��
	case	EM_GameMessageEvent_PK_PKInviteResultError			:	//PK�^����Ʀ����D
	case	EM_GameMessageEvent_Trade_ItemError					:	//������~�����D
	case	EM_GameMessageEvent_Trade_CanNotTrade				:	//���~���i���
	case	EM_GameMessageEvent_Trade_CanNotModify				:	//����T�w�ᤣ�i�ק�
	case	EM_GameMessageEvent_Trade_PosError					:	//��m��m�����D
	case	EM_GameMessageEvent_Trade_PosOnLock					:	//���~��w��
	case	EM_GameMessageEvent_Trade_MoneyError				:	//�������t��
	case	EM_GameMessageEvent_Trade_AccountError				:	//�Ť�����t��
	case	EM_GameMessageEvent_Trade_MoneyNotEnough			:	//��������
	case	EM_GameMessageEvent_Trade_AccountNotEnough			:	//�I�Ƥ���
	case	EM_GameMessageEvent_Trade_Error						:	//�������
	*/
	}
}
//-----------------------------------------------------------------------------
void NetCli_TalkChild::Rev_ScriptMessage	( int Sayer, int iTypeID, int iColorID, int iStringLen, const char* szString )
{
	EM_SCRIPTMESSAGE emType = (EM_SCRIPTMESSAGE)iTypeID;

	// �����t�Φr��
	CRoleSprite* sprite = g_pGameMain->FindSpriteByID(Sayer);
	if (sprite && sprite->IsScriptHide())
		return;

	//char szReplaceString[4096];

	const char* key = "[$MUTE]";
	string sRangeString = CNetCli_Script_Child::ReplaceSystemKeyword( szString );	
	bool boMute = (strncmp(sRangeString.c_str(), "[$MUTE]", 7) == 0) ? true : false;	
	int len = (int)strlen(key);
	size_t begin;
	while ( (begin = sRangeString.find(key)) != string::npos )
		sRangeString.replace(begin, len, "");

		/*
		ContentSize = sRangeString.size();
		Content		= sRangeString.c_str();

		ZeroMemory( szReplaceString, sizeof( szReplaceString ) );
		memcpy( szReplaceString, Content, ContentSize );

		Content		= szReplaceString;
		ContentSize	= ContentSize + 1;
		*/


	char szTempString[4096];
	if( iColorID != 0 )
	{
		sprintf( szTempString, "%s|c%x%s|r", (boMute) ? key : "", iColorID, sRangeString.c_str() );
	}
	else
	{
		sprintf( szTempString, "%s%s", (boMute) ? key : "", sRangeString.c_str() );
	}

	switch( emType )
	{
	case EM_SCRIPTMESSAGE_WARNING:	// �X�{�b�e��������
		{
			g_pGameMain->SendWarningMsg( szTempString );
		} break;
	case EM_SCRIPTMESSAGE_NOTICE:	// �X�{�b�e�����U�� CHAT FRAME ���V��,�����a���ܪ�
		{
			g_pChatFrame->SendMsgEvent( CHAT_MSG_SYSTEM, szTempString, "" );
		}
		break;
	case EM_SCRIPTMESSAGE_SYSTEM:
		{
			g_pGameMain->SendSystemMsg( szTempString );
		} break;
	case EM_SCRIPTMESSAGE_BOTH:
		{
			g_pGameMain->SendSystemMsg( szTempString );
			g_pGameMain->SendSystemChatMsg( szTempString );
		} break;
	}
}
void NetCli_TalkChild::Rev_RunningMsgEx( RunningMsgExTypeENUM Type , const char* Msg )
{
	lua_State* L = g_pGameMain->GetInterface()->GetUi()->GetLuaState();

	string strMsg = CNetCli_Script_Child::ReplaceSystemKeyword( Msg );
	const char* msg = strMsg.c_str();

	lua_pushstring( L, msg );
	lua_setglobal( L, "arg1" );


	//lua_pushstring( L, CNetCli_Script_Child::ReplaceSystemKeyword( Msg ).c_str() );

	if( Type == EM_RunningMsgExType_GM )
	{
		lua_pushstring( L, "gm" );
	}
	else if( Type == EM_RunningMsgExType_Monitor )
	{
		lua_pushstring( L, "monitor" );
	}
	else if( Type == EM_RunningMsgExType_Shop )
	{
		lua_pushstring( L, "shop" );
	}
	else
	{
 		lua_pushstring( L, "other" );
	}
	lua_setglobal( L, "arg2" );

	g_pInterface->SendWorldEvent( UiEvent_ScrollBannerMessageEX );

	g_pGameMain->SendSystemChatMsg( msg );
}

void NetCli_TalkChild::Rev_RunningMsg				( const char* Msg )
{
	string strMsg = CNetCli_Script_Child::ReplaceSystemKeyword( Msg );
	const char* msg = strMsg.c_str();

	g_pGameMain->SendScrollBannerMessage( msg );
}

void NetCli_TalkChild::Rev_OfflineMessage			( int FromDBID , int Time , const char* FromName  , const char* ToName , const char* Content )
{
	// �ഫ�r��
	g_pGameMain->MatchPatternString((BYTE*)Content);

	// �����K�y,�¦W�氣�~
	if ( RoleData()->CheckIgnoreNameList(FromName) == false && !g_pChatEdit->IsUndesirable(FromName) )
	{
		g_pChatFrame->AddOfflineMessage(FromName, Content);
	}
}

void NetCli_TalkChild::Rev_BillBoardInfo			( int PlayerDBID , int DataSize , char* Data )
{
	g_pPartyBoardFrame->AddPost( PlayerDBID, DataSize, Data );
}

void NetCli_TalkChild::Rev_BillBoardDelete			( int PlayerDBID )
{
	g_pPartyBoardFrame->DeletePost( PlayerDBID );
}