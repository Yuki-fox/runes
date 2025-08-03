#include "ClearAttribStoneFrame.h"
#include "../../mainproc/GameMain.h"
#include "../../mainproc/NetGlobal.h"
#include "..\NetWaker_member\Net_item\NetCli_Item.h"
#include "../../mainproc/GameMain.h"
#include "ChatFrame.h"
#include "BagFrame.h"

#include "ItemClipboard.h"
#include "ItemUTIL.h"

int			LuaFunc_CASF_GetAttribItemInfo( lua_State *L );
int			LuaFunc_CASF_SetSetClearAttribPos( lua_State *L );
int			LuaFunc_CASF_GetAttribStoneCount( lua_State *L );
int			LuaFunc_CASF_GetAttribStoneInfo( lua_State *L );
int			LuaFunc_CASF_ClearAttribStone( lua_State *L );
int			LuaFunc_CASF_ClearClose( lua_State *L );

CUI_ClearAttribStoneFrame*		g_pClearAttribStoneFrame=NULL;
CUI_ClearAttribStoneFrame::CUI_ClearAttribStoneFrame(CInterface* object ) : CInterfaceBase( object ),m_itemPos(-1)
{
	g_pClearAttribStoneFrame = this;

}
CUI_ClearAttribStoneFrame:: ~CUI_ClearAttribStoneFrame(void)
{
}
void CUI_ClearAttribStoneFrame::ClearClose(void)
{
	if (m_itemPos!=-1)
	{
			g_pBagFrame->Unlocked(m_itemPos);
			m_itemPos=-1;

	}
}
void CUI_ClearAttribStoneFrame::RegisterFunc(void)
{
	lua_State* l = GetLuaStateWorld();
	if (l)
	{

		lua_register( l, "CASF_GetAttribItemInfo",			LuaFunc_CASF_GetAttribItemInfo		);
		lua_register( l, "CASF_SetSetClearAttribPos",			LuaFunc_CASF_SetSetClearAttribPos		);
	
		lua_register( l, "CASF_GetAttribStoneCount",			LuaFunc_CASF_GetAttribStoneCount		);
		lua_register( l, "CASF_GetAttribStoneInfo",			LuaFunc_CASF_GetAttribStoneInfo		);
		lua_register( l, "CASF_ClearAttribStone",			LuaFunc_CASF_ClearAttribStone		);
		lua_register( l, "CASF_ClearClose",			LuaFunc_CASF_ClearClose		);


		
	}
}

static GameObjDbStructEx* _getItemFieldStruct_Attrib2(ItemFieldStruct* item,int Pos)
{
	GameObjDbStructEx* ret=NULL;
	if (Pos>=0&&Pos<6)
	{
		GameObjDbStructEx *inherent = CNetGlobal::GetObj(item->Inherent(Pos));
		if ( inherent && inherent->Classification == EM_ObjectClass_Attribute )
		{
				ret=inherent;
		}
	}

	return ret;
}


static ItemFieldStruct* _getItemFieldStruct_Attrib(int pos)
{
	if (pos>=0&& pos< _MAX_BODY_COUNT_)
	{
		ItemFieldStruct* item = CNetGlobal::RoleData()->GetBodyItem(pos);
		if (item )
		{
			GameObjDbStructEx* itemDB = g_ObjectData->GetObj( item->OrgObjID);

			if( !itemDB )
			{
				return NULL;
			}
			if (item->Mode.ItemLock)
			{
				const char * Error=g_ObjectData->GetString("SYS_GAMEMSGEVENT_421");
				lua_State* L = g_pClearAttribStoneFrame->GetLuaState();
				lua_pushstring(L, Error);
				lua_setglobal(L, "arg1");
				g_pClearAttribStoneFrame->SendWorldEvent("CLEARSTONE_ERROR");	
				return NULL;
			}

			
			switch ( itemDB->Classification )
			{
				case EM_ObjectClass_Item :
				{
					if( itemDB->Item.Class == EM_Item_Weapon || itemDB->Item.Class == EM_Item_Armor)
						return item;
				}
			}


		}
	}

return NULL;
}
bool CUI_ClearAttribStoneFrame::SetClearAttribPos(int index)
{

	ItemFieldStruct* item=_getItemFieldStruct_Attrib( index);
	if (item)
	{
		int count=0;
		for (int i=0;i<6;i++)
		{

			GameObjDbStructEx* attribDB=_getItemFieldStruct_Attrib2(item,i);

			if (attribDB&&attribDB->IsAttribute())
				count++;

		}

		//if (count>0)
		{
			ClearClose();


			ItemFieldStruct* item = CNetGlobal::RoleData()->GetBodyItem(index);

			int userIndex=g_pBagFrame->GetUseItemIndex();
			if (userIndex==-1)
			{
				ClipboardItemInfo itemInfo =g_pItemClipboard->GetItemInfo();

				if ( item && (item->Pos == EM_ItemState_NONE || itemInfo.bag.pos  ==index))
				{

					g_pBagFrame->Locked(index);
					m_itemPos=index;
				}	
			}

		
		
		}

		
	}
	else
	{
		if (m_itemPos!=-1)
			ClearClose();
		m_itemPos=-1;
	}


	return m_itemPos!=-1;
}

