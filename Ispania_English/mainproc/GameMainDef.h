#pragma once

// cursor image file name
#define	GLUE_SCREEN_LOGIN						"login"
#define	GLUE_SCREEN_CHAR_SELECT					"charselect"
#define	GLUE_SCREEN_CHAR_CREATE					"charcreate"

#define _Def_UIEvent_OnDrag_					"ON_ITEM_DRAG"
#define _Def_UIEvent_OnDrop_				    "ON_ITEM_DROP"
#define _Def_UIEvent_OnItemUpDate_				"ON_ITEM_UPDATE"
#define _Def_UIEvent_OnBankUpDate_				"ON_BANK_UPDATE"
#define _Def_UIEvent_OnEQUpDate_				"ON_EQ_UPDATE"
#define _Def_UIEvent_OnTradeUpDate_				"ON_TRADE_UPDATE"

#define DEF_COLOR_BLACK							0xFF000000
#define DEF_COLOR_WHITE							0xFFFFFFFF
#define DEF_COLOR_MAROON						0xFF7F0000
#define DEF_COLOR_DARK_GREEN					0xFF007F00
#define DEF_COLOR_OLIVE							0xFF7F7F00
#define DEF_COLOR_DARK_BLUE						0xFF00007F
#define DEF_COLOR_PURPLE						0xFF7F007F
#define DEF_COLOR_AQUAMARINE					0xFF007F7F
#define DEF_COLOR_LIGHT_GRAY					0xFFC0C0C0
#define DEF_COLOR_DARK_GRAY						0xFF808080
#define DEF_COLOR_RED							0xFFFF0000
#define DEF_COLOR_GREEN							0xFF00FF00
#define DEF_COLOR_BLUE							0xFF0000FF
#define DEF_COLOR_YELLOW						0xFFFFFF00
#define DEF_COLOR_MAGENTA						0xFFFF00FF
#define DEF_COLOR_CYAN							0xFF00FFFF
#define DEF_COLOR_ORANGE						0xFFDF681D

enum CursorType
{
	eCursor_Null					= 0,
	eCursor_DragItem,
	eCursor_Point,
	eCursor_Point2,
	eCursor_Rune,
	eCursor_Dissolution,
	eCursor_Enhance,
	eCursor_Brush,
	eCursor_Repair,
	eCursor_Cast_Phy,
	eCursor_Cast_Magic,
	eCursor_Mail,	
	eCursor_Attack,	
	eCursor_Interact,
	eCursor_UnableInteract,
	eCursor_Speak,
	eCursor_UnableSpeak,
	eCursor_Mine,
	eCursor_UnableMine,
	eCursor_Herb,
	eCursor_UnableHerb,
	eCursor_Fell,
	eCursor_Unablefell,
	eCursor_Fishing,
	eCursor_UnableFishing,
	eCursor_Shop,
	eCursor_UnableShop,
	eCursor_Locked,
	eCursor_Unlocked,
	eCursor_FurnitureMove,
 	eCursor_FurnitureRorare,
	eCursor_Restore,
	eCursor_Protect,
	eCursor_IgonreLevel,
	eCursor_Pickup,
	eCursor_Hyperlink,
	eCursor_UnableMail,
	eCursor_inspect,
	eCursor_unableinspect,
	eCursor_Egg,

	eCursor_Sit,
	eCursor_UnableSit,

	eCursor_OpenSound,
	eCursor_CloseSound,

	eCursos_Implement,
	eCursos_UnableImplement,

	eCursor_UnableAttack,
	eCursor_UnableRepair,
};