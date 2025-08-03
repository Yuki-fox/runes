#include "../../mainproc/GameMain.h"
#include "../../mainproc/RuneDev.h"

#include "GM_Config.h"

// ************************************************************************************************************************************************************

int LuaFunc_GM_Config_OnLoad(lua_State* L);
int LuaFunc_GM_Config_OnUpdate(lua_State* L);

int LuaFunc_GM_Config_OnApply(lua_State* L);
int LuaFunc_GM_Config_OnClose(lua_State* L);

// ************************************************************************************************************************************************************

CGM_Config*				g_pGM_Config = NULL;

// ************************************************************************************************************************************************************

// ----------------------------------------------------------------------------
CGM_Config::CGM_Config(CInterface* object)
:	CInterfaceBase(object),
	m_UI_Win(NULL),
	m_visibleOnLastUpdate(FALSE)
{
	g_pGM_Config = this;
}

// ----------------------------------------------------------------------------
CGM_Config::~CGM_Config()
{
	g_pGM_Config = NULL;
}

// ----------------------------------------------------------------------------
void CGM_Config::RegisterFunc()
{
	lua_State* L = GetLuaStateWorld();
	if (L)
	{
		lua_register(L, "GM_Config_OnLoad",				LuaFunc_GM_Config_OnLoad);
		lua_register(L, "GM_Config_OnUpdate",			LuaFunc_GM_Config_OnUpdate);

		lua_register(L, "GM_Config_OnApply",			LuaFunc_GM_Config_OnApply);
		lua_register(L, "GM_Config_Close",				LuaFunc_GM_Config_OnClose);
	}
}

// ----------------------------------------------------------------------------
bool CGM_Config::IsVisible()
{
	return m_UI_Win->IsVisible();
}

// ----------------------------------------------------------------------------
void CGM_Config::OnLoad()
{
	m_UI_Win = static_cast<CUiFrame *>(m_interface->GetUiWorld()->FindObject( "GM_Config_Frame"));

	m_UI_ShowHiddenCheck = static_cast<CUiCheckButton *>(m_interface->GetUiWorld()->FindObject( "GM_Config_ShowHiddenCheckButton"));
}

// ----------------------------------------------------------------------------
void CGM_Config::OnUpdate()
{
	if(m_UI_Win->IsVisible() && !m_visibleOnLastUpdate)
	{
		RuPropertyType propertyValue;

		if(g_ruEngineSettings->GetSetting("gm:show hidden", propertyValue))
		{
			m_UI_ShowHiddenCheck->SetChecked(boost::get<BOOL>(propertyValue) == TRUE);
		}
	}

	m_visibleOnLastUpdate = m_UI_Win->IsVisible();
}

// ----------------------------------------------------------------------------
void CGM_Config::OnApply()
{
	RuPropertyType propertyValue;

	if(g_ruEngineSettings->GetSetting("gm:show hidden", propertyValue))
	{
		if(m_UI_ShowHiddenCheck->IsChecked() != (boost::get<BOOL>(propertyValue) == TRUE))
		{
			g_ruEngineSettings->SetSetting("gm:show hidden", m_UI_ShowHiddenCheck->IsChecked() ? TRUE : FALSE);
		}
	}
	else
	{
		g_ruEngineSettings->SetSetting("gm:show hidden", m_UI_ShowHiddenCheck->IsChecked() ? TRUE : FALSE);
	}
}

// ----------------------------------------------------------------------------
void CGM_Config::OnClose()
{
}

// ************************************************************************************************************************************************************

// ----------------------------------------------------------------------------
int LuaFunc_GM_Config_OnLoad(lua_State* L)
{
	g_pGM_Config->OnLoad();
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GM_Config_OnUpdate(lua_State* L)
{
	g_pGM_Config->OnUpdate();
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GM_Config_OnApply(lua_State* L)
{
	g_pGM_Config->OnApply();
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GM_Config_OnClose(lua_State* L)
{
	g_pGM_Config->OnClose();
	return 0;
}

// ************************************************************************************************************************************************************
