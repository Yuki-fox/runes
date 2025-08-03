#pragma once

#include "../GuildHouses/GuildHousesFrame.h"
#include "../GuildHouses/GuildHouseStatus.h"
#include "../../netwaker_member/Net_BG_GuildWar/NetCli_BG_GuildWarChild.h"
#include "../../netwaker_member/Net_Guild/NetCli_GuildChild.h"
#include "GuildFrame.h"

#include "../interface.h"

//************************************************************************************************************************************************************
class CGuildWarScore;
extern CGuildWarScore* g_pGuildWarScore;
//************************************************************************************************************************************************************

class CGuildWarScore : public CInterfaceBase
{
protected:
	GuildHouseWarFightStruct	m_GuildInfo;

public:
								CGuildWarScore( CInterface* pObject );
	virtual						~CGuildWarScore( void );

	virtual	void				RegisterFunc();
	virtual	void				Update(float elapsedTime);

	virtual void				RBL_GuildScore(GuildHouseWarFightStruct& Info , int Time );

	int							m_EnergyB;
	int							m_EnergyR;
	int							m_guildIDB;
	int							m_guildIDR;
	int							m_guildScoreB;
	int							m_guildScoreR;
	float						m_RemainTime;
	//float						m_BeginTime;
	//float						m_EndTime;
};

