#include "../../netwaker_member/NetWakerClientInc.h"
#include "../mainproc/GameMain.h"
#include "../maintain/Maintain.h"
#include "../gm/GM_ObjEdit.h"
#include "../itemLink/ItemLink.h"
#include "CastingBarFrame.h"
#include "MailFrame.h"
#include "TradeFrame.h"
#include "ChatFrame.h"
#include "ChatEdit.h"
#include "GuildFrame.h"
#include "EmoteFrame.h"
#include "BotCheckFrame.h"
#include "SuitSkillFrame.h"
//#include "Map/WorldMap.h"
#include "../GuildHouses/GuildHousesFrame.h"
#include "Rune Engine/Rune/Fusion/RuFusion_Trekker.h"
#include <Tools\tools\md5\Mymd5.h>
CRoleSprite* _staticSprite = NULL;
CChatEdit* g_pChatEdit = NULL;
map<string, int>			g_chatTypeInfo;

using namespace NSObjectDataFile;

int LuaFunc_SendChatMessage(lua_State* L);
int LuaFunc_ParseText(lua_State* L);
int LuaFunc_InsertUndesirable(lua_State* L);				// 增加不接收名單
int LuaFunc_RemoveUndesirable(lua_State* L);				// 取消接收其訊息
int LuaFunc_IsUndesirable(lua_State* L);					// 是否在不受歡迎清單內

// --------------------------------------------------------------------------------------
// 特殊命令處理
void TalkToWhisper(const char* text);
void TalkToZone(const char* text);
void TalkToGM(const char* text);
void GMCommand(const char* text);
void ScriptCommand(const char* text);
void OpenMaintainFrame(const char* text);
void ModifyObj(const char* text);
void DelObj(const char* text);
void DoEmotion(const char* text);
void SetDefaultIdleMotion(const char* text);
void SetObjectID(const char* text);
void SetWeaponObjectID(const char* text);
void PlayMotion(const char* text);
void SetPaperdollPart(const char* text);
void SetTarrainDetailLevel(const char* text);
void TradeRequest(const char* text);
void TradeAgree(const char* text);
void TradeDisagree(const char* text);
void SetScale(const char* text);
void SetPos(const char* text);
void SetTargetPos(const char* text);
void SetGlow(const char* text);
void SetPlayerSpeed(const char* text);
void OpenBank(const char* text);
void ShowFps(const char* text);
void SpriteContainer(const char* text);
void SpritePos(const char* text);
void SetStableFrame(const char* text);
void SetPlayerTitle(const char* text);
void SetNpcTitle(const char* text);
void OpenMacroFrame(const char* text);
void OpenLootFrame(const char* text);
void SetWidgetGrid(const char* text);
void SetWidgetLine(const char* text);
void SendPing(const char* text);
void ListTest(const char* text);
void SetGameTime(const char* text);

//取出卡片
void TakeOutCard(const char* text);

// --------------------------------------------------------------------------------------
//登出
void Logout(const char* text);
// 物件重載 
void ReloadObj(const char* text);
// 物件版本資訊
void ObjVersion(const char* text);

// ----------------------------------------------------------------------------
// Channel 測試
void CreateChannel(const char* text);
void JoinChannel(const char* text);
void LeaveChannel(const char* text);
void TellChannel(const char* text);

void PlayerListChannel(const char* text);
void JoinDisabledChannel(const char* text);
void KickPlayerChannel(const char* text);
void SetManagerChannel(const char* text);

void ChannelZoneID(const char* text);

// --------------------------------------------------------------------------------------
// Party 測試
void PartyInvite(const char* text);
void PartyJoin(const char* text);
void PartyKickMember(const char* text);
void PartyRule(const char* text);
// --------------------------------------------------------------------------------------
//Party 物品分享
void PartyLoot(const char* text);
void PartyPass(const char* text);
void PartyAssist(const char* text);

// --------------------------------------------------------------------------------------
//好友名單 測試
void FriendInvite(const char* text);
void FriendJoin(const char* text);
void FriendDel(const char* text);

void FriendSetInfo(const char* text);
void FriendSetFriend(const char* text);

// --------------------------------------------------------------------------------------
//AC 測試
void	ACSell(const char* text);
void	ACSellCancel(const char* text);
void	ACSearch(const char* text);
void	ACSearchNext(const char* text);
void	ACClose(const char* text);
void	ACBuy(const char* text);
void	ACItemHistory(const char* text);

//施法測試
void Spell(const char* text);
//配法術TP
void SetTp(const char* text);


//合成測試
void Refine(const char* text);

//公會測試
void GuildRegister(const char* text);
void GuildPetitionSignature(const char* text);
void GuildPetitionLeave(const char* text);
void GuildPetitionQuit(const char* text);

void NewGuildCreate(const char* text);
void CloseCreateGuild(const char* text);
void NewGuildInvite(const char* text);
void NewGuildJoin(const char* text);
void NewGuildLeave(const char* text);
void NewGuildDel(const char* text);

void GuildInvite(const char* text);
void GuildJoin(const char* text);
void GuildLeave(const char* text);
void GuildDel(const char* text);

void GuildModify(const char* text);
void GuildModifyMember(const char* text);
void GuildInfo(const char* text);
void GuildMemberList(const char* text);
void GuildMemberInfo(const char* text);


//公會留言板測試
void GuildBoardPost(const char* text);
void GuildBoardList(const char* text);
void GuildBoardMessage(const char* text);
void GuildBoardMsgModify(const char* text);
void GuildBoardTypeModify(const char* text);

//公會供獻測試
void GuildContributionItem(const char* text);
void GuildContributionOK(const char* text);
void GuildContributionClose(const char* text);

//公會競標
void GuildFightAuctionClose(const char* text);
void GuildFightAuctionBit(const char* text);

//樂透彩測試
void LotteryExchangePrize(const char* text);
void LotteryCloseExchangePrize(const char* text);
void LotteryBuy(const char* text);
void LotteryCloseBuy(const char* text);
void LotteryInfoRequest(const char* text);


//測試取地上物品
void GetGroundItem(const char* text);

//測試 使用物品對物品
void UseItemToItem(const char* text);
void UseBagItem(const char* text);

//測試 排行榜
void BillboardIsAnonymous(const char* text);
void BillboardRankList(const char* text);
void BillboardClose(const char* text);

//測試 pk
void PKInvite(const char* text);
void PKAccept(const char* text);
void PKCancel(const char* text);


void Casting_Start(const char* text);
void Casting_End(const char* text);
void Casting_Failed(const char* text);
void Casting_Delay(const char* text);

void ShowObjPath(const char* text);

void Open_GM_ObjEdit(const char* text);
void Open_GM_Config(const char* text);
void Open_SYS_AudioSettings(const char* text);
void Open_SYS_DisplaySettings(const char* text);

void Flush_SoundCache(const char* text);

void FDBJournal_Begin(const char* text);
void FDBJournal_End(const char* text);

void WDBJournal_Begin(const char* text);
void WDBJournal_End(const char* text);

void PaperdollCache_ActivateWrites(const char* text);

void SetUIScale(const char* text);

void FlagTest(const char* text);

//全區旗標測試
void AllZoneStateTest(const char* text);
void SearchFindPartyPlayerTest(const char* text);

// 特殊行為挖制
void PlayerBlind(const char* text);

//設定霧距
void SetFog(const char* text);

// 設定滑鼠移動控制
void SetMouseMove(const char* text);

//cli to Cli 封包轉送測試
void ClitoCliNetTest(const char* text);

//使用法術
void CastSpellByID(const char* text);

// 裝備頭盔
void SetHelmetVisible(const char* text);

//////////////////////////////////////////////////////////////////////////
//小地圖怪物顯示
//////////////////////////////////////////////////////////////////////////
//要求更新怪物資訊
void NPCPosInfo_Reset(const char* text);
void NPCPosInfo_Color_Lv(const char* text);
void NPCPosInfo_Clear(const char* text);
void NPCPosInfo_Show_ID(const char* text);
void NPCPosInfo_Show_Type(const char* text);

//////////////////////////////////////////////////////////////////////////
// 房屋系統測試
//////////////////////////////////////////////////////////////////////////
void VisitHouseRequest(const char* text);
void CloseVisitHouse(const char* text);
void SwapBodyHouseItem(const char* text);
void SwapHouseItem(const char* text);
void HouseItemLayoutRequest(const char* text);
void ChangeHouseRequest(const char* text);
void BuyFunctionRequest(const char* text);
void FunctionSettingRequest(const char* text);
void BuyEnergyRequest(const char* text);

void LeaveHouse(const char* text);
void SetHousePassword(const char* text);
void ExchangeEq(const char* text);
// --------------------------------------------------------------------------------------
void ReloadUI(const char* text);
void ReloadCliScript(const char* text);

//公會屋戰測試
void GuildHouseWarReg(const char* text);
void GuildHouseWarEnter(const char* text);
void GuildHouseWarLeave(const char* text);

// 公會戰測試
// --------------------------------------------------------------------------------------
void GuildMemberDBID(const char* text);
void GuildFightInvite(const char* text);
// --------------------------------------------------------------------------------------
//全域劇情測試
void RunGlobalPlot(const char* text);
//
void ShowError(const char* text);
// --------------------------------------------------------------------------------------
//新商城測試( )
void DS_Function(const char* text);
void DS_SellType(const char* text);
void DS_ShopInfo(const char* text);
void DS_BuyItem(const char* text);

// --------------------------------------------------------------------------------------
/*
void ManageKey			( const char* text );
void ManagePassword		( const char* text );
void DisableLogin		( const char* text );
*/
// --------------------------------------------------------------------------------------
//公會PK測試宣戰
void GuildDeclareWar(const char* text);
void GuildWarList(const char* text);
void GuildID(const char* text);

// --------------------------------------------------------------------------------------

void ClientCrash(const char* text);
// --------------------------------------------------------------------------------------
//給予物品跑劇情 測試
void GiveItemRunPlot(const char* text);

void JoinBG(const char* text);
void EnterBG(const char* text);
void LeaveBG(const char* text);

void LearnRecipe(const char* text);

void SetCameraZoomLocked(const char* text);
void SetFreeCamera(const char* text);
void TestRecipe(const char* text);
void ReleaseAllFontIndex(const char* text);
void PathMoveTo(const char* text);

void ToggleOptionRenderTerrain(const char* text);
void CreateBackEntity(const char* text);

void GetBGScore(const char* text);
void GetBGStatus(const char* text);

void FuncCmd_CreateSprite(const char* text);
void FuncCmd_SetTargetDirection(const char* text);

void FuncCmd_CheckGuildName(const char* text);

// --------------------------------------------------------------------------------------
void CreateEffect(const char* text);

// --------------------------------------------------------------------------------------
void SwitchUpdateElapsedTime(const char* text);

// --------------------------------------------------------------------------------------
void FuncCmd_AddSpecialEventFilter(const char* text);
void FuncCmd_RemoveSpecialEventFilter(const char* text);
void FuncCmd_ClearSpecialEventFilter(const char* text);

// --------------------------------------------------------------------------------------
void FuncCmd_SaveRoleData(const char* text);
void FuncCmd_LoadRoleData(const char* text);

// --------------------------------------------------------------------------------------
void FuncCmd_CheckUniqueItem(const char* text);

// --------------------------------------------------------------------------------------
void FuncCmd_AttachMountSprite(const char* text);
void FuncCmd_DetachMountSprite(const char* text);
void FuncCmd_DetachImplement(const char* text);

// --------------------------------------------------------------------------------------
void FuncCmd_OpenBotCheck(const char* text);

// --------------------------------------------------------------------------------------
//void VivoxCommand		( const char* text );
void FuncCmd_VivoxCommand(const char* text);
void FuncCmd_VivoxEnable(const char* text);
void FuncCmd_VivoxDisable(const char* text);
void FuncCmd_VivoxJoin(const char* text);
void FuncCmd_VivoxExit(const char* text);
void FuncCmd_VivoxInMode(const char* text);
void FuncCmd_VivoxInModeG(const char* text);
//
void FuncCmd_VivoxPartMute(const char* text);
void FuncCmd_VivoxPartMuteG(const char* text);
//
void FuncCmd_VivoxDebugLV(const char* text);
void FuncCmd_VivoxDebugV(const char* text);

// --------------------------------------------------------------------------------------
void FuncCmd_SetCameraFollowTarget(const char* text);
// --------------------------------------------------------------------------------------
void FuncCmd_GetRankPersonalInfo(const char* text);

// --------------------------------------------------------------------------------------
void FuncCmd_CreateTargetEffect(const char* text);
void FuncCmd_SetTargetEffectType(const char* text);
void FuncCmd_ReleaseTargetEffect(const char* text);
void FuncCmd_SetTargetEffectVisible(const char* text);

// --------------------------------------------------------------------------------------
void FuncCmd_FindAttachPoint(const char* text);
// --------------------------------------------------------------------------------------
void FuncCmd_TestPacket(const char* text);
// --------------------------------------------------------------------------------------
void FuncCmd_DisplayEntiptyPointList(const char* text);
// --------------------------------------------------------------------------------------
void FuncCmd_SetSuitSkill(const char* text);
// --------------------------------------------------------------------------------------
void FuncCmd_MovePartition(const char* text);
// --------------------------------------------------------------------------------------
void FuncCmd_SetPlayerName(const char* text);
// --------------------------------------------------------------------------------------
void GarbageCollect(const char* text);
void GetZonecoordinate(const char* text);

// --------------------------------------------------------------------------------------
void SetDefaultWalkingAction(const char* text);
void SetDefaultRunningAction(const char* text);

void TestGMMsg(const char* text);

void FuncCmd_SetCameraControlMode(const char* text);

void FuncCmd_Play3DSoundByFile(const char* text);
void FuncCmd_PlaySoundByFile(const char* text);
void FuncCmd_CreateSpriteActStatic(const char* text);
void FuncCmd_CheckMd5(const char* text);
void FuncCmd_RequestBufferInfo(const char* text);

// --------------------------------------------------------------------------------------
CSplitArgString::CSplitArgString()
{
	m_str = NULL;
}

// --------------------------------------------------------------------------------------
CSplitArgString::CSplitArgString(const char* str)
{
	m_str = NULL;
	SetString(str);
}

// --------------------------------------------------------------------------------------
CSplitArgString::~CSplitArgString()
{
	if (m_str)
		delete[] m_str;
	m_str = NULL;
}

// --------------------------------------------------------------------------------------
void CSplitArgString::SetString(const char* str)
{
	if (str == NULL)
		return;

	m_startIndex.clear();
	m_endIndex.clear();
	m_startIndex.push_back(0);

	char* start = (char*)str;
	char* end;
	while (end = strchr(start, ' '))
	{
		m_endIndex.push_back(int(end - str));
		start = end;
		while (*start == ' ') start++;
		m_startIndex.push_back(int(start - str));
	}

	if (m_startIndex.size() != m_endIndex.size())
		m_endIndex.push_back((int)strlen(str));

	SAFE_DELETE(m_str);
	m_str = new char[strlen(str) + 1];
	strcpy(m_str, str);
}

// --------------------------------------------------------------------------------------
string CSplitArgString::GetString(int start, int end)
{
	string content;

	if (end == -1)
		end = (int)m_endIndex.size() - 1;

	start = max(start, 0);
	start = min(start, (int)(m_startIndex.size() - 1));
	end = max(end, 0);
	end = min(end, (int)(m_endIndex.size() - 1));

	if (m_str && start >= 0)
	{
		char* first = m_str + m_startIndex[start];
		content.assign(first, m_endIndex[end] - m_startIndex[start]);
	}

	return content;
}

