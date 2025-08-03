#pragma once

#include <vector>
#include "../../netwaker_member/Net_BattleGround/NetCli_BattleGround_Child.h"
#include "../interface.h"

#define MAX_BG_TEAM_CLASSIFY_NUM 9

using namespace std;

class CBattleGroundStatusFrame;
extern CBattleGroundStatusFrame*	g_pBattleGroundStatusFrame;

class CBattleGroundStatusFrame : public CInterfaceBase
{
protected:
	vector<StructBattleGroundScore> m_playerScores;
	vector<StructBattleGroundScore> m_scoreClassify;
	bool                            m_Visible;
	float							m_updateTimer;
	int                             m_curSortFunc;
	int                             m_curClassify;

	int								m_teamPlayerNum[MAX_BG_TEAM_CLASSIFY_NUM];

	int								m_WinTeam;

public:
	CBattleGroundStatusFrame(CInterface* object);
	virtual ~CBattleGroundStatusFrame();

	virtual	void	RegisterFunc();
	virtual	void	FrameOnShow();
	virtual	void	FrameOnHide();
	virtual	void	Update(float elapsedTime);
	virtual void	ClassifyScore(int iClassify);
	virtual void	ChangeSortFunc(int iSort);
	virtual void	UpdateScore(int iSrvID, int iTotalRole, StructBattleGroundScore* pScoreArray);	
	virtual	void	BattleGroundClose(int iWinTeam);
	virtual int     GetTeamPlayerNum(int iTeamID){return m_teamPlayerNum[iTeamID];}
	virtual int		GetPlayerScoreInfoNum();
	virtual	int		GetWinTeam() {return m_WinTeam;}

	virtual StructBattleGroundScore* GetPlayerScore(int iIndex);
};