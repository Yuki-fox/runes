#pragma once

#include "../interface.h"



class CNpcTrackFrame;
extern CNpcTrackFrame* g_pNpcTrackFrame;

struct STrackNpcInfo
{
	GameObjDbStructEx* pObjDB;
	int MapID ;
	float PosX ;
	float PosZ ;
};

#define DF_NpcTrackTargetInfoMax 10
struct SNpcTrackTargetInfo
{
	int DBID ; 
	float r  ; 
	float g  ;
	float b  ;
};

class CNpcTrackFrame : public CInterfaceBase
{
public:
	CNpcTrackFrame( CInterface* pObject );
	virtual~CNpcTrackFrame( void );

	virtual	void RegisterFunc();	
	virtual	void Update( float elapsedTime );

	virtual void ChangeZone();
public:

	void Initialize( const char* pViewName, const char* pPlayerTexture );
	void RenderWorldMap( CUiFrame* frame, int lParam, int rParam );
	int SearchNpc( const char* pName );
 	int SearchNpcByDBID( int DBID );
	int SearchQuestNpc( bool bIgnoreLV, bool bSameZone, bool sendWorldMap = false );
	STrackNpcInfo* GetNpc( int index );

 	void SetTarget( int index, int DBID, float r = 1.0f, float g = 1.0f, float b = 1.0f );
	void R_CtoC_TrackPlayerResult( const char* Name, int WorldMapID, float fX, float fY, float fZ ); 

public:
	static bool IsNpcVisible( GameObjDbStructEx* pObjDB, bool bCheckQuest );

protected:
 	void _SetTarget( int index, const char* Name, int WorldMapID, float fX, float fY, float fZ, float r , float g , float b );
	void _ClearTarget( int index );
public:
	CUiFrame*    m_pUiFrameWorldMap;
	CUiTexture*  m_pUiTextureSelf;

	int m_MapID;
 
	//string     m_strTrackName;
	//CRuVector3 m_vecTrackPositioln;
	//vector<GameObjDbStructEx*> m_SearchResult;

	void PushTrackNpcInfo( GameObjDbStructEx* pObjDB, bool bCheckQuest );
	vector<STrackNpcInfo> m_TrackNpcInfoTable;

	SNpcTrackTargetInfo m_TargetInfos[DF_NpcTrackTargetInfoMax];
	bool m_bTargetInfodirty;

	void UpdateTargetInfos();

	std::string m_TrackPlayerName; //°lÂÜª±®a¦WºÙ
	
};
