#pragma once

#include "../interface.h"
#include "../../netwaker_member/Net_Other/NetCli_OtherChild.h"

using namespace std;

//********************************************************************
class CPhantomFrame;
extern CPhantomFrame* g_pPhantomFrame;

//********************************************************************
class CPhantomFrame : public CInterfaceBase
{
protected:
	bool	   m_ForceShow;
	CUiLayout* m_PhantomTab;

public:
	CPhantomFrame(CInterface* object);
	virtual ~CPhantomFrame();

public:
	virtual	void Update(float elapsedTime);
	virtual	void RegisterFunc();

public:
	void Open();
	void Close();

public:
	void RL_PhantomRankUpResult(bool summon, bool result);
	void RL_PhantomLevelUpResult(bool result);
	void RL_PhantomEQResult(bool result);
	void RL_FixPhantom(int phantomid);
	void RL_FixPhantomEQ();
	void RL_OpenPhantomFrame();
	void R_FixItemInfo_Body();

public:
	void SetPhantomTab(CUiLayout* value) {m_PhantomTab = value;}
};
//********************************************************************