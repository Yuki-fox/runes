#include <map>
#include <vector>
#include "../../netwaker_member/Net_BattleGround/NetCli_BattleGround_Child.h"
#include "../interface.h"

using namespace std;

class CBattleGroundArenaScoreFrame;
extern CBattleGroundArenaScoreFrame*	g_pBattleGroundArenaScoreFrame;

#define BG_ARENA_TYPE_MAX  4

enum BG_ARENA_TYPE
{
	BG_ARENA_TYPE_1_VS_1 = 1,
	BG_ARENA_TYPE_2_VS_2,
	BG_ARENA_TYPE_6_VS_6,
	BG_ARENA_TYPE_4S
};

struct StructArenaInfoEx
{
	int iArenaType;
	int iTeamGUID;
	StructArenaInfo ArenaInfo;
};

class CBattleGroundArenaScoreFrame : public CInterfaceBase
{
private:
	StructArenaInfoEx m_ArenaInfos[BG_ARENA_TYPE_MAX];
	int				  m_ActiveDetailArenaType;
	bool			  m_TimeUnit;
	float             m_TimeToUpdate;

public:
	CBattleGroundArenaScoreFrame(CInterface* object);
	virtual ~CBattleGroundArenaScoreFrame();

	virtual	void	RegisterFunc();
	virtual	void	Update(float elapsedTime);
	virtual	void	UpdateArenaData(int iArenaType, int iTeamGUID, StructArenaInfo* pArenaInfo);	

public:
	void Update_After_n_Sec(float fTime){m_TimeToUpdate = fTime;}

public:
	int GetCarrerTotalWinCount();
	int GetCarrerTotalLoseCount();
	void SetActiveDetailArenaType(int iType){m_ActiveDetailArenaType = iType;}
	int GetActiveDetailArenaType(){return m_ActiveDetailArenaType;}

	void SetTimeUnit(int iTimeUnit) { m_TimeUnit = (iTimeUnit != 0); }
	bool GetTimeUnit(){return m_TimeUnit;}
	StructArenaInfoEx* GetInfo(int index) {return &m_ArenaInfos[index];}
};