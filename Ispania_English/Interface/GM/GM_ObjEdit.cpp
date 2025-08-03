#include "../../mainproc/GameMain.h"
#include "../../mainproc/RuneDev.h"

#include "../../netwaker_member/NetWakerClientInc.h"

#include "../WorldFrames/ChatEdit.h"
#include "../WorldFrames/ChatFrame.h"
#include "../WorldFrames/MacroFrame.h"

#include "GM_ObjEdit.h"

// ************************************************************************************************************************************************************

int LuaFunc_GM_ObjEdit_OnLoad(lua_State* L);
int LuaFunc_GM_ObjEdit_OnShow(lua_State* L);
int LuaFunc_GM_ObjEdit_OnUpdate(lua_State* L);
int LuaFunc_GM_ObjEdit_OnClose(lua_State* L);

int LuaFunc_GM_ObjEdit_OnClick(lua_State* L);
int LuaFunc_GM_ObjEdit_OnDragStart(lua_State* L);
int LuaFunc_GM_ObjEdit_OnReceiveDrag(lua_State* L);
int LuaFunc_GM_ObjEdit_OnDragStop(lua_State* L);

int LuaFunc_GM_ObjEdit_OnToggleModeClick(lua_State* L);

int LuaFunc_GM_ObjEdit_OnValueChanged(lua_State* L);

int LuaFunc_GM_ObjEdit_OnResetClick(lua_State* L);
int LuaFunc_GM_ObjEdit_OnCreateMacroClick(lua_State* L);

int LuaFunc_GM_ObjEdit_OnRotateLeftClick(lua_State* L);
int LuaFunc_GM_ObjEdit_OnRotateRightClick(lua_State* L);

int LuaFunc_GM_ObjEdit_OnAppendWaypointClick(lua_State* L);
int LuaFunc_GM_ObjEdit_OnDeleteWaypointClick(lua_State* L);
int LuaFunc_GM_ObjEdit_OnDeleteWaypointConfirm(lua_State* L);

int LuaFunc_GM_ObjEdit_OnCreateClick(lua_State* L);
int LuaFunc_GM_ObjEdit_OnDeleteClick(lua_State* L);
int LuaFunc_GM_ObjEdit_OnDeleteConfirm(lua_State* L);
int LuaFunc_GM_ObjEdit_OnApplyClick(lua_State* L);

int LuaFunc_GM_ObjEdit_Translate(lua_State* L);

int LuaFunc_GM_ObjEdit_MGE_OnAppendClick(lua_State* L);
int LuaFunc_GM_ObjEdit_MGE_OnDeleteClick(lua_State* L);
int LuaFunc_GM_ObjEdit_MGE_OnDeleteConfirm(lua_State* L);
int LuaFunc_GM_ObjEdit_MGE_OnNextClick(lua_State* L);
int LuaFunc_GM_ObjEdit_MGE_OnPreviousClick(lua_State* L);
int LuaFunc_GM_ObjEdit_MGE_OnReloadClick(lua_State* L);
int LuaFunc_GM_ObjEdit_MGE_OnSelectionChanged(lua_State* L);

int LuaFunc_GM_ObjEdit_ME_OnAppendClick(lua_State* L);
int LuaFunc_GM_ObjEdit_ME_OnDeleteClick(lua_State* L);
int LuaFunc_GM_ObjEdit_ME_OnDeleteConfirm(lua_State* L);
int LuaFunc_GM_ObjEdit_ME_OnGotoClick(lua_State* L);
int LuaFunc_GM_ObjEdit_ME_OnApplyClick(lua_State* L);
int LuaFunc_GM_ObjEdit_ME_OnNextClick(lua_State* L);
int LuaFunc_GM_ObjEdit_ME_OnPreviousClick(lua_State* L);
int LuaFunc_GM_ObjEdit_ME_OnSelectionChanged(lua_State* L);

// ************************************************************************************************************************************************************

CGM_ObjEdit*				g_pGM_ObjEdit = NULL;

// ************************************************************************************************************************************************************

struct MarkerGroup
{
	INT32							m_markerGroupID;
	char*							m_markerGroupName;
	CRuArrayList<CliFlagPosInfo>	m_markers;
};

// ************************************************************************************************************************************************************

// ----------------------------------------------------------------------------
CGM_ObjEdit::CGM_ObjEdit(CInterface* object)
:	CInterfaceBase(object),
	m_UI_Win(NULL),
	m_refreshingInterface(FALSE),
	m_objEditMode(OBJEDITMODE_NPC),
	m_spriteHasPrecedence(TRUE),
	m_dragging(FALSE),
	m_lastOrgID(0),
	m_currentCreateCommand(NULL),
	m_currentDeleteCommand(NULL),
	m_selectedSprite(NULL),
	m_selectedSpriteModifyInfo(NULL),
	m_selectedSpriteMoveInfo(NULL),
	m_MGE_Top(0),
	m_MGE_SelectedGroup(-1),
	m_ME_Top(0),
	m_ME_SelectedMarker(-1),
	m_ME_SelectedMarkerEntity(NULL),
	m_dragJustFinished(FALSE)
{
	g_pGM_ObjEdit = this;

	m_moverGizmo = ruNEW CRuGizmo_Mover();
}

// ----------------------------------------------------------------------------
CGM_ObjEdit::~CGM_ObjEdit()
{
	delete[] m_currentCreateCommand;
	delete[] m_currentDeleteCommand;

	delete reinterpret_cast<ModifyNPCInfo *>(m_selectedSpriteModifyInfo);
	delete reinterpret_cast<CRuArrayList<NPC_MoveBaseStruct> *>(m_selectedSpriteMoveInfo);

	g_pGM_ObjEdit = NULL;

	ruSAFE_RELEASE(m_moverGizmo);

	MGE_ClearMarkers();
}

// ----------------------------------------------------------------------------
void CGM_ObjEdit::RegisterFunc()
{
	lua_State* L = GetLuaStateWorld();
	if (L)
	{
		lua_register(L, "GM_ObjEdit_OnLoad",					LuaFunc_GM_ObjEdit_OnLoad);
		lua_register(L, "GM_ObjEdit_OnShow",					LuaFunc_GM_ObjEdit_OnShow);
		lua_register(L, "GM_ObjEdit_OnUpdate",					LuaFunc_GM_ObjEdit_OnUpdate);
		lua_register(L, "GM_ObjEdit_OnClose",					LuaFunc_GM_ObjEdit_OnClose);

		lua_register(L, "GM_ObjEdit_OnClick",					LuaFunc_GM_ObjEdit_OnClick);
		lua_register(L, "GM_ObjEdit_OnDragStart",				LuaFunc_GM_ObjEdit_OnDragStart);
		lua_register(L, "GM_ObjEdit_OnReceiveDrag",				LuaFunc_GM_ObjEdit_OnReceiveDrag);
		lua_register(L, "GM_ObjEdit_OnDragStop",				LuaFunc_GM_ObjEdit_OnDragStop);

		lua_register(L, "GM_ObjEdit_OnToggleModeClick",			LuaFunc_GM_ObjEdit_OnToggleModeClick);

		lua_register(L, "GM_ObjEdit_OnValueChanged",			LuaFunc_GM_ObjEdit_OnValueChanged);

		lua_register(L, "GM_ObjEdit_OnResetClick",				LuaFunc_GM_ObjEdit_OnResetClick);
		lua_register(L, "GM_ObjEdit_OnCreateMacroClick",		LuaFunc_GM_ObjEdit_OnCreateMacroClick);
		

		lua_register(L, "GM_ObjEdit_OnRotateLeftClick",			LuaFunc_GM_ObjEdit_OnRotateLeftClick);
		lua_register(L, "GM_ObjEdit_OnRotateRightClick",		LuaFunc_GM_ObjEdit_OnRotateRightClick);

		lua_register(L, "GM_ObjEdit_OnAppendWaypointClick",		LuaFunc_GM_ObjEdit_OnAppendWaypointClick);
		lua_register(L, "GM_ObjEdit_OnDeleteWaypointClick",		LuaFunc_GM_ObjEdit_OnDeleteWaypointClick);
		lua_register(L, "GM_ObjEdit_OnDeleteWaypointConfirm",	LuaFunc_GM_ObjEdit_OnDeleteWaypointConfirm);

		lua_register(L, "GM_ObjEdit_OnCreateClick",				LuaFunc_GM_ObjEdit_OnCreateClick);
		lua_register(L, "GM_ObjEdit_OnDeleteClick",				LuaFunc_GM_ObjEdit_OnDeleteClick);
		lua_register(L, "GM_ObjEdit_OnDeleteConfirm",			LuaFunc_GM_ObjEdit_OnDeleteConfirm);
		lua_register(L, "GM_ObjEdit_OnApplyClick",				LuaFunc_GM_ObjEdit_OnApplyClick);

		lua_register(L, "GM_ObjEdit_Translate",					LuaFunc_GM_ObjEdit_Translate);

		lua_register(L, "GM_ObjEdit_MGE_OnAppendClick",			LuaFunc_GM_ObjEdit_MGE_OnAppendClick);
		lua_register(L, "GM_ObjEdit_MGE_OnDeleteClick",			LuaFunc_GM_ObjEdit_MGE_OnDeleteClick);
		lua_register(L, "GM_ObjEdit_MGE_OnDeleteConfirm",		LuaFunc_GM_ObjEdit_MGE_OnDeleteConfirm);
		lua_register(L, "GM_ObjEdit_MGE_OnNextClick",			LuaFunc_GM_ObjEdit_MGE_OnNextClick);
		lua_register(L, "GM_ObjEdit_MGE_OnPreviousClick",		LuaFunc_GM_ObjEdit_MGE_OnPreviousClick);
		lua_register(L, "GM_ObjEdit_MGE_OnReloadClick",			LuaFunc_GM_ObjEdit_MGE_OnReloadClick);
		lua_register(L, "GM_ObjEdit_MGE_OnSelectionChanged",	LuaFunc_GM_ObjEdit_MGE_OnSelectionChanged);

		lua_register(L, "GM_ObjEdit_ME_OnAppendClick",			LuaFunc_GM_ObjEdit_ME_OnAppendClick);
		lua_register(L, "GM_ObjEdit_ME_OnDeleteClick",			LuaFunc_GM_ObjEdit_ME_OnDeleteClick);
		lua_register(L, "GM_ObjEdit_ME_OnDeleteConfirm",		LuaFunc_GM_ObjEdit_ME_OnDeleteConfirm);
		lua_register(L, "GM_ObjEdit_ME_OnGotoClick",			LuaFunc_GM_ObjEdit_ME_OnGotoClick);
		lua_register(L, "GM_ObjEdit_ME_OnApplyClick",			LuaFunc_GM_ObjEdit_ME_OnApplyClick);
		lua_register(L, "GM_ObjEdit_ME_OnNextClick",			LuaFunc_GM_ObjEdit_ME_OnNextClick);
		lua_register(L, "GM_ObjEdit_ME_OnPreviousClick",		LuaFunc_GM_ObjEdit_ME_OnPreviousClick);
		lua_register(L, "GM_ObjEdit_ME_OnSelectionChanged",		LuaFunc_GM_ObjEdit_ME_OnSelectionChanged);
	}
}

// ----------------------------------------------------------------------------
void CGM_ObjEdit::Update(float elapsedTime)
{
	// 檢查物件是否還存在
	if ( m_selectedSprite && !IBaseSprite::CheckedSpriteIsLife(m_selectedSprite) ) {
		m_selectedSprite = NULL;
		ResetEditorFields();
	}	
}

// ----------------------------------------------------------------------------
void CGM_ObjEdit::UpdateModifyNPCInfo(void *info)
{
	delete reinterpret_cast<ModifyNPCInfo *>(m_selectedSpriteModifyInfo);
	m_selectedSpriteModifyInfo = ruNEW ModifyNPCInfo;
	*reinterpret_cast<ModifyNPCInfo *>(m_selectedSpriteModifyInfo) = *reinterpret_cast<ModifyNPCInfo *>(info);

	m_UI_ObjID->SetTextA(IntToStr(reinterpret_cast<ModifyNPCInfo *>(m_selectedSpriteModifyInfo)->OrgObjID));
	m_UI_OrgName->SetTextA("");
	m_UI_GItemID->SetTextA(IntToStr(reinterpret_cast<ModifyNPCInfo *>(m_selectedSpriteModifyInfo)->GItemID));
	m_UI_Name->SetTextA(reinterpret_cast<ModifyNPCInfo *>(m_selectedSpriteModifyInfo)->Name.Begin());
	m_UI_ObjClass->SetTextA(reinterpret_cast<ModifyNPCInfo *>(m_selectedSpriteModifyInfo)->ClassName.Begin());
	m_UI_PID->SetTextA(IntToStr(reinterpret_cast<ModifyNPCInfo *>(m_selectedSpriteModifyInfo)->PID) );
	m_UI_AutoPlot->SetTextA(reinterpret_cast<ModifyNPCInfo *>(m_selectedSpriteModifyInfo)->AutoPlot.Begin());
	m_UI_QuestID->SetTextA(IntToStr(reinterpret_cast<ModifyNPCInfo *>(m_selectedSpriteModifyInfo)->QuestID) );

	for(INT32 i = 0; i < 32; ++i)
	{
		if(m_UI_CheckMode[i] == NULL)
			continue;

		if(TestBit(reinterpret_cast<ModifyNPCInfo *>(m_selectedSpriteModifyInfo)->Mode , i) == false)
			m_UI_CheckMode[i]->SetChecked(false);
		else
			m_UI_CheckMode[i]->SetChecked(true);
	}
}

