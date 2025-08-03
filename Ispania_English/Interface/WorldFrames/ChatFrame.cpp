#include "../../netwaker_member/NetWakerClientInc.h"
#include "../mainproc/GameMain.h"
#include "../itemlink/ItemLink.h"
#include "ChatFrame.h"
#include "chatMsn.h"
#include "../Interface/ItemLink/ItemLink.h"
#include "../../tools/MString/MString.h"

#include "../Interface/Login/AccountLogin.h"


#include "../Interface/WorldFrames/GuildFrame.h"


#define CHAT_HEADER_CODE						0x00000100					// 顏色碼儲存檢查碼


#define UiEvent_VoiceChannelMemberUpdate "VOICE_CHANNEL_MEMBER_UPDATE"

#define UiEvent_VoiceChannelUpdate "VOICE_CHANNEL_UPDATE"

#define UiEvent_InviteToChannel "INVITE_TO_CHANNEL"


MsnManager*                     g_pMsnManager=NULL;  
CChatFrame*						g_pChatFrame = NULL;

int LuaFunc_GetChatWindowInfo(lua_State* L);
int LuaFunc_GetChatWindowMessages(lua_State* L);
int	LuaFunc_GetChatWindowChannels(lua_State* L);
int LuaFunc_CreateChannel(lua_State* L);
int LuaFunc_JoinChannel(lua_State* L);
int LuaFunc_LeaveChannel(lua_State* L);
int LuaFunc_ChannelKick(lua_State* L);
int LuaFunc_ChannelInvite(lua_State* L);
int LuaFunc_SetChannelOwner(lua_State* L);
int LuaFunc_GetChannelOwner(lua_State* L);
int LuaFunc_GetChannelName(lua_State* L);
int	LuaFunc_GetChannelList(lua_State* L);
int LuaFunc_SetChannelColor(lua_State* L);
int LuaFunc_GetChannelColor(lua_State* L);
int	LuaFunc_SetChatWindowShown(lua_State* L);
int LuaFunc_SetChatWindowName(lua_State* L);
int LuaFunc_SetChatWindowColor(lua_State* L);
int LuaFunc_SetChatWindowAlpha(lua_State* L);
int LuaFunc_SetChatWindowLocked(lua_State* L);
int LuaFunc_SetChatWindowDocked(lua_State* L);
int LuaFunc_SetChatWindowSize(lua_State* L);
int LuaFunc_AddChatWindowMessages(lua_State* L);
int LuaFunc_RemoveChatWindowMessages(lua_State* L);
int	LuaFunc_AddChatWindowChannel(lua_State* L);
int	LuaFunc_RemoveChatWindowChannel(lua_State* L);
int	LuaFunc_Chat_GetMsnInfo(lua_State* L);
int LuaFunc_Chat_SetMsnInfoOpen(lua_State* L);
int LuaFunc_Chat_SetMsnClose(lua_State* L);
int LuaFunc_Chat_AddMessage(lua_State* L);
int LuaFunc_Chat_ClearAllMsg(lua_State* L);
int LuaFunc_Chat_Update(lua_State* L);
int LuaFunc_Chat_CopyToClipboard(lua_State* L);
//int LuaFunc_Chat_SetTextColor(lua_State* L);
//int LuaFunc_Chat_GetTextColor(lua_State* L);

int LuaFunc_ParseHyperlink(lua_State* L);


int LuaFunc_VoiceChannel_InfoRequest(lua_State* L);

int LuaFunc_VoiceChannel_GetMemberCount(lua_State* L);
int LuaFunc_VoiceChannel_GetMemberInfo(lua_State* L);

int LuaFunc_VoiceChannel_Join(lua_State* L);
int LuaFunc_VoiceChannel_Exit(lua_State* L);

int LuaFunc_VoiceChannel_IsCurrentChannel(lua_State* L);
int LuaFunc_VoiceChannel_GetCurrentChannel(lua_State* L);

int LuaFunc_VoiceChannel_GetPlayerInfo(lua_State* L);

int LuaFunc_VoiceChannel_GetPlayerInfo(lua_State* L);

//
int LuaFunc_VoiceChannel_PartnerMute(lua_State* L);

// ----------------------------------------------------------------------------
CChatFrame::CChatFrame(CInterface* object)
: CInterfaceBase(object)
{
	g_pChatFrame = this;

	m_loadServerData = false;
	m_chatColor.clear();
	m_channel.clear();
	m_offlineMessage.clear();
	//m_stVoiceChannelInfos.clear();
	//m_stVoiceChannelInfo.isDirty; // nothing assign ?! marked by slime.
	g_pMsnManager = new MsnManager(object);
}

// ----------------------------------------------------------------------------
CChatFrame::~CChatFrame()
{
	UI_SAFE_DELETE(g_pMsnManager);

	g_pChatFrame = NULL;
}

// ----------------------------------------------------------------------------
void CChatFrame::RegisterFunc()
{
	lua_State* L = GetLuaStateWorld();
	if (L)
	{
		lua_register( L, "GetChatWindowInfo",			LuaFunc_GetChatWindowInfo );
		lua_register( L, "GetChatWindowMessages",		LuaFunc_GetChatWindowMessages );
		lua_register( L, "GetChatWindowChannels",		LuaFunc_GetChatWindowChannels );
		lua_register( L, "CreateChannel",				LuaFunc_CreateChannel );
		lua_register( L, "JoinChannel",					LuaFunc_JoinChannel );
		lua_register( L, "LeaveChannel",				LuaFunc_LeaveChannel );
		lua_register( L, "ChannelKick",					LuaFunc_ChannelKick );

 		lua_register( L, "ChannelInvite",					LuaFunc_ChannelInvite );

		lua_register( L, "SetChannelOwner",				LuaFunc_SetChannelOwner );
		lua_register( L, "GetChannelOwner",				LuaFunc_GetChannelOwner );
		lua_register( L, "GetChannelName",				LuaFunc_GetChannelName );
		lua_register( L, "GetChannelList",				LuaFunc_GetChannelList );
		lua_register( L, "SetChannelColor",				LuaFunc_SetChannelColor );
		lua_register( L, "GetChannelColor",				LuaFunc_GetChannelColor );
		lua_register( L, "SetChatWindowShown",			LuaFunc_SetChatWindowShown );
		lua_register( L, "SetChatWindowName",			LuaFunc_SetChatWindowName );
		lua_register( L, "SetChatWindowColor",			LuaFunc_SetChatWindowColor );
		lua_register( L, "SetChatWindowAlpha",			LuaFunc_SetChatWindowAlpha );
		lua_register( L, "SetChatWindowLocked",			LuaFunc_SetChatWindowLocked );
		lua_register( L, "SetChatWindowDocked",			LuaFunc_SetChatWindowDocked );
		lua_register( L, "SetChatWindowSize",			LuaFunc_SetChatWindowSize );
		lua_register( L, "AddChatWindowMessages",		LuaFunc_AddChatWindowMessages );
		lua_register( L, "RemoveChatWindowMessages",	LuaFunc_RemoveChatWindowMessages );
		lua_register( L, "AddChatWindowChannel",		LuaFunc_AddChatWindowChannel );
		lua_register( L, "RemoveChatWindowChannel",		LuaFunc_RemoveChatWindowChannel );

		lua_register( L, "Chat_GetMsnInfo",				LuaFunc_Chat_GetMsnInfo );
		lua_register( L, "Chat_SetMsnInfoOpen",			LuaFunc_Chat_SetMsnInfoOpen );
		lua_register( L, "Chat_SetMsnClose",			LuaFunc_Chat_SetMsnClose );
		lua_register( L, "Chat_AddMessage",				LuaFunc_Chat_AddMessage );
		lua_register( L, "Chat_ClearAllMsg",			LuaFunc_Chat_ClearAllMsg );
		lua_register( L, "Chat_Update",					LuaFunc_Chat_Update);
		lua_register( L, "Chat_CopyToClipboard",		LuaFunc_Chat_CopyToClipboard);
		//		lua_register( L, "Chat_SetTextColor",		LuaFunc_Chat_SetTextColor);
		//		lua_register( L, "Chat_GetTextColor",		LuaFunc_Chat_GetTextColor);


		lua_register(L, "ParseHyperlink",				LuaFunc_ParseHyperlink);	
		lua_register(L, "GetChannelColor",				LuaFunc_GetChannelColor);	

		lua_register(L, "VoiceChannel_InfoRequest",			LuaFunc_VoiceChannel_InfoRequest);	
		lua_register(L, "VoiceChannel_GetMemberCount",		LuaFunc_VoiceChannel_GetMemberCount);	
		lua_register(L, "VoiceChannel_GetMemberInfo",		LuaFunc_VoiceChannel_GetMemberInfo);	
		lua_register(L, "VoiceChannel_Join",				LuaFunc_VoiceChannel_Join);	
		lua_register(L, "VoiceChannel_Exit",				LuaFunc_VoiceChannel_Exit);	

		lua_register(L, "VoiceChannel_IsCurrentChannel",	LuaFunc_VoiceChannel_IsCurrentChannel);
		lua_register(L, "VoiceChannel_GetCurrentChannel",	LuaFunc_VoiceChannel_GetCurrentChannel);


		lua_register(L, "VoiceChannel_GetPlayerInfo",		LuaFunc_VoiceChannel_GetPlayerInfo);

		lua_register(L, "VoiceChannel_PartnerMute",		LuaFunc_VoiceChannel_PartnerMute);
	}
}

