#include "PublicEncounterFrame.h"
#include "../../netwaker_member/Net_Script/NetCli_Script_Child.h"
#include "../../netwaker_member/Net_PublicEncounter/NetCli_PublicEncounterChild.h"

int LuaFunc_PE_GetPEName(lua_State* L);
int LuaFunc_PE_GetPEMessage(lua_State* L);

int LuaFunc_PE_GetSFCount(lua_State* L);
int LuaFunc_PE_GetPEScore(lua_State* L);


int LuaFunc_PE_EventIntoSendValue(lua_State* L);
int LuaFunc_PE_GetPEIndex(lua_State* L);

int LuaFunc_PE_GetInfo(lua_State* L);

int LuaFunc_PE_GetPOBInfo(lua_State* L);
int LuaFunc_PE_GetMessage(lua_State* L);
int LuaFunc_POB_GetMessage(lua_State* L);

//****************************************************************************
CPublicEncounterFrame*	g_pPublicEncounterFrame = NULL;

//****************************************************************************
CPublicEncounterFrame::CPublicEncounterFrame(CInterface* object)
: CInterfaceBase(object)
{
	g_pPublicEncounterFrame = this;
	m_PE_Conditioncount = 0;
	m_PObSuccesCount = 0;
	m_PObFailedCount = 0;
	m_iPEIndex = 0;
	m_iPHIndex = 0;
	m_iOBIndex = 0; 
	m_iTimeGap = 0;
	m_iActivePhaseIndex = 0;
	m_PEScore = 0;

	m_SaveActSize = 0;
	SavAcrClear();

	PE_data.clear();
}
//****************************************************************************
CPublicEncounterFrame::~CPublicEncounterFrame()
{
	g_pPublicEncounterFrame = NULL;
}

