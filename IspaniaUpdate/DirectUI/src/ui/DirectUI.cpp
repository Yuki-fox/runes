#include "StdAfx.h"
#include "DirectUI.h"

#include <GDIplus.h>

#include <sqplus.h>

#include <sq_stdio.h>
#include "ImagelIb\cImage.h"
#ifndef _UNICODE
#define scstrcpy strcpy
#else
#define scstrcpy wcscpy
#endif

IResource* g_IRes;
int g_ColourDepth=32;
bool g_r5G6B5=false;
 void DirectUI::ReleaseStream(SQStream* file)
 {

g_IRes->Delete(file);

 }
SQStream*  DirectUI::GetStream(LPCTSTR filename)
{

	return g_IRes->Create(filename); 
}
LPCTSTR LoadBuffer(LPCTSTR filename)
{
	char * buff=NULL;
	int size;

	SQStream* file=DirectUI::GetStream(filename);
		
		
	
	if (file)
	{
		size=file->Len();
	
		buff=new char[size+2];
		memset(buff,0,size+2);
		
		file->Read(buff,size);
		DirectUI::ReleaseStream(file);

	}	
	return (LPCTSTR)buff;

}




void DirectUI_ParseChild(CControlUI* pControl,SquirrelObject&sqarray )
{
	IContainerUI*  pContainer = static_cast<IContainerUI*>(pControl->GetInterface(_T("Container")));
	if (!pContainer)
		return ;

	//sq array
	int count=sqarray.Len();
	for (int i = 0; i < count; i++) 
	{
		SquirrelObject ih=sqarray.GetValue(i);
		if (ih.GetType()==OT_INSTANCE)
		{
			CControlUI *child=(CControlUI*)ih.GetInstanceUP(0);
			pContainer->Add(child);
		}
	}
}
void DirectUI_Parse(CControlUI* pControl,SquirrelObject&sqtable )
{

	SquirrelObject key,value;

	SQChar temp[1024];
	memset(temp,0,sizeof(1024));
	
	if(sqtable.BeginIteration())
	{
		while(sqtable.Next(key,value)) 
		{
			//fp[idx.ToInteger()] = val.ToFloat();
			if (key.GetType()==OT_STRING )
			{
				if ( scstrcmp(key.ToString(),_SC("childs") )==0)
				{
					if (value.GetType()==OT_ARRAY)
						DirectUI_ParseChild(pControl,value);
				}
				else
				{
					switch(value.GetType())
					{
					case OT_STRING:
						scstrcpy(temp,value.ToString());					
						break;
					case OT_INTEGER:
						scsprintf(temp,_SC("%d"),value.ToInteger());					
						break;
					case OT_FLOAT:
						scsprintf(temp,_SC("%f"),value.ToFloat());					
						break;
					case OT_BOOL:
						scsprintf(temp,_SC("%s"),value.ToBool()?_SC("true"):_SC("false"));					
						break;
					case OT_NULL:
						scstrcpy(temp,_SC("NULL"));					
						break;
					}
					pControl->SetAttribute(key.ToString(),temp);
				}
			}
		}
		sqtable.EndIteration();
	}

}
void DirectUI_ContainerParse(CControlUI* pControl,HSQUIRRELVM v )
{
	StackHandler sa(v);
	int nparams = sa.GetParamCount();
	
	if (nparams==2)
	{
		SQObjectType type = (SQObjectType)sa.GetType(2);

		if (type == OT_TABLE ) 
		{
			SquirrelObject tab=sa.GetObjectHandle(2);
			DirectUI_Parse(pControl,tab);
		}
	}
}

int DirectUI_Release_Hook1(SQUserPointer p, int size)
{
	if(p)
	{ 
		SQWindowHost *pWnd = (SQWindowHost *)p; 
		base::Debug::printf(_T("SQ Delete Wnd"));
	//	delete pWnd;
		//pWnd->release(); 
		 
	} 
	return 0; 
}

int DirectUI_Release_Hook(SQUserPointer p, int size)
{
	if(p)
	{ 
		
		CControlUI *pUI = (CControlUI *)p; 
		base::Debug::printf(_T("SQ Delete UI" ));
		//delete pUI;
	//	pUI->release(); 
	} 
	return 0; 
}
int DirectUI_Construct_Release_Hook(CControlUI *pUI) 
{ 
	//pUI->addRef();
	sq_setinstanceup(SquirrelVM::GetVMPtr(),1,pUI); 
	sq_setreleasehook(SquirrelVM::GetVMPtr(),1,DirectUI_Release_Hook); 
	return 1; 
}
//namespace script 
//{

_DECL_STATIC_NAMESPACE(SQ); // Globals

_MEMBER_FUNCTION_IMPL(SQ,LoadBuffer) 
{
	StackHandler sa(v);

	int count=sa.GetParamCount();
	LPCTSTR filename=sa.GetString(2);

	LPCTSTR buff=	LoadBuffer(filename);
	if (buff)
	{
		sa.Return(buff+1);
		delete []buff;
		return 1 ;

	}
	sa.Return(NULL);
	return 1 ;
	
}
_MEMBER_FUNCTION_IMPL(SQ,LoadModule) 
{
	StackHandler sa(v);

	int count=sa.GetParamCount();
	LPCTSTR filename=sa.GetString(2);

	bool ret=DirectUI::LoadModule(filename);
	//scprintf(_T("GB.Update()\n"));
	sa.Return(ret);

	return 1 ;
}
_MEMBER_FUNCTION_IMPL(SQ,SetTimer) 
{
	StackHandler sa(v);

	int count=sa.GetParamCount();
	SquirrelObject ih=sa.GetObjectHandle(2);
	int ID=sa.GetInt(3);
	int epls=sa.GetInt(4);
	CControlUI *child=(CControlUI*)ih.GetInstanceUP(0);
	child->GetManager()->SetTimer(child,ID,epls);

	return 0 ;
}

	//WindowHost
_BEGIN_NAMESPACE(SQ)
_MEMBER_FUNCTION(SQ,LoadBuffer,0,0)
_MEMBER_FUNCTION(SQ,LoadModule,0,0)
_MEMBER_FUNCTION(SQ,SetTimer,3,_SC("xnn"))

_BEGIN_NAMESPACE_CONSTANTS(SQ)

//style
_CONSTANT_IMPL(UI_WNDSTYLE_CONTAINER,OT_INTEGER)
_CONSTANT(UI_LAUNCHER_FRAME,OT_INTEGER,(long int)(UI_LAUNCHER_FRAME))


_CONSTANT_IMPL(UI_WNDSTYLE_FRAME,OT_INTEGER)

_CONSTANT_IMPL(UI_WNDSTYLE_CHILD,OT_INTEGER)
_CONSTANT(UI_WNDSTYLE_DIALOG,OT_INTEGER,(long int)(UI_WNDSTYLE_DIALOG))
//ex style
_CONSTANT_IMPL(UI_WNDSTYLE_EX_FRAME,OT_INTEGER)
_CONSTANT_IMPL(UI_WNDSTYLE_EX_DIALOG,OT_INTEGER)

//classStyle
_CONSTANT_IMPL(UI_CLASSSTYLE_CONTAINER,OT_INTEGER)
_CONSTANT_IMPL(UI_CLASSSTYLE_FRAME,OT_INTEGER)
_CONSTANT_IMPL(UI_CLASSSTYLE_CHILD,OT_INTEGER)
_CONSTANT_IMPL(UI_CLASSSTYLE_DIALOG,OT_INTEGER)


