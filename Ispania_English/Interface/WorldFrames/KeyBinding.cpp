#include "../MainProc/GameMain.h"
#include "KeyBinding.h"

CKeyBinding*				g_pKeyBinding = NULL;

int LuaFunc_SetBindingKey(lua_State* L);
int LuaFunc_GetBindingKey(lua_State* L);
int LuaFunc_GetNumBindings(lua_State* L);
int LuaFunc_GetBindings(lua_State* L);
int LuaFunc_DefaultBindingKey(lua_State* L);//fuming 8-10-2007
int LuaFunc_SaveBindingKey(lua_State* L);//fuming 8-23-2007

// ----------------------------------------------------------------------------------
CKeyBinding::CKeyBinding(CInterface* object)
	: CInterfaceBase(object)
{
	m_bindingKey.clear();
	m_bindings.clear();
	m_order.clear();

	g_pKeyBinding = this;
}

// ----------------------------------------------------------------------------------
CKeyBinding::~CKeyBinding()
{
	for ( map<string, BindingItem*>::iterator iter = m_bindings.begin(); iter != m_bindings.end(); iter++ )
		UI_SAFE_DELETE(iter->second);
	m_order.clear();
	m_bindings.clear();
	m_bindingKey.clear();
	g_pKeyBinding = NULL;
}

// ----------------------------------------------------------------------------------
void CKeyBinding::RegisterFunc()
{
	lua_State* L = GetLuaState();
	if ( L )
	{
		lua_register(L, "SetBindingKey",					LuaFunc_SetBindingKey);
		lua_register(L, "GetBindingKey",					LuaFunc_GetBindingKey);
		lua_register(L, "GetNumBindings",					LuaFunc_GetNumBindings);
		lua_register(L, "GetBindings",						LuaFunc_GetBindings);
		lua_register(L, "DefaultBindingKey",				LuaFunc_DefaultBindingKey);//fuming 8-10-2007
		lua_register(L, "SaveBindingKey",					LuaFunc_SaveBindingKey);//fuming 8-23-2007
	}
}

// ----------------------------------------------------------------------------------
void CKeyBinding::SaveClientData()
{

}

// ----------------------------------------------------------------------------------
void CKeyBinding::LoadClientData(int type, int size, char* data)
{

}

// ----------------------------------------------------------------------------------
void CKeyBinding::Save()
{
	/*
	if ( m_interface == NULL || m_interface->IsLoadVariables() == false )
		return;
	*/

	char globalName[MAX_PATH];
	GetGlobalPath("bindings.txt", globalName, MAX_PATH);
	
	FILE* stream;
	if ( (stream = fopen(globalName, "wt+, ccs=UTF-8")) != NULL )
	{
		char lineStr[256];		
		
		const char* keyText;
		wchar_t* tempStr;
		size_t strSize;

		for ( vector<BindingItem*>::iterator iter = m_order.begin(); iter != m_order.end(); iter++ )
		{
			char keyStr[256];

			keyStr[0] = 0;
			keyText = GetBindingKeyText((*iter)->hotkey[0]);
			if (keyText)
				strcat(keyStr, keyText);

			strcat(keyStr," ");

			keyText = GetBindingKeyText((*iter)->hotkey[1]);
			if (keyText)
				strcat(keyStr,keyText);

			sprintf(lineStr, "%s %s\n", (*iter)->name.c_str(), keyStr);
			_strupr(lineStr);

			tempStr = TransToWChar(lineStr);
			strSize = wcslen(tempStr);
			fwrite(tempStr, sizeof(wchar_t), strSize, stream);
			delete [] tempStr;
		}
	}

	if ( stream )
	{
		fclose(stream);
	}
}

