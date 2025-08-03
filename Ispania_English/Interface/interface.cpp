#include <windows.h>
#include <shlwapi.h>
#include "../ErrorList/errorlist.h"

#include "Login/LoginParent.h"
#include "Login/AccountLogin.h"
#include "Login/CharacterSelect.h"
#include "Login/CharacterCreate.h"
#include "Debug/DebugMessage.h"
#include "GM/GM_Config.h"
#include "GM/GM_ObjEdit.h"
#include "System/SYS_AudioSettings.h"
#include "System/SYS_DisplaySettings.h"
#include "WorldFrames/LoadingFrame.h"
#include "WorldFrames/GameControl.h"
#include "WorldFrames/UnitFrame.h"
#include "WorldFrames/ChatFrame.h"
#include "WorldFrames/ChatEdit.h"
#include "WorldFrames/BagFrame.h"
#include "WorldFrames/BankFrame.h"
#include "WorldFrames/StoreFrame.h"
#include "WorldFrames/ItemClipboard.h"
#include "WorldFrames/MacroFrame.h"
#include "WorldFrames/ActionFrame.h"
#include "WorldFrames/CharacterFrame.h"
#include "WorldFrames/EmoteFrame.h"
#include "WorldFrames/TradeFrame.h"
#include "WorldFrames/GameTooltip.h"
#include "WorldFrames/BuffFrame.h"
#include "WorldFrames/PartyFrame.h"
#include "WorldFrames/LootFrame.h"
#include "WorldFrames/MailFrame.h"
#include "WorldFrames/AuctionFrame.h"
#include "WorldFrames/CastingBarFrame.h"
#include "WorldFrames/ScrollCombatTextFrame.h"
#include "WorldFrames/ExchangeClassFrame.h"
#include "WorldFrames/MerchantFrame.h"
#include "WorldFrames/GuildFrame.h"
#include "WorldFrames/SpeakFrame.h"
#include "WorldFrames/KeyBinding.h"
#include "WorldFrames/TutorialFrame.h"
#include "WorldFrames/InspectFrame.h"
#include "WorldFrames/AccountBagFrame.h"
#include "WorldFrames/PetFrame.h"
#include "WorldFrames/BotCheckFrame.h"
#include "WorldFrames/DrawRuneFrame.h"
#include "WorldFrames/ClearAttribStoneFrame.h"

#include "WorldFrames/PartnerFrame.h"
#include "Billboard/BillboardFrame.h"
#include "Maintain/Maintain.h"
#include "Quest/UI_QuestList.h"
#include "Quest/UI_QuestBook.h"
#include "Quest/UI_QuestDetail.h"
#include "Quest/QuestTrack.h" 
#include "Quest/QuestTrackNew.h" 

#include "Teach/UI_TeachingFrame.h"
#include "ObjectBloodBar/ObjectBloodBar.h"
#include "ScriptBorder/UI_ScriptBorder.h"
#include "SkillBook/UI_SkillBook.h"
#include "Map/Minimap.h"
#include "Map/WorldMap.h"
#include "CraftFrame/UI_CraftFrame.h"
#include "Socal/UI_Socal.h"
#include "CardBook/UI_CardBook.h"
#include "CardBook/UI_PetBook.h"
#include "WorldFrames/LotteryFrame.h"
#include "Houses/HousesFrame.h"
#include "WorldFrames/NpcTrackFrame.h"
#include "WorldFrames/RaidTargetFrame.h"
#include "WorldFrames/CombatMeter.h"
#include "WorldFrames/BulletinBoard.h"
#include "WorldFrames/GiveItemFrame.h"
#include "TransportBook/TeleportBook.h"  //傳送書.
#include "GuildHouses/GuildHousesFrame.h"
#include "GuildHouses/GuildHouseStatus.h"	// 公會屋戰狀態介面
#include "RecipeStore\UI_RecipeStore.h" //配方商店
#include "WorldFrames/GmNotificationFrame.h" //回報GM
#include "WorldFrames/BattleGroundScoreFrame.h" //戰場陣營積分
#include "WorldFrames/BattleGroundStatusFrame.h" //戰場狀態介面
#include "WorldFrames/BattleGroundOptionButtonFrame.h" //戰場選項按鈕
#include "WorldFrames/BattleGroundRoomListFrame.h" //戰場房間列表
#include "WorldFrames/BattleGroundAreneScoreFrame.h" //競技場成績介面
#include "WorldFrames/ClientCountDownBarFrame.h"
#include "WorldFrames/HorseRacingRankingFrame.h"
#include "WorldFrames/BattleGroundQueueFrame.h"
#include "WorldFrames/BattleGroundBloodStatusFrame.h"
#include "ScoreBoard/UI_ScoreBoard.h"
#include "WorldFrames/SuitSkillFrame.h"				// 套裝碎取界面


#include "WorldFrames/UI_SortScoreBoard.h"
#include "PartyBoard/PartyBoardFrame.h"	//組隊招募留言板
#include "FusionStoneTradeFrame/FusionStoneTradeFrame.h"	//黑幣兌換系統
#include "../../tools/SmallObj/SmallObj.h"

#include "WorldFrames/GuildWarScore.h"  //戰場積分
#include "WorldFrames/PublicEncounterFrame.h"  //公眾事件
#include "WorldFrames/CoverStringFrame.h"  //遮蔽字串
#include "WorldFrames/RuneExchange.h"  //遮蔽字串
#include "WorldFrames/WorldBattleGround.h"  //遮蔽字串

//#include "MouseLight/MouseLight.h" //ROCCAT Kone Plus 滑鼠LED
#include "WorldFrames/AncillaryTitleFrame.h"  //遮蔽字串

#include "WorldFrames/WeekInstance.h" //鏡世界介面
#include "WorldFrames/PhantomFrame.h" //幻靈介面
#include "WorldFrames/LoginBonusFrame.h" //登入禮介面

#include "interface.h"
#include <Interface\ItemLink\ItemLink.h>

#define NEWBIE_TUTORIAL_INTERFACE_ID	199

CInterface* g_pInterface;

int	LuaFunc_GetGlobal(lua_State* L);
int LuaFunc_CreateUIComponent(lua_State* L);
int	LuaFunc_TEXT(lua_State* L);
int LuaFunc_ErrorOutput(lua_State* L);
int	LuaFunc_GetDefaultLanguage(lua_State* L);
int	LuaFunc_RunScript(lua_State* L);
int LuaFunc_SaveVariables(lua_State* L);
int LuaFunc_SaveVariablesPerCharacter(lua_State* L);
int LuaFunc_WriteFile(lua_State* L);
int LuaFunc_DWORDAND(lua_State* L);
int LuaFunc_DWORDXOR(lua_State* L);
int LuaFunc_DWORDRSHIFT(lua_State* L);
int LuaFunc_LinkToHash(lua_State* L);
// ------------------------------------------------------------------------------------------------
void UiOuputError(const char* errorStr)
{
	// if ( IsDebuggerPresent() )
	{
		g_pError->AddMessage(0, 0, errorStr);
	}

	if (g_pInterface == NULL)
		return;

	lua_State* L = g_pInterface->GetLuaState();
	lua_pushstring(L, errorStr);
	lua_setglobal(L, "arg1");
	g_pInterface->SendWorldEvent("SCRIPT_RUNTIME_ERROR");
}

// ------------------------------------------------------------------------------------------------
CTempFile::CTempFile()
{
	Clear();
}

// ------------------------------------------------------------------------------------------------
CTempFile::CTempFile(int size)
{
	Clear();

	Open(size);
}

// ------------------------------------------------------------------------------------------------
CTempFile::CTempFile(char* inData, int inDataSize)
{
	Clear();

	SetData(inData, inDataSize);
}

// ------------------------------------------------------------------------------------------------
CTempFile::~CTempFile()
{
	Close();
}

// ------------------------------------------------------------------------------------------------
void CTempFile::Clear()
{
	m_isNew = false;
	m_data = NULL;
	m_dataSize = 0;
	m_position = 0;
}

// ------------------------------------------------------------------------------------------------
void CTempFile::Open(int newSize)
{
	Close();

	if (newSize <= 0)
		newSize = 65536;

	m_data = new char[newSize];
	m_dataSize = newSize;
	m_position = 0;
	m_isNew = true;
}

// ------------------------------------------------------------------------------------------------
void CTempFile::Close()
{
	if (m_isNew && m_data)
		delete[] m_data;

	Clear();
}

// ------------------------------------------------------------------------------------------------
int CTempFile::Read(void* outData, int outDataSize)
{
	// 調整大小,最大值不超過剩餘大小
	if (outDataSize > m_dataSize - m_position)
	{
		outDataSize = m_dataSize - m_position;
	}

	// 讀取資料大於零
	if (outDataSize > 0 && outData)
	{
		memcpy(outData, m_data + m_position, outDataSize);
	}

	m_position += outDataSize;

	return outDataSize;
}

// ------------------------------------------------------------------------------------------------
int CTempFile::Write(void* data, int dataSize)
{
	if (m_isNew == false || m_data == NULL || dataSize <= 0)
		return 0;

	int surplusSize = m_dataSize - m_position;

	// 剩餘容量不足夠,產生更大的空間
	if (dataSize > surplusSize)
	{
		int newSize = m_dataSize << 1;
		while (newSize - m_position < dataSize)
			newSize <<= 1;

		char* tempData = new char[newSize];
		memcpy(tempData, m_data, m_dataSize);
		delete[] m_data;
		m_data = tempData;
		m_dataSize = newSize;
	}

	memcpy(m_data + m_position, data, dataSize);
	m_position += dataSize;
	return dataSize;
}

