#ifndef __NETWAKER_PUBLIC_ENCOUNTER_H__
#define __NETWAKER_PUBLIC_ENCOUNTER_H__
#include "PG/PG_PublicEncounter.h"
#include "../../MainProc/NetGlobal.h"

class CNetCli_PublicEncounter : public CNetGlobal
{
protected:
	//-------------------------------------------------------------------
	static CNetCli_PublicEncounter*	m_pThis;
	static bool _Init();
	static bool _Release();
	//-------------------------------------------------------------------

	static void _PG_PE_LtoC_PEConfigData		(int iSrvID, int iSize, PVOID pData);
	static void _PG_PE_LtoC_PEStatus			(int iSrvID, int iSize, PVOID pData);
	static void _PG_PE_LtoC_PhaseStatus			(int iSrvID, int iSize, PVOID pData);
	static void _PG_PE_LtoC_ObjectiveStatus		(int iSrvID, int iSize, PVOID pData);
	static void _PG_PE_LtoC_PlayerScore			(int iSrvID, int iSize, PVOID pData);
	static void _PG_PE_LtoC_PEActivated			(int iSrvID, int iSize, PVOID pData);
	static void _PG_PE_LtoC_PEDeactivated		(int iSrvID, int iSize, PVOID pData);
	static void _PG_PE_LtoC_PEActivePhaseChange	(int iSrvID, int iSize, PVOID pData);
	static void _PG_PE_LtoC_ObjectiveValueChage	(int iSrvID, int iSize, PVOID pData);
	static void _PG_PE_LtoC_PESuccess			(int iSrvID, int iSize, PVOID pData);
	static void _PG_PE_LtoC_PEFailed			(int iSrvID, int iSize, PVOID pData);
	static void _PG_PE_LtoC_PhaseSuccess		(int iSrvID, int iSize, PVOID pData);
	static void _PG_PE_LtoC_PhaseFailed			(int iSrvID, int iSize, PVOID pData);
	static void _PG_PE_LtoC_ObjectiveAchieved	(int iSrvID, int iSize, PVOID pData);
	static void _PG_PE_LtoC_ObjectiveFlag		(int iSrvID, int iSize, PVOID pData);
	static void _PG_PE_LtoC_PEFlag				(int iSrvID, int iSize, PVOID pData);

public:
	static void SL_RegionChanged(int iRegionID);

public:
	virtual void OnRZ_PEConfigData			(int iSrvID, int iSize, PVOID pData) = 0;
	virtual void OnRZ_PEStatus				(int iSrvID, int iPEIndex, int iStatus) = 0;
	virtual void OnRZ_PhaseStatus			(int iSrvID, int iPEIndex, int iPHIndex, int iStatus) = 0;
	virtual void OnRZ_ObjectiveStatus		(int iSrvID, int iPEIndex, int iPHIndex, int iOBIndex, int iStatus) = 0;
	virtual void OnRZ_PlayerScore			(int iSrvID, int iPEIndex, int iScore) = 0;
	virtual void OnRZ_PEActivated			(int iSrvID, int iPEIndex, int iActivePhaseIndex) = 0;
	virtual void OnRZ_PEDeactivated			(int iSrvID, int iPEIndex) = 0;
	virtual void OnRZ_PEActivePhaseChange	(int iSrvID, int iPEIndex, int iActivePhaseIndex) = 0;
	virtual void OnRZ_ObjectiveValueChage	(int iSrvID, int iPEIndex, int iPHIndex, int iOBIndex, int iNowValue) = 0;
	virtual void OnRZ_PESuccess				(int iSrvID, int iPEIndex, int iTimeGap) = 0;
	virtual void OnRZ_PEFailed				(int iSrvID, int iPEIndex, int iTimeGap) = 0;
	virtual void OnRZ_PhaseSuccess			(int iSrvID, int iPEIndex, int iPHIndex, int iTimeGap) = 0;
	virtual void OnRZ_PhaseFailed			(int iSrvID, int iPEIndex, int iPHIndex, int iTimeGap) = 0;
	virtual void OnRZ_ObjectiveAchieved		(int iSrvID, int iPEIndex, int iPHIndex, int iOBIndex) = 0;
	virtual void OnRZ_ObjectiveFlag			(int iSrvID, int iPEIndex, int iPHIndex, int iOBIndex, int iFlag) = 0;
	virtual void OnRZ_PEFlag				(int iSrvID, int iPEIndex, int iFlag) = 0;
};

#endif
