#include "DrawRuneFrame.h"
#include "../../mainproc/GameMain.h"
#include "../../mainproc/NetGlobal.h"
#include "..\NetWaker_member\Net_item\NetCli_Item.h"
#include "../../mainproc/GameMain.h"
#include "ChatFrame.h"
#include "BagFrame.h"

#include "ItemClipboard.h"
#include "ItemUTIL.h"

int			LuaFunc_GetDrawItemInfo( lua_State *L );
int			LuaFunc_SetDrawItemPos( lua_State *L );
int			LuaFunc_GetItemRuneCount( lua_State *L );
int			LuaFunc_GetItemRuneInfo( lua_State *L );
int			LuaFunc_DrawItemRune( lua_State *L );
int			LuaFunc_DrawClose( lua_State *L );

CUI_DrawRuneFrame*		g_pDrawRuneFrame=NULL;
CUI_DrawRuneFrame::CUI_DrawRuneFrame(CInterface* object ) : CInterfaceBase( object ),m_itemPos(-1)
{
	g_pDrawRuneFrame = this;

}
CUI_DrawRuneFrame:: ~CUI_DrawRuneFrame(void)
{
}
void CUI_DrawRuneFrame::DrawClose(void)
{
	if (m_itemPos!=-1)
	{
			g_pBagFrame->Unlocked(m_itemPos);
			m_itemPos=-1;

	}
}
void CUI_DrawRuneFrame::RegisterFunc(void)
{
	lua_State* l = GetLuaStateWorld();
	if (l)
	{

		lua_register( l, "DrawRuneFrame_GetDrawItemInfo",			LuaFunc_GetDrawItemInfo		);
		lua_register( l, "DrawRuneFrame_SetDrawItemPos",			LuaFunc_SetDrawItemPos		);
	
		lua_register( l, "DrawRuneFrame_GetItemRuneCount",			LuaFunc_GetItemRuneCount		);
		lua_register( l, "DrawRuneFrame_GetItemRuneInfo",			LuaFunc_GetItemRuneInfo		);
		lua_register( l, "DrawRuneFrame_DrawItemRune",			LuaFunc_DrawItemRune		);
		lua_register( l, "DrawRuneFrame_DrawClose",			LuaFunc_DrawClose		);


		
	}
}

GameObjDbStructEx* getItemFieldStructRune(ItemFieldStruct* item,int RunePos)
{
	GameObjDbStructEx* runeDB=0;
	if (RunePos>=0&&RunePos<4)
	{


		int dbID=item->Rune(RunePos);

		 runeDB = g_ObjectData->GetObj( dbID);


	}

	return runeDB;
}


ItemFieldStruct* getItemFieldStructRune(int pos)
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
				lua_State* L = g_pDrawRuneFrame->GetLuaState();
				lua_pushstring(L, Error);
				lua_setglobal(L, "arg1");
				g_pDrawRuneFrame->SendWorldEvent("DRAWRUNE_ERROR");	
				return NULL;
			}
			if (!itemDB->Mode.DisableMagicBox)
			{
				if (itemDB->IsArmor()||itemDB->IsWeapon())
					return item;

			}
		}
	}

return NULL;
}
bool CUI_DrawRuneFrame::SetDrawItemPos(int index)
{

	ItemFieldStruct* item=getItemFieldStructRune( index);
	if (item)
	{
		int count=0;
		for (int i=0;i<4;i++)
		{

			GameObjDbStructEx* runeDB=getItemFieldStructRune(item,i);

			if (runeDB&&runeDB->IsRune())
				count++;

		}

		//if (count>0)
		{
			DrawClose();


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
			DrawClose();
		m_itemPos=-1;
	}


	return m_itemPos!=-1;
}

void CUI_DrawRuneFrame::DrawOutRuneResult(int Result)
{
	const char * Error="UNKNOWN";

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
		
	
	}

	if (g_pBagFrame&&m_itemPos!=-1)
		g_pBagFrame->Locked(m_itemPos);
	if (Result!=EM_DrawOutRuneResult_OK)
	{


		lua_State* L = GetLuaState();
		lua_pushstring(L, Error);
		lua_setglobal(L, "arg1");
		SendWorldEvent("DRAWRUNE_ERROR");	
	//	g_pGameMain->SendWarningMsg(Error);
	}
	else
		SendWorldEvent("DRAWRUNE_OK");	
//	DrawClose();
	g_pChatFrame->SendMsgEvent(CHAT_MSG_SYSTEM, Error, "" );

}
void CUI_DrawRuneFrame::DrawOutRuneRequest(int index)
{
	GameObjDbStructEx* RuneDB=NULL;
	ItemFieldStruct* item=getItemFieldStructRune(m_itemPos);
	if (item)
	 RuneDB=getItemFieldStructRune(item,index);
	if (item&&RuneDB&&index>=0&&index<4)
	NetCli_Item::S_DrawOutRuneRequest(m_itemPos,item->Rune(index));
}