//****************************************************************************
void CPublicEncounterFrame::SavAcrClear()
{
	int i;
	for( i = 0 ; i < 20 ; i ++ )
	{
		m_SaveActPEindex[i] = 0 ;
	}
	g_pPublicEncounterFrame->m_PEitemField.clear();
}
//****************************************************************************
void CPublicEncounterFrame::RegisterFunc()
{
	lua_State* L = GetLuaStateWorld();

	if (L)
	{	
			lua_register(L, "PE_GetPEName",				LuaFunc_PE_GetPEName			);
			//lua_register(L, "PE_GetPECondition",		LuaFunc_PE_GetPECondition		);
			lua_register(L, "PE_GetSFCount",			LuaFunc_PE_GetSFCount			);
			lua_register(L, "PE_GetPEMessage",			LuaFunc_PE_GetPEMessage			);
			lua_register(L, "PE_GetPEScore",			LuaFunc_PE_GetPEScore			);

			lua_register(L, "PE_EventIntoSendValue",	LuaFunc_PE_EventIntoSendValue	);	 		
			lua_register(L, "PE_GetPEIndex",			LuaFunc_PE_GetPEIndex			);	
			lua_register(L, "PE_GetInfo",				LuaFunc_PE_GetInfo				);	
			lua_register(L, "PE_GetPOBInfo",			LuaFunc_PE_GetPOBInfo			);	
			lua_register(L, "PE_GetMessage",			LuaFunc_PE_GetMessage			);	
			lua_register(L, "POB_GetMessage",			LuaFunc_POB_GetMessage			);	

			
	}
}
//****************************************************************************
void CPublicEncounterFrame::Update(float elapsedTime)
{
	for ( map<int, SPE_Info>::iterator iter = g_pPublicEncounterFrame->PE_data.begin(); iter != g_pPublicEncounterFrame->PE_data.end(); iter++ ) 
	{
		for ( map<int, SPOB_Info>::iterator it = iter->second.pOB.begin(); it != iter->second.pOB.end(); it++ ) 
		{
			if( it->second.type == 7 && it->second.nowvalue > 0)
			{
				it->second.nowvalue = it->second.nowvalue - elapsedTime*1000;

				if(g_pPublicEncounterFrame->PE_data.size() > 0)
				{
					SendWorldEvent("PE_INTO");
				}
			}
		}	
	}	
}
//****************************************************************************
//離開直接關掉介面
void CPublicEncounterFrame::PE_LeaveAndClose()
{
	SendWorldEvent("PE_LeaveAndClose");
}
//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
int LuaFunc_PE_GetPEScore(lua_State* L)
{	
	map<int,SPE_Info>::iterator it;

	it = g_pPublicEncounterFrame->PE_data.find( g_pPublicEncounterFrame->m_PEindex );

	if( it != g_pPublicEncounterFrame->PE_data.end())
	{
		lua_pushnumber( L, it->second.Score );
		return 1;	
	}
	return 0;	
}
//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
int LuaFunc_PE_GetPEName(lua_State* L)
{
	int i = 0 ;
	for ( map<int, SPE_Info>::iterator iter = g_pPublicEncounterFrame->PE_data.begin(); iter != g_pPublicEncounterFrame->PE_data.end(); iter++ ) 
	{
		if( i == 0 )
		{
			CPublicEncounter* pPE = PublicEncounterManager::GetPEInstance(iter->second.iPEIndex);
			if( !pPE || !pPE->GetUIVisible())
				return 0;	

			CPublicEncounterPhase* pPH = PublicEncounterManager::GetPhaseInstance(iter->second.iPEIndex,iter->second.iPHIndex);
			if( !pPH)
				return 0;	

			string sz1String = CNetCli_Script_Child::ReplaceSystemKeyword( g_ObjectData->GetString(pPE->GetNameKeyStr()) );
			string sz2String = CNetCli_Script_Child::ReplaceSystemKeyword( g_ObjectData->GetString(pPH->GetNameKeyStr()) );			

			lua_pushstring(L , sz1String.c_str());
			lua_pushstring(L , sz2String.c_str());
			return 2;
		}

		i++;
	}
		
	/*

	map<int,SPE_Info>::iterator it;

	it = g_pPublicEncounterFrame->PE_data.find( g_pPublicEncounterFrame->m_PEindex );
	CPublicEncounterPhase* pPH = PublicEncounterManager::GetPhaseInstance(g_pPublicEncounterFrame->m_PEindex,it->second.iPHIndex);
	
	if( !pPH)
		return 0;	

	string sz1String = CNetCli_Script_Child::ReplaceSystemKeyword( g_ObjectData->GetString(pPE->GetNameKeyStr()) );
	string sz2String = CNetCli_Script_Child::ReplaceSystemKeyword( g_ObjectData->GetString(pPH->GetNameKeyStr()) );			

	lua_pushstring(L , sz1String.c_str());
	lua_pushstring(L , sz2String.c_str());	 
	return 2;
	*/


	
	/*const char * PEName = g_pPublicEncounterFrame->m_PEName;
	const char * PHName = g_pPublicEncounterFrame->m_PHName;
	if(PHName)
	{
	string sz1String = CNetCli_Script_Child::ReplaceSystemKeyword( g_ObjectData->GetString(g_pPublicEncounterFrame->m_PEName) );
	string sz2String = CNetCli_Script_Child::ReplaceSystemKeyword( g_ObjectData->GetString(g_pPublicEncounterFrame->m_PHName) );			

	lua_pushstring(L , sz1String.c_str());
	lua_pushstring(L , sz2String.c_str());

	return 2;
	}
			   */
	return 0;  

}
//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
int LuaFunc_PE_GetSFCount(lua_State* L)
{
	int sucesscount = 0 ;  
	int failedcount = 0 ;
	for ( map<int, SPE_Info>::iterator iter = g_pPublicEncounterFrame->PE_data.begin(); iter != g_pPublicEncounterFrame->PE_data.end(); iter++ ) 
	{
		for ( map<int, SPOB_Info>::iterator it = iter->second.pOB.begin(); it != iter->second.pOB.end(); it++ ) 
		{
			if( it->second.kind == 0 )
			{
				sucesscount++;
			}
			else if( it->second.kind == 1 )
			{
				failedcount++;
			}
		}
	}							
	lua_pushnumber(L , sucesscount);
	lua_pushnumber(L , failedcount);
	return 2;
}
//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
int LuaFunc_PE_GetPEMessage(lua_State* L)
{
	const char * PEFrameMessage = g_pPublicEncounterFrame->m_PEFrameMessage;

	if(PEFrameMessage)
	{
		string szString = CNetCli_Script_Child::ReplaceSystemKeyword(g_ObjectData->GetString(g_pPublicEncounterFrame->m_PEFrameMessage) );			
		lua_pushstring(L , szString.c_str());
		return 1;
	}

	return 0;
}
//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
int LuaFunc_PE_EventIntoSendValue(lua_State* L)
{
	int	iPEIndex	= luaL_checkint( L, 1 );
	int	index		= luaL_checkint( L, 2 );
	index = index - 1;
	int TypeSize = (int)g_pPublicEncounterFrame->m_PEitemField.size();
	int i , k = 0;

	for( i = 0 ; i < TypeSize ; i++)
	{
		if(iPEIndex == g_pPublicEncounterFrame->m_PEitemField[i].PEF_PEindex)
		{

			int kind = g_pPublicEncounterFrame->m_PEitemField[i].pOB[index].kind;
			int type = g_pPublicEncounterFrame->m_PEitemField[i].pOB[index].type;
			int value = g_pPublicEncounterFrame->m_PEitemField[i].pOB[index].nowvalue;

			lua_pushnumber(L , kind);
			lua_pushnumber(L , type);
			lua_pushnumber(L , value);

			int pob_index = g_pPublicEncounterFrame->m_PEitemField[i].pOB[index].index;
			int pe_index = g_pPublicEncounterFrame->m_PEitemField[i].PEF_PEindex;
			int ph_index = g_pPublicEncounterFrame->m_PEitemField[i].PEF_PHindex;

			CPublicEncounterObjective* pOB = PublicEncounterManager::GetObjectiveInstance(pe_index, ph_index, pob_index);
			if(pOB)
			{
				const char * pob_name = pOB->GetNameKeyStr();
				string szString = CNetCli_Script_Child::ReplaceSystemKeyword(g_ObjectData->GetString(pob_name) );			

				lua_pushstring(L , szString.c_str());

				int doorsill = pOB->GetDoorsill();
				lua_pushnumber(L , doorsill);

				return 5;
			}
			int count = 0;

			lua_pushnumber(L , count);
			lua_pushnumber(L , count);
			return 5;
		}
	} 

	return 0 ;
} 
//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
int LuaFunc_PE_GetPEIndex(lua_State* L)
{
	int	index		= luaL_checkint( L, 1 );
	if( index == -1 )   //回覆目前進入區域的PE數量
	{
		lua_pushnumber(L , (lua_Number)g_pPublicEncounterFrame->PE_data.size());
		return 1;    
	}
	else
	{		 
		int count = 1;
		for ( map<int, SPE_Info>::iterator iter = g_pPublicEncounterFrame->PE_data.begin(); iter != g_pPublicEncounterFrame->PE_data.end(); iter++ ) 
		{
			if( count == index )
			{
				lua_pushnumber(L , iter->second.iPEIndex);
				return 1;  
			}		
			count++;
		}
	}
	return 0;
}
//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
int LuaFunc_PE_GetInfo(lua_State* L)
{
	int	iPEIndex = luaL_checkint( L, 1 ); 


	map<int, SPE_Info>::iterator it = g_pPublicEncounterFrame->PE_data.find(iPEIndex);
	if( it != g_pPublicEncounterFrame->PE_data.end())
	{
		CPublicEncounter* pPE = PublicEncounterManager::GetPEInstance(iPEIndex);
		if(pPE)
		{
			const char*	PEName = pPE->GetNameKeyStr();   
			string PEnameString = CNetCli_Script_Child::ReplaceSystemKeyword( g_ObjectData->GetString(PEName) );
			lua_pushstring(L , PEnameString.c_str() );     //(1)	

			const char*	PEFrameMessage = pPE->GetDescKeyStr();
			string MessageString = CNetCli_Script_Child::ReplaceSystemKeyword( g_ObjectData->GetString(PEFrameMessage) );
			lua_pushstring(L , MessageString.c_str());    //(2)

			CPublicEncounterPhase* pPH = PublicEncounterManager::GetPhaseInstance(it->second.iPEIndex, it->second.iPHIndex);

			if(pPH)
			{
				const char*	PHName = pPH->GetNameKeyStr();	
				lua_pushstring(L , CNetCli_Script_Child::ReplaceSystemKeyword( g_ObjectData->GetString(PHName) ).c_str());    //(3)

				lua_pushnumber(L , it->second.Score);    //(4)
				lua_pushnumber(L , (lua_Number)it->second.pOB.size());    //(5)
				lua_pushboolean(L, pPE->GetScoreVisible() );
				return 6;
			}
		}	
	}
	return 0;
}
//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
int LuaFunc_PE_GetPOBInfo(lua_State* L)
{
	int	iPEIndex = luaL_checkint( L, 1 ); 
	int POBIdx = luaL_checkint( L, 2 ); 

	for ( map<int, SPE_Info>::iterator iterPE = g_pPublicEncounterFrame->PE_data.begin(); iterPE != g_pPublicEncounterFrame->PE_data.end(); iterPE++ ) 
	{
		if( iterPE->second.iPEIndex == iPEIndex)
		{
			CPublicEncounter* pPE = PublicEncounterManager::GetPEInstance(iterPE->second.iPEIndex);
			if(!pPE)
				return 0;

			int i = 0 ; 
			for ( map<int, SPOB_Info>::iterator iterPOB = iterPE->second.pOB.begin(); iterPOB != iterPE->second.pOB.end(); iterPOB++ ) 
			{
				if( i == POBIdx )
				{
					CPublicEncounterObjective* pOB = PublicEncounterManager::GetObjectiveInstance(iterPE->second.iPEIndex, iterPE->second.iPHIndex, iterPOB->second.index);
					if( !pOB || !pOB->GetVisible())
						return 0;

					const char*	POBName = pOB->GetNameKeyStr();
					lua_pushstring(L , CNetCli_Script_Child::ReplaceSystemKeyword( g_ObjectData->GetString(POBName) ).c_str());     //(1)
					lua_pushnumber(L , iterPOB->second.type);		 //(2)
					lua_pushnumber(L , iterPOB->second.kind);		 //(3)
					lua_pushnumber(L , iterPOB->second.nowvalue);    //(4)
					lua_pushnumber(L , pOB->GetDoorsill());    //(5)
					return 5;
				}
			}
		}
	}

	return 0 ;
}
//---------------------------------------------------------------------
//
//----------------------------------------------------------------------
int LuaFunc_PE_GetMessage(lua_State* L)
{	
	int index = luaL_checkint( L, 1 ); 

	int i = 0 ; 
	for ( map<int, SPE_Info>::iterator iterPE = g_pPublicEncounterFrame->PE_data.begin(); iterPE != g_pPublicEncounterFrame->PE_data.end(); iterPE++ ) 
	{
		if( i == index )
		{
			CPublicEncounter* pPE = PublicEncounterManager::GetPEInstance(iterPE->second.iPEIndex);
			if(pPE)
			{
				if ( pPE->GetDescKeyStr() )
				{
					const char* keyStr = g_ObjectData->GetString(pPE->GetDescKeyStr());
					if ( keyStr )
					{
						string strNote = CNetCli_Script_Child::ReplaceSystemKeyword( keyStr );
						lua_pushstring(L, strNote.c_str());
						return 1;
					}
				}
			}
			break;
		}
		else
		{
			i++;
		}
	}
	return 0 ;
}
//---------------------------------------------------------------------
// 取階段描述 /  任務條件描述
// arg[0] : PEindex
// arg[1] : index (若arg[2] != 2 代表要條件,此時在使用此參數)

