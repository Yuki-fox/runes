#pragma once
#include "../GameMain.h"
#include "MainMenu.h"

#define Register_CBackPackage    \
    g_pLuaScript->RegisterFunc("GetBodyItemName", const char* (int), CBackPackage::GetBodyItemName );	\
    g_pLuaScript->RegisterFunc("ItemFieldUpDate", void (const char*), CBackPackage::OnItemUpDate );     \
    g_pLuaScript->RegisterFunc("ItemFieldDrag"  , void (const char*), CBackPackage::OnItemDrag );       \
    g_pLuaScript->RegisterFunc("ItemFieldDrop"  , void (const char*), CBackPackage::OnItemDrop );       \
    g_pLuaScript->RegisterFunc("BankFieldUpDate", void (const char*), CBackPackage::OnBankUpDate );     \
    g_pLuaScript->RegisterFunc("BankFieldDrag"  , void (const char*), CBackPackage::OnBankDrag );       \
    g_pLuaScript->RegisterFunc("BankFieldDrop"  , void (const char*), CBackPackage::OnBankDrop );       \
    g_pLuaScript->RegisterFunc("EQFieldUpDate"  , void (const char*), CBackPackage::OnEQUpDate );       \
    g_pLuaScript->RegisterFunc("EQFieldDrag"    , void (const char*), CBackPackage::OnEQDrag );         \
    g_pLuaScript->RegisterFunc("EQFieldDrop"    , void (const char*), CBackPackage::OnEQDrop );         \
    g_pLuaScript->RegisterFunc("TradeFieldDrag" , void (const char*), CBackPackage::OnTradeDrag );      \
    g_pLuaScript->RegisterFunc("TradeFieldDrop" , void (const char*), CBackPackage::OnTradeDrop );      \
    g_pLuaScript->RegisterFunc("CheckDragEvent" , bool (void)       , CBackPackage::CheckDragEvent );   \
    g_pLuaScript->RegisterFunc("ZoneID"         , int  (void)       , CBackPackage::ZoneID );           \
    g_pLuaScript->RegisterFunc("UseItem_Body"   , void (const char*), CBackPackage::UseItem_Body );     \
    g_pLuaScript->RegisterFunc("UseItem_Bank"   , void (const char*), CBackPackage::UseItem_Bank );     \
    g_pLuaScript->RegisterFunc("UseItem_EQ"     , void (const char*), CBackPackage::UseItem_EQ );           

// **********************************************************************************************
// Globals
class	CBackPackage;

extern	CBackPackage							*g_pBackPackage;

// **********************************************************************************************

class CBackPackage
{
    //�w�ɳB�z
    static  int         _OnTimeUpdate( SchedularInfo* Obj , void* InputClass );
    //���~
    static  bool        _IsOnItemCmdProc;                //�O�_���b�B�z���~�T��(�V Server�n�D��T)
    
public:
	CBackPackage();
	virtual ~CBackPackage();

	static	void		Initial();
	static	void		Release();	
	//void				Update( float elapsedTime );
    static  void        ClsOnItemCmdProc();
    static  void        ItemDragClsProc( ItemClipboard* ClpInfo  );

    //LUA
    static  void        OnItemUpDate( const char* Name );    
    static  void        OnItemDrag( const char* Name );
    static  void        OnItemDrop( const char* Name );
    static  void        UseItem_Body( const char* Name );
    static  void        UseItem_Bank( const char* Name );
    static  void        UseItem_EQ( const char* Name );

    static  void        OnBankUpDate( const char* Name );    
    static  void        OnBankDrag( const char* Name );
    static  void        OnBankDrop( const char* Name );

    static  void        OnEQUpDate( const char* Name );    
    static  void        OnEQDrag( const char* Name );
    static  void        OnEQDrop( const char* Name );

    static  void        OnTradeDrag( const char* Name );
    static  void        OnTradeDrop( const char* Name );

    static  void        BodyMoneyUpdate( const char* Name );    
	static	int			GetBodyMoney();
	static	const char*	GetBodyItemName( int index );

    //�ˬd�O�_�����ͦ��Ԫ��ƥ�
    static  bool        CheckDragEvent( );


    //-----------------------------------------------------------------------------------------
    //�Ȯ�
    static  int         ZoneID( void ) { return RoleDataEx::G_ZoneID; };
};



