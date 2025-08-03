#include "WorldFrame.h"
#include "ChatFrame.h"
#include "MainMenu.h"
#include "UIOptionsFrame.h"

#include "../../netwaker_member/Net_Script/NetCli_Script.h"

#define SEND_MOVE_DELAY_TIME		0.5f
#define MAX_CAMERA_THETA			0.15f
#define MIN_CAMERA_THETA			-0.93f
#define MAX_CAMERA_DISTANCE			500.0f
#define MIN_CAMERA_DISTANCE			10.0f

// --------------------------------------------------------------------------------------
// Globals
CWorldFrame*						g_pWorldFrame = NULL;
CRuVector3							g_offsetPosstion( 0.0f, 0.0f, 0.0f );
//CRuVector3							g_offsetPosstion( -960.0f, 0.0f, -10560.0f );
//CRuVector3							g_offsetPosstion( -2000.0f, 500.0f, -8500.0f );

// --------------------------------------------------------------------------------------
void CWorldFrame::Initial()
{
	if ( g_pWorldFrame == NULL )
	{
		g_pWorldFrame = new CWorldFrame;
	}
}

// --------------------------------------------------------------------------------------
void CWorldFrame::Release()
{
	SAFE_DELETE( g_pWorldFrame );
}

// --------------------------------------------------------------------------------------
void CWorldFrame::ReceiveRoleSpriteDelete( CRoleSprite* pPlayer )
{
	if ( g_pWorldFrame )
	{
		if ( g_pWorldFrame->m_pPlayer == pPlayer )
			g_pWorldFrame->m_pPlayer = NULL;
		if ( g_pWorldFrame->m_pMouseInPlayer == pPlayer )
			g_pWorldFrame->m_pMouseInPlayer = NULL;
		if ( g_pWorldFrame->m_pTargetPlayer == pPlayer )
			g_pWorldFrame->m_pTargetPlayer = NULL;
	}
}

// --------------------------------------------------------------------------------------
CWorldFrame::CWorldFrame()
{
	Clear();	
}

// --------------------------------------------------------------------------------------
CWorldFrame::~CWorldFrame()
{
	Clear();
}

// --------------------------------------------------------------------------------------
void CWorldFrame::Clear()
{
	m_playerID				= -1;
	m_zoneID				= -1;
	m_mapID					= -1;

	m_mouseState			= 0;
	m_buttonPress[0]		= 0.0f;
	m_buttonPress[1]		= 0.0f;
	m_isInitial				= false;
	m_isMouseEnter			= false;	
	m_isLockFight			= false;
	m_isLockForward			= false;	
	m_pPlayer				= NULL;
	m_pMouseInPlayer		= NULL;
	m_pTargetPlayer			= NULL;
	m_lastSendMoveTime		= 0.0f;	
	m_pMoveTarget			= NULL;	

	m_lastSendMovePos.Set( 0.0f, 0.0f, 0.0f );
	m_lastSendDir.Set( 0.0f, 0.0f, -1.0f );
}

// --------------------------------------------------------------------------------------
void CWorldFrame::EnterWorld()
{
	CFrame* pGlueParent = (CFrame*)CLayout::GetLayout("GlueParent");
	if ( pGlueParent )
		pGlueParent->Hide();

	CFrame* pWorldFrame = (CFrame*)CLayout::GetLayout("WorldFrame");
	if ( pWorldFrame )
		pWorldFrame->Show();

	g_pGameMain->SetEnvironmentShow();

	CFrame* pRootFrame = (CFrame*)CLayout::GetLayout(DEF_UI_ROOT_NAME);
	if ( pRootFrame )
	{		
		pRootFrame->SetEvent( eEventType_Update			, "Control_OnUpdate(arg1, arg2)" );
		pRootFrame->SetEvent( eEventType_MouseDown		, "Control_OnMouseDown(arg1)" );
		pRootFrame->SetEvent( eEventType_MouseUp		, "Control_OnMouseUp(arg1)" );
		pRootFrame->SetEvent( eEventType_MouseWheel		, "Control_OnMouseWheel(arg1)" );
		pRootFrame->SetEvent( eEventType_Enter			, "Control_OnMouseEnter()" );
		pRootFrame->SetEvent( eEventType_Leave			, "Control_OnMouseLeave()" );
		pRootFrame->SetEvent( eEventType_ReceiveDrag	, "Control_OnReceiveDrag()" );
	}

	// Clear();
}

