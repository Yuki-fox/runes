#include "TutorialFrame.h"

#include "../../mainproc/GameMain.h"
CTutorialFrame*				g_pTutorailFrame = NULL;
#include "..\Quest\UI_QuestBook.h"
#define ID_TUTORIAL 123465535 //教學用ID


int LuaFunc_CheckTutorialFlag(lua_State *lua);//檢查旗標

int LuaFunc_SetTutorialFlag(lua_State *L);//設定旗標

int LuaFunc_Novice_AddObject(lua_State *L);//教學1
int LuaFunc_Novice_Update(lua_State *L);//檢查
int LuaFunc_Novice_CheckObjDistance(lua_State *L);//檢查距離
int LuaFunc_Novice_RemoveObject(lua_State *L);//移走
int LuaFunc_Novice_ClearAllState(lua_State *L);//清除
int LuaFunc_Novice_GetState(lua_State *L);//取得狀態
int LuaFunc_Novice_GetTouchNpc(lua_State *L);//取得NPCID
int LuaFunc_Novice_GetQuestID(lua_State *L);//取得QuestID
CTutorialFrame::CTutorialFrame(CInterface* object)
	: CInterfaceBase(object)
{
	g_pTutorailFrame = this;
	  m_state=0;
	 m_npc=-1;
	 m_questID=-1;
}

void CTutorialFrame::ClearAllState()
{

	 m_state=0 ;
	 m_npc=-1;
	 m_questID=-1;
	
}

CTutorialFrame::~CTutorialFrame()
{
}
// ----------------------------------------------------------------------------------
void CTutorialFrame::RegisterFunc()
{
	lua_State* L = GetLuaState();
	if (L)
	{
		lua_register(L, "CheckTutorialFlag",				LuaFunc_CheckTutorialFlag);
		lua_register(L, "SetTutorialFlag",					LuaFunc_SetTutorialFlag);
		lua_register(L, "Novice_AddObject",					LuaFunc_Novice_AddObject);
		lua_register(L, "Novice_Update",					LuaFunc_Novice_Update);
		lua_register(L, "Novice_CheckObjDistance",			LuaFunc_Novice_CheckObjDistance);
		lua_register(L, "Novice_RemoveObject",				LuaFunc_Novice_RemoveObject);
		lua_register(L, "Novice_ClearAllState",				LuaFunc_Novice_ClearAllState);		
		lua_register(L, "Novice_GetState",					LuaFunc_Novice_GetState);
		lua_register(L, "Novice_GetTouchNpc",				LuaFunc_Novice_GetTouchNpc);
		lua_register(L, "Novice_GetQuestID",				LuaFunc_Novice_GetQuestID);

	
	}

}
void CTutorialFrame::AddObject(int id)
{
	m_idList.push_back(id);
}
vector<int>& CTutorialFrame::GetObject()
{

	return m_idList;

}
void CTutorialFrame::RemoveObject(int id)
{
	vector<int>::iterator it=m_idList.begin();
	for(;it!=m_idList.end();++it)
	{
		if(*it==id)
		{
			g_pGameMain->DeleteSprite(id+ID_TUTORIAL);
			m_idList.erase(it);
			break;
		}
	}
}
void CTutorialFrame::doState(int flag)
{

	m_state|= 1<<flag;
}



float CTutorialFrame::CheckObjDistance(int id)
{
	float disance=100000;

	CRoleSprite *sprite= g_pGameMain->FindSpriteByWorldGUID(id+ID_TUTORIAL);
	CRoleSprite *player= g_pGameMain->GetPlayer();
	if (sprite&&player)
		disance=(player->GetPos() - sprite->GetPos()).Magnitude();
	return disance;
}
int CTutorialFrame::GetTouchNpc(void)
{

	return m_npc;
}
void CTutorialFrame::doClickNpc(int npc)
{

	m_npc=npc;
}
void CTutorialFrame::doNewQuest(int iQuestID)
{

	m_questID=iQuestID;
}

int CTutorialFrame::GetQuestID(int questID)
{

	//判斷任務表上已經有接了
	if (questID!=-1)
	{
		vector< int >::iterator it = g_pQuestBook->m_vecQuestID.begin();
		for( ; it != g_pQuestBook->m_vecQuestID.end(); it++ )
		{
			 if (*it==questID) 
			 {
				 m_questID=questID;
				 break;
			 }		
		}
	}







	return m_questID;
}
int CTutorialFrame::CheckAllObjects()
{

	return (int)m_idList.size();
}
int CTutorialFrame::GetState(void)
{

	return m_state;
}
bool CTutorialFrame::TutorialStep(int step)
{
	bool ret=false;
	switch(step )
	{
	case 1:
		ret=CheckAllObjects()==0;
		break;

	}

	return ret;

}
void CTutorialFrame::TriggerTutorial(int id)
{
	// 旗標已開啟
	if ( g_pGameMain->GetTutorialState(id) )
		return;

	g_pGameMain->SetTutorialState(id, true);

	lua_State* L = GetLuaState();
	if ( L )
	{
		lua_pushnumber(L, id+1);
		lua_setglobal(L, "arg1");
		SendWorldEvent("TUTORIAL_TRIGGER");
	}
}
//------------------------------------------------
//檢查旗標
int LuaFunc_CheckTutorialFlag(lua_State *L)
{
	int v=0;
	if ( lua_isnumber(L, 1) )
	{
		int index=(int)lua_tonumber(L, 1) - 1;


		 v=g_pGameMain->GetTutorialState(index);
		

	}

	lua_pushboolean(L, v );
   return 1;

}