// ------------------------------------------------------------------------------------------------
void CTempFile::SetData(char* data, int size)
{
	Close();

	m_data = data;
	m_dataSize = size;
	m_isNew = false;
	m_position = 0;
}

// ----------------------------------------------------------------------------------
int CTempFile::SetPosition(TempFilePosition position, int offset)
{
	switch (position)
	{
	case eTempFilePosition_Begin:			// 開始
		m_position = min(max(offset, 0), m_dataSize);
		break;

	case eTempFilePosition_Current:			// 目前位置
		m_position = min(max(m_position + offset, 0), m_dataSize);
		break;

	case eTempFilePosition_End:				// 結尾
		m_position = min(max(m_dataSize + offset, 0), m_dataSize);
		break;
	}

	return m_position;
}

// ----------------------------------------------------------------------------------
CInterfaceBase::CInterfaceBase(CInterface* object)
{
	m_interface = object;

	m_saveUpdateTime = 0.0;
}

// ----------------------------------------------------------------------------------
CInterfaceBase::~CInterfaceBase()
{
	m_interface = NULL;
}

// ----------------------------------------------------------------------------------
lua_State* CInterfaceBase::GetLuaState()
{
	return (m_interface) ? m_interface->GetLuaState() : NULL;
}

// ----------------------------------------------------------------------------------
void CInterfaceBase::SendLoginEvent(const char* event)
{
	CUi* ui = m_interface->GetUiLogin();
	if (ui)
		ui->SendEvent(event);
}

// ----------------------------------------------------------------------------------
void CInterfaceBase::SendWorldEvent(const char* event)
{
	CUi* ui = m_interface->GetUiWorld();
	if (ui)
		ui->SendEvent(event);
}

// ----------------------------------------------------------------------------------
bool CInterfaceBase::IsUpdate()
{
	return (m_interface) ? m_interface->IsUpdate() : false;
}

// ----------------------------------------------------------------------------------
void CInterfaceBase::SetSaveUpdateTime(float time)
{
	if (time > m_saveUpdateTime && m_saveUpdateTime > 0.0f)
		return;
	m_saveUpdateTime = time;
}

// ----------------------------------------------------------------------------------
bool CInterfaceBase::GetGlobalPath(const char* name, char* outPath, int outSize)
{
	if (name == NULL || outPath == NULL || outSize <= 0)
		return false;

	const char* savePath = m_interface->GetSavePath();
	char tmp[1024];

	memset(tmp, 0, sizeof(tmp));
	if (savePath[0])
		sprintf(tmp, "%s\\%s", savePath, name);
	else
		strcpy(tmp, name);

	{
		strncpy(outPath, tmp, outSize);
		outPath[outSize - 1] = 0;
	}

	return true;
}

// ----------------------------------------------------------------------------------
bool CInterfaceBase::GetLocalPath(const char* name, char* outPath, int outSize)
{
	if (name == NULL || outPath == NULL || outSize <= 0)
		return false;

	const char* savePath = m_interface->GetSavePath();
	const char* playerName = m_interface->GetPlayerName();
	char tmp[1024];

	memset(tmp, 0, sizeof(tmp));
	if (savePath[0])
		sprintf(tmp, "%s\\%s\\%s", savePath, playerName, name);
	else
		sprintf(tmp, "%s\\%s", playerName, name);

	{
		strncpy(outPath, tmp, outSize);
		outPath[outSize - 1] = 0;
	}
	return true;
}

// ----------------------------------------------------------------------------------
wstring	CInterfaceBase::GetGlobalPathW(const wchar_t* filename)
{
	if (m_interface && filename)
	{
		return m_interface->GetGlobalPathW(filename);
	}
	return L"";
}

// ----------------------------------------------------------------------------------
wstring	CInterfaceBase::GetLocalPathW(const wchar_t* filename)
{
	if (m_interface && filename)
	{
		return m_interface->GetLocalPathW(filename);
	}
	return L"";
}

