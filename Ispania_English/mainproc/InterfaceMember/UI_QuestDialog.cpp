#include "../GameMain.h"
#include ".\UI_QuestDialog.h"


// ¥þ°ì«Å§i
//-------------------------------------------------------------------
CUI_QuestDialog				*g_pUi_QuestDialog = NULL;

//-------------------------------------------------------------------
CUI_QuestDialog::CUI_QuestDialog(void)
{

}
// --------------------------------------------------------------------------------------
CUI_QuestDialog::~CUI_QuestDialog(void)
{

}
// --------------------------------------------------------------------------------------
void CUI_QuestDialog::Initial()
{
	if ( g_pUi_QuestDialog == NULL )
	{
		g_pUi_QuestDialog = new CUI_QuestDialog;
	}
}
// --------------------------------------------------------------------------------------
void CUI_QuestDialog::Release()
{
	SAFE_DELETE( g_pUi_QuestDialog );	
}
// --------------------------------------------------------------------------------------


