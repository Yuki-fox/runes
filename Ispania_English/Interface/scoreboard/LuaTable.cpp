extern "C"
{


#include <lua.h>
#include <Lauxlib.h>
}
#include <algorithm>
#include "Luatable.h"
//============================================================================//

inline bool lua_israwnumber(lua_State* L, int index)
{
	return (lua_type(L, index) == LUA_TNUMBER);
}


inline bool lua_israwstring(lua_State* L, int index)
{
	return (lua_type(L, index) == LUA_TSTRING);
}

inline int lua_toint(lua_State* L, int idx)
{
	return (int)lua_tointeger(L, idx);
}


inline bool lua_tobool(lua_State* L, int index)
{
	return lua_toboolean(L, index) != 0;
}


inline float lua_tofloat(lua_State* L, int idx)
{
	return (float)lua_tonumber(L, idx);
}

static const bool lowerCppKeys = false;




//============================================================================//
//============================================================================//
//
//  Utils from Spring
//

static inline void StringToLowerInPlace(std::string &s)
{
	std::transform(s.begin(), s.end(), s.begin(), (int (*)(int))tolower);
}


static inline std::string StringToLower(std::string s)
{
	StringToLowerInPlace(s);
	return s;
}



int CLuaTable::currentRef=LUA_NOREF;


//============================================================================//
//============================================================================//
//
//  CLuaTable
//
CLuaTable::CLuaTable()
: isValid(false)
, path("")

, L(NULL)
, refnum(LUA_NOREF)
{
}

CLuaTable::CLuaTable(lua_State* l,char * name):
path(name)
, L(l)
{


	if (!lua_istable(l,-1))
	{

		lua_createtable(l,0,1);
		

	}
	

	refnum  = luaL_ref(l, LUA_REGISTRYINDEX);
	isValid = (refnum != LUA_NOREF);
}





CLuaTable::CLuaTable(const CLuaTable& tbl)
{

	L      = tbl.L;


	if (tbl.PushTable()) 
	{
		lua_pushvalue(L, -1); // copy
		refnum = luaL_ref(L, LUA_REGISTRYINDEX);
	} 
	else 
	{
		refnum = LUA_NOREF;
	}
	isValid = (refnum != LUA_NOREF);


}


CLuaTable& CLuaTable::operator=(const CLuaTable& tbl)
{
	if ((refnum != LUA_NOREF) && (currentRef == refnum))
	{
		lua_settop(L, 0);
		currentRef = LUA_NOREF;
	}



	L    = tbl.L;
	path = tbl.path;

	if (tbl.PushTable()) 
	{
		lua_pushvalue(L, -1); // copy
		refnum = luaL_ref(L, LUA_REGISTRYINDEX);
	} 
	else 
	{
		refnum = LUA_NOREF;
	}

	isValid = (refnum != LUA_NOREF);

	return *this;
}


CLuaTable CLuaTable::SubTable(int key) const
{
	CLuaTable subTable;
	char buf[32];
	sprintf(buf, "[%i]", key);
	subTable.path = path + buf;

	if (!PushTable()) 
	{
		return subTable;
	}

	lua_pushinteger(L, key);
	lua_gettable(L, -2);
	if (!lua_istable(L, -1))
	{
		lua_pop(L,1);
	
		lua_pushinteger(L, key);
		lua_newtable(L);
		lua_rawset(L, -3);
		lua_pushinteger(L, key);
		lua_gettable(L, -2);
	}



	subTable.L       = L;
	subTable.refnum  = luaL_ref(L, LUA_REGISTRYINDEX);
	subTable.isValid = (subTable.refnum != LUA_NOREF);



	return subTable;
}


