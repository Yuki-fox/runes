#ifndef _INTERFACEMEMBER_H
#define _INTERFACEMEMBER_H

#include "GlueParent.h"
#include "AccountLogin.h"
#include "CharacterSelect.h"
#include "CharacterCreate.h"

#include "WorldFrame.h"
#include "MainMenu.h"
#include "BackPackage.h"
#include "UI_CharStatus.h"
#include "ChatFrame.h"
#include "UI_Trade.h"
#include "UI_Calculate.h"
#include "Quest/UI_QuestList.h"
#include "Quest/UI_QuestNPCList.h"
#include "Quest/UI_QuestDialog.h"
#include "UI_MaintainFrame.h"
#include "UI_ShortCutFrame.h"
#include "UI_CharHeadFrame.h"
#include "UI_CastingTimeBar.h"
#include "UIOptionsFrame.h"
#include "UI_SkillBook.h"

void	InterfaceMemberInitial();
void	InterfaceMemberRelease();
void	InterfaceMemberRegisterFunc();

#endif //_INTERFACEMEMBER_H