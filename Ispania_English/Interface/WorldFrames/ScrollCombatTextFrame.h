#pragma once

#include "../interface.h"
#include <string>
#include <vector>
#include <list>
#include <map>

class CScrollCombatTextFrame;
extern CScrollCombatTextFrame*			g_pScrollCombatTextFrame;

using namespace std;

#define SCT_DEFAULT_VALUE				0x7FFFFFFF

enum SCTInfoType
{
	eSCT_Null							= 0,	

	// State
	eSCT_Combat_Enter,					// 近入戰鬥
	eSCT_Combat_Leave,					// 離開戰鬥
	eSCT_Increase_EXP,					// 獲得經驗值
	eSCT_Decrease_EXP,					// 失去經驗值
	eSCT_Increase_TP,					// 獲得技能點數	
	eSCT_Decrease_TP,					// 失去技能點數
	eSCT_Increase_Honor,				// 獲得榮譽點數
	eSCT_Decrease_Honor,				// 失去榮譽點數

	// 平常傷害
	eSCT_Normal_Miss,
	eSCT_Normal_Dodge,
	eSCT_Normal_Parry,
	eSCT_Normal_Damage,
	eSCT_Normal_Critical,
	eSCT_Normal_Absorb,
	eSCT_Normal_Block,					// 盾檔

	// 法術效果
	eSCT_Magic_Miss,
	eSCT_Magic_Damage,
	eSCT_Magic_Critical,
	eSCT_Magic_NoEffect,
	eSCT_Magic_Absorb,
	eSCT_Magic_Resist,
	eSCT_Magic_Dodge,

	// 顯示字串
	eSCT_Show_Text,						// 顯示字串
};

enum SCTDisplayMode
{
	eSCT_Display_Vertical				= 0,
	eSCT_Display_Throb,							// 上下跳動
	eSCT_Display_Popdown,						
};

struct SCTInfo
{
	SCTInfo()
	{
		type		= eSCT_Null;
		hp			= SCT_DEFAULT_VALUE;
		mp			= SCT_DEFAULT_VALUE;
		sp			= SCT_DEFAULT_VALUE;
		value		= 0;
		source		= 0;
		target		= 0;
		delayTime	= 0.0f;
	}

	SCTInfoType type;
	int hp, mp, sp;
	int value;
	int source, target;
	float delayTime;
	string text;
};	

struct SCTDisplay
{
	SCTDisplay()
	{
		mode		= eSCT_Display_Vertical;
		transform	= false;
		magnify		= false;
		depth		= false;
		x			= 0.0f;
		y			= 0.0f;
		z			= 0.0f;
		xVector		= 0.0f;
		yVector		= 0.0f;
		xOffset		= 0.0f;
		yOffset		= 0.0f;

		text		= NULL;		
		time		= 0.0f;
		randX		= 0;
		scale		= 1.0f;
	}

	SCTDisplayMode mode;
	bool transform;
	bool magnify;
	bool depth;
	float x, y, z;
	float xVector, yVector;
	float xOffset, yOffset;		// 2D偏移量

	float randX;
	float scale;

	// 計算用
	CUiFontString* text;
	float time;
};

typedef vector<SCTInfo>			SctItem;
typedef map<int, SctItem>		SctQueue;

class CScrollCombatTextFrame : public CInterfaceBase
{
public:
	CScrollCombatTextFrame(CInterface* object);
	virtual ~CScrollCombatTextFrame();
	
	virtual	void				RegisterFunc();
	virtual	void				BuildFrames();
	virtual void				DestroyFrames();
	virtual	void				OnPaint(CUiFrame* frame, int lParam, int rParam);
	virtual	void				Update(float elapsedTime);	

	virtual	void				PushSCTInfo(SCTInfo& info);
protected:
	virtual void				UpdateQueue(float elapsedTime);
	virtual	void				UpdateItem(SctItem& item, float elapsedTime);
	virtual	void				UpdateDisplay(float elapsedTime);
	virtual	void				AddText(SCTDisplay display, const char* text, float r, float g, float b);
		
protected:
	list<SCTDisplay>			m_display;
	RuFontShadow				m_shadow;
	CRuFontType*				m_fontType;
	SctQueue					m_queue;
};