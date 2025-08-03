#pragma once

#include "..\..\netwaker_member\Net_Script\NetCli_Script.h"
#include <vector>

struct ScriptEventInfo
{
	ScriptEventInfo()
	{
		bRangeEvent		= false;
		dwCoolDown		= 0;
	};

	bool	bRangeEvent;
	DWORD	dwCoolDown;
};

class CNetScript : public CNetCli_Script
{
public:
	static	bool	Init					();
	static	bool	Release					();

	static	int		OnTimeUpdate			( SchedularInfo* Obj , void* InputClass );
	static	void	OnEvent_CastInterrupted	();
	static	void	OnEvent_CastFinished	();
	//---------------------------------------------------------------
	virtual void	OnTouchPlot				( CRoleSprite* pObj )	{ TouchEvent ( pObj->GetIdentify() ); }
	virtual void	OnRangePlot				( CRoleSprite* pObj )	{ RangeEvent ( pObj->GetIdentify() ); }
	virtual void	OnTouchQuestNPC			( CRoleSprite* pObj );
	virtual void	OnTouchQuestObj			( CRoleSprite* pObj );

	virtual void	OnAdd_Obj				( CRoleSprite* pObj );
	virtual void	OnDel_Obj				( CRoleSprite* pObj );


	virtual void	OnUpdate				();

	virtual void	OnQuestDetail			( int iQuestID , int iMode );

	virtual void	OnQuestAcceptResult		( int iQuestField, int iQuestID, EM_QUESTCHECK_RESULT emResult );
	virtual void	OnQuestCompleteResult	( int iQuestField, int iQuestID, EM_QUESTCHECK_RESULT emResult );
	virtual void	OnQuestDeleteResult		( int iQuestField, int iQuestID, EM_QUESTCHECK_RESULT emResult );

	virtual void	AcceptQuest				( int iQuestID );
	virtual void	CompleteQuest			( int iQuestID );
	
	virtual void	OnKillObj				( CRoleSprite* pObj );


	virtual void	OnQuestFlag				( int iQuestID,	int iValue );

	// 資料
	//---------------------------------------------------------------

	// 觸碰 QuestObj 時處理資訊
	int				m_pTouchObj_ID;
	int				m_pTouchObj_QuestField;

	static CRoleSprite*				m_pLastTouchObj;
	static vector< CRoleSprite* >	m_vecUnProcessRole;
	static vector< CRoleSprite* >	m_vecQuestRole;
	//static vector< CRoleSprite* >	m_vecRangePlotRole;
	static map< CRoleSprite* , ScriptEventInfo* > m_mapRangePlot;

};

