#pragma once

#include "NetCli_Recipe.h"

class NetCli_RecipeChild : public NetCli_Recipe
{
public:	
	static	bool _Init();
	static	bool _Release();

public:
	//¦X¦¨¦¨¥
	virtual void RL_LockResourceAndRefineResultOK( int RecipeID ,vector<ItemFieldStruct>& ItemList  );

	//¦X¦¨¥¢±Ñ
	virtual void RL_LockResourceAndRefineResultFailed( int RecipeID , vector<ItemFieldStruct>& ItemList  );

	//¦X¦¨±ø¥ó¤£²Å¦X
	virtual void RL_LockResourceAndRefineResultError( int RecipeID , RefineResultENUM	Result );

	virtual	void RL_ProduceItemResult(PG_Recipe_LtoC_ProduceItemResult *item);

	virtual	void RL_RecipeList( StaticFlag< DF_MAX_RECIPELIST >* pRecipeList );

	virtual void RL_LearnRecipeResult( int iRecipeID, EM_LearnRecipeResult iResult );

	virtual void ShowRequestItemError();

	virtual void RL_RefineCooldown( RefineBuffStruct	Cooldown );
};