// ----------------------------------------------------------------------------------
void CKeyBinding::Load()
{
	char globalName[MAX_PATH];
	GetGlobalPath("bindings.txt", globalName, MAX_PATH);

	// 先設為預設值,避免載入失敗按鍵失效
	DefaultBindingKey();

	FILE* stream;
	if ( (stream = fopen(globalName, "rt+, ccs=UTF-8")) != NULL )
	{
		wchar_t tempStr[256];		
		char* lineStr;
		char *pCommand;
		char *pKey1;
		char *pKey2;
		int key1,key2;


		//分隔的符號...  COMMAND Key1 key2  .
		char delim[]=" \t\r\n";
		while ( !feof(stream) )
		{
			key1=0;
			key2=0;
			fgetws(tempStr, 256, stream);
			//轉換成ascii.
			lineStr=TransToChar(tempStr);			

			//解析 Command.
			pCommand=strtok(lineStr,delim);
			pKey1=strtok(NULL,delim);
			//解析 熱鍵1.
			if (pKey1)
				key1=ChangeBindingKey(pKey1);

			//解析 熱鍵2.
			pKey2=strtok(NULL,delim);
			if (pKey2)
				key2=ChangeBindingKey(pKey2);

			//有命令 建立BindingKey
			if (pCommand)
			{

				// 修正 TARGET PET
				if ( strcmp(lineStr, "TARGETPLAYERPET") == 0 && key1 == (VK_F1 | KEY_BINDING_FLAG_ALT) )
					key1 = (VK_F1 | KEY_BINDING_FLAG_SHIFT);

				if ( strcmp(lineStr, "TARGETPARTY1PET") == 0 && key1 == (VK_F2 | KEY_BINDING_FLAG_ALT) )
					key1 = (VK_F2 | KEY_BINDING_FLAG_SHIFT);

				if ( strcmp(lineStr, "TARGETPARTY2PET") == 0 && key1 == (VK_F3 | KEY_BINDING_FLAG_ALT) )
					key1 = (VK_F3 | KEY_BINDING_FLAG_SHIFT);

				if ( strcmp(lineStr, "TARGETPARTY3PET") == 0 && key1 == (VK_F4 | KEY_BINDING_FLAG_ALT) )
					key1 = (VK_F4 | KEY_BINDING_FLAG_SHIFT);

				if ( strcmp(lineStr, "TARGETPARTY4PET") == 0 && key1 == (VK_F5 | KEY_BINDING_FLAG_ALT) )
					key1 = (VK_F5 | KEY_BINDING_FLAG_SHIFT);

				if ( strcmp(lineStr, "TARGETPARTY5PET") == 0 && key1 == (VK_F6 | KEY_BINDING_FLAG_ALT) )
					key1 = (VK_F6 | KEY_BINDING_FLAG_SHIFT);

				SetBindingKey(pCommand,key1,key2);
			}
			
			delete [] lineStr;
		}

		fclose(stream);
	}
}

// ----------------------------------------------------------------------------------
void CKeyBinding::Clear()
{
	for ( map<string, BindingItem*>::iterator iter = m_bindings.begin(); iter != m_bindings.end(); iter++ )
		UI_SAFE_DELETE(iter->second);
	m_order.clear();
	m_bindings.clear();
	m_bindingKey.clear();
}

