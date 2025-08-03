#pragma once

#include <string.h>
#include "../interface.h"

using namespace std;

class CLoadingFrame;
extern CLoadingFrame*				g_pLoadingFrame;

class CLoadingFrame : public CInterfaceBase
{
public:
	CLoadingFrame(CInterface* object);
	virtual ~CLoadingFrame();

	virtual	void				RegisterFunc();
	virtual void				BuildFrames();
	virtual void				DestroyFrames();
	virtual	void				Update(float elapsedTime);
	virtual	void				LoadingStart(bool forceReset=false);
	virtual	void				LoadingEnd(float delayTime);
	virtual	void				SetLoadingStatus(float value);
	virtual bool				GetLoadingState()				{ return m_isLoading; }

protected:
	virtual bool				FileExist(const char* pathName, string& outputString);

protected:
	bool						m_isLoading;
	float						m_delayEndTime;
	float						m_fadeDuration;

	CUiFrame*					m_loadingFrame;
	CUiTexture*					m_loadingScreen;
	CUiTexture*					m_loadingNameLogo;
	CUiTexture*					m_loadingMoliyoLogo;
	CUiTexture*					m_loadingCompanyLogo1;
	CUiTexture*					m_loadingCompanyLogo2;
	CUiTexture*					m_koreanImage1;
	CUiTexture*					m_koreanImage2;
	CUiStatusBar*				m_loadingBar;
	CUiTexture*					m_loadingTipBG;
	CUiFontString*				m_loadingTipText;
	CUiFontString*				m_koreanText;
	string						m_loadingFilename;
	vector<string>				m_loadingScreens;
};
