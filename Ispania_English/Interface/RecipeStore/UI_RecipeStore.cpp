#include "UI_RecipeStore.h"
#include "../../mainproc/GameMain.h"
#include "..\..\mainproc\NetGlobal.h"
#include "../../Interface/WorldFrames/ChatFrame.h"
#include "../../netwaker_member/Net_recipe/NetCli_Recipe.h"


int LuaFunc_FSF_GetRecipeCount( lua_State *L);
int LuaFunc_FSF_GetRecipeInfo( lua_State *L);

int LuaFunc_FSF_LearnRecipe( lua_State *L);
CRecipeStore * g_pRecipeStore;


CRecipeStore::CRecipeStore(CInterface* object): CInterfaceBase( object )
{
	g_pRecipeStore=this;

}

void CRecipeStore::RegisterFunc()
{	
	lua_State* l = GetLuaStateWorld();
	if (l)
	{
		lua_register( l, "FSF_GetRecipeCount",				LuaFunc_FSF_GetRecipeCount );
		lua_register( l, "FSF_GetRecipeInfo",				LuaFunc_FSF_GetRecipeInfo );
		lua_register( l, "FSF_LearnRecipe",					LuaFunc_FSF_LearnRecipe );

	}
}
int CRecipeStore::GetRecipeID(int index)
{

	int id=-1;
	if (index>=0&&index<m_Items.size())
		id=m_Items[index];

	return id;
}

int CRecipeStore::GetRecipeNum(void)
{

	return (int)m_Items.size();

}

void  CRecipeStore::LearnRecipeResult( int iRecipeID, EM_LearnRecipeResult iResult )
{
	const char *Err=g_ObjectData->GetString("SYS_FORMULA_LEARNOK");
	char temp[512];

	sprintf(temp,"%s",Err);

	
	GameObjDbStructEx*	pRecipe	=0;
	if (iRecipeID!=-1)
		pRecipe= g_ObjectData->GetObj( iRecipeID );

	char *Name=NULL;
	if( pRecipe )	
		Name=pRecipe->GetName();
	


	switch (iResult)
	{

		case EM_LearnRecipeResult_OK://			= 0,	//學到了
			{

				vector<int>::iterator it=m_Items.begin();
				for(;it!=m_Items.end();++it)
				{

					if (*it==   iRecipeID)
					{
						m_Items.erase(it);
						break;
					}


				}

			


			g_pRuGlobalEventManager->PlaySoundByFile("sound\\interface\\skill_learn.mp3", false, 0.0f, ruSOUNDTYPE_INTERFACE);
			Err=g_ObjectData->GetString("SYS_FORMULA_LEARNOK");
				sprintf(temp,Err,Name);
			}
			break;

		case EM_LearnRecipeResult_Learned	://,		//學過了
			Err=g_ObjectData->GetString("SYS_FORMULA_LEARNED");
				sprintf(temp,Err,Name);
			break;
		case EM_LearnRecipeResult_NoMoney://	,		//錢不足
			Err=g_ObjectData->GetString("SYS_FORMULA_NOMONEY");
				sprintf(temp,"%s",Err);
			break;
		case EM_LearnRecipeResult_SkillTooLow://,		//技能點數不夠
			Err=g_ObjectData->GetString("SYS_FORMULA_SKILLLOW");
				sprintf(temp,"%s",Err);
			break;


	}
	SendWorldEvent("FSF_UPDATE");
	g_pChatFrame->SendMsgEvent( CHAT_MSG_SYSTEM, temp, "" );

}
void CRecipeStore::LearnRecipe(int index)
{

	int recipe_DBID=GetRecipeID( index);
	if (recipe_DBID!=-1)
	{


		NetCli_Recipe::SL_LearnRecipe(recipe_DBID);
	}


}
struct Recipe_sortItem
{

	int index;
	int lev;
};

bool	CompRecipeLev( Recipe_sortItem& Recipe1 , Recipe_sortItem& Recipe2 )
{


	return  (Recipe1.lev <  Recipe2.lev );


}

