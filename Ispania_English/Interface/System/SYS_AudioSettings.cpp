#include "../../mainproc/GameMain.h"
#include "../../mainproc/RuneDev.h"

#include "SYS_AudioSettings.h"

#include "../../netwaker_member/Net_Other/NetCli_OtherChild.h"


// ************************************************************************************************************************************************************

int LuaFunc_MasterVolumeSlider_GetValue(lua_State* L);
int LuaFunc_MusicVolumeSlider_GetValue(lua_State* L);
int LuaFunc_AmbienceVolumeSlider_GetValue(lua_State* L);
int LuaFunc_SoundFXVolumeSlider_GetValue(lua_State* L);
int LuaFunc_InterfaceSFXVolumeSlider_GetValue(lua_State* L);
int LuaFunc_MusicFrequencySlider_GetValue(lua_State* L);

int LuaFunc_MasterVolumeSlider_SetValue(lua_State* L);
int LuaFunc_MusicVolumeSlider_SetValue(lua_State* L);
int LuaFunc_AmbienceVolumeSlider_SetValue(lua_State* L);
int LuaFunc_SoundFXVolumeSlider_SetValue(lua_State* L);
int LuaFunc_InterfaceSFXVolumeSlider_SetValue(lua_State* L);
int LuaFunc_MusicFrequencySlider_SetValue(lua_State* L);

int LuaFunc_VoiceEnableCheckButton_IsChecked(lua_State* L); //
int LuaFunc_VoiceEnableCheckButton_SetChecked(lua_State* L); //
int LuaFunc_MicEnableCheckButton_IsChecked(lua_State* L); //
int LuaFunc_MicEnableCheckButton_SetChecked(lua_State* L); //
int LuaFunc_BlinkEnableCheckButton_IsChecked(lua_State* L); // OBSOLETE/DEPRECATED
int LuaFunc_BlinkEnableCheckButton_SetChecked(lua_State* L);
int LuaFunc_MicDropDown_GetSelected(lua_State* L); // 
int LuaFunc_MicDropDown_SetSelected(lua_State* L); //
int LuaFunc_MicMode_GetSelected(lua_State* L); //
int LuaFunc_MicMode_SetSelected(lua_State* L); //
int LuaFunc_SpeakerDropDown_GetSelected(lua_State* L); // 
int LuaFunc_SpeakerDropDown_SetSelected(lua_State* L); //
int LuaFunc_VoiceSettings_GetMicNum(lua_State* L); // 
int LuaFunc_VoiceSettings_GetMicName(lua_State* L); // 
int LuaFunc_VoiceSettings_SetMicVolume(lua_State* L); // GKy 091218 OnChane use
int LuaFunc_MicVolumeSlider_GetValue(lua_State* L); // 
int LuaFunc_MicVolumeSlider_SetValue(lua_State* L); //
int LuaFunc_VoiceSettings_SetMicSensitivity(lua_State* L); //c+++ GKy 091216 OnChange use
int LuaFunc_MicSensitivitySlider_GetValue(lua_State* L); //c+++
int LuaFunc_MicSensitivitySlider_SetValue(lua_State* L); //c+++
int LuaFunc_VoiceSettings_GetSpeakerNum(lua_State* L); //
int LuaFunc_VoiceSettings_GetSpeakerName(lua_State* L); //
int LuaFunc_VoiceSettings_SetSpeakerVolume(lua_State* L); //+ GKy 091218 OnChane use
int LuaFunc_SpeakerVolumeSlider_GetValue(lua_State* L); //
int LuaFunc_SpeakerVolumeSlider_SetValue(lua_State* L); //
//int LuaFunc_Vivox_Enable(lua_State* L); // GKy 091214 may be no need. LuaFunc_VoiceEnableCheckButton_SetChecked can handle
//int LuaFunc_Vivox_Disable(lua_State* L); //X
int LuaFunc_VoiceSettings_TestStart(lua_State* L); //+++
int LuaFunc_VoiceSettings_TestStop(lua_State* L); //+++
//int LuaFunc_VoiceSettings_MicEnable(lua_State* L); //--
//int LuaFunc_VoiceSettings_MicDisable(lua_State* L); //--
//int LuaFunc_VoiceSettings_BlinkEnable(lua_State* L); 
//int LuaFunc_VoiceSettings_BlinkDisable(lua_State* L);
int LuaFunc_VoiceSettings_GetTestVolume(lua_State* L); //+++
int LuaFunc_VoiceChat_PushToTalk_Start(lua_State* L); //
int LuaFunc_VoiceChat_PushToTalk_Stop(lua_State* L); //
int LuaFunc_VoiceChat_GetSelfVolume(lua_State* L); //
int LuaFunc_VoiceChat_GetLastTalkerName(lua_State* L); //
int LuaFunc_VoiceSettings_IsDisabled(lua_State* L); // for Lua
int LuaFunc_VoiceSettings_IsInRealChannel(lua_State* L);