//----------------------------------------------------------------------
int LuaFunc_POB_GetMessage(lua_State* L)
{	
	int iPEIndex = luaL_checkint( L, 1 ); 
	int index2 = luaL_checkint( L, 2 ) - 1; 
	int kind = luaL_checkint( L, 3 );   //0 suc  1 fail

	for ( map<int, SPE_Info>::iterator iterPE = g_pPublicEncounterFrame->PE_data.begin(); iterPE != g_pPublicEncounterFrame->PE_data.end(); iterPE++ ) 
	{
		if( iterPE->second.iPEIndex == iPEIndex )
		{
			CPublicEncounter* pPE = PublicEncounterManager::GetPEInstance( iterPE->second.iPEIndex );
			if( !pPE)		 
				return 0;

			if( kind == 2 )
			{
				CPublicEncounterPhase* pPH = PublicEncounterManager::GetPhaseInstance(iterPE->second.iPEIndex, iterPE->second.iPHIndex);
				if( !pPH )
					return 0;
				if ( pPH->GetDescKeyStr())
				{
					const char* keyStr = g_ObjectData->GetString(pPH->GetDescKeyStr());
					if ( keyStr )
					{			
						lua_pushstring(L, CNetCli_Script_Child::ReplaceSystemKeyword( keyStr ).c_str());
						return 1;
					}
				}
			}
			int i = 0;
			for ( map<int, SPOB_Info>::iterator iterPOB = iterPE->second.pOB.begin(); iterPOB != iterPE->second.pOB.end(); iterPOB++ ) 
			{
				if( iterPOB->second.kind == kind )
				{
					if( i == index2 )
					{
						CPublicEncounterObjective* pOB = PublicEncounterManager::GetObjectiveInstance(iterPE->second.iPEIndex, iterPE->second.iPHIndex, iterPOB->second.index);
						if(!pOB)
							return 0;
						if ( pOB->GetDescKeyStr() )
						{
							const char* keyStr = g_ObjectData->GetString(pOB->GetDescKeyStr());
							if ( keyStr )
							{
								string strNote = CNetCli_Script_Child::ReplaceSystemKeyword( keyStr );
								if( strNote != "")
								{
									lua_pushstring(L, strNote.c_str());
									return 1;
								}
							}
						}
					}
					i++;
				}
			}			
		}
	}
	return 0;
}
//---------------------------------------------------------------------
//
//----------------------------------------------------------------------
void CPublicEncounterFrame::PE_Del( int iPEIndex ) 
{
	map<int,SPE_Info>::iterator it;
	
	it = PE_data.find( iPEIndex );

	if( it != PE_data.end())
	{
		PE_data.erase(it);
	}
}
//---------------------------------------------------------------------
//
//----------------------------------------------------------------------
void CPublicEncounterFrame::PE_Update( int iPEIndex ,int iActivePhaseIndex , PEChangeType SendType)
{
	bool IsPEVisible = false;

	for( map< int, SPE_Info >::iterator iter = PE_data.begin(); iter != PE_data.end(); iter++ )
	{
		if( iter->second.iPEIndex == iPEIndex)
		{
			IsPEVisible = true;
		}
	}							
	
	//ADD
	if( !IsPEVisible && SendType == ePEChangeType_Activated ){
		CPublicEncounter* pPE = PublicEncounterManager::GetPEInstance(iPEIndex);
		if(!pPE || !pPE->GetUIVisible() )
			return;

		SPE_Info PEdata;
		CPublicEncounterPhase* pPH = PublicEncounterManager::GetPhaseInstance(iPEIndex, iActivePhaseIndex);
		if( pPH )
		{
			PEdata.iPEIndex = iPEIndex;
			PEdata.iPHIndex = iActivePhaseIndex;
			PEdata.Score = pPE->GetScore();
			pPH = PublicEncounterManager::GetPhaseInstance(iPEIndex, iActivePhaseIndex);
			if( pPH )
			{
				vector<CPublicEncounterObjective*> pOB = pPH->GetObjectives();
				for (int i = 0; i < (int)pOB.size(); ++i)
				{
					SPOB_Info  POBdata;
					POBdata.nowvalue = (float)pOB[i]->GetNowValue();
					POBdata.index = pOB[i]->GetIndex();
					POBdata.kind = pOB[i]->GetKind();
					POBdata.type = pOB[i]->GetType();
					PEdata.pOB.insert(pair<int,SPOB_Info>( pOB[i]->GetIndex(), POBdata));
				} 
				PE_data.insert(pair<int,SPE_Info>( iPEIndex, PEdata));
			}
		}
		else
		{
			pPH = pPE->GetActivePhase();	
			if( pPH )
			{
				PEdata.iPEIndex = iPEIndex;
				PEdata.iPHIndex = pPH->GetIndex();
				PEdata.Score = pPE->GetScore();

				vector<CPublicEncounterObjective*> pOB = pPH->GetObjectives();
				for (int i = 0; i < (int)pOB.size(); ++i)
				{
					SPOB_Info  POBdata;
					POBdata.nowvalue = (float)pOB[i]->GetNowValue();
					POBdata.index = pOB[i]->GetIndex();
					POBdata.kind = pOB[i]->GetKind();
					POBdata.type = pOB[i]->GetType();
					PEdata.pOB.insert(pair<int,SPOB_Info>( pOB[i]->GetIndex(), POBdata));
				} 
			PE_data.insert(pair<int,SPE_Info>( iPEIndex, PEdata));
			}
		}   
	}
	else if( IsPEVisible ){	//Update
		CPublicEncounter* pPE = PublicEncounterManager::GetPEInstance(iPEIndex);

		map<int,SPE_Info>::iterator it;
		it = PE_data.find( iPEIndex );

		it->second.iPHIndex = iActivePhaseIndex;
		it->second.Score = pPE->GetScore();
		it->second.pOB.clear();

		CPublicEncounterPhase* pPH = PublicEncounterManager::GetPhaseInstance(iPEIndex, iActivePhaseIndex);
		if( pPH )
		{
			vector<CPublicEncounterObjective*> pOB = pPH->GetObjectives();
			for (int i = 0; i < (int)pOB.size(); ++i)
			{
				SPOB_Info  POBdata;
				POBdata.nowvalue = (float)pOB[i]->GetNowValue();
				POBdata.index = pOB[i]->GetIndex();
				POBdata.kind = pOB[i]->GetKind();
				POBdata.type = pOB[i]->GetType();
				it->second.pOB.insert(pair<int,SPOB_Info>( pOB[i]->GetIndex(), POBdata));
			} 
		}
	}


	
}