// ----------------------------------------------------------------------------
void CChatFrame::DefaultWindowInfo()
{
	for ( int i = 0; i < NUM_MAX_CHAT_WINDOWS; i++ )
	{
		m_windowInfo[i].typeList.clear();
		m_windowInfo[i].channelList.clear();
		m_windowInfo[i].name.clear();
		m_windowInfo[i].isLocked = true;
		m_windowInfo[i].isVisible = false;
		m_windowInfo[i].docked = 0;
		m_windowInfo[i].fontSize = 14;
		m_windowInfo[i].color.m_a = 0.6f;
		m_windowInfo[i].color.m_r = 0.13f;
		m_windowInfo[i].color.m_g = 0.12f;
		m_windowInfo[i].color.m_b = 0.11f;

		m_windowInfo[i].typeList.insert(eChatGroup_Channel);		
		// Default Chat Window
		switch (i)
		{
		case 0:			
			m_windowInfo[i].typeList.insert(eChatGroup_System);
			m_windowInfo[i].typeList.insert(eChatGroup_Say);
			m_windowInfo[i].typeList.insert(eChatGroup_Zone);
			m_windowInfo[i].typeList.insert(eChatGroup_Whisper);
			m_windowInfo[i].typeList.insert(eChatGroup_Party);
			m_windowInfo[i].typeList.insert(eChatGroup_Emote);
			m_windowInfo[i].typeList.insert(eChatGroup_Guild);			
			m_windowInfo[i].typeList.insert(eChatGroup_GM);
			m_windowInfo[i].typeList.insert(eChatGroup_GM_Talk);
			m_windowInfo[i].typeList.insert(eChatGroup_Yell);			
			m_windowInfo[i].typeList.insert(eChatGroup_BG);
			//m_windowInfo[i].typeList.push_back(eChatGroup_LFG);
			//m_windowInfo[i].typeList.push_back(eChatGroup_Sale);
			//m_windowInfo[i].typeList.push_back(eChatGroup_Loot);
			m_windowInfo[i].isVisible	= true;
			m_windowInfo[i].docked		= i+1;
			break;

		case 1:
			//m_windowInfo[i].typeList.insert(eChatGroup_System);
			m_windowInfo[i].typeList.insert(eChatGroup_Combat);
			m_windowInfo[i].isVisible	= true;
			m_windowInfo[i].docked		= i+1;
			break;

		case 2:
			//m_windowInfo[i].typeList.insert(eChatGroup_System);
			//m_windowInfo[i].typeList.insert(eChatGroup_Say);
			//m_windowInfo[i].typeList.insert(eChatGroup_Whisper);			
			//m_windowInfo[i].typeList.insert(eChatGroup_Raid);
			m_windowInfo[i].typeList.insert(eChatGroup_Party);
			m_windowInfo[i].isVisible	= true;
			m_windowInfo[i].docked		= i+1;
			break;

		case 3:
			//m_windowInfo[i].typeList.insert(eChatGroup_System);
			//m_windowInfo[i].typeList.insert(eChatGroup_Say);
			//m_windowInfo[i].typeList.insert(eChatGroup_Whisper);
			m_windowInfo[i].typeList.insert(eChatGroup_Guild);
			m_windowInfo[i].isVisible	= true;
			m_windowInfo[i].docked		= i+1;
			break;
		}
	}

	m_chatColor["SYSTEM"]		= 0xFFFF7F00;
	m_chatColor["SYSTEM_VALUE"]	= 0xFFFF7F00;
	m_chatColor["SYSTEM_GET"]	= 0xFFFF7F00;
	m_chatColor["SAY"]			= 0xFFFFFFFF;
	m_chatColor["ZONE"]			= 0xFFC4DF9B;
	m_chatColor["WHISPER"]		= 0xFFEA78DC;
	m_chatColor["PARTY"]		= 0xFF00BFF2;
	m_chatColor["RAID"]			= 0xFFF59978;
	m_chatColor["GUILD"]		= 0xFF40FF40;
	m_chatColor["YELL"]			= 0xFFC28EF0;
	m_chatColor["LFG"]			= 0xFFFFFFFF;
	m_chatColor["SALE"]			= 0xFFFFFFFF;
	m_chatColor["LOOT"]			= 0xFFFF7F00;
	m_chatColor["EMOTE"]		= 0xFFC7996E;
	m_chatColor["CHANNEL"]		= 0xFF00FFFF;
	m_chatColor["CHANNEL1"]		= 0xFF00FFFF;
	m_chatColor["CHANNEL2"]		= 0xFF00FFFF;
	m_chatColor["CHANNEL3"]		= 0xFF00FFFF;
	m_chatColor["CHANNEL4"]		= 0xFF00FFFF;
	m_chatColor["CHANNEL5"]		= 0xFF00FFFF;
	m_chatColor["CHANNEL6"]		= 0xFF00FFFF;
	m_chatColor["CHANNEL7"]		= 0xFF00FFFF;
	m_chatColor["CHANNEL8"]		= 0xFF00FFFF;
	m_chatColor["CHANNEL9"]		= 0xFF00FFFF;
	m_chatColor["COMBAT"]		= 0xFFE8E800;
	m_chatColor["GM"]			= 0xFFFF0000;
	m_chatColor["GM_TALK"]		= 0xFFE6E619;
	m_chatColor["BG"]			= 0xFFF59978;
}

// ----------------------------------------------------------------------------
void CChatFrame::Save()
{
	if ( m_interface == NULL || m_interface->IsLoadVariables() == false )
		return;

	FILE* fptr = NULL;

	// ChatFrame Setting
	{
		wstring globalName = GetGlobalPathW(L"ChatFrame.bsd");
		wstring localName = GetLocalPathW(L"ChatFrame.bsd");

		// 儲存共用資料			

		// 記錄資料	

		// 關閉檔案	

		// 儲存本地資料
		if ( (fptr = _wfopen(localName.c_str(), L"wb")) != NULL )
		{
			// 第一個檢查碼
			int code = CHAT_HEADER_CODE;
			fwrite(&code, sizeof(int), 1, fptr);

			// 頻道視窗資料
			{
				// 頻道個數
				int count = NUM_MAX_CHAT_WINDOWS;
				fwrite(&count, sizeof(int), 1, fptr);				

				// 儲存每筆資料
				for ( int i = 0; i < count; i++ )
				{
					ChatWindowSaveData data;

					// 頻道結構大小
					int size = sizeof(data);
					fwrite(&size, sizeof(int), 1, fptr);

					// 單筆結構
					strcpy(data.name, m_windowInfo[i].name.c_str());
					for ( set<int>::iterator iter = m_windowInfo[i].typeList.begin(); iter != m_windowInfo[i].typeList.end(); iter++ )
						data.typeList[*iter] = 1;
					for ( set<int>::iterator iter = m_windowInfo[i].channelList.begin(); iter != m_windowInfo[i].channelList.end(); iter++ )
						data.channelList[*iter] = 1;
					data.color		= m_windowInfo[i].color;
					data.isLocked	= m_windowInfo[i].isLocked;
					data.isVisible	= m_windowInfo[i].isVisible;
					data.docked		= m_windowInfo[i].docked;
					data.fontSize	= m_windowInfo[i].fontSize;
					fwrite(&data, sizeof(data), 1, fptr);
				}
			}			

			// 儲存顏色
			{
				// 儲存顏色碼個數
				int count = (int)m_chatColor.size();
				fwrite(&count, sizeof(int), 1, fptr);				

				// 儲存每筆資料
				for ( map<string, int>::iterator iter = m_chatColor.begin(); iter != m_chatColor.end(); iter++ )
				{
					ChatColorSaveData data;

					// 顏色碼單筆結構大小
					int size = sizeof(data);
					fwrite(&size, sizeof(int), 1, fptr);

					data.Set( (iter->first).c_str(), iter->second );
					fwrite(&data, sizeof(ChatColorSaveData), 1, fptr);
				}
			}

			// 關閉檔案
			fclose(fptr);
		}
	}	

	// Channel
	{
		wstring globalName = GetGlobalPathW(L"Channel.bsd");
		wstring localName = GetLocalPathW(L"Channel.bsd");

		// 儲存本地資料
		if ( (fptr = _wfopen(localName.c_str(), L"wb")) != NULL )
		{
			// 儲存頻道個數
			int count = (int)m_channel.size();
			fwrite(&count, sizeof(int), 1, fptr);			

			// 單第寫入
			for ( vector<UserDefineChannel>::iterator iter = m_channel.begin(); iter != m_channel.end(); iter++ )
			{
				// 儲存單筆資料大小
				int size = sizeof(UserDefineChannel);
				fwrite(&size, sizeof(int), 1, fptr);

				// 儲存結構
				fwrite(&(*iter), sizeof(UserDefineChannel), 1, fptr);
			}

			// 關閉檔案
			fclose(fptr);
		}
	}	
}