_CONSTANT_IMPL(WM_CREATE,OT_INTEGER)

_CONSTANT_IMPL(WM_TIMER,OT_INTEGER)


_END_NAMESPACE(SQ,NULL)


void WindowHost_CreateParse(CWindowHost* pWindow,HSQUIRRELVM v )
{
	StackHandler sa(v);
	int nparams = sa.GetParamCount();

	
	if (nparams==2)
	{
		SQObjectType type = (SQObjectType)sa.GetType(2);

		if (type == OT_TABLE ) 
		{
			SquirrelObject tab=sa.GetObjectHandle(2);
			RECT rc={0};		

			LPCTSTR name=tab.GetString(_T("name"));
			DWORD style=tab.GetInt(_T("style"));
			DWORD exstyle=tab.GetInt(_T("exstyle"));
			rc.right=tab.GetInt(_T("width"));
			rc.bottom=tab.GetInt(_T("height"));
			CWindowHost *parent=NULL;
			SquirrelObject ih=tab.GetValue(_T("parentWnd"));
			if (ih.GetType()==OT_INSTANCE)
				parent=(CWindowHost *)ih.GetInstanceUP(0);

			HWND parenthWnd=parent ? 	parent->GetHWND():NULL;

			pWindow->Create(parenthWnd,name,style,exstyle,rc,0 );
		}
	}
}

