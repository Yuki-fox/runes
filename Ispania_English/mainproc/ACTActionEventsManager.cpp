/***************************************************************
³o¬O¤@­Ó³B²z¦³Ãö°Ê§@»P§ðÀ»(ªk³N)«Ê¥]¬ÛÃöµ{¦¡

§ð’°«Ê¥]¡G
	«Ê¥]¶i¨Ó«á¥Ñ GetACTActionEvent() ¨ç¦¡²£¥Í¤@­ÓªÅªº¨Æ¥óµ²ºc¡A¦b±N«Ê¥]¤º©Ò¦³°Ñ¼Æ¼È®ÉÀx¦s¦b ACTActionEventsInfo µ²ºc¤º
	·í°Ê§@¨Æ¥ó EventCallback Ä²µo®É¡A±N­ì¨Ó«Ê¥]¤ºªº°Ñ¼Æ¨ú¥X¨Ã²£¥Í¯S®Äª«¥ó¥H¤Î¼Æ­ÈÅã¥Üª«¥ó¡C	

ªk³N«Ê¥]¡G
	ªk³Nªº«Ê¥]¬yµ{¤ñ¸û´_Âø¡A¦³»Î±µªº°ÝÃD¦b(¥i¯à¦³¤£©T©wªº«Ê¥]­Ó¼Æ¥H¤Î»Ý­nªí²{ A¯S®Äµ²§ô¦b±µB,C¯S®Äµ¥°ÝÃD)¡A©Ò¥H±N«Ê¥]ªº°Ñ¼Æ
	µ²ºc©w¸q¬°	

	CNetMagicPackageData ¹ïÀ³¦øªA¾¹°e¥Xªº¤@­Óªk³N¬ÛÃöµ²ºc¥Ñ BeginSpell() ²£¥Í EndSpell() ³qª¾§R°£, m_id ¬°¬ÛÃö«Ê¥]ªº¯Á¤Þ­È
	SpellShootPackage ¹ïÀ³¦øªA¾¹°e¥Xªº¤@­Óªk³N²£¥Í¤£¦Pªº³æÅé®ÄªG(EX ½d³ò¤ºªº¤pÃz¬µ), m_list ¬°¤@­Ó¶Ë®`²M³æ
	SingleMagicPackage ¨C¤@µ§¶Ë®`²M³æ¤º®e(¤£¦P¤H²£¥Í¤£¦Pµ²ªG)

	¬°¤F¹F¨ìµøÄ±ªº¥¿½Tªí²{¡A¤j³¡¥÷ªº«Ê¥]°Ê§@¥H¤Î«Ê¥]¯S®Ä³£¥²»Ý°µÀx¦s¡A¦b¤¶¥Ñ¨Æ¥óÄ²°Ê¤è¦¡¦b¾A·í®É¶¡°µ³B²z¡C¦]¦¹¬°¤FÁ×§K«ü¼Ð
	¤§¶¡ªº¦s¦b°ÝÃD(Âù¦Vªº«ü¼Ð³sµ²)¡Aª«¥ó¤§¶¡ªº·¾³q¨Ï¥Î¯Á¤Þ­È¨Ó°O¿ý¹ï¤è¡C


****************************************************************/

#include "GameMain.h"
#include "RoleData/ObjectDataClass.h"
#include "../netwaker_member/net_magic/NetCli_MagicChild.h"
#include "../Interface/WorldFrames/ScrollCombatTextFrame.h"
#include "../Interface/Debug/DebugMessage.h"

CACTActionEventsManager*		g_pACTActionEventsManager = NULL;

void EffectCallback_OnRelease(CEffectSprite* efftect, int userdata);
void EffectCallback_OnMoveEnd(CEffectSprite* efftect, int userdata);
void EffectCallback_OnUpdate(CEffectSprite* efftect, int userdata);

ACTActionEventsInfo::ACTActionEventsInfo()
{
	m_actionEvent = NULL;

	Clear();
}

ACTActionEventsInfo::~ACTActionEventsInfo()
{
	ruSAFE_RELEASE(m_actionEvent);
}

void ACTActionEventsInfo::Clear()
{
	m_type = ACT_ACTIONEVENTS_TYPE_NONE;
	m_source = 0;
	m_target = 0;	
	m_liveTime = 0.0f;

	if ( m_actionEvent )
	{
		m_actionEvent->DisableEventTriggerCallback();				// Ãö³¬callback
		m_actionEvent->Reset();
	}
}

void ACTActionEventsInfo::Reset()
{
	m_liveTime = 0.0f;
	if ( m_actionEvent == NULL )
		m_actionEvent = ruNEW CRuACT_ActionEvents();
	m_actionEvent->Reset();
}

void ACTActionEventsInfo::Recyele()
{
	m_liveTime = 25.0f;
	if ( m_actionEvent )
		m_actionEvent->DisableEventTriggerCallback();				// Ãö³¬callback
}

void ACTActionEventsInfo::Update(float elapsedTime)
{
	m_liveTime += elapsedTime;
}

CNetMagicPackageData::CNetMagicPackageData()
{
	m_idleExplode		= NULL;

	Clear();
}

void CNetMagicPackageData::Clear()
{
	// EndSpell();

	m_refCount			= 0;
	m_id				= 0;
	m_magic				= 0;
	m_owner				= 0;
	m_target			= 0;
	m_spellTime			= 0;
	m_shootIndex		= 0;
	m_x					= 0.0f;
	m_y					= 0.0f;
	m_z					= 0.0f;
	m_destructTime		= 0.0f;
	m_isActionDone		= false;
	m_idleExplode		= NULL;	

	m_shoot.clear();
}

void CNetMagicPackageData::Update(float elapsedTime)
{
	m_destructTime -= elapsedTime;

	if ( m_refCount > 0 ) {		
		m_destructTime = 2.0f;
	} else if ( m_destructTime > 2.0f ) {
		m_destructTime = 2.0f;
	}
}

void CNetMagicPackageData::Release()
{
	// m_destructTime = 10.0f;	
}

void CNetMagicPackageData::ReleaseEffect(CEffectSprite* sprite)
{
	// ¤¤Â_©Ò¦³¨S¦³¦Û°Ê
	for ( vector<SpellShootPackage>::iterator iter = m_shoot.begin(); iter != m_shoot.end(); iter++ )
	{
		for ( vector<ShootFlyEffectStruct>::iterator shoot_iter = iter->m_flyEffect.begin(); shoot_iter != iter->m_flyEffect.end(); shoot_iter++ )
		{
			if ( shoot_iter->fly == sprite )
				shoot_iter->fly = NULL;
		}
	}

	// ´`Àô¯S®ÄÅý¯S®Ä¤¤¤î
	if ( m_idleExplode == sprite )
		m_idleExplode = NULL;
}

void CNetMagicPackageData::EndSpell()
{
	DelRefCount();

	// ¤¤Â_©Ò¦³¨S¦³¦Û°Ê
	for ( vector<SpellShootPackage>::iterator iter = m_shoot.begin(); iter != m_shoot.end(); iter++ )
	{
		for ( vector<ShootFlyEffectStruct>::iterator shoot_iter = iter->m_flyEffect.begin(); shoot_iter != iter->m_flyEffect.end(); shoot_iter++ )
		{
			if ( IBaseSprite::CheckedSpriteIsLife(shoot_iter->fly) )
			{
				if ( shoot_iter->fly && shoot_iter->fly->IsAutoDestruct() == false )
				{
					SAFE_DELETE(shoot_iter->fly);
				}
			}
		}
	}
	
	// ´`Àô¯S®ÄÅý¯S®Ä¤¤¤î
	if ( m_idleExplode )
	{
		switch (m_idleExplode->GetEffectMotionStep())
		{
		case eEffectMotion_Step_Appear:					// ´`Àô¯S®Ä¶}©l
		case eEffectMotion_Step_Idle:					// ´`Àô¯S®Ä¤¤¶¡(Loop)
			m_idleExplode->SetEffectMotionStep(eEffectMotion_Step_Disappear);
			break;

		case eEffectMotion_Step_Disappear:				// ´`Àô¯S®Äµ²§ô
		case eEffectMotion_Step_Explode:				// Ãz¬µ
		case eEffectMotion_Step_Dead:					// µ²§ô
			break;
		}
	}
}