int LuaFunc_VivoxPrintDebugMessage(lua_State* L);

// ************************************************************************************************************************************************************

CSYS_AudioSettings*				g_pSYS_AudioSettings = NULL;

// ************************************************************************************************************************************************************

// ----------------------------------------------------------------------------
CSYS_AudioSettings::CSYS_AudioSettings(CInterface* object)
:	CInterfaceBase(object)
{
	g_pSYS_AudioSettings = this;
}

// ----------------------------------------------------------------------------
CSYS_AudioSettings::~CSYS_AudioSettings()
{
	g_pSYS_AudioSettings = NULL;
}

// ----------------------------------------------------------------------------
void CSYS_AudioSettings::RegisterFunc()
{
	lua_State* L = GetLuaStateWorld();
	if (L)
	{
		lua_register(L, "MasterVolumeSlider_GetValue",			LuaFunc_MasterVolumeSlider_GetValue);
		lua_register(L, "MusicVolumeSlider_GetValue",			LuaFunc_MusicVolumeSlider_GetValue);
		lua_register(L, "AmbienceVolumeSlider_GetValue",		LuaFunc_AmbienceVolumeSlider_GetValue);
		lua_register(L, "SoundFXVolumeSlider_GetValue",			LuaFunc_SoundFXVolumeSlider_GetValue);
		lua_register(L, "InterfaceSFXVolumeSlider_GetValue",	LuaFunc_InterfaceSFXVolumeSlider_GetValue);
		lua_register(L, "MusicFrequencySlider_GetValue",		LuaFunc_MusicFrequencySlider_GetValue);

		lua_register(L, "MasterVolumeSlider_SetValue",			LuaFunc_MasterVolumeSlider_SetValue);
		lua_register(L, "MusicVolumeSlider_SetValue",			LuaFunc_MusicVolumeSlider_SetValue);
		lua_register(L, "AmbienceVolumeSlider_SetValue",		LuaFunc_AmbienceVolumeSlider_SetValue);
		lua_register(L, "SoundFXVolumeSlider_SetValue",			LuaFunc_SoundFXVolumeSlider_SetValue);
		lua_register(L, "InterfaceSFXVolumeSlider_SetValue",	LuaFunc_InterfaceSFXVolumeSlider_SetValue);
		lua_register(L, "MusicFrequencySlider_SetValue",		LuaFunc_MusicFrequencySlider_SetValue);

		lua_register(L, "VoiceEnableCheckButton_IsChecked",		LuaFunc_VoiceEnableCheckButton_IsChecked);
		lua_register(L, "VoiceEnableCheckButton_SetChecked",	LuaFunc_VoiceEnableCheckButton_SetChecked);
		lua_register(L, "MicEnableCheckButton_IsChecked",		LuaFunc_MicEnableCheckButton_IsChecked);
		lua_register(L, "MicEnableCheckButton_SetChecked",		LuaFunc_MicEnableCheckButton_SetChecked);
		lua_register(L, "BlinkEnableCheckButton_IsChecked",		LuaFunc_BlinkEnableCheckButton_IsChecked);
		lua_register(L, "BlinkEnableCheckButton_SetChecked",	LuaFunc_BlinkEnableCheckButton_SetChecked);
		lua_register(L, "MicDropDown_GetSelected",				LuaFunc_MicDropDown_GetSelected);
		lua_register(L, "MicDropDown_SetSelected",				LuaFunc_MicDropDown_SetSelected);
		lua_register(L, "MicMode_GetSelected",					LuaFunc_MicMode_GetSelected);
		lua_register(L, "MicMode_SetSelected",					LuaFunc_MicMode_SetSelected);
		lua_register(L, "SpeakerDropDown_GetSelected",			LuaFunc_SpeakerDropDown_GetSelected);
		lua_register(L, "SpeakerDropDown_SetSelected",			LuaFunc_SpeakerDropDown_SetSelected);
		lua_register(L, "VoiceSettings_GetMicNum",				LuaFunc_VoiceSettings_GetMicNum);
		lua_register(L, "VoiceSettings_GetMicName",				LuaFunc_VoiceSettings_GetMicName);
		lua_register(L, "VoiceSettings_SetMicVolume",			LuaFunc_VoiceSettings_SetMicVolume);
		lua_register(L, "MicVolumeSlider_GetValue",				LuaFunc_MicVolumeSlider_GetValue);
		lua_register(L, "MicVolumeSlider_SetValue",				LuaFunc_MicVolumeSlider_SetValue);
		lua_register(L, "VoiceSettings_SetMicSensitivity",		LuaFunc_VoiceSettings_SetMicSensitivity);
		lua_register(L, "MicSensitivitySlider_GetValue",		LuaFunc_MicSensitivitySlider_GetValue);
		lua_register(L, "MicSensitivitySlider_SetValue",		LuaFunc_MicSensitivitySlider_SetValue);
		lua_register(L, "VoiceSettings_GetSpeakerNum",			LuaFunc_VoiceSettings_GetSpeakerNum);
		lua_register(L, "VoiceSettings_GetSpeakerName",			LuaFunc_VoiceSettings_GetSpeakerName);
		lua_register(L, "VoiceSettings_SetSpeakerVolume",		LuaFunc_VoiceSettings_SetSpeakerVolume);
		lua_register(L, "SpeakerVolumeSlider_GetValue",			LuaFunc_SpeakerVolumeSlider_GetValue);
		lua_register(L, "SpeakerVolumeSlider_SetValue",			LuaFunc_SpeakerVolumeSlider_SetValue);
		//lua_register(L, "Vivox_Enable",							LuaFunc_Vivox_Enable);
		//lua_register(L, "Vivox_Disable",						LuaFunc_Vivox_Disable);
		lua_register(L, "VoiceSettings_TestStart",				LuaFunc_VoiceSettings_TestStart);
		lua_register(L, "VoiceSettings_TestStop",				LuaFunc_VoiceSettings_TestStop);
		//lua_register(L, "VoiceSettings_MicEnable",				LuaFunc_VoiceSettings_MicEnable);
		//lua_register(L, "VoiceSettings_MicDisable",				LuaFunc_VoiceSettings_MicDisable);
		//lua_register(L, "VoiceSettings_BlinkEnable",			LuaFunc_VoiceSettings_BlinkEnable);
		//lua_register(L, "VoiceSettings_BlinkDisable",			LuaFunc_VoiceSettings_BlinkDisable);
		lua_register(L, "VoiceSettings_GetTestVolume",			LuaFunc_VoiceSettings_GetTestVolume);
		lua_register(L, "VoiceChat_PushToTalk_Start",			LuaFunc_VoiceChat_PushToTalk_Start);
		lua_register(L, "VoiceChat_PushToTalk_Stop",			LuaFunc_VoiceChat_PushToTalk_Stop);
		lua_register(L, "VoiceChat_GetSelfVolume",				LuaFunc_VoiceChat_GetSelfVolume);
		lua_register(L, "VoiceChat_GetLastTalkerName",			LuaFunc_VoiceChat_GetLastTalkerName);
		lua_register(L, "VoiceSettings_IsDisabled",				LuaFunc_VoiceSettings_IsDisabled);
		lua_register(L, "VoiceSettings_IsInRealChannel",		LuaFunc_VoiceSettings_IsInRealChannel);	// 是否在測試頻道之外的頻道

        lua_register(L, "VivoxPrintDebugMessage",               LuaFunc_VivoxPrintDebugMessage);
	}
}

