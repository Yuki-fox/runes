#include "stdafx.h"
#include <tchar.h>
#include "ReadUpdateIni.h"
#include "zipResource.h"
#include "VC_Script.h"
#include "cmdlineargs.h"
#include <d3d9.h>
#include "DXUTenum.h"







#pragma comment( lib, "dxguid.lib" )
#pragma comment( lib, "d3d9.lib" )
#pragma comment( lib, "winmm.lib" )
#pragma comment( lib, "comctl32.lib" )
HANDLE g_hOneInstance;  

void GetDisplayMode()
{
	IDirect3D9* pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	CD3DEnumeration* pd3dEnum=DXUTGetEnumeration(); 

	pd3dEnum->SetResolutionMinMax(640,480,4096,2048);
	pd3dEnum->Enumerate( pD3D,NULL, NULL );

	pD3D->Release();
}


void GetDiskFree(TCHAR* path ,ULARGE_INTEGER& ulFree )
{
	ULARGE_INTEGER ulAvailable; 
	ULARGE_INTEGER ulTotal; 
	

	

	GetModuleFileName(NULL,path,512);
	int len=lstrlen(path);
	for(int i=len-1;i>0;--i)
	{
		if (path[i]=='\\')
		{

			path[i+1]=0;

			break;
		}

	}



	GetDiskFreeSpaceEx(path, &ulAvailable,&ulTotal, &ulFree); 

}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int nCmdShow) 
{ 

	GetDisplayMode();
	TCHAR  d_account[2048]={0};
	TCHAR  d_password[2048]={0};
	TCHAR PatchURL[2048]={0};
	TCHAR PatchPort[2048]={0};






	CmdLineArgs args;

	TCHAR windowname[2048];

	

	CZipResource *res=new CZipResource;


	DirectUI_Open(hInstance);

	VC_Script::Initial();



	UpdateIni_Load(PatchURL,PatchPort,windowname); 

	IniFile2& ini=UpdateIni_StringIni();
	ini.SetSection(_T("BOOT"));


	g_hOneInstance = ::CreateMutex(NULL, FALSE, _T("clientupdate"));  
	if(GetLastError() == ERROR_ALREADY_EXISTS)  
	{  
		//AfxMessageBox("ALREADY_EXISTS");  
		base::String context=ini.StrDef(_T("ALREADY_EXISTS_ERROR"),_T("clientupdate.exe now runing"));
		base::String title=ini.StrDef(_T("ALREADY_EXISTS_TITLE"),_T("ALREADY EXISTS"));


		if (args.size()<=2)
		MessageBox(NULL,context.c_str(),title.c_str(),0);
		return -1;  
	}  



	HDC hDC = GetDC(NULL);
	int ColourDepth = GetDeviceCaps(hDC, BITSPIXEL);
	ReleaseDC(NULL, hDC);

	DirectUI_SetColourDepth(ColourDepth);
	/*
	if (ColourDepth!=32)
	{
		TCHAR temp[512];

		base::String context=ini.StrDef(_T("DESKTOP_COLOR_DEPTH_ERROR"),_T("Color depth %d bits can't run"));
		base::String title=ini.StrDef(_T("DESKTOP_COLOR_DEPTH_TITLE"),_T("Desktop Need 32 bit depth"));
		

		wsprintf(temp,title.c_str() ,ColourDepth);

		

		MessageBox(NULL,temp,context.c_str(),0);
		return -1;  
	}
	*/


	
	if (args.size()>=2)
	{


		if (lstrcmp(args[1],_T("0963"))) 
		{
			return 0;
		}

		
		int count=args.size()-2;
		int i=0;
		while(count)
		{
			base::String temp=args[2+i];
			int len=temp.length();
			base::String str;
			int index=temp.indexOf(":");
			if (index!=-1)
			{
				str=temp.substring(0,index);

				if (str.compareTo("PatchIP")==0)
					lstrcpy(PatchURL,temp.substring(index+1,len).c_str());
				else if (str.compareTo("PatchPort")==0)
					lstrcpy(PatchPort,temp.substring(index+1,len).c_str());
			}
			else if (lstrlen(d_account)==0)
				lstrcpy(d_account,args[2+i]);
				
			else if (lstrlen(d_password)==0)
				lstrcpy(d_password,args[2+i]);
			


 


			count--;
			i++;
		}



	}
	else  
		return 0;




	TCHAR path[512];
	ULARGE_INTEGER ulFree;
	GetDiskFree(path,ulFree);


	
	
	ULARGE_INTEGER ulsize;
	ulsize.QuadPart=((double)3000.0*1024.0*1024.0);
	
 

//	if (ulFree.QuadPart<ulsize.QuadPart)

	if (0)
	{
		TCHAR temp[512];
		ini.SetSection(_T("BOOT"));

		base::String title=ini.StrDef(_T("NOT_ENOUGH_TITLE"),_T("Not Enough Free Disk"));
		base::String context=ini.StrDef(_T("NOT_ENOUGH_FREE_DISK"),_T("%s Not Enough Free Disk Space to Update %dM Bytes"));
		
		wsprintf(temp,context.c_str(), path,ulsize.QuadPart/(1024*1024));

		MessageBox(NULL,temp,title.c_str() ,0);
		return 0;
	}

	res->AddResource(_T("vcfile"),"Sh4dOw_!?_4mazl1ng:)");

	res->AddResource(_T("skin"),0);
	DirectUI_SetResource(res);
	if (DirectUI_LoadMainWnd( _T("Main.nut")))
		DirectUI_MainLoop();
	else
	{
		ini.SetSection(_T("BOOT"));
		base::String title=ini.StrDef(_T("NOT_VCFILE_TITLE"),_T("vcfile.res"));
		base::String context=ini.StrDef(_T("LOAD_MAINWND_ERROR"),_T("load MainWnd error"));

		MessageBox(NULL,context.c_str(),title.c_str(),0);
	}
	res->Finish();
 

delete res;
 
VC_FinisLauncher();

	UpdateIni_Finish();
	DirectUI_Close();

	
	VC_RunAgain(PatchURL,PatchPort,d_account,d_password);


	VC_PlayGame(d_account,d_password);



return 0;
}
