#pragma once

#include <map>
#include <string>
#include "uiframe.h"

#define	MAX_MODEL_PAPERDOLL_PARTS	10

int Model_SetUnit			(lua_State* L);				// 外部實做
int	Model_SetPet			(lua_State* L);				// 設定寵物外型
using namespace std;

// ----------------------------------------------------------------------------------
class CUiLuaModel
{
public:
	static	void			OpenLib(lua_State* L);
	static	int				SetMetaTable(lua_State* L);
};

struct ModelAttachEntity
{
	ModelAttachEntity() { m_entity = NULL; m_enchantEntity = NULL; }
	ModelAttachEntity(CRuEntity* entity, CRuEntity* enchantEntity) { m_entity = entity; m_enchantEntity = enchantEntity; }

	CRuEntity* m_entity;
	CRuEntity* m_enchantEntity;
};

// ----------------------------------------------------------------------------------
class CUiModel : public CUiFrame
{
public:
	CUiModel(const char* name, CUiLayout* pParent, CUi* pUi);
	virtual ~CUiModel();

	

	virtual	void			SetLuaMetaTable(lua_State* l)			{ CUiLuaModel::SetMetaTable(l); }

	virtual	void			CreateEntity(CRuEntity* entity, CRuFusion_Mime* mime);
	virtual	void			CreateEntity(const char* actFilename);	
	virtual	void			ReleaseEntity();
	virtual	void			AttachEntity(const char* point, const char* actFilename, const char* enchantFilename=NULL);
	virtual void			ReleaseAttachEntity();
	virtual	void			StartMotion(const char* motion = NULL);
	virtual	void			SetEntityDiffuse(DWORD color);
	virtual void			SetEntityRotate(const CRuVector3 &axisOfRotation, REAL rotationAngle);

	virtual	void			SetPaperdollPart(const char* partName, const char* name);
	virtual	void			GetPaperdollPart(const char* partName,  char* name);
	virtual	void			SetPaperdollColor(const char* partName, int color1, int color2);
	virtual	void			GetPaperdollColor(const char* partName, int* color);

	virtual void			SetDyeFXColors(int colors[4]);
	virtual int				GetMaterialCount();
	virtual bool			CanDyeFXColors(int layer, bool outExists[4]);	
	virtual void			BuildDyeFXMask();

	virtual	void			SetBoneScale(const char* boneName, float scale);
	virtual	float			GetBoneScale(const char* boneName);
	virtual bool			InserLinkFrame(const char* linkName, CRuVector3 point);

	virtual	void			SetTargetBone(const char* targetBoneName);
	virtual	void			SetAlignmentFrames(const char* top, const char* bottom, float bias=0.5f, float scale=0.5f);
	virtual	void			SetTargetRotate(float rotateAngel);
	virtual	void			SetRelativeCameraPosition(const CRuVector3 &relativeCamPos);	
	virtual	void			SetMaskFile(const char* maskFile);
	virtual	void			SetModelOffset(CRuVector3& offset);
	virtual void			SetDefaultTexture(CUiTexture* defaultTexture);
	virtual	CRuEntity*		GetRuEntity()							{ return m_ruEntity; }
	virtual CUiTexture*		GetDefaultTexture()						{ return m_defaultTexture; }

	virtual int				GetMemSize();

protected:
	virtual	void			OnPaint(float alpha);
	virtual	void			OnUpdate(float elapsedTime);
	virtual void			OnShow(lua_State* L);

protected:
	CRuEntity*					m_ruParentEntity;
	CRuEntity*					m_ruEntity;
	CRuFusion_Mime*				m_ruFusionMime;
	CRuFX2D_Model*				m_ruFx2DModel;
	IRuTexture*					m_maskTexture;
	CUiTexture*					m_defaultTexture;

	CRuVector3					m_cameraPosition;
	CRuVector3					m_modelOffset;

	bool						m_dyeFXMaskRequiresLoad;

	map<string, ModelAttachEntity> m_attachEntity;

	struct {
		string					m_top;
		string					m_bottom;		
		float					m_bias;
		float					m_scale;
	}m_alignment;
};

extern CRuFusion_Mime_Template*	g_ruMimeTemplate;
extern int						g_mimeRefCount;
extern const char* g_partName[MAX_MODEL_PAPERDOLL_PARTS];