// ----------------------------------------------------------------------------
void CChatFrame::Load()
{	
	if ( m_loadServerData )
		return;

	FILE* fptr = NULL;	

	// ChatFrame Setting
	{
		wstring globalName = GetGlobalPathW(L"ChatFrame.bsd");
		wstring localName = GetLocalPathW(L"ChatFrame.bsd");

		// 讀取共用資料

		// 記錄資料	

		// 關閉檔案

		// 讀取本地資料
		FILE* fptr = NULL;
		if ( (fptr = _wfopen(localName.c_str(), L"rb")) != NULL )
		{					
			LoadWindowInfo(fptr);

			// 關閉檔案
			fclose(fptr);
		}
	}

	// Channel
	{
		wstring globalName = GetGlobalPathW(L"Channel.bsd");
		wstring localName = GetLocalPathW(L"Channel.bsd");

		// 讀取頻道資料
		if ( (fptr = _wfopen(localName.c_str(), L"rb")) != NULL )
		{
			// 讀取頻道個數
			int channelSize;
			fread(&channelSize, sizeof(int), 1, fptr);

			// 正常頻道數量
			if ( channelSize > 0 && channelSize <= 9 ) {				
				UserDefineChannel channelData;
				int size;

				// 讀取每一筆資訊
				for ( int i = 0; i < channelSize; i++ ) {

					// 判斷結構大小
					fread(&size, sizeof(int), 1, fptr);
					if ( size != sizeof(UserDefineChannel) )
						break;

					// 讀取單筆結構
					fread(&channelData, sizeof(UserDefineChannel), 1, fptr);

					// 檢查碼
					if ( channelData.header != CHAT_HEADER_CHECK_CODE )
						break;

					JoinChannelByName(channelData.name, channelData.password, channelData.id, false, true);
				}
			}

			// 關閉檔案
			fclose(fptr);
		}
	}
}

// ----------------------------------------------------------------------------
void CChatFrame::LoadWindowInfo(FILE* fptr)
{	
	int code, count, size;

	// 頻道視窗資料
	{
		// 第一個檢查碼
		fread(&code, sizeof(int), 1, fptr);
		if ( code != CHAT_HEADER_CODE )
			return;

		// 頻道個數
		fread(&count, sizeof(int), 1, fptr);
		if ( count != NUM_MAX_CHAT_WINDOWS )
			return;		

		// 頻道視窗資料
		ChatWindowSaveData data;
		for ( int i = 0; i < count; i++ )
		{
			// 頻道結構大小
			fread(&size, sizeof(int), 1, fptr);
			if ( size != sizeof(ChatWindowSaveData) )
				return;

			fread(&data, size, 1, fptr);
			if ( data.header != CHAT_HEADER_CHECK_CODE )
				return;

			m_windowInfo[i].typeList.clear();
			m_windowInfo[i].channelList.clear();
			m_windowInfo[i].name		= data.name;
			m_windowInfo[i].color		= data.color;
			m_windowInfo[i].isLocked	= data.isLocked;
			m_windowInfo[i].isVisible	= data.isVisible;
			m_windowInfo[i].docked		= data.docked;
			m_windowInfo[i].fontSize	= data.fontSize;

			if ( i == 0 ) {
				m_windowInfo[i].isVisible = true;
				m_windowInfo[i].docked = i + 1;
				m_windowInfo[i].typeList.insert(eChatGroup_BG);			// 強制加入戰場頻道
			}

			// Channel定義一定要加入
			m_windowInfo[i].typeList.insert(eChatGroup_Channel);
			for ( int n = 0; n < 32; n++ )
			{
				if ( data.typeList[n] )
					m_windowInfo[i].typeList.insert(n);

				if ( data.channelList[n] )
					m_windowInfo[i].channelList.insert(n);
			}
		}
	}

	// 儲存顏色
	{
		// 儲存顏色碼個數		
		fread(&count, sizeof(int), 1, fptr);
		if ( count < 0 || count > 99 )
			return;		

		ChatColorSaveData data;
		for ( int i = 0; i < count; i++ )
		{
			// 顏色碼單筆結構大小
			fread(&size, sizeof(int), 1, fptr);
			if ( size != sizeof(ChatColorSaveData) )
				return;

			fread(&data, size, 1, fptr);
			if ( data.header != CHAT_HEADER_CHECK_CODE )
				return;

			m_chatColor[data.chatType] = data.color;
		}
	}
}

// ----------------------------------------------------------------------------
void CChatFrame::SaveClientData()
{
	// 產生暫存資料
	CTempFile tempFile(65536);

	// 第一個檢查碼
	int code = CHAT_HEADER_CODE;
	tempFile.Write(&code, sizeof(int));

	// 頻道視窗資料
	{
		// 頻道個數
		int count = NUM_MAX_CHAT_WINDOWS;
		tempFile.Write(&count, sizeof(int));

		// 儲存每筆資料
		for ( int i = 0; i < count; i++ )
		{
			ChatWindowSaveData data;

			// 頻道結構大小
			int size = sizeof(data);
			tempFile.Write(&size, sizeof(int));

			// 單筆結構
			strcpy(data.name, m_windowInfo[i].name.c_str());
			for ( set<int>::iterator iter = m_windowInfo[i].typeList.begin(); iter != m_windowInfo[i].typeList.end(); iter++ )
				data.typeList[*iter] = 1;
			for ( set<int>::iterator iter = m_windowInfo[i].channelList.begin(); iter != m_windowInfo[i].channelList.end(); iter++ )
				data.channelList[*iter] = 1;
			data.color		= m_windowInfo[i].color;
			data.isLocked	= m_windowInfo[i].isLocked;
			data.isVisible	= m_windowInfo[i].isVisible;
			data.docked		= m_windowInfo[i].docked;
			data.fontSize	= m_windowInfo[i].fontSize;
			tempFile.Write(&data, sizeof(data));
		}
	}			

	// 儲存顏色
	{
		// 儲存顏色碼個數
		int count = (int)m_chatColor.size();
		tempFile.Write(&count, sizeof(int));

		// 儲存每筆資料
		for ( map<string, int>::iterator iter = m_chatColor.begin(); iter != m_chatColor.end(); iter++ )
		{
			ChatColorSaveData data;

			// 顏色碼單筆結構大小
			int size = sizeof(data);
			tempFile.Write(&size, sizeof(int));

			data.Set( (iter->first).c_str(), iter->second );
			tempFile.Write(&data, sizeof(ChatColorSaveData));
		}
	}

	// Channel
	{
		// 儲存頻道個數
		int count = (int)m_channel.size();
		tempFile.Write(&count, sizeof(int));

		// 單第寫入
		for ( vector<UserDefineChannel>::iterator iter = m_channel.begin(); iter != m_channel.end(); iter++ )
		{
			// 儲存單筆資料大小
			int size = sizeof(UserDefineChannel);
			tempFile.Write(&size, sizeof(int));

			// 儲存結構
			tempFile.Write(&(*iter), sizeof(UserDefineChannel));
		}
	}	

	g_pGameMain->SaveClientData(eSaveClientData_Chat, tempFile.GetPosition(), tempFile.GetData());

	// 將最後存檔時間去除
	SetSaveUpdateTime(0.0f);
}

// ----------------------------------------------------------------------------
void CChatFrame::LoadClientData(int type, int size, char* data)
{
	if ( type == eSaveClientData_Chat )
	{
		CTempFile tempFile(data, size);

		int code, count, structSize;

		// 第一個檢查碼
		tempFile.Read(&code, sizeof(int));
		if ( code != CHAT_HEADER_CODE )
			return;

		// 頻道視窗資料
		{
			// 頻道個數
			tempFile.Read(&count, sizeof(int));
			if ( count != NUM_MAX_CHAT_WINDOWS )
				return;

			// 頻道視窗資料
			ChatWindowSaveData windowData;
			for ( int i = 0; i < count; i++ )
			{
				// 頻道結構大小
				tempFile.Read(&structSize, sizeof(int));
				if ( structSize != sizeof(ChatWindowSaveData) )
					return;

				tempFile.Read(&windowData, structSize);
				if ( windowData.header != CHAT_HEADER_CHECK_CODE )
					return;

				m_windowInfo[i].typeList.clear();
				m_windowInfo[i].channelList.clear();
				m_windowInfo[i].name		= windowData.name;
				m_windowInfo[i].color		= windowData.color;
				m_windowInfo[i].isLocked	= windowData.isLocked;
				m_windowInfo[i].isVisible	= windowData.isVisible;
				m_windowInfo[i].docked		= windowData.docked;
				m_windowInfo[i].fontSize	= windowData.fontSize;

				if ( i == 0 ) {
					m_windowInfo[i].isVisible = true;
					m_windowInfo[i].docked = i + 1;
					m_windowInfo[i].typeList.insert(eChatGroup_BG);			// 強制加入戰場頻道
				}

				// Channel定義一定要加入
				m_windowInfo[i].typeList.insert(eChatGroup_Channel);
				for ( int n = 0; n < 32; n++ )
				{
					if ( windowData.typeList[n] )
						m_windowInfo[i].typeList.insert(n);

					if ( windowData.channelList[n] )
						m_windowInfo[i].channelList.insert(n);
				}
			}
		}

		// 儲存顏色
		{
			// 儲存顏色碼個數		
			tempFile.Read(&count, sizeof(int));
			if ( count < 0 || count > 99 )
				return;		

			ChatColorSaveData colorData;
			for ( int i = 0; i < count; i++ )
			{
				// 顏色碼單筆結構大小
				tempFile.Read(&structSize, sizeof(int));
				if ( structSize != sizeof(ChatColorSaveData) )
					return;

				tempFile.Read(&colorData, structSize);
				if ( colorData.header != CHAT_HEADER_CHECK_CODE )
					return;

				m_chatColor[colorData.chatType] = colorData.color;
			}
		}

		// 讀取頻道個數
		{
			int channelSize;
			tempFile.Read(&channelSize, sizeof(int));

			// 正常頻道數量
			if ( channelSize > 0 && channelSize <= 9 )
			{
				UserDefineChannel channelData;

				// 讀取每一筆資訊
				for ( int i = 0; i < channelSize; i++ ) {

					// 判斷結構大小
					tempFile.Read(&structSize, sizeof(int));
					if ( structSize != sizeof(UserDefineChannel) )
						break;

					// 讀取單筆結構					
					tempFile.Read(&channelData, sizeof(UserDefineChannel));

					// 檢查碼
					if ( channelData.header != CHAT_HEADER_CHECK_CODE )
						break;

					JoinChannelByName(channelData.name, channelData.password, channelData.id, false, true);
				}
			}
		}

		m_loadServerData = true;
	}
}

