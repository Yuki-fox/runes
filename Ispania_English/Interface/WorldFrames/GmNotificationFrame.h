#pragma once

#include <vector>
#include "../interface.h"

using namespace std;

class CGmNotificationFrame;
extern CGmNotificationFrame*			g_pGmNotificationFrame;

class CGmNotificationFrame : public CInterfaceBase
{
public:
	CGmNotificationFrame(CInterface* object);
	virtual ~CGmNotificationFrame();

	virtual	void RegisterFunc();
	virtual	void GmNotification(const char* pszSubject, const char* pszContent, const char* pszClassification);

};