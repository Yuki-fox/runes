#pragma once

#include "../GameMain.h"

#define Register_UI_CharStatus    \
	g_pLuaScript->RegisterFunc(	"UI_CharStatus_OnLoad",		void (const char*), CUI_CharStatus::UI_CharStatus_OnLoad		);\
	g_pLuaScript->RegisterFunc(	"UI_CharStatus_OnUpdate",	void (), CUI_CharStatus::UI_CharStatus_OnUpdate		);\


// Globals
//-------------------------------------------------------------------
class	CUI_CharStatus;
extern	CUI_CharStatus							*g_pUi_CharStatus;
//-------------------------------------------------------------------

class CUI_CharStatus
{
public:
	CUI_CharStatus(void);
	~CUI_CharStatus(void);

	static void				Initial						();
	static void				Release						();	
	
	static void				UI_CharStatus_OnLoad		( const char* cszName );
	static void				UI_CharStatus_OnUpdate		();
	
	static CFontString*		m_pFont_Str;
	static CFontString*		m_pFont_Sta;
	static CFontString*		m_pFont_Agi;
	static CFontString*		m_pFont_Mnd;
	static CFontString*		m_pFont_Int;


	static CFontString*		m_pFont_Dmg;
	static CFontString*		m_pFont_Arm;
	static CFontString*		m_pFont_Abr;
	static CFontString*		m_pFont_ATK;
	static CFontString*		m_pFont_DEF;
	static CFontString*		m_pFont_MATK;
	static CFontString*		m_pFont_MDEF;

	static CFontString*		m_pFont_Earth;
	static CFontString*		m_pFont_Water;
	static CFontString*		m_pFont_Fire;
	static CFontString*		m_pFont_Wind;
	static CFontString*		m_pFont_Light;
	static CFontString*		m_pFont_Darkness;


	static CFontString*		m_pFont_ClassTitle1;
	static CFontString*		m_pFont_ClassLV1;
	static CFontString*		m_pFont_ClassTitle2;
	static CFontString*		m_pFont_ClassLV2;
};
