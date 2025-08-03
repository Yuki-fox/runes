#include "../../mainproc/GameMain.h"
#include "../../mainproc/RuneDev.h"

#include "SYS_DisplaySettings.h"

// ************************************************************************************************************************************************************

int LuaFunc_SYS_DisplaySettings_OnShow(lua_State* L);
int LuaFunc_SYS_DisplaySettings_OnApply(lua_State* L);

int LuaFunc_SYS_DisplaySettings_GetNumDisplayModes(lua_State* L);
int LuaFunc_SYS_DisplaySettings_GetDisplayModeWidth(lua_State* L);
int LuaFunc_SYS_DisplaySettings_GetDisplayModeHeight(lua_State* L);
int LuaFunc_SYS_DisplaySettings_GetDisplayModeRefreshRate(lua_State* L);

int LuaFunc_SYS_DisplaySettings_SetResolutionNeedChange(lua_State* L);
int LuaFunc_SYS_DisplaySettings_SetNeedToBeWindowed(lua_State* L);

int LuaFunc_IsFullscreen(lua_State* L);

int LuaFunc_UIScaleCheckButton_IsChecked(lua_State* L);
int LuaFunc_UIScaleSlider_GetValue(lua_State* L);

int LuaFunc_RTLightMapCheckBox_IsChecked(lua_State* L);
int LuaFunc_TerrainShaderDetailSlider_GetValue(lua_State* L);
int LuaFunc_LightMapResSlider_GetValue(lua_State* L);
int LuaFunc_ShadowDetailSlider_GetValue(lua_State* L);

int LuaFunc_WaterReflectionCheckButton_IsChecked(lua_State* L);
int LuaFunc_WaterRefractionCheckButton_IsChecked(lua_State* L);
int LuaFunc_WaterQualitySlider_GetValue(lua_State* L);

int LuaFunc_BloomCheckButton_IsChecked(lua_State* L);
int LuaFunc_GlowCheckButton_IsChecked(lua_State* L);
int LuaFunc_BloomQualitySlider_GetValue(lua_State* L);

int LuaFunc_PaperdollDetailSlider_GetValue(lua_State* L);
int LuaFunc_ViewDistanceSlider_GetValue(lua_State* L);
int LuaFunc_WorldDetailSlider_GetValue(lua_State* L);
int LuaFunc_SkyDetailSlider_GetValue(lua_State* L);
int LuaFunc_TextureDetailSlider_GetValue(lua_State* L);
int LuaFunc_TerrainTextureDetailSlider_GetValue(lua_State* L);

int LuaFunc_SmoothBlendCheckButton_IsChecked(lua_State* L);
int LuaFunc_DistortFXCheckButton_IsChecked(lua_State* L);
int LuaFunc_DebaseTextureCheckButton_IsChecked(lua_State* L);
int LuaFunc_SpecularHighlightCheckButton_IsChecked(lua_State* L);
int LuaFunc_DisableMemoryWarningCheckButton_IsChecked(lua_State* L);

int LuaFunc_GetDisplayQuality(lua_State* L);

int LuaFunc_UIScaleCheckButton_SetChecked(lua_State* L);
int LuaFunc_UIScaleSlider_SetValue(lua_State* L);

int LuaFunc_RTLightMapCheckBox_SetChecked(lua_State* L);
int LuaFunc_TerrainShaderDetailSlider_SetValue(lua_State* L);
int LuaFunc_LightMapResSlider_SetValue(lua_State* L);
int LuaFunc_ShadowDetailSlider_SetValue(lua_State* L);

int LuaFunc_WaterReflectionCheckButton_SetChecked(lua_State* L);
int LuaFunc_WaterRefractionCheckButton_SetChecked(lua_State* L);
int LuaFunc_WaterQualitySlider_SetValue(lua_State* L);

int LuaFunc_BloomCheckButton_SetChecked(lua_State* L);
int LuaFunc_GlowCheckButton_SetChecked(lua_State* L);
int LuaFunc_BloomQualitySlider_SetValue(lua_State* L);

int LuaFunc_PaperdollDetailSlider_SetValue(lua_State* L);
int LuaFunc_ViewDistanceSlider_SetValue(lua_State* L);
int LuaFunc_WorldDetailSlider_SetValue(lua_State* L);
int LuaFunc_SkyDetailSlider_SetValue(lua_State* L);
int LuaFunc_TextureDetailSlider_SetValue(lua_State* L);
int LuaFunc_TerrainTextureDetailSlider_SetValue(lua_State* L);

int LuaFunc_SmoothBlendCheckButton_SetChecked(lua_State* L);
int LuaFunc_DistortFXCheckButton_SetChecked(lua_State* L);
int LuaFunc_DebaseTextureCheckButton_SetChecked(lua_State* L);
int LuaFunc_SpecularHighlightCheckButton_SetChecked(lua_State* L);
int LuaFunc_DisableMemoryWarningCheckButton_SetChecked(lua_State* L);

int LuaFunc_SetDisplayQuality(lua_State* L);

// ************************************************************************************************************************************************************

CSYS_DisplaySettings*				g_pSYS_DisplaySettings = NULL;

// ************************************************************************************************************************************************************

// ----------------------------------------------------------------------------
CSYS_DisplaySettings::CSYS_DisplaySettings(CInterface* object)
:	CInterfaceBase(object),
	m_resolutionNeedChange(FALSE),
	m_NeedToBeWindowed(FALSE)
{
	g_pSYS_DisplaySettings = this;
}