// ----------------------------------------------------------------------------
void CGM_ObjEdit::UpdateModifyNPCInfoFromInterface()
{
	ModifyNPCInfo *info = reinterpret_cast<ModifyNPCInfo *>(m_selectedSpriteModifyInfo);

	// Modify base NPC data
	info->X = (m_selectedSprite->GetPos().m_x);
	info->Y = (m_selectedSprite->GetPos().m_y);
	info->Z = (m_selectedSprite->GetPos().m_z);

	CRuVector3 dir = m_selectedSprite->GetDirection();
	float theta = acos( dir.Dot( CRuVector3(1.0f, 0.0f, 0.0f) ) );
	if ( dir.m_z > 0.0f )
		theta = PI + PI - theta;
	theta = theta * 180.0f / PI;

	info->Dir = theta;//static_cast<INT32>(theta);

	info->vX = m_selectedSprite->GetOrientingAxis().m_x;
	info->vY = m_selectedSprite->GetOrientingAxis().m_y;
	info->vZ = m_selectedSprite->GetOrientingAxis().m_z;

	info->OrgObjID = _wtol(m_UI_ObjID->GetText());
	info->QuestID = _wtol(m_UI_QuestID->GetText());
	info->PID = _wtol(m_UI_PID->GetText());

	char *name		= WCharToUFT8(m_UI_Name->GetText());
	char *autoPlot	= WCharToUFT8(m_UI_AutoPlot->GetText());
	char *className	= WCharToUFT8(m_UI_ObjClass->GetText());

	info->Name = name;
	/*
	if(name && name[0])
		info->Name = name;
	else
	{
		char *tmp = WCharToUFT8(m_UI_OrgName->GetText());
		info->Name = tmp;
		delete [] tmp;
	}
	*/
	info->AutoPlot	= autoPlot;
	info->ClassName	= className;
	info->Mode		= 0;

	for( int i = 0 ; i < 32 ; i++ )
	{
		if(m_UI_CheckMode[i] == NULL)
			continue;
		if(m_UI_CheckMode[i]->IsChecked() == false)
			continue;

		SetBit(info->Mode, i);
	}
	if (name) delete [] name;
	if (autoPlot) delete [] autoPlot;
	if (className) delete [] className;
}

// ----------------------------------------------------------------------------
void CGM_ObjEdit::UpdateNPCMoveInfo(INT32 objID, vector<NPC_MoveBaseStruct> &moveList)
{
	// Copy the move structure list
	delete reinterpret_cast<CRuArrayList<NPC_MoveBaseStruct> *>(m_selectedSpriteMoveInfo);
	m_selectedSpriteMoveInfo = ruNEW CRuArrayList<NPC_MoveBaseStruct>((INT32)moveList.size());

	vector<NPC_MoveBaseStruct>::iterator it;
	for(it = moveList.begin(); it != moveList.end(); it++)
	{
		reinterpret_cast<CRuArrayList<NPC_MoveBaseStruct> *>(m_selectedSpriteMoveInfo)->Add(*it);
	}

	// Refresh interface
	RefreshNPCMoveInfoInterface();
}

// ----------------------------------------------------------------------------
void CGM_ObjEdit::UpdateNPCMoveInfoFromInterface()
{
	if(m_refreshingInterface)
		return;

	CRuArrayList<NPC_MoveBaseStruct> &waypointList = *reinterpret_cast<CRuArrayList<NPC_MoveBaseStruct> *>(m_selectedSpriteMoveInfo);

	// Save npc move info
	for(INT32 i = 0; i < 64; ++i)
	{
		if(m_UI_Waypoint_Frame[i] == NULL || !m_UI_Waypoint_Frame[i]->IsVisible())
			break;

		if(i >= waypointList.Count())
			break;

		NPC_MoveBaseStruct &moveStruct = waypointList[i];

		char *tmp = WCharToUFT8(m_UI_Waypoint_Range[i]->GetText());
		if(tmp)
		{
			moveStruct.Range = static_cast<REAL>(atof(tmp));
			delete[] tmp;
			tmp = NULL;
		}

		tmp = TransToChar(m_UI_Waypoint_WaitBase[i]->GetText());
		if(tmp)
		{
			moveStruct.WaitTime_Base = atoi(tmp);
			delete[] tmp;
			tmp = NULL;
		}

		tmp = TransToChar(m_UI_Waypoint_WaitRand[i]->GetText());
		if(tmp)
		{
			moveStruct.WaitTime_Rand = atoi(tmp);
			delete[] tmp;
			tmp = NULL;
		}

		tmp = TransToChar(m_UI_Waypoint_ActionType[i]->GetText());
		if(tmp)
		{
			moveStruct.ActionType = atoi(tmp);
			delete[] tmp;
			tmp = NULL;
		}

		tmp = TransToChar(m_UI_Waypoint_Script[i]->GetText());
		if(tmp)
		{
			moveStruct.sLuaFunc = tmp;
			delete[] tmp;
			tmp = NULL;
		}

		lua_State* L = this->GetLuaState();
		if (L)
		{
			// Get selected ID for the move type drop down
			lua_getglobal(L, "UIDropDownMenu_GetSelectedID");
			lua_getglobal(L, m_UI_Waypoint_MoveType[i]->GetName());
			lua_pcall(L, 1, 1, 0);
			moveStruct.MoveType = static_cast<NPC_MoveTypeENUM>(luaL_checkint(L, -1) - 1);

			// Get selected ID for the action type drop down
			lua_getglobal(L, "UIDropDownMenu_GetSelectedID");
			lua_getglobal(L, m_UI_Waypoint_DirMode[i]->GetName());
			lua_pcall(L, 1, 1, 0);
			moveStruct.DirMode = static_cast<NPC_DirModeEnum>(luaL_checkint(L, -1) - 1);
		}

	}
}

// ----------------------------------------------------------------------------
void CGM_ObjEdit::UpdateMarkerList(int gItemID, void *vecFlagList)
{
	// Clear existing marker list
	MGE_ClearMarkers();

	// Re-cast reference to flag list vector
	vector< vector < CliFlagPosInfo > > &vecFlagGroups = *reinterpret_cast<vector< vector < CliFlagPosInfo > > *>(vecFlagList);

	// Define iterator
	vector< vector < CliFlagPosInfo > >::iterator it;

	for(it = vecFlagGroups.begin(); it != vecFlagGroups.end(); it++)
	{
		vector<CliFlagPosInfo> &vecFlags = (*it);

		if(vecFlags.size() > 0)
		{
			MarkerGroup *markerGroup = NULL;

			for(UINT32 i = 0, groupIDFound = 0; i < vecFlags.size(); ++i)
			{
				// Skip invalid/uninitialized flags
				if(vecFlags[i].GroupID < Def_ObjectClass_Flag)
					continue;

				// If we have not yet obtained a valid flag group ID, try to do that now
				if(groupIDFound == 0)
				{
					// Get DB descriptor for the flag group
					GameObjDbStructEx *dbStruct = g_ObjectData->GetObj(vecFlags[i].GroupID);

					if(dbStruct)
					{
						markerGroup = ruNEW MarkerGroup();

						// Set flag group ID
						markerGroup->m_markerGroupID = vecFlags[i].GroupID;

						// Set flag group name
						markerGroup->m_markerGroupName = ruNEW char [strlen(dbStruct->GetName()) + 1];
						strcpy(markerGroup->m_markerGroupName, dbStruct->GetName() );
					}
					else
					{
						// Cannot locate the flag group in the DB, break
						break;
					}

					groupIDFound = 1;
				}

				// Add flag to marker list
				markerGroup->m_markers.Add(vecFlags[i]);
			}

			// If the marker group is valid, add it to the list of marker groups
			if(markerGroup)
			{
				m_markerGroups.Add(markerGroup);
			}
		}
	}

	// Refresh marker group interface
	RefreshMarkerGroupInterface();
}

// ----------------------------------------------------------------------------
void CGM_ObjEdit::RefreshNPCMoveInfoInterface()
{
	m_refreshingInterface = TRUE;

	CRuArrayList<NPC_MoveBaseStruct> &waypointList = *reinterpret_cast<CRuArrayList<NPC_MoveBaseStruct> *>(m_selectedSpriteMoveInfo);

	char buffer[128];

	// Make sure the gizmo is detached
	m_moverGizmo->DetachFromParent();

	// Clear all flags
	g_pRuneDev->FlagEditor_ClearAll();

	// Clear selected waypoint
	m_selectedWaypointIdx = 0;
	m_selectedWaypoint = NULL;

	for(INT32 i = 0; i < 64; ++i)
	{
		if(m_UI_Waypoint_Range[i] == NULL)
			continue;

		if(i > waypointList.Count())
		{
			m_UI_Waypoint_Frame[i]->SetVisible(false);
			continue;
		}

		m_UI_Waypoint_Frame[i]->SetVisible(true);

		if(i == waypointList.Count())
		{
			m_UI_Waypoint_Range[i]->SetTextA("");
			m_UI_Waypoint_WaitBase[i]->SetTextA("");
			m_UI_Waypoint_WaitRand[i]->SetTextA("");
			m_UI_Waypoint_ActionType[i]->SetTextA("");
			m_UI_Waypoint_Script[i]->SetTextA("");

			lua_State* L = this->GetLuaState();
			if (L)
			{
				// Initialize the move type drop down
				lua_getglobal(L, "UIDropDownMenu_Initialize");
				lua_getglobal(L, m_UI_Waypoint_MoveType[i]->GetName());
				lua_getglobal(L, "GM_ObjEdit_Template_WaypointButton_MoveType_Show");
				lua_pcall(L, 2, 0, 0);

				// Set selected ID for the move type drop down
				lua_getglobal(L, "UIDropDownMenu_SetSelectedID");
				lua_getglobal(L, m_UI_Waypoint_MoveType[i]->GetName());
				lua_pushnumber(L, 1);
				lua_pcall(L, 2, 0, 0);

				// Initialize the action type drop down
				lua_getglobal(L, "UIDropDownMenu_Initialize");
				lua_getglobal(L, m_UI_Waypoint_DirMode[i]->GetName());
				lua_getglobal(L, "GM_ObjEdit_Template_WaypointButton_DirMode_Show");
				lua_pcall(L, 2, 0, 0);

				// Set selected ID for the action type drop down
				lua_getglobal(L, "UIDropDownMenu_SetSelectedID");
				lua_getglobal(L, m_UI_Waypoint_DirMode[i]->GetName());
				lua_pushnumber(L, 1);
				lua_pcall(L, 2, 0, 0);
			}

			m_UI_Waypoint_Delete[i]->SetVisible(false);
			m_UI_Waypoint_Append[i]->SetVisible(true);

			continue;
		}

		sprintf(buffer, "%f", waypointList[i].Range);
		m_UI_Waypoint_Range[i]->SetTextA(buffer);

		sprintf(buffer, "%d", waypointList[i].WaitTime_Base);
		m_UI_Waypoint_WaitBase[i]->SetTextA(buffer);

		sprintf(buffer, "%d", waypointList[i].WaitTime_Rand);
		m_UI_Waypoint_WaitRand[i]->SetTextA(buffer);

		sprintf(buffer, "%d", waypointList[i].ActionType);
		m_UI_Waypoint_ActionType[i]->SetTextA(buffer);

		if(waypointList[i].sLuaFunc.Size() > 0)
			m_UI_Waypoint_Script[i]->SetTextA(waypointList[i].sLuaFunc.Begin());
		else
			m_UI_Waypoint_Script[i]->SetTextA("");

		lua_State* L = this->GetLuaState();
		if (L)
		{
			// Initialize the move type drop down
			lua_getglobal(L, "UIDropDownMenu_Initialize");
			lua_getglobal(L, m_UI_Waypoint_MoveType[i]->GetName());
			lua_getglobal(L, "GM_ObjEdit_Template_WaypointButton_MoveType_Show");
			lua_pcall(L, 2, 0, 0);

			// Set selected ID for the move type drop down
			lua_getglobal(L, "UIDropDownMenu_SetSelectedID");
			lua_getglobal(L, m_UI_Waypoint_MoveType[i]->GetName());
			lua_pushnumber(L, waypointList[i].MoveType + 1);
			lua_pcall(L, 2, 0, 0);

			// Initialize the action type drop down
			lua_getglobal(L, "UIDropDownMenu_Initialize");
			lua_getglobal(L, m_UI_Waypoint_DirMode[i]->GetName());
			lua_getglobal(L, "GM_ObjEdit_Template_WaypointButton_DirMode_Show");
			lua_pcall(L, 2, 0, 0);

			// Set selected ID for the action type drop down
			lua_getglobal(L, "UIDropDownMenu_SetSelectedID");
			lua_getglobal(L, m_UI_Waypoint_DirMode[i]->GetName());
			lua_pushnumber(L, waypointList[i].DirMode + 1);
			lua_pcall(L, 2, 0, 0);
		}

		m_UI_Waypoint_Delete[i]->SetVisible(true);
		m_UI_Waypoint_Append[i]->SetVisible(false);

		// Create in-client representation for the flag
		g_pRuneDev->FlagEditor_CreateFlag(CRuVector3(waypointList[i].X, waypointList[i].Y, waypointList[i].Z), waypointList[i].Dir);
	}

	m_UI_Waypoint_ScrollFrame->UpdateScrollChildRect();

	m_refreshingInterface = FALSE;
}

