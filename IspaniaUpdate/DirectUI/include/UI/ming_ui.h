#ifndef MING_UI_H
#define MING_UI_H

#pragma once

#include "UI_API.h"
struct SQStream;
struct IResource
{
	virtual SQStream* Create(LPCTSTR file)=0;
	virtual void Delete(SQStream*)=0;
};
class CControlUI;
bool UILIB_API DirectUI_Open(HINSTANCE hInstance);
void UILIB_API DirectUI_Close(void);
bool UILIB_API DirectUI_LoadMainWnd(LPCTSTR modlue);
void UILIB_API DirectUI_MainLoop(void);
void UILIB_API DirectUI_SetResource(IResource *IRes);
void UILIB_API DirectUI_SetColourDepth(int ColourDepth);


CControlUI*  DirectUI_DialogBuilder(LPCTSTR scriptNut);

LPCTSTR DirectUI_LoadFromResource(int id);

#endif//MING_UI_H