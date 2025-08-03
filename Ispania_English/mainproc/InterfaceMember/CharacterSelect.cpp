#include "CharacterSelect.h"

// --------------------------------------------------------------------------------------
// Globals
CCharacterSelect*						g_pCharacterSelect = NULL;

// --------------------------------------------------------------------------------------
void CCharacterSelect::Initial()
{
	if ( g_pCharacterSelect == NULL )
	{
		g_pCharacterSelect = new CCharacterSelect;

		CFrame::RegisterEventString( _Def_UIEvent_CharacterListUpdate_ );
		CFrame::RegisterEventString( _Def_UIEvent_UpdateSelectedCharacter_ );
	}
}

// --------------------------------------------------------------------------------------
void CCharacterSelect::Release()
{
	SAFE_DELETE( g_pCharacterSelect );
}

// --------------------------------------------------------------------------------------
CCharacterSelect::CCharacterSelect()
{
	ClearCharacterInfo();
}

// --------------------------------------------------------------------------------------
CCharacterSelect::~CCharacterSelect()
{
	ClearCharacterInfo();
}

// --------------------------------------------------------------------------------------
void CCharacterSelect::SetCharacterInfo( int index, SampleRoleData* pRoleData )
{
	m_checkIndex.push_back( index );
	m_characterInfo.push_back( pRoleData );
}

// --------------------------------------------------------------------------------------
SampleRoleData* CCharacterSelect::GetCharacterInfo( int index )
{
	if ( index > 0 && index < (int)m_characterInfo.size() )
		return m_characterInfo[index];
	return NULL;
}

// --------------------------------------------------------------------------------------
void CCharacterSelect::SetCurrentSelectIndex( int index )
{
	if ( m_currentSelectIndex != index )
	{
		m_currentSelectIndex = index;
		CFrame::SendEventString( _Def_UIEvent_UpdateSelectedCharacter_, m_currentSelectIndex, 0 );
	}
}

// --------------------------------------------------------------------------------------
void CCharacterSelect::ClearCharacterInfo()
{
	m_characterInfo.clear();
	m_checkIndex.clear();
	m_currentSelectIndex = 0;

	// 
	m_checkIndex.push_back(0);
	m_characterInfo.push_back(NULL);
}

// --------------------------------------------------------------------------------------
int CCharacterSelect::GetListIndex( int index )
{
	if ( index > 0 && index < (int)m_checkIndex.size() )
		return m_checkIndex[index];
	return -1;
}

// --------------------------------------------------------------------------------------
void RegisterCharacterSelectFunc()
{
	g_pLuaScript->Register("GetNumCharacters"	, GetNumCharacters);
	g_pLuaScript->Register("GetCharacterInfo"	, GetCharacterInfo);
	g_pLuaScript->Register("SelectCharacter"	, SelectCharacter);
	g_pLuaScript->Register("DeleteCharacter"	, DeleteCharacter);
	g_pLuaScript->Register("EnterWorld"			, EnterWorld);
}

// --------------------------------------------------------------------------------------
int	GetNumCharacters(lua_State *L)
{
	luaStack l(L);
	l.Push( g_pCharacterSelect->GetNumCharacters() );
	return 1;
}

// --------------------------------------------------------------------------------------
int	GetCharacterInfo(lua_State *L)
{
	luaStack l(L);
	SampleRoleData* pRoleData = g_pCharacterSelect->GetCharacterInfo( l.GetParam<int>(1) );
	if ( pRoleData == NULL )
		return 0;

	l.Push( pRoleData->RoleName.Begin() );				// Name
	l.Push( static_cast<int>(pRoleData->Race) );		// Race
	l.Push( static_cast<int>(pRoleData->Sex) );			// Sex
	l.Push( pRoleData->Level );							// Level
	l.Push( pRoleData->ZoneName.Begin() );				// Zone Name

	return 5;
}

// --------------------------------------------------------------------------------------
int	SelectCharacter(lua_State *L)
{
	luaStack l(L);
	g_pCharacterSelect->SetCurrentSelectIndex( l.GetParam<int>(1) );	
	return 0;
}

// --------------------------------------------------------------------------------------
int	DeleteCharacter(lua_State *L)
{
	luaStack l(L);
	if ( l.GetParamCount() >= 1 )
	{
		int index = g_pCharacterSelect->GetListIndex( l.GetParam<int>(1) );
		CNetCli_Login::DeleteRole( index );
	}	
	return 0;
}

// --------------------------------------------------------------------------------------
int	EnterWorld(lua_State *L)
{
	int index = g_pCharacterSelect->GetListIndex( g_pCharacterSelect->GetCurrentSelectIndex() );
	CNetCli_Login::SelectRole( index );
	return 0;
}