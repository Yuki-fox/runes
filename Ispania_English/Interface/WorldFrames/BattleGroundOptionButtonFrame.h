#include <vector>
#include "../../netwaker_member/Net_BattleGround/NetCli_BattleGround_Child.h"
#include "../interface.h"

#define MAX_ENTER_BG_QUERY_DLG_NUM 4

using namespace std;

class CBattleGroundOptionButtonFrame;
extern CBattleGroundOptionButtonFrame*	g_pBattleGroundOptionButtonFrame;

class CBattleGroundOptionButtonFrame : public CInterfaceBase
{
public:
	int m_bDlgFlag[MAX_ENTER_BG_QUERY_DLG_NUM];

public:
	CBattleGroundOptionButtonFrame(CInterface* object);
	virtual ~CBattleGroundOptionButtonFrame();

	virtual	void RegisterFunc();
	virtual	void Update(float elapsedTime);

	virtual void ShowEnterBGQueryDialog(int iType, int iRoomID);
	virtual void StartFlashBGOptionButton();
	virtual void StopFlashBGOptionButton();

	int static IsTeam();
	int static IsLeader();

	void static LeaveBattleGroundGroup(int iZoneID);
};