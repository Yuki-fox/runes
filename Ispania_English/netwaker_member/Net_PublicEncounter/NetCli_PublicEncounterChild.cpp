#include "../../mainproc/GameMain.h"
#include "NetCli_PublicEncounterChild.h"
#include "../../Interface/WorldFrames/PublicEncounterFrame.h"


#define UiEvent_PECloseFrame				 "PE_CloseFrame"		//	 Ãö±¼¤¶­±


//---------------------------------------------------------------------
map<int, CPublicEncounter*>	PublicEncounterManager::m_PEIndex;
//---------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////////////////////////////
// class CPublicEncounterObjective
////////////////////////////////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
CPublicEncounterObjective::CPublicEncounterObjective(StructPEObjectiveBaseData* pBaseData)
{
	m_BaseData = *pBaseData;
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
CPublicEncounterObjective::~CPublicEncounterObjective()
{

}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
void CPublicEncounterObjective::Reset()
{
	Status = EM_OBJ_STATUS_PROCESSING;

	NowValue = InitValue;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
// class CPublicEncounterPhase
////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
CPublicEncounterPhase::CPublicEncounterPhase(StructPEPhaseBaseData* pBaseData)
{
	m_BaseData = *pBaseData;
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
CPublicEncounterPhase::~CPublicEncounterPhase()
{
	for (int i = 0; i < (int)m_Objectives.size(); ++i)
	{
		if (m_Objectives[i])
		{
			delete m_Objectives[i];
		}
	}

	m_Objectives.clear();
	m_ObjectivesIndex.clear();
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
CPublicEncounterObjective* CPublicEncounterPhase::AddObjective(StructPEObjectiveBaseData* pBaseData)
{
	CPublicEncounterObjective* Result = NULL;

	if (pBaseData)
	{
		if (pBaseData->iPHIndex == Index)
		{
			map<int, CPublicEncounterObjective*>::iterator it = m_ObjectivesIndex.find(pBaseData->iOBIndex);

			if (it == m_ObjectivesIndex.end())
			{
				Result = new CPublicEncounterObjective(pBaseData);
				m_ObjectivesIndex[pBaseData->iOBIndex] = Result;
				m_Objectives.push_back(Result);
			}		
		}
	}

	return Result;
}

//---------------------------------------------------------------------
//
//--------------------------------------------------------------------
CPublicEncounterObjective* CPublicEncounterPhase::GetObjective(int iOBIndex)
{
	CPublicEncounterObjective* Result = NULL;

	map<int, CPublicEncounterObjective*>::iterator it = m_ObjectivesIndex.find(iOBIndex);

	if (it != m_ObjectivesIndex.end())
	{
		Result = it->second;
	}

	return Result;
}

//---------------------------------------------------------------------
//
//--------------------------------------------------------------------
void CPublicEncounterPhase::Reset()
{
	for (int i = 0; i < (int)m_Objectives.size() ;++i)
	{
		m_Objectives[i]->Reset();
	}

	Status = EM_PHASE_STATUS_PROCESSING;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
// class CPublicEncounterPhase
////////////////////////////////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
CPublicEncounter::CPublicEncounter(StructPEBaseData* pBaseData)
{
	m_BaseData    = *pBaseData;
	m_Score       = 0;
	m_ActivePhase = GetPhase(m_BaseData.iActivePhaseIndex);
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
CPublicEncounter::~CPublicEncounter()
{
	for (int i = 0; i < (int)m_Phases.size(); ++i)
	{
		if (m_Phases[i])
		{
			delete m_Phases[i];
		}
	}

	m_Phases.clear();
	m_PhaseIndex.clear();
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
void CPublicEncounter::SetStatus(int iValue)
{
	if (m_BaseData.iStatus != iValue)
	{
		int iOldStatus = m_BaseData.iStatus;

		m_BaseData.iStatus = iValue;

		switch (m_BaseData.iStatus)
		{
		case EM_PE_STATUS_SLEEPING:
			{
			//	if (iOldStatus == EM_PE_STATUS_WAIT_FOR_RESET)
			//	{
					Reset();
			//	}
				break;
			}			
		case EM_PE_STATUS_PAUSE:
			break;
		case EM_PE_STATUS_PROCESSING:
			break;
		case EM_PE_STATUS_WAIT_FOR_RESET:
			break;
		case EM_PE_STATUS_SUCCESS:
			break;
		case EM_PE_STATUS_FAILED:
			break;
		}
	}
}
//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
bool CPublicEncounter::GetScoreVisible()
{
	return ((m_BaseData.iFlag & EM_PE_FLAG_SCORE_VISIBLE) == EM_PE_FLAG_SCORE_VISIBLE);
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
void CPublicEncounter::SetScoreVisible(bool value)
{
	if(value)
	{
		(m_BaseData.iFlag |= EM_PE_FLAG_SCORE_VISIBLE);
	}
	else
	{
		m_BaseData.iFlag &= (!EM_PE_FLAG_SCORE_VISIBLE);
	}
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
bool CPublicEncounter::GetUIVisible()
{
	return ((m_BaseData.iFlag & EM_PE_FLAG_UI_VISIBLE) == EM_PE_FLAG_UI_VISIBLE);
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
void CPublicEncounter::SetUIVisible(bool value)
{
	if(value)
	{
		(m_BaseData.iFlag |= EM_PE_FLAG_UI_VISIBLE);
	}
	else
	{
		m_BaseData.iFlag &= (!EM_PE_FLAG_UI_VISIBLE);
	}
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
CPublicEncounterPhase* CPublicEncounter::AddPhase(StructPEPhaseBaseData* pBaseData)
{
	CPublicEncounterPhase* Result = NULL;

	if (pBaseData)
	{
		if (pBaseData->iPEIndex == Index)
		{
			map<int, CPublicEncounterPhase*>::iterator it = m_PhaseIndex.find(pBaseData->iPHIndex);

			if (it == m_PhaseIndex.end())
			{
				Result = new CPublicEncounterPhase(pBaseData);
				m_PhaseIndex[pBaseData->iPHIndex] = Result;
				m_Phases.push_back(Result);
			}		
		}
	}

	return Result;
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
CPublicEncounterPhase* CPublicEncounter::GetPhase(int iPHIndex)
{
	CPublicEncounterPhase* Result = NULL;

	map<int, CPublicEncounterPhase*>::iterator it = m_PhaseIndex.find(iPHIndex);

	if (it != m_PhaseIndex.end())
	{
		Result = it->second;
	}

	return Result;
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
void CPublicEncounter::SetActivePhase(int iPHIndex)
{
	m_ActivePhase = GetPhase(iPHIndex);
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
void CPublicEncounter::AddRegion(StructPERegionData* pRegionData)
{
	if (pRegionData)
	{
		m_Regions.push_back(*pRegionData);
	}
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
void CPublicEncounter::Reset()
{
	//--------------------------------------------
	//reset all phase
	for (int i = 0; i < (int)m_Phases.size(); ++i )
	{
		CPublicEncounterPhase* pPhase = m_Phases[i];
		if (pPhase)
		{
			pPhase->Reset();
		}
	}
	//--------------------------------------------
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
// class PublicEncounterManager
////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
void PublicEncounterManager::Clear()
{
	map<int, CPublicEncounter*>::iterator it;

	for (it = m_PEIndex.begin(); it != m_PEIndex.end(); it++)
	{
		if (it->second)
		{
			delete it->second;
		}
	}

	m_PEIndex.clear();
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
CPublicEncounter* PublicEncounterManager::AddPE(StructPEBaseData* pBaseData)
{
	CPublicEncounter* Result = NULL;

	if (pBaseData)
	{
		map<int, CPublicEncounter*>::iterator it = m_PEIndex.find(pBaseData->iPEIndex);

		if (it == m_PEIndex.end())
		{
			Result = new CPublicEncounter(pBaseData);
			m_PEIndex[Result->Index] = Result;
		}
	}

	return Result;
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
void PublicEncounterManager::LoadConfig(int iSize, PVOID pData)
{
	Clear();

	CPublicEncounter* pPE = NULL;
	CPublicEncounterPhase* pPH = NULL;
	CPublicEncounterObjective* pOB = NULL;
	//-----------------------------------------------------------------
	//read packet
	PBYTE ptr = (PBYTE)pData;

	//PacketID
	int iPacketID   = *((int*)ptr);
	ptr		       += sizeof(int);

	if (iPacketID == EM_PG_PE_LtoC_PEConfigData)
	{
		//count PE number
		int iPENum  = *((int*)ptr);
		ptr		   += sizeof(int);

		for (int i = 0; i < iPENum; ++i)
		{
			// create a new PE
			StructPEBaseData* pPEBase = (StructPEBaseData*)ptr;
			pPE = AddPE(pPEBase);

			ptr += sizeof(StructPEBaseData);

			// read region number
			int iRGNum  = *((int*)ptr);
			ptr		   += sizeof(int);

			// read phase number
			int iPHNum  = *((int*)ptr);
			ptr		   += sizeof(int);

			//read Region data
			for (int j = 0; j < iRGNum; ++j)
			{
				if (pPE)
				{
					StructPERegionData* pRGBase = (StructPERegionData*)ptr;

					pPE->AddRegion(pRGBase);
				}

				ptr	+= sizeof(StructPERegionData);
			}

			//read phase data
			for (int j = 0; j < iPHNum; ++j)
			{
				if (pPE)
				{				
					// add a new phase
					StructPEPhaseBaseData* pPHBase = (StructPEPhaseBaseData*)ptr;

					pPH = pPE->AddPhase(pPHBase);
				}

				ptr += sizeof(StructPEPhaseBaseData);

				// read Objective number
				int iOBNum  = *((int*)ptr);
				ptr 	   += sizeof(int);

				//read Objective Data
				for (int k = 0; k < iOBNum; ++k)
				{
					if (pPH)
					{
						//add an Objective
						StructPEObjectiveBaseData* pOBBase = (StructPEObjectiveBaseData*)ptr;
						pOB = pPH->AddObjective(pOBBase);
					}

					ptr	 += sizeof(StructPEObjectiveBaseData);
				}	
			}	
		}
	}
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
void PublicEncounterManager::OnRegionChange(CRuWorld_ZoneDescriptor* zoneDescriptor)
{
	if (zoneDescriptor)
	{
		CNetCli_PublicEncounter::SL_RegionChanged(zoneDescriptor->GetZoneID());
	}	
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
CPublicEncounter* PublicEncounterManager::GetPEInstance(int iPEIndex)
{
	CPublicEncounter* Result = NULL;

	map<int, CPublicEncounter*>::iterator it = m_PEIndex.find(iPEIndex);

	if (it != m_PEIndex.end())
	{
		Result = it->second;
	}

	return Result;
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
CPublicEncounterPhase* PublicEncounterManager::GetPhaseInstance(int iPEIndex, int iPHIndex)
{
	CPublicEncounterPhase* Result = NULL;

	CPublicEncounter* pPE = GetPEInstance(iPEIndex);

	if (pPE)
	{
		Result = pPE->GetPhase(iPHIndex);
	}

	return Result;
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
CPublicEncounterObjective* PublicEncounterManager::GetObjectiveInstance(int iPEIndex, int iPHIndex, int iOBIndex)
{
	CPublicEncounterObjective* Result = NULL;

	CPublicEncounterPhase* pPH = GetPhaseInstance(iPEIndex, iPHIndex);

	if (pPH)
	{
		Result = pPH->GetObjective(iOBIndex);
	}

	return Result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
// class CNetCli_PublicEncounterChild
////////////////////////////////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
bool CNetCli_PublicEncounterChild::_Init()
{
	if( m_pThis == NULL )
		m_pThis = new CNetCli_PublicEncounterChild();

	CNetCli_PublicEncounter::_Init();

	g_pGameMain->RegisterZoneChangeCallBack(&PublicEncounterManager::OnRegionChange);

	return true;
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
bool CNetCli_PublicEncounterChild::_Release()
{
	CNetCli_PublicEncounter::_Release();
	SAFE_DELETE( m_pThis );
	return true;
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
void CNetCli_PublicEncounterChild::OnRZ_PEConfigData(int iSrvID, int iSize, PVOID pData)
{
	PublicEncounterManager::LoadConfig(iSize, pData);
	g_pPublicEncounterFrame->PE_LeaveAndClose();
	g_pPublicEncounterFrame->PE_RL(ePEChangeType_Close , 0 , 0);
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
void CNetCli_PublicEncounterChild::OnRZ_PEStatus(int iSrvID, int iPEIndex, int iStatus)
{
	CPublicEncounter* pPE = PublicEncounterManager::GetPEInstance(iPEIndex);

	if (pPE)
	{
		pPE->Status = iStatus;
	}
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
void CNetCli_PublicEncounterChild::OnRZ_PhaseStatus(int iSrvID, int iPEIndex, int iPHIndex, int iStatus)
{
	CPublicEncounterPhase* pPH = PublicEncounterManager::GetPhaseInstance(iPEIndex, iPHIndex);

	if (pPH)
	{
		pPH->Status = iStatus;
	}
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
void CNetCli_PublicEncounterChild::OnRZ_ObjectiveStatus(int iSrvID, int iPEIndex, int iPHIndex, int iOBIndex, int iStatus)
{
	CPublicEncounterObjective* pOB = PublicEncounterManager::GetObjectiveInstance(iPEIndex, iPHIndex, iOBIndex);

	if (pOB)
	{
		pOB->Status = iStatus;
	}
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
void CNetCli_PublicEncounterChild::OnRZ_PlayerScore(int iSrvID, int iPEIndex, int iScore)
{
	CPublicEncounter* pPE = PublicEncounterManager::GetPEInstance(iPEIndex);

	if (pPE)
	{
		pPE->Score = iScore;
		g_pPublicEncounterFrame->PE_RL(ePEChangeType_PlayerScore , iPEIndex , 0);

	}

	//Modify the displaying personal score on interface
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
void CNetCli_PublicEncounterChild::OnRZ_PEActivated(int iSrvID, int iPEIndex, int iActivePhaseIndex)
{
	CPublicEncounter* pPE = PublicEncounterManager::GetPEInstance(iPEIndex);
	if (pPE)
	{
		pPE->SetActivePhase(iActivePhaseIndex);
		//show the public encounter interface for specific PE
		g_pPublicEncounterFrame->PE_RL(ePEChangeType_Activated , iPEIndex , iActivePhaseIndex);
	}
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
void CNetCli_PublicEncounterChild::OnRZ_PEDeactivated(int iSrvID, int iPEIndex)
{
	CPublicEncounter* pPE = PublicEncounterManager::GetPEInstance(iPEIndex);
	
	if (pPE)
	{
		//hide the public encounter interface for specific PE
		g_pPublicEncounterFrame->PE_RL(ePEChangeType_Deactivated , iPEIndex , 0);

	}
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
void CNetCli_PublicEncounterChild::OnRZ_PEActivePhaseChange(int iSrvID, int iPEIndex, int iActivePhaseIndex)
{
	CPublicEncounter* pPE = PublicEncounterManager::GetPEInstance(iPEIndex);
	CPublicEncounterPhase* pPH = PublicEncounterManager::GetPhaseInstance(iPEIndex, iActivePhaseIndex);
	if (pPH)
	{
		pPE->SetActivePhase(iActivePhaseIndex);
		//change the data displaying on interface
		g_pPublicEncounterFrame->PE_RL(ePEChangeType_ActivePhaseChange , iPEIndex , iActivePhaseIndex);
	}

}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
void CNetCli_PublicEncounterChild::OnRZ_ObjectiveValueChage(int iSrvID, int iPEIndex, int iPHIndex, int iOBIndex, int iNowValue)
{
	CPublicEncounter* pPE = PublicEncounterManager::GetPEInstance(iPEIndex);
	if (pPE)
	{
		CPublicEncounterObjective* pOB = PublicEncounterManager::GetObjectiveInstance(iPEIndex, iPHIndex, iOBIndex);
		if (pOB)
		{
			pOB->NowValue = iNowValue;
			//change the data displaying on interface
		
			g_pPublicEncounterFrame->PE_RL(ePEChangeType_ObjValueChage , iPEIndex , iPHIndex);

		}		
	}
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
void CNetCli_PublicEncounterChild::OnRZ_PESuccess(int iSrvID, int iPEIndex, int iTimeGap)
{
	CPublicEncounter* pPE = PublicEncounterManager::GetPEInstance(iPEIndex);
	if (pPE)
	{
		g_pPublicEncounterFrame->PE_RL(ePEChangeType_Success , iPEIndex ,0);

	}
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
void CNetCli_PublicEncounterChild::OnRZ_PEFailed(int iSrvID, int iPEIndex, int iTimeGap)
{
	CPublicEncounter* pPE = PublicEncounterManager::GetPEInstance(iPEIndex);
	if (pPE)
	{
		g_pPublicEncounterFrame->PE_RL(ePEChangeType_Failed , iPEIndex ,0);

	}
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
void CNetCli_PublicEncounterChild::OnRZ_PhaseSuccess(int iSrvID, int iPEIndex, int iPHIndex, int iTimeGap)
{
	CPublicEncounterPhase* pPH = PublicEncounterManager::GetPhaseInstance(iPEIndex, iPHIndex);

	if (pPH)
	{
		g_pPublicEncounterFrame->PE_RL(ePEChangeType_PhaseSuccess , iPEIndex ,iPHIndex);


	}
}
//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
void CNetCli_PublicEncounterChild::OnRZ_PhaseFailed(int iSrvID, int iPEIndex, int iPHIndex, int iTimeGap)
{
	CPublicEncounterPhase* pPH = PublicEncounterManager::GetPhaseInstance(iPEIndex, iPHIndex);

	if (pPH)
	{
		g_pPublicEncounterFrame->PE_RL(ePEChangeType_PhaseFailed , iPEIndex ,iPHIndex);
	}
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
void CNetCli_PublicEncounterChild::OnRZ_ObjectiveAchieved(int iSrvID, int iPEIndex, int iPHIndex, int iOBIndex)
{
	CPublicEncounterPhase* pPH = PublicEncounterManager::GetPhaseInstance(iPEIndex, iPHIndex);
	if (pPH)
	{
		g_pPublicEncounterFrame->PE_RL(ePEChangeType_ObjAchieved , iPEIndex ,iPHIndex);

	}
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
void CNetCli_PublicEncounterChild::OnRZ_ObjectiveFlag(int iSrvID, int iPEIndex, int iPHIndex, int iOBIndex, int iFlag)
{
	CPublicEncounter* pPE = PublicEncounterManager::GetPEInstance(iPEIndex);
	if (pPE)
	{
		CPublicEncounterObjective* pOB = PublicEncounterManager::GetObjectiveInstance(iPEIndex, iPHIndex, iOBIndex);
		if (pOB)
		{
			pOB->Flag = iFlag;
		}		
	}
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
void CNetCli_PublicEncounterChild::OnRZ_PEFlag(int iSrvID, int iPEIndex, int iFlag)
{
	CPublicEncounter* pPE = PublicEncounterManager::GetPEInstance(iPEIndex);
	if (pPE)
	{
		pPE->Flag = iFlag;
	}
}