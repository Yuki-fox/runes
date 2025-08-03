#pragma once

// 系統訊息
#define UiEvent_EnterWorld						"ENTER_WORLD"
#define UiEvent_VariablesLoaded					"VARIABLES_LOADED"

#define UiEvent_Sys_WindowsResize				"SYS_WINDOWS_RESIZE"

#define UiEvent_SetLoginScreen					"SET_GLUE_SCREEN"
#define UiEvent_UpdateCharacterSelected			"UPDATE_SELECTED_CHARACTER"
#define UiEvent_CharacterListUpdate				"CHARACTER_LIST_UPDATE"

#define UiEvent_ReceiveChatMessage				"RECEIVE_CHAT_MESSAGE"
#define UiEvent_BagListUpdate					"BAG_LIST_UPDATE"


#define UiEvent_TargetChange					"TARGET_CHANGE"

#define UiEvent_ShowQuestList					"SHOW_QUESTLIST"				

#define UiEvent_ShowQuestDetailFromNPC			"SHOW_QUESTDETAIL_FROM_NPC"				
#define UiEvent_ShowQuestDetailFromBook			"SHOW_QUESTDETAIL_FROM_BOOK"				

#define UiEvent_ResetQuestBook					"RESET_QUESTBOOK"
#define UiEvent_ResetQuestTrack					"RESET_QUESTTRACK"
#define UiEvent_ResetQuestTracker				"RESER_QUESTTRECKER"	
#define UiEvent_ADDNewQuestBook					"ADDNEW_QUESTBOOK"
#define UiEvent_CheckGetItemBeQueueItem			"CHECKGETITEMBEQUEUEITEM"


#define UiEvent_SelectRewardItem				"SELECT_REWARD_ITEM"

#define UiEvent_CraftItemDone					"CRAFT_ITEM_DONE"

#define UiEvent_ShowScriptBorder				"SHOW_SCRIPTBORDER"

// 狀態改變用

#define UiEvent_RoleAttributeChange				"ROLE_ATTRIBUTE_CHANGE"		// 人物各種屬性
#define UiEvent_RoleStatusChange				"ROLE_STATUS_CHANGE"		// HP / MP / SP
#define UiEvent_Role_Exp_Change					"PLAYER_EXP"
#define UiEvent_Role_Money_Change				"PLAYER_MONEY"
#define UiEvent_RoleLVChange					"ROLE_LV_CHANGE"
//#define UiEvent_RoleTableChange					"ROLE_TABLE_CHANGE"			// Class
#define UiEvent_PlayerBuffChanged				"PLAYER_BUFF_CHANGED"		// 角色BUFF資料改變
#define UiEvent_TargetBuffChanged				"TARGET_BUFF_CHANGED"		// 目標對象BUFF資料改變

#define UiEvent_PlayerTitleIDChanged            "PLAYER_TITLE_ID_CHANGED"   // 角色裝備的稱號改變
#define UiEvent_PlayerShowTitleChanged          "PLAYER_SHOW_TITLE_CHANGED" // 角色顯示稱號改變
#define UiEvent_PlayerTitleFlagChanged          "PLAYER_TITLE_FLAG_CHANGED" // 角色稱號取得拔除 自訂稱號修改
#define UiEvent_PlayerGetTitle                  "PLAYER_GET_TITLE" // 角色取得 arg1 = name


#define UiEvent_TradePlayerItemState			"TRADE_PLAYER_ITEM_STATE"
#define UiEvent_TradeRecipientItemState			"TRADE_RECIPIENT_ITEM_STATE"

#define UiEvent_SkillUpdate						"SKILL_UPDATE"				// 學會新的技能
#define UiEvent_TpExpUpdate						"TP_EXP_UPDATE"				// 得到新的技能配點

#define UiEvent_StoreUpdateBuyGoods				"UPDATE_STORE_BUY_GOODS"	// 更新商店商品
#define UiEvent_StoreUpdateSellGoods			"UPDATE_STORE_SELL_GOODS"	// 更新商店商品

#define UiEvent_UnitHealth						"UNIT_HEALTH"				// 生命值改變
#define UiEvent_UnitMana						"UNIT_MANA"					// 法力值改變
#define UiEvent_UnitSkill						"UNIT_SKILL"				// 技能值改變

