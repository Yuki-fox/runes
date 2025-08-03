#ifndef _ROLESPRITE_H
#define _ROLESPRITE_H

#include "../../Rune/Rune.h"
#include "Scene.h"

#define MAX_ROLE_NAME				20

enum RoleSpriteBehavior
{
	RSB_STAND					= 0,
	RSB_DIE						= 1,
	RSB_WALK					= 2,	
	RSB_RUN						= 3,	
	RSB_ATTACK					= 4,
	RSB_MAGIC					= 5,

	RSB_FORCE_DWORD				= 0x7FFFFFFF,
};

enum RoleFace
{
	ROLE_FACE_ALONG				= 0,
	ROLE_FACE_BACK,
	ROLE_FACE_LEFT,
	ROLE_FACE_RIGHT,

	ROLE_FACE_FORCE_DWORD		= 0x7FFFFFFF,
};

// �@��Ĳ�o�ƥ�	
//typedef void (*RoleSpriteCallBack)	( UINT uMsg, INT paramCount, INT *paramArray );

class CRoleSprite
{
public:
	CRoleSprite();
	~CRoleSprite();		

	void					Update( FLOAT elapsedTime, CRuOptimalTerrain* pRuTerrain );
	void					Render();

	void					IncreaseRefCount()							{ m_refCount++; }
	void					DecreaseRefCount()							{ m_refCount--; }

	void					SetIdentify( DWORD identify )				{ m_identify = identify; }
	void					SetVisible( BOOL visibleBool )				{ m_visibleBool = visibleBool; }
	void					SetEnable( BOOL enableBool )				{ m_enableBool = enableBool; }
	void					SetGItemID( INT gItemID )					{ m_gItemID = gItemID; }
	void					SetBehavior( RoleSpriteBehavior behavior )	{ m_behavior = behavior; }
	void					SetWalkSpeed( FLOAT speed )					{ m_walkSpeed = speed; }
	void					SetHP( INT hp )								{ m_hp = hp; }
	void					SetMP( INT mp )								{ m_mp = mp; }
	void					SetMaxHP( INT maxHp )						{ m_maxHp = maxHp; }
	void					SetMaxMP( INT maxMp )						{ m_maxMp = maxMp; }
	void					SetLevel( INT level )						{ m_level = level; }
	void					SetModelRate( FLOAT modeRate )				{ m_modelRate = modeRate; }	
	void					SetDirection( CRuVector3 direction )		{ m_direction = direction; }
	
	void					SetPos( CRuVector3 &pos );
	void					MoveTo( CRuVector3 &pos );
	void					Move( RoleFace roleFace, FLOAT elapsedTime );
	void					SetName( LPCWSTR nameStr );
	bool					LoadCharacter( const char *resourceName );
	
	INT						GetRefCount()								{ return m_refCount; }
	DWORD					GetIdentigy()								{ return m_identify; }
	LPCWSTR					GetName()									{ return m_nameStr; }
	CRuVector3				GetPos()									{ return m_nowPos; }
	CRuVector3				GetTargetPos()								{ return m_tarPos; }
	CRuVector3				GetDirection()								{ return m_direction; }
	FLOAT					GetWalkSpeed()								{ return m_walkSpeed; }
	RoleSpriteBehavior		GetBehavior()								{ return m_behavior; }
	INT						GetHP()										{ return m_hp; }
	INT						GetMP()										{ return m_mp; }
	INT						GetMaxHP()									{ return m_maxHp; }
	INT						GetMaxMP()									{ return m_maxMp; }
	INT						GetLevel()									{ return m_level; }
	FLOAT					GetModelRate()								{ return m_modelRate; }
	INT						GetGItemID()								{ return m_gItemID; }

	BOOL					IsMoved()									{ return m_nowPos == m_tarPos; }
	BOOL					IsEnable()									{ return m_enableBool; }
	BOOL					IsVisible()									{ return m_visibleBool; }

protected:
	void					Release3DObject();
	void					SetRuEntifyPosition( CRuVector3 &position );

protected:	
	// 3D ����
	CRuACTEntity*			m_pRuEntity;				// ���⪫��
	//CRuEntity*			m_pRuEntity;				// ���⪫��

	DWORD					m_identify;					// ����s��
	INT						m_gItemID;					// Server�B�zID	
	INT						m_refCount;					// ����إ߭p�ƾ�
	BOOL					m_enableBool;				// �P��ΫD�P��
	BOOL					m_visibleBool;				// ��ܩ�����
	RoleSpriteBehavior		m_behavior;					// ������
	CRuVector3				m_nowPos;					// �{�b�y���I
	CRuVector3				m_tarPos;					// �ئa�y���I
	CRuVector3				m_direction;				// �¦V��V
	FLOAT					m_walkSpeed;				// ���ʳt��
	FLOAT					m_modelRate;				// 
	FLOAT					m_moveNeedTime;				// ���ʨ�ئa�y���I�һݮɶ�

	WCHAR					m_nameStr[MAX_ROLE_NAME];
	INT						m_level;
	INT						m_hp;
	INT						m_maxHp;
	INT						m_mp;
	INT						m_maxMp;
};

#endif //_ROLESPRITE_H