//-----------TWindowHostUI----------------------
	_DECL_CLASS(WindowHost);	
	
	_MEMBER_FUNCTION_IMPL(WindowHost,Create) 
	{
		_CHECK_SELF(CWindowHost,WindowHost);	
		WindowHost_CreateParse(self,v);	
		//DirectUI::s_hWnd=(HWND)self;
		return SQ_OK;
	}
	_MEMBER_FUNCTION_IMPL(WindowHost,ShowWindow) 
	{
		_CHECK_SELF(CWindowHost,WindowHost);	
		StackHandler sa(v);
		self->ShowWindow(bool(sa.GetBool(2)),bool(sa.GetBool(3)));
		return SQ_OK;
	}
	_MEMBER_FUNCTION_IMPL(WindowHost,ShowMiniWindow) 
	{
		_CHECK_SELF(CWindowHost,WindowHost);	
		StackHandler sa(v);
		self->ShowMiniWindow(bool(sa.GetBool(2)),bool(sa.GetBool(3)));
		return SQ_OK;
	}
	_MEMBER_FUNCTION_IMPL(WindowHost,CenterWindow) 
	{
		_CHECK_SELF(CWindowHost,WindowHost);		
		self->CenterWindow();		
		return SQ_OK;
	}
	_MEMBER_FUNCTION_IMPL(WindowHost,Close) 
	{
		_CHECK_SELF(CWindowHost,WindowHost);		
		self->Close();		
		return SQ_OK;
	}
	_MEMBER_FUNCTION_IMPL(WindowHost,SetTimer) 
	{
		_CHECK_SELF(CWindowHost,WindowHost);	
			StackHandler sa(v);

		self->SetTimer(sa.GetInt(2),sa.GetInt(3));		
		return SQ_OK;
	}
	_MEMBER_FUNCTION_IMPL(WindowHost,KillTimer) 
	{
		_CHECK_SELF(CWindowHost,WindowHost);	
		StackHandler sa(v);

		self->KillTimer(sa.GetInt(2));		
		return SQ_OK;
	}


	_MEMBER_FUNCTION_IMPL(WindowHost,ShowModal) 
	{
		_CHECK_SELF(CWindowHost,WindowHost);		
		self->ShowModal();		
		return SQ_OK;
	}


	

	_MEMBER_FUNCTION_IMPL(WindowHost,SetWindowPos) 
	{
		_CHECK_SELF(CWindowHost,WindowHost);	
		StackHandler sa(v);
		self->SetWindowPos(sa.GetInt(2),sa.GetInt(3),sa.GetBool(4));
		return SQ_OK;
	}

	
		_MEMBER_FUNCTION_IMPL(WindowHost,SetIcon) 
	{
		_CHECK_SELF(CWindowHost,WindowHost);	
		StackHandler sa(v);
		self->SetIcon(sa.GetInt(2));		
		return SQ_OK;
	}

	_MEMBER_FUNCTION_IMPL(WindowHost,EndDialog) 
	{
		_CHECK_SELF(CWindowHost,WindowHost);	
		StackHandler sa(v);
		self->EndDialog(sa.GetInt(2));		
		return SQ_OK;
	}

	
	
	_BEGIN_CLASS(WindowHost)	

		_MEMBER_FUNCTION(WindowHost,Create,2,_T("xt"))	
		_MEMBER_FUNCTION(WindowHost,ShowWindow,3,_T("xbb"))		
		_MEMBER_FUNCTION(WindowHost,ShowMiniWindow,3,_T("xbb"))		
		
		_MEMBER_FUNCTION(WindowHost,CenterWindow,1,_T("x"))		
		_MEMBER_FUNCTION(WindowHost,Close,1,_T("x"))		
		_MEMBER_FUNCTION(WindowHost,SetTimer,3,_T("xnn"))		
		_MEMBER_FUNCTION(WindowHost,KillTimer,2,_T("xn"))		
		_MEMBER_FUNCTION(WindowHost,ShowModal,1,_T("x"))	
		_MEMBER_FUNCTION(WindowHost,SetWindowPos,4,_T("xnnb"))	
		_MEMBER_FUNCTION(WindowHost,SetIcon,2,_T("xn"))	


		_MEMBER_FUNCTION(WindowHost,EndDialog,2,_T("xn"))	

		

	_END_CLASS(WindowHost)
	
	//----------------------------------------------------------------

	_DECL_CLASS(FrameWnd);	
	
	_MEMBER_FUNCTION_IMPL(FrameWnd,constructor) 
	{

		SQFrameWnd * newv = NULL;
		StackHandler sa(v);
		SquirrelObject so=sa.GetObjectHandle(1);
		newv = new SQFrameWnd(so);
	//	newv->addRef();

		sq_setinstanceup(v,1,newv); 
		sq_setreleasehook(v,1,DirectUI_Release_Hook1); 

		return SQ_OK;
		
	}


	_MEMBER_FUNCTION_IMPL(FrameWnd,SetPage) 
	{
		_CHECK_SELF(SQFrameWnd,FrameWnd);	

		StackHandler sa(v);
		SquirrelObject ih=sa.GetObjectHandle(2);
		SQPageWnd *child=(SQPageWnd*)ih.GetInstanceUP(0);

		
		self->SetPage(child);		
		return SQ_OK;
	}
	_MEMBER_FUNCTION_IMPL(FrameWnd,Relayout) 
	{
		_CHECK_SELF(SQFrameWnd,FrameWnd);	
		StackHandler sa(v);
		SquirrelObject ih=sa.GetObjectHandle(2);
		SQPageWnd *child=(SQPageWnd*)ih.GetInstanceUP(0);

		self->Relayout(child);
		return SQ_OK;
	}
	_BEGIN_CLASS(FrameWnd)		
		_MEMBER_FUNCTION(FrameWnd,constructor,1,_T("x"))			
		_MEMBER_FUNCTION(FrameWnd,SetPage,2,_T("xx"))	
		_MEMBER_FUNCTION(FrameWnd,Relayout,2,_T("xx"))	
	_END_CLASS_INHERITANCE(FrameWnd,WindowHost)

	//page
	_DECL_CLASS(PageWnd);	
	_MEMBER_FUNCTION_IMPL(PageWnd,constructor) 
	{

		SQPageWnd * newv = NULL;
		StackHandler sa(v);
		SquirrelObject so=sa.GetObjectHandle(1);
		newv = new SQPageWnd(so);

		//newv->addRef();
		sq_setinstanceup(v,1,newv); 
		sq_setreleasehook(v,1,DirectUI_Release_Hook1); 

		return SQ_OK;

	}

	
	_MEMBER_FUNCTION_IMPL(PageWnd,SetBackGround) 
	{

		_CHECK_SELF(SQPageWnd,PageWnd);	
		StackHandler sa(v);
		LPCTSTR name=sa.GetString(2);
		int r=sa.GetInt(3);
		int g=sa.GetInt(4);
		int b=sa.GetInt(5);


		self->SetBackGround(name,r,g,b);


		return SQ_OK;
	}
	_MEMBER_FUNCTION_IMPL(PageWnd,SetBackGroundRegion) 
	{

		_CHECK_SELF(SQPageWnd,PageWnd);	
		StackHandler sa(v);
		bool visible=sa.GetBool(2);
	


		self->SetBackGroundRegion(visible);


		return SQ_OK;
	}
	


	_BEGIN_CLASS(PageWnd)
		_MEMBER_FUNCTION(PageWnd,constructor,1,_T("x"))	
		_MEMBER_FUNCTION(PageWnd,SetBackGround,5,_T("xsnnn"))	
		_MEMBER_FUNCTION(PageWnd,SetBackGroundRegion,2,_T("xb"))			
	_END_CLASS_INHERITANCE(PageWnd,WindowHost)
	//Control

	_DECL_CLASS(Control);	
	_MEMBER_FUNCTION_IMPL(Control,SetPos) 
	{

		_CHECK_SELF(CControlUI,Control);	
		StackHandler sa(v);
		RECT rc;
		rc.left=sa.GetInt(2);
		rc.top=sa.GetInt(3);
		rc.right=sa.GetInt(4);
		rc.bottom=sa.GetInt(5);

		self->SetPos(rc);		


		return SQ_OK;
	}
	_MEMBER_FUNCTION_IMPL(Control,GetName) 
	{

		_CHECK_SELF(CControlUI,Control);	
		StackHandler sa(v);
	
		base::String name=self->GetName();		
		


		return sa.Return(name.c_str());
	}

	_MEMBER_FUNCTION_IMPL(Control,GetText) 
	{

		_CHECK_SELF(CControlUI,Control);	
		StackHandler sa(v);

		base::String text=self->GetText();		



		return sa.Return(text.c_str());
	}

	_MEMBER_FUNCTION_IMPL(Control,SetID) 
	{

		_CHECK_SELF(CControlUI,Control);	
		StackHandler sa(v);
		int ID=sa.GetInt(2);

		self->SetTag(ID);	

		



		return SQ_OK;
	}
	_MEMBER_FUNCTION_IMPL(Control,GetID) 
	{

		_CHECK_SELF(CControlUI,Control);	
		StackHandler sa(v);

		int ID=self->GetTag();		



		return sa.Return(ID);
	}

	_MEMBER_FUNCTION_IMPL(Control,SetText) 
	{

		_CHECK_SELF(CControlUI,Control);	
		StackHandler sa(v);

		base::String name=sa.GetString(2);

		self->SetText(name);		



		return SQ_OK;
	}

	_MEMBER_FUNCTION_IMPL(Control,SetEnabled) 
	{

		_CHECK_SELF(CControlUI,Control);	
		StackHandler sa(v);

		BOOL enable=sa.GetBool(2);

		self->SetEnabled(enable);		



		return SQ_OK;
	}

	_BEGIN_CLASS(Control)		

		_MEMBER_FUNCTION(Control,SetPos,5,_T("xnnnn"))	
		_MEMBER_FUNCTION(Control,GetName,1,_T("x"))	
		
		_MEMBER_FUNCTION(Control,SetID,2,_T("xn"))	
		_MEMBER_FUNCTION(Control,GetID,1,_T("x"))	
	
		_MEMBER_FUNCTION(Control,SetText,2,_T("xs"))	
		_MEMBER_FUNCTION(Control,GetText,1,_T("x"))	
		_MEMBER_FUNCTION(Control,SetEnabled,2,_T("xb"))	


	_END_CLASS(Control)

	//定義ActiveX.
	_DECL_CLASS(ImagePanel);	
		_MEMBER_FUNCTION_IMPL(ImagePanel,constructor) 
		{
			CImagePanelUI * newv = NULL;
			StackHandler sa(v);

			SquirrelObject so=sa.GetObjectHandle(1);
			newv = new CImagePanelUI(so);

			DirectUI_ContainerParse(newv,v);

			//	newv->DelayedControlCreation();

			return DirectUI_Construct_Release_Hook(newv);

		}
		_MEMBER_FUNCTION_IMPL(ImagePanel,SetImage) 
		{
			_CHECK_SELF(CImagePanelUI,ImagePanel);	
			StackHandler sa(v);
			LPCTSTR imagePath=sa.GetString(2);


			self->SetImage(imagePath);		


			return SQ_OK;
		}

	_BEGIN_CLASS(ImagePanel)		
		_MEMBER_FUNCTION(ImagePanel,constructor,2,_T("xt"))
		_MEMBER_FUNCTION(ImagePanel,SetImage,2,_T("xs"))

	_END_CLASS_INHERITANCE(ImagePanel,Control)

	 //定義ActiveX.
	_DECL_CLASS(WebActiveX);	
	//實做ActiveX.

	_MEMBER_FUNCTION_IMPL(WebActiveX,constructor) 
	{

		CWebActiveXUI * newv = NULL;
		StackHandler sa(v);

		SquirrelObject so=sa.GetObjectHandle(1);
		newv = new CWebActiveXUI(so);

		DirectUI_ContainerParse(newv,v);

	//	newv->DelayedControlCreation();

		return DirectUI_Construct_Release_Hook(newv);

	}
	_MEMBER_FUNCTION_IMPL(WebActiveX,Navigate) 
	{
		_CHECK_SELF(CWebActiveXUI,WebActiveX);	
		StackHandler sa(v);
		LPCTSTR url=sa.GetString(2);
	

		self->Navigate(url);		


		return SQ_OK;
	}
	//ActiveX函式表.
	_BEGIN_CLASS(WebActiveX)		

		_MEMBER_FUNCTION(WebActiveX,constructor,2,_T("xt"))
		_MEMBER_FUNCTION(WebActiveX,Navigate,2,_T("xs"))
	_END_CLASS_INHERITANCE(WebActiveX,Control)

	


	//Option

	_DECL_CLASS(Option);
	_MEMBER_FUNCTION_IMPL(Option,constructor) 
	{
		COptionUI * newv = NULL;
		StackHandler sa(v);

		SquirrelObject so=sa.GetObjectHandle(1);
		newv = new COptionUI(so);

		DirectUI_ContainerParse(newv,v);

		return DirectUI_Construct_Release_Hook(newv);
	}
	_BEGIN_CLASS(Option)
		_MEMBER_FUNCTION(Option,constructor,2,_T("xt")) // x = instance ('self/this' not yet created), no arguments.	
	_END_CLASS_INHERITANCE(Option,Control)		

	//Check

	_DECL_CLASS(Check);
	_MEMBER_FUNCTION_IMPL(Check,constructor) 
	{
		CCheckUI * newv = NULL;
		StackHandler sa(v);

		SquirrelObject so=sa.GetObjectHandle(1);
		newv = new CCheckUI(so);

		DirectUI_ContainerParse(newv,v);

		return DirectUI_Construct_Release_Hook(newv);
	}
	_MEMBER_FUNCTION_IMPL(Check,IsChecked) 
	{
		_CHECK_SELF(CCheckUI,Check);	
		StackHandler sa(v);
		bool c =self->IsChecked();		


		return sa.Return(c);
	}
	_MEMBER_FUNCTION_IMPL(Check,SetCheck) 
	{
		_CHECK_SELF(CCheckUI,Check);	
		StackHandler sa(v);
		bool c=sa.GetBool(2);


		self->SetCheck(c);		


		return SQ_OK;
	}

	_BEGIN_CLASS(Check)
		_MEMBER_FUNCTION(Check,constructor,2,_T("xt")) // x = instance ('self/this' not yet created), no arguments.	
		_MEMBER_FUNCTION(Check,IsChecked,1,_T("x"))
		_MEMBER_FUNCTION(Check,SetCheck,2,_T("xb"))
	_END_CLASS_INHERITANCE(Check,Control)		



	