#define UiEvent_PartyMemberChanged				"PARTY_MEMBER_CHANGED"		// 隊友改變
#define UiEvent_PartyLeaderChanged				"PARTY_LEADER_CHANGED"		// 隊長改變

//#define UiEvent_RaidRosterChanged				"RAID_ROSTER_CHANGED"		// 當團隊成立或解散 或是加入 離開團隊
#define UiEvent_RaidTargetChanged				"RAID_TARGET_CHANGED"		// 團隊目標改變

#define UiEvent_CancelLootRoll					"CANCEL_LOOT_ROLL"			// 取消擲骰項目
#define UIEvent_UpdateLootAssign				"UPDATE_LOOT_ASSIGN"		// 重新整理分配物品清單

#define UiEvent_InitMessageBox					"INIT_MESSAGEBOX"			// 設置 MessageBox

#define UiEvent_PlayerEnterFight				"PLAYER_ENTER_FIGHT"		// 進入戰鬥
#define UiEvent_PlayerLeaveFight				"PLAYER_LEAVE_FIGHT"		// 離開戰鬥

#define UiEvent_RoleEquipChange					"PLAYER_INVENTORY_CHANGED"	// 角色裝備改變
#define UiEvent_RoleCoolSuitChange				"PLAYER_COOL_SUIT_CHANGED"	// 角色酷裝改變


#define UiEvent_LifeSkillChange                 "PLAYER_LIFESKILL_CHANGED"  //生活技能改變


// 對話介面
#define UiEvent_CloseSpeakFrame					"CLOSE_SPEAKFRAME"			// 關掉對話介面


// 好友清單
#define UiEvent_PlayerModelInfoReady			"PLAYER_MODEL_INFO_READY"
#define UiEvent_ViewFriendAlert			        "VIEW_FRIEND_ALERT"			//無法檢視想細資料 需互為好友

#define UiEvent_AddFriend						"ADD_FRIEND"				// 

#define UiEvent_AddFriend						"ADD_FRIEND"				// 
#define UiEvent_DelFriend						"DEL_FRIEND"				// 
#define UiEvent_OnlineFriend					"ONLINE_FRIEND"				// 好友上線
#define UiEvent_ResetFriend						"RESET_FRIEND"				// 朋友資料結構改變, 介面需要重置
#define UiEvent_ShowFriendDetail				"SHOW_FRIEND_DETAIL"		// 

#define UiEvent_RESET_BAD_FRIEND				"RESET_BAD_FRIEND"				// 朋友資料結構改變, 介面需要重置

#define UiEvent_SearchGroupResult				"SEARCH_GROUP_RESULT"		// 離開戰鬥

//地圖事件
//#define UiEvent_UpdateMinimap					"UPDATE_MINIMAP"			// 每 40 ms 更新一次小地圖上的 ICON
#define UiEvent_ZoneChanged						"ZONE_CHANGED"				//改變zone
#define UiEvent_MapChanged						"MAP_CHANGED"				//移動到另一張地圖(ui)
#define UiEvent_MapPing							"MAP_PING"					//發出地圖訊息
#define UiEvent_MapDisable						"MAP_DISABLE"				//
#define UiEvent_MapEnable						"MAP_ENABLE"				//
#define UiEvent_WorldMapIconClick				"WORLDMAP_ICONCLICK"		//                       


#define UiEvent_ShowRequestDialog				"SHOW_REQUEST_DIALOG"		// 顯示詢問對話框
#define UiEvent_HideRequestDialog				"HIDE_REQUEST_DIALOG"		// 關畢詢問對話框

#define UiEvent_ShowRequestListDialog			"SHOW_REQUESTLIST_DIALOG"		// 關畢詢問對話框
#define UiEvent_HideRequestListDialog			"HIDE_REQUESTLIST_DIALOG"		// 關畢詢問對話框
#define UiEvent_ResetSpeakFrameText				"RESET_SPEAKFRAME_TEXT"			// 關畢詢問對話框
#define UiEvent_RESET_SPEAKFRAME_TITLE			"RESET_SPEAKFRAME_TITLE"		// 關畢詢問對話框

#define UiEvent_Craft_LearnRecipe				"CRAFT_LEARNRECIPE"


