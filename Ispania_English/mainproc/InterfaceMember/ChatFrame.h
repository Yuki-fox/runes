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
    //���ի��O
    //#########################################################################################
    //������ի��O
    static  void                UI_OpenTrade( const char* str );
    static  void                Trade( const char* str );
    static  void                AgreeTrade( const char* str );


	// Quest ����
	static	void				Touch( const char* str );				

    //�}�� NewObj ����
    static	void				NewObj( const char* str );

    //�ϥΪ��~
    static  void                UseItem_Body( const char* str );
    
    //�t�I����
    static  void                SetPoint( const char* str );	
    //�k�N�t�I����
    static  void                SetMagicPoint( const char* str );	

    //�I�k����
    static  void                SpellMagic( const char* str );	

    //�ө�����
    static  void                CloseShop( const char* str );	    //�����ө�����
    static  void                BuyItem( const char* str );	        //�R���~����
    static  void                SelectSellItem( const char* str );	//��ܽ�X���~����
    static  void                SellAll( const char* str );         //�T�w��X���~����
    static  void                ClsSell( const char* str );	        //�M����X���~����
    static  void                RepairWeapon( const char* str );	//�n�D�ײz���~����

    //�M�����n���~�P�@��
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