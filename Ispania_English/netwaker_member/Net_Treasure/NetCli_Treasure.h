#pragma once

#include "PG/PG_Treasure.h"
#include "../../MainProc/NetGlobal.h"

class NetCli_Treasure : public CNetGlobal
{
protected:
    //-------------------------------------------------------------------
    static	NetCli_Treasure* This;
    static	bool _Init();
    static	bool _Release();	
    //-------------------------------------------------------------------
	static void _PG_Treasure_LtoC_BaseInfo			( int Sockid , int Size , void* Data );
	static void _PG_Treasure_LtoC_OpenFailed		( int Sockid , int Size , void* Data );	
	static void _PG_Treasure_LtoC_Open				( int Sockid , int Size , void* Data );
	static void _PG_Treasure_LtoC_GetItemResult		( int Sockid , int Size , void* Data );
public:    
    NetCli_Treasure() { This = this; }

	static vector<ItemFieldStruct>	s_booty;
	static	NetCli_Treasure*	GetThis()						{ return This; }
	static	void				ClearBooty()					{ s_booty.clear(); }
	static	int					GetBootyCount()					{ return (int)s_booty.size(); }
	static	ItemFieldStruct*	GetBootyItem(int index);
	

	static void SL_OpenRequest	( int ItemGUID );
	static void SL_GetItem		( int ItemGUID , int ItemSerial , int BodyPos );
	static void SL_Close		( int ItemGUID );

	virtual void R_BaseInfo( int ItemGUID , bool IsEmpty ) = 0;
	virtual void R_OpenFailed( int ItemGUID ) = 0;
	virtual void R_Open( int ItemGUID , int MaxCount , ItemFieldStruct* Items ) = 0;
	virtual void R_GetItemResult( int BodyPos , int ItemSerial , TreasureGetItemResultENUM Result ) = 0;
};

