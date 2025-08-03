#include <windows.h>
#include "sprite.h"
#include "../ErrorList/errorlist.h"

#define EFFECT_LINE_ACCELERATION	50.0f

//-----------------------------------------------------------------------------
CEffectSprite::CEffectSprite(bool isAddToContainer)
	: IBaseSprite(isAddToContainer)
{
	m_classType			= eSpriteClass_Effect;

	// Override shadow type -- set to none
	m_shadowType		= eSpriteShadowType_None;

	m_pRelative			= NULL;
	m_pPolyLineSprite	= NULL;
	m_polyLinePoint		= eAttach_Null;
	m_attachPoint		= eAttach_Null;
	m_effectMotionStep	= eEffectMotion_Step_Null;	
	m_isStartUpdate		= true;
	m_isAutoDestruct	= true;
	m_isEnableSound		= true;

	m_arg1				= 0;
	m_arg2				= 0;

	m_effectStepTime	= 0.0f;
	m_explodeLeftTime	= 0.0f;	
	m_idleEffectLiveTime = 0.0f;
}

//-----------------------------------------------------------------------------
CEffectSprite::~CEffectSprite()
{
	if ( m_OnRelease.callback )
		m_OnRelease.callback(this, m_OnRelease.userData);

	DetachSprite();	

	m_pPolyLineSprite = NULL;
	m_polyLinePoint = eAttach_Null;

	m_pRelative = NULL;
	m_attachPoint = eAttach_Null;
}

//-----------------------------------------------------------------------------
BOOL CEffectSprite::SetPloyLineArgument(CRuEntity* entity, float Deviation, float VSpeed, float ParabolaHeight, float segmentLength)
{
	BOOL retVal = FALSE;

	// if the current entity is a poly line object, set its target
	if(entity->GetType().IsSubClassOf(CRuEntity_PolyLine::Type()))
	{
		static_cast<CRuEntity_PolyLine *>(entity)->SetDeviation(Deviation);
		static_cast<CRuEntity_PolyLine *>(entity)->SetVSpeed(VSpeed);
		static_cast<CRuEntity_PolyLine *>(entity)->SetParabolaHeight(ParabolaHeight);
		static_cast<CRuEntity_PolyLine *>(entity)->SetSegmentLength(segmentLength);
		retVal = TRUE;
	}

	// Iterate through all children
	CRuEntity *curChild = entity->GetFirstChild();
	while(curChild)
	{
		retVal = SetPloyLineArgument(curChild, Deviation, VSpeed, ParabolaHeight, segmentLength) | retVal;

		// Advance to next sibling
		curChild = curChild->GetNextSibling();
	}

	return retVal;
}

//-----------------------------------------------------------------------------
void CEffectSprite::ReceiveDestruct(IBaseSprite* pSprite)
{
	if (pSprite == m_pRelative)
	{		
		m_pRelative->DetachSprite(this);
		m_pRelative = NULL;
		SetDestructTime(0.5f);
	}
	if (pSprite == m_pPolyLineSprite)
	{
		m_pPolyLineSprite = NULL;
		SetDestructTime(0.5f);
	}
}

//-----------------------------------------------------------------------------
void CEffectSprite::Update(float elapsed)
{
	if ( m_OnUpdate.callback )
		m_OnUpdate.callback(this, m_OnUpdate.userData);

	if ( !m_isStartUpdate )
	{
		CRuVector3 attachPoint;

		if ( m_pRelative == NULL || m_pRelative->IsProxyLoadComplete() == false )
			return;		

		switch (m_attachPoint)
		{
		case eAttach_Freeze_Down:
			SetPos( m_pRelative->GetPos() );
			SetDirection( m_pRelative->GetDirection(), true, false );
			break;

		case eAttach_Freeze_Hit:				
			if ( m_pRelative->GetAttachPos(eAttach_Freeze_Hit, attachPoint) )
				SetPos( attachPoint );
			SetDirection( m_pRelative->GetDirection(), true, false );
			break;

		default:
			if ( m_pRelative->AttachSprite( this, m_attachPoint ) == false )
				SetPos( m_pRelative->GetPos() );
			break;
		}
		m_isStartUpdate = true;
	}

	// 避免產生影子
	if ( m_pRuEntity )
	{
		RuEntity_SetOptionWithTypeFilter(m_pRuEntity, ruENTITYOPTION_CASTERCLASS, 0, CRuModel::Type());
		RuEntity_SetOptionWithTypeFilter(m_pRuEntity, ruENTITYOPTION_CASTERCLASS, 0, CRuCompositeModel::Type());
	}

	UpdateEffectMotion(elapsed);
	IBaseSprite::Update(elapsed);
}