//DropDown
	_DECL_CLASS(DropDown);
	_MEMBER_FUNCTION_IMPL(DropDown,constructor) 
	{
		CDropDownUI * newv = NULL;
		StackHandler sa(v);

		SquirrelObject so=sa.GetObjectHandle(1);
		newv = new CDropDownUI(so);

		DirectUI_ContainerParse(newv,v);

		return DirectUI_Construct_Release_Hook(newv);
	}
	_MEMBER_FUNCTION_IMPL(DropDown,SelectItem) 
	{
		_CHECK_SELF(CDropDownUI,DropDown);	
		StackHandler sa(v);
		int s=sa.GetInt(2);


		self->SelectItem(s);		


		return SQ_OK;
	}

	_MEMBER_FUNCTION_IMPL(DropDown,GetCurSel) 
	{
		_CHECK_SELF(CDropDownUI,DropDown);	
		StackHandler sa(v);
		int s=	self->GetCurSel();		


		return sa.Return(s);
	}
	_MEMBER_FUNCTION_IMPL(DropDown,SetDropDownSize) 
	{
		_CHECK_SELF(CDropDownUI,DropDown);	
		StackHandler sa(v);
		SIZE szDropBox;

		szDropBox.cx=sa.GetInt(2);
		szDropBox.cy=sa.GetInt(3);

		self->SetDropDownSize(szDropBox);

		return SQ_OK;
	}

	_MEMBER_FUNCTION_IMPL(DropDown,RemoveAll) 
	{
		_CHECK_SELF(CDropDownUI,DropDown);	
		

		self->RemoveAll();

		return SQ_OK;
	}
	
	_MEMBER_FUNCTION_IMPL(DropDown,Add) 
	{
		_CHECK_SELF(CDropDownUI,DropDown);	

		StackHandler sa(v);
		SquirrelObject ih=sa.GetObjectHandle(2);
		CControlUI *child=(CControlUI*)ih.GetInstanceUP(0);


		self->Add(child);		
		return SQ_OK;
	}
	_BEGIN_CLASS(DropDown)
		_MEMBER_FUNCTION(DropDown,constructor,2,_T("xt")) // x = instance ('self/this' not yet created), no arguments.	

		_MEMBER_FUNCTION(DropDown,SelectItem,2,_T("xn"))
		_MEMBER_FUNCTION(DropDown,GetCurSel,1,_T("x"))
		_MEMBER_FUNCTION(DropDown,SetDropDownSize,3,_T("xnn"))
		_MEMBER_FUNCTION(DropDown,RemoveAll,1,_T("x"))
		_MEMBER_FUNCTION(DropDown,Add,2,_T("xx"))
		
	_END_CLASS_INHERITANCE(DropDown,Container)		




	
//Button



	_DECL_CLASS(Button);
	_MEMBER_FUNCTION_IMPL(Button,constructor) 
	{
		CButtonUI * newv = NULL;
		StackHandler sa(v);

		SquirrelObject so=sa.GetObjectHandle(1);
		newv = new CButtonUI(so);
		
		DirectUI_ContainerParse(newv,v);

		return DirectUI_Construct_Release_Hook(newv);
	}
	_BEGIN_CLASS(Button)
		_MEMBER_FUNCTION(Button,constructor,2,_T("xt")) // x = instance ('self/this' not yet created), no arguments.	
	_END_CLASS_INHERITANCE(Button,Control)		

	_DECL_CLASS(SQButton);
	_MEMBER_FUNCTION_IMPL(SQButton,constructor) 
	{
		SQButtonUI * newv = NULL;
		StackHandler sa(v);

		SquirrelObject so=sa.GetObjectHandle(1);
		newv = new SQButtonUI(so);

		DirectUI_ContainerParse(newv,v);

		return DirectUI_Construct_Release_Hook(newv);
	}

	_MEMBER_FUNCTION_IMPL(SQButton,SetImage) 
	{
		_CHECK_SELF(SQButtonUI,SQButton);	
		StackHandler sa(v);
		int value=sa.GetInt(2);
		const SQChar*path=sa.GetString(3);


		self->SetImage(value,path);		


		return SQ_OK;
	}

	_BEGIN_CLASS(SQButton)
		_MEMBER_FUNCTION(SQButton,constructor,2,_T("xt")) // x = instance ('self/this' not yet created), no arguments.	
		_MEMBER_FUNCTION(SQButton,SetImage,3,_T("xns")) 
	_END_CLASS_INHERITANCE(SQButton,Button)		

