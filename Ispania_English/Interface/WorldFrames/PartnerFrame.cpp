#include "PartnerFrame.h"
#include "..\NetWaker_member\Net_item\NetCli_Item.h"
#include "../../mainproc/GameMain.h"
#include "../../mainproc/NetGlobal.h"
#include "ItemUTIL.h"
#include "ChatFrame.h"
#include "ItemClipboard.h"

#include "..\NetWaker_member\Net_CultivatePet\NetCli_CultivatePet.h"

#include "BagFrame.h"
CUI_PartnerFrame*					g_pPartnerFrame = NULL;
int	LuaFunc_PartnerFrame_GetPartnerInfo( lua_State *L );

int	LuaFunc_PartnerFrame_IsCanAddPetColletcion( lua_State *L );
int	LuaFunc_PartnerFrame_CallPartner( lua_State *L );
int	LuaFunc_PartnerFrame_AddPartner( lua_State *L );
int	LuaFunc_PartnerFrame_CanAddToPartner( lua_State *L );
int	LuaFunc_PartnerFrame_GetPartnerCount( lua_State *L );
int	LuaFunc_PartnerFrame_RemovePartner( lua_State *L );

int	LuaFunc_PartnerFrame_PickPartner( lua_State *L );
int LuaFunc_PartnerFrame_AddPetCollection( lua_State *L );
int LuaFunc_PartnerFrame_GetPartnerBuff( lua_State *L );
void CUI_PartnerFrame::RegisterFunc()
{
	lua_State* L = GetLuaStateWorld();
	if (L)
	{
		lua_register( L, "PartnerFrame_AddPartner",			LuaFunc_PartnerFrame_AddPartner );
		lua_register( L, "PartnerFrame_RemovePartner",		LuaFunc_PartnerFrame_RemovePartner );
		lua_register( L, "PartnerFrame_CanAddToPartner",	LuaFunc_PartnerFrame_CanAddToPartner );
		lua_register( L, "PartnerFrame_GetPartnerCount",	LuaFunc_PartnerFrame_GetPartnerCount );
		lua_register( L, "PartnerFrame_CallPartner",		LuaFunc_PartnerFrame_CallPartner );
		lua_register( L, "PartnerFrame_GetPartnerInfo",		LuaFunc_PartnerFrame_GetPartnerInfo );
		lua_register( L, "PartnerFrame_PickPartner",		LuaFunc_PartnerFrame_PickPartner );
		lua_register( L, "PartnerFrame_GetPartnerBuff",		LuaFunc_PartnerFrame_GetPartnerBuff );
		lua_register( L, "PartnerFrame_AddPetCollection",		LuaFunc_PartnerFrame_AddPetCollection );
		lua_register( L, "PartnerFrame_IsCanAddPetColletcion",		LuaFunc_PartnerFrame_IsCanAddPetColletcion );

		
		
	}
}

CUI_PartnerFrame::CUI_PartnerFrame(CInterface* object): CInterfaceBase(object)
{
	g_pPartnerFrame=this;
	m_pos=-1;
	
	RoleDataEx *Role=CNetGlobal:: RoleData();
	for (int i=0;i<_DEF_MAX_PET_EXBAG_COUNT_;i++)
	{
		Role->PlayerBaseData->PetExBag[i];
	}


}
//	StaticVector< int , _DEF_MAX_PET_EXBAG_COUNT_ > PetExBag;	
//  StaticVector< HorseExBagBase , _DEF_MAX_HORSE_EXBAG_COUNT_ > HorseExBag;	
//
CUI_PartnerFrame::~CUI_PartnerFrame()
{

	g_pPartnerFrame=NULL;
}
GameObjDbStructEx* getExBagPartnerDB(int mode,int  index,int * colorBuff=NULL );

