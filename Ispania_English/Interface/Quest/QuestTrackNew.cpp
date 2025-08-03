#include "../../mainproc/GameMain.h"

#include "QuestTrackNew.h"


//************************************************************************************************************************************************************

//int LuaFunc_GetTargetSource(lua_State* L);

//************************************************************************************************************************************************************

CQuestTrackNew*		g_pQuestTrackNew = NULL;

//************************************************************************************************************************************************************
//************************************************************************************************************************************************************
//************************************************************************************************************************************************************
//************************************************************************************************************************************************************
//************************************************************************************************************************************************************

//----------------------------------------------------------------------------

CQuestTrackNew::CQuestTrackNew(CInterface* object)
: CInterfaceBase(object),
  m_UI_Win(NULL),
  m_visibleOnLastUpdate(FALSE)	  
{
	g_pQuestTrackNew = this;
}

//----------------------------------------------------------------------------
CQuestTrackNew::~CQuestTrackNew()
{
	g_pQuestTrackNew = NULL;
}

//----------------------------------------------------------------------------
void CQuestTrackNew::RegisterFunc()
{
	lua_State* L = GetLuaStateWorld();

	if (L)
	{
	//	lua_register(L, "GetTargetSource",			LuaFunc_GetTargetSource			);
	}
}

//----------------------------------------------------------------------------
bool CQuestTrackNew::IsVisible()
{
	return m_UI_Win->IsVisible();
}

//----------------------------------------------------------------------------

//************************************************************************************************************************************************************


//************************************************************************************************************************************************************
/*int LuaFunc_GetTargetSource(lua_State* L)
{
	int	iQuestID = luaL_checkint( L, 1 );
	
	GameObjDbStructEx*	pObj = g_ObjectData->GetObj( iQuestID );
	GameObjectQuestDetail* pObj2 = &( pObj->QuestDetail );
	GameObjectQuestDetail* pQuestDetail	= NULL;

	int i , j ;
	char szPreString[256];	
	char szStr[256];
	char szKeyWord[256];

	for( j = 0 ; j < _MAX_SIZE_QUEST_CONDITION ; j++ )
	{
		int iItemID = pObj2->iRequest_ItemID[j];

		GameObjDbStructEx*	ObjDb = g_ObjectData->GetObj( iItemID );	
		lua_pushnumber(L, iItemID );
		return 1;
///
if(ObjDb != NULL)
		{
			for( i = 0 ; i < _MAX_SIZE_QUEST_ITEM ; i++)
			{
				int iItemID2 = pQuestDetail->iProcess_KilltoGetItem_GetItem[ i ] ;
				if( iItemID == iItemID2)				
				{
					lua_pushnumber(L, iItemID2 );
					return 1;
				}
			}
		}   ///
	}
	return 0;
}*/