// ----------------------------------------------------------------------------
CSYS_DisplaySettings::~CSYS_DisplaySettings()
{
	g_pSYS_DisplaySettings = NULL;
}

// ----------------------------------------------------------------------------
void CSYS_DisplaySettings::RegisterFunc()
{
	lua_State* L = GetLuaStateWorld();
	if (L)
	{
		lua_register(L, "SYS_DisplaySettings_OnShow",						LuaFunc_SYS_DisplaySettings_OnShow);
		lua_register(L, "SYS_DisplaySettings_OnApply",						LuaFunc_SYS_DisplaySettings_OnApply);

		lua_register(L, "SYS_DispSet_GetNumDispModes",						LuaFunc_SYS_DisplaySettings_GetNumDisplayModes);
		lua_register(L, "SYS_DispSet_GetDispWidth",							LuaFunc_SYS_DisplaySettings_GetDisplayModeWidth);
		lua_register(L, "SYS_DispSet_GetDispHeight",						LuaFunc_SYS_DisplaySettings_GetDisplayModeHeight);
		lua_register(L, "SYS_DispSet_GetDispRefreshRate",					LuaFunc_SYS_DisplaySettings_GetDisplayModeRefreshRate);

		lua_register(L, "SYS_DispSet_SetResolutionNeedChange",				LuaFunc_SYS_DisplaySettings_SetResolutionNeedChange);
		lua_register(L, "SYS_DispSet_SetNeedToBeWindowed",					LuaFunc_SYS_DisplaySettings_SetNeedToBeWindowed);

		lua_register(L, "IsFullscreen",										LuaFunc_IsFullscreen);
		lua_register(L, "UIScaleCheckButton_IsChecked",						LuaFunc_UIScaleCheckButton_IsChecked);
		lua_register(L, "UIScaleSlider_GetValue",							LuaFunc_UIScaleSlider_GetValue);

		lua_register(L, "RTLightMapCheckBox_IsChecked",						LuaFunc_RTLightMapCheckBox_IsChecked);
		lua_register(L, "TerrainShaderDetailSlider_GetValue",				LuaFunc_TerrainShaderDetailSlider_GetValue);
		lua_register(L, "LightMapResSlider_GetValue",						LuaFunc_LightMapResSlider_GetValue);
		lua_register(L, "ShadowDetailSlider_GetValue",						LuaFunc_ShadowDetailSlider_GetValue);

		lua_register(L, "WaterReflectionCheckButton_IsChecked",				LuaFunc_WaterReflectionCheckButton_IsChecked);
		lua_register(L, "WaterRefractionCheckButton_IsChecked",				LuaFunc_WaterRefractionCheckButton_IsChecked);
		lua_register(L, "WaterQualitySlider_GetValue",						LuaFunc_WaterQualitySlider_GetValue);

		lua_register(L, "BloomCheckButton_IsChecked",						LuaFunc_BloomCheckButton_IsChecked);
		lua_register(L, "GlowCheckButton_IsChecked",						LuaFunc_GlowCheckButton_IsChecked);
		lua_register(L, "BloomQualitySlider_GetValue",						LuaFunc_BloomQualitySlider_GetValue);

		lua_register(L, "PaperdollDetailSlider_GetValue",					LuaFunc_PaperdollDetailSlider_GetValue);
		lua_register(L, "ViewDistanceSlider_GetValue",						LuaFunc_ViewDistanceSlider_GetValue);
		lua_register(L, "WorldDetailSlider_GetValue",						LuaFunc_WorldDetailSlider_GetValue);
		lua_register(L, "SkyDetailSlider_GetValue",							LuaFunc_SkyDetailSlider_GetValue);
		lua_register(L, "TextureDetailSlider_GetValue",						LuaFunc_TextureDetailSlider_GetValue);
		lua_register(L, "TerrainTextureDetailSlider_GetValue",				LuaFunc_TerrainTextureDetailSlider_GetValue);

		lua_register(L, "SmoothBlendCheckButton_IsChecked",					LuaFunc_SmoothBlendCheckButton_IsChecked);
		lua_register(L, "DistortFXCheckButton_IsChecked",					LuaFunc_DistortFXCheckButton_IsChecked);
		lua_register(L, "DebaseTextureCheckButton_IsChecked",				LuaFunc_DebaseTextureCheckButton_IsChecked);
		lua_register(L, "SpecularHighlightCheckButton_IsChecked",			LuaFunc_SpecularHighlightCheckButton_IsChecked);
		lua_register(L, "DisableMemoryWarningCheckButton_IsChecked",		LuaFunc_DisableMemoryWarningCheckButton_IsChecked);

		lua_register(L, "GetDisplayQuality",								LuaFunc_GetDisplayQuality);

		lua_register(L, "UIScaleCheckButton_SetChecked",					LuaFunc_UIScaleCheckButton_SetChecked);
		lua_register(L, "UIScaleSlider_SetValue",							LuaFunc_UIScaleSlider_SetValue);

		lua_register(L, "RTLightMapCheckBox_SetChecked",					LuaFunc_RTLightMapCheckBox_SetChecked);
		lua_register(L, "TerrainShaderDetailSlider_SetValue",				LuaFunc_TerrainShaderDetailSlider_SetValue);
		lua_register(L, "LightMapResSlider_SetValue",						LuaFunc_LightMapResSlider_SetValue);
		lua_register(L, "ShadowDetailSlider_SetValue",						LuaFunc_ShadowDetailSlider_SetValue);

		lua_register(L, "WaterReflectionCheckButton_SetChecked",			LuaFunc_WaterReflectionCheckButton_SetChecked);
		lua_register(L, "WaterRefractionCheckButton_SetChecked",			LuaFunc_WaterRefractionCheckButton_SetChecked);
		lua_register(L, "WaterQualitySlider_SetValue",						LuaFunc_WaterQualitySlider_SetValue);

		lua_register(L, "BloomCheckButton_SetChecked",						LuaFunc_BloomCheckButton_SetChecked);
		lua_register(L, "GlowCheckButton_SetChecked",						LuaFunc_GlowCheckButton_SetChecked);
		lua_register(L, "BloomQualitySlider_SetValue",						LuaFunc_BloomQualitySlider_SetValue);

		lua_register(L, "PaperdollDetailSlider_SetValue",					LuaFunc_PaperdollDetailSlider_SetValue);
		lua_register(L, "ViewDistanceSlider_SetValue",						LuaFunc_ViewDistanceSlider_SetValue);
		lua_register(L, "WorldDetailSlider_SetValue",						LuaFunc_WorldDetailSlider_SetValue);
		lua_register(L, "SkyDetailSlider_SetValue",							LuaFunc_SkyDetailSlider_SetValue);
		lua_register(L, "TextureDetailSlider_SetValue",						LuaFunc_TextureDetailSlider_SetValue);
		lua_register(L, "TerrainTextureDetailSlider_SetValue",				LuaFunc_TerrainTextureDetailSlider_SetValue);

		lua_register(L, "SmoothBlendCheckButton_SetChecked",				LuaFunc_SmoothBlendCheckButton_SetChecked);
		lua_register(L, "DistortFXCheckButton_SetChecked",					LuaFunc_DistortFXCheckButton_SetChecked);
		lua_register(L, "DebaseTextureCheckButton_SetChecked",				LuaFunc_DebaseTextureCheckButton_SetChecked);
		lua_register(L, "SpecularHighlightCheckButton_SetChecked",			LuaFunc_SpecularHighlightCheckButton_SetChecked);
		lua_register(L, "DisableMemoryWarningCheckButton_SetChecked",		LuaFunc_DisableMemoryWarningCheckButton_SetChecked);		

		lua_register(L, "SetDisplayQuality",								LuaFunc_SetDisplayQuality);
	}
}