//-----------------------------------------------------------------------------
void CEffectSprite::Render()
{
	// 關連物件(RoleSprite)本身不顯示,特效也隱藏
	if ( m_pRelative && !m_pRelative->CheckRenderState() )
		return;

	IBaseSprite::Render();
}

//-----------------------------------------------------------------------------
bool CEffectSprite::IsVisible()
{
	if ( m_pRelative && m_pRelative->IsVisible() == false )
		return false;
	return IBaseSprite::IsVisible();
}

//-----------------------------------------------------------------------------
void CEffectSprite::UpdateEffectMotion(float elapsed)
{
	m_effectStepTime += elapsed;
	switch ( m_effectMotionStep )
	{
	case eEffectMotion_Step_Appear:
		if ( IsMotionComplete() )
			SetEffectMotionStep(eEffectMotion_Step_Idle);
		break;

	case eEffectMotion_Step_Idle:
		if ( m_idleEffectLiveTime > 0.0f ) {
			m_idleEffectLiveTime -= elapsed;
			if ( m_idleEffectLiveTime < 0.001f ) {
				SetEffectMotionStep( eEffectMotion_Step_Disappear );
			}
		}
		break;

	case eEffectMotion_Step_Disappear:
		if ( IsMotionComplete() || m_effectStepTime > 5.0f )
		{
			if ( m_classType == eSpriteClass_Effect_Line )
			{
				//g_pError->AddMessage(0, 0, "Disappear Time %.3f", m_effectStepTime);
			}
			SetEffectMotionStep( eEffectMotion_Step_Dead, NULL, true );
		}
		break;

	case eEffectMotion_Step_Explode:
		if (m_effectStepTime == 0.0f)
			MotionEvent(ruENTITYEVENT_HIT);

		if ( IsMotionComplete() || m_effectStepTime > m_explodeLeftTime )
			SetEffectMotionStep( eEffectMotion_Step_Dead );
		break;

	case eEffectMotion_Step_Dead:
		s_pRuEventListener->StopSpriteSounds(this);
		break;
	}
}

//-----------------------------------------------------------------------------
void CEffectSprite::DetachSprite()
{
	m_attachPoint = eAttach_Null;

	if ( m_pRelative )
	{
		m_pRelative->DetachSprite(this);
	}
}

//-----------------------------------------------------------------------------
void CEffectSprite::AttachSprite(IBaseSprite* pRelative, int point, CRuVector3* offset)
{
	DetachSprite();

	if ( pRelative )
	{
		CRuEntity* entity = pRelative->GetRuEntity();
		if ( entity && offset )
		{
			entity->Translate(*offset);
			entity->Update(0.0f);
		}
	}

	m_pRelative		= pRelative;
	m_attachPoint	= point;
	m_isStartUpdate = false;
}