// ----------------------------------------------------------------------------------
CInterface::CInterface(const char* resourcePath, HWND hWnd)
{
	m_luaState = lua_open();

	CUiLuaLayout::OpenLib(m_luaState);
	CUiLuaFontString::OpenLib(m_luaState);
	CUiLuaTexture::OpenLib(m_luaState);
	CUiLuaFrame::OpenLib(m_luaState);
	CUiLuaModel::OpenLib(m_luaState);
	CUiLuaButton::OpenLib(m_luaState);
	CUiLuaCheckButton::OpenLib(m_luaState);
	CUiLuaRadioButton::OpenLib(m_luaState);
	CUiLuaEditBox::OpenLib(m_luaState);
	CUiLuaListBox::OpenLib(m_luaState);
	CUiLuaSlider::OpenLib(m_luaState);
	CUiLuaStatusBar::OpenLib(m_luaState);
	CUiLuaMessageFrame::OpenLib(m_luaState);
	CUiLuaScrollFrame::OpenLib(m_luaState);
	CUiLuaTooltip::OpenLib(m_luaState);
	/*
	luaL_baselibopen(m_luaState);
	lua_strlibopen(m_luaState);
	lua_tablibopen(m_luaState);
	lua_mathlibopen(m_luaState);
	lua_iolibopen(m_luaState);// lua iolib   2007 8-28 fuming
	*/

	luaopen_base(m_luaState);
	luaopen_string(m_luaState);
	luaopen_table(m_luaState);
	luaopen_math(m_luaState);
	// luaopen_os(m_luaState);
	// luaopen_io(m_luaState);

	// register global lua functions
	lua_register(m_luaState, "err_output", LuaFunc_ErrorOutput);
	lua_register(m_luaState, "getglobal", LuaFunc_GetGlobal);
	lua_register(m_luaState, "CreateUIComponent", LuaFunc_CreateUIComponent);
	lua_register(m_luaState, "TEXT", LuaFunc_TEXT);
	lua_register(m_luaState, "GetDefaultLanguage", LuaFunc_GetDefaultLanguage);
	lua_register(m_luaState, "RunScript", LuaFunc_RunScript);
	lua_register(m_luaState, "SaveVariables", LuaFunc_SaveVariables);
	lua_register(m_luaState, "SaveVariablesPerCharacter", LuaFunc_SaveVariablesPerCharacter);
	lua_register(m_luaState, "__WriteFile", LuaFunc_WriteFile);
	lua_register(m_luaState, "DWORDAND", LuaFunc_DWORDAND);
	lua_register(m_luaState, "DWORDXOR", LuaFunc_DWORDXOR);
	lua_register(m_luaState, "DWORDRSHIFT", LuaFunc_DWORDRSHIFT);
	lua_register(m_luaState, "LinkToHash", LuaFunc_LinkToHash);
	luaL_dostring(m_luaState,
		"function __SaveVariables(name, value, level, saved) \
	saved = saved or {}; \
	for n = 2, level do \
		__WriteFile(\"\\t\"); \
	end \
	local preStr, endStr; \
	if ( level > 1 ) then \
		if ( type(name) == \"number\" ) then \
			preStr = string.format(\"[%s] = \", tostring(name)); \
		else \
			preStr = string.format(\"[%s] = \", string.format(\"%q\", name)); \
		end \
		endStr = \",\\n\"; \
	else \
		preStr = name..\" = \"; \
		endStr = \"\\n\"; \
	end \
	if ( type(value) == \"number\" ) then \
		__WriteFile(preStr..tostring(value)..endStr); \
	elseif ( type(value) == \"string\" ) then \
		__WriteFile(preStr..string.format(\"%q\", value)..endStr); \
	elseif ( type(value) == \"boolean\" ) then \
		if ( value ) then \
			__WriteFile(preStr..\"true\"..endStr); \
		else \
			__WriteFile(preStr..\"false\"..endStr); \
		end \
	elseif ( type(value) == \"table\" ) then \
		if ( saved[value] ) then \
			__WriteFile(preStr..saved[value]..endStr); \
		else \
			__WriteFile(preStr..\"{\\n\"); \
			for i, v in pairs(value) do \
				__SaveVariables(i, v, level+1, saved); \
			end \
			for n = 2, level do \
				__WriteFile(\"\\t\"); \
			end \
			__WriteFile(\"}\"..endStr); \
		end \
	end \
end");

	if (lua_isstring(m_luaState, -1))
	{
		const char* str = lua_tostring(m_luaState, -1);
		str = NULL;
	}

	m_processType = 0;
	m_hWnd = hWnd;
	m_uiLogin = NULL;
	m_uiWorld = NULL;
	m_isVisible = true;
	m_isEnable = true;
	m_loadUIDelayTime = 0.0f;
	m_isReloadUI = false;
	m_width = 0.0f;
	m_height = 0.0f;
	m_scale = 1.0f;
	m_location = "TW";
	m_dataLanguage = "TW";

	m_stream = NULL;
	m_saveVariables.clear();
	m_playerName.clear();

	// 順序載入 XML 
	m_xmlFiles.clear();
	m_loadingPosition = 0;
	m_isLoadVariables = false;

	SetResourcePath(resourcePath);

	m_clientSaveData.clear();

	g_pInterface = this;

	// new interface class
	// login frames
	m_loginFrames.push_back(new CLoginParent(this));
	m_loginFrames.push_back(new CAccountLogin(this));
	m_loginFrames.push_back(new CCharacterSelect(this));
	m_loginFrames.push_back(new CCharacterCreate(this));
	//m_loginFrames.push_back( new CLoadingFrame (this)			);

	// world frames
	m_worldFrames.push_back(new CLoadingFrame(this));
	m_worldFrames.push_back(new CGameControl(this));

	m_worldFrames.push_back(new CGuildHousesFrame(this));
	m_worldFrames.push_back(new CGuildHouseWarStatus(this));	// 公會屋戰狀態介面

	m_worldFrames.push_back(new CUnitFrame(this));
	m_worldFrames.push_back(new CItemClipboard(this));
	m_worldFrames.push_back(new CGameTooltip(this));
	m_worldFrames.push_back(new CChatFrame(this));
	m_worldFrames.push_back(new CChatEdit(this));

	m_worldFrames.push_back(new CBagFrame(this));
	m_worldFrames.push_back(new CBankFrame(this));
	m_worldFrames.push_back(new CStoreFrame(this));
	m_worldFrames.push_back(new CDebugMessage(this));
	m_worldFrames.push_back(new CMacroFrame(this));
	m_worldFrames.push_back(new CActionFrame(this));
	m_worldFrames.push_back(new CCharacterFrame(this));
	m_worldFrames.push_back(new CEmoteFrame(this));
	m_worldFrames.push_back(new CTradeFrame(this));
	m_worldFrames.push_back(new CBillboardFrame(this));
	m_worldFrames.push_back(new CUI_CardBook(this));
	m_worldFrames.push_back(new CUI_PetBook(this));

	m_worldFrames.push_back(new CUI_QuestList(this));
	m_worldFrames.push_back(new CUI_QuestBook(this));
	m_worldFrames.push_back(new CUI_QuestDetail(this));
	m_worldFrames.push_back(new CQuestTrackFrame(this));


	m_worldFrames.push_back(new CUI_TeachingFrame(this));
	m_worldFrames.push_back(new ObjectBloodBar(this));
	m_worldFrames.push_back(new CUI_DrawRuneFrame(this));

	m_worldFrames.push_back(new CUI_ClearAttribStoneFrame(this));
	m_worldFrames.push_back(new CUI_PartnerFrame(this));


	m_worldFrames.push_back(new CGM_Config(this));
	m_worldFrames.push_back(new CGM_ObjEdit(this));

	m_worldFrames.push_back(new CSYS_AudioSettings(this));
	m_worldFrames.push_back(new CSYS_DisplaySettings(this));

	m_worldFrames.push_back(new CMaintain(this));
	m_worldFrames.push_back(new CUI_SkillBook(this));
	m_worldFrames.push_back(new CBuffFrame(this));
	m_worldFrames.push_back(new CPartyFrame(this));
	m_worldFrames.push_back(new CLootFrame(this));
	m_worldFrames.push_back(new CMailFrame(this));
	m_worldFrames.push_back(new CAuctionFrame(this));
	m_worldFrames.push_back(new CCastingBarFrame(this));
	m_worldFrames.push_back(new CScrollCombatTextFrame(this));
	m_worldFrames.push_back(new CExchangeClassFrame(this));
	m_worldFrames.push_back(new CWorldMapFrame(this));
	m_worldFrames.push_back(new CMinimapFrame(this));
	m_worldFrames.push_back(new CUI_CraftFrame(this));
	m_worldFrames.push_back(new CMerchantFrame(this));
	m_worldFrames.push_back(new CUI_ScriptBorder(this));
	m_worldFrames.push_back(new CGuildFrame(this));
	m_worldFrames.push_back(new CUI_Socal(this));
	m_worldFrames.push_back(new CSpeakFrame(this));
	m_worldFrames.push_back(new CKeyBinding(this));
	m_worldFrames.push_back(new CTutorialFrame(this));
	m_worldFrames.push_back(new CInspectFrame(this));
	m_worldFrames.push_back(new CLotteryFrame(this));
	m_worldFrames.push_back(new CHousesFrame(this));
	m_worldFrames.push_back(new CNpcTrackFrame(this));
	m_worldFrames.push_back(new CRaidTargetFrame(this));
	m_worldFrames.push_back(new CCombatMeter(this));
	m_worldFrames.push_back(new CBulletinBoard(this));
	m_worldFrames.push_back(new CGiveItemFrame(this));
	m_worldFrames.push_back(new CAccountBagFrame(this));
	m_worldFrames.push_back(new CPetFrame(this));

	m_worldFrames.push_back(new CUI_TeleportBook(this));//傳送書.
	m_worldFrames.push_back(new CRecipeStore(this));//配方商店.

	m_worldFrames.push_back(new CGmNotificationFrame(this));//回報GM介面
	m_worldFrames.push_back(new CBattleGroundScoreFrame(this));//戰場陣營積分
	m_worldFrames.push_back(new CBattleGroundStatusFrame(this));//戰場狀態介面
	m_worldFrames.push_back(new CBattleGroundOptionButtonFrame(this));//戰場選項按鈕
	m_worldFrames.push_back(new CBattleGroundRoomListFrame(this));//戰場房間列表
	m_worldFrames.push_back(new CBattleGroundArenaScoreFrame(this));//競技場成績
	m_worldFrames.push_back(new CClientCountDownBarFrame(this));//CountDownBar
	m_worldFrames.push_back(new CHorseRacingRankingFrame(this));//賽馬場排名
	m_worldFrames.push_back(new CBattleGroundQueueFrame(this));
	m_worldFrames.push_back(new CBattleGroundBloodStatusFrame(this));  //戰場的守塔血條
	m_worldFrames.push_back(new CUI_scoreBoard(this));  //小遊戲計分
	m_worldFrames.push_back(new CPartyBoardFrame(this));  //組隊招募留言板
	m_worldFrames.push_back(new CFusionStoneTradeFrame(this));  //黑幣兌換系統
	m_worldFrames.push_back(new CBotCheckFrame(this));	//機器人確認訊息
	m_worldFrames.push_back(new CGuildWarScore(this));

	m_worldFrames.push_back(new CUISortScoreBoard(this));//伺服器排行榜
	m_worldFrames.push_back(new CPublicEncounterFrame(this)); //公眾事件
	m_worldFrames.push_back(new CSuitSkillFrame(this));			// 套裝技能

	m_worldFrames.push_back(new CCoverStringFrame(this));			// 遮蔽字串
	m_worldFrames.push_back(new CRuneExchangeFrame(this));			// 遮蔽字串
	m_worldFrames.push_back(new CWorldBattleGround(this));			// 世界戰場
	m_worldFrames.push_back(new CAncillaryTitleFrame(this));		// 附屬稱號介面
	//m_worldFrames.push_back( new NPC_Modify(this));				// NPC EDIT
	//m_worldFrames.push_back( new CMouseLight(this));				// ROCCAT Kone
	m_worldFrames.push_back(new CWeekInstanceFrame(this));			// 鏡世界介面
	m_worldFrames.push_back(new CPhantomFrame(this));				// 幻靈介面
	m_worldFrames.push_back(new CLoginBonusFrame(this));			// 登入禮介面

	//m_uiMember.push_back(new CWorldFrame(this));	

	// register lua function	
	for (vector<CInterfaceBase*>::iterator iter = m_loginFrames.begin(); iter != m_loginFrames.end(); iter++)
		(*iter)->RegisterFunc();

	// register lua function
	for (vector<CInterfaceBase*>::iterator iter = m_worldFrames.begin(); iter != m_worldFrames.end(); iter++)
		(*iter)->RegisterFunc();
}

// ----------------------------------------------------------------------------------
CInterface::~CInterface()
{
	ClearClientData();
	if (m_processType == eInterface_Process_World)
		LeaveFramesWorld();

	// release interface class
	for (vector<CInterfaceBase*>::iterator iter = m_loginFrames.begin(); iter != m_loginFrames.end(); iter++)
		UI_SAFE_DELETE(*iter);
	m_loginFrames.clear();

	// release interface class
	for (vector<CInterfaceBase*>::iterator iter = m_worldFrames.begin(); iter != m_worldFrames.end(); iter++)
		UI_SAFE_DELETE(*iter);
	m_worldFrames.clear();

	UI_SAFE_DELETE(m_uiLogin);
	UI_SAFE_DELETE(m_uiWorld);

	if (m_luaState)
		lua_close(m_luaState);
}

// ----------------------------------------------------------------------------------
void CInterface::Initialize(const char* location, const char* dataLanguage, const char* country, const char* saveFolder)
{
	char* documentDir;

	documentDir = RuUserEnvironment_GetDocumentDirectory();

	if (documentDir)
	{
		char combinedSavePath[4096];
		memset(combinedSavePath, 0, sizeof(combinedSavePath));
		memcpy(combinedSavePath, documentDir, strlen(documentDir) + 1);
		strcat(combinedSavePath, "\\");

		// 如果有指定存檔目錄名稱
		if (saveFolder && strlen(saveFolder) > 0)
		{
			strcat(combinedSavePath, saveFolder);
		}
		else
		{
			strcat(combinedSavePath, INTERFACE_SAVE_PATH);
		}

		m_savePath = combinedSavePath;

#ifdef KALYDO
		// make sure all settings are separated from the client version.
		m_savePath.append(" Kalydo");
#endif
		delete[] documentDir;
	}
	else
	{
		m_savePath = "C:\\TEMP";

		OutputDebugString("Could not retrieve user documents directory.\n");
	}

	if (!m_savePath.empty())
	{
		wchar_t* wSavePath = MultiByteToWChar(m_savePath.c_str());
		if (wSavePath)
		{
			CreateDirectoryW(wSavePath, NULL);							// 建立目錄		

			delete[] wSavePath;
		}
	}

	//g_pActionFrame->DefaultAction();   // 因為學習表格會有偏位,所以需要改在版號進來時做設定
	m_location = location;
	m_dataLanguage = dataLanguage;
	m_country = country;

	g_pMacroFrame->SearchIcons();
	g_pEmoteFrame->InitEmoteDate();

	// 韓國隱藏目錄
	if (country && strcmp(country, "KR") == 0)
	{
		SetFileAttributes(m_savePath.c_str(), FILE_ATTRIBUTE_HIDDEN);		// 設定目錄隱藏
	}

	g_pAccountLogin->DownloadBoardString(CAccountLogin::ReceiveDownloadBoardString);
}

