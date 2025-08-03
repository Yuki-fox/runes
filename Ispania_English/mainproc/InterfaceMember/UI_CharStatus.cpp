#include "../GameMain.h"
#include ".\ui_charstatus.h"


// 全域宣告
//-------------------------------------------------------------------
CUI_CharStatus*		g_pUi_CharStatus = NULL;

CFontString*		CUI_CharStatus::m_pFont_Str			= NULL;
CFontString*		CUI_CharStatus::m_pFont_Sta			= NULL;
CFontString*		CUI_CharStatus::m_pFont_Agi			= NULL;

CFontString*		CUI_CharStatus::m_pFont_Mnd			= NULL;
CFontString*		CUI_CharStatus::m_pFont_Int			= NULL;

CFontString*		CUI_CharStatus::m_pFont_Dmg			= NULL;
CFontString*		CUI_CharStatus::m_pFont_Arm			= NULL;
CFontString*		CUI_CharStatus::m_pFont_Abr			= NULL;

CFontString*		CUI_CharStatus::m_pFont_ATK			= NULL;
CFontString*		CUI_CharStatus::m_pFont_DEF			= NULL;
CFontString*		CUI_CharStatus::m_pFont_MATK		= NULL;
CFontString*		CUI_CharStatus::m_pFont_MDEF		= NULL;

CFontString*		CUI_CharStatus::m_pFont_Earth		= NULL;
CFontString*		CUI_CharStatus::m_pFont_Water		= NULL;
CFontString*		CUI_CharStatus::m_pFont_Fire		= NULL;
CFontString*		CUI_CharStatus::m_pFont_Wind		= NULL;
CFontString*		CUI_CharStatus::m_pFont_Light		= NULL;
CFontString*		CUI_CharStatus::m_pFont_Darkness	= NULL;

CFontString*		CUI_CharStatus::m_pFont_ClassTitle1	= NULL;
CFontString*		CUI_CharStatus::m_pFont_ClassLV1	= NULL;
CFontString*		CUI_CharStatus::m_pFont_ClassTitle2	= NULL;
CFontString*		CUI_CharStatus::m_pFont_ClassLV2	= NULL;
//-------------------------------------------------------------------
CUI_CharStatus::CUI_CharStatus(void)
{

}

CUI_CharStatus::~CUI_CharStatus(void)
{
}

void CUI_CharStatus::Initial()
{
	if ( g_pUi_CharStatus == NULL )
	{
		g_pUi_CharStatus = new CUI_CharStatus;

		// Register Func
		//g_pLuaScript->RegisterFunc(	"UI_CharStatus_OnLoad",		void (const char*), UI_CharStatus_OnLoad		);
		//g_pLuaScript->RegisterFunc(	"UI_CharStatus_OnUpdate",	void (void),		UI_CharStatus_OnUpdate	);
		

	}
}

