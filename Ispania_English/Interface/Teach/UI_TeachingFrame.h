
#pragma once
#include "../interface.h"
struct TeachTableStruct;
struct ColoringStruct;
class CUiPlayerModel;
struct ColoringShopStruct;









class CUI_TeachingFrame  :public CInterfaceBase 
{

	
public:


	CUI_TeachingFrame(CInterface* object ) ;
	~CUI_TeachingFrame(void);

	void Load();
	void Save();

	void BuildFrames(void);
	void DestroyFrames(void);

	void R_ColoringShopResult(int  Result);
	void R_BodyShopResult(bool  Result);

	void VehicleItemColorApply(int* color);

	void GetVehicleItemColor(int* color);

	void ApplyBodySetting(void);
	int GetPlayerBoneScale(int index);
	void SetPlayerBoneScale(int index,int scale);

	void GetVehicleInfo(bool& lock,bool& invalid);

	void SetPlayer(CUiModel* player);
	void getGlobalPath(const char* file,char *buffer,int size);
	void getLocalPath(const char* file,char *buffer,int size);

	virtual	void			RegisterFunc			();	
	void RunGlobalPlot(int index);
	void EndRunGlobalPlot(int R);

	void getResourcePath(char *buffer,int size);

	//ColoringShopStruct* GetColoringShopStructByID(int id);

	//CUiPlayerModel * GetModelPlayer(void);
	CUiModel*  GetModelPlayer(void);

	//取得坐騎物品
	int GetClipboardVehicleItem(void);
	int RemoveVehicleItem(void);
	
	int GetTeachtCount(void);
	TeachTableStruct* GetTeachInfo(unsigned int i); 
public:
	ColoringStruct* m_beuatyStdio;
	//CUiPlayerModel *m_player;
	CUiModel* m_player;

	CUiModel* m_vehicle;

	int m_boneScale[11];


	

	int m_ItemPos;
	
	int m_runPlot;

};
extern CUI_TeachingFrame*		g_pTeachingFrame;