int CNetMagicPackageData::CreateShoot(int targetCount, int* targetList, CRuVector3 position)
{
	SpellShootPackage shoot;

	// ­pºâ¾¹¹ïÀ³¼W¥[
	// AddRefCount();
	
	// ³]©w¥Ø¼Ð¸s
	if ( targetCount > 0 )
	{
		ShootFlyEffectStruct flyEffect;
		for ( int i = 0; i < targetCount; i++ )
		{
			flyEffect.target	= targetList[i];
			flyEffect.fly		= NULL;
			flyEffect.targetPosition.Set(0.0f, 0.0f, 0.0f);
			shoot.m_flyEffect.push_back(flyEffect);
		}
	}
	// «ü©w³æ¤@¥Ø¼Ð
	else if ( m_target != 0 )
	{
		ShootFlyEffectStruct flyEffect;
		flyEffect.target	= m_target;
		flyEffect.fly		= NULL;
		flyEffect.targetPosition.Set(0.0f, 0.0f, 0.0f);
		shoot.m_flyEffect.push_back(flyEffect);
	}
	// ¹ï¦a¬I©ñ
	else if ( position != 0.0f )
	{
		ShootFlyEffectStruct flyEffect;
		flyEffect.target	= 0;
		flyEffect.fly		= NULL;
		flyEffect.targetPosition	= position;
		shoot.m_flyEffect.push_back(flyEffect);
	}
	m_shoot.push_back(shoot);
	return (int)m_shoot.size() - 1;
}

SpellShootPackage* CNetMagicPackageData::GetLastShootPackage()
{
	if ( m_shootIndex >= 0 && m_shootIndex < (int)m_shoot.size() )
		return &m_shoot[m_shootIndex];
	return NULL;
}

void CNetMagicPackageData::SetShootActionDone(int index)
{
	if ( index == -1 )
		index = (int)m_shoot.size() - 1;

	if ( (unsigned int)index < (unsigned int)m_shoot.size() )
		m_shoot[index].m_actionDone = true;

	/*
	SpellShootPackage* shoot = GetLastShootPackage();
	if ( shoot )
		shoot->m_actionDone = true;
	*/
}

//
// ·s¼W¤@­Ó¶Ë®`¯S®Ä
void CNetMagicPackageData::AddAttackData(int magicSerail, int target, int type, int magicBase, int valueType, int value)
{
	SpellShootPackage* shoot = GetLastShootPackage();
	if ( shoot )
	{
		SingleMagicPackage single;
		single.target				= target;
		single.attack.type			= type;
		single.attack.magicBase		= magicBase;
		single.attack.valueType		= valueType;
		single.attack.value			= value;
		single.attack.valueDeduct	= value;		

		GameObjDbStructEx* itemDB = g_ObjectData->GetObj(m_magic);
		if ( itemDB == NULL )
		{
			return;
		}

		// ¨Æ¥ý¥ý±N§ïÅÜ­È°µ°O¿ý,¥²§K¦]¬°®É¶¡¹L¤[³y¦¨®t­È
		switch(type)
		{
		case EM_MagicAtkResult_Normal:
		case EM_MagicAtkResult_Critial:
		case EM_MagicAtkResult_Parry:
			{
				CRoleSprite* sprite = g_pGameMain->FindSpriteByID(target);
				if ( sprite )
				{
					if ( sprite->GetManaType() == valueType )
						single.attack.valueDeduct = sprite->DeductMP(value, false);
					else if ( sprite->GetSkillType() == valueType )
						single.attack.valueDeduct = sprite->DeductSP(value, false);

					/*
					switch(valueType)
					{
					case EM_MagicAttackType_MP:
						single.attack.valueDeduct = sprite->DeductMP(value, false);
						break;

					case EM_MagicAttackType_SP_Warrior:
					case EM_MagicAttackType_SP_Ranger:
					case EM_MagicAttackType_SP_Rogue:
						single.attack.valueDeduct = sprite->DeductSP(value, false);
						break;
					}
					*/
				}
			}
			break;
		}

		//g_pError->AddMessage(0, 0, "magicSerail %d target %d type %d magicBase %d valueType %d value %d ", magicSerail, target, type, magicBase, valueType, value);

		// ¬O§_¦³­¸¦æ¯S®Ä¨Ã¥B¤£¬O¦ê³s¯S®Ä
		GameObjDbStructEx* imageDB = g_ObjectData->GetObj(itemDB->ImageID);
		if ( imageDB && imageDB->Image.FlyACT[0] != 0 && imageDB->Image.IsPolyLineEffect == false && imageDB->Image.MagicShowType != EM_GameObjectMagicShowType_PolyLine )
		{
			// ¬O§_¦³­¸¦æ¯S®Ä
			for ( vector<ShootFlyEffectStruct>::iterator iter_fly = shoot->m_flyEffect.begin(); iter_fly != shoot->m_flyEffect.end(); iter_fly++ )
			{
				// ·íªk³NÁÙ¦b­¸¦æ®É,¨Ã¥B¬°¥Ø«e
				if ( (iter_fly->fly != NULL || shoot->m_actionDone == false) && (iter_fly->target == target || shoot->m_flyEffect.size() == 1) )
				{
					vector<SingleMagicPackage>::iterator iter = shoot->m_list.begin();
					for ( ; iter != shoot->m_list.end(); iter++ )
					{
						// §ä´M¬Û¦P¥Ø¼Ð,¦ý¬O§ðÀ»«Ê¥]¬°ªÅªº¦ì¸m
						if ( iter->target == target && iter->attack.magicBase == 0 )
						{
							iter->attack.type		= type;
							iter->attack.magicBase	= magicBase;
							iter->attack.valueType	= valueType;
							iter->attack.value		= value;
							return;
						}
					}

					shoot->m_list.push_back(single);
					return;
				}
			}
		}
		// °Ê§@ÁÙ¨Sµ²§ô,«hµ¥«Ý°Ê§@¨Æ¥óÂIÄ²µo
		else if ( shoot->m_actionDone == false )
		{
			shoot->m_list.push_back(single);
			return;
		}

		// ¦pªG³£¤£²Å¦X¤W­±±ø¥ó,¥ß§Y²£¥ÍÃz¬µ
		if ( g_pACTActionEventsManager )
		{
			CNetMagicPackageData* magicData = g_pACTActionEventsManager->GetMagicData(magicSerail);
			g_pACTActionEventsManager->HandleMagicExtend(magicData, &single);
		}
	}
}

void CNetMagicPackageData::AddAssistData(int magicSerail, int target, int type, int magicBase, int magicLv, float time)
{
	SpellShootPackage* shoot = GetLastShootPackage();
	if ( shoot )
	{
		SingleMagicPackage single;
		single.target			= target;
		single.assist.type		= type;
		single.assist.magicBase	= magicBase;
		single.assist.magicLv	= magicLv;
		single.assist.time		= time;

		GameObjDbStructEx* itemDB = g_ObjectData->GetObj(m_magic);
		if ( itemDB == NULL )
		{
			return;
		}

		// ¬O§_¦³­¸¦æ¯S®Ä
		GameObjDbStructEx* imageDB = g_ObjectData->GetObj(itemDB->ImageID);
		if ( imageDB && imageDB->Image.FlyACT[0] != 0 && imageDB->Image.IsPolyLineEffect == false && imageDB->Image.MagicShowType != EM_GameObjectMagicShowType_PolyLine )
		{
			// ¬O§_¦³­¸¦æ¯S®Ä
			for ( vector<ShootFlyEffectStruct>::iterator iter_fly = shoot->m_flyEffect.begin(); iter_fly != shoot->m_flyEffect.end(); iter_fly++ )
			{
				if ( (iter_fly->fly != NULL || shoot->m_actionDone == false) && (iter_fly->target == target || shoot->m_flyEffect.size() == 1) )
				{
					vector<SingleMagicPackage>::iterator iter = shoot->m_list.begin();
					for ( ; iter != shoot->m_list.end(); iter++ )
					{
						// §ä´M¬Û¦P¥Ø¼Ð,¦ý¬O§ðÀ»«Ê¥]¬°ªÅªº¦ì¸m
						if ( iter->target == target && iter->assist.magicBase == 0 )
						{
							iter->assist.type		= type;
							iter->assist.magicBase	= magicBase;
							iter->assist.magicLv	= magicLv;
							iter->assist.time		= time;
							return;
						}
					}

					shoot->m_list.push_back(single);
					return;
				}
			}
		} else if ( shoot->m_actionDone == false ) {
			shoot->m_list.push_back(single);
			return;
		}	

		if ( g_pACTActionEventsManager )
		{
			CNetMagicPackageData* magicData = g_pACTActionEventsManager->GetMagicData(magicSerail);
			g_pACTActionEventsManager->HandleMagicExtend(magicData, &single);
		}
	}
}

