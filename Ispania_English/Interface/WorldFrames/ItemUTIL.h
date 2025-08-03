#pragma once
struct ItemFieldStruct;
struct GameObjDbStruct;
struct GameObjDbStructEx;
#include <string>
using namespace std;
class ItemUTIL
{
public:
	static const char *checkUtf8SafeString(const char* src,size_t& destLen,wchar_t* dest);
	static bool FindMagicTagStr( string& Str , char* findKey , GameObjDbStructEx** RetMagic , char* RetKey );
	static const string GetMagicColNote( GameObjDbStruct* MagicCol , int MagicLv );
	static const string GetMagicNoteEx( string Note , int MagicLv );
	static void ReplaceString( string& sourcestr , const char* oldstr, const char* newstr );
	static char* GetItemIcon(ItemFieldStruct * item);
	static char* GetItemIcon(GameObjDbStruct* itemDB, bool repalceEmptyIcon = false);
	static char* GetItemACTWorld(ItemFieldStruct * item);
	static char* GetItemACTWorld(GameObjDbStruct* itemDB);
	static const string GetItemNote( GameObjDbStruct* ItemDB );
	static int GetPet3DImageID(GameObjDbStruct* itemDB);
};
