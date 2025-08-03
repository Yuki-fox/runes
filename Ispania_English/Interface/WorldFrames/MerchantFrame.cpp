#include "../../netwaker_member/NetWakerClientInc.h"
#include "RoleData/ObjectDataClass.h"
#include "../mainproc/GameMain.h"
#include "MerchantFrame.h"
#include "ChatFrame.h"
#include "../../tools/SmallObj/SmallObj.h"
#include "../../tools/BaseFunction/BaseFunction.h"
#include "BagFrame.h"
#include "../ItemLink/ItemLink.h"
#include "GuildFrame.h"
#include "..\socal\UI_Socal.h"

#include "ItemUTIL.h"
#include "..\..\ui\core\Luatable.h"
#include "../Login/AccountLogin.h"
#include <ShellAPI.h>

#define SELL_TYPE_MAX 6
CMerchantFrame*				g_pMerchantFrame = NULL;

static time_t _ServerTimeToUTC(time_t uTime)
{
	time_t ret=uTime-g_pGameMain->GetServerTimeZone()*60*60;
	return ret;
}
/*
static time_t _get_localTimeX(time_t uTime)
{

	

	struct tm *gmt;  
	gmt = localtime( &ret )	;	

	time_t nowTime;
	time(&nowTime);

	char *str=TimeStr::DateTimeStr_UTC(ret);
	char *str1=TimeStr::DateTimeStr(uTime);


	 str=TimeStr::DateTimeStr_UTC(nowTime);
	str1=TimeStr::DateTimeStr(nowTime);
	//char str[80];  
	 //strftime(str, 80,"20%y:%m:%d:%H:%M:%s",gmt);  

	return ret;
}*/
struct GameBarHistoryLog
{
	int cost;//花費
	int date;//日其
	int count;//數量
	int dbid;// 在資料庫點數商品索引值
};
static time_t _get_NowTime()
{
	time_t time=g_pGameMain->GetGameTime() ;//+ g_pGameMain->GetServerTimeZone()*60*60;
	return time;
}

int LuaFunc_GetMerchantItemInfo(lua_State* L);
int LuaFunc_GetMerchantItemDetail(lua_State* L);
int LuaFunc_GetMerchantItemNums(lua_State* L);
int LuaFunc_GetMerchantFilterNums(lua_State* L);
int LuaFunc_GetMerchantFilterInfo(lua_State* L);
int LuaFunc_SetMerchantFilterType(lua_State* L); 
int LuaFunc_GetMerchantFilterType(lua_State* L);

int LuaFunc_MF_ShoppingBuy(lua_State* L);

int LuaFunc_GameBarClose(lua_State* L);

//要求兌換物品
int LuaFunc_ItemExchangeRequest(lua_State* L);

//打開界面 向Server要ItemMall Info
int LuaFunc_ItemMallFrame_Open(lua_State* L);

int LuaFunc_GC_RuneReload(lua_State *L) ;

//打開新界面2 向Server要ItemMall Info
int LuaFunc_CIMF_OpenMall2(lua_State* L);
int LuaFunc_CIMF_CloseMall2(lua_State* L);
int LuaFunc_CIMF_GetNums(lua_State* L);
int LuaFunc_CIMF_GetItemInfo(lua_State* L);
int LuaFunc_CIMF_GetTopItem(lua_State* L);
int LuaFunc_CIMF_GetFilterNums(lua_State* L);
int LuaFunc_CIMF_GetFilterInfo(lua_State* L);
int LuaFunc_CIMF_SelectType(lua_State* L);
int LuaFunc_CIMF_SelectFilterIndex(lua_State* L);
int LuaFunc_CIMF_ShoppingBuy(lua_State* L);
int LuaFunc_CIMF_MailGift(lua_State* L);
int LuaFunc_CIMF_GetListCount(lua_State* L);
int LuaFunc_CIMF_GetListName(lua_State* L);
int LuaFunc_CIMF_GetMessage(lua_State* L);
int LuaFunc_CIMF_GetMessageCount(lua_State* L);
int LuaFunc_CIMF_ClearHistory(lua_State* L);
int LuaFunc_CIMF_SearchItem(lua_State* L);
int LuaFunc_CIMF_SetHistory(lua_State* L);

//ItemLink
int LuaFunc_GetMerchantItemLink(lua_State* L);
int LuaFunc_GetItemMallLink(lua_State* L);
int LuaFunc_IsEnableMerchant(lua_State* L);

//GameBar
int LuaFunc_IsEnableGameBar(lua_State* L);
int LuaFunc_GetGameBarFreeCount(lua_State* L);
int LuaFunc_GetGameBarItemInfo(lua_State* L);
int LuaFunc_GetGameBarStart(lua_State* L);
int LuaFunc_GetGameBarPlayCost(lua_State* L);
int LuaFunc_GameBar_Stoped(lua_State* L);
int LuaFunc_GameBar_GetHistory(lua_State* L);
int LuaFunc_GameBar_GetHistorys(lua_State* L);
int LuaFunc_GameBar_CanPlay(lua_State* L);

//topup page
int LuaFunc_OpenTopupPage(lua_State* L);

int LuaFunc_ItemMallHelpContent(lua_State* L);

//zshop
int LuaFunc_IsEnableZShop(lua_State* L);
int LuaFunc_SetOnTimeWebBagRequest(lua_State* L);
int LuaFunc_OpenZShop(lua_State* L);
int LuaFunc_GetItemLink(lua_State* L);

#define DF_ITEMTYPE_COOLDOWN (60*5*1000)//隔 5分鐘

struct ItemMallInfo
{
	vector<DepartmentStoreBaseInfoStruct*> m_ItemMallInfoList;
};

struct IM_FListItem
{
	int type;
	int state;

	string m_name;

	IM_FListItem( ):type(-1),state(-1)
	{

	}

};

struct SellType2
{
	int m_type;
	
	bool m_Open;
	vector<int> m_items;

	vector<SellType2*> m_child;	

	SellType2():m_type(-1),m_Open(false){}

	virtual ~SellType2()
	{
		
	}
	void initial(int type)
	{
		m_type=type;
		m_Open=false;

	}

	SellType2(int type):m_type(type),m_Open(false)
	{


	}
	/*
	SellType2* getType(DWORD type)
	{
		SellType2* ret=find(type);
		if (!ret)
		{
			SellType2* value =new SellType2(type);
			


			m_child.push_back(value);
			ret=value;
			
		}
		return ret;
	}*/
	SellType2* find(DWORD type)
	{

		SellType2* ret=NULL;

		if (type==m_type)
			return this;


		for (int  j=0;j<m_child.size();j++)
		{

			ret=m_child[j]->find(type);
			if (ret)
				break;

		}



		return ret;


	}
	int getItem(int index)
	{

		int ret=-1;
		if (index>=0&&index<getNums())
		{

			ret=m_items[index];
		}

		return ret;


	}
	void Close(void)
	{
		m_Open=false;
		for (int  j=0;j<m_child.size();j++)
		{

			SellType2* ret=m_child[j];
			if (ret)ret->Close();
		
			
		}
	
		

	}

	void addItem( int item)
	{
		for (int i=0;i<m_items.size();i++)
		{
			if (m_items[i]==item)
			return;
		}
		m_items.push_back(item);
	}

	void addMenu(vector<DWORD>&menulist)
	{

		menulist.push_back(m_type);
		if (m_Open)
		{
			for (int  j=0;j<m_child.size();j++)
			{

				if (m_child[j]->m_items.size())
				m_child[j]->addMenu(menulist);

			}

		}


	}
	void clearChild()
	{
		for (int  j=0;j<m_child.size();j++)
		{

			SellType2* ret=m_child[j];
			
			if (ret)
			{
				ret->clearChild();
			//	delete ret;
			}
			//m_child[j]=NULL;
		}
		m_child.clear();
	}
	void finish()
	{
		
	

		m_items.clear();

	}
	size_t getNums()
	{

	
		return m_items.size();
	}

};
struct SellTypeTab 
{


	bool m_initial;
	
	map<DWORD,SellType2> m_typeMaps;
	
	
	SellTypeTab():m_initial(false)
	{

	}
	void clearChild()
	{

		map<DWORD,SellType2>::iterator it=m_typeMaps.begin();

		for (;it!=m_typeMaps.end();++it)
		{

			SellType2& sp=it->second;
			sp.clearChild();

		}
	}

	~SellTypeTab()
	{
		finish();
		m_typeMaps.clear();
	}

	void Close(void)
	{
		map<DWORD,SellType2>::iterator it=m_typeMaps.begin();

		for (;it!=m_typeMaps.end();++it)
		{

			SellType2& sp=it->second;
			sp.Close();

		}

	}


	void finish()
	{
		//m_typeMaps.clear();


		map<DWORD,SellType2>::iterator it=m_typeMaps.begin();
		
		for (;it!=m_typeMaps.end();++it)
		{

			SellType2& sp=it->second;
			sp.finish();
		
		}

	}
	

	void Initial()
	{
		finish();
		m_initial=true;
	}


	SellType2* getType(int type)
	{

		SellType2* ret=NULL;
		if (m_typeMaps.find(type)==m_typeMaps.end())
		{	
			SellType2 value(type);


			m_typeMaps[type]=value;

		}

			ret=&m_typeMaps[type];

		return ret;
	}
	
	int getNums(int type)
	{
		int count=0;
		if (m_typeMaps.find(type)!=m_typeMaps.end())
		{
			count=(int)m_typeMaps[type].getNums();


		}

		return count;
	}
	void addMenu(vector<DWORD>&menulist)
	{
		map<DWORD,SellType2>::iterator it=m_typeMaps.begin();

		//SellType2* pren=NULL;
		for (;it!=m_typeMaps.end();++it)
		{

			SellType2& sp=it->second;
			int s1,s2,s3;

			s1=(sp.m_type>>16)&0xff;
			s2=(sp.m_type>>8)&0xff;
			s3=sp.m_type&0xff;
			if (s2==0&&s3==0&&s1!=0xff&&sp.m_items.size())
			{
				//if (sp.m_Open)
					sp.addMenu(menulist);
			
			}
			




		

		}



	}
	void addItem(int type, DepartmentStoreBaseInfoStruct& item)
	{
		int s1,s2,s3;

		s1=(type>>16)&0xff;
		s2=(type>>8)&0xff;
		s3=type&0xff;

		SellType2 *sp=NULL;
		if (type==-1)
		{
			sp=	getType(type&0xff0000);

			sp->addItem(item.GUID);
		}
		else
		{

		sp=getType(type);
		

		SellType2 *parent=NULL;
		
				

		if (s3>0)
		{
			addChild(type&0xffff00,sp);
			parent=	getType(type&0xffff00);
			if (parent)
			{

				addChild(type&0xff0000,parent);		
				parent->addItem(item.GUID);
				addChild(type&0xff0000,parent);
				parent=	getType(type&0xff0000);
				parent->addItem(item.GUID);

			}
		}
		else if (s2>0&& s3==0)
		{
			
			addChild(type&0xff0000,sp);

			parent=	getType(type&0xff0000);
		
			parent->addItem(item.GUID);
		}
		
		sp->addItem(item.GUID);
		}
	}

	void	addChild(int type ,SellType2 *child)
	{


		SellType2 *sp=	getType(type);


		if (sp!=child)
		{

			int i;

			for ( i =0 ; i< sp->m_child.size() ;i++)
			{
				if (sp->m_child[i]->m_type==child->m_type)
					return ;

			}



			sp->m_child.push_back(child);
		}


		
	}

	int getItem(int type,int index)
	{

		return	m_typeMaps[type].getItem(index);

	}


};

struct SellType
{

	int m_type;//物品種類
	bool m_Open;
	int m_count;
	int *m_time;
	vector<SellType> m_child;	
	SellType():m_count(0),m_type(0),m_Open(false),m_time(NULL)
	{

	}
	SellType* find(DWORD type)
	{
		SellType* ret=NULL;
		if (type==m_type)
			return this;


		for (int  j=0;j<m_child.size();j++)
		{

			ret=m_child[j].find(m_type);
			if (ret)
				break;

		}

		return ret;



	}
	void addMenu(vector<DWORD>&menulist)
	{

		menulist.push_back(m_type);
		if (m_Open)
		{
			for (int  j=0;j<m_child.size();j++)
			{

				m_child[j].addMenu(menulist);
				
			}

		}


	}

};



class _ItemMallFrame
{

	void ShopInfoRequest(DWORD ItemType);
	void ItemTypeRequest(void);
	void OpenItemInfo(void);
	ItemMallInfo* AddNewTypeInfo(DWORD type);

public:
	_ItemMallFrame(CInterfaceBase *iface);
	~_ItemMallFrame();
void RuneReloadClick(bool bclick);


	bool Open(bool all=false);
	void Close();
	
	void ShopInfo(int count ,DepartmentStoreBaseInfoStruct* itemList);
	void ItemType(int count ,int * itemTyperList);


	void BuildFrames(void);
	void DestroyFrames(void);
	bool findType(SellType* ret,DWORD type);
	void UpDateItem(void);
	void SetFilterType(int type);
	int GetItemTypeNums(void);
	int GetItemType(int index,int& s1,int& s2,int& s3);
	void				Unlocked()								{ m_locked = false; }
	
	ItemMallInfo* GetItemMallTable(void)
	{
		return GetInfo(m_NowType);
	}
	SellType* GetType(DWORD  type);
	ItemMallInfo* GetInfo(DWORD type);


	DepartmentStoreBaseInfoStruct*  GetItemInfo(int index,int GUID);


	void SearchItem(const char *keyword);

	void ShoppingBuy(int index,const char *password);
	void ShoppingBuyByGuid(int Guid,int count,const char *password);
	void _ShoppingBuyByGuid();

	
	void BuyItemResult(int Result);
	void CalNewMenu(void);
	void BuyOk(void);
	void BuyFail(int errorcode);
	void MailGiftResult			( int Result );
	void RunningMessage(vector<string>& MsgList);

	void ImportAccountMoneyResult(int Runes);

	void MailGift( int ItemGUID ,  const char* Password , int BackGroundType 
		, const char* TargetName , const char* Title , const char* Content );

	void CalAllList(void);
	int AddMember(int type,int state,const char *name,bool force=false);

	void SelectType(int type);

	void SetHistory(int guid);

	void calAllItemMall(void);

	int IM2_GetNums(void);
	int IM2_GetItemType(int index,int& s1,int& s2,int &s3);
	int IM2_GetFilterNums(void);
	void IM2_CalNewMenu(void);
	void IM2_SetSelcetTypeID(int id,int state);
	void RefList(void);

	int GetMessageCount();
	const char * GetMessage(int index);
	int GetListCount();
	const char * GetListName(int index);
	int IM2_GetTopItem(int index);
	
	SellType2* IM2_GetType(int index);
	void PushTopList(DepartmentStoreBaseInfoStruct* pitem);
	void ClearHistory(void);
	void setDisableSendGift(bool IsDisableSendGift );


	int SlotMachinePlayResult( int itemID,int itemCount,int date,int cost);
	void SlotMachineInfoChange();
	void SlotMachineInfo( int itemID[] , int itemCount[] );
	void GetMachineInfo();

	vector <GameBarHistoryLog> m_gameBarHistoryLogs;


	GameBarHistoryLog m_newlog;
	void AddGameBarLogs();

