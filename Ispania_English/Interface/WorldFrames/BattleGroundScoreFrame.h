#pragma once

#include <vector>
#include "../../netwaker_member/Net_BattleGround/NetCli_BattleGround_Child.h"
#include "../interface.h"

using namespace std;

class CBattleGroundScoreFrame;
extern CBattleGroundScoreFrame*	g_pBattleGroundScoreFrame;

class CBattleGroundScoreFrame : public CInterfaceBase
{
public:
	CBattleGroundScoreFrame(CInterface* object);
	virtual ~CBattleGroundScoreFrame();
	virtual	void RegisterFunc();

	virtual	void UpdateScore(StructBattleGroundValue* pData);
};