// ----------------------------------------------------------------------------
CChatEdit::CChatEdit(CInterface* object)
	: CInterfaceBase(object)
{
	// 建立發話頻道字串跟代碼索引
	g_chatTypeInfo.insert(make_pair("SAY", eChatType_SAY));
	g_chatTypeInfo.insert(make_pair("YELL", eChatType_YELL));
	g_chatTypeInfo.insert(make_pair("ZONE", eChatType_ZONE));
	g_chatTypeInfo.insert(make_pair("WHISPER", eChatType_WHISPER));
	g_chatTypeInfo.insert(make_pair("PARTY", eChatType_PARTY));
	g_chatTypeInfo.insert(make_pair("RAID", eChatType_RAID));
	g_chatTypeInfo.insert(make_pair("GUILD", eChatType_GUILD));
	g_chatTypeInfo.insert(make_pair("GM", eChatType_GM));
	g_chatTypeInfo.insert(make_pair("CHANNEL", eChatType_CHANNEL));
	g_chatTypeInfo.insert(make_pair("BG", eChatType_BG));
	/*
	g_chatTypeInfo.insert( make_pair("YELL", eChatType_YELL) );
	g_chatTypeInfo.insert( make_pair("LFG", eChatType_LFG) );
	g_chatTypeInfo.insert( make_pair("SALE", eChatType_SALE) );
	g_chatTypeInfo.insert( make_pair("CHANNEL", eChatType_CHANNEL) );
	*/

	//RegisterCmd("help",	NULL, Help);
	RegisterCmd("t", "talk to player", TalkToWhisper);
	RegisterCmd("zone", "talk to zone", TalkToZone);
	RegisterCmd("gm", "talk to gm", TalkToGM, 0);
	RegisterCmd("?", "GM command", GMCommand);
	RegisterCmd("script", "lua script command", ScriptCommand);
	RegisterCmd("new", "", OpenMaintainFrame);
	RegisterCmd("modify", "", ModifyObj);
	RegisterCmd("del", "", DelObj);

	RegisterCmd("emotion", "", DoEmotion);
	RegisterCmd("SetDefaultIdleMotion", "", SetDefaultIdleMotion);
	RegisterCmd("setobjectid", "", SetObjectID);
	RegisterCmd("setweaponobjectid", "", SetWeaponObjectID);
	RegisterCmd("playmotion", "", PlayMotion);
	RegisterCmd("setpaperdollpart", "", SetPaperdollPart);

	RegisterCmd("terrain_detail_level", "", SetTarrainDetailLevel);
	RegisterCmd("trade_request", "", TradeRequest);
	RegisterCmd("trade_agree", "", TradeAgree);
	RegisterCmd("trade_disagree", "", TradeDisagree);
	RegisterCmd("scale", "", SetScale);
	RegisterCmd("set_pos", "", SetPos);
	RegisterCmd("settargetpos", "", SetTargetPos);
	RegisterCmd("glow", "", SetGlow);
	RegisterCmd("speed", "", SetPlayerSpeed);
	RegisterCmd("bank", "", OpenBank);
	RegisterCmd("fps", "", ShowFps);
	RegisterCmd("sprite_container", "", SpriteContainer);
	RegisterCmd("sprite_pos", "", SpritePos);
	RegisterCmd("stable_frame", "", SetStableFrame);
	RegisterCmd("set_player_title", "", SetPlayerTitle);
	RegisterCmd("set_npc_title", "", SetNpcTitle);
	RegisterCmd("macro", "", OpenMacroFrame);
	RegisterCmd("loot", "", OpenLootFrame);
	RegisterCmd("grid", "", SetWidgetGrid);
	RegisterCmd("move_line", "", SetWidgetLine);

	//登出
	RegisterCmd("logout", "", Logout);
	//物件重載
	RegisterCmd("reloadobj", "", ReloadObj);

	//頻道測試
	RegisterCmd("ccreate", "", CreateChannel);
	RegisterCmd("cjoin", "", JoinChannel);
	RegisterCmd("cleave", "", LeaveChannel);
	RegisterCmd("ctell", "", TellChannel);

	RegisterCmd("cplayerlist", "", PlayerListChannel);
	RegisterCmd("cjoindisabled", "", JoinDisabledChannel);
	RegisterCmd("ckickplayer", "", KickPlayerChannel);
	RegisterCmd("csetmanager", "", SetManagerChannel);
	RegisterCmd("czoneid", "", ChannelZoneID);

	//Party 測試
	RegisterCmd("partyinvite", "", PartyInvite);
	RegisterCmd("partyjoin", "", PartyJoin);
	RegisterCmd("partykickMember", "", PartyKickMember);
	RegisterCmd("partyrule", "", PartyRule);

	RegisterCmd("partyloot", "", PartyLoot);
	RegisterCmd("partypass", "", PartyPass);
	RegisterCmd("partyassist", "", PartyAssist);


	RegisterCmd("friendinvite", "", FriendInvite);
	RegisterCmd("friendjoin", "", FriendJoin);
	RegisterCmd("frienddel", "", FriendDel);
	RegisterCmd("friendsetinfo", "", FriendSetInfo);
	RegisterCmd("friendsetfriend", "", FriendSetFriend);

	//ac 測試
	RegisterCmd("acsell", "", ACSell);
	RegisterCmd("acsellcancel", "", ACSellCancel);
	RegisterCmd("acsearch", "", ACSearch);
	RegisterCmd("acsearchnext", "", ACSearchNext);
	RegisterCmd("acclose", "", ACClose);
	RegisterCmd("acbuy", "", ACBuy);
	RegisterCmd("acitemhistory", "", ACItemHistory);

	//施法測試
	RegisterCmd("spell", "", Spell);
	RegisterCmd("settp", "", SetTp);

	//合成測試
	RegisterCmd("refine", "", Refine);

	//公會測試
	RegisterCmd("guild_register", "", GuildRegister);
	RegisterCmd("guild_petition_signature", "", GuildPetitionSignature);
	RegisterCmd("guild_petition_leave", "", GuildPetitionLeave);
	RegisterCmd("guild_petition_quit", "", GuildPetitionQuit);

	RegisterCmd("newguildcreate", "", NewGuildCreate);
	RegisterCmd("CloseCreateGuild", "", CloseCreateGuild);

	RegisterCmd("newguildinvite", "", NewGuildInvite);
	RegisterCmd("newguildjoin", "", NewGuildJoin);
	RegisterCmd("newguildleave", "", NewGuildLeave);
	RegisterCmd("newguilddel", "", NewGuildDel);

	RegisterCmd("guildinvite", "", GuildInvite);
	RegisterCmd("guildjoin", "", GuildJoin);
	RegisterCmd("guildleave", "", GuildLeave);
	RegisterCmd("guilddel", "", GuildDel);

	RegisterCmd("guildmodify", "", GuildModify);
	RegisterCmd("guildmodifymember", "", GuildModifyMember);
	RegisterCmd("guildinfo", "", GuildInfo);
	RegisterCmd("guildmemberlist", "", GuildMemberList);
	RegisterCmd("guildmemberinfo", "", GuildMemberInfo);

	//公會留言版測試
	RegisterCmd("GuildBoardPost", "", GuildBoardPost);
	RegisterCmd("GuildBoardList", "", GuildBoardList);
	RegisterCmd("GuildBoardMessage", "", GuildBoardMessage);
	RegisterCmd("GuildBoardMsgModify", "", GuildBoardMsgModify);
	RegisterCmd("GuildBoardTypeModify", "", GuildBoardTypeModify);

	//公會供獻測試
	RegisterCmd("GuildContributionItem", "", GuildContributionItem);
	RegisterCmd("GuildContributionOK", "", GuildContributionOK);
	RegisterCmd("GuildContributionClose", "", GuildContributionClose);
	//公會競標
	RegisterCmd("GuildFightAuctionClose", "", GuildFightAuctionClose);
	RegisterCmd("GuildFightAuctionBit", "", GuildFightAuctionBit);

	//排行榜測試
	RegisterCmd("BillboardIsAnonymous", "", BillboardIsAnonymous);
	RegisterCmd("BillboardRankList", "", BillboardRankList);
	RegisterCmd("BillboardClose", "", BillboardClose);

	//測試 pk
	RegisterCmd("PKInvite", "", PKInvite);
	RegisterCmd("PKAccept", "", PKAccept);
	RegisterCmd("PKCancel", "", PKCancel);

	// 物件版本資訊
	RegisterCmd("ObjVersion", "", ObjVersion);

	RegisterCmd("Ping", "", SendPing);

	RegisterCmd("GetGroundItem", "", GetGroundItem);
	RegisterCmd("UseItemToItem", "", UseItemToItem);
	RegisterCmd("UseBagItem", "", UseBagItem);

	RegisterCmd("Casting_Start", "", Casting_Start);
	RegisterCmd("Casting_End", "", Casting_End);
	RegisterCmd("Casting_Failed", "", Casting_Failed);
	RegisterCmd("Casting_Delay", "", Casting_Delay);

	// 巡邏劇情資訊
	RegisterCmd("ShowObjPath", "", ShowObjPath);

	// System settings
	RegisterCmd("objedit", "", Open_GM_ObjEdit);
	RegisterCmd("gm_objedit", "", Open_GM_ObjEdit);
	RegisterCmd("gm_config", "", Open_GM_Config);

	RegisterCmd("sys_audiosettings", "", Open_SYS_AudioSettings);
	RegisterCmd("sys_displaysettings", "", Open_SYS_DisplaySettings);

	RegisterCmd("flushsoundcache", "", Flush_SoundCache);

	// Other maintenance commands
	RegisterCmd("fdbjournal_begin", "", FDBJournal_Begin);
	RegisterCmd("fdbjournal_end", "", FDBJournal_End);

	RegisterCmd("wdbjournal_begin", "", WDBJournal_Begin);
	RegisterCmd("wdbjournal_end", "", WDBJournal_End);

	RegisterCmd("paperdollcache_activatewrites", "", PaperdollCache_ActivateWrites);

	// 
	RegisterCmd("ui_scale", "", SetUIScale);

	RegisterCmd("FlagTest", "", FlagTest);


	RegisterCmd("AllZoneStateTest", "", AllZoneStateTest);
	RegisterCmd("SearchFindPartyPlayerTest", "", SearchFindPartyPlayerTest);
	RegisterCmd("ListTest", "", ListTest);

	RegisterCmd("blind", "", PlayerBlind);
	RegisterCmd("setgametime", "", SetGameTime);

	RegisterCmd("TakeOutCard", "", TakeOutCard);
	RegisterCmd("SetFog", "", SetFog);

	RegisterCmd("SetMouseMove", "", SetMouseMove);
	RegisterCmd("ClitoCliNetTest", "", ClitoCliNetTest);

	//樂透彩測試
	RegisterCmd("LotteryExchangePrize", "", LotteryExchangePrize);
	RegisterCmd("LotteryCloseExchangePrize", "", LotteryCloseExchangePrize);
	RegisterCmd("LotteryBuy", "", LotteryBuy);
	RegisterCmd("LotteryCloseBuy", "", LotteryCloseBuy);
	RegisterCmd("LotteryInfoRequest", "", LotteryInfoRequest);

	RegisterCmd("CastSpellByID", "", CastSpellByID);
	RegisterCmd("SetHelmetVisible", "", SetHelmetVisible);

	RegisterCmd("NPCPos_Reset", "", NPCPosInfo_Reset);
	RegisterCmd("NPCPos_Color_Lv", "", NPCPosInfo_Color_Lv);
	RegisterCmd("NPCPos_Clear", "", NPCPosInfo_Clear);
	RegisterCmd("NPCPos_Show_ID", "", NPCPosInfo_Show_ID);
	RegisterCmd("NPCPos_Show_Type", "", NPCPosInfo_Show_Type);

	RegisterCmd("Play3DSoundByFile", "", FuncCmd_Play3DSoundByFile);
	RegisterCmd("PlaySoundByFile", "", FuncCmd_PlaySoundByFile);
	//房屋測試
	RegisterCmd("VisitHouseRequest", "", VisitHouseRequest);
	RegisterCmd("CloseVisitHouse", "", CloseVisitHouse);
	RegisterCmd("SwapBodyHouseItem", "", SwapBodyHouseItem);
	RegisterCmd("SwapHouseItem", "", SwapHouseItem);
	RegisterCmd("HouseItemLayoutRequest", "", HouseItemLayoutRequest);
	RegisterCmd("ChangeHouseRequest", "", ChangeHouseRequest);
	RegisterCmd("BuyFunctionRequest", "", BuyFunctionRequest);
	RegisterCmd("FunctionSettingRequest", "", FunctionSettingRequest);
	RegisterCmd("BuyEnergyRequest", "", BuyEnergyRequest);
	RegisterCmd("LeaveHouse", "", LeaveHouse);
	RegisterCmd("SetHousePassword", "", SetHousePassword);
	RegisterCmd("ExchangeEq", "", ExchangeEq);

	// 
	RegisterCmd("ReloadUI", "", ReloadUI, 0);
	RegisterCmd("ReloadCliScript", "", ReloadCliScript);

	RegisterCmd("GuildMemberDBID", "", GuildMemberDBID);
	RegisterCmd("GuildFightInvite", "", GuildFightInvite);
	RegisterCmd("RunGlobalPlot", "", RunGlobalPlot);

	//
	RegisterCmd("ShowError", "", ShowError);


	RegisterCmd("DS_Function", "", DS_Function);
	RegisterCmd("DS_SellType", "", DS_SellType);
	RegisterCmd("DS_ShopInfo", "", DS_ShopInfo);
	RegisterCmd("DS_BuyItem", "", DS_BuyItem);
	/*
	RegisterCmd("ManageKey"							, "" , ManageKey				);
	RegisterCmd("ManagePassword"					, "" , ManagePassword			);
	RegisterCmd("DisableLogin"						, "" , DisableLogin				);
	*/
	//公會屋戰測試
	RegisterCmd("GuildHouseWarReg", "", GuildHouseWarReg);
	RegisterCmd("GuildHouseWarEnter", "", GuildHouseWarEnter);
	RegisterCmd("GuildHouseWarLeave", "", GuildHouseWarLeave);

	//公會PK測試宣戰
	RegisterCmd("GuildDeclareWar", "", GuildDeclareWar);
	RegisterCmd("GuildWarList", "", GuildWarList);
	RegisterCmd("GuildID", "", GuildID);

	RegisterCmd("ClientCrash", "", ClientCrash);
	RegisterCmd("GiveItemRunPlot", "", GiveItemRunPlot);

	RegisterCmd("JoinBG", "", JoinBG);
	RegisterCmd("EnterBG", "", EnterBG);
	RegisterCmd("LeaveBG", "", LeaveBG);
	RegisterCmd("LearnRecipe", "", LearnRecipe);



	RegisterCmd("CameraZoom", "", SetCameraZoomLocked);
	RegisterCmd("SetFreeCamera", "", SetFreeCamera);
	RegisterCmd("TestRecipe", "", TestRecipe);
	RegisterCmd("ReleaseAllFontIndex", "", ReleaseAllFontIndex);
	RegisterCmd("PathMoveTo", "", PathMoveTo);
	RegisterCmd("ToggleTerrainRender", "", ToggleOptionRenderTerrain);
	RegisterCmd("CreateBackEntity", "", CreateBackEntity);
	RegisterCmd("GetBGScore", "", GetBGScore);
	RegisterCmd("GetBGStatus", "", GetBGStatus);

	RegisterCmd("CreateEffect", "", CreateEffect);

	RegisterCmd("SwitchUpdateTime", "", SwitchUpdateElapsedTime);

	RegisterCmd("AddSpecialEventFilter", "", FuncCmd_AddSpecialEventFilter);
	RegisterCmd("RemoveSpecialEventFilter", "", FuncCmd_RemoveSpecialEventFilter);
	RegisterCmd("ClearSpecialEventFilter", "", FuncCmd_ClearSpecialEventFilter);

	RegisterCmd("CreateSprite", "", FuncCmd_CreateSprite);
	RegisterCmd("SetTargetDirection", "", FuncCmd_SetTargetDirection);
	RegisterCmd("CheckGuildName", "", FuncCmd_CheckGuildName);

	RegisterCmd("SaveRoleData", "", FuncCmd_SaveRoleData);
	RegisterCmd("LoadRoleData", "", FuncCmd_LoadRoleData);

	RegisterCmd("CheckUniqueItem", "", FuncCmd_CheckUniqueItem);

	RegisterCmd("AttachMountSprite", "", FuncCmd_AttachMountSprite);
	RegisterCmd("DetachMountSprite", "", FuncCmd_DetachMountSprite);
	RegisterCmd("DetachImplement", "", FuncCmd_DetachImplement);

	RegisterCmd("OpenBotCheck", "", FuncCmd_OpenBotCheck);

	// Vivox
	//RegisterCmd("vivox"								, "" , VivoxCommand			);
	RegisterCmd("vivox", "", FuncCmd_VivoxCommand);
	RegisterCmd("VivoxEnable", "", FuncCmd_VivoxEnable);
	RegisterCmd("VivoxJoin", "", FuncCmd_VivoxJoin);
	RegisterCmd("VivoxExit", "", FuncCmd_VivoxExit);
	RegisterCmd("VivoxOff", "", FuncCmd_VivoxDisable);
	RegisterCmd("VivoxInMode", "", FuncCmd_VivoxInMode);
	RegisterCmd("VivoxInModeG", "", FuncCmd_VivoxInModeG);
	RegisterCmd("vivoxPmute", "", FuncCmd_VivoxPartMute);
	RegisterCmd("vivoxPmuteG", "", FuncCmd_VivoxPartMuteG);
	RegisterCmd("vvxdebugLV", "", FuncCmd_VivoxDebugLV);
	RegisterCmd("vvxdebugV", "", FuncCmd_VivoxDebugV);

	RegisterCmd("SetCameraFollowTarget", "", FuncCmd_SetCameraFollowTarget);
	RegisterCmd("GetRankPersonalInfo", "", FuncCmd_GetRankPersonalInfo);

	RegisterCmd("CreateTargetEffect", "", FuncCmd_CreateTargetEffect);
	RegisterCmd("ReleaseTargetEffect", "", FuncCmd_ReleaseTargetEffect);
	RegisterCmd("SetTargetEffectType", "", FuncCmd_SetTargetEffectType);
	RegisterCmd("SetTargetEffectVisible", "", FuncCmd_SetTargetEffectVisible);

	RegisterCmd("FindAttachPoint", "", FuncCmd_FindAttachPoint);

	RegisterCmd("TestPacket", "", FuncCmd_TestPacket);

	RegisterCmd("DisplayEntiptyPointList", "", FuncCmd_DisplayEntiptyPointList);
	RegisterCmd("SetSuitSkill", "", FuncCmd_SetSuitSkill);
	RegisterCmd("MovePartition", "", FuncCmd_MovePartition);
	RegisterCmd("SetPlayerName", "", FuncCmd_SetPlayerName);
	RegisterCmd("GarbageCollect", "", GarbageCollect);
	RegisterCmd("GetZonecoordinate", "", GetZonecoordinate);

	RegisterCmd("SetDefaultWalkingAction", "", SetDefaultWalkingAction);
	RegisterCmd("SetDefaultRunningAction", "", SetDefaultRunningAction);

	RegisterCmd("TestGMMsg", "", TestGMMsg);

	RegisterCmd("SetCameraControlMode", "", FuncCmd_SetCameraControlMode);
	RegisterCmd("CreateSpriteActStatic", "", FuncCmd_CreateSpriteActStatic);
	RegisterCmd("CheckMD5", "", FuncCmd_CheckMd5);
	RegisterCmd("RequestBufferInfo", "", FuncCmd_RequestBufferInfo);
	
	g_pChatEdit = this;
	m_channel = eChatType_Range;

	memset(m_sendCount, 0, sizeof(m_sendCount));
}

