#pragma once

#include "../interface.h"

class CPartyFrame;
extern CPartyFrame*				g_pPartyFrame;

struct InstanceServerInfo
{
	int liveTime;
	int extendTime;
	int keyId;
	int instanceKey;
	double requestFrameTime;
};

class CPartyFrame : public CInterfaceBase
{
public:
	CPartyFrame(CInterface* object);
	virtual ~CPartyFrame();

	virtual	void				Update(float elapsedTime);
	virtual	void				RegisterFunc();
	virtual	void				RefreshPartyMember();
	virtual	void				RefreshPartyLeader();

	virtual void				ClearBattleGroup();
	virtual void				SendResetInstance();
	virtual void				RequestInstanceRestTime(bool isForce = false);
	virtual void				SwitchExtendInstanceTime(int instanceIndex);
	virtual void				RL_InstanceRestTime( int ZoneID , int ExtendTime , int LiveTime , int KeyID );
	virtual void				RL_FixInstanceExtend( int ZoneID , int ExtendTime , int LiveTime , int KeyID );
	virtual void				RL_ResetInstanceResult(bool boResult);
	virtual void				RL_ResetInstanceInfo(int zoneID, bool boResult);
	virtual void				ClearInstanceServerInfo()					{ m_instanceServerInfo.clear(); }

protected:
	float						m_resetInstanceTime;
	bool						m_resetInstanceLock;
	int							m_resetInstanceCount;
	vector<int>					m_resetInstanceFailed;
	map<int, InstanceServerInfo>	m_instanceServerInfo;
};