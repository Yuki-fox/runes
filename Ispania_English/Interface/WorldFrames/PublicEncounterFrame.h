#pragma once
#include <vector>
#include "../interface.h"
#include <string>
#include "../../netwaker_member/Net_PublicEncounter/NetCli_PublicEncounterChild.h"
using namespace std;

enum PEChangeType{
	ePEChangeType_None					= 0,	   //�L
	ePEChangeType_Activated				= 1,	   //�i�J�ϰ�
	ePEChangeType_Deactivated			= 2,	   //���}�ϰ�
	ePEChangeType_ActivePhaseChange		= 3,	   //���ȶ��q�ܧ�
	ePEChangeType_ObjValueChage			= 4,	   
	ePEChangeType_Success				= 5,	   
	ePEChangeType_Failed				= 6,	   //���ȥ��ѵ���
	ePEChangeType_PhaseSuccess			= 7,	   
	ePEChangeType_PhaseFailed			= 8,	   //���q���ѵ���
	ePEChangeType_ObjAchieved			= 9,	   //���q����F�� 
	ePEChangeType_PlayerScore			= 10,	   //�n���ܧ�
	ePEChangeType_Close					= 20,	   //���}��������
};

//******************************************************************************
class CPublicEncounterFrame;
extern CPublicEncounterFrame*	g_pPublicEncounterFrame;
//******************************************************************************

struct PEF_POBNowvalueStruct
{
	int index;
	int nowvalue;
	int kind; 
	int type; //�ɶ�or �ƶq
};


struct PEF_PEindexStruct
{
	int PEF_PEindex;
	int PEF_PHindex;
	int PEF_Score;

	vector< PEF_POBNowvalueStruct >	pOB ; 
	PEF_PEindexStruct()
	{
		PEF_Score = 0;
	}
};



struct SPOB_Info
{
	int index;
	float nowvalue;
	int kind; 
	int type; //�ɶ�or �ƶq
};
struct SPE_Info
{
	int iPEIndex;
	int iPHIndex;
	int Score; 

	map<int , SPOB_Info >	pOB ; 
	SPE_Info()
	{
		Score = 0;
	}
};


class CPublicEncounterFrame : public CInterfaceBase
{
public:
	CPublicEncounterFrame( CInterface* pObject );
	virtual				~CPublicEncounterFrame();

	virtual	void		RegisterFunc();
	virtual void		SavAcrClear();
	

	const char*				m_PEName;
	const char*				m_PEFrameMessage;
	const char*				m_PHName;
	const char*				m_PBName;
	int						m_PEFrameStepName;
	int						m_PE_Conditioncount;
	int						m_PObSuccesCount;
	int						m_PObFailedCount;

	int						m_iPEIndex;
	int						m_iPHIndex;
	int						m_iOBIndex;
	int						m_iTimeGap;
	int						m_iActivePhaseIndex;
	int						m_PEScore;

	int						m_SaveActPEindex[20];  //�x�s�i�椤�ΫO�d��PublicEncounter ID;
	int						m_SaveActSize;

	vector< PEF_PEindexStruct >	m_PEitemField;

	//���}��������
	virtual	void PE_LeaveAndClose();


	virtual void Update(float elapsedTime);
	virtual void PE_Del( int iPEIndex ) ;
	virtual void PE_Update( int iPEIndex ,int iActivePhaseIndex , PEChangeType SendType);
	virtual void PE_RL(PEChangeType SendType , int iPEIndex , int iActivePhaseIndex);
	map<int , SPE_Info > PE_data;
	int m_PEindex;

};