CEffectSprite* CNetMagicPackageData::GetIdleExplode()
{	
	if ( m_idleExplode && IBaseSprite::CheckedSpriteIsLife(m_idleExplode) )
		return m_idleExplode;
	return NULL;
}

bool CNetMagicPackageData::CheckFlyEffectEnd()
{
	/*
	if ( m_flyEffect && IBaseSprite::CheckedSpriteIsLife(m_flyEffect) ) {
		switch (m_flyEffect->GetEffectMotionStep())
		{
		case eEffectMotion_Step_Appear:					// ´`Àô¯S®Ä¶}©l
		case eEffectMotion_Step_Idle:					// ´`Àô¯S®Ä¤¤¶¡(Loop)
			return false;

		case eEffectMotion_Step_Disappear:				// ´`Àô¯S®Äµ²§ô
		case eEffectMotion_Step_Explode:				// Ãz¬µ
		case eEffectMotion_Step_Dead:					// µ²§ô
			break;
		}
	}
	*/
	return true;
}

CACTActionEventsManager::CACTActionEventsManager()
{
	g_pACTActionEventsManager = this;
}

CACTActionEventsManager::~CACTActionEventsManager()
{
	Release();
	g_pACTActionEventsManager = NULL;
}

void CACTActionEventsManager::Release()
{
	m_actionEventInfo.Clear();
	m_magicData.Clear();
	m_usedEvent.clear();
	m_usedMagic.clear();
}

void CACTActionEventsManager::Update(float elapsedTime)
{
	// ¨Æ¥óµ²ºc§ó·s
	vector<ACTActionEventsInfo*> deletQueue;	
	for ( set<ACTActionEventsInfo*>::iterator iter = m_usedEvent.begin(); iter != m_usedEvent.end(); iter++ )
	{
		// ¦s¬¡®É¶¡¹Lªø,¤£½T©w­ì¦]¦s¬¡¹L¤[ÁÙ¬O»Ý­n²M°£
		if ( (*iter)->m_liveTime > 60.0f )
			deletQueue.push_back(*iter);
		else
			(*iter)->Update(elapsedTime);
	}
	for ( vector<ACTActionEventsInfo*>::iterator iter = deletQueue.begin(); iter != deletQueue.end(); iter++ )
		ReleaseACTActionEvent(*iter);

	vector<int> deleteList;
	for ( map<int, CNetMagicPackageData*>::iterator iter = m_usedMagic.begin(); iter != m_usedMagic.end(); iter++ )
	{
		CNetMagicPackageData* magicData = iter->second;
		bool finishEffect = false;				// µ²§ô©Ò¦³¯S®Ä

		// °Ê§@¤wµ²§ô,²M°£³Ñ¤UÃz¬µ¶Ë®`
		if ( magicData->m_isActionDone && magicData->CheckFlyEffectEnd() )
		{
			/*
			for ( vector<SpellShootPackage>::iterator iter = magicData->m_shoot.begin(); iter != magicData->m_shoot.end(); iter++ )
			{
				iter->m_flyEffect
			}

			HandleMagicResult(iter->first);
			*/
		}

		// §ó·s
		magicData->Update(elapsedTime);
		if ( magicData->m_destructTime > 0.0f ) {
			CRoleSprite* sprite = g_pGameMain->FindSpriteByID(magicData->m_owner);
			CRoleSprite* target = g_pGameMain->FindSpriteByID(magicData->m_target);
			if ( sprite == NULL || sprite->GetDeadState() || (target == NULL && magicData->m_x == 0.0f && magicData->m_y == 0.0f && magicData->m_z == 0.0f)  )
				finishEffect = true;
		} else {
			finishEffect = true;
			deleteList.push_back(iter->first);
		}
		
		if ( finishEffect )
		{
			//if ( magicData->CheckFlyEffectEnd() )
			// Ä²µo©Ò¦³³æÅéªk³N®ÄªG
			for ( int i = 0; i < (int)magicData->m_shoot.size(); i++ )
				HandleMagicResult(iter->first, i);
		}
	}
	for ( vector<int>::iterator iter = deleteList.begin(); iter != deleteList.end(); iter++ )
		ReleaseMagicData(*iter);	
}

void CACTActionEventsManager::ReleaseACTActionEvent(ACTActionEventsInfo* pActionEvent)
{
	// «ü¼Ð¿ù»~
	if ( pActionEvent == NULL )
		return;

	ACTActionEventsType type = pActionEvent->m_type;
	int magicSerial = pActionEvent->m_magic.m_serial;

	// ²M°£°Ñ¼Æ
	pActionEvent->Clear();	

	// ¦^¦¬«ü¼Ð
	m_usedEvent.erase(pActionEvent);
	m_actionEventInfo.DeleteObj(pActionEvent);

	// ÀË¬d¬O§_¨S¦³ª«¥óÃö³s,¦pªG¬O²M°£Magic Data
	switch ( type )
	{
	case ACT_ACTIONEVENTS_TYPE_MAGIC:
		if ( CheckACTActionEventByID(magicSerial) == false )
		{
			// ªk³NÃþ«¬»Ý­n²M°£¬ÛÃöªºª«¥óµ²ºc
			ReleaseMagicData(magicSerial);
		}
		break;
	}
}

ACTActionEventsInfo* CACTActionEventsManager::GetACTActionEvent()
{
	ACTActionEventsInfo* actionEventInfo = m_actionEventInfo.NewObj();
	if ( actionEventInfo )
	{
		actionEventInfo->Reset();
		m_usedEvent.insert(actionEventInfo);
	}
	return actionEventInfo;
}

bool CACTActionEventsManager::CheckACTActionEventByID(int id)
{
	for ( set<ACTActionEventsInfo*>::iterator iter = m_usedEvent.begin(); iter != m_usedEvent.end(); iter++ )
	{
		if ( (*iter)->m_magic.m_serial == id )
			return true;			
	}
	return false;
}

CNetMagicPackageData* CACTActionEventsManager::CreateMagicData(int magicSerail)
{	
	if ( m_usedMagic.find(magicSerail) != m_usedMagic.end() ) {
		ReleaseMagicData(magicSerail);
	}

	CNetMagicPackageData* magicData = m_magicData.NewObj();
	if ( magicData )
	{
		magicData->Clear();
		magicData->m_id				= magicSerail;
		magicData->m_destructTime	= 1000000.0f;		
		m_usedMagic.insert(make_pair(magicSerail, magicData));
	}
	return magicData;
}

CNetMagicPackageData* CACTActionEventsManager::GetMagicData(int magicSerail)
{
	map<int, CNetMagicPackageData*>::iterator iter = m_usedMagic.find(magicSerail);
	if ( iter != m_usedMagic.end() )
	{
		return iter->second;
	}
	return NULL;
}

void CACTActionEventsManager::ReleaseMagicData(int magicSerail)
{
	map<int, CNetMagicPackageData*>::iterator iter = m_usedMagic.find(magicSerail);
	if ( iter != m_usedMagic.end() )
	{
		m_magicData.DeleteObj(iter->second);
		m_usedMagic.erase(iter);
	}
}