	int m_BarItemID[20] ;
	int m_BarItemCount[20];
	int m_ResultItem;

private:
	int m_nowTime;

	char m_password[512];
	CInterfaceBase *m_base;
	int m_NowType;
	int m_GuidItem;
	bool						m_IsDisableSendGift;

	DepartmentStoreBaseInfoStruct* m_item;
	bool						m_butflag;
	vector<DWORD> m_ItemTypeList;
	
	vector<SellType> m_SellTypeList;

	vector<int>  m_datelive;  

	map<int,DepartmentStoreBaseInfoStruct> m_ItemMaps;

	vector<DepartmentStoreBaseInfoStruct*> m_TopList;

	vector<IM_FListItem> m_lists;
	vector<SellTypeTab> m_SellTypeList2;
	vector<string> m_Messages;


	

	bool m_all;
	int m_SelectType;
	int m_SelcetTypeID;
	int m_BKSelcetTypeID;
	int m_BKOpen;

	int m_BuyMax;
	int m_BuyIndex;


	int m_forceUpdateType;
	int m_lockBag;

	void Clear(void);
	map<DWORD,ItemMallInfo> m_AllTypeInfo;
	int m_index;

	bool						m_locked;
};

_ItemMallFrame*  g_pItemMallFrame;
_ItemMallFrame::_ItemMallFrame(CInterfaceBase *iface):m_base(iface),m_butflag(false),m_index(0)
{
	m_forceUpdateType=-1;
	m_BKSelcetTypeID=-1;
	m_nowTime=-1;
	m_NowType=-1;
	m_locked=false;
	m_lockBag=-1;
	m_SelectType=0;
	m_BKOpen=false;
	m_IsDisableSendGift=true;

	m_ResultItem = 0;
	
}
const char * _ItemMallFrame::GetListName(int index)
{

const char *name=0;


	if (index>=0&&index<m_lists.size())
		name= m_lists[index].m_name.c_str();

return name;

}
int _ItemMallFrame::GetListCount()

{
//	RefList();
	return (int)m_lists.size();
}
void _ItemMallFrame::setDisableSendGift(bool IsDisableSendGift )
{
	m_IsDisableSendGift=IsDisableSendGift;
}

void _ItemMallFrame::ClearHistory(void)
{
	if (m_SellTypeList2.size()>=6)
	m_SellTypeList2[5].Initial();
}


void _ItemMallFrame::SetHistory(int GUID)
{
	map<int ,DepartmentStoreBaseInfoStruct>::iterator it=m_ItemMaps.find(GUID);

	if  (it!=m_ItemMaps.end())
	{
		DepartmentStoreBaseInfoStruct& item=it->second;
		GameObjDbStructEx*	OrgDB;
		OrgDB =RoleDataEx:: St_GetObjDB(item.Item.OrgObjID );
		if (OrgDB)
		{
			int s1=item.SellType[0]&0xff;
			int s2=item.SellType[1]&0xff;
			int s3=item.SellType[2]&0xff;
			int Dtype=(s1<<16)|(s2<<8)|s3;
			if (m_SellTypeList2.size()>=6)
			m_SellTypeList2[5].addItem(Dtype,item);
			
		}

	}
}
void CMerchantFrame::AddGameBarLogs()
{


	m_bDirty=true;

	g_pItemMallFrame->AddGameBarLogs();
}

void _ItemMallFrame::AddGameBarLogs()
{


	int count=(int)m_gameBarHistoryLogs.size();

	

	if (count>=100)
	{

		for (int i=99;i>0;--i)
		{
			m_gameBarHistoryLogs[i]=m_gameBarHistoryLogs[i-1];

		}


		m_gameBarHistoryLogs[0]=m_newlog;

	}
	else
	{
		m_gameBarHistoryLogs.insert(m_gameBarHistoryLogs.begin(),m_newlog);

	}

}
int  _ItemMallFrame::SlotMachinePlayResult(  int itemID , int itemCount ,int date,int cost)
{

	int index=0;
	for (int i=0;i<10;i++)
	{
		if (m_BarItemID[i]==itemID)
		{
			index=i;
			break;
		}
	}

	m_newlog.cost=cost;
	m_newlog.dbid=itemID;
	m_newlog.date=date;
	m_newlog.count=itemCount;


	g_pMerchantFrame->AddGameBarLogs();
	g_pMerchantFrame->SaveGamebarLog();

	
	return index;
}
void _ItemMallFrame::SlotMachineInfoChange()
{

	NetCli_DepartmentStore::SL_SlotMachineInfoRequest();

}
void _ItemMallFrame::SlotMachineInfo( int itemID[] , int itemCount[] )
{
	int begin = (m_ResultItem != 0) ? 10 : 0;

	for (int i=0;i<10;i++)
	{
		m_BarItemID[i+begin]=itemID[i];
		m_BarItemCount[i+begin]=itemCount[i];
	}
	m_base->SendWorldEvent("GAMEBAR_ITEM_CHANGED");

}

void _ItemMallFrame::GetMachineInfo()
{
	m_ResultItem = 0;
	memcpy(&m_BarItemID[0], &m_BarItemID[10], sizeof(int) * 10);
	memcpy(&m_BarItemCount[0], &m_BarItemCount[10], sizeof(int) * 10);
}

void _ItemMallFrame::SearchItem(const char *keyword)
{

	map<int ,DepartmentStoreBaseInfoStruct>::iterator it=m_ItemMaps.begin();

	if (m_SellTypeList2.size()>=6)
	m_SellTypeList2[5].Initial();
	int count=0;
	char temp1[512];
	strcpy(temp1,keyword);
	strlwr(temp1);
	for (;it!=m_ItemMaps.end();++it)
	{

		DepartmentStoreBaseInfoStruct& item=it->second;

		GameObjDbStructEx*	OrgDB;
		OrgDB =RoleDataEx:: St_GetObjDB(item.Item.OrgObjID );
		if (OrgDB)
		{


			const char*	name=OrgDB->GetName();
			char temp[512];
			strcpy(temp,name);
			strlwr(temp);
			if (strstr(temp,temp1)>0)
			{

				int s1=item.SellType[0]&0xff;
				int s2=item.SellType[1]&0xff;
				int s3=item.SellType[2]&0xff;
				int Dtype=(s1<<16)|(s2<<8)|s3;
				if (m_SellTypeList2.size()>=6)
				m_SellTypeList2[5].addItem(Dtype,item);
				count++;
			}
		}

	}
	if (count==0)
	{

		char temp[512];
		
		const char *pMsg=g_ObjectData->GetString("SYS_ITEM_NOT_FIND");
		sprintf(temp,"%s %s",keyword,pMsg);


		g_pChatFrame->SendMsgEvent(CHAT_MSG_SYSTEM, temp,"" );

		g_pGameMain->SendWarningMsg(temp);	
	}



}

void _ItemMallFrame::BuildFrames(void)
{
	m_NowType=-1;
	m_nowTime=-1;
	m_AllTypeInfo.clear();
	
	m_ItemTypeList.clear();
	m_locked=false;
	m_datelive.clear();
	m_SellTypeList.clear();
	m_TopList.clear();
	
	m_SelectType=0;
	m_lists.clear();


	m_SellTypeList2.resize(SELL_TYPE_MAX );

	m_index=0;

	for (int i=0;i<SELL_TYPE_MAX-1;i++)
	{

		m_SellTypeList2[i].Initial();
	}

}

void _ItemMallFrame::RefList()
{
CalAllList();


m_base->SendWorldEvent("IM_UPDATE_LIST");

}


int _ItemMallFrame::IM2_GetTopItem(int index)
{

	int GUID=0;
	if (index>=0&&index<m_TopList.size())
		GUID=m_TopList[index]->GUID;
	return GUID;

	
}
void _ItemMallFrame::DestroyFrames(void)
{

	for (int i=0;i<m_SellTypeList2.size();i++)
	{

		m_SellTypeList2[i].clearChild();
	}
	m_SellTypeList2.clear();
	

	m_nowTime=-1;
	m_datelive.clear();
	m_AllTypeInfo.clear();
	m_ItemTypeList.clear();
	m_SellTypeList.clear();

}
void _ItemMallFrame::SelectType(int type)
{

	m_SelectType=type;
	IM2_CalNewMenu();


	//

}

int _ItemMallFrame::GetMessageCount()
{

	return (int)m_Messages.size();
}
const char * _ItemMallFrame::GetMessage(int index)
{
	const char *msg=NULL;
	if (index>=0&&index<m_Messages.size())
		msg=m_Messages[index].c_str();



return msg;
}
void _ItemMallFrame::IM2_SetSelcetTypeID(int id,int state)
{
	int old=m_SelcetTypeID;
		m_SelcetTypeID=id;

	




	SellType2* type= g_pItemMallFrame->IM2_GetType(id);
	if (type)
	{
		if (type->m_child.size())
		{
			if (state==-1)
			type->m_Open=!type->m_Open;
			else
				type->m_Open=state==1 ? true:false;

			g_pItemMallFrame->IM2_CalNewMenu();
		}

	}
	if (old!=id)

		m_base->	SendWorldEvent("IMF_ITEM_UPDATE");

}

SellType2* _ItemMallFrame::IM2_GetType(int index)
{

	if (m_SelectType >=0 &&m_SelectType< m_SellTypeList2.size())

		return m_SellTypeList2[m_SelectType].getType(index);

	return NULL;

}
int _ItemMallFrame::IM2_GetItemType(int index,int& s1,int& s2,int &s3)
{

	int ret=-1;
	if (index>=0&&index<(int)m_ItemTypeList.size())
		ret=m_ItemTypeList[index];

	s1=(ret>>16)&0xff;
	s2=(ret>>8)&0xff;
	s3=(ret)&0xff;

	return ret;

}
int _ItemMallFrame:: IM2_GetFilterNums(void)
{

	
	return (int)m_ItemTypeList.size();


}

void _ItemMallFrame:: RuneReloadClick(bool bclick)
{

	m_butflag=bclick;
}
_ItemMallFrame::~_ItemMallFrame()
{
	//g_pItemMallFrame=NULL;
}
void _ItemMallFrame::calAllItemMall(void)
{

	RefList();
	m_SellTypeList2.resize(SELL_TYPE_MAX );

	m_index=0;
	
	for (int i=0;i<SELL_TYPE_MAX-1;i++)
	{

		m_SellTypeList2[i].Initial();
	}


	m_TopList.clear();


	NetCli_DepartmentStore::SL_RunningMessageRequest();

}

DepartmentStoreBaseInfoStruct*  _ItemMallFrame::GetItemInfo(int index,int GUID)
{
	DepartmentStoreBaseInfoStruct*  ret=NULL;
	if (m_SelectType>=0&&m_SelectType<SELL_TYPE_MAX)
	{

		
		
		if (index!=-1)
		{

/*
			ItemMallInfo* ItemretS2=GetInfo(m_SelcetTypeID);
			if (ItemretS2)
			{
				int nums=ItemretS2->m_ItemMallInfoList.size();
				if (index<nums)
				{
					DepartmentStoreBaseInfoStruct& item = *ItemretS2->m_ItemMallInfoList[index];

					GUID=item.GUID;

				}
			}
*/			
			if (m_SelectType>=0&&m_SelectType<m_SellTypeList2.size())

			 GUID=m_SellTypeList2[m_SelectType].getItem(m_SelcetTypeID,index);

		}

			if (GUID>0)
				ret=&m_ItemMaps[GUID];

		




	}


	return ret;



}


int  _ItemMallFrame::IM2_GetNums(void)
{
	int nums=0;
	if (m_SelectType>=0&&m_SelectType<m_SellTypeList2.size())
	{
	

		nums= m_SellTypeList2[m_SelectType].getNums(m_SelcetTypeID);
	}
	return nums;
}


void _ItemMallFrame::ImportAccountMoneyResult(int Runes)
{
	if (m_butflag)
	{


	const char *pMsg=g_ObjectData->GetString("SYS_RUNE_IN_NO");

	char temp[1024];
	if (Runes>0)
	{
		pMsg=g_ObjectData->GetString("SYS_RUNE_IN_OK");
		sprintf(temp,pMsg,Runes);
	}
	else
		sprintf(temp,"%s",pMsg);


	g_pChatFrame->SendMsgEvent(CHAT_MSG_SYSTEM, temp,"" );
	g_pGameMain->SendWarningMsg(temp);		
	m_butflag=false;
	}

}
void  _ItemMallFrame::BuyOk(void)
{
	const char *pMsg=g_ObjectData->GetString("SYS_ACCOUNT_BUY_SUCCEEDED");


	char temp[1024];

	GameObjDbStructEx*	OrgDB;
	OrgDB =RoleDataEx:: St_GetObjDB(m_item->Item.OrgObjID);

	m_BuyIndex++;
	if (OrgDB)

		sprintf(temp,"%d: %s (%s)",m_BuyIndex,pMsg,OrgDB->GetName() );



	g_pChatFrame->SendMsgEvent(CHAT_MSG_SYSTEM, temp,"" );
	

	if (m_BuyIndex<(m_BuyMax))
		_ShoppingBuyByGuid();


	

}


void  _ItemMallFrame::BuyFail(int errorcode)
{

	const char *pMsg=g_ObjectData->GetString("SYS_ACCOUNT_ERR_3");	

	switch (errorcode)
	{
	case EM_DepartmentStoreBuyItemResult_SellOut	:	//賣完了
		pMsg=g_ObjectData->GetString("SYS_ACCOUNT_ERR_5");	
		break;
	case EM_DepartmentStoreBuyItemResult_MoneyErr:	//金錢不足
		pMsg=g_ObjectData->GetString("SYS_ACCOUNT_ERR_1");	
		break;
	case EM_DepartmentStoreBuyItemResult_PacketErr:	//包裹滿了
		pMsg=g_ObjectData->GetString("SYS_ACCOUNT_ERR_2");	
		break;
	case EM_DepartmentStoreBuyItemResult_Err	:		//其它錯誤
		pMsg=g_ObjectData->GetString("SYS_ACCOUNT_ERR_3");	
		break;
	case EM_DepartmentStoreBuyItemResult_TimeOut	:		//其它錯誤
		pMsg=g_ObjectData->GetString("SYS_ACCOUNT_ERR_6");	
		break;
	}



	g_pGameMain->SendWarningMsg(pMsg);			// 遊戲商城購買失敗
	


	
}
void _ItemMallFrame::BuyItemResult(int Result)
{
	bool ok=false;
	int id=-1;
	bool GetNewData=true;

	//m_locked=false;
	if (g_pBagFrame)
		g_pBagFrame->Unlocked(m_lockBag);

	switch (Result)
	{

	case EM_DepartmentStoreBuyItemResult_OK	:		//成

		if (m_item)
		{

			GameObjDbStructEx*	OrgDB;
			OrgDB =RoleDataEx:: St_GetObjDB(m_item->Item.OrgObjID);
			if (OrgDB)

		
			id =OrgDB->GUID;

			m_item->Sell.Count++;
		}

		ok=true;
		GetNewData=false;

		BuyOk();
		break;
	case EM_DepartmentStoreBuyItemResult_SellOut	:	//賣完了
			if (m_item)
			{
				
			m_item->Sell.Count=0;
				m_item->Sell.OrgCount=m_item->Sell.MaxCount;

			}
	case EM_DepartmentStoreBuyItemResult_MoneyErr:	//金錢不足
	case EM_DepartmentStoreBuyItemResult_PacketErr:	//包裹滿了
	case EM_DepartmentStoreBuyItemResult_Err	:		//其它錯誤
	case EM_DepartmentStoreBuyItemResult_TimeOut	:		//其它錯誤
		BuyFail(Result);
		m_BuyMax=0;
		m_BuyIndex=0;


		
	break;

	case EM_DepartmentStoreBuyItemResult_PasswordErr:

		{
		const char *	text = g_ObjectData->GetString("SYS_LOGIN_PASSWORLD_FAIL");


		g_pGameMain->SendWarningMsg(text);	
		g_pChatFrame->AddMessage(5, text);
		
		}
		break;


	}

	lua_State* L =m_base-> GetLuaState();


	lua_pushboolean(L, ok);

	lua_setglobal(L, "arg1");
	lua_pushnumber(L, id);

	lua_setglobal(L, "arg2");
	m_base->	SendWorldEvent("ITEMMALL_ITEM_OK");

	if (GetNewData)
	{
		m_nowTime=-1;
		//	Open(1);
		m_all=true;
		calAllItemMall();

		m_index=0;

		if (m_index>=0&&m_index<m_SellTypeList.size())
		{
			m_NowType=m_SellTypeList[m_index].m_type;
			ShopInfoRequest((m_NowType>>16)&0xff);
		}
		m_base->	SendWorldEvent("ITEMMALL_LOADING");
	}



	

}






