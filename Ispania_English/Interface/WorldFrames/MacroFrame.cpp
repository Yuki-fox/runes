#include <shlwapi.h>
#include "../../mainproc/GameMain.h"
#include "../WorldFrames/ItemClipboard.h"
#include "../WorldFrames/ChatEdit.h"
#include "MacroFrame.h"

#define MACRO_ICON_PATH					""
#define MACRO_VERSION					0x01

CMacroFrame*				g_pMacroFrame = NULL;

int	LuaFunc_GetMacroInfo(lua_State* L);
int LuaFunc_HasMacro(lua_State* L);
int LuaFunc_GetNumMacroIcons(lua_State* L);
int	LuaFunc_GetMacroIconInfo(lua_State* L);
int	LuaFunc_EditMacro(lua_State* L);
int	LuaFunc_PickupMacroItem(lua_State* L);

// ----------------------------------------------------------------------------
CMacroFrame::CMacroFrame(CInterface* object)
	: CInterfaceBase(object)
{
	g_pMacroFrame = this;

	for ( int i = 0; i < NUM_MAX_MACROS; i++ )
	{
		m_macro[i].id	= i;
		m_macro[i].icon = -1;
		memset(m_macro[i].name, 0, sizeof(m_macro[i].name));
		memset(m_macro[i].text, 0, sizeof(m_macro[i].text));
	}	
}

// ----------------------------------------------------------------------------
CMacroFrame::~CMacroFrame()
{
	g_pMacroFrame = NULL;
}

// ----------------------------------------------------------------------------
void CMacroFrame::SearchIcons()
{
#define MACRO_NUM_CLASS			8
#define MACRO_NUM_ICONS			100

	const char* shortClass[MACRO_NUM_CLASS] = {
		"war",			// 戰士
		"run",			// 盜賊
		"ran",			// 遊俠
		"aug",			// 祭司
		"mag",			// 法師
		"dru",			// 德魯依
		"kni",			// 騎士
		"thi",			// 言靈使
	};

	if ( g_ruResourceManager == NULL )
		return;

	char tmp[256];
	for ( int c = 0; c < MACRO_NUM_CLASS; c++ )
	{
		for ( int r = 1; r < MACRO_NUM_ICONS; r++ )
		{
			for ( int i = 1; i < MACRO_NUM_ICONS; i++ )
			{
				sprintf(tmp, "Interface\\Icons\\skill_%s%d-%d.dds", shortClass[c], r, i);
				if ( g_ruResourceManager->FileExists(tmp) ) {
					m_icons.push_back(tmp);
				}					
			}
		}
	}
}

// ----------------------------------------------------------------------------
void CMacroFrame::Update(float elapsedTime)
{
	lua_State* L = GetLuaStateWorld();

	m_isUpdate = true;	

	g_pGameMain->SkipCastSpellCheck(true);

	map<int, ExecuteMacroLine>::iterator iter = m_executeLines.begin();
	while ( iter != m_executeLines.end() )
	{		
		if ( iter->second.sleep - elapsedTime > 0.0f )
		{
			iter->second.sleep -= elapsedTime;
			iter++;
		}
		else
		{
			list<string>::iterator first = iter->second.lines.begin();
			while ( first != iter->second.lines.end() )
			{
				string lineStr = *first;
				first = iter->second.lines.erase(first);

				if ( strnicmp(lineStr.c_str(), "/wait", 5) == 0 )
				{
					if ( iter->second.sleep < 0.0f )
						iter->second.sleep = 0.0f;

					float time = min(max((float)atof(lineStr.c_str() + 5), 0.0f), 60.0f);
					iter->second.sleep += time;
					break;
				}
				else if ( L )
				{
					lua_getglobal(L, "ExecuteMacroLine");
					lua_pushstring(L, lineStr.c_str());
					lua_call(L, 1, 0);
				}				
			}

			if ( iter->second.lines.empty() )
				iter = m_executeLines.erase(iter);
			else
				iter++;
		}
	}

	g_pGameMain->SkipCastSpellCheck(false);

	m_isUpdate = false;
}