#define UiEvent_SEARCHQUESTNPCRESULT			"SEARCHQUESTNPCRESULT"	

//工會
#define UiEvent_GuildBoardResult				"Guild_Board_Result"



//彩票
#define UiEvent_OpenExchangePrize				"OpenExchangePrize"
#define UiEvent_CloseExchangePrize				"CloseExchangePrize"
#define UiEvent_OpenBuyLottery					"OpenBuyLottery"
#define UiEvent_CloseBuyLottery					"CloseBuyLottery"


//跑馬燈
#define UiEvent_ScrollBannerMessage				"ScrollBannerMessage"
#define UiEvent_ScrollBannerMessageEX			"ScrollBannerMessageEX"
#define UiEvent_ShowMessageDialog				"ShowMessageDialog"  //訊息對話盒 只有一段訊息 一個關閉按鈕


// 玩家
#define UiEvent_CreatePlayerSprite				"CREATE_PLAYER_SPRITE"

//
#define UiEvent_ShowGemePromrt					"SHOW_GEME_PROMRT"



//魔幻寶盒
#define UiEvent_MagicBoxEnable					"MAGICBOX_ENABLE"
#define UiEvent_MagicboxChanged					"MAGICBOX_CHANGED"
#define UiEvent_MagicboxNotEnoughEnergy			"MAGICBOX_NOT_ENOUGH_ENERGY"



//銀行
#define UiEvent_BankOpen		 "BANK_OPEN"
#define UiEvent_BankClose		 "BANK_CLOSE"
#define UiEvent_BankUpdate		 "BANK_UPDATE"


// FOCUS
#define UiEvent_Focus_Changed					"FOCUS_CHANGED"

//戰場相關介面
#define UiEvent_OpenBattleGroundCampScoreFrame  "OPEN_BATTLEGROUND_CAMP_SCORE_FRAME"
#define UiEvent_CloseBattleGroundCampScoreFrame "CLOSE_BATTLEGROUND_CAMP_SCORE_FRAME"
#define UiEvent_UpdateBattleGroundCampScore		"UPDATE_BATTLEGROUND_CAMP_SCORE"
#define UiEvent_UpdateBattleGroundToweriVar		"UPDATE_BATTLEGROUND_TOWER_IVAR"

#define UiEvent_OpenBattleGroundPlayerScoreFrame  "OPEN_BATTLEGROUND_PLAYER_SCORE_FRAME"
#define UiEvent_BattleGroundClose	"ON_BATTLEGROUND_CLOSE"
#define UiEvent_CloseBattleGroundPlayerScoreFrame "CLOSE_BATTLEGROUND_PLAYER_SCORE_FRAME"
#define UiEvent_UpdateBattleGroundPlayerScore	  "UPDATE_BATTLEGROUND_PLAYER_SCORE"

#define UiEvent_ShowMiniMapBGOptionButton "SHOW_MINIMAP_BG_OPTION"
#define UiEvent_HideMiniMapBGOptionButton "HIDE_MINIMAP_BG_OPTION"

#define UiEvent_OpenBattleGroundRoomListFrame "OPEN_BATTLEGROUND_ROOM_LIST_FRAME"
#define UiEvent_CloseBattleGroundRoomListFrame "CLOSE_BATTLEGROUND_ROOM_LIST_FRAME"
#define UiEvent_UpdateBattleGroundRoomListFrame "UPDATE_BATTLEGROUND_ROOM_LIST_FRAME"

#define UiEvent_UpdateCharArenaScoreListFrame  "UPDATE_CHAR_ARENA_SCORE_LIST_FRAME"

#define UiEvent_OpenCharArenaScoreDetailFrame "OPEN_CHAR_AREA_SCORE_DETAIL_FRAME"
#define UiEvent_UpdateCharArenaScoreDetailFrame "UPDATE_CHAR_ARENA_SCORE_DETAIL_FRAME"


#define UiEvent_TargetHateListUpdated "TARGET_HATE_LIST_UPDATED"

#define UiEvent_ClientCountDownStart "CLIENT_COUNT_DOWN_START"
#define UiEvent_ClientCountDownStop "CLIENT_COUNT_DOWN_STOP"