void CLuaTable::SetTable(int key,CLuaTable&table) 
{
	if (!PushTable()) 
	{
		return ;
	}
	lua_pushinteger(L, key);
	lua_rawgeti(L, LUA_REGISTRYINDEX, table.refnum);
	if (!lua_istable(L, -1))
	{
		lua_pop(L,2);
		return ;
	}

	lua_rawset(L, -3);

}
void CLuaTable::SetTable(const std::string& key,CLuaTable&table) 
{
	if (!PushTable()) 
	{
		return ;
	}
	lua_pushstring(L, key.c_str());
	lua_rawgeti(L, LUA_REGISTRYINDEX, table.refnum);
	if (!lua_istable(L, -1))
	{
		lua_pop(L,2);
		return ;
	}
	lua_rawset(L, -3);





}
CLuaTable CLuaTable::SubTable(const std::string& mixedKey) const
{

	const std::string key = !lowerCppKeys ? mixedKey : StringToLower(mixedKey);

	CLuaTable subTable;
	subTable.path = path + "." + key;

	if (!PushTable()) 
	{
		return subTable;
	}

	lua_pushstring(L, key.c_str());
	lua_gettable(L, -2);
	if (!lua_istable(L, -1))
	{
		lua_pop(L,2);
		
		lua_pushstring(L, key.c_str());	

		lua_newtable(L);
		lua_rawset(L, -3);
			lua_pushstring(L, key.c_str());	
		lua_gettable(L, -2);
	}


	subTable.L       = L;
	subTable.refnum  = luaL_ref(L, LUA_REGISTRYINDEX);
	subTable.isValid = (subTable.refnum != LUA_NOREF);



	return subTable;
}


CLuaTable CLuaTable::SubTableExpr(const std::string& expr) const
{
	if (expr.empty()) 
	{
		return CLuaTable(*this);
	}
	if (!isValid)
	{
		return CLuaTable();
	}

	std::string::size_type endPos;
	CLuaTable nextTable;

	if (expr[0] == '[') 
	{ // numeric key
		endPos = expr.find(']');
		if (endPos == std::string::npos)
		{
			return CLuaTable(); // missing brace
		}
		const char* startPtr = expr.c_str() + 1; // skip the '['
		char* endPtr;
		const int index = strtol(startPtr, &endPtr, 10);
		if (endPtr == startPtr)
		{
			return CLuaTable(); // invalid index
		}
		endPos++; // eat the ']'
		nextTable = SubTable(index);
	}
	else 
	{ // string key
		endPos = expr.find_first_of(".[");
		if (endPos == std::string::npos) 
		{
			return SubTable(expr);
		}
		nextTable = SubTable(expr.substr(0, endPos));
	}

	if (expr[endPos] == '.') 
	{
		endPos++; // eat the dot
	}
	return nextTable.SubTableExpr(expr.substr(endPos));
}


CLuaTable::~CLuaTable()
{
	if (L && (refnum != LUA_NOREF)) 
	{
		luaL_unref(L, LUA_REGISTRYINDEX, refnum);
		if ( currentRef == refnum) 
		{
			lua_settop(L, 0);
			currentRef = LUA_NOREF;
		}
	}

}


//============================================================================//

bool CLuaTable::PushTable() const
{
	if (!isValid) 
	{
		return false;
	}

	if ((refnum != LUA_NOREF) && (currentRef == refnum)) 
	{
		if (!lua_istable(L, -1))
		{
			printf("Internal Error: CLuaTable::PushTable() = %s\n", path.c_str());
			currentRef = LUA_NOREF;
			lua_settop(L, 0);
			return false;
		}
		return true;
	}

	lua_settop(L, 0);

	lua_rawgeti(L, LUA_REGISTRYINDEX, refnum);
	if (!lua_istable(L, -1)) 
	{
		isValid = false;
		currentRef = LUA_NOREF;
		lua_settop(L, 0);
		return false;
	}

	currentRef = refnum;

	return true;
}


bool CLuaTable::PushValue(int key) const
{
	if (!PushTable()) 
	{
		return false;
	}
	lua_pushinteger(L, key);
	lua_gettable(L, -2);
	if (lua_isnoneornil(L, -1)) 
	{
		lua_pop(L, 1);
		return false;
	}
	return true;
}