// --------------------------------------------------------------------------------------
void CWorldFrame::SetZoneID( int zoneID )
{
	if ( m_zoneID != zoneID )
	{
        m_zoneID = zoneID;
	}
}

// --------------------------------------------------------------------------------------
void CWorldFrame::SetMapID( int mapID )
{
	if ( m_mapID != mapID )
	{
		m_mapID = mapID;		

		// 載入地表
		g_pGameMain->LoadTerrain( "world_export.ros" );
	}
}

// --------------------------------------------------------------------------------------
void CWorldFrame::GameWorldUpdate( float elapsedTime )
{
	m_elapsedTime = elapsedTime;
	if ( m_playerID == -1 )
		return;

	if ( m_pPlayer == NULL )
		m_pPlayer = g_pGameMain->FindRoleSprite( m_playerID );
	if ( m_pMoveTarget == NULL )
	{
		m_pMoveTarget = new CEffectSprite;
		if ( m_pMoveTarget->LoadRuEntity("\\model\\widget\\target_marker\\act_target_marker.ros") )
			m_pMoveTarget->SetEffectMotionStep(eEffectMotion_Step_Idle);
	}

	IBaseSprite::SetMainPlayerId( m_playerID );
	if ( m_pPlayer )
		g_pGameMain->SetPlayerPos( m_pPlayer->GetPos() );
	if ( !m_isInitial )								// 初始化鏡頭所在位置
	{
		m_isInitial = true;
		CRuCamera* pCamera = g_pGameMain->GetCamera();
		if ( pCamera )
		{
			if ( m_pPlayer )
			{
				pCamera->SetCameraPosition( CRuVector3(0.0f, 0.0f, 0.0f) );		// 鏡頭所在位置
				pCamera->SetTargetPosition( m_pPlayer->GetDirection() * 50 );	// 鏡頭目的位置
				pCamera->SetCameraUpVector( CRuVector3(0.0f, 1.0f, 0.0f) );		// 鏡頭上方所朝方向
			}
			else
			{
				pCamera->SetCameraPosition( CRuVector3(0.0f, 0.0f, 50.0f) );	// 鏡頭所在位置
				pCamera->SetTargetPosition( CRuVector3(0.0f, 0.0f, 0.0f) );		// 鏡頭目的位置
				pCamera->SetCameraUpVector( CRuVector3(0.0f, 1.0f, 0.0f) );		// 鏡頭上方所朝方向
			}
		}
	}

	// 記算按鍵按下時間
	if ( m_mouseState & eMouse_Flag_LButton )
		m_buttonPress[0] += elapsedTime;
	else
		m_buttonPress[0] = 0.0f;

	if ( m_mouseState & eMouse_Flag_RButton )
		m_buttonPress[1] += elapsedTime;
	else
		m_buttonPress[1] = 0.0f;

	if ( IsMouseEnter() )
	{
		CInput* pInput = CFrame::GetInput();
		m_pMouseInPlayer = g_pGameMain->GetTopRoleSprite( pInput->GetXPos(), pInput->GetYPos(), m_playerID );
	}
	else
		m_pMouseInPlayer = NULL;	

	// 
	if ( m_pPlayer && !m_pPlayer->IsMove() )
	{
		m_pMoveTarget->SetPos( CRuVector3(0.0f, 0.0f, 0.0f) );
		m_pMoveTarget->Hide();		
	}

	// 事件檢測
	CheckTargetStatus();

	// 改變游標狀態
	ChangeCursor();

	// 是否擁有鍵盤焦點
	HandleKeyboard();

	// 檢測是否需要送出移動封包
	SendPlayerPos();

	// 檢測是否送出攻擊封包
	SendPlayerAttack();	

	// 鏡頭控制
	ControlCamera();
}