//-----------------------------------------------------------------------------
void CEffectSprite::SetPolyLineTarget(IBaseSprite* target, int point)
{
	if ( m_pRuEntity )
	{
		m_pPolyLineSprite = target;
		m_polyLinePoint = point;

		if ( m_pPolyLineSprite )
		{
			CRuEntity* entity = m_pPolyLineSprite->FindLinkEntity(point);
			if ( entity )
			{
				if ( GetPauseTime() <= 0.0f )
				{
					Update(0.0f);
				}

				// 
				RuPolyLine_SetTarget(m_pRuEntity, entity);
			}
		}
	}

	float scale = 1.0f;
	if ( m_pRelative )
	{
		CRuEntity* entity = m_pRelative->FindLinkEntity(m_attachPoint);
		if ( entity ) {
			scale /= entity->GetScale(ruFRAMETYPE_WORLD);
		}
		scale /= m_pRelative->GetScale();
	}
	if ( m_pPolyLineSprite )
		scale /= max(m_pPolyLineSprite->GetScale(), 1.0f);

	SetScale(scale);
}

//-----------------------------------------------------------------------------
void CEffectSprite::SetReleaseCallback(EfectSpriteCallback callback, int userdata)
{
	m_OnRelease.callback = callback;
	m_OnRelease.userData = userdata;
}

//-----------------------------------------------------------------------------
void CEffectSprite::SetActionEventCallback(EfectSpriteCallback callback, int userdata)
{
	m_OnActionEvent.callback = callback;
	m_OnActionEvent.userData = userdata;
}

//-----------------------------------------------------------------------------
void CEffectSprite::SetUpdateCallback(EfectSpriteCallback callback, int userdata)
{
	m_OnUpdate.callback = callback;
	m_OnUpdate.userData = userdata;
}

//-----------------------------------------------------------------------------
CRuEntity* CEffectSprite::GetRelativeRuEntity()
{
	CRuEntity* entity = NULL;
	if ( m_pRelative )
		entity = m_pRelative->FindLinkEntity(m_attachPoint);
	return entity;
}

//-----------------------------------------------------------------------------
CRuEntity* CEffectSprite::GetPolyLineRuEntity()
{
	CRuEntity* entity = NULL;
	if ( m_pPolyLineSprite )
		entity = m_pPolyLineSprite->FindLinkEntity(m_polyLinePoint);
	return entity;
}

//-----------------------------------------------------------------------------
void CEffectSprite::SetEffectMotionStep(int step, CRuACT_ActionEvents *actionEventsOut, bool immediately)
{
	m_effectMotionStep	= step;
	m_effectStepTime	= 0.0f;
	m_explodeLeftTime	= 0.0f;

	switch (m_effectMotionStep)
	{
	case eEffectMotion_Step_Appear:		
		if ( !PlayMotion("appear", actionEventsOut) )
			SetEffectMotionStep(eEffectMotion_Step_Idle, actionEventsOut);
		break;

	case eEffectMotion_Step_Idle:
		if ( !PlayMotion("idle") && !PlayMotion("stand_idle01") )
			SetEffectMotionStep(eEffectMotion_Step_Disappear, actionEventsOut);		
		break;

	case eEffectMotion_Step_Disappear:
		if ( !PlayMotion("disappear", actionEventsOut) )
			SetEffectMotionStep(eEffectMotion_Step_Dead, actionEventsOut, true);
		break;

	case eEffectMotion_Step_Explode:
		{
			m_explodeLeftTime	= 10.0f;

			CRuACT_ActionEvents *actionEvents = actionEventsOut ? actionEventsOut : ruNEW CRuACT_ActionEvents();

			if ( !PlayMotion("explode", actionEvents) )
				SetEffectMotionStep(eEffectMotion_Step_Dead, actionEvents);
			else
			{			
				CRuController_EventTrigger* eventTrigger;
				if ( (eventTrigger = actionEvents->GetEventController(ruENTITYEVENT_EFFECT_FINISHED)) != NULL )
					m_explodeLeftTime = eventTrigger->GetStartTime();
			}

			if(actionEvents != actionEventsOut)
			{
				ruSAFE_RELEASE(actionEvents);
			}
		}

		break;

	case eEffectMotion_Step_Dead:
		if ( m_isAutoDestruct )
		{
			if ( immediately )
				SetDestructTime(0.0f);
			else
				SetDestructTime(3.0f);
		}
		break;
	}
}