// ----------------------------------------------------------------------------------
void CKeyBinding::DefaultBindingKey()
{
	const char* country = g_pGameMain->GetCountry();

	SetBindingKey("MOVEFORWARD",		'W', VK_UP);	
	SetBindingKey("MOVEBACKWARD",		'S', VK_DOWN);
	SetBindingKey("STRAFERIGHT",		'D', VK_RIGHT);
	SetBindingKey("STRAFELEFT",			'A', VK_LEFT);
	SetBindingKey("TURNRIGHT",			'E');
	SetBindingKey("TURNLEFT",			'Q');
		
	SetBindingKey("JUMP",				VK_SPACE);
	SetBindingKey("TOGGLEAUTORUN",		VK_INSERT);			// Insert
	SetBindingKey("TOGGLERUN",			VK_MULTIPLY);		// '*'
	SetBindingKey("TOGGLESHEATH",		'Z');
	SetBindingKey("SITORSTAND",			'X');
	SetBindingKey("FOLLOWTARGET",		VK_OEM_5);			// "\"
	SetBindingKey("SCREENSHOT",			VK_SNAPSHOT);		// 拍照
	SetBindingKey("TOGGLEGAMEMENU",		VK_ESCAPE);	

	SetBindingKey("TOGGLECHARACTER0",		'C');
	SetBindingKey("TOGGLECHARACTER1",		'V');
	SetBindingKey("TOGGLECHARACTER2",		'Y');
	SetBindingKey("TOGGLECHARACTER3",		'T');
	SetBindingKey("TOGGLECHARACTER4",		'H');
	SetBindingKey("TOGGLEBACKPACK",			'B');
	SetBindingKey("TOGGLESPELLBOOK",		'K');
	SetBindingKey("TOGGLEQUESTBOOK",		'L');
	SetBindingKey("TOGGLEWORLDMAP",			'M');
	SetBindingKey("TOGGLEAREAMAP",			'M' | KEY_BINDING_FLAG_SHIFT );	//區域地圖

	SetBindingKey("TOGGLESOCIALFRAME",		'O');
	//丁丁說移除 SetBindingKey("TOGGLECRAFTFRAME",		'J');

	SetBindingKey("TOGGLEHATEFRIEND",		'j');			//仇人列表 j             
	SetBindingKey("TOGGLEBEAUTYSTUDIO",		VK_OEM_PERIOD);			//造型工坊 .
	SetBindingKey("TOGGLEBULLETINBOARD",	VK_OEM_COMMA);			//活動之家 ,
	SetBindingKey("TOGGLENPCTRACK",			VK_BACK);			//世界搜尋 Backspace


	//--排行榜--
	SetBindingKey("TOGGLESORTSCOREBOARD",	'i');			//排行榜.	
	//吸取符文.	
	SetBindingKey("TOGGLEDRAWOUTRUNE",	';');			//吸取符文.	
	//--fuming 8-14-2007
	SetBindingKey("TOGGLEGUILD",			'G');			//公會	.
	SetBindingKey("TOGGLEBLACKLISTFRAME",	'N');			//黑名單.
	SetBindingKey("TOGGLESEARCHGROUP",		'U');			//組隊搜尋.
	SetBindingKey("TOGGLEINFO",				'I');			//個人名片資訊.
	SetBindingKey("TOGGLECHARTBOOK",		'P');			//遊戲圖鑑.
	SetBindingKey("TOGGLEPARTERFRAME",		0);				//夥伴背包
	SetBindingKey("TOGGLEITEMMALL",			VK_OEM_2);		//商城 Item MALL.
	SetBindingKey("TOGGLETEACHING",			VK_F10);		//遊戲教學.	
	SetBindingKey("TOGGLEGAMEHELP",			VK_F11);		//遊戲圖示.	
	

	SetBindingKey("TOGGLETUTORIALEXPLAIN",	VK_F12);
	//SetBindingKey("TOGGLEGAMEMENU",			VK_OEM_3);

	SetBindingKey("TOGGLENPCNAME",			VK_F7);
	SetBindingKey("TOGGLEHEALTH",			VK_F8);
	SetBindingKey("TOGGLEPLATES",			VK_F9);

	// CHAT
	SetBindingKey("OPENCHAT",				VK_RETURN);
	SetBindingKey("REPLY",					'R');									// 回覆密語
	SetBindingKey("REPEAT",					'R'| KEY_BINDING_FLAG_SHIFT);			// 上一個密語對象
	SetBindingKey("CHATPAGEUP",				VK_PRIOR);			// PageUp
	SetBindingKey("CHATPAGEDOWN",			VK_NEXT);			// PageDown
	SetBindingKey("CHATTOP",				VK_HOME);			// Home
	SetBindingKey("CHATBOTTOM",				VK_END);			// End
	SetBindingKey("CHAT_PARTY",				'P'| KEY_BINDING_FLAG_CTRL);
	SetBindingKey("CHAT_GUILD",				'G'| KEY_BINDING_FLAG_CTRL);
	SetBindingKey("CHAT_SAY",				'S'| KEY_BINDING_FLAG_CTRL);
	SetBindingKey("CHAT_YELL",				'Y'| KEY_BINDING_FLAG_CTRL);
	SetBindingKey("CHAT_RAID",				'A'| KEY_BINDING_FLAG_CTRL);
	SetBindingKey("CHAT_ZONE",				'Z'| KEY_BINDING_FLAG_CTRL);

	SetBindingKey("TOGGLEUI",				KEY_BINDING_FLAG_CTRL | 'Z');			// CTRL + Z
	SetBindingKey("TOGGLEFPSTEXT",			KEY_BINDING_FLAG_CTRL | 'X');			// CTRL + X

	// Minimap
	SetBindingKey("MINIMAPZOOMIN",			VK_SUBTRACT);
	SetBindingKey("MINIMAPZOOMOUT",			VK_ADD);

	SetBindingKey("TARGETNEARESTENEMY",		VK_TAB);
	SetBindingKey("TARGETPREVIOUSENEMY",	VK_TAB | KEY_BINDING_FLAG_SHIFT);
	SetBindingKey("TARGETNEARESTFRIEND",	VK_TAB | KEY_BINDING_FLAG_CTRL);
	//SetBindingKey("TARGETPREVIOUSFRIEND",	VK_ESCAPE);
	SetBindingKey("TARGETSELF",				VK_F1);
	SetBindingKey("TARGETPARTY1",			VK_F2);
	SetBindingKey("TARGETPARTY2",			VK_F3);
	SetBindingKey("TARGETPARTY3",			VK_F4);
	SetBindingKey("TARGETPARTY4",			VK_F5);
	SetBindingKey("TARGETPARTY5",			VK_F6);

	SetBindingKey("TARGETPLAYERPET",		VK_F1 | KEY_BINDING_FLAG_SHIFT);
	SetBindingKey("TARGETPARTY1PET",		VK_F2 | KEY_BINDING_FLAG_SHIFT);
	SetBindingKey("TARGETPARTY2PET",		VK_F3 | KEY_BINDING_FLAG_SHIFT);
	SetBindingKey("TARGETPARTY3PET",		VK_F4 | KEY_BINDING_FLAG_SHIFT);
	SetBindingKey("TARGETPARTY4PET",		VK_F5 | KEY_BINDING_FLAG_SHIFT);
	SetBindingKey("TARGETPARTY5PET",		VK_F6 | KEY_BINDING_FLAG_SHIFT);
	SetBindingKey("ASSISTTARGET",			'F');

	SetBindingKey("RAIDTARGET1",			VK_NUMPAD1);
	SetBindingKey("RAIDTARGET2",			VK_NUMPAD2);
	SetBindingKey("RAIDTARGET3",			VK_NUMPAD3);
	SetBindingKey("RAIDTARGET4",			VK_NUMPAD4);
	SetBindingKey("RAIDTARGET5",			VK_NUMPAD5);
	SetBindingKey("RAIDTARGET6",			VK_NUMPAD6);
	SetBindingKey("RAIDTARGET7",			VK_NUMPAD7);
	SetBindingKey("RAIDTARGET8",			VK_NUMPAD8);

	SetBindingKey("ACTIONBAR1BUTTON1",	'1');
	SetBindingKey("ACTIONBAR1BUTTON2",	'2');
	SetBindingKey("ACTIONBAR1BUTTON3",	'3');
	SetBindingKey("ACTIONBAR1BUTTON4",	'4');
	SetBindingKey("ACTIONBAR1BUTTON5",	'5');
	SetBindingKey("ACTIONBAR1BUTTON6",	'6');
	SetBindingKey("ACTIONBAR1BUTTON7",	'7');
	SetBindingKey("ACTIONBAR1BUTTON8",	'8');
	SetBindingKey("ACTIONBAR1BUTTON9",	'9');
	SetBindingKey("ACTIONBAR1BUTTON10",	'0');
	SetBindingKey("ACTIONBAR1BUTTON11",	VK_OEM_MINUS);
	SetBindingKey("ACTIONBAR1BUTTON12",	VK_OEM_PLUS);

	SetBindingKey("PETACTIONBARBUTTON1",	'1'	| KEY_BINDING_FLAG_CTRL);
	SetBindingKey("PETACTIONBARBUTTON2",	'2'	| KEY_BINDING_FLAG_CTRL);
	SetBindingKey("PETACTIONBARBUTTON3",	'3'	| KEY_BINDING_FLAG_CTRL);
	SetBindingKey("PETACTIONBARBUTTON4",	'4'	| KEY_BINDING_FLAG_CTRL);
	SetBindingKey("PETACTIONBARBUTTON5",	'5'	| KEY_BINDING_FLAG_CTRL);
	SetBindingKey("PETACTIONBARBUTTON6",	'6'	| KEY_BINDING_FLAG_CTRL);
	SetBindingKey("PETACTIONBARBUTTON7",	'7'	| KEY_BINDING_FLAG_CTRL);
	SetBindingKey("PETACTIONBARBUTTON8",	'8'	| KEY_BINDING_FLAG_CTRL);
	SetBindingKey("PETACTIONBARBUTTON9",	'9'	| KEY_BINDING_FLAG_CTRL);
	SetBindingKey("PETACTIONBARBUTTON10",	'10' | KEY_BINDING_FLAG_CTRL);

	SetBindingKey("EXTRAACTIONBUTTON1" ,	'1'	 | KEY_BINDING_FLAG_ALT);
	SetBindingKey("EXTRAACTIONBUTTON2" ,	'2'	 | KEY_BINDING_FLAG_ALT);
	SetBindingKey("EXTRAACTIONBUTTON3" ,	'3'	 | KEY_BINDING_FLAG_ALT);
	SetBindingKey("EXTRAACTIONBUTTON4" ,	'4'	 | KEY_BINDING_FLAG_ALT);
	SetBindingKey("EXTRAACTIONBUTTON5" ,	'5'	 | KEY_BINDING_FLAG_ALT);
	SetBindingKey("EXTRAACTIONBUTTON6" ,	'6'	 | KEY_BINDING_FLAG_ALT);
	SetBindingKey("EXTRAACTIONBUTTON7" ,	'7'	 | KEY_BINDING_FLAG_ALT);
	SetBindingKey("EXTRAACTIONBUTTON8" ,	'8'	 | KEY_BINDING_FLAG_ALT);
	SetBindingKey("EXTRAACTIONBUTTON9" ,	'9'	 | KEY_BINDING_FLAG_ALT);
	SetBindingKey("EXTRAACTIONBUTTON10",	'10' | KEY_BINDING_FLAG_ALT);

	// 法國
	if ( strcmp(country, "FR") == 0 )
	{
		SetBindingKey("MOVEFORWARD",		'W', VK_UP);	
		SetBindingKey("MOVEBACKWARD",		'S', VK_DOWN);
		SetBindingKey("STRAFERIGHT",		'D', VK_RIGHT);
		SetBindingKey("STRAFELEFT",			'A', VK_LEFT);
		SetBindingKey("TURNRIGHT",			'E');
		SetBindingKey("TURNLEFT",			'Q');
	}
	// 俄羅斯
	else if ( strcmp(country, "RU") == 0 )
	{

	}
	// 西班牙
	else if ( strcmp(country, "ES") == 0 )
	{

	}
	// 德國
	else if ( strcmp(country, "ES") == 0 )
	{

	}
	// 日本
	else if ( g_pGameMain->CheckCountry("JP") == true ) //else if ( strcmp(country, "JP") == 0 )
	{

	}
	// 韓國
	else if ( strcmp(country, "KR") == 0 )
	{

	}
	// 土耳其
	else if ( strcmp(country, "TR") == 0 )
	{

	}
	// 羅馬尼亞
	else if ( strcmp(country, "RO") == 0 )
	{
	}
	else if ( strcmp(country, "AE") == 0 )
	{
		SetBindingKey("CHAT_ZONE",			VK_OEM_PERIOD| KEY_BINDING_FLAG_CTRL);		// CTRL + .
	}
	else
	{		
	}
}

