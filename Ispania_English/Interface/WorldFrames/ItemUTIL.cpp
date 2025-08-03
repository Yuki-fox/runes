#include "ItemUtil.h"
#include "..\mainproc\GameMain.h"
#include "RoleData\ObjectStruct.h"
#include "RoleData\RoleBaseDef.h"
#include "..\Mainproc\NetGlobal.h"
#include "../../Interface/Map/MapCommon.h"
#include "../../netwaker_member/Net_Script/NetCli_Script_Child.h"

static const  BYTE cUtf8Limits[] = 
{
	0xC0, // Start of a 2-byte sequence
	0xE0, // Start of a 3-byte sequence
	0xF0, // Start of a 4-byte sequence
	0xF8, // Start of a 5-byte sequence
	0xFC, // Start of a 6-byte sequence
	0xFE // Invalid: not defined by original UTF-8 specification
};
static char* g_strBuff=NULL;
static int g_maxSrcLen=0;

int ItemUTIL::GetPet3DImageID(GameObjDbStruct* petDB)
{


	
		if (!petDB)
			return 0;
		if (petDB->Item.IncMagic_Onuse)//檢查施法設定物件
		{
			GameObjDbStruct* magicDB = CNetGlobal::GetObj(petDB->Item.IncMagic_Onuse);
			if (magicDB)
			{

				int magicID=magicDB->MagicCol.Magic[0][0].MagicBaseID;//檢查施法欄位第一個



				GameObjDbStruct* magicDB2 = CNetGlobal::GetObj(magicID);//檢查施法物件


				if (magicDB2)
				{
					int summonID=magicDB2->MagicBase.SummonCreature.ObjID;//檢查招換物件
					GameObjDbStruct* summonDB = CNetGlobal::GetObj(summonID);
					if (summonDB)//招換物件對應的ImageObject ID
						return summonDB->ImageID;
				}

			}

		}
		return 0;
	
}
const char* ItemUTIL::checkUtf8SafeString( const char* src,size_t& destLen,wchar_t* dest)
{
	size_t destPos = 0, srcPos = 0;
	int maxSrcLen=(int)strlen(src);
	destLen = maxSrcLen;

	/*
	if (destLen>0)
	{
		if (maxSrcLen>destLen)
			maxSrcLen=destLen;


	}
	*/


	if (g_maxSrcLen<maxSrcLen || g_strBuff == NULL )
	{

		g_maxSrcLen=maxSrcLen;
		if (g_strBuff)
			delete []g_strBuff;
		g_strBuff=new char[maxSrcLen+1];


	}

	memcpy(g_strBuff,src,maxSrcLen);
	g_strBuff[maxSrcLen]=0;

	while(srcPos<maxSrcLen)
	{
		BYTE c; // Note that byte_t should be unsigned
		size_t numAdds;

		if(srcPos == maxSrcLen || g_strBuff[srcPos] == '\0') 
		{
			

			destLen = destPos;
			return g_strBuff;
		}

		c = g_strBuff[srcPos++];

		if(c < 0x80)
		{ // 0-127, US-ASCII (single byte)
			if(dest)
				dest[destPos] = (wchar_t)c;
			++destPos;
			continue;
		}

		if(c < 0xC0) // The first octet for each code point should within 0-191
			break;

		for(numAdds = 1; numAdds < 5; ++numAdds)
			if(c < cUtf8Limits[numAdds])
				break;
		DWORD value = c - cUtf8Limits[numAdds - 1];

		do
		{
			BYTE c2;
			if(srcPos == maxSrcLen || g_strBuff[srcPos] == '\0')
			{
				g_strBuff[srcPos]=0;
				destLen = destPos;

				return g_strBuff;
			}
			c2 = g_strBuff[srcPos++];
			if(c2 < 0x80 || c2 >= 0xC0)
				break;
			value <<= 6;
			value |= (c2 - 0x80);
		} while(--numAdds != 0);

		if(value < 0x10000) 
		{
			if(dest)
				dest[destPos] = (wchar_t)value;
			++destPos;
		}
		else
		{
			value -= 0x10000;
			if(value >= 0x100000)
				break;
			if(dest) 
			{
				dest[destPos + 0] = (wchar_t)(0xD800 + (value >> 10));
				dest[destPos + 1] = (wchar_t)(0xDC00 + (value & 0x3FF));
			}
			destPos += 2;
		}
	}

	destLen = destPos;
	return g_strBuff;
}
// ----------------------------------------------------------------------------------
bool ItemUTIL::FindMagicTagStr( string& Str , char* findKey , GameObjDbStructEx** RetMagic , char* RetKey )
{
	int Pos = (int)Str.find( findKey );
	if( Pos == string::npos )
		return false;

	int MagicID = atol( Str.c_str() + Pos + strlen( findKey ) );
	sprintf( RetKey , "%s%d)", findKey , MagicID );

	if( Str.find( RetKey ) == string::npos )
		return false;

	(*RetMagic) = g_ObjectData->GetObj( MagicID );
	if( (*RetMagic)->IsMagicBase() == false )
		return false;

	return true;
}
const string ItemUTIL::GetMagicNoteEx( string Note  , int MagicLv )
{
	char	tempStr[128];
	char	findKey[32];

	//關鍵字 ??DMG 傷害力 
	string Str = Note;
	if( Str.length() == 0  )
		return "";

	//////////////////////////////////////////////////////////////////////////
	GameObjDbStructEx* MagicBase;
	//特殊設定
	for( int j = 0 ; j < 5 ;  )
	{
		sprintf( findKey , "(Buff%d-", j );
		if( FindMagicTagStr( Str , findKey , &MagicBase , findKey ) == false )
		{
			j++;
			continue;
		}
		int MagicBaseLv = __min( MagicLv , MagicBase->MagicBase.BuffMaxLv );

		float Power = MagicBase->MagicBase.Ability.EqTypeValue[j] * ( 100 + MagicBase->MagicBase.Ability_SkillLVArg * MagicBaseLv ) / 100;
		if( Power < 0 )
			Power *= -1;
		sprintf( tempStr , "%.1f" , Power  );
		ItemUTIL::ReplaceString( Str , findKey , tempStr );
	}

	for( int j = 0 ; j < 5 ;  )
	{
		sprintf( findKey , "(Max-Buff%d-", j );
		if( FindMagicTagStr( Str , findKey , &MagicBase , findKey ) == false )
		{
			j++;
			continue;
		}
		int MagicBaseLv = MagicBase->MagicBase.BuffMaxLv;

		float Power = MagicBase->MagicBase.Ability.EqTypeValue[j] * ( 100 + MagicBase->MagicBase.Ability_SkillLVArg * MagicBaseLv ) / 100;
		if( Power < 0 )
			Power *= -1;
		sprintf( tempStr , "%.1f" , Power  );
		ItemUTIL::ReplaceString( Str , findKey , tempStr );
	}

	//QQ 法術的技能時間
	sprintf( findKey , "(Max-BuffTime-");
	if( FindMagicTagStr( Str , findKey , &MagicBase , findKey ) == true )
	{
		int MagicBaseLv = MagicBase->MagicBase.BuffMaxLv;

		float Power = MagicBase->MagicBase.EffectTime
			* ( 100 + MagicBase->MagicBase.EffectTime_SkillLVArg * MagicBaseLv ) / 100;
		if( Power < 0 )
			Power *= -1;
		sprintf( tempStr , "%.1f" , Power  );
		ItemUTIL::ReplaceString( Str , findKey , tempStr );			
	}



	//魔法盾
	while( FindMagicTagStr( Str , "(Buff-Shield-" , &MagicBase , findKey ) == true )
	{
		int MagicBaseLv = __min( MagicLv , MagicBase->MagicBase.BuffMaxLv );
		float Power = MagicBase->MagicBase.MagicShield.Point * ( 100 + MagicBase->MagicBase.MagicShield.SkillLvArg * MagicBaseLv ) / 100;
		sprintf( tempStr , "%.1f" , Power  );
		ItemUTIL::ReplaceString( Str , findKey , tempStr );
	}
	//Dot
	while( FindMagicTagStr( Str , "(Buff-Dot-" , &MagicBase , findKey ) == true )
	{
		int MagicBaseLv = __min( MagicLv , MagicBase->MagicBase.BuffMaxLv );
		float Power = MagicBase->MagicBase.DotMagic.Base * ( 100 + MagicBase->MagicBase.DotMagic.DotSkillLVArg * MagicBaseLv ) / 100;
		if( Power < 0 )
			Power *= -1;

		sprintf( tempStr , "%.1f" , Power  );
		ItemUTIL::ReplaceString( Str , findKey , tempStr );
	}

	while( FindMagicTagStr( Str , "(Max-Buff-Dot-" , &MagicBase , findKey ) == true )
	{
		int MagicBaseLv = MagicBase->MagicBase.BuffMaxLv;
		float Power = MagicBase->MagicBase.DotMagic.Base * ( 100 + MagicBase->MagicBase.DotMagic.DotSkillLVArg * MagicBaseLv ) / 100;
		if( Power < 0 )
			Power *= -1;

		sprintf( tempStr , "%.1f" , Power  );
		ItemUTIL::ReplaceString( Str , findKey , tempStr );
	}


	//Time
	while( FindMagicTagStr( Str , "(Buff-Time-" , &MagicBase , findKey ) == true )
	{
		int MagicBaseLv = __min( MagicLv , MagicBase->MagicBase.BuffMaxLv );
		float Power = MagicBase->MagicBase.EffectTime * ( 100 + MagicBase->MagicBase.EffectTime_SkillLVArg * MagicBaseLv ) / 100;
		if( Power < 0 )
			Power *= -1;

		sprintf( tempStr , "%.1f" , Power  );
		ItemUTIL::ReplaceString( Str , findKey , tempStr );

	}
	//////////////////////////////////////////////////////////////////////////
	//攻擊法術

	while( FindMagicTagStr( Str , "(DMG-" , &MagicBase , findKey ) == true )
		//	for( int i = 0 ; i < 10 ; i++ )
	{
		float Power = MagicBase->MagicBase.Attack.DmgPower * ( 100 + MagicBase->MagicBase.Attack.DmgPower_SkillLVArg * MagicLv ) / 100;
		if( Power < 0 )
			Power *= -1;
		switch( MagicBase->MagicBase.AtkCalType )
		{
		case EM_MagicAttackCalBase_Physics://物理計算
			sprintf( tempStr , "%.1f%%" ,  Power*100  );
			break;
		case EM_MagicAttackCalBase_Magic://法術計算
			sprintf( tempStr , "%.1f" , Power   );
			/*
			if( Power > 0 )
			sprintf( tempStr , "%.1f" , Power   );
			else
			sprintf( tempStr , "%.1f" , Power*0.5   );
			*/
			break;
		case EM_MagicAttackCalBase_Throw://投擲計算
			sprintf( tempStr , "%.1f%%" , Power* 100 );
			break;
		case EM_MagicAttackCalBase_Shoot://射擊計算
			sprintf( tempStr , "%.1f%%" , Power* 100 );
			break;
		case EM_MagicAttackCalBase_Fix://定值
			sprintf( tempStr , "%.1f" , Power );
			break;
		case EM_MagicAttackCalBase_HolyHeal://神聖治療計算( 負值表示Miss  )
			sprintf( tempStr , "%.1f" , Power );
			break;
		case EM_MagicAttackCalBase_DarkHeal://不死系療計算( 負值表示Miss  )
			sprintf( tempStr , "%.1f" , Power  );
			break;
		case EM_MagicAttackCalBase_SpecialAction://沒攻擊只有特殊行為
			break;
		case EM_MagicAttackCalBase_FixPer://百分比
			sprintf( tempStr , "%.1f%%" , Power );
			break;
		case EM_MagicAttackCalBase_Physics_Point:
			sprintf( tempStr , "%.1f" , Power   );
			break;
		case EM_MagicAttackCalBase_MagicEx:
			sprintf( tempStr , "%.1f%%" , Power* 100 );
			break;
		}

		ItemUTIL::ReplaceString( Str , findKey , tempStr );
	}

	while( FindMagicTagStr( Str , "(FixDMG-" , &MagicBase , findKey ) == true )
	{
		float Power = MagicBase->MagicBase.Attack.FixValue * ( 100 + MagicBase->MagicBase.Attack.FixDmg_SkillLVArg * MagicLv ) / 100;
		if( Power < 0 )
			Power *= -1;
		sprintf( tempStr , "%.2f" , Power  );
		ItemUTIL::ReplaceString( Str , findKey , tempStr );

	}

	while( FindMagicTagStr( Str , "(FixPerDMG-" , &MagicBase , findKey ) == true )
	{
		float Power = MagicBase->MagicBase.Attack.FixValue * ( 100 + MagicBase->MagicBase.Attack.FixDmg_SkillLVArg * MagicLv ) / 100;
		if( Power < 0 )
			Power *= -1;
		sprintf( tempStr , "%.2f%%" , Power*100  );
		ItemUTIL::ReplaceString( Str , findKey , tempStr );

	}

	return Str;
}

