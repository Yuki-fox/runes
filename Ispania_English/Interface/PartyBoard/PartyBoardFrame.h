#pragma once

#include "../interface.h"

#include <set>
#include <map>
#include <vector>
#include <string>

enum EPartyBoardPostType
{
	ePartyBoard_NoAd,
	ePartyBoard_PartyAd,
	ePartyBoard_PeoplewareAd,
};

// 使用SChat_RegisterBillBoardInfo傳送，大小要小於1024
struct SPartyBoardPartyAd
{
	StaticString <_MAX_NAMESTRING_LEN_> PosterName;
	StaticString <_MAX_NAMESTRING_LEN_> AreaName;
	StaticString <512> Comment;

	int		iAreaID;

	BYTE	byTarget;
	BYTE	byArea;
	BYTE	PartyMode	:1;
	BYTE	AssignMode	:2;
	BYTE	LvLimitMode	:1;
	BYTE	_temp		:4;
	BYTE	byMemberNumNeed;
	BYTE	byMemberNumNow;
	BYTE	byTemp;

	short	sLowLvLimit;

	BYTE	byDutyAttack;
	BYTE	byDutyDefend;
	BYTE	byDutyCure;
	BYTE	byDutyNoLimit;
};

// 使用SChat_RegisterBillBoardInfo傳送，大小要小於1024
struct SPartyBoardPeoplewareAd
{
	StaticString <_MAX_NAMESTRING_LEN_> PosterName;
	StaticString <_MAX_NAMESTRING_LEN_> AreaName[3];
	StaticString <512> Comment;

	int		iAreaID[3];

	BYTE	byTarget[3];
	BYTE	byArea[3];
	BYTE	byActAs;
	BYTE	byTemp;

	Voc_ENUM	Voc;
	short		VocLV;
	Voc_ENUM	Voc_Sub;
	short		Voc_SubLV;
};

struct SPartyBoardRaidInfo
{
	StaticString <_MAX_NAMERECORD_SIZE_> Name;

	union
	{
		int			_State;
		struct  
		{
			bool	isOnline		: 1;
			bool	isLeader		: 1;
		};
	};

	int				Voc;								//職業
	int				Voc_Sub;							//副職業
	int				Level;
	int				Level_Sub;
	int				Sex;
};

class CPartyBoardFrame : public CInterfaceBase
{
public:
	CPartyBoardFrame	( CInterface* pObject );
	~CPartyBoardFrame	( void );

	virtual	void			RegisterFunc					 ();
	 void			ClearList					 ();
	virtual void			DestroyFrames					 ();

	void					AddPost( int iPlayerDBID , int iDataSize , char* szData );
	void					DeletePost		( int iPlayerDBID );

	int						GetPostNum			( int iTab	  );
	int						GetPoster			( int iTab, int iIndex );
	SPartyBoardPartyAd		GetPartyPost		( int iIndex  );
	SPartyBoardPeoplewareAd	GetPeoplewarePost	( int iIndex  );
	void					PartyJoinResult		( bool bYesNo );

	void					SetAskerName   ( string AskerName );
	string					GetAskerName					 ();

	void					AnalyzeRaidInfo( int iLeaderDBID, vector< PartyMemberInfoStruct > & List );
	bool					HasRaidMemberInfo ( int iMemberID );
	SPartyBoardRaidInfo		GetRaidMemberInfo ( int iMemberID );

	void					UpdateLeaderName( string oldLeaderName );

	void					SetMyPostType( EPartyBoardPostType type ) { m_myPostType = type; }
	EPartyBoardPostType		GetMyPostType() { return m_myPostType; }

	int						GetMyPostIndex();
	bool					IsPostForRaid();

private:
	vector<pair<int, SPartyBoardPartyAd>>		m_vecPartyPosts;
	vector<pair<int, SPartyBoardPeoplewareAd>>	m_vecPeoplewarePosts;
	string										m_AskerName;
	map<int, SPartyBoardRaidInfo>				m_mapRaidInfo;
	EPartyBoardPostType							m_myPostType;
};

extern CPartyBoardFrame* g_pPartyBoardFrame;