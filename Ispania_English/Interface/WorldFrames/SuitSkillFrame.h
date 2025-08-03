#pragma once

#include "../interface.h"
#include "../../netwaker_member/net_eq/NetCli_EQChild.h"

#define SKILL_PLATE_MAX_JOB		11			// 0 主   1副
#define SKILL_PLATE_MAX_ITEM	10			//技能盤的最大按鈕數量 

class CSuitSkillFrame;
extern CSuitSkillFrame*			g_pSuitSkillFrame;

using namespace std;

struct SoubleItemLocked 
{
	SoubleItemLocked()
	{
		_value = 0;
	}

	SoubleItemLocked(int t, int i)
	{
		type = t;
		pos = i;
	}

	bool operator < (const SoubleItemLocked& arg) const
	{
		return _value < arg._value;
	}	

	bool operator == (const SoubleItemLocked& arg) const
	{
		return _value == arg._value;
	}

	// 取得物品資料
	ItemFieldStruct* GetItemFieldStruct()
	{		
		switch (type)
		{
			// 背包
		case 0:
			return CNetGlobal::RoleData()->GetBodyItem( pos );
			break;

			// 裝備
		case 1:
			return CNetGlobal::RoleData()->GetEqItem( pos );
			break;
		}
		return NULL;	
	}

	// 鎖定物品
	void Lock()
	{
		ItemFieldStruct* item = GetItemFieldStruct();
		if ( item && item->Pos == EM_ItemState_NONE )
			item->Pos = EM_ItemState_CliDisabled;
	}

	// 解鎖物品
	void Unlock()
	{
		ItemFieldStruct* item = GetItemFieldStruct();
		if ( item && item->Pos == EM_ItemState_CliDisabled )
			item->Pos = EM_ItemState_NONE;
	}

	union {
		int _value;
		struct {			
			short pos;					// 所在欄位索引值(第幾個位置)
			short type;					// 來源類型(0 背包 1裝備)
		};
	};	
};
struct SkillPlateData
{
	int SkillID;
	int Pos;
	int State;
};
class CSuitSkillFrame : public CInterfaceBase
{
public:
	CSuitSkillFrame(CInterface* object);
	virtual ~CSuitSkillFrame();

	virtual	void				RegisterFunc();
	virtual	void				BuildFrames();
	virtual void				DestroyFrames();
	virtual	void				Update(float elapsedTime);
	virtual	void				Close();
	virtual	bool				IsOpen()						{ return m_npcId != -1; }
	virtual void				ClearItems();
	virtual	bool				Cut(int index);
	virtual	void				Paste(int index);
	virtual SoubleItemLocked*	GetSoubleItem(int equipIndex);
	virtual int					GetSuitID();
	virtual bool				InsertItem(int equipIndex, int type, int pos);
	virtual bool				DeleteItem(int equipIndex);
	virtual void				UpdateItem(int equipIndex);
	virtual void				SoubleItem(int selectItem);	
	virtual bool				CheckSuitItem();
	virtual void				SkillEquip(int slot, int type, int index,int ContainerID = -1);	

	virtual	int					GetNpcId()						{ return m_npcId; }	
	
	virtual void				R_Open(int npcId);
	virtual void				R_LearnSuitSkillResult( int LearnSkillID , LearnSuitSkillResultENUM Result );
	virtual void				R_SetSuitSkillResult( SetSuitSkillResultENUM Result );
	virtual void				R_UseSuitSkillResult( UseSuitSkillResultENUM Result );

	virtual	void				InitJobSkill_Count();
	virtual void				SendSkillPlatePosToServer(int SkillID, int Pos);
	virtual	void				SkillPlateDelete(); //剪貼簿拉到空白處後刪除
	virtual	void				SetSkillPlateOldID(int SkillID, int Pos); 
	void						SaveClientData();
	void						LoadClientData(int type, int size, char* data);
	virtual	void				Loadplateitems_EQ(); 
	virtual void				SaveEQ_plateitems(int pos , int iskillID);
	virtual int					GetJobForPlateItem();
	virtual bool				CheckSkillItem( int index );
	virtual bool				CheckSkillItem_voc( int SkillID);

	int							m_job0_Skillcount;				// 戰士技能
	int							m_job1_Skillcount;				// 遊俠技能
	int							m_job2_Skillcount;				// 影行者技能
	int							m_job3_Skillcount;				// 法師技能
	int							m_job4_Skillcount;				// 祭師技能
	int							m_job5_Skillcount;				// 騎士技能
	int							m_job6_Skillcount;				// 森林牧者技能
	int							m_job7_Skillcount;				// 自然神官技能
	int							m_job8_Skillcount;				// 通用技能
	int							m_SkillPlateIndex[10];			//用來存被鎖定的listitem	
	SkillPlateData				m_PlateInfo;
	int							m_skillplateitems[SKILL_PLATE_MAX_JOB][SKILL_PLATE_MAX_ITEM];		//替換職業時的PLATE (存入SERVER)
	int							m_jobSkillcount[SKILL_PLATE_MAX_JOB];


protected:		
	int							m_npcId;						// 開啟NPC編號
	map<int, SoubleItemLocked>	m_soubleItem;					// 記錄放入的物品
	int							m_pickupItemIndex;				// 拿取物品編號
};