// ----------------------------------------------------------------------------
CChatEdit::~CChatEdit()
{
	g_pChatEdit = NULL;
}

// ----------------------------------------------------------------------------
void CChatEdit::RegisterFunc()
{
	lua_State* l = GetLuaStateWorld();
	if (l)
	{
		lua_register(l, "SendChatMessage", LuaFunc_SendChatMessage);
		lua_register(l, "ParseText", LuaFunc_ParseText);

		lua_register(l, "InsertUndesirable", LuaFunc_InsertUndesirable);
		lua_register(l, "RemoveUndesirable", LuaFunc_RemoveUndesirable);
		lua_register(l, "IsUndesirable", LuaFunc_IsUndesirable);
	}
}

// ----------------------------------------------------------------------------
void CChatEdit::Update(float elapsedTime)
{
	for (int i = 0; i < 20; i++)
	{
		m_sendCount[i] -= elapsedTime;
		if (m_sendCount[i] < 0.0f)
			m_sendCount[i] = 0.0f;

		m_banTime[i] -= elapsedTime;
		if (m_banTime[i] < 0.0f)
			m_banTime[i] = 0.0f;
	}
}

// ----------------------------------------------------------------------------
void CChatEdit::RegisterCmd(const char* cmdStr, const char* explain, com_func func, int powerLevel)
{
	if (cmdStr == NULL || func == NULL)
		return;

	string tmp(cmdStr);
	_strlwr((char*)tmp.c_str());

	CommandInfo info;
	if (explain)
		info.explain = explain;
	info.func = func;
	info.powerLevel = powerLevel;

	m_command.insert(make_pair(tmp, info));

	/*
	//交易測試
	RegisterCmd("opentrade", NULL, UI_OpenTrade);
	RegisterCmd("trade", NULL, Trade);
	RegisterCmd("agreetrade", NULL, AgreeTrade);


	RegisterCmd("title", "{on or off} object title", TitleVisible);
	RegisterCmd("sprite_update", "{on or off}", SpriteUpdate);
	RegisterCmd("sprite_render", "{on or off}", SpriteRender);

	RegisterCmd("Touch", NULL, Touch);

	RegisterCmd("newobj", NULL, NewObj);

	RegisterCmd("useitem", NULL, UseItem_Body);

	//設定點術策試
	RegisterCmd("setpoint", "{ str or sta or int ...}" , SetPoint);
	RegisterCmd("spellmagic", NULL, SpellMagic);
	RegisterCmd("showfps", "{on or off}", ShowFps);
	RegisterCmd("camera_set", "{x, y, z}", SetCameraPos);
	RegisterCmd("camera_move", "{x, y, z}", MoveCameraPos);
	RegisterCmd("camera_far_plane", "{far(int)}", SetCameraFarPlane);
	RegisterCmd("setfog", "{min(int) max(int)}", SetFog);
	RegisterCmd("glow", "{on or off}", Glow);
	RegisterCmd("options", "open options frame", OpenOptionsFrame);
	*/
}

// ----------------------------------------------------------------------------
// bit 31已經禁言, 0-30 禁言次數
void CChatEdit::SetBanChannelTime(int channel, float time)
{
	if (channel < 0 || channel >= 20)
		return;

	m_banTime[channel] += time;
	if (m_banTime[channel] > 7200.0f)
		m_banTime[channel] = 7200.0f;
}

// ----------------------------------------------------------------------------
void CChatEdit::SendChatMessage(ChatTypeInfo type, const char* text, const char* language, const char* extra)
{
	if (text == NULL)
		return;

	string linkType, linkData, linkName;
	if (ParseHyperlink(text, linkType, linkData, linkName) == true)
	{
		// 內容不能有"/"
		if (strchr(linkData.c_str(), '/') != NULL)
			return;

		if (strchr(linkName.c_str(), '/') != NULL)
			return;
	}

	int len = (int)strlen(text);
	char buf[1024];

	Translator(text, buf);			// 轉譯特別字元	
	IgnoreColorCode(buf);

	char playerName[MAX_PATH];
	memset(playerName, 0, sizeof(playerName));

	CRoleSprite* player = g_pGameMain->GetPlayer();
	TalkSenderInfo senderInfo;

	if (player)
	{
		strcpy(playerName, player->GetName());
		senderInfo.vocation[0] = player->GetVocation();
		senderInfo.vocation[1] = player->GetVocationSub();
		senderInfo.level[0] = player->GetLevel();
		senderInfo.level[1] = player->GetLevelSub();
	}

	// 你已經被系統警告不能發言
	if (m_banTime[type] > 0.0f)
	{
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_BAN_TALK"));
		return;
	}

	// 計算重複發話
	float frameTime = (float)g_pGameMain->GetFrameTime();
	if (type != eChatType_GM && m_sendCount[type] > 9.99f)
	{
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_SEND_MESSAGE_WARNING"));
		return;
	}
	else
	{
		switch (type)
		{
		case eChatType_ZONE:
			m_sendCount[type] += 20.0f;
			break;

		default:
			m_sendCount[type] += 2.0f;
			break;
		}
	}

	switch (type)
	{
	case eChatType_SAY:				// 範圍
		NetCli_Talk::Range(buf, senderInfo);
		break;

	case eChatType_ZONE:			// 區域
		NetCli_Talk::Channel(EM_GroupObjectType_SysZoneChannel, g_pGameMain->GetZoneID(), playerName, buf, senderInfo);
		break;

	case eChatType_YELL:			// 世界頻道
		if (g_pGameMain->IsBattleGroundZone())
		{
			g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_ERR_CHAT_INBATTLEGROUND"));
			g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_ERR_CHAT_INBATTLEGROUND"));
			return;
		}
		NetCli_Talk::Channel(EM_GroupObjectType_SysChannel, 0, playerName, buf, senderInfo);
		break;

	case eChatType_WHISPER:			// 密語
		if (extra != NULL)
		{
			NetCli_Talk::TalkTo(extra, buf, senderInfo);
		}
		break;

	case eChatType_PARTY:			// 隊伍
		switch (NetCli_PartyChild::GetPartyState())
		{
		case ePartyState_Leader:				// 隊伍領隊
		case ePartyState_Member:				// 隊伍成員
			NetCli_Talk::Channel(EM_GroupObjectType_Party, 0, playerName, buf, senderInfo);
			break;

		default:
			g_pChatFrame->SendMsgEvent(CHAT_MSG_SYSTEM, g_ObjectData->GetString("SYS_ERR_CHAT_NOPARTY"), playerName);
			break;
		}
		break;

	case eChatType_RAID:			// 團隊
		break;

	case eChatType_GUILD:			// 公會
		if (player == NULL)
			return;

		//if ( NetCli_GuildChild::GetGuildNameByID(player->GetGuildID()) )
		if (player->GetGuildID() > 0)
			NetCli_Talk::Channel(EM_GroupObjectType_Guild, 0, playerName, buf, senderInfo);
		else
			g_pChatFrame->SendMsgEvent(CHAT_MSG_SYSTEM, g_ObjectData->GetString("SYS_ERR_CHAT_NOGUILD"), playerName);
		break;

	case eChatType_LFG:				// 招幕
		break;

	case eChatType_SALE:			// 買賣
		break;

	case eChatType_CHANNEL:			// 自定頻道
		if (extra != NULL)
		{
			int channelID = g_pChatFrame->GetSendChannelID(atoi(extra));
			NetCli_Talk::Channel(EM_GroupObjectType_PlayerChannel, channelID, playerName, buf, senderInfo);
		}
		break;

	case eChatType_GM:				// GM 頻道
	{
		ParseText(buf);
	}
	break;

	case eChatType_BG:				// 戰場頻道
		// if ( g_pGameMain->IsBattleGroundZone() && NetCli_PartyChild::s_BattleMember.size() > 1 )
		if (g_pGameMain->IsBattleGroundZone())
			NetCli_Party::S_Zone_Talk(buf);
		else
			g_pChatFrame->SendMsgEvent(CHAT_MSG_SYSTEM, g_ObjectData->GetString("SYS_ERR_CHAT_NOBG"), playerName);
		break;
	}
}

// ----------------------------------------------------------------------------
void CChatEdit::SendChatMessage(const char* text, int channel)
{
	static char tmp[1024];
	if (text == NULL || text[0] == 0)
		return;

	Translator(text, tmp);

	if (text[0] == '/')
		ParseText(tmp + 1);
	else
	{
		switch (m_channel)
		{
		case eChatType_Whisper:
			//NetCli_Talk::TalkTo( m_tellTarget.c_str(), tmp );
			break;

		case eChatType_Range:
			//NetCli_Talk::Range( tmp );
			break;

		case eChatType_Zone:
			//			NetCli_Talk::Zone( tmp );
			break;

		case eChatType_GMCommand:
			NetCli_Talk::GMCommand(tmp);
			break;

		case eChatType_System:
			break;
		}
	}
}

// ----------------------------------------------------------------------------
void CChatEdit::ParseText(const char* text)
{
	CSplitArgString arg_text(text);
	string str = arg_text.GetString(0, 0);
	_strlwr((char*)str.c_str());

	map<string, CommandInfo>::iterator iter = m_command.find(str);
	if (iter == m_command.end())
	{
		// 命令錯誤
		// RECEIVE_CHAT_MESSAGE
		// System error 9
		char tmp[MAX_PATH];
		sprintf(tmp, "%s command failed", str.c_str());
		g_pChatFrame->AddMessage(9, tmp);
	}
	else
	{
#ifndef _DEBUG
		if (CNetGlobal::RoleData()->PlayerBaseData->ManageLV > (iter->second).powerLevel)
#endif
		{
			string second_text = arg_text.GetString(1);
			(iter->second).func(second_text.c_str());
		}
	}
}

// ----------------------------------------------------------------------------
void CChatEdit::Translator(const char* src, char* dest)
{
	while (*src)
	{
		if (*src > 0x80)
		{
			*dest++ = *src++;
			*dest++ = *src++;
		}
		else if (*src == '%')
		{
			if (strnicmp(src + 1, "t_id", 4) == 0)
			{
				CRoleSprite* player = g_pGameMain->GetPlayerTarget();
				src += 5;

				if (player)
				{
					char tmp[16];
					sprintf(tmp, "%d", player->GetIdentify());

					int len = (int)strlen(tmp);
					memcpy(dest, tmp, len);
					dest += len;
				}
			}
			else if (strnicmp(src + 1, "s_id", 4) == 0)
			{
				CRoleSprite* player = g_pGameMain->GetPlayer();
				src += 5;

				if (player)
				{
					char tmp[16];
					sprintf(tmp, "%d", player->GetIdentify());

					int len = (int)strlen(tmp);
					memcpy(dest, tmp, len);
					dest += len;
				}
			}
			else if (strnicmp(src + 1, "t", 1) == 0)
			{
				CRoleSprite* player = g_pGameMain->GetPlayerTarget();
				src += 2;

				if (player && player->GetName())
				{
					int len = (int)strlen(player->GetName());
					memcpy(dest, player->GetName(), len);
					dest += len;
				}
			}
			else if (strnicmp(src + 1, "s", 1) == 0)
			{
				CRoleSprite* player = g_pGameMain->GetPlayer();
				src += 2;

				if (player && player->GetName())
				{
					int len = (int)strlen(player->GetName());
					memcpy(dest, player->GetName(), len);
					dest += len;
				}
			}
			/*
			else if ( strnicmp(src + 1, "%", 1) == 0 )
			{
				src += 2;
				*dest++ = '%';
			}
			*/
			else
				*dest++ = *src++;
		}
		else
			*dest++ = *src++;
	}
	*dest = 0;
}

// ----------------------------------------------------------------------------
void CChatEdit::IgnoreColorCode(char* src)
{
	int i = 0;
	int size = (int)strlen(src);

	while (src[i] && i < size)
	{
		bool valid = true;

		if (src[i] == '|')
		{
			const char* next = NULL;
			switch (src[i + 1])
			{
				// 連結
			case 'H':
				if (next = strstr(&src[i + 2], "|h"))
				{
					// 前段資料
					const char* begin = next + 2;

					// 後段資料
					const char* end = strstr(begin, "|h");
					if (end != NULL) {
						valid = false;
						i += int(end - (src + i) + 2);
					}
				}
				break;

				// 顏色開始
			case 'c':
				if (strlen(&src[i + 2]) > 7) {
					valid = false;
					memmove(src + i, src + i + 10, size - i);
					size -= 10;			// 2控字元 + 8個色碼
				}
				break;

			case 'o':
			case 'O':
				if (strlen(&src[i + 2]) > 7) {
					valid = false;
					memmove(src + i, src + i + 10, size - i);
					size -= 10;			// 2控字元 + 8個色碼
				}
				break;

				// 顏色結束,回復預設值
			case 'r':
				valid = false;
				memmove(src + i, src + i + 2, size - i);
				size -= 2;
				break;
			}
		}

		// 有效的字元
		if (valid) {
			i++;
		}
	}
}