// --------------------------------------------------------------------------------------
void CWorldFrame::MouseDown( int key )
{
	ItemClipboard* pItem = g_pMainMenu->PasteItemClipboard();
	RoleDataEx* Role = g_pGameMain->RoleData();
	switch ( key )
	{
	case eMouse_Flag_LButton:
		m_mouseState |= eMouse_Flag_LButton;
		LockCursor();

		// 若有拖曳中物件
		if ( pItem )
		{
			ItemFieldStruct* pItemField = NULL;			
			switch ( pItem->type )
			{
			case EM_DragDropType_Body:
				if ( pItemField = Role->GetBodyItem( pItem->pos ) )
					NetCli_Item::_DeleteItem_Body( *pItemField, pItem->pos );
				break;

			case EM_DragDropType_Bank:
				if ( pItemField = Role->GetBankItem( pItem->pos ) )
					NetCli_Item::_DeleteItem_Bank( *pItemField, pItem->pos );
				break;

			case EM_DragDropType_EQ:
				if ( pItemField = Role->GetEqItem( pItem->pos ) )
					NetCli_Item::_DeleteItem_EQ( *pItemField, pItem->pos );
				break;
			}
			g_pMainMenu->ClearItemClipboard();
		}

		if( m_pMouseInPlayer != NULL )
		{
			// 點一次就執行的動作
			switch( m_pMouseInPlayer->GetCheckedStatus() )
			{
			case eChecked_QuestObj:
				CNetCli_Script::OnEvent_TouchQuestObj( m_pMouseInPlayer );
				break;
			}
		}	
		break;

	case eMouse_Flag_RButton:
		m_mouseState |= eMouse_Flag_RButton;
		LockCursor();
		break;

	case eMouse_Flag_MButton:
		m_mouseState |= eMouse_Flag_MButton;
		break;

	case eMouse_Flag_XButton:
		m_mouseState |= eMouse_Flag_XButton;
		break;
	}
}

// --------------------------------------------------------------------------------------
void CWorldFrame::MouseUp( int key )
{
	switch ( key )
	{
	case eMouse_Flag_LButton:
		m_mouseState &= ~eMouse_Flag_LButton;
		UnlockCursor();

		if ( m_buttonPress[0] < 0.5f )
			SetTargetPlayer( m_pMouseInPlayer );
		break;

	case eMouse_Flag_RButton:
		m_mouseState &= ~eMouse_Flag_RButton;
		UnlockCursor();

		if ( m_buttonPress[1] < 0.5f )
		{
			CancelAttack();

			if ( m_pPlayer )
				m_pPlayer->ClearPathMove();
		}			
		break;

	case eMouse_Flag_MButton:
		m_mouseState &= ~eMouse_Flag_MButton;
		break;

	case eMouse_Flag_XButton:
		m_mouseState &= ~eMouse_Flag_XButton;
		break;
	}
}

// --------------------------------------------------------------------------------------
void CWorldFrame::MouseWheel( int delta )
{
	CRuCamera* pCamera = g_pGameMain->GetCamera();
	if ( pCamera )
	{
		float zDelta = delta * 0.02f;
		float dis = (pCamera->GetTargetPosition() - pCamera->GetCameraPosition()).Magnitude();

		if ( dis - zDelta > MAX_CAMERA_DISTANCE )
			zDelta -= MAX_CAMERA_DISTANCE - (dis - zDelta);
		if ( dis - zDelta < MIN_CAMERA_DISTANCE )
			zDelta -= MIN_CAMERA_DISTANCE - (dis - zDelta);
		
		pCamera->MoveCamera( CRuVector3(0.0f, 0.0f, zDelta), true );
	}
}

// --------------------------------------------------------------------------------------
void CWorldFrame::ReceiveDrag()
{
}

// --------------------------------------------------------------------------------------
void CWorldFrame::LockFight()
{
	m_isLockFight = true;
	EnterFightMode();
}

// --------------------------------------------------------------------------------------
void CWorldFrame::UnlockFight()
{
	m_isLockFight = false;
	LeaveFightMode();
}

// --------------------------------------------------------------------------------------
void CWorldFrame::SendPlayerPos()
{
	if ( m_pPlayer == NULL )
		return;

	CRuVector3 nowPos = m_pPlayer->GetPos();
	CRuVector3 direction = m_pPlayer->GetDirection();
	int moveType = m_pPlayer->GetMoveType();

	if ( moveType != m_moveType )
	{
		switch ( moveType )
		{
		//
		case eMoveType_Falling:
		case eMoveType_Jump:
			switch ( m_moveType )
			{
			case eMoveType_Idle:
			case eMoveType_Arrival:
			case eMoveType_Path:
			case eMoveType_Vector:
				SendPlayerMove( nowPos, direction, true );
				break;
			}
			break;

		default:
			// 下降到地面補送封包
			if ( m_moveType == eMoveType_Falling )
				SendPlayerMove( nowPos, direction, true );			
			break;
		}

		m_moveType = moveType;
	}

	switch ( m_moveType )
	{
	case eMoveType_Idle:
	case eMoveType_Arrival:
	case eMoveType_Path:
	case eMoveType_Vector:
		SendPlayerMove( nowPos, direction, false );		
		break;
	}
}

