#pragma once

#include <string>
#include <vector>
#include "../interface.h"


class CEmoteFrame;
extern CEmoteFrame*				g_pEmoteFrame;

using namespace std;


struct SEmoteItem
{	
	int ID;		
	int MotionID;
	const char* pIconName;	
	const char* pName ;
	const char* pToken;
	const char* pMsg1 ;
	const char* pMsg2 ;
};

class CEmoteFrame : public CInterfaceBase
{
public:
	CEmoteFrame(CInterface* object);
	virtual ~CEmoteFrame();

	void InitEmoteDate();	

	virtual	void	RegisterFunc();
	virtual	void	Update(float elapsedTime);

	virtual	void	PickupEmoteItem(int serial);

	void DoEmote( int ID );
	void R_DoEmote( int sourceID , const char* szTargetName, int emoteID );

	int GetEmoteItemCount();
	SEmoteItem* GetEmoteItemByIndex( int index );
	SEmoteItem* GetEmoteItemByID( int ID );

	//���o�������O��icon
	//�`�N�n�X�h���r��u�O�Ȧs
	const char* GetEmoteIcon( int emoteID );
	const char* GetEmoteIcon( SEmoteItem* pItem );

protected:

protected:

	SEmoteItem* m_pEmoteItemList;
	int m_EmoteItemCount;

	typedef std::map<int,SEmoteItem*> TD_IDEmoteItemMap;

	TD_IDEmoteItemMap m_IDEmoteItemMap;
	float m_totalTime;							// �ֿn�o�e�ɶ�

	//const char* m_pIconPath;
	char _Temp[MAX_PATH];
};




