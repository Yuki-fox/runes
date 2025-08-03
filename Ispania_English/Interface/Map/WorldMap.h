#pragma once

#include "../interface.h"

#include "MapCommon.h"

#define DF_TRACKINFO_MAX 11 
//0       追蹤NPC UI用
//1 ~ 10  追蹤NPC


class CWorldMapFrame;
extern CWorldMapFrame* g_pWorldMapFrame;

#include <string>

struct SWorldMapIconFrame
{
	SWorldMapIconFrame( CUiFrame* _pFrme , CUiTexture* _pTexture )
	{
		pFrme = _pFrme;
		pTexture = _pTexture;
	}
	CUiFrame* pFrme;
	CUiTexture* pTexture;
};

struct structQuestTrackIcon
{
	int iNPCID;
	int	iIcon;
	int iProirity;
	int	iType;
};

class CWorldMapView
{
public:
	CWorldMapView();

	CUiFrame*   m_pUiFrameWorldMap;
	CUiTexture* m_pUiTextureSelf;

	CUiSize m_oldSize;

	int m_WorldMapID;		//目前顯示哪一個zone

	std::vector< SWorldMapIconFrame > m_IconFrameList;
	std::map< CUiFrame* , SMapTrackInfo* > m_FrameInfoMap;

	//不會動的ICON 使用數量
	int m_StaticIconFrameCount;

	bool m_bStaticIconFrameListDirty;

	bool m_bShowQuestNpc;
	bool m_bShowParty;
	bool m_bShowTrackNpc;


	enum EWorldMapViewType
	{
		WMV_WorldMap,
		WMV_AreaMap,
	};

	EWorldMapViewType eType;
 
	void Init( const char* pViewName , const char* pPlayerCursor );
	void Fini();

	void SetWorldMapID( int WorldMapID );

	void Dirty();

	void SetShowQuestNpc( bool bShow );
	void SetShowParty( bool bShow );
	void SetShowTrackNpc( bool bShow );

	void Update();
     
	void UpdateStaticIconFrameList( std::vector< SMapTrackInfo >& rTable );

	bool IconFrame_OnMouseEnter( CUiFrame* frame );
	bool IconFrame_OnMouseLeave( CUiFrame* frame );
	bool IconFrame_OnDoubleClick( CUiFrame* frame );
	bool IconFrame_OnClick( CUiFrame* frame );

};

class CWorldMapFrame : public CInterfaceBase
{
public:
	CWorldMapFrame(CInterface* object);
	virtual ~CWorldMapFrame();

 	virtual	void RegisterFunc();	
	virtual void DestroyFrames();
	virtual void Update( float elapsedTime );	

	virtual void ChangeZone();

	void InitializeWorldMap( const char* pViewName , const char* pPlayerCursor );
	void InitializeAreaMap( const char* pViewName , const char* pPlayerCursor );
	
	void RenderWorldMap( CUiFrame* frame, int lParam, int rParam );

   	void SetPingPosition( const SMapCoordinate& rMapCoord );
	const SWorldCoordinate&	GetPingPosition();
	int	GetPingMapID(){ return m_PingMapID; }

	void R_CtoC_MapPing( const char* pPingerName, int ZoneID, float fX, float fZ );

	//CUiPoint			GetWorldTexturePos	( float fZoneX, float fZoneY, float fZoneW, float fZoneH );
	//CUiPoint			GetWorldTexturePos	( float right, float top, float left, float bottom );

public:	
	//追蹤npc
	void ShowTrackTarget( int Index, const char* wdbFile, float PosX, float PosZ, const char* szName, int IconType, float r = 1.0f, float g = 1.0f, float b = 1.0f );
	void HideTrackTarget( int Index );

public:	
	//追蹤任務npc
	void AddTrackQuestNPC( const char* wdbFile, float PosX, float PosZ, const char* szName, int DBID, int IconType, float r = 1.0f, float g = 1.0f, float b = 1.0f );
	void ClearTrackQuestNPC();

public:
	//AddMapIcon
	//加入一個Icon 到世界地圖跟小地圖

	void AddMapIcon( int groupId, const char* wdbFile, float PosX, float PosZ, const char* szName, int IconType, float r = 1.0f, float g = 1.0f, float b = 1.0f , bool trackArrow = false, int id = -1 );
	void AddMapIcon( int groupId, int zoneID, float PosX, float PosZ, const char* szName, int IconType, float r = 1.0f, float g = 1.0f, float b = 1.0f , bool trackArrow = false, int id = -1 );
	
	//groupId 群組ID 同一個群組的會一起被清除	
	void ClearMapIcon( int groupId, int id = -1 );

	//當換ZOON時 清除group
	void ClearMapIconWhenChangeZone( int groupId );

protected:
	std::set< int > m_ChangeZoneClearGroupIDs; //當換ZOON時 清除group
public:

	bool m_SearchQuestNpc_bIgnoreLV;
	int m_SearchQuestNpc_DirtyTime;

	bool m_SearchQuestNpc_ShowFlag[4];

public:	
	//提供企劃在世界地圖顯示icon
	void AddIcon( 
		const char* wdbFile, 
		float PosX, 
		float PosZ, 
		float r = 1.0f, 
		float g = 1.0f, 
		float b = 1.0f, 
		int IconType = EM_MapIconType_None,
		float size = 16,
		char* szName = NULL,
		int id = -1 );
 	void ClearIcon();

	//當有用過AddIcon 才能瞬移
	bool m_bGmMode;

public:	
	//設定Map 的 Icon & Tips
	//加到目前zoon 換zoon清除
	virtual void RL_SetMapMark	( int MarkID, float PosX, float PosZ, const char* szName, int IconType );
	virtual void RL_ClsMapMark	( int MarkID );

	virtual	void SetMapIncon	( map< int, structQuestTrackIcon >& mapNPC, int iNPCID, int iProirity, int iIcon, int iType );

public:	


	//sMapTrackInfo m_TrackInfo;

	//靜態
 	std::vector< SMapTrackInfo > m_InsideTrackInfo; //給企劃使用
 	std::vector< SMapTrackInfo > m_QuestNpcInfo;    //任務NPC
	std::vector< SMapTrackInfo > m_TrackNPC;        //追蹤NPC

	std::map< int , std::vector< SMapTrackInfo > >	m_MapIcon;         //追蹤NPC


	//SMapTrackInfo m_TrackTeammate[_MAX_PARTY_COUNT_];
	//SMapTrackInfo m_TrackInfo[DF_TRACKINFO_MAX];

	//動態 每個FRAME更新
	std::vector< SMapTrackInfo > m_TrackTeammate; //追蹤隊友

	SWorldCoordinate m_WorldCoordPing;

	int m_PingMapID; //點哪一個地圖發出ping的


	CWorldMapView m_WorldMapView;
	CWorldMapView m_AreaMapView;



};
