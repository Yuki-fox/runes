#pragma	   once

#include "../GameMain.h"

// **********************************************************************************************
// Globals
class	CWorldFrame;

extern	CWorldFrame					*g_pWorldFrame;
extern	CRuVector3					g_offsetPosstion;

// **********************************************************************************************

void RegisterWorldFrameFunc ();
int Control_OnUpdate (lua_State *L);
int Control_OnMouseDown (lua_State *L);
int Control_OnMouseUp (lua_State *L);
int Control_OnMouseWheel (lua_State *L);
int Control_OnMouseEnter (lua_State *L);
int Control_OnMouseLeave (lua_State *L);
int Control_OnReceiveDrag (lua_State *L);
int	GetFramerate (lua_State *L);
int	GetTime (lua_State *L);
int GetPlayerID (lua_State *L);
int	GetPlayerPos (lua_State *L);

class CWorldFrame
{
public:
	CWorldFrame();
	~CWorldFrame();

	static void				Initial();
	static void				Release();	
	static void				ReceiveRoleSpriteDelete( CRoleSprite* pPlayer );

	void					Clear();
	void					EnterWorld();
	void					SetPlayerID( int playerID )					{ m_playerID = playerID; }
	int						GetPlayerID()								{ return m_playerID; }
	void					SetZoneID( int zoneID );
	void					SetMapID( int mapID );	
	void					GameWorldUpdate( float elapsedTime );	
	void					MouseDown( int key );
	void					MouseUp( int key );
	void					MouseWheel( int delta );
	bool					IsMouseEnter()								{ return m_isMouseEnter; }
	void					MouseEnter()								{ m_isMouseEnter = true; }
	void					MouseLeave()								{ m_isMouseEnter = false; }
	void					ReceiveDrag();
	void					LockFight();
	void					UnlockFight();
	bool					IsLockFight()								{ return m_isLockFight; }
	void					SetTargetPlayer( CRoleSprite* pTarget );
	CRoleSprite*			GetTargetPlayer()							{ return m_pTargetPlayer; }
	CRoleSprite*			GetPlayer()									{ return m_pPlayer; }
	void					PathMove( CRuVector3& pos );
	void					MoveToScreenPoint( int xMouse, int yMouse );
	void					MoveToPlayer( CRoleSprite* pPlayer );

protected:
	void					SendPlayerPos();
	void					SendPlayerMove( CRuVector3& pos, CRuVector3& dir, bool force );
	void					SendPlayerAttack();
	void					CheckTargetStatus();
	void					HandleKeyboard();
	void					ControlCamera();
	void					CancelAttack();
	void					EnterFightMode();
	void					LeaveFightMode();
	void					SetAction( int action );
	void					SendDoAction( int action, int status )		{ Net_Move::S_DoAction( m_playerID, (action & 0xFFFF) | (status << 16), 0 ); }
	void					LockCursor();
	void					UnlockCursor();
	void					ChangeCursor();

protected:
	int						m_playerID;						// 玩家角色編號
	int						m_zoneID;						// 區域名稱
	int						m_mapID;						// 地圖名稱
	
	int						m_mouseState;					// 
	int						m_moveType;
	float					m_buttonPress[2];
	bool					m_isInitial;
	bool					m_isMouseEnter;
	bool					m_isFirstViewpoint;				// 第一人稱視角	
	bool					m_isLockFight;
	bool					m_isLockForward;
	float					m_elapsedTime;
	float					m_lastSendMoveTime;	

	bool					m_isAttack;
	bool					m_isTouchQuest;

	CRuVector3				m_lastSendMovePos;
	CRuVector3				m_lastSendDir;
	CRoleSprite*			m_pPlayer;
	CRoleSprite*			m_pMouseInPlayer;
	CRoleSprite*			m_pTargetPlayer;
	CRoleSprite*			m_pOldTargetPlayer;
	CEffectSprite*			m_pMoveTarget;
};
