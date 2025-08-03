#pragma once

#include "../GameMain.h"

#include <map>
#include <string>

/*
#define Register_UILua_Quest    \
	g_pLuaScript->RegisterFunc(	"UI_CharStatus_OnLoad",		void (const char*), CUI_CharStatus::UI_CharStatus_OnLoad		);\
	g_pLuaScript->RegisterFunc(	"UI_CharStatus_OnUpdate",	void (), CUI_CharStatus::UI_CharStatus_OnUpdate		);\
*/

/*
	界面主要用途:
		a. 秀出 NPC Quest 先前導引
		b. 秀出 Quest 清單
*/

// Globals
//-------------------------------------------------------------------
class	CUI_QuestDialog;
extern	CUI_QuestDialog							*g_pUi_QuestDialog;
//-------------------------------------------------------------------

class CUI_QuestDialog
{
public:
	CUI_QuestDialog(void);
	~CUI_QuestDialog(void);

	static void				Initial						();
	static void				Release						();


};

