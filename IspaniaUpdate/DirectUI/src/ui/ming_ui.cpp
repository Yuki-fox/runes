#include "stdafx.h"
#include <ui\MinG_ui.h>
#include <base\base_System.h>
#include "DirectUI.h"
//#include <GDIplus.h>
ULONG_PTR g_gdiplusToken;
bool DirectUI_Open(HINSTANCE hInstance)
{

	//Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	//Gdiplus::GdiplusStartup(&g_gdiplusToken, &gdiplusStartupInput, NULL);

	base::System::initial();
	 //CPaintManagerUI::SetResourceInstance(hInstance);
 CPaintManagerUI::SetResourceInstance((HINSTANCE)hInstance);

	 HRESULT Hr = ::CoInitialize(NULL);
	 if( FAILED(Hr) ) return false;

	 if( ::LoadLibrary(_T("d3d9.dll")) == NULL ) 
		 ::MessageBox(NULL, _T("Denne applikation vises bedst med DirectX 9 installeret!"), _T("Test"), MB_ICONINFORMATION);



	 DirectUI::Open();

	return true;
}
void  DirectUI_SetColourDepth(int ColourDepth)
{
DirectUI::SetColourDepth(ColourDepth);
}
void DirectUI_Close(void)
{

	 DirectUI::Close();

	 ::CoUninitialize();
	// Gdiplus::GdiplusShutdown(g_gdiplusToken);

	 base::System::finish();
}

void DirectUI_SetResource(IResource *IRes)
{

return  DirectUI::SetResource(IRes);
}


bool DirectUI_LoadMainWnd(LPCTSTR modlue)
{



	return  DirectUI::LoadModule(modlue);
}




void DirectUI_MainLoop(void)
{

	CPaintManagerUI::MessageLoop();

}

CControlUI*  DirectUI_DialogBuilder(LPCTSTR scriptNut)
{


	return DirectUI::DialogBuilder(scriptNut);

}

LPCTSTR DirectUI_LoadFromResource(int id)
{
	return CPaintManagerUI::LoadFromResource(id);


}