// ----------------------------------------------------------------------------
void CSYS_DisplaySettings::OnShow()
{
	// Get current display dimensions

#ifdef KALYDO
	// for Kalydo, always get the fullscreen size as the windowed size is fixed.
	SIZE fullscreenSize = g_pRuneDev->GetResolution(false);
#else
	SIZE fullscreenSize = g_ruEngine->IsFullscreen() ? g_pRuneDev->GetResolution(false) : g_pRuneDev->GetResolution(true);
#endif

	// Build list of display modes
	INT32 selectedModeID = 1;
	m_displayModes.Clear();
	for(INT32 i = 0; i < g_ruEngine->GetNumAvailableDisplayModes(); ++i)
	{
		RuDisplayMode dispMode = g_ruEngine->GetAvailableDisplayModeByIndex(i);

		// Filter out everything except 32-bit modes
		if(dispMode.m_format == ruSURFACEFMT_X8R8G8B8)
		{
			// Make sure this mode is not a duplicate mode
			BOOL modeIsDuplicate = FALSE;
			for(INT32 j = 0; j < m_displayModes.Count(); ++j)
			{
				if((m_displayModes[j].m_width == dispMode.m_width) && (m_displayModes[j].m_height == dispMode.m_height))
				{
					modeIsDuplicate = TRUE;
					break;
				}
			}

			if(!modeIsDuplicate)
			{
				m_displayModes.Add(dispMode);

				if(fullscreenSize.cx == dispMode.m_width && fullscreenSize.cy == dispMode.m_height)
				{
					selectedModeID = m_displayModes.Count();
				}
			}
		}
	}

	lua_State* L = this->GetLuaState();
	if (L)
	{
		switch( m_interface->GetProcessType() )
		{
		case CInterface::eInterface_Process_Login:
			lua_getglobal(L, "LoginDropDownList_Initialize");
			lua_getglobal(L, "LSF_ResolutionDropDown");
			lua_getglobal(L, "LSF_ResolutionDropDown_Show");
			lua_pcall(L, 2, 0, 0);

			lua_getglobal(L, "LoginDropDownList_SetSelectedID");
			lua_getglobal(L, "LSF_ResolutionDropDown");
			lua_pushnumber(L, selectedModeID);
			lua_pcall(L, 2, 0, 0);
			break;
		case CInterface::eInterface_Process_World:
			lua_getglobal(L, "UIDropDownMenu_Initialize");
			lua_getglobal(L, "SYS_DisplaySettings_ResolutionDropDown");
			lua_getglobal(L, "SYS_DisplaySettings_ResolutionDropDown_Show");
			lua_pcall(L, 2, 0, 0);

			lua_getglobal(L, "UIDropDownMenu_SetSelectedID");
			lua_getglobal(L, "SYS_DisplaySettings_ResolutionDropDown");
			lua_pushnumber(L, selectedModeID);
			lua_pcall(L, 2, 0, 0);
			break;
		}
	}
}