const string ItemUTIL::GetMagicColNote( GameObjDbStruct* MagicCol , int MagicLv )
{
	char	tempStr[128];
	char	findKey[32];

	//關鍵字 ??DMG 傷害力 
	string Str = CNetCli_Script_Child::ReplaceSystemKeyword(MagicCol->ShortNote);
	if( Str.length() == 0  )
		return "";

	for( int i = 0 ; i < 9 ; i++ )
	{
		GameObjDbStruct* MagicBase = CNetGlobal::GetObj( MagicCol->MagicCol.Magic_List[i].MagicBaseID );
		if( MagicBase == NULL )
			continue;


		//////////////////////////////////////////////////////////////////////////
		//攻擊系
		//////////////////////////////////////////////////////////////////////////
		if( MagicBase->MagicBase.MagicFunc ==EM_MAGICFUN_HPMP )
		{
			sprintf( findKey , "(DMG%d)" , i );
			if( MagicBase->MagicBase.Attack.DmgPower!= 0 && Str.find( findKey ) != string::npos )
			{
				float Power = MagicBase->MagicBase.Attack.DmgPower * ( 100 + MagicBase->MagicBase.Attack.DmgPower_SkillLVArg * MagicLv ) / 100;
				if( Power < 0 )
					Power *= -1;
				switch( MagicBase->MagicBase.AtkCalType )
				{
				case EM_MagicAttackCalBase_Physics://物理計算
					sprintf( tempStr , "%.1f%%" ,  Power*100  );
					break;
				case EM_MagicAttackCalBase_Magic://法術計算
					sprintf( tempStr , "%.1f" , Power   );
					/*
					if( Power > 0 )
					sprintf( tempStr , "%.1f" , Power   );
					else
					sprintf( tempStr , "%.1f" , Power*0.5   );
					*/
					break;
				case EM_MagicAttackCalBase_Throw://投擲計算
					sprintf( tempStr , "%.1f%%" , Power* 100 );
					break;
				case EM_MagicAttackCalBase_Shoot://射擊計算
					sprintf( tempStr , "%.1f%%" , Power* 100 );
					break;
				case EM_MagicAttackCalBase_Fix://定值
					sprintf( tempStr , "%.1f" , Power );
					break;
				case EM_MagicAttackCalBase_HolyHeal://神聖治療計算( 負值表示Miss  )
					sprintf( tempStr , "%.1f" , Power );
					break;
				case EM_MagicAttackCalBase_DarkHeal://不死系療計算( 負值表示Miss  )
					sprintf( tempStr , "%.1f" , Power  );
					break;
				case EM_MagicAttackCalBase_SpecialAction://沒攻擊只有特殊行為
					break;
				case EM_MagicAttackCalBase_FixPer://百分比
					sprintf( tempStr , "%.1f%%" , Power );
					break;
				case EM_MagicAttackCalBase_Physics_Point:
					sprintf( tempStr , "%.1f" , Power   );
					/*
					if( Power > 0 )
					sprintf( tempStr , "%.1f" , Power   );
					else
					sprintf( tempStr , "%.1f" , Power*0.5   );
					*/
					break;
				case EM_MagicAttackCalBase_MagicEx:
					sprintf( tempStr , "%.1f%%" , Power* 100 );
					break;
				}

				ItemUTIL::ReplaceString( Str , findKey , tempStr );
			}

			sprintf( findKey , "(FixDMG%d)" , i );
			if( MagicBase->MagicBase.Attack.FixValue != 0 && Str.find( findKey ) != string::npos )
			{
				float Power = MagicBase->MagicBase.Attack.FixValue * ( 100 + MagicBase->MagicBase.Attack.FixDmg_SkillLVArg * MagicLv ) / 100;
				if( Power < 0 )
					Power *= -1;
				sprintf( tempStr , "%.2f" , Power  );
				ItemUTIL::ReplaceString( Str , findKey , tempStr );
			}

			sprintf( findKey , "(FixPerDMG%d)" , i );
			if( MagicBase->MagicBase.Attack.FixValue != 0 && Str.find( findKey ) != string::npos )
			{
				float Power = MagicBase->MagicBase.Attack.FixValue * ( 100 + MagicBase->MagicBase.Attack.FixDmg_SkillLVArg * MagicLv ) / 100;
				if( Power < 0 )
					Power *= -1;
				sprintf( tempStr , "%.2f%%" , Power*100  );
				ItemUTIL::ReplaceString( Str , findKey , tempStr );
			}
		}
		//////////////////////////////////////////////////////////////////////////
		int RealMagicLv = MagicLv;
		if( MagicBase->MagicBase.BuffMaxLv < RealMagicLv )
			RealMagicLv = MagicBase->MagicBase.BuffMaxLv;

		if( MagicBase->MagicBase.MagicFunc == EM_MAGICFUN_Assist )
		{
			for( int j = 0 ; j < 5 ; )
			{
				sprintf( findKey , "(Buff%d-%d)" , i , j );
				if( MagicBase->MagicBase.Ability.EqTypeValue[j] != 0 && Str.find( findKey ) != string::npos )
				{		
					float Power = MagicBase->MagicBase.Ability.EqTypeValue[j] * ( 100 + MagicBase->MagicBase.Ability_SkillLVArg * RealMagicLv ) / 100;
					if( Power < 0 )
						Power *= -1;
					sprintf( tempStr , "%.1f" , Power  );
					ItemUTIL::ReplaceString( Str , findKey , tempStr );
				}
				else
				{
					j++;
				}
			}

			//魔法盾
			sprintf( findKey , "(Buff%d-Shield)" , i );
			if( MagicBase->MagicBase.MagicShield.Point != 0 && Str.find( findKey ) != string::npos )
			{

				float Power = MagicBase->MagicBase.MagicShield.Point * ( 100 + MagicBase->MagicBase.MagicShield.SkillLvArg * RealMagicLv ) / 100;

				sprintf( tempStr , "%.1f" , Power  );
				ItemUTIL::ReplaceString( Str , findKey , tempStr );

			}

			//Dot
			sprintf( findKey , "(Buff%d-Dot)" , i  );
			if( MagicBase->MagicBase.DotMagic.Base  != 0 && Str.find( findKey ) != string::npos )
			{
				float Power = MagicBase->MagicBase.DotMagic.Base * ( 100 + MagicBase->MagicBase.DotMagic.DotSkillLVArg * RealMagicLv ) / 100;
				if( Power < 0 )
					Power *= -1;
				sprintf( tempStr , "%.1f" , Power  );
				ItemUTIL::ReplaceString( Str , findKey , tempStr );

			}

			//Time
			sprintf( findKey , "(Buff%d-Time)" , i  );
			if( MagicBase->MagicBase.EffectTime != 0 && Str.find( findKey ) != string::npos )
			{
				float Power = MagicBase->MagicBase.EffectTime * ( 100 + MagicBase->MagicBase.EffectTime_SkillLVArg * RealMagicLv ) / 100;
				if( Power < 0 )
					Power *= -1;
				sprintf( tempStr , "%.1f" , Power  );
				ItemUTIL::ReplaceString( Str , findKey , tempStr );

			}
		}
	}



	//////////////////////////////////////////////////////////////////////////

	return GetMagicNoteEx( Str , MagicLv );
}


