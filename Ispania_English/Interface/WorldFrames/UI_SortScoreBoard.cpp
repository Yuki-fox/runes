#include "UI_SortScoreBoard.h"
#include "PG/PG_Billboard.h"
#include "../netwaker_member/Net_Billboard/NetCli_Billboard.h"
#include "../Interface/WorldFrames/GuildFrame.h"
#include "../WorldFrames/ItemUTIL.h"
#include "../../mainproc/GameMain.h"
#define UScoreBoard_TIME 24*60*60*1000
 CUISortScoreBoard * g_pSortScoreBoard;
 

 int LuaFunc_SSB_GetScoreItemBoard(lua_State* L);
 int LuaFunc_SSB_GetBoardCount(lua_State* L);
 int LuaFunc_SSB_GetScoreItemInfo(lua_State* L);
 int LuaFunc_SSB_GetScoreMyInfo(lua_State* L);
 int LuaFunc_SSB_SetMyNote(lua_State* L);

struct UScoreBoard
{
	int m_Time;//過期時間

	int m_SortType ;
	int m_SortValue ;
	int m_MyRank ;
	int m_oldIndex;
	int m_MyLastRank ;

	int m_NoteTime;//過期時間
	char m_Note[65];

	std::vector<NewBillBoardRoleInfoStruct> m_ranks;

	UScoreBoard(int _SortType):m_SortType(_SortType),m_SortValue(0),m_MyRank(-1),m_MyLastRank(-1),m_NoteTime(0)
	{
		memset(m_Note,0,sizeof(m_Note));
	}

	NewBillBoardRoleInfoStruct *getIndex(int index);
	NewBillBoardRoleInfoStruct *getByName(const char *PlayerName);

	bool SetMyNote(const char *name);

	void SearchRankResult(int TotalCount , int ID , NewBillBoardRoleInfoStruct& Info);
	void SearchMyRankResult(int SortValue , int MyRank , int MyLastRank ,const char* Note);
};
bool UScoreBoard::SetMyNote(const char *Note)
{
	bool ret=false;

	char temp[512];
	
	size_t destLen=63;
	strncpy(temp,Note,63);

	temp[63]=0;

	
	const char* safeString=ItemUTIL::checkUtf8SafeString(temp,destLen,NULL);
	strncpy(m_Note,safeString,63);
	int index=m_MyRank-1;

	if (m_oldIndex!=-1)
		index=m_oldIndex;

	if (index>=0&&index<m_ranks.size())
	{
		m_ranks[index].Note=m_Note;
		ret=true;
	}

	return ret;
}
void UScoreBoard::SearchRankResult(int TotalCount , int ID , NewBillBoardRoleInfoStruct& Info)
{
	//int i;
	NewBillBoardRoleInfoStruct* pInfo=&Info;
	if (m_ranks.size()<TotalCount)
	m_ranks.resize(TotalCount);
	
	if (ID>=0&&ID<TotalCount)
		m_ranks[ID]=*pInfo;

	m_Time=timeGetTime();
}
NewBillBoardRoleInfoStruct *UScoreBoard::getIndex(int index)
{
	NewBillBoardRoleInfoStruct *ret=NULL;
	if (index>=0&&index<m_ranks.size())
	ret=&m_ranks[index];
	return ret; 
}
NewBillBoardRoleInfoStruct *UScoreBoard::getByName(const char *PlayerName)
{
	NewBillBoardRoleInfoStruct *ret=NULL;
	int count=(int)m_ranks.size();
	bool inBoard=false;
	if (count>0)
	{
		NewBillBoardRoleInfoStruct* it=&m_ranks[0];
		for(int i=0;i<count;++it,i++)
		{	
			if (it->RoleName==PlayerName)
			{
				ret=it;
				break;
			}
		}
	}
	return ret; 
}
void UScoreBoard::SearchMyRankResult(int SortValue , int MyRank , int MyLastRank ,const char* Note)
{

		m_SortValue =SortValue;
		m_MyRank  =MyRank;
		m_MyLastRank  =MyLastRank;
		int len=(int)strlen(Note);
		if (len>64)
			len=64;
		strncpy(m_Note,Note,len);

		int count=(int)m_ranks.size();
		m_oldIndex=-1;
	
		if (count>0)
		{
			const char *PlayerName=g_pGameMain->GetPlayerName();
			NewBillBoardRoleInfoStruct* it=&m_ranks[0];
			for(int i=0;i<count;++it,i++)
			{	
				if (it->RoleName==PlayerName)
				{
					m_oldIndex=i;
					break;
				}
			}
		}

}