// 找到第一個可堆疊的地方 商城背包
// 
int	 Item_SearchFirstFit(DepartmentStoreBaseInfoStruct& Ditem)
{

	bool isHeap=true;
 int nowCount=Ditem.Item.Count;
	// 計次物品不能堆疊
	if ( Ditem.Item.Mode.HideCount )
		isHeap = false;

	int maxBagCount = _MAX_BODY_EX_COUNT_;//商城包包數量
	if (  isHeap )
	{
		if (nowCount == 0 )
			return -1;

		GameObjDbStructEx*	OrgDB;
		OrgDB =RoleDataEx:: St_GetObjDB(Ditem.Item.OrgObjID );
		if( OrgDB == NULL )
			return -1;

		for ( int i = 0; i < maxBagCount; i++ )
		{
			ItemFieldStruct* item = CNetGlobal::RoleData()->GetBodyItem(i);

		
			if( item->OrgObjID != OrgDB->GUID || item->Pos != EM_ItemState_NONE )
				continue;

			if(item->Count + max(nowCount, 1) <= max(OrgDB->MaxHeap, 1) )
				return i;
		}
	}

	for( int i = 0; i < maxBagCount; i++ )
	{
		ItemFieldStruct* item = CNetGlobal::RoleData()->GetBodyItem(i);

		
		if ( item->Pos != EM_ItemState_NONE)
			continue;

		if( item->IsEmpty())
			return i;
	}

	return -1;
}

void _ItemMallFrame::ShoppingBuyByGuid(int Guid,int count,const char *password)
{


	strcpy(m_password,password);

	m_BuyMax=count;
	if (m_BuyMax<=0)
		m_BuyMax=1;
	if (m_BuyMax>10)
		m_BuyMax=10;

	m_BuyIndex=0;
	m_GuidItem=Guid;

	_ShoppingBuyByGuid();
}

void _ItemMallFrame::_ShoppingBuyByGuid()
{

	
	DepartmentStoreBaseInfoStruct* item =  g_pItemMallFrame->GetItemInfo(-1,m_GuidItem);	

	if (item)
	{

		//int AlltotalMoney = 0;
		int totalMoney =0;
		int totalBonus =0;
		int totalFree=0;


		int bagpos=-1;
		int total_stock=item->Item.Count*1;				// 單次堆疊數
		//if (stock >Obj->MaxHeap)
		//	stock=Obj->MaxHeap;

		totalMoney=item->Sell.Cost*1;
		totalBonus=item->Sell.Cost_Bonus*1;

		totalFree=item->Sell.Cost_Free*1;

		// 搜尋商城背包空位
		bagpos=	Item_SearchFirstFit(*item);


		if (bagpos!=-1)
		{



			GameObjDbStructEx* Obj = g_ObjectData->GetObj(item->Item.OrgObjID);

			int maxHeap=Obj->MaxHeap;


			// 檢查金錢是否足夠
			if ( totalMoney > CNetGlobal::RoleData()->BodyMoney_Account() 
				|| totalBonus > CNetGlobal::RoleData()->PlayerBaseData->Money_Bonus
				|| totalFree > g_pBagFrame->GetBilldinCount()
				)
			{
				if (totalMoney > CNetGlobal::RoleData()->BodyMoney_Account() )
				g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_ACCOUNT_ERR1_1"));			// 金錢不足
				if (totalBonus > CNetGlobal::RoleData()->PlayerBaseData->Money_Bonus )
					g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_ACCOUNT_ERR1_2"));			// 金錢不足
				if (totalFree > g_pBagFrame->GetBilldinCount() )
					g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_ACCOUNT_ERR1_3"));			// 金錢不足
				return;
			}

			m_lockBag=bagpos;

			if (g_pBagFrame)
				g_pBagFrame->Locked(bagpos);


			m_item=item;
			//	m_locked=true;
			
			m_BKSelcetTypeID=m_SelcetTypeID;
			SellType2 *ret= IM2_GetType(m_SelcetTypeID);
			m_BKOpen=ret->m_Open;

			

			

			int stock=total_stock;
			if (stock>maxHeap)
				stock=maxHeap;

			NetCli_DepartmentStore::SL_BuyItem( item->GUID, bagpos, stock,m_password);

			
		}
		else
		{

			lua_State* L =m_base-> GetLuaState();

			lua_pushboolean(L, false);

			lua_setglobal(L, "arg1");
			lua_pushnumber(L, -1);

			lua_setglobal(L, "arg2");

			m_base->	SendWorldEvent("ITEMMALL_ITEM_OK");

			char temp[512];

			sprintf(temp,g_ObjectData->GetString("IMF_BAGFULL_ITEMFAIL"),(m_BuyMax-m_BuyIndex));

			g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_ITEMMALL_BAGFULL"));			//商城 背包已滿IMF_BAGFULL_ITEMFAILL
			


			g_pChatFrame->SendMsgEvent(CHAT_MSG_SYSTEM, temp,"" );

			//return;


		}
	}
	
}



void _ItemMallFrame::ShoppingBuy(int index, const char *password)
{

	//if (m_locked) 
	//	return ;

	ItemMallInfo *pInfoTable= GetItemMallTable();
	int count=(int)pInfoTable->m_ItemMallInfoList.size();


	if ( index >= 0 && index < count )
	{
		DepartmentStoreBaseInfoStruct& item =* pInfoTable->m_ItemMallInfoList[index];
		
		int AlltotalMoney = 0;
		int totalMoney =0;
		int totalBonus =0;
	

		int bagpos=-1;
		int stock=item.Item.Count;				// 單次堆疊數
		//if (stock >Obj->MaxHeap)
		//	stock=Obj->MaxHeap;

		totalMoney=item.Sell.Cost;
		totalBonus=item.Sell.Cost_Bonus;

		// 搜尋商城背包空位
	bagpos=	Item_SearchFirstFit(item);

		
		if (bagpos!=-1)
		{
			

			
			GameObjDbStructEx* Obj = g_ObjectData->GetObj(item.Item.OrgObjID);


			// 檢查金錢是否足夠
			if ( totalMoney > CNetGlobal::RoleData()->BodyMoney_Account() && totalBonus > CNetGlobal::RoleData()->PlayerBaseData->Money_Bonus)
			{
				g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_ACCOUNT_ERR_1"));			// 金錢不足
				return;
			}

			m_lockBag=bagpos;

			if (g_pBagFrame)
				g_pBagFrame->Locked(bagpos);


			m_item=&item;
		//	m_locked=true;

			char tempp[128];
			strcpy(tempp,password);
			NetCli_DepartmentStore::SL_BuyItem( item.GUID, bagpos, stock, tempp);
		}
		else
		{
			lua_State* L =m_base-> GetLuaState();


			lua_pushboolean(L, false);

			lua_setglobal(L, "arg1");
			lua_pushnumber(L, -1);

			lua_setglobal(L, "arg2");
			m_base->	SendWorldEvent("ITEMMALL_ITEM_OK");

			g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_ITEMMALL_BAGFULL"));			//商城 背包已滿
			//return;


		}
	}
}


bool	CompItemMall( DepartmentStoreBaseInfoStruct* item1 , DepartmentStoreBaseInfoStruct* item2 )
{
	bool ret;
	if (item1->SortNumber ==  item2->SortNumber)
	{
		ret=item1->Sell.Cost<  item2->Sell.Cost;
	}
	else
		ret=item1->SortNumber <  item2->SortNumber;
	return  ret;
}

void _ItemMallFrame::IM2_CalNewMenu(void)
{


	m_ItemTypeList.clear();
	if (m_BKSelcetTypeID>=0)
	{

		SellType2* type=IM2_GetType(m_BKSelcetTypeID);
		if (type->m_child.size()>0)
			type->m_Open = (m_BKOpen != 0);
		m_BKSelcetTypeID=-1;
	}


	if (m_SelectType>=0&&m_SelectType<m_SellTypeList2.size())
		m_SellTypeList2[m_SelectType].addMenu(m_ItemTypeList);


	
	

	std::sort(m_ItemTypeList.begin(),m_ItemTypeList.end());


	m_base->SendWorldEvent("IMF_FILTER_UPDATE");

}



void _ItemMallFrame::CalNewMenu(void)
{
	/*
	int i;
	int count=	m_SellTypeList.size();
	m_ItemTypeList.clear();

	for ( i=0;i<count;i++)
	{
		SellType& ref=m_SellTypeList[i];
		ref.addMenu(m_ItemTypeList);
	}
	std::sort(m_ItemTypeList.begin(),m_ItemTypeList.end());


m_base->	SendWorldEvent("ITEMMALL_ITEMTYPE_UPDATE");
*/
}
void _ItemMallFrame::SetFilterType(int type)
{
	m_NowType=type;
	OpenItemInfo();
}
void _ItemMallFrame::Clear(void)
{
	m_AllTypeInfo.clear();
}
void _ItemMallFrame::ItemType(int count ,int * itemTyperList)
{
	m_SellTypeList.clear();

	

	m_datelive.clear();

	m_datelive.resize(count);


	if (count>0)
	{
		int i;
		SellType itemType;
		for (i=0;i<count;i++)
		{
			itemType.m_type=(itemTyperList[i]&0xff)<<16;
			if (itemType.m_type>0)
			{
				
				itemType.m_count=0;
				itemType.m_time=&m_datelive[i];
				m_SellTypeList.push_back(itemType);
				

			}
			
		}

	
		UpDateItem();
		int size=(int)m_SellTypeList.size();

		if ((m_NowType==-1||m_all)&&size>0)
		{

			m_NowType=m_SellTypeList[0].m_type;

			ShopInfoRequest((m_NowType>>16)&0xff);
			
		}
	}

	CalNewMenu();
	

	m_nowTime=timeGetTime();
}

int _ItemMallFrame::GetItemType(int index,int& s1,int& s2 ,int& s3)
{
	int ret=-1;
	if (index>=0&&index<(int)m_ItemTypeList.size())
		ret=m_ItemTypeList[index];

	s1=(ret>>16)&0xff;
	s2=(ret>>8)&0xff;
	s3=(ret)&0xff;


	return ret;


}

int  _ItemMallFrame::GetItemTypeNums(void)
{

	
	int size=(int)m_ItemTypeList.size();

	
	




	return size;

}
void CMerchantFrame::ChangeZone()							
{


	SendWorldEvent("GAMEBAR_CHANGEZONE");
}	
ItemMallInfo* _ItemMallFrame::AddNewTypeInfo(DWORD type)
{
	ItemMallInfo value;
	

	
	m_AllTypeInfo[type]=value;

	return &m_AllTypeInfo[type];

}
void _ItemMallFrame::ItemTypeRequest(void)
{
	NetCli_DepartmentStore::SL_SellTypeRequest();
}

void _ItemMallFrame::ShopInfoRequest(DWORD ItemType)
{
	 
	NetCli_DepartmentStore::SL_ShopInfoRequest(ItemType );
}

ItemMallInfo* _ItemMallFrame::GetInfo(DWORD type)
{
	ItemMallInfo* ret=NULL;		


	if (m_AllTypeInfo.find(type)!= m_AllTypeInfo.end() )
	{
		ItemMallInfo* obj=&m_AllTypeInfo[type];
		
			ret=obj;		
	}
	return ret;
}

SellType* _ItemMallFrame::GetType(DWORD type)
{
	int i;
	SellType* ret=NULL;
	int count=(int)m_SellTypeList.size();
	for(i=0;i<count;i++)
	{

		ret =m_SellTypeList[i].find(type);
		if (ret)
		
			
			break;
		


	}
	return ret;

}

void _ItemMallFrame::RunningMessage(vector<string>& MsgList)
{
m_Messages=MsgList;
m_base->	SendWorldEvent("ITEMMALL_MESSAGELIST_UPDATE");

}


void _ItemMallFrame::MailGift			( int ItemGUID , const char* Password , int BackGroundType 
					 , const char* TargetName , const char* Title , const char* Content )

{
	CRoleSprite* sprite = g_pGameMain->GetPlayer();
	const char *pPlayerName="None";
	if (sprite)
		pPlayerName=sprite->GetName();


	char temp1[512];
	strcpy(temp1,TargetName);
	strlwr(temp1);
	char temp2[512];
	strcpy(temp2,pPlayerName);
	strlwr(temp2);
	if (!strcmp (temp2,temp1) )
	{

		m_base->	SendWorldEvent("ITEMMALL_MAILNAME_ERROR");

		const char*  ErrMsg=g_ObjectData->GetString("ITEMMALL_MAILNAME_ERROR");	

		g_pGameMain->SendWarningMsg(ErrMsg);	

		g_pChatFrame->AddMessage(5, ErrMsg);
		return ;
	}

	DepartmentStoreBaseInfoStruct* item=g_pItemMallFrame->GetItemInfo(-1,ItemGUID);
	if (item)
	{

		int CountX=item->Item.Count;


		GameObjDbStructEx* Obj = g_ObjectData->GetObj(item->Item.OrgObjID);
		if (Obj)
		{


			int t=0;
			for (int i=0;i<8;i++)
			{
				if (item->Item.IntAbility[i])
					t++;
			}

			if (t)
			CountX=1;
			m_BKSelcetTypeID=m_SelcetTypeID;
			SellType2 *ret= IM2_GetType(m_SelcetTypeID);
			m_BKOpen=ret->m_Open;

			m_item=item;
			NetCli_DepartmentStore::SL_MailGift(ItemGUID,CountX,Password,(MailBackGroundTypeENUM)BackGroundType,TargetName,Title,Content);
		}
	}



}

int  _ItemMallFrame::AddMember(int type,int state,const char *name,bool force)
{

	int count=(int)m_lists.size();
	for (int i=0;i<count;i++)
	{
		if (m_lists[i].m_name==name)
			return i;

	}

	int index=(int)m_lists.size();
	

	IM_FListItem item;
	item.m_name=name;
	item.type=type;
	item.state=state;
	
	m_lists.push_back(item);

	return index;


}


