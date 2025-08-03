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
			BaseLearnLV     = 0;        // ��1�� �߱o���� �Ƨǥ�
			LearnLV         = 0;        // �߱o���� �Ƨǥ�
			Name            = "";
		}

		int		iGUID;
		int		iPowerPoint;
		int		iPowerPointToLV;
		int		iPowerLV;
		int		iSpecial;			// �S���ޯ�,0 �����q�ۦ�,  1 �O����( �ѵ{������ )
		bool    bLearned;			// �O�_�Ƿ|
		int     BaseLearnLV;        // ��1�� �߱o���� �Ƨǥ�
		int     LearnLV;            // �߱o���� �Ƨǥ�
		string  Name;				// �W�� �Ƨǥ�

		int	NeedMagicID[2];         //�ݭn�k�N
		int	NeedMagicLv[2];	        //�ݭn�k�N����
	};

	CUI_SkillBook	( CInterface* pObject );
	~CUI_SkillBook	(void);
	
	virtual	void			RegisterFunc	();
	//--------------------------------------------------------------------------------------------------------
	//virtual	bool		Use				( int iIndex, int iLevel );		
	static int				Use				( int iSkillID );
		// �ϥ�
	
	static int				GetSkillGUID	( int iType, int iIndex );
		// �ϥΪk�N�ѯ��ެd�߸�Ʈw�N��
		// �Ǧ^ 0 ��O�Ӫk�N���s�b
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
		// �Ǧ^�ޯ����

	//--------------------------------------------------------------------------------------------------------


	//--------------------------------------------------------------------------------------------------------
	//int						m_iLastSkillIndex;
	//int						m_iMaxSkillList;


	static int				m_iSpellingGUID;		// ���b�I�i�k�N����Ʈw�N��
	//int						m_iSpellingIndex;		

	//iType = 1: // Main Job
	//iType = 2: // Sub Job
	//iType = 3: // SP
	//iType = 4: // pet
	//iType = 0: // ��X�ޯ�
	enum ESkillType
	{
		ST_Common,
		ST_MainJob,
		ST_SubJob,
		ST_SP,
		ST_Pet,
		ST_Count,
	};

	static void SkillUpdate( int iType = -1 );								// �Ƿ|�s���ޯ�
	static void PetSkillUpdate( int petMagicID );							// ��s�d���ޯ�
	static vector< SpellInfo >* GetSpellInfoVector( int iType );
	static bool GetPetSkillInfo( int petMagicID, vector<int>& outSkill );	// ���o�d���ޯ�


	static bool IsSkillLearned( int iType, int skillID );

	//���o�P�ޯ��m���ޯ�
	static int GetFinalSkillID( int iType, int skillID );
	static int GetFinalSkillID( int skillID );

protected:
	static vector< SpellInfo >	m_vecSkill[ST_Count];
	static bool m_bSpellInfoDirty[ST_Count];
	static map< int , CUI_SkillBook::SpellInfo > m_SaveLvPos_SpellInfo_Map[ST_Count];
	static map< int , int > m_SkillID_SaveLvPos_Map[ST_Count];
	static int m_nPetMagicID;
};