void CACTActionEventsManager::EventCallback(CRuController_EventTrigger *controller, PTRVALUE userData)
{
	//g_pDebugMessage->DebugOutput(0, "EventCallback Event=%d(%x)", controller->GetEventID(), (int)controller);
	//g_pError->AddMessage(0, 0, "EventCallback Event=%d(%x)", controller->GetEventID(), (int)controller);

	ACTActionEventsInfo* actionEventInfo = (ACTActionEventsInfo*)userData;
	if ( actionEventInfo == NULL || m_usedEvent.find(actionEventInfo) == m_usedEvent.end() )
		return;
	
	switch(actionEventInfo->m_type)
	{
	case ACT_ACTIONEVENTS_TYPE_ATTACK:
		{
			CRoleSprite* sourceSprite = g_pGameMain->FindSpriteByID(actionEventInfo->m_source);
			CRoleSprite* targetSprite = g_pGameMain->FindSpriteByID(actionEventInfo->m_target);

			// ¥Ø¼Ð»P¨Ó·½À³¸Ó³£¦s¦b,¨Ã¥B¥Ø¼Ð¤£¯à¦b¦º¤`ª¬ºA
			if ( sourceSprite && targetSprite && targetSprite->GetActorState() != ruFUSION_ACTORSTATE_DEAD )
			{
				SCTInfo sctInfo;
				sctInfo.source = actionEventInfo->m_source;
				sctInfo.target = actionEventInfo->m_target;

				switch (actionEventInfo->m_attack.m_dmgType)
				{
				case EM_ATTACK_DMGTYPE_MISS:			//MISS
					// ¶Ë®`¼Æ­ÈÅã¥Ü
					sctInfo.type = eSCT_Normal_Miss;
					g_pScrollCombatTextFrame->PushSCTInfo(sctInfo);

					// Miss ­µ®Ä
					// ­×§ï¬°¥u¦³¦Û¤v¦³¥¼©R¤¤­µ®Ä
					if ( sourceSprite == g_pGameMain->GetPlayer() )
					{
						char soundPath[256];
						const char* weapon = sourceSprite->GetSoundWeaponType(actionEventInfo->m_attack.m_offHand);
						sprintf(soundPath, "sound\\item\\weapon\\%s\\miss01.mp3", weapon);
						g_pRuGlobalEventManager->Play3DSoundByFile(soundPath, sourceSprite->GetPos(), FALSE);
					}

					/*
					{
						char soundPath[256];
						const char* weapon = targetSprite->GetSoundWeaponType(actionEventInfo->m_attack.m_offHand);
						sprintf(soundPath, "sound\\item\\weapon\\%s\\miss01.mp3", weapon);
						g_pRuGlobalEventManager->Play3DSoundByFile(soundPath, targetSprite->GetPos(), FALSE);
					}
					*/
					break;

				case EM_ATTACK_DMGTYPE_DODGE:		    //DODGE
					// Dodge ­µ®Ä
					// ­×§ï¬°¥u¦³¦Û¤v¦³¥¼©R¤¤­µ®Ä
					if ( sourceSprite == g_pGameMain->GetPlayer() )
					{
						char soundPath[256];
						const char* weapon = sourceSprite->GetSoundWeaponType(actionEventInfo->m_attack.m_offHand);
						sprintf(soundPath, "sound\\item\\weapon\\%s\\dodge%d.mp3", weapon, rand()%3 + 1 );
						g_pRuGlobalEventManager->Play3DSoundByFile(soundPath, sourceSprite->GetPos(), FALSE);
					}

					// ¶Ë®`¼Æ­ÈÅã¥Ü
					sctInfo.type = eSCT_Normal_Dodge;
					g_pScrollCombatTextFrame->PushSCTInfo(sctInfo);

					// ¥Ø¼Ðª«¥ó¹ï¬M°Ê§@
					targetSprite->SetActorState(ruFUSION_ACTORSTATE_DODGE);
					break;				

				case EM_ATTACK_DMGTYPE_HITBACK:			//¤ÏÀ»
					break;

				case EM_ATTACK_DMGTYPE_NO_EFFECT:		//¨S®ÄªG
					sctInfo.type = eSCT_Magic_NoEffect;
					g_pScrollCombatTextFrame->PushSCTInfo(sctInfo);
					break;

				case EM_ATTACK_DMGTYPE_ABSORB:			//§l¦¬
					sctInfo.type = eSCT_Normal_Absorb;
					g_pScrollCombatTextFrame->PushSCTInfo(sctInfo);
					break;

				case EM_ATTACK_DMGTYPE_SHIELD_BLOCK:	//¬ÞÀÉ
					sctInfo.type = eSCT_Normal_Block;
					g_pScrollCombatTextFrame->PushSCTInfo(sctInfo);

					// ¥Ø¼Ðª«¥ó¹ï¬M°Ê§@
					targetSprite->SetActorState(ruFUSION_ACTORSTATE_SHIELD_BLOCK);
					break;

				case EM_ATTACK_DMGTYPE_NORMAL:			//¤@¯ë
				case EM_ATTACK_DMGTYPE_DOUBLE:			//·|¤ß¤@À»
				case EM_ATTACK_DMGTYPE_TRIPLE:			//¥²±þ¤@À»
				case EM_ATTACK_DMGTYPE_HALF:		    //³Q¹jÀÉ
					// ¶Ë®`¼Æ­ÈÅã¥Ü
					sctInfo.hp = actionEventInfo->m_attack.m_damage;
					switch (actionEventInfo->m_attack.m_dmgType)
					{
					case EM_ATTACK_DMGTYPE_HALF:
						sctInfo.type = eSCT_Normal_Parry;
						g_pScrollCombatTextFrame->PushSCTInfo(sctInfo);

						// ¥Ø¼Ðª«¥ó¹ï¬M°Ê§@
						targetSprite->SetActorState(targetSprite->GetParryMotion());
						break;

					case EM_ATTACK_DMGTYPE_NORMAL:
						sctInfo.type = eSCT_Normal_Damage;
						g_pScrollCombatTextFrame->PushSCTInfo(sctInfo);

						// ¥Ø¼Ðª«¥ó¹ï¬M°Ê§@
						targetSprite->SetActorState(ruFUSION_ACTORSTATE_HURT_NORMAL);
						break;

					case EM_ATTACK_DMGTYPE_DOUBLE:
					case EM_ATTACK_DMGTYPE_TRIPLE:
						sctInfo.type = eSCT_Normal_Critical;
						g_pScrollCombatTextFrame->PushSCTInfo(sctInfo);

						// ¥Ø¼Ðª«¥ó¹ï¬M°Ê§@
						targetSprite->SetActorState(ruFUSION_ACTORSTATE_HURT_CRITICAL);
						break;
					}

					// ²£¥Í§ðÀ»¯S®Ä
					if ( g_pGameMain->GetHideOtherPlayerEffect() )
					{
					}
					else
					{
						CEffectSprite* pExplode = CreateEffect("model\\fx\\hit_fx\\act_hit_fx.ros");
						if ( pExplode ) {
							pExplode->GetRuEntity()->SetOption(ruENTITYOPTION_NOMOTIONPROPAGATE, TRUE);
							pExplode->SetEffectMotionStep(eEffectMotion_Step_Explode);

							if ( targetSprite->CheckSameForward(sourceSprite->GetPos() - targetSprite->GetPos()) )
								pExplode->AttachSprite(targetSprite, eAttach_Hit_Point1);
							else
								pExplode->AttachSprite(targetSprite, eAttach_Hit_Point2);
						}
					}

					// §ðÀ»­µ®Ä
					{
						char soundPath[256];
						const char* weapon = sourceSprite->GetSoundWeaponType(actionEventInfo->m_attack.m_offHand);
						const char* armor = targetSprite->GetSoundArmorType();

						switch (actionEventInfo->m_attack.m_dmgType)
						{
						default:
						case EM_ATTACK_DMGTYPE_NORMAL:
							sprintf(soundPath, "sound\\item\\weapon\\%s\\hit_%s_%02d.mp3", weapon, armor, (rand()%3)+1);
							break;

						case EM_ATTACK_DMGTYPE_DOUBLE:
						case EM_ATTACK_DMGTYPE_TRIPLE:
							sprintf(soundPath, "sound\\item\\weapon\\%s\\hit_%s_critical.mp3", weapon, armor);
							break;
						}
						g_pRuGlobalEventManager->Play3DSoundByFile(soundPath, targetSprite->GetPos(), FALSE);
					}

					// ***********************************************************
					// §ó·s¨¤¦â¥Í©R­È
					// ***********************************************************
					// targetSprite->DeductHP( actionEventInfo->m_attack.m_damage );
					break;					
				}			
			}
		}
		break;

	case ACT_ACTIONEVENTS_TYPE_MAGIC:
		{
			int magicSerial = actionEventInfo->m_magic.m_serial;
			int shootNumber = max(actionEventInfo->m_magic.m_shootNum, 0);
			CNetMagicPackageData* magicData = GetMagicData(magicSerial);
			if ( magicData )
			{
				// ¥X¤â¯S®Ä
				CreateShootEffect(magicSerial, shootNumber);

				// ²£¥Í­¸¦æ¯S®Ä©Î©R¤¤Ãz¬µ¯S®Ä
				if ( CreateFlyEffect(magicSerial, shootNumber) == NULL && CreateExplodeEffect(magicSerial, shootNumber) == NULL ) 
				{
					HandleMagicResult(magicSerial, shootNumber);		// ³æÅéÃz¬µ®ÄªG
				}
				magicData->SetActionDone();
				magicData->SetShootActionDone(shootNumber);
			}
		}
		break;
	}

	actionEventInfo->Recyele();
	// ReleaseACTActionEvent(actionEventInfo);
}

