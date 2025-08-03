

#include "ItemLink.h"



//#include "../Common/Encrypt.h"
#include "../../mainproc/GameMain.h"
#include "../Common/Com_Function.h"
#include "../mainproc/NetGlobal.h"


#include "Rune Engine/Tools/FilePackage/cFilePackage.h"

// #include "Rune Engine/Rune/Rune.h"						// 3D引擎
#include "..\WorldFrames\ItemUtil.h"


bool ParseHyperlink( const char* link , string& outtype , string& outdata , string& outname )
{
	int len = (int)strlen( link );

	if( len > 1023 )
	{
		return false;
	}

	char buf[1024];

	strcpy( buf, link );

	char* type ;
	char* data ;
	char* name ;
	char* end  ;

	type = strstr( buf , "|H" );
	if( !type )	return false;

	type = type + 2;

	data = strstr( type , ":" );
	if( !data )	return false;

	data[0] = '\0';
	data = data + 1;

	name = strstr( data , "|h" );
	if( !name )	return false;

	name[0] = '\0';
	name = name + 2;

	//name = strstr( name , "[" );
  	//if( !name )	return false;
	//name = name + 1;

	end = strstr( name , "|h" );
	if( !end )	return false;
	end[0] = '\0';

	//去除名稱[]
   	char* temp = strstr( name , "[" );
	if( temp )
	{
		temp = temp + 1;
		end = strstr( temp , "]" );
		if( end )
		{
			name = temp;
			end[0] = '\0';
		}
	}	
   
	outtype = type; 
	outdata = data; 
	outname = name; 

	return true;
}




std::map<INT64,int> g_mapItemFieldChecksum;


void RegisterItemFieldChecksum( ItemFieldStruct* item )
{
	if( NULL == item || item->IsEmpty() )
	{
		return;
	}

	DWORD dwChecksum = 0;

	int i;
	int len = sizeof( ItemFieldStruct ) / 4;

	for( i = 0 ; i < len ; ++i )
	{
		if( i % 2 == 0 )
			dwChecksum = dwChecksum ^ ((DWORD*)item)[i];
		else
 			dwChecksum = dwChecksum + ((DWORD*)item)[i];
	}	

	if( item->Serial == 0 && item->CreateTime == 0 )
	{

		if( dwChecksum == 0 )
		{
			return;
		}

		item->Serial = dwChecksum;
		RegisterItemFieldChecksum( item );
		return;
	}

	INT64 key = ( ((INT64)item->Serial) << 32 ) + item->CreateTime; 
  	g_mapItemFieldChecksum[ key ] = dwChecksum;
}

bool CheckItemFieldChecksum( ItemFieldStruct* item )
{
	if( NULL == item || item->IsEmpty() )
	{
		return false;
	}
	INT64 key = ( ((INT64)item->Serial) << 32 ) + item->CreateTime; 
	std::map<INT64,int>::iterator itPos = g_mapItemFieldChecksum.find( key );

	if( itPos == g_mapItemFieldChecksum.end() )
	{
		return false;
	}

	DWORD dwChecksum = 0;

	int i;
	int len = sizeof( ItemFieldStruct ) / 4;
 	for( i = 0 ; i < len ; ++i )
	{
		if( i % 2 == 0 )
			dwChecksum = dwChecksum ^ ((DWORD*)item)[i];
		else
			dwChecksum = dwChecksum + ((DWORD*)item)[i];
	}

	if( dwChecksum == itPos->second )
	{
		return true;
	}

	return false;
}

struct SItemLink_WeaponArmor
{
	SItemLink_WeaponArmor()
	{
		//		_temp[0] = 0;
		//		_temp[1] = 0;
		//		_temp[2] = 0;
	}	

	//unsigned char	OrgQuality;			//原始的品質
	unsigned char	Quality;			//品質
	unsigned char	PowerQuality	:5;	//威力品質
	unsigned char	Rare			:3;	//稀有度
	unsigned char	Level			:5;	//等級
	unsigned char	RuneVolume		:3;	//符文容量