void _ItemMallFrame::CalAllList(void)
{
		m_lists.clear();
	int count=g_pGuildFrame->GetNumGuildMembers();
	CRoleSprite* sprite = g_pGameMain->GetPlayer();
	const char *pPlayerName="None";

	if (sprite)
	pPlayerName=sprite->GetName();

	GuildMemberListInfo* info; 
	for (int i=0;i<count;i++)
	{

		info=g_pGuildFrame->GetGuildRosterInfo(i);
		if (info)
		{ 
			GuildMemberBaseInfoStruct* pmember=info->member;
			bool bOnLine=pmember->IsOnline;


			if (strcmp(pPlayerName,pmember->MemberName.Begin()))
				AddMember(0,bOnLine ? 1:0,pmember->MemberName.Begin());
		}
	}

	BaseFriendStruct* pList = NULL;
	int MaxCount = 0;
	CNetGlobal::RoleData()->PlayerSelfData->FriendList.GetListInfo( EM_FriendListType_FamilyFriend, &pList, MaxCount );

	count = 0;

	for(int i = 0 ; i < MaxCount ; ++i )
	{
		if( pList[i].IsEmpty() == false )
		{
			int dbid=pList[i].DBID;
			bool bOnLine =g_pUi_Socal->m_PlayerOnlineSet.find(dbid)!=g_pUi_Socal->m_PlayerOnlineSet.end();
			AddMember(1,bOnLine ? 1:0,pList[i].Name);
			++count;
		}
	}

	CNetGlobal::RoleData()->PlayerSelfData->FriendList.GetListInfo( EM_FriendListType_Friend, &pList, MaxCount );

	count = 0;

	for(int i = 0 ; i < MaxCount ; ++i )
	{
		if( pList[i].IsEmpty() == false )
		{
			int dbid=pList[i].DBID;
			bool bOnLine =g_pUi_Socal->m_PlayerOnlineSet.find(dbid)!=g_pUi_Socal->m_PlayerOnlineSet.end();
			AddMember(2,bOnLine ? 1:0,pList[i].Name);
			++count;
		}
	}
#ifdef _DEBUGx

	{
		char temp[256];
		for (int i=0;i<40;i++)
		{
			sprintf(temp,"test_%d",	i);
			AddMember(0,1,temp);
		}
	}

#endif

}



void _ItemMallFrame::MailGiftResult			( int Result )
{
	const char *ErrMsg=g_ObjectData->GetString("SYS_ACCOUNT_ERR_1");
	int id=-1;
	bool odk=false;
	bool GetNewData=true;
	switch(Result)
	{
	case EM_CheckMailGiftResultType_OK://				,
		{
			ErrMsg=g_ObjectData->GetString("SYS_ACCOUNT_BUY_SUCCEEDED");				
			if (m_item)
			{
				GameObjDbStructEx*	OrgDB;
				OrgDB =RoleDataEx:: St_GetObjDB(m_item->Item.OrgObjID);
				if (OrgDB)
					id =OrgDB->GUID;
			}
			odk=true;
			GetNewData=false;
		}

		break;
	case EM_CheckMailGiftResultType_TargetNotFind://				,
		ErrMsg=g_ObjectData->GetString("SYS_ACCOUNT_ERR_7");

		m_base->SendWorldEvent("ITEMMALL_PLAYER_NOTFAND");


		break;
	case EM_CheckMailGiftResultType_ItemError://				,

		ErrMsg=g_ObjectData->GetString("SYS_ACCOUNT_ERR_3");	
		break;
	case EM_CheckMailGiftResultType_MoneyError://				,
		ErrMsg=g_ObjectData->GetString("SYS_ACCOUNT_ERR_1");
		break;
	case EM_CheckMailGiftResultType_PasswordError://				,
		ErrMsg=g_ObjectData->GetString("SYS_LOGIN_PASSWORLD_FAIL");
		break;
	case EM_CheckMailGiftResultType_DataError://		
		ErrMsg=g_ObjectData->GetString("SYS_ACCOUNT_ERR_3");	
		break;
	case EM_CheckMailGiftResultType_TimeOut://				,
		ErrMsg=g_ObjectData->GetString("SYS_ACCOUNT_ERR_6");	
		break;
	case EM_CheckMailGiftResultType_SellOut://				,

		ErrMsg=g_ObjectData->GetString("SYS_ACCOUNT_ERR_5");	
		break;
	case EM_CheckMailGiftResultType_NotSendToSelf:

		ErrMsg=g_ObjectData->GetString("SYS_ACCOUNT_ERR_8");	
		break;
	case EM_CheckMailGiftResultType_DisableSendGift:

		ErrMsg=g_ObjectData->GetString("SYS_ACCOUNT_ERR_9");	
	break;

	}
g_pGameMain->SendWarningMsg(ErrMsg);			

g_pChatFrame->AddMessage(5, ErrMsg);




lua_State* L =m_base-> GetLuaState();


lua_pushboolean(L, odk);

lua_setglobal(L, "arg1");
lua_pushnumber(L, id);

lua_setglobal(L, "arg2");
m_base->	SendWorldEvent("ITEMMALL_ITEM_OK");


if (GetNewData)
{
	m_nowTime=-1;
	//	Open(1);
	m_all=true;
	calAllItemMall();

	m_index=0;

	if (m_index>=0&&m_index<m_SellTypeList.size())
	{
		m_NowType=m_SellTypeList[m_index].m_type;
		ShopInfoRequest((m_NowType>>16)&0xff);
	}
	m_base->	SendWorldEvent("ITEMMALL_LOADING");
}




}
void CMerchantFrame::RunningMessage( vector<string>& MsgList )
{
	if (g_pItemMallFrame)
		g_pItemMallFrame->RunningMessage(MsgList);
}
void CMerchantFrame::SlotMachineInfo( int itemID[] , int itemCount[])
{
	if (g_pItemMallFrame)
		g_pItemMallFrame->SlotMachineInfo(itemID,itemCount);
}
void CMerchantFrame::SlotMachineInfoChange()
{
	if (g_pItemMallFrame)
		g_pItemMallFrame->SlotMachineInfoChange();
}
void CMerchantFrame::SlotMachinePlayResult( int result ,   int itemID , int itemCount  )
{
	if (g_pItemMallFrame)
	{
		
		


		if (result==0)
		{
			lua_State* L = GetLuaState();

			time_t NowTime =_get_NowTime(); 

			g_pItemMallFrame->m_ResultItem = itemID;

			int index=	g_pItemMallFrame->SlotMachinePlayResult( itemID ,  itemCount,(int)NowTime,m_GameBarCost );
			
			lua_pushnumber(L, index);

			lua_setglobal(L, "arg1");

		
			SlotMachineInfoChange();


			SendWorldEvent("GAMEBAR_RESULT_OK");
		}
		else
			SendWorldEvent("GAMEBAR_RESULT_FAILED");



	}
}
void CMerchantFrame::MailGiftResult			( int Result )
{

	if (g_pItemMallFrame)
		g_pItemMallFrame->MailGiftResult(Result);
}



void _ItemMallFrame::OpenItemInfo(void)
{

	NetCli_DepartmentStore::SL_ImportAccountMoney();
	int nowtime=timeGetTime();
	bool bReload=false;
	SellType*  ret=GetType(m_NowType);
	if (ret)
	{
		if ((nowtime- *ret->m_time  )>=DF_ITEMTYPE_COOLDOWN)
			bReload=true;


		if (ret->m_child.size()>0)			
			ret->m_Open=!ret->m_Open;

		CalNewMenu();
	}

	if (bReload)
		ShopInfoRequest((m_NowType>>16)&0xff);


}


bool _ItemMallFrame::Open(bool All)
{
	m_all=All;
	int nowtime=timeGetTime();


	

	NetCli_DepartmentStore::SL_SlotMachineInfoRequest();

	//是否要更新商城資料 可以動態
	if (m_nowTime==-1 || (nowtime-m_nowTime )>=DF_ITEMTYPE_COOLDOWN)
	{

		if (m_all)
			calAllItemMall();

		ItemTypeRequest();
	}
	//判斷瀏覽類別
	if (m_NowType!=-1&&!m_all)
		OpenItemInfo();

	for (int i=0;i<m_SellTypeList2.size()-1;i++)
	{

		m_SellTypeList2[i].Close();
	}

	//NetCli_Other::SL_OpenSession(EM_SessionType_OpenItemMall);
	
	return m_IsDisableSendGift;

	//清除購物車
//	ClearShoppingCart();

}

void _ItemMallFrame::Close()
{
	//std::map<int, int>::iterator it = NetCli_Other::m_SessionID.find(EM_SessionType_OpenItemMall);

	//if (it != NetCli_Other::m_SessionID.end())
	//{
	//	NetCli_Other::SL_CloseSession(EM_SessionType_OpenItemMall, it->second);
	//}
}

bool _ItemMallFrame::findType(SellType* ret,DWORD type)
{
	int i;
	int size=(int)ret->m_child.size();
	for(i=0;i<size;i++)
	{
		if (ret->m_child[i].m_type==type)
			return false;
	}


	SellType value;

	value.m_time=ret->m_time;

	value.m_type=type;


	ret->m_child.push_back(value);



	ret->m_Open=true;

	

	return true;


}

void _ItemMallFrame::PushTopList(DepartmentStoreBaseInfoStruct* pitem)
{

	int i;
	int count=(int)m_TopList.size();
	for (i=0;i<count;i++)
	{
		DepartmentStoreBaseInfoStruct* pV=m_TopList[i];
		if (pV->GUID==pitem->GUID)
			return ;

		if (pV->Sell.Count<pitem->Sell.Count)
		{

			m_TopList.push_back(pitem);
			int l=count;
			for (;l>i;l--)
			{

				m_TopList[l]=m_TopList[l-1];

			}
			m_TopList[i]=pitem;
			return ;

		}


	}
	m_TopList.push_back(pitem);
}

void _ItemMallFrame::ShopInfo(int count, DepartmentStoreBaseInfoStruct* itemList)
{
	int index=m_NowType;
	if (m_forceUpdateType!=-1)
	{
		index=m_forceUpdateType;
		m_forceUpdateType=-1;
	}
	


	SellType* ret =GetType(index);
	if (ret==NULL)
		return ;

	ret->m_count=count;

	ret->m_child.clear();
	
	*ret->m_time=timeGetTime();

	int i;
	for(i=0;i<count;i++)
	{
		DWORD Dtype;
#ifdef _DEBUGX //測試商城選項

		/*
		if (itemList[i].SellType[0]==11)
		{
			itemList[i].SellType[1]=(i%6)+1;

			if (itemList[i].SellType[1]==5)
				itemList[i].SellType[2]=(i%2)+1;
		}

		if (itemList[i].SellType[0]==10)
		{

			itemList[i].SellType[1]=(i%6)+1;

		}
*/


#endif

		DepartmentStoreBaseInfoStruct &item=itemList[i];


		if (!g_pGameMain->getMerchant())
		{
			if (item.Sell.Cost>0)
				continue;

		}



		//如果沒有次選項都歸類同一個
		if (item.SellType[1]==-1)
			item.SellType[1]=0;
		//如果沒有次選項都歸類同一個
		if (item.SellType[2]==-1)
			item.SellType[2]=0;
		int s1=item.SellType[0]&0xff;
		int s2=item.SellType[1]&0xff;
		int s3=item.SellType[2]&0xff;


		

		Dtype=(s1<<16)|(s2<<8)|s3;
		ItemMallInfo* Itemret=GetInfo(Dtype);
		if (Itemret==NULL)
			Itemret=AddNewTypeInfo(Dtype);

		if(s3>0)
		{
			Dtype=(s1<<16)|(s2<<8);
			 Itemret=GetInfo(Dtype);
			if (Itemret==NULL)
				Itemret=AddNewTypeInfo(Dtype);


		}

		Itemret->m_ItemMallInfoList.clear();
		
	}

	if (count>0)
	{
		DWORD Dtype=(itemList[0].SellType[0]&0xff)<<16;
		ItemMallInfo* Itemret=GetInfo(Dtype);
		if (Itemret==NULL)
			Itemret=AddNewTypeInfo(Dtype);
		Itemret->m_ItemMallInfoList.clear();

	}

	
	
	
	
	for(i=0;i<count;i++)
	{
		DWORD Dtype;
		DepartmentStoreBaseInfoStruct &item=itemList[i];

		if (!g_pGameMain->getMerchant())
		{
			if (item.Sell.Cost>0)
				continue;

		}

		m_ItemMaps[item.GUID]=item;

		DepartmentStoreBaseInfoStruct &pitem=m_ItemMaps[item.GUID];

	

		int s1=item.SellType[0]&0xff;
		int s2=item.SellType[1]&0xff;
		int s3=item.SellType[2]&0xff;
		Dtype=(s1<<16)|(s2<<8)|s3;

	
		time_t 	time1=_ServerTimeToUTC(TimeExchangeFloatToInt(item.Sell.BeginTime));


	

		
	//	time_t NowTime;
	//	time( &NowTime );
	//	gmtime();
		time_t NowTime =_get_NowTime();// g_pGameMain->GetGameTime() + g_pGameMain->GetServerTimeZone()*60*60;
		
		//struct tm *time_now;  
		//time_now = localtime(&NowTime);  
		 //char str[80];  
		// strftime(str, 80,"20%y:%m:%d:%H:%M:%s",time_now);  

		int df=(int)difftime(NowTime,time1);
		int  sec=df;
		int mine=sec/60;
		int hour=mine/60;
		

		int day=hour/24;
		int iType=item.Sell.Icon;
		//pitem.Sell.Icon=0;

	

		

		

#ifdef _DEBUGX
		day=rand()%50;
#endif
		if (day>=0&&day<14  &&item.Sell.Cost_Free==0)
		{
			m_SellTypeList2[0].addItem(-1,pitem);
			pitem.Sell.Icon=2;

			
			
		}
		PushTopList(&pitem);
#ifdef _DEBUGX
		iType=rand()%5;

		switch (rand()%3)
		{
		case 0:
			item.Sell.Cost=111;
			item.Sell.Cost_Bonus=0;
			item.Sell.Cost_Free=0;

			break;
		case 1:
			item.Sell.Cost=0;
			item.Sell.Cost_Bonus=20;
			item.Sell.Cost_Free=0;
			break;
		case 2:
			item.Sell.Cost=0;
			item.Sell.Cost_Bonus=0;
			item.Sell.Cost_Free=10;
			break;

		}
#endif
		//break;

		

		if (pitem.Sell.MaxCount>0)
			pitem.Sell.Icon=3;

		if (pitem.Sell.BeginTime ||pitem.Sell.EndTime)
		{


			//time_t NowTime;
			//time( &NowTime );
			time_t NowTime = _get_NowTime();//g_pGameMain->GetGameTime() + g_pGameMain->GetServerTimeZone()*60*60;

			time_t 	time1=_ServerTimeToUTC(TimeExchangeFloatToInt(pitem.Sell.EndTime));

			//time1=_get_localTime(time1);

			int df=(int)difftime( time1,NowTime);
			int  sec=df;
			int mine=sec/60;
			int hour=mine/60;
			int day=hour/24;


			if (day<365)
				pitem.Sell.Icon=4;
		}

	
		if (iType==1 || pitem.Sell.Icon==4|| pitem.Sell.Icon==3)//off sale
			m_SellTypeList2[1].addItem(Dtype,pitem);


		if (item.Sell.Cost>0)
			m_SellTypeList2[2].addItem(Dtype,pitem);
		else if (item.Sell.Cost_Bonus>0)
			m_SellTypeList2[3].addItem(Dtype,pitem);
		else if (item.Sell.Cost_Free)
			m_SellTypeList2[4].addItem(Dtype,pitem);





			
		

	
		

		ItemMallInfo* ItemretMenu=GetInfo(Dtype);

		


		ItemMallInfo* Itemret=GetInfo(s1<<16);

		if (Itemret&&Itemret!=ItemretMenu)
		{
			findType(ret,Dtype);
			Itemret->m_ItemMallInfoList.push_back(&pitem);
			std::sort(Itemret->m_ItemMallInfoList.begin(),Itemret->m_ItemMallInfoList.end(),CompItemMall);
		}


		if(s3>0)
		{


			Dtype=(s1<<16)|(s2<<8);
			findType(ret,Dtype);
			ItemMallInfo* ItemretS2=GetInfo(Dtype);
			
			ItemretS2->m_ItemMallInfoList.push_back(&pitem);

		}

			

		ItemretMenu->m_ItemMallInfoList.push_back(&pitem);
		std::sort(ItemretMenu->m_ItemMallInfoList.begin(),ItemretMenu->m_ItemMallInfoList.end(),CompItemMall);

	}





	




	
	// 通知更新事件
	//m_base->	SendWorldEvent("ITEMMALL_ITEMTYPE_UPDATE");
	

	if (m_all)
	{
		m_index++;
		if (m_index<m_SellTypeList.size())
		{

			m_NowType=m_SellTypeList[m_index].m_type;
			ShopInfoRequest((m_NowType>>16)&0xff);
		}
		else
		{

			
		IM2_CalNewMenu();
			
			m_all=false;

			int i;
			int count=10;
			if (m_TopList.size()<10)
				count=(int)m_TopList.size();
			for (i=0;i<count;i++)
			{
				if (m_TopList[i]->Sell.Icon!=4&&m_TopList[i]->Sell.Icon!=3)
			m_TopList[i]->Sell.Icon=1;
			}

			// 通知更新事件
			//if (index==m_NowType)
				m_base->SendWorldEvent("ITEMMALL_INFO_UPDATE");
				m_base->SendWorldEvent("IMF_ITEM_UPDATE");

		}



	}else
		CalNewMenu();
}