#define UiEvent_UpdateQueueFrameInfo "UPDATE_QUEUE_FRAME_INFO"
#define UiEvent_GotBattleGroundQueueResult "GOT_BATTLE_GROUND_QUEUE_RESULT"
#define UiEvent_ClearAllQueueStatus "CLEAR_ALL_QUEUE_STATUS"

#define UiEvent_OpenBattleGroundDialog "OPEN_ENTER_BATTLEGROUND_QUERY_DIALOG"
#define UiEvent_CloseBattleGroundDialog "CLOSE_ENTER_BATTLEGROUND_QUERY_DIALOG"

#define UiEvent_StartFlashBattleGroundOptionButton "START_FLASH_BG_OPTION_BUTTON"
#define UiEvent_StopFlashBattleGroundOptionButton "STOP_FLASH_BG_OPTION_BUTTON"

//賽馬場
#define UiEvent_HorseRacingRanking "UPDATE_HORSE_RACING_RANKING"
#define UiEvent_HorseRacingFinalRanking "HORSE_RACING_FINAL_RANKING"

// 公會戰相關介面
#define UiEvent_OpenGuildHousePlayerScoreFrame  "OPEN_GUILDHOUSE_PLAYER_SCORE_FRAME"
#define UiEvent_GuildHouseClose	"ON_GUILDHOUSE_CLOSE"
#define UiEvent_UpdateGuildHousePlayerScore	  "UPDATE_GUILDHOUSE_PLAYER_SCORE"

//防沉迷相關
#define UiEvent_PlayTimeQuataNotify	  "PLAYTIME_QOUTA_NOTIFY"

//ROCCAT Kone Plus 滑鼠LED
#define UiEvent_UpdateMouseLight	"UPDATE_MOUSE_LIGHT"

//鏡世界介面
#define UiEvent_OpenWeekInstanceFrame "OPEN_WEEK_INSTANCE_FRAME"
#define UiEvent_CloseWeekInstanceFrame "CLOSE_WEEK_INSTANCE_FRAME"
#define UiEvent_UpdateWeekInstanceFrame "UPDATE_WEEK_INSTANCE_FRAME"

//幻靈介面
#define UiEvent_OpenPhantomFrame "OPEN_PHANTOM_FRAME"
#define UiEvent_ClosePhantomFrame "CLOSE_PHANTOM_FRAME"
#define UiEvent_UpdatePhantomFrame "UPDATE_PHANTOM_FRAME"
#define UiEvent_UpdatePhantomBagFrame "UPDATE_PHANTOM_BAG_FRAME"

//登入禮介面
#define UiEvent_OpenLoginBonusFrame "OPEN_LOGIN_BONUS_FRAME"
#define UiEvent_CloseLoginBonusFrame "CLOSE_LOGIN_BONUS_FRAME"
#define UiEvent_UpdateLoginBonusFrame "UPDATE_LOGIN_BONUS_FRAME"
#define UiEvent_ShowLoginBonusFrameRequestButton "SHOW_LOGIN_BONUS_FRAME_REQUEST_BUTTON"
#define UiEvent_HideLoginBonusFrameRequestButton "HIDE_LOGIN_BONUS_FRAME_REQUEST_BUTTON"
#define UiEvent_ShowLoginBonusMinimapButton "SHOW_LOGIN_BONUS_MINIMAP_BUTTON"
#define UiEvent_HideLoginBonusMinimapButton "HIDE_LOGIN_BONUS_MINIMAP_BUTTON"
#define UiEvent_ShowLoginBonusMinimapButtonFlash "SHOW_LOGIN_BONUS_MINIMAP_BUTTON_FLASH"
#define UiEvent_HideLoginBonusMinimapButtonFlash "HIDE_LOGIN_BONUS_MINIMAP_BUTTON_FLASH"
#define UiEvent_ShowLoginBonusNextPageButton "SHOW_LOGIN_BONUS_NEXT_PAGE_BUTTON"
#define UiEvent_HideLoginBonusNextPageButton "HIDE_LOGIN_BONUS_NEXT_PAGE_BUTTON"
#define UiEvent_ShowLoginBonusPrePageButton "SHOW_LOGIN_BONUS_PRE_PAGE_BUTTON"
#define UiEvent_HideLoginBonusPrePageButton "HIDE_LOGIN_BONUS_PRE_PAGE_BUTTON"