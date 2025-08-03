#pragma once

#include "../interface.h"

class CGiveItemFrame;
extern CGiveItemFrame*			g_pGiveItemFrame;

#define DF_GiveItemMax 4

class CGiveItemFrame : public CInterfaceBase
{
public:
	CGiveItemFrame(CInterface* object);
	virtual ~CGiveItemFrame();

	virtual	void RegisterFunc();
	virtual	void Update( float elapsedTime );
	void Pickup( int index );
	void PutDown( int index, int BagItemIndex );
	void OpenGiveItemFrame( bool IsSort , int Count , int TargetGUID );
	void CloseGiveItemFrame();
  	void S_GiveItem();

	void SetGiveItemIndex( int index, int BagItemIndex );
	int  GetGiveItemIndex( int index );
protected:
	bool m_IsSort;
	int  m_Count;
	int  m_npcID;


	int m_BagItem[DF_GiveItemMax];
	
};