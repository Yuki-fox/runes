#pragma once

#include "PG/PG_Recipe.h"
#include "../../MainProc/NetGlobal.h"

class NetCli_Recipe : public CNetGlobal
{
protected:
    //-------------------------------------------------------------------
    
    static	bool _Init();
    static	bool _Release();	
    
	static void _PG_Recipe_LtoC_LockResourceAndRefineResult ( int Sockid , int Size , void* Data );     
	static void _PG_Recipe_LtoC_ProduceItemResult			( int Sockid , int Size , void* Data );     
	static void _PG_Recipe_LtoC_RecipeList					( int Sockid , int Size , void* Data );     
	static void _PG_Recipe_LtoC_LearnRecipeResult			( int Sockid , int Size , void* Data );     
	static void _PG_Recipe_LtoC_RefineCooldown				( int Sockid , int Size , void* Data );     


protected:
	static vector<ItemFieldStruct> _ItemList;
	//�n�D���Y�Ӱt���( Client ��)
	static bool LockRecipeResource( int Recipe );
	//�n�D�Ѷ}�t���������~( Client ��)
	static void UnlockRecipeResource( );

public:    
	static	NetCli_Recipe* This;
    NetCli_Recipe() { This = this; }

	static	NetCli_Recipe*	GetThis()						{ return This; }

	//�n�D�X�����~ 
	static bool SL_RefineItem		( int RecipeID );
	static void SL_LearnRecipe		( int RecipeID );



	virtual void RL_LockResourceAndRefineResultOK( int RecipeID ,vector<ItemFieldStruct>& ItemList  ) = 0;

	//�X������ ItemList �����Ӫ����~
	virtual void RL_LockResourceAndRefineResultFailed( int RecipeID , vector<ItemFieldStruct>& ItemList  ) = 0;

	//�X�����󤣲ŦX
	virtual void RL_LockResourceAndRefineResultError( int RecipeID , RefineResultENUM	Result ) = 0;

	virtual	void RL_ProduceItemResult(PG_Recipe_LtoC_ProduceItemResult *item) = 0;
	
	virtual	void RL_RecipeList( StaticFlag< DF_MAX_RECIPELIST >* pRecipeList ) = 0;

	virtual void RL_LearnRecipeResult( int iRecipeID, EM_LearnRecipeResult iResult ) = 0;

	virtual void ShowRequestItemError() = 0;

	virtual void RL_RefineCooldown( RefineBuffStruct	Cooldown ) = 0;
};

