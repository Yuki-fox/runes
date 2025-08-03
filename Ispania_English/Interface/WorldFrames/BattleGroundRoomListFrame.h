#pragma once

#include <vector>
#include "../../netwaker_member/Net_BattleGround/NetCli_BattleGround_Child.h"
#include "../interface.h"

using namespace std;

class CBattleGroundRoomListFrame;
extern CBattleGroundRoomListFrame*	g_pBattleGroundRoomListFrame;

class CBattleGroundRoomListFrame : public CInterfaceBase
{
public:
	vector< structBattleGroundResult > m_vecRoom;
	int                                m_iType;

public:
	CBattleGroundRoomListFrame(CInterface* object);
	virtual ~CBattleGroundRoomListFrame();

	virtual	void	RegisterFunc();
	virtual void    Open();
	virtual void    Close();

	virtual void    UpdateRoomList(int iType, vector< structBattleGroundResult > vecRoom);
};