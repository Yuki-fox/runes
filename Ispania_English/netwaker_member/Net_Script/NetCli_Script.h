#pragma once

#include <map>
#include <string>
#include <Windows.h>


#include <string>
#include "../../mainproc/RuneDev.h"			// for CRoleSprite*

#include "PG/PG_Script.h"
#include "../../MainProc/NetGlobal.h"

class CNetCli_Script : public CNetGlobal
{
public:
	CNetCli_Script(void);
	~CNetCli_Script(void);

	// 封包註冊
	//-----------------------------------------------------------
	static void _PG_Script_LtoC_QuestAccept_Result		( int iCliID , int iSize , PVOID pData );    
	static void _PG_Script_LtoC_QuestComplete_Result	( int iCliID , int iSize , PVOID pData ); 
	static void _PG_Script_LtoC_QuestDelete_Result		( int iCliID , int iSize , PVOID pData ); 
	static void	_PG_Script_LtoC_QuestFlag				( int iCliID , int iSize , PVOID pData );
	static void	_PG_Script_LtoC_ClickQuestObjResult		( int iCliID , int iSize , PVOID pData );
	static void	_PG_Script_LtoC_KillQuestObjNotify		( int iCliID , int iSize , PVOID pData );
	static void	_PG_Script_LtoC_PlayMotion				( int iCliID , int iSize , PVOID pData );
	static void	_PG_Script_LtoC_SpeakOption				( int iCliID , int iSize , PVOID pData );
	static void	_PG_Script_LtoC_ChoiceOptionResult		( int iCliID , int iSize , PVOID pData );
	static void	_PG_Script_LtoC_CloseSpeak				( int iCliID , int iSize , PVOID pData );
	
	static void	_PG_Script_LtoC_PlaySound				( int iCliID , int iSize , PVOID pData );
	static void	_PG_Script_LtoC_PlayMusic				( int iCliID , int iSize , PVOID pData );
	static void	_PG_Script_LtoC_Play3DSound				( int iCliID , int iSize , PVOID pData );
	
	
	static void	_PG_Script_LtoC_SetQuestState			( int iCliID , int iSize , PVOID pData );

	static void	_PG_Script_LtoC_ClearBorder				( int iCliID , int iSize , PVOID pData );
	static void	_PG_Script_LtoC_AddBorderPage			( int iCliID , int iSize , PVOID pData );
	static void	_PG_Script_LtoC_ShowBorder				( int iCliID , int iSize , PVOID pData );

	static void	_PG_Script_LtoC_ScriptCast				( int iCliID , int iSize , PVOID pData );
	static void	_PG_Script_LtoC_ScriptCastResult		( int iCliID , int iSize , PVOID pData );
	static void	_PG_Script_LtoC_ResetDailyQuest			( int iCliID , int iSize , PVOID pData );

	static void	_PG_Script_LtoC_AttachCastMotionToolID	( int iCliID , int iSize , PVOID pData );
	static void	_PG_Script_LtoC_StartClientCountDown	( int iCliID , int iSize , PVOID pData );
	static void	_PG_Script_LtoC_AttachTool				( int iCliID , int iSize , PVOID pData );

	static void _PG_Script_LtoC_SetCameraFollowUnit		( int iCliID , int iSize , PVOID pData );
	static void _PG_Script_LtoC_StopSound				( int iCliID , int iSize , PVOID pData );

	//-----------------------------------------------------------
	static bool			Init				();
	static bool			Release				();
	static void			Update				();

	// 由外部呼叫的動作
	//-----------------------------------------------------------
	static void			OnEvent_AddObj					( int iObjID )		{ if( m_pThis ) m_pThis->OnAddObj( iObjID );	}
	static void			OnEvent_DelObj					( int iObjID )		{ if( m_pThis ) m_pThis->OnDelObj( iObjID );	}
	static void			OnEvent_KillObj					( int iObjID )		{ if( m_pThis ) m_pThis->OnKillObj( iObjID );	}


	//static void			OnEvent_TouchQuestNPC			( int iRoleID )		{ ifm_pThis->OnTouchQuestNPC( iRoleID );	}
	static void			OnEvent_TouchQuestNPC			( int iRoleID )		{ if( m_pThis ) m_pThis->OnTouchQuestNPC	( iRoleID );	}
	static void			OnEvent_TouchQuestObj			( int iObjID )		{ if( m_pThis ) m_pThis->OnTouchQuestObj	( iObjID );		}
	static void			OnEvent_TouchPlot				( int iRoleID )		{ TouchEvent( iRoleID ); }
	static void			OnEvent_RangePlot				( int iRoleID )		{ RangeEvent( iRoleID ); }
	static void			OnEvent_CollisionTrigger		( int iObjID )		{ if( m_pThis ) m_pThis->OnEnterTrigger( iObjID ); }
	static void			OnEvent_CollisionOutTrigger		( int iObjID )		{ if( m_pThis ) m_pThis->OnLeaveTrigger( iObjID ); }


	/*
	static void			OnEvent_TouchQuestNPC			( CRoleSprite* pObj ) {	m_pThis->OnTouchQuestNPC ( pObj );	}
	static void			OnEvent_TouchQuestObj			( CRoleSprite* pObj ) {	m_pThis->OnTouchQuestObj ( pObj );	}
	static void			OnEvent_TouchPlot				( CRoleSprite* pObj ) { m_pThis->OnTouchPlot ( pObj );	}
	static void			OnEvent_RangePlot				( CRoleSprite* pObj ) { m_pThis->OnRangePlot ( pObj );	}
	*/
	

