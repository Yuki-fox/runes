#pragma once

#include "../interface.h"
#include "../../netwaker_member/Net_Other/NetCli_OtherChild.h"

using namespace std;

//********************************************************************
class CWeekInstanceFrame;
extern CWeekInstanceFrame* g_pWeekInstanceFrame;

//********************************************************************
enum WeekInstanceDayENUM
{
	EM_WeekInstanceDay_Sunday	 = 0,
	EM_WeekInstanceDay_Monday	 = 1,
	EM_WeekInstanceDay_Tuesday	 = 2,
	EM_WeekInstanceDay_Wednesday = 3,
	EM_WeekInstanceDay_Thuresday = 4,
	EM_WeekInstanceDay_Friday	 = 5,
	EM_WeekInstanceDay_Saturday  = 6,
};

struct WeekInstance
{
	int			ZoneID;
	std::string Name;
	int			Level;

	WeekInstance()
	{
		Init();
	}

	void Init()
	{
		ZoneID = 0;
		Name = "";
		Level = 0;
	}
};

struct WeekInstanceGroup
{
	std::vector<WeekInstance> Instances;

	WeekInstanceGroup()
	{
		Init();
	}

	void Init()
	{
		Instances.clear();
	}
};

//********************************************************************
class CWeekInstanceFrame : public CInterfaceBase
{
protected:
	std::map<int, std::map<int, WeekInstanceGroup>> m_Instances;
	WeekInstanceDayENUM								m_Today;
	WeekInstanceDayENUM								m_ShowDay;

public:
	CWeekInstanceFrame(CInterface* object);
	virtual ~CWeekInstanceFrame();

public:
	virtual	void RegisterFunc();

public:
	void Open();
	void Close();

public:
	WeekInstanceGroup* GetGroup(int id);

public:
	void RL_WeekInstances(int today, int count, WeekZoneStruct info[]);
	void RL_WeekInstancesTeleportResult(bool result);
	void RL_WeekInstancesReset(bool result);

public:
	WeekInstanceDayENUM	GetToday()								{return m_Today;}
	void				SetToday(WeekInstanceDayENUM value)		{m_Today = value;}
	WeekInstanceDayENUM	GetShowDay()							{return m_ShowDay;}
	void				SetShowDay(WeekInstanceDayENUM value)	{m_ShowDay = value;}

public:
	__declspec(property(get = GetToday	, put = SetToday	)) WeekInstanceDayENUM Today;
	__declspec(property(get = GetShowDay, put = SetShowDay	)) WeekInstanceDayENUM ShowDay;
};
//********************************************************************