// --------------------------------------------------------------------------------------
void CWorldFrame::SendPlayerMove( CRuVector3& pos, CRuVector3& dir, bool force )
{
	bool isSend = false;

	if ( force )
	{
		isSend = true;
		m_lastSendMoveTime = 0.0f;
	}
	else
	{
		if ( m_lastSendMovePos != pos || m_lastSendDir != dir )
		{
			m_lastSendMoveTime += m_elapsedTime;
			if ( m_lastSendMoveTime >= SEND_MOVE_DELAY_TIME )
			{
				isSend = true;
				m_lastSendMoveTime -= SEND_MOVE_DELAY_TIME;
			}
		}
		else
			m_lastSendMoveTime = 0.0f;
	}

	if ( isSend )
	{
		float theta = 0.0f;

		m_lastSendMovePos = pos;
		m_lastSendDir = dir;
		pos -= g_offsetPosstion;

		// 取得角色所朝方向
		theta = acos( dir.Dot( CRuVector3(1.0f, 0.0f, 0.0f) ) );
		if ( dir.m_z > 0.0f )
			theta = PI + PI - theta;

		// 轉換為角度
		theta = theta * 180.0f / PI;

		Net_Move::PlayerMoveObject( m_playerID, pos.m_x, pos.m_y, pos.m_z, theta );
		// g_pError->AddMessage( 0, 0, "(%.2f %.2f %.2f)", pos.m_x, pos.m_y, pos.m_z );
	}
}

// --------------------------------------------------------------------------------------
void CWorldFrame::SendPlayerAttack()
{	
}

// --------------------------------------------------------------------------------------
void CWorldFrame::CheckTargetStatus()
{
    float Range = 10.0f;
	// 檢查事件觸發
	if ( m_isTouchQuest )
	{
		m_isTouchQuest = false;

		if ( m_pTargetPlayer && m_pPlayer )
		{
			CRuVector3 dir = m_pTargetPlayer->GetPos() - m_pPlayer->GetPos();

			// 判別可對目標物執行動作
			switch ( m_pTargetPlayer->GetCheckedStatus() )
			{
			case eChecked_TouchPlot:
                Range = m_pTargetPlayer->GetTouchRange();
			case eChecked_QuestNPC:
			case eChecked_QuestObj:
                {
                    // 判斷距離
				    if ( dir.Magnitude() > Range )
                    {
					    if ( g_pUIOptionsFrame->m_isHitMove )
					    {
						    MoveToPlayer( m_pTargetPlayer );
						    m_isTouchQuest = true;
					    }
					    else
						    g_pError->AddMessage( 2, 0, "距離過遠,無法觸發事件" );
				    }
				    else
				    {
					    m_pPlayer->ClearPathMove();
					    if ( g_pUIOptionsFrame->m_isAutoFaceToFace )
						    m_pPlayer->SetDirection( m_pTargetPlayer->GetPos() - m_pPlayer->GetPos() );

					    switch ( m_pTargetPlayer->GetCheckedStatus() )
					    {
					    case eChecked_TouchPlot:
						    CNetCli_Script::OnEvent_TouchPlot( m_pTargetPlayer );
						    break;

					    case eChecked_QuestNPC:
						    CNetCli_Script::OnEvent_TouchQuestNPC( m_pTargetPlayer );
						    break;

					    case eChecked_QuestObj:
						    CNetCli_Script::OnEvent_TouchQuestObj( m_pTargetPlayer );
						    break;
					    }
				    }
                }
				break;
			}
		}
	}

	// 檢查攻擊觸發
	if ( m_isAttack )
	{
		m_isAttack = false;

		if ( m_pTargetPlayer && m_pPlayer )
		{
			CRuVector3 dir = m_pTargetPlayer->GetPos() - m_pPlayer->GetPos();

			switch (m_pTargetPlayer->GetCheckedStatus())
			{
			// 判斷距離
			case eChecked_Fight:
				// 判斷距離
				if ( dir.Magnitude() > 20.0f )
				{
					if ( g_pUIOptionsFrame->m_isHitMove )
					{
						m_isAttack = true;
						MoveToPlayer( m_pTargetPlayer );
					}
				}
				else
					m_pPlayer->ClearPathMove();

				break;
			}
		}
	}

	if ( m_pPlayer )
	{
		if ( m_pPlayer->IsFight() && m_pTargetPlayer == NULL )
			CancelAttack();
	}
}