	unsigned char	_temp[1];			//給記憶體補齊4byte
	
	//	unsigned char	_temp[3];		//給記憶體補齊4byte

	unsigned short	Inherent[6];	//天生附加能力

	//int     Inherent;		//天生附加能力
	int     Rune[4];		//符石
	int 	Durable;		//目前耐久度
};

struct SItemLink_PosRecord
{
	int	ZoneID;						//區域ID
	//int X, Y, Z ;
	int X, Z ;
	int	MapID;						//地圖ID
};

struct SItemLink_Lottery
{
	int	Index;						//期數	
	int Count;
	char SelectID[20];
};

//主要是為了顯示魔石
struct SItemLink_Normal
{
	int             CreateTime;		//CreateTime;
	//unsigned short	Inherent[6];	//天生附加能力
	int IntAbility[8]; 
};

// 坐騎顏色表
struct SItemLink_MountColor
{
	int Color[4];					// 4個階層顏色
};

// 打包商品資訊
//struct SItemLink_Packet
//{
//	int IntAbility[8]; 
//};


// ----------------------------------------------------------------------------------
bool ItemField_To_ItemLink( ItemFieldStruct* item, string& link, bool checksum )
{
	//int index = luaL_checkint(L, 2) - 1;
	//ItemFieldStruct* item;

	link.clear();
	if ( !item || item->IsEmpty() )
	{
		return false;
	}

	if( checksum )
	{
		if( false == CheckItemFieldChecksum( item ) )
		{
			return false;
		}
	}

	//融合物品 1-21-09 
	//int guidDB=item->OrgObjID ;
	//if (item->ImageObjectID>0)
	//	guidDB=	item->ImageObjectID;

	GameObjDbStructEx* itemDB = g_ObjectData->GetObj( item->OrgObjID);

	if( !itemDB )
	{
		return false;
	}

	DWORD data[12];
	ZeroMemory( data, sizeof(data) );

	memcpy( &data[0] , &item->OrgObjID , sizeof( DWORD ) );
	memcpy( &data[1] , &item->Mode     , sizeof( DWORD ) );

	switch ( itemDB->Classification )
	{
	case EM_ObjectClass_Treasure:
		break;
	case EM_ObjectClass_Item:

		switch (itemDB->Item.Class)
		{
		case EM_Item_Normal:
			{
				if( itemDB->Item.ItemType == EM_ItemType_PosRecord )
				{
					SItemLink_PosRecord s;

					s.ZoneID = item->ZoneID;						//區域ID
					s.X      = item->X     ;
					//s.Y        = item->Y     ;
					s.Z      = item->Z     ;
					s.MapID  = item->MapID ;						//地圖ID

					memcpy( &data[2] , &s , sizeof( s ) );
				}
				else if( itemDB->Item.ItemType == EM_ItemType_Lottery )
				{
					SItemLink_Lottery s;

					s.Index = item->Lottery.Index;
					s.Count = item->Lottery.Count;
					memcpy( &s.SelectID , &item->Lottery.SelectID , sizeof( s.SelectID ) );

					memcpy( &data[2] , &s , sizeof( s ) );
				}
				else if ( itemDB->Item.ItemType == EM_ItemType_SummonHorse )
				{
					SItemLink_MountColor s;
					memcpy(s.Color, item->HorseColor, sizeof(s.Color));

					memcpy( &data[2] , &s , sizeof( s ) );
				}
				//else if( itemDB->Item.ItemType == EM_ItemType_Packet && itemDB->Item.UseType == EM_ItemUseType_UnPacket )
				//{
				//	SItemLink_Packet s;
				//	memcpy(s.IntAbility, item->IntAbility, sizeof(s.IntAbility));
				//	memcpy( &data[2] , &s , sizeof( s ) );
				//}
				else
				{
					SItemLink_Normal s;

					s.CreateTime  = item->CreateTime;

					//s.Inherent[0] = item->Inherent(0);
					//s.Inherent[1] = item->Inherent(1);
					//s.Inherent[2] = item->Inherent(2);
					//s.Inherent[3] = item->Inherent(3);
					//s.Inherent[4] = item->Inherent(4);
					//s.Inherent[5] = item->Inherent(5);

					memcpy(s.IntAbility, item->IntAbility, sizeof(s.IntAbility));
					memcpy( &data[2] , &s , sizeof( s ) );
				}

				break;
			}
		case EM_Item_Weapon :
		case EM_Item_Armor  :
			{
				SItemLink_WeaponArmor s;
				s.RuneVolume   = item->RuneVolume   ;
				s.Rare		   = item->Rare;
				//s.OrgQuality   = item->OrgQuality   ;
				s.Quality      = item->Quality      ;
				s.PowerQuality = item->PowerQuality ;
				s.Level        = item->Level        ;
				//			s.Inherent     = item->Inherent     ; --Hsiang 2008/7/9
				s.Inherent[0]  = item->Inherent(0)	;
				s.Inherent[1]  = item->Inherent(1)	;
				s.Inherent[2]  = item->Inherent(2)	;
				s.Inherent[3]  = item->Inherent(3)	;
				s.Inherent[4]  = item->Inherent(4)	;
				s.Inherent[5]  = item->Inherent(5)	;
				s.Rune[0]      = item->Rune(0)      ;
				s.Rune[1]      = item->Rune(1)      ;
				s.Rune[2]      = item->Rune(2)      ;
				s.Rune[3]      = item->Rune(3)      ;
				s.Durable      = item->Durable      ;

				memcpy( &data[2] , &s , sizeof( s ) );

				break;
			}
		case EM_Item_Rune :
		case EM_Item_Card :
			{
				break;
			}

		default:
			//return false;
			break;
		}
		break;
	case EM_ObjectClass_MagicCollect:
		break;
	case EM_ObjectClass_MagicBase:
		break;
	case EM_ObjectClass_Recipe:
		break;
	}

	//Checksum
	DWORD dwChecksum = 0;
	dwChecksum = data[0] + data[1] + data[2] + data[3] + data[4] + data[5] + data[6] + data[7] + data[8] + data[9] + data[10] ;
	int i;
	for( i = 0 ; i < 11 ; ++i )
	{
		dwChecksum += data[i] * i;
		dwChecksum += data[i] >> 16 * i;
		dwChecksum = dwChecksum ^ data[i];
	}
	for( i = 0 ; i < 11 ; ++i )
	{
		dwChecksum += dwChecksum * data[i];
		dwChecksum += dwChecksum * data[i] >> 16;
		dwChecksum = dwChecksum ^ data[i];
	}
	dwChecksum = ( dwChecksum & 0xFFFF );

	data[11] = dwChecksum;
	//	CEncrypt::Encrypt( data , data , sizeof( DWORD ) * 9 , data[9] );	


	//|cff9d9d9d|Hitem:7073:0:0:0:0:0:0:0|h[Broken Fang]|h|r
	//	Broken up in its components: 
	//"|cff9d9d9d" – Colorizes the link with a medium grey color (Broken Fang is vendortrash) 
	//	The first two characters after pipe-c may be the alpha level, where ff is fully opaque. 
	//	The next three sets of two characters represent the red, green, and blue levels, just like HTML. 
	//	"|H" – "Hyperlink link data starts here" 
	//	"item:7073:0:0:0:0:0:0:0" – Read more at itemString. 
	//	"|h" – "End of link, text follows" 
	//	"[Broken Fang]" – The actual text displayed 
	//	"|h" – "End of hyperlink" 
	//	"|r" – Restores color to normal


	//|cff1eff00 |Hitem:1eff00 |h[]|h|r 26
	//"|Hitem:15444:0:0:0:0:0:0:639052726|h|cff1eff00[奧格瑪法杖]|r|h"		 "|c%x|H%s|h%s|h|r"

	char* itemString = Com_FormatString( "%x %x %x %x %x %x %x %x %x %x %x %x",
		data[0], data[1], data[2], data[3], data[4], data[5],
		data[6], data[7], data[8], data[9], data[10], data[11] );


	// 取得物品名稱
	char itemName[128];
	if ( !RoleDataEx::GetItemFieldName( item, itemName, 128 ) )
	{
		strcpy(itemName, itemDB->GetName());
	}

	// 稀有度
	int color = g_ObjectData->GetItemQualityColor( g_ObjectData->GetItemQuality( itemDB, item ) );

	link = "|Hitem:";
	link += itemString;
	link += Com_FormatString( "|h|c%08x[%s]|r|h", color, itemName ); 

	return true;
}