// ----------------------------------------------------------------------------------
BindingItem* CKeyBinding::AddBinding(const char* name, const char* script, bool runOnUp, const char* header)
{
	// 名稱不正確或者已重複
	if ( name == NULL || strlen(name) <= 0 || FindBinding(name) )
		return NULL;

	BindingItem* nowBinding = new BindingItem();
	nowBinding->name = name;
	nowBinding->runOnUp = runOnUp;	
	if ( header )
		nowBinding->header = header;
	if ( script )
		nowBinding->script = script;
	m_order.push_back(nowBinding);
	m_bindings.insert(make_pair(name, nowBinding));	

	return nowBinding;
}

// ----------------------------------------------------------------------------------
BindingItem* CKeyBinding::FindBinding(const char* name)
{
	map<string, BindingItem*>::iterator iter = m_bindings.find(name);
	if ( iter != m_bindings.end() )
		return iter->second;
	return NULL;
}

// ----------------------------------------------------------------------------------
BindingItem* CKeyBinding::FindBinding(int key)
{
	map<int, BindingItem*>::iterator iter;
	int temp = key & 0xFF00FFFF;
	if ( key & KEY_BINDING_FLAG_SHIFT )
	{
		iter = m_bindingKey.find(temp | KEY_BINDING_FLAG_SHIFT);
		if ( iter != m_bindingKey.end() )
			return iter->second;
	}
	if ( key & KEY_BINDING_FLAG_CTRL )
	{		
		iter = m_bindingKey.find(temp | KEY_BINDING_FLAG_CTRL);
		if ( iter != m_bindingKey.end() )
			return iter->second;
	}
	if ( key & KEY_BINDING_FLAG_ALT )
	{		
		iter = m_bindingKey.find(temp | KEY_BINDING_FLAG_ALT);
		if ( iter != m_bindingKey.end() )
			return iter->second;
	}
	iter = m_bindingKey.find(temp);
	if ( iter != m_bindingKey.end() )
		return iter->second;

	return NULL;
}