// ----------------------------------------------------------------------------
void CChatFrame::BuildFrames()
{
	if (g_pMsnManager)
	{
		g_pMsnManager->Initial(m_interface->GetUi());

		if (g_pGameMain)
			g_pMsnManager->SetSelfName(g_pGameMain->GetPlayerName());
	}

	DefaultWindowInfo();
}
// ----------------------------------------------------------------------------
void CChatFrame::DestroyFrames()
{
	m_loadServerData = false;
	m_channel.clear();
	//m_stVoiceChannelInfos.clear();
	//m_stVoiceChannelInfo.isDirty; // nothing assign ?! marked by slime.
	if (g_pMsnManager)
		g_pMsnManager->Finish();
}

// ----------------------------------------------------------------------------
void CChatFrame::Update(float elapsedTime)
{
	if (g_pMsnManager)
		g_pMsnManager->Update(elapsedTime);

	// 固定間隔做儲存
	SetSaveUpdateTime();
}

// ----------------------------------------------------------------------------
void CChatFrame::LeaveWorld()
{
	ExitVoiceChannel( );
}

// ----------------------------------------------------------------------------
// 送出訊息到交談視窗(畫面左下角位置)
// type CHAT_MSG_SYSTEM, CHAT_MSG_SYSTEM_GET, CHAT_MSG_SYSTEM_VALUE, CHAT_MSG_SAY..
// sender 發話者,代入 NULL 代表無發話者
// msg 發話內容
void CChatFrame::SendChatMessage(const char* type, const char* sender, const char* msg, int vocation)
{
	lua_State* L = GetLuaStateWorld();

	if ( L == NULL || type == NULL )
		return;	

	// arg1
	if ( strncmp(msg, "[$MUTE]", 7) == 0 ) {
		lua_pushstring(L, msg + 7);
	} else {
		lua_pushstring(L, msg);
	}
	lua_setglobal(L, "arg1");

	// arg2	and arg4 and arg5
	{
		const char* talker;

		if ( sender[0] == NPC_HEADER )
			talker = sender+1;
		else
			talker = sender;

		// arg4 Player Name
		{
			lua_pushstring(L, talker);
			lua_setglobal(L, "arg4");
		}

		// arg5 sayer class
		if ( vocation >= 0 )
		{
			const char* classToken = g_ObjectData->GetClassTokenByID((Voc_ENUM)vocation);
			if ( strlen(classToken) > 0 )
			{
				lua_pushstring(L, classToken);
				lua_setglobal(L, "arg5");
			}
		}

		// hyperlink name
		string playerName;

		if ( PlayerName_To_HyperLink(sender, playerName) ) {
			lua_pushstring(L, playerName.c_str());
		} else {
			lua_pushstring(L, (sender) ? sender : "");
		}
		lua_setglobal(L, "arg2");
	}	

	// arg3 channel id
	if ( strncmp(type, CHAT_MSG_CHANNEL, strlen(CHAT_MSG_CHANNEL)) == 0 )
	{		
		int channelID = GetChannelID(type + strlen(CHAT_MSG_CHANNEL));
		lua_pushnumber(L, channelID);
		lua_setglobal(L, "arg3");

		type = CHAT_MSG_CHANNEL;
	}	

	SendWorldEvent(type);

	// 清除用過的參數
	for ( int i = 0; i < 10; i++ )
	{
		char temp[512];
		sprintf_s(temp, 512, "arg%d", i);
		lua_pushnil(L);
		lua_setglobal(L, temp);
	}	
}

// ----------------------------------------------------------------------------
void CChatFrame::SendChatMessageEx(const char* type, const char* sender, const char* format, ...)
{
	// 字串轉換
	static char buf[4096];	

	va_list args;
	va_start(args, format);
	SwapStringExA(buf, format, args);
	// vsprintf_s(buf, 4096, format, args);
	va_end(args);

	SendChatMessage(type, sender, buf);
}

// ----------------------------------------------------------------------------
void CChatFrame::AddOfflineMessage(const char* sender, const char* msg)
{
	ChatOfflineMessage offline;
	offline.sender = sender;
	offline.msg = msg;
	m_offlineMessage.push_back(offline);
}

// ----------------------------------------------------------------------------
void CChatFrame::ShowOfflineMessage()
{
	for ( vector<ChatOfflineMessage>::iterator iter = m_offlineMessage.begin(); iter != m_offlineMessage.end(); iter++ )
	{
		SendChatMessage(CHAT_MSG_WHISPER_OFFLINE, iter->sender.c_str(), iter->msg.c_str());
	}
	m_offlineMessage.clear();
}

// ----------------------------------------------------------------------------
string CChatFrame::AddClassColor(const char* name, int voc)
{
	if ( name != NULL )
	{
		char temp[512];
		int color = g_ObjectData->GetClassColorByID((Voc_ENUM)voc);
		sprintf_s(temp, 512, "|c%08x%s|r", color, name);
		return temp;
	}
	return "";
}

// ----------------------------------------------------------------------------
// 送出訊息到交談視窗(畫面左下角位置)
// type CHAT_MSG_SYSTEM, CHAT_MSG_SYSTEM_GET, CHAT_MSG_SYSTEM_VALUE, CHAT_MSG_SAY..
// sender 發話者,代入 NULL 代表無發話者
// msg 發話內容
void CChatFrame::SendMsgEvent(const char* type, const char* msg, const char* sender)
{	
	SendChatMessage(type, sender, msg);
}

// ----------------------------------------------------------------------------
void CChatFrame::AddMessage(int channel, const char* text)
{
	SendChatMessage(CHAT_MSG_SYSTEM, "", text);
}

// ----------------------------------------------------------------------------
ChatWindowInfo*	CChatFrame::GetChatWindowInfo(int id)
{
	if ( (unsigned int)id < NUM_MAX_CHAT_WINDOWS )
	{
		return &m_windowInfo[id];
	}
	return NULL;
}
// ----------------------------------------------------------------------------
int	CChatFrame::JoinChannelByName(const char* name, const char* password, int channel, bool isCreate, bool isForce)
{
	if ( name == NULL )
		name = "";
	if ( password == NULL )
		password = "";

	// 超過最大自定頻道數量
	if ( m_channel.size() >= 9 )
		return 0;

	// 檢查名稱是否存在	
	vector<UserDefineChannel>::iterator iter;
	for ( iter = m_channel.begin(); iter != m_channel.end(); iter++ )
	{
		// 頻道名稱大小寫不同
		if ( strcmp(name, iter->name) == 0 )
			return 0;
	}

	bool resetID = true;

	// 檢查是否有相同編號ID
	if ( channel > 0 && channel <= 9 ) {
		for ( iter = m_channel.begin(); iter != m_channel.end(); iter++ )
		{
			if ( iter->id == channel )
				break;
		}
		if ( iter == m_channel.end() ) {
			resetID = false;
		}
	}

	if ( resetID )
	{
		// 並取得可用的頻道編號 1 - 9
		channel = 1;
		iter = m_channel.begin();
		while ( iter != m_channel.end() )
		{
			if ( iter->id == channel ) {
				channel++;
				iter = m_channel.begin();
			} else {
				iter++;
			}
		}
	}

	// 先加入頻道與名稱,在接收回應封包決定是否成?
	UserDefineChannel channelData;
	channelData.id = channel;
	strcpy(channelData.name, name);
	strcpy(channelData.password, password);
	m_channel.push_back(channelData);
	sort(m_channel.begin(), m_channel.end());

	if ( isCreate )
		NetCli_Channel::S_CreateChannel( name, password );			// 送出封包
	else
		NetCli_Channel::S_JoinChannel( name, password, isForce );			// 送出封包


	return channel;
}

