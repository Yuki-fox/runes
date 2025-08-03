#pragma once

#include "../interface.h"
#include <map>

using namespace std;

class CGameControl;
extern CGameControl*			g_pGameControl;

enum DuelStepType
{
	eDuelStep_None				= 0,
	eDuelStep_Request			= 1,			// 要求決鬥
	eDuelStep_Receive			= 2,			// 收到對方要求
	eDuelStep_Refuse			= 3,			// 收到拒絕
	eDuelStep_Accept			= 4,			// 同意
	eDuelStep_Cancel			= 5,			// 取消
	eDuelStep_Ready				= 6,			// 準備開始
	eDuelStep_Start				= 7,			// 開始
	eDuelStep_Finish			= 8,			// 結束
};

struct DuelInfo
{
	DuelInfo()					{ step = eDuelStep_None; target = 0; time = 0.0f; }
		
	DuelStepType				step;
	string						targetName;		// 對象名稱
	int							target;			// 對象
	float						time;			// 開始時間

	CRuVector3					centerPos;		// 中心點
	float						range;			// 範圍
};

class CGameControl : public CInterfaceBase
{
public:
	CGameControl(CInterface* object);
	virtual ~CGameControl();

	virtual	void				RegisterFunc();
	virtual	void				BuildFrames();
	virtual void				DestroyFrames();
	virtual	void				Update(float elapsedTime);
	virtual	bool				WndMsgProc(UINT iMsg, WPARAM wParam, LPARAM lParam);

	virtual	void				OnKeyDown(WPARAM wParam, LPARAM lParam);
	virtual	void				OnKeyUp(WPARAM wParam, LPARAM lParam);
	virtual	void				OnKillFocus(bool ignoreOnMouseDown=true);
	
	virtual	void				OnMouseDown(CUiFrame* frame, INT64 lParam, INT64 rParam);
	virtual	void				OnMouseUp(CUiFrame* frame, INT64 lParam, INT64 rParam);
	virtual	void				OnMouseWheel(CUiFrame* frame, INT64 lParam, INT64 rParam);
	virtual	void				OnClick(CUiFrame* frame, INT64 lParam, INT64 rParam);
	virtual	void				OnDoubleClick(CUiFrame* frame, INT64 lParam, INT64 rParam);
	virtual	void				OnMouseMove(CUiFrame* frame, INT64 lParam, INT64 rParam);
	virtual	void				OnMouseEnter(CUiFrame* frame, INT64 lParam, INT64 rParam);
	virtual	void				OnMouseLeave(CUiFrame* frame, INT64 lParam, INT64 rParam);
	virtual	void				OnDragBegin(CUiFrame* frame, INT64 lParam, INT64 rParam);
	virtual	void				OnDragStop(CUiFrame* frame, INT64 lParam, INT64 rParam);
	virtual	void				OnReceiveDrag(CUiFrame* frame, INT64 lParam, INT64 rParam);

	virtual	void				SetDuelStep(DuelStepType step, int target);
	virtual	void				StartDuel(float x, float y, float z, float range)	{ m_duel.centerPos.Set(x, y, z); m_duel.range = range; }
	virtual	DuelInfo			GetDuelInfo()							{ return m_duel; }

	virtual	float				GetMouseOffsetX(){ return m_mouseOffsetX; }
	virtual	float				GetMouseOffsetY(){ return m_mouseOffsetY; }
	virtual	float				GetButtonDownTime(){ return m_buttonDownTime; }

	virtual void				DeleteItemConfirm(ItemFieldStruct* item);
	virtual bool				IsMouseEnter()							{ return m_isMouseEnter; }
	virtual void				SendWedgeReport();
	virtual void				CancelWedgeReport();
	virtual void				R_UnlockRolePosOK(bool Result);

	virtual void				R_PartyDice(int PlayerDBID, int Type, int RandValue);
	virtual void				R_InviteMount(const char* name, int GItemID);
	virtual void				AnswerInviteMount(bool accept);
	virtual void				LeaveMount();
	virtual CUiTexture*			GetDarkTexture()						{ return m_darkTexture; }
protected:

	union {
		int __data[4];
		struct {
			bool				m_rotateCamera			:1;
			bool				m_ignoreOnMouseDown		:1;
			bool				m_isMouseEnter			:1;
			bool				m_LButtonDown			:1;
			bool				m_RButtonDown			:1;
			bool				m_isAutoRun				:1;			
		};
	};	
	float						m_mouseOffsetX;
	float						m_mouseOffsetY;
	float						m_buttonDownTime;
	map<int, int>				m_keystate;

	bool						m_wedgeLocked;	
	DuelInfo					m_duel;	
	string						m_mountInviteName;				// 接收到受到邀請共乘坐騎,對方名稱
	int							m_mountInviteGItemID;			// 接收到受到邀請共乘坐騎,對方GItemID

	CUiTexture*					m_darkTexture;					// 背景前遮罩
};