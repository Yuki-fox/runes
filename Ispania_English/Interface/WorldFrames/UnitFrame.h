#pragma once

#include "../interface.h"

class CUnitFrame;
extern CUnitFrame*				g_pUnitFrame;

enum BuffType
{
	eBuffType_None				= 0,
	eBuffType_1,
	eBuffType_2,
	eBuffType_3,
	eBuffType_4,
	eBuffType_5,
	eBuffType_6,
	eBuffType_7,
	eBuffType_8,
	eBuffType_9,
	eBuffType_10,
	eBuffType_11,
	eBuffType_12,
	eBuffType_13,
	eBuffType_14,
	eBuffType_15,
	eBuffType_16,
};


enum EModelSetItemResult
{
	MSIR_Success            ,
	MSIR_Success_Not_Player ,
	MSIR_Fail               ,
	MSIR_NoPaperdoll        ,
	MSIR_SexError_LimitBoy  ,
	MSIR_SexError_LimitGirl ,
};


class CUnitFrame : public CInterfaceBase
{
public:
	enum UintType
	{
		eUnit_None					= 0,
		eUnit_Player,
		eUnit_Target,
		eUnit_Party,
		eUnit_Raid,
		eUnit_Pet,
	};	

	CUnitFrame(CInterface* object);
	virtual ~CUnitFrame();

	static	void				UnitEvent( const char* eventStr, int type, int index );
	static	BuffType			GetBuffType(int id);

	// �Ѽ�1 MODEL����
	// �Ѽ�2 sprite
	// �Ѽ�3 boolean �O�_��ܪZ��(true ���)
	// �Ѽ�4 boolean �O�_��ܥثe�~��MODEL(true �ܨ����ܫ�Ҽ�)
	static	void				Model_SetRoleSprite( CUiModel* model, CRoleSprite* sprite, bool showWeapon = false, bool showChangeShape = false );

  	// �Ѽ�4 boolean ��ܦX���᪺�ˤl
	static EModelSetItemResult	Model_SetItem( CUiModel* model, GameObjDbStructEx* pItemDB, ItemFieldStruct* pItemField = NULL, bool UseImageObjectID = false );

	virtual	void				RegisterFunc();
protected:
};