class SortScoreBoardImp
{
public:

	~SortScoreBoardImp();
	UScoreBoard * FindScoreBoard(int SortType );
	UScoreBoard * GetScoreBoard(int SortType );


	void Initial(void );
	void Destroy(void );

	bool SetMyNote(int sortType,const char *name);
	int GetBoardCount(int sortType);
	void SortTypeRequest(int sortType);
	bool GetScoreItemBoard(int sortType);
	void ClearAllScoreBoard(void);
	void SearchRankResult(int SortType , int TotalCount , int ID , NewBillBoardRoleInfoStruct& Info);

	void SearchMyRankResult(int SortType , int SortValue , int MyRank , int MyLastRank ,const char* Note);
	std::vector<UScoreBoard*> m_UScoreBoards;
	int m_RequestTime;
};

SortScoreBoardImp *g_SortScoreBoardImp;

SortScoreBoardImp::~SortScoreBoardImp()
{
ClearAllScoreBoard();
}
void  SortScoreBoardImp::SortTypeRequest(int  sortType)
{
	int time=timeGetTime();
	if ((time-m_RequestTime)>3000)
	{
		NetCli_Billboard:: SL_New_SearchRank	( sortType,0,100 );
		NetCli_Billboard:: SL_New_SearchMyRank	( sortType );
		m_RequestTime=time;
	}



}
bool SortScoreBoardImp::SetMyNote(int sortType,const char *note)
{

	bool ret=false;

	UScoreBoard * pObj=FindScoreBoard(sortType );
	if (pObj)
	{
		int time=timeGetTime();

		ret=pObj->SetMyNote(note);
		if (ret&&(time-pObj->m_NoteTime)>(6*1000))
		{

			NetCli_Billboard::SL_New_RegisterNote ( sortType , note);

			
			pObj->m_NoteTime=time;

			


		}
		
	}
	return ret;

}
void SortScoreBoardImp::Initial(void )
{
	ClearAllScoreBoard();
	m_RequestTime=timeGetTime();
}
void SortScoreBoardImp::Destroy(void )
{
	ClearAllScoreBoard();
}

void SortScoreBoardImp::ClearAllScoreBoard(void)
{
	int i;
	int size=(int)m_UScoreBoards.size();
	for(i=0;i<size;i++)
	{

		UScoreBoard *pObj=m_UScoreBoards[i];
		if (pObj)
			delete pObj;
		m_UScoreBoards[i]=0;
		
	}
	m_UScoreBoards.clear();
}
UScoreBoard * SortScoreBoardImp::FindScoreBoard(int SortType )
{

	int i;
	UScoreBoard *ret=NULL;
	int size=(int)m_UScoreBoards.size();
	for(i=0;i<size;i++)
	{

		UScoreBoard *pObj=m_UScoreBoards[i];
		if (pObj&&pObj->m_SortType==SortType)	
		{
			ret=pObj;
			break;	
		}

	}
	return ret;

}

bool  SortScoreBoardImp::GetScoreItemBoard(int SortType)
{

	bool ret=true;
	UScoreBoard *pObj=NULL;
	pObj=FindScoreBoard(SortType);
	if (pObj)
	{

		int time=timeGetTime();
		if ((time-pObj->m_Time)<UScoreBoard_TIME)
		{
			ret =false;
			
		}

	}
	

	return ret;
	

}
int SortScoreBoardImp::GetBoardCount(int sortType)
{
	int count=0;
	UScoreBoard *pObj=FindScoreBoard(sortType);

	if (pObj)
		count= (int)pObj->m_ranks.size();

	return count;

}