// ----------------------------------------------------------------------------------
bool ItemGUID_To_ItemLink( int GUID, string& link )
{
	GameObjDbStructEx* itemDB = g_ObjectData->GetObj( GUID );

	if( !itemDB )
	{
		return false;
	}

	// 稀有度
	int color = g_ObjectData->GetItemQualityColor( g_ObjectData->GetItemQuality( itemDB, NULL ) );

	// 取得物品名稱
	char itemName[128];
	if ( !RoleDataEx::GetItemFieldName( NULL, itemName, 128 ) )
	{
		strcpy(itemName, itemDB->GetName());
	}

	link = Com_FormatString( "|Hitem:%x|h|c%08x[%s]|r|h", GUID, color, itemName ); 

	return true;
}

//bool ItemLink_IsEmpty(ItemFieldStruct* item)
//{	
//	int size = sizeof(ItemFieldStruct);
//	int offset = sizeof(int) * 2;
//	return memcmp( item + offset, 0, size - offset ) == 0;
//}

// ----------------------------------------------------------------------------------
static ItemFieldStruct s_ItemField;
bool ItemLink_To_ItemField( const char* link, ItemFieldStruct*& poutItemField, GameObjDbStructEx*& poutItemDB )
{
	//if( NULL == poutItemField || NULL == poutItemDB )
	//{
	//	return false;
	//}

	DWORD data[12];
	ZeroMemory( data, sizeof(data) );

	int n = sscanf( link, "|Hitem:%x %x %x %x %x %x %x %x %x %x %x %x", 
		&data[0], &data[1], &data[2], &data[3], &data[4], &data[5],
		&data[6], &data[7], &data[8], &data[9], &data[10], &data[11] );

	if( n == 1 )
	{
		int OrgObjID;
		memcpy( &OrgObjID , &data[0] , sizeof( DWORD ) );

		poutItemField = NULL;
		poutItemDB = g_ObjectData->GetObj( OrgObjID );

		if( poutItemDB )
			return true;
		else
			return false;
	}

	if( n != 12 )
	{
		return false;
	}


	char moo[200];
	//	CEncrypt::Encrypt( data , data , sizeof( DWORD ) * 9 , data[9] );
	DWORD dwChecksum = 0;
	dwChecksum = data[0] + data[1] + data[2] + data[3] + data[4] + data[5] + data[6] + data[7] + data[8] + data[9] + data[10] ;
	int i;
	for( i = 0 ; i < 11 ; ++i )
	{
		dwChecksum += data[i] * i;
		dwChecksum += data[i] >> 16 * i;
		dwChecksum = dwChecksum ^ data[i];
	}
	for( i = 0 ; i < 11 ; ++i )
	{
		dwChecksum += dwChecksum * data[i];
		dwChecksum += dwChecksum * data[i] >> 16;
		dwChecksum = dwChecksum ^ data[i];
	}
	dwChecksum = ( dwChecksum & 0xFFFF );
	if( dwChecksum != data[11] )
	{
		return false;
	}

	ItemFieldStruct* item = &s_ItemField;
	item->Init();

	memcpy( &item->OrgObjID , &data[0] , sizeof( DWORD ) );
	memcpy( &item->Mode     , &data[1] , sizeof( DWORD ) );

	GameObjDbStructEx* itemDB = g_ObjectData->GetObj( item->OrgObjID );

	if( !itemDB )
	{
		return false;
	}

	switch ( itemDB->Classification )
	{
	case EM_ObjectClass_Treasure:
		break;
	case EM_ObjectClass_Item:

		switch (itemDB->Item.Class)
		{
		case EM_Item_Normal:
			{
				if( itemDB->Item.ItemType == EM_ItemType_PosRecord )
				{
					SItemLink_PosRecord s;
					memcpy( &s , &data[2] , sizeof( SItemLink_PosRecord ) );

					item->ZoneID = s.ZoneID;						//區域ID
					item->X      = s.X     ;
					item->Y      = 0       ; //s.Y     ;
					item->Z      = s.Z     ;
					item->MapID  = s.MapID ;						//地圖ID

				}
				else if( itemDB->Item.ItemType == EM_ItemType_Lottery )
				{
					SItemLink_Lottery s;
					memcpy( &s , &data[2] , sizeof( SItemLink_Lottery ) );

					item->Lottery.Index = s.Index;
					item->Lottery.Count = s.Count;
					memcpy( &item->Lottery.SelectID , &s.SelectID , sizeof( s.SelectID ) );
				}
				else if ( itemDB->Item.ItemType == EM_ItemType_SummonHorse )
				{
					SItemLink_MountColor s;
					memcpy( &s , &data[2] , sizeof( SItemLink_Lottery ) );

					memcpy(item->HorseColor, s.Color, sizeof(item->HorseColor));
				}
				//else if( itemDB->Item.ItemType == EM_ItemType_Packet && itemDB->Item.UseType == EM_ItemUseType_UnPacket )
				//{
				//	SItemLink_Packet s;
				//	memcpy( &s , &data[2] , sizeof( SItemLink_Packet ) );
				//	memcpy(item->IntAbility, s.IntAbility, sizeof(item->IntAbility));
				//}
				else
				{
					SItemLink_Normal s;
					memcpy( &s , &data[2] , sizeof( SItemLink_Normal ) );

					item->CreateTime  = s.CreateTime;

					//item->Inherent( 0 , s.Inherent[0] ) ;
					//item->Inherent( 1 , s.Inherent[1] ) ;
					//item->Inherent( 2 , s.Inherent[2] ) ;
					//item->Inherent( 3 , s.Inherent[3] ) ;
					//item->Inherent( 4 , s.Inherent[4] ) ;
					//item->Inherent( 5 , s.Inherent[5] ) ;

					memcpy(item->IntAbility, s.IntAbility, sizeof(item->IntAbility));
				
				}

				break;
			}
		case EM_Item_Weapon :
		case EM_Item_Armor  :
			{

				SItemLink_WeaponArmor s;
				memcpy( &s , &data[2] , sizeof( s ) );

				item->RuneVolume   = s.RuneVolume   ;
				//item->OrgQuality   = s.OrgQuality   ;
				item->Quality      = s.Quality      ;
				item->PowerQuality = s.PowerQuality ;
				item->Level        = s.Level        ;
				item->Rare		   = s.Rare			;
				item->Inherent(0 , s.Inherent[0] ) ;
				item->Inherent(1 , s.Inherent[1] ) ;
				item->Inherent(2 , s.Inherent[2] ) ;
				item->Inherent(3 , s.Inherent[3] ) ;
				item->Inherent(4 , s.Inherent[4] ) ;
				item->Inherent(5 , s.Inherent[5] ) ;
				item->Rune(0 ,s.Rune[0]  )     ;
				item->Rune(1 ,s.Rune[1]  )     ;
				item->Rune(2 ,s.Rune[2]  )     ;
				item->Rune(3 ,s.Rune[3]  )     ;
				item->Durable      = s.Durable      ;

				break;
			}
		case EM_Item_Rune :
		case EM_Item_Card :
			{
				break;
			}
		default:
			//return false;
			break;
		}
	case EM_ObjectClass_MagicCollect:
		break;
	case EM_ObjectClass_MagicBase:
		break;
	case EM_ObjectClass_Recipe:
		break;
	}

	poutItemField = &s_ItemField;
	poutItemDB = itemDB;

	poutItemField->Count = 1; //0會被init

	return true;
}