// ----------------------------------------------------------------------------
CMerchantFrame::CMerchantFrame(CInterface* object)
	: CInterfaceBase(object)
	
{
	m_bDirty=false;
	m_itemUpdate = false;
	m_itemNums = 0;
	m_FilterType = 1;
	m_TopupStatus = EM_TopupStatus_StandBy;
	m_TopupWaitTick = 0;
	m_TouupUrl = "";
	m_ZShopStatus = EM_ZShopStatus_StandBy;
	m_ZShopWaitTick = 0;
	
	g_pMerchantFrame = this;

	g_pItemMallFrame= new _ItemMallFrame(this);
}

// ----------------------------------------------------------------------------
CMerchantFrame::~CMerchantFrame()
{
	g_pMerchantFrame = NULL;
	if (g_pItemMallFrame)
	delete g_pItemMallFrame;
	g_pItemMallFrame=NULL;
}

// ----------------------------------------------------------------------------
void CMerchantFrame::RegisterFunc()
{
	lua_State* L = GetLuaState();
	if (L)
	{		
		lua_register(L, "GetMerchantItemInfo"	,	LuaFunc_GetMerchantItemInfo		);
		lua_register(L, "GetMerchantItemDetail"	,	LuaFunc_GetMerchantItemDetail	);
	
		lua_register(L, "GetMerchantItemNums"	,	LuaFunc_GetMerchantItemNums		);

		lua_register(L, "GetMerchantFilterNums"	,	LuaFunc_GetMerchantFilterNums	);
		lua_register(L, "GetMerchantFilterInfo"	,	LuaFunc_GetMerchantFilterInfo	);
		lua_register(L, "SetMerchantFilterType"	,	LuaFunc_SetMerchantFilterType	);
		lua_register(L, "GetMerchantFilterType"	,	LuaFunc_GetMerchantFilterType	);

		lua_register(L, "MF_ShoppingBuy"		,	LuaFunc_MF_ShoppingBuy			);
	
		lua_register(L, "GameBarClose"			,	LuaFunc_GameBarClose			);
 		lua_register(L, "ItemExchangeRequest"	,	LuaFunc_ItemExchangeRequest		);

		//
		lua_register(L, "ItemMallFrame_Open"	,	LuaFunc_ItemMallFrame_Open		);
		lua_register(L, "GC_RuneReload"			,	LuaFunc_GC_RuneReload			);
	  	lua_register(L, "GetMerchantItemLink"	,	LuaFunc_GetMerchantItemLink		);
		lua_register(L, "GetItemMallLink"		,	LuaFunc_GetItemMallLink			);

		lua_register(L, "CIMF_OpenMall2"		,	LuaFunc_CIMF_OpenMall2			);
		lua_register(L, "CIMF_CloseMall2"		,	LuaFunc_CIMF_CloseMall2			);
		lua_register(L, "CIMF_GetNums"			,	LuaFunc_CIMF_GetNums			);
		lua_register(L, "CIMF_GetItemInfo"		,	LuaFunc_CIMF_GetItemInfo		);
		lua_register(L, "CIMF_GetFilterNums"	,	LuaFunc_CIMF_GetFilterNums		);
		lua_register(L, "CIMF_GetTopItem"		,	LuaFunc_CIMF_GetTopItem			);
		lua_register(L, "CIMF_MailGift"			,	LuaFunc_CIMF_MailGift			);

		lua_register(L, "CIMF_GetListCount"		,	LuaFunc_CIMF_GetListCount		);
		lua_register(L, "CIMF_GetListName"		,	LuaFunc_CIMF_GetListName		);

		lua_register(L, "CIMF_GetMessageCount"	,	LuaFunc_CIMF_GetMessageCount	);
		lua_register(L, "CIMF_GetMessage"		,	LuaFunc_CIMF_GetMessage			);

		lua_register(L, "CIMF_SearchItem"		,	LuaFunc_CIMF_SearchItem			);
		lua_register(L, "CIMF_ClearHistory"		,	LuaFunc_CIMF_ClearHistory		);
		lua_register(L, "CIMF_SetHistory"		,	LuaFunc_CIMF_SetHistory			);
		
		lua_register(L, "CIMF_GetFilterInfo"	,	LuaFunc_CIMF_GetFilterInfo		);
		lua_register(L, "CIMF_SelectType"		,	LuaFunc_CIMF_SelectType			);

		lua_register(L, "CIMF_SelectFilterIndex",	LuaFunc_CIMF_SelectFilterIndex	);
		lua_register(L, "CIMF_ShoppingBuy"		,	LuaFunc_CIMF_ShoppingBuy		);

		lua_register(L, "IsEnableMerchant"		,	LuaFunc_IsEnableMerchant		);
		lua_register(L, "IsEnableGameBar"		,	LuaFunc_IsEnableGameBar			);
		lua_register(L, "GetGameBarFreeCount"	,	LuaFunc_GetGameBarFreeCount		);
		lua_register(L, "GetGameBarItemInfo"	,	LuaFunc_GetGameBarItemInfo		);
		lua_register(L, "GetGameBarStart"		,	LuaFunc_GetGameBarStart			);
		lua_register(L, "GetGameBarPlayCost"	,	LuaFunc_GetGameBarPlayCost		);
		lua_register(L, "GameBar_Stoped"		,	LuaFunc_GameBar_Stoped			);
		lua_register(L, "GameBar_GetHistory"	,	LuaFunc_GameBar_GetHistory		);
		lua_register(L, "GameBar_GetHistorys"	,	LuaFunc_GameBar_GetHistorys		);

		lua_register(L, "GameBar_CanPlay"		,	LuaFunc_GameBar_CanPlay			);
		lua_register(L, "OpenTopupPage"			,	LuaFunc_OpenTopupPage			);

		lua_register(L, "ItemMallHelpContent"	,	LuaFunc_ItemMallHelpContent		);

		//zshop
		lua_register(L, "IsEnableZShop"			,	LuaFunc_IsEnableZShop			);
		lua_register(L, "SetOnTimeWebBagRequest",	LuaFunc_SetOnTimeWebBagRequest	);
		lua_register(L, "OpenZShop"				,	LuaFunc_OpenZShop				);
		lua_register(L, "GetItemLink"			,	LuaFunc_GetItemLink				);
	}
}

// ----------------------------------------------------------------------------
void CMerchantFrame::Update(float elapsedTime)
{
	lua_State* L = GetLuaState();

	if ( m_itemUpdate && L )
	{
		m_itemUpdate = false;
		lua_pushnumber(L, m_itemNums);
		lua_setglobal(L, "arg1");
		SendWorldEvent("MERCHANT_ITEM_UPDATE");
	}

	unsigned long NowTick = GetTickCount();

	switch (m_TopupStatus)
	{
	case EM_TopupStatus_StandBy:
		break;
	case EM_TopupStatus_WaitSession:
		{
			if (NowTick >= m_TopupWaitTick)
			{
				m_TopupStatus = EM_TopupStatus_StandBy;
				m_TopupWaitTick = 0;
				m_TouupUrl = "";
			}
		}
		break;
	case EM_TopupStatus_WaitOpen:
		{
			if (NowTick >= m_TopupWaitTick)
			{
				TopupOpenWebPage();

				m_TopupStatus = EM_TopupStatus_StandBy;
				m_TopupWaitTick = 0;
				m_TouupUrl = "";
			}
		}
		break;
	}

	switch (m_ZShopStatus)
	{
	case EM_ZShopStatus_StandBy:
		break;
	case EM_ZShopStatus_WaitSession:
		{
			if (NowTick >= m_ZShopWaitTick)
			{
				m_ZShopStatus = EM_ZShopStatus_StandBy;
				m_ZShopWaitTick = 0;
			}
		}
		break;
	case EM_ZShopStatus_WaitOpen:
		{
			if (NowTick >= m_ZShopWaitTick)
			{
				ZShopOpenWebPage();

				m_ZShopStatus = EM_ZShopStatus_StandBy;
				m_ZShopWaitTick = 0;
			}
		}
		break;
	}
}

void  CMerchantFrame::ImportAccountMoneyResult(int Runes)
{

	

	if (g_pItemMallFrame)
		g_pItemMallFrame->ImportAccountMoneyResult(Runes);



}

// ----------------------------------------------------------------------------
void CMerchantFrame::SetFilterType( int type )
{
	m_FilterType = type;

	if (g_pItemMallFrame)
	g_pItemMallFrame->SetFilterType(type);

	
}

// ----------------------------------------------------------------------------
int CMerchantFrame::GetFilterType()
{
	return m_FilterType;
}

void CMerchantFrame::BuildFrames(void)
{

	g_pItemMallFrame->BuildFrames();

}
void CMerchantFrame::DestroyFrames(void)
{


	

	g_pItemMallFrame->DestroyFrames();
}

// ----------------------------------------------------------------------------
void _ItemMallFrame::UpDateItem(void)
{
	
	int count=(int)m_SellTypeList.size();
	for (int i=0;i<count;i++)
	{
		SellType& ret=m_SellTypeList[i];
		
		if (ret.m_type==m_NowType)
		{
			if (*ret.m_time==-1)
				ShopInfoRequest((m_NowType>>16)&0xff);

		}

	}
	
		
	

}
 
void CMerchantFrame::BuyItemResult(int  Result)
{

	//m_wait=false;
if (g_pItemMallFrame)
g_pItemMallFrame->BuyItemResult(Result);
//History
if (Result==0)
SetSaveUpdateTime();

}

 void CMerchantFrame::SaveGamebarLog()	
 {
	 if (!m_bDirty)
		 return ;
	
	 CTempFile tempFile(65536);



	 int count=(int)g_pItemMallFrame->m_gameBarHistoryLogs.size();

	 for (int i=0;i<count;i++)
	 {

		 GameBarHistoryLog &v=g_pItemMallFrame->m_gameBarHistoryLogs[i];

		 tempFile.Write(&v.dbid,sizeof(int));
		 tempFile.Write(&v.count,sizeof(int));
		 tempFile.Write(&v.date,sizeof(int));
		 tempFile.Write(&v.cost,sizeof(int));

	 }
	 g_pGameMain->SaveClientData(eSaveClientData_GameBar, tempFile.GetPosition(), tempFile.GetData());

 m_bDirty=false;

 }

// ----------------------------------------------------------------------------
// 要求 Server 儲存資料
 void CMerchantFrame::SaveClientData()	
 {

	 lua_State* L = GetLuaState();
	 {
		 // 產生暫存資料
		 CTempFile tempFile(65536);
	

		//lua_pushstring("g_IM2_History");



		lua_pushstring(L,"g_IM2_History");
		lua_gettable(L,LUA_GLOBALSINDEX);

		CLuaTable table(L,"g_IM2_History");

		const char *Accountstr=  g_pGameMain->GetAccountName();
		CLuaTable Accounttable=table.SubTable(Accountstr);

	//if (root.GetType("g_IM2_History")==LUA_TTABLE)
	// 	 CLuaTable table=root.SubTable("g_IM2_History");

	 std::vector<int> dbids;


	 if (Accounttable.GetKeys(dbids) )
	 {
		 int count=(int)dbids.size();
		  for(int i=0;i<count;i++)
		  {
			  int key=dbids[i];

				if ( Accounttable.GetType(key)==LUA_TTABLE)
				{
					CLuaTable item=Accounttable.SubTable(key);
					int iNum=item.GetInt("count",-1);
					if (iNum!=-1)
					{
						tempFile.Write(&key,sizeof(int));
						tempFile.Write(&iNum,sizeof(int));

					}
				}

		  }

	 }



		g_pGameMain->SaveClientData(eSaveClientData_ItemMallHistory, tempFile.GetPosition(), tempFile.GetData());
	 }
	 

		SaveGamebarLog()	;
	 
	 


	 

	 // 將最後存檔時間去除
	 SetSaveUpdateTime(0.0f);

 }		

 void CMerchantFrame::LeaveWorld()
 {

	 g_pItemMallFrame->m_gameBarHistoryLogs.clear();
 }
