#pragma once

#include <vector>
#include "../../netwaker_member/Net_BattleGround/NetCli_BattleGround_Child.h"
#include "../interface.h"


using namespace std;
class CBattleGroundQueueFrame;
extern CBattleGroundQueueFrame*	g_pBattleGroundQueueFrame;

class CBattleGroundQueueFrame : public CInterfaceBase
{
public:
	CBattleGroundQueueFrame(CInterface* object);
	bool FlagOfOpenInterface;
	virtual ~CBattleGroundQueueFrame();
	virtual	void	RegisterFunc();
	virtual	void	Update(float elapsedTime);
	virtual	void	GetBattleGroundResult(int BGType);

	void GetJoinBGTeamDBID();

	int m_IsAllowJoinWithTeam;
	int m_TeamDBID[DF_MAX_TEAMDBID];
	int m_LeaderDBID;
	int m_IsLeaderSelf;
	int m_partypeoplecount;

	int emQueueStatus;
	int queuecount;
	int iType;
	int resultType;
	DWORD dwTimeExcepted;
	int dwTimeToLeaveQueue;
	//char BGN[128];
};