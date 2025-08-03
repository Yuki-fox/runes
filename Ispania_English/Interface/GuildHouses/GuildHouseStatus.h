#pragma once

#include <vector>
#include "../../netwaker_member/Net_BG_GuildWar/NetCli_BG_GuildWarChild.h"
#include "../interface.h"

enum
{
	GHS_COLOR_ALL,
	GHS_COLOR_TEAM_BLUE,
	GHS_COLOR_TEAM_RED,
	GHS_COLOR_MAX,
};

using namespace std;

class CGuildHouseWarStatus;
extern CGuildHouseWarStatus*	g_pGuildHouseWarStatus;

struct SGuildHouseWarPlayerInfo
{
	GuildWarPlayerInfoStruct	Status;
	int							iTeamColor;

	SGuildHouseWarPlayerInfo()
	{
		iTeamColor = GHS_COLOR_ALL;		
	}
};

struct SGuildHouseWarRewardInfo
{
	Voc_ENUM					Voc;
	Voc_ENUM					Voc_Sub;
	int							iLevel;
	int							iLevel_Sub;	
	int							iPrizeType;
	int							iScore;
	int							iOrderID;

	SGuildHouseWarRewardInfo()
	{
		Voc = EM_Vocation_None;
		Voc_Sub = EM_Vocation_None;
		iLevel = 0;
		iLevel_Sub = 0;
		iPrizeType = -1;
		iScore = -1;
		iOrderID = -1;
	}
};

class CGuildHouseWarStatus : public CInterfaceBase
{
protected:
	vector<SGuildHouseWarPlayerInfo>	m_playerScores;
	vector<SGuildHouseWarPlayerInfo>	m_scoreClassify;
	map<int, SGuildHouseWarRewardInfo>	m_mapRewardInfo;
	bool                            	m_Visible;
	float								m_updateTimer;
	int                             	m_curSortFunc;
	int                             	m_curClassify;

	int									m_teamPlayerNum[GHS_COLOR_MAX];

	int									m_WinTeam;
	int									m_iBlueTeamGuildID;
	int									m_iMyTeamColor;

	GuildHouseWarFightStruct			m_Info;

public:
	CGuildHouseWarStatus(CInterface* object);
	virtual ~CGuildHouseWarStatus();

	virtual	void	RegisterFunc();
	virtual	void	FrameOnShow();
	virtual	void	FrameOnHide();
	virtual	void	Update(float elapsedTime);
	virtual void	ClassifyScore(int iClassify);
	virtual void	ChangeSortFunc(int iSort);
	virtual void	UpdateScore(int iTotalRole, int iIndex, GuildWarPlayerInfoStruct* pScoreArray);
	virtual void	UpdateFinalInfo(int PlayerDBID, int PrizeType, int Score, int OrderID, Voc_ENUM Voc, Voc_ENUM Voc_Sub, int Level, int Level_Sub);
	virtual	void	GuildHouseWarClose(int iWinTeam);
	virtual void	EndWarCalc();
	virtual int     GetTeamPlayerNum(int iTeamColor){return m_teamPlayerNum[iTeamColor];}
	virtual int		GetPlayerScoreInfoNum();
	virtual	void	Reset() {m_WinTeam = 0; m_mapRewardInfo.clear();}
	virtual	int		GetWinTeam() {return m_WinTeam;}
	virtual	int		GetMyTeamColor() {return m_iMyTeamColor;}
	virtual void	SetBlueTeamGuildID(int iGuildID) {m_iBlueTeamGuildID = iGuildID;}

	virtual SGuildHouseWarPlayerInfo* GetPlayerScore(int iIndex);
	virtual SGuildHouseWarRewardInfo* GetRewardInfo(int iPlayerDBID);
	virtual	void GetGuildScore( GuildHouseWarFightStruct& Info );

	vector<SGuildHouseWarPlayerInfo>& GetPlayerScoresTable(){ return m_playerScores; }
};