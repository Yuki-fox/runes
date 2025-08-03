#pragma once

#include "../interface.h"

class CTutorialFrame;
extern CTutorialFrame*			g_pTutorailFrame;



class CTutorialFrame : public CInterfaceBase
{
public:
	enum STATE
	{
		STATE_BACK,
		STATE_FORWARD,
		STATE_LEFT,
		STATE_RIGHT,
		STATE_TURNLEFT,
		STATE_TURNRIGHT,
		STATE_MOUSEMOVE,
		STATE_MOUSEROTATE,
		STATE_JUMP,
		STATE_MAX


	};
	CTutorialFrame(CInterface* object);
	virtual ~CTutorialFrame();


	

	void ClearAllState();

	void doState(int flag);
	int GetState(void);
	 	void doNewQuest(int iQuestID);

	int	GetQuestID(int questID);
	 void AddObject(int id);
	vector<int>& GetObject();
	int CheckAllObjects();
	float CheckObjDistance(int id);
	void RemoveObject(int id);
	bool TutorialStep(int step);
	void doClickNpc(int npc);
	int GetTouchNpc(void);
	virtual	void				RegisterFunc();
	virtual	void				TriggerTutorial(int id);
protected:	
	vector<int> m_idList;
	int  m_state;
	int m_npc;
	int m_questID;
	
};