// ----------------------------------------------------------------------------
int CChatFrame::LeaveChannelByID(int id, bool receive)
{
	for ( vector<UserDefineChannel>::iterator iter = m_channel.begin(); iter != m_channel.end(); iter++ )
	{
		if ( iter->id == id )
		{
			// 檢查是否已經加入Chat Server
			int channelID = NetCli_ChannelChild::GetChannelID( (*iter).name );
			if ( channelID >= 0 )
			{
				NetCli_Channel::S_LeaveChannel( EM_GroupObjectType_PlayerChannel, channelID );
			}

			if ( receive )
			{
				m_channel.erase( iter );
				SendChannelLeave(id);
 			}
			return id;
		}
	}
	return 0;
}

// ----------------------------------------------------------------------------
int	CChatFrame::LeaveChannelByName(const char* name, bool receive)
{
	if ( name == NULL )
		return 0;


	//判斷是不是目前語音頻道
	if( GetVoiceChannelType() == EVCT_USER && 
		0 == strcmp( GetVoiceChannelName(), name ) )
	{
		ExitVoiceChannel( );
	}


	int cid=-1;
	g_pMsnManager-> CloseMsn(name);

	for ( vector<UserDefineChannel>::iterator iter = m_channel.begin(); iter != m_channel.end(); iter++ )
	{
		if ( strcmp(iter->name, name) == 0 )
			return LeaveChannelByID(iter->id, receive);
	}
 
	return 0;
}
const char*	CChatFrame::GetChannelPass(int id)
{

	for ( vector<UserDefineChannel>::iterator iter = m_channel.begin(); iter != m_channel.end(); iter++ )
	{
		if ( iter->id == id )
		{
			return iter->password;
		}
	}
	return NULL;
}
// ----------------------------------------------------------------------------
void CChatFrame::SendChannelLeave(int id)
{
	// 通知事件,離開頻道
	lua_State* L = GetLuaStateWorld();
	if ( L )
	{
		lua_pushnumber(L, id);
		lua_setglobal(L, "arg1");
		g_pChatFrame->SendWorldEvent(UIEVENT_CHAT_MSG_CHANNEL_LEAVE);
	}
}
// ----------------------------------------------------------------------------
void CChatFrame::ReceiveJoinChannel(const char* name)
{
	lua_State* L = GetLuaStateWorld();
	int id = GetChannelID(name);
	if ( id > 0 ) {
		m_windowInfo[0].channelList.insert(id);
	}


	int cid=	NetCli_ChannelChild::GetChannelID(name);

	const char *Leader=NetCli_ChannelChild::GetChannelLeader( cid);
	const char *Pass=	GetChannelPass(id);
	g_pMsnManager-> JoinChannelOK(name, id,Pass);
	g_pMsnManager-> ChangeChannelOwner(name, id,Leader);

	lua_pushnumber( L, id);	
	lua_setglobal( L, "arg1" );
	lua_pushstring( L, name);	
	lua_setglobal( L, "arg2" );
	g_pChatFrame->SendWorldEvent(UIEVENT_CHAT_MSG_CHANNEL_JOIN);

}

// ----------------------------------------------------------------------------
const char* CChatFrame::GetChannelName(int id)
{
	for ( vector<UserDefineChannel>::iterator iter = m_channel.begin(); iter != m_channel.end(); iter++ )
	{
		if ( iter->id == id )
			return iter->name;
	}
	return "";
}

// ----------------------------------------------------------------------------
int	CChatFrame::GetChannelID(const char* name)
{
	if ( name == NULL )
		name = "";

	for ( vector<UserDefineChannel>::iterator iter = m_channel.begin(); iter != m_channel.end(); iter++ )
	{
		if ( strcmp(iter->name, name) == 0 )
			return iter->id;
	}
	return 0;
}

// ----------------------------------------------------------------------------
// 回傳伺服器所記錄的CHANNEL ID
// -1 失敗
int CChatFrame::GetSendChannelID(int id)
{
	vector<UserDefineChannel>::iterator iter = m_channel.begin();
	for ( ; iter != m_channel.end(); iter++ )
	{
		if ( iter->id == id )
			break;
	}

	if ( iter != m_channel.end() )
	{
		int channelID = NetCli_ChannelChild::GetChannelID( iter->name );
		if ( channelID >= 0 )
		{
			return channelID;
		}
	}
	return -1;
}

// ----------------------------------------------------------------------------
bool CChatFrame::SendChannelMsg(int id, const char* playerName, const char* msg)
{
	vector<UserDefineChannel>::iterator iter = m_channel.begin();
	for ( ; iter != m_channel.end(); iter++ )
	{
		if ( iter->id == id )
			break;
	}

	if ( iter != m_channel.end() )
	{
		int channelID = NetCli_ChannelChild::GetChannelID( iter->name );
		if ( channelID >= 0 )
		{
			// NetCli_Talk::Channel(EM_GroupObjectType_PlayerChannel, channelID, (char*)playerName, (char*)msg );
			return true;
		}
	}
	return false;
}
// ----------------------------------------------------------------------------
bool CChatFrame::SetChannelColor(const char* channel, int color)
{
	g_pChatFrame->SendWorldEvent(UIEVENT_CHAT_MSG_CHANNEL_COLOR_CHANGE);
	map<string, int>::iterator iter = m_chatColor.find(channel);
	if ( iter != m_chatColor.end() )
	{
		iter->second = color;

		return true;
	}
	return false;
}
// ----------------------------------------------------------------------------
int	CChatFrame::GetChannelColor(const char* channel)
{
	map<string, int>::iterator iter = m_chatColor.find(channel);
	if ( iter != m_chatColor.end() )
		return iter->second;
	return 0;
}
// ----------------------------------------------------------------------------
void CChatFrame::HaveNewWhisper(const char *who, const char *msg,int type)
{
	g_pMsnManager->HaveNewWhisper(who, msg, type);
}


// ----------------------------------------------------------------------------
void CChatFrame:: CreateChannelOK(const char *name,int channelNo)
{
	lua_State* L = GetLuaStateWorld();
	int id=	GetChannelID(name);
	lua_pushnumber( L, id);	
	lua_setglobal( L, "arg1" );
	g_pChatFrame->SendWorldEvent(UIEVENT_CHAT_MSG_CHANNEL_CREATE);


	g_pMsnManager->CreateChannelOK(name, id);


}

void CChatFrame::DelChannelMember(const char* ChannelName, int channelNo, const char* MemberName)
{
	g_pMsnManager->DelChannelMember(ChannelName, channelNo, MemberName);

	if (ChannelName && MemberName && (m_stVoiceChannelInfo.ChannelName == ChannelName))
	{
        std::vector<std::string>::iterator itPos = m_stVoiceChannelInfo.vecMemberNameList.begin();
		std::vector<std::string>::iterator itEnd = m_stVoiceChannelInfo.vecMemberNameList.end();
		for ( ; itPos != itEnd ; ++itPos )
		{
			if ( (*itPos) == MemberName )
			{
				m_stVoiceChannelInfo.vecMemberNameList.erase( itPos );
				break; // only erase first one found.
			}
		}

		lua_State* L = GetLuaState();
		lua_pushstring( L, "user" );
		lua_setglobal( L, "arg1" );
		lua_pushstring( L, ChannelName );
		lua_setglobal( L, "arg2" );
		SendWorldEvent( UiEvent_VoiceChannelMemberUpdate );
	}
}

void CChatFrame::AddChannelMember(const char* ChannelName, int channelNo, const char* MemberName)
{
	g_pMsnManager->AddChannelMember(ChannelName,channelNo, MemberName);

	if (ChannelName && MemberName && (m_stVoiceChannelInfo.ChannelName == ChannelName))
	{
		m_stVoiceChannelInfo.vecMemberNameList.push_back( MemberName );

		lua_State* L = GetLuaState();
		lua_pushstring( L, "user" );
		lua_setglobal( L, "arg1" );
		lua_pushstring( L, ChannelName );
		lua_setglobal( L, "arg2" );
		SendWorldEvent( UiEvent_VoiceChannelMemberUpdate );
	}
}

void CChatFrame::R_ChannelPlayerListResult( GroupObjectTypeEnum Type , int ChannelID , std::vector<std::string> &Name )
{
	const char* szChannelName = NetCli_ChannelChild::GetChannelNameByID( ChannelID );
	for (unsigned int i = 0; i < Name.size(); ++i)
	{
		g_pMsnManager->AddChannelMember( szChannelName, ChannelID, Name[i].c_str() );
	}

	if (m_stVoiceChannelInfo.eType == EVCT_USER &&
        m_stVoiceChannelInfo.ChannelName == szChannelName)
	{
		m_stVoiceChannelInfo.bSent = false;
		m_stVoiceChannelInfo.LastTime = timeGetTime();
		m_stVoiceChannelInfo.bDirty = false;

		m_stVoiceChannelInfo.vecMemberNameList.clear();

		for (unsigned int i = 0; i < Name.size(); ++i)
		{
			m_stVoiceChannelInfo.vecMemberNameList = Name; // Notice: don't know why need loop here, by slime.
		}

		lua_State* L = GetLuaState();
		lua_pushstring( L, "user" );
		lua_setglobal( L, "arg1" );
		lua_pushstring( L, szChannelName );
		lua_setglobal( L, "arg2" );
		SendWorldEvent( UiEvent_VoiceChannelMemberUpdate );
	}
}

void CChatFrame::GetVoiceChannelToken( e_VoiceChannelType etype, const string& strName, string& outToken )
{

}