// ----------------------------------------------------------------------------------
bool PlayerName_To_HyperLink( const char* name, string& link )
{
	char tempName[MAX_PATH];
	if ( name == NULL || name[0] == 0 )
		return false;

	// 系統名稱
	if ( name[0] == 0x01 ) {
		sprintf_s(tempName, MAX_PATH, "[%s]", name + 1);
	} else {
		sprintf_s(tempName, MAX_PATH, "|Hplayer:%s|h[%s]|h", name, name);
	}	
	link = tempName;
	return true;
}

// ----------------------------------------------------------------------------------
bool SkillGUID_To_HyperLink( int GUID, int skillPoint, string& link )
{
	GameObjDbStructEx* pBaseSkillObj = g_ObjectData->GetObj( GUID );

	if( !pBaseSkillObj )
	{
		return false;
	}

	int color = 0xFF8080FF;

	if( skillPoint > 0 )
	{
		link = Com_FormatString( "|Hskill:%d %d|h|c%08x[%s+%d]|r|h", GUID, skillPoint, color, pBaseSkillObj->GetName(), skillPoint ); 
	}
	else
	{
		link = Com_FormatString( "|Hskill:%d %d|h|c%08x[%s]|r|h", GUID, skillPoint, color, pBaseSkillObj->GetName() ); 
	}
	
	return true;
}