// ----------------------------------------------------------------------------
void CGM_ObjEdit::RefreshMarkerGroupInterface()
{
	// Reset everything
	m_MGE_SelectedGroup = -1;
	m_ME_SelectedMarker = -1;

	for(INT32 i = m_MGE_Top, j = 0; j < 10; ++i, ++j)
	{
		if(i > m_markerGroups.Count())
		{
			m_UI_MarkerGroup_Frame[j]->SetVisible(false);
			continue;
		}

		if(i + m_MGE_Top == m_MGE_SelectedGroup)
			m_UI_MarkerGroup_HighlightFrame[j]->SetVisible(true);
		else
			m_UI_MarkerGroup_HighlightFrame[j]->SetVisible(false);

		if(i == m_markerGroups.Count())
		{
			m_UI_MarkerGroup_Frame[j]->SetVisible(true);

			m_UI_MarkerGroup_GroupID[j]->SetEnable(true);
			m_UI_MarkerGroup_GroupID[j]->SetTextA("");

			m_UI_MarkerGroup_GroupName[j]->SetEnable(false);
			m_UI_MarkerGroup_GroupName[j]->SetTextA("");

			m_UI_MarkerGroup_Delete[j]->SetVisible(false);
			m_UI_MarkerGroup_Append[j]->SetVisible(true);
			continue;
		}

		// Show frame
		m_UI_MarkerGroup_Frame[j]->SetVisible(true);

		MarkerGroup *markerGroup = reinterpret_cast<MarkerGroup *>(m_markerGroups[i]);

		char buffer[128];
		sprintf(buffer, "%d", markerGroup->m_markerGroupID);
		m_UI_MarkerGroup_GroupID[j]->SetEnable(false);
		m_UI_MarkerGroup_GroupID[j]->SetTextA(buffer);

		m_UI_MarkerGroup_GroupName[j]->SetEnable(false);
		m_UI_MarkerGroup_GroupName[j]->SetTextA(markerGroup->m_markerGroupName);

		m_UI_MarkerGroup_Delete[j]->SetVisible(true);
		m_UI_MarkerGroup_Append[j]->SetVisible(false);
	}

	RefreshMarkerInterface();
}

// ----------------------------------------------------------------------------
void CGM_ObjEdit::RefreshMarkerInterface(BOOL refreshDisplayOnly)
{
	if(!refreshDisplayOnly)
	{
		// Clear all flags
		g_pRuneDev->FlagEditor_ClearAll();

		m_ME_SelectedMarker = -1;
		m_ME_SelectedMarkerEntity = NULL;
	}

	if(m_MGE_SelectedGroup >= 0 && m_MGE_SelectedGroup < m_markerGroups.Count())
	{
		MarkerGroup *markerGroup = reinterpret_cast<MarkerGroup *>(m_markerGroups[m_MGE_SelectedGroup]);

		for(INT32 i = m_ME_Top, j = 0; j < 10; ++i, ++j)
		{
			if(i > markerGroup->m_markers.Count())
			{
				m_UI_Marker_Frame[j]->SetVisible(false);
				continue;
			}

			if(i == markerGroup->m_markers.Count())
			{
				m_UI_Marker_Frame[j]->SetVisible(true);

				m_UI_Marker_ID[j]->SetTextA("");
				m_UI_Marker_X[j]->SetTextA("");
				m_UI_Marker_Y[j]->SetTextA("");
				m_UI_Marker_Z[j]->SetTextA("");
				m_UI_Marker_Dir[j]->SetTextA("");

				m_UI_Marker_Delete[j]->SetVisible(false);
				m_UI_Marker_Append[j]->SetVisible(true);

				continue;
			}

			// Update highlighter
			if(i == m_ME_SelectedMarker)
				m_UI_Marker_HighlightFrame[j]->SetVisible(true);
			else
				m_UI_Marker_HighlightFrame[j]->SetVisible(false);

			// Show frame
			m_UI_Marker_Frame[j]->SetVisible(true);

			char buffer[128];
			sprintf(buffer, "%d", markerGroup->m_markers[i].FlagID);
			m_UI_Marker_ID[j]->SetTextA(buffer);

			sprintf(buffer, "%.0f", markerGroup->m_markers[i].X);
			m_UI_Marker_X[j]->SetTextA(buffer);
			m_UI_Marker_X[j]->SetEnable(false);

			sprintf(buffer, "%.0f", markerGroup->m_markers[i].Y);
			m_UI_Marker_Y[j]->SetTextA(buffer);
			m_UI_Marker_Y[j]->SetEnable(false);

			sprintf(buffer, "%.0f", markerGroup->m_markers[i].Z);
			m_UI_Marker_Z[j]->SetTextA(buffer);
			m_UI_Marker_Z[j]->SetEnable(false);

			sprintf(buffer, "%.0f", markerGroup->m_markers[i].Dir);
			m_UI_Marker_Dir[j]->SetTextA(buffer);
			m_UI_Marker_Dir[j]->SetEnable(false);

			m_UI_Marker_Delete[j]->SetVisible(true);
			m_UI_Marker_Append[j]->SetVisible(false);
		}

		if(!refreshDisplayOnly)
		{
			for(INT32 i = 0; i < markerGroup->m_markers.Count(); ++i)
			{
				CliFlagPosInfo &flagInfo = markerGroup->m_markers[i];
				g_pRuneDev->FlagEditor_CreateFlag(CRuVector3(flagInfo.X, flagInfo.Y, flagInfo.Z), flagInfo.Dir, flagInfo.FlagID);
			}
		}
	}
	else
	{
		for(INT32 j = 0; j < 10; ++j)
		{
			m_UI_Marker_Frame[j]->SetVisible(false);
		}
	}
}

// ----------------------------------------------------------------------------
bool CGM_ObjEdit::IsVisible()
{
	return m_UI_Win->IsVisible();
}

// ----------------------------------------------------------------------------
void CGM_ObjEdit::OnLoad()
{
	char Buf[256];

	m_UI_Win = static_cast<CUiFrame *>(m_interface->GetUiWorld()->FindObject( "GM_ObjEdit_Frame"));
	m_UI_Palette = static_cast<CUiFrame *>(m_interface->GetUiWorld()->FindObject( "GM_ObjEdit_Palette"));

	m_UI_OrientationFrame = static_cast<CUiFrame *>(m_interface->GetUiWorld()->FindObject("GM_ObjEdit_OrientationGroupBox"));
	m_UI_NPCPropertiesFrame = static_cast<CUiFrame *>(m_interface->GetUiWorld()->FindObject("GM_ObjEdit_PropertiesGroupBox"));
	m_UI_WaypointEditorFrame = static_cast<CUiFrame *>(m_interface->GetUiWorld()->FindObject("GM_ObjEdit_WaypointEditor"));
	m_UI_MarkerGroupFrame = static_cast<CUiFrame *>(m_interface->GetUiWorld()->FindObject("GM_ObjEdit_MarkerGroupEditor"));
	m_UI_MarkersFrame = static_cast<CUiFrame *>(m_interface->GetUiWorld()->FindObject("GM_ObjEdit_MarkerEditor"));

	m_UI_ObjID       = static_cast<CUiEditBox *>(m_interface->GetUiWorld()->FindObject("GM_ObjEdit_Role_ID"));
	m_UI_OrgName     = static_cast<CUiEditBox *>(m_interface->GetUiWorld()->FindObject("GM_ObjEdit_Role_OrgName"));
	m_UI_GItemID     = static_cast<CUiEditBox *>(m_interface->GetUiWorld()->FindObject("GM_ObjEdit_Role_GItemID"));
	m_UI_Name        = static_cast<CUiEditBox *>(m_interface->GetUiWorld()->FindObject("GM_ObjEdit_Role_Name"));
	m_UI_ObjClass    = static_cast<CUiEditBox *>(m_interface->GetUiWorld()->FindObject("GM_ObjEdit_Role_ObjClass"));
	m_UI_PID         = static_cast<CUiEditBox *>(m_interface->GetUiWorld()->FindObject("GM_ObjEdit_Role_PID"));
	m_UI_AutoPlot    = static_cast<CUiEditBox *>(m_interface->GetUiWorld()->FindObject("GM_ObjEdit_Role_AutoPlot"));
	m_UI_QuestID     = static_cast<CUiEditBox *>(m_interface->GetUiWorld()->FindObject("GM_ObjEdit_Role_QuestID"));

	for( int i = 0 ; i < 32 ; i++ )
	{
		sprintf( Buf , "GM_ObjEdit_Role_Mode_%02d" , i + 1  );
		m_UI_CheckMode[i] = static_cast<CUiCheckButton *>(m_interface->GetUiWorld()->FindObject(Buf));
	}

	m_UI_TranslateUp = static_cast<CUiButton *>(m_interface->GetUiWorld()->FindObject( "GM_ObjEdit_TranslateUpButton"));
	m_UI_TranslateDown = static_cast<CUiButton *>(m_interface->GetUiWorld()->FindObject( "GM_ObjEdit_TranslateDownButton"));
	m_UI_TranslateLeft = static_cast<CUiButton *>(m_interface->GetUiWorld()->FindObject( "GM_ObjEdit_TranslateLeftButton"));
	m_UI_TranslateRight = static_cast<CUiButton *>(m_interface->GetUiWorld()->FindObject( "GM_ObjEdit_TranslateRightButton"));

	m_UI_RotateLeft = static_cast<CUiButton *>(m_interface->GetUiWorld()->FindObject( "GM_ObjEdit_RotateLeftButton"));
	m_UI_RotateRight = static_cast<CUiButton *>(m_interface->GetUiWorld()->FindObject( "GM_ObjEdit_RotateRightButton"));

	m_UI_Radio_ViewFrame = static_cast<CUiCheckButton *>(m_interface->GetUiWorld()->FindObject( "GM_ObjEdit_ViewFrameRadioButton"));
	m_UI_Radio_ObjectFrame = static_cast<CUiCheckButton *>(m_interface->GetUiWorld()->FindObject( "GM_ObjEdit_ObjectFrameRadioButton"));
	m_UI_Check_SnapToNormal = static_cast<CUiCheckButton *>(m_interface->GetUiWorld()->FindObject( "GM_ObjEdit_Orient_SnapToNormal"));
	m_UI_Check_SnapToTerrain = static_cast<CUiCheckButton *>(m_interface->GetUiWorld()->FindObject( "GM_ObjEdit_Orient_SnapToTerrain"));
	m_UI_Check_SnapToTerrain->SetChecked(true);

	m_UI_OrientingAxisX = static_cast<CUiEditBox *>(m_interface->GetUiWorld()->FindObject("GM_ObjEdit_OrientingAxisX"));
	m_UI_OrientingAxisY = static_cast<CUiEditBox *>(m_interface->GetUiWorld()->FindObject("GM_ObjEdit_OrientingAxisY"));
	m_UI_OrientingAxisZ = static_cast<CUiEditBox *>(m_interface->GetUiWorld()->FindObject("GM_ObjEdit_OrientingAxisZ"));

	m_UI_Waypoint_ScrollFrame = static_cast<CUiScrollFrame *>(m_interface->GetUiWorld()->FindObject( "GM_ObjEdit_WaypointEditor_ScrollFrame"));

	for( int i = 0 ; i < 64 ; i++ )
	{
		sprintf( Buf , "GM_ObjEdit_WaypointEditor_ScrollFrame_Child_Waypoint_%d" , i + 1  );
		m_UI_Waypoint_Frame[i] = static_cast<CUiFrame *>(m_interface->GetUiWorld()->FindObject(Buf));

		sprintf( Buf , "GM_ObjEdit_WaypointEditor_ScrollFrame_Child_Waypoint_%d_Range" , i + 1  );
		m_UI_Waypoint_Range[i] = static_cast<CUiEditBox *>(m_interface->GetUiWorld()->FindObject(Buf));

		sprintf( Buf , "GM_ObjEdit_WaypointEditor_ScrollFrame_Child_Waypoint_%d_MoveType" , i + 1  );
		m_UI_Waypoint_MoveType[i] = static_cast<CUiFrame *>(m_interface->GetUiWorld()->FindObject(Buf));

		sprintf( Buf , "GM_ObjEdit_WaypointEditor_ScrollFrame_Child_Waypoint_%d_WaitBase" , i + 1  );
		m_UI_Waypoint_WaitBase[i] = static_cast<CUiEditBox *>(m_interface->GetUiWorld()->FindObject(Buf));

		sprintf( Buf , "GM_ObjEdit_WaypointEditor_ScrollFrame_Child_Waypoint_%d_WaitRand" , i + 1  );
		m_UI_Waypoint_WaitRand[i] = static_cast<CUiEditBox *>(m_interface->GetUiWorld()->FindObject(Buf));

		sprintf( Buf , "GM_ObjEdit_WaypointEditor_ScrollFrame_Child_Waypoint_%d_DirMode" , i + 1  );
		m_UI_Waypoint_DirMode[i] = static_cast<CUiFrame *>(m_interface->GetUiWorld()->FindObject(Buf));

		sprintf( Buf , "GM_ObjEdit_WaypointEditor_ScrollFrame_Child_Waypoint_%d_ActionType" , i + 1  );
		m_UI_Waypoint_ActionType[i] = static_cast<CUiEditBox *>(m_interface->GetUiWorld()->FindObject(Buf));

		sprintf( Buf , "GM_ObjEdit_WaypointEditor_ScrollFrame_Child_Waypoint_%d_Script" , i + 1  );
		m_UI_Waypoint_Script[i] = static_cast<CUiEditBox *>(m_interface->GetUiWorld()->FindObject(Buf));

		sprintf( Buf , "GM_ObjEdit_WaypointEditor_ScrollFrame_Child_Waypoint_%d_Delete" , i + 1  );
		m_UI_Waypoint_Delete[i] = static_cast<CUiButton *>(m_interface->GetUiWorld()->FindObject(Buf));

		sprintf( Buf , "GM_ObjEdit_WaypointEditor_ScrollFrame_Child_Waypoint_%d_Append" , i + 1  );
		m_UI_Waypoint_Append[i] = static_cast<CUiButton *>(m_interface->GetUiWorld()->FindObject(Buf));
	}

	for(int i = 0; i < 10; ++i)
	{
		sprintf( Buf , "GM_ObjEdit_MarkerGroupEditor_Child_MarkerGroup_%d" , i + 1  );
		m_UI_MarkerGroup_Frame[i] = static_cast<CUiFrame *>(m_interface->GetUiWorld()->FindObject(Buf));

		sprintf( Buf , "GM_ObjEdit_MarkerGroupEditor_Child_MarkerGroup_%d_Highlight" , i + 1  );
		m_UI_MarkerGroup_HighlightFrame[i] = static_cast<CUiFrame *>(m_interface->GetUiWorld()->FindObject(Buf));

		sprintf( Buf , "GM_ObjEdit_MarkerGroupEditor_Child_MarkerGroup_%d_GroupID" , i + 1  );
		m_UI_MarkerGroup_GroupID[i] = static_cast<CUiEditBox *>(m_interface->GetUiWorld()->FindObject(Buf));

		sprintf( Buf , "GM_ObjEdit_MarkerGroupEditor_Child_MarkerGroup_%d_GroupName" , i + 1  );
		m_UI_MarkerGroup_GroupName[i] = static_cast<CUiEditBox *>(m_interface->GetUiWorld()->FindObject(Buf));

		sprintf( Buf , "GM_ObjEdit_MarkerGroupEditor_Child_MarkerGroup_%d_Delete" , i + 1  );
		m_UI_MarkerGroup_Delete[i] = static_cast<CUiButton *>(m_interface->GetUiWorld()->FindObject(Buf));

		sprintf( Buf , "GM_ObjEdit_MarkerGroupEditor_Child_MarkerGroup_%d_Append" , i + 1  );
		m_UI_MarkerGroup_Append[i] = static_cast<CUiButton *>(m_interface->GetUiWorld()->FindObject(Buf));
	}

	for(int i = 0; i < 10; ++i)
	{
		sprintf( Buf , "GM_ObjEdit_MarkerEditor_Child_Marker_%d" , i + 1  );
		m_UI_Marker_Frame[i] = static_cast<CUiFrame *>(m_interface->GetUiWorld()->FindObject(Buf));

		sprintf( Buf , "GM_ObjEdit_MarkerEditor_Child_Marker_%d_Highlight" , i + 1  );
		m_UI_Marker_HighlightFrame[i] = static_cast<CUiFrame *>(m_interface->GetUiWorld()->FindObject(Buf));

		sprintf( Buf , "GM_ObjEdit_MarkerEditor_Child_Marker_%d_FlagID" , i + 1  );
		m_UI_Marker_ID[i] = static_cast<CUiEditBox *>(m_interface->GetUiWorld()->FindObject(Buf));

		sprintf( Buf , "GM_ObjEdit_MarkerEditor_Child_Marker_%d_X" , i + 1  );
		m_UI_Marker_X[i] = static_cast<CUiEditBox *>(m_interface->GetUiWorld()->FindObject(Buf));

		sprintf( Buf , "GM_ObjEdit_MarkerEditor_Child_Marker_%d_Y" , i + 1  );
		m_UI_Marker_Y[i] = static_cast<CUiEditBox *>(m_interface->GetUiWorld()->FindObject(Buf));

		sprintf( Buf , "GM_ObjEdit_MarkerEditor_Child_Marker_%d_Z" , i + 1  );
		m_UI_Marker_Z[i] = static_cast<CUiEditBox *>(m_interface->GetUiWorld()->FindObject(Buf));

		sprintf( Buf , "GM_ObjEdit_MarkerEditor_Child_Marker_%d_Dir" , i + 1  );
		m_UI_Marker_Dir[i] = static_cast<CUiEditBox *>(m_interface->GetUiWorld()->FindObject(Buf));

		sprintf( Buf , "GM_ObjEdit_MarkerEditor_Child_Marker_%d_Delete" , i + 1  );
		m_UI_Marker_Delete[i] = static_cast<CUiButton *>(m_interface->GetUiWorld()->FindObject(Buf));

		sprintf( Buf , "GM_ObjEdit_MarkerEditor_Child_Marker_%d_Append" , i + 1  );
		m_UI_Marker_Append[i] = static_cast<CUiButton *>(m_interface->GetUiWorld()->FindObject(Buf));
	}

	m_UI_CreateButton = static_cast<CUiButton *>(m_interface->GetUiWorld()->FindObject( "GM_ObjEdit_Create"));
	m_UI_DeleteButton = static_cast<CUiButton *>(m_interface->GetUiWorld()->FindObject( "GM_ObjEdit_Delete"));
	m_UI_ApplyButton = static_cast<CUiButton *>(m_interface->GetUiWorld()->FindObject( "GM_ObjEdit_Apply"));
}