bool CLuaTable::PushValue(const std::string& mixedKey) const
{
	const std::string key = !lowerCppKeys ? mixedKey : StringToLower(mixedKey);
	if (!PushTable()) 
	{
		return false;
	}
	lua_pushstring(L, key.c_str());
	lua_gettable(L, -2);
	if (lua_isnoneornil(L, -1))
	{
		lua_pop(L, 1);
		return false;
	}
	return true;
}


//============================================================================//
//============================================================================//
//
//  Key existance testing
//

bool CLuaTable::KeyExists(int key) const
{
	if (!PushValue(key)) 
	{
		return false;
	}
	lua_pop(L, 1);
	return true;
}


bool CLuaTable::KeyExists(const std::string& key) const
{
	if (!PushValue(key))
	{
		return false;
	}
	lua_pop(L, 1);
	return true;
}


//============================================================================//
//============================================================================//
//
//  Value types
//

int CLuaTable::GetType(int key) const
{
	if (!PushValue(key))
	{
		return -1;
	}
	const int type = lua_type(L, -1);
	lua_pop(L, 1);
	return type;
}


int CLuaTable::GetType(const std::string& key) const
{
	if (!PushValue(key))
	{
		return -1;
	}
	const int type = lua_type(L, -1);
	lua_pop(L, 1);
	return type;
}


//============================================================================//
//============================================================================//
//
//  Object lengths
//

int CLuaTable::GetLength() const
{
	if (!PushTable()) 
	{
		return 0;
	}
	return lua_objlen(L, -1);
}


int CLuaTable::GetLength(int key) const
{
	if (!PushValue(key))
	{
		return 0;
	}
	const int len = lua_objlen(L, -1);
	lua_pop(L, 1);
	return len;
}


int CLuaTable::GetLength(const std::string& key) const
{
	if (!PushValue(key))
	{
		return 0;
	}
	const int len = lua_objlen(L, -1);
	lua_pop(L, 1);
	return len;
}


//============================================================================//
//============================================================================//
//
//  Key list functions
//

bool CLuaTable::GetKeys(std::vector<int>& data) const
{
	if (!PushTable()) 
	{
		return false;
	}
	const int table = lua_gettop(L);
	for (lua_pushnil(L); lua_next(L, table) != 0; lua_pop(L, 1))
	{
		if (lua_israwnumber(L, -2))
		{
			const int value = lua_toint(L, -2);
			data.push_back(value);
		}
	}
	std::sort(data.begin(), data.end());
	return true;
}


bool CLuaTable::GetKeys(std::vector<std::string>& data) const
{
	if (!PushTable()) 
	{
		return false;
	}
	const int table = lua_gettop(L);
	for (lua_pushnil(L); lua_next(L, table) != 0; lua_pop(L, 1)) 
	{
		if (lua_israwstring(L, -2)) {
			const std::string value = lua_tostring(L, -2);
			data.push_back(value);
		}
	}
	std::sort(data.begin(), data.end());
	return true;
}


//============================================================================//
//============================================================================//
//
//  Map functions
//

bool CLuaTable::GetMap(std::map<int, float>& data) const
{
	if (!PushTable()) 
	{
		return false;
	}
	const int table = lua_gettop(L);
	for (lua_pushnil(L); lua_next(L, table) != 0; lua_pop(L, 1)) 
	{
		if (lua_israwnumber(L, -2) && lua_israwnumber(L, -1))
		{
			const int   key   =   lua_toint(L, -2);
			const float value = lua_tofloat(L, -1);
			data[key] = value;
		}
	}
	return true;
}


bool CLuaTable::GetMap(std::map<int, std::string>& data) const
{
	if (!PushTable()) 
	{
		return false;
	}
	const int table = lua_gettop(L);
	for (lua_pushnil(L); lua_next(L, table) != 0; lua_pop(L, 1)) 
	{
		if (lua_israwnumber(L, -2) && lua_israwstring(L, -1)) 
		{
			const int    key   = lua_toint(L, -2);
			const std::string value = lua_tostring(L, -1);
			data[key] = value;
		}
	}
	return true;
}