// ----------------------------------------------------------------------------------
bool QuestGUID_To_HyperLink( int GUID, string& link )
{
	GameObjDbStructEx* pObjDb = g_ObjectData->GetObj( GUID );

	if( !pObjDb )
	{
		return false;
	}

	int color = 0xFFA0A020;

	link = Com_FormatString( "|Hquest:%x|h|c%08x[%s]|r|h", GUID, color, pObjDb->GetName() ); 

	return true;
}



// ----------------------------------------------------------------------------------
#include "../WorldFrames/UnitFrame.h"
// ----------------------------------------------------------------------------------

int Model_SetItemLink(lua_State* L)
{
	CUiModel* model = dynamic_cast<CUiModel*>(CUiLuaLayout::CheckLayoutTable(L, 1));
	if (model == NULL)
		lua_error(L);

	const char *pItemLink = luaL_checkstring(L, 2);


	ItemFieldStruct* pItemField = NULL;
	GameObjDbStructEx* pItemDB;
	if( !ItemLink_To_ItemField( pItemLink, pItemField, pItemDB ) )
	{
		return 0;
	}
	EModelSetItemResult eResult = CUnitFrame::Model_SetItem( model, pItemDB, pItemField );


	switch( eResult )
	{
	case MSIR_Success            :
		lua_pushstring( L, "Success" );
		return 1;
		break;
	case MSIR_Success_Not_Player :
		lua_pushstring( L, "Success_Not_Player" );
		return 1;
		break;
 	case MSIR_NoPaperdoll        :
		lua_pushstring( L, "NoPaperdoll" );
		return 1;
		break;
	case MSIR_SexError_LimitBoy  :
		lua_pushstring( L, "SexError_LimitBoy" );
		return 1;
		break;
	case MSIR_SexError_LimitGirl :
		lua_pushstring( L, "SexError_LimitGirl" );
		return 1;
		break;
	//case MSIR_Fail               :
	default:
		break;
	}

	lua_pushstring( L, "Fail" );
	return 1;

}