//===================SliderBar================
	_DECL_CLASS(SliderBar);
	_MEMBER_FUNCTION_IMPL(SliderBar,constructor) 
	{
		CSliderBarUI * newv = NULL;
		StackHandler sa(v);

		SquirrelObject so=sa.GetObjectHandle(1);
		newv = new CSliderBarUI(so);

		DirectUI_ContainerParse(newv,v);

		return DirectUI_Construct_Release_Hook(newv);
	}
	_MEMBER_FUNCTION_IMPL(SliderBar,SetValue) 
	{
		_CHECK_SELF(CSliderBarUI,SliderBar);	
		StackHandler sa(v);
		int value=sa.GetInt(2);


		self->SetValue(value);		


		return SQ_OK;
	}
	_MEMBER_FUNCTION_IMPL(SliderBar,GetValue) 
	{
		_CHECK_SELF(CSliderBarUI,SliderBar);	
		StackHandler sa(v);
		


	return	sa.Return(self->GetValue());

		

		
	}
	_MEMBER_FUNCTION_IMPL(SliderBar,SetRange) 
	{
		_CHECK_SELF(CSliderBarUI,SliderBar);	
		StackHandler sa(v);
	    int min=sa.GetInt(2);
		 int max=sa.GetInt(3);


		self->SetRange(min,max);		


		return SQ_OK;
	}
	_BEGIN_CLASS(SliderBar)
		_MEMBER_FUNCTION(SliderBar,constructor,2,_T("xt")) // x = instance ('self/this' not yet created), no arguments.	
		_MEMBER_FUNCTION(SliderBar,SetValue,2,_T("xn"))
		_MEMBER_FUNCTION(SliderBar,GetValue,1,_T("x"))
		_MEMBER_FUNCTION(SliderBar,SetRange,3,_T("xnn"))
	_END_CLASS_INHERITANCE(SliderBar,Control)		



	//=============ProgressBar===============
	_DECL_CLASS(ProgressBar);

	_MEMBER_FUNCTION_IMPL(ProgressBar,constructor) 
	{
		CProgressBarUI * newv = NULL;
		StackHandler sa(v);

		SquirrelObject so=sa.GetObjectHandle(1);
		newv = new CProgressBarUI(so);

		DirectUI_ContainerParse(newv,v);

		return DirectUI_Construct_Release_Hook(newv);
	}
	_MEMBER_FUNCTION_IMPL(ProgressBar,SetValue) 
	{
		_CHECK_SELF(CProgressBarUI,ProgressBar);	
		StackHandler sa(v);
		int value=sa.GetInt(2);


		self->SetValue(value);		


		return SQ_OK;
	}
	_MEMBER_FUNCTION_IMPL(ProgressBar,GetValue) 
	{
		_CHECK_SELF(CProgressBarUI,ProgressBar);	
		StackHandler sa(v);



		return	sa.Return(self->GetValue());




	}
	_MEMBER_FUNCTION_IMPL(ProgressBar,SetMax) 
	{
		_CHECK_SELF(CProgressBarUI,ProgressBar);	
		StackHandler sa(v);
		int max=sa.GetInt(2);
		


		self->SetMax(max);		


		return SQ_OK;
	}

	_MEMBER_FUNCTION_IMPL(ProgressBar,SetImage) 
	{
		_CHECK_SELF(CProgressBarUI,ProgressBar);	
		StackHandler sa(v);
		LPCTSTR nameImage=sa.GetString(2);



		self->SetImage(nameImage);		


		return SQ_OK;
	}



	_BEGIN_CLASS(ProgressBar)
		_MEMBER_FUNCTION(ProgressBar,constructor,2,_T("xt")) // x = instance ('self/this' not yet created), no arguments.	
		_MEMBER_FUNCTION(ProgressBar,SetValue,2,_T("xn"))
		_MEMBER_FUNCTION(ProgressBar,GetValue,1,_T("x"))
		_MEMBER_FUNCTION(ProgressBar,SetMax,2,_T("xn"))
		_MEMBER_FUNCTION(ProgressBar,SetImage,2,_T("xs"))

		
	_END_CLASS_INHERITANCE(ProgressBar,Control)	










	_DECL_CLASS(FadedLine);

	_MEMBER_FUNCTION_IMPL(FadedLine,constructor) 
	{
		CFadedLineUI * newv = NULL;
		StackHandler sa(v);

		SquirrelObject so=sa.GetObjectHandle(1);
		newv = new CFadedLineUI(so);

		DirectUI_ContainerParse(newv,v);

		return DirectUI_Construct_Release_Hook(newv);
	}
	_BEGIN_CLASS(FadedLine)
		_MEMBER_FUNCTION(FadedLine,constructor,2,_T("xt")) // x = instance ('self/this' not yet created), no arguments.	
	_END_CLASS_INHERITANCE(FadedLine,Control)	

	_DECL_CLASS(LabelPanel);

	_MEMBER_FUNCTION_IMPL(LabelPanel,constructor) 
	{
		CLabelPanelUI * newv = NULL;
		StackHandler sa(v);

		SquirrelObject so=sa.GetObjectHandle(1);
		newv = new CLabelPanelUI(so);

		DirectUI_ContainerParse(newv,v);

		return DirectUI_Construct_Release_Hook(newv);
	}
	_BEGIN_CLASS(LabelPanel)
		_MEMBER_FUNCTION(LabelPanel,constructor,2,_T("xt")) // x = instance ('self/this' not yet created), no arguments.	
	_END_CLASS_INHERITANCE(LabelPanel,Control)	

	_DECL_CLASS(TextPanel);

	_MEMBER_FUNCTION_IMPL(TextPanel,constructor) 
	{
		CTextPanelUI * newv = NULL;
		StackHandler sa(v);

		SquirrelObject so=sa.GetObjectHandle(1);
		newv = new CTextPanelUI(so);

		DirectUI_ContainerParse(newv,v);

		return DirectUI_Construct_Release_Hook(newv);
	}
	_BEGIN_CLASS(TextPanel)
		_MEMBER_FUNCTION(TextPanel,constructor,2,_T("xt")) // x = instance ('self/this' not yet created), no arguments.	
	_END_CLASS_INHERITANCE(TextPanel,LabelPanel)	


	_DECL_CLASS(WarningPanel);

	_MEMBER_FUNCTION_IMPL(WarningPanel,constructor) 
	{
		CWarningPanelUI * newv = NULL;
		StackHandler sa(v);

		SquirrelObject so=sa.GetObjectHandle(1);
		newv = new CWarningPanelUI(so);

		DirectUI_ContainerParse(newv,v);

		return DirectUI_Construct_Release_Hook(newv);
	}
	_BEGIN_CLASS(WarningPanel)
		_MEMBER_FUNCTION(WarningPanel,constructor,2,_T("xt")) // x = instance ('self/this' not yet created), no arguments.	
	_END_CLASS_INHERITANCE(WarningPanel,LabelPanel)	



	

	_DECL_CLASS(ToolbarTitlePanel);

	_MEMBER_FUNCTION_IMPL(ToolbarTitlePanel,constructor) 
	{
		CToolbarTitlePanelUI * newv = NULL;
		StackHandler sa(v);

		SquirrelObject so=sa.GetObjectHandle(1);
		newv = new CToolbarTitlePanelUI(so);

		DirectUI_ContainerParse(newv,v);

		return DirectUI_Construct_Release_Hook(newv);
	}
	_BEGIN_CLASS(ToolbarTitlePanel)
		_MEMBER_FUNCTION(ToolbarTitlePanel,constructor,2,_T("xt")) // x = instance ('self/this' not yet created), no arguments.	
	_END_CLASS_INHERITANCE(ToolbarTitlePanel,Control)
	
	_DECL_CLASS(ListElement);

	
	_BEGIN_CLASS(ListElement)
		
	_END_CLASS_INHERITANCE(ListElement,Control)	


	_DECL_CLASS(SingleLineEdit);
	_MEMBER_FUNCTION_IMPL(SingleLineEdit,constructor) 
	{
		CSingleLineEditUI * newv = NULL;
		StackHandler sa(v);

		SquirrelObject so=sa.GetObjectHandle(1);
		newv = new CSingleLineEditUI(so);

		DirectUI_ContainerParse(newv,v);

		return DirectUI_Construct_Release_Hook(newv);
	}

	_BEGIN_CLASS(SingleLineEdit)
		_MEMBER_FUNCTION(SingleLineEdit,constructor,2,_T("xt")) // x = instance ('self/this' not yet created), no arguments.	
	_END_CLASS_INHERITANCE(SingleLineEdit,Control)	

	

	_DECL_CLASS(NavigatorButton);

	_MEMBER_FUNCTION_IMPL(NavigatorButton,constructor) 
	{
		CNavigatorButtonUI * newv = NULL;
		StackHandler sa(v);

		SquirrelObject so=sa.GetObjectHandle(1);
		newv = new CNavigatorButtonUI(so);

		DirectUI_ContainerParse(newv,v);

		return DirectUI_Construct_Release_Hook(newv);
	}
	_BEGIN_CLASS(NavigatorButton)
		_MEMBER_FUNCTION(NavigatorButton,constructor,2,_T("xt")) // x = instance ('self/this' not yet created), no arguments.	
	_END_CLASS_INHERITANCE(NavigatorButton,ListElement)	


	//ListLabelElement
	_DECL_CLASS(ListLabelElement);

	_MEMBER_FUNCTION_IMPL(ListLabelElement,constructor) 
	{
		CListLabelElementUI * newv = NULL;
		StackHandler sa(v);

		SquirrelObject so=sa.GetObjectHandle(1);
		newv = new CListLabelElementUI(so);

		DirectUI_ContainerParse(newv,v);

		return DirectUI_Construct_Release_Hook(newv);
	}
	_BEGIN_CLASS(ListLabelElement)
		_MEMBER_FUNCTION(ListLabelElement,constructor,2,_T("xt")) // x = instance ('self/this' not yet created), no arguments.	
	_END_CLASS_INHERITANCE(ListLabelElement,ListElement)	
	
		

	//ListHeaderItem

	_DECL_CLASS(ListHeaderItem);

	_MEMBER_FUNCTION_IMPL(ListHeaderItem,constructor) 
	{
		CListHeaderItemUI * newv = NULL;
		StackHandler sa(v);

		SquirrelObject so=sa.GetObjectHandle(1);
		newv = new CListHeaderItemUI(so);

		DirectUI_ContainerParse(newv,v);

		return DirectUI_Construct_Release_Hook(newv);
	}
	_BEGIN_CLASS(ListHeaderItem)
		_MEMBER_FUNCTION(ListHeaderItem,constructor,2,_T("xt")) // x = instance ('self/this' not yet created), no arguments.	
	_END_CLASS_INHERITANCE(ListHeaderItem,Control)		

		

	_DECL_CLASS(ToolGripper);

	_MEMBER_FUNCTION_IMPL(ToolGripper,constructor) 
	{
		CToolGripperUI * newv = NULL;
		StackHandler sa(v);

		SquirrelObject so=sa.GetObjectHandle(1);
		newv = new CToolGripperUI(so);

		DirectUI_ContainerParse(newv,v);

		return DirectUI_Construct_Release_Hook(newv);
	}
	_BEGIN_CLASS(ToolGripper)
		_MEMBER_FUNCTION(ToolGripper,constructor,2,_T("xt")) // x = instance ('self/this' not yet created), no arguments.	
	_END_CLASS_INHERITANCE(ToolGripper,Control)		

	

	_DECL_CLASS(PaddingPanel);

	_MEMBER_FUNCTION_IMPL(PaddingPanel,constructor) 
	{
		CPaddingPanelUI * newv = NULL;
		StackHandler sa(v);

		SquirrelObject so=sa.GetObjectHandle(1);
		newv = new CPaddingPanelUI(so);

		DirectUI_ContainerParse(newv,v);

		return DirectUI_Construct_Release_Hook(newv);
	}
	_BEGIN_CLASS(PaddingPanel)
		_MEMBER_FUNCTION(PaddingPanel,constructor,2,_T("xt")) // x = instance ('self/this' not yet created), no arguments.	
	_END_CLASS_INHERITANCE(PaddingPanel,Control)			

	_DECL_CLASS(TitleShadow);

	_MEMBER_FUNCTION_IMPL(TitleShadow,constructor) 
	{
		CTitleShadowUI * newv = NULL;
		StackHandler sa(v);

		SquirrelObject so=sa.GetObjectHandle(1);
		newv = new CTitleShadowUI(so);

		DirectUI_ContainerParse(newv,v);

		return DirectUI_Construct_Release_Hook(newv);
	}
	_BEGIN_CLASS(TitleShadow)
		_MEMBER_FUNCTION(TitleShadow,constructor,2,_T("xt")) // x = instance ('self/this' not yet created), no arguments.	
	_END_CLASS_INHERITANCE(TitleShadow,TextPanel)	

	
	_DECL_CLASS(Container);

	_MEMBER_FUNCTION_IMPL(Container,constructor) 
	{
		CContainerUI * newv = NULL;
		StackHandler sa(v);

		SquirrelObject so=sa.GetObjectHandle(1);
		newv = new CContainerUI(so);
		DirectUI_ContainerParse(newv,v);
		return DirectUI_Construct_Release_Hook(newv);
	}
	_MEMBER_FUNCTION_IMPL(Container,SetWidth) 
	{
		StackHandler sa(v);
		_CHECK_SELF(CContainerUI,Container);		
		int nparams = sa.GetParamCount();
		self->SetWidth(sa.GetInt(2));		
		return SQ_OK;
	}
	_MEMBER_FUNCTION_IMPL(Container,SetHeight) 
	{
		StackHandler sa(v);
		_CHECK_SELF(CContainerUI,Container);		
		int nparams = sa.GetParamCount();
		self->SetHeight(sa.GetInt(2));
		return SQ_OK;
	}

	_MEMBER_FUNCTION_IMPL(Container,Add) 
	{
		StackHandler sa(v);
		_CHECK_SELF(CContainerUI,Container);		
		int nparams = sa.GetParamCount();
		SquirrelObject ih=sa.GetObjectHandle(2);
		CControlUI *child=(CControlUI*)ih.GetInstanceUP(0);

		self->Add(child);
		return SQ_OK;
	}
	_BEGIN_CLASS(Container)
		_MEMBER_FUNCTION(Container,constructor,2,_T("xt")) // x = instance ('self/this' not yet created), no arguments.	
		_MEMBER_FUNCTION(Container,	SetWidth,2,_T("xn"))
		_MEMBER_FUNCTION(Container,	SetHeight,2,_T("xn"))
		_MEMBER_FUNCTION(Container,	Add,2,_T("xx"))
	_END_CLASS_INHERITANCE(Container,Control)		

	_DECL_CLASS(HorizontalLayout);
	
	_MEMBER_FUNCTION_IMPL(HorizontalLayout,constructor) 
	{
		CHorizontalLayoutUI * newv = NULL;		
		StackHandler sa(v);

		SquirrelObject so=sa.GetObjectHandle(1);
		
		newv = new CHorizontalLayoutUI(so);
		DirectUI_ContainerParse(newv,v);
	
		return DirectUI_Construct_Release_Hook(newv);
	}
	_BEGIN_CLASS(HorizontalLayout)
		_MEMBER_FUNCTION(HorizontalLayout,constructor,2,_T("xt")) // x = instance ('self/this' not yet created), no arguments.	
	_END_CLASS_INHERITANCE(HorizontalLayout,Container)			