//-----------------------------------------------------------------------------
void CEffectSprite::SetIdleEffectLiveTime(float time)
{
	m_idleEffectLiveTime = time;
}

//-----------------------------------------------------------------------------
CLineEffectSprite::CLineEffectSprite(bool isAddToContainer)
	: CEffectSprite(isAddToContainer)
{
	m_classType				= eSpriteClass_Effect_Line;

	// Override shadow type -- set to none
	m_shadowType			= eSpriteShadowType_None;
	m_targetType			= EffectTarget_Sprite;

	m_pBegin				= NULL;
	m_beginPoint			= eAttach_Null;
	m_delayEndTime			= 0.0f;
	m_velocity				= 0.0f;
	m_startMoveDelayTime	= 0.0f;
	m_targetPosition.m_x	= 0.0f;
	m_targetPosition.m_y	= 0.0f;
	m_targetPosition.m_z	= 0.0f;
}

//-----------------------------------------------------------------------------
void CLineEffectSprite::ReceiveDestruct(IBaseSprite* pSprite)
{
	if ( m_pBegin == pSprite )
		m_pBegin = NULL;
	CEffectSprite::ReceiveDestruct(pSprite);
}

//-----------------------------------------------------------------------------
void CLineEffectSprite::SetMoveEndCallback(EfectSpriteCallback callback, int userdata)
{
	m_OnMoveEnd.callback = callback;
	m_OnMoveEnd.userData = userdata;
}

//-----------------------------------------------------------------------------
void CLineEffectSprite::Update(float elapsed)
{
	CRuVector3 pos, dest;	

	// 初始發射座標
	if ( m_pBegin )
	{
		// 取得遠程武器射擊坐標點
		if ( ((CRoleSprite*)m_pBegin)->GetShootAttachPos(pos) == false )
		{			
			m_pBegin->GetAttachPos(m_beginPoint, pos);
		}
		SetPos(pos);

		m_pBegin = NULL;
		m_beginPoint = eAttach_Null;

		// 指定初始方向
		switch (m_targetType)
		{
		case EffectTarget_Sprite:
			if (m_pRelative) m_pRelative->GetAttachPos(m_attachPoint, dest);
			break;

		case EffectTarget_LinePosition:
			dest = m_targetPosition;
			break;
		}
		SetDirection(dest - pos);
	}
	else if ( m_pRelative != 0 || m_targetPosition != 0.0f )
	{
		switch (m_targetType)
		{
		case EffectTarget_Sprite:			
			if (m_pRelative) m_pRelative->GetAttachPos(m_attachPoint, pos);
			break;

		case EffectTarget_LinePosition:
			pos = m_targetPosition;
			break;
		}

		switch(m_effectMotionStep)
		{
		case eEffectMotion_Step_Appear:
		case eEffectMotion_Step_Idle:
		case eEffectMotion_Step_Explode:
			if ( m_delayEndTime > 0.001f )
			{
				m_delayEndTime -= elapsed;
				if ( m_delayEndTime < 0.001f )
				{
					SetEffectMotionStep( eEffectMotion_Step_Disappear );
				}
			}
			else
			{
				CRuVector3 dir = m_direction;

				if ( pos != m_nowPos )
				{
					dir = pos - m_nowPos;
					SetDirection(dir.Normalize());
				}

				if ( m_startMoveDelayTime < 0.01f )
				{	
					CRuVector3 forward, dir2;
					/*
					float velocity = elapsed * EFFECT_LINE_ACCELERATION + m_velocity;
					forward = dir * ((velocity*velocity - m_velocity*m_velocity) / (2*EFFECT_LINE_ACCELERATION));
					*/
					float velocity = m_velocity;
					forward = dir * max(elapsed * m_velocity, 0.1f);
					dir2 = (pos - (m_nowPos + forward)).Normalize();

					if ( dir2.Dot(dir) < -0.92388 )
					{
						m_velocity = 0.0f;
						SetPos(pos);
						
						if ( m_OnMoveEnd.callback )
							m_OnMoveEnd.callback(this, m_OnMoveEnd.userData);

						// 對地攻擊立即刪除
						if ( m_targetType == EffectTarget_LinePosition ) 
						{
							SetEffectMotionStep(eEffectMotion_Step_Dead, 0, true);
						}
						else
						{
							m_delayEndTime = 0.25f;
						}
					}
					else
					{
						SetPos(m_nowPos + forward);
					}
					m_velocity = velocity;
					m_startMoveDelayTime = 0.0f;
				}
				else
					m_startMoveDelayTime -= elapsed;
			}
			break;

		case eEffectMotion_Step_Disappear:
		case eEffectMotion_Step_Dead:
			m_velocity = 0.0f;
			break;
		}
	}
	else if ( !m_isDestruct )
	{
		SetDestructTime(0.5f);	
	}

	UpdateEffectMotion(elapsed);

	IBaseSprite::Update(elapsed);
}