// ************************************************************************************************************************************************************

//////////////////////////////////////////////////////////////////////////

// 回傳 Vivox 除錯訊息 (for UI)
int LuaFunc_VivoxPrintDebugMessage(lua_State* L)
{

    lua_pushstring( L, "");

    return 1;
}

//////////////////////////////////////////////////////////////////////////

// ----------------------------------------------------------------------------
int LuaFunc_MasterVolumeSlider_GetValue(lua_State* L)
{
	RuPropertyType propertyValue;
	REAL volume = 0.0f;

	if(g_ruEngineSettings->GetSetting("audioFX:master volume", propertyValue))
	{
		volume = boost::get<REAL>(propertyValue) * 100;		
	}

	lua_pushnumber( L, volume );

	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_MusicVolumeSlider_GetValue(lua_State* L)
{
	RuPropertyType propertyValue;
	REAL volume = 0.0f;

	if(g_ruEngineSettings->GetSetting("audioFX:music volume", propertyValue))
	{
		volume = boost::get<REAL>(propertyValue) * 100;
	}

	lua_pushnumber( L, volume );

	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_AmbienceVolumeSlider_GetValue(lua_State* L)
{
	RuPropertyType propertyValue;
	REAL volume = 0.0f;

	if(g_ruEngineSettings->GetSetting("audioFX:ambience volume", propertyValue))
	{
		volume = boost::get<REAL>(propertyValue) * 100;		
	}

	lua_pushnumber( L, volume );

	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_SoundFXVolumeSlider_GetValue(lua_State* L)
{
	RuPropertyType propertyValue;
	REAL volume = 0.0f;

	if(g_ruEngineSettings->GetSetting("audioFX:sound fx volume", propertyValue))
	{
		volume = boost::get<REAL>(propertyValue) * 100;		
	}

	lua_pushnumber( L, volume );

	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_InterfaceSFXVolumeSlider_GetValue(lua_State* L)
{
	RuPropertyType propertyValue;
	REAL volume = 0.0f;

	if(g_ruEngineSettings->GetSetting("audioFX:interface sfx volume", propertyValue))
	{
		volume = boost::get<REAL>(propertyValue) * 100;		
	}

	lua_pushnumber( L, volume );

	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_MusicFrequencySlider_GetValue(lua_State* L)
{
	RuPropertyType propertyValue;
	REAL freq = 0.0f;

	if(g_ruEngineSettings->GetSetting("audioFX:music frequency", propertyValue))
	{
		freq = boost::get<REAL>(propertyValue);
	}

	lua_pushnumber( L, freq );

	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_MasterVolumeSlider_SetValue(lua_State* L)
{
	RuPropertyType propertyValue;
	REAL volume = (float)luaL_checknumber( L, 1 );

	if(g_ruEngineSettings->GetSetting("audioFX:master volume", propertyValue))
	{
		REAL oldVolume = boost::get<REAL>(propertyValue);
		REAL newVolume = volume / 100.0f;

		if(fabs(newVolume - oldVolume) > 0.01f)
		{
			g_ruEngineSettings->SetSetting("audioFX:master volume", newVolume);
		}
	}
	else
	{
		g_ruEngineSettings->SetSetting("audioFX:master volume", volume / 100.0f);
	}

	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_MusicVolumeSlider_SetValue(lua_State* L)
{
	RuPropertyType propertyValue;
	REAL volume = (float)luaL_checknumber( L, 1 );

	if(g_ruEngineSettings->GetSetting("audioFX:music volume", propertyValue))
	{
		REAL oldVolume = boost::get<REAL>(propertyValue);
		REAL newVolume = volume / 100.0f;

		if(fabs(newVolume - oldVolume) > 0.01f)
		{
			g_ruEngineSettings->SetSetting("audioFX:music volume", newVolume);
		}
	}
	else
	{
		g_ruEngineSettings->SetSetting("audioFX:music volume", volume / 100.0f);
	}

	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_AmbienceVolumeSlider_SetValue(lua_State* L)
{
	RuPropertyType propertyValue;
	REAL volume = (float)luaL_checknumber( L, 1 );

	if(g_ruEngineSettings->GetSetting("audioFX:ambience volume", propertyValue))
	{
		REAL oldVolume = boost::get<REAL>(propertyValue);
		REAL newVolume = volume / 100.0f;

		if(fabs(newVolume - oldVolume) > 0.01f)
		{
			g_ruEngineSettings->SetSetting("audioFX:ambience volume", newVolume);
		}
	}
	else
	{
		g_ruEngineSettings->SetSetting("audioFX:ambience volume", volume / 100.0f);
	}

	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_SoundFXVolumeSlider_SetValue(lua_State* L)
{
	RuPropertyType propertyValue;
	REAL volume = (float)luaL_checknumber( L, 1 );

	if(g_ruEngineSettings->GetSetting("audioFX:sound fx volume", propertyValue))
	{
		REAL oldVolume = boost::get<REAL>(propertyValue);
		REAL newVolume = volume / 100.0f;

		if(fabs(newVolume - oldVolume) > 0.01f)
		{
			g_ruEngineSettings->SetSetting("audioFX:sound fx volume", newVolume);
		}
	}
	else
	{
		g_ruEngineSettings->SetSetting("audioFX:sound fx volume", volume / 100.0f);
	}

	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_InterfaceSFXVolumeSlider_SetValue(lua_State* L)
{
	RuPropertyType propertyValue;
	REAL volume = (float)luaL_checknumber( L, 1 );

	if(g_ruEngineSettings->GetSetting("audioFX:interface sfx volume", propertyValue))
	{
		REAL oldVolume = boost::get<REAL>(propertyValue);
		REAL newVolume = volume / 100.0f;

		if(fabs(newVolume - oldVolume) > 0.01f)
		{
			g_ruEngineSettings->SetSetting("audioFX:interface sfx volume", newVolume);
		}
	}
	else
	{
		g_ruEngineSettings->SetSetting("audioFX:interface sfx volume", volume / 100.0f);
	}

	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_MusicFrequencySlider_SetValue(lua_State* L)
{
	RuPropertyType propertyValue;
	REAL freq = (float)luaL_checknumber( L, 1 );

	if(g_ruEngineSettings->GetSetting("audioFX:music frequency", propertyValue))
	{
		REAL oldFreq = boost::get<REAL>(propertyValue);

		if(fabs(freq - oldFreq) > 0.1f)
		{
			g_ruEngineSettings->SetSetting("audioFX:music frequency", freq);
		}
	}
	else
	{
		g_ruEngineSettings->SetSetting("audioFX:music frequency", freq);
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////
// Vovix Settings
//////////////////////////////////////////////////////////////////////////

// ----------------------------------------------------------------------------
int LuaFunc_VoiceEnableCheckButton_IsChecked(lua_State* L)
{

	lua_pushboolean(L, FALSE);

	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_VoiceEnableCheckButton_SetChecked(lua_State* L)
{

	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_MicEnableCheckButton_IsChecked(lua_State* L)
{
	lua_pushboolean(L, FALSE);

	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_MicEnableCheckButton_SetChecked(lua_State* L)
{

	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_BlinkEnableCheckButton_IsChecked(lua_State* L)
{
	BOOL isChecked = FALSE;
	lua_pushboolean( L, isChecked );
	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_BlinkEnableCheckButton_SetChecked(lua_State* L)
{
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_MicDropDown_GetSelected(lua_State* L)
{

	lua_pushnumber(L, 0);

	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_MicDropDown_SetSelected(lua_State* L)
{

	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_MicMode_GetSelected(lua_State* L)
{

	lua_pushnumber(L, 0);

	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_MicMode_SetSelected(lua_State* L)
{

	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_SpeakerDropDown_GetSelected(lua_State* L)
{

	lua_pushnumber(L, 0);

	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_SpeakerDropDown_SetSelected(lua_State* L)
{

	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_VoiceSettings_GetMicNum(lua_State* L)
{

	lua_pushnumber(L, 0);

	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_VoiceSettings_GetMicName(lua_State* L)
{

    wchar_t szwBuffer[128];

	lua_pushstring(L, szwBuffer);

	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_VoiceSettings_SetMicVolume(lua_State* L)
{

	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_MicVolumeSlider_GetValue(lua_State* L)
{
	//lua_pushnumber(L, volume); // 0.0~1.0 ?
	lua_pushnumber(L, 0); // 0~100 try

	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_MicVolumeSlider_SetValue(lua_State* L)
{
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_VoiceSettings_SetMicSensitivity(lua_State* L)
{
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_MicSensitivitySlider_GetValue(lua_State* L)
{
	lua_pushnumber(L, 0);

	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_MicSensitivitySlider_SetValue(lua_State* L)
{
    return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_VoiceSettings_GetSpeakerNum(lua_State* L)
{
	lua_pushnumber(L, 0);

	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_VoiceSettings_GetSpeakerName(lua_State* L)
{
    wchar_t szwBuffer[128];
	lua_pushstring(L, szwBuffer);

	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_VoiceSettings_SetSpeakerVolume(lua_State* L)
{
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_SpeakerVolumeSlider_GetValue(lua_State* L)
{
	lua_pushnumber(L, 0); // 0~100

	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_SpeakerVolumeSlider_SetValue(lua_State* L)
{
    return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_VoiceSettings_TestStart(lua_State* L)
{
    return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_VoiceSettings_TestStop(lua_State* L)
{
    return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_VoiceSettings_GetTestVolume(lua_State* L)
{
	lua_pushnumber(L, 0); // 0~100

    return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_VoiceChat_PushToTalk_Start(lua_State* L)
{
    return 0;
}
// ----------------------------------------------------------------------------
int LuaFunc_VoiceChat_PushToTalk_Stop(lua_State* L)
{
    return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_VoiceChat_GetSelfVolume(lua_State* L)
{
	lua_pushnumber(L, 0); // 0~100

	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_VoiceChat_GetLastTalkerName(lua_State* L)
{
    lua_pushstring(L, "");

	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_VoiceSettings_IsDisabled(lua_State* L)
{
	lua_pushboolean(L, g_pGameMain->GetVoiceDisable());

	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_VoiceSettings_IsInRealChannel(lua_State* L)
{
	lua_pushboolean(L, FALSE);
	return 1;
}

// ************************************************************************************************************************************************************