// ----------------------------------------------------------------------------
void CGM_ObjEdit::OnShow()
{
	m_UI_Win->SetScale(0.75f);

	ResetEditorFields();

	g_pRuneDev->SprEdit_Begin();
}

// ----------------------------------------------------------------------------
void CGM_ObjEdit::OnUpdate()
{
	switch(m_objEditMode)
	{
		case OBJEDITMODE_NPC:
			NPC_Update();
			break;

		case OBJEDITMODE_MARKER:
			MGE_Update();
			break;
	}
}

// ----------------------------------------------------------------------------
void CGM_ObjEdit::OnClose()
{
	ResetEditorFields();

	g_pRuneDev->SprEdit_End();
}

// ----------------------------------------------------------------------------
void CGM_ObjEdit::OnClick()
{
	switch(m_objEditMode)
	{
		case OBJEDITMODE_NPC:
			if(m_dragJustFinished == FALSE)
			{
				// Update gizmo click
				if((m_spriteHasPrecedence && m_selectedSprite) || m_selectedWaypoint)
				{
					CRuCamera *camera = g_pRuneDev->GetCamera();

					// Get the target position for the pick ray in world coordinates
					CRuVector3 homPt, viewPt, worldPt;
					camera->Coord_ScreenToHomogeneous(g_pGameMain->GetCursorPtX(), g_pGameMain->GetCursorPtY(), g_pRuneDev->GetScreenWidth(), g_pRuneDev->GetScreenHeight(), homPt);
					camera->Coord_HomogeneousToView(homPt, viewPt);
					camera->Coord_ViewToWorld(viewPt, worldPt);

					// Build collision segment
					CRuRay3 colRay(camera->GetCameraPosition(), (worldPt - camera->GetCameraPosition()).Normalize());

					// Activate the manipulator
					m_moverGizmo->ActivateManipulator(colRay);
				}

				if(NPC_SelectTargetAtCursor())
				{
				}
			}

			break;

		case OBJEDITMODE_MARKER:
			if(m_dragJustFinished == FALSE)
			{
				// Update gizmo click
				if(m_ME_SelectedMarkerEntity)
				{
					CRuCamera *camera = g_pRuneDev->GetCamera();

					// Get the target position for the pick ray in world coordinates
					CRuVector3 homPt, viewPt, worldPt;
					camera->Coord_ScreenToHomogeneous(g_pGameMain->GetCursorPtX(), g_pGameMain->GetCursorPtY(), g_pRuneDev->GetScreenWidth(), g_pRuneDev->GetScreenHeight(), homPt);
					camera->Coord_HomogeneousToView(homPt, viewPt);
					camera->Coord_ViewToWorld(viewPt, worldPt);

					// Build collision segment
					CRuRay3 colRay(camera->GetCameraPosition(), (worldPt - camera->GetCameraPosition()).Normalize());

					// Activate the manipulator
					m_moverGizmo->ActivateManipulator(colRay);
				}

				if(ME_SelectMarkerAtCursor())
				{
				}
			}

			break;
	}
}

// ----------------------------------------------------------------------------
void CGM_ObjEdit::OnDragStart()
{
	switch(m_objEditMode)
	{
		case OBJEDITMODE_NPC:
			if((m_spriteHasPrecedence && m_selectedSprite) || m_selectedWaypoint)
			{
				CRuCamera *camera = g_pRuneDev->GetCamera();

				// Get the target position for the pick ray in world coordinates
				CRuVector3 homPt, viewPt, worldPt;
				camera->Coord_ScreenToHomogeneous(g_pGameMain->GetCursorPtX(), g_pGameMain->GetCursorPtY(), g_pRuneDev->GetScreenWidth(), g_pRuneDev->GetScreenHeight(), homPt);
				camera->Coord_HomogeneousToView(homPt, viewPt);
				camera->Coord_ViewToWorld(viewPt, worldPt);

				// Build collision segment
				CRuRay3 colRay(camera->GetCameraPosition(), (worldPt - camera->GetCameraPosition()).Normalize());

				// Activate the manipulator
				if(m_moverGizmo->ActivateManipulator(colRay))
				{
					m_dragging = TRUE;
				}
			}

			break;

		case OBJEDITMODE_MARKER:
			if(m_ME_SelectedMarkerEntity)
			{
				CRuCamera *camera = g_pRuneDev->GetCamera();

				// Get the target position for the pick ray in world coordinates
				CRuVector3 homPt, viewPt, worldPt;
				camera->Coord_ScreenToHomogeneous(g_pGameMain->GetCursorPtX(), g_pGameMain->GetCursorPtY(), g_pRuneDev->GetScreenWidth(), g_pRuneDev->GetScreenHeight(), homPt);
				camera->Coord_HomogeneousToView(homPt, viewPt);
				camera->Coord_ViewToWorld(viewPt, worldPt);

				// Build collision segment
				CRuRay3 colRay(camera->GetCameraPosition(), (worldPt - camera->GetCameraPosition()).Normalize());

				// Activate the manipulator
				if(m_moverGizmo->ActivateManipulator(colRay))
				{
					m_dragging = TRUE;
				}
			}

			break;
	}

}

// ----------------------------------------------------------------------------
void CGM_ObjEdit::OnReceiveDrag()
{
}

// ----------------------------------------------------------------------------
void CGM_ObjEdit::OnDragStop()
{
	if(m_dragging)
	{
		m_dragging = FALSE;
		m_dragJustFinished = TRUE;
	}
}

// ----------------------------------------------------------------------------
void CGM_ObjEdit::OnToggleModeClick()
{
	// Toggle mode
	switch(m_objEditMode)
	{
		case OBJEDITMODE_NPC:
			m_objEditMode = OBJEDITMODE_MARKER;
			break;

		case OBJEDITMODE_MARKER:
			m_objEditMode = OBJEDITMODE_NPC;
			break;
	}

	// Update interface
	RefreshInterfaceLayout();
}

// ----------------------------------------------------------------------------
void CGM_ObjEdit::OnRotateLeftClick()
{
	if(m_spriteHasPrecedence)
	{
		if(m_selectedSprite)
		{
			CRuVector3 newDirection = m_selectedSprite->RotateDirection(0.01f);
			m_selectedSprite->SetDirection(newDirection, true);
/*
			char moo[200];
			sprintf(moo, "%f %f %f\n", newDirection.m_x, newDirection.m_y, newDirection.m_z);
			OutputDebugString(moo);
*/
		}
	}
	else
	{
		if(m_selectedWaypoint)
		{
			CRuQuaternion rotQuat;
			rotQuat.FromAngleAxis(CRuVector3(0.0f, 1.0f, 0.0f), 0.1f);

			REAL scale = m_selectedWaypoint->GetScale(ruFRAMETYPE_LOCAL);
			CRuQuaternion rotation = m_selectedWaypoint->GetRotation(ruFRAMETYPE_LOCAL);
			CRuVector3 position = m_selectedWaypoint->GetTranslation(ruFRAMETYPE_LOCAL);

			m_selectedWaypoint->ResetAllTransforms();
			m_selectedWaypoint->Scale(scale);
			m_selectedWaypoint->Rotate(rotation);
			m_selectedWaypoint->Rotate(rotQuat);
			m_selectedWaypoint->Translate(position);

			// Update waypoint information
			CRuArrayList<NPC_MoveBaseStruct> &waypointList = *reinterpret_cast<CRuArrayList<NPC_MoveBaseStruct> *>(m_selectedSpriteMoveInfo);
			NPC_MoveBaseStruct &moveStruct = waypointList[m_selectedWaypointIdx];

			CRuVector3 direction(1.0f, 0.0f, 0.0f);
			m_selectedWaypoint->GetWorldTransform().TransformVector(direction);

			// 取得角色所朝方向
			float theta = acos( direction.Dot( CRuVector3(1.0f, 0.0f, 0.0f) ) );
			if ( direction.m_z > 0.0f )
				theta = PI + PI - theta;

			// 轉換為角度
			theta = theta * 180.0f / PI;

			moveStruct.Dir = theta;
		}
	}
}

