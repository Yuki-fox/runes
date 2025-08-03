#pragma once

#include <string>
#include <vector>
#include "../interface.h"


class CRaidTargetFrame;
extern CRaidTargetFrame* g_pRaidTargetFrame;


class CRaidTargetFrame : public CInterfaceBase
{
public:
	CRaidTargetFrame(CInterface* object);
	virtual ~CRaidTargetFrame();


	virtual	void RegisterFunc();
	virtual void DestroyFrames();
	virtual	void Render();


	void Initialize( const char* pFrameName, int count );
	void ClearAllRaidTarget();

	int GetRaidTargetIndex	( const char* uint );
	void SetRaidTarget		( const char* uint, int icon );
	void R_SetRaidTarget    ( int targetID , int iconID );

	int GetIconID( int targetID );
 	CUiFrame* GetIconFrame( int targetID );
protected:

	struct SRaidTargeinfo
	{
		int TargetID;
		CUiFrame* pUiFrame;
		bool bLock;
	};
	SRaidTargeinfo* m_RaidTargeinfoList;
	int m_RaidTargeinfoCount;

protected:


};




