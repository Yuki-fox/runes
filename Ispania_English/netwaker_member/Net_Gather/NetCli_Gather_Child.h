#pragma once

#include "NetCli_Gather.h"
#include <vector>

class CNetCli_Gather_Child : public CNetCli_Gather
{
public:
	static	bool		Init						();
	static	bool		Release						();

	static	void		Update						( float elapsedTime );
	
	virtual void		OnTouchGatherObj			( int iRoleID );
	virtual void		OnClickGatherObjResult		( int iResult, int iItemID, int iItemVal );

	virtual	void		OnGatherMotion				( int iGatherPlayerID, int iMotionID, int iMineType );
	virtual void		PlayGatherSound				( int iMineType );
	virtual	void		StopGatherSound				();
	virtual void		PlayGatherSuccessSound		( int iMineType );
	virtual void		PlayGatherFailedSound		( int iMineType );

	static	int			CheckGatherObj				( int iGatherObjID );
	static	int			GetMineTypeByObjID			( int iGatherObjID );

	static	void		GetMinetooltips				( int iGatherObjID, string& stringtips, int& iMineType, int& iSkillNow, int& iReqSkill );

	static	bool		m_IsWorking;
	static	float		m_fGatherRestTimer;
	static	int			m_iGatherObj;
	static	INT64 m_iGatherSound;
	static	bool		m_bAutoGather;

};

enum ENUM_CHECK_GATHER
{
	EM_CHECK_GATHER_OK			= 1,
	EM_CHECK_GATHER_SKILLTOOLOW	= -1,
	EM_CHECK_GATHER_WRONGTOOL	= -2,
	EM_CHECK_GATHER_NOTOOL		= -3,
	EM_CHECK_GATHER_NOSKILL		= -4
};