// --------------------------------------------------------------------------------------
void CWorldFrame::HandleKeyboard()
{
	CInput* pInput = CFrame::GetInput();

	if ( m_pPlayer == NULL || !CFrame::IsRootHaveKeyboard() )
		return;

	// 角色移動
	float forwardDelta = 0.0f, rightDelta = 0.0f;
	CRuVector3 vec;	

	if ( pInput->GetKeyState(KEY_NUMLOCK) & eKS_Down )
		m_isLockForward = !m_isLockForward;
	if ( pInput->GetKeyState(KEY_W) & eKS_Press )
	{
		m_pPlayer->ClearPathMove();
		m_isLockForward = false;
		forwardDelta += m_elapsedTime;
	}
	if ( pInput->GetKeyState(KEY_S) & eKS_Press )
	{
		m_pPlayer->ClearPathMove();
		m_isLockForward = false;
		forwardDelta -= m_elapsedTime;
	}
	if ( pInput->GetKeyState(KEY_D) & eKS_Press )
	{
		m_pPlayer->ClearPathMove();
		m_isLockForward = false;
		rightDelta += m_elapsedTime;
	}
	if ( pInput->GetKeyState(KEY_A) & eKS_Press )
	{
		m_pPlayer->ClearPathMove();
		m_isLockForward = false;
		rightDelta -= m_elapsedTime;
	}
	if ( m_isLockForward )
		forwardDelta = m_elapsedTime;

	vec = m_pPlayer->GetMoveVector( forwardDelta, rightDelta );

	// walk <-> run
	if ( pInput->GetKeyState(KEY_DIVIDE) & eKS_Down )
	{
		m_pPlayer->SetWalk( !m_pPlayer->IsWalk() );
		SendDoAction( eAction_Null, m_pPlayer->GetStateFlag() );
	}
	// Holster
	if ( pInput->GetKeyState(KEY_Z) & eKS_Down )
	{
		if ( !m_pPlayer->IsFight() )
		{
			if ( m_pPlayer->IsUnholster() )
				SetAction( eAction_Holster );
			else
				SetAction( eAction_Unholster );
		}
	}
	// sit
	if ( pInput->GetKeyState(KEY_X) & eKS_Down )
	{
		if ( !m_pPlayer->IsDead() && !m_pPlayer->IsSleep() )
		{
			if ( m_pPlayer->IsSit() )
				SetAction( eAction_StandUp );
			else
				SetAction( eAction_Sit );
		}
	}	
	if ( pInput->GetKeyState(KEY_ENTER) & eKS_Down )
	{
		CEditBox* pEdit = (CEditBox*)CLayout::GetLayout( "MainInputEditBox" );
		if ( pEdit )
			pEdit->SetFocus();
	}
	// jump
	if ( pInput->GetKeyState(KEY_SPACE) & eKS_Down )		
		m_pPlayer->Jump( vec );

	if ( vec != CRuVector3(0.0f, 0.0f, 0.0f) )
		m_pPlayer->VectorMove( vec );
}

