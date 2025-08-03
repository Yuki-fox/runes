#pragma once
#include "../GameMain.h"
#include "MainMenu.h"

#define Register_UI_Calculate \
    g_pLuaScript->RegisterFunc( "Cal_GetMaxValue"   , int  ( void )         , CUI_Calculate::Cal_GetMaxValue );\
    g_pLuaScript->RegisterFunc( "Cal_Onload"        , void ( const char*  ) , CUI_Calculate::Cal_Onload );\
    g_pLuaScript->RegisterFunc( "Cal_OK"            , void ( int )          , CUI_Calculate::Cal_OK);


typedef void    (*Calculate_CallBack )( void* Obj , int Value );
// **********************************************************************************************
// Globals
class	CUI_Calculate;

extern	CUI_Calculate							*g_pUI_Calculate;

// **********************************************************************************************
class CUI_Calculate
{
    CFrame*             _Win;
    int                 _MaxValue;
    void*               _Obj;
    Calculate_CallBack  _CBFun;
public:
	CUI_Calculate();
	virtual ~CUI_Calculate();

	static	void		Initial();
	static	void		Release();	

    void    Open    ( int MaxValue , void* Obj , Calculate_CallBack CBFun );


    //-------------------------------------------------------------------------------
    //LUA
    static  int     Cal_GetMaxValue( ) { return g_pUI_Calculate->_MaxValue; };
    
    static  void    Cal_Onload( const char* Str );

    static  void    Cal_OK( int Value );

};