// ----------------------------------------------------------------------------------
BindingItem* CKeyBinding::GetBindingByIndex(int index)
{
	if (index < 0 || index >= (int)m_order.size())
		return NULL;
	return m_order[index];
}

// ----------------------------------------------------------------------------------
void CKeyBinding::SetBindingKey(const char* name, int key1, int key2)
{	
	BindingItem* binding = FindBinding(name);
	if ( binding )
	{		
		// 清除舊有索引表格
		for ( int i = 0; i < 2; i++ )
		{
			map<int, BindingItem*>::iterator iter = m_bindingKey.find(binding->hotkey[i]);
			if ( iter != m_bindingKey.end() )
				m_bindingKey.erase(iter);
			binding->hotkey[i] = 0;
		}

		if ( key1 == key2 )
			key2 = 0;

		vector<int> hotkey;
		if ( key1 & 0xFF )
			hotkey.push_back(BindingKeyNormalize(key1));
		if ( key2 & 0xFF )
			hotkey.push_back(BindingKeyNormalize(key2));

		int count = 0;
		for ( vector<int>::iterator iter = hotkey.begin(); iter != hotkey.end(); iter++ )
		{
			map<int, BindingItem*>::iterator keyIter = m_bindingKey.find(*iter);
			if ( keyIter != m_bindingKey.end() )
			{
				// 相同按鍵需將舊有資料清除
				keyIter->second->ClearHotkey(*iter);
				keyIter->second = binding;
			}
			else
			{
				m_bindingKey.insert(make_pair(*iter, binding));
			}
			binding->hotkey[count++] = *iter;
		}

		if ( hotkey.size() > 0 ) {
			SendWorldEvent("UPDATE_BINDINGS");
		}
	}
}

