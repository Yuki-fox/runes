#pragma once
#include "../GameMain.h"
#include "MainMenu.h"
using namespace std;

#define Register_UI_ShortCutFrame \
    g_pLuaScript->RegisterFunc("OnShortCutUpDate", void (const char* , int ) , CUI_ShortCutFrame::OnShortCutUpDate );  \
    g_pLuaScript->RegisterFunc("OnShortCutClick" , void (const char* , int ) , CUI_ShortCutFrame::OnClick );           \
    g_pLuaScript->RegisterFunc("OnShortCutDrop"  , void (const char* , int ) , CUI_ShortCutFrame::OnMouseDropItem );  \
    g_pLuaScript->RegisterFunc("OnShortCutDrag"  , void (const char* , int ) , CUI_ShortCutFrame::OnMouseDragItem );     


// **********************************************************************************************
// Globals
class	CUI_ShortCutFrame;

extern	CUI_ShortCutFrame							*g_pUI_ShortCutFrame;

// **********************************************************************************************
/*
enum	ShortCutTypeENUM
{
    EM_ShortCut_Null		= 0  ,
    EM_ShortCut_Magic		= 1  ,
    EM_ShortCut_Macro		= 2  ,
    EM_ShortCut_Item		= 3  ,
    EM_ShortCut_BankItem    = 4	 ,
    EM_ShortCut_EQ          = 5	 ,
    EM_ShortCut_FORCE_DWORD	= 0x7fffffff, 
};
*/
// **********************************************************************************************
//快捷鍵原型
struct UIShortCutBaseStruct
{

    DragDropTypeEnum	Type;

    string		Title;		    //頭銜
    string      ImgFile;        //圖形

    string		Macro;		    //指令

    int			OrgID;			//物件ID or 法術ID
    int			Version;		//物品序號
    int			Pos;			//對應欄位

    UIShortCutBaseStruct()
    {
        Clear();
    }
    void		Clear()
    {
        OrgID = 0;
        Version = 0;
        Pos = 0;
        Type = EM_DragDropType_Null;
    }
    bool        IsEmpty()
    {
        return (Type == EM_DragDropType_Null);
    }
};
// **********************************************************************************************

class CUI_ShortCutFrame
{

	static	UIShortCutBaseStruct		m_Base[10][10];			//快捷機本資料

public:
	CUI_ShortCutFrame();
	virtual ~CUI_ShortCutFrame();

	static	void		Initial();
	static	void		Release();	

    static  void        OnShortCutUpDate( const char* Name , int Page );
    static  void        OnClick( const char* Name , int Page );

    static  void        OnMouseUpItem( const char* Name , int Page );
    static  void        OnMouseDownItem( const char* Name , int Page );
    static  void        OnMouseDragItem( const char* Name , int Page );
    static  void        OnMouseDropItem( const char* Name , int Page );

    static  void        SetMacro( int Page ,  int Pos , const char* Title , const char* ImgFile , const char* Macro );
    static  void        SetItem( int Page , int Pos , int OrgID , int Version );
    static  void        SetMacroBase( int Page , int Pos , UIShortCutBaseStruct* Info );

};