	//static void			OnEvent_QuestDetail			( int iQuestID , int bComplete ) { m_pThis->OnQuestDetail( iQuestID, bComplete ); }
	//static void			OnEvent_AcceptQuest			( int iQuestID ) { m_pThis->OnAcceptQuest( iQuestID ); }
	//static void			OnEvent_AcceptQuest			( int iQuestID ) { m_pThis->OnAcceptQuest( iQuestID ); }

	// 給繼承物件處理用事件
	//---------------------------------------------------------------
	virtual void		OnAddObj						( int iRoleID	) = 0;
	virtual void		OnDelObj						( int iRoleID	) = 0;
	virtual void		OnKillObj						( int iRoleID	) = 0;

	virtual void		OnTouchQuestNPC					( int iRoleID	) = 0;
	virtual void		OnTouchQuestObj					( int iObjID	) = 0;
	virtual void		OnEnterTrigger					( int iObjID	) = 0;
	virtual void		OnLeaveTrigger					( int iObjID	) = 0;

	/*
	virtual void		OnTouchQuestNPC		( CRoleSprite* pObj ) = 0;
	virtual void		OnTouchQuestObj		( CRoleSprite* pObj ) = 0;
	virtual void		OnTouchPlot			( CRoleSprite* pObj ) = 0;
	virtual void		OnRangePlot			( CRoleSprite* pObj ) = 0;

	virtual void		OnUpdate			() = 0;
	*/
	//virtual void		OnQuestDetail		( int iQuestID , bool bComplete ) = 0;
	//virtual void		OnAcceptQuest		( int iQuestID ) = 0;

	virtual void		OnPlayMotion					( int iObjID, const char* pszMotion ) = 0;
	virtual void		OnSpeakOption					( int iCliID , int iSize , PVOID pData ) = 0;
	virtual	void		OnPlaySound						( int iObjID, const char* pszSoundFile, bool bLoop, int iSoundID ) = 0;
	virtual	void		OnPlay3DSound					( int iObjID, const char* pszSoundFile, bool bLoop, int iSoundID ) = 0;
	virtual	void		OnPlayMusic						( int iObjID, const char* pszSoundFile, bool bLoop, int iSoundID ) = 0;
	virtual	void		OnStopSound						( int iObjID, int iSoundID ) = 0;
	virtual void		OnQuestState					( int iQuestID, int iState ) = 0;
	
	// Script Border
	virtual void		OnClearBorder					() = 0;
	virtual void		OnAddBorderPage					( const char* pszAddPage ) = 0;
	virtual	void		OnShowBorder					( int iQuestID, const char* pszTitle, const char* pszTexture ) = 0;



	// 資料
	//---------------------------------------------------------------
	static CNetCli_Script*		m_pThis;

	
	// 發送封包
	//---------------------------------------------------------------
	
	static void			TouchEvent					( int iObjID );
	static void			RangeEvent					( int iObjID );
	static void			CollisionEvent				( int iObjID );


	static void			AcceptQuest					( int iQuestNpcID, int iQuestID );		
		virtual void		OnQuestAcceptResult		( int iQuestField, int iQuestID, EM_QUESTCHECK_RESULT emResult ) = 0;

	static void			CompleteQuest				( int iQuestNpcID, int iQuestID, int iChoiceID );		
		virtual void		OnQuestCompleteResult	( int iQuestNpcID, int iQuestField, int iQuestID, EM_QUESTCHECK_RESULT emResult ) = 0;

	
	static void			DeleteQuest					( int iQuestField, int iQuestID );
		virtual void		OnQuestDeleteResult		( int iQuestField, int iQuestID, EM_QUESTCHECK_RESULT emResult ) = 0;
	
	static void			ClickQuestObj				( int iQuestField, int iQuestID, int iClickObjID, int iClickObj );
		virtual void		OnClickQuestObjResult	( int iResult, int iItemID, int iItemVal ) = 0;

	// Zone Server 送來的通知
	virtual void		OnKillQuestObjNotify		( int iQuestField, int iQuestID, int iDeadObjID ) = 0;
	//virtual void		OnGetQuestObjNotify			( int iQuestField, int iQuestID, int iDeadObjID ) = 0;

	static void			LoadSrvOption				( int iNPCID );

	static void			ChoiceOption				( int iOptionID );
		virtual	void		OnChoiceOptionResult	() = 0;
		virtual void		OnCloseSpeak			() = 0;

	static void			DelayPatrol					( int iNPCID, int iTime );
	// GM 命令
	//---------------------------------------------------------------
	virtual void		OnQuestFlag					( int iQuestID,	int iValue ) = 0;

	//---------------------------------------------------------------
	static bool			m_bSendingOption;


	// 石碑介面
	//---------------------------------------------------------------
	static	void		CloseBorder					();			// 當 Client 關畢 Border 時通知 Server 清除資料
	static	void		AcceptBorderQuest			();			// 當 Client 點下石碑介面接受任務時通知 Server


	// 劇情施法條
	virtual	void		OnScriptCast				( const char* pszString, int iTime, int iMotionID, int iMotionEndID ) = 0;
	virtual	void		OnScriptCastResult			( int iResult ) = 0;

	virtual void		OnResetDailyQuest			( int iType ) = 0;

	virtual	void		OnAttachCastMotionToolID	( int iObjID, int iToolID ) = 0;
	virtual	void		OnAttachTool				( int iObjID, int iToolID, const char* pszLink ) = 0;
	virtual	void		OnStartClientCountDown		( int iStartNumber, int iEndNumber, int iOffset, int iMode, int iShowStatusBar, const char* str ) = 0;

	virtual void		OnSetCameraFollowUnit		( int iObjID, int iOffX, int iOffY, int iOffZ ) = 0;

	//map< DWORD, StructQuestInfo* >		m_mapQuestInfo;
	//map< DWORD, StructQuestNPC* >		m_mapQuestNPC;
};

