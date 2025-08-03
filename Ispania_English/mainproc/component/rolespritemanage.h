#ifndef _ROLESPRITEMANAGE_H
#define _ROLESPRITEMANAGE_H

#include <map>
#include <queue>
#include "../../Tools/RecyclingArray/RecyclingArray.h"
#include "rolesprite.h"

using namespace std;

#define MAX_ROLE_COUNT				2000

class CRoleSpriteManage
{
public:
	CRoleSpriteManage();
	~CRoleSpriteManage();

	CRoleSprite*			CreateRoleSprite( DWORD identify );
	CRoleSprite*			FindRoleSprite( DWORD identify );
	void					DeleteRoleSprite( DWORD identify );
	void					Update( FLOAT absoluteTime, FLOAT elapsedTime );
	void					Render();

protected:
	void					WakeRemoveSprite( DWORD identify );
	void					RemoveSprite( DWORD identify );

protected:
	CRecyclingArray<CRoleSprite, MAX_ROLE_COUNT>	m_array;
	map<DWORD, CRoleSprite*>	m_role;
	map<DWORD, float>			m_removeItem;
	float						m_absoluteTime;
};

#endif //_ROLESPRITEMANAGE_H