// ----------------------------------------------------------------------------------
void CKeyBinding::RunBinding(int key, bool keyUp)
{
	// 在載具上特殊觸發按鍵
	CRoleSprite* player = g_pGameMain->GetPlayer();
	if ( player && player->GetImplementSprite() )
	{
		switch ( key )
		{
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
		case '0':
			if ( keyUp )
			{
				lua_State* L = GetLuaState();
				if ( L )
				{
					char temp[512];
					sprintf_s(temp, "UseImplementAction(%c)", key);
					luaL_dostring(L, temp);
				}
			}
			return;
		}
	}

	BindingItem* binding = FindBinding(key);
	if ( binding )
	{		
		if ( !keyUp || binding->runOnUp )
		{
			lua_State* L = GetLuaState();
			if ( L )
			{
				if ( keyUp )
					lua_pushstring(L, "up");
				else
					lua_pushstring(L, "down");
				lua_setglobal(L, "keystate");
			}
			RunBinding(binding);
		}
	}
}

// ----------------------------------------------------------------------------------
void CKeyBinding::RunBinding(BindingItem* binding)
{
	lua_State* L = GetLuaState();
	if ( L && binding && !binding->script.empty() )
	{
		luaL_dostring(L, binding->script.c_str());
	}
}

// ----------------------------------------------------------------------------------
const char*	CKeyBinding::GetBindingKeyText(int key)
{
	static char temp[256];

	const char* keyName = CUiFrame::GetKeyName(key & 0xFF);
	if ( keyName )
	{
		if ( key & KEY_BINDING_FLAG_SHIFT )
			sprintf(temp, "%s+%s", "shift", keyName);
		else if ( key & KEY_BINDING_FLAG_CTRL )
			sprintf(temp, "%s+%s", "ctrl", keyName);
		else if ( key & KEY_BINDING_FLAG_ALT )
			sprintf(temp, "%s+%s", "alt", keyName);
		else
			return keyName;

		return temp;
	}
	return NULL;
}

