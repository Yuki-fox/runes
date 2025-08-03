

#include "UI_scoreBoard.h"
#include "../../netwaker_member/Net_Script/NetCli_Script_Child.h"
 CUI_scoreBoard*		g_pScoreBoardFrame=NULL;













CUI_scoreBoard::CUI_scoreBoard(CInterface* object ) :CInterfaceBase(object)
{
g_pScoreBoardFrame=this;
}

CUI_scoreBoard::~CUI_scoreBoard(void)
{

	g_pScoreBoardFrame=NULL;
}

void CUI_scoreBoard::RegisterFunc(void)
{
	lua_State* l = GetLuaStateWorld();
	if (l)
	{
		


		
	}

}
void CUI_scoreBoard::SetSmallGameMenuType( int MenuID , int Type  )
{

		
		lua_State* L = GetLuaState();
		lua_pushnumber( L, MenuID  );
		lua_setglobal( L, "arg1" );
		lua_pushnumber( L, Type  );
		lua_setglobal( L, "arg2" );


		SendWorldEvent("GSF_SETMENNTYPE");



}

void CUI_scoreBoard::SetSmallGameMenuValue( int MenuID , int ID , int Value )
{
	

	lua_State* L = GetLuaState();
	lua_pushnumber( L, MenuID  );
	lua_setglobal( L, "arg1" );
	lua_pushnumber( L, ID  );
	lua_setglobal( L, "arg2" );
	char temp[64];
	sprintf(temp,"%d",Value);
	lua_pushstring( L, temp );
	lua_setglobal( L, "arg3" );


	SendWorldEvent("GSF_INFO_UPDATE");


}
void CUI_scoreBoard::SetSmallGameMenuStr( int MenuID , int ID , const char* Content  )
{

	lua_State* L = GetLuaState();
	lua_pushnumber( L, MenuID  );
	lua_setglobal( L, "arg1" );
	lua_pushnumber( L, ID  );
	lua_setglobal( L, "arg2" );

	string strContent = CNetCli_Script_Child::ReplaceSystemKeyword( Content );




	lua_pushstring( L, strContent.c_str() );
	lua_setglobal( L, "arg3" );


	SendWorldEvent("GSF_INFO_UPDATE");

}


