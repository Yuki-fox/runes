#pragma once
#include "../GameMain.h"
#include "MainMenu.h"

#define Register_UI_MaintainFrame \
        g_pLuaScript->RegisterFunc( "OnLoadMaintainFrame"           , void ( )  , CUI_MaintainFrame::OnLoad );\
        g_pLuaScript->RegisterFunc( "OnAcceptClickMaintainFrame"    , void ( )  , CUI_MaintainFrame::OnAcceptClick );\
        g_pLuaScript->RegisterFunc( "OnResetPosMaintainFrame"       , void ( )  , CUI_MaintainFrame::OnResetPos );\
        g_pLuaScript->RegisterFunc( "OnUpdateMaintainFrame"         , void ( )  , CUI_MaintainFrame::OnUpdate );\
        g_pLuaScript->RegisterFunc( "CreateMacroMaintainFrame"      , void ( )  , CUI_MaintainFrame::OnCreateMacro );

// **********************************************************************************************
// Globals
class	CUI_MaintainFrame;

extern	CUI_MaintainFrame							*g_pUI_MaintainFrame;

// **********************************************************************************************
class CUI_MaintainFrame
{
    static  CFrame*             _UI_Win;
    static  CFontString*        _UI_Title;
    static  CEditBox*           _UI_ObjID;
    static  CEditBox*           _UI_OrgName;
    static  CEditBox*           _UI_GItemID;
    static  CEditBox*           _UI_Name;
    static  CEditBox*           _UI_ObjClass;
    static  CEditBox*           _UI_PID;
    static  CEditBox*           _UI_AutoPlot;
    static  CEditBox*           _UI_QuestID;
    static  CEditBox*           _UI_PosX;
    static  CEditBox*           _UI_PosY;
    static  CEditBox*           _UI_PosZ;
    static  CEditBox*           _UI_PosDir;
    static  CButton*            _UI_PosReset;
    static  CCheckButton*       _UI_CheckMode[32];
    static  CEditBox*           _UI_MacroName;
    static  CButton*            _UI_MacroCreate;
    static  CFontString*        _UI_Macro;
    static  CButton*            _UI_Accept;
    static  CButton*            _UI_Cencel;

    static  int                 _GItemID;


public:
	CUI_MaintainFrame();
	virtual ~CUI_MaintainFrame();

	static	void		Initial();
	static	void		Release();	

    static  bool        IsVisible() { return (_UI_Win !=NULL && _UI_Win->IsVisible() ); } ;

    static  void        OnLoad( );

    static  bool        Open( );
    static  bool        ModifyOpen( ModifyNPCInfo*  Info );
    static  bool        Hide( );

    static  void        OnCreateMacro( );
    static  void        OnAcceptClick( );
    static  void        OnResetPos( );
    static  void        OnUpdate( );

    static  void        SetModifyInfo( ModifyNPCInfo* Info );
    static  void        GetModifyInfo( ModifyNPCInfo* Info );

};



