#include "NetCli_ACChild.h"
#include "../Net_Script/NetCli_Script_Child.h"
#include "../../mainproc/GameMain.h"
#include "../Interface/WorldFrames/AuctionFrame.h"
#include "../Interface/WorldFrames/ChatFrame.h"
#include "../Interface/ItemLink/ItemLink.h"

vector<AuctionBuyItemInfo>		NetCli_ACChild::s_buyList;
vector<AuctionSellItemInfo>		NetCli_ACChild::s_sellList;
vector<AuctionHistoryItemInfo>	NetCli_ACChild::s_history;
map<int, AuctionBrowseItemInfo>	NetCli_ACChild::s_browseList;

bool							NetCli_ACChild::s_isUpdateBuyList;
bool							NetCli_ACChild::s_isUpdateSellList;
int								NetCli_ACChild::s_browseMaxCount;
int								NetCli_ACChild::s_borwseNextCount;
int								NetCli_ACChild::s_browseLastIndex;
int								NetCli_ACChild::s_searchItemSize;
int								NetCli_ACChild::s_searchItemCount;

PriceTypeENUM					NetCli_ACChild::s_createPriceType;
int								NetCli_ACChild::s_createPrice;

map<int, ACItemInfoStruct>		NetCli_ACChild::s_itemInfoTemp;

//-----------------------------------------------------------------------------
bool NetCli_ACChild::_Init()
{
	if (This)
		return false;

	This = new NetCli_ACChild;

	s_buyList.clear();
	s_sellList.clear();
	s_history.clear();
	s_browseList.clear();

	s_isUpdateBuyList = false;
	s_isUpdateSellList = false;

	s_browseMaxCount = 0;
	s_borwseNextCount = 0;
	s_browseLastIndex = 0;


	s_createPriceType = EM_ACPriceType_GameMoney;
	s_createPrice = 0;

	return NetCli_AC::_Init();
}

//-----------------------------------------------------------------------------
bool NetCli_ACChild::_Release()
{
	NetCli_AC::_Release();
	if (This)
	{
		delete This;
		This = NULL;
	}
	return true;
}
//-----------------------------------------------------------------------------
void NetCli_ACChild::ClearBrowseList()
{
	s_browseList.clear();
	s_browseMaxCount = 0;
	s_borwseNextCount = 0;
	s_browseLastIndex = 0;
}
//-----------------------------------------------------------------------------
void NetCli_ACChild::ClearBuyList()
{
	s_buyList.clear();
	s_isUpdateBuyList = true;
}

//-----------------------------------------------------------------------------
void NetCli_ACChild::ClearSellList()
{
	s_sellList.clear();
	s_isUpdateSellList = true;
}