bool CACTActionEventsManager::CreateFlyEffect(int magicSerail, int shootNumber)
{
	CNetMagicPackageData* magicData = GetMagicData(magicSerail);
	if ( magicData == NULL )
		return false;

	GameObjDbStructEx* itemDB = g_ObjectData->GetObj(magicData->m_magic);
	if ( itemDB == NULL )
		return false;

	GameObjDbStructEx* imageDB = g_ObjectData->GetObj(itemDB->ImageID);
	if ( imageDB == NULL || imageDB->Image.FlyACT[0] == 0 )
		return false;

	CRoleSprite* ownerSprite = g_pGameMain->FindSpriteByID(magicData->m_owner);
	if ( ownerSprite == NULL )
		return false;

	if ( shootNumber < 0 || shootNumber >= (int)magicData->m_shoot.size() )
		return false;		
	
	bool boolResult = false;	

	// °{¹qÃì¯S®Ä»Ý¨D°Ñ¼Æ
	static float dealyTimeValue = 0.07f;
	static float disappearTime = 0.5f;
	float delayTime = 0.0f;
	int lastPoint = 0;
	int targetPoint = 0;
	CRoleSprite* lastSprite = NULL;

	SpellShootPackage& shoot = magicData->m_shoot[shootNumber];
	for ( vector<ShootFlyEffectStruct>::iterator iter = shoot.m_flyEffect.begin(); iter != shoot.m_flyEffect.end(); iter++ )
	{	
		//g_pError->AddMessage(0, 0, "CreateFlyEffect size %d", shoot.m_flyEffect.size());
		CRoleSprite* targetSprite = g_pGameMain->FindSpriteByID(iter->target);
		bool boShowEffect = g_pGameMain->CheckDisplayEffect(ownerSprite, targetSprite);

		//³]©w¯S®Ä¥²»Ý³QÅã¥Ü
		if ( itemDB->MagicCol.Flag.forceDisplayEffect )
			boShowEffect = true;

		if ( iter->target == magicData->m_owner ||
			 iter->fly != NULL ||
			 (targetSprite == NULL && iter->targetPosition.m_x == 0.0f && iter->targetPosition.m_y == 0.0f && iter->targetPosition.m_z == 0.0f) )
			continue;		

		// °{¹qÃì¯S®Ä
		if ( imageDB->Image.MagicShowType == EM_GameObjectMagicShowType_PolyLine )
		{
			iter->fly = new CEffectSprite;
			if ( boShowEffect && iter->fly->LoadRuEntity(imageDB->Image.FlyACT, false, imageDB->Image.ActRate[EM_ImageActType_Fly] / 100.0f) && targetSprite )
			{
				//g_pError->AddMessage(0, 0, "CreateFlyEffect delayTime %.4f", delayTime);

				// ªì©l¦ì¸mª«¥ó
				if ( lastSprite == NULL )
				{
					lastSprite = ownerSprite;
					lastPoint = IBaseSprite::GetAttachPointEnum(imageDB->Image.FlyBeginPoint);
				}

				targetPoint = IBaseSprite::GetAttachPointEnum(imageDB->Image.TargetPoint);

				iter->fly->SetPauseTime(delayTime);
				iter->fly->SetDestructTime(disappearTime);
				iter->fly->SetEffectMotionStep(eEffectMotion_Step_Appear);
				iter->fly->AttachSprite(lastSprite, lastPoint);
				iter->fly->SetPolyLineTarget(targetSprite, targetPoint);
				iter->fly->SetReleaseCallback(EffectCallback_OnRelease, magicSerail);
				iter->fly->SetUpdateCallback(EffectCallback_OnUpdate, magicSerail);

				iter->fly->m_arg1 = shootNumber;
				iter->fly->m_arg2 = iter->target;
				boolResult = true;
			}
			else
			{
				SAFE_DELETE(iter->fly);
			}

			delayTime += dealyTimeValue;
			lastSprite = targetSprite;
			lastPoint = targetPoint;
		}
		else
		{
			// ¦ê³s¯S®Ä
			if ( imageDB->Image.IsPolyLineEffect && targetSprite )
			{
				bool create = true;

				// ÀË¬d¤§«e¬O§_¦³²£¥Í,¦pªG¦³¤£²£¥Í·sª«¥ó
				if ( shootNumber > 0 )
				{
					SpellShootPackage& preShoot = magicData->m_shoot[shootNumber - 1];
					for ( vector<ShootFlyEffectStruct>::iterator pre_iter = preShoot.m_flyEffect.begin(); pre_iter != preShoot.m_flyEffect.end(); pre_iter++ )
					{
						if ( iter->target == pre_iter->target )
						{
							create = false;
							break;
						}
					}
				}

				if ( create )
				{
					iter->fly = new CEffectSprite;
					if ( boShowEffect && iter->fly->LoadRuEntity(imageDB->Image.FlyACT, false, imageDB->Image.ActRate[EM_ImageActType_Fly] / 100.0f) )
					{
						iter->fly->SetAutoDestruct(false);
						iter->fly->SetEffectMotionStep(eEffectMotion_Step_Appear);
						iter->fly->AttachSprite(ownerSprite, IBaseSprite::GetAttachPointEnum(imageDB->Image.FlyBeginPoint));
						iter->fly->SetPolyLineTarget(targetSprite, IBaseSprite::GetAttachPointEnum(imageDB->Image.TargetPoint));
						iter->fly->SetReleaseCallback(EffectCallback_OnRelease, magicSerail);
					}
					else
					{
						SAFE_DELETE(iter->fly);
					}
				}
			}
			// ­¸¦æ¯S®Ä
			else
			{
				char flyActStr[256];
				if ( imageDB->Image.IsThrowType )
				{
					int index = 0;
					ItemFieldStruct* item = CNetGlobal::RoleData()->GetEqItem(EM_EQWearPos_Ammo);
					if ( item )
					{
						GameObjDbStruct* itemDB = CNetGlobal::GetObj(item->OrgObjID);
						if ( itemDB )
							index = itemDB->Item.ImageEfficacyType;
					}
					sprintf_s(flyActStr, 256, imageDB->Image.FlyACT, index);
				}
				else
					strcpy(flyActStr, imageDB->Image.FlyACT);

				CEffectSprite* fly = NULL;

				if ( boShowEffect )
				{
					if ( imageDB->Image.MagicShowType == EM_GameObjectMagicShowType_Parabola || imageDB->Image.MagicShowType == EM_GameObjectMagicShowType_ParabolaMulti ) {
						fly = new CParabolaEffectSprite;
					} else {
						fly = new CLineEffectSprite;
					}
				}				
				
				if ( fly && fly->LoadRuEntity( flyActStr, false, imageDB->Image.ActRate[EM_ImageActType_Fly] / 100.0f ) )
				{
					// ³]©w°_©lª«¥ó»Pµ²§ôª«¥ó°Ñ¦ÒÂI
					fly->SetBeginSprite(ownerSprite, IBaseSprite::GetAttachPointEnum(imageDB->Image.FlyBeginPoint));

					CRoleSprite* flyTarget = targetSprite;
					if ( imageDB->Image.MagicShowType == EM_GameObjectMagicShowType_Single )
					{
						switch ( itemDB->MagicCol.RangeSelectType )
						{
						case EM_MagicSpellSelectType_Line1:
						case EM_MagicSpellSelectType_Line2:
							flyTarget = g_pGameMain->FindSpriteByID(magicData->m_target);
							if ( flyTarget == NULL )
								flyTarget = targetSprite;
							break;
						}
					}

					if ( flyTarget )
						fly->SetEndSprite(flyTarget, IBaseSprite::GetAttachPointEnum(imageDB->Image.TargetPoint));
					else
						fly->SetTargetPosition(iter->targetPosition);

					// ¥Ñ¯S®Ä¨Æ¥óÂI¨ú±o­¸¦æ©µ¿ð®É¶¡
					CRuACT_ActionEvents *actionEvent = ruNEW CRuACT_ActionEvents();
					fly->SetEffectMotionStep(eEffectMotion_Step_Appear, actionEvent);						
					CRuController_EventTrigger* eventTrigger = actionEvent->GetEventController(ruENTITYEVENT_RANGED_FLIGHT);
					if ( eventTrigger )
						fly->SetStartMoveDelayTime(eventTrigger->GetStartTime());
					ruSAFE_RELEASE(actionEvent);

					fly->SetMoveSpeed(150.0f * (imageDB->Image.ShootMotionScale / 100.0f) );

					// ¥[¤W¦ê±µ¨Æ¥ó
					fly->SetMoveEndCallback(EffectCallback_OnMoveEnd, magicSerail);
					fly->SetReleaseCallback(EffectCallback_OnRelease, magicSerail);
					fly->m_arg1 = shootNumber;

					// ¥u¦³³æ¤@­¸¦æ¯S®Ä
					if ( shoot.m_flyEffect.size() == 1 )
						fly->m_arg2 = -1;
					else
						fly->m_arg2 = iter->target;			

					fly->Update(0.0f);
					boolResult = true;
				}
				else
				{
					SAFE_DELETE(fly);
				}
				
				iter->fly = fly;
			}
		}
	}

	// §R°£¤£¦s¦bªº¦ê³s¯S®Ä(¤W¦¸¦³²£¥Í)
	if ( imageDB->Image.IsPolyLineEffect && shootNumber > 0 )
	{
		SpellShootPackage& preShoot = magicData->m_shoot[shootNumber - 1];
		for ( vector<ShootFlyEffectStruct>::iterator pre_iter = preShoot.m_flyEffect.begin(); pre_iter != preShoot.m_flyEffect.end(); pre_iter++ )
		{
			bool isRemove = true;

			for ( vector<ShootFlyEffectStruct>::iterator iter = shoot.m_flyEffect.begin(); iter != shoot.m_flyEffect.end(); iter++ )
			{
				if ( pre_iter->target == iter->target )
				{
					isRemove = false;
					break;
				}
			}

			if ( isRemove )
				SAFE_DELETE(pre_iter->fly);
		}
	}	

	return boolResult;
	
	/*
	if ( targetSprite->GetActorState() == ruFUSION_ACTORSTATE_DEAD )
		return false;	

	char flyActStr[256];
	if ( itemDB->ImageObj->IsThrowType )
	{
		int index = 0;
		ItemFieldStruct* item = CNetGlobal::RoleData()->GetEqItem(EM_EQWearPos_Ammo);
		if ( item )
		{
			GameObjDbStruct* itemDB = CNetGlobal::GetObj(item->OrgObjID);
			if ( itemDB )
				index = itemDB->Item.ImageEfficacyType;
		}
		sprintf(flyActStr, itemDB->ImageObj->FlyACT, index);
	}
	else
		strcpy(flyActStr, itemDB->ImageObj->FlyACT);

	CLineEffectSprite* pFly = new CLineEffectSprite;
	if ( pFly->LoadRuEntity( flyActStr ) )
	{
		//pFly->SetScale(scale, eSpriteScale_World);
		//pFly->SetPauseTime(actionStartTime);

		// ³]©w°_©lª«¥ó»Pµ²§ôª«¥ó°Ñ¦ÒÂI
		pFly->SetBeginSprite(ownerSprite, IBaseSprite::GetAttachPointEnum(itemDB->ImageObj->FlyBeginPoint));
		pFly->SetEndSprite(targetSprite, IBaseSprite::GetAttachPointEnum(itemDB->ImageObj->TargetPoint));

		// ¥Ñ¯S®Ä¨Æ¥óÂI¨ú±o­¸¦æ©µ¿ð®É¶¡
		CRuACT_ActionEvents *actionEvent = ruNEW CRuACT_ActionEvents();
		pFly->SetEffectMotionStep(eEffectMotion_Step_Appear, actionEvent);						
		CRuController_EventTrigger* eventTrigger = actionEvent->GetEventController(ruENTITYEVENT_RANGED_FLIGHT);
		if ( eventTrigger )
			pFly->SetStartMoveDelayTime(eventTrigger->GetStartTime());
		ruSAFE_RELEASE(actionEvent);

		// »·¶ZÂ÷§Þ¯àÀË¬d,³]©w­¸¦æ³t«×
		//if ( itemDB->MagicCol.Need[0].Type == EM_MagicSpellNeedType_WeaponType_LongDistWeapon ||
		//	itemDB->MagicCol.Need[1].Type == EM_MagicSpellNeedType_WeaponType_LongDistWeapon )
		//{
		//	pFly->SetMoveSpeed(200.0f);
		//}
		//else
		//	pFly->SetMoveSpeed(100.0f);

		pFly->SetMoveSpeed(150.0f);

		// ¥[¤W¦ê±µ¨Æ¥ó
		pFly->SetMoveEndCallback(EffectCallback_OnMoveEnd, magicSerail);
	}
	else
	{
		delete pFly;
		pFly = NULL;
	}
	magicData->m_flyEffect = pFly;
	return (magicData->m_flyEffect != NULL);
	*/
}