// --------------------------------------------------------------------------------------
void CWorldFrame::ControlCamera()
{
	CRuCamera* pCamera = g_pGameMain->GetCamera();
	CInput* pInput = CFrame::GetInput();

	if ( pCamera == NULL )
		return;	

	// 鏡頭旋轉
	if ( pInput->GetXDelta() != 0 || pInput->GetYDelta() != 0 )
	{
		// 若左鍵及右鍵被按下
		if ( (m_mouseState & eMouse_Flag_LButton) || (m_mouseState & eMouse_Flag_RButton) )
		{
			CRuVector3 dir = pCamera->GetTargetPosition() - pCamera->GetCameraPosition();
			dir.m_x = abs( dir.m_x );
			dir.m_z = abs( dir.m_z );
			dir.Normalize();
			float theta = dir.Dot( CRuVector3(0.0f, 1.0f, 0.0f) );

			float xDelta = pInput->GetXDelta() * -0.005f;
			float yDelta = pInput->GetYDelta() * -0.005f;
			if ( theta + yDelta > MAX_CAMERA_THETA )
				yDelta = MAX_CAMERA_THETA - theta;
			if ( theta + yDelta < MIN_CAMERA_THETA )
				yDelta = MIN_CAMERA_THETA - theta;

			pCamera->Cycle( xDelta, CRuVector3(0.0f, 1.0f, 0.0f) );
			pCamera->Cycle( yDelta, pCamera->GetCameraRightVector() );

			// 讓角色正向鏡頭方向
			if ( (m_mouseState & eMouse_Flag_RButton) && m_pPlayer && !m_pPlayer->IsDead() )
			{
				if ( m_pPlayer->IsSit() )
					m_pPlayer->SetAction(eAction_Idle);
				m_pPlayer->SetDirection( pCamera->GetTargetPosition() - pCamera->GetCameraPosition() );

				/*
				if ( g_pUIOptionsFrame->m_isHitMove )
					m_pPlayer->ClearPathMove();
				*/
			}
		}
	}

	// 鏡頭跟隨角色移動
	if ( m_pPlayer )
	{
		if ( g_pUIOptionsFrame->m_isCameraFollow )
		{
			CRuVector3 relativeCamPos = m_pPlayer->GetDirection() * (pCamera->GetCameraPosition() - pCamera->GetTargetPosition()).Magnitude();
			pCamera->SetTargetPosition( m_pPlayer->GetPos() + CRuVector3(0.0f, 16.0f, 0.0f) );
			pCamera->SetCameraPosition( m_pPlayer->GetPos() - relativeCamPos + CRuVector3(0.0f, 16.0f, 0.0f) );
			pCamera->SetCameraUpVector( CRuVector3(0.0f, 1.0f, 0.0f) );
		}
		else
		{
			CRuVector3 relativeCamPos = pCamera->GetCameraPosition() - pCamera->GetTargetPosition();
			CRuVector3 absoluteCamPos = relativeCamPos + m_pPlayer->GetPos();
			pCamera->SetTargetPosition( m_pPlayer->GetPos() + CRuVector3(0.0f, 16.0f, 0.0f) );
			pCamera->SetCameraPosition( absoluteCamPos + CRuVector3(0.0f, 16.0f, 0.0f) );
			pCamera->SetCameraUpVector( CRuVector3(0.0f, 1.0f, 0.0f) );
		}
	}
}

// --------------------------------------------------------------------------------------
void CWorldFrame::SetTargetPlayer( CRoleSprite* pTarget )
{	
	m_isAttack = false;
	m_isTouchQuest = false;

	if ( pTarget )
	{
		if ( m_pTargetPlayer == pTarget )
		{
			switch ( pTarget->GetCheckedStatus() )
			{
			case eChecked_Fight:
				if ( !pTarget->IsDead() )
				{
					m_isAttack = true;
					EnterFightMode();
					NetCli_Attack::S_AttackSignal( pTarget->GetIdentify() );
				}				
				break;
			}
		}

		switch ( pTarget->GetCheckedStatus() )
		{
		case eChecked_TouchPlot:
		case eChecked_QuestNPC:
		case eChecked_QuestObj:
			m_isTouchQuest = true;
			break;
		}
	}
	else if ( g_pUIOptionsFrame->m_isHitMove )
		MoveToScreenPoint( CFrame::GetInput()->GetXPos(), CFrame::GetInput()->GetYPos() );

	m_pTargetPlayer = pTarget;
}

// --------------------------------------------------------------------------------------
void CWorldFrame::MoveToScreenPoint( int xMouse, int yMouse )
{
	CRuCamera* pCamera = g_pGameMain->GetCamera();
	if ( pCamera && m_pPlayer && !m_pPlayer->IsDead() )
	{
		// Calculate view origin in world space
		CRuVector3 homPt, viewPt, worldPt, pt1, pt2, tarPt;

		pCamera->Coord_ScreenToHomogeneous( xMouse, yMouse, g_pGameMain->GetScreenWidth(), g_pGameMain->GetScreenHeight(), homPt);
		pCamera->Coord_HomogeneousToView(homPt, viewPt);
		pCamera->Coord_ViewToWorld(viewPt, worldPt);

		pt1 = pCamera->GetCameraPosition();
		pt2 = pt1 + (worldPt - pt1) * 1000.0f;

		if ( IBaseSprite::TerrainTest( pt1, pt2, &tarPt ) )
		{			
			// 設定目標點顯示標地物
			m_pMoveTarget->SetPos(tarPt);
			m_pMoveTarget->Show();

			m_pPlayer->SetDirection( tarPt - m_pPlayer->GetPos() );
			m_pPlayer->PathMove( tarPt );
		}
	}
}