// ----------------------------------------------------------------------------------
int CKeyBinding::ChangeBindingKey(const char* keyText)
{	
	int key = 0;

	const char* begin = strchr(keyText, '+');
	if ( begin != NULL && begin != keyText )
	{
		key = (int)CUiFrame::GetKeyNum(begin + 1);
		if ( strnicmp(keyText, BINDINGS_KEY_TEXT_SHIFT, begin - keyText) == 0 )
			key |= KEY_BINDING_FLAG_SHIFT;
		else if ( strnicmp(keyText, BINDINGS_KEY_TEXT_CTRL, begin - keyText) == 0 )
			key |= KEY_BINDING_FLAG_CTRL;
		else if ( strnicmp(keyText, BINDINGS_KEY_TEXT_ALT, begin - keyText) == 0 )
			key |= KEY_BINDING_FLAG_ALT;
	}
	else
		key = (int)CUiFrame::GetKeyNum(keyText);

	return key;
}

// ----------------------------------------------------------------------------------
int CKeyBinding::BindingKeyNormalize(int key)
{
	int temp = key & 0xFF00FFFF;
	if ( key & KEY_BINDING_FLAG_SHIFT )
		temp |= KEY_BINDING_FLAG_SHIFT;
	else if ( key & KEY_BINDING_FLAG_CTRL )
		temp |= KEY_BINDING_FLAG_CTRL;
	else if ( key & KEY_BINDING_FLAG_ALT )
		temp |= KEY_BINDING_FLAG_ALT;
	return temp;
}

// ----------------------------------------------------------------------------------
void CKeyBinding::ReadLine(const char* lineStr)
{
}

// ----------------------------------------------------------------------------------
int LuaFunc_SetBindingKey(lua_State* L)
{
	int key[2] = { 0, 0 };
	const char* binding = luaL_checkstring(L, 1);
	for ( int i = 0; i < 2; i++ )
	{		
		if ( lua_isstring(L, i+2) )
			key[i] = g_pKeyBinding->ChangeBindingKey(lua_tostring(L, i+2));
	}

	g_pKeyBinding->SetBindingKey(binding, key[0], key[1]);
	return 0;
}

// ----------------------------------------------------------------------------------
int LuaFunc_GetBindingKey(lua_State* L)
{
	BindingItem* item = g_pKeyBinding->FindBinding(luaL_checkstring(L, 1));
	if ( item == NULL )
		return 0;

	lua_pushstring(L, g_pKeyBinding->GetBindingKeyText(item->hotkey[0]));
	lua_pushstring(L, g_pKeyBinding->GetBindingKeyText(item->hotkey[1]));
	return 2;
}

// ----------------------------------------------------------------------------------
int LuaFunc_GetNumBindings(lua_State* L)
{
	lua_pushnumber(L, g_pKeyBinding->GetNumBindings());
	return 1;
}

// ----------------------------------------------------------------------------------
int LuaFunc_GetBindings(lua_State* L)
{
	if ( lua_isnumber(L, 1) )
	{
		BindingItem* item = g_pKeyBinding->GetBindingByIndex((int)lua_tonumber(L, 1) - 1);
		if ( item )
		{
			lua_pushstring(L, item->header.c_str());
			lua_pushstring(L, item->name.c_str());
			lua_pushstring(L, g_pKeyBinding->GetBindingKeyText(item->hotkey[0]));
			lua_pushstring(L, g_pKeyBinding->GetBindingKeyText(item->hotkey[1]));
			return 4;
		}
	}
	return 0;
}
//----------------------------------------------------------------------------------
// fuming 8-10-2007.
// 恢復預設值  .
int LuaFunc_DefaultBindingKey(lua_State* L)
{

	g_pKeyBinding->DefaultBindingKey();
	return 0;
}
//----------------------------------------------------------------------------------
// fuming 8-23-2007.
// 熱鍵存檔  .
int LuaFunc_SaveBindingKey(lua_State* L)
{

	g_pKeyBinding->Save();
	return 0;
}