UScoreBoard * SortScoreBoardImp::GetScoreBoard(int SortType )
{
	UScoreBoard *pObj=NULL;
	pObj=FindScoreBoard(SortType);
	if (!pObj)
	{
		pObj=new UScoreBoard(SortType);
		m_UScoreBoards.push_back(pObj);
	}

	return pObj;


}

void SortScoreBoardImp::SearchRankResult(int SortType , int TotalCount , int ID , NewBillBoardRoleInfoStruct& Info)
{
	UScoreBoard *pObj=GetScoreBoard(SortType);
	if (pObj)
	{

		pObj->SearchRankResult(TotalCount,ID,Info);

	}

}
void SortScoreBoardImp::SearchMyRankResult(int SortType , int SortValue , int MyRank , int MyLastRank ,const char* Note)
{
	UScoreBoard *pObj=GetScoreBoard(SortType);
	if (pObj)
	{

		pObj->SearchMyRankResult(SortValue,MyRank,MyLastRank,Note);

	}
}

CUISortScoreBoard::CUISortScoreBoard(CInterface* object):CInterfaceBase(object)
{
	g_pSortScoreBoard=this;
	g_SortScoreBoardImp=new SortScoreBoardImp;

}
CUISortScoreBoard::~CUISortScoreBoard()
{
	g_pSortScoreBoard=NULL;
	if (g_SortScoreBoardImp)
	delete g_SortScoreBoardImp;
	g_SortScoreBoardImp=0;

}
	void		 CUISortScoreBoard::		BuildFrames()		
	{
		if (g_SortScoreBoardImp)
		{
			g_SortScoreBoardImp->Initial();
		}

	}				// UI ROOT 已建立,但未載入XML之前
 void		 CUISortScoreBoard::		DestroyFrames()		
 {
	 if (g_SortScoreBoardImp)
	 {
		 g_SortScoreBoardImp->Destroy();
	 }
 }				// UI ROOT 將被刪除
void CUISortScoreBoard::SetMyNote(int sortType,const char *name)
{
		if (g_SortScoreBoardImp)
		{
			if (g_SortScoreBoardImp->SetMyNote(sortType,name))
			{

			
				lua_State *L=GetLuaState();
				lua_pushnumber( L, sortType + 1 );
				lua_setglobal( L, "arg1" );
				SendWorldEvent("SSB_SORTTYPE_UPDATE");
			}
		}

}
void CUISortScoreBoard::GetScoreItemBoard(int sortType)
{
	if (g_SortScoreBoardImp->GetScoreItemBoard(sortType))
	{
		g_SortScoreBoardImp->SortTypeRequest(  sortType);

	

	}
	else
	{
		lua_State *L=GetLuaState();
		lua_pushnumber( L, sortType + 1 );
		lua_setglobal( L, "arg1" );
		SendWorldEvent("SSB_SORTTYPE_UPDATE");


	}
		

}

void CUISortScoreBoard::RegisterFunc(void)
{
	lua_State* L = GetLuaStateWorld();
	if (L)
	{
		lua_register(L, "SSB_GetScoreItemBoard"			,LuaFunc_SSB_GetScoreItemBoard		);
		lua_register(L, "SSB_GetBoardCount"			,LuaFunc_SSB_GetBoardCount	);
		lua_register(L, "SSB_GetScoreItemInfo"			,LuaFunc_SSB_GetScoreItemInfo		);
		lua_register(L, "SSB_GetScoreMyInfo"			,LuaFunc_SSB_GetScoreMyInfo		);
		lua_register(L, "SSB_SetMyNote"					,LuaFunc_SSB_SetMyNote		);		
	}

}
void CUISortScoreBoard::SearchRankResult	( int SortType , int TotalCount , int ID , NewBillBoardRoleInfoStruct& Info ) 
{
	if (g_SortScoreBoardImp)
	{
		g_SortScoreBoardImp->SearchRankResult(SortType,TotalCount,ID,Info);

/*
		lua_State *L=GetLuaState();
		lua_pushnumber( L, SortType + 1 );
		lua_setglobal( L, "arg1" );
		SendWorldEvent("SSB_SORTTYPE_UPDATE");
		*/
	}

}
void CUISortScoreBoard::SearchMyRankResult	( int SortType , int SortValue , int MyRank , int MyLastRank , const char* Note ) 
{
	if (g_SortScoreBoardImp)
	{

		g_SortScoreBoardImp->SearchMyRankResult(SortType,SortValue,MyRank,MyLastRank,Note);
		lua_State *L=GetLuaState();
		lua_pushnumber( L, SortType + 1 );
		lua_setglobal( L, "arg1" );
		SendWorldEvent("SSB_SORTTYPE_UPDATE");
	}
}