bool CLuaTable::GetMap(std::map<std::string, float>& data) const
{
	if (!PushTable())
	{
		return false;
	}
	const int table = lua_gettop(L);
	for (lua_pushnil(L); lua_next(L, table) != 0; lua_pop(L, 1))
	{
		if (lua_israwstring(L, -2) && lua_israwnumber(L, -1)) 
		{
			const std::string key   = lua_tostring(L, -2);
			const float  value = lua_tofloat(L, -1);
			data[key] = value;
		}
	}
	return true;
}


bool CLuaTable::GetMap(std::map<std::string, std::string>& data) const
{
	if (!PushTable()) 
	{
		return false;
	}
	const int table = lua_gettop(L);
	for (lua_pushnil(L); lua_next(L, table) != 0; lua_pop(L, 1)) 
	{
		if (lua_israwstring(L, -2) && lua_israwstring(L, -1)) 
		{
			const std::string key   = lua_tostring(L, -2);
			const std::string value = lua_tostring(L, -1);
			data[key] = value;
		}
	}
	return true;
}
//============================================================================//
//============================================================================//
//
//  Parsing utilities
//

/*
static bool ParseTableFloat(lua_State* L,
int tableIndex, int index, float& value)
{
lua_pushnumber(L, index);
lua_gettable(L, tableIndex);
if (!lua_israwnumber(L, -1)) {
lua_pop(L, 1);
return false;
}
value = lua_tofloat(L, -1);
lua_pop(L, 1);
return true;
}
*/


static bool ParseBoolean(lua_State* L, int index, bool& value)
{
	const int type = lua_type(L, index);
	if (type == LUA_TBOOLEAN) 
	{
		value = lua_tobool(L, index);
		return true;
	}
	else if (type == LUA_TNUMBER) 
	{
		value = (lua_tofloat(L, index) != 0.0f);
		return true;
	}
	else if (type == LUA_TSTRING) 
	{
		const std::string str = StringToLower(lua_tostring(L, index));
		if ((str == "1") || (str == "true")) 
		{
			value = true;
			return true;
		}
		if ((str == "0") || (str == "false"))
		{
			value = false;
			return true;
		}
	}
	return false;
}


//============================================================================//
//============================================================================//
//
//  String key functions
//

int CLuaTable::GetInt(const std::string& key, int def) const
{
	if (!PushValue(key)) {
		return def;
	}
	if (!lua_israwnumber(L, -1)) 
	{
		lua_pop(L, 1);
		return def;
	}
	const int value = lua_toint(L, -1);
	lua_pop(L, 1);
	return value;
}


bool CLuaTable::GetBool(const std::string& key, bool def) const
{
	if (!PushValue(key))
	{
		return def;
	}
	bool value;
	if (!ParseBoolean(L, -1, value))
	{
		lua_pop(L, 1);
		return def;
	}
	lua_pop(L, 1);
	return value;
}


float CLuaTable::GetFloat(const std::string& key, float def) const
{
	if (!PushValue(key)) 
	{
		return def;
	}
	if (!lua_israwnumber(L, -1))
	{
		lua_pop(L, 1);
		return def;
	}
	const float value = lua_tofloat(L, -1);
	lua_pop(L, 1);
	return value;
}


