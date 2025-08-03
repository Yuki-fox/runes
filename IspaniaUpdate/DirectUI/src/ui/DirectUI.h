
#pragma once
#include <UI\ming_ui.h>

class  DirectUI
{
public:
	static void Open(void);
	static void Close(void);
	static bool LoadModule(LPCTSTR modlue);
	static void SetResource(IResource *IRes);

	static SQStream*  GetStream(LPCTSTR filename);
	static void ReleaseStream(SQStream* stream);
	static void  SetColourDepth(int ColourDepth);

	static HBITMAP LoadImage(LPCTSTR imagePath,int red,int green,int blue);
	static CControlUI*  DialogBuilder(LPCTSTR scriptNut);
};