void CChatFrame::JoinVoiceChannel( e_VoiceChannelType etype, const string& strName )
{
	return;
	m_eVoiceChannelType = EVCT_NONE;
	m_strVoiceChannelName.clear();

	string strVoiceChannelName;

	GetVoiceChannelToken( etype, strName, strVoiceChannelName );


	lua_State* L = GetLuaState();
	SendWorldEvent( UiEvent_VoiceChannelUpdate );
}

void CChatFrame::ExitVoiceChannel( )
{
	m_eVoiceChannelType = EVCT_NONE;
	m_strVoiceChannelName.clear();

	lua_State* L = GetLuaState();
	SendWorldEvent( UiEvent_VoiceChannelUpdate );
}

void CChatFrame::VoiceChannelInfoRequest( e_VoiceChannelType type, const char* name )
{
	if( type == EVCT_GUILD )
	{
		int GuildID = CNetGlobal::RoleData()->BaseData.GuildID;
		if( GuildID <= 0 )
		{
			return;
		}

		m_stVoiceChannelInfo.eType = type;
		m_stVoiceChannelInfo.ChannelName = name;
		m_stVoiceChannelInfo.bSent = false;
		m_stVoiceChannelInfo.LastTime = timeGetTime();
		m_stVoiceChannelInfo.bDirty = false;

		m_stVoiceChannelInfo.vecMemberNameList.clear();

		GuildMemberListInfo* pMemberInfo;
		int count = g_pGuildFrame->GetNumGuildMembers();
		
		for( int i = 0 ; i < count ; ++i )
		{
			pMemberInfo = g_pGuildFrame->GetGuildRosterInfo( i );

			if( pMemberInfo->member->IsOnline )
			{
				m_stVoiceChannelInfo.vecMemberNameList.push_back( pMemberInfo->member->MemberName.Begin() );
			}
		}

		lua_State* L = GetLuaState();
		lua_pushstring( L, "guild" );
		lua_setglobal( L, "arg1" );
		lua_pushstring( L, name );
		lua_setglobal( L, "arg2" );
		SendWorldEvent( UiEvent_VoiceChannelMemberUpdate );
	}
	else if( type == EVCT_PARTY )
	{
		PartyBaseInfoStruct& rPartyInfo = NetCli_PartyChild::GetPartyInfo();

		if( rPartyInfo.PartyID == -1 )
		{
			return ;
		}

		m_stVoiceChannelInfo.eType = type;
		m_stVoiceChannelInfo.ChannelName = name;
		m_stVoiceChannelInfo.bSent = false;
		m_stVoiceChannelInfo.LastTime = timeGetTime();
		m_stVoiceChannelInfo.bDirty = false;
		m_stVoiceChannelInfo.vecMemberNameList.clear();

		PartyMemberInfo* pMemberInfo;

		int count = (int)NetCli_PartyChild::s_member.size();
		for( int i = 0 ; i < count ; ++i )
		{
			pMemberInfo = NetCli_PartyChild::GetGroupMember( i );

			if( pMemberInfo->GItemID != -1 )
			{
				m_stVoiceChannelInfo.vecMemberNameList.push_back( NetCli_OtherChild::GetRoleName( pMemberInfo->DBID ) );	
			}
		}

		lua_State* L = GetLuaState();
		lua_pushstring( L, "party" );
		lua_setglobal( L, "arg1" );
		lua_pushstring( L, name );
		lua_setglobal( L, "arg2" );
		SendWorldEvent( UiEvent_VoiceChannelMemberUpdate );
	}
	else if( type == EVCT_USER )
	{
		if( m_stVoiceChannelInfo.eType == type &&
			m_stVoiceChannelInfo.ChannelName == name )
		{
			if( m_stVoiceChannelInfo.bSent ) //已發封包
			{
				return;
			}

			if( m_stVoiceChannelInfo.LastTime > 0 && timeGetTime() - m_stVoiceChannelInfo.LastTime < 1000 )
			{
				return;
			}
		}
		
		//封包太密集
		if( m_stVoiceChannelInfo.LastTime > 0 && timeGetTime() - m_stVoiceChannelInfo.LastTime < 1000 )
		{
			return;
		}

		m_stVoiceChannelInfo.bDirty = true;
		m_stVoiceChannelInfo.eType = type;
		m_stVoiceChannelInfo.ChannelName = name;
		m_stVoiceChannelInfo.bSent = true;

	 	int ChannelID = NetCli_ChannelChild::GetChannelID( name );
		
		NetCli_Channel::S_ChannelPlayerList( EM_GroupObjectType_PlayerChannel, ChannelID );
	}

}

s_VoiceChannelInfo* CChatFrame::GetVoiceChannelInfo( e_VoiceChannelType type, const char* name )
{
	if( m_stVoiceChannelInfo.eType == type &&
		m_stVoiceChannelInfo.ChannelName == name && 
		m_stVoiceChannelInfo.bDirty == false )
	{
		return &m_stVoiceChannelInfo;
	}

	return NULL;
}

// Cli to Cli 邀請加入頻道
void CChatFrame::R_CtoC_ChannelInvite( const char* AskerName, const char* channelName, const char* password )
{
	lua_State* L = GetLuaState();
	lua_pushstring( L, channelName );
	lua_setglobal( L, "arg1" );
	lua_pushstring( L, password );
	lua_setglobal( L, "arg2" );
	lua_pushstring( L, AskerName );
	lua_setglobal( L, "arg3" );
	SendWorldEvent( UiEvent_InviteToChannel );
}

void CChatFrame::ChangeChannelOwner(const char *name,int channelNo,const char *LeaderName)
{
	g_pMsnManager->ChangeChannelOwner(name,channelNo, LeaderName);
}
// ----------------------------------------------------------------------------
void CChatFrame::HaveNewChannel(int Type,int channelNo,const char *who, const char *msg,int type)
{

	const char*		name=0;
	int MsnType=0;
	int channelID=0;
	switch (Type)
	{
	case EM_GroupObjectType_Party:
		MsnType=MT_PARTY;
		break;
	case EM_GroupObjectType_Guild:
		MsnType=MT_GUILD;
		break;
	case EM_GroupObjectType_SysChannel:
		MsnType=MT_YELL;
		break;
	case EM_GroupObjectType_PlayerChannel:
		{
			MsnType		=	MT_CHANNEL;
			name		=	NetCli_ChannelChild::GetChannelNameByID(channelNo);
			channelID	=	GetChannelID(name);
		}

		break;

	case EM_GroupObjectType_SysZoneChannel:
		MsnType=MT_ZONE;
		break;



	}
	g_pMsnManager->HaveNewChannel(MsnType,channelID, who, msg,type);
}