//-----------------------------------------------------------------------------
void CLineEffectSprite::SetDirection(CRuVector3 direction, bool IsImmediate)
{
	if ( direction.m_x == 0.0f && direction.m_y == 0.0f && direction.m_z == 0.0f )
		return;

	if ( m_direction != direction.Normalize() )
	{
		m_direction = direction;
		//		SetRuEntityDirection(m_pRuEntity, m_direction, m_scale);

		CRuQuaternion orientQuat;
		orientQuat.RotateIntoVector(direction, CRuVector3(0.0f, 0.0f, 1.0f));

		m_pRuEntity->SetLocal3x3Transform(orientQuat.GetMatrix());

		m_pRuEntity->Scale(m_scale, ruTRANSFORMOP_SET);
	}
}

//-----------------------------------------------------------------------------
void CLineEffectSprite::SetBeginSprite(IBaseSprite* pSprite, int point)
{
	m_pBegin = pSprite;
	m_beginPoint = point;
}

//-----------------------------------------------------------------------------
void CLineEffectSprite::SetEndSprite(IBaseSprite* pSprite, int point)
{
	m_targetType = EffectTarget_Sprite;
	m_pRelative = pSprite;
	m_attachPoint = point;
}

//-----------------------------------------------------------------------------
void CLineEffectSprite::SetTargetPosition(CRuVector3 position)
{
	m_targetType = EffectTarget_LinePosition;
	m_targetPosition = position;
}

//-----------------------------------------------------------------------------
void CLineEffectSprite::SetMoveSpeed(float speed)
{
	m_moveSpeed = speed;
	m_velocity = speed;
}

//-----------------------------------------------------------------------------
float CLineEffectSprite::GetFlyTime()
{
	float time = m_startMoveDelayTime;
	if ( m_velocity > 0.0f )
	{
		CRuVector3 src = m_nowPos;	
		if ( m_pBegin )
			m_pBegin->GetAttachPos(m_beginPoint, src);

		CRuVector3 dest;
		switch (m_targetType)
		{
		case EffectTarget_Sprite:
			m_pRelative->GetAttachPos(m_attachPoint, dest);
			break;

		case EffectTarget_LinePosition:
			dest = m_targetPosition;
			break;
		}
		
		if (m_pRelative)
		{
			CRuVector3 dest;
			m_pRelative->GetAttachPos(m_attachPoint, dest);

			CRuVector3 dir = dest - src;
			float v = sqrt(m_velocity*m_velocity + 2.0f*EFFECT_LINE_ACCELERATION*dir.Magnitude());
			time += (v - m_velocity) / EFFECT_LINE_ACCELERATION;
		}
	}
	return time;
}

//-----------------------------------------------------------------------------
CParabolaEffectSprite::CParabolaEffectSprite(bool isAddToContainer)
	: CLineEffectSprite(isAddToContainer)
{
	m_classType				= eSpriteClass_Effect_Parabola;

	m_moveTime				= 0.0f;
	m_updateTime			= 0.0f;
	m_parabolaHeight		= 50.0f;
}

