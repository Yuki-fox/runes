#ifndef _CHATFRAME_H
#define _CHATFRAME_H

#include <map>
#include <string>

#define Register_CChatFrame    \
   g_pLuaScript->RegisterFunc( "SendChatMessage", void ( const char* message, int type ), CChatFrame::SendChatMessage ); 


// **********************************************************************************************
// Globals
class	CChatFrame;

extern	CChatFrame						*g_pChatFrame;

// **********************************************************************************************
enum {
	eChatType_NULL				= 0,
	eChatType_Whisper,
	eChatType_Range,
	eChatType_Zone,
    eChatType_GMCommand,
	eChatType_System
};

// -----------------------------------------------------------------------------
typedef void(*com_func)( const char* );

// -----------------------------------------------------------------------------
struct CommandInfo
{
	com_func func;
	std::string	explain;
};

// -----------------------------------------------------------------------------
class CSplitArgString
{
public:
	CSplitArgString();
	CSplitArgString( const char* str );
	~CSplitArgString();

	void						SetString( const char* str );
	const char* 				GetString( int start, int end = -1 );
    int                         GetArgSize() { return m_endIndex.size();}
protected:
	char						m_tmp[256];
	char*						m_str;
	std::vector<int>			m_startIndex;
	std::vector<int>			m_endIndex;

};

class CChatFrame
{
public:
	CChatFrame();
	~CChatFrame();

	static	void				Initial();
	static	void				Release();
	static	void				RegisterCmd( const char* cmdStr, const char* explain, com_func func );

	static	void				SendChatMessage( const char* message, int type );
	static	void				ParseText( const char* text );
	static	void				SetChatType( int type );
	static	void				SetChatContent( const char* text );
	static	void				SetTellTarget( const char* text );	

	// command
	static	void				TalkToWhisper( const char* str );
	static	void				TalkToZone( const char* str );
    static	void				GMCommand( const char* str );
	static	void				Help( const char* str );
    static  void                GMMsg( const char* str );

    // system
    static	void				TitleVisible( const char* str );
    static	void				SpriteUpdate( const char* str );
    static	void				SpriteRender( const char* str );
    static	void				SetMoveSpeed( const char* str );

    static	void				ShowFps( const char* str );

    //#########################################################################################
    //測試指令
    //#########################################################################################
    //交易測試指令
    static  void                UI_OpenTrade( const char* str );
    static  void                Trade( const char* str );
    static  void                AgreeTrade( const char* str );


	// Quest 測試
	static	void				Touch( const char* str );				

    //開啟 NewObj 介面
    static	void				NewObj( const char* str );

    //使用物品
    static  void                UseItem_Body( const char* str );
    
    //配點測試
    static  void                SetPoint( const char* str );	
    //法術配點測試
    static  void                SetMagicPoint( const char* str );	

    //施法測試
    static  void                SpellMagic( const char* str );	

    //商店測試
    static  void                CloseShop( const char* str );	    //關閉商店測試
    static  void                BuyItem( const char* str );	        //買物品測試
    static  void                SelectSellItem( const char* str );	//選擇賣出物品測試
    static  void                SellAll( const char* str );         //確定賣出物品測試
    static  void                ClsSell( const char* str );	        //清除賣出物品測試
    static  void                RepairWeapon( const char* str );	//要求修理物品測試

    //清除重要物品與劇情
    static  void                ClsQuest( const char* str );
    //#########################################################################################
	

    

	
	static	void				AddMessage( int channel, int color, const char* format, ... );

protected:
	static	void				Translator( const char* src, char* dest );

protected:
	static	std::map<std::string, CommandInfo>	s_command;
	static	int					s_type;
	static	char				s_content[256];
	static	char				s_tellTarget[256];
};

#endif //_CHATFRAME_H