// ----------------------------------------------------------------------------
// 要求 Server 儲存角色資料
void CMacroFrame::SaveClientData()
{
	// 產生暫存資料
	CTempFile tempFile(65536);

	int version, size;

	// 版本檢查編號
	version = MACRO_VERSION;

	// 結構大小
	size = sizeof(m_macro);

	tempFile.Write(&version, sizeof(int));
	tempFile.Write(&size, sizeof(int));
	tempFile.Write(m_macro, size);

	g_pGameMain->SaveClientData(eSaveClientData_Macro, tempFile.GetPosition(), tempFile.GetData());

	// 將最後存檔時間去除
	SetSaveUpdateTime(0.0f);
}

// ----------------------------------------------------------------------------
// 讀取角色資料
void CMacroFrame::LoadClientData(int type, int size, char* data)
{
	if ( type == eSaveClientData_Macro )
	{
		CTempFile tempFile(data, size);

		int version, size;

		tempFile.Read(&version, sizeof(int));
		tempFile.Read(&size, sizeof(int));

		if ( version != MACRO_VERSION || size != sizeof(m_macro) )
			return;

		tempFile.Read(m_macro, size);
	}
}

// ----------------------------------------------------------------------------
void CMacroFrame::RegisterFunc()
{
	lua_State* L = GetLuaStateWorld();
	if (L)
	{
		lua_register( L, "GetMacroInfo",			LuaFunc_GetMacroInfo );
		lua_register( L, "HasMacro",				LuaFunc_HasMacro );
		lua_register( L, "GetNumMacroIcons",		LuaFunc_GetNumMacroIcons );
		lua_register( L, "GetMacroIconInfo",		LuaFunc_GetMacroIconInfo );
		lua_register( L, "EditMacro",				LuaFunc_EditMacro );
		lua_register( L, "PickupMacroItem",			LuaFunc_PickupMacroItem );
	}
}

// ----------------------------------------------------------------------------
void CMacroFrame::DestroyFrames()
{
	m_executeLines.clear();
	Save();

	for ( int i = 0; i < NUM_MAX_MACROS; i++ )
	{
		m_macro[i].id	= i;
		m_macro[i].icon = -1;
		memset(m_macro[i].name, 0, sizeof(m_macro[i].name));
		memset(m_macro[i].text, 0, sizeof(m_macro[i].text));
	}
}

// ----------------------------------------------------------------------------
int	CMacroFrame::GetMacroIndexById(int id)
{
	for ( int i = 0; i < NUM_MAX_MACROS; i++ )
	{
		if ( m_macro[i].id == id )
			return i;
	}
	return -1;
}

// ----------------------------------------------------------------------------
bool CMacroFrame::CreateMacro(const char* name, int iconId, const char* text)
{
	for ( int i = 0; i < NUM_MAX_MACROS; i++ )
	{
		if ( m_macro[i].icon == -1 )
		{
			EditMacro(i, name, iconId, text);
			return true;
		}
	}
	return false;
}

// ----------------------------------------------------------------------------
void CMacroFrame::EditMacro(int index, const char* name, int iconId, const char* text)
{
	MacroItem* item = GetMacroItem(index);
	if ( item )
	{
		if ( name == NULL )
			memset( item->name, 0, sizeof(item->name) );
		else
		{
			strncpy(item->name, name, MACRO_MAX_NAME);
			item->name[MACRO_MAX_NAME - 1] = 0;
		}		

		if ( text == NULL )
			memset( item->text, 0, sizeof(item->text) );
		else
		{
			strncpy(item->text, text, MACRO_MAX_CONTENT);
			item->name[MACRO_MAX_CONTENT - 1] = 0;
		}

		RemoveExecuteLine(index);

		item->icon = iconId;
		SendWorldEvent("MACROFRAME_UPDATE");
		Save();

		SetSaveUpdateTime();
	}
}

// ----------------------------------------------------------------------------
void CMacroFrame::RemoveExecuteLine(int id)
{
	map<int, ExecuteMacroLine>::iterator iter = m_executeLines.find(id);
	if ( iter != m_executeLines.end() )
		m_executeLines.erase(iter);
}