// ----------------------------------------------------------------------------
 // 取得由 Server 讀取資料	
 void CMerchantFrame::LoadClientData(int type, int size, char* data)	
 {

	 lua_State* L = GetLuaState();
	 if (type==eSaveClientData_ItemMallHistory)
	 {
		 
		 // 產生暫存資料
		 CTempFile tempFile(data,size);

		 int itemDBID;
		 int itemCount;

		 lua_pushstring(L,"g_IM2_History");
		 lua_gettable(L,LUA_GLOBALSINDEX);
		 CLuaTable table(L,"g_IM2_History");

		 const char *Accountstr=  g_pGameMain->GetAccountName();
		 CLuaTable Accounttable=table.SubTable(Accountstr);


		 int read=0;
		for(int i=0;i<size;)
		{
			 read=tempFile.Read(&itemDBID, sizeof(int));
			 i+=read;
			 if (read==4)
			 {
				read=  tempFile.Read(&itemCount, sizeof(int));
				 i+=read;
				if (read==4)
				 {
					 CLuaTable item=Accounttable.SubTable(itemDBID);
					item.SetInt("count",itemCount);
				  }
				 else
					 break;

			 }
			 else
				 break;

		 }
	 }
	 if (type==eSaveClientData_GameBar)
	 {
		 // 產生暫存資料
		 CTempFile tempFile(data,size);
		 int itemDBID;
		 int itemCount;
		 int itemTime;
		 int cost;


		g_pItemMallFrame->m_gameBarHistoryLogs.clear();

		 int count=0;
		 for(int i=0;i<size;)
		 {
			 i+=tempFile.Read(&itemDBID, sizeof(int));
			 i+=tempFile.Read(&itemCount, sizeof(int));
			 i+=tempFile.Read(&itemTime, sizeof(int));
			 i+=tempFile.Read(&cost, sizeof(int));

			 GameBarHistoryLog v;
			 v.cost=cost;
			 v.count=itemCount;
			 v.date=itemTime;
			 v.dbid=itemDBID;
			
			 g_pItemMallFrame->m_gameBarHistoryLogs.push_back(v);
			
			

			 count++;
		}
			
	 }
 }		
// ----------------------------------------------------------------------------

DepartmentStoreBaseInfoStruct* CMerchantFrame::GetItemInfo(int index,int GUID)	
{

	DepartmentStoreBaseInfoStruct*ret=NULL;

	if (g_pItemMallFrame)
		ret=g_pItemMallFrame->GetItemInfo(index,GUID);

	return ret;

}


// ----------------------------------------------------------------------------

vector<DepartmentStoreBaseInfoStruct*>&  CMerchantFrame::GetNowItemList()	
{


	ItemMallInfo* ret=NULL;
	if (g_pItemMallFrame)
		ret=g_pItemMallFrame->GetItemMallTable();


	return ret->m_ItemMallInfoList;

}


// ----------------------------------------------------------------------------

void CMerchantFrame::ItemType(int TypeCount , int * TypeList )
{

	if (g_pItemMallFrame)
		g_pItemMallFrame->ItemType(TypeCount,TypeList);


}
void CMerchantFrame::setDisableSendGift(bool IsDisableSendGift )
{
	if (g_pItemMallFrame)
		g_pItemMallFrame->setDisableSendGift(IsDisableSendGift);
}
void CMerchantFrame::ShopInfo(int ItemCount , DepartmentStoreBaseInfoStruct Item[500] )
{
	int i;
	for( i = 0 ; i < ItemCount ; ++i )
	{
		Item[i].Item.CreateTime = 0;
		Item[i].Item.Serial = 0;
		RegisterItemFieldChecksum( &Item[i].Item );
	}

	if (g_pItemMallFrame)
		g_pItemMallFrame->ShopInfo(ItemCount,Item);
}

void CMerchantFrame::TopupOpenSession(const char* url)
{
	m_TouupUrl = url;	
	NetCli_Other::SL_OpenSession(EM_SessionType_OpenItemMall, boost::bind(&CMerchantFrame::TopupOpenSessionResult, this, _1, _2));
	m_TopupWaitTick = GetTickCount() + 15000;
	m_TopupStatus = EM_TopupStatus_WaitSession;
}

void CMerchantFrame::TopupOpenSessionResult(int sessiontype, int sessionid)
{
	m_TopupWaitTick = GetTickCount() + 1500;
	m_TopupStatus = EM_TopupStatus_WaitOpen;
}

void CMerchantFrame::TopupOpenWebPage()
{
	lua_State* L = GetLuaState();
	std::string	KeyStr = "";

	char Buff[64];
	size_t found = std::string::npos;

	//-------------------------------------------------	
	if (g_pGameMain != NULL)
	{
		KeyStr = "[$ROLEDBID]";
		sprintf(Buff, "%d", g_pGameMain->GetDBID());

		found = m_TouupUrl.find(KeyStr.c_str());
		while (found != std::string::npos)
		{
			m_TouupUrl.replace(found, KeyStr.length(), Buff);
			found = m_TouupUrl.find(KeyStr.c_str());
		}
	}
	//-------------------------------------------------

	//-------------------------------------------------	
	if (g_pAccountLogin != NULL)
	{
		KeyStr = "[$WORLDID]";
		sprintf(Buff, "%d", g_pAccountLogin->GetWorldID());

		found = m_TouupUrl.find(KeyStr.c_str());
		while (found != std::string::npos)
		{
			m_TouupUrl.replace(found, KeyStr.length(), Buff);
			found = m_TouupUrl.find(KeyStr.c_str());
		}
	}
	//-------------------------------------------------

	//-------------------------------------------------
	KeyStr = "[$SESSIONTYPE]";
	sprintf(Buff, "%d", EM_SessionType_OpenItemMall);

	found = m_TouupUrl.find(KeyStr.c_str());
	while (found != std::string::npos)
	{
		m_TouupUrl.replace(found, KeyStr.length(), Buff);
		found = m_TouupUrl.find(KeyStr.c_str());
	}
	//-------------------------------------------------

	//-------------------------------------------------
	if (g_pGameMain != NULL)
	{
		KeyStr = "[$SESSIONID]";
		sprintf(Buff, "%d", g_pGameMain->GetItemMallSessionID());

		found = m_TouupUrl.find(KeyStr.c_str());
		while (found != std::string::npos)
		{
			m_TouupUrl.replace(found, KeyStr.length(), Buff);
			found = m_TouupUrl.find(KeyStr.c_str());
		}
	}

	//-------------------------------------------------

	bool Disable = true;

	if(g_ObjectData->_DisableTag.size() > 671)
	{
		Disable = g_ObjectData->_DisableTag[671];
	}

	if (Disable == false)
	{
		lua_pushstring(L, m_TouupUrl.c_str());
		lua_setglobal(L, "arg1");
		SendWorldEvent("WEBFRAM_URL");
		SendWorldEvent("WEBFRAM_SHOW");
	}
	else
	{
		//-------------------------------------------------
		if (g_pGameMain != NULL)
		{
			HWND hWnd=g_pGameMain->GetWnd();
			ShellExecute(hWnd, "open", m_TouupUrl.c_str(), 0, 0, SW_SHOW);
		}
		//-------------------------------------------------
	}
}

void CMerchantFrame::ZShopOpenSession()
{
	NetCli_Other::SL_OpenSession(EM_SessionType_OpenZShop, boost::bind(&CMerchantFrame::ZShopOpenSessionResult, this, _1, _2));
	m_ZShopWaitTick = GetTickCount() + 15000;
	m_ZShopStatus = EM_ZShopStatus_WaitSession;
}

void CMerchantFrame::ZShopOpenSessionResult(int sessiontype, int sessionid)
{
	m_ZShopWaitTick = GetTickCount() + 1500;
	m_ZShopStatus = EM_ZShopStatus_WaitOpen;
}

void CMerchantFrame::ZShopOpenWebPage()
{
	lua_State* L = GetLuaState();
	std::string	KeyStr = "";
	std::string Url = "";

	char Buff[64];
	size_t found = std::string::npos;

	//-------------------------------------------------	
	if (g_pGameMain != NULL)
	{
		Url = g_pGameMain->GetZShopURL();

		KeyStr = "[$ROLEDBID]";
		sprintf(Buff, "%d", g_pGameMain->GetDBID());

		found = Url.find(KeyStr.c_str());
		while (found != std::string::npos)
		{
			Url.replace(found, KeyStr.length(), Buff);
			found = Url.find(KeyStr.c_str());
		}
	}
	//-------------------------------------------------

	//-------------------------------------------------	
	if (g_pAccountLogin != NULL)
	{
		KeyStr = "[$WORLDID]";
		sprintf(Buff, "%d", g_pAccountLogin->GetWorldID());

		found = Url.find(KeyStr.c_str());
		while (found != std::string::npos)
		{
			Url.replace(found, KeyStr.length(), Buff);
			found = Url.find(KeyStr.c_str());
		}
	}
	//-------------------------------------------------

	//-------------------------------------------------
	KeyStr = "[$SESSIONTYPE]";
	sprintf(Buff, "%d", EM_SessionType_OpenZShop);

	found = Url.find(KeyStr.c_str());
	while (found != std::string::npos)
	{
		Url.replace(found, KeyStr.length(), Buff);
		found = Url.find(KeyStr.c_str());
	}
	//-------------------------------------------------

	//-------------------------------------------------
	if (g_pGameMain != NULL)
	{
		KeyStr = "[$SESSIONID]";
		sprintf(Buff, "%d", g_pGameMain->GetZShopSessionID());

		found = Url.find(KeyStr.c_str());
		while (found != std::string::npos)
		{
			Url.replace(found, KeyStr.length(), Buff);
			found = Url.find(KeyStr.c_str());
		}
	}
	//-------------------------------------------------

	lua_pushstring(L, Url.c_str());
	lua_setglobal(L, "arg1");
	SendWorldEvent("WEBFRAM_URL");
	SendWorldEvent("WEBFRAM_SHOW");
}

// ----------------------------------------------------------------------------
int LuaFunc_GetMerchantItemDetail(lua_State* L)
{

	int index = luaL_checkint(L, 1) - 1;
	int type = luaL_checkint(L, 2)-1 ;

	ItemMallInfo *pInfoTable =NULL;
	if (g_pItemMallFrame)
		pInfoTable = g_pItemMallFrame->GetItemMallTable();
	if ( pInfoTable == NULL )
		return 0;
	char Detail[2048];
	bool enable=false;
		
	int count = (int)pInfoTable->m_ItemMallInfoList.size();
	if ( index >= 0 && index < count )
	{
		DepartmentStoreBaseInfoStruct& item = *pInfoTable->m_ItemMallInfoList[index];
		GameObjDbStructEx* Obj = g_ObjectData->GetObj(item.Item.OrgObjID);

		if ( Obj )
		{
			switch (type)
			{
			case 0://time
				{
					//struct tm *gmt,nowtm;
					//time_t NowTime;
                    //time_t time1,time2;
					//time( &NowTime );
				//	time_t NowTime = _get_NowTime();//g_pGameMain->GetGameTime() + g_pGameMain->GetServerTimeZone()*60*60;
					//gmt = localtime( &NowTime );	
					//nowtm=*gmt;
					//char temp[1024];
					int time = -1;
					switch (Obj->LiveTimeType)
					{
					case EM_ItemLiveTimeType_None:			//不會消失
#ifdef _DEBUGX
						time = Obj->LiveTime;
#endif
						break;

					case EM_ItemLiveTimeType_RealTime:		//真實時間
						time = Obj->LiveTime;
						break;
					}

					if (time!=-1)
					{
						enable=true;
						const char * msg=	g_ObjectData->GetString("MERCHANT_ITEM_TIMER");				
						char timeText[256];
						g_ObjectData->SecondsToTimeAbbrev((float)time, timeText);
						sprintf(Detail,msg,timeText);
						
					}
				}

				break;
			case 1://count

				if (item.Sell.MaxCount>0)
				{

					enable=true;
					const char * msg=	g_ObjectData->GetString("ITEMMALL_TEXT_2");
					int count=item.Sell.MaxCount-(item.Sell.OrgCount+item.Sell.Count);
					sprintf(Detail,msg,count,item.Sell.OrgCount+item.Sell.Count ,item.Sell.MaxCount);	

					
				}

				break;
			case 2://close
				{
					
					if (item.Sell.BeginTime ||item.Sell.EndTime)
					{
						enable=true;
						struct tm *gmt;
						time_t time1,time2;
						
						char BeginTime[512];
						char EndTime[512];
						BeginTime[0]=0;					
						EndTime[0]=0;

						const char * msg=	g_ObjectData->GetString("ITEMMALL_TEXT_1");

						time1=0;
						if (item.Sell.BeginTime>0)
							time1=_ServerTimeToUTC(TimeExchangeFloatToInt(item.Sell.BeginTime));

						struct tm gmt1;
						gmt = localtime( &time1 );	
						if (gmt)
							gmt1=*gmt;

						time2=0;
						if (item.Sell.EndTime>0)
							time2=_ServerTimeToUTC(TimeExchangeFloatToInt(item.Sell.EndTime));	

						struct tm gmt2;
						gmt = localtime( &time2 );	
						if (gmt)
							gmt2=*gmt;
						Detail[0]=0;
						if ((gmt1.tm_year+1900)>=2000)
						{
							const char * msg=	g_ObjectData->GetString("ITEMMALL_TEXT_1");
							time_t localtime=time1;// + g_pGameMain->GetServerTimeZone()*60*60;

							sprintf(BeginTime,msg,TimeStr::DateTimeStr((int)localtime));
							strcat(Detail,BeginTime);
						}
						if ((gmt2.tm_year+1900)<=2030)
						{
							const char * msg=	g_ObjectData->GetString("ITEMMALL_TEXT_5");

							time_t localtime=time2;// + g_pGameMain->GetServerTimeZone()*60*60;
							sprintf(EndTime,msg,TimeStr::DateTimeStr((int)localtime));
							strcat(Detail,"\n");
							strcat(Detail,EndTime);
						}

								
						
					}

				}
				break;
			case 3://bonus
				{
					int Get_Bonus;
#ifdef _DEBUGX
					Get_Bonus=(rand()%6)*100;
#else
					Get_Bonus= item.Sell.Get_Bonus;
#endif

					if (Get_Bonus>0)
					{
						enable=true;

						const char * msg=	g_ObjectData->GetString("ITEMMALL_TEXT_4");

						sprintf(Detail,msg,Get_Bonus);

						
					}
				}
				break;
			case 5://VIP
				{

					if ( item.NeedMemberID>0)
					{
						enable=true;
						const char * msg=	g_ObjectData->GetString("ITEMMALL_TEXT_3");
						sprintf(Detail,msg, item.NeedMemberID);
						
					}

				}
				break;
			case 4://funcion

				{
					enable=true;

					const char * msg=	g_ObjectData->GetString("MERCHANT_ITEM_FUNCTION");
					sprintf(Detail,"\n%s%s",msg,Obj->ShortNote);
					
				}
				break;


			}
		}
	}

	lua_pushboolean(L, enable);	
	
	lua_pushstring(L, Detail);	
	
	return 2;
}

