#pragma once

#include "../GameMain.h"

#include <map>


// 互動介面, 可以設定當離開某 NPC 特定距離後發出事件
class CControlInteractUI
{
public:
	CControlInteractUI(void){};
	~CControlInteractUI(void){};

	static	void	SetInteractRangeOn		( CControlInteractUI* pUI, CRoleSprite* pRoleObj );
	static	void	SetInteractRangeOff		( CControlInteractUI* pUI, CRoleSprite* pRoleObj );

	static	int		OnTimeUpdate			( SchedularInfo* Obj , void* InputClass );

	virtual	void	OnLeave_InteractRange	( CControlInteractUI* pUI, CRoleSprite* pRoleObj ) = 0;

	static map< CRoleSprite* , CControlInteractUI* >	m_mapInteractRange;
	static bool											m_IsUpdate;

};

typedef void (*PF_EVENT_INTERRUPTED)	(void);
typedef void (*PF_EVENT_FINISHED)		(void);

// Casting Time Bar 控制介面, 當設定此界面後, 若角色本身動作改變, 則發出事件
class CCastingTimeBar_Controler
{
public:
	CCastingTimeBar_Controler(void){};
	~CCastingTimeBar_Controler(void){};

	virtual void	StartMode1				( CRoleSprite* pObj );
		// 不計算與目標的距離, 只要本身動作改變, 即發出事件

	
	virtual void	OnEvent_Interrupted		( CCastingTimeBar_Controler* pUI )		{};
	virtual void	OnEvent_Finished		( CCastingTimeBar_Controler* pUI )		{};

	static	int		OnTimeUpdate			();

	static	PF_EVENT_INTERRUPTED	m_pfInterrupted;
	static	PF_EVENT_FINISHED		m_pfFinished;
};