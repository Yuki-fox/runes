#pragma once


#include <string>
using namespace std;

enum EMapIconType
{
	EM_MapIconType_None							= 0 ,
	EM_MapIconType_NewQuest						= 1 ,   //	新任務
	EM_MapIconType_UnfinishQuest				= 2 ,   //	未完成任務
	EM_MapIconType_FinishedQuest				= 3 ,   //	完成任務
	EM_MapIconType_TrustQuest					= 4 ,   // 委託任務
	EM_MapIconType_Bank							= 5 ,   // 銀行
	EM_MapIconType_MailBox						= 6 ,   // 郵筒
	EM_MapIconType_FlightPoint					= 7 ,   // 飛行點
	EM_MapIconType_AuctionHouse					= 8 ,   // 拍賣所
 	EM_MapIconType_Mine							= 9 ,   // 採集點-礦
	EM_MapIconType_Wood							= 10,   // 採集點-木
	EM_MapIconType_Herb							= 11,   // 採集點-藥草
  	EM_MapIconType_Inn							= 12,   // 旅店
	EM_MapIconType_GuildMan						= 13,   // 公會建立者
	EM_MapIconType_HouseMan						= 14,   // 房屋管理者
	EM_MapIconType_Weapon						= 15,   // 武器屋
	EM_MapIconType_Armor						= 16,   // 防具屋
	EM_MapIconType_Potion						= 17,   // 藥水店
	EM_MapIconType_Food							= 18,   // 飲食店
	EM_MapIconType_Magic						= 19,   // 魔法商店
	EM_MapIconType_Mat							= 20,   // 材料行
	EM_MapIconType_Shop							= 21,   // 雜貨店
	EM_MapIconType_Fish							= 22,   // 漁具店
	EM_MapIconType_Recipe						= 23,   // 配方
  	EM_MapIconType_Warrior						= 24,
	EM_MapIconType_Ranger						= 25,
	EM_MapIconType_Rogue						= 26,
	EM_MapIconType_Wizard						= 27,
	EM_MapIconType_Priest						= 28,
	EM_MapIconType_Knight						= 29,
	EM_MapIconType_RuneDancer					= 30,
	EM_MapIconType_Druid						= 31,
	EM_MapIconType_Harpsyn            			= 32,
	EM_MapIconType_Psyron           			= 33,
	EM_MapIconType_Duelist            			= 34,  // 職業保留
	EM_MapIconType_temp4            			= 35,  // 職業保留
	EM_MapIconType_temp5            			= 36,  // 職業保留
	EM_MapIconType_temp6            			= 37,  // 職業保留
	EM_MapIconType_temp7            			= 38,  // 職業保留
	EM_MapIconType_temp8            			= 39,  // 職業保留
	EM_MapIconType_temp9            			= 40,  // 職業保留
	EM_MapIconType_temp10           			= 41,  // 職業保留
  	EM_MapIconType_Flag							= 42,   
	EM_MapIconType_NPC							= 43,
	EM_MapIconType_Player						= 44, 
	EM_MapIconType_TrackArrow					= 45,
 	EM_MapIconType_LotteryMan					= 46,  // 彩券
	EM_MapIconType_VocationMan					= 47,  // 職業管理員
 	EM_MapIconType_36							= 48,	// //不使用 讓已下號碼對齊用
	EM_MapIconType_QuestNPC						= 49,	// 任務所需要擊殺的目標
	EM_MapIconType_QuestOBJ						= 50,	// 任務所需要點擊的目標
	EM_MapIconType_TrackNpcArrow				= 51,	// 追蹤NPC箭頭
 	EM_MapIconType_TrackNpc						= 52,	// 追蹤NPC
 	EM_MapIconType_RepeatQuest					= 53,	// 可重覆型任務可接
	EM_MapIconType_RepeatQuestDone				= 54,	// 可重覆型任務完成
	EM_MapIconType_GrayQuest					= 55,	// 可接但尚未到達等級條件的任務
  	EM_MapIconType_Speak						= 56,	// 說話
	EM_MapIconType_GrayTower					= 57,	// 塔 (灰色) 
	EM_MapIconType_RedTower						= 58,	// 塔 (紅色)
	EM_MapIconType_BlueTower					= 59,	// 塔 (藍色)
	EM_MapIconType_Tomb							= 60,	// 墓碑
	EM_MapIconType_BGTeam1						= 61,   // 戰場隊伍標記1
	EM_MapIconType_BGTeam2						= 62,   // 戰場隊伍標記2
	EM_MapIconType_BGTeam3						= 63,   // 戰場隊伍標記3
	EM_MapIconType_BGTeam4						= 64,   // 戰場隊伍標記4
	EM_MapIconType_BGTeam5						= 65,   // 戰場隊伍標記5
	EM_MapIconType_BGTeam6						= 66,   // 戰場隊伍標記6
	EM_MapIconType_BGTeam7						= 67,   // 戰場隊伍標記7
	EM_MapIconType_BGTeam8						= 68,   // 戰場隊伍標記8
	EM_MapIconType_Fighting						= 69,   // 戰鬥中
	EM_MapIconType_PublicQuest					= 70,   // PE QUEST
	EM_MapIconType_PublicQuestDone				= 71,   // PE QUEST
	EM_MapIconType_END							= 72
};

//
extern const char* g_szIconTextureName;

//座標
struct SWorldCoordinate
{
	string wdbFile;
	float PosX;
	float PosZ;
};

struct SMapCoordinate
{
	int MapID;
	float PosX; //百分比 0~1
	float PosZ; //百分比 0~1
};

struct SMapTrackInfo
{
	SMapTrackInfo()
	{
		show = false;
		size = 16;
		id = -1;
		trackArrow = false;
	}
	bool show;
	string strName;
	SWorldCoordinate coord;
	int icontype;
	float r;
	float g;
	float b;
	float size;
	int id;	//當id!=-1 會 gotoid
	bool trackArrow; //當超出小地圖範圍時，有沒有追蹤箭頭
};

//
bool Func_GetMapIconTextureInfo( EMapIconType eID  , CRuRectangle& rect );

//EX: Wailing Mountains -> 嚎叫山麓
const char* Func_GetLocalMapName( const char* pMapName );
const char* Func_GetLocalMapName( int MapID );

//失敗傳回false
bool Func_TransformCoordinate_MapToWorld( const SMapCoordinate& rMapCoord, SWorldCoordinate& rWorldCoord );

//注意 rMapCoord.MapID 必須要指定 要轉換的到的地圖id
//當wdbFile不一樣 或MapID失敗 
//及座標 不在 0.0 ~ 1.05 之間 傳回false
bool Func_TransformCoordinate_WorldToMap( const SWorldCoordinate& rWorldCoord, SMapCoordinate& rMapCoord );


//用座標取得zoneid
//回傳GUID 找不到傳回-1
//int Func_GetZoneIDByPosition( const char* pMapName , float fX , float fZ );
