void  ItemUTIL::ReplaceString( string& sourcestr , const char* oldstr, const char* newstr )
{

	// search in the lowercase versions,
	// replace in the original-case version.
	int pos=0;
	int lenoldstr = (int)strlen(oldstr);
	while( 1 )
	{
		pos = (int)sourcestr.find( oldstr );
		if( pos == string::npos )
			break;

		sourcestr.erase( pos ,lenoldstr );
		sourcestr.insert( pos ,newstr );
	}

}

char * ItemUTIL::GetItemIcon(ItemFieldStruct * item)
{
	// 金錢
	/*
	if ( item->OrgObjID == g_ObjectData->GetMonetItemID(EM_ACPriceType_GameMoney) )
	{
		return "interface\\Icons\\coin_03.dds";
	}
	// 符文幣
	else if ( item->OrgObjID == g_ObjectData->GetMonetItemID(EM_ACPriceType_AccountMoney) )
	{
		return "interface\\Icons\\coin_04.dds";
	}
	*/

	//融合過的物品?
	int dbid=item->ImageObjectID;
	if (dbid==0) //沒有用原本的圖像
		dbid=item->OrgObjID;

	GameObjDbStruct* itemDB = CNetGlobal::GetObj(  dbid);
	if ( itemDB )
	{
		GameObjDbStruct* imageDB = g_ObjectData->GetObj(itemDB->ImageID);
		if ( imageDB )
		{
			const char* icon = imageDB->Image.ACTField;
			if ( icon == NULL || icon[0] == 0 /*|| g_ruResourceManager->FileExists(icon) == false*/ )
				icon = "interface\\Icons\\skill_unlearn.dds";
			return (char*)icon;
		}
	}
	return NULL;
}

