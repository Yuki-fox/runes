#pragma once

#include "../interface.h"
#define DF_SKILLBOOK_PAGEMAXSPLL	14

#define DF_SKILLLIST_MAXSPELL		5


// Globals
//-------------------------------------------------------------------
class	CUI_SkillBook;
extern	CUI_SkillBook							*g_pUi_SkillBook;
//-------------------------------------------------------------------

enum EM_SKILLBOOK_RESULT
{
	EM_SKILLBOOK_OK				= 0,
	EM_SKILLBOOk_NOPLAYER		,
	EM_SKILLBOOk_NOTARGET		,
	EM_SKILLBOOK_SETAREA		,
	EM_SKILLBOOK_NOSPELL		,
	EM_SKILLBOOK_CASTING		,

};

class CUI_SkillBook : public CInterfaceBase
{
public:

	struct SpellInfo
	{
		void Reset()
		{
			iGUID			= 0;
			iPowerPoint		= 0;
			iPowerPointToLV = 0;
			iPowerLV		= 0;
			iSpecial		= 0;
			bLearned        = false;
			BaseLearnLV     = 0;        // 第1級 習得等級 排序用
			LearnLV         = 0;        // 習得等級 排序用
			Name            = "";
		}

		int		iGUID;
		int		iPowerPoint;
		int		iPowerPointToLV;
		int		iPowerLV;
		int		iSpecial;			// 特殊的技能,0 為普通招式,  1 是攻擊( 由程式產生 )
		bool    bLearned;			// 是否學會
		int     BaseLearnLV;        // 第1級 習得等級 排序用
		int     LearnLV;            // 習得等級 排序用
		string  Name;				// 名稱 排序用

		int	NeedMagicID[2];         //需要法術
		int	NeedMagicLv[2];	        //需要法術等級
	};

	CUI_SkillBook	( CInterface* pObject );
	~CUI_SkillBook	(void);
	
	virtual	void			RegisterFunc	();
	//--------------------------------------------------------------------------------------------------------
	//virtual	bool		Use				( int iIndex, int iLevel );		
	static int				Use				( int iSkillID );
		// 使用
	
	static int				GetSkillGUID	( int iType, int iIndex );
		// 使用法術書索引查詢資料庫代號
		// 傳回 0 表是該法術不存在
	static SpellInfo*		GetSkillInfo	( int iType, int iIndex );

	static	void			CancleSpelling	();

	static	bool			WaitSetTarget	();
	static	void			UpdateCooldown	(int ColdownClass, int ColdownType);

	static	void			UseAttack		();
	static	void			GetAttackIcon	( char* pszIcon );

	//virtual	void			UpdateCooldown	(int ColdownClass, int ColdownType);
	


						
	//--------------------------------------------------------------------------------------------------------
	// LUA
	//--------------------------------------------------------------------------------------------------------
	//SkillBaseStruct*		GetSkill		( int iType, int iIndex );
	//static int				GetSkillIndex	( int iType, int iIndex );
		// 傳回技能索引

	//--------------------------------------------------------------------------------------------------------


	//--------------------------------------------------------------------------------------------------------
	//int						m_iLastSkillIndex;
	//int						m_iMaxSkillList;


	static int				m_iSpellingGUID;		// 正在施展法術的資料庫代號
	//int						m_iSpellingIndex;		

	//iType = 1: // Main Job
	//iType = 2: // Sub Job
	//iType = 3: // SP
	//iType = 4: // pet
	//iType = 0: // 綜合技能
	enum ESkillType
	{
		ST_Common,
		ST_MainJob,
		ST_SubJob,
		ST_SP,
		ST_Pet,
		ST_Count,
	};

	static void SkillUpdate( int iType = -1 );								// 學會新的技能
	static void PetSkillUpdate( int petMagicID );							// 更新寵物技能
	static vector< SpellInfo >* GetSpellInfoVector( int iType );
	static bool GetPetSkillInfo( int petMagicID, vector<int>& outSkill );	// 取得寵物技能


	static bool IsSkillLearned( int iType, int skillID );

	//取得同技能位置的技能
	static int GetFinalSkillID( int iType, int skillID );
	static int GetFinalSkillID( int skillID );

protected:
	static vector< SpellInfo >	m_vecSkill[ST_Count];
	static bool m_bSpellInfoDirty[ST_Count];
	static map< int , CUI_SkillBook::SpellInfo > m_SaveLvPos_SpellInfo_Map[ST_Count];
	static map< int , int > m_SkillID_SaveLvPos_Map[ST_Count];
	static int m_nPetMagicID;
};