// ----------------------------------------------------------------------------
void CMacroFrame::ClearMacro(int index)
{
	EditMacro(index, NULL, -1, NULL);
}

// ----------------------------------------------------------------------------
MacroItem* CMacroFrame::GetMacroItem(int index)
{
	if ( index >= 0 && index < NUM_MAX_MACROS )
		return &m_macro[index];
	return NULL;
}

// ----------------------------------------------------------------------------
const char* CMacroFrame::GetMacroIcon(int index)
{
	if ( m_icons.empty() ) {
		SearchIcons();
	}

	if ( index >= 0 && index < (int)m_icons.size() )
		return m_icons[index].c_str();
	return "Interface\\Icons\\Icon-Default";
}

// ----------------------------------------------------------------------------
int CMacroFrame::GetNumMacroIcons()
{
	if ( m_icons.empty() ) {
		SearchIcons();
	}
	return (int)m_icons.size();
}

// ----------------------------------------------------------------------------
void CMacroFrame::PickupMacroItem(int index)
{
	MacroItem* item = GetMacroItem( index );
	if ( item )
	{
		ClipboardItemInfo itemInfo = g_pItemClipboard->GetItemInfo();

		int destIndex;
		switch ( g_pItemClipboard->GetType() )
		{
		case eClipboardItem_Type_Macro:			
			destIndex = GetMacroIndexById(itemInfo.macro.id);
			if ( destIndex >= 0 && destIndex < NUM_MAX_MACROS )
			{
				MacroItem tmp = m_macro[index];
				m_macro[index] = m_macro[destIndex];
				m_macro[destIndex] = tmp;
			}
			g_pItemClipboard->Clear();			// 清除資料
			SendWorldEvent("MACROFRAME_UPDATE");
			break;

		default:
			g_pItemClipboard->Clear();			// 清除資料
			g_pItemClipboard->SetName(item->name);
			g_pItemClipboard->SetFile(GetMacroIcon(item->icon));
			g_pItemClipboard->PickupMacro(item->id);
			break;
		}		
	}
}

// ----------------------------------------------------------------------------
void CMacroFrame::UseMacro(int id)
{	
	if ( m_isUpdate || g_pGameMain->IsUIUpdate() )
		return;

	MacroItem* item = GetMacroItem( GetMacroIndexById(id) );
	if ( item && item->text[0] )
	{
		list<string> lines;
		char temp[1024];
		int count = 0;
		int index = 0;

		while ( item->text[index] )
		{
			if ( item->text[index] == '\n' )
			{
				temp[count] = 0;
				if ( count > 0 ) {
					lines.push_back(temp);
					count = 0;
				}
			}
			else
			{
				temp[count++] = item->text[index];
			}
			index++;
		}
		if ( count > 0 )
		{
			temp[count] = 0;
			lines.push_back(temp);
		}
		if ( lines.empty() )
			lines.push_back(item->text);

		map<int, ExecuteMacroLine>::iterator iter = m_executeLines.find(id);
		if ( iter != m_executeLines.end() )
		{
			iter->second.sleep = 0.0f;
			iter->second.lines = lines;
		}
		else
		{
			ExecuteMacroLine data;
			data.sleep = 0.0f;
			data.lines = lines;
			m_executeLines.insert(make_pair(id, data));
		}

		/*
		lua_State* L = GetLuaStateWorld();
		if (L)
		{
			for ( vector<string>::iterator iter = lines.begin(); iter != lines.end(); iter++ )
			{
				lua_getglobal(L, "ExecuteMacroLine");
				lua_pushstring(L, (*iter).c_str());
				lua_call(L, 1, 0);
			}
		}
		*/
	}
}

// ----------------------------------------------------------------------------
void CMacroFrame::Save()
{
	if ( m_interface == NULL || m_interface->IsLoadVariables() == false )
		return;

	FILE* fptr = NULL;
	wstring globalName = GetGlobalPathW(L"Macro.bsd");
	wstring localName = GetLocalPathW(L"Macro.bsd");	

	// 儲存共用資料	

	// 儲存本地資料
	if ( (fptr = _wfopen(localName.c_str(), L"wb")) != NULL )
	{
		// 檔案大小
		int count = NUM_MAX_MACROS;
		int size = sizeof(MacroItem);

		// 檔頭資料
		fwrite(&size, sizeof(int), 1, fptr);
		fwrite(&count, sizeof(int), 1, fptr);

		// 記錄資料
		fwrite( m_macro, sizeof(m_macro), 1, fptr);

		// 關閉檔案
		fclose(fptr);
	}
}

