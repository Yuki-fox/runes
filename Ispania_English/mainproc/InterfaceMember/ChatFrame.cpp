#include <string.h>
#include "..\GameMain.h"
#include "WorldFrame.h"
#include "UI_Trade.h"
#include "UI_MaintainFrame.h"
#include "ChatFrame.h"

#include "../netmember/NetScript.h"


using namespace std;

void SetCameraPos( const char* str );
void MoveCameraPos( const char* str );
void SetCameraFarPlane( const char* str );
void Glow( const char* str );
void SetFog( const char* str );
void OpenOptionsFrame( const char* str );

// --------------------------------------------------------------------------------------
// Globals
CChatFrame*						g_pChatFrame = NULL;

// --------------------------------------------------------------------------------------
// static
int									CChatFrame::s_type;
char								CChatFrame::s_content[256];
char								CChatFrame::s_tellTarget[256];
std::map<std::string, CommandInfo>	CChatFrame::s_command;

// --------------------------------------------------------------------------------------
CSplitArgString::CSplitArgString()
{
	m_str = NULL;
}

// --------------------------------------------------------------------------------------
CSplitArgString::CSplitArgString( const char* str )
{
	m_str = NULL;
	SetString( str );
}

// --------------------------------------------------------------------------------------
CSplitArgString::~CSplitArgString()
{
	SAFE_DELETE( m_str );
}
	
// --------------------------------------------------------------------------------------
void CSplitArgString::SetString( const char* str )
{
	if ( str == NULL )
		return;

	m_startIndex.clear();
	m_endIndex.clear();
	m_startIndex.push_back( 0 );

	char* start = (char*)str;
	char* end;
	while ( end = strchr(start, ' ') )
	{
		m_endIndex.push_back( end - str );
		start = end;
		while ( *start == ' ' ) start++;
		m_startIndex.push_back( start - str );
	}

	if ( m_startIndex.size() != m_endIndex.size() )
		m_endIndex.push_back( strlen(str) );

	SAFE_DELETE( m_str );
	m_str = new char[strlen(str) + 1];
	strcpy( m_str, str );
}

// --------------------------------------------------------------------------------------
const char* CSplitArgString::GetString( int start, int end )
{
	if ( m_str == NULL )
		return NULL;
	
	if ( start < 0 || start >= (int)m_startIndex.size() )
		return NULL;

	memset( m_tmp, 0, sizeof(m_tmp) );
	char *first = m_str + m_startIndex[start];
	if ( end == -1 )
		strcpy( m_tmp, first );
	else
		strncpy( m_tmp, first, m_endIndex[end] - m_startIndex[start] );

	return m_tmp;
}

// --------------------------------------------------------------------------------------
CChatFrame::CChatFrame()
{
}

// --------------------------------------------------------------------------------------
CChatFrame::~CChatFrame()
{
}

// --------------------------------------------------------------------------------------
void CChatFrame::Initial()
{
	if ( g_pChatFrame == NULL )
	{
		g_pChatFrame = new CChatFrame;

		// Register Function
		RegisterCmd( "help", NULL, Help );
		RegisterCmd( "t", "talk to player", TalkToWhisper );
		RegisterCmd( "zone", "talk to zone", TalkToZone );
        RegisterCmd( "?", "GM command", GMCommand );
        RegisterCmd( "GM", "回報訊息給GM tools", GMMsg );

        //交易測試
        RegisterCmd( "opentrade", NULL, UI_OpenTrade );
        RegisterCmd( "trade", NULL, Trade );
        RegisterCmd( "agreetrade", NULL, AgreeTrade );


		RegisterCmd( "title", "{on or off} object title", TitleVisible );
		RegisterCmd( "sprite_update", "{on or off}", SpriteUpdate );
		RegisterCmd( "sprite_render", "{on or off}", SpriteRender );

		RegisterCmd( "Touch", NULL, Touch );
		
        RegisterCmd( "newobj", NULL, NewObj );

        RegisterCmd( "useitem", NULL, UseItem_Body );

        //測試商店
        RegisterCmd( "closeshop", NULL, CloseShop );
        RegisterCmd( "buyitem", NULL, BuyItem );
        RegisterCmd( "selectsellitem", NULL, SelectSellItem );
        RegisterCmd( "sellall", NULL, SellAll );
        RegisterCmd( "clssell", NULL, ClsSell );
        RegisterCmd( "repairweapon", NULL, RepairWeapon );

        //清除任務
        RegisterCmd( "clsquest", "清除", ClsQuest );

        //設定點術策試
        RegisterCmd( "setpoint", "{ str or sta or int ...}" , SetPoint );
        RegisterCmd( "setmagicpoint", "{ MagicID }" , SetMagicPoint );

        RegisterCmd( "spellmagic", NULL, SpellMagic );
		RegisterCmd( "showfps", "{on or off}", ShowFps );
		RegisterCmd( "camera_set", "{x, y, z}", SetCameraPos );
		RegisterCmd( "camera_move", "{x, y, z}", MoveCameraPos );
		RegisterCmd( "camera_far_plane", "{far(int)}", SetCameraFarPlane );
		RegisterCmd( "setfog", "{min(int) max(int)}", SetFog );
		RegisterCmd( "glow", "{on or off}", Glow );
		RegisterCmd( "options", "open options frame", OpenOptionsFrame );
        
	}
}

