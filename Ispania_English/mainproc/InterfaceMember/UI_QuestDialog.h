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
	�ɭ��D�n�γ~:
		a. �q�X NPC Quest ���e�ɤ�
		b. �q�X Quest �M��
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