// ----------------------------------------------------------------------------------
int Model_TakeOffAll(lua_State* L)
{
	CUiModel* model = dynamic_cast<CUiModel*>(CUiLuaLayout::CheckLayoutTable(L, 1));
	if (model == NULL)
		lua_error(L);

	//model->ReleaseAttachEntity();

 //	model->SetPaperdollPart( PAPERDOLL_PART_NAME_HEAD     , "type01" );
	model->SetPaperdollPart( PAPERDOLL_PART_NAME_HELMET   , NULL );
	model->SetPaperdollPart( PAPERDOLL_PART_NAME_HAND     , "body000-001" );
	model->SetPaperdollPart( PAPERDOLL_PART_NAME_FOOT     , "body000-001" );
	model->SetPaperdollPart( PAPERDOLL_PART_NAME_TORSO    , "body000-001" );
	model->SetPaperdollPart( PAPERDOLL_PART_NAME_LEG      , "body000-001" );
	model->SetPaperdollPart( PAPERDOLL_PART_NAME_BACK     , NULL );
	model->SetPaperdollPart( PAPERDOLL_PART_NAME_BELT     , NULL );
	model->SetPaperdollPart( PAPERDOLL_PART_NAME_SHOULDER , NULL );

	int color[2];
	color[0] = color[1] = 0x00000000;

	model->SetPaperdollColor( PAPERDOLL_PART_NAME_HELMET   , color[0], color[1] );
	model->SetPaperdollColor( PAPERDOLL_PART_NAME_HAND     , color[0], color[1] );
	model->SetPaperdollColor( PAPERDOLL_PART_NAME_SHOULDER , color[0], color[1] );
	model->SetPaperdollColor( PAPERDOLL_PART_NAME_TORSO    , color[0], color[1] );
	model->SetPaperdollColor( PAPERDOLL_PART_NAME_LEG      , color[0], color[1] );
	model->SetPaperdollColor( PAPERDOLL_PART_NAME_BELT     , color[0], color[1] );
	model->SetPaperdollColor( PAPERDOLL_PART_NAME_FOOT     , color[0], color[1] );
	model->SetPaperdollColor( PAPERDOLL_PART_NAME_BACK     , color[0], color[1] );

	return 0;
}