// --------------------------------------------------------------------------------------
void CChatFrame::Release()
{
	SAFE_DELETE( g_pChatFrame );
}

// --------------------------------------------------------------------------------------
void CChatFrame::RegisterCmd( const char* cmdStr, const char* explain, com_func func )
{
	if ( cmdStr == NULL || func == NULL )
		return;
	
	string tmp( cmdStr );
	strlwr( (char*)tmp.c_str() );

	CommandInfo info;
	if ( explain )
		info.explain = explain;
	info.func = func;

	s_command.insert( make_pair( tmp, info ) );
}

// --------------------------------------------------------------------------------------
void CChatFrame::SendChatMessage( const char* message, int type )
{
	static char tmp[1024];
	if ( message == NULL || message[0] == 0 )
		return;

	type = 2;
	SetChatType( type );

	Translator( message, tmp );
	SetChatContent( tmp );

	ParseText( tmp );
	switch ( s_type )
	{
	case eChatType_Whisper:
		NetCli_Talk::TalkTo( s_tellTarget, s_content );
		break;

	case eChatType_Range:
		NetCli_Talk::Range( s_content );
		break;

	case eChatType_Zone:
		NetCli_Talk::Zone( s_content );
		break;

    case eChatType_GMCommand:
        NetCli_Talk::GMCommand( s_content );
        break;

	case eChatType_System:
		break;
	}
}

// --------------------------------------------------------------------------------------
void CChatFrame::ParseText( const char* text )
{
	CSplitArgString arg;
	if ( text == NULL || text[0] != '/' )
		return;

	arg.SetString( text + 1 );
	string str = arg.GetString(0 , 0);
	strlwr( (char*)str.c_str() );

	map<string, CommandInfo>::iterator iter = s_command.find( str );
	if ( iter == s_command.end() )
		return;

//    SetChatType( eChatType_System );
	(iter->second).func( arg.GetString(1) );
	
}

// --------------------------------------------------------------------------------------
void CChatFrame::SetChatType( int type )
{
	s_type = type;
}

// --------------------------------------------------------------------------------------
void CChatFrame::SetChatContent( const char* text )
{
    if( text == NULL )
        text = "";
	strcpy( s_content, text );
}

// --------------------------------------------------------------------------------------
void CChatFrame::SetTellTarget( const char* text )
{
	strcpy( s_tellTarget, text );
}

// --------------------------------------------------------------------------------------
void CChatFrame::AddMessage( int channel, int color, const char* format, ... )
{
	static char buf[1024];

	va_list args;
	va_start( args, format );
	vsprintf( buf, format, args );
	va_end( args);

	CMessageFrame* pMessageFrame = (CMessageFrame*)CLayout::GetLayout( "ChatFrameMessage" );
	if ( pMessageFrame )
		pMessageFrame->AddMessageA( buf, color );
}