// ----------------------------------------------------------------------------------
void CInterface::BuildFramesLogin(float width, float height)
{
	CreateUiLogin(width, height);

	m_uiLogin->InitializeXmlParser(m_resourcePath.c_str());

	// 解釋TOC檔案
	LoadXmlFromFile("interface\\LoginXML\\LoginXML.toc", "interface\\loginXml\\");

	// 載入列表
	for (vector<string>::iterator iter = m_xmlFiles.begin(); iter != m_xmlFiles.end(); iter++) {
		m_uiLogin->LoadXmlFile(iter->c_str());
	}

	m_uiLogin->ReleaseXmlParser();
}

// ----------------------------------------------------------------------------------
void CInterface::BuildFramesWorld(float width, float height)
{
	if (OpenWorldXml()) {
		float status;
		while ((status = GetXmlLoadingStatus()) < 1.0f) {
		}
		CloseWorldXml();
	}

	/*

	virtual bool				OpenWorldXml();
	virtual bool				CloseWorldXml();
	virtual float				GetXmlLoadingStatus();

	CreateUiWorld(width, height);

	// register lua function
	for (vector<CInterfaceBase*>::iterator iter = m_worldFrames.begin(); iter != m_worldFrames.end(); iter++)
		(*iter)->RegisterFunc();

	if ( m_uiWorld )
	{
		m_uiWorld->InitializeXmlParser(m_resourcePath.c_str());

		// Binding
		g_pKeyBinding->Clear();
		string bindingFilename = m_resourcePath + "Interface\\WorldXml\\Bindings.xml";
		m_uiWorld->LoadXmlBinding(bindingFilename.c_str());

		// register user interface key bindings
		CXmlUi* xmlUi = m_uiWorld->GetXmlUi();
		for ( vector<CXmlBinding*>::iterator iter = xmlUi->m_bindings.begin(); iter != xmlUi->m_bindings.end(); iter++ )
		{
			g_pKeyBinding->AddBinding((*iter)->m_name.c_str(), (*iter)->m_content.c_str(), (*iter)->m_runOnUp, (*iter)->m_header.c_str());
		}
		g_pKeyBinding->Load();

		// World
		string pathname = m_resourcePath + "interface\\WorldXML\\";
		string tocFilename = pathname + "WorldXML.toc";
		LoadXmlFile(tocFilename.c_str(), pathname.c_str());

		// AddOns
		pathname = m_resourcePath + "interface\\AddOns";
		LoadXmlFolders(pathname.c_str());
	}




	// World
	pathname = m_resourcePath + "interface\\WorldXML\\";
	tocFilename = pathname + "WorldXML.toc";


	// LoadFiles(m_uiWorld, tocFilename.c_str());



	// AddOns
	tmp = m_resourcePath + "interface\\AddOns";
	// LoadFolders(m_uiWorld, tmp.c_str());

	if ( m_uiWorld )
	{


		// Binding
		m_xmlUi->OpenBinding(filename);
		m_xmlUi->OpenBinding(filename);

		// register user interface key bindings
		CXmlUi* xmlUi = m_uiWorld->GetXmlUi();
		for ( vector<CXmlBinding*>::iterator iter = xmlUi->m_bindings.begin(); iter != xmlUi->m_bindings.end(); iter++ )
		{
			g_pKeyBinding->AddBinding((*iter)->m_name.c_str(), (*iter)->m_content.c_str(), (*iter)->m_runOnUp, (*iter)->m_header.c_str());
		}
		g_pKeyBinding->DefaultBindingKey();


		//m_uiWorld->BuildFrames();
	}

	for (vector<CInterfaceBase*>::iterator iter = m_worldFrames.begin(); iter != m_worldFrames.end(); iter++)
		(*iter)->BuildFrames();

	ClearXmlFiles();


	// load data
	// Load();
	*/
}

// ----------------------------------------------------------------------------------
void CInterface::LeaveFramesWorld()
{
	m_isReloadUI = false;

	ClearClientData();

	g_pPartyFrame->ClearInstanceServerInfo();

	// 解除戰場組隊
	g_pPartyFrame->ClearBattleGroup();

	//判斷是否是離開 房屋
	g_pHousesFrame->LeaveWorld();

	//判斷是否是離開 房屋
	g_pGuildHousesFrame->LeaveWorld();

	//清掉這個世界的公會列表
	g_pGuildFrame->LeaveWorld();

	//離開語音頻道
	g_pChatFrame->LeaveWorld();

	g_pUi_Socal->LeaveWorld();

	g_pActionFrame->ClearImplementAction();

	g_pPetFrame->LeaveWorld();

	g_pUi_Socal->LeaveWorld();
	g_pMerchantFrame->LeaveWorld();

	if (m_isLoadVariables)
	{
		// 儲存角色參數
		SavePlayerData();

		SendWorldEvent("SAVE_VARIABLES");

		// 儲存Lua參數		
		SaveVariablesToFile("SaveVariables.lua");

		m_isLoadVariables = false;
	}

	// 清除動態建立物件指標
	ClearUIComponent();

	// 刪除 World UI
	DestroyUiWorld();


	// 清除玩家名稱
	SetPlayerName("");

	//if(g_pMouseLight->m_pRoccat)//滑鼠事件
	//{
	//	g_pMouseLight->m_pRoccat->ChangeMouseLight(0, 0, 0, 0, 0);
	//}
}

// ----------------------------------------------------------------------------------
bool CInterface::WndProc(UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	if (m_isEnable)
	{
		// 韓文輸入按下滑鼠左右鍵會觸發
		switch (iMsg)
		{
		case WM_KEYUP:
		case WM_SYSKEYUP:
			if (lParam == 0x80000000)
				return false;
			break;
		}

		switch (m_processType)
		{
		case eInterface_Process_Login:
			return m_uiLogin->MsgProc(iMsg, wParam, lParam);

		case eInterface_Process_World:

			// 特別熱鍵需要額外觸發
			switch (iMsg)
			{
			case WM_KEYUP:
			case WM_SYSKEYUP:
				switch (wParam)
				{
					// CTRL + TAB
				case VK_TAB:
					if (GetKeyState(VK_CONTROL) < 0)
					{
						// 執行熱鍵腳本(Lua Script)
						g_pKeyBinding->RunBinding(int(KEY_BINDING_FLAG_CTRL | wParam), false);
					}
					break;

				case VK_SNAPSHOT:
					int keyState = 0;
					if (GetKeyState(VK_SHIFT) < 0)
						keyState |= KEY_BINDING_FLAG_SHIFT;
					if (GetKeyState(VK_CONTROL) < 0)
						keyState |= KEY_BINDING_FLAG_CTRL;
					if (GetKeyState(VK_MENU) < 0)
						keyState |= KEY_BINDING_FLAG_ALT;

					// 執行熱鍵腳本(Lua Script)
					g_pKeyBinding->RunBinding(int(keyState | wParam), false);
					break;
				}
				break;
			}

			// g_pChatFrame->SendChatMessageEx(CHAT_MSG_SYSTEM, "", "Msg(%d) wParam(%d) lParam(%d)", iMsg, wParam, lParam);
			if (iMsg == WM_ACTIVATE && LOWORD(wParam) == WA_INACTIVE || iMsg == WM_KILLFOCUS || iMsg == WM_EXITSIZEMOVE) {
				g_pGameControl->OnKillFocus();
			}
			else if (m_uiWorld->MsgProc(iMsg, wParam, lParam)) {
				g_pGameControl->OnKillFocus(false);
				return true;
			}
			else {
				m_uiWorld->SetUpdateState(false);
				bool isResult = g_pGameControl->WndMsgProc(iMsg, wParam, lParam);
				m_uiWorld->SetUpdateState(true);
				if (isResult)
					return true;
			}
			break;
		}
	}
	return false;
}