// --------------------------------------------------------------------------------------
void CWorldFrame::MoveToPlayer( CRoleSprite* pPlayer )
{
	static CRoleSprite* pLastPlayer = NULL;
	static float lastMoveTime = 0.0f;
	
	if ( pPlayer && m_pPlayer && !m_pPlayer->IsDead() )
	{
		float time = g_pGameMain->GetFrameTime();

		if ( pLastPlayer != pPlayer || time - lastMoveTime > 2.0f )
		{
			CRuVector3 pos;

			// 設定目標點顯示標地物		
			if ( !pPlayer->GetAttachPos(eAttach_Name_Title, pos) )
				pos = pPlayer->GetPos();

			m_pMoveTarget->SetPos(pos);
			m_pMoveTarget->Show();		

			m_pPlayer->SetDirection( pPlayer->GetPos() - m_pPlayer->GetPos() );
			m_pPlayer->PathMove( pPlayer->GetPos() );

			lastMoveTime = time;
		}
	}
	pLastPlayer = pPlayer;
}

// --------------------------------------------------------------------------------------
void CWorldFrame::CancelAttack()
{
	m_isAttack = false;
	NetCli_Attack::S_EndAttackSignal();

	if ( !IsLockFight() )
		LeaveFightMode();
}	

// --------------------------------------------------------------------------------------
void CWorldFrame::EnterFightMode()
{
	if ( m_pPlayer && !m_pPlayer->IsFight() )
	{
		m_pPlayer->SetFight( true );
		SetAction( eAction_Fight_Idle );
	}
}

// --------------------------------------------------------------------------------------
void CWorldFrame::LeaveFightMode()
{
	if ( m_pPlayer == NULL )
		return;

	if ( m_pPlayer->IsFight() )
	{
		m_pPlayer->SetFight( false );		
		SetAction( eAction_Idle );
	}
}

// --------------------------------------------------------------------------------------
void CWorldFrame::SetAction( int action )
{
	if ( m_pPlayer && SUCCEEDED( m_pPlayer->SetAction(action) ) )
	{
		switch ( action )
		{
		case eAction_Idle:
		case eAction_Fight_Idle:
		case eAction_Sit:
		case eAction_StandUp:
		case eAction_Sleep:
		case eAction_Holster:
		case eAction_Unholster:
			SendDoAction( action, m_pPlayer->GetStateFlag() );
			break;
		}
	}
}

// --------------------------------------------------------------------------------------
void CWorldFrame::ChangeCursor()
{
	if ( g_pMainMenu->PasteItemClipboard() )
		return;
	
	if ( m_pMouseInPlayer )
	{
		switch ( m_pMouseInPlayer->GetCheckedStatus() )
		{
		case eChecked_Null:
		case eChecked_RangePlot:
			g_pGameMain->SetCursorIcon(CURSOR_NORMAL_POINT);
			break;

		case eChecked_Fight:
			g_pGameMain->SetCursorIcon(CURSOR_ATTACK);
			break;

		case eChecked_TouchPlot:
		case eChecked_QuestObj:
			g_pGameMain->SetCursorIcon(CURSOR_QUEST_POINT);
			break;

		case eChecked_QuestNPC:
			g_pGameMain->SetCursorIcon(CURSOR_QUEST_SPEAK);
			break;
		}
	}
	else
		g_pGameMain->SetCursorIcon(CURSOR_NORMAL_POINT);
}

// --------------------------------------------------------------------------------------
void CWorldFrame::LockCursor()
{
	g_pGameMain->LockCursorPos();
	g_pGameMain->SetCursorVisible( false );
}

// --------------------------------------------------------------------------------------
void CWorldFrame::UnlockCursor()
{
	if ( !(m_mouseState & eMouse_Flag_LButton) && !(m_mouseState & eMouse_Flag_RButton) )
	{
		g_pGameMain->UnlockCursorPos();
		g_pGameMain->SetCursorVisible( true );
	}	
}