bool CACTActionEventsManager::CreateExplodeEffect(int magicSerail, int shootNumber)
{
	CNetMagicPackageData* magicData = GetMagicData(magicSerail);
	if ( magicData == NULL || magicData->GetIdleExplode() )			// ÀË¬d¬O§_¤w¸g²£¥Í´`Àô¯S®Ä
		return false;

	GameObjDbStructEx* itemDB = g_ObjectData->GetObj(magicData->m_magic);
	if ( itemDB == NULL )
		return false;

	GameObjDbStructEx* imageDB = g_ObjectData->GetObj(itemDB->ImageID);
	if ( imageDB == NULL || imageDB->Image.ExplodeACT[0] == 0 )
		return false;

	CRoleSprite* ownerSprite = g_pGameMain->FindSpriteByID(magicData->m_owner);
	CRoleSprite* targetSprite = g_pGameMain->FindSpriteByID(magicData->m_target);

	if ( itemDB->MagicCol.TargetType == EM_Releation_Plot )
	{
		if ( shootNumber < 0 || shootNumber >= (int)magicData->m_shoot.size() )
			return false;

		SpellShootPackage& shoot = magicData->m_shoot[shootNumber];
		if ( shoot.m_list.empty() )
			return false;

		targetSprite = g_pGameMain->FindSpriteByID(shoot.m_list[0].target);
	}

	/*
	if ( ownerSprite == NULL || (targetSprite == NULL && itemDB->MagicCol.TargetType != EM_Releation_Locatoin) )
		return false;
	*/

	if ( ownerSprite == NULL )
		return false;
	
	// ¯S®Ä¥²»Ý³QÅã¥Ü, ©ÎªÌ¥i¥H¥²ÁôÂÃ¤£Åã¥Ü
	if ( !itemDB->MagicCol.Flag.forceDisplayEffect && !g_pGameMain->CheckDisplayEffect(ownerSprite) )
		return false;

	// ¨Ã«D¹ï¦a,¨Ã¥B¥Ø¼Ð¤w¸g¦º¤`
	if ( itemDB->MagicCol.TargetType != EM_Releation_Locatoin ) {
		if ( targetSprite == NULL || targetSprite->GetDeadState() /*|| targetSprite->GetActorState() == ruFUSION_ACTORSTATE_DEAD*/ )
			return false;
	}

	CEffectSprite* pExplode = CreateEffect(imageDB->Image.ExplodeACT, imageDB->Image.ActRate[EM_ImageActType_Explode] / 100.0f);
	if ( pExplode ) {
		switch (itemDB->MagicCol.TargetType)
		{
		case EM_Releation_Locatoin:
			pExplode->SetPos(CRuVector3(magicData->m_x, magicData->m_y, magicData->m_z));
			break;

		default:
			pExplode->AttachSprite(targetSprite, IBaseSprite::GetAttachPointEnum(imageDB->Image.TargetPoint));
			break;
		}
		
		if ( pExplode->HasMotion("explode") ) {
			pExplode->SetEffectMotionStep(eEffectMotion_Step_Explode);
		} else {
			pExplode->SetEffectMotionStep(eEffectMotion_Step_Appear);
			pExplode->SetReleaseCallback(EffectCallback_OnRelease, magicSerail);
			magicData->m_idleExplode = pExplode;
		}
	}	
	return (pExplode != NULL);
}

