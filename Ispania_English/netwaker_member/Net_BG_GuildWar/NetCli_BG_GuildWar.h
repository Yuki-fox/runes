#pragma once

#include "PG/PG_BG_GuildWar.h"
#include "../../MainProc/NetGlobal.h"

class NetCli_BG_GuildWar:public CNetGlobal
{
protected:
	static NetCli_BG_GuildWar *This;
	static bool _Init();
	static bool _Release();
	static void _PG_BG_GuildWar_BLtoC_GuildNameResult	( int Sockid , int Size , void* Data );
	static void _PG_BG_GuildWar_LtoC_OpenMenuResult		( int Sockid , int Size , void* Data );
	static void _PG_BG_GuildWar_LtoC_RegisterResult		( int Sockid , int Size , void* Data );
	static void _PG_BG_GuildWar_LtoC_Status				( int Sockid , int Size , void* Data );
	static void _PG_BG_GuildWar_LtoC_EnterResult		( int Sockid , int Size , void* Data );
	static void _PG_BG_GuildWar_BLtoC_LeaveResult		( int Sockid , int Size , void* Data );
	static void _PG_BG_GuildWar_BLtoC_GuildScore		( int Sockid , int Size , void* Data );
	static void _PG_BG_GuildWar_BLtoC_BeginWar			( int Sockid , int Size , void* Data );
	static void _PG_BG_GuildWar_BLtoC_EndWar			( int Sockid , int Size , void* Data );
	static void _PG_BG_GuildWar_BLtoC_PlayerInfoResult	( int Sockid , int Size , void* Data );
	static void _PG_BG_GuildWar_BLtoC_PlayerFinalInfo	( int Sockid , int Size , void* Data );
	static void _PG_BG_GuildWar_BLtoC_PlayerListInfo_Zip( int Sockid , int Size , void* Data );

	static void _PG_BG_GuildWar_LtoC_GuildWarHisotry	( int Sockid , int Size , void* Data );
	static void _PG_BG_GuildWar_LtoC_GuildWarRankInfo	( int Sockid , int Size , void* Data );
public:

	//�n�D����A�����|�W�٭n�D
	static void	SBL_GuildNameRequest( int WorldGuildID );
		//�n�D����A�����|�W�ٵ��G
		virtual	void RBL_GuildNameResult( int WorldGuildID , const char* GuildName ) = 0;

	//�}�ҵ��U���|�Ԥ���
	static void	SL_OpenMenuRequest( );
		//�}�ҵ��U���|�Ԥ���
		virtual	void RL_OpenMenuResult(int PlayerDBID , int ZoneGroupID , GuildHouseWarStateENUM State , int NextTime , int Count , GuildHouseWarInfoStruct List[1000] ) = 0;

	//���U���|��( or ���� )
	static void	SL_RegisterRequest( int ZoneGroupID , GuildWarRegisterTypeENUM Type , int GuildCount , bool IsAcceptAssignment = false , int TargetGuild = 0 );
		//���U���|��( or ���� )���G
		virtual	void RL_RegisterResult(GuildWarRegisterResultENUM Result,GuildHouseWarInfoStruct& Info ) = 0;

	//���|�Ԫ��p( �}�l , �ǳ� , ���� )
	virtual	void RL_Status(GuildWarStatusTypeENUM Status , int NextTime ) = 0;
	//�n�D�i�J���|�Գ�
	static void	SL_EnterRequest( int ZoneGroupID );
	//�n�D�i�J���G
	virtual	void RL_EnterResult(bool Result ) = 0;

	//�n�D���}���|�Գ�
	static void	SBL_LeaveRequest( );
		//�n�D���}���|�Գ����G
		virtual	void RBL_LeaveResult(bool Result ) = 0;

	//���|���Z����T
	virtual	void RBL_GuildScore( GuildHouseWarFightStruct& Info , int Time  ) = 0;
	//���|�Զ}�l
	virtual	void RBL_BeginWar(  ) = 0;
	//���|�Ե���
	virtual	void RBL_EndWar(  ) = 0;

	//�n�D���|�Գ����a���
	static void	SBL_PlayerInfoRequest( );
		virtual	void RBL_PlayerInfoResult( int TotalCount , int ID , GuildWarPlayerInfoStruct& Info ) = 0;
	virtual	void RBL_PlayerFinalInfo( int PlayerDBID , int PrizeType , int Score , int OrderID , Voc_ENUM Voc , Voc_ENUM Voc_Sub , int Level , int Level_Sub ) = 0;

	//////////////////////////////////////////////////////////////////////////
	//���|�ԱƦ�]
	static void SL_GuildWarRankInfoRequest( GuildWarRankInfoTypeENUM Type , int WorldGuildID );
	virtual	void RL_GuildWarHisotry( GuildHouseWarHistoryStruct Hisotry[50] ) = 0;
	virtual	void RL_GuildWarRankInfo( GuildWarRankInfoStruct& Info ) = 0;
};