void CRecipeStore::RecipeList( StaticFlag< DF_MAX_RECIPELIST >* pRecipeList )
{


	m_Items.clear();
	RoleDataEx* pRole = CNetGlobal::RoleData();

	vector<Recipe_sortItem> levList;

	for( int i = 0; i < DF_MAX_RECIPELIST; i++ )
	{
#ifdef _DEBUG
		int t=rand()%10;
		if( pRecipeList->GetFlag( i ) == true )
#else
		if( pRecipeList->GetFlag( i ) == true )
#endif
		{
			// NPC 身上有該配方
			int					iRecipeID		= i + Def_ObjectClass_Recipe;
			GameObjDbStruct*	pRecipe			= g_ObjectData->GetObj( iRecipeID );

			if( pRecipe )
			{
			

				int					iRecipeKeyItem	= pRecipe->Recipe.Request.KeyItemID;

				// 檢查是否能夠正確使用
				if( iRecipeKeyItem != 0 && pRole->CheckKeyItem( pRecipe->Recipe.Request.KeyItemID ) == false )
				{

					Recipe_sortItem item;

					item.index=iRecipeID;
					item.lev=pRecipe->Recipe.Request.SkillLV;


					levList.push_back(item);
					

				//	m_Items.push_back(iRecipeID);
					// 人物身上沒有該配方的重要物品, 表沒學習過
				/*	if( pRole->BaseData.SkillValue.GetSkill( pRecipe->Recipe.Request.Skill ) < pRecipe->Recipe.Request.SkillLV )
					{
						// 人物尚未到達學習等級
						char szMsg[256];
						sprintf( szMsg, "Recipt: ID = %d, %s, %d, Skill too Low", iRecipeID, pRecipe->Name, pRecipe->Recipe.LearnFee );
						g_pChatFrame->SendMsgEvent( CHAT_MSG_SYSTEM, szMsg, "" );

					}
					else
					{
						// 人物可以學習了
						char szMsg[256];
						sprintf( szMsg, "Recipt: ID = %d, %s, %d, Okay", iRecipeID, pRecipe->Name, pRecipe->Recipe.LearnFee );
						g_pChatFrame->SendMsgEvent( CHAT_MSG_SYSTEM, szMsg, "" );

					}
					*/
				}
			}
		}
		/*
		else
		{
			// NPC 身上沒有該配方
		}
		*/

		
	}	

	std::sort(levList.begin(),levList.end(),CompRecipeLev);

	int i;
	for (i=0;i<levList.size();i++)
	{

		m_Items.push_back(levList[i].index);

	}
	SendWorldEvent("FSF_OPEN");
}

int LuaFunc_FSF_GetRecipeCount( lua_State *L)
{

	int count=0;
	if (g_pRecipeStore)
		count=g_pRecipeStore->GetRecipeNum();

		lua_pushnumber(L,count );
	return 1;
}

int LuaFunc_FSF_GetRecipeInfo( lua_State *L)
{
	int index = luaL_checkint(L, 1) -1;	
	const char *Name=NULL;
	const char *file=NULL;
	int Skilllev=0;
	int Learnlev=0;
	int cost=0;
	int r_ID=-1;
		
	if (g_pRecipeStore)
		r_ID=g_pRecipeStore->GetRecipeID(index);

	GameObjDbStructEx*	pRecipe	=0;
	if (r_ID!=-1)
			pRecipe= g_ObjectData->GetObj( r_ID );

	if( pRecipe )
	{

		RoleDataEx* pRole = CNetGlobal::RoleData();
		Skilllev = (int)pRole->BaseData.SkillValue.GetSkill( pRecipe->Recipe.Request.Skill ) ;
		Name=pRecipe->GetName();
		Learnlev=pRecipe->Recipe.Request.SkillLV;
		cost=pRecipe->Recipe.LearnFee;

		GameObjDbStruct* imageDB = CNetGlobal::GetObj(pRecipe->ImageID);
		if ( imageDB )
			file = imageDB->Image.ACTField;		
	}

	lua_pushstring(L,Name);
	lua_pushnumber(L,Skilllev );
	lua_pushnumber(L,Learnlev );

	lua_pushnumber(L,cost );
	lua_pushstring(L,file);

	return 5;
}

int LuaFunc_FSF_LearnRecipe( lua_State *L)
{
	int index = luaL_checkint(L, 1) -1;	

	if (g_pRecipeStore)
		g_pRecipeStore->LearnRecipe(index);

	return 0;
}