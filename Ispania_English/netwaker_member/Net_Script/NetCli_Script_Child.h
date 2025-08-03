#pragma once

#include "NetCli_Script.h"
#include <vector>
#include <set>

struct ScriptEventInfo
{
	ScriptEventInfo()
	{
		bRangeEvent		= false;
	};

	bool	bRangeEvent;
};


class CNetCli_Script_Child : public CNetCli_Script
{
public:
	static	bool	Init						();
	static	bool	Release						();

	static	int		OnTimeUpdate				( SchedularInfo* Obj , void* InputClass );
	static	int		OnMinimapQuestObjTimeUpdate ( SchedularInfo* Obj , void* InputClass );
	static	void	OnModifyScriptState			( CRoleSprite* pObj );
	

	static	void	AddQuestlink				( int iQuestID );
	static	void	DelQuestlink				( int iQuestID );
	//static	void	OnEvent_CastInterrupted	();
	//static	void	OnEvent_CastFinished	();
	//---------------------------------------------------------------
	//virtual void		OnTouchPlot				( CRoleSprite* pObj )	{ TouchEvent ( pObj->GetIdentify() ); }
	//virtual void		OnRangePlot				( CRoleSprite* pObj )	{ RangeEvent ( pObj->GetIdentify() ); }

	
		// QuestList 被點下, 有 iRoleID 表 Client 點下 Quest NPC
		// 若為 iRoleID == 0 表 Client 打開 Quest 清單

	virtual	void		InitColorString					();

	virtual void		OnTouchQuestNPC					( int iRoleID );
	virtual void		OnTouchQuestObj					( int iObjID  );

	virtual void		OnAddObj						( int iObjID );
	virtual void		OnDelObj						( int iObjID );

	virtual void		OnEnterTrigger					( int iObjID	);
	virtual void		OnLeaveTrigger					( int iObjID	);

	//virtual void		OnModifyObj						( int iObjID );

	//virtual void	OnTouchQuestNPC			( CRoleSprite* pObj );
	//virtual void	OnTouchQuestObj			( CRoleSprite* pObj );

	//virtual void	OnAdd_Obj				( CRoleSprite* pObj );
	//virtual void	OnDel_Obj				( CRoleSprite* pObj );


	//virtual void	OnUpdate				();

	//virtual void	OnQuestDetail			( int iQuestID , int iMode );

	virtual void		OnQuestAcceptResult				( int iQuestField, int iQuestID, EM_QUESTCHECK_RESULT emResult );
	virtual void		OnQuestCompleteResult			( int iQuestNpcID, int iQuestField, int iQuestID, EM_QUESTCHECK_RESULT emResult );
	virtual void		OnQuestDeleteResult				( int iQuestField, int iQuestID, EM_QUESTCHECK_RESULT emResult );
	virtual void		OnClickQuestObjResult			( int iResult, int iItemID, int iItemVal );
	virtual void		OnKillQuestObjNotify			( int iQuestField, int iQuestID, int iDeadObjID );

	//virtual void	AcceptQuest				( int iQuestID );
	//virtual void	CompleteQuest			( int iQuestID );
	
	virtual void		OnKillObj						( int iRoleID );


	virtual void		OnQuestFlag						( int iQuestID,	int iValue );

	virtual void		OnPlayMotion					( int iObjID, const char* pszMotion );
	virtual void		OnSpeakOption					( int iCliID , int iSize , PVOID pData );
	virtual void		OnChoiceOptionResult			();
	virtual void		OnCloseSpeak					();
	virtual	void		OnPlaySound						( int iObjID, const char* pszSoundFile, bool bLoop, int iSoundID );
	virtual	void		OnPlay3DSound					( int iObjID, const char* pszSoundFile, bool bLoop, int iSoundID );
	virtual	void		OnPlayMusic						( int iObjID, const char* pszSoundFile, bool bLoop, int iSoundID );

	virtual	void		OnStopSound						( int iObjID, int iSoundID );

	virtual void		OnQuestState					( int iQuestID, int iState );

	static int			CheckQuestObj					( int iQuestObjID );

	virtual	int			GetQuestModePriority			( int iMode );


	// Script Border
	//--------------------------------------------------------------------
	virtual void		OnClearBorder					();
	virtual void		OnAddBorderPage					( const char* pszAddPage );
	virtual	void		OnShowBorder					( int iQuestID, const char* pszTitle, const char* pszTexture );


	// Script Cast Time Bar
	//--------------------------------------------------------------------
	virtual void		OnScriptCast				( const char* pszString, int iTime, int iMotionID, int iMotionEndID );
	virtual	void		OnScriptCastResult			( int iResult );

	virtual	void		OnAttachCastMotionToolID	( int iObjID, int iToolID );
	virtual	void		OnAttachTool				( int iObjID, int iToolID, const char* pszLink );
	virtual	void		OnStartClientCountDown		( int iStartNumber, int iEndNumber, int iOffset, int iMode, int iShowStatusBar, const char* str );


	// Minimap and Questlink
	//--------------------------------------------------------------------
	static	bool		CheckQuestlink				( GameObjDbStructEx* pObj );
	static	void		ResetQuestLink				( int iQuestID );


	virtual void		OnResetDailyQuest			( int iType );


	// Script language
	//--------------------------------------------------------------------
	static	string		ReplaceSystemKeyword		( string Source, bool bQuestColor = false, bool bHyperlink = false, bool bNoloopCheckVar = false );
		// bNoloopCheckVar 主要是任務模版中的只跑一次, 而 Hyper link 中有使用 [%s] 做為 KEYWORD, 故使用 bNoloopCheckVar 就不重覆執行

	virtual void		OnSetCameraFollowUnit		( int iObjID, int iOffX, int iOffY, int iOffZ );


	// 資料
	//---------------------------------------------------------------

	// 觸碰 QuestObj 時處理資訊
	//int				m_pTouchObj_ID;
	int				m_pTouchObj_QuestField;
	int				m_iClickQuestObjID;						// 點擊任務物品使用


	//static CRoleSprite*				m_pLastTouchObj;
	static int						m_iEnterCollisionObjID;
	static CRuVector3				m_LastCollisionPos;
	static int						m_iLastTouchObjID;
	static vector< CRoleSprite* >	m_vecUnProcessRole;
	static vector< CRoleSprite* >	m_vecQuestRole;
	//static vector< CRoleSprite* >	m_vecRangePlotObj;		// 範圍劇情 NPC
	static set< int >				m_mapRangePlotObj;		// 範圍劇情 NPC
	//static vector< CRoleSprite* >	m_vecRangePlotRole;
	//static map< CRoleSprite* , ScriptEventInfo* > m_mapRangePlot;

	static int						m_iScriptCastMotionEnd;
	static int						m_iScriptCastAttachToolID;


	string							m_SpeakDetail;

	static string					m_szColor_NPC;
	static string					m_szColor_Item;
	static string					m_szColor_Zone;
	static string					m_szQuest_Complete;
	static string					m_szQuest_UnComplete;
	static string					m_szQuest_Title;
	static string					m_szQuest_NoTitle;

	static string					m_pSource;
	static bool						m_bQuestColor;
	static bool						m_bHyperLink;
	static bool						m_bNoloopCheckVar;
	static string					m_lastResult;

};