// --------------------------------------------------------------------------------------
void CChatFrame::Translator( const char* src, char* dest )
{
	while ( *src )
	{
		if ( *src > 0x80 )
		{
			*dest++ = *src++;
			*dest++ = *src++;
		}
		else if ( *src == '%' )
		{
			if ( strnicmp(src + 1, "t_id", 4) == 0 )
			{
				CRoleSprite* player = g_pWorldFrame->GetTargetPlayer();
				src += 5;

				if ( player )
				{
					char tmp[16];
					sprintf( tmp, "%d", player->GetIdentify() );

					int len = strlen(tmp);
					memcpy( dest, tmp, len );
					dest += len;
				}
			}
			else if ( strnicmp(src + 1, "s_id", 4) == 0 )
			{
				CRoleSprite* player = g_pWorldFrame->GetPlayer();
				src += 5;

				if ( player )
				{
					char tmp[16];
					sprintf( tmp, "%d", player->GetIdentify() );

					int len = strlen(tmp);
					memcpy( dest, tmp, len );
					dest += len;
				}
			}
			else if ( strnicmp(src + 1, "t", 1) == 0 )
			{
				CRoleSprite* player = g_pWorldFrame->GetTargetPlayer();
				src += 2;

				if ( player && player->GetName() )
				{
					int len = strlen( player->GetName() );
					memcpy( dest, player->GetName(), len );
					dest += len;
				}
			}
			else if ( strnicmp(src + 1, "s", 1) == 0 )
			{
				CRoleSprite* player = g_pWorldFrame->GetPlayer();
				src += 2;

				if ( player && player->GetName() )
				{
					int len = strlen( player->GetName() );
					memcpy( dest, player->GetName(), len );
					dest += len;
				}
			}			
			else if ( strnicmp(src + 1, "%", 1) == 0 )
			{
				src += 2;
				*dest++ = '%';
			}
			else
				*dest++ = *src++;
		}
		else
			*dest++ = *src++;
	}
	*dest = 0;
}

// --------------------------------------------------------------------------------------
void CChatFrame::TalkToWhisper( const char* str )
{
	CSplitArgString arg(str);
	SetChatType( eChatType_Whisper );
	SetTellTarget( arg.GetString( 0, 0) );
	SetChatContent( arg.GetString( 1 ) );
}

// --------------------------------------------------------------------------------------
void CChatFrame::TalkToZone( const char* str )
{
	CSplitArgString arg(str);
	SetChatType( eChatType_Zone );
	SetChatContent( arg.GetString(0) );
}

// --------------------------------------------------------------------------------------
void CChatFrame::GMCommand( const char* str )
{
	CSplitArgString arg(str);
	SetChatType( eChatType_GMCommand );
	SetChatContent( arg.GetString(0) );
}

// --------------------------------------------------------------------------------------
void CChatFrame::Help( const char* str )
{	
	for ( map<string, CommandInfo>::iterator iter = s_command.begin(); iter != s_command.end(); iter++ )
		AddMessage( 0, DEF_COLOR_GREEN, "[%s]:%s", (iter->first).c_str(), (iter->second).explain.c_str() );
}
// --------------------------------------------------------------------------------------
void   CChatFrame::UI_OpenTrade( const char* str )
{
    CUI_Trade::OpenTradeMenu( 0 );
}
// --------------------------------------------------------------------------------------
void   CChatFrame::Trade( const char* str )
{
    CSplitArgString arg(str);
    CUI_Trade::TradeRequest(  atol( arg.GetString(0) ) , -1 );
    //NetCli_Trade::S_RequestTrade( atol( arg.GetString(0) ) );
}
void   CChatFrame::AgreeTrade( const char* str )
{
    CSplitArgString arg(str);
    NetCli_Trade::S_AgreeTrade( atol( arg.GetString(0) ) );
}

// --------------------------------------------------------------------------------------
void CChatFrame::TitleVisible( const char* str )
{
	if ( str )
	{
		if ( strcmp( str, "on" ) == 0 )
			g_pGameMain->ShowRoleSpriteTitle();
		else if ( strcmp( str, "off" ) == 0 )
			g_pGameMain->HideRoleSpriteTitle();
	}
}