// ----------------------------------------------------------------------------------
void CInterface::Update(float elapsedTime, InputState& state)
{
	m_isUpdate = true;

	// 判斷是否有設定重新載入UI
	if (m_loadUIDelayTime > 0.0f)
	{
		m_loadUIDelayTime -= elapsedTime;
		if (m_loadUIDelayTime < 0.001f) {
			m_loadUIDelayTime = 0.0f;

			// 載入CLINET LUA SCRIPT
			OpenWorldXml();
			return;
		}
	}

	UpdateUIComponent();

	CUi* ui = GetUi();
	if (m_isEnable && ui)
		ui->Update(elapsedTime, state);

	switch (m_processType)
	{
	case eInterface_Process_Login:
		for (vector<CInterfaceBase*>::iterator iter = m_loginFrames.begin(); iter != m_loginFrames.end(); iter++)
			(*iter)->Update(elapsedTime);
		break;

	case eInterface_Process_World:
		if (g_pLoadingFrame && g_pLoadingFrame->GetLoadingState()) {
			g_pLoadingFrame->Update(elapsedTime);							// 載入中不執行更新
		}
		else {
			for (vector<CInterfaceBase*>::iterator iter = m_worldFrames.begin(); iter != m_worldFrames.end(); iter++)
			{
				(*iter)->Update(elapsedTime);

				// 
				if ((*iter)->GetSaveUpdateTime() > 0.0f)
				{
					float saveTime = (*iter)->GetSaveUpdateTime() - elapsedTime;
					if (saveTime < 0.0f)
					{
						(*iter)->SaveClientData();
						(*iter)->SetSaveUpdateTime(0.0f);
					}
					else
					{
						(*iter)->SetSaveUpdateTime(saveTime);
					}
				}
			}
		}
		break;
	}

	m_isUpdate = false;
}

// ----------------------------------------------------------------------------------
void CInterface::Render()
{
	switch (m_processType)
	{
	case eInterface_Process_Login:
		for (vector<CInterfaceBase*>::iterator iter = m_loginFrames.begin(); iter != m_loginFrames.end(); iter++)
			(*iter)->Render();
		break;

	case eInterface_Process_World:
		for (vector<CInterfaceBase*>::iterator iter = m_worldFrames.begin(); iter != m_worldFrames.end(); iter++)
			(*iter)->Render();
		break;
	}

	CUi* ui = GetUi();
	if (m_isVisible && ui)
		ui->Render();
}

// ----------------------------------------------------------------------------------
void CInterface::PlayerChangeZone()
{
	for (vector<CInterfaceBase*>::iterator iter = m_worldFrames.begin(); iter != m_worldFrames.end(); iter++)
		(*iter)->ChangeZone();
}

// ----------------------------------------------------------------------------------
void CInterface::SetScreenSize(float x, float y)
{
	m_width = x;
	m_height = y;

	if (m_uiLogin)
		m_uiLogin->SetScreenSize(CUiSize(x, y));
	if (m_uiWorld)
		m_uiWorld->SetScreenSize(CUiSize(x / m_scale, y / m_scale));

	SendWorldEvent("SCREEN_RESIZE");
}

// ----------------------------------------------------------------------------------
void CInterface::SetScale(float scale)
{
	if (m_scale != scale)
	{
		m_scale = scale;
		if (m_uiWorld)
		{
			m_uiWorld->SetScale(m_scale);
			m_uiWorld->SetScreenSize(CUiSize(m_width / m_scale, m_height / m_scale));
		}

		// changed function
	}
}

// ----------------------------------------------------------------------------------
void CInterface::SetResourcePath(const char* resourcePath)
{
	m_resourcePath.clear();

	if (resourcePath && resourcePath[0])
	{
		m_resourcePath = resourcePath;
		if (m_resourcePath[m_resourcePath.size() - 1] != '\\')
			m_resourcePath += "\\";
	}
}

// ----------------------------------------------------------------------------------
CUi* CInterface::GetUi()
{
	switch (m_processType)
	{
	case eInterface_Process_Login:
		return m_uiLogin;

	case eInterface_Process_World:
		return m_uiWorld;
	}
	return NULL;
}

// ----------------------------------------------------------------------------------
CUiInput* CInterface::GetUiInput()
{
	CUi* ui = GetUi();
	if (ui)
		return ui->GetUiInput();
	return NULL;
}

// ----------------------------------------------------------------------------------
void CInterface::SetProcessType(int type)
{
	// 清除所有視窗事件
	/*
	{
		MSG msg;
		BOOL boMessage;
		while ( (boMessage = PeekMessage( &msg, NULL, WM_KEYFIRST, WM_KEYLAST, PM_REMOVE )) ) {
			g_pError->AddMessage(0 ,0, "remove msg %x", msg.message);
		}
	}
	*/

	m_processType = type;
	switch (m_processType)
	{
	case eInterface_Process_Login:
		// 由世界離開,返回登入界面
		SendLoginEvent("SHOW_LOGIN");
		break;

	case eInterface_Process_World:
		// 清除 Login UI 暫存資料
		g_pCharacterCreate->DeleteRoleSprite();
		g_pCharacterSelect->Close();

		// 這事件不準,觸發時可能元件尚未件立(ActionBarFrame)
		SendWorldEvent("PLAYER_ENTERING_WORLD");
		break;
	}
}

// ----------------------------------------------------------------------------------
void CInterface::SetCapture()
{
	CUi* ui = GetUi();
	if (ui)
	{
		ui->SetCapture(ui->GetRoot(), 0);
	}
}

// ----------------------------------------------------------------------------------
void CInterface::ReleaseCapture()
{
	CUi* ui = GetUi();
	if (ui)
	{
		ui->SetCapture(NULL, 0);
	}
}

// ----------------------------------------------------------------------------------
void CInterface::SetPlayerName(const char* playerName)
{
	wstring wName, savePath;

	m_playerName.clear();
	if (playerName)
	{
		m_playerName = playerName;
		wchar_t* ws = MultiByteToWChar(playerName);
		wName = ws;
		delete[] ws;

		/*
		wchar_t* ws = MultiByteToWChar(playerName);
		char* s = TransToChar(ws);
		m_playerName = s;
		wName = ws;
		delete [] s;
		delete [] ws;
		*/
	}

	if (!m_playerName.empty())
	{
		wchar_t* ws = MultiByteToWChar(m_savePath.c_str());
		savePath = ws;
		delete[] ws;

		savePath += L"\\";
		savePath += wName;

		CreateDirectoryW(savePath.c_str(), NULL);			// 建立目錄
	}
}

// ----------------------------------------------------------------------------------
void CInterface::LoadPlayerData()
{
	lua_State* L = GetLuaState();
	if (L)
	{
		luaL_dostring(L,
			" \
			if ( AGGROFRAME_ENABLE ) then \
				SetAggroPrompt(AGGROFRAME_ENABLE); \
			end \
			if ( BOOTYFRAME_AUTO_TAKE ) then \
				SetAutoTakeLoot(BOOTYFRAME_AUTO_TAKE); \
			end \
			if ( GOODSPACK_AUTOOPEN ) then \
				SetAutoOpenGoodsPack(GOODSPACK_AUTOOPEN); \
			end \
			"
		);
	}

	for (vector<CInterfaceBase*>::iterator iter = m_worldFrames.begin(); iter != m_worldFrames.end(); iter++)
	{
		// 如果未讀取SERVER資料,在由檔案內載入
		(*iter)->Load();
	}
}

// ----------------------------------------------------------------------------------
void CInterface::SavePlayerData()
{
	// save data
	for (vector<CInterfaceBase*>::iterator iter = m_worldFrames.begin(); iter != m_worldFrames.end(); iter++)
		(*iter)->Save();
}

// ----------------------------------------------------------------------------------
void CInterface::SaveLuaVariable(const char* variableName, bool perCharacter)
{
	if (perCharacter)
		m_saveVariablesPerCharacter.insert(variableName);
	else
		m_saveVariables.insert(variableName);
}

// ----------------------------------------------------------------------------------
void CInterface::SaveVariablesToFile(const char* filename)
{
	if (m_stream != NULL)
		return;

	string newFilename = m_savePath;
	if (!newFilename.empty())
		newFilename += "\\";
	newFilename += filename;

	//if ( (m_stream = fopen(filename, "wt+, ccs=UTF-8")) != NULL )
	if ((m_stream = fopen(newFilename.c_str(), "wt+")) != NULL)
	{
		for (set<string>::iterator iter = m_saveVariables.begin(); iter != m_saveVariables.end(); iter++)
		{
			string tableName = *iter;

			// 取出每一個變數名稱
			lua_getglobal(m_luaState, tableName.c_str());

			// 變數參數是否存在
			if (!lua_isnil(m_luaState, -1))
			{
				lua_getglobal(m_luaState, "__SaveVariables");
				lua_pushstring(m_luaState, tableName.c_str());
				lua_pushvalue(m_luaState, -3);
				lua_pushnumber(m_luaState, 1);
				lua_pushnil(m_luaState);
				lua_call(m_luaState, 4, 0);
			}
			lua_pop(m_luaState, 1);
		}
		fclose(m_stream);
	}

	newFilename = m_savePath;
	if (!newFilename.empty())
		newFilename += "\\";
	newFilename += m_playerName;
	newFilename += "\\";
	newFilename += filename;

	wchar_t* tmp = MultiByteToWChar(filename);
	wstring wfilename = GetLocalPathW(tmp);
	delete[] tmp;

	//if ( (m_stream = fopen(filename, "wt+, ccs=UTF-8")) != NULL )
	if ((m_stream = _wfopen(wfilename.c_str(), L"wt+")) != NULL)
	{
		for (set<string>::iterator iter = m_saveVariablesPerCharacter.begin(); iter != m_saveVariablesPerCharacter.end(); iter++)
		{
			string tableName = *iter;

			// 取出每一個變數名稱
			lua_getglobal(m_luaState, tableName.c_str());

			// 變數參數是否存在
			if (!lua_isnil(m_luaState, -1))
			{
				lua_getglobal(m_luaState, "__SaveVariables");
				lua_pushstring(m_luaState, tableName.c_str());
				lua_pushvalue(m_luaState, -3);
				lua_pushnumber(m_luaState, 1);
				lua_pushnil(m_luaState);
				lua_call(m_luaState, 4, 0);
			}
			lua_pop(m_luaState, 1);
		}
		fclose(m_stream);
	}

	m_stream = NULL;
}

