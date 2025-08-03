#include <windows.h>
#include "rolespritemanage.h"

//-------------------------------------------------------------------------------------------------
CRoleSpriteManage::CRoleSpriteManage()
{
	m_role.clear();
	m_absoluteTime = 0.0f;
}

//-------------------------------------------------------------------------------------------------
CRoleSpriteManage::~CRoleSpriteManage()
{
}

//-------------------------------------------------------------------------------------------------
CRoleSprite* CRoleSpriteManage::CreateRoleSprite( DWORD identify )
{
	CRoleSprite *pNew;
	if ( (pNew = FindRoleSprite( identify )) == NULL )
	{
		pNew = m_array.New();
		if ( pNew == NULL )
			return NULL;

		pNew->SetIdentify( identify );
		m_role.insert( make_pair(identify, pNew) );
	}
	else
		WakeRemoveSprite( identify );

	pNew->IncreaseRefCount();
	return pNew;
}

//-------------------------------------------------------------------------------------------------
CRoleSprite* CRoleSpriteManage::FindRoleSprite( DWORD identify )
{
	map<DWORD, CRoleSprite*>::iterator iter = m_role.find( identify );
	if ( iter != m_role.end() )
		return iter->second;
	return NULL;
}

//-------------------------------------------------------------------------------------------------
// 刪除物件時並不會立即被刪除,需經過1秒鐘後才被真正移除
void CRoleSpriteManage::DeleteRoleSprite( DWORD identify )
{
	CRoleSprite *pRole = FindRoleSprite( identify );
	if ( pRole )
	{
		pRole->DecreaseRefCount();
		if ( pRole->GetRefCount() <= 0 )
			m_removeItem.insert( make_pair(identify, m_absoluteTime + 1.0f) );
	}
}

//-------------------------------------------------------------------------------------------------
void CRoleSpriteManage::Update( FLOAT absoluteTime, FLOAT elapsedTime )
{
	m_absoluteTime = absoluteTime;

	// 是否有待移除物件
	if ( !m_removeItem.empty() )
	{
		map<DWORD, float>::iterator iter = m_removeItem.begin();
		while ( iter != m_removeItem.end() )
		{
			map<DWORD, float>::iterator removeItem = iter;
			iter++;

			if ( iter->second > m_absoluteTime )
			{
				RemoveSprite( removeItem->first );
				m_removeItem.erase( removeItem );
			}
		}
	}

	for ( map<DWORD, CRoleSprite*>::iterator iter = m_role.begin(); iter != m_role.end(); iter++ )
	{
		if ( (iter->second)->IsEnable() )
			(iter->second)->Update( elapsedTime );
	}
}

//-------------------------------------------------------------------------------------------------
void CRoleSpriteManage::Render()
{
	for ( map<DWORD, CRoleSprite*>::iterator iter = m_role.begin(); iter != m_role.end(); iter++ )
	{
		if ( (iter->second)->IsVisible() )
			(iter->second)->Render();
	}
}

//-------------------------------------------------------------------------------------------------
void CRoleSpriteManage::WakeRemoveSprite( DWORD identify )
{	
	map<DWORD, float>::iterator iter = m_removeItem.find( identify );
	if ( iter != m_removeItem.end() )
		m_removeItem.erase( iter );
}

//-------------------------------------------------------------------------------------------------
void CRoleSpriteManage::RemoveSprite( DWORD identify )
{
	map<DWORD, CRoleSprite*>::iterator iter = m_role.find( identify );
	if ( iter != m_role.end() )
	{
		m_array.Delete( iter->second );				// 自實體陣列刪除
		m_role.erase( iter );
	}
}