//--------TabFolder-------------
	_DECL_CLASS(TabFolder);
		_MEMBER_FUNCTION_IMPL(TabFolder,constructor) 
		{
			CTabFolderUI * newv = NULL;
			StackHandler sa(v);

			SquirrelObject so=sa.GetObjectHandle(1);
			newv = new CTabFolderUI(so);
			DirectUI_ContainerParse(newv,v);
			return DirectUI_Construct_Release_Hook(newv);

		}

	_BEGIN_CLASS(TabFolder)
		_MEMBER_FUNCTION(TabFolder,constructor,2,_T("xt")) 
	_END_CLASS_INHERITANCE(TabFolder,Container)	
//--------TabPage-------------
	_DECL_CLASS(TabPage);
	_MEMBER_FUNCTION_IMPL(TabPage,constructor) 
	{
		CTabPageUI * newv = NULL;
		StackHandler sa(v);

		SquirrelObject so=sa.GetObjectHandle(1);
		newv = new CTabPageUI(so);
		DirectUI_ContainerParse(newv,v);
		return DirectUI_Construct_Release_Hook(newv);

	}

	_BEGIN_CLASS(TabPage)
		_MEMBER_FUNCTION(TabPage,constructor,2,_T("xt")) 
	_END_CLASS_INHERITANCE(TabPage,Container)	

