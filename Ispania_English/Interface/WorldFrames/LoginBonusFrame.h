#pragma once

#include "../interface.h"
#include "../../netwaker_member/Net_Login/NetCli_Login_Child.h"

//********************************************************************
class CLoginBonusFrame;
extern CLoginBonusFrame* g_pLoginBonusFrame;

struct LBBoxEx
{
	LBBox Box;
	float Remaining;
	int	  Duration;

	LBBoxEx()
	{
		Init();
	}

	void Init()
	{
		Box.Init();
		Remaining = 0;
		Duration = 0;
	}
};

struct LoginBonusPage
{
	int					 Year;
	int					 Month;
	int					 MDay;
	std::vector<LBBoxEx> Boxes;
	std::string			 Description;

	LoginBonusPage()
	{
		Init();
	}

	void Init()
	{
		Year  = 0;
		Month = 0;
		MDay  = 0;
		Boxes.clear();
	}
};

//********************************************************************
class CLoginBonusFrame : public CInterfaceBase
{
protected:
	int							  m_Page;
	std::map<int, LoginBonusPage> m_Pages;
	bool						  m_Visible;
	int							  m_MaxPage;
	int							  m_MinPage;

public:
	CLoginBonusFrame(CInterface* object);
	virtual ~CLoginBonusFrame();

public:
	virtual	void Update(float elapsedTime);
	virtual	void RegisterFunc();

public:
	void Open();
	void Close();

public:
	LoginBonusPage* GetPage(int index);
	LoginBonusPage* GetPage1();
	LoginBonusPage* GetPage2();
	void			PrePage();
	void			NextPage();
	void			RequestBonus();
	void			ClearPages();

public:
	virtual void RL_BonusList		  (std::vector<LBBox>& boxes, int year, int month, int mday, int page, const wchar_t* description);
	virtual void RL_RequestBonusResult(int id, LBRequestResultENUM result);
	virtual void RL_BonusPeriod		  (int maxpage, int minpage);

public:
	bool GetVisible()			{return m_Visible;}
	void SetVisible(bool value)	{ m_Visible = value;}

public:
	__declspec(property(get = GetVisible, put = SetVisible	)) bool Visible;
};
//********************************************************************