#pragma once

#include "NetCli_PublicEncounter.h"
class CPublicEncounter;
class CPublicEncounterPhase;
class CPublicEncounterObjective;
class PublicEncounterManager;

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
class CPublicEncounterObjective
{
protected:
	StructPEObjectiveBaseData m_BaseData;

public:
	CPublicEncounterObjective(StructPEObjectiveBaseData* pBaseData);
	virtual ~CPublicEncounterObjective();

public:
	int			GetIndex()				{return m_BaseData.iOBIndex;}
	int			GetKind()				{return m_BaseData.iKind;}
	int			GetType()				{return m_BaseData.iType;}
	int			GetInitValue()			{return m_BaseData.iInitValue;}
	int			GetDoorsill()			{return m_BaseData.iDoorsill;}
	const char*	GetNameKeyStr()			{return m_BaseData.szNameKeyStr;}
	const char*	GetDescKeyStr()			{return m_BaseData.szDescKeyStr;}
	int			GetStatus()				{return m_BaseData.iStatus;}
	void		SetStatus(int value)	{m_BaseData.iStatus = value;}
	int			GetFlag()				{return m_BaseData.iFlag;}
	void		SetFlag(int value)		{m_BaseData.iFlag = value;}
	bool		GetVisible()			{return ((m_BaseData.iFlag & EM_OBJ_FLAG_VISIBLE) == EM_OBJ_FLAG_VISIBLE);}
	bool		GetMinorFlag()			{return ((m_BaseData.iFlag & EM_OBJ_FLAG_MINOR) == EM_OBJ_FLAG_MINOR);}
	int			GetNowValue()			{return m_BaseData.iNowValue;}
	void		SetNowValue(int value)	{m_BaseData.iNowValue = value;}

public:
	void				Reset();

public:
	__declspec(property(get = GetIndex							))	int			Index;
	__declspec(property(get = GetKind							))	int			Kind;
	__declspec(property(get = GetType							))	int			Type;
	__declspec(property(get = GetInitValue						))	int			InitValue;
	__declspec(property(get = GetDoorsill						))	int			Doorsill;
	__declspec(property(get = GetNowValue	, put = SetNowValue	))	int			NowValue;
	__declspec(property(get = GetDescKeyStr						))	const char*	DescriptionKeyStr;
	__declspec(property(get = GetNameKeyStr						))	const char*	NameKeyStr;
	__declspec(property(get = GetStatus		, put = SetStatus	))	int			Status;
	__declspec(property(get = GetFlag		, put = SetFlag		))	int			Flag;
	__declspec(property(get = GetVisible						))	bool		Visible;
	__declspec(property(get = GetMinorFlag						))	bool		IsMinor;
};

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
class CPublicEncounterPhase
{
protected:
	StructPEPhaseBaseData					m_BaseData;
	vector<CPublicEncounterObjective*>		m_Objectives;
	map<int, CPublicEncounterObjective*>	m_ObjectivesIndex;

public:
	CPublicEncounterPhase(StructPEPhaseBaseData* pBaseData);
	virtual ~CPublicEncounterPhase();

public:
	int									GetIndex()				{return m_BaseData.iPHIndex;}
	const char*							GetNameKeyStr()			{return m_BaseData.szNameKeyStr;}
	const char*							GetDescKeyStr()			{return m_BaseData.szDescKeyStr;}	
	int									GetStatus()				{return m_BaseData.iStatus;}
	void								SetStatus(int value)	{m_BaseData.iStatus = value;};
	float								GetPhaseScore()			{return m_BaseData.fPHScore;}
	int									GetSuccLogic()			{return m_BaseData.iSuccLogic;}
	int									GetFailLogic()			{return m_BaseData.iFailLogic;}
	vector<CPublicEncounterObjective*>&	GetObjectives()			{return m_Objectives;}

public:
	CPublicEncounterObjective* AddObjective(StructPEObjectiveBaseData* pBaseData);
	CPublicEncounterObjective* GetObjective(int iOBIndex);
	void					   Reset();

public:
	__declspec(property(get = GetIndex									))	int									Index;
	__declspec(property(get = GetName									))	const char*							Name;
	__declspec(property(get = GetDescription							))	const char*							Description;	
	__declspec(property(get = GetStatus				, put = SetStatus	))	int									Status;
	__declspec(property(get = GetPhaseScore								))	float								PhaseScore;
	__declspec(property(get = GetSuccLogic								))	int									SuccLogic;
	__declspec(property(get = GetFailLogic								))	int									FailLogic;
	__declspec(property(get = GetObjectives								))	vector<CPublicEncounterObjective*>&	Objectives;
};

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
class CPublicEncounter
{
protected:
	StructPEBaseData				 m_BaseData;
	vector<StructPERegionData>	 	 m_Regions;					// indicate which regions area event occurred.
	vector<CPublicEncounterPhase*>	 m_Phases;

	map<int, CPublicEncounterPhase*> m_PhaseIndex;