// ----------------------------------------------------------------------------------
int Model_TakeOffWeapon(lua_State* L)
{
	CUiModel* model = dynamic_cast<CUiModel*>(CUiLuaLayout::CheckLayoutTable(L, 1));
	if (model == NULL)
		lua_error(L);

	model->ReleaseAttachEntity();

	return 0;
}

// ----------------------------------------------------------------------------
int Model_SetModel(lua_State* L)
{

	CUiModel* model = dynamic_cast<CUiModel*>(CUiLuaLayout::CheckLayoutTable(L, 1));
	if (model == NULL)
		lua_error(L);

	int imageID = (int)luaL_checknumber(L, 2);
	GameObjDbStruct* imagedb = CNetGlobal::GetObj(imageID);	
	if (imagedb)
	{
		GameObjDbStruct* imageDB = CNetGlobal::GetObj(imagedb->ImageID);

		if (strlen(imageDB->Image.ACTWorld)==0)//沒有對應的模型檔
		{
			//可能是寵物蛋系列
			imageID=ItemUTIL::GetPet3DImageID(imagedb);

			imageDB= CNetGlobal::GetObj(imageID);	


		}


		if ( imageDB )
		{
			model->CreateEntity(imageDB->Image.ACTWorld);
		}
	
	//	int c1=RGB(rand(),rand(),rand());
	//	int c2=RGB(rand(),rand(),rand());
	//	model->SetColor(CUiModel::C_MAIN , c1,c2);
	}
	else
	{
		model->ReleaseEntity();
	}
 
	return 0;
}