// --------------------------------------------------------------------------------------
void CChatFrame::SpriteUpdate( const char* str )
{
	if ( str )
	{
		if ( strcmp( str, "on" ) == 0 )
			g_pGameMain->SetSpriteEnable( true );
		else if ( strcmp( str, "off" ) == 0 )
			g_pGameMain->SetSpriteEnable( false );
	}
}

// --------------------------------------------------------------------------------------
void CChatFrame::SpriteRender( const char* str )
{
	if ( str )
	{
		if ( strcmp( str, "on" ) == 0 )
			g_pGameMain->SetSpriteVisible( true );
		else if ( strcmp( str, "off" ) == 0 )
			g_pGameMain->SetSpriteVisible( false );
	}	
}

// --------------------------------------------------------------------------------------
void CChatFrame::SetMoveSpeed( const char* str )
{
}

// --------------------------------------------------------------------------------------
void CChatFrame::Touch( const char* str )
{
	CSplitArgString arg(str);

	CNetCli_Script::OnEvent_TouchQuestNPC( NULL );
//	CNetCli_Script::OnEvent_Touch( NULL );
	//CNetScript::OnTouch( NULL );
	

}
// --------------------------------------------------------------------------------------
void CChatFrame::NewObj( const char* str )
{
    g_pUI_MaintainFrame->Open();   
}