//------------------------------------------------
//設定旗標
int LuaFunc_SetTutorialFlag(lua_State *L)
{
	
	if ( lua_isnumber(L, 1) )
	{
		int index=(int)lua_tonumber(L, 1) - 1;

		g_pGameMain->SetTutorialState(index, (lua_toboolean(L, 2) != 0));
	}
	return 0;

}

void MinG_CreateClientSprite(const char *rosName,CRuVector3& pos ,	float direction , int WorldGUID)
{
	//產生一個地標
	CRoleSprite* pSprite = NULL;
	int DBID=-1;


	bool isNew;
	pSprite = g_pGameMain->CreateSprite( WorldGUID, DBID, WorldGUID, isNew );
	assert( pSprite );
	if ( pSprite )
	{

		
		float angle = direction * PI / 180.0f;

		//pSprite->SetObjectId(OrgObjID);
		pSprite->LoadRuEntity( rosName );
		if (!pSprite->PlayMotion("idle"))
		pSprite->PlayMotion("stand_idle01");

		pSprite->SetShadowType(eSpriteShadowType_None);		// 沒有影子
		pSprite->SetCursorState(false);		// 可以被游標檢測
		pSprite->SetTouchState(false);		// 可以被點選
		pSprite->SetTargetState(false);		// 不能出現目標框
		pSprite->SetNameVisible(false);		// 不能出現頭上字串


		//case EM_GameObjectMineType_Mine:
		pSprite->SetMinimapIcon( 0 ); // EM_MINIMEM_MinimapOption_Mine
		pSprite->SetCheckedStatus( eChecked_Null );  

		// Set fixed sprite orienting axis if the axis is valid

		// Set fixed orienting axis
		pSprite->SetOrientingAxis(CRuVector3(0.0, 1.0, 0.0));

		// Enable re-basing of direction vector around the orienting axis
		pSprite->SetRebaseDirection(true);

		// Disable orient to surface normal
		pSprite->SetOrientToSurfaceNormal(false);

		pSprite->SetName( "Client_Point" );
		CRuQuaternion orientation;
		orientation.FromAngleAxis(CRuVector3(0.0, 1.0, 0.0), angle);
		CRuVector3 direction = CRuVector3( 0.0f, 0.0f, -1.0f) * orientation.GetMatrix();
		pSprite->SetDirection( direction , true, true);
		pSprite->SetFade(false, 1.0f);
		pSprite->SetGravityMode(false);

		pSprite->SetPos(pos);

		// 設定sprite顯示
		//pSprite->SetVisible(GraphObj->Mode.Show);
		pSprite->SetVisible( true );

		// 設定sprite碰撞開關
		pSprite->EnableCollision(false);

		pSprite->SetCanAttackState(false);
		pSprite->SetEnemyState(false);

	}



}


int LuaFunc_Novice_AddObject(lua_State *L)
//教學1
{
	const char* name = lua_tostring(L, 1);
	float x = (float)lua_tonumber(L, 2);
	float y = (float)lua_tonumber(L, 3);
	float z = (float)lua_tonumber(L, 4);
	float dir = (float)lua_tonumber(L, 5);
	int id = (int)lua_tonumber(L, 6);

	MinG_CreateClientSprite(name,CRuVector3(x,y,z),dir,ID_TUTORIAL+id);
	
	g_pTutorailFrame->AddObject(id);

	return 0;
}

int LuaFunc_Novice_Update(lua_State *L)
{

	int index=(int )lua_tonumber(L, 1);


	bool b=g_pTutorailFrame->TutorialStep(index);
	lua_pushboolean(L,b);
	
	
	return 1;
}

int LuaFunc_Novice_CheckObjDistance(lua_State *L)
{
	int id=(int )lua_tonumber(L, 1);
	float f=g_pTutorailFrame->CheckObjDistance(id);
	lua_pushnumber(L,f);


	return 1;

}
int LuaFunc_Novice_RemoveObject(lua_State *L)
{
	int id=(int )lua_tonumber(L, 1);
		g_pTutorailFrame->RemoveObject(id);

		return 0;


}

int LuaFunc_Novice_ClearAllState(lua_State *L)//清除
{
	g_pTutorailFrame->ClearAllState();
	
	return 0;
}


int LuaFunc_Novice_GetState(lua_State *L)//取得狀態
{

	int state=g_pTutorailFrame->GetState();

	int s=1;
	for (int i=0;i<CTutorialFrame::STATE_MAX;i++)
	{

		lua_pushboolean(L,(state&s));
		s=s<<1;

	}

	return CTutorialFrame::STATE_MAX;
}
int LuaFunc_Novice_GetTouchNpc(lua_State *L)
{

	int id =g_pTutorailFrame->GetTouchNpc();
	lua_pushnumber(L,id);
	return 1;
}

int LuaFunc_Novice_GetQuestID(lua_State *L)//取得QuestID
{

	int id=-1;
	if (lua_isnumber(L,1))
	 id=(int )lua_tonumber(L, 1);

	id =g_pTutorailFrame->GetQuestID(id);
	lua_pushnumber(L,id);
	return 1;

}