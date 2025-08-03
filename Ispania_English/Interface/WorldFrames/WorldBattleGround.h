#pragma once
#include "../interface.h"
#include <string>
#include <map>
#include <vector>

/*enum WorldBatleGroundENUM
{
	SYS_WORLD_EVENT_NONE = -1,
	SYS_WORLD_EVENT_OPEN_TIME,			//幾點開始
	SYS_WORLD_EVENT_COURSE_TIME,		//多久開一場：
	SYS_WORLD_EVENT_END_TIME,			//多久結算：
	SYS_WORLD_EVENT_PRIZE_TIME,			//獎勵時間：
	SYS_WORLD_EVENT_INFLUENCE_SCORE,	//勢力比
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

	int		m_SettleAccountsTime;	//多久結算
	int		m_RewardTime;			//獎勵時間
	int		m_Rate;					//勢力比
	float	m_ChangeWaitTime;

protected:
	int						m_IsVisible;
};