// --------------------------------------------------------------------------------------
void CUI_CharStatus::Release()
{
	SAFE_DELETE( g_pUi_CharStatus );	
}
// --------------------------------------------------------------------------------------
void CUI_CharStatus::UI_CharStatus_OnUpdate()
{
	/*
	CCheckButton*       ThisObj = (CCheckButton*)CLayout::GetLayout( Name );
	CTexture*           Texture;
	RoleDataEx*         Role;
	int                 ID;
	GameObjDbStruct*    ItemDB;
	char                Buf[256];


	if( ThisObj == NULL )
		return;
	ID      = ThisObj->GetID();
	Role    = g_pGameMain->RoleData();
	ItemFieldStruct& Item = Role->GetBodyItem( ID );
	ItemDB = g_pGameMain->GetObjDB( Item.OrgObjID );

	sprintf( Buf , "%sTexture", Name  );
	Texture = (CTexture*)CLayout::GetLayout( Buf );

	if( ItemDB == NULL )
	{
		ThisObj->SetTextA( "" );
		Texture->SetVisible( false );
	}
	else
	{
		Texture->SetVisible( true );
		sprintf( Buf , "%d %s" , Item.Count , ItemDB->Name );
		ThisObj->SetTextA( Buf );
		sprintf( Buf , "interface/icons/%s" , ItemDB->ACTField );
		Texture->SetFilePath( Buf );
	}
	*/
	RoleDataEx*         pRole = g_pGameMain->RoleData();

	char szOut[255];

	// STR 力量
	if( m_pFont_Str )
	{
		sprintf( szOut, "%.f", pRole->TempData.Attr.Fin.STR );
		//sprintf( szOut, "%.f=%.f+%.f", pRole->TempData.Attr.Fin.STR,  pRole->TempData.Attr.Bas.STR + pRole->TempData.Attr.Bas.BaseStr, pRole->TempData.Attr.Mid.STR );
		m_pFont_Str->SetTextA( szOut );
	}

	// STA 耐力
	if( m_pFont_Sta )
	{		
		sprintf( szOut, "%.f", pRole->TempData.Attr.Fin.STA );
		//sprintf( szOut, "%.f=%.f+%.f", pRole->TempData.Attr.Fin.STA,  pRole->TempData.Attr.Bas.STA + pRole->TempData.Attr.Bas.BaseSta, pRole->TempData.Attr.Mid.STA );
		m_pFont_Sta->SetTextA( szOut );
	}

	// AGI 敏捷
	if( m_pFont_Agi )
	{
		sprintf( szOut, "%.f", pRole->TempData.Attr.Fin.AGI );
		//sprintf( szOut, "%.f=%.f+%.f", pRole->TempData.Attr.Fin.AGI,  pRole->TempData.Attr.Bas.AGI+ pRole->TempData.Attr.Bas.BaseAgi, pRole->TempData.Attr.Mid.AGI );
		m_pFont_Agi->SetTextA( szOut );
	}

	// Mnd 精神
	if( m_pFont_Mnd )
	{
		sprintf( szOut, "%.f", pRole->TempData.Attr.Fin.MND );
		//sprintf( szOut, "%.f=%.f+%.f", pRole->TempData.Attr.Fin.MND,  pRole->TempData.Attr.Bas.MND + pRole->TempData.Attr.Bas.BaseMnd, pRole->TempData.Attr.Mid.MND );
		m_pFont_Mnd->SetTextA( szOut );
	}

	// Int 智慧
	if( m_pFont_Int )
	{
		sprintf( szOut, "%.f", pRole->TempData.Attr.Fin.INT );
		//sprintf( szOut, "%.f=%.f+%.f", pRole->TempData.Attr.Fin.INT,  pRole->TempData.Attr.Bas.INT + pRole->TempData.Attr.Bas.BaseInt, pRole->TempData.Attr.Mid.INT );
		m_pFont_Int->SetTextA( szOut );
	}

	// Dmg
	if( m_pFont_Dmg )
	{
		sprintf( szOut, "%.f", pRole->TempData.Attr.Fin.DMG );
		//sprintf( szOut, "%.f=%.f+%.f", pRole->TempData.Attr.Fin.DMG,  pRole->TempData.Attr.Bas.DMG, pRole->TempData.Attr.Mid.DMG );
		m_pFont_Dmg->SetTextA( szOut );
	}

	// Armor
	if( m_pFont_Arm )
	{
		sprintf( szOut, "%.f", pRole->TempData.Attr.Fin.DEF );
		//sprintf( szOut, "%.f=%.f+%.f", pRole->TempData.Attr.Fin.DEF,  pRole->TempData.Attr.Bas.DEF, pRole->TempData.Attr.Mid.DEF );
		m_pFont_Arm->SetTextA( szOut );
	}

	// Abr
	if( m_pFont_Abr )
	{
		sprintf( szOut, "%.f", pRole->TempData.Attr.Fin.Abr );
		//sprintf( szOut, "%.f=%.f+%.f", pRole->TempData.Attr.Fin.Abr,  pRole->TempData.Attr.Bas.Abr, pRole->TempData.Attr.Mid.Abr );
		m_pFont_Abr->SetTextA( szOut );
	}

	// ATK
	if( m_pFont_ATK )
	{
		sprintf( szOut, "%.f", pRole->TempData.Attr.Fin.ATK );
		//sprintf( szOut, "%.f=%.f+%.f", pRole->TempData.Attr.Fin.Abr,  pRole->TempData.Attr.Bas.Abr, pRole->TempData.Attr.Mid.Abr );
		m_pFont_ATK->SetTextA( szOut );
	}

	// DEF
	if( m_pFont_DEF )
	{
		sprintf( szOut, "%.f", pRole->TempData.Attr.Fin.DEF );
		//sprintf( szOut, "%.f=%.f+%.f", pRole->TempData.Attr.Fin.Abr,  pRole->TempData.Attr.Bas.Abr, pRole->TempData.Attr.Mid.Abr );
		m_pFont_DEF->SetTextA( szOut );
	}

	// MATK
	if( m_pFont_MATK )
	{
		sprintf( szOut, "%.f", pRole->TempData.Attr.Fin.MATK );
		//sprintf( szOut, "%.f=%.f+%.f", pRole->TempData.Attr.Fin.Abr,  pRole->TempData.Attr.Bas.Abr, pRole->TempData.Attr.Mid.Abr );
		m_pFont_MATK->SetTextA( szOut );
	}

	// MDEF
	if( m_pFont_MDEF )
	{
		sprintf( szOut, "%.f", pRole->TempData.Attr.Fin.MDEF );
		//sprintf( szOut, "%.f=%.f+%.f", pRole->TempData.Attr.Fin.Abr,  pRole->TempData.Attr.Bas.Abr, pRole->TempData.Attr.Mid.Abr );
		m_pFont_MDEF->SetTextA( szOut );
	}


	// LV
	if( m_pFont_ClassLV1 )
	{
		sprintf( szOut, "%d", pRole->TempData.Attr.Level );
		m_pFont_ClassLV1->SetTextA( szOut );
	}
}
//-------------------------------------------------------------------
void CUI_CharStatus::UI_CharStatus_OnLoad( const char* cszName )
{
	char		szObjName[255];
	CLayout*	pObj	= NULL;


	
	m_pFont_ClassTitle1	= (CFontString*)CLayout::GetLayout( "UI_CharStatus_ClassTitle1" );
	m_pFont_ClassLV1	= (CFontString*)CLayout::GetLayout( "UI_CharStatus_ClassLevel1" );

	// 取得 Value 字串資料
	for( int index = 1; index <=8; index++ )
	{
		sprintf( szObjName, "%s_Value%d_Value", cszName, index );
		pObj = CLayout::GetLayout( szObjName );

		if( pObj )
		{
			switch( index ) 
			{
			case 1:	m_pFont_Str = (CFontString*)pObj;	break;
			case 2: m_pFont_Sta = (CFontString*)pObj;	break;
			case 3: m_pFont_Agi = (CFontString*)pObj;	break;
			case 4:	m_pFont_Mnd = (CFontString*)pObj;	break;
			case 5: m_pFont_Int = (CFontString*)pObj;	break;

			case 6: m_pFont_Dmg = (CFontString*)pObj;	break;
			case 7: m_pFont_Arm = (CFontString*)pObj;	break;
			case 8: m_pFont_Abr = (CFontString*)pObj;	break;
			
			case 9:	m_pFont_ATK = (CFontString*)pObj;	break;
			case 10: m_pFont_DEF = (CFontString*)pObj;	break;
			case 11: m_pFont_MATK = (CFontString*)pObj;	break;
			case 12: m_pFont_MDEF = (CFontString*)pObj;	break;

			case 13: m_pFont_Earth = (CFontString*)pObj;	break;
			case 14: m_pFont_Water = (CFontString*)pObj;	break;
			case 15: m_pFont_Fire = (CFontString*)pObj;	break;
			case 16: m_pFont_Wind = (CFontString*)pObj;	break;
			case 17: m_pFont_Light = (CFontString*)pObj;	break;
			case 18: m_pFont_Darkness = (CFontString*)pObj;	break;


			

			}
		}
	}
}