int LuaFunc_SSB_GetScoreItemBoard(lua_State* L)
{
	int sortType=luaL_checkint(L,1)-1;

	if (g_pSortScoreBoard)
	{
		g_pSortScoreBoard->GetScoreItemBoard(sortType);
		
	}
	return 0;

}
int LuaFunc_SSB_GetBoardCount(lua_State* L)
{
	int count=0;
	int sortType=luaL_checkint(L,1)-1;

	if (g_SortScoreBoardImp)
	{
		count=g_SortScoreBoardImp->GetBoardCount(sortType);

	}
	lua_pushnumber( L, count );
	
	return 1;
}
int LuaFunc_SSB_GetScoreItemInfo(lua_State* L)
{
	int sortType=luaL_checkint(L,1)-1;
	int index=luaL_checkint(L,2)-1;

	int rank =-1;
	int lastrank=-1;
	char name[65]={0};
	char  guild [65]={0};
	int score=0;
	char note[65]={0};
	if (g_SortScoreBoardImp)
	{
		UScoreBoard *pObj=g_SortScoreBoardImp->FindScoreBoard(sortType);
		if (pObj)
		{
			NewBillBoardRoleInfoStruct *Info=pObj->getIndex(index);
			if (Info)
			{
				rank	=Info->Rank;
				lastrank=Info->LastRank;

				strncpy(name,Info->RoleName,64);

				const char* GuildName=g_pGuildFrame->GetGuildNameByID(Info->GuildID);

				if (GuildName)
				strncpy(guild,GuildName,64);
				score=Info->SortValue;

				strncpy(note,Info->Note,64);
			
			}
		

		}
		
	}

	lua_pushnumber( L, rank );
	lua_pushnumber( L, lastrank );
	lua_pushstring(L,name);
	lua_pushstring(L,guild);
	lua_pushnumber( L, score );
	lua_pushstring(L,note);

	return 6;//rank lastrank name guild score note
}
int LuaFunc_SSB_GetScoreMyInfo(lua_State* L)
{

	int sortType=luaL_checkint(L,1)-1;


	int rank =-1;
	int lastRank=-1;

	int score=0;
	//int bestScore=0;
	//int bestRank=0;
	char note[65]={0};

	if (g_SortScoreBoardImp)
	{
		UScoreBoard *pObj=g_SortScoreBoardImp->FindScoreBoard(sortType);
		if (pObj)
		{

			const char *PlayerName=g_pGameMain->GetPlayerName();

			NewBillBoardRoleInfoStruct *Info=pObj->getByName(PlayerName);
			if (Info)
			{
				rank=Info->Rank;
				lastRank=Info->LastRank;
				score=Info->SortValue;

			}
			else
			{
				rank=pObj->m_MyRank;
				lastRank=pObj->m_MyLastRank;
				score=pObj->m_SortValue;

			}
			
			strncpy(note,pObj->m_Note,64);
		}

	}
	lua_pushnumber( L, rank );
	lua_pushnumber( L, lastRank );

	lua_pushnumber( L, score );
	//lua_pushnumber( L, bestScore );
	//lua_pushnumber( L, bestRank );


	lua_pushstring(L,note);


	return 4;
}
int LuaFunc_SSB_SetMyNote(lua_State* L)
{
	int sortType=luaL_checkint(L,1)-1;
	const char *name=luaL_checkstring(L,2);

	if (g_pSortScoreBoard)
	{
		g_pSortScoreBoard->SetMyNote(sortType,name);

	}


	return 0;
}