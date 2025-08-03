#pragma once
#include "../interface.h"
class CRoleSprite;
#define OBB_PLAYER_FLAG 1
#define OBB_NPC_FLAG 2
#define OBB_EMENY_FLAG 4
#define OBB_SELF_FLAG 8
#define OBB_TITLE_FLAG 16
class ObjectBloodBar:public CInterfaceBase 
{
	class imp;
public:
	ObjectBloodBar(CInterface* object );
	virtual ~ObjectBloodBar();
	void			RegisterFunc			();
	void			Render();
	void Load(void);
//	void SetBarValue(int id,int value);
	void AddObj(int objID);
	void LoadClientData(int type, int size, char* data);

	void ChangeHp(int objID);

	void ChangeGameID(int oldID,int newID);
	void ChangeName(int objID);

	void ChangeState(int objID);

	void TargetChange(int objID,INT64 oid);

	
	void BuildFrames(void);
	void DestroyFrames(void);


	BOOL IsTheSpriteHaveBloodBar(int objID);

	//void SetBarHide(int id);
	

	void OnBarClick(int objID);
	
	void DelObj(int objID);
	//int SetBloodBar(int Color,CRoleSprite* obj);
	void Clear(void);

	void SetVisable(int visableFlag);
	int  GetVisable(void);

	void SetDistance(int distance);
	int  GetDistance(void);

	

	void SetBloodBar(bool visible);
	bool GetBloodBar(void  );

	imp* m_imp;
};

extern ObjectBloodBar*		g_pObjectBloodBar;