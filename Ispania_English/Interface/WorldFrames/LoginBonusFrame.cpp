#include "../mainproc/GameMain.h"
#include "LoginBonusFrame.h"
#include "../../netwaker_member/Net_Script/NetCli_Script_Child.h"

//********************************************************************
CLoginBonusFrame* g_pLoginBonusFrame = NULL;

//********************************************************************
int LuaFunc_LoginBonusGetPage(lua_State* L);
int LuaFunc_LoginBonusGetDate(lua_State* L);
int LuaFunc_LoginBonusGetDescription(lua_State* L);
int LuaFunc_LoginBonusRequest(lua_State* L);
int LuaFunc_LoginBonusPrePage(lua_State* L);
int LuaFunc_LoginBonusNextPage(lua_State* L);
int LuaFunc_LoginBonusClearPages(lua_State* L);
int LuaFunc_LoginBonusFrameVisible(lua_State* L);

//********************************************************************
//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
CLoginBonusFrame::CLoginBonusFrame(CInterface* object)
:CInterfaceBase(object)
{	
	g_pLoginBonusFrame = this;
	m_Page = 0;
	m_Visible = false;
	m_MaxPage = INT_MIN;
	m_MinPage = INT_MAX;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
CLoginBonusFrame::~CLoginBonusFrame()
{
	g_pLoginBonusFrame = NULL;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void CLoginBonusFrame::Update(float elapsedTime)
{
	bool ShowRequestButton = false;
	bool ShowMinimapButtonFlash = false;

	std::map<int, LoginBonusPage>::iterator it = m_Pages.find(0);

	if (it == m_Pages.end())
	{
		GetPage(0);
	}
	else
	{
		for (it = m_Pages.begin(); it != m_Pages.end(); it++)
		{
			LoginBonusPage& Page = it->second;

			for (unsigned i = 0; i < Page.Boxes.size(); ++i)
			{
				LBBoxEx& BoxEx = Page.Boxes[i];

				if (BoxEx.Duration > 0)
				{
					if (BoxEx.Remaining > 0)
					{
						BoxEx.Remaining -= elapsedTime;

						if (BoxEx.Remaining <= 0)
						{
							BoxEx.Remaining = 0;
							BoxEx.Box.Status = EM_LBBoxStatus_Avaliable;
						}
					}
				}

				if (it->first == 0)
				{
					if (BoxEx.Box.Status == EM_LBBoxStatus_Avaliable)
					{
						if (m_Visible == false)
						{
							ShowMinimapButtonFlash = true;
						}

						if (m_Page == 0)
						{
							ShowRequestButton = true;
						}
					}
				}
			}
		}
	}

	SendWorldEvent(UiEvent_HideLoginBonusMinimapButton);

	if ((m_MinPage <= 0) && (m_MaxPage >= 0))
	{
		if(g_ObjectData->_DisableTag.size() > 670)
		{
			if (g_ObjectData->_DisableTag[670] == false)
			{
				SendWorldEvent(UiEvent_ShowLoginBonusMinimapButton);
			}
		}
	}

	if (ShowRequestButton == true)
	{
		SendWorldEvent(UiEvent_ShowLoginBonusFrameRequestButton);
	}
	else
	{
		SendWorldEvent(UiEvent_HideLoginBonusFrameRequestButton);
	}

	if (ShowMinimapButtonFlash == true)
	{
		SendWorldEvent(UiEvent_ShowLoginBonusMinimapButtonFlash);
	}
	else
	{
		SendWorldEvent(UiEvent_HideLoginBonusMinimapButtonFlash);
	}

	if (m_Page > m_MinPage)
	{
		SendWorldEvent(UiEvent_ShowLoginBonusPrePageButton);
	}
	else
	{
		SendWorldEvent(UiEvent_HideLoginBonusPrePageButton);
	}

	if (m_Page < m_MaxPage)
	{
		SendWorldEvent(UiEvent_ShowLoginBonusNextPageButton);
	}
	else
	{
		SendWorldEvent(UiEvent_HideLoginBonusNextPageButton);
	}
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void CLoginBonusFrame::RegisterFunc()
{
	lua_State* L = GetLuaStateWorld();

	if (L != NULL)
	{
		lua_register(L, "LoginBonusGetPage"			, LuaFunc_LoginBonusGetPage			);
		lua_register(L, "LoginBonusGetDate"			, LuaFunc_LoginBonusGetDate			);
		lua_register(L, "LoginBonusGetDescription"	, LuaFunc_LoginBonusGetDescription	);
		lua_register(L, "LoginBonusRequest"			, LuaFunc_LoginBonusRequest			);
		lua_register(L, "LoginBonusPrePage"			, LuaFunc_LoginBonusPrePage			);
		lua_register(L, "LoginBonusNextPage"		, LuaFunc_LoginBonusNextPage		);
		lua_register(L, "LoginBonusClearPages"		, LuaFunc_LoginBonusClearPages		);
		lua_register(L, "LoginBonusFrameVisible"	, LuaFunc_LoginBonusFrameVisible	);
	}
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void CLoginBonusFrame::Open()
{
	SendWorldEvent(UiEvent_OpenLoginBonusFrame);
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void CLoginBonusFrame::Close()
{
	SendWorldEvent(UiEvent_CloseLoginBonusFrame);
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
LoginBonusPage* CLoginBonusFrame::GetPage(int index)
{
	LoginBonusPage* Result = NULL;

	std::map<int, LoginBonusPage>::iterator it = m_Pages.find(index);

	if (it != m_Pages.end())
	{
		Result = &(it->second);
	}
	else
	{
		LoginBonusPage Empty;
		m_Pages[index] = Empty;
		CNetCli_Login::SL_RequestBonusList(index);
	}

	return Result;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
LoginBonusPage* CLoginBonusFrame::GetPage1()
{
	return GetPage(m_Page);
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
LoginBonusPage* CLoginBonusFrame::GetPage2()
{
	return GetPage(m_Page + 1);
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void CLoginBonusFrame::PrePage()
{
	if (m_Page > m_MinPage)
	{
		m_Page--;
		SendWorldEvent(UiEvent_UpdateLoginBonusFrame);
	}
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void CLoginBonusFrame::NextPage()
{
	if (m_Page < m_MaxPage)
	{
		m_Page++;
		SendWorldEvent(UiEvent_UpdateLoginBonusFrame);
	}
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void CLoginBonusFrame::RequestBonus()
{
	LoginBonusPage* pPage = GetPage(0);

	if (pPage != NULL)
	{
		for (unsigned i = 0; i < pPage->Boxes.size(); ++i)
		{
			if (pPage->Boxes[i].Box.Status == EM_LBBoxStatus_Avaliable)
			{
				CNetCli_Login::SL_RequestBonus(pPage->Boxes[i].Box.ID);
			}
		}
	}
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void CLoginBonusFrame::ClearPages()
{
	m_Page = 0;
	m_Pages.clear();
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void CLoginBonusFrame::RL_BonusList(std::vector<LBBox>& boxes, int year, int month, int mday, int page, const wchar_t* description)
{
	LoginBonusPage& Page = m_Pages[page];	
	Page.Year		 = year;
	Page.Month		 = month;
	Page.MDay		 = mday;
	Page.Description = WCharToUtf8(description);

	LBBoxEx BoxEx;

	Page.Boxes.clear();
	for (unsigned i = 0; i < boxes.size(); ++i)
	{
		BoxEx.Init();
		BoxEx.Box = boxes[i];

		if (BoxEx.Box.Status == EM_LBBoxStatus_Unavaliable)
		{
			for (unsigned j = 0; j < BoxEx.Box.Constraints.size(); ++j)
			{
				LBConstraint& Constraint = BoxEx.Box.Constraints[j];

				switch(Constraint.Type)
				{
				case EM_LBConstraintType_OnlineTime:
					{
						if (Constraint.Remaining > 0)
						{
							if (Constraint.Remaining > (int)BoxEx.Remaining)
							{
								BoxEx.Remaining = Constraint.Remaining + 3.0f;
								BoxEx.Duration  = Constraint.OnlineTime.Time + 3;
							}
						}
					}
					break;
				case EM_LBConstraintType_LoginDays:
					{
						if (Constraint.Remaining > 0)
						{
							if ((Constraint.Remaining*86400) > (int)BoxEx.Remaining)
							{
								BoxEx.Remaining = (float)(Constraint.Remaining*86400);
								BoxEx.Duration = (int)BoxEx.Remaining;
							}
						}
					}
					break;
				}
			}
		}

		Page.Boxes.push_back(BoxEx);
	}

	SendWorldEvent(UiEvent_UpdateLoginBonusFrame);
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void CLoginBonusFrame::RL_RequestBonusResult(int id, LBRequestResultENUM result)
{
	switch(result)
	{
	case EM_LBRequestResult_OK:
		{
			LoginBonusPage& Page0 = m_Pages[0];

			for (unsigned i = 0; i < Page0.Boxes.size(); ++i)
			{
				LBBoxEx& BoxEx = Page0.Boxes[i];

				if (BoxEx.Box.ID == id)
				{
					BoxEx.Box.Status = EM_LBBoxStatus_Requested;
					break;
				}
			}

			g_pGameMain->SendSystemChatMsg(g_ObjectData->GetString("SYS_REQUEST_LOGIN_BONUS_OK"));
		}
		break;
	case EM_LBRequestResult_NotExist:
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_REQUEST_LOGIN_BONUS_NOT_EXIST"));
		break;
	case EM_LBRequestResult_Requested:
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_REQUEST_LOGIN_BONUS_REQUESTED"));
		break;
	case EM_LBRequestResult_ItemDataError:
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_GAMEMSGEVENT_405"));
		break;
	case EM_LBRequestResult_BagFull:
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SC_111491_DIALOG3"));
		break;
	case EM_LBRequestResult_Expired:
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_REQUEST_LOGIN_BONUS_EXPIRED"));
		break;
	case EM_LBRequestResult_Constrained:
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_REQUEST_LOGIN_BONUS_CONSTRAINED"));
		break;
	}

	SendWorldEvent(UiEvent_UpdateLoginBonusFrame);
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
void CLoginBonusFrame::RL_BonusPeriod(int maxpage, int minpage)
{
	m_MaxPage = maxpage;
	m_MinPage = minpage;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
int LuaFunc_LoginBonusGetPage(lua_State* L)
{
	int Idx = luaL_checkint(L, 1);

	lua_newtable(L);

	if (g_pLoginBonusFrame != NULL)
	{
		LoginBonusPage* pPage = NULL;

		switch(Idx)
		{
		case 0:
			pPage = g_pLoginBonusFrame->GetPage1();
			break;
		case 1:
			pPage = g_pLoginBonusFrame->GetPage2();
			break;
		}

		if (pPage != NULL)
		{
			std::vector<LBGoods> Bonuses;
			std::vector<int> Statuses;
			std::vector<int> Durations;
			std::vector<int> Remainings;

			for (unsigned i = 0; i < pPage->Boxes.size(); ++i)
			{
				LBBoxEx& BoxEx = pPage->Boxes[i];

				for (unsigned j = 0; j < BoxEx.Box.Goods.size(); ++j)
				{
					Bonuses.push_back(BoxEx.Box.Goods[j]);
					Statuses.push_back(BoxEx.Box.Status);
					Durations.push_back(BoxEx.Duration);
					Remainings.push_back((int)BoxEx.Remaining);
				}

				if (BoxEx.Box.Money > 0)
				{
					LBGoods Goods;
					Goods.Type = EM_LBGoodsType_Item;
					Goods.Item.OrgObjID = 200000;
					Goods.Item.Count = BoxEx.Box.Money;
					Bonuses.push_back(Goods);
					Statuses.push_back(BoxEx.Box.Status);
					Durations.push_back(BoxEx.Duration);
					Remainings.push_back((int)BoxEx.Remaining);
				}
			}

			for (unsigned i = 0; i < Bonuses.size(); ++i)
			{
				LBGoods& Goods = Bonuses[i];

				lua_pushnumber(L, (i+1));
				lua_newtable(L);

				lua_pushstring(L, "Type");
				lua_pushnumber(L, Goods.Type);
				lua_settable(L, -3);

				lua_pushstring(L, "Status");
				lua_pushnumber(L, Statuses[i]);
				lua_settable(L, -3);

				lua_pushstring(L, "Duration");
				lua_pushnumber(L, Durations[i]);
				lua_settable(L, -3);

				lua_pushstring(L, "Remaining");
				lua_pushnumber(L, Remainings[i]);
				lua_settable(L, -3);

				switch(Goods.Type)
				{
				case EM_LBGoodsType_Item:
					{
						lua_pushstring(L, "OrgObjID");
						lua_pushnumber(L, Goods.Item.OrgObjID);
						lua_settable(L, -3);

						lua_pushstring(L, "Count");
						lua_pushnumber(L, Goods.Item.Count);
						lua_settable(L, -3);
					}
					break;
				case EM_LBGoodsType_Boost:
					{
						lua_pushstring(L, "MagicID");
						lua_pushnumber(L, Goods.Boost.MagicID);
						lua_settable(L, -3);

						lua_pushstring(L, "MagicLv");
						lua_pushnumber(L, Goods.Boost.MagicLv);
						lua_settable(L, -3);

						lua_pushstring(L, "EffectTime");
						lua_pushnumber(L, Goods.Boost.EffectTime);
						lua_settable(L, -3);
					}
					break;
				}

				lua_settable(L, -3);
			}
		}
	}

	return 1;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
int LuaFunc_LoginBonusGetDate(lua_State* L)
{
	int Idx = luaL_checkint(L, 1);

	double Year = 0;
	double Month = 0;
	double MDay = 0;

	if (g_pLoginBonusFrame != NULL)
	{
		LoginBonusPage* pPage = NULL;

		switch(Idx)
		{
		case 0:
			pPage = g_pLoginBonusFrame->GetPage1();
			break;
		case 1:
			pPage = g_pLoginBonusFrame->GetPage2();
			break;
		}

		if (pPage != NULL)
		{
			Year  = pPage->Year;
			Month = pPage->Month;
			MDay  = pPage->MDay;
		}
	}

	lua_pushnumber(L, Year);
	lua_pushnumber(L, Month);
	lua_pushnumber(L, MDay);

	return 3;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
int LuaFunc_LoginBonusGetDescription(lua_State* L)
{
	int Idx = luaL_checkint(L, 1);

	LoginBonusPage* pPage = NULL;

	switch(Idx)
	{
	case 0:
		pPage = g_pLoginBonusFrame->GetPage1();
		break;
	case 1:
		pPage = g_pLoginBonusFrame->GetPage2();
		break;
	}

	if (pPage != NULL)
	{
		lua_pushstring(L, CNetCli_Script_Child::ReplaceSystemKeyword(pPage->Description).c_str());
	}
	else
	{
		lua_pushstring(L, "");
	}

	return 1;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
int LuaFunc_LoginBonusRequest(lua_State* L)
{
	if (g_pLoginBonusFrame != NULL)
	{
		g_pLoginBonusFrame->RequestBonus();
	}

	return 0;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
int LuaFunc_LoginBonusPrePage(lua_State* L)
{
	if (g_pLoginBonusFrame != NULL)
	{
		g_pLoginBonusFrame->PrePage();
	}

	return 0;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
int LuaFunc_LoginBonusNextPage(lua_State* L)
{
	if (g_pLoginBonusFrame != NULL)
	{
		g_pLoginBonusFrame->NextPage();
	}

	return 0;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
int LuaFunc_LoginBonusClearPages(lua_State* L)
{
	if (g_pLoginBonusFrame != NULL)
	{
		g_pLoginBonusFrame->ClearPages();
	}

	return 0;
}

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------
int LuaFunc_LoginBonusFrameVisible(lua_State* L)
{
	int Visible = luaL_checkint(L, 1);

	if (g_pLoginBonusFrame != NULL)
	{
		g_pLoginBonusFrame->SetVisible((Visible == 1));
	}

	return 0;
}