	int								 m_Score;
	CPublicEncounterPhase*			 m_ActivePhase;

public:
	CPublicEncounter(StructPEBaseData* pBaseData);
	virtual ~CPublicEncounter();

public:
	int								GetIndex()					{return m_BaseData.iPEIndex;}
	CPublicEncounterPhase*			GetActivePhase()			{return m_ActivePhase;}
	int								GetStatus()					{return m_BaseData.iStatus;}
	void							SetStatus(int value);
	int								GetScore()					{return m_Score;}
	void							SetScore(int value)			{m_Score = value;}
	const char*						GetNameKeyStr()				{return m_BaseData.szNameKeyStr;}
	const char*						GetDescKeyStr()				{return m_BaseData.szDescKeyStr;}
	int								GetFlag()					{return m_BaseData.iFlag;}
	void							SetFlag(int value)			{m_BaseData.iFlag = value;}
	bool							GetScoreVisible();
	void							SetScoreVisible(bool value);
	bool							GetUIVisible();
	void							SetUIVisible(bool value);
	vector<StructPERegionData>&		GetRegions()				{return m_Regions;}
	vector<CPublicEncounterPhase*>&	GetPhases()					{return m_Phases;}


public:
	CPublicEncounterPhase*	AddPhase(StructPEPhaseBaseData* pBaseData);
	CPublicEncounterPhase*	GetPhase(int iPHIndex);
	void					SetActivePhase(int iPHIndex);
	void					AddRegion(StructPERegionData* pRegionData);
	void					Reset();

public:
	__declspec(property(get = GetIndex									))	int								Index;
	__declspec(property(get = GetActivePhase							))	CPublicEncounterPhase*			ActivePhase;
	__declspec(property(get = GetStatus			, put = SetStatus		))	int								Status;
	__declspec(property(get = GetScore			, put = SetScore		))	int								Score;
	__declspec(property(get = GetNameKeyStr								))	const char*						NameKeyStr;
	__declspec(property(get = GetDescKeyStr								))	const char*						DescriptionKeyStr;
	__declspec(property(get = GetFlag			, put = SetFlag			))	int								Flag;
	__declspec(property(get = GetScoreVisible	, put = SetScoreVisible	))	bool							ScoreVisible;
	__declspec(property(get = GetUIVisible		, put = SetUIVisible	))	bool							UIVisible;
	__declspec(property(get = GetRegions								))	vector<StructPERegionData>&		Regions;
	__declspec(property(get = GetPhases									))	vector<CPublicEncounterPhase*>&	Phases;
};

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
class PublicEncounterManager
{
protected:
	static map<int, CPublicEncounter*>	m_PEIndex;

protected:
	static void Clear();
	static CPublicEncounter* AddPE(StructPEBaseData* pBaseData);

public:
	static void LoadConfig(int iSize, PVOID pData);
	static void OnRegionChange(CRuWorld_ZoneDescriptor* zoneDescriptor);
	static CPublicEncounter* GetPEInstance(int iPEIndex);
	static CPublicEncounterPhase* GetPhaseInstance(int iPEIndex, int iPHIndex);
	static CPublicEncounterObjective* GetObjectiveInstance(int iPEIndex, int iPHIndex, int iOBIndex);
};

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
class CNetCli_PublicEncounterChild : public CNetCli_PublicEncounter
{
public:
	static bool _Init();
	static bool _Release();

public:
	virtual void OnRZ_PEConfigData			(int iSrvID, int iSize, PVOID pData);
	virtual void OnRZ_PEStatus				(int iSrvID, int iPEIndex, int iStatus);
	virtual void OnRZ_PhaseStatus			(int iSrvID, int iPEIndex, int iPHIndex, int iStatus);
	virtual void OnRZ_ObjectiveStatus		(int iSrvID, int iPEIndex, int iPHIndex, int iOBIndex, int iStatus);
	virtual void OnRZ_PlayerScore			(int iSrvID, int iPEIndex, int iScore);
	virtual void OnRZ_PEActivated			(int iSrvID, int iPEIndex, int iActivePhaseIndex);
	virtual void OnRZ_PEDeactivated			(int iSrvID, int iPEIndex);
	virtual void OnRZ_PEActivePhaseChange	(int iSrvID, int iPEIndex, int iActivePhaseIndex);
	virtual void OnRZ_ObjectiveValueChage	(int iSrvID, int iPEIndex, int iPHIndex, int iOBIndex, int iNowValue);
	virtual void OnRZ_PESuccess				(int iSrvID, int iPEIndex, int iTimeGap);
	virtual void OnRZ_PEFailed				(int iSrvID, int iPEIndex, int iTimeGap);
	virtual void OnRZ_PhaseSuccess			(int iSrvID, int iPEIndex, int iPHIndex, int iTimeGap);
	virtual void OnRZ_PhaseFailed			(int iSrvID, int iPEIndex, int iPHIndex, int iTimeGap);
	virtual void OnRZ_ObjectiveAchieved		(int iSrvID, int iPEIndex, int iPHIndex, int iOBIndex);
	virtual void OnRZ_ObjectiveFlag			(int iSrvID, int iPEIndex, int iPHIndex, int iOBIndex, int iFlag);
	virtual void OnRZ_PEFlag				(int iSrvID, int iPEIndex, int iFlag);
};