// ----------------------------------------------------------------------------
void CChatEdit::InsertUndesirable(const char* name)
{
	if (name == NULL)
		return;

	float frameTime = (float)g_pGameMain->GetFrameTime();

	g_pChatFrame->SendChatMessageEx(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_REPORT_SPAM"), name);

	map<string, float>::iterator iter = m_undesirable.find(name);
	if (iter != m_undesirable.end())
	{
		// 時間過短不重複發送
		if (iter->second - frameTime < 30.0f * 60.0f)
			return;

		// 重新計算時間
		iter->second = frameTime;
	}
	else
	{
		// 新增角色
		m_undesirable.insert(make_pair(name, frameTime));
	}

	// 送出給SERVER
	NetCli_Other::SL_MoneyVendorReport(name, "");
}

// ----------------------------------------------------------------------------
void CChatEdit::RemoveUndesirable(const char* name)
{
	if (name == NULL)
		return;

	map<string, float>::iterator iter = m_undesirable.find(name);
	if (iter != m_undesirable.end())
	{
		g_pChatFrame->SendChatMessageEx(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_REPORT_SPAM_LIFT"), name);

		m_undesirable.erase(iter);
	}
}

// ----------------------------------------------------------------------------
bool CChatEdit::IsUndesirable(const char* name)
{
	map<string, float>::iterator iter = m_undesirable.find(name);
	if (iter != m_undesirable.end())
		return true;
	else
		return false;
}

// ----------------------------------------------------------------------------
void CChatEdit::RL_MoneyVendorReport(int count)
{
	if (count >= 50)
	{
		m_banTime[eChatType_ZONE] = 3600.0f;
	}
	else
	{
		m_banTime[eChatType_ZONE] = 0.0f;
	}
}

// ----------------------------------------------------------------------------
int LuaFunc_SendChatMessage(lua_State* L)
{
	const char* text = luaL_checkstring(L, 1);
	const char* type = luaL_checkstring(L, 2);
	const char* language = (lua_isstring(L, 3)) ? lua_tostring(L, 3) : NULL;
	const char* extra = (lua_isstring(L, 4)) ? lua_tostring(L, 4) : NULL;

	string chatType = type;
	_strupr((char*)chatType.c_str());
	map<string, int>::iterator iter = g_chatTypeInfo.find(chatType);
	if (iter != g_chatTypeInfo.end())
	{
		g_pChatEdit->SendChatMessage((ChatTypeInfo)iter->second, text, language, extra);
	}
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_ParseText(lua_State* L)
{
	g_pChatEdit->ParseText(luaL_checkstring(L, 1));
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_InsertUndesirable(lua_State* L)
{
	g_pChatEdit->InsertUndesirable(luaL_checkstring(L, 1));
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_RemoveUndesirable(lua_State* L)
{
	g_pChatEdit->RemoveUndesirable(luaL_checkstring(L, 1));
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_IsUndesirable(lua_State* L)
{
	lua_pushboolean(L, g_pChatEdit->IsUndesirable(luaL_checkstring(L, 1)));
	return 1;
}

// ----------------------------------------------------------------------------
void TalkToWhisper(const char* text)
{
	/*
	CSplitArgString arg_text(text);
	string player, content;
	player = arg_text.GetString(0, 0);
	content = arg_text.GetString(1, 1);
	NetCli_Talk::TalkTo(player.c_str(), content.c_str());
	*/
}

// ----------------------------------------------------------------------------
void TalkToZone(const char* text)
{
	//	NetCli_Talk::Zone(text);
}

// ----------------------------------------------------------------------------
void GMCommand(const char* text)
{
	NetCli_Talk::GMCommand(text);
}
void TalkToGM(const char* text)
{
	char Buf[1024];
	sprintf(Buf, "[%s] : %s", g_ObjectData->GetString("SYS_TALK_TO_GM"), text);
	g_pChatFrame->AddMessage(5, Buf);

	NetCli_Talk::GMMsg(text);
}
// ----------------------------------------------------------------------------
void ScriptCommand(const char* text)
{
	lua_State* L = g_pChatEdit->GetLuaStateWorld();
	if (L)
	{
		if (luaL_dostring(L, text) != 0)
		{
			const char* err = lua_tostring(L, -1);
			lua_pop(L, 1);
			UiOuputError(err);
		}
	}
}


// ----------------------------------------------------------------------------
void OpenMaintainFrame(const char* text)
{
	g_pMaintain->Open();
	/*
	lua_State* L = g_pChatEdit->GetLuaStateWorld();
	if (L)
	{
		luaL_dostring(L, "MaintainInterface:Show()");
	}
	*/
}

// ----------------------------------------------------------------------------
void DoEmotion(const char* text)
{
	if (text)
	{
		g_pGameMain->DoEmotion(CRuFusion_ActorState(atoi(text)));
	}
}

// ----------------------------------------------------------------------------
void SetDefaultIdleMotion(const char* text)
{
	CRoleSprite* sprite = g_pGameMain->GetPlayer();
	if (sprite)
	{
		sprite->SetDefaultIdleAction(CRuFusion_Mime_Action(atoi(text)));
	}
}

// ----------------------------------------------------------------------------
void SetObjectID(const char* text)
{
	if (text)
	{
		CRoleSprite* sprite = g_pGameMain->GetPlayer();
		if (sprite)
		{
			sprite->SetObjectId(atoi(text), true);
			sprite->SetGravityMode(true);
		}
	}
}

// ----------------------------------------------------------------------------
void SetWeaponObjectID(const char* text)
{
	CRoleSprite* sprite = g_pGameMain->GetPlayer();
	if (sprite)
	{
		CSplitArgString arg_text(text);
		RoleHandsType handsType = eRoleHands_None;
		string position = arg_text.GetString(0, 0);
		string id = arg_text.GetString(1, 1);
		string level = arg_text.GetString(2, 2);

		if (stricmp(position.c_str(), "right") == 0) {
			handsType = eRoleHands_MainHand;
		}
		else if (stricmp(position.c_str(), "left") == 0) {
			handsType = eRoleHands_OffHand;
		}
		else if (stricmp(position.c_str(), "range") == 0) {
			handsType = eRoleHands_Ranged;
		}
		else if (stricmp(position.c_str(), "crafting") == 0) {
			handsType = eRoleHands_Crafting;
		}

		sprite->SetHandsObjectId(handsType, atoi(id.c_str()), atoi(level.c_str()));
		sprite->SetWeaponPosition(sprite->GetUnholsterState());
	}
}

// ----------------------------------------------------------------------------
void PlayMotion(const char* text)
{
	if (text)
	{
		CRoleSprite* sprite = g_pGameMain->GetPlayer();
		if (sprite && sprite->GetRuEntity())
		{
			RuACT_PlayMotionEx(sprite->GetRuEntity(), -1, text);
		}
	}
}

// ----------------------------------------------------------------------------
void SetPaperdollPart(const char* text)
{
	if (text)
	{
		CSplitArgString arg_text(text);
		string part = arg_text.GetString(0, 0);
		string name = arg_text.GetString(1, 1);

		CRoleSprite* sprite = g_pGameMain->GetPlayer();
		if (sprite)
		{
			sprite->SetPaperdollPart(part.c_str(), name.c_str());
		}
	}
}

// ----------------------------------------------------------------------------
void SetTarrainDetailLevel(const char* text)
{
	int level = 0;
	if (text)
		level = atoi(text);
	g_pGameMain->SetTerrainDetailLevel(level);
}

// ----------------------------------------------------------------------------
void TradeRequest(const char* text)
{
	if (text)
	{
		g_pTradeFrame->SetProcess(eTradeState_Request_Made, atoi(text));
	}
}

// ----------------------------------------------------------------------------
void TradeAgree(const char* text)
{
	g_pTradeFrame->SetProcess(eTradeState_Agree, g_pTradeFrame->GetTradeTarget());
}

// ----------------------------------------------------------------------------
void TradeDisagree(const char* text)
{
	g_pTradeFrame->SetProcess(eTradeState_Disagree, 0);
}

// ----------------------------------------------------------------------------
void SetScale(const char* text)
{
	float scale = 1.0f;
	if (text)
		scale = (float)atof(text);

	CRoleSprite* sprite = g_pGameMain->GetPlayerTarget();
	if (sprite == NULL)
		sprite = g_pGameMain->GetPlayer();

	if (sprite)
		sprite->SetScale(scale);
}

// ----------------------------------------------------------------------------
void SetPos(const char* text)
{
	CRuVector3 pos(0.0f, 0.0f, 0.0f);
	CSplitArgString arg_text(text);
	pos.m_x = (float)atof(arg_text.GetString(0, 0).c_str());
	pos.m_y = (float)atof(arg_text.GetString(1, 1).c_str());
	pos.m_z = (float)atof(arg_text.GetString(2, 2).c_str());

	CRoleSprite* sprite = g_pGameMain->GetPlayer();
	if (sprite)
	{
		CRoleSprite* spriteControlProxy = sprite->GetProxyControlSprite();

		spriteControlProxy->SetPos(pos);
	}
}

// ----------------------------------------------------------------------------
void SetTargetPos(const char* text)
{
	CRuVector3 pos(0.0f, 0.0f, 0.0f);
	CSplitArgString arg_text(text);
	pos.m_x = (float)atof(arg_text.GetString(0, 0).c_str());
	pos.m_y = (float)atof(arg_text.GetString(1, 1).c_str());
	pos.m_z = (float)atof(arg_text.GetString(2, 2).c_str());

	CRoleSprite* sprite = g_pGameMain->GetPlayer();
	if (sprite)
		sprite->SetTargetPos(pos, false);
}

// ----------------------------------------------------------------------------
void SetGlow(const char* text)
{
	bool open = false;
	if (text && strcmp(text, "on") == 0)
		open = true;
	g_pGameMain->SetGlow(open);
}

// ----------------------------------------------------------------------------
void SetPlayerSpeed(const char* text)
{
	float speed = 33.0f;
	if (text)
		speed = (float)atof(text);

	CRoleSprite* player = g_pGameMain->GetPlayer();
	if (player)
		player->SetMoveSpeed(speed);
}

// ----------------------------------------------------------------------------
void OpenBank(const char* text)
{
	lua_State* L = g_pChatEdit->GetLuaStateWorld();
	if (L)
	{
		luaL_dostring(L, "BankFrame:Show()");
	}
}

// ----------------------------------------------------------------------------
void ShowFps(const char* text)
{
	bool open = false;
	if (text && strcmp(text, "on") == 0)
		open = true;
	g_pGameMain->SetShowFps(open);
}

// ----------------------------------------------------------------------------
void SpriteContainer(const char* text)
{
	int spriteCount = 0;
	set<IBaseSprite*>& container = IBaseSprite::GetContainer();
	for (set<IBaseSprite*>::iterator iter = container.begin(); iter != container.end(); iter++)
	{
		++spriteCount;

		char tmp[MAX_PATH];
		memset(tmp, 0, sizeof(tmp));
		switch ((*iter)->GetClassType())
		{
		case eSpriteClass_Base:
		{
			sprintf(tmp, "[Base] id : %d ", (*iter)->GetIdentify());
		}
		break;

		case eSpriteClass_Role:
		{
			CRoleSprite* role = (CRoleSprite*)(*iter);
			switch ((*iter)->GetObjectType())
			{
			case eSpriteObject_Unknown:
				sprintf(tmp, "[Role][Unknown] id : %d Name : %s", role->GetIdentify(), role->GetName());
				break;
			case eSpriteObject_Player:
				sprintf(tmp, "[Role][Player] id : %d Name : %s", role->GetIdentify(), role->GetName());
				break;
			case eSpriteObject_NPC:
				sprintf(tmp, "[Role][NPC] id : %d Name : %s", role->GetIdentify(), role->GetName());
				break;
			case eSpriteObject_QuestNPC:
				sprintf(tmp, "[Role][QuestNPC] id : %d Name : %s", role->GetIdentify(), role->GetName());
				break;
			case eSpriteObject_Effect:
				sprintf(tmp, "[Role][Effect] id : %d Name : %s", role->GetIdentify(), role->GetName());
				break;
			default:
				sprintf(tmp, "[Role][Unknown] id : %d Name : %s", role->GetIdentify(), role->GetName());
				break;
			}
		}
		break;

		case eSpriteClass_Effect:
			sprintf(tmp, "[Effect] id : %d ", (*iter)->GetIdentify());
			break;

		case eSpriteClass_Effect_Line:
			sprintf(tmp, "[Effect_Line] id : %d ", (*iter)->GetIdentify());
			break;
		}

		g_pChatFrame->AddMessage(0, tmp);
	}

	char outputStr[256];
	sprintf(outputStr, "Sprite Count : %d ", spriteCount);
	g_pChatFrame->AddMessage(0, outputStr);
}

// ----------------------------------------------------------------------------
void SpritePos(const char* text)
{
	CRoleSprite* sprite = g_pGameMain->FindSpriteByID(atoi(text));
	if (sprite)
	{
		char tmp[MAX_PATH];
		memset(tmp, 0, sizeof(tmp));

		sprintf(tmp, "sprite pos %.2f, %.2f %2.f", sprite->GetPos().m_x, sprite->GetPos().m_y, sprite->GetPos().m_z);
		g_pChatFrame->AddMessage(0, tmp);
	}
}

// ----------------------------------------------------------------------------
void SetStableFrame(const char* text)
{
	bool isStable = false;
	if (text && stricmp(text, "on") == 0)
		isStable = true;

	g_pGameMain->SetStableFrame(isStable);
}

// ----------------------------------------------------------------------------
void SetPlayerTitle(const char* text)
{
	bool isVisible = false;
	if (text && stricmp(text, "on") == 0)
		isVisible = true;
	g_pGameMain->SetPlayerTitleVisible(isVisible);
}

// ----------------------------------------------------------------------------
void SetNpcTitle(const char* text)
{
	bool isVisible = false;
	if (text && stricmp(text, "on") == 0)
		isVisible = true;
	g_pGameMain->SetNPCTitleVisible(isVisible);
}

// ----------------------------------------------------------------------------
void OpenMacroFrame(const char* text)
{
	lua_State* L = g_pChatEdit->GetLuaStateWorld();
	if (L)
	{
		luaL_dostring(L, "MacroFrame:Show()");
	}
}

// ----------------------------------------------------------------------------
void OpenLootFrame(const char* text)
{
	lua_State* L = g_pChatEdit->GetLuaStateWorld();
	if (L)
	{
		luaL_dostring(L, "LootFrame:Show()");
	}
}

// ----------------------------------------------------------------------------
void SetWidgetGrid(const char* text)
{
	if (text && stricmp(text, "off") == 0)
		g_pGameMain->ReleaseWidgetGrid();
	else
	{
		CSplitArgString arg(text);
		CRuVector3 pos(0.0f, 0.0f, 0.0f);
		float size = 100.0f;
		float spacing = 5.0f;
		bool bLocked = false;

		for (int i = 0; i < arg.GetArgSize(); ++i)
		{
			switch (i)
			{
			case 0:
				bLocked = (atoi(arg.GetString(0, 0).c_str()) == 0) ? false : true;
				break;

			case 1:
				pos.m_x = (float)atof(arg.GetString(1, 1).c_str());
				break;

			case 2:
				pos.m_y = (float)atof(arg.GetString(2, 2).c_str());
				break;

			case 3:
				pos.m_z = (float)atof(arg.GetString(3, 3).c_str());
				break;

			case 4:
				size = (float)atof(arg.GetString(4, 4).c_str());
				break;

			case 5:
				spacing = (float)atof(arg.GetString(5, 5).c_str());
				break;
			}
		}
		g_pGameMain->CreateWidgetGrid(pos, bLocked, size, spacing);
	}
}

// ----------------------------------------------------------------------------
void SetWidgetLine(const char* text)
{
	CRoleSprite* target = g_pGameMain->GetPlayerTarget();
	if (target)
	{
		if (stricmp(text, "on") == 0)
			target->CreateWidgetLine();
		else if (stricmp(text, "off") == 0)
			target->ReleaseWidgetLine();
	}
}

// ----------------------------------------------------------------------------
// Channel 測試
void CreateChannel(const char* text)
{
	CSplitArgString arg_text(text);
	string ChannelName = arg_text.GetString(0, 0);
	string Password = arg_text.GetString(1, 1);

	NetCli_Channel::S_CreateChannel((char*)ChannelName.c_str(), (char*)Password.c_str());
}
void JoinChannel(const char* text)
{
	CSplitArgString arg_text(text);
	string ChannelName = arg_text.GetString(0, 0);
	string Password = arg_text.GetString(1, 1);

	NetCli_Channel::S_JoinChannel((char*)ChannelName.c_str(), (char*)Password.c_str(), false);
}
void LeaveChannel(const char* text)
{
	CSplitArgString arg_text(text);
	int ChannelType = atol((char*)(arg_text.GetString(0, 0).c_str()));
	int ChannelID = atol((char*)(arg_text.GetString(1, 1).c_str()));


	NetCli_Channel::S_LeaveChannel((GroupObjectTypeEnum)ChannelType, ChannelID);
}
void TellChannel(const char* text)
{
	/*
	EM_GroupObjectType_SysChannel    = 0   ,
	EM_GroupObjectType_PlayerChannel    ,
	EM_GroupObjectType_Party            ,
	EM_GroupObjectType_Guild            ,
	*/
	CSplitArgString arg_text(text);
	int ChannelType = atol((char*)(arg_text.GetString(0, 0).c_str()));
	int ChannelID = atol((char*)(arg_text.GetString(1, 1).c_str()));
	string Context = arg_text.GetString(2, 2);

	//NetCli_Talk::Channel( (GroupObjectTypeEnum)ChannelType , ChannelID , "Name" , (char*)Context.c_str() );

}
void PlayerListChannel(const char* text)
{
	CSplitArgString arg_text(text);
	int ChannelType = atol((char*)(arg_text.GetString(0, 0).c_str()));
	int ChannelID = atol((char*)(arg_text.GetString(1, 1).c_str()));

	NetCli_Channel::S_ChannelPlayerList((GroupObjectTypeEnum)ChannelType, ChannelID);
}
void JoinDisabledChannel(const char* text)
{
	CSplitArgString arg_text(text);
	int ChannelType = atol((char*)(arg_text.GetString(0, 0).c_str()));
	int ChannelID = atol((char*)(arg_text.GetString(1, 1).c_str()));
	int	JoinEnabled = atol((char*)(arg_text.GetString(2, 2).c_str()));

	NetCli_Channel::S_JoinPlayerDisabled((GroupObjectTypeEnum)ChannelType, ChannelID, JoinEnabled != 0);
}
void KickPlayerChannel(const char* text)
{
	CSplitArgString arg_text(text);
	int ChannelType = atol((char*)(arg_text.GetString(0, 0).c_str()));
	int ChannelID = atol((char*)(arg_text.GetString(1, 1).c_str()));
	string PlayerName = arg_text.GetString(2, 2);

	NetCli_Channel::S_KickPlayer((GroupObjectTypeEnum)ChannelType, ChannelID, (char*)PlayerName.c_str());
}
void SetManagerChannel(const char* text)
{
	CSplitArgString arg_text(text);
	int ChannelType = atol((char*)(arg_text.GetString(0, 0).c_str()));
	int ChannelID = atol((char*)(arg_text.GetString(1, 1).c_str()));
	string PlayerName = arg_text.GetString(2, 2);

	NetCli_Channel::S_SetManager((GroupObjectTypeEnum)ChannelType, ChannelID, (char*)PlayerName.c_str());
}
void ChannelZoneID(const char* text)
{
	CSplitArgString arg_text(text);
	int ZoneID = atol((char*)(arg_text.GetString(0, 0).c_str()));

	NetCli_Channel::S_ChannelZoneID(ZoneID);
}
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// Party 測試
void PartyInvite(const char* text)
{
	CSplitArgString arg_text(text);
	string Name = arg_text.GetString(0, 0);
	NetCli_PartyChild::InviteToParty(Name.c_str());
}
void PartyJoin(const char* text)
{
	NetCli_PartyChild::InviteAnswerState(true);
}
void PartyKickMember(const char* text)
{
	CSplitArgString arg_text(text);
	string MemberName = arg_text.GetString(0, 0);
	NetCli_PartyChild::KickMember((char*)MemberName.c_str());
}
void PartyRule(const char* text)
{
	CSplitArgString arg_text(text);
	string Type = arg_text.GetString(0, 0);
	string TypeValue = arg_text.GetString(1, 1);
	PartyBaseInfoStruct Info = NetCli_PartyChild::GetPartyInfo();

	if (Type.compare("Leader") == 0)
	{
		//改隊長
		Info.LeaderName = TypeValue.c_str();
	}
	else if (Type.compare("Attack") == 0)
	{
		Info.AttackName = TypeValue.c_str();

	}
	else if (Type.compare("Share") == 0)
	{
		Info.ShareName = TypeValue.c_str();
		Info.ItemShareType = EM_PartyItemShare_Assign;
	}
	else if (Type.compare("ShareType") == 0)
	{
		if (TypeValue.compare("Roll") == 0)
		{
			Info.ItemShareType = EM_PartyItemShare_Roll;
		}
		/*else if( TypeValue.compare( "InOrder" ) == 0 )
		{
		Info.ItemShareType = EM_PartyItemShare_InOrder;
		}*/
		else
		{
			g_pChatFrame->AddMessage(9, "語法錯誤 ( Type : Leader , Attack , Share , ShareType ) (ShareType: Roll , InOrder )");
			return;
		}
	}
	else
	{
		g_pChatFrame->AddMessage(9, "語法錯誤 ( Type : Leader , Attack , Share , ShareType ) (ShareType: Roll , InOrder )");

		return;
	}

	NetCli_PartyChild::S_PartyRule(Info);

}

void PartyTell(const char* text)
{
	CSplitArgString arg_text(text);
	int ChannelID = atol((char*)(arg_text.GetString(0, 0).c_str()));
	string Context = arg_text.GetString(1, 1);

	NetCli_Channel::S_DataTransmitToCli_Group(EM_GroupObjectType_Party, ChannelID, (char*)Context.c_str(), (int)Context.size() + 1);

}
void PartyLoot(const char* text)
{
	CSplitArgString arg_text(text);
	int ItemVersion = atol((char*)(arg_text.GetString(0, 0).c_str()));
	NetCli_Party::S_LootItem(ItemVersion);
}
void PartyPass(const char* text)
{
	CSplitArgString arg_text(text);
	int ItemVersion = atol((char*)(arg_text.GetString(0, 0).c_str()));
	NetCli_Party::S_PassItem(ItemVersion);

}
void PartyAssist(const char* text)
{
	CSplitArgString arg_text(text);
	int ItemVersion = atol((char*)(arg_text.GetString(0, 0).c_str()));
	int DBID = atol((char*)(arg_text.GetString(1, 1).c_str()));
	NetCli_Party::S_AssistItem(ItemVersion, DBID);

}

void FriendInvite(const char* text)
{
	CSplitArgString arg_text(text);
	string Name = arg_text.GetString(0, 0);

	//x_error
	//NetCli_FriendList::SChat_BefriendRequest( (char*)Name.c_str() );
}
void FriendJoin(const char* text)
{
	CSplitArgString arg_text(text);
	string Name = arg_text.GetString(0, 0);

	//x_error
	//NetCli_FriendList::SChat_BefriendResult( (char*)Name.c_str() , true );
}
void FriendDel(const char* text)
{
	CSplitArgString arg_text(text);
	string Name = arg_text.GetString(0, 0);

	//x_error
	//NetCli_FriendList::SL_DelFriend( (char*)Name.c_str() );
}

void FriendSetInfo(const char* text)
{
	CSplitArgString arg_text(text);
	string Note = arg_text.GetString(0, 0);

	//x_error
	//NetCli_FriendList::SL_ModifyMyInfo( (char*)Note.c_str() );
}
void FriendSetFriend(const char* text)
{
	CSplitArgString arg_text(text);
	string Name = arg_text.GetString(0, 0);
	string Group = arg_text.GetString(1, 1);
	string Note = arg_text.GetString(2, 2);

	//x_error
	//NetCli_FriendList::SL_SetFriendInfo		( (char*)Name.c_str() , (char*)Group.c_str() , (char*)Note.c_str() );
}
// ----------------------------------------------------------------------------
// 物件重載 
void ReloadObj(const char* text)
{
	g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", "Reload Object...");
	ObjectDataClass::Release();
	ObjectDataClass::Init(g_pRuneDev->GetFullFilePath(""), g_pGameMain->GetDataLanguage(), g_pGameMain->GetStringLanguage(), false);

	//因為情緒指令 直接紀錄系統文字指標 所以必須重載
	g_pEmoteFrame->InitEmoteDate();
	g_pInterface->SendWorldEvent(UiEvent_VariablesLoaded);
}
// ----------------------------------------------------------------------------
void ShowObjPath(const char* szText)
{
	CSplitArgString arg_text(szText);

	int iObjID = atol((char*)(arg_text.GetString(0, 0).c_str()));

	// 發出封包要求顯示該物件的巡邏路徑資料
	Net_GSrvConnectChild::GetNPCMoveInfo(iObjID);

}

// ----------------------------------------------------------------------------
void Open_GM_ObjEdit(const char* text)
{
	lua_State* L = g_pChatEdit->GetLuaStateWorld();
	if (L)
	{
		luaL_dostring(L, "GM_ObjEdit_Frame:Show()");
	}
}

// ----------------------------------------------------------------------------
void Open_GM_Config(const char* text)
{
	lua_State* L = g_pChatEdit->GetLuaStateWorld();
	if (L)
	{
		luaL_dostring(L, "GM_Config_Frame:Show()");
	}
}

// ----------------------------------------------------------------------------
void Open_SYS_AudioSettings(const char* text)
{
	lua_State* L = g_pChatEdit->GetLuaStateWorld();
	if (L)
	{
		luaL_dostring(L, "SYS_AudioSettings_Frame:Show()");
	}
}

// ----------------------------------------------------------------------------
void Open_SYS_DisplaySettings(const char* text)
{
	lua_State* L = g_pChatEdit->GetLuaStateWorld();
	if (L)
	{
		luaL_dostring(L, "SYS_DisplaySettings_Frame:Show()");
	}
}

// ----------------------------------------------------------------------------
void Flush_SoundCache(const char* text)
{

}

// ----------------------------------------------------------------------------
void FDBJournal_Begin(const char* text)
{
	g_pGameMain->FDBJournal_Begin(text);
}

// ----------------------------------------------------------------------------
void FDBJournal_End(const char* text)
{
	g_pGameMain->FDBJournal_End();
}

// ----------------------------------------------------------------------------
void WDBJournal_Begin(const char* text)
{
	g_pGameMain->WDBJournal_Begin(text);
}

// ----------------------------------------------------------------------------
void WDBJournal_End(const char* text)
{
	g_pGameMain->WDBJournal_End();
}

// ----------------------------------------------------------------------------
void PaperdollCache_ActivateWrites(const char* text)
{
	if (stricmp(text, "on") == 0)
	{
		g_pGameMain->PaperdollCache_ActivateWrites(TRUE);
	}
	else if (stricmp(text, "off") == 0)
	{
		g_pGameMain->PaperdollCache_ActivateWrites(FALSE);
	}
}

// ----------------------------------------------------------------------------
void SetUIScale(const char* text)
{
	if (text)
	{
		g_pInterface->SetScale((float)atof(text));
	}
}

// ----------------------------------------------------------------------------
void ModifyObj(const char* text)
{
	CRoleSprite* Obj = g_pGameMain->GetPlayerTarget();


	if (Obj == NULL)
	{
		g_pChatFrame->AddMessage(9, "目標不存在!!");
		return;
	}

	char tmp[256];
	sprintf(tmp, "/? modify %d", Obj->GetIdentify());
	g_pChatEdit->SendChatMessage(tmp, 0);

}
// ----------------------------------------------------------------------------
void DelObj(const char* text)
{
	CRoleSprite* Obj = g_pGameMain->GetPlayerTarget();

	if (Obj == NULL)
	{
		g_pChatFrame->AddMessage(9, "目標不存在!!");
		return;
	}

	char tmp[256];
	sprintf(tmp, "/? delobj %d", Obj->GetIdentify());
	g_pChatEdit->SendChatMessage(tmp, 0);

}
// ----------------------------------------------------------------------------
void ACSell(const char* text)
{
	CSplitArgString arg_text(text);

	int		Pos = atol((char*)(arg_text.GetString(0, 0).c_str()));
	int		Price = atol((char*)(arg_text.GetString(1, 1).c_str()));
	int		BuyOutPrice = atol((char*)(arg_text.GetString(2, 2).c_str()));

	RoleDataEx* Role = CNetGlobal::RoleData();

	ItemFieldStruct* Item = Role->GetBodyItem(Pos);

	if (Item == NULL)
		return;

	NetCli_ACChild::SL_SellItem(Pos, *Item, Price, BuyOutPrice, EM_ACPriceType_GameMoney, 1000);

}
// ----------------------------------------------------------------------------
void	ACSellCancel(const char* text)
{
	CSplitArgString arg_text(text);

	int		ACItemDBID = atol((char*)(arg_text.GetString(0, 0).c_str()));
	NetCli_ACChild::SL_CancelSellItem(ACItemDBID);
}
// ----------------------------------------------------------------------------
void	ACSearch(const char* text)
{
	CSplitArgString arg_text(text);
	ACSearchConditionStruct Data;
	Data.LevelHi = 100;
	Data.LevelLow = 0;
	Data.Type[0] = -1;
	Data.Type[1] = -1;
	Data.Type[2] = -1;
	Data.SearchName = "狼";
	NetCli_ACChild::SL_SearchItem(Data);
}
// ----------------------------------------------------------------------------
void	ACSearchNext(const char* text)
{


	NetCli_ACChild::SL_GetNextSearchItem();
}
// ----------------------------------------------------------------------------
void	ACClose(const char* text)
{
	NetCli_ACChild::SL_CloseAC();
}
void	ACBuy(const char* text)
{
	CSplitArgString arg_text(text);
	int		ACItemDBID = atol((char*)(arg_text.GetString(0, 0).c_str()));
	int		Prices = atol((char*)(arg_text.GetString(1, 1).c_str()));

	NetCli_ACChild::SL_BuyItem(ACItemDBID, Prices, EM_ACPriceType_GameMoney, "");
}

void	ACItemHistory(const char* text)
{
	CSplitArgString arg_text(text);
	int		OrgObjID = atol((char*)(arg_text.GetString(0, 0).c_str()));
	int		Inherent = atol((char*)(arg_text.GetString(1, 1).c_str()));

	NetCli_ACChild::SL_ItemHistoryRequest(OrgObjID);
}

//施法測試
void Spell(const char* text)
{
	CSplitArgString arg_text(text);
	int		MagicID = atol((char*)(arg_text.GetString(0, 0).c_str()));
	//	int		TargetID;

	CRoleSprite* player = g_pGameMain->GetPlayer();
	CRoleSprite* target = g_pGameMain->GetPlayerTarget();

	if (target == NULL)
		target = player;

	RoleDataEx* Role = CNetGlobal::RoleData();
	for (int i = 0; i < _MAX_SPSkill_COUNT_ + _MAX_NormalSkill_COUNT_ + _MAX_NormalSkill_COUNT_; i++)
	{
		if (Role->PlayerTempData->Skill.AllSkill[i] != MagicID)
			continue;

		if (target && player)
		{
			NetCli_Magic::BeginSpell(player->GetIdentify(), target->GetIdentify(), MagicID, i);
		}
		return;
	}

	g_pChatFrame->AddMessage(9, "不會此法術");

}
//設定配制法術等級
void SetTp(const char* text)
{
	CSplitArgString arg_text(text);
	int		MagicID = atol((char*)(arg_text.GetString(0, 0).c_str()));
	//	int		TargetID;

	CRoleSprite* player = g_pGameMain->GetPlayer();

	RoleDataEx* Role = CNetGlobal::RoleData();
	for (int i = 0; i < _MAX_SPSkill_COUNT_ + _MAX_NormalSkill_COUNT_; i++)
	{
		if (Role->PlayerTempData->Skill.AllSkill[i] != MagicID)
			continue;

		NetCli_Magic::SetMagicPoint(MagicID, i);

		return;
	}

	g_pChatFrame->AddMessage(9, "不會此法術");

}

//合成測試
void Refine(const char* text)
{
	CSplitArgString arg_text(text);
	int		RecipeID = atol((char*)(arg_text.GetString(0, 0).c_str()));

	NetCli_Recipe::SL_RefineItem(RecipeID);
}

//公會測試
// ----------------------------------------------------------------------------
void GuildRegister(const char* text)
{
	g_pGuildFrame->RegisterGuild();
}

// ----------------------------------------------------------------------------
void GuildPetitionSignature(const char* text)
{
	NetCli_Guild::SL_NewGuildInvite((char*)text);
}

// ----------------------------------------------------------------------------
void GuildPetitionLeave(const char* text)
{
	g_pGuildFrame->PetitionKick(text);
}

// ----------------------------------------------------------------------------
void GuildPetitionQuit(const char* text)
{
	g_pGuildFrame->PetitionQuit();
}

void CloseCreateGuild(const char* text)
{
	NetCli_Guild::SL_CloseCreateGuild();
}

void NewGuildCreate(const char* text)
{
	CSplitArgString arg_text(text);

	string GuildName = arg_text.GetString(0, 0);
	NetCli_Guild::SL_CreateGuild((char*)GuildName.c_str());

}
void NewGuildInvite(const char* text)
{
	CSplitArgString arg_text(text);
	string PlayerName = arg_text.GetString(0, 0);
	NetCli_Guild::SL_NewGuildInvite((char*)PlayerName.c_str());
}
void NewGuildJoin(const char* text)
{
	CSplitArgString arg_text(text);
	int		GuildID = atol((char*)(arg_text.GetString(0, 0).c_str()));
	int		LeaderDBID = atol((char*)(arg_text.GetString(1, 1).c_str()));


	NetCli_Guild::SL_NewGuildInviteResult(GuildID, LeaderDBID, true);
}

void NewGuildLeave(const char* text)
{
	CSplitArgString arg_text(text);
	string PlayerName = arg_text.GetString(0, 0);
	NetCli_Guild::SL_LeaveNewGuild((char*)PlayerName.c_str());
}
void NewGuildDel(const char* text)
{
	NetCli_Guild::SL_DelNewGuild();
}

//---------------------------------------------------------------------------------------------
void GuildInvite(const char* text)
{



	//戰場中
	if (g_pGameMain->IsBattleGroundZone() || (g_pGuildHousesFrame->isGuildWaring()))
	{

		const char* pMsg;

		pMsg = g_ObjectData->GetString("GUILD_MSG_CANT_INVITE_GUILDWAR");
		g_pGameMain->SendWarningMsg(pMsg);


		return;

	}


	CSplitArgString arg_text(text);
	string PlayerName = arg_text.GetString(0, 0);
	NetCli_Guild::SL_GuildInvite((char*)PlayerName.c_str());
}
void GuildJoin(const char* text)
{
	CSplitArgString arg_text(text);
	int		GuildID = atol((char*)(arg_text.GetString(0, 0).c_str()));
	int		LeaderDBID = atol((char*)(arg_text.GetString(1, 1).c_str()));

	NetCli_Guild::SL_GuildInviteResult(GuildID, LeaderDBID, true);
}

void GuildLeave(const char* text)
{
	CSplitArgString arg_text(text);
	string PlayerName = arg_text.GetString(0, 0);
	NetCli_Guild::SL_LeaveGuild((char*)PlayerName.c_str());
}
void GuildDel(const char* text)
{
	NetCli_Guild::SL_DelGuild();
}
//-------------------------------------------------------------------------------------------
void GuildModify(const char* text)
{
	//測試用  debug 硬塞
	GuildBaseStruct Guild;

	NetCli_Guild::SL_ModifyGuildInfoRequest(Guild);
}
void GuildModifyMember(const char* text)
{
	//測試用  debug 硬塞
	GuildMemberStruct Member;

	NetCli_Guild::SL_ModifyGuildMemberInfoRequest(Member);

}
void GuildInfo(const char* text)
{
	CSplitArgString arg_text(text);
	int		GuildID = atol((char*)(arg_text.GetString(0, 0).c_str()));
	NetCli_Guild::SL_GuildInfoRequet(GuildID);
}
void GuildMemberList(const char* text)
{
	CSplitArgString arg_text(text);
	int		GuildID = atol((char*)(arg_text.GetString(0, 0).c_str()));
	NetCli_Guild::SL_GuildMemberListRequet(GuildID);
}
void GuildMemberInfo(const char* text)
{
	CSplitArgString arg_text(text);
	int		MemberDBID = atol((char*)(arg_text.GetString(0, 0).c_str()));
	NetCli_Guild::SL_GuildMemberInfoRequet(MemberDBID);
}

//公會留言板測試
void GuildBoardPost(const char* text)
{
	static int ID = 0;
	char Buf[256];
	sprintf(Buf, "Title%d", ID);
	NetCli_Guild::SL_BoardPostRequest(Buf, "Message");

	ID++;
}
void GuildBoardList(const char* text)
{
	CSplitArgString arg_text(text);
	int		PageID = atol((char*)(arg_text.GetString(0, 0).c_str()));
	NetCli_Guild::SL_BoardListRequest(PageID);
}
void GuildBoardMessage(const char* text)
{
	CSplitArgString arg_text(text);
	int		MessageGUID = atol((char*)(arg_text.GetString(0, 0).c_str()));
	NetCli_Guild::SL_BoardMessageRequest(MessageGUID);
}
void GuildBoardMsgModify(const char* text)
{
	CSplitArgString arg_text(text);
	string	Msg = arg_text.GetString(1, 1);
	int		MessageGUID = atol((char*)(arg_text.GetString(0, 0).c_str()));

	NetCli_Guild::SL_BoardModifyMessageRequest(MessageGUID, (char*)Msg.c_str());
}
void GuildBoardTypeModify(const char* text)
{
	CSplitArgString arg_text(text);
	GuildBoardModeStruct	Mode;
	Mode._Type = atol((char*)(arg_text.GetString(1, 1).c_str()));

	int		MessageGUID = atol((char*)(arg_text.GetString(0, 0).c_str()));

	NetCli_Guild::SL_BoardModifyModeRequest(MessageGUID, Mode);

}


void ObjVersion(const char* text)
{

	char	Buf[256];
	/*
		map< string , ObjFileHead >& FileHeadList = *g_ObjectData->ObjectDB()->FileHeadList();
		map< string , ObjFileHead >::iterator Iter;

		g_pChatFrame->AddMessage( 5, "----------Client端物件資訊-------------" );
		for( Iter = FileHeadList.begin() ; Iter != FileHeadList.end() ; Iter++ )
		{
			sprintf( Buf , "%s" , Iter->second.FileInfoStr );
			g_pChatFrame->AddMessage( 5, Buf );
		}
	*/
	vector< ObjFileInfo* > FileInfoList = *g_ObjectData->ObjectDB()->FileInfoList();
	vector< ObjFileInfo* >::iterator Iter;
	g_pChatFrame->AddMessage(5, "----------Client端物件資訊-------------");
	for (Iter = FileInfoList.begin(); Iter != FileInfoList.end(); Iter++)
	{
		sprintf(Buf, "%s", (*Iter)->FileHead.FileInfoStr);
		g_pChatFrame->AddMessage(5, Buf);
	}

	return;
}

void Casting_Start(const char* text)
{
	CSplitArgString arg_text(text);
	//g_pCastingBarFrame->CastingStart(arg_text.GetString(0, 0).c_str(), (float)atof( (char*)(arg_text.GetString(1, 1).c_str()) ));
}

void Casting_End(const char* text)
{
	//g_pCastingBarFrame->CastingStop();
}

void Casting_Failed(const char* text)
{
	//g_pCastingBarFrame->CastingFailed();
}

void Casting_Delay(const char* text)
{
	//g_pCastingBarFrame->CastingDelayed(atof(text));
}

void SendPing(const char* text)
{
	Net_Move::S_Ping(timeGetTime());
}

//測試取地上物品
void GetGroundItem(const char* text)
{
	CSplitArgString arg(text);
	int ItemID = atol((char*)(arg.GetString(0, 0).c_str()));
	NetCli_Item::_GetGroundItem(ItemID, -1);
}

//測試 使用物品對物品
void UseItemToItem(const char* text)
{
	RoleDataEx* Role = CNetGlobal::RoleData();
	CSplitArgString arg(text);
	int UseItemPos = atol((char*)(arg.GetString(0, 0).c_str()));
	int TargetItemPos = atol((char*)(arg.GetString(1, 1).c_str()));

	ItemFieldStruct* UseItem = Role->GetBodyItem(UseItemPos);
	ItemFieldStruct* TargetItem = Role->GetBodyItem(TargetItemPos);

	if (UseItem == NULL || TargetItem == NULL)
	{
		g_pChatFrame->AddMessage(5, "輸入位置有問題");
		return;
	}


	NetCli_Item::_UseItemToItem_Body(*UseItem, UseItemPos, *TargetItem, TargetItemPos);
}

// 測試 使用背包物品
void UseBagItem(const char* text)
{
	int index = atoi(text);
	ItemFieldStruct* item = CNetGlobal::RoleData()->GetBodyItem(index);
	if (item && !item->IsEmpty() && item->Pos == EM_ItemState_NONE) {
		NetCli_Item::_UseItem_Body(*item, index, -1);
	}
}

//登出
void Logout(const char* text)
{
	g_pChatFrame->AddMessage(5, "登出");
	Net_GSrvConnect::Logout();
}

void BillboardIsAnonymous(const char* text)
{
	CSplitArgString arg(text);
	bool IsAnonymous = (atol((char*)(arg.GetString(0, 0).c_str())) != 0);
	RoleDataEx* Role = CNetGlobal::RoleData();
	Role->PlayerBaseData->IsBillboardAnonymous = IsAnonymous;

	NetCli_Billboard::S_SetAnonymous(IsAnonymous);
}
void BillboardRankList(const char* text)
{
	CSplitArgString arg(text);
	int Type = atol((char*)(arg.GetString(0, 0).c_str()));
	int RankBegin = atol((char*)(arg.GetString(1, 1).c_str()));

	NetCli_Billboard::S_RankInfoRequest((BillBoardMainTypeENUM)Type, RankBegin);
}
void BillboardClose(const char* text)
{
	NetCli_Billboard::S_CloseBillboardRequest();
}

void FlagTest(const char* text)
{
	CSplitArgString arg(text);
	int iCmd = atol((char*)(arg.GetString(0, 0).c_str()));

	switch (iCmd)
	{
	case 1: // GetFlag
	{
		Net_GSrvConnect::GetFlagList();
	} break;
	case 2: // EditFlag
	{
		CliFlagPosInfo Flag;
		Net_GSrvConnect::EditFlag(Flag);
	} break;
	case 3: // DelFlag
	{
		int iFlagGroupID, iFlagID;
		Net_GSrvConnect::DelFlag(iFlagGroupID, iFlagID);
	} break;
	case 4: // DelFlagGroup
	{
		int iFlagGroupID;
		Net_GSrvConnect::DelFlagGroup(iFlagGroupID);
	} break;
	}


}


void PKInvite(const char* text)
{
	CSplitArgString arg(text);
	int TargetID = atol((char*)(arg.GetString(0, 0).c_str()));

	NetCli_PK::S_PKInviteRequest(TargetID);
}
void PKAccept(const char* text)
{
	CSplitArgString arg(text);
	int TargetID = atol((char*)(arg.GetString(0, 0).c_str()));
	int Result = atol((char*)(arg.GetString(1, 1).c_str()));

	NetCli_PK::S_PKInviteResult(TargetID, (Result != 0));
}
void PKCancel(const char* text)
{
	NetCli_PK::S_PKCancel();
}

//全區旗標測試
void AllZoneStateTest(const char* text)
{
	PlayerStateStruct  State;
	State.FindParty = true;
	NetCli_RoleValue::S_SetPlayerAllZoneState(State);
}

void SearchFindPartyPlayerTest(const char* text)
{
	NetCli_FriendList::SL_SearchFindPartyPlayerRequest(EM_Vocation_None, EM_Vocation_None, 0, 100, 0, 100);
}

void ListTest(const char* text)
{
	NetCli_Other::test();
}

//-----------------------------------------------------------------------------
void PlayerBlind(const char* text)
{
	if (strcmp(text, "on") == 0)
		g_pGameMain->SetBlindEffect(true);
	else
		g_pGameMain->SetBlindEffect(false);
}
//-----------------------------------------------------------------------------
void SetGameTime(const char* text)
{
	CSplitArgString arg(text);
	time_t timet;

	int iHr = atol((char*)(arg.GetString(0, 0).c_str()));
	int iMin = atol((char*)(arg.GetString(1, 1).c_str()));

	tm timetm;

	time(&timet);
	tm* newTime = localtime(&timet);

	timetm = *newTime;

	timetm.tm_hour = iHr;
	timetm.tm_min = iMin;

	timet = mktime(&timetm);

	g_pGameMain->SetGameTime((int)timet, (int)g_pGameMain->GetServerTimeZone());

}

//取出卡片
void TakeOutCard(const char* text)
{
	CSplitArgString arg(text);

	int CardObjID = atol((char*)(arg.GetString(0, 0).c_str()));

	NetCli_Item::_TakeOutCardRequest(CardObjID);
}

//設定霧距
void SetFog(const char* text)
{
	CSplitArgString arg(text);

	float Min = (float)atof((char*)(arg.GetString(0, 0).c_str()));
	float Max = (float)atof((char*)(arg.GetString(1, 1).c_str()));

	g_pGameMain->SetFogDistance(Min, Max);

}

// 設定滑鼠移動
void SetMouseMove(const char* text)
{
	if (strcmp(text, "on") == 0)
		g_pGameMain->SetMouseMoveEnable(true);
	else
		g_pGameMain->SetMouseMoveEnable(false);
}

//cli to Cli 封包轉送測試
void ClitoCliNetTest(const char* text)
{
	CSplitArgString arg(text);
	string Name = arg.GetString(0, 0);
	string Txt = arg.GetString(1, 1);

	NetCli_Channel::SendToCliTest((char*)Name.c_str(), (char*)Txt.c_str());
}

//公會供獻測試
void GuildContributionItem(const char* text)
{
	CSplitArgString arg(text);
	RoleDataEx* Role = CNetGlobal::RoleData();
	//int Pos , ItemFieldStruct& Item , bool IsPut;
	int Pos = atol((char*)(arg.GetString(0, 0).c_str()));
	if (Pos >= Role->PlayerBaseData->Body.Count)
		return;

	ItemFieldStruct* Item = Role->GetBodyItem(Pos);
	if (Item == NULL)
		return;

	bool IsPut = true;
	if (Item->Pos == EM_USE_GUILDCONTRIBUTION)
		IsPut = false;
	else if (Item->Pos != EM_ItemState_NONE)
		return;

	NetCli_Guild::SL_GuildContributionItem(Pos, *Item, IsPut);
}
void GuildContributionOK(const char* text)
{
	NetCli_Guild::SL_GuildContributionOK(0, 0);
}
void GuildContributionClose(const char* text)
{
	NetCli_Guild::SL_GuildContributionClose();
}

//公會競標測試
void GuildFightAuctionClose(const char* text)
{
	NetCli_Guild::SL_GuildFightAuctionClose();
}
void GuildFightAuctionBit(const char* text)
{
	CSplitArgString arg(text);
	int BitScore = atol((char*)(arg.GetString(0, 0).c_str()));
	//int NPCIDBitScore = atol( (char*)(arg.GetString(0, 0).c_str() ) );
	NetCli_Guild::SL_GuildFightAuctionBid(BitScore, 0);
}

void GuildMemberDBID(const char* text)
{
	for (vector< GuildMemberBaseInfoStruct >::iterator it = NetCli_Guild::_MemberList.begin(); it != NetCli_Guild::_MemberList.end(); it++)
	{
		char szBuff[2048];
		sprintf(szBuff, "DBID = %d , Name = %s, Online = %d", (*it).MemberDBID, (*it).MemberName, (int)(*it).IsOnline);
		g_pChatFrame->SendMsgEvent("CHAT_MSG_SYSTEM", szBuff, "");
	}
}

void GuildFightInvite(const char* text)
{
	CSplitArgString arg(text);
	int SrvID = atol((char*)(arg.GetString(0, 0).c_str()));
	int RoomID = atol((char*)(arg.GetString(1, 1).c_str()));
	int GuildID = atol((char*)(arg.GetString(2, 2).c_str()));
	int iCrystalID = atol((char*)(arg.GetString(2, 2).c_str()));
	int DBID = atol((char*)(arg.GetString(3, 3).c_str()));

	NetCli_Guild::SL_GuildFightRequestInvite(SrvID, RoomID, iCrystalID, GuildID, DBID);
}


//樂透彩測試
void LotteryExchangePrize(const char* text)
{
	CSplitArgString arg(text);
	int		Pos = atol((char*)(arg.GetString(0, 0).c_str()));
	RoleDataEx* Role = CNetGlobal::RoleData();
	ItemFieldStruct* Item = Role->GetBodyItem(Pos);

	if (Item == NULL)
		return;

	NetCli_Lottery::SL_ExchangePrize(*Item, Pos);
}
void LotteryCloseExchangePrize(const char* text)
{
	NetCli_Lottery::SL_CloseExchangePrize();
}
void LotteryBuy(const char* text)
{
	CSplitArgString arg(text);
	int Count = atol((char*)(arg.GetString(0, 0).c_str()));
	if (Count > 20 || Count < 1)
		return;

	int	Number[20];
	int Money = 0;
	for (int i = 0; i < 20; i++)
		Number[i] = i + 1;


	LotteryInfoTableStruct* LotteryInfoTable = g_ObjectData->LotteryInfoTable(Count);
	if (LotteryInfoTable == NULL)
		return;

	NetCli_Lottery::SL_BuyLottery(LotteryInfoTable->CombinationCount * 500, Count, Number);
}
void LotteryCloseBuy(const char* text)
{
	NetCli_Lottery::SL_CloseBuyLottery();
}
void LotteryInfoRequest(const char* text)
{
	NetCli_Lottery::SL_LotteryInfoRequest();
}

void CastSpellByID(const char* text)
{
	g_pGameMain->CastSpellByID(atoi(text));
}

void SetHelmetVisible(const char* text)
{
	bool visible = false;
	if (strcmp(text, "on") == 0)
		visible = true;
	//NetCli_EQ::S_SetHideHelmet(visible);
}

void NPCPosInfo_Reset(const char* text)
{
	NetCli_Other::S_AllObjectPosRequest();
}

void NPCPosInfo_Color_Lv(const char* text)
{
	NetCli_OtherChild::Map_ResetPointColor_Lv();
}

void NPCPosInfo_Clear(const char* text)
{
	NetCli_OtherChild::Map_ClearAllObjectPoint();
}
void NPCPosInfo_Show_ID(const char* text)
{
	CSplitArgString arg(text);
	int OrgObjID = atol((char*)(arg.GetString(0, 0).c_str()));

	NetCli_OtherChild::Map_SetObject_ID(OrgObjID);
}

// Type 0 玩家 1 NPC 2 QuestNPC 3 礦產
void NPCPosInfo_Show_Type(const char* text)
{
	CSplitArgString arg(text);
	int TypeID = atol((char*)(arg.GetString(0, 0).c_str()));

	NetCli_OtherChild::Map_SetObject_Type(TypeID);
}

//////////////////////////////////////////////////////////////////////////
// 房屋系統測試
//////////////////////////////////////////////////////////////////////////
void VisitHouseRequest(const char* text)
{
	CSplitArgString arg(text);
	int HouseDBID = atol((char*)(arg.GetString(0, 0).c_str()));
	char* Password = (char*)(arg.GetString(1, 1).c_str());
	NetCli_Houses::S_VisitHouseRequest(HouseDBID, Password);
}
void CloseVisitHouse(const char* text)
{
	NetCli_Houses::S_CloseVisitHouse();
}
void SwapBodyHouseItem(const char* text)
{
	CSplitArgString arg(text);
	int BodyPos = atol((char*)(arg.GetString(0, 0).c_str()));
	int HouseParentItemDBID = atol((char*)(arg.GetString(1, 1).c_str()));
	int HousePos = atol((char*)(arg.GetString(2, 2).c_str()));
	bool IsFromBody = (atol((char*)(arg.GetString(3, 3).c_str()))) == 0 ? false : true;

	NetCli_Houses::S_SwapBodyHouseItem(BodyPos, HousePos, HouseParentItemDBID, IsFromBody);
}

void SwapHouseItem(const char* text)
{
	CSplitArgString arg(text);

	int HouseParentItemDBID1 = atol((char*)(arg.GetString(0, 0).c_str()));
	int HousePos1 = atol((char*)(arg.GetString(1, 1).c_str()));
	int HouseParentItemDBID2 = atol((char*)(arg.GetString(2, 2).c_str()));
	int HousePos2 = atol((char*)(arg.GetString(3, 3).c_str()));

	NetCli_Houses::S_SwapHouseItemRequest(HouseParentItemDBID1, HousePos1, HouseParentItemDBID2, HousePos2);
}

void HouseItemLayoutRequest(const char* text)
{
	CSplitArgString arg(text);
	int					ItemDBID = atol((char*)(arg.GetString(0, 0).c_str()));
	House3DLayoutStruct Layout;
	NetCli_Houses::S_HouseItemLayoutRequest(ItemDBID, Layout);
}
void ChangeHouseRequest(const char* text)
{
	CSplitArgString arg(text);
	int		HouseType = atol((char*)(arg.GetString(0, 0).c_str()));
	NetCli_Houses::S_ChangeHouseRequest(HouseType);
}
void BuyFunctionRequest(const char* text)
{
	CSplitArgString arg(text);
	int		FunctionID = atol((char*)(arg.GetString(0, 0).c_str()));
	NetCli_Houses::S_BuyFunctionRequest(FunctionID);
}
void FunctionSettingRequest(const char* text)
{
	CSplitArgString arg(text);
	int		FunctionID = atol((char*)(arg.GetString(0, 0).c_str()));
	int		Mode = atol((char*)(arg.GetString(1, 1).c_str()));
	NetCli_Houses::S_FunctionSettingRequest(FunctionID, Mode);
}
void BuyEnergyRequest(const char* text)
{
	CSplitArgString arg(text);
	int		Money_Account = atol((char*)(arg.GetString(0, 0).c_str()));

	NetCli_Houses::S_BuyEnergyRequest(Money_Account, "");
}

void LeaveHouse(const char* text)
{
	CSplitArgString arg(text);
	int PlayerDBID = atol((char*)(arg.GetString(0, 0).c_str()));

	NetCli_Houses::S_LeaveHouse(PlayerDBID);
}

void ReloadUI(const char* text)
{
	g_pInterface->ReloadUI();
}

void ReloadCliScript(const char* text)
{
	g_pGameMain->ReloadClientLua();
}

void SetHousePassword(const char* text)
{
	CSplitArgString arg(text);
	NetCli_Houses::S_SetPassword(arg.GetString(0, 0).c_str());
}
void ExchangeEq(const char* text)
{
	CSplitArgString arg(text);
	int		BoxID = atol((char*)(arg.GetString(0, 0).c_str()));
	int		BoxPos = atol((char*)(arg.GetString(1, 1).c_str()));
	int		EqPos = atol((char*)(arg.GetString(2, 2).c_str()));
	NetCli_Houses::S_ExchangeEq(BoxID, BoxPos, (EQWearPosENUM)EqPos);
}

// --------------------------------------------------------------------------------------
//全域劇情測試
void RunGlobalPlot(const char* text)
{
	CSplitArgString arg(text);
	int		PlotID = atol((char*)(arg.GetString(0, 0).c_str()));
	NetCli_OtherChild::S_RunGlobalPlotRequest(PlotID);
}

// --------------------------------------------------------------------------------------
void ShowError(const char* text)
{
	g_pError->Show();
	g_pError->SetPos(0, 0);
}
// --------------------------------------------------------------------------------------
//商城測試
void DS_Function(const char* text)
{
	CSplitArgString arg(text);
	int		Value = atol((char*)(arg.GetString(0, 0).c_str()));
	NetCli_DepartmentStore::SL_ShopFunctionRequest(Value);
}
void DS_SellType(const char* text)
{
	NetCli_DepartmentStore::SL_SellTypeRequest();
}
void DS_ShopInfo(const char* text)
{
	CSplitArgString arg(text);
	int		Value = atol((char*)(arg.GetString(0, 0).c_str()));
	NetCli_DepartmentStore::SL_ShopInfoRequest(Value);
}
void DS_BuyItem(const char* text)
{
	CSplitArgString arg(text);
	int		Value = atol((char*)(arg.GetString(0, 0).c_str()));
	//NetCli_DepartmentStore::SL_BuyItem( Value );
}
/*
void ManageKey			( const char* text )
{
	NetCli_Other::S_ManagePasswordKey( );
}
void ManagePassword			( const char* text )
{
	CSplitArgString arg(text);
	NetCli_Other::S_ManagePassword( (char*)arg.GetString(0, 0).c_str() );
}
void DisableLogin		( const char* text )
{
	CSplitArgString arg(text);
	NetCli_Other::SL_CloseMaster( (char*)arg.GetString(0, 0).c_str() );
}
*/
void GuildDeclareWar(const char* text)
{
	CSplitArgString arg(text);
	int		TargetGuildID = atol((char*)(arg.GetString(0, 0).c_str()));
	NetCli_Guild::SL_DeclareWar(1, TargetGuildID);
}

void GuildWarList(const char* text)
{
	vector<GuildDeclareWarInfo>& guildWar = NetCli_GuildChild::s_declareWarInfo;

	g_pChatFrame->SendChatMessageEx(CHAT_MSG_SYSTEM, "", "GuildDeclareWarInfo'size %d", guildWar.size());

	for (vector<GuildDeclareWarInfo>::iterator iter = guildWar.begin(); iter != guildWar.end(); iter++)
	{
		const char* guild1 = g_pGuildFrame->GetGuildNameByID(iter->bothID[0]);
		const char* guild2 = g_pGuildFrame->GetGuildNameByID(iter->bothID[1]);

		if (guild1 == NULL || guild1[0] == 0)
			guild1 = "unknown";
		if (guild2 == NULL || guild2[0] == 0)
			guild2 = "unknown";

		g_pChatFrame->SendChatMessageEx(CHAT_MSG_SYSTEM, "", "%s[%d] %s[%d]", guild1, iter->bothID[0], guild2, iter->bothID[1]);
	}
}

void GuildID(const char* text)
{
	CRoleSprite* sprite = g_pGameMain->GetPlayer();
	int guild = 0;
	if (sprite)
		guild = sprite->GetGuildID();

	const char* guild1 = g_pGuildFrame->GetGuildNameByID(guild);
	g_pChatFrame->SendChatMessageEx(CHAT_MSG_SYSTEM, "", "Player's guild %s[%d] RoleData[%d]", guild1, guild, CNetGlobal::RoleData()->BaseData.GuildID);
}

void ClientCrash(const char* text)
{
	CSplitArgString arg(text);

	int		iType = atol((char*)(arg.GetString(0, 0).c_str()));

	switch (iType)
	{
	case 1:
	{
		vector<int> temp;
		vector<int>::iterator it = temp.begin();
		it++;
		int b = *it;
	} break;
	case 2:
	{
		char* sz = NULL;
		int a = 0;
		sprintf(sz, "%d %s", 1, a);
		printf(sz);
	}	break;
	default:
		int* a = NULL;
		*a = 3;
		break;
	}

}

//給予物品跑劇情 測試
void GiveItemRunPlot(const char* text)
{
	int ItemPos[4] = { 0 ,1 , 2 , 3 };

	NetCli_Other::S_GiveItemPlot(true, ItemPos);
}

void JoinBG(const char* text)
{
	CSplitArgString arg(text);
	int		OBJID = atol((char*)(arg.GetString(0, 0).c_str()));
	int		BGID = atol((char*)(arg.GetString(1, 1).c_str()));

	CNetCli_BattleGround::JoinBattleGround(OBJID, BGID, -1, BGID, NULL);
}

void EnterBG(const char* text)
{
	CSplitArgString arg(text);

	//int		iSrvID	= atol( (char*)(arg.GetString(0, 0).c_str()) );
	//int		iType	= atol( (char*)(arg.GetString(1, 1).c_str()) );
	//int		iRoomID	= atol( (char*)(arg.GetString(2, 2).c_str()) );
	//int		iTeamID	= atol( (char*)(arg.GetString(3, 3).c_str()) );

	//CNetCli_BattleGround::EnterBattleGround( iSrvID, iType, iRoomID, iTeamID );

	int		iType = atol((char*)(arg.GetString(0, 0).c_str()));
	int		iRoomID = atol((char*)(arg.GetString(1, 1).c_str()));
	int		iTeamID = atol((char*)(arg.GetString(2, 2).c_str()));
	CNetCli_BattleGround::EnterBattleGround(iType, iRoomID, iTeamID);
}

void SetCameraZoomLocked(const char* text)
{
	bool locked = false;
	if (strcmp(text, "on") == 0) {
		locked = true;
	}
	else if (strcmp(text, "off") == 0) {
		locked = false;
	}
	g_pGameMain->SetCameraZoomLocked(locked);
}

void SetFreeCamera(const char* text)
{
	bool locked = false;
	if (strcmp(text, "on") == 0) {
		locked = true;
	}
	else if (strcmp(text, "off") == 0) {
		locked = false;
	}
	g_pGameMain->SetFreeCameraEnalbe(locked);
}

void LeaveBG(const char* text)
{
	CNetCli_BattleGround::LeaveBattleGround();
}

void LearnRecipe(const char* text)
{
	CSplitArgString arg(text);
	int		iRecipeID = atol((char*)(arg.GetString(0, 0).c_str()));
	NetCli_Recipe::SL_LearnRecipe(iRecipeID);
}

void ReleaseAllFontIndex(const char* text)
{
	CRuFontTexture2::ReleaseAllFontIndex();
}

void PathMoveTo(const char* text)
{
	CRuVector3 pos(0.0f, 0.0f, 0.0f);
	CSplitArgString arg_text(text);
	pos.m_x = (float)atof(arg_text.GetString(0, 0).c_str());
	pos.m_y = (float)atof(arg_text.GetString(1, 1).c_str());
	pos.m_z = (float)atof(arg_text.GetString(2, 2).c_str());
	g_pGameMain->PathMoveTo(pos);
}

void ToggleOptionRenderTerrain(const char* text)
{
	CRuOptimalTerrain* terrain = g_pGameMain->GetRuTerrain();
	if (terrain)
		terrain->ToggleOption_RenderTerrain();
}

void CreateBackEntity(const char* text)
{
	CRoleSprite* player = g_pGameMain->GetPlayer();
	if (player)
	{
		player->CreateBackEntity(atoi(text));
	}
}

void TestRecipe(const char* text)
{
	//CSplitArgString arg(text);
	//int		iRecipeID	= atol( (char*)(arg.GetString(0, 0).c_str()) );

	StaticFlag< DF_MAX_RECIPELIST > vecTemp;

	vecTemp.SetFlagOn(1);
	vecTemp.SetFlagOn(2);
	vecTemp.SetFlagOn(3);
	vecTemp.SetFlagOn(4);
	vecTemp.SetFlagOn(5);

	vecTemp.SetFlagOn(6);
	vecTemp.SetFlagOn(7);
	vecTemp.SetFlagOn(8);
	vecTemp.SetFlagOn(9);
	vecTemp.SetFlagOn(10);
	vecTemp.SetFlagOn(11);

	NetCli_Recipe::This->RL_RecipeList(&vecTemp);
}

void	GetBGScore(const char* text)
{
	StructBattleGroundScore	Array[24];

	ZeroMemory(Array, sizeof(Array));

	Array[0].iDBID = 1;
	Array[0].iRoomID = 1;
	Array[0].iTeamID = 1;
	Array[0].Name = "Alice";
	Array[0].Score.iKillVal = (int)(((double)rand() / (double)RAND_MAX) * 100);
	Array[0].Score.iDamageVal = rand();
	Array[0].Score.iDeadVal = (int)(((double)rand() / (double)RAND_MAX) * 100);
	Array[0].Score.iHealVal = rand();


	Array[1].iDBID = 2;
	Array[1].iRoomID = 1;
	Array[1].iTeamID = 1;
	Array[1].Name = "Eiki";
	Array[1].Score.iKillVal = (int)(((double)rand() / (double)RAND_MAX) * 100);
	Array[1].Score.iDamageVal = rand();
	Array[1].Score.iDeadVal = (int)(((double)rand() / (double)RAND_MAX) * 100);
	Array[1].Score.iHealVal = rand();

	Array[2].iDBID = 3;
	Array[2].iRoomID = 1;
	Array[2].iTeamID = 2;
	Array[2].Name = "Marisa";
	Array[2].Score.iKillVal = (int)(((double)rand() / (double)RAND_MAX) * 100);
	Array[2].Score.iDamageVal = rand();
	Array[2].Score.iDeadVal = (int)(((double)rand() / (double)RAND_MAX) * 100);
	Array[2].Score.iHealVal = rand();

	Array[3].iDBID = 4;
	Array[3].iRoomID = 1;
	Array[3].iTeamID = 2;
	Array[3].Name = "Reimu";
	Array[3].Score.iKillVal = (int)(((double)rand() / (double)RAND_MAX) * 100);
	Array[3].Score.iDamageVal = rand();
	Array[3].Score.iDeadVal = (int)(((double)rand() / (double)RAND_MAX) * 100);
	Array[3].Score.iHealVal = rand();

	Array[4].iDBID = 5;
	Array[4].iRoomID = 1;
	Array[4].iTeamID = 1;
	Array[4].Name = "Reisei";
	Array[4].Score.iKillVal = (int)(((double)rand() / (double)RAND_MAX) * 100);
	Array[4].Score.iDamageVal = rand();
	Array[4].Score.iDeadVal = (int)(((double)rand() / (double)RAND_MAX) * 100);
	Array[4].Score.iHealVal = rand();

	Array[5].iDBID = 6;
	Array[5].iRoomID = 1;
	Array[5].iTeamID = 1;
	Array[5].Name = "Erin";
	Array[5].Score.iKillVal = (int)(((double)rand() / (double)RAND_MAX) * 100);
	Array[5].Score.iDamageVal = rand();
	Array[5].Score.iDeadVal = (int)(((double)rand() / (double)RAND_MAX) * 100);
	Array[5].Score.iHealVal = rand();

	Array[6].iDBID = 7;
	Array[6].iRoomID = 1;
	Array[6].iTeamID = 2;
	Array[6].Name = "Sakuya";
	Array[6].Score.iKillVal = (int)(((double)rand() / (double)RAND_MAX) * 100);
	Array[6].Score.iDamageVal = rand();
	Array[6].Score.iDeadVal = (int)(((double)rand() / (double)RAND_MAX) * 100);
	Array[6].Score.iHealVal = rand();

	Array[7].iDBID = 8;
	Array[7].iRoomID = 1;
	Array[7].iTeamID = 2;
	Array[7].Name = "Reminia";
	Array[7].Score.iKillVal = (int)(((double)rand() / (double)RAND_MAX) * 100);
	Array[7].Score.iDamageVal = rand();
	Array[7].Score.iDeadVal = (int)(((double)rand() / (double)RAND_MAX) * 100);
	Array[7].Score.iHealVal = rand();

	Array[8].iDBID = 9;
	Array[8].iRoomID = 1;
	Array[8].iTeamID = 1;
	Array[8].Name = "Komichi";
	Array[8].Score.iKillVal = (int)(((double)rand() / (double)RAND_MAX) * 100);
	Array[8].Score.iDamageVal = rand();
	Array[8].Score.iDeadVal = (int)(((double)rand() / (double)RAND_MAX) * 100);
	Array[8].Score.iHealVal = rand();

	Array[9].iDBID = 10;
	Array[9].iRoomID = 1;
	Array[9].iTeamID = 1;
	Array[9].Name = "Suika";
	Array[9].Score.iKillVal = (int)(((double)rand() / (double)RAND_MAX) * 100);
	Array[9].Score.iDamageVal = rand();
	Array[9].Score.iDeadVal = (int)(((double)rand() / (double)RAND_MAX) * 100);
	Array[9].Score.iHealVal = rand();

	Array[10].iDBID = 11;
	Array[10].iRoomID = 1;
	Array[10].iTeamID = 2;
	Array[10].Name = "Meilin";
	Array[10].Score.iKillVal = (int)(((double)rand() / (double)RAND_MAX) * 100);
	Array[10].Score.iDamageVal = rand();
	Array[10].Score.iDeadVal = (int)(((double)rand() / (double)RAND_MAX) * 100);
	Array[10].Score.iHealVal = rand();

	Array[11].iDBID = 12;
	Array[11].iRoomID = 1;
	Array[11].iTeamID = 2;
	Array[11].Name = "Chiruno";
	Array[11].Score.iKillVal = (int)(((double)rand() / (double)RAND_MAX) * 100);
	Array[11].Score.iDamageVal = rand();
	Array[11].Score.iDeadVal = (int)(((double)rand() / (double)RAND_MAX) * 100);
	Array[11].Score.iHealVal = rand();

	Array[12].iDBID = 13;
	Array[12].iRoomID = 1;
	Array[12].iTeamID = 1;
	Array[12].Name = "Yukari";
	Array[12].Score.iKillVal = (int)(((double)rand() / (double)RAND_MAX) * 100);
	Array[12].Score.iDamageVal = rand();
	Array[12].Score.iDeadVal = (int)(((double)rand() / (double)RAND_MAX) * 100);
	Array[12].Score.iHealVal = rand();

	Array[13].iDBID = 14;
	Array[13].iRoomID = 1;
	Array[13].iTeamID = 1;
	Array[13].Name = "aaaaaaaaaaaaa";
	Array[13].Score.iKillVal = (int)(((double)rand() / (double)RAND_MAX) * 100);
	Array[13].Score.iDamageVal = rand();
	Array[13].Score.iDeadVal = (int)(((double)rand() / (double)RAND_MAX) * 100);
	Array[13].Score.iHealVal = rand();

	Array[14].iDBID = 15;
	Array[14].iRoomID = 1;
	Array[14].iTeamID = 2;
	Array[14].Name = "bbbbbbbbbbbbbb";
	Array[14].Score.iKillVal = (int)(((double)rand() / (double)RAND_MAX) * 100);
	Array[14].Score.iDamageVal = rand();
	Array[14].Score.iDeadVal = (int)(((double)rand() / (double)RAND_MAX) * 100);
	Array[14].Score.iHealVal = rand();

	Array[15].iDBID = 16;
	Array[15].iRoomID = 1;
	Array[15].iTeamID = 2;
	Array[15].Name = "cccccccccccc";
	Array[15].Score.iKillVal = (int)(((double)rand() / (double)RAND_MAX) * 100);
	Array[15].Score.iDamageVal = rand();
	Array[15].Score.iDeadVal = (int)(((double)rand() / (double)RAND_MAX) * 100);
	Array[15].Score.iHealVal = rand();

	Array[16].iDBID = 17;
	Array[16].iRoomID = 1;
	Array[16].iTeamID = 1;
	Array[16].Name = "ddddddddddddddd";
	Array[16].Score.iKillVal = (int)(((double)rand() / (double)RAND_MAX) * 100);
	Array[16].Score.iDamageVal = rand();
	Array[16].Score.iDeadVal = (int)(((double)rand() / (double)RAND_MAX) * 100);
	Array[16].Score.iHealVal = rand();

	Array[17].iDBID = 18;
	Array[17].iRoomID = 1;
	Array[17].iTeamID = 1;
	Array[17].Name = "eeeeeeeeeeeeeeee";
	Array[17].Score.iKillVal = (int)(((double)rand() / (double)RAND_MAX) * 100);
	Array[17].Score.iDamageVal = rand();
	Array[17].Score.iDeadVal = (int)(((double)rand() / (double)RAND_MAX) * 100);
	Array[17].Score.iHealVal = rand();

	Array[18].iDBID = 19;
	Array[18].iRoomID = 1;
	Array[18].iTeamID = 2;
	Array[18].Name = "ffffffffffffffffff";
	Array[18].Score.iKillVal = (int)(((double)rand() / (double)RAND_MAX) * 100);
	Array[18].Score.iDamageVal = rand();
	Array[18].Score.iDeadVal = (int)(((double)rand() / (double)RAND_MAX) * 100);
	Array[18].Score.iHealVal = rand();

	Array[19].iDBID = 20;
	Array[19].iRoomID = 1;
	Array[19].iTeamID = 2;
	Array[19].Name = "gggggggggggggg";
	Array[19].Score.iKillVal = (int)(((double)rand() / (double)RAND_MAX) * 100);
	Array[19].Score.iDamageVal = rand();
	Array[19].Score.iDeadVal = (int)(((double)rand() / (double)RAND_MAX) * 100);
	Array[19].Score.iHealVal = rand();

	Array[20].iDBID = 21;
	Array[20].iRoomID = 1;
	Array[20].iTeamID = 1;
	Array[20].Name = "hhhhhhhhhhhhhhhhh";
	Array[20].Score.iKillVal = (int)(((double)rand() / (double)RAND_MAX) * 100);
	Array[20].Score.iDamageVal = rand();
	Array[20].Score.iDeadVal = (int)(((double)rand() / (double)RAND_MAX) * 100);
	Array[20].Score.iHealVal = rand();

	Array[21].iDBID = 22;
	Array[21].iRoomID = 1;
	Array[21].iTeamID = 1;
	Array[21].Name = "iiiiiiiiiii";
	Array[21].Score.iKillVal = (int)(((double)rand() / (double)RAND_MAX) * 100);
	Array[21].Score.iDamageVal = rand();
	Array[21].Score.iDeadVal = (int)(((double)rand() / (double)RAND_MAX) * 100);
	Array[21].Score.iHealVal = rand();

	Array[22].iDBID = 23;
	Array[22].iRoomID = 1;
	Array[22].iTeamID = 2;
	Array[22].Name = "jjjjjjjjjjjjjjjjj";
	Array[22].Score.iKillVal = (int)(((double)rand() / (double)RAND_MAX) * 100);
	Array[22].Score.iDamageVal = rand();
	Array[22].Score.iDeadVal = (int)(((double)rand() / (double)RAND_MAX) * 100);
	Array[22].Score.iHealVal = rand();

	Array[23].iDBID = 24;
	Array[23].iRoomID = 1;
	Array[23].iTeamID = 2;
	Array[23].Name = "kkkkkkkkkkkkkkk";
	Array[23].Score.iKillVal = (int)(((double)rand() / (double)RAND_MAX) * 100);
	Array[23].Score.iDamageVal = rand();
	Array[23].Score.iDeadVal = (int)(((double)rand() / (double)RAND_MAX) * 100);
	Array[23].Score.iHealVal = rand();


	CNetCli_BattleGround_Child::m_pThis->OnRZ_BattleGroundScoreInfo(0, 24, Array);
}

void GetBGStatus(const char* text)
{
	StructBattleGroundValue Data;
	SecureZeroMemory(&Data, sizeof(Data));
	Data.iRoomID = 1;
	Data.iTeamScore[0] = (int)(((double)rand() / (double)RAND_MAX) * 100);
	Data.iTeamScore[1] = (int)(((double)rand() / (double)RAND_MAX) * 100);
	Data.iBattleGroundID = 15;

	CNetCli_BattleGround_Child::m_pThis->OnRZ_BattleGroundStatus(0, &Data);
}

void CreateEffect(const char* text)
{
	CRoleSprite* player = g_pGameMain->GetPlayer();
	if (player == NULL)
		return;

	CParabolaEffectSprite* effectSprite = new CParabolaEffectSprite;
	if (effectSprite->LoadRuEntity(text))
	{
		// 設定動作
		if (effectSprite->HasMotion("explode")) {
			effectSprite->SetEffectMotionStep(eEffectMotion_Step_Explode);
		}
		else {
			effectSprite->SetEffectMotionStep(eEffectMotion_Step_Appear);			// IDLE物件自動2秒後消失
			effectSprite->SetIdleEffectLiveTime(10.0f);
		}

		// 開始位置
		effectSprite->SetMoveSpeed(120.0f);
		effectSprite->SetBeginSprite(player, eAttach_Right_Hand);

		// 結束位置
		CRoleSprite* target = g_pGameMain->GetPlayerTarget();
		effectSprite->SetTargetPosition(target->GetPos());
	}
	else
	{
		delete effectSprite;
	}
}

void SwitchUpdateElapsedTime(const char* text)
{
	bool show = false;
	if (stricmp(text, "on") == 0)
		show = true;
	g_pGameMain->SwitchUpdateElapsedTime(show);
}

// ----------------------------------------------------------------------------
void FuncCmd_AddSpecialEventFilter(const char* text)
{
	g_pGameMain->PushSpecialEvent(text);
}

// ----------------------------------------------------------------------------
void FuncCmd_RemoveSpecialEventFilter(const char* text)
{
	g_pGameMain->RemoveSpecialEvent(text);
}

// ----------------------------------------------------------------------------
void FuncCmd_ClearSpecialEventFilter(const char* text)
{
	g_pGameMain->ClearSpecialEvent();
}

// --------------------------------------------------------------------------------------
void FuncCmd_SaveRoleData(const char* text)
{
	const char* playerName = g_pGameMain->GetPlayerName();
	if (playerName != NULL)
	{
		wchar_t* ws = MultiByteToWChar(playerName);
		wstring wPlayerName = ws;
		if (ws) delete[] ws;

		if (g_pGameMain->SaveRoleData(wPlayerName.c_str()) == false)
		{
			g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", "Save Role Data Failed!!");
		}
		else
		{
			g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", "Save Role Data Succeeded!!");
		}
	}
}

// ----------------------------------------------------------------------------
void FuncCmd_LoadRoleData(const char* text)
{
	if (text == NULL)
	{
		g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", "please input filename");
		return;
	}

	wchar_t* ws = MultiByteToWChar(text);
	wstring wPlayerName = ws;
	if (ws) delete[] ws;

	if (g_pGameMain->LoadRoleData(wPlayerName.c_str()) == false)
	{
		g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", "Load role'data Failed!!");
	}
	else
	{
		g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", "Load role'data Succeeded!!");
	}
}

// ----------------------------------------------------------------------------
void FuncCmd_CheckUniqueItem(const char* text)
{
	// 訊息玩家是否有此物品
	NetCli_Channel::SendToCliAskHasItem(text, g_pGameMain->GetPlayerName(), 1, 220772);
}

// ----------------------------------------------------------------------------
void FuncCmd_AttachMountSprite(const char* text)
{
	CRoleSprite* player = g_pGameMain->GetPlayer();
	CRoleSprite* target = g_pGameMain->GetPlayerTarget();
	if (target && player)
	{
		Net_Move::S_AttachObjRequest(player->GetIdentify(), target->GetIdentify(), "p_mount01", "p_mount02", 2);
	}
}

// ----------------------------------------------------------------------------
void FuncCmd_DetachMountSprite(const char* text)
{
	CRoleSprite* player = g_pGameMain->GetPlayer();
	CRoleSprite* target = g_pGameMain->GetPlayerTarget();
	if (player && target)
	{
		Net_Move::S_DetachObjRequest(player->GetIdentify(), target->GetIdentify());
	}
}

// ----------------------------------------------------------------------------
void FuncCmd_DetachImplement(const char* text)
{
	CRoleSprite* player = g_pGameMain->GetPlayer();
	CRoleSprite* implement = (player) ? player->GetImplementSprite() : NULL;
	if (player && implement)
	{
		Net_Move::S_DetachObjRequest(player->GetIdentify(), implement->GetIdentify());
	}
}

// ----------------------------------------------------------------------------
void FuncCmd_OpenBotCheck(const char* text)
{
	g_pBotCheckFrame->Open();
}

// ----------------------------------------------------------------------------
void FuncCmd_CreateSprite(const char* text)
{
	CRoleSprite* sprite = new CRoleSprite(true);

	if (sprite)
	{
		sprite->SetObjectId(atoi(text), true);
		sprite->SetGravityMode(true);

		CRoleSprite* player = g_pGameMain->GetPlayer();
		if (player)
		{
			sprite->SetMoveSpeed(player->GetMoveSpeed());
			sprite->SetPos(player->GetPos());
			sprite->FollowSprite(player, 50.0f);
		}
	}
}
// ----------------------------------------------------------------------------
void FuncCmd_SetTargetDirection(const char* text)
{
	CRoleSprite* target = g_pGameMain->GetPlayerTarget();
	if (target)
	{
		CRuVector3 dir(0.0f, 0.0f, 0.0f);
		CSplitArgString arg_text(text);
		dir.m_x = (float)atof(arg_text.GetString(0, 0).c_str());
		dir.m_y = (float)atof(arg_text.GetString(1, 1).c_str());
		dir.m_z = (float)atof(arg_text.GetString(2, 2).c_str());
		dir.Normalize();
		target->SetDirection(dir);
	}
}

// ----------------------------------------------------------------------------
void FuncCmd_CheckGuildName(const char* text)
{
	CSplitArgString arg_text(text);
	const char* name = text;
	LanguageTypeENUM language = EM_LanguageType_DE;
	// string name = arg_text.GetString(0, 0);
	// int language = g_ObjectData->QueryLanguageID(arg_text.GetString(1, 1).c_str());
	wstring outString;

	int resultCode = g_ObjectData->CheckGuildNameRules(name, (CountryTypeENUM)language, outString);
	char* temp = TransToChar(outString.c_str());
	g_pChatFrame->SendChatMessageEx(MESSAGE_TYPE_SYSTEM, "", "Result %d %s", resultCode, temp);
	delete[] temp;
}

//公會屋戰測試
void GuildHouseWarReg(const char* text)
{
	//	NetCli_GuildHousesWar::SL_WarRegister( EM_GuildHouseWarRegisterType_Register );
}
void GuildHouseWarEnter(const char* text)
{
	//	NetCli_GuildHousesWar::SL_EnterWar( );
}
void GuildHouseWarLeave(const char* text)
{
	//	NetCli_GuildHousesWar::SL_LeaveWar( );
}

//////////////////////////////////////////////////////////////////////////
// Vivox
//////////////////////////////////////////////////////////////////////////

void FuncCmd_VivoxCommand(const char* text)
{

}

void FuncCmd_VivoxEnable(const char* text)
{

}

void FuncCmd_VivoxExit(const char* text)
{

}

void FuncCmd_VivoxDisable(const char* text)
{

}

void FuncCmd_VivoxJoin(const char* text)
{

}

void FuncCmd_VivoxInMode(const char* text)
{

}

void FuncCmd_VivoxInModeG(const char* text)
{

}

void FuncCmd_VivoxPartMute(const char* text)
{

}

void FuncCmd_VivoxPartMuteG(const char* text)
{

}

void FuncCmd_VivoxDebugLV(const char* text)
{

}

// GKy 100128 add for Vivox debug and test
void FuncCmd_VivoxDebugV(const char* text)
{

}

//////////////////////////////////////////////////////////////////////////

void FuncCmd_SetCameraFollowTarget(const char* text)
{
	g_pGameMain->SetCameraFollowSprite(g_pGameMain->GetPlayerTarget());
}

void FuncCmd_GetRankPersonalInfo(const char* text)
{
	CSplitArgString arg(text);
	int mode = atoi((char*)(arg.GetString(0, 0).c_str()));

	//CNetCli_BattleGround::GetRankPersonalInfo( mode );
}

void FuncCmd_CreateTargetEffect(const char* text)
{
	CRoleSprite* player = g_pGameMain->GetPlayer();
	if (player)
	{
		CRoleSprite* implement = (player->GetImplementSprite()) ? player->GetImplementSprite() : player;

		g_pGameMain->CreateTargetEffect(TargetLineEffect_Line, implement);
	}
}

void FuncCmd_SetTargetEffectType(const char* text)
{
	CSplitArgString arg(text);

	int effectType = atoi((char*)(arg.GetString(0, 0).c_str()));
	int height = atoi((char*)(arg.GetString(1, 1).c_str()));

	g_pGameMain->SetTargetEffectType((TargetLineEffectType)effectType, height);
}

void FuncCmd_ReleaseTargetEffect(const char* text)
{
	g_pGameMain->ReleaseTargetEffect();
}

void FuncCmd_SetTargetEffectVisible(const char* text)
{
	bool show = false;
	if (stricmp(text, "on") == 0)
		show = true;
	g_pGameMain->SetTargetEntityVisible(show);
}

void FuncCmd_FindAttachPoint(const char* text)
{
	CRoleSprite* target = g_pGameMain->GetPlayerTarget();
	if (target)
	{
		if (RuEntity_FindLinkFrame(target->GetRuEntity(), text, TRUE))
			g_pChatFrame->SendChatMessageEx(CHAT_MSG_SYSTEM, "", "%s is exist", text);
		else
			g_pChatFrame->SendChatMessageEx(CHAT_MSG_SYSTEM, "", "%s not found", text);
	}
}

void FuncCmd_TestPacket(const char* text)
{
	/*
	CRoleSprite* target = g_pGameMain->GetPlayerTarget();
	if ( target )
	{
		if ( RuEntity_FindLinkFrame( target->GetRuEntity(), text, TRUE ) )
			g_pChatFrame->SendChatMessageEx(CHAT_MSG_SYSTEM, "", "%s is exist", text);
		else
			g_pChatFrame->SendChatMessageEx(CHAT_MSG_SYSTEM, "", "%s not found", text);
	}
	*/

	PG_Talk_LtoC_SystemMsg	Packet;

	Packet.ContentSize = 22;


	char szBuff[23];

	szBuff[0] = 84;
	szBuff[1] = 49;
	szBuff[2] = 69;
	szBuff[3] = 66;
	szBuff[4] = 85;
	szBuff[5] = 71;
	szBuff[6] = 93;
	szBuff[7] = 32;
	szBuff[8] = 84;
	szBuff[9] = 49;
	szBuff[10] = 32;
	szBuff[11] = 61;
	szBuff[12] = 32;
	szBuff[13] = 57;
	szBuff[14] = 32;
	szBuff[15] = 84;
	szBuff[16] = 50;
	szBuff[17] = 32;
	szBuff[18] = 61;
	szBuff[19] = 32;
	szBuff[20] = 56;
	szBuff[21] = 0;

	Packet.Content = szBuff;




	//NetCli_TalkChild::S_PG_Talk_LtoC_SystemMsg( 0, sizeof( PG_Talk_LtoC_SystemMsg ), &Packet );



}

void FuncCmd_DisplayEntiptyPointList(const char* text)
{

}

void FuncCmd_SetSuitSkill(const char* text)
{
	CSplitArgString arg(text);

	SkillPlateData Info;
	Info.SkillID = atoi((char*)arg.GetString(0, 0).c_str());
	Info.Pos = atoi((char*)arg.GetString(1, 1).c_str());;
	Info.State = 0;

	//CliNetWaker::PushClientData(&Info , sizeof(Info));

	g_pSuitSkillFrame->SendSkillPlatePosToServer(Info.SkillID, Info.Pos);
}

void FuncCmd_MovePartition(const char* text)
{
	if (stricmp(text, "on") == 0)
		g_pGameMain->MovePartition(true);
	else
		g_pGameMain->MovePartition(false);
}

void FuncCmd_SetPlayerName(const char* text)
{
	CRoleSprite* player = g_pGameMain->GetPlayer();
	if (player)
	{
		player->SetName(text);
	}
}

void GarbageCollect(const char* text)
{
	g_ruResourceManager->GarbageCollect();

	// Release textures held by the dictionary
	g_ruResourceManager->ClearTextureDictionary();

	// Release animations held by the dictionary
	g_ruResourceManager->ClearAnimationDictionary();
	((CRuEngine_ResourceManager*)g_ruResourceManager)->ClearAnimationDictionary_GR2();

	// Release entities held by the dictionary
	g_ruResourceManager->ClearEntityDictionary();

	// Clear generic object dictionary
	g_ruResourceManager->ClearGenericObjectDictionary();
}

void GetZonecoordinate(const char* text)
{

	CSplitArgString arg_text(text);
	string part = arg_text.GetString(0, 0);
	string name = arg_text.GetString(1, 1);

	int					ZoneID = atoi(name.c_str());

	if (ZoneID == 0)
		return;

	ZoneID = (ZoneID % _DEF_ZONE_BASE_COUNT_) + Def_ObjectClass_Zone;

	GameObjDbStructEx* pZoneObj = g_ObjectData->GetObj(ZoneID);

	if (pZoneObj == NULL)
		return;


	CRuFusion_Trekker* _Trekker = NULL;
	_Trekker = ruNEW CRuFusion_Trekker(CombineFilePath(g_pGameMain->GetResourceRootDir(), pZoneObj->Zone.MapFile));

	CRuAABB controlBoundsOut;
	_Trekker->FindMaximalZoneControlBounds(pZoneObj->Zone.MapID, &controlBoundsOut);


}

void SetDefaultWalkingAction(const char* text)
{
	CRoleSprite* sprite = g_pGameMain->GetPlayer();
	if (sprite) {
		CRuFusion_Actor* pActor = sprite->GetRuActor();
		if (pActor) {
			pActor->SetDefaultWalkingAction((CRuFusion_Mime_Action)atoi(text));
		}
	}
}

void SetDefaultRunningAction(const char* text)
{
	CRoleSprite* sprite = g_pGameMain->GetPlayer();
	if (sprite) {
		CRuFusion_Actor* pActor = sprite->GetRuActor();
		if (pActor) {
			pActor->SetDefaultRunningAction((CRuFusion_Mime_Action)atoi(text), (CRuFusion_Mime_Action)atoi(text));
		}
	}
}

void TestGMMsg(const char* text)
{
	NetCli_Talk::GetThis()->Rev_GmMsg(text);
}

void FuncCmd_SetCameraControlMode(const char* text)
{
	if (text)
	{
		if (strcmp(text, "on") == 0)
		{
			g_pGameMain->SetCameraControlMode(true);
		}
		else if (strcmp(text, "off") == 0)
		{
			g_pGameMain->SetCameraControlMode(false);
		}
	}
}

void FuncCmd_Play3DSoundByFile(const char* text)
{
	CRoleSprite* pPlayer = g_pGameMain->GetPlayer();
	g_pRuGlobalEventManager->Play3DSoundByFile(text, pPlayer->GetPos(), FALSE);
}

void FuncCmd_PlaySoundByFile(const char* text)
{
	g_pRuGlobalEventManager->PlaySoundByFile(text, false, 0.0f);
}

void FuncCmd_CreateSpriteActStatic(const char* text)
{
	if (_staticSprite != NULL)
	{
		delete _staticSprite;
		_staticSprite = NULL;
	}
	CRoleSprite* sprite = new CRoleSprite(true);
	if (sprite)
	{
		// /cmd CreateSpriteActStatic model\element\thunderhoof_mesa\prop\stone\act_waterfall_06.ros
		sprite->SetObjectId(100065, true);
		sprite->LoadRuEntity(text);
		sprite->SetGravityMode(true);
		sprite->SetScale(0.1);
		CRoleSprite* player = g_pGameMain->GetPlayer();
		if (player)
		{
			sprite->SetPos(player->GetPos());
		}
	}
	_staticSprite = sprite;
}

void FuncCmd_CheckMd5(const char* text)
{
	char tmp[256];
	IRuStream* stream = g_ruResourceManager->LoadStream(text);
	if (stream != NULL)
	{
		MyMD5Class myMd5Class;
		myMd5Class.ComputeByteHash(stream->OpenStreamMapping(), NULL, stream->GetStreamSize());
		std::string md5 = myMd5Class.GetMd5Str();

		sprintf(tmp, "FILE:%s MD5:%s", text, md5.c_str());
		g_pChatFrame->AddMessage(9, tmp);

		delete stream;
	}
	else
	{
		sprintf(tmp, "FAIL FILE:%s", text);
		g_pChatFrame->AddMessage(9, tmp);
	}
}

void FuncCmd_RequestBufferInfo(const char* text)
{
	PG_Item_CtoL_RequestBufferInfo Send;
	CNetGlobal::SendToLocalPublic(sizeof(Send), &Send);
}