//-----------------------------------------------------------------------------
float CParabolaEffectSprite::GetMagicParabolaTime(CRuVector3& p1, CRuVector3& p2, int parabolaHeight, float speed)
{
	float distance = (p1 - p2).Magnitude();

	if ( parabolaHeight != 0 )
	{
		CRuVector3 forward = (p2 - p1).Normalize();
		CRuVector3 p4 = p1;
		int count = (int)(distance / 15.0f);
		float power = distance / count;

		distance = 0.0f;
		for ( int i = 1; i <= count; i++ )
		{
			CRuVector3 p3 = p1 + forward * power * (REAL)i;

			// Scale T such that its range is in [-1, 1]
			REAL t = (REAL) i / (count) * 2.0f - 1.0f;

			REAL adjustedY = parabolaHeight - t * t * parabolaHeight;

			p3.m_y += adjustedY;

			distance += (p3 - p4).Magnitude();

			p4 = p3;
		}					
	}	
	return distance / speed;
}

//-----------------------------------------------------------------------------
void CParabolaEffectSprite::Update(float elapsed)
{
	// 初始發射座標
	if ( m_pBegin )
	{
		CRuVector3 position, forward;
		m_pBegin->GetAttachPos(m_beginPoint, position);
		SetPos(position);

		// 目標物件抓取坐標點
		if ( m_pRelative ) m_pRelative->GetAttachPos(m_attachPoint, m_targetPosition);

		m_pBegin = NULL;
		m_beginPoint = eAttach_Null;

		// 計算到達所需時間
		m_beginPosition = position;
		m_moveTime = GetMoveTime();
	}
	else
	{
		switch(m_effectMotionStep)
		{
		case eEffectMotion_Step_Appear:
		case eEffectMotion_Step_Idle:
		case eEffectMotion_Step_Explode:
			// 延遲刪除時間
			if ( m_delayEndTime > 0.001f )
			{
				m_delayEndTime -= elapsed;
				if ( m_delayEndTime < 0.001f )
				{
					SetEffectMotionStep( eEffectMotion_Step_Disappear );
				}
			}
			else
			{	
				// 延遲開始移動時間
				if ( m_startMoveDelayTime > 0.001f)
				{
					m_startMoveDelayTime -= elapsed;
				}
				else
				{
					if ( m_updateTime < m_moveTime )
					{
						CRuVector3 forward, nowPos;
						forward = m_targetPosition - m_beginPosition;

						// X, Z 軸坐標
						nowPos = m_beginPosition + (forward * (m_updateTime / m_moveTime));

						// Scale T such that its range is in [-1, 1]
						REAL t = m_updateTime / m_moveTime * 2.0f - 1.0f;

						REAL adjustedY = m_parabolaHeight - t * t * m_parabolaHeight;

						nowPos.m_y += adjustedY;

						// Y 軸坐標
						//nowPos.m_y = m_beginPosition.m_y + (m_moveTime * 98.0f * 0.5f) * m_updateTime + 0.5f * -98.0f * m_updateTime * m_updateTime;

						SetPos(nowPos);
					}
					else
					{
						SetPos(m_targetPosition);

						SetEffectMotionStep(eEffectMotion_Step_Dead, 0, true);

						// 觸發到達事件
						if ( m_OnMoveEnd.callback )
							m_OnMoveEnd.callback(this, m_OnMoveEnd.userData);
					}
					m_updateTime += elapsed;
				}
			}
			break;
		}
	}	

	UpdateEffectMotion(elapsed);
	IBaseSprite::Update(elapsed);
}

//-----------------------------------------------------------------------------
float CParabolaEffectSprite::GetMoveTime()
{
	return GetMagicParabolaTime(m_nowPos, m_targetPosition, (int)m_parabolaHeight, m_moveSpeed);
}

//-----------------------------------------------------------------------------
void CParabolaEffectSprite::SetParabolaHeight(float parabolaHeight)
{
	m_parabolaHeight = parabolaHeight;
}