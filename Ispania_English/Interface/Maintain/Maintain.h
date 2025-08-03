#pragma once

#include "../interface.h"

class CMaintain;
extern CMaintain*			g_pMaintain;

class CMaintain : public CInterfaceBase
{
public:
	CMaintain(CInterface* object);
	virtual ~CMaintain();
	
	virtual	void				RegisterFunc();
	virtual	bool				IsVisible() { return (_UI_Win !=NULL && _UI_Win->IsVisible() ); } ;
	virtual	void				OnLoad();

	virtual	bool				Open();
	virtual	bool				ModifyOpen( void*  data );
	virtual	bool				Hide();

	virtual	void				OnCreateMacro();
	virtual	void				OnAcceptClick();
	virtual	void				OnResetPos();
	virtual	void				OnUpdate();

	virtual	void				SetModifyInfo( void* data );
	virtual	void				GetModifyInfo( void* data );

protected:
	CUiFrame*				_UI_Win;
	CUiFontString*			_UI_Title;
	CUiEditBox*				_UI_ObjID;
	CUiEditBox*				_UI_OrgName;
	CUiEditBox*				_UI_GItemID;
	CUiEditBox*				_UI_Name;
	CUiEditBox*				_UI_ObjClass;
	CUiEditBox*				_UI_PID;
	CUiEditBox*				_UI_AutoPlot;
	CUiEditBox*				_UI_QuestID;
	CUiEditBox*				_UI_PosX;
	CUiEditBox*				_UI_PosY;
	CUiEditBox*				_UI_PosZ;
	CUiEditBox*				_UI_PosDir;
	CUiButton*				_UI_PosReset;
	CUiCheckButton*			_UI_CheckMode[32];
	CUiEditBox*				_UI_MacroName;
	CUiButton*				_UI_MacroCreate;
	CUiFontString*			_UI_Macro;
	CUiButton*				_UI_Accept;
	CUiButton*				_UI_Cencel;

	int					    _GItemID;

	string					_Cmd;
};