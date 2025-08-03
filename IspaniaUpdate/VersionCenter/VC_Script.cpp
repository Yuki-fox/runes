#include "stdafx.h"
#include "VC_Script.h"
#include <sqplus.h>
#include "ReadUpdateIni.h"
#include "resource.h"


#include <UI\SQPageWnd.h>
#include <d3d9.h>
#include "DXUTenum.h"
void RuneConfig_Save(void);
void RunDownload(void);




_DECL_STATIC_NAMESPACE(VC); // Globals

_MEMBER_FUNCTION_IMPL(VC,RuneConfig_Save) 
{
	



	RuneConfig_Save();


	return 0 ;

}

_MEMBER_FUNCTION_IMPL(VC,RunDownload) 
{





	RunDownload();


	return 0 ;

}
_MEMBER_FUNCTION_IMPL(VC,FinisLauncher) 
{



	VC_FinisLauncher();


	return 0 ;

}


_MEMBER_FUNCTION_IMPL(VC,GetDLFileSizeNow) 
{
	StackHandler sa(v);

 
	float now=DLFile_GetNow();
	if (now<=0.0001)
		now=1;

	return sa.Return(now);

}
_MEMBER_FUNCTION_IMPL(VC,GetDLSpeed) 
{
	StackHandler sa(v);


	float now=DLFile_GetSpeed();
	if (now<=0.0001)
		now=1;

	return sa.Return(now);
}
_MEMBER_FUNCTION_IMPL(VC,MD5_GetPar)
{
	StackHandler sa(v);
	float  par=MD5_GetPar();

	if (par<=0)
		par=1;

	if (par>100)
		par=100;

	return sa.Return(par);
}
_MEMBER_FUNCTION_IMPL(VC,GetDLFileSizeMax) 
{
	StackHandler sa(v);
	float max=DLFile_GetMax();

	if (max<=0.0001)
		max=1;
	

	return sa.Return(max);

}

_MEMBER_FUNCTION_IMPL(VC,ExecuteFile) 
{
	VC_ExecuteFile();


	return 0;

}


_MEMBER_FUNCTION_IMPL(VC,CheckFileMD5) 
{
	VC_CheckFileMD5();


	return 0;

}


_MEMBER_FUNCTION_IMPL(VC,SetNewVersion) 
{
	VC_SetNewVersion();


	return 0;

}
_MEMBER_FUNCTION_IMPL(VC,PlayGame) 
{
	VC_SetPlayGame(true);


	return 0;

}

_MEMBER_FUNCTION_IMPL(VC,SelectZone) 
{


	StackHandler sa(v);
	int select=sa.GetInt(2);
	VC_SelectZone(select);


	return 0;

}/*
_MEMBER_FUNCTION_IMPL(VC,MoveWindows) 
{
	StackHandler sa(v);
	SquirrelObject ih=sa.GetObjectHandle(2);
	SQPageWnd *child=(SQPageWnd*)ih.GetInstanceUP(0);

}
*/
_MEMBER_FUNCTION_IMPL(VC,SetLink) 
{


	StackHandler sa(v);
	SquirrelObject ih=sa.GetObjectHandle(2);
	SQPageWnd *child=(SQPageWnd*)ih.GetInstanceUP(0);
	const TCHAR * link=sa.GetString(3);
	VC_SetLink(child->GetHWND(),link);


	return 0;

}
_MEMBER_FUNCTION_IMPL(VC,SetCountryAndLanguage) 
{
	TCHAR windowname[512];

	StackHandler sa(v);
	int country=sa.GetInt(2);
	int language=sa.GetInt(3);
	VC_SetCountryAndLanguage(country,language,windowname);

	return 0;
}
  

_MEMBER_FUNCTION_IMPL(VC,IsFirstInstall) 
{ 

	StackHandler sa(v);

	bool ret=	VC_IsFirstInstall();
	return sa.Return(ret);
	
}

