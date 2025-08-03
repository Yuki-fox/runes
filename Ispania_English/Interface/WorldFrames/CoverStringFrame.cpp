#include "CoverStringFrame.h"
#include "../../mainproc/GameMain.h"
#include "../../netwaker_member/NetWakerClientInc.h"
#include <map>

//****************************************************************************
CCoverStringFrame*	g_pCoverStringFrame = NULL;

int LuaFunc_CString_AddString(lua_State* L);
int LuaFunc_CString_RemoveString(lua_State* L);
int LuaFunc_CString_GetString(lua_State* L);
int LuaFunc_CString_GetIndex(lua_State* L);
//****************************************************************************
CCoverStringFrame::CCoverStringFrame(CInterface* object): CInterfaceBase(object)
{
	g_pCoverStringFrame = this;
}
//****************************************************************************
CCoverStringFrame::~CCoverStringFrame()
{
	g_pCoverStringFrame = NULL;
}
//****************************************************************************
void CCoverStringFrame::RegisterFunc()
{
	lua_State* L = GetLuaStateWorld();
	if (L)
	{	
		lua_register(L, "CString_AddString",	LuaFunc_CString_AddString		);
		lua_register(L, "CString_RemoveString",	LuaFunc_CString_RemoveString	);
		lua_register(L, "CString_GetString",	LuaFunc_CString_GetString		);
		lua_register(L, "CString_GetIndex",		LuaFunc_CString_GetIndex		);
		
	}
}
//****************************************************************************
void CCoverStringFrame::Save()
{
	FILE* fptr = NULL;
	wstring globalName = GetGlobalPathW(L"CoverString.bsd");
	wstring localName = GetLocalPathW(L"CoverString.bsd");
	string CString;
	int i ;
	if ( (fptr = _wfopen(localName.c_str(), L"wb")) != NULL )
	{
		int iCStringSize	= (int)g_pCoverStringFrame->m_CStringitemField_Cover.size();

		// 檔頭資料II
		fwrite(&iCStringSize, sizeof(int), 1, fptr);

		// 記錄資料II
		CoverStringstructSave temp;
		for( i = 0 ; i < iCStringSize ; i++ )
		{
			strncpy( temp.buf, m_CStringitemField_Cover[i].cstring.c_str(), 128 );
			fwrite( &temp , sizeof(CoverStringstructSave) , 1 ,fptr );
		}

		iCStringSize	= (int)g_pCoverStringFrame->m_CStringitemField_Show.size();
		// 檔頭資料II
		fwrite(&iCStringSize, sizeof(int), 1, fptr);

		// 記錄資料II
		for( i = 0 ; i < iCStringSize ; i++ )
		{
			strncpy( temp.buf, m_CStringitemField_Show[i].cstring.c_str(), 128 );
			fwrite(&temp , sizeof(CoverStringstructSave) , 1 ,fptr );
		}


		// 關閉檔案
		fclose(fptr);
	}
}
//****************************************************************************
void CCoverStringFrame::Load()
{
	FILE* fptr = NULL;
	wstring globalName = GetGlobalPathW(L"CoverString.bsd");
	wstring localName = GetLocalPathW(L"CoverString.bsd");

	m_CStringitemField_Cover.clear();
	m_CStringitemField_Show.clear();
	m_CStringitemField_Overlap.clear();
	// 讀取共用資料
	if ( (fptr = _wfopen(localName.c_str(), L"rb")) != NULL )
	{
		string CString;
		int iCStringSize;

		fread(&iCStringSize, sizeof(int), 1, fptr);
		for( int i = 0; i < iCStringSize; i++ )
		{
			CoverStringstructSave temp;
			CoverStringstruct Info;
			fread(&temp, sizeof(CoverStringstructSave), 1, fptr);
			Info.cstring = temp.buf;

			m_CStringitemField_Cover.push_back(Info);

			if ( g_pGameMain->IsPatternStringVisible((BYTE*) Info.cstring.c_str())  )
			{
				g_pGameMain->PushPatternString((BYTE*) Info.cstring.c_str());
			}
		}

		fread(&iCStringSize, sizeof(int), 1, fptr);
		for( int i = 0; i < iCStringSize; i++ )
		{
			CoverStringstructSave temp;
			CoverStringstruct Info;
			fread(&temp, sizeof(CoverStringstructSave), 1, fptr);
			Info.cstring = temp.buf;

			m_CStringitemField_Show.push_back(Info);
			m_Overlap.cstring = Info.cstring;
			g_pGameMain->RemovePatternString((BYTE*) Info.cstring.c_str() , 2);
		}


		// 關閉檔案
		fclose(fptr);
	}	

	SetSaveUpdateTime();
}
//****************************************************************************
void CCoverStringFrame::MatchOverlap()
{
	int k = 0;
	CoverStringstruct Info;
	if( m_Overlap.cstring.length() > 0 )
	{	
		if( m_CStringitemField_Overlap.size() > 0) 
		{
			Info = m_CStringitemField_Overlap.back();
			if( strcmp(Info.cstring.c_str() , m_Overlap.cstring.c_str()) == 0 )
			{
				k = 1 ;
			}	
		}
		for ( vector<CoverStringstruct>::iterator iter = m_CStringitemField_Overlap.begin(); iter != m_CStringitemField_Overlap.end(); iter++ )
		{
			if( strcmp(iter->cstring.c_str() , m_Overlap.cstring.c_str()) == 0 )
			{
				k = 1 ;
			}
		}
		if( k == 0 )
		{
			m_CStringitemField_Overlap.push_back(m_Overlap);
		}
	}
}
bool CCoverStringFrame::MatchCoverStringFromOverlap( BYTE* text , int index)
{
	int i , k = 0;
	int iLen , textlen = 0 , templen = 0;
	if( ( index - 1 ) %3 == 0 ){ index -= 1; } 
	if( ( index - 2 ) %3 == 0 ){ index -= 2; }

	if( m_CStringitemField_Show.size() >= 1 )
	{
		CoverStringstruct Info = m_CStringitemField_Show.back();	
		iLen = -1;
		k = 0;

		BYTE* temp2 = (BYTE*) Info.cstring.c_str();
		if( sizeof(temp2) <= sizeof(text) )
		{
			for( i = 0 ; i < sizeof(temp2)*3 ; i+=3 )
			{
				if( text[index] == temp2[i] )
				{
					iLen = i;
					break;
				}
			}
			if( iLen >= 0)
			{	
				for( i = 0 ; i < iLen ; i ++ )
				{
					if( text[ index - i ] != temp2[iLen - i] )
					{
						k = 1;
						break;
					}
				}
				textlen = 0 ;
				templen = 0 ;
				while( text[index + textlen] != 0 )
				{
					textlen++;
				}
				while( temp2[iLen + templen] != 0 )
				{
					templen++;
				}

				if( textlen >= templen )
				{
					i = 1;
					while( text[index + i] != 0 )
					{
						if( text[index + i] == 0 && temp2[iLen + i] != 0)
						{
							k = 1;
							break;
						} 
						else if( text[index + i] != 0 && temp2[iLen + i] == 0 )
						{
							k = 1;
							break;
						}
						else if( text[ index + i ] != temp2[iLen + i] )
						{
							k = 1;
							break;
						} 
						else if( text[ index + i ] == temp2[iLen + i] )
						{
							i++;				
						}
					}
					if( k == 0)
					{
						return false;
					}
				}
				else
				{
					return true;
				}
			}
		}
	
	
	}


	for ( vector<CoverStringstruct>::iterator iter = m_CStringitemField_Show.begin(); iter != m_CStringitemField_Show.end(); iter++ )
	{
		iLen = -1;
		k = 0;
		BYTE* temp = (BYTE*) iter->cstring.c_str();

		if( sizeof(temp) <= sizeof(text) )
		{
			for( i = 0 ; i < sizeof(temp)*3 ; i+=3 )
			{
				if( text[index] == temp[i] )
				{
					iLen = i;
					break;
				}
			}
			if( iLen >= 0)
			{	
				for( i = 0 ; i < iLen ; i ++ )
				{
					if( text[ index - i ] != temp[iLen - i] )
					{
						k = 1;	
					}
				}

				textlen = 0 ;
				templen = 0 ;
				while( text[index + textlen] != 0 )
				{
					textlen++;
				}
				while( temp[iLen + templen] != 0 )
				{
					templen++;
				}

				i = 1;
				while( text[index + i] != 0 )
				{
					if( text[index + i] == 0 && temp[iLen + i] != 0)
					{
						k = 1;
						break;
					} 
					else if( text[index + i] != 0 && temp[iLen + i] == 0 )
					{
						k = 1;
						break;
					}
					else if( text[ index + i ] != temp[iLen + i] )
					{
						k = 1;
						break;
					}
					else if( text[ index + i ] == temp[iLen + i] )
					{
						i++;				
					}
				}
				if( k == 0 )
				{
					return false;
				}
			}
		}
	}
	return true;
}