//--------Toolbar-------------
	_DECL_CLASS(Toolbar);

	_MEMBER_FUNCTION_IMPL(Toolbar,constructor) 
	{
		CToolbarUI * newv = NULL;
		StackHandler sa(v);

		SquirrelObject so=sa.GetObjectHandle(1);
		newv = new CToolbarUI(so);

		DirectUI_ContainerParse(newv,v);

		return DirectUI_Construct_Release_Hook(newv);
	}
	_BEGIN_CLASS(Toolbar)
		_MEMBER_FUNCTION(Toolbar,constructor,2,_T("xt")) // x = instance ('self/this' not yet created), no arguments.	
	_END_CLASS_INHERITANCE(Toolbar,HorizontalLayout)		

	_DECL_CLASS(VerticalLayout);
	
	_MEMBER_FUNCTION_IMPL(VerticalLayout,constructor) 
	{
		CVerticalLayoutUI * newv = NULL;		
		StackHandler sa(v);

		SquirrelObject so=sa.GetObjectHandle(1);
		newv = new CVerticalLayoutUI(so);
		DirectUI_ContainerParse(newv,v);
		return DirectUI_Construct_Release_Hook(newv);
	}
	_BEGIN_CLASS(VerticalLayout)
		_MEMBER_FUNCTION(VerticalLayout,constructor,2,_T("xt")) // x = instance ('self/this' not yet created), no arguments.	
	_END_CLASS_INHERITANCE(VerticalLayout,Container)	

	_DECL_CLASS(TaskPanel);

	_MEMBER_FUNCTION_IMPL(TaskPanel,constructor) 
	{
		CTaskPanelUI * newv = NULL;
		StackHandler sa(v);

		SquirrelObject so=sa.GetObjectHandle(1);
		newv = new CTaskPanelUI(so);

		DirectUI_ContainerParse(newv,v);

		return DirectUI_Construct_Release_Hook(newv);
	}


	_BEGIN_CLASS(TaskPanel)
		_MEMBER_FUNCTION(TaskPanel,constructor,2,_T("xt")) // x = instance ('self/this' not yet created), no arguments.	
	_END_CLASS_INHERITANCE(TaskPanel,VerticalLayout)	





	//DialogLayout
	_DECL_CLASS(DialogLayout);

	_MEMBER_FUNCTION_IMPL(DialogLayout,constructor) 
	{
		CDialogLayoutUI * newv = NULL;		
		StackHandler sa(v);

		SquirrelObject so=sa.GetObjectHandle(1);
		newv = new CDialogLayoutUI(so);
		DirectUI_ContainerParse(newv,v);
		return DirectUI_Construct_Release_Hook(newv);
	}
	_BEGIN_CLASS(DialogLayout)
		_MEMBER_FUNCTION(DialogLayout,constructor,2,_T("xt")) // x = instance ('self/this' not yet created), no arguments.	
	_END_CLASS_INHERITANCE(DialogLayout,Container)	

	//Statusbar
	_DECL_CLASS(Statusbar);

	_MEMBER_FUNCTION_IMPL(Statusbar,constructor) 
	{
		CStatusbarUI * newv = NULL;		
		StackHandler sa(v);

		SquirrelObject so=sa.GetObjectHandle(1);
		newv = new CStatusbarUI(so);
		DirectUI_ContainerParse(newv,v);
		return DirectUI_Construct_Release_Hook(newv);
	}
	_BEGIN_CLASS(Statusbar)
		_MEMBER_FUNCTION(Statusbar,constructor,2,_T("xt")) // x = instance ('self/this' not yet created), no arguments.	
	_END_CLASS_INHERITANCE(Statusbar,Container)	

	


	_DECL_CLASS(NavigatorPanel);

	_MEMBER_FUNCTION_IMPL(NavigatorPanel,constructor) 
	{
		CNavigatorPanelUI * newv = NULL;
		StackHandler sa(v);

		SquirrelObject so=sa.GetObjectHandle(1);
		newv = new CNavigatorPanelUI(so);

		DirectUI_ContainerParse(newv,v);

		return DirectUI_Construct_Release_Hook(newv);
	}
	_BEGIN_CLASS(NavigatorPanel)
		_MEMBER_FUNCTION(NavigatorPanel,constructor,2,_T("xt")) // x = instance ('self/this' not yet created), no arguments.	
	_END_CLASS_INHERITANCE(NavigatorPanel,VerticalLayout)	

	//List
	_DECL_CLASS(List);
	_MEMBER_FUNCTION_IMPL(List,constructor) 
	{
		CListUI * newv = NULL;
		StackHandler sa(v);

		SquirrelObject so=sa.GetObjectHandle(1);
		newv = new CListUI(so);

		DirectUI_ContainerParse(newv,v);

		return DirectUI_Construct_Release_Hook(newv);
	}
	_BEGIN_CLASS(List)
		_MEMBER_FUNCTION(List,constructor,2,_T("xt")) // x = instance ('self/this' not yet created), no arguments.	
	_END_CLASS_INHERITANCE(List,VerticalLayout)	


	//Cabvas

	_DECL_CLASS(Canvas);

	_MEMBER_FUNCTION_IMPL(Canvas,constructor) 
	{
		CCanvasUI * newv = NULL;
		StackHandler sa(v);

		SquirrelObject so=sa.GetObjectHandle(1);
		newv = new CCanvasUI(so);

		DirectUI_ContainerParse(newv,v);

		return DirectUI_Construct_Release_Hook(newv);
	}
	_MEMBER_FUNCTION_IMPL(Canvas,SetWatermark) 
	{
		StackHandler sa(v);
		_CHECK_SELF(CCanvasUI,Canvas);		
		int nparams = sa.GetParamCount();
			LPCTSTR filename=sa.GetString(2);

		self->SetWatermark(filename);

		return SQ_OK;
	}
	
	_BEGIN_CLASS(Canvas)
		_MEMBER_FUNCTION(Canvas,constructor,2,_T("xt")) // x = instance ('self/this' not yet created), no arguments.	
			_MEMBER_FUNCTION(Canvas,	SetWatermark,2,_T("xs"))
	_END_CLASS_INHERITANCE(Canvas,Container)	

	_DECL_CLASS(WindowCanvas);

	_MEMBER_FUNCTION_IMPL(WindowCanvas,constructor) 
	{
		CWindowCanvasUI * newv = NULL;
		StackHandler sa(v);

		SquirrelObject so=sa.GetObjectHandle(1);
		newv = new CWindowCanvasUI(so);

		DirectUI_ContainerParse(newv,v);

		return DirectUI_Construct_Release_Hook(newv);
	}
	_BEGIN_CLASS(WindowCanvas)
		_MEMBER_FUNCTION(WindowCanvas,constructor,2,_T("xt")) // x = instance ('self/this' not yet created), no arguments.	
	_END_CLASS_INHERITANCE(WindowCanvas,Canvas)	