char* ItemUTIL::GetItemIcon(GameObjDbStruct* itemDB, bool repalceEmptyIcon)
{
	if ( itemDB )
	{
		GameObjDbStruct* imageDB = CNetGlobal::GetObj(itemDB->ImageID);
		if ( imageDB )
		{
			return imageDB->Image.ACTField;
		}
	}
	return (repalceEmptyIcon) ? "interface\\Icons\\skill_unlearn.dds" : NULL;
}

char * ItemUTIL::GetItemACTWorld(ItemFieldStruct * item)
{
	//融合過的物品?
	int dbid=item->ImageObjectID;
	if (dbid==0) //沒有用原本的圖像
		dbid=item->OrgObjID;
  
	GameObjDbStruct* itemDB = CNetGlobal::GetObj(  dbid);
	if ( itemDB )
	{
		GameObjDbStruct* imageDB = g_ObjectData->GetObj(itemDB->ImageID);
		if ( imageDB )
		{
			return imageDB->Image.ACTWorld;
		}
	}
	return NULL;
}

char* ItemUTIL::GetItemACTWorld(GameObjDbStruct* itemDB)
{
	if ( itemDB )
	{
		GameObjDbStruct* imageDB = CNetGlobal::GetObj(itemDB->ImageID);
		if ( imageDB )
		{
			return imageDB->Image.ACTWorld;
		}
	}
	return NULL;
}

 const string   ItemUTIL::GetItemNote( GameObjDbStruct* ItemDB )
{
	//char	tempStr[128];
	//char	findKey[32];
	const char*
		findKey = "(Magic)";
	//關鍵字 (Magic)
	string Str = CNetCli_Script_Child::ReplaceSystemKeyword(ItemDB->ShortNote);
	if( ItemDB == NULL || Str.length() == 0 )
		return "";

	int begin, end;

	begin = (int)Str.find("(maxcount)");	
	if ( begin != string::npos && ItemDB->Classification == EM_ObjectClass_Item )
	{
		char temp[512];
		sprintf_s(temp, "%d", ItemDB->Item.BodyMaxCount);
		end = begin + (int)strlen("(maxcount)");
		Str.replace(begin, end - begin, temp);
	}	

	// 特殊取得副本替代字串
	begin = (int)Str.find("(SYSTEM:");
	end = (int)Str.find(")");
	if ( begin != string::npos && end != string::npos )
	{
		char replaceStr[4096];
		char typeStr[512];
		memset(replaceStr, 0, sizeof(replaceStr));
		memset(typeStr, 0, sizeof(typeStr));
		Str.copy( typeStr, end - (begin + 1 + 7), begin + 1 + 7);

		char temp[512];
		for ( int i = 1; i < 32; i++ )
		{
			sprintf_s(temp, 512, "%s_%d", typeStr, i);
			int value = g_ObjectData->GetSysKeyValue( temp );
			if ( value > 0 )
			{
				const char* zoneName = Func_GetLocalMapName(value);					
				if ( replaceStr[0] )
					strcat(replaceStr, " ");
				strcat(replaceStr, zoneName);
			}
		}

		Str.replace(begin, end - begin + 1, replaceStr);
	}

	//////////////////////////////////////////////////////////////////////////
	if( ItemDB->Item.UseType == EM_ItemUseType_Magic )
	{		
		if( Str.find( findKey ) != string::npos )
		{
			GameObjDbStructEx* MagicCol = g_ObjectData->GetObj( ItemDB->Item.IncMagic_Onuse );
			if( MagicCol != NULL )
			{
				ReplaceString( Str , findKey , GetMagicColNote( MagicCol , 0 ).c_str() );
			}
		}
	}
	return Str;
 }