//---------------------------------------------------------------------
//
//----------------------------------------------------------------------
void CPublicEncounterFrame::PE_RL(PEChangeType SendType , int iPEIndex ,int iActivePhaseIndex)
{
	CPublicEncounter* pPE = PublicEncounterManager::GetPEInstance(iPEIndex);
	if(pPE)
	{
		g_pPublicEncounterFrame->m_PEindex = iPEIndex   ;
	}

	switch(SendType)
	{
	case ePEChangeType_Activated:				//進入區域	  V
	case ePEChangeType_ObjValueChage:			
	case ePEChangeType_PhaseSuccess	:			
	case ePEChangeType_PhaseFailed:				//階段失敗結束	V
	case ePEChangeType_ObjAchieved:				//階段條件達成	V
	case ePEChangeType_PlayerScore:				//積分變更		 V
	case ePEChangeType_ActivePhaseChange:		//任務階段變更	 V
		PE_Update(  iPEIndex , iActivePhaseIndex,SendType);		

		break; 
	case ePEChangeType_Success:					
	case ePEChangeType_Failed:					//任務失敗結束	  V
	case ePEChangeType_Deactivated:				//離開區域		 V
		PE_Del(  iPEIndex );
		SendWorldEvent("PE_LeaveAndClose");
		break; 
	case ePEChangeType_Close:					//離開直接關掉
		PE_data.clear();
		SendWorldEvent("PE_LeaveAndClose");
		break; 
	}	

	if(g_pPublicEncounterFrame->PE_data.size() > 0)
	{
		SendWorldEvent("PE_INTO");
	}
		

}