void CUI_ClearAttribStoneFrame::ClearAttribStoneResult(bool Result)
{
	const char * Error=g_ObjectData->GetString("CASF_STR_CLEAROK");
/*
	switch (Result )
	{
	case EM_DrawOutRuneResult_OK				://
		Error=g_ObjectData->GetString("DRAWRUNE_STR_OK");
		break;
	case EM_DrawOutRuneResult_DataErr		://,
		Error=g_ObjectData->GetString("DRAWRUNE_STR_DATAERROR");
		break;
	case EM_DrawOutRuneResult_MoneyErr		://,
		Error=g_ObjectData->GetString("DRAWRUNE_STR_MONEYERROR");
		break;
	case EM_DrawOutRuneResult_RuneErr		://,
		Error=g_ObjectData->GetString("DRAWRUNE_STR_RUNEERROR");
		break;
	case EM_DrawOutRuneResult_ItemQueueErr	://,	//太多東西在 物品queue內
		Error=g_ObjectData->GetString("DRAWRUNE_STR_ITEMQUEUEERROR");
		break;
	case EM_DrawOutRuneResult_ItemLock	://,	//物品安全鎖
		Error=g_ObjectData->GetString("SYS_GAMEMSGEVENT_421");
		break;
		
	
	}*/

	if (g_pBagFrame&&m_itemPos!=-1)
		g_pBagFrame->Locked(m_itemPos);
	if (!Result)
	{

		Error=g_ObjectData->GetString("CASF_STR_CLEARERROR");
		

		lua_State* L = GetLuaState();
		lua_pushstring(L, Error);
		lua_setglobal(L, "arg1");
		SendWorldEvent("CLEARSTONE_ERROR");	
	//	g_pGameMain->SendWarningMsg(Error);
	}
	else
		SendWorldEvent("CLEARSTONE_OK");	
//	DrawClose();
	g_pChatFrame->SendMsgEvent(CHAT_MSG_SYSTEM, Error, "" );

}
void CUI_ClearAttribStoneFrame::ClearAttribStoneRequest(int index)
{
	GameObjDbStructEx* RuneDB=NULL;
	ItemFieldStruct* item=_getItemFieldStruct_Attrib(m_itemPos);
	if (item)
	 RuneDB=_getItemFieldStruct_Attrib2(item,index);
	if (item&&RuneDB&&index>=0&&index<6)
	{
		GameObjDbStructEx* itemDB = g_ObjectData->GetObj( item->OrgObjID);
		NetCli_Item::SL_ClsAttribute(0,m_itemPos,item->Inherent(index));

	}

	//rawOutRuneRequest(m_itemPos,item->Rune(index));
}