// ----------------------------------------------------------------------------
int LuaFunc_GetMerchantItemInfo(lua_State* L)
{
	int index = luaL_checkint(L, 1) - 1;

	ItemMallInfo *pInfoTable = NULL;

		if (g_pItemMallFrame)
			pInfoTable =g_pItemMallFrame->GetItemMallTable();
	if ( pInfoTable == NULL )
		return 0;

	int count = (int)pInfoTable->m_ItemMallInfoList.size();
	if ( index >= 0 && index < count )
	{
		DepartmentStoreBaseInfoStruct& item =* pInfoTable->m_ItemMallInfoList[index];
		GameObjDbStructEx* Obj = g_ObjectData->GetObj(item.Item.OrgObjID);

		// 取得圖形資料
		GameObjDbStructEx* ImageObj = Obj;
		if ( item.Item.ImageObjectID > 0 )
		{
			ImageObj = g_ObjectData->GetObj(item.Item.ImageObjectID);
			if ( ImageObj == NULL )
				ImageObj = Obj;
		}

		GameObjDbStructEx* imageDB = NULL;
		if ( ImageObj )
		{
			imageDB = CNetGlobal::GetObj(ImageObj->ImageID);
		}

		if ( Obj )
		{
			bool SellOut=false;		


			if (item.Sell.MaxCount>0)
			{
			
				if (item.Sell.MaxCount<=(item.Sell.OrgCount+item.Sell.Count))
					SellOut=true;
			}

			const char *TextureName = NULL;
			if ( imageDB )
			{
				TextureName = imageDB->Image.ACTField;
			}
			int count= (item.Item.Count >0) ? item.Item.Count : 1;
			int sellNum=item.Sell.OrgCount;

			GameObjDbStructEx* imageDB = CNetGlobal::GetObj(ImageObj->ImageID);
			if ( imageDB )

		//	if (count >Obj->MaxHeap)
		//		count=Obj->MaxHeap;


			
			lua_pushstring(L, TextureName);		// Icon
			lua_pushnumber(L, item.Sell.Cost);							// price
			lua_pushnumber(L,count);	// stock
			lua_pushstring(L, Obj->GetName());							// Name
			lua_pushnumber(L,SellOut);						// Icon
			lua_pushnumber(L, item.Sell.Cost_Bonus);						// 需要
		//	lua_pushnumber(L, item.Sell.Get_Bonus);						// 回聵
			int icon= item.Sell.Icon;

			
#ifdef _DEBUGX
		//	icon=rand()%6;		
#endif


			lua_pushnumber(L,icon);						// Icon

			lua_pushnumber(L,sellNum);						// Icon
	
			return 8;
		}

	}
	return 0;

}

// ----------------------------------------------------------------------------
int LuaFunc_GetMerchantItemNums(lua_State* L)
{
	int count=0;


	ItemMallInfo *pInfoTable=NULL;
	if (g_pItemMallFrame)
		pInfoTable =g_pItemMallFrame->GetItemMallTable();
	if (pInfoTable)
	 count=(int)pInfoTable->m_ItemMallInfoList.size();

	




	lua_pushnumber(L, count);
	return 1;
}
// ----------------------------------------------------------------------------
int LuaFunc_GetMerchantFilterNums(lua_State* L)
{

	int ret=0;
	if (g_pItemMallFrame)
	 ret=g_pItemMallFrame->GetItemTypeNums();
	lua_pushnumber(L, ret);

	
	return 1;	
}

// ----------------------------------------------------------------------------
int LuaFunc_GetMerchantFilterInfo(lua_State* L)
{
	int index = luaL_checkint(L, 1) -1;

	int s1,s2,s3;
	int ret=0;
	if (g_pItemMallFrame)
	 ret=g_pItemMallFrame->GetItemType(index,s1,s2,s3);
	int count=0;

	char Name[128];
	int expand=0;
	const char* pName="unDefine";
	if (ret>0) // ret ==0 || ret==-1
	{

		char Key[128];
		
		if (s2==0&&s3==0)
			sprintf( Key , "SYS_ACCOUNTSTORE_TYPE_%02d" , s1 );
		else if (s3==0)
			sprintf( Key , "SYS_ACCOUNTSTORE_TYPE_%02d_%02d" , s1 ,s2);
		else
			sprintf( Key , "SYS_ACCOUNTSTORE_TYPE_%02d_%02d_%02d" , s1 ,s2,s3);
		//對應不同語系
		/*{		
			char localKey[128];		
			const char* language = g_pGameMain->GetLanguage();
			sprintf( localKey , "%s_%s" , Key, language);
			pName = g_ObjectData-> GetString( localKey );
		}
		if (strlen(pName)<=0)*/

		 pName = g_ObjectData-> GetString( Key );

		 if (strlen(pName)>0)
		 {

			 const char * pName2 = g_ObjectData-> GetString( pName );
			 if (strlen(pName2)>0)
				 pName=pName2;

		 }

		 Name[0]=0;
		
		 strcat(Name,strlen(pName)>0 ?pName:Key);
		 SellType* type= g_pItemMallFrame->GetType(ret);
		if (type)
		{
			count=type->m_count;		
			if (type->m_child.size())
				expand=1;
			

		}
	}
	
	lua_pushnumber(L, ret);

	lua_pushstring(L, Name);
	lua_pushnumber(L, count);
	lua_pushnumber(L, s1);
	lua_pushnumber(L, s2);
	lua_pushnumber(L, s3);
	lua_pushnumber(L, expand);
	
	return 7;
}