bool func_CheckString( vector<CoverStringstruct>& table, string& CString )
{
	int length = 0;
	char buf1[128];
	string stringtemp;
	for ( vector<CoverStringstruct>::iterator iter = table.begin(); iter != table.end(); iter++ )
	{
		strncpy( (char*)buf1, iter->cstring.c_str() , 128 );
		length = 0;
		for ( ; buf1[length]; length++ ) 
		{
			if ( buf1[length] >= 'A' && buf1[length] <= 'Z' )
			{
				buf1[length] = buf1[length] + 32;
			}
		}

		stringtemp = buf1;

		if( CString == stringtemp )
		{
			g_pGameMain->SendWarningMsg(g_ObjectData->GetString("CSTRING_ERROR_REPEAT"));	
			return false;
		}
		else if( ( CString.length() % iter->cstring.length() == 0 ) && iter->cstring.length() != CString.length() )
		{
			int count = (int)CString.length() / (int)stringtemp.length();
			string temp;
			for( int num = 0; num < count; ++num )
			{
				temp = temp + stringtemp ;
			}
			if( CString == temp )
			{
				g_pGameMain->SendWarningMsg(g_ObjectData->GetString("CSTRING_ERROR_REPEAT"));	
				return false;
			}			
		}
	}

	return true;
}

