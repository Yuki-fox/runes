#include <windows.h>
#include "eventmanager.h"
#include "sprite.h"
#include "../ErrorList/errorlist.h"
CRuGlobalEventManager* g_pRuGlobalEventManager = NULL;
//-----------------------------------------------------------------------------
CRuGlobalEventManager::CRuGlobalEventManager()
{
	m_lastUpdateTime = 0;
	m_controllerFrame = ruNEW CRuEntity_Container_Impl();
	m_audioLibrary = NULL;
	m_camera = NULL;
	m_soundEvents = ruNEW CRuHashMap<PTRVALUE, SoundEventDescriptor*>(256);
	m_masterSoundLevel = 1.000f;
	m_ambientSoundLevel = 0.500f;
	m_musicSoundLevel = 0.400f;
	m_soundFXSoundLevel = 1.000f;
	m_uiSoundLevel = 1.000f;
	m_musicFrequency = 1.0f;
	m_enableSoundFX = TRUE;
	m_enableMusic = TRUE;
	m_enableEnvironmentMusic = TRUE;
	m_zoneSettings = NULL;
	m_musicTrigger = NULL;
	m_ambienceTrigger = NULL;
	m_supplementAmbienceTrigger = NULL;
	m_randomAmbienceTrigger = NULL;
	m_3DAmbienceTriggerHash = ruNEW CRuHashMap<CRuGUID, CRuWorld_Trigger*>(256);
	m_active3DAmbienceSounds = ruNEW CRuHashMap<CRuGUID, PTRVALUE>(128);
	m_musicSoundHandle = 0;
	m_ambientSoundHandle = 0;
	m_supplementAmbientSoundHandle = 0;
	m_rerollMusic = TRUE;
	m_activeMusicName = "";
	m_activeAmbienceName = "";
	m_activeSupplementAmbienceName = "";
	// Assign self as the global event manager
	g_pRuGlobalEventManager = this;
	// Apply settings
	GetSettingsFromEngine();
	// Register playback finished listener on Symphony
	//g_ruSymphony->Event_PlaybackFinished().RegisterHandler(m_GUID, boost::bind(&CRuGlobalEventManager::PlaybackFinishedHandler, this, _1));
	g_ruSoundFMOD->Event_PlaybackFinished().RegisterHandler(m_GUID, boost::bind(&CRuGlobalEventManager::PlaybackFinishedHandler, this, _1));
	// Register listener on engine settings
	g_ruEngineSettings->RegisterSettingsChangeHandler(m_GUID, boost::bind(&CRuGlobalEventManager::EngineSettingsChangedHandler, this, _1, _2));
}
//-----------------------------------------------------------------------------
CRuGlobalEventManager::~CRuGlobalEventManager()
{
	ClearSoundEventQueue();
	delete m_soundEvents;
	ruSAFE_RELEASE(m_controllerFrame);
	ruSAFE_RELEASE(m_audioLibrary);
	ruSAFE_RELEASE(m_camera);
	ruSAFE_RELEASE(m_zoneSettings);
	ruSAFE_RELEASE(m_musicTrigger);
	ruSAFE_RELEASE(m_ambienceTrigger);
	ruSAFE_RELEASE(m_supplementAmbienceTrigger);
	ruSAFE_RELEASE(m_randomAmbienceTrigger);
	delete m_3DAmbienceTriggerHash;
	// TODO: also gotta stop all ambience sounds here
	delete m_active3DAmbienceSounds;
	g_pRuGlobalEventManager = NULL;
}
//-----------------------------------------------------------------------------
void CRuGlobalEventManager::SetAudioLibrary(CRuFusion_AudioLibrary* audioLibrary)
{
	if (audioLibrary)
	{
		ruSAFE_RELEASE(m_audioLibrary);
		m_audioLibrary = audioLibrary;
		m_audioLibrary->AddRef();
	}
}
//-----------------------------------------------------------------------------
void CRuGlobalEventManager::SetCamera(CRuCamera* camera)
{
	if (camera)
	{
		camera->AddRef();
		ruSAFE_RELEASE(m_camera);
		m_camera = camera;
	}
}
//-----------------------------------------------------------------------------
void CRuGlobalEventManager::AddEntity(CRuEntity* pRuEntity)
{
	m_controllerFrame->AddEntity(pRuEntity);
}
//-----------------------------------------------------------------------------
void CRuGlobalEventManager::RemoveEntity(CRuEntity* pRuEntity)
{
	/*
	for ( int i = 0; i < m_controllerFrame->GetNumEntities(); i++ )
	{
		CRuEntity_Controller* pRuControllerEntity = (CRuEntity_Controller*)m_controllerFrame->GetEntity(i);
		if ( pRuControllerEntity )
		{
			// pRuControllerEntity->GetTarget == pRuEntity
			if ( pRuControllerEntity->GetTarget() == pRuEntity )
			{
				pRuControllerEntity->SelfDestruct();
				i -= 1;
			}
		}
	}
	*/
}
//-----------------------------------------------------------------------------
void CRuGlobalEventManager::Update(REAL elapsedTime)
{
	// Chain update call to the controller container
	m_controllerFrame->Update(elapsedTime);
	// Process ambient sound
	const char* ambientSound = "";
	if (m_ambienceTrigger)
	{
		const char* propertyNames[4] = { "CRuWorld_Trigger:Ambience Dawn", "CRuWorld_Trigger:Ambience Day", "CRuWorld_Trigger:Ambience Dusk", "CRuWorld_Trigger:Ambience Night" };
		const char* propertyName = propertyNames[m_timeGroup];
		RuPropertyType propertyValue;
		ambientSound = m_ambienceTrigger->GetProperty(propertyName, propertyValue) ? boost::get<const char*>(propertyValue) : "";
	}
	else if (m_zoneSettings)
	{
		ambientSound = m_zoneSettings->GetAmbientSound();
	}
	// If the ambience sound is different from the one currently playing
	if (m_ambientSoundHandle == 0 || stricmp(m_activeAmbienceName, ambientSound) != 0)
	{
		// Stop currently playing ambient sound
		if (m_ambientSoundHandle)
		{
			/*
			char stopStr[2048];
			sprintf(stopStr, "stopping amb handle %d\n", m_ambientSoundHandle);
			OutputDebugString(stopStr);
*/
			//g_ruSymphony->FadeOutSound(m_ambientSoundHandle, 2000.0f);
			g_ruAudible->Stop(m_ambientSoundHandle, 2.0f);
			m_ambientSoundHandle = NULL;
		}
		// Look for ambient sound in the audio library - if not found, try to use the search string as a path
		const char* audioResource = m_audioLibrary->GetAssociatedAudio("ambience", ambientSound);
		audioResource = audioResource ? audioResource : ambientSound;
		// Play the ambient sound
		//m_ambientSoundHandle = g_ruSymphony->PlaySoundByPath(audioResource, TRUE, m_ambientSoundLevel, ruSOUNDTYPE_AMBIENCE);
		int loopCount = (TRUE) ? 0x7FFFFFFF : 1;
		m_ambientSoundHandle = g_ruAudible->Play(audioResource, (CRuAudible::AudibleType)ruSOUNDTYPE_AMBIENCE, (loopCount > 1) ? TRUE : FALSE);			
		// Store current ambient sound
		m_activeAmbienceName = ambientSound;
		/*
		char outputStr[2048];
		sprintf(outputStr, "amb snd %s %d\n", m_activeAmbienceName, m_ambientSoundHandle);
		OutputDebugString(outputStr);
*/
	}
	// Process supplement ambient sound
	const char* supplementAmbientSound = "";
	if (m_supplementAmbienceTrigger)
	{
		const char* propertyNames[4] = { "CRuWorld_Trigger:Supplement Ambience Dawn", "CRuWorld_Trigger:Supplement Ambience Day", "CRuWorld_Trigger:Supplement Ambience Dusk", "CRuWorld_Trigger:Supplement Ambience Night" };
		const char* propertyName = propertyNames[m_timeGroup];
		RuPropertyType propertyValue;
		supplementAmbientSound = m_supplementAmbienceTrigger->GetProperty(propertyName, propertyValue) ? boost::get<const char*>(propertyValue) : "";
	}
	else if (m_zoneSettings)
	{
		RuPropertyType propertyValue;
		supplementAmbientSound = m_zoneSettings->GetProperty("CRuWorld_ZoneSettings:Looping Ambience", propertyValue) ? boost::get<const char*>(propertyValue) : "";
	}
	// If the supplement ambience sound is different from the one currently playing
	if (m_supplementAmbientSoundHandle == 0 || stricmp(m_activeSupplementAmbienceName, supplementAmbientSound) != 0)
	{
		// Stop currently playing supplement ambient sound
		if (m_supplementAmbientSoundHandle)
		{
			//g_ruSymphony->FadeOutSound(m_supplementAmbientSoundHandle, 2000.0f);
			g_ruAudible->Stop(m_supplementAmbientSoundHandle, 2.0f);
			m_supplementAmbientSoundHandle = NULL;
		}
		const char* audioResource = m_audioLibrary->GetAssociatedAudio("ambience", supplementAmbientSound);
		audioResource = audioResource ? audioResource : supplementAmbientSound;
		// Play the supplement ambient sound
		//m_supplementAmbientSoundHandle = g_ruSymphony->PlaySoundByPath(audioResource, TRUE, m_ambientSoundLevel, ruSOUNDTYPE_AMBIENCE);
		int loopCount = (TRUE) ? 0x7FFFFFFF : 1;
		m_supplementAmbientSoundHandle = g_ruAudible->Play(audioResource, (CRuAudible::AudibleType)ruSOUNDTYPE_AMBIENCE, (loopCount > 1) ? TRUE : FALSE);		
		// Store current supplement ambient sound
		m_activeSupplementAmbienceName = supplementAmbientSound;
	}
	// Process random ambient sounds
	m_timeToNextRandomAmbienceRoll -= elapsedTime;
	if (m_timeToNextRandomAmbienceRoll < 0.0f)
	{
		RuPropertyType propertyValue;
		const char* randomAmbience = "";
		REAL occurrenceRate = 0.0f;
		if (m_randomAmbienceTrigger)
		{
			const char* propertyNames[4] = { "CRuWorld_Trigger:Random Ambience Dawn", "CRuWorld_Trigger:Random Ambience Day", "CRuWorld_Trigger:Random Ambience Dusk", "CRuWorld_Trigger:Random Ambience Night" };
			const char* propertyName = propertyNames[m_timeGroup];
			// Get the occurrence rate
			REAL occurrenceRate = m_randomAmbienceTrigger->GetProperty("CRuWorld_Trigger:Random Ambience Occurrence Rate", propertyValue) ? boost::get<REAL>(propertyValue) : 0.01f;
			// Get the associated sound
			randomAmbience = m_randomAmbienceTrigger->GetProperty(propertyName, propertyValue) ? boost::get<const char*>(propertyValue) : "";
		}
		else if (m_zoneSettings)
		{
			// Get the occurrence rate
			REAL occurrenceRate = m_zoneSettings->GetProperty("CRuWorld_ZoneSettings:Random Ambience Occurrence Rate", propertyValue) ? boost::get<REAL>(propertyValue) : 0.01f;
			// Get the associated sound
			randomAmbience = m_zoneSettings->GetProperty("CRuWorld_ZoneSettings:Random Ambience", propertyValue) ? boost::get<const char*>(propertyValue) : "";
		}
		if (RuGlobalMTRand().GenerateFloat() < occurrenceRate)
		{
			// Look for the sound in the audio library
			const char* audioResource = m_audioLibrary->GetAssociatedAudio("random ambience", randomAmbience);
			if (audioResource)
			{
				//g_ruSymphony->PlaySoundByPath(audioResource, FALSE, m_ambientSoundLevel, ruSOUNDTYPE_AMBIENCE);
				int loopCount = (FALSE) ? 0x7FFFFFFF : 1;
				g_ruAudible->Play(audioResource, (CRuAudible::AudibleType)ruSOUNDTYPE_AMBIENCE, (loopCount > 1) ? TRUE : FALSE);					
			}
		}
		// Reset time to next roll
		m_timeToNextRandomAmbienceRoll = 1.0f;
	}
	// Process music sounds
	m_timeToNextMusicRoll -= elapsedTime;
	if (m_timeToNextMusicRoll < 0.0f && m_enableEnvironmentMusic)
	{
		const char* music = "";
		REAL musicOccurrenceRate = 0.0f;
		if (m_musicTrigger)
		{
			const char* musicPropertyNames[4] = { "CRuWorld_Trigger:Dawn Music", "CRuWorld_Trigger:Day Music", "CRuWorld_Trigger:Dusk Music", "CRuWorld_Trigger:Night Music" };
			const char* propertyName = musicPropertyNames[m_timeGroup];
			RuPropertyType propertyValue;
			music = m_musicTrigger->GetProperty(propertyName, propertyValue) ? boost::get<const char*>(propertyValue) : "";
			musicOccurrenceRate = m_musicTrigger->GetProperty("CRuWorld_Trigger:Music Occurrence Rate", propertyValue) ? boost::get<REAL>(propertyValue) : 0.01f;
		}
		else if (m_zoneSettings)
		{
			music = m_zoneSettings->GetThemeMusic();
			musicOccurrenceRate = m_zoneSettings->GetThemeMusicOccurrenceRate();
		}
		//if (strlen(music) > 0)
		//{
		//	char outputStr[256];
		//	sprintf(outputStr, "m_timeToNextMusicRoll:%f music:%s\n", m_timeToNextMusicRoll, music);
		//	OutputDebugString(outputStr);
		//}
		PlayMusicByFile(music, musicOccurrenceRate);
		// Reset time to next roll
		m_timeToNextMusicRoll = 1.0f;
	}
	// Play Sound
	list<PlaySoundDelay>::iterator iter = m_playSoundDelay.begin();
	while (iter != m_playSoundDelay.end())
	{
		PlaySoundDelay& data = *iter;
		if (data.m_delayTime < 0.0f)
		{
			list<PlaySoundDelay>::iterator removeIter = iter;
			if (data.m_is3D)
			{
				//g_ruSymphony->Play3DSound(data.m_soundPath.c_str(), ruSOUNDTYPE_SFX, m_soundFXSoundLevel, data.m_loop, data.m_pos, FALSE);
				int loopCount = (data.m_loop) ? 0x7FFFFFFF : 1;
				g_ruAudible->Play(data.m_soundPath.c_str(), (CRuAudible::AudibleType)ruSOUNDTYPE_SFX, (loopCount > 1) ? TRUE : FALSE, &data.m_pos);	
			}
			else
			{
				//g_ruSymphony->PlaySound(data.m_soundPath.c_str(), ruSOUNDTYPE_SFX, m_soundFXSoundLevel, data.m_loop);
				int loopCount = (data.m_loop) ? 0x7FFFFFFF : 1;
				g_ruAudible->Play(data.m_soundPath.c_str(), (CRuAudible::AudibleType)ruSOUNDTYPE_SFX, (loopCount > 1) ? TRUE : FALSE);
			}
			iter++;
			m_playSoundDelay.erase(removeIter);
		}
		else
		{
			data.m_delayTime -= elapsedTime;
			iter++;
		}
	}
	// Update 3D ambience triggers
	Update3DAmbienceTriggers(elapsedTime);
	// Symphony update cycle
	//g_ruSymphony->Update(elapsedTime);

	DWORD t = timeGetTime();
	REAL elapsedTimeR = (t - m_lastUpdateTime) * 0.001f;
	m_lastUpdateTime = t;

	g_ruSoundFMOD->Update(NULL);
	g_ruAudible->Update(elapsedTimeR);
}
//-----------------------------------------------------------------------------
void CRuGlobalEventManager::RegisterEventHandlers(CRuEntity* entity, PTRVALUE userData)
{
	if (entity)
	{
		entity->Event_Dispose().RegisterHandler(CRuGUID(), boost::bind(&CRuGlobalEventManager::HandleEvent_Dispose, this, _1), userData);
		entity->Event_Trigger().RegisterHandler(CRuGUID(), boost::bind(&CRuGlobalEventManager::HandleEvent_Trigger, this, _1), userData);
	}
}
BOOL CRuGlobalEventManager::HandleEvent_Dispose(RuEventArgs* eventArgs)
{
	RuObjectEvent_Dispose_Args* disposeArgs = static_cast<RuObjectEvent_Dispose_Args*>(eventArgs);
	IBaseSprite* pSprite = reinterpret_cast<CRoleSprite*>(disposeArgs->m_userData);
	StopSpriteSounds(pSprite);
	return TRUE;
}
BOOL CRuGlobalEventManager::HandleEvent_Trigger(RuEventArgs* eventArgs)
{
	RuEntityEvent_Trigger_Args* triggerArgs = static_cast<RuEntityEvent_Trigger_Args*>(eventArgs);
	IBaseSprite* pSprite = reinterpret_cast<CRoleSprite*>(triggerArgs->m_userData);
	if (pSprite->IsVisible() == false)
		return FALSE;
	switch (triggerArgs->m_triggerID)
	{
	case ruENTITYEVENT_HIT:
		pSprite->MotionEvent(triggerArgs->m_triggerID);
		return TRUE;
	case ruENTITYEVENT_FOOTSTEP:
	{
		/*
				// Build a particle system or some such
				CRuController_ParticleSystem *psTest = ruNEW CRuController_ParticleSystem();
				psTest->SetParticleSystem(g_ruResourceManager->LoadEntity("model\\fx\\fx_fire.ros"));
				psTest->SetStartTime(0.1f);
				psTest->SetDuration(eventDuration);
				psTest->SetLoopTime(0.0f);
				psTest->SetTarget(entity);
				psTest->Update(0.0f);
				psTest->SetSelfDestructTimer(eventDuration + 2.0f);
				// Add controller to the container frame
				m_controllerFrame->AddEntity(psTest);
*/
// Play footstep sound
		const CRuVector3& spritePosition = pSprite->GetPos();
		//PTRVALUE soundHandle = g_ruSymphony->Play3DSound("sound\\common\\footstep\\footstep_generic.mp3", ruSOUNDTYPE_SFX, m_soundFXSoundLevel, FALSE, spritePosition, TRUE);
		int loopCount = (FALSE) ? 0x7FFFFFFF : 1;
		PTRVALUE soundHandle = g_ruAudible->Play("sound\\common\\footstep\\footstep_generic.mp3", (CRuAudible::AudibleType)ruSOUNDTYPE_SFX, (loopCount > 1) ? TRUE : FALSE, &spritePosition);						
	}
	return FALSE;
	case ruENTITYEVENT_GHOSTTRAIL:
	{
		CRuController_GhostTrail* gtController = ruNEW CRuController_GhostTrail();
		gtController->SetFrameType(ruFRAMETYPE_WORLD);
		//				gtController->SetInterval(0.001f);
		gtController->SetMaxAge(triggerArgs->m_duration);
		gtController->SetFadeCoefficients(0.0f, 0.0f, 1.0f);
		gtController->SetMaxSegments(64);
		gtController->SetTrailAnchors("effect01", "effect02");
		gtController->SetTrailTextureName("texture\\fx\\weapon_trail\\blade_flash");
		gtController->SetDistortFXEnable(TRUE);
		gtController->SetDistortTextureName("texture\\common\\caustic_2");
		gtController->SetDistortFrameCount(1);
		gtController->SetTarget(triggerArgs->m_entity);
		gtController->SetActiveDuration(triggerArgs->m_duration);
		gtController->SetFadeOutDuration(0.5f);
		gtController->SetStartTime(0.0f);
		gtController->SetDuration(triggerArgs->m_duration + 0.5f);
		gtController->SetLoopTime(0.0f);
		gtController->SetSelfDestructTimer(triggerArgs->m_duration + 2.0f);
		// Add controller to the container frame
		m_controllerFrame->AddEntity(gtController);
	}
	return FALSE;
	case ruENTITYEVENT_HOLSTER:
		pSprite->MotionEvent(triggerArgs->m_triggerID);
		return FALSE;
	case ruENTITYEVENT_UNHOLSTER:
		pSprite->MotionEvent(triggerArgs->m_triggerID);
		return FALSE;
	case ruENTITYEVENT_AUDIO_DIRECT:
	{
		// If the sprite is an effect object and its sound state is off, ignore the sound event
		if (pSprite->IsVisible() == false || (pSprite->GetObjectType() == eSpriteObject_Effect && static_cast<CEffectSprite*>(pSprite)->GetSoundState() == false))
			return TRUE;
		BOOL loop = (BOOL)triggerArgs->m_parameter0;
		//PTRVALUE soundHandle = g_ruSymphony->PlaySound(triggerArgs->m_triggerString, ruSOUNDTYPE_SFX, m_soundFXSoundLevel, loop);
		int loopCount = (loop) ? 0x7FFFFFFF : 1;
		PTRVALUE soundHandle = g_ruAudible->Play(triggerArgs->m_triggerString, (CRuAudible::AudibleType)ruSOUNDTYPE_SFX, (loopCount > 1) ? TRUE : FALSE);			
		// If the sample is looping, store the sound handle somewhere
		if (loop)
		{
			PushSoundEvent(pSprite, soundHandle);
		}
	}
	return FALSE;
	case ruENTITYEVENT_AUDIO_TAGGED:
	{
		// If the sprite is an effect object and its sound state is off, ignore the sound event
		if (pSprite->IsVisible() == false || (pSprite->GetObjectType() == eSpriteObject_Effect && static_cast<CEffectSprite*>(pSprite)->GetSoundState() == false))
			return TRUE;
		// Get classification of the ACT entity
		CRuACTEntity* actEntity = RuEntity_FindParentACTEntity(triggerArgs->m_entity);
		const char* classification = actEntity ? actEntity->GetTemplate()->GetClassification() : "hm_female";
		BOOL loop = (BOOL)triggerArgs->m_parameter0;
		const char* audioResource = m_audioLibrary->GetAssociatedAudio(classification, triggerArgs->m_triggerString);
		if (audioResource)
		{
			//g_ruSymphony->PlaySoundByPath(audioResource, loop, m_soundFXSoundLevel, ruSOUNDTYPE_SFX);
			int loopCount = (loop) ? 0x7FFFFFFF : 1;
			PTRVALUE handleValue = g_ruAudible->Play(audioResource, (CRuAudible::AudibleType)ruSOUNDTYPE_SFX, (loopCount > 1) ? TRUE : FALSE);
		}
	}
	return FALSE;
	case ruENTITYEVENT_AUDIO_3D_DIRECT:
	{
		// If the sprite is an effect object and its sound state is off, ignore the sound event
		if (pSprite->IsVisible() == false || (pSprite->GetObjectType() == eSpriteObject_Effect && static_cast<CEffectSprite*>(pSprite)->GetSoundState() == false))
			return TRUE;
		const CRuVector3& spritePosition = triggerArgs->m_entity->GetTranslation(ruFRAMETYPE_WORLD);
		BOOL loop = (BOOL)triggerArgs->m_parameter0;
		//PTRVALUE soundHandle = g_ruSymphony->Play3DSound(triggerArgs->m_triggerString, ruSOUNDTYPE_SFX, m_soundFXSoundLevel, loop != ruAUDIOLOOPMODE_NONE, spritePosition, FALSE);
		int loopCount = (loop) ? 0x7FFFFFFF : 1;
		PTRVALUE soundHandle = g_ruAudible->Play(triggerArgs->m_triggerString, (CRuAudible::AudibleType)ruSOUNDTYPE_SFX, (loopCount > 1) ? TRUE : FALSE, & spritePosition);
		// If the sample is looping, store the sound handle somewhere
		if (loop && soundHandle)
		{
			PushSoundEvent(pSprite, soundHandle);
		}
	}
	return FALSE;
	case ruENTITYEVENT_AUDIO_3D_TAGGED:
	{
		// If the sprite is an effect object and its sound state is off, ignore the sound event
		if (pSprite->IsVisible() == false || (pSprite->GetObjectType() == eSpriteObject_Effect && static_cast<CEffectSprite*>(pSprite)->GetSoundState() == false))
			return TRUE;
		// Get classification of the ACT entity
		CRuACTEntity* actEntity = RuEntity_FindParentACTEntity(triggerArgs->m_entity);
		const char* classification = actEntity ? actEntity->GetTemplate()->GetClassification() : "hm_female";
		BOOL loop = (BOOL)triggerArgs->m_parameter0;
		INT32 playProbability = triggerArgs->m_parameter1 > 0 ? (INT32)triggerArgs->m_parameter1 : 100;
		if (playProbability >= RuGlobalMTRand().GenerateFloat() * 100.0f)
		{
			const char* audioResource = m_audioLibrary->GetAssociatedAudio(classification, triggerArgs->m_triggerString);
			if (audioResource)
			{
				const CRuVector3& spritePosition = triggerArgs->m_entity->GetTranslation(ruFRAMETYPE_WORLD);
				//PTRVALUE soundHandle = g_ruSymphony->Play3DSound(audioResource, ruSOUNDTYPE_SFX, m_soundFXSoundLevel, loop != ruAUDIOLOOPMODE_NONE, spritePosition, FALSE);
				int loopCount = (loop) ? 0x7FFFFFFF : 1;
				PTRVALUE soundHandle = g_ruAudible->Play(audioResource, (CRuAudible::AudibleType)ruSOUNDTYPE_SFX, (loopCount > 1) ? TRUE : FALSE, & spritePosition);

				// If the sample is looping, store the sound handle somewhere
				if (loop && soundHandle)
				{
					PushSoundEvent(pSprite, soundHandle);
				}
			}
		}
	}
	return FALSE;
	case ruENTITYEVENT_AUDIO_TERMINATE:
	{
		// Terminate all audio associated with the sprite
		StopSpriteSounds(pSprite);
	}
	return FALSE;
	case ruENTITYEVENT_RANGED_LOAD:
	{
		//g_pError->AddMessage(0, 0, "ruENTITYEVENT_RANGED_LOAD");
		CRoleSprite* sprite = dynamic_cast<CRoleSprite*>(pSprite);
		if (sprite)
			sprite->SetRangedMotion("bow_draw");
	}
	return FALSE;
	case ruENTITYEVENT_RANGED_RELEASE:
	{
		//g_pError->AddMessage(0, 0, "ruENTITYEVENT_RANGED_RELEASE");
		CRoleSprite* sprite = dynamic_cast<CRoleSprite*>(pSprite);
		if (sprite)
			sprite->SetRangedMotion("bow_shoot");
	}
	return FALSE;
	case ruENTITYEVENT_CAMERA_JIGGLE:
	{
		IBaseSprite* sprite = dynamic_cast<IBaseSprite*>(pSprite);
		if (m_camera && sprite)
		{
			CRuVector3 diffVec = sprite->GetPos() - m_camera->GetTargetPosition();
			REAL dist = diffVec.Magnitude();
			REAL factor = RuFalloff_Linear(dist, 240.0f);
			m_camera->Jiggle(25.0f * factor, 300.0f, 10.0f);
		}
	}
	return FALSE;
	}
	return TRUE;
}
void CRuGlobalEventManager::StopSpriteSounds(void* sprite)
{
	PTRVALUE spriteKey = reinterpret_cast<PTRVALUE>(sprite);
	DWORD spriteKeyHash = RuGlobalHashGen().GetHashCode(spriteKey);
	SoundEventDescriptor* soundEventDesc = NULL;
	/*
	const char *name = "";
	if(static_cast<IBaseSprite *>(sprite)->GetRuEntity())
	{
		name = static_cast<IBaseSprite *>(sprite)->GetRuEntity()->GetResourceName();
	}
	char moo[500];
	sprintf(moo, "Stopping sounds for %s...", name);
	OutputDebugString(moo);
*/
	if (m_soundEvents->ContainsKey(spriteKey, spriteKeyHash))
	{
		soundEventDesc = m_soundEvents->Get(spriteKey, spriteKeyHash);
		/*
		sprintf(moo, "%d sounds.\n", soundEventDesc->m_soundHandles.Count());
		OutputDebugString(moo);
*/
// Stop all active sound handles
		for (INT32 i = 0; i < soundEventDesc->m_soundHandles.Count(); ++i)
		{
			// Start volume ramp to zero
			//BOOL fadeResult = g_ruSymphony->FadeOutSound(soundEventDesc->m_soundHandles[i]);
			BOOL fadeResult = g_ruAudible->Stop(soundEventDesc->m_soundHandles[i], 0.250f);	
			if (!fadeResult)
			{
				OutputDebugString("Failed.\n");
			}
		}
		// Remove sprite's sound event descriptor from the hash map and destroy it
		m_soundEvents->Remove(spriteKey, spriteKeyHash);
		delete soundEventDesc;
	}
	else
	{
		//		OutputDebugString("none.\n");
	}
}
void CRuGlobalEventManager::SetTimeGroup(INT32 timeGroup)
{
	m_timeGroup = timeGroup % 4;
}
void CRuGlobalEventManager::SetZoneSettings(CRuWorld_ZoneSettings* zoneSettings)
{
	CRuWorld_ZoneSettings* oldZoneSettings = m_zoneSettings;
	if (zoneSettings)
	{
		zoneSettings->AddRef();
	}
	m_zoneSettings = zoneSettings;
	ruSAFE_RELEASE(oldZoneSettings);
}
void CRuGlobalEventManager::SetMusicTrigger(CRuWorld_Trigger* musicTrigger)
{
	if (m_musicTrigger != musicTrigger)
	{
		CRuWorld_Trigger* oldTrigger = m_musicTrigger;
		if (musicTrigger)
		{
			m_musicTrigger = musicTrigger;
			m_musicTrigger->AddRef();
			RuPropertyType propertyValue;
			m_rerollMusic = m_musicTrigger->GetProperty("CRuWorld_Trigger:Always Trigger Music", propertyValue) ? boost::get<BOOL>(propertyValue) : FALSE;
		}
		else
		{
			m_musicTrigger = NULL;
		}
		ruSAFE_RELEASE(oldTrigger);
	}
}
void CRuGlobalEventManager::SetAmbienceTrigger(CRuWorld_Trigger* ambienceTrigger)
{
	if (m_ambienceTrigger != ambienceTrigger)
	{
		CRuWorld_Trigger* oldTrigger = m_ambienceTrigger;
		if (ambienceTrigger)
		{
			m_ambienceTrigger = ambienceTrigger;
			m_ambienceTrigger->AddRef();
		}
		else
		{
			m_ambienceTrigger = NULL;
		}
		ruSAFE_RELEASE(oldTrigger);
	}
}
void CRuGlobalEventManager::SetSupplementAmbienceTrigger(CRuWorld_Trigger* supplementAmbienceTrigger)
{
	if (m_supplementAmbienceTrigger != supplementAmbienceTrigger)
	{
		CRuWorld_Trigger* oldTrigger = m_supplementAmbienceTrigger;
		if (supplementAmbienceTrigger)
		{
			m_supplementAmbienceTrigger = supplementAmbienceTrigger;
			m_supplementAmbienceTrigger->AddRef();
		}
		else
		{
			m_supplementAmbienceTrigger = NULL;
		}
		ruSAFE_RELEASE(oldTrigger);
	}
}
void CRuGlobalEventManager::SetRandomAmbienceTrigger(CRuWorld_Trigger* randomAmbienceTrigger)
{
	if (m_randomAmbienceTrigger != randomAmbienceTrigger)
	{
		CRuWorld_Trigger* oldTrigger = m_randomAmbienceTrigger;
		if (randomAmbienceTrigger)
		{
			m_randomAmbienceTrigger = randomAmbienceTrigger;
			m_randomAmbienceTrigger->AddRef();
		}
		else
		{
			m_randomAmbienceTrigger = NULL;
		}
		ruSAFE_RELEASE(oldTrigger);
	}
}
void CRuGlobalEventManager::AddRef3DAmbienceTriggers()
{
	// Increment local reference on all triggers
	for (INT32 i = 0; i < m_3DAmbienceTriggers.Count(); ++i)
	{
		m_3DAmbienceTriggers[i]->AddRef();
	}
}
void CRuGlobalEventManager::Clear3DAmbienceTriggers()
{
	// Release local reference to all triggers
	for (INT32 i = 0; i < m_3DAmbienceTriggers.Count(); ++i)
	{
		ruSAFE_RELEASE(m_3DAmbienceTriggers[i]);
	}
	// Clear triggers array
	m_3DAmbienceTriggers.Clear();
}
CRuArrayList<CRuWorld_Trigger*>& CRuGlobalEventManager::Get3DAmbienceTriggers()
{
	return m_3DAmbienceTriggers;
}
void CRuGlobalEventManager::Update3DAmbienceTriggers(REAL elapsedTime)
{
	//CRuVector3 listenerPos = g_ruSymphony->GetListenerPosition();
	CRuVector3 listenerPos = g_ruAudible->GetListenerPosition();
	// Pre-process the trigger list for any grouped triggers
	for (INT32 i = 0; i < m_3DAmbienceTriggers.Count(); ++i)
	{
		CRuWorld_Trigger* trigger = m_3DAmbienceTriggers[i];
		RuPropertyType propertyValue;
		const char* sound = trigger->GetProperty("CRuWorld_Trigger:3D Ambience Sound", propertyValue) ? boost::get<const char*>(propertyValue) : "";
		BOOL groupedSound = trigger->GetProperty("CRuWorld_Trigger:3D Ambience Grouped", propertyValue) ? boost::get<BOOL>(propertyValue) : FALSE;
		if (groupedSound)
		{
			CRuOBB triggerBounds = trigger->GetTriggerBounds();
			REAL triggerDistance = triggerBounds.GetDistance(listenerPos);
			if (m_3DAmbienceGroupTriggers.ContainsKey(sound) == FALSE)
			{
				// Duplicate the trigger
				CRuWorld_Trigger* masterTrigger = ruNEW CRuWorld_Trigger();
				masterTrigger->SetTriggerBounds(triggerBounds);
				masterTrigger->SetTriggerType(trigger->GetTriggerType());
				masterTrigger->SetZoneID(trigger->GetZoneID());
				masterTrigger->SetZoneType(trigger->GetZoneType());
				masterTrigger->SetProperty("CRuWorld_Trigger:3D Ambience Sound", sound);
				masterTrigger->SetProperty("CRuWorld_Trigger:3D Ambience Grouped", groupedSound);
				// Insert into the group trigger tree
				m_3DAmbienceGroupTriggers.Add(sound, masterTrigger);
			}
			else
			{
				// If this trigger is closer than the current master trigger, use this trigger's bounds
				CRuWorld_Trigger* masterTrigger = m_3DAmbienceGroupTriggers.Get(sound);
				CRuOBB masterTriggerBounds = masterTrigger->GetTriggerBounds();
				REAL masterTriggerDistance = masterTriggerBounds.GetDistance(listenerPos);
				if (triggerDistance < masterTriggerDistance)
				{
					masterTrigger->SetTriggerBounds(triggerBounds);
				}
			}
			// Remove trigger from the array
			m_3DAmbienceTriggers.RemoveAt(i);
			--i;
		}
	}
	// Insert 3D ambience group triggers into list
	Insert3DAmbienceGroupTriggers();
	// Update the trigger hash
	m_3DAmbienceTriggerHash->Clear();
	for (INT32 i = 0; i < m_3DAmbienceTriggers.Count(); ++i)
	{
		CRuWorld_Trigger* trigger = m_3DAmbienceTriggers[i];
		m_3DAmbienceTriggerHash->Add(trigger->GetGUID(), trigger->GetGUID().HashCode(), trigger);
	}
	// Start all sounds which should be active
	StartActive3DAmbienceTriggers();
	// Update all sound positions
	UpdateActive3DAmbienceTriggers();
	// Stop all sounds which no longer should exist
	StopInactive3DAmbienceTriggers();
}
void CRuGlobalEventManager::StartActive3DAmbienceTriggers()
{
	for (INT32 i = 0; i < m_3DAmbienceTriggers.Count(); ++i)
	{
		CRuWorld_Trigger* trigger = m_3DAmbienceTriggers[i];
		// If the trigger is already active, skip it
		if (m_active3DAmbienceSounds->ContainsKey(trigger->GetGUID(), trigger->GetGUID().HashCode()))
		{
			continue;
		}
		// Calculate distance to trigger
		CRuOBB triggerBounds = trigger->GetTriggerBounds();
		//REAL distance = triggerBounds.GetDistance(g_ruSymphony->GetListenerPosition());
		REAL distance = triggerBounds.GetDistance(g_ruAudible->GetListenerPosition());
		//if (distance > g_ruSymphony->GetDefault3DMaxDist())
		if (distance > g_ruAudible->GetDefault3DMaxDist())
		{
			continue;
		}
		// Activate trigger
		RuPropertyType propertyValue;
		const char* sound = trigger->GetProperty("CRuWorld_Trigger:3D Ambience Sound", propertyValue) ? boost::get<const char*>(propertyValue) : "";
		BOOL groupedSound = trigger->GetProperty("CRuWorld_Trigger:3D Ambience Grouped", propertyValue) ? boost::get<BOOL>(propertyValue) : FALSE;
		// Look for sound in the audio library - if not found, try to use the search string as a path
		const char* audioResource = m_audioLibrary->GetAssociatedAudio("ambience", sound);
		audioResource = audioResource ? audioResource : sound;
		// Play the sound
		//CRuVector3 nearestPt = triggerBounds.GetClosestPoint(g_ruSymphony->GetListenerPosition());
		CRuVector3 nearestPt = triggerBounds.GetClosestPoint(g_ruAudible->GetListenerPosition());
		//PTRVALUE soundHandle = g_ruSymphony->Play3DSound(audioResource, ruSOUNDTYPE_SFX, m_soundFXSoundLevel, TRUE, nearestPt, FALSE);
		int loopCount = (TRUE) ? 0x7FFFFFFF : 1;
		PTRVALUE soundHandle = g_ruAudible->Play(audioResource, (CRuAudible::AudibleType)ruSOUNDTYPE_SFX, (loopCount > 1) ? TRUE : FALSE, &nearestPt);
		// Store sound handle into active ambient sound list
		m_active3DAmbienceSounds->Add(trigger->GetGUID(), trigger->GetGUID().HashCode(), soundHandle);
	}
}
void CRuGlobalEventManager::UpdateActive3DAmbienceTriggers()
{
	for (INT32 i = 0; i < m_3DAmbienceTriggers.Count(); ++i)
	{
		CRuWorld_Trigger* trigger = m_3DAmbienceTriggers[i];
		// If the trigger is active, update its sound position
		if (m_active3DAmbienceSounds->ContainsKey(trigger->GetGUID(), trigger->GetGUID().HashCode()))
		{
			CRuOBB triggerBounds = trigger->GetTriggerBounds();
			//CRuVector3 nearestPt = triggerBounds.GetClosestPoint(g_ruSymphony->GetListenerPosition());
			CRuVector3 nearestPt = triggerBounds.GetClosestPoint(g_ruAudible->GetListenerPosition());
			RuPropertyType propertyValue;
			const char* sound = trigger->GetProperty("CRuWorld_Trigger:3D Ambience Sound", propertyValue) ? boost::get<const char*>(propertyValue) : "";
			BOOL groupedSound = trigger->GetProperty("CRuWorld_Trigger:3D Ambience Grouped", propertyValue) ? boost::get<BOOL>(propertyValue) : FALSE;
			PTRVALUE soundHandle = m_active3DAmbienceSounds->Get(trigger->GetGUID(), trigger->GetGUID().HashCode());
			//g_ruSymphony->Set3DSoundPosition(soundHandle, nearestPt);
			g_ruAudible->UpdatePosition(soundHandle, &nearestPt);
		}
	}
}
void CRuGlobalEventManager::Insert3DAmbienceGroupTriggers()
{
	m_3DAmbienceGroupTriggers.ForAllElements(boost::bind(&CRuGlobalEventManager::Insert3DAmbienceGroupTriggersCallback, this, _1, _2));
}
BOOL CRuGlobalEventManager::Insert3DAmbienceGroupTriggersCallback(const char* key, void* data)
{
	CRuWorld_Trigger* trigger = *reinterpret_cast<CRuWorld_Trigger**>(data);
	trigger->AddRef();
	m_3DAmbienceTriggers.Add(trigger);
	return TRUE;
}
void CRuGlobalEventManager::StopInactive3DAmbienceTriggers()
{
	// Clear deleted trigger list
	m_deleted3DAmbienceTriggers.Clear();
	// Iterate through all triggers
	m_active3DAmbienceSounds->ForAllElements(boost::bind(&CRuGlobalEventManager::StopInactive3DAmbienceTriggersCallback, this, _1, _2));
	// Remove all deleted triggers
	for (INT32 i = 0; i < m_deleted3DAmbienceTriggers.Count(); ++i)
	{
		m_active3DAmbienceSounds->Remove(m_deleted3DAmbienceTriggers[i], m_deleted3DAmbienceTriggers[i].HashCode());
	}
}
BOOL CRuGlobalEventManager::StopInactive3DAmbienceTriggersCallback(const void* key, void* data)
{
	// Cast key & data into respective values
	const CRuGUID& guid = *reinterpret_cast<const CRuGUID*>(key);
	PTRVALUE soundHandle = *reinterpret_cast<PTRVALUE*>(data);
	BOOL soundInactive = FALSE;
	const char* sound = "";
	// Is the trigger GUID still in the hash?
	soundInactive = (m_3DAmbienceTriggerHash->ContainsKey(guid, guid.HashCode()) == FALSE);
	// Is the trigger farther than the maximum distance?
	if (!soundInactive)
	{
		CRuWorld_Trigger* trigger = m_3DAmbienceTriggerHash->Get(guid, guid.HashCode());
		CRuOBB triggerBounds = trigger->GetTriggerBounds();
		//REAL distance = triggerBounds.GetDistance(g_ruSymphony->GetListenerPosition());
		REAL distance = triggerBounds.GetDistance(g_ruAudible->GetListenerPosition());
		RuPropertyType propertyValue;
		sound = trigger->GetProperty("CRuWorld_Trigger:3D Ambience Sound", propertyValue) ? boost::get<const char*>(propertyValue) : "";
		//if (distance > g_ruSymphony->GetDefault3DMaxDist())
		if (distance > g_ruAudible->GetDefault3DMaxDist())	
		{
			soundInactive = TRUE;
		}
	}
	if (soundInactive)
	{
		// Fade out sound and add to list of stopping sound handles
		//g_ruSymphony->FadeOutSound(soundHandle, 250.0f);
		g_ruAudible->Stop(soundHandle, 0.250f);
		// Add to list of triggers to be removed
		m_deleted3DAmbienceTriggers.Add(guid);
		// If trigger is a group trigger, remove it from the group trigger tree
		if (m_3DAmbienceGroupTriggers.ContainsKey(sound))
		{
			CRuWorld_Trigger* trigger = m_3DAmbienceGroupTriggers.Get(sound);
			m_3DAmbienceGroupTriggers.Remove(sound);
			ruSAFE_RELEASE(trigger);
		}
	}
	return TRUE;
}
void CRuGlobalEventManager::PlaySoundByFile(const char* soundPath, BOOL loop, REAL delay, INT32 soundType, INT32 soundIndex)
{
	if (delay > 0.0f)
	{
		PlaySoundDelay data;
		data.m_soundPath = soundPath;
		data.m_loop = loop ? ruAUDIOLOOPMODE_LOOP_250MS : ruAUDIOLOOPMODE_NONE;
		data.m_is3D = FALSE;
		data.m_delayTime = delay;
		m_playSoundDelay.push_back(data);
	}
	else
	{
		REAL soundLevel = 1.0f;
		switch (soundType)
		{
		default:
		case ruSOUNDTYPE_SFX:
			soundLevel = m_soundFXSoundLevel;
			break;
		case ruSOUNDTYPE_AMBIENCE:
			soundLevel = m_ambientSoundLevel;
			break;
		case ruSOUNDTYPE_MUSIC:
			soundLevel = m_musicSoundLevel;
			break;
		case ruSOUNDTYPE_INTERFACE:
			soundLevel = m_uiSoundLevel;
			break;
		}
		//PTRVALUE handleValue = g_ruSymphony->PlaySoundByPath(soundPath, loop ? ruAUDIOLOOPMODE_LOOP_250MS : ruAUDIOLOOPMODE_NONE, soundLevel, soundType);
		int loopCount = (loop) ? 0x7FFFFFFF : 1;
		PTRVALUE handleValue = g_ruAudible->Play(soundPath, (CRuAudible::AudibleType)soundType, (loopCount > 1) ? TRUE : FALSE);
		if (handleValue && soundIndex)
		{
			m_serverPlaySound[soundIndex] = handleValue;
		}
	}
}
void CRuGlobalEventManager::StopSoundByIndex(INT32 soundIndex)
{
	map<INT32, PTRVALUE>::iterator iter = m_serverPlaySound.find(soundIndex);
	if (iter != m_serverPlaySound.end())
	{
		//g_ruSymphony->FadeOutSound(iter->second);
		g_ruAudible->Stop(iter->second, 0.250f);
		m_serverPlaySound.erase(iter);
	}
}
void CRuGlobalEventManager::ClearAllSoundIdex()
{
	for (map<INT32, PTRVALUE>::iterator iter = m_serverPlaySound.begin(); iter != m_serverPlaySound.end(); iter++)
	{
		//g_ruSymphony->FadeOutSound(iter->second);
		g_ruAudible->Stop(iter->second, 0.250f);
	}
	m_serverPlaySound.clear();
}
void CRuGlobalEventManager::Play3DSoundByFile(const char* soundPath, CRuVector3 pos, BOOL loop, REAL delay, INT32 soundType, INT32 soundIndex)
{
	if (delay > 0.0f)
	{
		PlaySoundDelay data;
		data.m_soundPath = soundPath;
		data.m_pos = pos;
		data.m_loop = loop ? ruAUDIOLOOPMODE_LOOP_250MS : ruAUDIOLOOPMODE_NONE;
		data.m_is3D = TRUE;
		data.m_delayTime = delay;
		m_playSoundDelay.push_back(data);
	}
	else
	{
		REAL soundLevel = 1.0f;
		switch (soundType)
		{
		default:
		case ruSOUNDTYPE_SFX:
			soundLevel = m_soundFXSoundLevel;
			break;
		case ruSOUNDTYPE_AMBIENCE:
			soundLevel = m_ambientSoundLevel;
			break;
		case ruSOUNDTYPE_MUSIC:
			soundLevel = m_musicSoundLevel;
			break;
		case ruSOUNDTYPE_INTERFACE:
			soundLevel = m_uiSoundLevel;
			break;
		}
		// PTRVALUE handleValue = g_ruSymphony->Play3DSound(soundPath, soundType, soundLevel, loop, pos, FALSE);
		int loopCount = (loop) ? 0x7FFFFFFF : 1;
		PTRVALUE handleValue = g_ruAudible->Play(soundPath, (CRuAudible::AudibleType)soundType, (loopCount > 1) ? TRUE : FALSE, & pos);
		if (handleValue && soundIndex)
		{
			m_serverPlaySound[soundIndex] = handleValue;
		}
	}
}
void CRuGlobalEventManager::PlayMusicByFile(const char* musicPath, REAL musicOccurrenceRate, BOOL loop)
{
	// Stop currently playing music if it is different from what is specified
	if (m_musicSoundHandle && strlen(musicPath) > 0 && stricmp(musicPath, m_activeMusicName) != 0)
	{
		StopMusic();
	}
	if (m_musicSoundHandle == 0)
	{
		if (m_rerollMusic || RuGlobalMTRand().GenerateFloat() < (musicOccurrenceRate * m_musicFrequency))
		{
			// Look for music in the audio library - if not found, try to use the search string as a path
			const char* audioResource = m_audioLibrary->GetAssociatedAudio("music", musicPath);
			audioResource = audioResource ? audioResource : musicPath;
			// Play the music
			//m_musicSoundHandle = g_ruSymphony->PlaySoundByPath(audioResource, loop, m_musicSoundLevel, ruSOUNDTYPE_MUSIC);
			int loopCount = (loop) ? 0x7FFFFFFF : 1;
			m_musicSoundHandle = g_ruAudible->Play(audioResource, (CRuAudible::AudibleType)ruSOUNDTYPE_MUSIC, (loopCount > 1) ? TRUE : FALSE);
			// Save the music name
			m_activeMusicName = musicPath;
			// Clear reroll music
			m_rerollMusic = FALSE;
		}
	}
	else
	{
	}
}
void CRuGlobalEventManager::StopMusic()
{
	//g_ruSymphony->FadeOutSound(m_musicSoundHandle, 2000.0f)
	g_ruAudible->Stop(m_musicSoundHandle, 2.0f);
	m_musicSoundHandle = NULL;
}
void CRuGlobalEventManager::SetEnvironmentMusicEnable(BOOL enableEnvironmentMusic)
{
	m_enableEnvironmentMusic = enableEnvironmentMusic;
}
PTRVALUE CRuGlobalEventManager::GetMusicSoundHandle()
{
	return m_musicSoundHandle;
}
void CRuGlobalEventManager::PushSoundEvent(void* sprite, PTRVALUE soundHandle)
{
	PTRVALUE spriteKey = reinterpret_cast<PTRVALUE>(sprite);
	DWORD spriteKeyHash = RuGlobalHashGen().GetHashCode(spriteKey);
	SoundEventDescriptor* soundEventDesc = NULL;
	if (m_soundEvents->ContainsKey(spriteKey, spriteKeyHash))
	{
		soundEventDesc = m_soundEvents->Get(spriteKey, spriteKeyHash);
	}
	else
	{
		soundEventDesc = ruNEW SoundEventDescriptor();
		m_soundEvents->Add(spriteKey, spriteKeyHash, soundEventDesc);
	}
	// Append new sound handle to the sprite's list of active sound handles
	soundEventDesc->m_soundHandles.Add(soundHandle);
}
void CRuGlobalEventManager::ClearSoundEventQueue()
{
	// Iterate through each trigger and destroy
	m_soundEvents->ForAllElements(boost::bind(&CRuGlobalEventManager::ClearSoundEventQueueCallback, this, _1, _2));
	// Clear all triggers
	m_soundEvents->Clear();
}
BOOL CRuGlobalEventManager::ClearSoundEventQueueCallback(const void* key, void* data)
{
	SoundEventDescriptor* soundEvent = *((SoundEventDescriptor**)data);
	delete soundEvent;
	return TRUE;
}
BOOL CRuGlobalEventManager::PlaybackFinishedHandler(RuEventArgs* eventArgs)
{
	RuSymphonyEvent_PlaybackFinished_Args* finArgs = static_cast<RuSymphonyEvent_PlaybackFinished_Args*>(eventArgs);
	// Is the finished handle the music handle?
	// if (finArgs->m_soundHandle == m_musicSoundHandle)
	if (finArgs->m_soundHandle == (PTRVALUE)g_ruAudible->GetSoundChannel(m_musicSoundHandle))
	{
		m_musicSoundHandle = 0;
	}
	return TRUE;
}
BOOL CRuGlobalEventManager::EngineSettingsChangedHandler(const char* settingName, const RuPropertyType& setting)
{
	GetSettingsFromEngine();
	return TRUE;
}
void CRuGlobalEventManager::GetSettingsFromEngine()
{
	RuPropertyType propertyValue;
	if (g_ruEngineSettings->GetSetting("audioFX:sound fx enable", propertyValue))
	{
		m_enableSoundFX = boost::get<INT32>(propertyValue);
	}
	else
	{
		m_enableSoundFX = TRUE;
	}
	if (g_ruEngineSettings->GetSetting("audioFX:music enable", propertyValue))
	{
		m_enableMusic = boost::get<INT32>(propertyValue);
	}
	else
	{
		m_enableMusic = TRUE;
	}
	if (g_ruEngineSettings->GetSetting("audioFX:master volume", propertyValue))
	{
		m_masterSoundLevel = boost::get<REAL>(propertyValue);
		//g_ruSymphony->SetMasterSoundVolume(m_masterSoundLevel);
	}
	if (g_ruEngineSettings->GetSetting("audioFX:ambience volume", propertyValue))
	{
		m_ambientSoundLevel = boost::get<REAL>(propertyValue);
		//g_ruSymphony->AdjustSoundVolumeByType(ruSOUNDTYPE_AMBIENCE, m_ambientSoundLevel);
	}
	if (g_ruEngineSettings->GetSetting("audioFX:music volume", propertyValue))
	{
		m_musicSoundLevel = boost::get<REAL>(propertyValue);
		//g_ruSymphony->AdjustSoundVolumeByType(ruSOUNDTYPE_MUSIC, m_musicSoundLevel);
	}
	if (g_ruEngineSettings->GetSetting("audioFX:sound fx volume", propertyValue))
	{
		m_soundFXSoundLevel = boost::get<REAL>(propertyValue);
		//g_ruSymphony->AdjustSoundVolumeByType(ruSOUNDTYPE_SFX, m_soundFXSoundLevel);
	}
	if (g_ruEngineSettings->GetSetting("audioFX:interface sfx volume", propertyValue))
	{
		m_uiSoundLevel = boost::get<REAL>(propertyValue);
		//g_ruSymphony->AdjustSoundVolumeByType(ruSOUNDTYPE_INTERFACE, m_uiSoundLevel);
	}
	if (g_ruEngineSettings->GetSetting("audioFX:music frequency", propertyValue))
	{
		m_musicFrequency = boost::get<REAL>(propertyValue);
	}
	g_ruAudible->SetVolumeLevels(m_masterSoundLevel, m_soundFXSoundLevel, m_ambientSoundLevel, m_musicSoundLevel, m_uiSoundLevel);
}