// ----------------------------------------------------------------------------
int LuaFunc_SetMerchantFilterType(lua_State* L)
{
	int type = (int)lua_tonumber(L, 1);
	if (g_pItemMallFrame)
	g_pMerchantFrame->SetFilterType( type );
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GetMerchantFilterType(lua_State* L)
{
	int ret=0;
	if (g_pItemMallFrame)
		ret=g_pMerchantFrame->GetFilterType();
	lua_pushnumber( L, ret );
	return 1;	
}

// ----------------------------------------------------------------------------
/*
int LuaFunc_PickupMerchantListItem(lua_State* L)
{
	int index = luaL_checkint(L, 1) - 1;
	g_pItemMallFrame->PushShoppingCart(index);
	return 0;
}
*/

// ----------------------------------------------------------------------------
/*
int	LuaFunc_GetNumShoppingCartItems(lua_State* L)
{
	lua_pushnumber(L, g_pItemMallFrame->GetShoppingCart().size());
	return 1;
}
*/
// ----------------------------------------------------------------------------
/*
int	LuaFunc_GetShoppingCartItem(lua_State* L)
{
	// 界面物品項目由1開始
	int index = luaL_checkint(L, 1) - 1;

	vector<ShoppingCartItem>& shoppingCart = g_pItemMallFrame->GetShoppingCart();
	if ( index >= 0 && index < (int)shoppingCart.size() )
	{
		ShoppingCartItem& item = shoppingCart[index];
		GameObjDbStructEx* Obj = g_ObjectData->GetObj(item.objectID);
		if ( Obj )
		{
			lua_pushstring(L, Obj->ImageObj->ACTField);				// Icon
			lua_pushnumber(L, item.price);							// price
			lua_pushnumber(L, (item.stock > 1) ? item.stock : 0);	// stock
			lua_pushstring(L, Obj->Name);							// Name
			lua_pushstring(L, Obj->ShortNote);						// explain

			int time = -1;
			switch (Obj->LiveTimeType)
			{
			case EM_ItemLiveTimeType_None:			//不會消失
				break;

			case EM_ItemLiveTimeType_RealTime:		//真實時間
				time = Obj->LiveTime;
				break;
			}
			lua_pushnumber(L, time);								// 生存時間

		


		}
		return 6;
	}
	return 0;
}
*/
// ----------------------------------------------------------------------------
int LuaFunc_MF_ShoppingBuy(lua_State* L)
{
	int index = luaL_checkint(L, 1) - 1;
	//const char *password=luaL_checkstring(L, 2) ;
	const char *PassWord=g_pGameMain->GetSecondPassword();

	if (g_pItemMallFrame)
	g_pItemMallFrame->ShoppingBuy(index,PassWord);
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_CIMF_ShoppingBuy(lua_State* L)
{
	int GUID = luaL_checkint(L, 1);
	int count = luaL_checkint(L, 2);
	if (count<=0)
		count=1;
	if (count>100)
		count=100;

	//const char *password=luaL_checkstring(L, 2) ;
	const char *PassWord=g_pGameMain->GetSecondPassword();

	if (g_pItemMallFrame)
	g_pItemMallFrame->ShoppingBuyByGuid(GUID,count,PassWord);
	return 0;
}


// ----------------------------------------------------------------------------
int LuaFunc_GameBarClose(lua_State* L)
{
	g_pMerchantFrame->SaveGamebarLog();
	return 0;
}


// ----------------------------------------------------------------------------
//要求兌換物品
// ----------------------------------------------------------------------------
int LuaFunc_ItemExchangeRequest(lua_State* L)
{
	const char* ItemSerial = luaL_checkstring( L, 1 ) ;
	const char* ItemKey  = luaL_checkstring( L, 2 ) ;

	if (g_pItemMallFrame)
	NetCli_DepartmentStoreChild::SL_ExchangeItemRequest( ItemSerial , ItemKey );

	return 0;
}



// ----------------------------------------------------------------------------
//打開界面 向Server要ItemMall Info
// ----------------------------------------------------------------------------
int LuaFunc_ItemMallFrame_Open(lua_State* L)
{
	

if (g_pItemMallFrame)
	g_pItemMallFrame->Open();

return 0;
}

//-----------------------------------------------------
int LuaFunc_GC_RuneReload(lua_State *L) 
{

	//int cc = lua_toboolean( L , 1 ) != 0;



	if (g_pGameMain->getMerchant()&&g_pItemMallFrame)
        g_pItemMallFrame->RuneReloadClick(lua_toboolean(L, 1) != 0);
		//g_pItemMallFrame->RuneReloadClick(cc);

	if(g_pGameMain->getMerchant()) 
	NetCli_DepartmentStore::SL_ImportAccountMoney();

	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GetItemMallLink(lua_State* L)
{
	int Guid = luaL_checkint(L, 1) ;

	DepartmentStoreBaseInfoStruct*  item=NULL;
	if(g_pItemMallFrame) 
	item=g_pItemMallFrame->GetItemInfo(-1, Guid);
	if (item)
	{
		ItemFieldStruct* pItemField = &item->Item;

		string link;
		if( !ItemField_To_ItemLink( pItemField, link, true ) )
		{
			return 0;
		}

		lua_pushstring(L, link.c_str() );
	return 1;
	}
return 0;


}

// ----------------------------------------------------------------------------
int LuaFunc_GetMerchantItemLink(lua_State* L)
{
	int index = luaL_checkint(L, 1) - 1;

	ItemMallInfo *pInfoTable = NULL;
	if(g_pItemMallFrame) 
		pInfoTable =g_pItemMallFrame->GetItemMallTable();
	if ( pInfoTable == NULL )
		return 0;

	int count =(int)pInfoTable->m_ItemMallInfoList.size();
	if ( index >= 0 && index < count )
	{
		DepartmentStoreBaseInfoStruct& item = *pInfoTable->m_ItemMallInfoList[index];

		ItemFieldStruct* pItemField = &item.Item;

		string link;
		if( !ItemField_To_ItemLink( pItemField, link, true ) )
		{
			return 0;
		}

		lua_pushstring(L, link.c_str() );

		return 1;
	}
	return 0;

}

// ----------------------------------------------------------------------------
//打開界面2 向Server要ItemMall Info
// ----------------------------------------------------------------------------



int LuaFunc_CIMF_OpenMall2(lua_State* L)
{
	bool IsDisableSendGift=false;
	if (g_pItemMallFrame)
	IsDisableSendGift=	g_pItemMallFrame->Open(true);

	lua_pushboolean( L, IsDisableSendGift);

	return 1;

}
// ----------------------------------------------------------------------------
//打開界面2 向Server要ItemMall Info
// ----------------------------------------------------------------------------
int LuaFunc_CIMF_CloseMall2(lua_State* L)
{
	if (g_pItemMallFrame != NULL)
		g_pItemMallFrame->Close();

	return 0;
}


int LuaFunc_CIMF_GetNums(lua_State* L)
{
	//int index = luaL_checkint(L, 1) - 1;
	int count=0;
	if (g_pItemMallFrame)
		count=g_pItemMallFrame->IM2_GetNums();
	
	lua_pushnumber( L, count);

	return 1;

}

// ----------------------------------------------------------------------------
//打開界面2 向Server要Item Info
// ----------------------------------------------------------------------------

int LuaFunc_CIMF_GetItemInfo(lua_State* L)
{

	int index = luaL_checkint(L, 1) - 1;
	int GUID = luaL_checkint(L, 2) ;
	char name[512];
	char *IconText=NULL;
	int SelltypeX=0;
	int diamond=0;
	int bouns=0;
	int free=0;
	int flash=0;


	char  countMsg[2048];	
	char  timeMsg[2048];
	char  acc_MSG[2048];
	char  freed_bouns[2048];
	char  Detail[2048];
	int atCount=0;
	int get_bouns=0;

	countMsg[0]=0;	
	timeMsg[0]=0;	
			


	int DCount=1;
	

	const char *TextureName=NULL;

	int sellNum=0;



	if (g_pItemMallFrame)
	{
		DepartmentStoreBaseInfoStruct* item =  g_pItemMallFrame->GetItemInfo(index,GUID);	

		if (item)
		{
			GUID=item->GUID;
			
			GameObjDbStructEx* Obj = g_ObjectData->GetObj(item->Item.OrgObjID);
			if (Obj)
			{

				GameObjDbStructEx* ImageObj=Obj;	
				

				strcpy(name,ImageObj->GetName());
				if (lstrlenW(item->ItemName)>0)
					WCharToUTF8(item->ItemName,name,512);
				SelltypeX=item->Sell.Icon;


				diamond=item->Sell.Cost;
				bouns=item->Sell.Cost_Bonus;
				free=item->Sell.Cost_Free;

				get_bouns=item->Sell.Get_Bonus;
					freed_bouns[0]=0;

				if (get_bouns>0)
				{	
					const char *pMsg=g_ObjectData->GetString("IMF_STR_BONUS");
					wsprintf(freed_bouns,pMsg,get_bouns );
				}

			//	time_t NowTime;
				time_t NowTime =_get_NowTime();// g_pGameMain->GetGameTime() + g_pGameMain->GetServerTimeZone()*60*60;
				//time( &NowTime );

				time_t 	time1=_ServerTimeToUTC(TimeExchangeFloatToInt(item->Sell.EndTime));

				int df=(int)difftime(time1,NowTime);
				int  sec=df;
				int mine=sec/60;
				int hour=mine/60;

				float par=100;
				


				DCount=item->Item.Count;
				atCount=item->Sell.MaxCount-(item->Sell.OrgCount+item->Sell.Count);
				if (atCount<0)
					atCount=0;

				if (item->Sell.MaxCount>0)
					par=(float)(atCount)/item->Sell.MaxCount;

				if (hour<1||par<0.1)
					flash=1;

#ifdef _DEBUGX
		if ((rand()%10)==1)
		flash=1;
#endif
		

			
		
				if (item->Item.ImageObjectID>0)
					ImageObj = g_ObjectData->GetObj(item->Item.ImageObjectID);

				if ( ImageObj == NULL )
					ImageObj = Obj;

				GameObjDbStructEx* imageDB = CNetGlobal::GetObj(ImageObj->ImageID);
				if ( imageDB )
				{
					TextureName = imageDB->Image.ACTField;
				}

				 DCount= (item->Item.Count >0) ? item->Item.Count : 1;


				const char *pAmountMsg=g_ObjectData->GetString("IMF_STR_AMOUNT");
				 strcpy(acc_MSG,pAmountMsg );

				 sellNum=item->Sell.OrgCount;

				 string ms=ItemUTIL::GetItemNote(Obj);

				 if (	lstrlenW(item->Note)>0 )					
				 {
					 char temp[512];
					 WCharToUTF8(item->Note,temp,512);
					 ms=temp;


				 }

				 strncpy(Detail,ms.c_str(),2048);
		
				 if (item->Sell.MaxCount>0)
				 {			
					 const char * msg=	g_ObjectData->GetString("ITEMMALL_TEXT_7");
					 int count=item->Sell.MaxCount-(item->Sell.OrgCount +item->Sell.Count);
					 if (count<0)
						 count=0;
					 sprintf(countMsg,msg,item->Sell.MaxCount/DCount,count/DCount);	
				 }

				 atCount=atCount/DCount;

				if (item->Sell.EndTime)
				{
			char BeginTime[512];
			char EndTime[512];
			time_t 	time1=_ServerTimeToUTC(TimeExchangeFloatToInt(item->Sell.BeginTime));

			time_t 	time2=_ServerTimeToUTC(TimeExchangeFloatToInt(item->Sell.EndTime));
			struct tm *gmt;

			struct tm gmt1;
			gmt = localtime( &time1 );	
			if (gmt)
				gmt1=*gmt;
			struct tm gmt2;
			gmt = localtime( &time2 );	
			if (gmt)
				gmt2=*gmt;
			
			if ((gmt1.tm_year+1900)>=2000)
			{
				const char * msg=	g_ObjectData->GetString("ITEMMALL_TEXT_1");
				time_t localtime=time1 ;//+ g_pGameMain->GetServerTimeZone()*60*60;
				sprintf(BeginTime,msg,TimeStr::DateTimeStr((int)localtime));
				strcat(timeMsg,BeginTime);
			}
			if ((gmt2.tm_year+1900)<=2030)
			{
				const char * msg=	g_ObjectData->GetString("ITEMMALL_TEXT_6");

				// 加上時區時間差
				time_t NowTime =_get_NowTime();// g_pGameMain->GetGameTime() + g_pGameMain->GetServerTimeZone()*60*60;
				//time( &NowTime );


				int df = (int)difftime(time2, NowTime);
				int sec = df;
				
				int mine=sec/60;
				sec=sec%60;
			
				int hour=mine/60;
					
					mine=mine%60;
				int day=hour/24;
				hour=hour%24;
				char DStr[256];
				char TStr[256];
				time_t localtime=time2 ;//+ g_pGameMain->GetServerTimeZone()*60*60;
				sprintf(DStr,"%s\n",TimeStr::DateTimeStr((int)localtime));
				//sprintf( DStr , "%d/%02d/%02d" , 1900+ gmt2.tm_year , 1 + gmt2.tm_mon , gmt2.tm_mday );

				sprintf( TStr , "%02d:%02d:%02d:%02d" , day ,hour,mine,sec );
				sprintf( EndTime , msg , DStr , TStr );

				
				strcat(timeMsg,"\n");
				strcat(timeMsg,EndTime);
			}

		}
				}
	

		}

	}

	lua_pushnumber(L, GUID );//名字
	lua_pushstring(L, name );//名字
	lua_pushstring(L, TextureName );//ICON貼圖
	lua_pushstring(L, acc_MSG );//個數
	

	lua_pushnumber( L, SelltypeX );//new hot 
	lua_pushnumber( L, diamond );
	lua_pushnumber( L, bouns );
	lua_pushnumber( L, free );
	lua_pushstring( L, freed_bouns);

	lua_pushnumber( L, flash);

	lua_pushnumber( L, DCount);



	lua_pushstring(L, countMsg );

	lua_pushstring(L, timeMsg );
	lua_pushstring(L, Detail );


	lua_pushnumber(L, atCount );
	lua_pushnumber(L, get_bouns );

	return 16;

}
// ----------------------------------------------------------------------------
//要Type  Filter Button Nums
// ----------------------------------------------------------------------------


int LuaFunc_CIMF_GetFilterNums(lua_State* L)
{
  int count=0;



  if (g_pItemMallFrame)
	count=  g_pItemMallFrame->IM2_GetFilterNums();
  


	lua_pushnumber( L, count);
 return 1;

}

int LuaFunc_CIMF_MailGift(lua_State* L)
{
	 int ItemGUID = luaL_checkint(L, 1);
	const  char* Password =g_pGameMain->GetSecondPassword();
	 const char* TargetName =luaL_checkstring(L, 2);
	 const char* Title =luaL_checkstring(L, 3);
	 const char* Content =luaL_checkstring(L, 4);



	 if (g_pItemMallFrame)
		 g_pItemMallFrame->MailGift(ItemGUID,Password,0,TargetName,Title,Content);
	 return 0;
}

//--------------------------------------------------
int LuaFunc_CIMF_SelectType(lua_State* L)
{

	int type = luaL_checkint(L, 1) -1;

	 if (g_pItemMallFrame)
	g_pItemMallFrame->SelectType(type);

	return 0;

}
//--------------------------------------------------

int LuaFunc_CIMF_GetFilterInfo(lua_State* L)
{

	


	int index = luaL_checkint(L, 1) -1;

	int s1,s2,s3;
	int ret=0;
	 if (g_pItemMallFrame)
	ret=g_pItemMallFrame->IM2_GetItemType(index,s1,s2,s3);


	int count=0;

	char Name[128];
	int expand=0;
	const char* pName="unDefine";
	if (ret>0) // ret ==0 || ret==-1
	{

		//SellType2* type= g_pItemMallFrame->IM2_GetType(ret);

	


		char Key[128];

		if (s2==0&&s3==0)
			sprintf( Key , "SYS_ACCOUNTSTORE_TYPE_%02d" , s1 );
		else if (s3==0)
			sprintf( Key , "SYS_ACCOUNTSTORE_TYPE_%02d_%02d" , s1 ,s2);
		else
			sprintf( Key , "SYS_ACCOUNTSTORE_TYPE_%02d_%02d_%02d" , s1 ,s2,s3);

		pName = g_ObjectData-> GetString( Key );

		if (strlen(pName)>0)
		{

			const char * pName2 = g_ObjectData-> GetString( pName );
			if (strlen(pName2)>0)
				pName=pName2;

		}

		Name[0]=0;

		strcat(Name,strlen(pName)>0 ?pName:Key);
		SellType2* type= g_pItemMallFrame->IM2_GetType(ret);
		if (type)
		{
			
			count=(int)type->m_items.size();		
			if (type->m_child.size())
			{
				
				expand=1;
			}


		}
	}

	lua_pushnumber(L, ret);

	lua_pushstring(L, Name);
	lua_pushnumber(L, count);
	lua_pushnumber(L, s1);
	lua_pushnumber(L, s2);
	lua_pushnumber(L, s3);
	lua_pushnumber(L, expand);

	return 7;
}

int LuaFunc_CIMF_SelectFilterIndex(lua_State* L)
{

	int index = luaL_checkint(L, 1) ;

	int state = luaL_checkint(L, 2) ;



	if (index==-1)
		index=index&0xff0000;

 if (g_pItemMallFrame)
	g_pItemMallFrame->IM2_SetSelcetTypeID(index,state);
	

	//int s1,s2,s3;
	//int ret=g_pItemMallFrame->IM2_GetItemType(index,s1,s2,s3);

	


	return 0;
}

//----------------------------------------------------------
int LuaFunc_CIMF_GetTopItem(lua_State* L)
{
	int GUID=0;
	int index = luaL_checkint(L, 1)-1 ;


	 if (g_pItemMallFrame)
	GUID=	g_pItemMallFrame->IM2_GetTopItem(index);



	lua_pushnumber(L, GUID);

	return 1;
}


int LuaFunc_CIMF_GetListCount(lua_State* L)
{
	int count=0;
 if (g_pItemMallFrame)
		count=g_pItemMallFrame->GetListCount();


		lua_pushnumber(L, count);

return 1;
}

int LuaFunc_CIMF_GetListName(lua_State* L)
{
	int index = luaL_checkint(L, 1)-1 ;
	const char *name="NULL";

	 if (g_pItemMallFrame)
		name=g_pItemMallFrame->GetListName(index);

	lua_pushstring(L, name);

	return 1;
}
int LuaFunc_CIMF_SearchItem(lua_State* L)
{
	const char * keyword=luaL_checkstring(L,1);


	 if (g_pItemMallFrame)
		g_pItemMallFrame->SearchItem(keyword);

	return 0;
}

int LuaFunc_CIMF_GetMessage(lua_State* L)
{
int index = luaL_checkint(L, 1) ;
	const char *Message="NULL";

	 if (g_pItemMallFrame)
		Message=g_pItemMallFrame->GetMessage(index);

	lua_pushstring(L, Message);

	return 1;

}
int LuaFunc_CIMF_GetMessageCount(lua_State* L)
{

	int count=0;
	 if (g_pItemMallFrame)
		count=g_pItemMallFrame->GetMessageCount();
	
	
	lua_pushnumber(L, count);
	return 1;
}
int LuaFunc_CIMF_ClearHistory(lua_State* L)
{

	 if (g_pItemMallFrame)
		g_pItemMallFrame->ClearHistory();
	return 0;
}
int LuaFunc_CIMF_SetHistory(lua_State* L)
{

	int GUID = luaL_checkint(L, 1) ;
	

	 if (g_pItemMallFrame)
		g_pItemMallFrame->SetHistory(GUID);

	

	return 0;
}
int LuaFunc_IsEnableMerchant(lua_State* L)
{

	lua_pushboolean(L,g_pGameMain->getMerchant());

	return 1;
}

//------------------------------------------------------------------
int LuaFunc_IsEnableGameBar(lua_State* L)
{
int cost = g_ObjectData->GetMoneyKeyValue( "SlotMachineCost" , 0 );
	lua_pushboolean(L,cost>0);
	return 1;
}

//------------------------------------------------------------------
int LuaFunc_GetGameBarFreeCount(lua_State* L)
{

	RoleDataEx*		Role		= CNetGlobal::RoleData();

	int   FreeCount=(int)Role->GetValue(EM_RoleValue_SlotMachineFreeCount);

	lua_pushnumber(L, FreeCount);
	return 1;
}
//---
int LuaFunc_GameBar_CanPlay(lua_State* L)
{
	int   bCanPlay=false;
	RoleDataEx*			pRole	= CNetGlobal::RoleData();
	if (pRole)
	{
		int pos=pRole->SearchBodyFirstFit( NULL, false );	
		if (pos!=-1)
		{

			bCanPlay=true;
		}
		else
		{
			const char *Error=g_ObjectData->GetString("DRAWRUNE_STR_ITEMQUEUEERROR");

			g_pGameMain->SendWarningMsg(Error);

		}
	}	 
	lua_pushboolean(L, bCanPlay);
	return 1;
}

int LuaFunc_GetGameBarItemInfo(lua_State* L)
{

	int index = luaL_checkint(L, 1) -1;
	int dbid=-1;
	int Count=0;
	const char *TextureName = NULL;
	 if (g_pItemMallFrame)
	 {
		 if (index>=0&&index<10)
		 {
			Count=  g_pItemMallFrame->m_BarItemCount[index];
			dbid=  g_pItemMallFrame->m_BarItemID[index];

			GameObjDbStructEx* Obj = g_ObjectData->GetObj(dbid);

			
			if (Obj)
			{
				TextureName= Obj->Image.ACTField;

				GameObjDbStructEx* imageDB = g_ObjectData->GetObj(Obj->ImageID);			
				if ( imageDB )
				{
					TextureName = imageDB->Image.ACTField;
				}

			}
		 }
	 }


	lua_pushstring(L, TextureName);	
	lua_pushnumber(L, dbid);
	lua_pushnumber(L, Count);


	return 3;
}	
//---
int LuaFunc_GetGameBarStart(lua_State* L)
{

		NetCli_DepartmentStore::SL_SlotMachinePlay();

		int cost = g_ObjectData->GetMoneyKeyValue( "SlotMachineCost" , 0 );


		RoleDataEx*		Role		= CNetGlobal::RoleData();

		int   FreeCount=(int)Role->GetValue(EM_RoleValue_SlotMachineFreeCount);
		if (FreeCount<=0)
		{

			g_pMerchantFrame->m_GameBarCost=cost;

		}
		else
			g_pMerchantFrame->m_GameBarCost=0;


		return 0;
}

int LuaFunc_GetGameBarPlayCost(lua_State* L)
{


	int cost = g_ObjectData->GetMoneyKeyValue( "SlotMachineCost" , 0 );
	//SlotMachineMaxFreeCount


	lua_pushnumber(L, cost);
	return 1;
}
int LuaFunc_GameBar_Stoped(lua_State* L)
{
	g_pItemMallFrame->GetMachineInfo();
	
	NetCli_DepartmentStore::SL_GetSlotMachineItem();


	return 0;
}
int LuaFunc_GameBar_GetHistory(lua_State* L)
{

	int count=(int)g_pItemMallFrame->m_gameBarHistoryLogs.size();
	int index= luaL_checkint(L, 1) ;


	if (index<0||index>count)
		return 0;
	

	

	const char *TextureName=NULL;

	const char *Name=NULL;
	char Date[128];
	int Count=0;


	GameBarHistoryLog &v=g_pItemMallFrame->m_gameBarHistoryLogs[index];
	
	GameObjDbStructEx* Obj = g_ObjectData->GetObj(v.dbid);
	if (Obj)
	{
		

		TextureName= Obj->Image.ACTField;

		GameObjDbStructEx* imageDB = g_ObjectData->GetObj(Obj->ImageID);			
		if ( imageDB )
		{
			TextureName = imageDB->Image.ACTField;
		}
		
		Name=Obj->Name;
		sprintf(Date,"%s",TimeStr::DateTimeStr(v.date));
		Count=v.count;
		
		
	}





	lua_pushstring(L, TextureName);	
	lua_pushstring(L, Name);	
	lua_pushnumber(L, Count);
	lua_pushstring(L, Date);	
	lua_pushnumber(L, v.cost);	
	lua_pushnumber(L, v.dbid);	

	return 6;
}

int LuaFunc_GameBar_GetHistorys(lua_State* L)
{
	
	

	lua_pushnumber(L, (lua_Number)g_pItemMallFrame->m_gameBarHistoryLogs.size());
	return 1;
}

int LuaFunc_OpenTopupPage(lua_State* L)
{
	if (g_pMerchantFrame == NULL)
		return 0;

	if (g_pMerchantFrame->GetTopupStatus() != EM_TopupStatus_StandBy)
		return 0;

	std::string Url = luaL_checkstring(L, 1);
	g_pMerchantFrame->TopupOpenSession(Url.c_str());

	return 0;
}

int LuaFunc_ItemMallHelpContent(lua_State* L)
{
	if ( g_pGameMain->GetUpdateInfo().itemMallHelpContent )
	{
		const char* content = g_ObjectData->GetString("ITEMMALL_QA");
		if ( content )
		{
			lua_pushstring(L, content);
			return 1;
		}
	}

	return 0;
}

int LuaFunc_IsEnableZShop(lua_State* L)
{
	lua_pushboolean(L, g_pGameMain->GetEnableZShop());
	return 1;
}

int LuaFunc_SetOnTimeWebBagRequest(lua_State* L)
{
	NetCli_DepartmentStoreChild::g_OnTimeWebBagRequest = (luaL_checkint(L, 1) > 0);
	return 0;
}

int LuaFunc_OpenZShop(lua_State* L)
{
	if (g_pMerchantFrame == NULL)
		return 0;

	if (g_pMerchantFrame->GetZShopStatus() != EM_ZShopStatus_StandBy)
		return 0;

	g_pMerchantFrame->ZShopOpenSession();

	return 0;
}

int LuaFunc_GetItemLink(lua_State* L)
{
	int GUID = luaL_checkint(L, 1) ;
	std::string Result;

	if(ItemGUID_To_ItemLink(GUID, Result) == false)
		return 0;

	lua_pushstring(L, Result.c_str());
	return 1;
}