bool IsPetCollection(int dbid,int&CardID, bool& bAdd)
{

	CardID = -1;
	int count=(int)g_ObjectData->_PetCardList.size();
	for ( unsigned i = 0 ; i < (unsigned)count ; i++ )
	{
		if( g_ObjectData->_PetCardList[i] == dbid )
		{
			RoleDataEx *Role=CNetGlobal:: RoleData();
			CardID=i;
			if (i>=0&&i<(unsigned)Role->PlayerBaseData->PetCard.Size())
			{

				bAdd= !Role->PlayerBaseData->PetCard.GetFlag(i);
				return false;//關掉
				//return true; 
			}

			return false;
		}
	}

	return false;
}
GameObjDbStructEx* getExBagPartnerDB(int mode,int  index,int * colorBuff )
{
	GameObjDbStructEx* partnerDB=NULL;
	RoleDataEx *Role=CNetGlobal:: RoleData();
	int dbid=-1;
	if (mode==EM_ExBagType_Pet)
	{
		StaticVector< int , _DEF_MAX_PET_EXBAG_COUNT_ >& vectorPet=Role->PlayerBaseData->PetExBag;
		int count=vectorPet.Size();

		if (index>=0&&index<count)
		dbid=vectorPet[index];
	}
	else
	{
		StaticVector< HorseExBagBase , _DEF_MAX_HORSE_EXBAG_COUNT_ >& vectorH=Role->PlayerBaseData->HorseExBag;
		int count=vectorH.Size();

		if (index>=0&&index<count)

		dbid=vectorH[index].ItemID;
		if (colorBuff)
		{

			for(int i=0;i<4;i++)
			{
				colorBuff[i]=vectorH[index].HorseColor[i];

			}
		}

	}

	if (dbid!=-1)
	partnerDB=g_ObjectData->GetObj( dbid);


	return partnerDB;

}
int CUI_PartnerFrame::GetPartnerBuff(int mode,int index)
{
	lua_State* L = GetLuaState();

	if (mode==EM_ExBagType_Pet)
		return 0;



	
	GameObjDbStructEx* partnerDB=	getExBagPartnerDB(mode, index);
	if ( partnerDB  )	
	{

		int magicObj=partnerDB->Item.IncMagic_Onuse;


		GameObjDbStructEx* magicDB=g_ObjectData->GetObj( magicObj);
		if (magicDB)
		{
			GameObjDbStruct* MagicBaseDB = g_ObjectData->GetObj(magicDB->MagicCol.Magic_List[0].MagicBaseID);
			if (MagicBaseDB)
			{

			

				int CarryCount =MagicBaseDB->MagicBase.CarryCount;
				bool WaterWalk =MagicBaseDB->MagicBase.Effect.WaterWalk;
				bool fly =MagicBaseDB->MagicBase.Effect.Fly;

				int i;
				int speed =0;
				for (i=0;i<Max_WearEqType_Count;i++)
				{
					if (MagicBaseDB->MagicBase.Ability.EqType[i]==EM_WearEqType_RideSpeed)
					{

						//取最大速度
						if (MagicBaseDB->MagicBase.Ability.EqTypeValue[i]>speed)
							speed=MagicBaseDB->MagicBase.Ability.EqTypeValue[i];

					}

				}
				lua_pushinteger(L, speed);						
				lua_pushinteger(L, CarryCount);		
				lua_pushboolean(L, fly);		
				lua_pushboolean(L, WaterWalk);		

				//lua_pushboolean(L, Pe);		
				
				return 4;
			}
		}

	}
	return 0;




}
int CUI_PartnerFrame::GetPartnerInfo(int mode,int index)
{

	lua_State* L = GetLuaState();
	char icon[512]={0};
	char name[512]={0};

	//char temp[512]={0};
	int dbid=-1;

	int Color[4]={-1,-1,-1,-1};
	



	GameObjDbStructEx* partnerDB=	getExBagPartnerDB(mode, index,Color);
	if ( partnerDB  )	
	{

		strcpy(icon,ItemUTIL::GetItemIcon(partnerDB,true));
		strcpy(name,partnerDB->GetName());

		dbid=partnerDB->GUID;
		
		

	}

	lua_pushstring(L, icon);			
	lua_pushinteger(L, dbid);		
	lua_pushstring(L, name);		

	for (int i=0;i<4;i++)
	{
		lua_pushinteger(L, Color[i]);		

	}


	return 7;

}

GameObjDbStructEx* getItemDBPartner(int pos)
{
	if (pos>=0&& pos< _MAX_BODY_COUNT_)
	{
		ItemFieldStruct* item = CNetGlobal::RoleData()->GetBodyItem(pos);
		if (item)
		{
			GameObjDbStructEx* itemDB = g_ObjectData->GetObj( item->OrgObjID);

			if( !itemDB )
			{
				return NULL;
			}


			switch ( itemDB->Classification )
			{
				case EM_ObjectClass_Item :
				{
					if( itemDB->Item.Class == EM_Item_Normal)
					{
						if (itemDB->Item.ExBagType==EM_ExBagType_Pet ||itemDB->Item.ExBagType==EM_ExBagType_Horse )//EM_ExBagType_None	,
							return itemDB;
					}
				}
			}


		}
	}

	return NULL;
}
int  CUI_PartnerFrame::CanAddToPartner(int index)
{
	
	lua_State* L = GetLuaStateWorld();

	GameObjDbStructEx* itemDB=getItemDBPartner(index);
	bool bOk=false;

	if (itemDB)
	{
		bOk=true;
	}
	else
	{
		if (g_pItemClipboard)
			g_pItemClipboard->Clear();
	}
	lua_pushboolean(L, bOk);			

	return 1;

}

