#pragma once

#include "../interface.h"
#include "../netwaker_member/net_login/NetCli_Login.h"

class CCharacterCreate;
extern CCharacterCreate*		g_pCharacterCreate;

enum eCREATE_CHARACTER_RESULT
{
	CREATE_CHARACTER_SUCCESS		= 0,
	CREATE_CHARACTER_ERR_SHORT		= -1,				// 太短
	CREATE_CHARACTER_ERR_LONG		= -2,				// 太長
	CREATE_CHARACTER_ERR_WRONGFUL	= -3,				// 不合法字元
	CREATE_CHARACTER_ERR_TOKEN		= -4,				// 特別字元
};

class CRoleSprite;
class CCharacterCreate : public CInterfaceBase
{
public:
	CCharacterCreate(CInterface* object);
	virtual ~CCharacterCreate();

	//virtual	void				Update(float elapsedTime);
	virtual	void				RegisterFunc();
	virtual	void				CreateRoleSprite();
	virtual	void				DeleteRoleSprite();
	virtual	void				UpdateRoleSprite(int race, int sex, int vocation, int face, int hair, int skinColor, int hairColor);
	virtual	void				UpdateRoleSpriteBoneScale(float* boneScale);

	virtual	eCREATE_CHARACTER_RESULT	CreateCharacter(const char* name);
	virtual	CRoleSprite*		GetRoleSprite()							{ return m_sprite; }
	virtual	CRoleSprite*		GetCharacterSprite()					{ return m_sprite; }
	virtual	void				CreateResult(int emResult, int itemID[5], int itemCount[5]);
	virtual void				RenameResult(int emResult);

	virtual	eCREATE_CHARACTER_RESULT	CheckRightfulName(const char* name);

	virtual void SetLookAtFace( bool bLookAtFace , bool bUpdateBoneScale = true );

	virtual int					GetRace()								{ return m_race; }
	virtual int					GetSex()								{ return m_sex; }
	virtual int					GetVocation()							{ return m_vocation; }
	virtual int					GetFace()								{ return m_face; }
	virtual int					GetHair()								{ return m_hair; }
	virtual int					GetSkinColor()							{ return m_skinColor; }
	virtual int					GetSkinColorIndex();
	virtual int					GetHairColor()							{ return m_hairColor; }

	virtual void				RandomHair();
	virtual void				RandomFace();

	virtual void				SetMaxCreateRoleCount(int count)		{ m_maxCreateRoleCount = count; }
	virtual int					GetMaxCreateRoleCount()					{ return m_maxCreateRoleCount; }

	virtual void				ClearReserveRole();
	virtual vector<ReserverRoleStruct>&	GetReserveRole()				{ return m_reserveCharacter; }
	virtual void				R_ReserveRole(ReserverRoleStruct& info);

protected:
	CRoleSprite*	m_sprite;

	int				m_race;
	int				m_sex;
	int				m_vocation;
	int				m_face;
	int				m_hair;
	int				m_skinColor;
	int				m_hairColor;
	float			m_headScale;
	float			m_breastsScale;
	float			m_bodyScale;
	float			m_chestScale;
	float			m_pelvisScale;
	float			m_armScale;
	float			m_forearmScale;
	float			m_handScale;
	float			m_legScale;
	float			m_calfScale;
	float			m_footScale;
	//float			m_delta;

	bool			m_bLookAtFace;

	int				m_maxCreateRoleCount;

	vector<ReserverRoleStruct>	m_reserveCharacter;

	RuPropertyType m_paperdollTextureDetail;
	RuPropertyType m_textureDetail;
};