// ----------------------------------------------------------------------------
void CSYS_DisplaySettings::OnApply()
{
	if( m_resolutionNeedChange )
	{
		lua_State* L = this->GetLuaState();
		if (L)
		{
			// Get selected ID for the display mode
			switch( m_interface->GetProcessType() )
			{
			case CInterface::eInterface_Process_Login:
				lua_getglobal(L, "LoginDropDownList_GetSelectedID");
				lua_getglobal(L, "LSF_ResolutionDropDown");
				lua_pcall(L, 1, 1, 0);
				break;
			case CInterface::eInterface_Process_World:
				lua_getglobal(L, "UIDropDownMenu_GetSelectedID");
				lua_getglobal(L, "SYS_DisplaySettings_ResolutionDropDown");
				lua_pcall(L, 1, 1, 0);
				break;
			}			
			
			INT32 selectedDisplayModeIdx = static_cast<NPC_MoveTypeENUM>(luaL_checkint(L, -1) - 1);

			// Set video mode
			g_pRuneDev->SetScreenSize(m_displayModes[selectedDisplayModeIdx].m_width, m_displayModes[selectedDisplayModeIdx].m_height, (m_NeedToBeWindowed != 0), TRUE);

			m_resolutionNeedChange = false;
		}
	}

	g_pGameMain->SaveSystemVariables();
}

// ----------------------------------------------------------------------------
INT32 CSYS_DisplaySettings::GetNumDisplayModes()
{
	return m_displayModes.Count();
}

// ----------------------------------------------------------------------------
INT32 CSYS_DisplaySettings::GetDisplayModeWidth(INT32 idx)
{
	return m_displayModes[idx].m_width;
}

// ----------------------------------------------------------------------------
INT32 CSYS_DisplaySettings::GetDisplayModeHeight(INT32 idx)
{
	return m_displayModes[idx].m_height;
}

// ----------------------------------------------------------------------------
INT32 CSYS_DisplaySettings::GetDisplayModeRefreshRate(INT32 idx)
{
	return m_displayModes[idx].m_refreshRate;
}

// ----------------------------------------------------------------------------
void CSYS_DisplaySettings::SetResolutionNeedChange(BOOL needChange)
{
	m_resolutionNeedChange = needChange;
}

// ----------------------------------------------------------------------------
void CSYS_DisplaySettings::SetNeedToBeWindowed(BOOL beWindowed)
{
	m_NeedToBeWindowed = beWindowed;
}

// ************************************************************************************************************************************************************

