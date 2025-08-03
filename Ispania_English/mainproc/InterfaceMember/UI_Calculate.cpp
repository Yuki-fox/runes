#include "../GameMain.h"
#include "UI_Calculate.h"

CUI_Calculate							*g_pUI_Calculate = NULL;

// --------------------------------------------------------------------------------------
void CUI_Calculate::Initial()
{
    if ( g_pUI_Calculate == NULL )
    {
        g_pUI_Calculate = new CUI_Calculate;
    }
}

// --------------------------------------------------------------------------------------
void CUI_Calculate::Release()
{
    SAFE_DELETE( g_pUI_Calculate );	
}

CUI_Calculate::CUI_Calculate()
{
    _MaxValue = 9999;
    _Obj = NULL;
    _CBFun = NULL;
}
CUI_Calculate::~CUI_Calculate()
{

}
// --------------------------------------------------------------------------------------

void    CUI_Calculate::Open    ( int MaxValue , void* Obj , Calculate_CallBack CBFun )
{
    _MaxValue = MaxValue;
    _Obj = Obj;
    _CBFun = CBFun;
    _Win->Show();
}

void    CUI_Calculate::Cal_Onload( const char* Name )
{
    g_pUI_Calculate->_Win = (CFrame*)CLayout::GetLayout( Name );
}

void    CUI_Calculate::Cal_OK( int Value )
{
    if( g_pUI_Calculate->_CBFun != NULL )
        g_pUI_Calculate->_CBFun( g_pUI_Calculate->_Obj , Value );

}