void DelPartnerToBag(int mode,int index)
{
	RoleDataEx *Role=CNetGlobal:: RoleData();
	switch (mode)
	{

	case EM_ExBagType_Pet:
		{


			if (index>=0&&index<Role->PlayerBaseData->PetExBag.Size())
				Role->PlayerBaseData->PetExBag.Erase(index);


		}


		break;
	case EM_ExBagType_Horse:
		{
			if ( index>=0&&index<Role->PlayerBaseData->HorseExBag.Size())
			{
				

				Role->PlayerBaseData->HorseExBag.Erase(index);
			}

		}
		break;
	}

}
void AddPartnerToBag(int mode,int dbid,int *color)
{

	RoleDataEx *Role=CNetGlobal:: RoleData();

	switch (mode)
	{

	case EM_ExBagType_Pet:
		{

		
		if ( Role->PlayerBaseData->PetExBag.Size()<100)
			Role->PlayerBaseData->PetExBag.Push_Back(dbid);
		

		}

		
		break;
	case EM_ExBagType_Horse:
		{
			if ( Role->PlayerBaseData->HorseExBag.Size()<100)
			{
				HorseExBagBase a;
				a.ItemID=dbid;
				for (int i=0;i<4;i++)
					a.HorseColor[i]=color[i];

				Role->PlayerBaseData->HorseExBag.Push_Back(a);
			}

		}
		break;
	}
	


}
bool CUI_PartnerFrame:: CanUsePartner(int mode,int index)
{
	
	GameObjDbStructEx* partnerDB=	getExBagPartnerDB(mode, index);
	if ( partnerDB  )	
	{
		return true;

	}
	return false;

}
void  CUI_PartnerFrame::RegPetCardResult( bool result )
{
	const char * OutPutStr=NULL;
	if (result)
	{
		OutPutStr=g_ObjectData->GetString("PARTNERFRAME_STR_ADDPETCOLLECTIONOK");
		SendWorldEvent("PARTNERFRAME_ADDPETCOLLECTION");	
	}
	else

		OutPutStr=g_ObjectData->GetString("PARTNERFRAME_STR_ADDPETCOLLECTIONFAIL");

	if (OutPutStr)

		g_pChatFrame->SendMsgEvent(CHAT_MSG_SYSTEM, OutPutStr, "" );
}

int CUI_PartnerFrame::GetActionInfo(int mode,int index)
{
		lua_State* L = GetLuaStateWorld();

		
		GameObjDbStructEx* partnerDB=	getExBagPartnerDB(mode, index);
		if ( partnerDB  )	
		{

			char icon[512];
			char name[512];
			strcpy(icon,ItemUTIL::GetItemIcon(partnerDB,true));
			strcpy(name,partnerDB->GetName());
			
			lua_pushstring(L,icon);
			lua_pushstring(L,name);
			lua_pushnumber(L, 1);
			lua_pushboolean(L, false);
			return 4;
		}
		return 0;
}
GameObjDbStructEx*  CUI_PartnerFrame::GetItem(int mode,int index)
{


	
	return 	getExBagPartnerDB(mode, index);
}
void  CUI_PartnerFrame::PickupAction(int mode,int index)
{
	

		char icon[512];
		GameObjDbStructEx* partnerDB=	getExBagPartnerDB(mode, index);
		if ( partnerDB  )	
		{

			strcpy(icon,ItemUTIL::GetItemIcon(partnerDB,true));

		


			g_pItemClipboard->SetFile(icon );
			g_pItemClipboard->PickupPartner( mode,index);

		}
	
}

void  CUI_PartnerFrame::PopExBagResult( int  Result )
{
	
	//PopExBagResultENUM
	bool error=false;

	const char * OutPutStr=NULL;
	switch (Result)
	{
	case EM_PopExBagResult_OK:
		DelPartnerToBag(m_mode,m_pos);
		OutPutStr=g_ObjectData->GetString("PARTNERFRAME_STR_REMOVEPARTNEROK");

		SendWorldEvent("PARTNERFRAME_REMOVEPARTNER");	

		break;
	case EM_PopExBagResult_Failed:	
		error=true;
		break;
	}

	if (error)
	{
		OutPutStr=g_ObjectData->GetString("PARTNERFRAME_STR_REMOVEPARTNERFAIL");
		lua_State* L = GetLuaState();
		lua_pushstring(L, OutPutStr);
		lua_setglobal(L, "arg1");
		SendWorldEvent("PARTNERFRAME_ERROR");	

		

	}
	if (OutPutStr)

	g_pChatFrame->SendMsgEvent(CHAT_MSG_SYSTEM, OutPutStr, "" );
}