// ----------------------------------------------------------------------------
int LuaFunc_GetChatWindowInfo(lua_State* L)
{
	ChatWindowInfo* info = g_pChatFrame->GetChatWindowInfo(luaL_checkint(L, 1) -1);
	if ( info )
	{		
		lua_pushstring(L, info->name.c_str() );
		lua_pushnumber(L, info->fontSize );		
		lua_pushnumber(L, info->color.m_r );
		lua_pushnumber(L, info->color.m_g );
		lua_pushnumber(L, info->color.m_b );
		lua_pushnumber(L, info->color.m_a );
		lua_pushboolean(L, info->isVisible);
		lua_pushboolean(L, info->isLocked);
		if ( info->docked > 0 )
			lua_pushnumber(L, info->docked);
		else
			lua_pushboolean(L, 0);
		return 9;
	}
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GetChatWindowMessages(lua_State* L)
{
	ChatWindowInfo* info = g_pChatFrame->GetChatWindowInfo(luaL_checkint(L, 1) - 1);
	int size = 0;
	if ( info )
	{		
		for ( set<int>::iterator iter = info->typeList.begin(); iter != info->typeList.end(); iter++ )
		{
			lua_pushstring(L, ChatWindowInfo::GetTypeListText(*iter));
			size++;
		}
	}
	return size;
}

// ----------------------------------------------------------------------------
int	LuaFunc_GetChatWindowChannels(lua_State* L)
{
	ChatWindowInfo* info = g_pChatFrame->GetChatWindowInfo(luaL_checkint(L, 1) - 1);
	int size = 0;
	if ( info )
	{
		for ( set<int>::iterator iter = info->channelList.begin(); iter != info->channelList.end(); iter++ )
		{
			lua_pushnumber(L, *iter);
			lua_pushstring(L, g_pChatFrame->GetChannelName(*iter));
			size += 2;
		}
	}
	return size;
}

// ----------------------------------------------------------------------------
// arg1 channel name
// arg2 channel password
int LuaFunc_CreateChannel(lua_State* L)
{
	const char* name = luaL_checkstring(L, 1);
	const char* password = "";

	if ( lua_isstring(L, 2) )
		password = lua_tostring(L, 2);

	lua_pushnumber(L, g_pChatFrame->JoinChannelByName(name, password, -1, true));
	return 1;
}

// ----------------------------------------------------------------------------
// arg1 channel name
// arg2 channel password
int LuaFunc_JoinChannel(lua_State* L)
{
	const char* name = luaL_checkstring(L, 1);
	const char* password = "";

	if ( lua_isstring(L, 2) )
		password = lua_tostring(L, 2);

	lua_pushnumber(L, g_pChatFrame->JoinChannelByName(name, password, -1, false));
	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_LeaveChannel(lua_State* L)
{
	const char* channel = luaL_checkstring(L, 1);
	if ( channel[0] == 0 )
		return 0;

	if ( strlen(channel) == 1 && *channel >= '1' && *channel <= '9' )
		g_pChatFrame->LeaveChannelByID(atoi(channel));	
	else
		g_pChatFrame->LeaveChannelByName(channel);

	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_ChannelKick(lua_State* L)
{
	const char* channel = luaL_checkstring(L, 1);
	const char* playerName = luaL_checkstring(L, 2);
	if ( channel[0] == 0 || playerName[0] == 0 )
		return 0;

	if ( strlen(channel) == 1 && *channel >= '1' && *channel <= '9' ) {
		channel = g_pChatFrame->GetChannelName(atoi(channel));
	}

	NetCli_ChannelChild::ChannelKick(channel, playerName);
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_ChannelInvite(lua_State* L)
{
	const char* channel = luaL_checkstring(L, 1);
	const char* playerName = luaL_checkstring(L, 2);
	const char* password = 0;
	if ( channel[0] == 0 || playerName[0] == 0 )
		return 0;

	if ( strlen(channel) == 1 && *channel >= '1' && *channel <= '9' ) {
		channel = g_pChatFrame->GetChannelName(atoi(channel));
	}
 
	vector<UserDefineChannel>& channelList = g_pChatFrame->GetChannelList();

	vector<UserDefineChannel>::iterator itPos = channelList.begin();
	vector<UserDefineChannel>::iterator itEnd = channelList.end();

	string strPassword;

	for( ; itPos != itEnd ; ++itPos )
	{
		UserDefineChannel& info = *itPos;

		if( 0 == strcmp( info.name , channel ) )
		{
			strPassword = info.password;
			break;
		}
	}

	//NetCli_ChannelChild::ChannelKick(channel, playerName);
	NetCli_ChannelChild::ChannelInvite( playerName, channel, strPassword.c_str() );

	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_SetChannelOwner(lua_State* L)
{
	const char* channel = luaL_checkstring(L, 1);
	const char* playerName = luaL_checkstring(L, 2);
	if ( channel[0] == 0 || playerName[0] == 0 )
		return 0;

	if ( strlen(channel) == 1 && *channel >= '1' && *channel <= '9' ) {
		channel = g_pChatFrame->GetChannelName(atoi(channel));
	}

	NetCli_ChannelChild::SetChannelOwner(channel, playerName);
	return 0;
}


// ----------------------------------------------------------------------------
int LuaFunc_GetChannelOwner(lua_State* L)
{
	const char* channel = luaL_checkstring(L, 1);
	if ( channel[0] == 0 )
		return 0;

	if ( strlen(channel) == 1 && *channel >= '1' && *channel <= '9' ) {
		channel = g_pChatFrame->GetChannelName(atoi(channel));
	}

	int channelID = NetCli_ChannelChild::GetChannelID( channel );	

	const char* channelLeader = NetCli_ChannelChild::GetChannelLeader( channelID );

	lua_pushstring( L, channelLeader );

	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_GetChannelName(lua_State* L)
{
	int channel = luaL_checkint(L, 1);
	lua_pushstring(L, g_pChatFrame->GetChannelName(channel));
	return 1;
}

// ----------------------------------------------------------------------------
int	LuaFunc_GetChannelList(lua_State* L)
{
	int size = 0;
	vector<UserDefineChannel>& channelList = g_pChatFrame->GetChannelList();
	for ( vector<UserDefineChannel>::iterator iter = channelList.begin(); iter != channelList.end(); iter++ )
	{
		lua_pushnumber(L, iter->id);
		lua_pushstring(L, iter->name);
		size++;
	}
	return size * 2;
}

// ----------------------------------------------------------------------------
int LuaFunc_SetChatWindowName(lua_State* L)
{
	ChatWindowInfo* info = g_pChatFrame->GetChatWindowInfo(luaL_checkint(L, 1) -1);
	if ( info )
	{
		info->name = luaL_checkstring(L, 2);
	}
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_SetChatWindowColor(lua_State* L)
{
	ChatWindowInfo* info = g_pChatFrame->GetChatWindowInfo(luaL_checkint(L, 1) -1);
	if ( info )
	{
		info->color.m_r = (float)luaL_checknumber(L, 2);
		info->color.m_g = (float)luaL_checknumber(L, 3);
		info->color.m_b = (float)luaL_checknumber(L, 4);
	}
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_SetChatWindowAlpha(lua_State* L)
{
	ChatWindowInfo* info = g_pChatFrame->GetChatWindowInfo(luaL_checkint(L, 1) -1);
	if ( info )
	{
		info->color.m_a = (float)luaL_checknumber(L, 2);
	}
	return 0;
}

// ----------------------------------------------------------------------------
int	LuaFunc_SetChatWindowShown(lua_State* L)
{
	int id = luaL_checkint(L, 1) -1;
	if ( id > 0 )
	{
		ChatWindowInfo* info = g_pChatFrame->GetChatWindowInfo(id);
		if ( info )
		{
			if ( lua_isboolean(L, 2) && lua_toboolean(L, 2) )
				info->isVisible = true;
			else
				info->isVisible = false;
		}
	}
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_SetChatWindowLocked(lua_State* L)
{
	ChatWindowInfo* info = g_pChatFrame->GetChatWindowInfo(luaL_checkint(L, 1) -1);
	if ( info )
	{
		if ( lua_isboolean(L, 2) && lua_toboolean(L, 2) )
			info->isLocked = true;
		else
			info->isLocked = false;
	}
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_SetChatWindowDocked(lua_State* L)
{
	ChatWindowInfo* info = g_pChatFrame->GetChatWindowInfo(luaL_checkint(L, 1) -1);
	if ( info )
	{
		switch ( lua_type(L, 2) )
		{
		case LUA_TNUMBER:
			info->docked = (int)lua_tonumber(L, 2);
			break;

		default:
			info->docked = 0;
			break;
		}
	}
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_SetChatWindowSize(lua_State* L)
{
	ChatWindowInfo* info = g_pChatFrame->GetChatWindowInfo(luaL_checkint(L, 1) -1);
	if ( info )
	{
		info->fontSize = luaL_checkint(L, 2);
	}
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_AddChatWindowMessages(lua_State* L)
{
	ChatWindowInfo* info = g_pChatFrame->GetChatWindowInfo(luaL_checkint(L, 1) -1);
	if ( info )
	{
		int typeNum = info->GetTypeListNum(luaL_checkstring(L, 2));
		if ( typeNum >= 0 )
			info->typeList.insert(typeNum);
	}
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_RemoveChatWindowMessages(lua_State* L)
{
	ChatWindowInfo* info = g_pChatFrame->GetChatWindowInfo(luaL_checkint(L, 1) -1);
	if ( info )
	{
		int typeNum = info->GetTypeListNum(luaL_checkstring(L, 2));
		if ( typeNum >= 0 )
			info->typeList.erase(typeNum);
	}
	return 0;
}

// ----------------------------------------------------------------------------
int	LuaFunc_AddChatWindowChannel(lua_State* L)
{
	ChatWindowInfo* info = g_pChatFrame->GetChatWindowInfo(luaL_checkint(L, 1) -1);
	int channel = 0;
	if ( info )
	{
		if ( (channel = g_pChatFrame->GetChannelID(luaL_checkstring(L, 2))) > 0 )
		{
			if ( info->channelList.find(channel) == info->channelList.end() )
				info->channelList.insert(channel);
		}
	}
	lua_pushnumber(L, channel);
	return 1;
}

// ----------------------------------------------------------------------------
int	LuaFunc_RemoveChatWindowChannel(lua_State* L)
{
	ChatWindowInfo* info = g_pChatFrame->GetChatWindowInfo(luaL_checkint(L, 1) -1);
	int channel = 0;
	if ( info )
	{
		if ( (channel = g_pChatFrame->GetChannelID(luaL_checkstring(L, 2))) > 0 )
		{
			if ( info->channelList.find(channel) != info->channelList.end() )
				info->channelList.erase(channel);
			else
				channel = 0;
		}
	}
	lua_pushnumber(L, channel);
	return 1;
}

//-------------------------------------------------
int	LuaFunc_Chat_GetMsnInfo(lua_State* L)
{
	int id=luaL_checkint(L, 1) -1;
	ChatMsnInfo* info=NULL;
	if (g_pMsnManager)
		info = g_pMsnManager->GetMsnInfo(id);
	if (info)
	{
		lua_pushboolean(L, info->m_used);
		lua_pushnumber(L, info->m_alpha);
		lua_pushnumber(L, info->m_MsnType);
		lua_pushnumber(L, info->m_channel);
		lua_pushnumber(L, info->m_tempX);
		lua_pushnumber(L, info->m_tempY);

		lua_pushstring(L, info->m_target.c_str());
		lua_pushstring(L, info->m_msg.c_str());
	}



	return 8;

}

int LuaFunc_Chat_SetMsnInfoOpen(lua_State* L)
{
	int id=luaL_checkint(L, 1) -1;
	if (g_pMsnManager)
		g_pMsnManager->SetMsnInfoOpen(id);

	return 0;
}
//--------------------------------------------------
int LuaFunc_Chat_SetMsnClose(lua_State* L)
{
	int id=luaL_checkint(L, 1) -1;
	if (g_pMsnManager)
		g_pMsnManager->SetMsnClose(id);

	return 0;
}
//--------------------------------------------------
int LuaFunc_Chat_AddMessage(lua_State* L)
{

	int id=luaL_checkint(L, 1) -1;
	const char* msg = luaL_checkstring(L, 2);
	int color  = luaL_checkint(L, 3);


	ChatMsnInfo* info=NULL;
	if (g_pMsnManager)
		info=g_pMsnManager->GetMsnInfo(id);

	if (info)
		info->AddMessage(msg,color);



	return 0;
}


int LuaFunc_Chat_ClearAllMsg(lua_State* L)
{
	int id=luaL_checkint(L, 1) -1;
	ChatMsnInfo* info=NULL;
	if (g_pMsnManager)
		info=g_pMsnManager->GetMsnInfo(id);

	if (info)
		info->ClearAllMsg();


	return 0;
}
int LuaFunc_Chat_Update(lua_State* L)
{

	int id=luaL_checkint(L, 1) -1;
	ChatMsnInfo* info=NULL;
	if (g_pMsnManager)
		info=g_pMsnManager->GetMsnInfo(id);

	if (info)
		info->Update();
	return 0;
}

int LuaFunc_Chat_CopyToClipboard(lua_State* L)
{

	const char* utf8name = luaL_checkstring(L, 1);

	MsnManager::CopyToClipboard(utf8name);
	return 0;
}
/*
//-------------------------------------------------------------------------------------
int LuaFunc_Chat_SetTextColor(lua_State* L)
{

const char* MsnName = luaL_checkstring(L, 1);
int r=luaL_checkint(L, 2);
int g=luaL_checkint(L, 3);
int b=luaL_checkint(L, 4);
if (g_pMsnManager)
g_pMsnManager->SetMsnTextColor(MsnName,r,g,b);

return 0;

}

//-------------------------------------------------------------------------------------
int LuaFunc_Chat_GetTextColor(lua_State* L)
{
const char* MsnName = luaL_checkstring(L, 1);
int r,g,b;


if (g_pMsnManager)
g_pMsnManager->GetMsnTextColor(MsnName,r,g,b);

return 3;

}
*/
//-------------------------------------------------------------------------------------
int LuaFunc_ParseHyperlink(lua_State* L)
{
	const char* link = luaL_checkstring(L, 1);
	string type ; 
	string data ; 
	string name ;
	if( !ParseHyperlink( link , type , data , name ) )
	{
		return 0;
	}

	if ( strchr(data.c_str(), '/') != NULL )
		data = "";
	if ( strchr(name.c_str(), '/') != NULL )
		name = "";

	lua_pushstring(L, type.c_str() );
	lua_pushstring(L, data.c_str() );
	lua_pushstring(L, name.c_str() );

	return 3;
}

//-------------------------------------------------------------------------------------
int LuaFunc_SetChannelColor(lua_State* L)
{	
	const char* channel = luaL_checkstring(L, 1);
	CUiColor color(1.0f, 1.0f, 1.0f, 1.0f);
	if ( lua_isnumber(L, 2) )
		color.m_r = (float)lua_tonumber(L, 2);
	if ( lua_isnumber(L, 3) )
		color.m_g = (float)lua_tonumber(L, 3);
	if ( lua_isnumber(L, 4) )
		color.m_b = (float)lua_tonumber(L, 4);
	g_pChatFrame->SetChannelColor(channel, color.GetColor());
	return 0;
}

//-------------------------------------------------------------------------------------
int LuaFunc_GetChannelColor(lua_State* L)
{
	CUiColor color;
	color.SetColor( g_pChatFrame->GetChannelColor(luaL_checkstring(L, 1)) );
	lua_pushnumber(L, color.m_r);
	lua_pushnumber(L, color.m_g);
	lua_pushnumber(L, color.m_b);
	return 3;
}

//-------------------------------------------------------------------------------------
e_VoiceChannelType Func_GetVoiceChannelType( const string& strType )
{
	e_VoiceChannelType etype = EVCT_NONE;
	if( strType == "party" )
	{
		etype = EVCT_PARTY;
	}
	else if( strType == "guild" )
	{
		etype = EVCT_GUILD;	
	}
	else if( strType == "user" )
	{
		etype = EVCT_USER;
	}

	return etype;
}

//-------------------------------------------------------------------------------------
void Func_GetVoiceChannelTypeString( e_VoiceChannelType etype, string outChannelType )
{
	outChannelType.clear();
	if( etype == EVCT_GUILD )
	{
		outChannelType = "party";
	}
	else if( etype == EVCT_PARTY )
	{
		outChannelType = "guild";	
	}
	else if( etype == EVCT_USER )
	{
		outChannelType = "user";
	}
}


//-------------------------------------------------------------------------------------
int LuaFunc_VoiceChannel_InfoRequest(lua_State* L)
{
	string strChannelType = luaL_checkstring( L, 1 );
	string strChannelName = luaL_checkstring( L, 2 );

	e_VoiceChannelType etype = Func_GetVoiceChannelType( strChannelType );

	g_pChatFrame->VoiceChannelInfoRequest( etype, strChannelName.c_str() );

	return 0;
}


//-------------------------------------------------------------------------------------
int LuaFunc_VoiceChannel_GetMemberCount(lua_State* L)
{
	string strChannelType = luaL_checkstring( L, 1 );
	string strChannelName = luaL_checkstring( L, 2 );

	e_VoiceChannelType etype = Func_GetVoiceChannelType( strChannelType );

	s_VoiceChannelInfo* pVoiceChannelInfo = g_pChatFrame->GetVoiceChannelInfo( etype, strChannelName.c_str() );

	if( NULL == pVoiceChannelInfo )
	{
		return 0 ;
	}

	lua_pushnumber( L, (lua_Number)pVoiceChannelInfo->vecMemberNameList.size() );	

	return 1;
}

//-------------------------------------------------------------------------------------
int LuaFunc_VoiceChannel_GetMemberInfo(lua_State* L)
{
	string strChannelType = luaL_checkstring( L, 1 );
	string strChannelName = luaL_checkstring( L, 2 );

	e_VoiceChannelType etype = Func_GetVoiceChannelType( strChannelType );

	s_VoiceChannelInfo* pVoiceChannelInfo = g_pChatFrame->GetVoiceChannelInfo( etype, strChannelName.c_str() );

	if( NULL == pVoiceChannelInfo )
	{
		return 0 ;
	}

 	int index = luaL_checkint( L, 3 ) - 1;

	if( index < 0 || index >= pVoiceChannelInfo->vecMemberNameList.size() )
	{
		return 0;
	}

	const char* szName = pVoiceChannelInfo->vecMemberNameList[index].c_str();

	lua_pushstring( L, szName );

	return 1;
}

//-------------------------------------------------------------------------------------
int LuaFunc_VoiceChannel_Join(lua_State* L)
{
	string strChannelType = luaL_checkstring( L, 1 );
	string strChannelName = luaL_checkstring( L, 2 );

	e_VoiceChannelType etype = Func_GetVoiceChannelType( strChannelType );

	g_pChatFrame->JoinVoiceChannel( etype, strChannelName );
	
	return 0;
}

//-------------------------------------------------------------------------------------
int LuaFunc_VoiceChannel_Exit(lua_State* L)
{
	g_pChatFrame->ExitVoiceChannel();
	return 0;
}

//-------------------------------------------------------------------------------------
int LuaFunc_VoiceChannel_IsCurrentChannel(lua_State* L)
{
	string strChannelType = luaL_checkstring( L, 1 );
	string strChannelName = luaL_checkstring( L, 2 );

	e_VoiceChannelType etype = Func_GetVoiceChannelType( strChannelType );

	string strToken;
	g_pChatFrame->GetVoiceChannelToken( etype, strChannelName, strToken );


	bool bYes = false;

	lua_pushboolean( L, bYes );

	return 1;
}

//-------------------------------------------------------------------------------------
int LuaFunc_VoiceChannel_GetCurrentChannel(lua_State* L)
{
	e_VoiceChannelType eType = g_pChatFrame->GetVoiceChannelType();
	const char* szName = g_pChatFrame->GetVoiceChannelName();


	string channelType;
	Func_GetVoiceChannelTypeString( eType, channelType );

	lua_pushstring( L, channelType.c_str() );
 	lua_pushstring( L, szName );

	return 2;
}


//-------------------------------------------------------------------------------------
int LuaFunc_VoiceChannel_GetPlayerInfo(lua_State* L)
{
	string strName = luaL_checkstring( L, 1 );

	int DBID = NetCli_OtherChild::GetDBID( strName.c_str() );
	return 0;
}

//-------------------------------------------------------------------------------------
int LuaFunc_VoiceChannel_PartnerMute(lua_State* L)
{
    std::string strName = luaL_checkstring( L, 1 );
	bool bYes = lua_toboolean( L, 2 ) == 0 ? false : true;

	int DBID = NetCli_OtherChild::GetDBID( strName.c_str() );
	if (DBID == -1)
	{
		return 0;
	}
	
	return 0;
}
