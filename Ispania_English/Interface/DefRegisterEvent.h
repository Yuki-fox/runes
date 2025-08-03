#pragma once

// �t�ΰT��
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

// ���A���ܥ�

#define UiEvent_RoleAttributeChange				"ROLE_ATTRIBUTE_CHANGE"		// �H���U���ݩ�
#define UiEvent_RoleStatusChange				"ROLE_STATUS_CHANGE"		// HP / MP / SP
#define UiEvent_Role_Exp_Change					"PLAYER_EXP"
#define UiEvent_Role_Money_Change				"PLAYER_MONEY"
#define UiEvent_RoleLVChange					"ROLE_LV_CHANGE"
//#define UiEvent_RoleTableChange					"ROLE_TABLE_CHANGE"			// Class
#define UiEvent_PlayerBuffChanged				"PLAYER_BUFF_CHANGED"		// ����BUFF��Ƨ���
#define UiEvent_TargetBuffChanged				"TARGET_BUFF_CHANGED"		// �ؼй�HBUFF��Ƨ���

#define UiEvent_PlayerTitleIDChanged            "PLAYER_TITLE_ID_CHANGED"   // ����˳ƪ��ٸ�����
#define UiEvent_PlayerShowTitleChanged          "PLAYER_SHOW_TITLE_CHANGED" // ������ܺٸ�����
#define UiEvent_PlayerTitleFlagChanged          "PLAYER_TITLE_FLAG_CHANGED" // ����ٸ����o�ް� �ۭq�ٸ��ק�
#define UiEvent_PlayerGetTitle                  "PLAYER_GET_TITLE" // ������o arg1 = name


#define UiEvent_TradePlayerItemState			"TRADE_PLAYER_ITEM_STATE"
#define UiEvent_TradeRecipientItemState			"TRADE_RECIPIENT_ITEM_STATE"

#define UiEvent_SkillUpdate						"SKILL_UPDATE"				// �Ƿ|�s���ޯ�
#define UiEvent_TpExpUpdate						"TP_EXP_UPDATE"				// �o��s���ޯ�t�I

#define UiEvent_StoreUpdateBuyGoods				"UPDATE_STORE_BUY_GOODS"	// ��s�ө��ӫ~
#define UiEvent_StoreUpdateSellGoods			"UPDATE_STORE_SELL_GOODS"	// ��s�ө��ӫ~

#define UiEvent_UnitHealth						"UNIT_HEALTH"				// �ͩR�ȧ���
#define UiEvent_UnitMana						"UNIT_MANA"					// �k�O�ȧ���
#define UiEvent_UnitSkill						"UNIT_SKILL"				// �ޯ�ȧ���

#define UiEvent_PartyMemberChanged				"PARTY_MEMBER_CHANGED"		// ���ͧ���
#define UiEvent_PartyLeaderChanged				"PARTY_LEADER_CHANGED"		// ��������

//#define UiEvent_RaidRosterChanged				"RAID_ROSTER_CHANGED"		// ��ζ����ߩθѴ� �άO�[�J ���}�ζ�
#define UiEvent_RaidTargetChanged				"RAID_TARGET_CHANGED"		// �ζ��ؼЧ���

#define UiEvent_CancelLootRoll					"CANCEL_LOOT_ROLL"			// �����Y�붵��
#define UIEvent_UpdateLootAssign				"UPDATE_LOOT_ASSIGN"		// ���s��z���t���~�M��

#define UiEvent_InitMessageBox					"INIT_MESSAGEBOX"			// �]�m MessageBox

#define UiEvent_PlayerEnterFight				"PLAYER_ENTER_FIGHT"		// �i�J�԰�
#define UiEvent_PlayerLeaveFight				"PLAYER_LEAVE_FIGHT"		// ���}�԰�

#define UiEvent_RoleEquipChange					"PLAYER_INVENTORY_CHANGED"	// ����˳Ƨ���
#define UiEvent_RoleCoolSuitChange				"PLAYER_COOL_SUIT_CHANGED"	// ����Ÿ˧���


#define UiEvent_LifeSkillChange                 "PLAYER_LIFESKILL_CHANGED"  //�ͬ��ޯ����


// ��ܤ���
#define UiEvent_CloseSpeakFrame					"CLOSE_SPEAKFRAME"			// ������ܤ���


// �n�ͲM��
#define UiEvent_PlayerModelInfoReady			"PLAYER_MODEL_INFO_READY"
#define UiEvent_ViewFriendAlert			        "VIEW_FRIEND_ALERT"			//�L�k�˵��Q�Ӹ�� �ݤ����n��

#define UiEvent_AddFriend						"ADD_FRIEND"				// 

#define UiEvent_AddFriend						"ADD_FRIEND"				// 
#define UiEvent_DelFriend						"DEL_FRIEND"				// 
#define UiEvent_OnlineFriend					"ONLINE_FRIEND"				// �n�ͤW�u
#define UiEvent_ResetFriend						"RESET_FRIEND"				// �B�͸�Ƶ��c����, �����ݭn���m
#define UiEvent_ShowFriendDetail				"SHOW_FRIEND_DETAIL"		// 

#define UiEvent_RESET_BAD_FRIEND				"RESET_BAD_FRIEND"				// �B�͸�Ƶ��c����, �����ݭn���m