void CACTActionEventsManager::CreateShootEffect(int magicSerail, int shootNumber)
{
	CNetMagicPackageData* magicData = GetMagicData(magicSerail);
	if ( magicData == NULL )
		return;

	GameObjDbStructEx* itemDB = g_ObjectData->GetObj(magicData->m_magic);
	if ( itemDB == NULL )
		return ;

	GameObjDbStructEx* imageDB = g_ObjectData->GetObj(itemDB->ImageID);
	if ( imageDB == NULL || imageDB->Image.ShootACT[0] == 0 )
		return;

	CRoleSprite* ownerSprite = g_pGameMain->FindSpriteByID(magicData->m_owner);
	CRoleSprite* targetSprite = g_pGameMain->FindSpriteByID(magicData->m_target);
	if ( ownerSprite == NULL )
		return;

	// ¨Ã«D¹ï¦a,¨Ã¥B¥Ø¼Ð¤w¸g¦º¤`
	if ( itemDB->MagicCol.TargetType != EM_Releation_Locatoin ) {
		if ( targetSprite == NULL || targetSprite->GetActorState() == ruFUSION_ACTORSTATE_DEAD )
			return;
	}

	// ¯S®Ä¥²»Ý³QÅã¥Ü, ©ÎªÌ¥i¥H¥²ÁôÂÃ¤£Åã¥Ü
	if ( !itemDB->MagicCol.Flag.forceDisplayEffect && !g_pGameMain->CheckDisplayEffect(ownerSprite) )
		return;

	CEffectSprite* effect = CreateEffect(imageDB->Image.ShootACT, imageDB->Image.ActRate[EM_ImageActType_Shoot] / 100.0f);
	if ( effect ) {
		// ³]©w­±¦V
		switch(itemDB->MagicCol.TargetType)
		{
		case EM_Releation_Locatoin:				// ¹ï¦a
			{
				CRuVector3 targetPos(magicData->m_x, magicData->m_y, magicData->m_z);
				effect->SetDirection(targetPos - ownerSprite->GetPos());
			}
			break;

		default:
			// ¨ä¥LÃþ«¬,¦³¥Ø¼Ðª««h´Â¦V¥Ø¼Ð
			// ¼Q®gª«¥ó·|¦]¬°³]©w¤è¦V¦Ó¿ù»~
			/*
			if ( targetSprite && targetSprite != ownerSprite ) {
				effect->SetDirection(targetSprite->GetPos() - ownerSprite->GetPos());
			}
			*/
			break;
		}
		effect->AttachSprite(ownerSprite, IBaseSprite::GetAttachPointEnum(imageDB->Image.ShootPoint));
		effect->SetEffectMotionStep(eEffectMotion_Step_Explode);
		effect->Update(0.0f);
	}
}

void CACTActionEventsManager::HandleMagicResult(int magicSerail, int shootNumber)
{
	CNetMagicPackageData* magicData = GetMagicData(magicSerail);
	if ( magicData == NULL || magicData->m_shoot.empty() ) {
		return;
	}

	if ( shootNumber < 0 || shootNumber >= (int)magicData->m_shoot.size() )
		return;

	SpellShootPackage& shoot = magicData->m_shoot[shootNumber];
	vector<SingleMagicPackage>::iterator iter = shoot.m_list.begin();
	while ( iter != shoot.m_list.end() )
	{
		HandleMagicAttackByID(magicSerail, shootNumber, iter->target);
		iter = shoot.m_list.begin();
	}
}

void CACTActionEventsManager::HandleMagicAttackByID(int magicSerail, int shootNumber, int targetId)
{
	CNetMagicPackageData* magicData = GetMagicData(magicSerail);
	if ( magicData == NULL )
		return;
	
	if ( shootNumber < 0 || shootNumber >= (int)magicData->m_shoot.size() )
		return;

	SpellShootPackage& shoot = magicData->m_shoot[shootNumber];
	vector<SingleMagicPackage>::iterator iter = shoot.m_list.begin();
	for ( ; iter != shoot.m_list.end(); iter++ )
	{
		if ( iter->target == targetId )
			break;
	}
	if ( iter == shoot.m_list.end() )
		return;

	HandleMagicExtend(magicData, &(*iter));
	shoot.m_list.erase(iter);
}

