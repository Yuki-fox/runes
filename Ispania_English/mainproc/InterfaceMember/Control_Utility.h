#pragma once

#include "../GameMain.h"

#include <map>


// ���ʤ���, �i�H�]�w�����}�Y NPC �S�w�Z����o�X�ƥ�
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

// Casting Time Bar �����, ��]�w���ɭ���, �Y���⥻���ʧ@����, �h�o�X�ƥ�
class CCastingTimeBar_Controler
{
public:
	CCastingTimeBar_Controler(void){};
	~CCastingTimeBar_Controler(void){};

	virtual void	StartMode1				( CRoleSprite* pObj );
		// ���p��P�ؼЪ��Z��, �u�n�����ʧ@����, �Y�o�X�ƥ�

	
	virtual void	OnEvent_Interrupted		( CCastingTimeBar_Controler* pUI )		{};
	virtual void	OnEvent_Finished		( CCastingTimeBar_Controler* pUI )		{};

	static	int		OnTimeUpdate			();

	static	PF_EVENT_INTERRUPTED	m_pfInterrupted;
	static	PF_EVENT_FINISHED		m_pfFinished;
};