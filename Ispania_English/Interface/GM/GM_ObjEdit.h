#pragma once

#include "../interface.h"
#include "../sprite/sprite.h"							// ¨¤¦â,¯S®Ä

// ************************************************************************************************************************************************************

class CGM_ObjEdit;
extern CGM_ObjEdit*				g_pGM_ObjEdit;

// ************************************************************************************************************************************************************

class CGM_ObjEdit : public CInterfaceBase
{
protected:
	enum ObjEditMode
	{
		OBJEDITMODE_NPC			= 0,
		OBJEDITMODE_MARKER		= 1
	};

	CUiFrame*					m_UI_Win;
	CUiFrame*					m_UI_Palette;

	CUiFrame*					m_UI_OrientationFrame;
	CUiFrame*					m_UI_NPCPropertiesFrame;
	CUiFrame*					m_UI_WaypointEditorFrame;
	CUiFrame*					m_UI_MarkerGroupFrame;
	CUiFrame*					m_UI_MarkersFrame;

	CUiEditBox*					m_UI_ObjID;
	CUiEditBox*					m_UI_OrgName;
	CUiEditBox*					m_UI_GItemID;
	CUiEditBox*					m_UI_Name;
	CUiEditBox*					m_UI_ObjClass;
	CUiEditBox*					m_UI_PID;
	CUiEditBox*					m_UI_AutoPlot;
	CUiEditBox*					m_UI_QuestID;
	CUiCheckButton*				m_UI_CheckMode[32];

	CUiEditBox*					m_UI_OrientingAxisX;
	CUiEditBox*					m_UI_OrientingAxisY;
	CUiEditBox*					m_UI_OrientingAxisZ;

	CUiButton*					m_UI_TranslateUp;
	CUiButton*					m_UI_TranslateDown;
	CUiButton*					m_UI_TranslateLeft;
	CUiButton*					m_UI_TranslateRight;

	CUiButton*					m_UI_RotateLeft;
	CUiButton*					m_UI_RotateRight;

	CUiCheckButton*				m_UI_Radio_ViewFrame;
	CUiCheckButton*				m_UI_Radio_ObjectFrame;
	CUiCheckButton*				m_UI_Check_SnapToNormal;
	CUiCheckButton*				m_UI_Check_SnapToTerrain;

	CUiScrollFrame*				m_UI_Waypoint_ScrollFrame;
	CUiFrame*					m_UI_Waypoint_Frame[64];
	CUiEditBox*					m_UI_Waypoint_Range[64];
	CUiFrame*					m_UI_Waypoint_MoveType[64];
	CUiEditBox*					m_UI_Waypoint_WaitBase[64];
	CUiEditBox*					m_UI_Waypoint_WaitRand[64];
	CUiFrame*					m_UI_Waypoint_DirMode[64];
	CUiEditBox*					m_UI_Waypoint_ActionType[64];
	CUiEditBox*					m_UI_Waypoint_Script[64];
	CUiButton*					m_UI_Waypoint_Delete[64];
	CUiButton*					m_UI_Waypoint_Append[64];

	CUiFrame*					m_UI_MarkerGroup_Frame[10];
	CUiFrame*					m_UI_MarkerGroup_HighlightFrame[10];
	CUiEditBox*					m_UI_MarkerGroup_GroupID[10];
	CUiEditBox*					m_UI_MarkerGroup_GroupName[10];
	CUiButton*					m_UI_MarkerGroup_Delete[10];
	CUiButton*					m_UI_MarkerGroup_Append[10];

	CUiFrame*					m_UI_Marker_Frame[10];
	CUiFrame*					m_UI_Marker_HighlightFrame[10];
	CUiEditBox*					m_UI_Marker_ID[10];
	CUiEditBox*					m_UI_Marker_X[10];
	CUiEditBox*					m_UI_Marker_Y[10];
	CUiEditBox*					m_UI_Marker_Z[10];
	CUiEditBox*					m_UI_Marker_Dir[10];
	CUiButton*					m_UI_Marker_Delete[10];
	CUiButton*					m_UI_Marker_Append[10];

	CUiButton*					m_UI_CreateButton;
	CUiButton*					m_UI_DeleteButton;
	CUiButton*					m_UI_ApplyButton;

	ObjEditMode					m_objEditMode;

	BOOL						m_refreshingInterface;
	BOOL						m_spriteHasPrecedence;			
	BOOL						m_dragging;						

	INT32						m_lastOrgID;
	char*						m_currentCreateCommand;
	char*						m_currentDeleteCommand;