void  CUI_PartnerFrame::UseExBagResult( bool Result )
{

	const char * OutPutStr=NULL;
	if (Result)
	{
		///OutPutStr=g_ObjectData->GetString("PARTNERFRAME_STR_CALLPARTNEROK");
		
	}
	else
	{
		OutPutStr=g_ObjectData->GetString("PARTNERFRAME_STR_CALLPARTNERFAIL");
		lua_State* L = GetLuaState();
		lua_pushstring(L, OutPutStr);
		lua_setglobal(L, "arg1");
		SendWorldEvent("PARTNERFRAME_ERROR");	
	}
	if (OutPutStr)

		g_pChatFrame->SendMsgEvent(CHAT_MSG_SYSTEM, OutPutStr, "" );

}
void  CUI_PartnerFrame::PutInExBagResult(int Result)
{
	const char * Error="UNKNOWN";

	if (g_pItemClipboard)
	g_pItemClipboard->Clear();
	switch (Result)
	{
		case EM_PutInExBagResult_OK		:
			{
				AddPartnerToBag(m_mode,m_id,m_color);
				SendWorldEvent("PARTNERFRAME_ADDPARTNER");	

			}
			Error=g_ObjectData->GetString("PARTNERFRAME_STR_OK");
			break;
		case EM_PutInExBagResult_DataErr	://,
			Error=g_ObjectData->GetString("PARTNERFRAME_STR_DATAERR");

			break;
		case EM_PutInExBagResult_Exist	://,
			Error=g_ObjectData->GetString("PARTNERFRAME_STR_EXIST");
			break;
		case EM_PutInExBagResult_Full	://,
			Error=g_ObjectData->GetString("PARTNERFRAME_STR_FULL");

			break;
	}


	if (Result!=EM_PutInExBagResult_OK)
	{


		lua_State* L = GetLuaState();
		lua_pushstring(L, Error);
		lua_setglobal(L, "arg1");
		SendWorldEvent("PARTNERFRAME_ERROR");	
		//	g_pGameMain->SendWarningMsg(Error);
	}

	g_pChatFrame->SendMsgEvent(CHAT_MSG_SYSTEM, Error, "" );

	
}
void CUI_PartnerFrame::RemovePartner(int mode,int index)
{
	RoleDataEx *Role=CNetGlobal:: RoleData();
	bool bOK=false;

	// 搜尋背包空位
	int destPos = Role->SearchBodyFirstFit(NULL, false);		// 找尋一個可以插入背包的位置

	if( destPos == -1 )
	{
		g_pGameMain->SendSystemMsg(g_ObjectData->GetString( "SYS_BAG_ENOUGH_CAPACITY" ));
		g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString( "SYS_BAG_ENOUGH_CAPACITY" ));
		return ;
	}

	switch(mode)
	{
	case EM_ExBagType_Pet:
		if (index>=0&& index<Role->PlayerBaseData->PetExBag.Size())
		{
			//g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString( "PARTNERFRAME_STR_CANTREMOVEPET" ));
			bOK=true;
		}
		break;
	case EM_ExBagType_Horse:
		if (index>=0&& index<Role->PlayerBaseData->HorseExBag.Size())
			bOK=true;	
		break;
	}


	
	if (bOK)
	{
		m_mode=mode;
		m_pos=index;

		NetCli_Item::SL_PopExBagRequest((ExBagTypeENUM)mode,index,destPos);
	}
}

int  CUI_PartnerFrame::AddPartner(int index)
{
	lua_State* L = GetLuaStateWorld();

	GameObjDbStructEx* itemDB=getItemDBPartner(index);
	bool bOk=false;

	if (itemDB==NULL)
	{
		SendWorldEvent("ADDPARTNER_ERRORTYPE");	
	}
	else
	{
		ItemFieldStruct* item = CNetGlobal::RoleData()->GetBodyItem(index);
		NetCli_Item::SL_PutInExBagRequest(itemDB->Item.ExBagType,item->OrgObjID,index);
		

		m_mode=itemDB->Item.ExBagType;
		m_id=item->OrgObjID;


		if (m_mode==EM_ExBagType_Horse)
		{
			for (int i=0;i<4;i++)
				m_color[i]=	item->HorseColor[i];

		}

	
	}

	return 0;

}
int CUI_PartnerFrame::CallPartner(int mode,int index)
{
	if (mode==EM_ExBagType_Horse)//跑施法動作
		g_pGameMain->ItemCastingStart(ITEM_CASTING_PARTNER,mode,index,0);

	else if (mode==EM_ExBagType_Pet)//寵物不用施法條

	_CallPartner(mode,index);
	
	return 0;
}