int  CUI_ClearAttribStoneFrame::GetClearItemInfo(void)
{

	lua_State* L = GetLuaState();
	ItemFieldStruct *item=_getItemFieldStruct_Attrib(m_itemPos);

	char icon[512]={0};

	char temp[512]={0};
	if (item)
	{

		GameObjDbStructEx* itemDB = CNetGlobal::GetObj(item->OrgObjID);			

		strcpy(icon,ItemUTIL::GetItemIcon(item));
					

		strcpy(temp,itemDB->GetName());

	}
	lua_pushstring(L, icon);			
	lua_pushstring(L, temp);		
	lua_pushinteger(L, m_itemPos);	

	return 3;



}
int CUI_ClearAttribStoneFrame::GetAttribStoneCount(void )
{

	int count=0;
	ItemFieldStruct *item=_getItemFieldStruct_Attrib(m_itemPos);
	
	if (item)
	{

		for (int i=0;i<6;i++)
		{

			GameObjDbStructEx* attribDB=_getItemFieldStruct_Attrib2(item,i);

			if (attribDB&&attribDB->IsAttribute())
				count++;

		}
		

			

	}

	

	return count;



}
int	LuaFunc_CASF_GetAttribStoneCount( lua_State *L )
{

	int count=0;
 	if (g_pClearAttribStoneFrame)
		count=g_pClearAttribStoneFrame->GetAttribStoneCount();
	

	lua_pushinteger(L,count);


	return 1;
}
/*

static void _getWearEqTypeText( WearEqTypeENUM type, float value, bool inborn, const char* name, int abilityCount)
{
	if ( type == EM_WearEqType_None || value == 0 )
		return;

	char buf[256];
	float r, g, b;	

	g_ObjectData->GetWearEqTypeString(type, value, buf, 256);
	if ( value < 0.0f )
	{
		r = 1.0f;
		g = 0.0f;
		b = 0.0f;
	}
	else
	{
		if ( inborn )
		{
			r = 1.0f;
			g = 1.0f;
			b = 0.0f;

			if ( abilityCount == 3 )
			{
				r = 0.94f;
				g = 0.38f;
				b = 0.05f;
			}
			else if ( abilityCount == 4 )
			{
				r = 0.94f;
				g = 0.08f;
				b = 0.88f;
			}
		}
		else
		{
			r = 0.0f;
			g = 1.0f;
			b = 0.0f;
		}
	}

	if ( name == NULL || name[0] == 0 )
	{
		tooltip->AddLine(buf, r, g, b);
	}
	else
	{
		tooltip->AddDoubleLine(buf, name, r, g, b, 1.0f, 1.0f, 1.0f);
	}	
}

static void _getAbilityText(GameObjectAttributeStruct& ability, const char* name)
{	
	int abilityCount = 0;
	for ( int i = 0; i < Max_WearEqType_Count; i++)
	{
		if ( ability.Ability.EqType[i] != EM_WearEqType_None && ability.Ability.EqTypeValue[i] != 0 )
			abilityCount++;
	}

	for ( int i = 0; i < Max_WearEqType_Count; i++ )
	{
		TooltipAddWearEqTypeText(tooltip, ability.Ability.EqType[i], ability.Ability.EqTypeValue[i] * scale, ability.IsBirthAbility, name, abilityCount);
		name = NULL;
	}
}

*/
static bool _findClearItem()
{

	int DBID=g_ObjectData->GetSysKeyValue("AttributeClearItemID");

	return g_pBagFrame->GetIndex(DBID)!=-1;


}
int	LuaFunc_CASF_GetAttribStoneInfo( lua_State *L )
{

	int index		= luaL_checkint(L, 1) -1;

	char icon[512]={0};

	char temp[512]={0};
	int itemPos=-1;
	bool bClearItem=false;
	if (g_pClearAttribStoneFrame)
		itemPos=g_pClearAttribStoneFrame->m_itemPos;

	ItemFieldStruct *item=NULL;
	if (itemPos!=-1)
		item=_getItemFieldStruct_Attrib(itemPos);


	if (item)
	{

		GameObjDbStructEx* attribDB=	_getItemFieldStruct_Attrib2(item, index);
		if ( attribDB && attribDB->IsAttribute() )	
		{

			strcpy(icon,ItemUTIL::GetItemIcon(attribDB,true));

			const char*	name = attribDB->GetName();

			GameObjectAttributeStruct &attrib=attribDB->Attirbute;

			strcpy(temp,name);
			char buf[256];
			char temp_b[512];
			float r,g,b;
			// 能力
			for ( int i = 0; i < Max_WearEqType_Count; i++ )
			{
			
				buf[0]=0;

				float value = (float)attrib.Ability.EqTypeValue[i];
				int type=attrib.Ability.EqType[i];
				if ( type  != EM_WearEqType_None && value != 0 )					\
				{
					g_ObjectData->GetWearEqTypeString((WearEqTypeENUM)type, value* 1.0f, buf  , 256);
					if ( value < 0.0f )
					{
						r = 1.0f;
						g = 0.0f;
						b = 0.0f;
					}
					else
					{
						r = 0.0f;
						g = 1.0f;
						b = 0.0f;					
					}

				}				
			
				if (strlen(buf)>0)
				{
					sprintf_s(temp_b, 512, " |cff%02x%02x%02x%s|r", (int)r*255,(int)g*255,(int)b*255 , buf);

					strcat(temp,temp_b);
				}
				
				
			}

			// 說明
			if ( attribDB->ShortNote[0] != 0 )
			{
				strcat(temp,ItemUTIL::GetItemNote(attribDB).c_str());
			}

			bClearItem=_findClearItem();
			//itemCost=attribDB->Item.DrawOutCost;



		}


	}

	lua_pushstring(L,icon);

	lua_pushstring(L,temp);
	lua_pushboolean(L,bClearItem);

	return 3;

}
int	LuaFunc_CASF_ClearAttribStone( lua_State *L )
{
	int index	= luaL_checkint(L, 1) -1;
	if (g_pClearAttribStoneFrame)
		g_pClearAttribStoneFrame->ClearAttribStoneRequest(index);

	return 0;
}

int			LuaFunc_CASF_SetSetClearAttribPos( lua_State *L )
{
	int pos	= luaL_checkint(L, 1) -1;

	bool ret=false;
	//int diamond	=g_ObjectData->GetSysKeyValue("DrawOutRuneCost");
	if (g_pClearAttribStoneFrame)
		ret=g_pClearAttribStoneFrame->SetClearAttribPos(pos);
	lua_pushboolean(L,ret);
//	lua_pushinteger(L,diamond);

	return 1;
}
//------------------------------------------
int	LuaFunc_CASF_GetAttribItemInfo( lua_State *L )
{

	if (g_pClearAttribStoneFrame)
	{
		return g_pClearAttribStoneFrame->GetClearItemInfo();

	}

	return 0;
}
//------------------------------------------
int LuaFunc_CASF_ClearClose( lua_State *L )
{

	if (g_pClearAttribStoneFrame)
	{
		 g_pClearAttribStoneFrame->ClearClose();

	}

	return 0;
}