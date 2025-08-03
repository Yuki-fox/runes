#pragma once

#include "RoleData/RoleBaseDef.h"


bool ParseHyperlink( const char* link , string& outtype , string& outdata , string& outname );


void RegisterItemFieldChecksum( ItemFieldStruct* item ); //Checksum
bool CheckItemFieldChecksum( ItemFieldStruct* item );

bool ItemField_To_ItemLink( ItemFieldStruct* item, string& link, bool checksum = false );
bool ItemGUID_To_ItemLink( int GUID, string& link );
bool ItemLink_To_ItemField( const char* link, ItemFieldStruct*& poutItemField, GameObjDbStructEx*& poutItemDB ); 
bool PlayerName_To_HyperLink( const char* name, string& link );
bool SkillGUID_To_HyperLink( int GUID, int skillPoint, string& link );
bool QuestGUID_To_HyperLink( int GUID, string& link );
DWORD ItemLink_To_Hash(const char* link);




















