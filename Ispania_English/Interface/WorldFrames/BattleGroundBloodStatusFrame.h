#pragma once

#include <vector>
#include "../../netwaker_member/Net_BattleGround/NetCli_BattleGround_Child.h"
#include "../interface.h"

using namespace std;
class CBattleGroundBloodStatusFrame;
extern CBattleGroundBloodStatusFrame*	g_pBattleGroundBloodStatusFrame;


class CBattleGroundBloodStatusFrame : public CInterfaceBase
{
public:
	CBattleGroundBloodStatusFrame(CInterface* object);
	virtual ~CBattleGroundBloodStatusFrame();

	virtual	void	RegisterFunc();
	virtual void	UpdateTowerInfo(StructBattleGroundValue* pData);

};