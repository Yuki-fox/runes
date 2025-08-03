#pragma once
#include "../interface.h"
#include "SmallObj/StaticFlag.h"
#include "PG/PG_Recipe.h"
class CRecipeStore :public CInterfaceBase 
{
public:
	CRecipeStore(CInterface* object);

	void RegisterFunc();	

public :
	void RecipeList( StaticFlag< DF_MAX_RECIPELIST >* pRecipeList );
	int GetRecipeNum(void);
	int GetRecipeID(int index);
	void LearnRecipe(int index);
	void LearnRecipeResult( int iRecipeID, EM_LearnRecipeResult iResult );

public:
	vector<int> m_Items;

};

extern CRecipeStore*		g_pRecipeStore;