	IBaseSprite*				m_selectedSprite;
	INT32						m_selectedSpriteID;
	void*						m_selectedSpriteModifyInfo;
	void*						m_selectedSpriteMoveInfo;

	INT32						m_selectedWaypointIdx;
	CRuEntity*					m_selectedWaypoint;

	CRuArrayList<void *>		m_markerGroups;

	INT32						m_MGE_Top;
	INT32						m_MGE_SelectedGroup;
	INT32						m_ME_Top;
	INT32						m_ME_SelectedMarker;
	CRuEntity*					m_ME_SelectedMarkerEntity;

	CRuGizmo_Mover*				m_moverGizmo;

	BOOL						m_dragJustFinished;
	INT32						m_waypointToBeDeleted;
	INT32						m_markerGroupToBeDeleted;
	INT32						m_markerToBeDeleted;

public:
								CGM_ObjEdit(CInterface* object);
	virtual						~CGM_ObjEdit();

	// Initialization
	virtual	void				RegisterFunc();
	virtual void				Update(float elapsedTime);

	// Other stuff
	void						UpdateModifyNPCInfo(void *info);
	void						UpdateModifyNPCInfoFromInterface();
	void						UpdateNPCMoveInfo(INT32 objID, vector<NPC_MoveBaseStruct> &moveList);
	void						UpdateNPCMoveInfoFromInterface();
	void						UpdateMarkerList(int gItemID, void *vecFlagList);

	void						RefreshNPCMoveInfoInterface();
	void						RefreshMarkerGroupInterface();
	void						RefreshMarkerInterface(BOOL refreshDisplayOnly = FALSE);
	
	virtual	bool				IsVisible();

	// Root frame events
	virtual	void				OnLoad();
	virtual void				OnShow();
	virtual	void				OnUpdate();
	virtual	void				OnClose();

	// Root frame mouse handling
	virtual	void				OnClick();
	virtual	void				OnDragStart();
	virtual	void				OnReceiveDrag();
	virtual	void				OnDragStop();

	// Root frame button handling
	virtual void				OnToggleModeClick();

	// Orientation frame
	virtual	void				OnRotateLeftClick();
	virtual	void				OnRotateRightClick();

	// NPC info & move info frame
	virtual	void				OnResetClick();
	virtual void				OnCreateMacroClick();

	virtual void				OnAppendWaypointClick();
	virtual	void				OnDeleteWaypointClick(const char *waypointName);
	virtual	void				OnDeleteWaypointConfirm();

	virtual	void				OnCreateClick();
	virtual	void				OnDeleteClick();
	virtual	void				OnDeleteConfirm();
	virtual	void				OnApplyClick();

	BOOL						NPC_SelectTargetAtCursor();
	BOOL						NPC_ProcessMouseHighlights();

	void						NPC_TranslateTarget(INT32 axisIdx);
	void						NPC_DeleteWaypointAtIndex(INT32 idx);

	void						NPC_Update();

	// Marker group editor
	void						MGE_OnAppendClick();
	void						MGE_OnDeleteClick(const char *markerGroupName);
	void						MGE_OnDeleteConfirm();
	void						MGE_OnNextClick();
	void						MGE_OnPreviousClick();
	void						MGE_OnReloadClick();
	void						MGE_OnSelectionChanged(INT32 selectionIdx);

	void						MGE_ClearMarkers();

	BOOL						MGE_ProcessMouseHighlights();

	void						MGE_Update();

	void						MGE_DeleteFlagGroup(INT32 groupIdx);

	// Marker editor
	void						ME_OnAppendClick();
	void						ME_OnDeleteClick(const char *markerName);
	void						ME_OnDeleteConfirm();
	void						ME_OnGotoClick();
	void						ME_OnApplyClick();
	void						ME_OnNextClick();
	void						ME_OnPreviousClick();
	void						ME_OnSelectionChanged(INT32 selectionIdx);

	void						ME_SaveFlagInfo(INT32 groupIdx, INT32 flagIdx);
	void						ME_SaveAllFlags(INT32 groupIdx);
	void						ME_DeleteFlagInfo(INT32 groupIdx, INT32 flagIdx);

	BOOL						ME_SelectMarkerAtCursor();
	void						ME_AdjustTopToContainSelectedMarker();

	// Global support
	void						RefreshInterfaceLayout();
	void						ResetEditorFields();

	INT32						GetTextLength(const WCHAR *text);
	INT32						ConvertTextToInt(const WCHAR *text);
	REAL						ConvertTextToReal(const	WCHAR *text);
	REAL						ConvertDirectionFromVectorToReal(const CRuVector3 &direction);
};

// ************************************************************************************************************************************************************