//CControlCanvasUI

	_DECL_CLASS(ControlCanvas);

	_MEMBER_FUNCTION_IMPL(ControlCanvas,constructor) 
	{
		CControlCanvasUI * newv = NULL;
		StackHandler sa(v);

		SquirrelObject so=sa.GetObjectHandle(1);
		newv = new CControlCanvasUI(so);

		DirectUI_ContainerParse(newv,v);

		return DirectUI_Construct_Release_Hook(newv);
	}
	_BEGIN_CLASS(ControlCanvas)
		_MEMBER_FUNCTION(ControlCanvas,constructor,2,_T("xt")) // x = instance ('self/this' not yet created), no arguments.	
	_END_CLASS_INHERITANCE(ControlCanvas,Canvas)	




	
	_DECL_CLASS(TileLayout);

	_MEMBER_FUNCTION_IMPL(TileLayout,constructor) 
	{
		CTileLayoutUI * newv = NULL;
		StackHandler sa(v);

		SquirrelObject so=sa.GetObjectHandle(1);
		newv = new CTileLayoutUI(so);

		DirectUI_ContainerParse(newv,v);

		return DirectUI_Construct_Release_Hook(newv);
	}
	_BEGIN_CLASS(TileLayout)
		_MEMBER_FUNCTION(TileLayout,constructor,2,_T("xt")) // x = instance ('self/this' not yet created), no arguments.	
	_END_CLASS_INHERITANCE(TileLayout,Container)	



	


	int sqUI_Open()
	{
		_INIT_STATIC_NAMESPACE(SQ); 

		_INIT_CLASS(WindowHost);
			_INIT_CLASS(FrameWnd);
			_INIT_CLASS(PageWnd);

		_INIT_CLASS(Control);
			_INIT_CLASS(ImagePanel);
			_INIT_CLASS(WebActiveX);
			_INIT_CLASS(SliderBar);
			_INIT_CLASS(ProgressBar);
		

		_INIT_CLASS(FadedLine);		
		_INIT_CLASS(Button);		
			_INIT_CLASS(SQButton);		
		_INIT_CLASS(Option);	
		_INIT_CLASS(Check);	

		

	
		_INIT_CLASS(ListElement);
			_INIT_CLASS(NavigatorButton);
			_INIT_CLASS(ListLabelElement);




		
		_INIT_CLASS(SingleLineEdit);
		_INIT_CLASS(ListHeaderItem);

			

			
		_INIT_CLASS(ToolGripper);	
		_INIT_CLASS(PaddingPanel);
		_INIT_CLASS(LabelPanel);
			_INIT_CLASS(TextPanel);
				_INIT_CLASS(WarningPanel);
			
		
		_INIT_CLASS(ToolbarTitlePanel);
		_INIT_CLASS(TitleShadow);

		


		
		
		_INIT_CLASS(Container);
			_INIT_CLASS(TabFolder);
			_INIT_CLASS(TabPage);
			_INIT_CLASS(HorizontalLayout);
				_INIT_CLASS(Toolbar);
			_INIT_CLASS(DialogLayout);
			_INIT_CLASS(Statusbar);		
			_INIT_CLASS(DropDown);	
			_INIT_CLASS(VerticalLayout);
				_INIT_CLASS(NavigatorPanel);
				_INIT_CLASS(List);
				_INIT_CLASS(TaskPanel);

			_INIT_CLASS(Canvas);
				_INIT_CLASS(WindowCanvas);
				_INIT_CLASS(ControlCanvas);

				
			
			_INIT_CLASS(TileLayout);
				
	

		return 0; /* nothing in stack */
	}

//}

void  DirectUI::SetColourDepth(int ColourDepth)
{
	g_ColourDepth=ColourDepth;
	if (g_ColourDepth==16)
	{
		IDirect3D9* pD3D = Direct3DCreate9(D3D_SDK_VERSION);
		D3DDISPLAYMODE displayMode;
		pD3D->GetAdapterDisplayMode( 0, &displayMode );
		if (displayMode.Format==D3DFMT_R5G6B5)
			g_r5G6B5=true;
		pD3D->Release();
	}


}




void DirectUI::Open(void)
{
	SquirrelVM::Init();
	sqUI_Open();
}

void DirectUI::Close(void)
{

	
	 SquirrelVM::Shutdown();
	 CPaintManagerUI::Finish();
}
CControlUI*  DirectUI::DialogBuilder(LPCTSTR scriptNut)
{


	

	CControlUI* pRoot=NULL;	

	SquirrelObject main = SquirrelVM::CompileBuffer(scriptNut);

	
	SquirrelObject ret=SquirrelVM::RunScript(main);

	//取得UserPoint
	pRoot=(CControlUI*)ret.GetInstanceUP(0);

	 

	return pRoot;
}

void DirectUI::SetResource(IResource* IRes)
{


	g_IRes=IRes;
}

bool IsR5G6B5()
{
	

	return g_r5G6B5;

}
HBITMAP _LoadImage(  SQStream* file)
{

	cImage image; 
	image.Load(file);
	PixelFormat fmt=PF_A8R8G8B8;
	switch (g_ColourDepth)
	{
	case 32:
		fmt=PF_A8R8G8B8;
		break;
	case 16:


		fmt= IsR5G6B5() ? PF_R5G6B5:PF_A1R5G5B5;
		break;
	case 8:
		fmt=PF_I8;
		break;

	}
	image.ConvertFormat( fmt);
	int width=image.GetXSize();
	int bytes=width*image.GetDepth()/8;

	//int t=(g_ColourDepth/8)-1;
	
	DWORD size=( (bytes +3)&~3 ) *image.GetYSize() ;
	
	HBITMAP  hBmp=CreateBitmap(image.GetXSize(),image.GetYSize(),1,g_ColourDepth,NULL);
	SetBitmapBits(hBmp,size,image.GetPixels());
	g_IRes->Delete(file);
	return hBmp;


}
 HBITMAP DirectUI::LoadImage(LPCTSTR imagePath,int red,int green,int blue)
 {

	

	 
	 TCHAR  tempPath[1024];

	  SQStream* file=	g_IRes->Create(imagePath);
	  if(file)
	  {
	
		  return _LoadImage(file);
	  }
	  TCHAR  fileName[1024];
	  lstrcpy(fileName,imagePath);
	  for (int i=lstrlen(fileName)-1;i>0;i--)
	  {
		  if (fileName[i]=='.')
		  {

			  fileName[i]=0;
			  break;

		  }

	  }
	  TCHAR * imageExt[]={_T("jpg"),_T("png"),_T("bmp")};



	 for (int i=0;i<sizeof (imageExt) /sizeof(TCHAR*);i++)
	 {
		 wsprintf(tempPath,_T("%s.%s"),fileName,imageExt[i]);
		 SQStream* file=	g_IRes->Create(tempPath);
		 if (file)
		 {
			return _LoadImage(file);
			
		 }
	 }
	return  NULL;
 }

bool DirectUI::LoadModule(LPCTSTR modlue)
{


	bool ret=false;
	LPCTSTR buff=LoadBuffer(modlue);

	if (buff)
	{
		try
		{
			SquirrelObject func=SquirrelVM::CompileBuffer((TCHAR*)(buff+1),modlue);
			//SquirrelObject func=SquirrelVM::CompileBuffer(_T("local t=0;"));
			SquirrelObject Qret=	SquirrelVM::RunScript(func);

		}
		catch (SquirrelError & e)
		{
			TCHAR temp[512];
			wsprintf(temp,_T("Script runtime error %s :%s"),modlue,e.desc); 
			MessageBox(NULL,temp,_T("--Error--"),0);
			
		}
		
	
		delete []buff;
		ret=true;
	}	

	//DialogBuilder
	return ret;
}