// ----------------------------------------------------------------------------
void CGM_ObjEdit::OnRotateRightClick()
{
	if(m_spriteHasPrecedence)
	{
		if(m_selectedSprite)
		{
			CRuVector3 newDirection = m_selectedSprite->RotateDirection(-0.01f);
			m_selectedSprite->SetDirection(newDirection, true);
/*
			char moo[200];
			sprintf(moo, "%f %f %f\n", newDirection.m_x, newDirection.m_y, newDirection.m_z);
			OutputDebugString(moo);
*/
		}
	}
	else
	{
		if(m_selectedWaypoint)
		{
			CRuQuaternion rotQuat;
			rotQuat.FromAngleAxis(CRuVector3(0.0f, 1.0f, 0.0f), -0.1f);

			REAL scale = m_selectedWaypoint->GetScale(ruFRAMETYPE_LOCAL);
			CRuQuaternion rotation = m_selectedWaypoint->GetRotation(ruFRAMETYPE_LOCAL);
			CRuVector3 position = m_selectedWaypoint->GetTranslation(ruFRAMETYPE_LOCAL);

			m_selectedWaypoint->ResetAllTransforms();
			m_selectedWaypoint->Scale(scale);
			m_selectedWaypoint->Rotate(rotation);
			m_selectedWaypoint->Rotate(rotQuat);
			m_selectedWaypoint->Translate(position);

			// Update waypoint information
			CRuArrayList<NPC_MoveBaseStruct> &waypointList = *reinterpret_cast<CRuArrayList<NPC_MoveBaseStruct> *>(m_selectedSpriteMoveInfo);
			NPC_MoveBaseStruct &moveStruct = waypointList[m_selectedWaypointIdx];

			CRuVector3 direction(1.0f, 0.0f, 0.0f);
			m_selectedWaypoint->GetWorldTransform().TransformVector(direction);

			// 取得角色所朝方向
			float theta = acos( direction.Dot( CRuVector3(1.0f, 0.0f, 0.0f) ) );
			if ( direction.m_z > 0.0f )
				theta = PI + PI - theta;

			// 轉換為角度
			theta = theta * 180.0f / PI;

			moveStruct.Dir = theta;
		}
	}
}

// ----------------------------------------------------------------------------
void CGM_ObjEdit::OnResetClick()
{
	ResetEditorFields();
}

// ----------------------------------------------------------------------------
void CGM_ObjEdit::OnCreateMacroClick()
{
	if ( m_currentCreateCommand == NULL || m_currentCreateCommand[0] ) {
		char name[512];
		char macroContent[512];
		WCharToUTF8(m_UI_Name->GetText(), name, 512);
		sprintf_s(macroContent, 512, "/GM %s", m_currentCreateCommand);
		if ( !g_pMacroFrame->CreateMacro(name, 0 , macroContent) ) {
			g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", "Create Macro Error!!");
		}
	}
}

// ----------------------------------------------------------------------------
void CGM_ObjEdit::OnAppendWaypointClick()
{
	CRoleSprite *player = g_pGameMain->GetPlayer();

	if(player)
	{
		CRuVector3 position = player->GetPos();
		CRuVector3 direction = player->GetDirection();

		// 取得角色所朝方向
		float theta = acos( direction.Dot( CRuVector3(1.0f, 0.0f, 0.0f) ) );
		if ( direction.m_z > 0.0f )
			theta = PI + PI - theta;

		// 轉換為角度
		theta = theta * 180.0f / PI;

		// Construct new move struct
		NPC_MoveBaseStruct moveStruct;
		moveStruct.MoveType = EM_NPC_MoveType_Rand;
		moveStruct.X = position.m_x;
		moveStruct.Y = position.m_y;
		moveStruct.Z = position.m_z;
		moveStruct.Dir = theta;

		// Add move struct to move list
		CRuArrayList<NPC_MoveBaseStruct> &waypointList = *reinterpret_cast<CRuArrayList<NPC_MoveBaseStruct> *>(m_selectedSpriteMoveInfo);
		waypointList.Add(moveStruct);

		// Refresh interface
		RefreshNPCMoveInfoInterface();
	}
}

// ----------------------------------------------------------------------------
void CGM_ObjEdit::OnDeleteWaypointClick(const char *waypointName)
{
	for(INT32 i = 0; i < 64; ++i)
	{
		if(m_UI_Waypoint_Delete[i] && stricmp(m_UI_Waypoint_Delete[i]->GetName(), waypointName) == 0)
		{
			m_waypointToBeDeleted = i;

			lua_State* L = this->GetLuaState();
			if (L)
			{
				// Initialize the move type drop down
				lua_getglobal(L, "StaticPopup_Show");
				lua_pushstring(L, "GM_OBJEDIT_DELETE_WAYPOINT");
				lua_pushnil(L);
				lua_pushnil(L);
				lua_pushnil(L);
				lua_pcall(L, 4, 1, 0);
				lua_pop(L, 1);
			}

			break;
		}
	}
}

// ----------------------------------------------------------------------------
void CGM_ObjEdit::OnDeleteWaypointConfirm()
{
	NPC_DeleteWaypointAtIndex(m_waypointToBeDeleted);
}

// ----------------------------------------------------------------------------
void CGM_ObjEdit::OnCreateClick()
{
	switch(m_objEditMode)
	{
		case OBJEDITMODE_NPC:
			{
				if(m_currentCreateCommand && strlen(m_currentCreateCommand) > 0)
				{
					g_pChatEdit->SendChatMessage(eChatType_GM, m_currentCreateCommand, "", "");
				}
			}

			break;

		case OBJEDITMODE_MARKER:
			{
			}

			break;
	}
}

// ----------------------------------------------------------------------------
void CGM_ObjEdit::OnDeleteClick()
{
	switch(m_objEditMode)
	{
		case OBJEDITMODE_NPC:
			{
				if(m_currentDeleteCommand && strlen(m_currentDeleteCommand) > 0)
				{

					lua_State* L = this->GetLuaState();
					if (L)
					{
						// Initialize the move type drop down
						lua_getglobal(L, "StaticPopup_Show");
						lua_pushstring(L, "GM_OBJEDIT_DELETE_NPC");
						lua_pushnil(L);
						lua_pushnil(L);
						lua_pushnil(L);
						lua_pcall(L, 4, 1, 0);
						lua_pop(L, 1);
					}
				}
			}

			break;

		case OBJEDITMODE_MARKER:
			{
			}

			break;
	}
}

// ----------------------------------------------------------------------------
void CGM_ObjEdit::OnDeleteConfirm()
{
	switch(m_objEditMode)
	{
		case OBJEDITMODE_NPC:
			{
				g_pChatEdit->SendChatMessage(eChatType_GM, m_currentDeleteCommand, "", "");
				ResetEditorFields();
			}

			break;

		case OBJEDITMODE_MARKER:
			{
			}

			break;
	}
}

// ----------------------------------------------------------------------------
void CGM_ObjEdit::OnApplyClick()
{
	if(m_selectedSprite)
	{
		// Modify NPC move info
		UpdateNPCMoveInfoFromInterface();

		if(m_selectedSpriteMoveInfo)
		{
			CRuArrayList<NPC_MoveBaseStruct> &waypointList = *reinterpret_cast<CRuArrayList<NPC_MoveBaseStruct> *>(m_selectedSpriteMoveInfo);
			vector<NPC_MoveBaseStruct> moveList;

			for(INT32 i = 0; i < waypointList.Count(); ++i)
			{
				// Add move struct to the move list
				moveList.push_back(waypointList[i]);
			}

			Net_GSrvConnect::SaveNPCMoveInfo(m_selectedSpriteID, moveList);
		}

		// Modify NPC base info
		UpdateModifyNPCInfoFromInterface();

		Net_GSrvConnect::ModyNPC(reinterpret_cast<ModifyNPCInfo *>(m_selectedSpriteModifyInfo));

		// Reset editor
		ResetEditorFields();
	}
}

// ----------------------------------------------------------------------------
BOOL CGM_ObjEdit::NPC_SelectTargetAtCursor()
{
	BOOL spriteSelected = FALSE;
	IBaseSprite *sprite = NULL;
	REAL spriteDistance = FLT_MAX;

	spriteSelected = g_pRuneDev->SprEdit_PickAtCursor(g_pRuneDev->GetCursorPtX(), g_pRuneDev->GetCursorPtY(), &sprite, &spriteDistance);

	BOOL waypointSelected = FALSE;
	INT32 waypointIndex = 0;
	CRuEntity *waypoint = NULL;
	REAL waypointDistance = FLT_MAX;

	waypointSelected = g_pRuneDev->FlagEditor_GetFlagAtCursor(g_pRuneDev->GetCursorPtX(), g_pRuneDev->GetCursorPtY(), &waypointIndex, &waypoint, &waypointDistance);

	// Extra processing for the sprite
	if(sprite && sprite != g_pGameMain->GetPlayer())
	{
		if(sprite != m_selectedSprite)
		{
			// Reset editor
			ResetEditorFields();

			m_selectedSprite = sprite;
			m_selectedSpriteID = m_selectedSprite->GetIdentify();

			g_pRuneDev->ProtectSprite((CRoleSprite *) m_selectedSprite);

			// Get sprite base information
			char cmdStr[128];
			sprintf(cmdStr, "? modify %d", m_selectedSpriteID);
			g_pChatEdit->SendChatMessage(eChatType_GM, cmdStr, "", "");

			// Get NPC movement information
			Net_GSrvConnect::GetNPCMoveInfo(m_selectedSpriteID);

			char temp[128];	
			if ( m_UI_OrientingAxisX ) {
				if ( m_selectedSprite ) {
					sprintf_s(temp, 128, "%.3f", m_selectedSprite->GetOrientingAxis().m_x);
				} else {
					memset(temp, 0, sizeof(temp));
				}

				m_UI_OrientingAxisX->SetTextA(temp);
			}
			if ( m_UI_OrientingAxisY ) {
				if ( m_selectedSprite ) {
					sprintf_s(temp, 128, "%.3f", m_selectedSprite->GetOrientingAxis().m_y);
				} else {
					memset(temp, 0, sizeof(temp));
				}		

				m_UI_OrientingAxisY->SetTextA(temp);
			}
			if ( m_UI_OrientingAxisZ ) {
				if ( m_selectedSprite ) {
					sprintf_s(temp, 128, "%.3f", m_selectedSprite->GetOrientingAxis().m_z);
				} else {
					memset(temp, 0, sizeof(temp));
				}

				m_UI_OrientingAxisZ->SetTextA(temp);
			}
		}
	}

	// Determine whether sprite has precedence over waypoint or vice-versa
	if(waypointSelected)
	{
		m_selectedWaypointIdx = waypointIndex;
		m_selectedWaypoint = waypoint;

		if(spriteSelected)
		{
			if(waypointDistance < spriteDistance)
				m_spriteHasPrecedence = FALSE;
			else
				m_spriteHasPrecedence = TRUE;
		}
		else
		{
			m_spriteHasPrecedence = FALSE;
		}
	}
	else
	{
		m_spriteHasPrecedence = TRUE;
	}

	return spriteSelected || waypointSelected;
}

// ----------------------------------------------------------------------------
BOOL CGM_ObjEdit::NPC_ProcessMouseHighlights()
{
	BOOL spriteSelected = FALSE;
	IBaseSprite *sprite = NULL;
	REAL spriteDistance = FLT_MAX;

	spriteSelected = g_pRuneDev->SprEdit_PickAtCursor(g_pRuneDev->GetCursorPtX(), g_pRuneDev->GetCursorPtY(), &sprite, &spriteDistance);

	BOOL waypointSelected = FALSE;
	INT32 waypointIndex = 0;
	CRuEntity *waypoint = NULL;
	REAL waypointDistance = FLT_MAX;

	waypointSelected = g_pRuneDev->FlagEditor_GetFlagAtCursor(g_pRuneDev->GetCursorPtX(), g_pRuneDev->GetCursorPtY(), &waypointIndex, &waypoint, &waypointDistance);

	CRuCamera *camera = g_pRuneDev->GetCamera();

	// Get the target position for the pick ray in world coordinates
	CRuVector3 homPt, viewPt, worldPt;
	camera->Coord_ScreenToHomogeneous(g_pGameMain->GetCursorPtX(), g_pGameMain->GetCursorPtY(), g_pRuneDev->GetScreenWidth(), g_pRuneDev->GetScreenHeight(), homPt);
	camera->Coord_HomogeneousToView(homPt, viewPt);
	camera->Coord_ViewToWorld(viewPt, worldPt);

	// Build collision segment
	CRuRay3 colRay(camera->GetCameraPosition(), (worldPt - camera->GetCameraPosition()).Normalize());

	BOOL gizmoHighlight = m_moverGizmo->HighlightManipulator(colRay);

	return (spriteSelected && sprite != g_pGameMain->GetPlayer()) || waypointSelected || gizmoHighlight;
}

// ----------------------------------------------------------------------------
void CGM_ObjEdit::NPC_TranslateTarget(INT32 axisIdx)
{
	if(m_selectedSprite)
	{
		CRuVector3 translationAxis(0.0f, 0.0f, 0.0f);

		switch(axisIdx)
		{
			case 0:
				translationAxis.Set(0.0f, 0.0f, 1.0f);
				break;
			case 1:
				translationAxis.Set(0.0f, 0.0f, -1.0f);
				break;
			case 2:
				translationAxis.Set(-1.0f, 0.0f, 0.0f);
				break;
			case 3:
				translationAxis.Set(1.0f, 0.0f, 0.0f);
				break;
		}

		if(m_UI_Radio_ViewFrame->IsChecked())
		{
			CRuCamera *camera = g_pRuneDev->GetCamera();
			camera->GetInverseCameraTransform().TransformVector(translationAxis);

			translationAxis.m_y = 0;

			m_selectedSprite->SetPos(m_selectedSprite->GetPos() + translationAxis);
		}
		else if(m_UI_Radio_ObjectFrame->IsChecked())
		{
		}
	}
}

// ----------------------------------------------------------------------------
void CGM_ObjEdit::NPC_DeleteWaypointAtIndex(INT32 idx)
{
	CRuArrayList<NPC_MoveBaseStruct> &waypointList = *reinterpret_cast<CRuArrayList<NPC_MoveBaseStruct> *>(m_selectedSpriteMoveInfo);

	if(idx >= 0 && idx < waypointList.Count())
	{
		// Remove waypoint
		waypointList.RemoveAt(idx);

		// Re-arrange interface elements
		RefreshNPCMoveInfoInterface();
	}
}

