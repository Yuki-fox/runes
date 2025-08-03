#pragma once

#include <map>
#include <string>
#include <Windows.h>


#include <string>
#include "../../mainproc/RuneDev.h"			// for CRoleSprite*

#include "PG/PG_Gather.h"
#include "../../MainProc/NetGlobal.h"

class CNetCli_Gather : public CNetGlobal
{
public:
	CNetCli_Gather(void);
	~CNetCli_Gather(void);

	// 封包註冊
	//-----------------------------------------------------------
	static void _PG_Gather_LtoC_ClickGatherObj_Result	( int iCliID , int iSize , PVOID pData );
	static void _PG_Gather_LtoC_GatherMotion			( int iCliID , int iSize , PVOID pData );
	/*
	static void _PG_GatherScript_LtoC_QuestAccept_Result		( int iCliID , int iSize , PVOID pData );    
	static void _PG_Script_LtoC_QuestComplete_Result	( int iCliID , int iSize , PVOID pData ); 
	static void _PG_Script_LtoC_QuestDelete_Result		( int iCliID , int iSize , PVOID pData ); 
	static void	_PG_Script_LtoC_QuestFlag				( int iCliID , int iSize , PVOID pData );
	static void	_PG_Script_LtoC_ClickQuestObjResult		( int iCliID , int iSize , PVOID pData );
	static void	_PG_Script_LtoC_QuestKillObjNotify		( int iCliID , int iSize , PVOID pData );
	static void	_PG_Script_LtoC_PlayMotion				( int iCliID , int iSize , PVOID pData );
	static void	_PG_Script_LtoC_SpeakOption				( int iCliID , int iSize , PVOID pData );
	static void	_PG_Script_LtoC_ChoiceOptionResult		( int iCliID , int iSize , PVOID pData );
	static void	_PG_Script_LtoC_CloseSpeak				( int iCliID , int iSize , PVOID pData );
	*/

	//-----------------------------------------------------------
	static	bool			Init						();
	static	bool			Release						();

	// 由外部呼叫的動作
	//-----------------------------------------------------------
	static void			OnEvent_TouchGatherObj			( int iRoleID )		{ if( m_pThis ) m_pThis->OnTouchGatherObj	( iRoleID );	}

	// 給繼承物件處理用事件
	//---------------------------------------------------------------
	virtual void		OnTouchGatherObj				( int iRoleID	) = 0;	

	// 採集
	//---------------------------------------------------------------
	void				ClickGatherObj					( int iObjID );
	virtual void		OnClickGatherObjResult			( int iResult, int iItemID, int iItemVal ) = 0;
	static	void		CancelGather					( int iObjID );
	//---------------------------------------------------------------

	virtual	void		OnGatherMotion					( int iGatherPlayerID, int iMotionID, int iMineType ) = 0;

	// 變數
	static	CNetCli_Gather*		m_pThis;
};

