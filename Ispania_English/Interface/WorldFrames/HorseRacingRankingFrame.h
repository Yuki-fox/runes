#pragma once

#include "../../netwaker_member/Net_BattleGround/NetCli_BattleGround_Child.h"
#include "../interface.h"

using namespace std;

class CHorseRacingRankingFrame;
extern CHorseRacingRankingFrame*	g_pHorseRacingRankingFrame;

struct StructHorseRacingRankingData
{
	int		iTeamID;
	int     iRank;
	int		iTime;
	int     iInGame;
	char	szName[32];
};


class CHorseRacingRankingFrame : public CInterfaceBase
{
public:
	vector<StructHorseRacingRankingData> m_RankingData;
	//string m_PlayersName[DF_BG_MAXTEAM];

private:
	void FindPlayerNameByTeamID(int iTeamID, string& strOut);

public:
	CHorseRacingRankingFrame(CInterface* object);
	virtual ~CHorseRacingRankingFrame();
	virtual	void RegisterFunc();

	virtual void UpdateRanking(StructBattleGroundValue* pData);
	virtual	void FinalRanking( int iWinTeam );
};