// ----------------------------------------------------------------------------
int LuaFunc_SYS_DisplaySettings_OnShow(lua_State* L)
{
	g_pSYS_DisplaySettings->OnShow();
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_SYS_DisplaySettings_OnApply(lua_State* L)
{
	g_pSYS_DisplaySettings->OnApply();
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_SYS_DisplaySettings_GetNumDisplayModes(lua_State* L)
{
	INT32 ret = g_pSYS_DisplaySettings->GetNumDisplayModes();
	lua_pushnumber(L, ret);

	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_SYS_DisplaySettings_GetDisplayModeWidth(lua_State* L)
{
	int idx = luaL_checkint(L, 1);
	INT32 ret = g_pSYS_DisplaySettings->GetDisplayModeWidth(idx);
	lua_pushnumber(L, ret);

	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_SYS_DisplaySettings_GetDisplayModeHeight(lua_State* L)
{
	int idx = luaL_checkint(L, 1);
	INT32 ret = g_pSYS_DisplaySettings->GetDisplayModeHeight(idx);
	lua_pushnumber(L, ret);

	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_SYS_DisplaySettings_GetDisplayModeRefreshRate(lua_State* L)
{
	int idx = luaL_checkint(L, 1);
	INT32 ret = g_pSYS_DisplaySettings->GetDisplayModeRefreshRate(idx);
	lua_pushnumber(L, ret);

	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_SYS_DisplaySettings_SetResolutionNeedChange(lua_State* L)
{
	g_pSYS_DisplaySettings->SetResolutionNeedChange(lua_toboolean(L, 1) != 0);

	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_SYS_DisplaySettings_SetNeedToBeWindowed(lua_State* L)
{
	g_pSYS_DisplaySettings->SetNeedToBeWindowed(lua_toboolean(L, 1) != 0);

	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_IsFullscreen(lua_State* L)
{
#ifdef KALYDO
	// for Kalydo, the engine always thinks it is windowed. Get the setting from the gamemain.
	lua_pushboolean( L, g_pGameMain->IsWindowed() == FALSE );
#else
	lua_pushboolean( L, g_ruEngine->IsFullscreen() == TRUE );
#endif

	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_UIScaleCheckButton_IsChecked(lua_State* L)
{
	RuPropertyType propertyValue;
	BOOL isChecked = FALSE;

	if( g_ruEngineSettings->GetSetting("uiscale:enable", propertyValue) )
	{
		isChecked = boost::get<BOOL>(propertyValue);
	}

	lua_pushboolean( L, isChecked );

	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_UIScaleSlider_GetValue(lua_State* L)
{
	RuPropertyType propertyValue;
	REAL uiScale = 0.0f;

	if( g_ruEngineSettings->GetSetting("ui:scale", propertyValue) )
	{
		uiScale = boost::get<REAL>(propertyValue);
	}

	lua_pushnumber( L, uiScale );

	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_RTLightMapCheckBox_IsChecked(lua_State* L)
{
	RuPropertyType propertyValue;
	BOOL isChecked = FALSE;

	if( g_ruEngineSettings->GetSetting("RTLightMap:enable", propertyValue) )
	{
		isChecked = boost::get<BOOL>(propertyValue);
	}

	lua_pushboolean( L, isChecked );

	return 1;		
}

// ----------------------------------------------------------------------------
int LuaFunc_TerrainShaderDetailSlider_GetValue(lua_State* L)
{
	RuPropertyType propertyValue;
	INT32 detailLevel = 0;

	if( g_ruEngineSettings->GetSetting("terrain:splat technique", propertyValue) )
	{
		detailLevel = boost::get<INT32>(propertyValue);
	}

	lua_pushnumber( L, detailLevel );

	return 1;		
}

// ----------------------------------------------------------------------------
int LuaFunc_LightMapResSlider_GetValue(lua_State* L)
{
	RuPropertyType propertyValue;
	INT32 textureSize = 0;

	if( g_ruEngineSettings->GetSetting("RTLightMap:texture size", propertyValue) )
	{
		textureSize = boost::get<INT32>(propertyValue) / 128 - 1;
	}

	lua_pushnumber( L, textureSize );

	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_ShadowDetailSlider_GetValue(lua_State* L)
{
	RuPropertyType propertyValue;
	INT32 detailLevel = 0;

	if( g_ruEngineSettings->GetSetting("shadow:detail level", propertyValue) )
	{
		detailLevel = boost::get<INT32>(propertyValue);
	}

	lua_pushnumber( L, detailLevel );

	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_WaterReflectionCheckButton_IsChecked(lua_State* L)
{
	RuPropertyType propertyValue;
	BOOL isChecked = FALSE;

	if( g_ruEngineSettings->GetSetting("gfx:water reflection", propertyValue) )
	{
		isChecked = boost::get<BOOL>(propertyValue);
	}

	lua_pushboolean( L, isChecked );

	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_WaterRefractionCheckButton_IsChecked(lua_State* L)
{
	RuPropertyType propertyValue;
	BOOL isChecked = FALSE;

	if( g_ruEngineSettings->GetSetting("gfx:water refraction", propertyValue) )
	{
		isChecked = boost::get<BOOL>(propertyValue);
	}

	lua_pushboolean( L, isChecked );

	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_WaterQualitySlider_GetValue(lua_State* L)
{
	RuPropertyType propertyValue;
	INT32 quality = 0;

	if( g_ruEngineSettings->GetSetting("gfx:water quality", propertyValue) )
	{
		quality = boost::get<INT32>(propertyValue);
	}

	lua_pushnumber( L, quality );

	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_BloomCheckButton_IsChecked(lua_State* L)
{
	RuPropertyType propertyValue;
	BOOL isChecked = FALSE;

	if( g_ruEngineSettings->GetSetting("glowFX:enable", propertyValue) )
	{
		isChecked = boost::get<BOOL>(propertyValue);
	}

	lua_pushboolean( L, isChecked );

	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_GlowCheckButton_IsChecked(lua_State* L)
{
	RuPropertyType propertyValue;
	BOOL isChecked = FALSE;

	if( g_ruEngineSettings->GetSetting("glow:enable", propertyValue) )
	{
		isChecked = boost::get<BOOL>(propertyValue);
	}

	lua_pushboolean( L, isChecked );

	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_BloomQualitySlider_GetValue(lua_State* L)
{
	RuPropertyType propertyValue;
	INT32 quality = 0;

	if( g_ruEngineSettings->GetSetting("glow:quality", propertyValue) )
	{
		quality = boost::get<INT32>(propertyValue);
	}

	lua_pushnumber( L, quality );

	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_PaperdollDetailSlider_GetValue(lua_State* L)
{
	RuPropertyType propertyValue;
	INT32 maxMipLevel = 0;

	maxMipLevel = max(1, 6 - g_pGameMain->GetPaperdollTextureDetail());
	
	/*
	if( g_ruEngineSettings->GetSetting("paperdoll:max mip level", propertyValue) )
	{
		maxMipLevel = max(1, 6 - boost::get<INT32>(propertyValue));
	}
	*/

	lua_pushnumber( L, maxMipLevel );

	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_ViewDistanceSlider_GetValue(lua_State* L)
{
	RuPropertyType propertyValue;
	REAL viewDistance = 0.0f;

	if( g_ruEngineSettings->GetSetting("world:view distance", propertyValue) )
	{
		viewDistance = boost::get<REAL>(propertyValue) - 500.0f;
	}

	lua_pushnumber( L, viewDistance );

	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_WorldDetailSlider_GetValue(lua_State* L)
{
	RuPropertyType propertyValue;
	INT32 detailClass = 0;

	if( g_ruEngineSettings->GetSetting("terrain:detail class", propertyValue) )
	{
		detailClass = boost::get<INT32>(propertyValue);
	}

	lua_pushnumber( L, detailClass );

	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_SkyDetailSlider_GetValue(lua_State* L)
{
	RuPropertyType propertyValue;
	INT32 detailLevel = 0;

	if( g_ruEngineSettings->GetSetting("gfx:sky detail", propertyValue) )
	{
		detailLevel = boost::get<INT32>(propertyValue);
	}

	lua_pushnumber( L, detailLevel );

	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_TextureDetailSlider_GetValue(lua_State* L)
{
	RuPropertyType propertyValue;
	INT32 detailLevel = 0;

	detailLevel = g_pGameMain->GetTextureDetail();

	/*
	if( g_ruEngineSettings->GetSetting("texture:max detail level", propertyValue) )
	{
		detailLevel = boost::get<INT32>(propertyValue);
	}
	*/

	lua_pushnumber( L, detailLevel );

	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_TerrainTextureDetailSlider_GetValue(lua_State* L)
{
	RuPropertyType propertyValue;
	INT32 detailLevel = 0;

	if( g_ruEngineSettings->GetSetting("terrain:splat detail", propertyValue) )
	{
		detailLevel = boost::get<INT32>(propertyValue);
	}

	lua_pushnumber( L, detailLevel );

	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_SmoothBlendCheckButton_IsChecked(lua_State* L)
{
	RuPropertyType propertyValue;
	BOOL isChecked = FALSE;

	if( g_ruEngineSettings->GetSetting("smoothblend:enable", propertyValue) )
	{
		isChecked = boost::get<BOOL>(propertyValue);
	}

	lua_pushboolean( L, isChecked );

	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_DistortFXCheckButton_IsChecked(lua_State* L)
{
	RuPropertyType propertyValue;
	BOOL isChecked = FALSE;

	if( g_ruEngineSettings->GetSetting("distortFX:enable", propertyValue) )
	{
		isChecked = boost::get<BOOL>(propertyValue);
	}

	lua_pushboolean( L, isChecked );

	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_DebaseTextureCheckButton_IsChecked(lua_State* L)
{
	BOOL isChecked = g_pRuneDev->IsDebaseTexture();

	lua_pushboolean( L, isChecked );

	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_SpecularHighlightCheckButton_IsChecked(lua_State* L)
{
	RuPropertyType propertyValue;
	BOOL isChecked = FALSE;

	if( g_ruEngineSettings->GetSetting("gfx:specular highlight", propertyValue) )
	{
		isChecked = boost::get<BOOL>(propertyValue);
	}

	lua_pushboolean( L, isChecked );

	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_DisableMemoryWarningCheckButton_IsChecked(lua_State* L)
{
	BOOL isChecked = g_pRuneDev->IsDisableMemoryWarning();

	lua_pushboolean( L, isChecked );

	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_GetDisplayQuality(lua_State* L)
{
	RuPropertyType propertyValue;
	INT32 quality = 0;

	if( g_ruEngineSettings->GetSetting("display:quality", propertyValue) )
	{
		quality = boost::get<INT32>(propertyValue);
	}

	lua_pushnumber( L, quality );

	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_UIScaleCheckButton_SetChecked(lua_State* L)
{
	RuPropertyType propertyValue;
	bool isChecked = lua_toboolean( L, 1 ) == 1 ? true : false;

	if( g_ruEngineSettings->GetSetting("uiscale:enable", propertyValue) )
	{
		if( isChecked != (boost::get<BOOL>(propertyValue) == TRUE) )
		{
			g_ruEngineSettings->SetSetting("uiscale:enable", isChecked);
		}
	}
	else
	{
		g_ruEngineSettings->SetSetting("uiscale:enable", isChecked);
	}

	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_UIScaleSlider_SetValue(lua_State* L)
{
	RuPropertyType propertyValue;
	REAL newViewDistance = (float)luaL_checknumber( L, 1 );

	if(g_ruEngineSettings->GetSetting("ui:scale", propertyValue))
	{
		REAL oldViewDistance = boost::get<REAL>(propertyValue);

		if(fabs(newViewDistance - oldViewDistance) > 0.01f)
		{
			g_ruEngineSettings->SetSetting("ui:scale", newViewDistance);
		}
	}
	else
	{
		g_ruEngineSettings->SetSetting("ui:scale", newViewDistance);
	}

	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_RTLightMapCheckBox_SetChecked(lua_State* L)
{
	RuPropertyType propertyValue;
	bool isChecked = lua_toboolean( L, 1 ) == 1 ? true : false;

	return 0;
	if( g_ruEngineSettings->GetSetting("RTLightMap:enable", propertyValue) )
	{
		if( isChecked != (boost::get<BOOL>(propertyValue) == TRUE) )
		{
			g_ruEngineSettings->SetSetting("RTLightMap:enable", isChecked);
		}
	}
	else
	{
		g_ruEngineSettings->SetSetting("RTLightMap:enable", isChecked);
	}

	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_TerrainShaderDetailSlider_SetValue(lua_State* L)
{
	RuPropertyType propertyValue;
	INT32 newDetailLevel = luaL_checkint( L, 1 );

	if(g_ruEngineSettings->GetSetting("terrain:splat technique", propertyValue))
	{
		REAL oldDetailLevel = (REAL)boost::get<INT32>(propertyValue);

		if(newDetailLevel != oldDetailLevel)
		{
			g_ruEngineSettings->SetSetting("terrain:splat technique", newDetailLevel);
		}
	}
	else
	{
		g_ruEngineSettings->SetSetting("terrain:splat technique", newDetailLevel);
	}

	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_LightMapResSlider_SetValue(lua_State* L)
{
	RuPropertyType propertyValue;
	INT32 newTextureSize = luaL_checkint( L, 1 );

	if(g_ruEngineSettings->GetSetting("RTLightMap:texture size", propertyValue))
	{
		INT32 oldTextureSize = boost::get<INT32>(propertyValue) / 128 - 1;

		if(newTextureSize != oldTextureSize)
		{
			g_ruEngineSettings->SetSetting("RTLightMap:texture size", (newTextureSize + 1) * 128);
		}
	}
	else
	{
		g_ruEngineSettings->SetSetting("RTLightMap:texture size", (newTextureSize + 1) * 128);
	}

	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_ShadowDetailSlider_SetValue(lua_State* L)
{
	RuPropertyType propertyValue;
	INT32 newDetailLevel = luaL_checkint( L, 1 );

	if(g_ruEngineSettings->GetSetting("shadow:detail level", propertyValue))
	{
		INT32 oldDetailLevel = boost::get<INT32>(propertyValue);

		if(newDetailLevel != oldDetailLevel)
		{
			g_ruEngineSettings->SetSetting("shadow:detail level", newDetailLevel);
		}
	}
	else
	{
		g_ruEngineSettings->SetSetting("shadow:detail level", newDetailLevel);
	}

	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_WaterReflectionCheckButton_SetChecked(lua_State* L)
{
	RuPropertyType propertyValue;
	bool isChecked = lua_toboolean( L, 1 ) == 1 ? true : false;

	if( g_ruEngineSettings->GetSetting("gfx:water reflection", propertyValue) )
	{
		if( isChecked != (boost::get<BOOL>(propertyValue) == TRUE) )
		{
			g_ruEngineSettings->SetSetting("gfx:water reflection", isChecked);
		}
	}
	else
	{
		g_ruEngineSettings->SetSetting("gfx:water reflection", isChecked);
	}

	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_WaterRefractionCheckButton_SetChecked(lua_State* L)
{
	RuPropertyType propertyValue;
	bool isChecked = lua_toboolean( L, 1 ) == 1 ? true : false;

	if( g_ruEngineSettings->GetSetting("gfx:water refraction", propertyValue) )
	{
		if( isChecked != (boost::get<BOOL>(propertyValue) == TRUE) )
		{
			g_ruEngineSettings->SetSetting("gfx:water refraction", isChecked);
		}
	}
	else
	{
		g_ruEngineSettings->SetSetting("gfx:water refraction", isChecked);
	}

	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_WaterQualitySlider_SetValue(lua_State* L)
{
	RuPropertyType propertyValue;
	INT32 newQuality = luaL_checkint( L, 1 );

	if(g_ruEngineSettings->GetSetting("gfx:water quality", propertyValue))
	{
		INT32 oldQuality = boost::get<INT32>(propertyValue);

		if(newQuality != oldQuality)
		{
			g_ruEngineSettings->SetSetting("gfx:water quality", newQuality);
		}
	}
	else
	{
		g_ruEngineSettings->SetSetting("gfx:water quality", newQuality);
	}

	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_BloomCheckButton_SetChecked(lua_State* L)
{
	RuPropertyType propertyValue;
	bool isChecked = lua_toboolean( L, 1 ) == 1 ? true : false;

	if( g_ruEngineSettings->GetSetting("glowFX:enable", propertyValue) )
	{
		if( isChecked != (boost::get<BOOL>(propertyValue) == TRUE) )
		{
			g_ruEngineSettings->SetSetting("glowFX:enable", isChecked);
		}
	}
	else
	{
		g_ruEngineSettings->SetSetting("glowFX:enable", isChecked);
	}

	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GlowCheckButton_SetChecked(lua_State* L)
{
	RuPropertyType propertyValue;
	bool isChecked = lua_toboolean( L, 1 ) == 1 ? true : false;

	if( g_ruEngineSettings->GetSetting("glow:enable", propertyValue) )
	{
		if( isChecked != (boost::get<BOOL>(propertyValue) == TRUE) )
		{
			g_ruEngineSettings->SetSetting("glow:enable", isChecked);
		}
	}
	else
	{
		g_ruEngineSettings->SetSetting("glow:enable", isChecked);
	}

	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_BloomQualitySlider_SetValue(lua_State* L)
{
	RuPropertyType propertyValue;
	INT32 newQuality = luaL_checkint( L, 1 );

	if(g_ruEngineSettings->GetSetting("glow:quality", propertyValue))
	{
		INT32 oldQuality = boost::get<INT32>(propertyValue);

		if(newQuality != oldQuality)
		{
			g_ruEngineSettings->SetSetting("glow:quality", newQuality);
		}
	}
	else
	{
		g_ruEngineSettings->SetSetting("glow:quality", newQuality);
	}

	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_PaperdollDetailSlider_SetValue(lua_State* L)
{
	RuPropertyType propertyValue;
	INT32 newDetailLevel = luaL_checkint( L, 1 );

	g_pGameMain->SetPaperdollTextureDetail(6 - newDetailLevel);

	/*
	if(g_ruEngineSettings->GetSetting("paperdoll:max mip level", propertyValue))
	{
		INT32 oldDetailLevel = max(1, 6 - boost::get<INT32>(propertyValue));

		if(newDetailLevel != oldDetailLevel)
		{
			g_ruEngineSettings->SetSetting("paperdoll:max mip level", 6 - newDetailLevel);
		}
	}
	else
	{
		g_ruEngineSettings->SetSetting("paperdoll:max mip level", 6 - newDetailLevel);
	}
	*/

	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_ViewDistanceSlider_SetValue(lua_State* L)
{
	RuPropertyType propertyValue;
	REAL newViewDistance = (float)luaL_checknumber( L, 1 ) + 500.0f;

	if(g_ruEngineSettings->GetSetting("world:view distance", propertyValue))
	{
		REAL oldViewDistance = boost::get<REAL>(propertyValue);

		if(fabs(newViewDistance - oldViewDistance) > 1.0f)
		{
			g_ruEngineSettings->SetSetting("world:view distance", newViewDistance);
		}
	}
	else
	{
		g_ruEngineSettings->SetSetting("world:view distance", newViewDistance);
	}

	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_WorldDetailSlider_SetValue(lua_State* L)
{
	RuPropertyType propertyValue;
	INT32 newDetailClass = luaL_checkint( L, 1 );

	if(g_ruEngineSettings->GetSetting("terrain:detail class", propertyValue))
	{
		INT32 oldDetailClass = boost::get<INT32>(propertyValue);

		if(newDetailClass != oldDetailClass)
		{
			g_ruEngineSettings->SetSetting("terrain:detail class", newDetailClass);
		}
	}
	else
	{
		g_ruEngineSettings->SetSetting("terrain:detail class", newDetailClass);
	}

	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_SkyDetailSlider_SetValue(lua_State* L)
{
	RuPropertyType propertyValue;
	INT32 newDetailLevel = luaL_checkint( L, 1 );

	if(g_ruEngineSettings->GetSetting("gfx:sky detail", propertyValue))
	{
		INT32 oldDetailLevel = boost::get<INT32>(propertyValue);

		if(newDetailLevel != oldDetailLevel)
		{
			g_ruEngineSettings->SetSetting("gfx:sky detail", newDetailLevel);
		}
	}
	else
	{
		g_ruEngineSettings->SetSetting("gfx:sky detail", newDetailLevel);
	}

	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_TextureDetailSlider_SetValue(lua_State* L)
{
	RuPropertyType propertyValue;
	INT32 newDetailLevel = luaL_checkint( L, 1 );

	g_pGameMain->SetTextureDetail(newDetailLevel);

	/*
	if(g_ruEngineSettings->GetSetting("texture:max detail level", propertyValue))
	{
		INT32 oldDetailLevel = boost::get<INT32>(propertyValue);

		if(newDetailLevel != oldDetailLevel)
		{
			g_ruEngineSettings->SetSetting("texture:max detail level", newDetailLevel);
		}
	}
	else
	{
		g_ruEngineSettings->SetSetting("texture:max detail level", newDetailLevel);
	}
	*/

	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_TerrainTextureDetailSlider_SetValue(lua_State* L)
{
	RuPropertyType propertyValue;
	INT32 newDetailLevel = luaL_checkint( L, 1 );

	if(g_ruEngineSettings->GetSetting("terrain:splat detail", propertyValue))
	{
		INT32 oldDetailLevel = boost::get<INT32>(propertyValue);

		if(newDetailLevel != oldDetailLevel)
		{
			g_ruEngineSettings->SetSetting("terrain:splat detail", newDetailLevel);
		}
	}
	else
	{
		g_ruEngineSettings->SetSetting("terrain:splat detail", newDetailLevel);
	}

	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_SmoothBlendCheckButton_SetChecked(lua_State* L)
{
	RuPropertyType propertyValue;
	bool isChecked = lua_toboolean( L, 1 ) == 1 ? true : false;

	if( g_ruEngineSettings->GetSetting("smoothblend:enable", propertyValue) )
	{
		if( isChecked != (boost::get<BOOL>(propertyValue) == TRUE) )
		{
			g_ruEngineSettings->SetSetting("smoothblend:enable", isChecked);
		}
	}
	else
	{
		g_ruEngineSettings->SetSetting("smoothblend:enable", isChecked);
	}

	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_DistortFXCheckButton_SetChecked(lua_State* L)
{
	RuPropertyType propertyValue;
	bool isChecked = lua_toboolean( L, 1 ) == 1 ? true : false;

	if( g_ruEngineSettings->GetSetting("distortFX:enable", propertyValue) )
	{
		if( isChecked != (boost::get<BOOL>(propertyValue) == TRUE) )
		{
			g_ruEngineSettings->SetSetting("distortFX:enable", isChecked);
		}
	}
	else
	{
		g_ruEngineSettings->SetSetting("distortFX:enable", isChecked);
	}

	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_DebaseTextureCheckButton_SetChecked(lua_State* L)
{
	bool isChecked = lua_toboolean( L, 1 ) == 1 ? true : false;

	g_pRuneDev->SetDebaseTexture( isChecked );

	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_SpecularHighlightCheckButton_SetChecked(lua_State* L)
{
	RuPropertyType propertyValue;
	bool isChecked = lua_toboolean( L, 1 ) == 1 ? true : false;

	if( g_ruEngineSettings->GetSetting("gfx:specular highlight", propertyValue) )
	{
		if( isChecked != (boost::get<BOOL>(propertyValue) == TRUE) )
		{
			g_ruEngineSettings->SetSetting("gfx:specular highlight", isChecked);
		}
	}
	else
	{
		g_ruEngineSettings->SetSetting("gfx:specular highlight", isChecked);
	}

	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_DisableMemoryWarningCheckButton_SetChecked(lua_State* L)
{
	bool isChecked = lua_toboolean( L, 1 ) == 1 ? true : false;

	g_pRuneDev->SetDisableMemoryWarning( isChecked );

	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_SetDisplayQuality(lua_State* L)
{
	RuPropertyType propertyValue;
	INT32 newQuality = luaL_checkint( L, 1 );

	if(g_ruEngineSettings->GetSetting("display:quality", propertyValue))
	{
		INT32 oldQuality = boost::get<INT32>(propertyValue);

		if(newQuality != oldQuality)
		{
			g_ruEngineSettings->SetSetting("display:quality", newQuality);
		}
	}
	else
	{
		g_ruEngineSettings->SetSetting("display:quality", newQuality);
	}

	return 0;
}

// ************************************************************************************************************************************************************