// ----------------------------------------------------------------------------
void CGM_ObjEdit::NPC_Update()
{
	if(m_dragJustFinished)
		m_dragJustFinished = FALSE;

	if(NPC_ProcessMouseHighlights() == FALSE)
	{
		this->m_interface->GetUiWorld()->SetMouseExclude(m_UI_Win);
	}
	else
	{
		this->m_interface->GetUiWorld()->SetMouseExclude(NULL);
	}

	m_moverGizmo->SetCollisionObject(IBaseSprite::GetCollisionContainer());
	m_moverGizmo->SetSnapToCollisionObject(m_UI_Check_SnapToTerrain->IsChecked());

	// Update gizmo
	if(m_spriteHasPrecedence && m_selectedSprite)
	{
		if(m_moverGizmo->GetParent() != m_selectedSprite->GetRuEntity())
		{
			m_moverGizmo->DetachFromParent();
			m_selectedSprite->GetRuEntity()->AddChild(m_moverGizmo);
		}
	}
	else if(m_selectedWaypoint)
	{
		if(m_moverGizmo->GetParent() != m_selectedWaypoint)
		{
			m_moverGizmo->DetachFromParent();
			m_selectedWaypoint->AddChild(m_moverGizmo);
		}
	}

	CRuVector3 upVector;
	if ( m_UI_OrientingAxisX && m_UI_OrientingAxisY && m_UI_OrientingAxisZ && m_selectedSprite ) {		
		upVector.m_x = (REAL)_wtof(m_UI_OrientingAxisX->GetText());
		upVector.m_y = (REAL)_wtof(m_UI_OrientingAxisY->GetText());
		upVector.m_z = (REAL)_wtof(m_UI_OrientingAxisZ->GetText());
		if ( upVector.m_x != 0.0f && upVector.m_y != 0.0f && upVector.m_z != 0.0f ) {
			m_selectedSprite->SetOrientingAxis(upVector.Normalize(), true);			
		}

		// 記錄最後狀態
		upVector = m_selectedSprite->GetOrientingAxis();
	}

	// Dragging action
	if(m_dragging)
	{
		CRuCamera *camera = g_pRuneDev->GetCamera();

		// Get the target position for the pick ray in world coordinates
		CRuVector3 homPt, viewPt, worldPt;
		camera->Coord_ScreenToHomogeneous(g_pGameMain->GetCursorPtX(), g_pGameMain->GetCursorPtY(), g_pRuneDev->GetScreenWidth(), g_pRuneDev->GetScreenHeight(), homPt);
		camera->Coord_HomogeneousToView(homPt, viewPt);
		camera->Coord_ViewToWorld(viewPt, worldPt);

		// Build collision segment
		CRuRay3 colRay(camera->GetCameraPosition(), (worldPt - camera->GetCameraPosition()).Normalize());

		m_moverGizmo->Manipulate(colRay);

		if(m_spriteHasPrecedence && m_selectedSprite)
		{
			if(m_selectedSprite)
			{
				m_selectedSprite->UpdatePositionDirectionFromEntity();

				// Snap sprite up axis to terrain normal if specified
				if(m_UI_Check_SnapToNormal->IsChecked())
				{
					g_pRuneDev->SprEdit_SnapToNormalAtCursor(m_selectedSprite, g_pRuneDev->GetCursorPtX(), g_pRuneDev->GetCursorPtY());

					// Set orientation mode to unconstrained
					m_selectedSprite->SetOrientMode(CRuFusion_Actor::ruORIENTMODE_UNCONSTRAINED);
				}
				else
				{
					// Set orientation mode to constrained
					m_selectedSprite->SetOrientMode(CRuFusion_Actor::ruORIENTMODE_CONSTRAINED);
				}

				// Update sprite orientation based on current status
				m_selectedSprite->UpdateOrientation();
			}
		}
		else
		{
			if(m_selectedWaypoint)
			{
				// Update waypoint information
				CRuArrayList<NPC_MoveBaseStruct> &waypointList = *reinterpret_cast<CRuArrayList<NPC_MoveBaseStruct> *>(m_selectedSpriteMoveInfo);
				NPC_MoveBaseStruct &moveStruct = waypointList[m_selectedWaypointIdx];

				CRuVector3 direction(1.0f, 0.0f, 0.0f);
				m_selectedWaypoint->GetWorldTransform().TransformVector(direction);

				// 取得角色所朝方向
				float theta = acos( direction.Dot( CRuVector3(1.0f, 0.0f, 0.0f) ) );
				if ( direction.m_z > 0.0f )
					theta = PI + PI - theta;

				// 轉換為角度
				theta = theta * 180.0f / PI;

				moveStruct.X = m_selectedWaypoint->GetTranslation(ruFRAMETYPE_WORLD).m_x;
				moveStruct.Y = m_selectedWaypoint->GetTranslation(ruFRAMETYPE_WORLD).m_y;
				moveStruct.Z = m_selectedWaypoint->GetTranslation(ruFRAMETYPE_WORLD).m_z;
				moveStruct.Dir = theta;
			}
		}
	}

	if ( m_selectedSprite && upVector != m_selectedSprite->GetOrientingAxis() )
	{
		if ( m_UI_OrientingAxisX && m_UI_OrientingAxisY && m_UI_OrientingAxisZ )
		{
			char temp[128];
			sprintf_s(temp, 128, "%.3f", m_selectedSprite->GetOrientingAxis().m_x);
			m_UI_OrientingAxisX->SetTextA(temp);
			sprintf_s(temp, 128, "%.3f", m_selectedSprite->GetOrientingAxis().m_y);
			m_UI_OrientingAxisY->SetTextA(temp);
			sprintf_s(temp, 128, "%.3f", m_selectedSprite->GetOrientingAxis().m_z);
			m_UI_OrientingAxisZ->SetTextA(temp);
		}
	}

	// Get the base object ID
	int orgID = 0;

	char *tmp = TransToChar(m_UI_ObjID->GetText());
	if(tmp)
	{
		orgID = atol(tmp);
		delete[] tmp;
	}

	// Get base object data
	GameObjDbStructEx *orgObjDB = CNetGlobal::GetObj(orgID);

	if(orgObjDB == NULL)
	{
		m_UI_OrgName->SetTextA("無");
	}
	else
	{
		m_UI_OrgName->SetTextA(orgObjDB->GetName());

		if(m_lastOrgID != orgID && m_selectedSprite == NULL)
		{        
			ObjectModeStruct initMode = orgObjDB->GetModeFlag();

			for(INT32 i = 0; i < 32; ++i)
			{   
				if(m_UI_CheckMode[i] == NULL)
					continue;

				if(TestBit(initMode._Mode, i) == false)
					m_UI_CheckMode[i]->SetChecked(false);
				else
					m_UI_CheckMode[i]->SetChecked(true);
			}
		}

		m_lastOrgID = orgID;
	}

	if(orgObjDB)
	{
		INT32 modeFlags = 0;
		INT32 questID = 0;
		INT32 pid = 0;

		for(INT32 i = 0; i < 32; ++i)
		{
			if(m_UI_CheckMode[i] == NULL || m_UI_CheckMode[i]->IsChecked() == false)
				continue;

			SetBit(modeFlags , i);
		}

		tmp = TransToChar(m_UI_QuestID->GetText());
		if(tmp)
		{
			questID = atol(tmp);
			delete[] tmp;

		}
		tmp = TransToChar(m_UI_PID->GetText());
		if(tmp)
		{
			pid = atol(tmp);
			delete[] tmp;
		}

		// Command syntax: CreateObject OrgObjID, x, y, z, dir, Count, AutoPlot, ObjName, ClassName, QuestID, Mode
		const WCHAR* nameStr;
		const WCHAR* autoPlotStr = m_UI_AutoPlot->GetText();
		const WCHAR* classNameStr = m_UI_ObjClass->GetText();

		if(wcslen(m_UI_Name->GetText()) != 0)
			nameStr = m_UI_Name->GetText();
		else
			nameStr = L"";

		if(wcslen(autoPlotStr) == 0)
			autoPlotStr = L"-";

		if(wcslen(classNameStr) == 0)
			classNameStr = L"-";

		WCHAR buf[1024];
		swprintf(buf, 1024, L"? CreateObj %d,\"%s\",0,0,0,0,1,\"%s\",\"%s\",%d,0x%x,%d", orgID, nameStr, autoPlotStr, classNameStr,  questID, modeFlags, pid);

		tmp = WCharToUFT8(buf);
		if(tmp)
		{
			delete[] m_currentCreateCommand;
			m_currentCreateCommand = tmp;
		}
	}
	else
	{
		delete[] m_currentCreateCommand;
		m_currentCreateCommand = NULL;
	}

	if(m_selectedSprite)
	{
		WCHAR buf[1024];
		swprintf(buf, 1024, L"? delobj %d", m_selectedSpriteID);

		tmp = WCharToUFT8(buf);
		if(tmp)
		{
			delete[] m_currentDeleteCommand;
			m_currentDeleteCommand = tmp;
		}
	}
	else
	{
		delete[] m_currentDeleteCommand;
		m_currentDeleteCommand = NULL;
	}

	// Update interface
	if(m_currentCreateCommand && strlen(m_currentCreateCommand) > 0)
	{
		m_UI_CreateButton->SetEnable(true);
	}
	else
	{
		m_UI_CreateButton->SetEnable(false);
	}

	if(m_currentDeleteCommand && strlen(m_currentDeleteCommand) > 0)
	{
		m_UI_DeleteButton->SetEnable(true);
		m_UI_ApplyButton->SetEnable(true);
	}
	else
	{
		m_UI_DeleteButton->SetEnable(false);
		m_UI_ApplyButton->SetEnable(false);
	}
}

// ----------------------------------------------------------------------------
void CGM_ObjEdit::MGE_OnAppendClick()
{
	INT32 editorFrameIdx = 0;
	INT32 flagGroupIDToAppend = 0;

	// Look for the flag group ID we want to append
	for(INT32 i = 0; i < 10; ++i)
	{
		if(m_UI_MarkerGroup_Frame[i]->IsVisible() && m_UI_MarkerGroup_Append[i]->IsVisible())
		{
			INT32 flagGroupID = ConvertTextToInt(m_UI_MarkerGroup_GroupID[i]->GetText());
			GameObjDbStruct *dbStruct = g_ObjectData->GetObj(flagGroupID);

			if(dbStruct)
			{
				editorFrameIdx = i;
				flagGroupIDToAppend = flagGroupID;
			}

			break;
		}
	}

	if(flagGroupIDToAppend >= Def_ObjectClass_Flag)
	{
		BOOL alreadyExists = FALSE;

		// Ensure there are no existing flag groups with the same group ID
		for(INT32 i = 0; i < m_markerGroups.Count(); ++i)
		{
			MarkerGroup *markerGroup = reinterpret_cast<MarkerGroup *>(m_markerGroups[i]);

			if(markerGroup->m_markerGroupID == flagGroupIDToAppend)
			{
				alreadyExists = TRUE;
				break;
			}
		}

		// The group does not already exist, so create it now
		if(!alreadyExists)
		{
			MarkerGroup *markerGroup = ruNEW MarkerGroup();
			markerGroup->m_markerGroupID = flagGroupIDToAppend;

			markerGroup->m_markerGroupName = WCharToUFT8(m_UI_MarkerGroup_GroupName[editorFrameIdx]->GetText());
			if(!markerGroup->m_markerGroupName)
			{
				markerGroup->m_markerGroupName = ruNEW char [1];
				markerGroup->m_markerGroupName[0] = NULL;
			}

			m_markerGroups.Add(markerGroup);

			RefreshMarkerGroupInterface();
		}
	}
}

// ----------------------------------------------------------------------------
void CGM_ObjEdit::MGE_OnDeleteClick(const char *markerGroupName)
{
	for(INT32 i = 0; i < 10; ++i)
	{
		if(m_UI_MarkerGroup_Delete[i] && stricmp(m_UI_MarkerGroup_Delete[i]->GetName(), markerGroupName) == 0)
		{
			m_markerGroupToBeDeleted = i + m_MGE_Top;

			lua_State* L = this->GetLuaState();
			if (L)
			{
				// Initialize the dialog box
				lua_getglobal(L, "StaticPopup_Show");
				lua_pushstring(L, "GM_OBJEDIT_MGE_DELETE");
				lua_pushnil(L);
				lua_pushnil(L);
				lua_pushnil(L);
				lua_pcall(L, 4, 1, 0);
				lua_pop(L, 1);
			}

			break;
		}
	}
}

// ----------------------------------------------------------------------------
void CGM_ObjEdit::MGE_OnDeleteConfirm()
{
	MGE_DeleteFlagGroup(m_markerGroupToBeDeleted);

	// Refresh marker interface
	RefreshMarkerGroupInterface();
}

// ----------------------------------------------------------------------------
void CGM_ObjEdit::MGE_OnNextClick()
{
	if(m_MGE_Top + 10 <= m_markerGroups.Count())
	{
		m_MGE_Top += 10;
		m_MGE_SelectedGroup = -1;

		RefreshMarkerGroupInterface();
	}
}

// ----------------------------------------------------------------------------
void CGM_ObjEdit::MGE_OnPreviousClick()
{
	if(m_MGE_Top - 10 >= 0)
	{
		m_MGE_Top -= 10;
		m_MGE_SelectedGroup = -1;

		RefreshMarkerGroupInterface();
	}
}

