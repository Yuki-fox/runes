#pragma once

#include "../interface.h"
#include <string>
#include <map>

using namespace std;

class CCastingBarFrame;
extern CCastingBarFrame*		g_pCastingBarFrame;

enum CastingBarType
{
	eCastingBar_None			= 0,
	eCastingBar_Quset			= 1,				// ����
	eCastingBar_SpellMagic		= 2,				// �I�k
	eCastingBar_Crafting		= 3,				// �Ķ�
	eCastingBar_UseItem			= 4,				// �ϥΪ��~
	eCastingBar_Teleport		= 5,				// �ǰe��
	eCastingBar_PetSummon		= 6,				// �l���d��
	eCastingBar_All				= 7,				// ����
};

class CCastingBarFrame : public CInterfaceBase
{
public:
	CCastingBarFrame(CInterface* object);
	virtual ~CCastingBarFrame();

	virtual	void				RegisterFunc();
	virtual	void				CastingStart(CastingBarType type, const char* text, float time);
	virtual	void				CastingStop(CastingBarType type);
	virtual	void				CastingFailed(CastingBarType type);
	virtual	void				CastingDelayed(CastingBarType type, float time);
	virtual	void				Clear();
	virtual	bool				IsCasting()		{ return m_isCasting; }
	virtual void				ChangeZone();

protected:
	CastingBarType				m_castingType;
	string						m_text;
	bool						m_isCasting;
	float						m_minTime;
	float						m_maxTime;
};