#define UiEvent_SearchGroupResult				"SEARCH_GROUP_RESULT"		// ���}�԰�

//�a�Ϩƥ�
//#define UiEvent_UpdateMinimap					"UPDATE_MINIMAP"			// �C 40 ms ��s�@���p�a�ϤW�� ICON
#define UiEvent_ZoneChanged						"ZONE_CHANGED"				//����zone
#define UiEvent_MapChanged						"MAP_CHANGED"				//���ʨ�t�@�i�a��(ui)
#define UiEvent_MapPing							"MAP_PING"					//�o�X�a�ϰT��
#define UiEvent_MapDisable						"MAP_DISABLE"				//
#define UiEvent_MapEnable						"MAP_ENABLE"				//
#define UiEvent_WorldMapIconClick				"WORLDMAP_ICONCLICK"		//                       


#define UiEvent_ShowRequestDialog				"SHOW_REQUEST_DIALOG"		// ��ܸ߰ݹ�ܮ�
#define UiEvent_HideRequestDialog				"HIDE_REQUEST_DIALOG"		// �����߰ݹ�ܮ�

#define UiEvent_ShowRequestListDialog			"SHOW_REQUESTLIST_DIALOG"		// �����߰ݹ�ܮ�
#define UiEvent_HideRequestListDialog			"HIDE_REQUESTLIST_DIALOG"		// �����߰ݹ�ܮ�
#define UiEvent_ResetSpeakFrameText				"RESET_SPEAKFRAME_TEXT"			// �����߰ݹ�ܮ�
#define UiEvent_RESET_SPEAKFRAME_TITLE			"RESET_SPEAKFRAME_TITLE"		// �����߰ݹ�ܮ�

#define UiEvent_Craft_LearnRecipe				"CRAFT_LEARNRECIPE"


#define UiEvent_SEARCHQUESTNPCRESULT			"SEARCHQUESTNPCRESULT"	

//�u�|
#define UiEvent_GuildBoardResult				"Guild_Board_Result"



//�m��
#define UiEvent_OpenExchangePrize				"OpenExchangePrize"
#define UiEvent_CloseExchangePrize				"CloseExchangePrize"
#define UiEvent_OpenBuyLottery					"OpenBuyLottery"
#define UiEvent_CloseBuyLottery					"CloseBuyLottery"


//�]���O
#define UiEvent_ScrollBannerMessage				"ScrollBannerMessage"
#define UiEvent_ScrollBannerMessageEX			"ScrollBannerMessageEX"
#define UiEvent_ShowMessageDialog				"ShowMessageDialog"  //�T����ܲ� �u���@�q�T�� �@���������s


// ���a
#define UiEvent_CreatePlayerSprite				"CREATE_PLAYER_SPRITE"

//
#define UiEvent_ShowGemePromrt					"SHOW_GEME_PROMRT"



//�]���_��
#define UiEvent_MagicBoxEnable					"MAGICBOX_ENABLE"
#define UiEvent_MagicboxChanged					"MAGICBOX_CHANGED"
#define UiEvent_MagicboxNotEnoughEnergy			"MAGICBOX_NOT_ENOUGH_ENERGY"



//�Ȧ�
#define UiEvent_BankOpen		 "BANK_OPEN"
#define UiEvent_BankClose		 "BANK_CLOSE"
#define UiEvent_BankUpdate		 "BANK_UPDATE"


// FOCUS
#define UiEvent_Focus_Changed					"FOCUS_CHANGED"

//�Գ���������
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

//�ɰ���
#define UiEvent_HorseRacingRanking "UPDATE_HORSE_RACING_RANKING"
#define UiEvent_HorseRacingFinalRanking "HORSE_RACING_FINAL_RANKING"

// ���|�Ԭ�������
#define UiEvent_OpenGuildHousePlayerScoreFrame  "OPEN_GUILDHOUSE_PLAYER_SCORE_FRAME"
#define UiEvent_GuildHouseClose	"ON_GUILDHOUSE_CLOSE"
#define UiEvent_UpdateGuildHousePlayerScore	  "UPDATE_GUILDHOUSE_PLAYER_SCORE"

//���I�g����
#define UiEvent_PlayTimeQuataNotify	  "PLAYTIME_QOUTA_NOTIFY"

//ROCCAT Kone Plus �ƹ�LED
#define UiEvent_UpdateMouseLight	"UPDATE_MOUSE_LIGHT"

//��@�ɤ���
#define UiEvent_OpenWeekInstanceFrame "OPEN_WEEK_INSTANCE_FRAME"
#define UiEvent_CloseWeekInstanceFrame "CLOSE_WEEK_INSTANCE_FRAME"
#define UiEvent_UpdateWeekInstanceFrame "UPDATE_WEEK_INSTANCE_FRAME"

//���F����
#define UiEvent_OpenPhantomFrame "OPEN_PHANTOM_FRAME"
#define UiEvent_ClosePhantomFrame "CLOSE_PHANTOM_FRAME"
#define UiEvent_UpdatePhantomFrame "UPDATE_PHANTOM_FRAME"
#define UiEvent_UpdatePhantomBagFrame "UPDATE_PHANTOM_BAG_FRAME"

//�n�J§����
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