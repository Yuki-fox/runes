#pragma once

#include <map>
#include <vector>
#include <string>
#include "NetCli_AC.h"

using namespace std;

struct AuctionBuyItemInfo
{
	ACItemInfoStruct	data;
	string				sellerName;
	bool				isHigher;
};

struct AuctionSellItemInfo
{
	ACItemInfoStruct	data;
	string				buyerName;
};

struct AuctionBrowseItemInfo
{	
	ACItemInfoStruct	data;
	int					index;
	bool				isBuyer;
	string				sellerName;
};

struct AuctionHistoryItemInfo
{
	StaticString <_MAX_NAMERECORD_SIZE_>	SellerName;		// 賣方名字
	StaticString <_MAX_NAMERECORD_SIZE_>	BuyerName;		// 買方名字
	PriceTypeENUM		priceType;							// 幣值
	int					price;								// 賣價
	int					data;	
	int                 count; 
	ItemFieldStruct		item;								//賣的東西
};

class NetCli_ACChild : public NetCli_AC
{
public:	
	static bool _Init();
	static bool _Release();
	
	static	void	ClearBrowseList();
	static	void	ClearBuyList();
	static	void	ClearSellList();
	static	void	AddBuyItem(ACItemInfoStruct& Data , char* SellerName , int BuyerDBID);
	static	void	DeleteBuyItem(int ACItemDBID);
	static	void	UpdateBuyItem(int ACItemDBID, ACItemStateENUM state, int newPrices , PriceTypeENUM PricesType , int buyerDBID);
	static	void	AddSellItem(ACItemInfoStruct& Data , char* BuyerName);	
	static	void	DeleteSellItem(int ACItemDBID);
	static	void	UpdateSellItem(int ACItemDBID, ACItemStateENUM state, int prices, char* buyer);
	static	int		GetBrowseMaxPages();

	static	int		GetBuyNumItems()		{ return (int)s_buyList.size(); }
	static	int		GetSellNumItems()		{ return (int)s_sellList.size(); }
	static	int		GetBrowseNumItems()		{ return (int)s_browseList.size(); }
	static	int		GetHistoryNumItems()	{ return (int)s_history.size(); }
	static	int		GetBrowseMaxNums()		{ return s_browseMaxCount; }
	static	int		GetBrowseNextCount()	{ return s_borwseNextCount; }
	static	AuctionBuyItemInfo*		GetBuyItem(int index);
	static	AuctionSellItemInfo*	GetSellItem(int index);
	static	AuctionBrowseItemInfo*	GetBrowseItem(int index);
	static	AuctionHistoryItemInfo*	GetHistoryItem(int index);

	static	void	CreateMoney				( PriceTypeENUM	 SellType , int	SellMoney ,	int Prices , int BuyOutPrices , int LiveTime , PriceTypeENUM PriceType , const char* Password );

public:
	virtual void RD_SearchItemCountResult	( int MaxCount , int Count , int BeginIndex );
	virtual void RD_SearchItemResult		( int Index , ACItemInfoStruct& Data , char* SellerName  , int BuyerDBID );
	virtual void RD_FixSearchItemResult		( int ACItemDBID , ACItemStateENUM State , int NewPrices , PriceTypeENUM Type , int BuyerDBID );
	virtual void RD_SelfCompetitive_BuyItem	( ACItemInfoStruct& Data , char* SellerName , int BuyerDBID  );
	virtual void RD_SelfCompetitive_SellItem( ACItemInfoStruct& Data , char* BuyerName );
	virtual void RD_SelfCompetitive_ItemDataEnd(  );
	virtual void RD_CancelSellItemResult	( int ACItemDBID );
	virtual void RD_CloseAC					( );
	virtual void RD_OpenAC					( int NpcGUID , bool Result );
	virtual void RD_LostCompetitiveItem		( int ACItemDBID , ItemFieldStruct& Item );
	virtual void RD_FixBuyOrderItem			( int ACItemDBID , ACItemStateENUM State , int NewPrices , PriceTypeENUM PricesType , int BuyerDBID );
	virtual void RD_FixSellOrderItem		( int ACItemDBID , ACItemStateENUM State , int NewPrices , PriceTypeENUM PricesType , char* BuyerName );
	virtual void RD_ItemHistoryListResult	( vector< DB_ACItemHistoryStruct >& ItemList );

	virtual void RL_BuyItemResult			( bool Result , int ACItemDBID , int Prices , PriceTypeENUM Type , ACItemStateENUM State );
	virtual void RL_SellItemResult			( ACSellItemResultENUM Result , ACItemInfoStruct& Item );
	virtual void RL_AcAlreadyOpen			( int TareTargtetNPC );
	virtual void RL_SellMoneyResult			( SellMoneyResultENUM Result , ACItemInfoStruct& Item );
	virtual void RL_AcTradeOk				( const char* BuyerName , const char* SellerName , int PriceType , int Money , int Tax , ItemFieldStruct& Item );
protected:
	static	vector<AuctionBuyItemInfo>		s_buyList;
	static	vector<AuctionSellItemInfo>		s_sellList;
	static	map<int, AuctionBrowseItemInfo>	s_browseList;
	static	vector<AuctionHistoryItemInfo>	s_history;

public:
	static	bool							s_isUpdateBuyList;
	static	bool							s_isUpdateSellList;
	static	int								s_browseMaxCount;
	static	int								s_borwseNextCount;
	static	int								s_browseLastIndex;
	static	int								s_searchItemSize;
	static	int								s_searchItemCount;

	static	PriceTypeENUM					s_createPriceType;
	static	int								s_createPrice;
	static	map<int, ACItemInfoStruct>		s_itemInfoTemp;
};

