#pragma once
#include "../interface.h"
#include <string>
#include <map>
#include <vector>

#define EQUIP_PLATE_MAX_ITEM	6	//可安裝的裝備上限是6


//31~50  1 51~80  2 81~100
#define EQUIP_PLATE_LEVEL_01_1	31	//
#define EQUIP_PLATE_LEVEL_01_2	50	//
#define EQUIP_PLATE_LEVEL_02_1	51	//
#define EQUIP_PLATE_LEVEL_02_2	80	//
#define EQUIP_PLATE_LEVEL_03_1	81	//
#define EQUIP_PLATE_LEVEL_03_2	100	//

enum	EQUIP_PLATE_LEVEL_
{
	EQUIP_PLATE_LEVEL_31_50		= 0		,	
	EQUIP_PLATE_LEVEL_51_80		= 1		,	
	EQUIP_PLATE_LEVEL_81_100	= 2		,	
};
	


//******************************************************************************
class CRuneExchangeFrame;
extern CRuneExchangeFrame*	g_pRuneExchangeFrame;

using namespace std;

struct Eq_Exchange_Item
{
	int itemID;
	int BagPos;
};


class CRuneExchangeFrame : public CInterfaceBase
{
public:
	CRuneExchangeFrame( CInterface* pObject );
	virtual				~CRuneExchangeFrame();

	virtual	void			RegisterFunc();	
	virtual	void			Open(int npcID);
	virtual	void			Close();
	virtual	void			Update(float elapsedTime);

	virtual	int				RE_GetCost();
	virtual	int				GetRunePlateItem( int index );	//得到裝備項目
	virtual	void			SetRunePlateItem( int index , int itemID , int ItemPos );	  //加入裝備項目
	virtual	bool			RE_CheckEqItem( int itemID ,int pos );	//檢查是否為稀有度3的裝備
	virtual	void			CheckRE_EqItem( int itype );	//得到裝備項目
	virtual	void			SetRE_Init();		//全部欄位清空

	virtual	int				RE_GetPos(int index)	{ return m_RunePlateItem[index].BagPos; }
	virtual	bool			IsRE_Visilble()		{	return m_RE_Visible;	}

	int						m_ItemLvType;  //-1 none  0 31~50  1 51~80  2 81~100
protected:
	int						m_npcID;	// 開啟NPC編號
	Eq_Exchange_Item		m_RunePlateItem[EQUIP_PLATE_MAX_ITEM];
	bool					m_RE_Visible;
};