// ----------------------------------------------------------------------------
void CGM_ObjEdit::MGE_OnReloadClick()
{
	Net_GSrvConnect::GetFlagList();
}

// ----------------------------------------------------------------------------
void CGM_ObjEdit::MGE_OnSelectionChanged(INT32 selectionIdx)
{
	m_MGE_SelectedGroup = m_MGE_Top + selectionIdx;
	m_ME_Top = 0;

	for(INT32 i = 0; i < 10; ++i)
	{
		if(i + m_MGE_Top == m_MGE_SelectedGroup)
			m_UI_MarkerGroup_HighlightFrame[i]->SetVisible(true);
		else
			m_UI_MarkerGroup_HighlightFrame[i]->SetVisible(false);
	}

	RefreshMarkerInterface();
}

// ----------------------------------------------------------------------------
void CGM_ObjEdit::MGE_ClearMarkers()
{
	for(INT32 i = 0; i < m_markerGroups.Count(); ++i)
	{
		MarkerGroup *markerGroup = reinterpret_cast<MarkerGroup *>(m_markerGroups[i]);
		delete[] markerGroup->m_markerGroupName;
		delete markerGroup;
	}

	m_markerGroups.Clear();
}

// ----------------------------------------------------------------------------
BOOL CGM_ObjEdit::MGE_ProcessMouseHighlights()
{
	BOOL markerSelected = FALSE;
	INT32 markerIndex = 0;
	CRuEntity *marker = NULL;
	REAL markerDistance = FLT_MAX;

	markerSelected = g_pRuneDev->FlagEditor_GetFlagAtCursor(g_pRuneDev->GetCursorPtX(), g_pRuneDev->GetCursorPtY(), &markerIndex, &marker, &markerDistance);

	CRuCamera *camera = g_pRuneDev->GetCamera();

	// Get the target position for the pick ray in world coordinates
	CRuVector3 homPt, viewPt, worldPt;
	camera->Coord_ScreenToHomogeneous(g_pGameMain->GetCursorPtX(), g_pGameMain->GetCursorPtY(), g_pRuneDev->GetScreenWidth(), g_pRuneDev->GetScreenHeight(), homPt);
	camera->Coord_HomogeneousToView(homPt, viewPt);
	camera->Coord_ViewToWorld(viewPt, worldPt);

	// Build collision segment
	CRuRay3 colRay(camera->GetCameraPosition(), (worldPt - camera->GetCameraPosition()).Normalize());

	BOOL gizmoHighlight = m_moverGizmo->HighlightManipulator(colRay);

	return markerSelected || gizmoHighlight;
}

// ----------------------------------------------------------------------------
void CGM_ObjEdit::MGE_Update()
{
	if(m_dragJustFinished)
		m_dragJustFinished = FALSE;

	m_moverGizmo->SetCollisionObject(IBaseSprite::GetCollisionContainer());
	m_moverGizmo->SetSnapToCollisionObject(m_UI_Check_SnapToTerrain->IsChecked());

	if(MGE_ProcessMouseHighlights() == FALSE)
	{
		this->m_interface->GetUiWorld()->SetMouseExclude(m_UI_Win);
	}
	else
	{
		this->m_interface->GetUiWorld()->SetMouseExclude(NULL);
	}

	if(m_ME_SelectedMarkerEntity)
	{
		if(m_moverGizmo->GetParent() != m_ME_SelectedMarkerEntity)
		{
			m_moverGizmo->DetachFromParent();
			m_ME_SelectedMarkerEntity->AddChild(m_moverGizmo);
		}
	}

	// Dragging action
	if(m_dragging)
	{
		CRuCamera *camera = g_pRuneDev->GetCamera();

		// Get the target position for the pick ray in world coordinates
		CRuVector3 homPt, viewPt, worldPt;
		camera->Coord_ScreenToHomogeneous(g_pGameMain->GetCursorPtX(), g_pGameMain->GetCursorPtY(), g_pRuneDev->GetScreenWidth(), g_pRuneDev->GetScreenHeight(), homPt);
		camera->Coord_HomogeneousToView(homPt, viewPt);
		camera->Coord_ViewToWorld(viewPt, worldPt);

		// Build collision segment
		CRuRay3 colRay(camera->GetCameraPosition(), (worldPt - camera->GetCameraPosition()).Normalize());

		m_moverGizmo->Manipulate(colRay);

		if(m_ME_SelectedMarkerEntity)
		{
			CRuVector3 pos = m_ME_SelectedMarkerEntity->GetTranslation(ruFRAMETYPE_WORLD);
			CRuVector3 direction(1.0f, 0.0f, 0.0f);
			m_ME_SelectedMarkerEntity->GetWorldTransform().TransformVector(direction);

			MarkerGroup *markerGroup = reinterpret_cast<MarkerGroup *>(m_markerGroups[m_MGE_SelectedGroup]);
			CliFlagPosInfo &flagInfo = markerGroup->m_markers[m_ME_SelectedMarker];

			flagInfo.X = pos.m_x;
			flagInfo.Y = pos.m_y;
			flagInfo.Z = pos.m_z;
			flagInfo.Dir = ConvertDirectionFromVectorToReal(direction);

			// NOTE: We no longer save on update... user must click APPLY button to save
//			ME_SaveFlagInfo(m_MGE_SelectedGroup, m_ME_SelectedMarker);
		}
	}
	// Look for user-entered flag group IDs and update flag group name accordingly
	for(INT32 i = 0; i < 10; ++i)
	{
		if(m_UI_MarkerGroup_Frame[i]->IsVisible() && m_UI_MarkerGroup_Append[i]->IsVisible())
		{
			int flagID = 0;

			char *tmp = WCharToUFT8(m_UI_MarkerGroup_GroupID[i]->GetText());
			if(tmp)
			{
				flagID = atol(tmp);
				delete[] tmp;
			}

			GameObjDbStructEx *dbStruct = g_ObjectData->GetObj(flagID);

			if(dbStruct)
			{
				// Set flag group name
				m_UI_MarkerGroup_GroupName[i]->SetTextA(dbStruct->GetName());
			}
		}
	}
}

// ----------------------------------------------------------------------------
void CGM_ObjEdit::MGE_DeleteFlagGroup(INT32 groupIdx)
{
	if(groupIdx >= 0 && groupIdx < m_markerGroups.Count())
	{
		MarkerGroup *markerGroup = reinterpret_cast<MarkerGroup *>(m_markerGroups[groupIdx]);

		Net_GSrvConnect::DelFlagGroup(markerGroup->m_markerGroupID);

		delete[] markerGroup->m_markerGroupName;
		delete markerGroup;

		m_markerGroups.RemoveAt(groupIdx);
	}
}

// ----------------------------------------------------------------------------
void CGM_ObjEdit::ME_OnAppendClick()
{
	MarkerGroup *markerGroup = reinterpret_cast<MarkerGroup *>(m_markerGroups[m_MGE_SelectedGroup]);

	INT32 editorFrameIdx = 0;
	INT32 flagIDToAppend = 0;

	// Look for the flag ID we want to append
	for(INT32 i = 0; i < 10; ++i)
	{
		if(m_UI_Marker_Frame[i]->IsVisible() && m_UI_Marker_Append[i]->IsVisible())
		{
			editorFrameIdx = i;

			if(GetTextLength(m_UI_Marker_ID[editorFrameIdx]->GetText()) == 0)
			{
				for(INT32 j = 0; j < markerGroup->m_markers.Count(); ++j)
				{
					if(markerGroup->m_markers[j].FlagID >= flagIDToAppend)
					{
						flagIDToAppend = markerGroup->m_markers[j].FlagID + 1;
					}
				}
			}
			else
			{
				flagIDToAppend = ConvertTextToInt(m_UI_Marker_ID[i]->GetText());
			}

			break;
		}
	}

	if(flagIDToAppend >= 0)
	{
		BOOL alreadyExists = FALSE;

		// Ensure there are no existing flag with the same ID
		for(INT32 i = 0; i < markerGroup->m_markers.Count(); ++i)
		{
			if(markerGroup->m_markers[i].FlagID == flagIDToAppend)
			{
				alreadyExists = TRUE;
				break;
			}
		}

		// The flag does not already exist, so create it now
		if(!alreadyExists)
		{
			// Create flag
			CliFlagPosInfo flagInfo;
			flagInfo.GroupID = markerGroup->m_markerGroupID;
			flagInfo.FlagID = flagIDToAppend;

			if(	GetTextLength(m_UI_Marker_X[editorFrameIdx]->GetText()) == 0 ||
				GetTextLength(m_UI_Marker_Y[editorFrameIdx]->GetText()) == 0 ||
				GetTextLength(m_UI_Marker_Z[editorFrameIdx]->GetText()) == 0 ||
				GetTextLength(m_UI_Marker_Dir[editorFrameIdx]->GetText()) == 0)
			{
				CRoleSprite *player = g_pGameMain->GetPlayer();

				if(player)
				{
					CRuVector3 position = player->GetPos();
					CRuVector3 direction = player->GetDirection();

					flagInfo.X = position.m_x;
					flagInfo.Y = position.m_y;
					flagInfo.Z = position.m_z;
					flagInfo.Dir = ConvertDirectionFromVectorToReal(direction);
				}
			}
			else
			{
				flagInfo.X = ConvertTextToReal(m_UI_Marker_X[editorFrameIdx]->GetText());
				flagInfo.Y = ConvertTextToReal(m_UI_Marker_Y[editorFrameIdx]->GetText());
				flagInfo.Z = ConvertTextToReal(m_UI_Marker_Z[editorFrameIdx]->GetText());
				flagInfo.Dir = ConvertTextToReal(m_UI_Marker_Dir[editorFrameIdx]->GetText());
			}

			// Add flag to flag list
			markerGroup->m_markers.Add(flagInfo);

			// Save flag info to server
			ME_SaveFlagInfo(m_MGE_SelectedGroup, markerGroup->m_markers.Count() - 1);

			// Clear editor focus
			m_UI_Marker_ID[editorFrameIdx]->ClearFocus();

			// Refresh marker interface
			m_ME_SelectedMarker = -1;
			RefreshMarkerInterface();
		}
	}
}

// ----------------------------------------------------------------------------
void CGM_ObjEdit::ME_OnDeleteClick(const char *markerName)
{
	for(INT32 i = 0; i < 10; ++i)
	{
		if(m_UI_Marker_Delete[i] && stricmp(m_UI_Marker_Delete[i]->GetName(), markerName) == 0)
		{
			m_markerToBeDeleted = i + m_ME_Top;

			lua_State* L = this->GetLuaState();
			if (L)
			{
				// Initialize the dialog box
				lua_getglobal(L, "StaticPopup_Show");
				lua_pushstring(L, "GM_OBJEDIT_ME_DELETE");
				lua_pushnil(L);
				lua_pushnil(L);
				lua_pushnil(L);
				lua_pcall(L, 4, 1, 0);
				lua_pop(L, 1);
			}

			break;
		}
	}
}

// ----------------------------------------------------------------------------
void CGM_ObjEdit::ME_OnDeleteConfirm()
{
	ME_DeleteFlagInfo(m_MGE_SelectedGroup, m_markerToBeDeleted);

	// Refresh marker interface
	m_ME_SelectedMarker = -1;
	RefreshMarkerInterface();
}

// ----------------------------------------------------------------------------
void CGM_ObjEdit::ME_OnGotoClick()
{
	if(m_MGE_SelectedGroup >= 0 && m_MGE_SelectedGroup < m_markerGroups.Count())
	{
		MarkerGroup *markerGroup = reinterpret_cast<MarkerGroup *>(m_markerGroups[m_MGE_SelectedGroup]);

		if(m_ME_SelectedMarker >= 0 && m_ME_SelectedMarker < markerGroup->m_markers.Count())
		{
			CliFlagPosInfo &flagInfo = markerGroup->m_markers[m_ME_SelectedMarker];

			CRoleSprite* sprite = g_pGameMain->GetPlayer();
			if (sprite)
				sprite->SetPos(CRuVector3(flagInfo.X, flagInfo.Y, flagInfo.Z));
		}
	}
}

// ----------------------------------------------------------------------------
void CGM_ObjEdit::ME_OnApplyClick()
{
	this->ME_SaveAllFlags(m_MGE_SelectedGroup);
}

// ----------------------------------------------------------------------------
void CGM_ObjEdit::ME_OnNextClick()
{
	if(m_MGE_SelectedGroup >= 0 && m_MGE_SelectedGroup < m_markerGroups.Count())
	{
		MarkerGroup *markerGroup = reinterpret_cast<MarkerGroup *>(m_markerGroups[m_MGE_SelectedGroup]);

		if(m_ME_Top + 10 <= markerGroup->m_markers.Count())
		{
			m_ME_Top += 10;
			m_ME_SelectedMarker = -1;

			RefreshMarkerInterface();
		}
	}
}

// ----------------------------------------------------------------------------
void CGM_ObjEdit::ME_OnPreviousClick()
{
	if(m_ME_Top - 10 >= 0)
	{
		m_ME_Top -= 10;
		m_ME_SelectedMarker = -1;

		RefreshMarkerInterface();
	}
}

// ----------------------------------------------------------------------------
void CGM_ObjEdit::ME_OnSelectionChanged(INT32 selectionIdx)
{
	m_ME_SelectedMarker = m_ME_Top + selectionIdx;

	for(INT32 i = 0; i < 10; ++i)
	{
		if(i + m_ME_Top == m_ME_SelectedMarker)
		{
			m_UI_Marker_HighlightFrame[i]->SetVisible(true);
			m_ME_SelectedMarkerEntity = g_pRuneDev->FlagEditor_GetFlagAtIndex(m_ME_SelectedMarker);
		}
		else
		{
			m_UI_Marker_HighlightFrame[i]->SetVisible(false);
		}
	}
}