// ----------------------------------------------------------------------------------
void CInterface::LoadVariablesFromFile(const char* filename)
{
	string newFilename = m_savePath;
	if (!newFilename.empty())
		newFilename += "\\";
	newFilename += filename;
	luaL_dofile(m_luaState, newFilename.c_str());

	newFilename = m_savePath;
	if (!newFilename.empty())
		newFilename += "\\";
	newFilename += m_playerName;
	newFilename += "\\";
	newFilename += filename;
	luaL_dofile(m_luaState, newFilename.c_str());

	wchar_t* tmp = MultiByteToWChar(filename);
	wstring wfilename = GetLocalPathW(tmp);
	delete[] tmp;

	FILE* stream = NULL;
	if ((stream = _wfopen(wfilename.c_str(), L"rb")) != NULL)
	{
		// 取得檔案大小
		fseek(stream, 0, SEEK_END);
		int iFileSize = ftell(stream);

		fseek(stream, 0, SEEK_SET);

		char* buff = new char[iFileSize + 1];
		memset(buff, 0, iFileSize * sizeof(char));
		fread(buff, iFileSize, 1, stream);
		buff[iFileSize] = 0;

		luaL_dostring(m_luaState, buff);

		delete[] buff;

		fclose(stream);
	}
}

// ----------------------------------------------------------------------------------
void CInterface::WriteFile(void* data, int size)
{
	if (m_stream)
	{
		fwrite(data, 1, size, m_stream);
	}
}

// ----------------------------------------------------------------------------------
void CInterface::ReloadUI()
{
	// 儲存角色參數
	SavePlayerData();

	// 送出儲存參數
	SendWorldEvent("SAVE_VARIABLES");

	// 儲存Lua參數		
	SaveVariablesToFile("SaveVariables.lua");


	m_isLoadVariables = false;

	m_isReloadUI = true;
	m_loadUIDelayTime = 0.5f;

	g_pLoadingFrame->LoadingStart(true);

	// g_pLoadingFrame->LoadingStart();
	// LoadVariablesFromFile("SaveVariables.lua");
	// g_pLoadingFrame->LoadingEnd(0.2f);
	// m_uiWorld->SendEvent(UiEvent_VariablesLoaded);
}

// ----------------------------------------------------------------------------------
void CInterface::VariablesLoaded()
{
	// 更新角色資料
	// m_uiWorld->SendEvent(UiEvent_VariablesLoaded);

	// 通知角色金錢數值更改
	// m_uiWorld->SendEvent(UiEvent_Role_Money_Change);

	// 更新所有Coldown
	g_pActionFrame->UpdateCooldown(0, 0);
	g_pBagFrame->UpdateCooldown(0, 0);
	g_pCharacterFrame->UpdateCooldown(0, 0);
	g_pUi_SkillBook->UpdateCooldown(0, 0);
}

// ----------------------------------------------------------------------------------
bool CInterface::OpenWorldXml()
{
	DestroyUiWorld();

	CreateUiWorld(m_width, m_height);

	if (m_uiWorld == NULL)
		return false;

	// UI初始化
	for (vector<CInterfaceBase*>::iterator iter = m_worldFrames.begin(); iter != m_worldFrames.end(); iter++)
		(*iter)->BuildFrames();

	// UI初始化
	m_loadingPosition = 0;				// 初始化目前載入進度
	m_xmlFiles.clear();
	m_uiWorld->InitializeXmlParser(m_resourcePath.c_str());

	// Binding(熱鍵表)
	if (g_pKeyBinding)
	{
		g_pKeyBinding->Clear();
		string bindingFilename = m_resourcePath + "Interface\\WorldXml\\Bindings.xml";
		m_uiWorld->LoadXmlBinding(bindingFilename.c_str());

		// register user interface key bindings
		CXmlUi* xmlUi = m_uiWorld->GetXmlUi();
		for (vector<CXmlBinding*>::iterator iter = xmlUi->m_bindings.begin(); iter != xmlUi->m_bindings.end(); iter++)
		{
			g_pKeyBinding->AddBinding((*iter)->m_name.c_str(), (*iter)->m_content.c_str(), (*iter)->m_runOnUp, (*iter)->m_header.c_str());
		}
		g_pKeyBinding->Load();
	}

	// 取得所有載入檔案列表,記錄在 m_xmlFiles 內
	{
		// World
		string pathname = m_resourcePath + "interface\\WorldXML\\";
		string tocFilename = pathname + "WorldXML.toc";

		LoadXmlFromFile("interface\\worldxml\\worldxml.toc", "interface\\worldxml\\");

		// AddOns
		pathname = m_resourcePath + "interface\\AddOns\\";
		LoadXmlFolders(pathname.c_str());
	}

	return true;
}

// ----------------------------------------------------------------------------------
bool CInterface::CloseWorldXml()
{
	if (m_uiWorld == NULL || m_uiWorld->GetXmlUi() == NULL)
		return false;

	m_loadingPosition = 0;
	m_xmlFiles.clear();
	m_uiWorld->ReleaseXmlParser();

	// 通知所有界面載入完畢
	if (m_isReloadUI)
	{
		m_isReloadUI = false;
		for (vector<CInterfaceBase*>::iterator iter = m_worldFrames.begin(); iter != m_worldFrames.end(); iter++)
			(*iter)->ReloadFrames();

		/*
		if ( )
			// 特殊界面開啟
			SendWorldEvent("PLAYER_DEAD");
		*/
	}

	// 設定分隔符號
	if (m_luaState)
	{
		lua_getglobal(m_luaState, "COMMA");
		if (lua_isstring(m_luaState, -1))
		{
			g_ObjectData->SetCommaSymbol(lua_tostring(m_luaState, -1));
		}
	}

	// 載入共用儲存資料
	LoadVariablesFromFile("SaveVariables.lua");

	// 載入元件儲存資料
	LoadPlayerData();

	// 讀取伺服器送來資料
	LoadClientData();

	m_uiWorld->SendEvent("UPDATE_CHAT_FRAMES");

	// 通知界面更新,參數載入完畢
	m_uiWorld->SendEvent(UiEvent_VariablesLoaded);

	g_pPartyFrame->RequestInstanceRestTime();

	// 顯示離線訊息
	g_pChatFrame->ShowOfflineMessage();

	// 新手教學
	g_pTutorailFrame->TriggerTutorial(NEWBIE_TUTORIAL_INTERFACE_ID);

	// 重新計算特殊貨幣值
	g_pBagFrame->RefreshCoin();

	// 儲存資料載入OK
	m_isLoadVariables = true;

	return true;
}

// ----------------------------------------------------------------------------------
// 每次取得將會執行載入一個檔案,一直到回傳值為1.0f
float CInterface::GetXmlLoadingStatus()
{
	float status = 1.0f;
	int size = (int)m_xmlFiles.size();
	if (m_loadingPosition < size) {
		int count = m_loadingPosition % 6 + 1;
		for (int i = 0; i < count; i++) {
			m_uiWorld->LoadXmlFile(m_xmlFiles[m_loadingPosition].c_str());
			m_loadingPosition++;
			if (m_loadingPosition >= size)
				break;
		}
		status = (float)m_loadingPosition / (float)size;
	}
	return status;
}