int  CUI_DrawRuneFrame::GetDrawItemInfo(void)
{

	lua_State* L = GetLuaState();
	ItemFieldStruct *item=getItemFieldStructRune(m_itemPos);

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
int CUI_DrawRuneFrame::GetItemRuneCount(void )
{

	int count=0;
	ItemFieldStruct *item=getItemFieldStructRune(m_itemPos);
	
	if (item)
	{

		for (int i=0;i<4;i++)
		{

			GameObjDbStructEx* runeDB=getItemFieldStructRune(item,i);

			if (runeDB&&runeDB->IsRune())
				count++;

		}
		

			

	}

	

	return count;



}
int	LuaFunc_GetItemRuneCount( lua_State *L )
{

	int count=0;
	if (g_pDrawRuneFrame)
		count=g_pDrawRuneFrame->GetItemRuneCount();
	

	lua_pushinteger(L,count);


	return 1;
}
int	LuaFunc_GetItemRuneInfo( lua_State *L )
{

	int index		= luaL_checkint(L, 1) -1;

	char icon[512]={0};

	char temp[512]={0};
	int itemPos=-1;
	int itemCost=0;
	if (g_pDrawRuneFrame)
		itemPos=g_pDrawRuneFrame->m_itemPos;

	ItemFieldStruct *item=NULL;
	if (itemPos!=-1)
		item=getItemFieldStructRune(itemPos);


	if (item)
	{

		GameObjDbStructEx* runeDB=	getItemFieldStructRune(item, index);
		if ( runeDB && runeDB->Item.Class == EM_Item_Rune )	
		{

			strcpy(icon,ItemUTIL::GetItemIcon(runeDB,true));

			const char*	name = runeDB->GetName();

			strcpy(temp,name);
			char buf[256];
			char temp_b[512];
			float r,g,b;
			// 能力
			for ( int i = 0; i < Max_WearEqType_Count; i++ )
			{
			
				buf[0]=0;
				
				float value = (float)runeDB->Item.Ability.EqTypeValue[i];
				
				
				g_ObjectData->GetWearEqTypeString(runeDB->Item.Ability.EqType[i], value* 1.0f, buf  , 256);


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
				if (strlen(buf)>0)
				{
					sprintf_s(temp_b, 512, " |cff%02x%02x%02x%s|r", (int)r*255,(int)g*255,(int)b*255 , buf);

					strcat(temp,temp_b);
				}
				
				
			}

			// 說明
			if ( runeDB->ShortNote[0] != 0 )
			{
				strcat(temp,ItemUTIL::GetItemNote(runeDB).c_str());
			}

			itemCost=runeDB->Item.DrawOutCost;



		}


	}

	lua_pushstring(L,icon);

	lua_pushstring(L,temp);
	lua_pushinteger(L,itemCost);

	return 3;

}
int	LuaFunc_DrawItemRune( lua_State *L )
{
	int index	= luaL_checkint(L, 1) -1;
	if (g_pDrawRuneFrame)
		g_pDrawRuneFrame->DrawOutRuneRequest(index);

	return 0;
}

int			LuaFunc_SetDrawItemPos( lua_State *L )
{
	int pos	= luaL_checkint(L, 1) -1;

	bool ret=false;
	//int diamond	=g_ObjectData->GetSysKeyValue("DrawOutRuneCost");
	if (g_pDrawRuneFrame)
		ret=g_pDrawRuneFrame->SetDrawItemPos(pos);
	lua_pushboolean(L,ret);
//	lua_pushinteger(L,diamond);

	return 1;
}
//------------------------------------------
int	LuaFunc_GetDrawItemInfo( lua_State *L )
{

	if (g_pDrawRuneFrame)
	{
		return g_pDrawRuneFrame->GetDrawItemInfo();

	}

	return 0;
}
//------------------------------------------
int LuaFunc_DrawClose( lua_State *L )
{

	if (g_pDrawRuneFrame)
	{
		 g_pDrawRuneFrame->DrawClose();

	}

	return 0;
}