//使用物品
void CChatFrame::UseItem_Body( const char* str )
{
    CSplitArgString arg(str);
    ItemFieldStruct* Item;
    int         Pos;
    int         OrgID;
    int         TargetID;
    int         Version;
    RoleDataEx* Role    = g_pGameMain->RoleData();   

    switch( arg.GetArgSize() )
    {
    case 1:
        TargetID = -1;
        Version = 0;
        OrgID = atol( arg.GetString( 0 ) );
        break;
    case 2:
        TargetID = -1;
        Version = atol( arg.GetString( 1 ) );
        OrgID = atol( arg.GetString( 0 ) );
        break;
    case 3:
        TargetID = atol( arg.GetString( 0 ) );
        Version = atol( arg.GetString( 2 ) );
        OrgID = atol( arg.GetString( 1 ) );
        break;
    default:
        return;
    }
    Pos = Role->GetBodyItemPos( OrgID , Version );
    if( Pos == -1 )
        return;

    Item = Role->GetBodyItem( Pos );
    if( Item->IsEmpty() )
        return;

    if( Item->Pos != EM_ItemState_NONE )
        return;

    NetCli_Item::_UseItem_Body( *Item ,  Pos , TargetID );

    Item->Pos = EM_ItemState_CliDisabled;
    Role->TempData.UpdateInfo.Body = true;
}
// --------------------------------------------------------------------------------------
void CChatFrame::SpellMagic( const char* str )
{
    CSplitArgString arg(str);
    if( arg.GetArgSize() != 3 )
    {
        g_pChatFrame->AddMessage( 0, DEF_COLOR_WHITE, "參數錯誤");
        return ;
    }
    int TargetID = atol( arg.GetString( 0 ) );
    int MagicID  = atol( arg.GetString( 1 ) );
    int MagicPos = atol( arg.GetString( 2 ) );
    CRoleSprite* Player = g_pWorldFrame->GetPlayer();

    NetCli_Magic::BeginSpell( Player->GetIdentify() , TargetID , MagicID , MagicPos );
}
// --------------------------------------------------------------------------------------
void CChatFrame::ShowFps( const char* str )
{
	if ( str )
	{
		if ( strcmp( str, "on" ) == 0 )
			g_pGameMain->SetShowFps( true );
		else if ( strcmp( str, "off" ) == 0 )
			g_pGameMain->SetShowFps( false );
	}
}
// --------------------------------------------------------------------------------------
void CChatFrame::SetPoint( const char* str )
{
    if( str == NULL )
    {
        g_pChatFrame->AddMessage( 0, DEF_COLOR_WHITE, "參數錯誤");
        return ;
    }

    CSplitArgString arg(str);
    if( stricmp( arg.GetString(0) , "Str" ) == 0 )
    {
        NetCli_RoleValue::S_RequestSetPoint( EM_RoleValue_STR );
    }
    else if( stricmp( arg.GetString(0) , "Sta" ) == 0 )
    {
        NetCli_RoleValue::S_RequestSetPoint( EM_RoleValue_STA );
    }
    else if( stricmp( arg.GetString(0) , "Mnd" ) == 0 )
    {
        NetCli_RoleValue::S_RequestSetPoint( EM_RoleValue_MND );
    }
    else if( stricmp( arg.GetString(0) , "Int" ) == 0 )
    {
        NetCli_RoleValue::S_RequestSetPoint( EM_RoleValue_INT );
    }
    else if( stricmp( arg.GetString(0) , "Agi" ) == 0 )
    {
        NetCli_RoleValue::S_RequestSetPoint( EM_RoleValue_AGI );
    }
}
//法術配點測試
void CChatFrame::SetMagicPoint( const char* str )
{
    if( str == NULL )
    {
        g_pChatFrame->AddMessage( 0, DEF_COLOR_WHITE, "參數錯誤");
        return ;
    }

    int i;

    CSplitArgString arg( str );
    int MagicID = atol( arg.GetString(0) );
    int MagicPos = -1;
    RoleDataEx* Role    = g_pGameMain->RoleData();   

    for( i = 0 ; i < _MAX_NormalSkill_COUNT_ ; i++ )
    {
        if( Role->BaseData.Skill.MainJob[i].SkillID == MagicID )
        {
            MagicPos = i;
            break;
        }
    }
    if( MagicPos == -1 )
    {
        g_pChatFrame->AddMessage( 0, DEF_COLOR_WHITE, "找不到此法術");
        return;
    }
    NetCli_Magic::SetMagicPoint( MagicID , MagicPos );
}
// --------------------------------------------------------------------------------------
void CChatFrame::GMMsg( const char* str )
{
    NetCli_Talk::GMMsg( str );
}
// --------------------------------------------------------------------------------------
void SetCameraPos( const char* str )
{
	CSplitArgString arg(str);
	CRuVector3 pos(0.0f, 0.0f, 0.0f);

	for ( int i = 0; i < arg.GetArgSize(); i++ )
	{
		switch ( i )
		{
		case 0:
			pos.m_x = (float)atof(arg.GetString(i, i));
			break;
		case 1:
			pos.m_y = (float)atof(arg.GetString(i, i));
			break;
		case 2:
			pos.m_z = (float)atof(arg.GetString(i, i));
			break;
		}
	}

	g_pGameMain->SetCameraPos( pos );
	g_pChatFrame->AddMessage( 0, DEF_COLOR_PURPLE, "Set camera position (%.2f, %.2f, %.2f)", pos.m_x, pos.m_y, pos.m_z );
}
// --------------------------------------------------------------------------------------
void MoveCameraPos( const char* str )
{
	CSplitArgString arg(str);
	CRuVector3 pos(0.0f, 0.0f, 0.0f);	

	for ( int i = 0; i < arg.GetArgSize(); i++ )
	{
		switch ( i )
		{
		case 0:
			pos.m_x = (float)atof(arg.GetString(i, i));
			break;
		case 1:
			pos.m_y = (float)atof(arg.GetString(i, i));
			break;
		case 2:
			pos.m_z = (float)atof(arg.GetString(i, i));
			break;
		}
	}
	g_pGameMain->MoveCameraPos( pos );
	g_pChatFrame->AddMessage( 0, DEF_COLOR_PURPLE, "Move camera position (%.2f, %.2f, %.2f)", pos.m_x, pos.m_y, pos.m_z );
}
// --------------------------------------------------------------------------------------
void SetCameraFarPlane( const char* str )
{
	if ( str )
	{
		CSplitArgString arg(str);
		float value = (float)atof(arg.GetString(0, 0));
		g_pGameMain->SetCameraFarPlane( value );
		g_pChatFrame->AddMessage( 0, DEF_COLOR_PURPLE, "Set camera far plane is %.2f", value );
	}
}
// --------------------------------------------------------------------------------------
void Glow( const char* str )
{
	if ( str )
	{
		if ( strcmp( str, "on" ) == 0 )
			g_pGameMain->SetGlow( true );
		else if ( strcmp( str, "off" ) == 0 )
			g_pGameMain->SetGlow( false );
	}
	else
	{		
		if ( g_pGameMain->IsGlow() )
			g_pChatFrame->AddMessage( 0, DEF_COLOR_PURPLE, "Glow %s", "on" );
		else
			g_pChatFrame->AddMessage( 0, DEF_COLOR_PURPLE, "Glow %s", "off" );
	}
}
// --------------------------------------------------------------------------------------
void SetFog( const char* str )
{	
	float fmin = 500;
	float fmax = 800;
	if ( str )
	{
		CSplitArgString arg(str);
		for ( int i = 0; i < arg.GetArgSize(); i++ )
		{
			switch (i)
			{
			case 0:
				fmin = (float)atof(arg.GetString(i, i));
				break;

			case 1:
				fmax = (float)atof(arg.GetString(i, i));
				break;
			}
		}
	}
	g_pGameMain->SetFogDistance( fmin, fmax );
	g_pChatFrame->AddMessage( 0, DEF_COLOR_PURPLE, "Set Fog (%.2f - %.2f)", fmin, fmax );
}
// --------------------------------------------------------------------------------------
void OpenOptionsFrame( const char* str )
{
	CFrame* pFrame = (CFrame*)CLayout::GetLayout( "OptionsFrame" );
	if ( pFrame )
		pFrame->Show();
}
// --------------------------------------------------------------------------------------
//商店測試
void  CChatFrame::CloseShop( const char* str )	    //關閉商店測試
{
    //CSplitArgString arg(str);
    NetCli_Shop::S_ShopCloseNotify();
}
void  CChatFrame::BuyItem( const char* str )	    //買物品測試
{
    if( str == NULL )
        return;

    CSplitArgString arg(str);
    NetCli_Shop::S_BuyItemRequest( atol(arg.GetString( 0 ) ) , atol(arg.GetString( 1 ) ) );
}
void  CChatFrame::SelectSellItem( const char* str )	//選擇賣出物品測試
{
    if( str == NULL )
        return;
    CSplitArgString arg(str);
    RoleDataEx* Role    = g_pGameMain->RoleData();   
    int ItemPos = atol(arg.GetString(0) );

    ItemFieldStruct* Item = Role->GetBodyItem( ItemPos );
    if( Item == NULL )
        return;

    if( Item->Pos == EM_ItemState_NONE )
    {
        NetCli_Shop::S_SelectSellItemRequest( ItemPos , Item , true );
    }
    else if( Item->Pos == EM_USE_SELSELL )
    {
        NetCli_Shop::S_SelectSellItemRequest( ItemPos , Item , false );
    }

}
void  CChatFrame::SellAll( const char* str )        //確定賣出物品測試
{
    NetCli_Shop::S_SellA11ItemRequest();
}
void  CChatFrame::ClsSell( const char* str )	    //清除賣出物品測試
{
    NetCli_Shop::S_ClsSellA11Item();
}
void  CChatFrame::RepairWeapon( const char* str )	//要求修理物品測試
{
    if( str == NULL )
        return;
    CSplitArgString arg(str);
    RoleDataEx* Role    = g_pGameMain->RoleData();   
    int ItemPos = atol(arg.GetString(0) );

    ItemFieldStruct* Item = Role->GetBodyItem( ItemPos );
    if( Item == NULL )
        return;

    NetCli_Shop::S_RepairWeaponRequest( ItemPos , false );
}

//清除重要物品與劇情
void  CChatFrame::ClsQuest( const char* str )
{
    RoleDataEx* Role    = g_pGameMain->RoleData();   
    memset( &Role->BaseData.Quest , 0 , sizeof(Role->BaseData.Quest) );
    memset( &Role->BaseData.KeyItem , 0 , sizeof( Role->BaseData.KeyItem ) );
    NetCli_Talk::GMCommand( "ClsQuest" );
}

// --------------------------------------------------------------------------------------
