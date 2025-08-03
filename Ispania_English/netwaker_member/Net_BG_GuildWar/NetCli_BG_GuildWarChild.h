#pragma once

#include "NetCli_BG_GuildWar.h"

class NetCli_BG_GuildWarChild : public NetCli_BG_GuildWar
{
public:	
	static	bool _Init();
	static	bool _Release();

public:

	//�n�D����A�����|�W�ٵ��G
	virtual	void RBL_GuildNameResult( int WorldGuildID , const char* GuildName );
	//�}�ҵ��U���|�Ԥ���
	virtual	void RL_OpenMenuResult(int PlayerDBID , int ZoneGroupID , GuildHouseWarStateENUM State , int NextTime , int Count , GuildHouseWarInfoStruct List[1000] );
	//���U���|��( or ���� )���G
	virtual	void RL_RegisterResult(GuildWarRegisterResultENUM Result , GuildHouseWarInfoStruct& Info );
	//���|�Ԫ��p( �}�l , �ǳ� , ���� )
	virtual	void RL_Status(GuildWarStatusTypeENUM Status , int NextTime );
	//�n�D�i�J���G
	virtual	void RL_EnterResult(bool Result );
	//�n�D���}���|�Գ����G
	virtual	void RBL_LeaveResult(bool Result );

	//////////////////////////////////////////////////////////////////////////
	//�i�J�Գ��� �Գ���T�B�z
	//////////////////////////////////////////////////////////////////////////
	//���|���Z����T
	virtual	void RBL_GuildScore( GuildHouseWarFightStruct& Info , int Time );
	//���|�Զ}�l
	virtual	void RBL_BeginWar(  );
	//���|�Ե���
	virtual	void RBL_EndWar(  );
	
	//////////////////////////////////////////////////////////////////////////
	//�n�D���|�Գ����a���
	//////////////////////////////////////////////////////////////////////////
	virtual	void RBL_PlayerInfoResult( int TotalCount , int ID , GuildWarPlayerInfoStruct& Info );
	virtual	void RBL_PlayerFinalInfo( int PlayerDBID , int PrizeType , int Score , int OrderID , Voc_ENUM Voc , Voc_ENUM Voc_Sub , int Level , int Level_Sub );

	//////////////////////////////////////////////////////////////////////////
	//���|�ԱƦ�]
	virtual	void RL_GuildWarHisotry( GuildHouseWarHistoryStruct Hisotry[50] );
	virtual	void RL_GuildWarRankInfo( GuildWarRankInfoStruct& Info );
};