std::string CLuaTable::GetString(const std::string& key, const std::string& def) const
{
	if (!PushValue(key)) 
	{
		return def;
	}
	if (!lua_israwstring(L, -1))
	{
		lua_pop(L, 1);
		return def;
	}
	const std::string value = lua_tostring(L, -1);
	lua_pop(L, 1);
	return value;
}
void* CLuaTable::GetUserData(int key, void* def) const
{
	if (!PushValue(key)) 
	{
		return def;
	}
	if (!lua_isuserdata(L, -1)) 
	{
		lua_pop(L, 1);
		return def;
	}
	void*  value= lua_touserdata(L, -1);
	lua_pop(L, 1);
	return value;
}
void* CLuaTable::GetUserData(const std::string& key, void* def) const
{
	if (!PushValue(key)) 
	{
		return def;
	}
	if (!lua_isuserdata(L, -1)) 
	{
		lua_pop(L, 1);
		return def;
	}
	void*  value= lua_touserdata(L, -1);
	lua_pop(L, 1);
	return value;

}
void CLuaTable::SetUserData(int key, void* value) 
{
	if (!PushTable()) 
	{
		return ;
	}
	lua_pushlightuserdata(L, value);
	lua_rawseti(L, -2,key);

}
void CLuaTable::SetUserData(const std::string& key, void* value) 
{
	if (!PushTable()) 
	{
		return ;
	}
	lua_pushlightuserdata(L,value);
	lua_setfield(L, -2,key.c_str());
}

void CLuaTable::SetInt(int key, int value) 
{
	if (!PushTable()) 
	{
		return ;
	}
	lua_pushnumber(L, value);
	lua_rawseti(L, -2,key);
}
void CLuaTable:: SetBool(int key, bool value) 
{
	if (!PushTable()) 
	{
		return ;
	}
	lua_pushboolean(L, value);
	lua_rawseti(L, -2,key);
}
void CLuaTable::SetFloat(int key, float value) 
{
	if (!PushTable()) 
	{
		return ;
	}
	lua_pushnumber(L, value);
	 lua_rawseti(L, -2,key);
}
void CLuaTable::SetString(int key, const std::string& value) 
{
	if (!PushTable()) 
	{
		return ;
	}
	lua_pushstring(L, value.c_str());
	 lua_rawseti(L, -2,key);


	
}

void CLuaTable::SetInt(const std::string& key, int value) 
{
	if (!PushTable()) 
	{
		return ;
	}
	lua_pushnumber(L, value);
	lua_setfield(L, -2,key.c_str());
}
void CLuaTable::SetBool(const std::string& key, bool value) 
{
	if (!PushTable()) 
	{
		return ;
	}
	lua_pushboolean(L, value);
	lua_setfield(L, -2,key.c_str());
}
void CLuaTable::SetFloat(const std::string& key, float value) 
{
	if (!PushTable()) 
	{
		return ;
	}
	lua_pushnumber(L, value);
	lua_setfield(L, -2,key.c_str());
}
void CLuaTable::SetString(const std::string& key, const std::string& value) 
{
	if (!PushTable()) 
	{
		return ;
	}
	lua_pushstring(L, value.c_str());
	lua_setfield(L, -2,key.c_str());
}

//============================================================================//
//============================================================================//
//
//  Number key functions
//

int CLuaTable::GetInt(int key, int def) const
{
	if (!PushValue(key))
	{
		return def;
	}
	if (!lua_israwnumber(L, -1)) 
	{
		lua_pop(L, 1);
		return def;
	}
	const int value = lua_toint(L, -1);
	lua_pop(L, 1);
	return value;
}


bool CLuaTable::GetBool(int key, bool def) const
{
	if (!PushValue(key))
	{
		return def;
	}
	bool value;
	if (!ParseBoolean(L, -1, value))
	{
		lua_pop(L, 1);
		return def;
	}
	lua_pop(L, 1);
	return value;
}


float CLuaTable::GetFloat(int key, float def) const
{
	if (!PushValue(key)) 
	{
		return def;
	}
	if (!lua_israwnumber(L, -1))
	{
		lua_pop(L, 1);
		return def;
	}
	const float value = lua_tofloat(L, -1);
	lua_pop(L, 1);
	return value;
}


std::string CLuaTable::GetString(int key, const std::string& def) const
{
	if (!PushValue(key)) 
	{
		return def;
	}
	if (!lua_israwstring(L, -1))
	{
		lua_pop(L, 1);
		return def;
	}
	const std::string value = lua_tostring(L, -1);
	lua_pop(L, 1);
	return value;
}