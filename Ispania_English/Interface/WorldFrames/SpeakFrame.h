#pragma once

#include "../interface.h"
#include <string>
#include <list>

using namespace std;

class CSpeakFrame;
extern CSpeakFrame*				g_pSpeakFrame;

struct SpeakInfo
{
	int			id;				// ����s��
	float		time;			// �ѩ�ɶ�

	CUiFrame*		frame;		// �ج[
	CUiFontString*	font;		// �r��
};

class CSpeakFrame : public CInterfaceBase
{
public:
	CSpeakFrame(CInterface* object);
	virtual ~CSpeakFrame();
	virtual	void				BuildFrames();
	virtual void				DestroyFrames();
	virtual	void				Update(float elapsedTime);	
	virtual	void				Render();
	virtual	void				AddText(int id, const char* text, float r, float g, float b);

protected:
	RuFontShadow				m_fontShadow;
	CRuFontType*				m_fontType;
	list<SpeakInfo>				m_data;
};