// ----------------------------------------------------------------------------------
int Model_SetDummyMale(lua_State* L)
{
	CUiModel* model = dynamic_cast<CUiModel*>(CUiLuaLayout::CheckLayoutTable(L, 1));
	if (model == NULL)
		lua_error(L);

	model->CreateEntity( "model\\character\\pc\\dummy\\male\\act_dummy_male.ros" );

	model->SetPaperdollPart( PAPERDOLL_PART_NAME_HEAD     , "type01" );
	model->SetPaperdollPart( PAPERDOLL_PART_NAME_HELMET   , NULL );
	model->SetPaperdollPart( PAPERDOLL_PART_NAME_HAND     , "body000-001" );
	model->SetPaperdollPart( PAPERDOLL_PART_NAME_FOOT     , "body000-001" );
	model->SetPaperdollPart( PAPERDOLL_PART_NAME_TORSO    , "body000-001" );
	model->SetPaperdollPart( PAPERDOLL_PART_NAME_LEG      , "body000-001" );
	model->SetPaperdollPart( PAPERDOLL_PART_NAME_BACK     , NULL );
	model->SetPaperdollPart( PAPERDOLL_PART_NAME_BELT     , NULL );
	model->SetPaperdollPart( PAPERDOLL_PART_NAME_SHOULDER , NULL );

	return 0;
}

// ----------------------------------------------------------------------------------
int Model_SetDummyFemale(lua_State* L)
{
	CUiModel* model = dynamic_cast<CUiModel*>(CUiLuaLayout::CheckLayoutTable(L, 1));
	if (model == NULL)
		lua_error(L);

	model->CreateEntity( "model\\character\\pc\\dummy\\female\\act_dummy_female.ros" );

	model->SetPaperdollPart( PAPERDOLL_PART_NAME_HEAD     , "type01" );
	model->SetPaperdollPart( PAPERDOLL_PART_NAME_HELMET   , NULL );
	model->SetPaperdollPart( PAPERDOLL_PART_NAME_HAND     , "body000-001" );
	model->SetPaperdollPart( PAPERDOLL_PART_NAME_FOOT     , "body000-001" );
	model->SetPaperdollPart( PAPERDOLL_PART_NAME_TORSO    , "body000-001" );
	model->SetPaperdollPart( PAPERDOLL_PART_NAME_LEG      , "body000-001" );
	model->SetPaperdollPart( PAPERDOLL_PART_NAME_BACK     , NULL );
	model->SetPaperdollPart( PAPERDOLL_PART_NAME_BELT     , NULL );
	model->SetPaperdollPart( PAPERDOLL_PART_NAME_SHOULDER , NULL );

	return 0;
}

DWORD ItemLink_To_Hash(const char* link)
{
	DWORD data[12];
	ZeroMemory(data, sizeof(data));

	int n = sscanf(link, "|Hitem:%x %x %x %x %x %x %x %x %x %x %x %x",
		&data[0], &data[1], &data[2], &data[3], &data[4], &data[5],
		&data[6], &data[7], &data[8], &data[9], &data[10], &data[11]);

	if (n != 12)
	{
		return 0;
	}

	DWORD dwChecksum = 0;
	dwChecksum = data[0] + data[1] + data[2] + data[3] + data[4] + data[5] + data[6] + data[7] + data[8] + data[9] + data[10];
	int i;
	for (i = 0; i < 11; ++i)
	{
		dwChecksum += data[i] * i;
		dwChecksum += data[i] >> 16 * i;
		dwChecksum = dwChecksum ^ data[i];
	}
	for (i = 0; i < 11; ++i)
	{
		dwChecksum += dwChecksum * data[i];
		dwChecksum += dwChecksum * data[i] >> 16;
		dwChecksum = dwChecksum ^ data[i];
	}
	dwChecksum = (dwChecksum & 0xFFFF);

	return dwChecksum;
}

