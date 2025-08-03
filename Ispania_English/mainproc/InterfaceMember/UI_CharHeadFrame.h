#pragma once

#include "../GameMain.h"

#define Register_UI_CharHeadFrame    \
	g_pLuaScript->RegisterFunc(	"OnLoad_CharHeadFrame",		void (),	CUI_CharHeadFrame::OnLoad_CharHeadFrame		);\
	g_pLuaScript->RegisterFunc(	"OnLoad_TargetHeadFrame",	void (),	CUI_CharHeadFrame::OnLoad_TargetHeadFrame	);\

// Globals
//-------------------------------------------------------------------
class	CUI_CharHeadFrame;
extern	CUI_CharHeadFrame							*g_pUi_CharHeadFrame;
//-------------------------------------------------------------------

struct _HEADFREAMINFO
{
	CFontString*		m_pName;

	CFontString*		m_pLV;
	/*
	CFontString*		m_pLV;
	CFontString*		m_pHP;
	CFontString*		m_pMP;
	CFontString*		m_pSP;	
	*/

	CStatusBar*			m_pHP_Bar;
	CStatusBar*			m_pMP_Bar;
	CStatusBar*			m_pSP_Bar;
	CLayout*			m_pFrame;
};


class CUI_CharHeadFrame
{
public:
	CUI_CharHeadFrame(void);
	~CUI_CharHeadFrame(void);

	static void				Initial						();
	static void				Release						();	

	static int				OnTimeUpdate				( SchedularInfo* Obj , void* InputClass );	
		// 定時處理
	
	static void				OnLoad_CharHeadFrame		();
	static void				OnLoad_TargetHeadFrame		();	
	
	static void				Update						();

	/*
	static CFontString*		m_pFont_RoleName;
	static CFontString*		m_pFont_RoleLV;
	static CFontString*		m_pFont_RoleHP;
	static CFontString*		m_pFont_RoleMP;

	static CFontString*		m_pFont_TargetName;
	static CFontString*		m_pFont_TargetLV;
	static CFontString*		m_pFont_TargetHP;
	static CFontString*		m_pFont_TargetMP;

	static CStatusBar*		m_pBar_TargetHP;
	static CStatusBar*		m_pBar_TargetMP;

	static CLayout*			m_pTargetFrame;		// 目標狀態欄
	static CLayout*			m_pCharFrame;		// 人物狀態欄
	*/

	static CRoleSprite*		m_pTargetRole;		// 目標人物

	static _HEADFREAMINFO	m_RoleInfo;
	static _HEADFREAMINFO	m_TargetRoleInfo;

};
