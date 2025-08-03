#pragma	   once

#include <vector>
#include "../GameMain.h"
	

// **********************************************************************************************
// Globals
class	CCharacterSelect;

extern	CCharacterSelect					*g_pCharacterSelect;

// **********************************************************************************************

// functions
void	RegisterCharacterSelectFunc		();
int		GetNumCharacters				(lua_State *L);
int		GetCharacterInfo				(lua_State *L);
int		SelectCharacter					(lua_State *L);
int		DeleteCharacter					(lua_State *L);
int		EnterWorld						(lua_State *L);

using namespace std;

class CCharacterSelect
{
public:
	CCharacterSelect();
	~CCharacterSelect();

	static void				Initial					();
	static void				Release					();

	int						GetNumCharacters		()					{ return (int)m_characterInfo.size() - 1; }	
	void					SetCharacterInfo		( int index, SampleRoleData* pRoleData );
	SampleRoleData*			GetCharacterInfo		( int index );
	void					SetCurrentSelectIndex	( int index );
	int						GetCurrentSelectIndex	()					{ return m_currentSelectIndex; }
	void					ClearCharacterInfo		();
	int						GetListIndex			( int index );

protected:
	vector<SampleRoleData*>	m_characterInfo;		// 選擇角色,角色資料
	vector<int>				m_checkIndex;			// 檢查用
	int						m_currentSelectIndex;	// 目前選擇角色編號
};

