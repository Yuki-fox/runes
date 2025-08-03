#pragma	   once

// **********************************************************************************************
// Globals
class	CCharacterCreate;

extern	CCharacterCreate					*g_pCharacterCreate;

// **********************************************************************************************

void	RegisterCharacterCreateFunc			();
int		CreateCharacter						(lua_State *L);

class CCharacterCreate
{
public:
	CCharacterCreate();
	~CCharacterCreate();

	static void				Initial					();
	static void				Release					();	

protected:
};