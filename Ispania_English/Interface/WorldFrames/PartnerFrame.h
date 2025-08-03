#pragma once
#include "../interface.h"
struct GameObjDbStructEx;
class CUI_PartnerFrame:public  CInterfaceBase
{
public:
	CUI_PartnerFrame(CInterface* object);


	void RemovePartner(int mode,int index);
	int AddPartner(int index);
	int CanAddToPartner(int index);
	int GetPartnerInfo(int mode,int index);
	void UseExBagResult( bool Result );
	void PopExBagResult( int  Result );
	
	int GetPartnerCount(int mode);
	void PutInExBagResult(int Result);
	int CallPartner(int mode,int index);
	void _CallPartner(int mode,int index);
	void PickupAction(int mode,int index);


	int GetPartnerBuff(int mode,int index);
	GameObjDbStructEx* GetItem(int mode, int pos);
	int GetActionInfo(int mode,int index);
	bool  CanUsePartner(int mode,int index);
	
	void RegPetCardResult( bool result );
	virtual ~CUI_PartnerFrame();
	int m_mode;
	int m_pos;
	int m_id;
	int m_color[4];
	virtual	void				RegisterFunc();
};
extern CUI_PartnerFrame* g_pPartnerFrame;