void CACTActionEventsManager::HandleMagicExtend(CNetMagicPackageData* magicData, SingleMagicPackage* single)
{
	if ( magicData == NULL || single == NULL )
		return;

	CRoleSprite* targetSprite = g_pGameMain->FindSpriteByID(single->target);
	if ( targetSprite /*&& targetSprite->GetActorState() != ruFUSION_ACTORSTATE_DEAD*/ )
	{	
		GameObjDbStructEx* itemDB = g_ObjectData->GetObj(magicData->m_magic);

		// ÁôÂÃ¬Y¨Ç¯S®Ä
		bool boShow = g_pGameMain->CheckDisplayEffect(magicData->m_owner, single->target);

		if ( itemDB && itemDB->MagicCol.Flag.forceDisplayEffect )
			boShow = true;

		// §ðÀ»®ÄªG
		if ( single->attack.magicBase != 0 )
		{
			GameObjDbStruct* magicBase = g_ObjectData->GetObj(single->attack.magicBase);
			if ( magicBase != NULL )
			{
				GameObjDbStruct* imageDB = g_ObjectData->GetObj(magicBase->ImageID);

				// ¯S®Äªí²{
				switch (single->attack.type)
				{
				case EM_MagicAtkResult_Normal:		// À»¤¤
				case EM_MagicAtkResult_Critial:		// ÃzÀ»
				case EM_MagicAtkResult_NoEffect:	// µL®Ä(§K¬Ì)
				case EM_MagicAtkResult_Absorb:		// §l¦¬
					if ( imageDB != NULL )
					{
						if ( single->attack.type == EM_MagicAtkResult_NoEffect && magicBase->MagicBase.Setting.NoEffect )
						{
						}
						else
						{
							CEffectSprite* effect;							

							if ( boShow && (effect = CreateEffect(imageDB->Image.ExtendACT, imageDB->Image.ActRate[EM_ImageActType_Extend] / 100.0f)) != NULL )
							{
								effect->AttachSprite(targetSprite, IBaseSprite::GetAttachPointEnum(imageDB->Image.ExtenPoint));
								if ( effect->HasMotion("explode") ) {
									effect->SetEffectMotionStep(eEffectMotion_Step_Explode);
								} else {
									effect->SetEffectMotionStep(eEffectMotion_Step_Appear);			// IDLEª«¥ó¦Û°Ê2¬í«á®ø¥¢
									effect->SetIdleEffectLiveTime(2.0f);
								}

								// effect->SetVisible(boShow);
							}							
						}
					}
					break;
				}
			}

			if ( magicBase->MagicBase.Setting.DisableShowMagicInfo == false )
			{
				// ¼Æ­ÈÅã¥Ü
				SCTInfo sctInfo;
				sctInfo.source = magicData->m_owner;
				sctInfo.target = single->target;
				switch (single->attack.type)
				{
				case EM_MagicAtkResult_Normal:
				case EM_MagicAtkResult_Critial:
				case EM_MagicAtkResult_Parry:
					switch(single->attack.valueType)
					{
					case EM_MagicAttackType_HP:
						sctInfo.hp = single->attack.value;
						if ( sctInfo.hp < 0 )
							targetSprite->SetActorState(ruFUSION_ACTORSTATE_HURT_NORMAL);		// ¨ü¶Ë°Ê§@
						break;

					case EM_MagicAttackType_MP:
						sctInfo.mp = single->attack.valueDeduct;
						break;

					case EM_MagicAttackType_SP_Warrior:
					case EM_MagicAttackType_SP_Ranger:
					case EM_MagicAttackType_SP_Rogue:
						sctInfo.sp = single->attack.valueDeduct;
						break;
					}

					switch ( single->attack.type )
					{
					case EM_MagicAtkResult_Normal:
						sctInfo.type = eSCT_Magic_Damage;
						break;

					case EM_MagicAtkResult_Critial:
						sctInfo.type = eSCT_Magic_Critical;
						break;

					case EM_MagicAtkResult_Parry:
						sctInfo.type = eSCT_Normal_Parry;
						break;
					}
					g_pScrollCombatTextFrame->PushSCTInfo(sctInfo);
					break;

				case EM_MagicAtkResult_Miss:
					sctInfo.type = eSCT_Magic_Miss;
					g_pScrollCombatTextFrame->PushSCTInfo(sctInfo);
					break;

				case EM_MagicAtkResult_NoEffect:
					if ( !magicBase->MagicBase.Setting.IgnoreBuffMessage )
					{
						sctInfo.type = eSCT_Magic_NoEffect;
						g_pScrollCombatTextFrame->PushSCTInfo(sctInfo);
					}
					break;

				case EM_MagicAtkResult_Absorb:
					sctInfo.type = eSCT_Magic_Absorb;
					g_pScrollCombatTextFrame->PushSCTInfo(sctInfo);
					break;

				case EM_MagicAtkResult_Dodge:
					switch ( magicBase->MagicBase.EffectType )
					{
					case EM_Magic_Magic:
						sctInfo.type = eSCT_Magic_Resist;
						break;

					case EM_Magic_Phy:
						sctInfo.type = eSCT_Magic_Dodge;
						break;
					}					
					g_pScrollCombatTextFrame->PushSCTInfo(sctInfo);
					break;
				}
			}			
		}

		if ( single->assist.magicBase != 0 )
		{
			GameObjDbStruct* magicBase = g_ObjectData->GetObj(single->assist.magicBase);
			if ( magicBase != NULL )
			{
				GameObjDbStruct* imageDB = g_ObjectData->GetObj(magicBase->ImageID);

				// ¯S®Äªí²{
				switch (single->assist.type)
				{
				case EM_MagicAtkResult_Normal:		// À»¤¤
				case EM_MagicAtkResult_Critial:		// ÃzÀ»
				case EM_MagicAtkResult_NoEffect:	// µL®Ä(§K¬Ì)
				case EM_MagicAtkResult_Absorb:		// §l¦¬
					if ( imageDB != NULL && imageDB->Image.ExtendACT[0] != 0 )
					{
						if ( single->assist.type == EM_MagicAtkResult_NoEffect && magicBase->MagicBase.Setting.NoEffect )
						{
						}
						else
						{
							CEffectSprite* pExtend = new CEffectSprite;
							if ( boShow && pExtend->LoadRuEntity(imageDB->Image.ExtendACT, false, imageDB->Image.ActRate[EM_ImageActType_Extend] / 100.0f) )
							{
								//pExtend->SetPauseTime(delayTime);
								//pExtend->SetScale(scale, eSpriteScale_World);
								pExtend->AttachSprite(targetSprite, IBaseSprite::GetAttachPointEnum(imageDB->Image.ExtenPoint));
								pExtend->SetEffectMotionStep(eEffectMotion_Step_Explode);

								// 11/22 ·s¼W
								// pExtend->SetVisible(boShow);
							}
							else
								delete pExtend;
						}
					}
					break;
				}
			}			

			if ( magicBase->MagicBase.Setting.DisableShowMagicInfo == false )
			{
				// ¼Æ­ÈÅã¥Ü
				SCTInfo sctInfo;
				sctInfo.source = magicData->m_owner;
				sctInfo.target = single->target;
				switch (single->assist.type)
				{
				case EM_MagicAtkResult_Normal:
				case EM_MagicAtkResult_Critial:
					// ¾A»Ý­n¥[¤Jªk³N¦WºÙ
					break;

				case EM_MagicAtkResult_Miss:
					sctInfo.type = eSCT_Magic_Miss;
					g_pScrollCombatTextFrame->PushSCTInfo(sctInfo);
					break;

				case EM_MagicAtkResult_NoEffect:
					if ( !magicBase->MagicBase.Setting.IgnoreBuffMessage )
					{
						sctInfo.type = eSCT_Magic_NoEffect;
						g_pScrollCombatTextFrame->PushSCTInfo(sctInfo);
					}
					break;

				case EM_MagicAtkResult_Absorb:
					sctInfo.type = eSCT_Magic_Absorb;
					g_pScrollCombatTextFrame->PushSCTInfo(sctInfo);
					break;

				case EM_MagicAtkResult_Dodge:
					switch ( magicBase->MagicBase.EffectType )
					{
					case EM_Magic_Magic:
						sctInfo.type = eSCT_Magic_Resist;
						break;

					case EM_Magic_Phy:
						sctInfo.type = eSCT_Magic_Dodge;
						break;
					}					
					g_pScrollCombatTextFrame->PushSCTInfo(sctInfo);
					break;
				}
			}
		}
	}
}

void EffectCallback_OnRelease(CEffectSprite* efftect, int userdata)
{
	if ( g_pACTActionEventsManager == NULL )
		return;

	CNetMagicPackageData* magicData = g_pACTActionEventsManager->GetMagicData(userdata);
	if ( magicData == NULL )
		return;

	magicData->ReleaseEffect(efftect);
}

void EffectCallback_OnMoveEnd(CEffectSprite* efftect, int userdata)
{
	if ( g_pACTActionEventsManager == NULL || efftect == NULL )
		return;

	int shootNumber = efftect->m_arg1;
	int targetId = efftect->m_arg2;

	// ²£¥Í©R¤¤¯S®Ä
	g_pACTActionEventsManager->CreateExplodeEffect(userdata, shootNumber);

	// ²£¥Í³æÅéÃz¬µ,¸ò¾Ú¦h¤H»P­Ó¤HÃz¬µ­Ó¼Æ¤£¦P
	if ( targetId == -1 )
	{
		g_pACTActionEventsManager->HandleMagicResult(userdata, shootNumber);
	}
	else
	{
		g_pACTActionEventsManager->HandleMagicAttackByID(userdata, shootNumber, targetId);
	}
}

void EffectCallback_OnUpdate(CEffectSprite* efftect, int userdata)
{
	if ( g_pACTActionEventsManager == NULL || efftect == NULL )
		return;	

	efftect->SetUpdateCallback(NULL, 0);

	int shootNumber = efftect->m_arg1;
	int targetId = efftect->m_arg2;

	//g_pError->AddMessage(0, 0, "EffectCallback_OnUpdate %d shootNumber %d targetId %d", userdata, shootNumber, targetId);

	// ²£¥Í©R¤¤¯S®Ä
	g_pACTActionEventsManager->CreateExplodeEffect(userdata, shootNumber);

	// ²£¥Í³æÅéÃz¬µ,¸ò¾Ú¦h¤H»P­Ó¤HÃz¬µ­Ó¼Æ¤£¦P
	if ( targetId == -1 )
	{
		g_pACTActionEventsManager->HandleMagicResult(userdata, shootNumber);
	}
	else
	{
		g_pACTActionEventsManager->HandleMagicAttackByID(userdata, shootNumber, targetId);
	}
}

CEffectSprite* CACTActionEventsManager::CreateEffect(const char* filename, float scale)
{
	CEffectSprite* effectSprite = new CEffectSprite();
	if ( effectSprite ) {
		if ( !effectSprite->LoadRuEntity(filename, false, scale) ) {
			delete effectSprite;
			effectSprite = NULL;
		} else {
			effectSprite->SetDestructTime(30.0f);						// 30¬í«á¦Û°Ê®ø¥¢,Á×§KµL­­¦s¦b
		}
	}
	return effectSprite;
}