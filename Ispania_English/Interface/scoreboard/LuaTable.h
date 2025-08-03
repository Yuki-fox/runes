#pragma  once 
#include <string>
#include <map>
#include <vector>
class CLuaTable 
{

	static int currentRef;



public:
	CLuaTable(lua_State* L,char *  name="root");
	CLuaTable();

	CLuaTable(const CLuaTable& tbl);
	CLuaTable& operator=(const CLuaTable& tbl);
	~CLuaTable();

	CLuaTable SubTable(int key) const;
	CLuaTable SubTable(const std::string& key) const;
	CLuaTable SubTableExpr(const std::string& expr) const;

	void SetTable(int key,CLuaTable&table) ;
	void SetTable(const std::string& key,CLuaTable&table) ;

	bool IsValid() const { return (L != NULL); }

	const std::string& GetPath() const { return path; }

	int GetLength() const;                  // lua '#' operator
	int GetLength(int key) const;           // lua '#' operator
	int GetLength(const std::string& key) const; // lua '#' operator

	bool GetKeys(std::vector<int>& data) const;
	bool GetKeys(std::vector<std::string>& data) const;

	bool GetMap(std::map<int, float>& data) const;
	bool GetMap(std::map<int, std::string>& data) const;
	bool GetMap(std::map<std::string, float>& data) const;
	bool GetMap(std::map<std::string, std::string>& data) const;

	bool KeyExists(int key) const;
	bool KeyExists(const  std::string& key) const;

	int GetType(int key) const;
	int GetType(const std::string& key) const;

	// numeric keys
	int    GetInt(int key, int def) const;
	bool   GetBool(int key, bool def) const;
	float  GetFloat(int key, float def) const;
	std::string GetString(int key, const std::string& def) const;
	void* GetUserData(int key, void* def) const;

	void SetInt(int key, int value) ;
	void SetBool(int key, bool value) ;
	void SetFloat(int key, float value) ;
	void SetString(int key, const std::string& value) ;
	void SetUserData(int key, void* value) ;

	// string keys  (always lowercase)
	int    GetInt(const std::string& key, int def) const;
	bool   GetBool(const std::string& key, bool def) const;
	float  GetFloat(const std::string& key, float def) const;
	std::string GetString(const std::string& key, const std::string& def) const;
	void* GetUserData(const std::string& key, void* def) const;
	void SetInt(const std::string& key, int value) ;
	void SetBool(const std::string& key, bool value) ;
	void SetFloat(const std::string& key, float value) ;
	void SetString(const std::string& key, const std::string& value) ;

	void SetUserData(const std::string& key, void* value) ;

private:


	bool PushTable() const;
	bool PushValue(int key) const;
	bool PushValue(const std::string& key) const;

private:
	mutable bool isValid;
	std::string path;

	lua_State* L;
	int refnum;
};