//-----------------------------------------------------------------------------
void NetCli_ACChild::AddBuyItem(ACItemInfoStruct& Data , char* SellerName , int BuyerDBID)
{
	if ( SellerName == NULL )
		SellerName = "";

	//g_pError->AddMessage(0, 0, "[AddBuyItem] [ItemID %d] SellerName %s BuyerDBID %d", Data.DBID, SellerName, BuyerDBID);

	s_isUpdateBuyList = true;
	for ( vector<AuctionBuyItemInfo>::iterator iter = s_buyList.begin(); iter != s_buyList.end(); iter++ )
	{
		if ( (*iter).data.DBID == Data.DBID )
		{
			(*iter).isHigher	= (BuyerDBID == g_pGameMain->GetDBID());
			(*iter).data		= Data;
			(*iter).sellerName	= SellerName;
			return;
		}
	}

	AuctionBuyItemInfo info;
	info.isHigher	= (BuyerDBID == g_pGameMain->GetDBID());
	info.data		= Data;
	info.sellerName	= SellerName;
	s_buyList.push_back(info);
}
//-----------------------------------------------------------------------------
void NetCli_ACChild::DeleteBuyItem(int ACItemDBID)
{	
	for ( vector<AuctionBuyItemInfo>::iterator iter = s_buyList.begin(); iter != s_buyList.end(); iter++ )
	{
		if ( iter->data.DBID == ACItemDBID )
		{
			// �Ȧs���~��T
			s_itemInfoTemp.insert(make_pair(ACItemDBID, iter->data));

			s_buyList.erase(iter);
			s_isUpdateBuyList = true;
			return;
		}
	}
}
//-----------------------------------------------------------------------------
void NetCli_ACChild::UpdateBuyItem(int ACItemDBID , ACItemStateENUM state , int newPrices , PriceTypeENUM PricesType , int buyerDBID)
{
	for ( vector<AuctionBuyItemInfo>::iterator iter = s_buyList.begin(); iter != s_buyList.end(); iter++ )
	{
		if ( (*iter).data.DBID == ACItemDBID )
		{
			switch ( state )
			{
			case EM_ACItemState_Normal:
				s_isUpdateBuyList		= true;
				(*iter).data.NowPrices	= newPrices;
				(*iter).isHigher		= (g_pGameMain->GetDBID() == buyerDBID);
				break;

			case EM_ACItemState_BuyOut:
			case EM_ACItemState_Cancel:
				DeleteBuyItem(ACItemDBID);
				break;
			}
			break;
		}
	}
}
//-----------------------------------------------------------------------------
void NetCli_ACChild::AddSellItem(ACItemInfoStruct& Data , char* BuyerName)
{
	if ( BuyerName == NULL )
		BuyerName = "";

	s_isUpdateSellList = true;
	for ( vector<AuctionSellItemInfo>::iterator iter = s_sellList.begin(); iter != s_sellList.end(); iter++ )
	{
		if ( (*iter).data.DBID == Data.DBID )
		{
			(*iter).data = Data;
			(*iter).buyerName = BuyerName;
			return;
		}
	}

	AuctionSellItemInfo info;
	info.buyerName	= BuyerName;
	info.data		= Data;
	s_sellList.push_back(info);
}
//-----------------------------------------------------------------------------
void NetCli_ACChild::DeleteSellItem(int ACItemDBID)
{
	for ( vector<AuctionSellItemInfo>::iterator iter = s_sellList.begin(); iter != s_sellList.end(); iter++ )
	{
		if ( (*iter).data.DBID == ACItemDBID )
		{
			s_sellList.erase(iter);
			s_isUpdateSellList = true;

			// �������


			return;
		}
	}
}
//-----------------------------------------------------------------------------
void NetCli_ACChild::UpdateSellItem(int ACItemDBID, ACItemStateENUM state, int prices, char* buyer)
{
	for ( vector<AuctionSellItemInfo>::iterator iter = s_sellList.begin(); iter != s_sellList.end(); iter++ )
	{
		if ( (*iter).data.DBID == ACItemDBID )
		{
			switch ( state )
			{
			case EM_ACItemState_Normal:
				s_isUpdateSellList		= true;
				(*iter).buyerName		= buyer;
				(*iter).data.NowPrices	= prices;
				break;

			case EM_ACItemState_BuyOut:
			case EM_ACItemState_Cancel:
				DeleteSellItem(ACItemDBID);
				break;
			}
			break;
		}
	}
}
//-----------------------------------------------------------------------------
int NetCli_ACChild::GetBrowseMaxPages()
{
	return (s_browseLastIndex + s_borwseNextCount) / 50 + 1;
}
//-----------------------------------------------------------------------------
AuctionBuyItemInfo*	NetCli_ACChild::GetBuyItem(int index)
{
	if ( (unsigned int)index < s_buyList.size() )
		return &s_buyList[index];
	return NULL;
}
//-----------------------------------------------------------------------------
AuctionSellItemInfo* NetCli_ACChild::GetSellItem(int index)
{
	if ( (unsigned int)index < s_sellList.size() )
		return &s_sellList[index];
	return NULL;
}
//-----------------------------------------------------------------------------
AuctionBrowseItemInfo* NetCli_ACChild::GetBrowseItem(int index)
{
	map<int, AuctionBrowseItemInfo>::iterator iter = s_browseList.find(index);
	if ( iter != s_browseList.end() )
		return &(iter->second);
	return NULL;
}
//-----------------------------------------------------------------------------
AuctionHistoryItemInfo*	NetCli_ACChild::GetHistoryItem(int index)
{
	if ( (unsigned int)index < s_history.size() )
		return &s_history[index];
	return NULL;
}
//-----------------------------------------------------------------------------
void NetCli_ACChild::CreateMoney(PriceTypeENUM SellType, int SellMoney, int Prices, int BuyOutPrices, int LiveTime, PriceTypeENUM PriceType, const char* Password)
{
	if ( s_createPrice > 0 )
		return;

	s_createPrice = SellMoney;
	s_createPriceType = SellType;

	SL_SellMoney(SellType, SellMoney, Prices, BuyOutPrices, LiveTime, PriceType, Password);
}
//-----------------------------------------------------------------------------
void NetCli_ACChild::RD_SearchItemCountResult	( int MaxCount , int Count , int BeginIndex )
{
	//char	Buf[1024];
	//sprintf( Buf , "[AC�j�M] : MaxCount=%d Count=%d  BeginIndex=%d " , MaxCount , Count , BeginIndex  );
	//g_pChatFrame->AddMessage(5, Buf);

	if ( BeginIndex < 50 )
	{
		ClearBrowseList();

		s_browseMaxCount = MaxCount;	// �O���d�M�`����
		s_borwseNextCount = s_browseMaxCount;
	}

	s_borwseNextCount -= Count;
	s_searchItemSize = Count;			// ���o�榸�d�M�Ӽ�
	s_searchItemCount = 0;				// �p�ƾ�

	if ( MaxCount == 0 )
	{
		// �����ŦX�ӫ~
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_AC_SEARCH_EMPTY"));

		// �s������s,�D�ʰe�X��s�����ƥ�
		g_pAuctionFrame->SendWorldEvent("AUCTION_BROWSE_UPDATE");
	}
}
void NetCli_ACChild::RD_SearchItemResult( int Index , ACItemInfoStruct& Data , char* SellerName , int BuyerDBID )
{
	ItemFieldStruct& Item = Data.Item;
	GameObjDbStruct* ItemDB = GetObj( Item.OrgObjID );
	if( ItemDB == NULL )
		return;

	/*
	char	Buf[1024];
	sprintf( Buf , "[AC�j�M] : Index=%d ItemDBID=%d  Name=%s Type=%d" , Index , Data.DBID , ItemDB->Name , Data.Type );	
	g_pChatFrame->AddMessage(5 , Buf );
	*/

	RegisterItemFieldChecksum( &Data.Item );

	AuctionBrowseItemInfo info;
	info.data		= Data;
	info.index		= Index;
	info.isBuyer	= (g_pGameMain->GetDBID() == BuyerDBID);
	info.sellerName	= SellerName;
	s_browseList.insert(make_pair(Index, info));

	s_browseLastIndex = Index;
	s_searchItemCount++;

	if ( s_searchItemCount >= s_searchItemSize )
	{
		// �s������s,�D�ʰe�X��s�����ƥ�
		g_pAuctionFrame->SendWorldEvent("AUCTION_BROWSE_UPDATE");
	}
}
//-----------------------------------------------------------------------------
void NetCli_ACChild::RD_FixSearchItemResult		( int ACItemDBID , ACItemStateENUM	State , int NewPrices , PriceTypeENUM Type , int BuyerDBID )
{
	/*
	char	Buf[1024];
	sprintf(Buf , "AC���~�ץ� ACItemDBID=%d State=%d NewPrices=%d " , ACItemDBID , State , NewPrices);
	g_pChatFrame->AddMessage(5, Buf );
	*/

	map<int, AuctionBrowseItemInfo>::iterator iter = s_browseList.begin();
	for ( ; iter != s_browseList.end(); iter++ )
	{
		if ( (iter->second).data.DBID == ACItemDBID )
			break;
	}

	switch ( State )
	{
	case EM_ACItemState_Normal:
		if ( iter != s_browseList.end() )
		{
			lua_State* L = g_pAuctionFrame->GetLuaStateWorld();

			(iter->second).data.NowPrices = NewPrices;
			(iter->second).isBuyer = (g_pGameMain->GetDBID() == BuyerDBID);

			// �q��UI���浧��ƭץ�
			lua_pushnumber(L, iter->first);
			lua_setglobal(L, "arg1");
			lua_pushnumber(L, (iter->second).data.NowPrices);
			lua_setglobal(L, "arg2");
			lua_pushboolean(L, (iter->second).isBuyer);
			lua_setglobal(L, "arg3");
			g_pAuctionFrame->SendWorldEvent("AUCTION_BORWSE_PRICE_UPDATE");

			/*
			// �q��UI���浧��ƭץ�
			if ( BuyerDBID == g_pGameMain->GetDBID() )
			{
				lua_State* L = g_pAuctionFrame->GetLuaStateWorld();

				lua_pushnumber(L, iter->first);
				lua_setglobal(L, "arg1");
				lua_pushnumber(L, NewPrices);
				lua_setglobal(L, "arg2");
				g_pAuctionFrame->SendWorldEvent("AUCTION_BROWSE_BID_SUCCESS");
			}
			*/
		}
		break;

	case EM_ACItemState_BuyOut:
	case EM_ACItemState_Cancel:
		s_browseMaxCount--;
		if ( iter != s_browseList.end() )
		{
			map<int, AuctionBrowseItemInfo>::iterator perv_iter = iter;
			if ( perv_iter != s_browseList.begin() )
			{
				perv_iter--;
				s_browseLastIndex = perv_iter->first;
			}
			else
				s_browseLastIndex = 0;

			// �Ȧs���~��T
			s_itemInfoTemp.insert(make_pair(ACItemDBID, iter->second.data));

			// �R���ӵ����,��Ƥ����ץ�
			s_browseList.erase(iter);

			// �q��UI��s����
			if ( BuyerDBID == g_pGameMain->GetDBID() )
				g_pAuctionFrame->SendWorldEvent("AUCTION_BROWSE_UPDATE");
		}
		else
		{
			s_borwseNextCount--;
		}
		break;
	}	
}
//-----------------------------------------------------------------------------
void NetCli_ACChild::RD_SelfCompetitive_BuyItem	( ACItemInfoStruct& Data , char* SellerName , int BuyerDBID  )
{
	AddBuyItem(Data, SellerName, BuyerDBID);
}
//-----------------------------------------------------------------------------
void NetCli_ACChild::RD_SelfCompetitive_SellItem( ACItemInfoStruct& Data , char* BuyerName )
{
	AddSellItem(Data, BuyerName);
}
//-----------------------------------------------------------------------------
void NetCli_ACChild::RD_CancelSellItemResult	( int ACItemDBID )
{
	// �������
	for ( vector<AuctionSellItemInfo>::iterator iter = s_sellList.begin(); iter != s_sellList.end(); iter++ )
	{
		if ( (*iter).data.DBID == ACItemDBID )
		{
			string itemName;
			if ( ItemField_To_ItemLink(&(iter->data.Item), itemName) == false )
				itemName = g_ObjectData->GetItemColorName(&(iter->data.Item));			
			g_pChatFrame->SendChatMessageEx(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_AC_CANCEL_SELL_ITEM"), itemName.c_str());
			break;
		}
	}
	DeleteSellItem(ACItemDBID);	
}
//-----------------------------------------------------------------------------
void NetCli_ACChild::RD_CloseAC					( )
{
	//g_pChatFrame->AddMessage(5, "�n�D����AC" );

	ClearBrowseList();
	ClearBuyList();
	ClearSellList();
	s_itemInfoTemp.clear();
	g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_AC_CLOSE"));
	g_pAuctionFrame->SendWorldEvent("AUCTION_CLOSE");
}
//-----------------------------------------------------------------------------
void NetCli_ACChild::RD_OpenAC					( int NpcGUID , bool Result )
{
	//g_pChatFrame->AddMessage(5, "�n�D�}��AC" );

	ClearBrowseList();
	ClearBuyList();
	ClearSellList();
	g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_AC_OPEN"));
	g_pAuctionFrame->OpenAC(NpcGUID);
}
//-----------------------------------------------------------------------------
// ���~�ʶR�^��
void NetCli_ACChild::RL_BuyItemResult			( bool Result , int ACItemDBID , int Prices , PriceTypeENUM Type , ACItemStateENUM State )
{
	if ( Result )
	{
		switch (State)
		{
		case EM_ACItemState_Normal:
	
			{
				ACItemInfoStruct data;

				// �s������
				map<int, AuctionBrowseItemInfo>::iterator browse_iter = s_browseList.begin();
				for ( ; browse_iter != s_browseList.end(); browse_iter++ )
				{
					if ( (browse_iter->second).data.DBID == ACItemDBID )
					{
						data = (browse_iter->second).data;
						data.NowPrices = Prices;
						AddBuyItem(data, (char*)(browse_iter->second).sellerName.c_str(), g_pGameMain->GetDBID());


						string itemName;
						if ( ItemField_To_ItemLink(&(data.Item), itemName) == false )
							itemName = g_ObjectData->GetItemColorName(&(data.Item));
						g_pChatFrame->SendChatMessageEx(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_AC_BID_SUCCESS"), itemName.c_str());					
						return;
					}
				}

				// �v�ж���
				vector<AuctionBuyItemInfo>::iterator buy_iter = s_buyList.begin();
				for ( ; buy_iter != s_buyList.end(); buy_iter++ )
				{
					if ( (*buy_iter).data.DBID == ACItemDBID )
					{
						data = (*buy_iter).data;
						data.NowPrices = Prices;
						AddBuyItem(data, (char*)(*buy_iter).sellerName.c_str(), g_pGameMain->GetDBID());

									
						string itemName;
						if ( ItemField_To_ItemLink(&(data.Item), itemName) == false )
							itemName = g_ObjectData->GetItemColorName(&(data.Item));
						g_pChatFrame->SendChatMessageEx(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_AC_BID_SUCCESS"), itemName.c_str());
						return;
					}
				}
			}
			break;

		case EM_ACItemState_BuyOut:
			{
				map<int, ACItemInfoStruct>::iterator iter = s_itemInfoTemp.find(ACItemDBID);
				if ( iter != s_itemInfoTemp.end() )
				{
					ACItemInfoStruct data = iter->second;
					s_itemInfoTemp.erase(iter);

	
					string itemName;
					if ( ItemField_To_ItemLink(&(data.Item), itemName) == false )
						itemName = g_ObjectData->GetItemColorName(&(data.Item));
					g_pChatFrame->SendChatMessageEx(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_AC_BUYOUT_SUCCESS"), itemName.c_str());
					break;
				}
			}
			break;

		case EM_ACItemState_Cancel:
			break;
		}
	}
	else
	{
		g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_AC_BID_FAILED"));
	}
}
//-----------------------------------------------------------------------------
void NetCli_ACChild::RL_SellItemResult			( ACSellItemResultENUM Result , ACItemInfoStruct& Item )
{
	string itemName;
	if ( ItemField_To_ItemLink(&(Item.Item), itemName) == false )
		itemName = g_ObjectData->GetItemColorName(&(Item.Item));

	switch (Result)
	{
	case EM_ACSellItemResult_OK:				//��
		g_pChatFrame->SendChatMessageEx(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_AC_CREATE_ITEM_SUCCESS"), itemName.c_str());
		AddSellItem(Item, NULL);

		// Update Auction Auction Item
		g_pAuctionFrame->SetAuctionItemIndex(-1);
		break;

	case EM_ACSellItemResult_DataError:			//��ƿ��~
		g_pGameMain->SendWarningMsgEx(g_ObjectData->GetString("SYS_AC_CREATE_ITEM_FAILED"), itemName.c_str());
		break;

	case EM_ACSellItemResult_MaxCountError:		//�ƶq��F�W��
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_AC_CREATE_ITEM_MAXCOUNT"));
		break;

	case EM_ACSellItemResult_DisableTrade:		//�T����
		g_pGameMain->SendWarningMsgEx(g_ObjectData->GetString("SYS_AC_CREATE_ITEM_FAILED"), itemName.c_str());
		break;

	case EM_ACSellItemResult_MoneyError:		//�������~
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_AC_NOT_ENOUGH_MONEY"));
		break;
	}
}
//-----------------------------------------------------------------------------
void NetCli_ACChild::RD_LostCompetitiveItem( int ACItemDBID , ItemFieldStruct& Item )
{
	// �X���Q�W�L�F
	string itemName;
	if ( ItemField_To_ItemLink(&Item, itemName) == false )
		itemName = g_ObjectData->GetItemColorName(&Item);
	g_pChatFrame->SendChatMessageEx(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_AC_EXCEED"), itemName.c_str());
}
//-----------------------------------------------------------------------------
void NetCli_ACChild::RD_FixBuyOrderItem( int ACItemDBID , ACItemStateENUM State , int NewPrices , PriceTypeENUM PricesType , int BuyerDBID )
{
	UpdateBuyItem(ACItemDBID, State, NewPrices , PricesType , BuyerDBID);
}
//-----------------------------------------------------------------------------
void NetCli_ACChild::RD_FixSellOrderItem( int ACItemDBID , ACItemStateENUM State , int NewPrices , PriceTypeENUM PricesType , char* BuyerName )
{
	UpdateSellItem(ACItemDBID, State, NewPrices, BuyerName);
}
//-----------------------------------------------------------------------------
void NetCli_ACChild::RD_ItemHistoryListResult	( vector< DB_ACItemHistoryStruct >& ItemList )
{
	AuctionHistoryItemInfo info;

	s_history.clear();
	if ( ItemList.empty() ) {
		g_pAuctionFrame->SendWorldEvent("AUCTION_HISTORY_HIDE");
		g_pGameMain->SendSystemMsg(g_ObjectData->GetString("SYS_AC_HISTORY_NONE"));
	} else {
		for ( vector<DB_ACItemHistoryStruct>::iterator iter = ItemList.begin(); iter != ItemList.end(); iter++ )
		{
			DB_ACItemHistoryStruct &table=(*iter);
			info.SellerName = CNetCli_Script_Child::ReplaceSystemKeyword(table.SellerName.Begin()).c_str();
			info.BuyerName = CNetCli_Script_Child::ReplaceSystemKeyword(table.BuyerName.Begin()).c_str();
			info.priceType = table.PricesType;
			info.price = table.Prices;
			info.data = table.Date;
			info.item = table.Item;

			info.count = (*iter).Item.Count;
			s_history.push_back(info);
		}

		// ��s���v���
		g_pAuctionFrame->SendWorldEvent("AUCTION_HISTORY_SHOW");
	}	
}
//-----------------------------------------------------------------------------
void NetCli_ACChild::RD_SelfCompetitive_ItemDataEnd(  )
{
	int a = 0;
}
void NetCli_ACChild::RL_AcAlreadyOpen		( int TareTargtetNPC )
{
	// RD_OpenAC(TareTargtetNPC, true);
	g_pAuctionFrame->OpenAC(TareTargtetNPC);
}

void NetCli_ACChild::RL_SellMoneyResult			( SellMoneyResultENUM Result , ACItemInfoStruct& Item )
{
	switch (Result)
	{
	case EM_SellMoneyResult_OK:					// ��榨?
		/*
		{
			int itemID = g_ObjectData->GetMonetItemID(s_createPriceType);
			ObjectDataClass* itemDB = g_ObjectData->GetObj(itemDB);
			if ( itemDB == NULL )
			{
				// �s�����~
				g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_AC_CREATE_MONEY_ERR"));
			}
			else
			{


				GetMonetItemID
			}			
		}
		*/

		g_pChatFrame->SendChatMessageEx(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_AC_CREATE_MONEY_SUCCESS"));
		AddSellItem(Item, NULL);

		// Update Auction Auction Item
		g_pAuctionFrame->SetAuctionItemIndex(-1);
		break;
	case EM_SellMoneyResult_Err:				//�@����� (���i������A)
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_AC_CREATE_MONEY_ERR"));
		break;
	case EM_SellMoneyResult_MoneyErr:			//��������
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_AC_NOT_ENOUGH_MONEY"));
		break;
	case EM_SellMoneyResult_MoneyTypeErr:		//�������������D( �ιC�������C���� , �Υαb�������b���� )
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_AUCTION_MONEY_TYPE_ERR"));
		break;
	case EM_SellMoneyResult_PasswordErr:		// �G���K�X���~
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_GAMEMSGEVENT_014"));
		break;
	}	

	s_createPriceType = EM_ACPriceType_GameMoney;
	s_createPrice = 0;
}
void NetCli_ACChild::RL_AcTradeOk( const char* BuyerName , const char* SellerName , int PriceType , int Money , int Tax , ItemFieldStruct& Item )
{
	string itemName;
	if ( ItemField_To_ItemLink(&Item, itemName) == false )
		itemName = g_ObjectData->GetItemColorName(&Item);

	if ( strcmp(g_pGameMain->GetPlayerName(), SellerName) == 0 )	
		g_pChatFrame->SendChatMessageEx(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_AC_SELL_SUCCESS"), itemName.c_str());
	else
		g_pChatFrame->SendChatMessageEx(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_AC_BID_WINNER"), itemName.c_str());
}