// ----------------------------------------------------------------------------
void CGM_ObjEdit::ME_SaveFlagInfo(INT32 groupIdx, INT32 flagIdx)
{
	if(groupIdx >= 0 && groupIdx < m_markerGroups.Count())
	{
		MarkerGroup *markerGroup = reinterpret_cast<MarkerGroup *>(m_markerGroups[groupIdx]);

		if(flagIdx >= 0 && flagIdx < markerGroup->m_markers.Count())
		{
			CliFlagPosInfo &flagInfo = markerGroup->m_markers[flagIdx];
			Net_GSrvConnect::EditFlag(flagInfo);
		}
	}
}

// ----------------------------------------------------------------------------
void CGM_ObjEdit::ME_SaveAllFlags(INT32 groupIdx)
{
	if(groupIdx >= 0 && groupIdx < m_markerGroups.Count())
	{
		MarkerGroup *markerGroup = reinterpret_cast<MarkerGroup *>(m_markerGroups[groupIdx]);

		for(INT32 i = 0; i < markerGroup->m_markers.Count(); ++i)
		{
			CliFlagPosInfo &flagInfo = markerGroup->m_markers[i];
			Net_GSrvConnect::EditFlag(flagInfo);
		}
	}
}

// ----------------------------------------------------------------------------
void CGM_ObjEdit::ME_DeleteFlagInfo(INT32 groupIdx, INT32 flagIdx)
{
	if(groupIdx >= 0 && groupIdx < m_markerGroups.Count())
	{
		MarkerGroup *markerGroup = reinterpret_cast<MarkerGroup *>(m_markerGroups[groupIdx]);

		if(flagIdx >= 0 && flagIdx < markerGroup->m_markers.Count())
		{
			CliFlagPosInfo &flagInfo = markerGroup->m_markers[flagIdx];
			Net_GSrvConnect::DelFlag(flagInfo.GroupID, flagInfo.FlagID);
			markerGroup->m_markers.RemoveAt(flagIdx);
		}
	}
}

// ----------------------------------------------------------------------------
BOOL CGM_ObjEdit::ME_SelectMarkerAtCursor()
{
	BOOL markerSelected = FALSE;
	INT32 markerIndex = 0;
	CRuEntity *marker = NULL;
	REAL markerDistance = FLT_MAX;

	markerSelected = g_pRuneDev->FlagEditor_GetFlagAtCursor(g_pRuneDev->GetCursorPtX(), g_pRuneDev->GetCursorPtY(), &markerIndex, &marker, &markerDistance);

	if(markerSelected)
	{
		m_ME_SelectedMarker = markerIndex;
		m_ME_SelectedMarkerEntity = marker;

		ME_AdjustTopToContainSelectedMarker();
		RefreshMarkerInterface(TRUE);
	}

	return markerSelected;
}

// ----------------------------------------------------------------------------
void CGM_ObjEdit::ME_AdjustTopToContainSelectedMarker()
{
	m_ME_Top = (m_ME_SelectedMarker / 10) * 10;
}

// ----------------------------------------------------------------------------
void CGM_ObjEdit::RefreshInterfaceLayout()
{
	// Clear editor status
	ResetEditorFields();

	// Update interface visibility
	switch(m_objEditMode)
	{
		case OBJEDITMODE_NPC:
			{
				m_UI_Palette->SetSize(CUiSize(1132, 208), true);

				m_UI_NPCPropertiesFrame->SetVisible(true);
				m_UI_WaypointEditorFrame->SetVisible(true);
				m_UI_MarkerGroupFrame->SetVisible(false);
				m_UI_MarkersFrame->SetVisible(false);

				m_UI_CreateButton->SetVisible(true);
				m_UI_DeleteButton->SetVisible(true);
				m_UI_ApplyButton->SetVisible(true);
			}

			break;

		case OBJEDITMODE_MARKER:
			{
				m_UI_Palette->SetSize(CUiSize(790, 330), true);

				m_UI_NPCPropertiesFrame->SetVisible(false);
				m_UI_WaypointEditorFrame->SetVisible(false);
				m_UI_MarkerGroupFrame->SetVisible(true);
				m_UI_MarkersFrame->SetVisible(true);

				m_UI_CreateButton->SetVisible(false);
				m_UI_DeleteButton->SetVisible(false);
				m_UI_ApplyButton->SetVisible(false);
			}

			break;
	}
}

// ----------------------------------------------------------------------------
void CGM_ObjEdit::ResetEditorFields()
{
	if(m_selectedSprite)
	{
		g_pRuneDev->ForceDeleteSprite((CRoleSprite *) m_selectedSprite);
	}

	// Clear stored commands
	delete[] m_currentCreateCommand;
	m_currentCreateCommand = NULL;

	delete[] m_currentDeleteCommand;
	m_currentDeleteCommand = NULL;

	// Clear editor status
	m_lastOrgID				= 0;
	m_spriteHasPrecedence = TRUE;
	m_dragging = FALSE;
	m_selectedSprite = NULL;
	m_selectedSpriteID = 0;

	m_selectedWaypointIdx = 0;
	m_selectedWaypoint = NULL;

	m_MGE_Top = 0;
	m_MGE_SelectedGroup = -1;
	m_ME_Top = 0;
	m_ME_SelectedMarker = -1;
	m_ME_SelectedMarkerEntity = NULL;

	m_moverGizmo->DetachFromParent();

	// Reset all text boxes
	m_UI_ObjID->SetText(L"");
	m_UI_OrgName->SetText(L"");
	m_UI_GItemID->SetText(L"");
	m_UI_Name->SetText(L"");
	m_UI_ObjClass->SetText(L"");
	m_UI_PID->SetText(L"");
	m_UI_AutoPlot->SetText(L"");
	m_UI_QuestID->SetText(L"");

	if ( m_UI_OrientingAxisX ) m_UI_OrientingAxisX->SetText(L"");
	if ( m_UI_OrientingAxisY ) m_UI_OrientingAxisY->SetText(L"");
	if ( m_UI_OrientingAxisZ ) m_UI_OrientingAxisZ->SetText(L"");

	for(INT32 i = 0; i < 32; ++i)
	{
		if(m_UI_CheckMode[i] == NULL)
			continue;

		m_UI_CheckMode[i]->SetChecked(false);
	}

	for(INT32 i = 0; i < 64; ++i)
	{
		if(m_UI_Waypoint_Frame[i] == NULL)
			continue;

		m_UI_Waypoint_Frame[i]->SetVisible(false);
	}

	g_pRuneDev->FlagEditor_ClearAll();
}

// ----------------------------------------------------------------------------
INT32 CGM_ObjEdit::GetTextLength(const WCHAR *text)
{
	return (INT32) wcslen(text);
}

// ----------------------------------------------------------------------------
INT32 CGM_ObjEdit::ConvertTextToInt(const WCHAR *text)
{
	INT32 num = 0;

	char *tmp = WCharToUFT8(text);
	if(tmp)
	{
		num = atol(tmp);
		delete[] tmp;
	}

	return num;
}

// ----------------------------------------------------------------------------
REAL CGM_ObjEdit::ConvertTextToReal(const WCHAR *text)
{
	REAL num = 0;

	char *tmp = WCharToUFT8(text);
	if(tmp)
	{
		num = static_cast<REAL>(atof(tmp));
		delete[] tmp;
	}

	return num;
}

// ----------------------------------------------------------------------------
REAL CGM_ObjEdit::ConvertDirectionFromVectorToReal(const CRuVector3 &direction)
{
	// 取得角色所朝方向
	REAL theta = acos(direction.Dot(CRuVector3(1.0f, 0.0f, 0.0f)));
	if(direction.m_z > 0.0f)
		theta = PI + PI - theta;

	// 轉換為角度
	theta = theta * 180.0f / PI;

	return theta;
}

// ************************************************************************************************************************************************************

// ----------------------------------------------------------------------------
int LuaFunc_GM_ObjEdit_OnLoad(lua_State* L)
{
	g_pGM_ObjEdit->OnLoad();
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GM_ObjEdit_OnShow(lua_State* L)
{
	g_pGM_ObjEdit->OnShow();
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GM_ObjEdit_OnUpdate(lua_State* L)
{
	g_pGM_ObjEdit->OnUpdate();
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GM_ObjEdit_OnToggleModeClick(lua_State* L)
{
	g_pGM_ObjEdit->OnToggleModeClick();
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GM_ObjEdit_OnClick(lua_State* L)
{
	g_pGM_ObjEdit->OnClick();
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GM_ObjEdit_OnDragStart(lua_State* L)
{
	g_pGM_ObjEdit->OnDragStart();
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GM_ObjEdit_OnReceiveDrag(lua_State* L)
{
	g_pGM_ObjEdit->OnReceiveDrag();
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GM_ObjEdit_OnDragStop(lua_State* L)
{
	g_pGM_ObjEdit->OnDragStop();
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GM_ObjEdit_OnValueChanged(lua_State* L)
{
	g_pGM_ObjEdit->UpdateNPCMoveInfoFromInterface();
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GM_ObjEdit_OnResetClick(lua_State* L)
{
	g_pGM_ObjEdit->OnResetClick();
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GM_ObjEdit_OnCreateMacroClick(lua_State* L)
{
	g_pGM_ObjEdit->OnCreateMacroClick();
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GM_ObjEdit_OnRotateLeftClick(lua_State* L)
{
	g_pGM_ObjEdit->OnRotateLeftClick();
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GM_ObjEdit_OnRotateRightClick(lua_State* L)
{
	g_pGM_ObjEdit->OnRotateRightClick();
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GM_ObjEdit_OnAppendWaypointClick(lua_State* L)
{
	g_pGM_ObjEdit->OnAppendWaypointClick();
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GM_ObjEdit_OnDeleteWaypointClick(lua_State* L)
{
	const char *waypointName = luaL_checkstring(L, 1);

	g_pGM_ObjEdit->OnDeleteWaypointClick(waypointName);

	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GM_ObjEdit_OnDeleteWaypointConfirm(lua_State* L)
{
	g_pGM_ObjEdit->OnDeleteWaypointConfirm();

	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GM_ObjEdit_OnCreateClick(lua_State* L)
{
	g_pGM_ObjEdit->OnCreateClick();
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GM_ObjEdit_OnDeleteClick(lua_State* L)
{
	g_pGM_ObjEdit->OnDeleteClick();
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GM_ObjEdit_OnDeleteConfirm(lua_State* L)
{
	g_pGM_ObjEdit->OnDeleteConfirm();
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GM_ObjEdit_OnApplyClick(lua_State* L)
{
	g_pGM_ObjEdit->OnApplyClick();
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GM_ObjEdit_OnClose(lua_State* L)
{
	g_pGM_ObjEdit->OnClose();
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GM_ObjEdit_Translate(lua_State* L)
{
	int axisIdx = luaL_checkint(L, 1);

	g_pGM_ObjEdit->NPC_TranslateTarget(axisIdx);

	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GM_ObjEdit_MGE_OnAppendClick(lua_State* L)
{
	g_pGM_ObjEdit->MGE_OnAppendClick();
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GM_ObjEdit_MGE_OnDeleteClick(lua_State* L)
{
	const char *markerGroupName = luaL_checkstring(L, 1);
	g_pGM_ObjEdit->MGE_OnDeleteClick(markerGroupName);
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GM_ObjEdit_MGE_OnDeleteConfirm(lua_State* L)
{
	g_pGM_ObjEdit->MGE_OnDeleteConfirm();
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GM_ObjEdit_MGE_OnNextClick(lua_State* L)
{
	g_pGM_ObjEdit->MGE_OnNextClick();
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GM_ObjEdit_MGE_OnPreviousClick(lua_State* L)
{
	g_pGM_ObjEdit->MGE_OnPreviousClick();
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GM_ObjEdit_MGE_OnReloadClick(lua_State* L)
{
	g_pGM_ObjEdit->MGE_OnReloadClick();
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GM_ObjEdit_MGE_OnSelectionChanged(lua_State* L)
{
	int selectionIdx = luaL_checkint(L, 1);
	g_pGM_ObjEdit->MGE_OnSelectionChanged(selectionIdx - 1);
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GM_ObjEdit_ME_OnAppendClick(lua_State* L)
{
	g_pGM_ObjEdit->ME_OnAppendClick();
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GM_ObjEdit_ME_OnDeleteClick(lua_State* L)
{
	const char *markerName = luaL_checkstring(L, 1);
	g_pGM_ObjEdit->ME_OnDeleteClick(markerName);
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GM_ObjEdit_ME_OnDeleteConfirm(lua_State* L)
{
	g_pGM_ObjEdit->ME_OnDeleteConfirm();
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GM_ObjEdit_ME_OnGotoClick(lua_State* L)
{
	g_pGM_ObjEdit->ME_OnGotoClick();
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GM_ObjEdit_ME_OnApplyClick(lua_State* L)
{
	g_pGM_ObjEdit->ME_OnApplyClick();
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GM_ObjEdit_ME_OnNextClick(lua_State* L)
{
	g_pGM_ObjEdit->ME_OnNextClick();
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GM_ObjEdit_ME_OnPreviousClick(lua_State* L)
{
	g_pGM_ObjEdit->ME_OnPreviousClick();
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GM_ObjEdit_ME_OnSelectionChanged(lua_State* L)
{
	int selectionIdx = luaL_checkint(L, 1);
	g_pGM_ObjEdit->ME_OnSelectionChanged(selectionIdx - 1);
	return 0;
}

// ************************************************************************************************************************************************************