_MEMBER_FUNCTION_IMPL(VC,GetDisplayCount) 
{ 
	
	CD3DEnumeration* pd3dEnum=DXUTGetEnumeration(); 
	StackHandler sa(v);
	int count=0;

	base::Array<CD3DEnumAdapterInfo*>& a=*pd3dEnum->GetAdapterInfoList();
	if (a.size()>1)
	{

		count=a[0]->displayModeList.size();
	}

	
	return sa.Return(count);
	



}
_MEMBER_FUNCTION_IMPL(VC,GetMode) 
{ 
	CD3DEnumeration* pd3dEnum=DXUTGetEnumeration(); 

	StackHandler sa(v);
	int index=sa.GetInt(2);
	int w=640;
	int h=480;

	base::Array<CD3DEnumAdapterInfo*>& a=*pd3dEnum->GetAdapterInfoList();
	if (a.size()>1)
	{

		CD3DEnumAdapterInfo* pA=a[0];

		if (index<pA->displayModeList.size())
		{
			D3DDISPLAYMODE &mode=pA->displayModeList[index];
			

			
			w=mode.Width;
			h=mode.Height;

		}
	}


	SquirrelObject oarray=SquirrelVM::CreateArray(0);


	oarray.ArrayAppend(w);
	oarray.ArrayAppend(h);



	return sa.Return(oarray);

}
_BEGIN_NAMESPACE(VC)
_MEMBER_FUNCTION(VC,RuneConfig_Save,0,0)
_MEMBER_FUNCTION(VC,RunDownload,0,0)
_MEMBER_FUNCTION(VC,FinisLauncher,0,0)
//_MEMBER_FUNCTION(VC,MoveWindows,0,0)

_MEMBER_FUNCTION(VC,MD5_GetPar,0,0)
_MEMBER_FUNCTION(VC,GetDLFileSizeNow,0,0)

_MEMBER_FUNCTION(VC,GetDLFileSizeMax,0,0)

_MEMBER_FUNCTION(VC,GetDLSpeed,0,0)
_MEMBER_FUNCTION(VC,ExecuteFile,0,0)
_MEMBER_FUNCTION(VC,CheckFileMD5,0,0)

_MEMBER_FUNCTION(VC,SetNewVersion,0,0)
_MEMBER_FUNCTION(VC,PlayGame,0,0)

_MEMBER_FUNCTION(VC,SelectZone,0,0)
_MEMBER_FUNCTION(VC,SetLink,0,0)
_MEMBER_FUNCTION(VC,SetCountryAndLanguage,3,_SC("tnn"))
_MEMBER_FUNCTION(VC,IsFirstInstall,0,0)
_MEMBER_FUNCTION(VC,GetDisplayCount,0,0)
_MEMBER_FUNCTION(VC,GetMode,2,_SC("tn"))


_BEGIN_NAMESPACE_CONSTANTS(VC)
 

_CONSTANT_IMPL(EM_CheckVersion,OT_INTEGER)
_CONSTANT_IMPL(EM_UpdateFailed,OT_INTEGER)
_CONSTANT_IMPL(EM_Downloading,OT_INTEGER)
_CONSTANT_IMPL(EM_CheckMD5ing,OT_INTEGER)
_CONSTANT_IMPL(EM_ExecuteFile,OT_INTEGER)
_CONSTANT_IMPL(EM_DownloadingOK,OT_INTEGER)
_CONSTANT_IMPL(EM_MD5CheckOK,OT_INTEGER)


_CONSTANT_IMPL(EM_VersionOK,OT_INTEGER)
_CONSTANT_IMPL(EM_UpdateOK,OT_INTEGER)

_CONSTANT_IMPL(WM_DLOK,OT_INTEGER)
_CONSTANT_IMPL(WM_DLFail,OT_INTEGER)
_CONSTANT_IMPL(WM_MD5OK,OT_INTEGER)
_CONSTANT_IMPL(WM_MD5Fail,OT_INTEGER)
_CONSTANT_IMPL(WM_ExecOK,OT_INTEGER)
_CONSTANT_IMPL(WM_ExecFail,OT_INTEGER)
_CONSTANT_IMPL(WM_UpdateOK,OT_INTEGER)
_CONSTANT_IMPL(WM_RegionError,OT_INTEGER)
_CONSTANT_IMPL(WM_VersionBetter,OT_INTEGER)



_CONSTANT_IMPL(WM_MOUSEMOVE,OT_INTEGER)
_CONSTANT_IMPL(WM_LBUTTONDOWN,OT_INTEGER)
_CONSTANT_IMPL(WM_LBUTTONUP,OT_INTEGER)
_CONSTANT_IMPL(WM_NCHITTEST,OT_INTEGER)
_CONSTANT_IMPL(IDI_ICON1,OT_INTEGER)


_CONSTANT_IMPL(HTCAPTION,OT_INTEGER)
_CONSTANT_IMPL(HTTRANSPARENT,OT_INTEGER)
_END_NAMESPACE(VC,NULL)

void VC_Script::Initial(void)
{

_INIT_STATIC_NAMESPACE(VC); 


}