void CUI_PartnerFrame::_CallPartner(int mode,int index)
{

	RoleDataEx *Role=CNetGlobal:: RoleData();
	bool bOK=false;
	switch (mode)
	{
	case EM_ExBagType_Pet:
		if (index>=0&& index<Role->PlayerBaseData->PetExBag.Size())
				bOK=true;
		break;
	case EM_ExBagType_Horse:
		if (index>=0&& index<Role->PlayerBaseData->HorseExBag.Size())
			bOK=true;	
		break;
	}


	if (bOK)
	NetCli_Item::SL_UseExBagRequest((ExBagTypeENUM)mode,index);


}
int CUI_PartnerFrame::GetPartnerCount(int mode)
{

	RoleDataEx *Role=CNetGlobal:: RoleData();
	int count=0;
	if (mode==EM_ExBagType_Pet)
	{

		count=Role->PlayerBaseData->PetExBag.Size();
	}
	else
	{

		count=Role->PlayerBaseData->HorseExBag.Size();

	}
	return count;

}


int	LuaFunc_PartnerFrame_GetPartnerCount( lua_State *L )
{

		int mode		= luaL_checkint(L, 1) ;

		int count=0;
		if (g_pPartnerFrame)
			count= g_pPartnerFrame->GetPartnerCount(mode);

		lua_pushinteger(L, count);			

		return 1;
}
int	 LuaFunc_PartnerFrame_GetPartnerInfo( lua_State *L )
{

	int mode		= luaL_checkint(L, 1) ;
	int index		= luaL_checkint(L, 2) -1;


	if (g_pPartnerFrame)
		return g_pPartnerFrame->GetPartnerInfo(mode,index);

	return 0;
}

int LuaFunc_PartnerFrame_CallPartner( lua_State *L )
{


	int mode		= luaL_checkint(L, 1) ;
	int index		= luaL_checkint(L, 2) -1;
	if (g_pPartnerFrame)
		return g_pPartnerFrame->CallPartner(mode,index);


return 0;
}
int LuaFunc_PartnerFrame_CanAddToPartner( lua_State *L )
{
	int pos		= luaL_checkint(L, 1) -1;

	if (g_pPartnerFrame)
		return g_pPartnerFrame->CanAddToPartner(pos);





	return 0;
}
int	LuaFunc_PartnerFrame_PickPartner( lua_State *L )
{
	int mode		= luaL_checkint(L, 1) ;
	int index		= luaL_checkint(L, 2) -1;
	if (g_pPartnerFrame)
		 g_pPartnerFrame->PickupAction(mode,index);

	return 0;
}
int LuaFunc_PartnerFrame_AddPetCollection( lua_State *L )
{
	int pos		= luaL_checkint(L, 1) -1;

	NetCli_CultivatePet::SL_RegPetCard( pos );
return 0;
}
int LuaFunc_PartnerFrame_RemovePartner( lua_State *L )
{


	int mode	= luaL_checkint(L, 1);
	int pos		= luaL_checkint(L, 2) -1;

	if (g_pPartnerFrame)
		 g_pPartnerFrame->RemovePartner(mode,pos);





	return 0;
}
int LuaFunc_PartnerFrame_AddPartner( lua_State *L )
{

	
	int pos		= luaL_checkint(L, 1) -1;

	if (g_pPartnerFrame)
		return g_pPartnerFrame->AddPartner(pos);


	


return 0;
}

int LuaFunc_PartnerFrame_GetPartnerBuff( lua_State *L )
{
	int mode		= luaL_checkint(L, 1) ;
	int index		= luaL_checkint(L, 2) -1;
	if (g_pPartnerFrame)
		return g_pPartnerFrame->GetPartnerBuff(mode,index);

return 0;

}
int LuaFunc_PartnerFrame_IsCanAddPetColletcion( lua_State *L )
{

	int dbid		= luaL_checkint(L, 1) ;
	bool bShow=false;//寵物星座圖鑑

	int index;

	bool bAdd=true;
	

	bShow=IsPetCollection(dbid,index,bAdd);


	lua_pushboolean(L, bShow);//寵物星座圖鑑		
	lua_pushboolean(L, bAdd);//寵物星座圖鑑		
	return 2;

}