// ----------------------------------------------------------------------------
void CMacroFrame::Load()
{
	FILE* fptr = NULL;
	wstring globalName = GetGlobalPathW(L"Macro.bsd");
	wstring localName = GetLocalPathW(L"Macro.bsd");	

	// 讀取共用資料
	if ( (fptr = _wfopen(localName.c_str(), L"rb")) != NULL )
	{
		// 讀取資料
		int size, count;
		fread( &size, sizeof(size), 1, fptr );
		fread(&count, sizeof(int), 1, fptr);

		if ( size == sizeof(MacroItem2) )
		{
			MacroItem2 temp;
			for ( int i = 0; i < min(count, NUM_MAX_MACROS); i++ )
			{
				fread(&temp, size, 1, fptr);
				m_macro[i].id = temp.id;
				m_macro[i].icon = temp.icon;
				memcpy(m_macro[i].name, temp.name, NUM_NAME_MACRO);
				memcpy(m_macro[i].text, temp.text, NUM_CONTENT_MACRO);
			}
		}
		else if ( size == sizeof(MacroItem) )
		{
			for ( int i = 0; i < min(count, NUM_MAX_MACROS); i++ )
				fread(&m_macro[i], size, 1, fptr);
		}

		// 關閉檔案
		fclose(fptr);
	}

	// 如果未在伺服器上存檔
	SetSaveUpdateTime();
}

// ----------------------------------------------------------------------------
int LuaFunc_HasMacro(lua_State* L)
{
	MacroItem* item = g_pMacroFrame->GetMacroItem(luaL_checkint(L, 1) - 1);
	if ( item )
	{
		lua_pushboolean(L, !item->IsEmpty());
		return 1;
	}
	return 0;
}

// ----------------------------------------------------------------------------
int	LuaFunc_GetMacroInfo(lua_State* L)
{
	MacroItem* item = g_pMacroFrame->GetMacroItem(luaL_checkint(L, 1) - 1);
	if ( item && !item->IsEmpty() )
	{
		lua_pushnumber(L, item->icon);
		lua_pushstring(L, item->name);
		lua_pushstring(L, item->text);
		return 3;
	}
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GetNumMacroIcons(lua_State* L)
{
	lua_pushnumber(L, g_pMacroFrame->GetNumMacroIcons());
	return 1;
}

// ----------------------------------------------------------------------------
int	LuaFunc_GetMacroIconInfo(lua_State* L)
{
	const char* icon = g_pMacroFrame->GetMacroIcon(luaL_checkint(L, 1));
	lua_pushstring(L, icon);
	return 1;
}

// ----------------------------------------------------------------------------
int	LuaFunc_EditMacro(lua_State* L)
{
	int index = luaL_checkint(L, 1) - 1;
	int icon = -1;
	const char* name = NULL;
	const char* text = NULL;

	for ( int i = 2; i <= lua_gettop(L); i++ )
	{
		switch (i)
		{
		case 2:		// name			
			if ( lua_type(L, 2) == LUA_TSTRING )
				name = lua_tostring(L, 2);
			break;
			
		case 3:		// icon
			if ( lua_type(L, 3) == LUA_TNUMBER )
				icon = (int)lua_tonumber(L, 3);
			break;

		case 4:		// text
			if ( lua_type(L, 4) == LUA_TSTRING )
				text = lua_tostring(L, 4);
			break;
		}
	}
	g_pMacroFrame->EditMacro(index, name, icon, text);
	return 0;
}

// ----------------------------------------------------------------------------
int	LuaFunc_PickupMacroItem(lua_State* L)
{
	g_pMacroFrame->PickupMacroItem(luaL_checkint(L, 1) - 1);
	return 0;
}