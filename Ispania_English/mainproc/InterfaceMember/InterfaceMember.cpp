#include "..\GameMain.h"
#include "InterfaceMember.h"

// --------------------------------------------------------------------------------------
void InterfaceMemberInitial()
{
	CGlueParent::Initial();
	CAccountLogin::Initial();
	CCharacterSelect::Initial();
	CCharacterCreate::Initial();

	CWorldFrame::Initial();
	CMainMenu::Initial();
	CUI_CharStatus::Initial();
	CBackPackage::Initial();
	CChatFrame::Initial();

    CUI_Trade::Initial();
    CUI_Calculate::Initial();
	CUI_CharHeadFrame::Initial();
	
	CUI_QuestList::Initial();
	CUI_QuestDialog::Initial();
	CUI_QuestNPCList::Initial();
	CUI_CastingTimeBar::Initial();
	CUI_SkillBook::Initial();

	CUIOptionsFrame::Initial();
}

// --------------------------------------------------------------------------------------
void InterfaceMemberRelease()
{
	CGlueParent::Release();
	CAccountLogin::Release();
	CCharacterSelect::Release();
	CCharacterCreate::Release();

	CWorldFrame::Release();
	CMainMenu::Release();
	CUI_CharStatus::Release();
	CBackPackage::Release();
	CChatFrame::Release();

    CUI_Trade::Release();
    CUI_Calculate::Release();
	CUI_CharHeadFrame::Release();

	CUI_QuestList::Release();
	CUI_QuestDialog::Release();
	CUI_CastingTimeBar::Release();
	CUI_SkillBook::Release();

	CUIOptionsFrame::Release();
}

// --------------------------------------------------------------------------------------
void InterfaceMemberRegisterFunc()
{	
	RegisterGlueParentFunc();
	RegisterAccountLoginFunc();
	RegisterCharacterSelectFunc();
	RegisterCharacterCreateFunc();

	RegisterWorldFrameFunc();
    Register_CChatFrame;
	Register_UI_CharStatus;
    Register_CBackPackage;
    Register_UI_Trade;
    Register_UI_Calculate;
	Register_UILua_QuestList;
	Register_UILua_QuestDialog;
	Register_UILua_QuestNPCList;

    Register_UI_MaintainFrame;
	Register_UI_ShortCutFrame;
	//Register_UI_CharFrame;
	Register_UI_CharHeadFrame;
	Register_UI_CastingTimeBar;
	Register_UI_SkillBook;

	RegisterUIOptionsFrameFunc();
}