//****************************************************************************
//檢查字串是否重複
int CCoverStringFrame::Check_m_CStringitemField(string CString , int type)
{
	//BYTE buf1[128];
	char buf2[128];

	//string stringtemp;


	int length = 0;

	strncpy( (char*)buf2, CString.c_str() , 128 );
	for ( ; buf2[length]; length++ ) 
	{
		if ( buf2[length] >= 'A' && buf2[length] <= 'Z' )
		{
			buf2[length] = buf2[length] + 32;
		}
	}

	CString = buf2;

	if( func_CheckString(m_CStringitemField_Cover,CString) == false )
	{
		return false;
	}

	if( func_CheckString(m_CStringitemField_Show,CString) == false )
	{
		return false;
	}

	return true;
}
//****************************************************************************
//加入字串
void CCoverStringFrame::Add_m_CStringitemField(string CString , int type)
{
	CoverStringstruct Info;
	Info.cstring = CString;
	BYTE buf[128];
	int length = 0;
	if( type == 1 )
	{
		if ( g_pGameMain->IsPatternStringVisible(buf) )
		{
			m_CStringitemField_Cover.push_back(Info);		
			strncpy( (char*)buf, Info.cstring.c_str() , 128 );
			for ( ; buf[length]; length++ ) 
			{
				if ( buf[length] >= 'A' && buf[length] <= 'Z' )
				{
					buf[length] = buf[length] + 32;
				}
			}

			g_pGameMain->PushPatternString( buf );
		}
		else{
			g_pGameMain->SendWarningMsg(g_ObjectData->GetString("CSTRING_ERROR_REPEAT"));	
		}
	}

	if(type == 2)
	{
		m_CStringitemField_Show.push_back(Info);
		m_Overlap.cstring = CString;
		g_pGameMain->RemovePatternString((BYTE*) CString.c_str() , 2);
	}
	Save();
}
//****************************************************************************
//刪除字串
int CCoverStringFrame::Remove_m_CStringitemField(string CString , int type)
{
	int i = 0 , index = -1 ;

	if( type == 1 )
	{
		CoverStringstruct Info = m_CStringitemField_Cover.back();
		if( strcmp(Info.cstring.c_str() , CString.c_str()) == 0 )
		{
			m_CStringitemField_Cover.pop_back();
			g_pGameMain->RemovePatternString((BYTE*) CString.c_str() , 0);
			index = -1 ;
		}
		else
		{
			for ( vector<CoverStringstruct>::iterator iter = m_CStringitemField_Cover.begin(); iter != m_CStringitemField_Cover.end(); iter++ )
			{
				i += 1;
				if( strcmp(iter->cstring.c_str() , CString.c_str()) == 0 )
				{
					if( iter != m_CStringitemField_Cover.end() )
					{
						m_CStringitemField_Cover.erase(iter);
						g_pGameMain->RemovePatternString((BYTE*) CString.c_str() , 0);
						index = i;
					}
				}
			}
		}
	}
	if( type == 2 )
	{
		CoverStringstruct Info = m_CStringitemField_Show.back();
		if( strcmp(Info.cstring.c_str() , CString.c_str()) == 0 )
		{
			m_CStringitemField_Show.pop_back();
			index = -1 ;
		}
		else
		{
			for ( vector<CoverStringstruct>::iterator iter = m_CStringitemField_Show.begin(); iter != m_CStringitemField_Show.end(); iter++ )
			{
				i += 1;
				if( strcmp(iter->cstring.c_str() , CString.c_str()) == 0 )
				{
					if( iter != m_CStringitemField_Show.end() )
					{
						m_CStringitemField_Show.erase(iter);
						index = i;
					}
				}
			}
		}
		//若是內建字串 取消顯示之後 要重新丟回去 
		if( m_CStringitemField_Overlap.size() > 0)
		{
			Info = m_CStringitemField_Overlap.back();
			if( strcmp(Info.cstring.c_str() , CString.c_str()) == 0 )
			{
				CoverStringstructSave temp;
				strncpy( temp.buf, CString.c_str() , 128 );

				if ( g_pGameMain->IsPatternStringVisible((BYTE*) temp.buf)  )
				{
					g_pGameMain->PushPatternString((BYTE*) temp.buf);
				}

				m_CStringitemField_Overlap.pop_back();
			}		

			for ( vector<CoverStringstruct>::iterator iter = m_CStringitemField_Overlap.begin(); iter != m_CStringitemField_Overlap.end(); iter++ )
			{
				if( strcmp(iter->cstring.c_str() , CString.c_str()) == 0 )
				{
					CoverStringstructSave temp;
					strncpy( temp.buf, CString.c_str() , 128 );
					if ( g_pGameMain->IsPatternStringVisible((BYTE*) temp.buf)  )
					{
						g_pGameMain->PushPatternString((BYTE*) temp.buf);
					}
					m_CStringitemField_Overlap.erase(iter);
				}
			}
		}
	}
	Save();
	return index;
}
//****************************************************************************
int	LuaFunc_CString_AddString(lua_State* L)
{
	string CString = luaL_checkstring(L, 1); 
	int type = luaL_checkint(L, 2);

	if(g_pCoverStringFrame->Check_m_CStringitemField(CString , type))
	{
		g_pCoverStringFrame->Add_m_CStringitemField(CString , type);
		return true;
	}
	return false;
}
//****************************************************************************
int LuaFunc_CString_RemoveString(lua_State* L)
{
	string CString = luaL_checkstring(L, 1); 
	int type = luaL_checkint(L, 2);

	int index = g_pCoverStringFrame->Remove_m_CStringitemField(CString , type);		
	lua_pushnumber(L, index);
	return 1;
}
//****************************************************************************
int LuaFunc_CString_GetString(lua_State* L)
{
	int index = luaL_checkint(L, 1);
	int type = luaL_checkint(L, 2);
	string szString;
	if( index == -1 )
	{
		if( type == 1) 
		{
			lua_pushnumber(L, (lua_Number)g_pCoverStringFrame->m_CStringitemField_Cover.size());
		}
		else 
		{
			lua_pushnumber(L, (lua_Number)g_pCoverStringFrame->m_CStringitemField_Show.size());
		}
		return 1;
	}

	if( type == 1 )
	{
		if ( index >= 0 && index < g_pCoverStringFrame->m_CStringitemField_Cover.size() )
		{
			szString = g_pCoverStringFrame->m_CStringitemField_Cover[index].cstring;
			lua_pushstring( L, szString.c_str() );
			return 1;
		}
	}
	else
	{
		if ( index >= 0 && index < g_pCoverStringFrame->m_CStringitemField_Show.size() )
		{
			szString = g_pCoverStringFrame->m_CStringitemField_Show[index].cstring;
			lua_pushstring( L, szString.c_str() );
			return 1;
		}
	}

	return 0;
}

int LuaFunc_CString_GetIndex(lua_State* L)
{
	string CString = luaL_checkstring(L, 1); 
	int type = luaL_checkint(L, 2);

	if( type == 1 )
	{
		for ( int i = 0 ; i < g_pCoverStringFrame->m_CStringitemField_Cover.size() ; i++ )
		{
			if( strcmp(g_pCoverStringFrame->m_CStringitemField_Cover[i].cstring.c_str() , CString.c_str()) == 0 )
			{
				lua_pushnumber(L, i);
				return 1;
			}
		}
	}

	if( type == 2 )
	{
		for ( int i = 0 ; i < g_pCoverStringFrame->m_CStringitemField_Show.size() ; i++ )
		{
			if( strcmp(g_pCoverStringFrame->m_CStringitemField_Show[i].cstring.c_str() , CString.c_str()) == 0 )
			{
				lua_pushnumber(L, i);
				return 1;
			}
		}
	}
	lua_pushnumber(L, -1);
	return 1;
}