// ----------------------------------------------------------------------------------
void CInterface::LoadXmlFolders(const char* folder)
{
	if (folder == NULL)
		return;

	WIN32_FIND_DATA fd;
	HANDLE h;
	string fullName(folder);

	if (fullName[fullName.size() - 1] == '\\')
		fullName += "*.*";
	else
		fullName += "\\*.*";

	h = FindFirstFile(fullName.c_str(), &fd);
	if (h != INVALID_HANDLE_VALUE)
	{
		do
		{
			string tmp(folder);
			if (tmp[tmp.size() - 1] != '\\')
				tmp += "\\";
			tmp += fd.cFileName;

			//目錄
			if ((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				if (strcmp(fd.cFileName, ".") && strcmp(fd.cFileName, ".."))
					LoadXmlFolders(tmp.c_str());
			}
			//檔案
			else if (IsTocFile(tmp.c_str()))
			{
				string pathname = folder;
				pathname += "\\";

				const char* resourceName;
				if (strnicmp(m_resourcePath.c_str(), pathname.c_str(), m_resourcePath.size()) == 0) {
					resourceName = pathname.c_str() + m_resourcePath.size();
				}
				else {
					resourceName = pathname.c_str();
				}

				LoadXmlFromFile(tmp.c_str(), resourceName);
			}
		} while (FindNextFile(h, &fd));
		FindClose(h);
	}
}

// ----------------------------------------------------------------------------------
void CInterface::LoadXmlFile(const char* tocFile, const char* pathname)
{
	if (IsTocFile(tocFile)) {
		FILE* fptr = fopen(tocFile, "r");
		if (fptr) {
			char line[1024];
			while (!feof(fptr)) {
				if (fgets(line, 1024, fptr) != 0) {
					AddXmlFile(line, pathname);
				}
			}
			fclose(fptr);
		}
	}
}

// ----------------------------------------------------------------------------------
void CInterface::LoadXmlFromMemory(const char* data, int size, const char* pathname)
{
	char line[1024];

	if (data && size > 0)
	{
		int loop, length;
		while (size > 0)
		{
			// 取得單行長度
			length = size;
			loop = 0;
			while (loop < size) {
				if (data[loop] < ' ') {
					length = loop + 1;
					break;
				}
				loop++;
			}

			// 避免益位
			if (length < 1024) {
				strncpy(line, data, length);
				line[length] = 0;
			}
			else {
				strncpy(line, data, 1023);
				line[1023] = 0;
			}

			size -= length;
			data += length;
			AddXmlFile(line, pathname);
		}
	}
}

// ----------------------------------------------------------------------------------
void CInterface::LoadXmlFromFile(const char* filename, const char* pathname)
{
	const char* resourceName;
	if (strncmp(filename, m_resourcePath.c_str(), m_resourcePath.size()) == 0) {
		resourceName = filename + m_resourcePath.size();
	}
	else {
		resourceName = filename;
	}

	IRuStream* stream = g_ruResourceManager->LoadStream(resourceName);
	if (stream) {
		LoadXmlFromMemory((const char*)stream->OpenStreamMapping(), stream->GetStreamSize(), pathname);
		delete stream;
	}
}

// ----------------------------------------------------------------------------------
void CInterface::AddXmlFile(const char* filename, const char* pathname)
{
	int length = (int)strlen(filename);

	// 檔案名稱含副檔名需要大於5個字元(x.lua or x.xml)
	if (filename == NULL || length < 5)
		return;

	char temp[1024];
	const char* begin = filename;
	const char* end = filename + length - 1;
	while (*begin && (*begin == '\t' || *begin == ' ')) begin++;
	while (end > begin && (*end == '\t' || *end == ' ' || *end == '\n' || *end == 13)) end--;

	int size = int(end - begin + 1);
	memcpy(temp, begin, size);
	temp[size] = 0;

	if (strncmp(temp, "##", 2) == 0)
	{
		return;
	}

	// 路徑資料
	if (strncmp(temp, "$root\\", 6) == 0)
	{
		memcpy(temp, temp + 6, size - 6 + 1);
		pathname = NULL;
	}

	char* location = (char*)strstr(temp, "$language");
	if (location != NULL)
	{
		int len = (int)strlen("$language");
		int locationSize = (int)m_location.size();
		memcpy(location, m_location.c_str(), locationSize);
		strcpy(location + locationSize, location + len);
	}

	location = (char*)strstr(temp, "$location");
	if (location != NULL)
	{
		int len = (int)strlen("$location");
		int locationSize = (int)m_dataLanguage.size();
		memcpy(location, m_dataLanguage.c_str(), locationSize);
		strcpy(location + locationSize, location + len);
	}

	string fullname = (pathname) ? pathname : "";
	fullname += temp;

	// 避免載入相同檔案
	for (vector<string>::iterator iter = m_xmlFiles.begin(); iter != m_xmlFiles.end(); iter++)
	{
		if (iter->compare(fullname) == 0)
			return;
	}
	m_xmlFiles.push_back(fullname.c_str());
}

// ----------------------------------------------------------------------------------
void CInterface::LoadLuaFile(const char* filename)
{
	if (filename == NULL || m_luaState == NULL)
		return;

	if (luaL_loadfile(m_luaState, filename) != 0 || lua_resume(m_luaState, 0) != 0)
	{
		g_pError->AddMessage(0, 0, lua_tostring(m_luaState, -1));
		lua_pop(m_luaState, 1);
	}
}

// ----------------------------------------------------------------------------------
void CInterface::SendWorldEvent(const char* event)
{
	if (m_uiWorld) {
		m_uiWorld->SendEvent(event);
	}
}

// ----------------------------------------------------------------------------------
void CInterface::SendLoginEvent(const char* event)
{
	if (m_uiLogin) {
		m_uiLogin->SendEvent(event);
	}
}

// ----------------------------------------------------------------------------------
void CInterface::ResetUI()
{
	if (g_pChatFrame)
	{
		g_pChatFrame->DefaultWindowInfo();
		SendWorldEvent("UPDATE_CHAT_FRAMES");
	}
}

// ----------------------------------------------------------------------------------
void CInterface::SetLocation(const char* location, const char* dataLanguage)
{
	m_location = location;
	m_dataLanguage = dataLanguage;
}

// ----------------------------------------------------------------------------------
bool CInterface::IsLoadingUI()
{
	return (int)m_xmlFiles.size() > m_loadingPosition;
}

// ----------------------------------------------------------------------------------
wstring CInterface::GetGlobalPathW(const wchar_t* filename)
{
	if (filename == NULL)
		return L"";

	wstring savePath;
	if (m_savePath.size() > 0)
	{
		wchar_t* ws = MultiByteToWChar(m_savePath.c_str());
		savePath = ws;
		if (ws) delete[] ws;
	}

	savePath += L"\\";
	savePath += filename;

	return savePath;
}

// ----------------------------------------------------------------------------------
wstring	CInterface::GetLocalPathW(const wchar_t* filename)
{
	if (filename == NULL)
		return L"";

	wstring savePath;
	if (m_savePath.size() > 0)
	{
		wchar_t* ws = MultiByteToWChar(m_savePath.c_str());
		savePath = ws;
		if (ws) delete[] ws;
	}

	wstring playerName;
	if (m_playerName.size() > 0)
	{
		wchar_t* ws = MultiByteToWChar(m_playerName.c_str());
		playerName = ws;
		if (ws) delete[] ws;
	}

	savePath += L"\\";
	savePath += playerName;

	savePath += L"\\";
	savePath += filename;

	return savePath;
}

// ----------------------------------------------------------------------------------
void CInterface::PushClientData(int keyID, int size, char* data)
{
	// 清除舊有資料
	map<int, CClientSaveData>::iterator iter = m_clientSaveData.find(keyID);
	if (iter != m_clientSaveData.end())
	{
		(iter->second).Release();

		m_clientSaveData.erase(iter);
	}

	if (data != 0)
	{
		CClientSaveData newData;
		newData.m_size = size;
		newData.m_data = new char[size];
		memcpy(newData.m_data, data, size);

		m_clientSaveData.insert(make_pair(keyID, newData));
	}
}

// ----------------------------------------------------------------------------------
void CInterface::ClearClientData()
{
	for (map<int, CClientSaveData>::iterator iter = m_clientSaveData.begin(); iter != m_clientSaveData.end(); iter++)
	{
		(iter->second).Release();
	}
	m_clientSaveData.clear();
}

// ----------------------------------------------------------------------------------
void CInterface::LoadClientData()
{
	for (map<int, CClientSaveData>::iterator iter = m_clientSaveData.begin(); iter != m_clientSaveData.end(); iter++)
	{
		for (vector<CInterfaceBase*>::iterator baseIter = m_worldFrames.begin(); baseIter != m_worldFrames.end(); baseIter++)
		{
			(*baseIter)->LoadClientData(iter->first, (iter->second).m_size, (iter->second).m_data);
		}
	}
}

// ----------------------------------------------------------------------------------
void CInterface::SaveClientData()
{
	for (vector<CInterfaceBase*>::iterator iter = m_worldFrames.begin(); iter != m_worldFrames.end(); iter++)
	{
		if ((*iter)->GetSaveUpdateTime() > 0.0f)
		{
			(*iter)->SaveClientData();
			(*iter)->SetSaveUpdateTime(0.0f);
		}
	}
}

// ----------------------------------------------------------------------------------
void CInterface::PushUIComponent(CUiLayout* layout)
{
	if (layout)
	{
		switch (layout->GetUiType())
		{
		case CUiLayout::eUiType_Layout:
		case CUiLayout::eUiType_FontString:
		case CUiLayout::eUiType_Texture:
			break;

		default:
			m_createComponent.push_back(layout);
			break;
		}
	}
}

// ----------------------------------------------------------------------------------
void CInterface::UpdateUIComponent()
{
	for (vector<CUiLayout*>::iterator iter = m_createComponent.begin(); iter != m_createComponent.end(); iter++)
	{
		((CUiFrame*)(*iter))->Resume();
	}

	ClearUIComponent();
}

// ----------------------------------------------------------------------------------
bool CInterface::IsTocFile(const char* filename)
{
	if (filename == NULL)
		return false;
	int length = (int)strlen(filename);
	if (length > 4 && (strnicmp(&filename[length - 4], ".toc", 4) == 0))
		return true;
	return false;
}

// ----------------------------------------------------------------------------------
void CInterface::LoadFiles(CUi* ui, const char* filename)
{
	if (IsTocFile(filename) && ui)
	{
		ui->OpenTocFile(filename, m_resourcePath.c_str());
	}
}

// ----------------------------------------------------------------------------------
void CInterface::LoadFolders(CUi* ui, const char* foldername)
{
	if (foldername == NULL)
		return;

	WIN32_FIND_DATA fd;
	HANDLE h;
	string fullName(foldername);

	if (fullName[fullName.size() - 1] == '\\')
		fullName += "*.*";
	else
		fullName += "\\*.*";

	h = FindFirstFile(fullName.c_str(), &fd);
	if (h != INVALID_HANDLE_VALUE)
	{
		do
		{
			string tmp(foldername);
			if (tmp[tmp.size() - 1] != '\\')
				tmp += "\\";
			tmp += fd.cFileName;
			//strlwr( (char*)tmp.c_str() );

			//目錄
			if ((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				if (strcmp(fd.cFileName, ".") && strcmp(fd.cFileName, ".."))
					LoadFolders(ui, tmp.c_str());
			}
			//檔案
			else
				LoadFiles(ui, tmp.c_str());

		} while (FindNextFile(h, &fd));
		FindClose(h);
	}
}

// --------------------------------------------------------------------------------------
void CInterface::CreateUiLogin(float width, float height)
{
	UI_SAFE_DELETE(m_uiLogin);
	m_uiLogin = new CUi(m_luaState, m_hWnd);

	if (m_uiLogin)
	{
		if (stricmp(m_country.c_str(), "KR") == 0)
			m_uiLogin->SetIMEComStr(true);
		else
			m_uiLogin->SetIMEComStr(true);

		m_uiLogin->SetScreenSize(CUiSize(width, height));
	}
}

// --------------------------------------------------------------------------------------
void CInterface::CreateUiWorld(float width, float height)
{
	m_uiWorld = new CUi(m_luaState, m_hWnd);

	if (m_uiWorld)
	{
		if (stricmp(m_country.c_str(), "KR") == 0)
			m_uiWorld->SetIMEComStr(true);
		else
			m_uiWorld->SetIMEComStr(false);

		m_uiWorld->SetScale(m_scale);
		m_uiWorld->SetScreenSize(CUiSize(width / m_scale, height / m_scale));
	}
}

// --------------------------------------------------------------------------------------
void CInterface::DestroyUiWorld()
{
	if (m_uiWorld == NULL)
		return;

	for (vector<CInterfaceBase*>::iterator iter = m_worldFrames.begin(); iter != m_worldFrames.end(); iter++)
	{
		(*iter)->DestroyFrames();
		(*iter)->SetSaveUpdateTime(0.0f);
	}

	// Release World UI Class
	UI_SAFE_DELETE(m_uiWorld);
}

// --------------------------------------------------------------------------------------
int	LuaFunc_GetGlobal(lua_State* L)
{
	lua_getglobal(L, luaL_checkstring(L, 1));
	return 1;
}

// ----------------------------------------------------------------------------------
// arg1 frameType 元件類型
// arg2 frameName 元件名稱
// arg3 parent 父元件名稱
// arg4 inherit 繼承元件名稱
// arg5 layer 如果為文字或圖形需要指定層級
int LuaFunc_CreateUIComponent(lua_State* L)
{
	const char* frameType = luaL_checkstring(L, 1);
	const char* frameName = luaL_checkstring(L, 2);
	const char* parentName = luaL_checkstring(L, 3);
	const char* inherit = lua_isstring(L, 4) ? lua_tostring(L, 4) : NULL;
	int layer = lua_isnumber(L, 5) ? (int)lua_tonumber(L, 5) : -1;

	int type = CUiLayout::eUiType_Layout;
	if (strcmp("FontString", frameType) == 0)
		type = CUiLayout::eUiType_FontString;
	else if (strcmp("Texture", frameType) == 0)
		type = CUiLayout::eUiType_Texture;
	else if (strcmp("Frame", frameType) == 0)
		type = CUiLayout::eUiType_Frame;
	else if (strcmp("Model", frameType) == 0)
		type = CUiLayout::eUiType_Model;
	else if (strcmp("Button", frameType) == 0)
		type = CUiLayout::eUiType_Button;
	else if (strcmp("CheckButton", frameType) == 0)
		type = CUiLayout::eUiType_CheckButton;
	else if (strcmp("RadioButton", frameType) == 0)
		type = CUiLayout::eUiType_RadioButton;
	else if (strcmp("StatusBar", frameType) == 0)
		type = CUiLayout::eUiType_StatusBar;
	else if (strcmp("Slider", frameType) == 0)
		type = CUiLayout::eUiType_Slider;
	else if (strcmp("EditBox", frameType) == 0)
		type = CUiLayout::eUiType_EditBox;
	else if (strcmp("MessageFrame", frameType) == 0)
		type = CUiLayout::eUiType_MessageFrame;
	else if (strcmp("ScrollFrame", frameType) == 0)
		type = CUiLayout::eUiType_ScrollFrame;
	else if (strcmp("Tooltip", frameType) == 0)
		type = CUiLayout::eUiType_Tooltip;
	else
		return 0;

	if (g_pInterface)
	{
		CUi* ui = g_pInterface->GetUiWorld();
		if (ui)
		{
			// 檢查元件是否已經存在,回傳元件table
			CUiLayout* layout = ui->FindObject(frameName);
			if (layout != NULL)
			{
				CUiLuaLayout::PushLayoutTable(L, layout);
				return 1;
			}
			CUiLayout* parentUi = ui->FindObject(parentName);

			// 父元件
			if (parentUi)
			{
				// 父元件不能為 FontString 或者 Texture 
				switch (parentUi->GetUiType())
				{
				case CUiLayout::eUiType_FontString:
					return 0;

				case CUiLayout::eUiType_Texture:
					return 0;
				}
			}

			layout = ui->CreateObject(type, frameName, parentUi, inherit);
			if (layout)
			{
				if (parentUi && layer >= 0)
				{
					switch (type)
					{
					case CUiLayout::eUiType_FontString:
					case CUiLayout::eUiType_Texture:
						((CUiFrame*)parentUi)->SetLayers(layer, layout);
						break;
					}
				}

				if (layout->GetAnchorsSize() == 0)
				{
					layout->SetAnchor(CUiAnchor());
				}
				//layout->SetAnchorSize(CUiSize(0.0f, 0.0f));
				CUiLuaLayout::PushLayoutTable(L, layout);
				g_pInterface->PushUIComponent(layout);
				return 1;
			}
		}
	}
	return 0;
}

// ----------------------------------------------------------------------------------
int	LuaFunc_TEXT(lua_State* L)
{
	const char* text = luaL_checkstring(L, 1);
	lua_getglobal(L, text);
	if (lua_isstring(L, -1)) {
		text = lua_tostring(L, -1);
	}
	else {
		const char* gstr = g_ObjectData->GetString(text);
		if (strlen(gstr) > 0)
			text = gstr;
	}
	lua_pushstring(L, text);
	return 1;
}

// ----------------------------------------------------------------------------------
int LuaFunc_ErrorOutput(lua_State* L)
{
	switch (lua_type(L, 1))
	{
	case LUA_TTABLE:
	{
		CUiLayout* layout = CUiLuaLayout::CheckLayoutTable(L, 1);
	} break;

	case LUA_TSTRING:
	{
		UiOuputError(luaL_checkstring(L, 1));
	} break;
	}
	return 0;
}

// ----------------------------------------------------------------------------------
int	LuaFunc_GetDefaultLanguage(lua_State* L)
{
	return 0;
}

// ----------------------------------------------------------------------------------
int	LuaFunc_RunScript(lua_State* L)
{
	if (luaL_dostring(L, luaL_checkstring(L, 1)) != 0)
	{
		const char* err = lua_tostring(L, -1);
		lua_pop(L, 1);
		UiOuputError(err);
	}
	return 0;
}

// ----------------------------------------------------------------------------------
int LuaFunc_WriteFile(lua_State* L)
{
	if (lua_isstring(L, 1))
	{
		const char* str = lua_tostring(L, 1);
		int size = (int)strlen(str) * sizeof(char);
		g_pInterface->WriteFile((void*)str, size);

		/*
		WCHAR* wstr = TransToWChar(str);
		int size = wcslen(wstr) * sizeof(WCHAR);
		g_pInterface->WriteFile((void*)wstr, size);
		delete [] wstr;
		*/
	}
	return 0;
}

// ----------------------------------------------------------------------------------
int LuaFunc_SaveVariables(lua_State* L)
{
	if (lua_isstring(L, 1))
	{
		g_pInterface->SaveLuaVariable(lua_tostring(L, 1), false);
	}
	return 0;
}

// ----------------------------------------------------------------------------------
int LuaFunc_SaveVariablesPerCharacter(lua_State* L)
{
	if (lua_isstring(L, 1))
	{
		g_pInterface->SaveLuaVariable(lua_tostring(L, 1), true);
	}
	return 0;
}

int LuaFunc_DWORDAND(lua_State* L)
{
	DWORD value1 = (DWORD)luaL_checknumber(L, 1);
	DWORD value2 = (DWORD)luaL_checknumber(L, 2);
	DWORD result = (value1 & value2);
	char moo[200];
	sprintf(moo, "LuaFunc_DWORDAND value1:%d value2:%d result:%d\n", value1, value2, result);
	OutputDebugString(moo);
	lua_pushnumber(L, result);
	return 1;
}

int LuaFunc_DWORDXOR(lua_State* L)
{
	DWORD value1 = (DWORD)luaL_checknumber(L, 1);
	DWORD value2 = (DWORD)luaL_checknumber(L, 2);
	DWORD result = (value1 ^ value2);
	char moo[200];
	sprintf(moo, "LuaFunc_DWORDAND value1:%d value2:%d result:%d\n", value1, value2, result);
	OutputDebugString(moo);
	lua_pushnumber(L, result);
	return 1;
}

int LuaFunc_DWORDRSHIFT(lua_State* L)
{
	DWORD value1 = (DWORD)luaL_checknumber(L, 1);
	DWORD value2 = (DWORD)luaL_checknumber(L, 2);
	DWORD result = (value1 >> value2);
	char moo[200];
	sprintf(moo, "LuaFunc_DWORDAND value1:%d value2:%d result:%d\n", value1, value2, result);
	OutputDebugString(moo);
	lua_pushnumber(L, result);
	return 1;
}

int LuaFunc_LinkToHash(lua_State* L)
{
	lua_pushnumber(L, ItemLink_To_Hash(luaL_checkstring(L, 1)));
	return 1;
}