// --------------------------------------------------------------------------------------
void RegisterWorldFrameFunc()
{
	g_pLuaScript->Register("Control_OnUpdate"		, Control_OnUpdate);
	g_pLuaScript->Register("Control_OnMouseDown"	, Control_OnMouseDown);
	g_pLuaScript->Register("Control_OnMouseUp"		, Control_OnMouseUp);
	g_pLuaScript->Register("Control_OnMouseWheel"	, Control_OnMouseWheel);
	g_pLuaScript->Register("Control_OnMouseEnter"	, Control_OnMouseEnter);
	g_pLuaScript->Register("Control_OnMouseLeave"	, Control_OnMouseLeave);
	g_pLuaScript->Register("Control_OnReceiveDrag"	, Control_OnReceiveDrag);
	g_pLuaScript->Register("GetFramerate"			, GetFramerate);
	g_pLuaScript->Register("GetTime"				, GetTime );
	g_pLuaScript->Register("GetPlayerID"			, GetPlayerID );
	g_pLuaScript->Register("GetPlayerPos"			, GetPlayerPos );
}

// --------------------------------------------------------------------------------------
int Control_OnUpdate (lua_State *L)
{
	luaStack l(L);
	g_pWorldFrame->GameWorldUpdate( l.GetParam<float>(1) );
	return 0;
}

// --------------------------------------------------------------------------------------
int Control_OnMouseDown (lua_State *L)
{
	luaStack l(L);
	const char* mouseKey = l.GetParam<const char*>(1);
	if ( mouseKey )
	{
		if ( strcmp( mouseKey, "LBUTTON" ) == 0 )
			g_pWorldFrame->MouseDown( eMouse_Flag_LButton );
		else if ( strcmp( mouseKey, "RBUTTON" ) == 0 )
			g_pWorldFrame->MouseDown( eMouse_Flag_RButton );
		else if ( strcmp( mouseKey, "MBUTTON" ) == 0 )
			g_pWorldFrame->MouseDown( eMouse_Flag_MButton );
		else if ( strcmp( mouseKey, "XBUTTON" ) == 0 )
			g_pWorldFrame->MouseDown( eMouse_Flag_XButton );
	}	
	return 0;
}

// --------------------------------------------------------------------------------------
int Control_OnMouseUp (lua_State *L)
{
	luaStack l(L);
	const char* mouseKey = l.GetParam<const char*>(1);
	if ( mouseKey )
	{
		if ( strcmp( mouseKey, "LBUTTON" ) == 0 )
			g_pWorldFrame->MouseUp( eMouse_Flag_LButton );
		else if ( strcmp( mouseKey, "RBUTTON" ) == 0 )
			g_pWorldFrame->MouseUp( eMouse_Flag_RButton );
		else if ( strcmp( mouseKey, "MBUTTON" ) == 0 )
			g_pWorldFrame->MouseUp( eMouse_Flag_MButton );
		else if ( strcmp( mouseKey, "XBUTTON" ) == 0 )
			g_pWorldFrame->MouseUp( eMouse_Flag_XButton );
	}	
	return 0;
}

// --------------------------------------------------------------------------------------
int Control_OnMouseWheel (lua_State *L)
{
	luaStack l(L);
	g_pWorldFrame->MouseWheel( l.GetParam<int>(1) );
	return 0;
}

// --------------------------------------------------------------------------------------
int Control_OnMouseEnter (lua_State *L)
{
	g_pWorldFrame->MouseEnter();
	return 0;
}

// --------------------------------------------------------------------------------------
int Control_OnMouseLeave (lua_State *L)
{
	g_pWorldFrame->MouseLeave();
	return 0;
}

// --------------------------------------------------------------------------------------
int Control_OnReceiveDrag (lua_State *L)
{
	g_pWorldFrame->ReceiveDrag();
	return 0;
}

// --------------------------------------------------------------------------------------
int GetFramerate(lua_State *L)
{
	luaStack l(L);
	l.Push( g_pGameMain->GetFramerate() );
	return 1;
}

// --------------------------------------------------------------------------------------
int GetTime(lua_State *L)
{
	luaStack l(L);
	l.Push( g_pGameMain->GetFrameTime() );
	return 1;
}

// --------------------------------------------------------------------------------------
int GetPlayerID (lua_State *L)
{
	luaStack l(L);
	l.Push( g_pWorldFrame->GetPlayerID() );
	return 1;
}

// --------------------------------------------------------------------------------------
int	GetPlayerPos (lua_State *L)
{
	luaStack l(L);
	CRoleSprite* pPlayer = g_pGameMain->FindRoleSprite( l.GetParam<int>(1) );
	if ( pPlayer )
	{
		CRuVector3 pos = pPlayer->GetPos() - g_offsetPosstion;
		l.Push( pos.m_x );
		l.Push( pos.m_y );
		l.Push( pos.m_z );
		return 3;
	}
	else
		return 0;
}