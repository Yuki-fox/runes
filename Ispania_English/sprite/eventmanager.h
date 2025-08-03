#pragma once
#include <list>
#include <map>
#include <string>
#include "Rune Engine/Rune/Rune.h"
#include "Rune Engine/Rune/Rune Engine Audio.h"
//-----------------------------------------------------------------------------
// Model �ʧ@�ƥ�B�z
class CRuGlobalEventManager
{
  protected:
	struct SoundEventDescriptor
	{
		CRuArrayList<PTRVALUE> m_soundHandles; //!< List of active sound handles for the sprite
	};
	CRuGUID m_GUID; //!< GUID used for internal event registration
	CRuEntity_Container_Impl *m_controllerFrame; //!< Frame that contains all controllers
	CRuFusion_AudioLibrary *m_audioLibrary;		 //!< Audio library
	CRuCamera *m_camera;						 //!< Camera used by camera jiggler
	CRuHashMap<PTRVALUE, SoundEventDescriptor *> *m_soundEvents; //!< Hash of active sound events
	// Sound volume levels
	REAL m_masterSoundLevel;
	REAL m_ambientSoundLevel;
	REAL m_musicSoundLevel;
	REAL m_soundFXSoundLevel;
	REAL m_uiSoundLevel;
	REAL m_musicFrequency;
	// Settings
	BOOL m_enableSoundFX;
	BOOL m_enableMusic;
	BOOL m_enableEnvironmentMusic;
	// Zone & triggers
	INT32 m_timeGroup;
	CRuWorld_ZoneSettings *m_zoneSettings;
	CRuWorld_Trigger *m_musicTrigger;
	CRuWorld_Trigger *m_ambienceTrigger;
	CRuWorld_Trigger *m_supplementAmbienceTrigger;
	CRuWorld_Trigger *m_randomAmbienceTrigger;
	CRuArrayList<CRuWorld_Trigger *> m_3DAmbienceTriggers;
	CRuHashMap<CRuGUID, CRuWorld_Trigger *> *m_3DAmbienceTriggerHash;
	CRuTernaryStringTree<CRuWorld_Trigger *> m_3DAmbienceGroupTriggers;
	CRuHashMap<CRuGUID, PTRVALUE> *m_active3DAmbienceSounds;
	CRuArrayList<CRuGUID> m_deleted3DAmbienceTriggers;
	// Persistent sound handles
	PTRVALUE m_musicSoundHandle;
	PTRVALUE m_ambientSoundHandle;
	PTRVALUE m_supplementAmbientSoundHandle;
	BOOL m_rerollMusic;
	CRuString m_activeMusicName;
	CRuString m_activeAmbienceName;
	CRuString m_activeSupplementAmbienceName;
	REAL m_timeToNextRandomAmbienceRoll;
	REAL m_timeToNextMusicRoll;
	DWORD m_lastUpdateTime;
	struct PlaySoundDelay
	{
		string m_soundPath;
		CRuVector3 m_pos;
		RuAudioLoopMode m_loop;
		BOOL m_is3D;
		REAL m_delayTime;
	};
	list<PlaySoundDelay> m_playSoundDelay;
	map<INT32, PTRVALUE> m_serverPlaySound;
  public:
	CRuGlobalEventManager();
	virtual ~CRuGlobalEventManager();
	void SetAudioLibrary(CRuFusion_AudioLibrary *audioLibrary);
	void SetCamera(CRuCamera *camera);
	void AddEntity(CRuEntity *pRuEntity);
	void RemoveEntity(CRuEntity *pRuEntity);
	// Updates
	void Update(REAL elapsedTime);
	// Entity event registration
	void RegisterEventHandlers(CRuEntity *entity, PTRVALUE userData);
	// Event handlers
	BOOL HandleEvent_Dispose(RuEventArgs *eventArgs);
	BOOL HandleEvent_Trigger(RuEventArgs *eventArgs);
	void StopSpriteSounds(void *sprite);
	// Zone and trigger handling
	void SetTimeGroup(INT32 timeGroup);
	void SetZoneSettings(CRuWorld_ZoneSettings *zoneSettings);
	void SetMusicTrigger(CRuWorld_Trigger *musicTrigger);
	void SetAmbienceTrigger(CRuWorld_Trigger *ambienceTrigger);
	void SetSupplementAmbienceTrigger(CRuWorld_Trigger *supplementAmbienceTrigger);
	void SetRandomAmbienceTrigger(CRuWorld_Trigger *randomAmbienceTrigger);
	// 3D ambience trigger handling
	void AddRef3DAmbienceTriggers();
	void Clear3DAmbienceTriggers();
	CRuArrayList<CRuWorld_Trigger *> &Get3DAmbienceTriggers();
	void Update3DAmbienceTriggers(REAL elapsedTime);
	void StartActive3DAmbienceTriggers();
	void UpdateActive3DAmbienceTriggers();
	void Insert3DAmbienceGroupTriggers();
	BOOL Insert3DAmbienceGroupTriggersCallback(const char *key, void *data);
	void StopInactive3DAmbienceTriggers();
	BOOL StopInactive3DAmbienceTriggersCallback(const void *key, void *data);
	// Raw sound playback
	void PlaySoundByFile(const char *soundPath, BOOL loop, REAL delay = 0.0f, INT32 soundType = ruSOUNDTYPE_SFX, INT32 soundIndex = 0);
	void Play3DSoundByFile(const char *soundPath, CRuVector3 pos, BOOL loop, REAL delay = 0.0f, INT32 soundType = ruSOUNDTYPE_SFX, INT32 soundIndex = 0);
	void StopSoundByIndex(INT32 soundIndex);
	void ClearAllSoundIdex();
	// ���񭵼�
	void PlayMusicByFile(const char *musicPath, REAL musicOccurrenceRate = 1.0f, BOOL loop = FALSE);
	void StopMusic();
	void SetEnvironmentMusicEnable(BOOL enableEnvironmentMusic);
	PTRVALUE GetMusicSoundHandle();
  protected:
	void PushSoundEvent(void *sprite, PTRVALUE soundHandle);
	void ClearSoundEventQueue();
	BOOL ClearSoundEventQueueCallback(const void *key, void *data);
	// Event handlers
	BOOL PlaybackFinishedHandler(RuEventArgs *eventArgs);
	BOOL EngineSettingsChangedHandler(const char *settingName, const RuPropertyType &setting);
	void GetSettingsFromEngine();
};
//-----------------------------------------------------------------------------
extern CRuGlobalEventManager *g_pRuGlobalEventManager;