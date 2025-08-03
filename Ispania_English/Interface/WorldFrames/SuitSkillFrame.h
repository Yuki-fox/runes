#pragma once

#include "../interface.h"
#include "../../netwaker_member/net_eq/NetCli_EQChild.h"

#define SKILL_PLATE_MAX_JOB		11			// 0 �D   1��
#define SKILL_PLATE_MAX_ITEM	10			//�ޯ�L���̤j���s�ƶq 

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

	// ���o���~���
	ItemFieldStruct* GetItemFieldStruct()
	{		
		switch (type)
		{
			// �I�]
		case 0:
			return CNetGlobal::RoleData()->GetBodyItem( pos );
			break;

			// �˳�
		case 1:
			return CNetGlobal::RoleData()->GetEqItem( pos );
			break;
		}
		return NULL;	
	}

	// ��w���~
	void Lock()
	{
		ItemFieldStruct* item = GetItemFieldStruct();
		if ( item && item->Pos == EM_ItemState_NONE )
			item->Pos = EM_ItemState_CliDisabled;
	}

	// ���ꪫ�~
	void Unlock()
	{
		ItemFieldStruct* item = GetItemFieldStruct();
		if ( item && item->Pos == EM_ItemState_CliDisabled )
			item->Pos = EM_ItemState_NONE;
	}

	union {
		int _value;
		struct {			
			short pos;					// �Ҧb�����ޭ�(�ĴX�Ӧ�m)
			short type;					// �ӷ�����(0 �I�] 1�˳�)
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
	virtual	void				SkillPlateDelete(); //�ŶKï�Ԩ�ťճB��R��
	virtual	void				SetSkillPlateOldID(int SkillID, int Pos); 
	void						SaveClientData();
	void						LoadClientData(int type, int size, char* data);
	virtual	void				Loadplateitems_EQ(); 
	virtual void				SaveEQ_plateitems(int pos , int iskillID);
	virtual int					GetJobForPlateItem();
	virtual bool				CheckSkillItem( int index );
	virtual bool				CheckSkillItem_voc( int SkillID);

	int							m_job0_Skillcount;				// �Ԥh�ޯ�
	int							m_job1_Skillcount;				// �C�L�ޯ�
	int							m_job2_Skillcount;				// �v��̧ޯ�
	int							m_job3_Skillcount;				// �k�v�ޯ�
	int							m_job4_Skillcount;				// ���v�ޯ�
	int							m_job5_Skillcount;				// �M�h�ޯ�
	int							m_job6_Skillcount;				// �˪L���̧ޯ�
	int							m_job7_Skillcount;				// �۵M���x�ޯ�
	int							m_job8_Skillcount;				// �q�Χޯ�
	int							m_SkillPlateIndex[10];			//�ΨӦs�Q��w��listitem	
	SkillPlateData				m_PlateInfo;
	int							m_skillplateitems[SKILL_PLATE_MAX_JOB][SKILL_PLATE_MAX_ITEM];		//����¾�~�ɪ�PLATE (�s�JSERVER)
	int							m_jobSkillcount[SKILL_PLATE_MAX_JOB];


protected:		
	int							m_npcId;						// �}��NPC�s��
	map<int, SoubleItemLocked>	m_soubleItem;					// �O����J�����~
	int							m_pickupItemIndex;				// �������~�s��
};
