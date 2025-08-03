#pragma once
#include "../interface.h"
#include <string>
#include <map>
#include <vector>

/*enum WorldBatleGroundENUM
{
	SYS_WORLD_EVENT_NONE = -1,
	SYS_WORLD_EVENT_OPEN_TIME,			//�X�I�}�l
	SYS_WORLD_EVENT_COURSE_TIME,		//�h�[�}�@���G
	SYS_WORLD_EVENT_END_TIME,			//�h�[����G
	SYS_WORLD_EVENT_PRIZE_TIME,			//���y�ɶ��G
	SYS_WORLD_EVENT_INFLUENCE_SCORE,	//�դO��
};   */
//******************************************************************************
class CWorldBattleGround;
extern CWorldBattleGround*	g_pWorldBattleGround;

using namespace std;

class CWorldBattleGround : public CInterfaceBase
{
public:
	CWorldBattleGround( CInterface* pObject );
	virtual				~CWorldBattleGround();

	virtual	void			RegisterFunc();	
	virtual	void			WBGOpen();	
	virtual	void			WBGClose();	
	virtual void			R_GetWorldVarResult(const char* pszVarName, int iVarValue);
	virtual void			Update(float elapsedTime);
	virtual	void			SetWBGChangTime(){ m_ChangeWaitTime = 30 ;}

	int		m_SettleAccountsTime;	//�h�[����
	int		m_RewardTime;			//���y�ɶ�
	int		m_Rate;					//�դO��
	float	m_ChangeWaitTime;

protected:
	int						m_IsVisible;
};