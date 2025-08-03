#include <windows.h>
#include "uimodel.h"

CRuFusion_Mime_Template*		g_ruMimeTemplate = NULL;
CRuFusion_Mime_Template*		g_ruMimeTemplate_Deprecated = NULL;
int								g_mimeRefCount = 0;

const char* g_partName[MAX_MODEL_PAPERDOLL_PARTS] = {
	"head", "hair", "helmet", "shoulder", "torso", "hand", "belt", "leg", "foot", "back"
};

// ----------------------------------------------------------------------------------
CUiModel::CUiModel(const char* name, CUiLayout* pParent, CUi* pUi)
	: CUiFrame(name, pParent, pUi)
{
	m_uiType			= eUiType_Model;

	m_ruParentEntity	= NULL;
	m_ruEntity			= NULL;
	m_ruFusionMime		= NULL;
	m_maskTexture		= NULL;
	m_ruFx2DModel		= ruNEW CRuFX2D_Model();

	m_alignment.m_top		= "p_top";
	m_alignment.m_bottom	= "p_down";
	m_alignment.m_bias		= 0.5f;
	m_alignment.m_scale		= 0.5f;
	m_defaultTexture		= NULL;

	if ( g_mimeRefCount == 0 ) {
		g_mimeRefCount++;
		g_ruMimeTemplate = ruNEW CRuFusion_Mime_Template();
		g_ruMimeTemplate_Deprecated = ruNEW CRuFusion_Mime_Template();
		g_ruMimeTemplate_Deprecated->InitializeDeprecatedBoneRotators();
		g_ruMimeTemplate_Deprecated->InitializeDeprecatedBoneScalers();
	}

	//m_cameraPosition.Set(0.5f, -0.10f, 1.0f);
}

// ----------------------------------------------------------------------------------
CUiModel::~CUiModel()
{
	m_defaultTexture = NULL;

	ruSAFE_RELEASE(m_ruFx2DModel);

	ReleaseEntity();

	g_mimeRefCount--;
	if ( g_mimeRefCount <= 0 ) {
		g_mimeRefCount = 0;
		ruSAFE_RELEASE(g_ruMimeTemplate);
		ruSAFE_RELEASE(g_ruMimeTemplate_Deprecated);
	}
}

// ----------------------------------------------------------------------------------
void CUiModel::CreateEntity(CRuEntity* entity, CRuFusion_Mime* mime)
{
	const char* filename = NULL;

	if ( entity ) {
		filename = entity->GetResourceName();
	}

	CreateEntity(filename);

	// 複製紙娃娃資料
	if ( m_ruEntity )
	{
		IRuPaperdoll* sourcePaperdoll = RuEntity_FindFirstPaperdoll(entity);
		IRuPaperdoll* targetPaperdoll = RuEntity_FindFirstPaperdoll(m_ruEntity);

		if ( sourcePaperdoll && targetPaperdoll )
		{
			// HACK: Check to see how many models are attached under the source paperdoll, if more than one, then the model which is not a composite model is a cached model
			CRuEntity *curChild = sourcePaperdoll->GetFirstChild();
			while(curChild)
			{
				if(curChild->GetType().IsSubClassOf(CRuCompositeModel::Type()) == FALSE)
				{
					break;
				}

				// Advance to next sibling
				curChild = curChild->GetNextSibling();
			}

			if(curChild)
			{
				CRuEntity *cachedModelClone = static_cast<CRuEntity *>(curChild->Clone());
				targetPaperdoll->AddChild(cachedModelClone);
				ruSAFE_RELEASE(cachedModelClone);
			}
			else
			{
				BOOL layerActivated[4];
				RUCOLOR layerColor[4];			

				// 設定紙娃娃顏色與部位外觀
				for ( int i = 0; i < MAX_MODEL_PAPERDOLL_PARTS; i++ )
				{
					targetPaperdoll->SetComponent(g_partName[i], sourcePaperdoll->GetComponent(g_partName[i]));
					if ( sourcePaperdoll->GetComponentColors(g_partName[i], layerActivated, layerColor) )
						targetPaperdoll->SetComponentColors(g_partName[i], layerActivated, layerColor);
				}
				RuPaperdoll_Build(m_ruEntity);
			}
		}
		
		// Set Mime
		if ( mime )
		{
			const char* boneScaleName;
			CRuVector3 boneScale;
			for ( int i = 0; i < mime->GetNumBoneScalers(); i++ ) {
				boneScaleName = mime->GetBoneScalerNameByIndex(i);
				if ( mime->GetBoneScalerParametersByIndex(i, boneScale) )
				{
					m_ruFusionMime->SetBoneScalerParameters(boneScaleName, boneScale);
				}
			}

			//RuMime_SetBoneScale(m_pRuMimeEntity, boneName, scale);	
		}
	}
}
float CUiModel::GetBoneScale(const char* boneName)
{

	CRuVector3 boneScale(1,1,1);
	if (m_ruFusionMime)
	{
		const char* boneScaleName;
		bool get=false;
	
		for ( int i = 0; i < m_ruFusionMime->GetNumBoneScalers(); i++ ) 
		{
			boneScaleName = m_ruFusionMime->GetBoneScalerNameByIndex(i);


			if(stricmp(boneName, "Body") == 0)
			{
				if (!strcmp(boneScaleName,"Body Scale"))
				{
					get=true;				
					
				}
				
			}
			else if(stricmp(boneName, "Head") == 0)
			{
				if (!strcmp(boneScaleName,"Head Scale"))
				{
					get=true;				
					
				}				
			}
			else if(stricmp(boneName, "Chest") == 0)
			{
				if (!strcmp(boneScaleName,"Chest Scale"))
				{
					get=true;				
					
				}			
				
			}
			else if(stricmp(boneName, "Pelvis") == 0)
			{
				if (!strcmp(boneScaleName,"Butt Scale"))
				{
					get=true;				
					
				}			
				
			}
			else if(stricmp(boneName, "Arms") == 0)
			{
				if (!strcmp(boneScaleName,"Left Arm Scale"))
				{
					get=true;				
					
				}			
			//	else if (!strcmp(boneScaleName,"Right Arm Scale"))
			//	{
			//		get=true;				
			//		
			//	}		
				
			}
			else if(stricmp(boneName, "Forearm") == 0)
			{
				if (!strcmp(boneScaleName,"Left Forearm Scale"))
				{
					get=true;				
					
				}		
			//	else if (!strcmp(boneScaleName,"Right Forearm Scale"))
			//	{
			//		get=true;				
			//		
			//	}		
				
			}
			else if(stricmp(boneName, "Hand") == 0)
			{
				if (!strcmp(boneScaleName,"Left Hand Scale"))
				{
					get=true;				
					
				}	
			//	else if (!strcmp(boneScaleName,"Right Hand Scale"))
			//	{
			//		get=true;				
			//		
			//	}		
				
			}
			else if(stricmp(boneName, "Legs") == 0)
			{
				if (!strcmp(boneScaleName,"Left Leg Scale"))
				{
					get=true;				
					
				}
			//	else if (!strcmp(boneScaleName,"Right Leg Scale"))
			//	{
			//		get=true;				
					
			//	}		
				
			}
			else if(stricmp(boneName, "Calf") == 0)
			{
				if (!strcmp(boneScaleName,"Left Calf Scale"))
				{
					get=true;				
					
				}
			//	else if (!strcmp(boneScaleName,"Right Calf Scale"))
			//	{
			//		get=true;				
				
			//	}		
				
			}
			else if(stricmp(boneName, "Foot") == 0)
			{
				if (!strcmp(boneScaleName,"Left Foot Scale"))
				{
					get=true;									

				}	
			//	else if (!strcmp(boneScaleName,"Right Foot Scale"))
			//	{
			//		get=true;				
					
			//	}		

				
			}
			else if(stricmp(boneName, "Breasts") == 0)
			{
				if (!strcmp(boneScaleName,"Left Breast Scale"))
				{
					get=true;				
					
				}	
			//	else if (!strcmp(boneScaleName,"Right Breast Scale"))
			//	{
			//		get=true;				
				
			//	}		
				
			}		
			if (get )
			{
				m_ruFusionMime->GetBoneScalerParametersByIndex(i, boneScale) ;
				break;
			}

		}


	}
return boneScale.m_z;
}

// ----------------------------------------------------------------------------------
bool CUiModel::InserLinkFrame(const char* linkName, CRuVector3 offset)
{
	CRuEntity* parentEntity = (m_ruParentEntity) ? m_ruParentEntity : m_ruEntity;

	// 未產生 Entity
	if ( parentEntity == NULL || linkName == NULL )
		return false;

	// 檢查參考點名稱是否已經存在
	CRuEntity* frame = RuEntity_FindLinkFrame(parentEntity, linkName, TRUE);
	if ( frame )
		return false;

	if ( m_ruParentEntity == NULL )
	{
		// 額外產生父元件
		m_ruParentEntity = ruNEW CRuEntity();
	
		// 額外多產生一個中間層
		CRuLinkFrame* linkTemp = ruNEW CRuLinkFrame();
		linkTemp->SetLinkName("linkTemp");

		linkTemp->AddChild(m_ruEntity);

		m_ruParentEntity->AddChild(linkTemp);
		ruSAFE_RELEASE(linkTemp);

		m_ruFx2DModel->SetModel(m_ruParentEntity);
	}

	CRuEntity* firstChild = m_ruParentEntity->GetFirstChild();

	// 產生參考點
	CRuLinkFrame* newLinkFrame = ruNEW CRuLinkFrame();
	newLinkFrame->SetLinkName(linkName);
	newLinkFrame->SetTranslation(offset);

	firstChild->AddChild(newLinkFrame);
	ruSAFE_RELEASE(newLinkFrame);

    return true;
}

// ----------------------------------------------------------------------------------
void CUiModel::CreateEntity(const char* actFilename)
{
	ReleaseEntity();
	if ( actFilename )
	{
		if ( (m_ruEntity = g_ruResourceManager->LoadEntity(actFilename)) != NULL )
		{
			m_ruFusionMime = ruNEW CRuFusion_Mime();
			m_ruFusionMime->SetMimeTemplate(g_ruMimeTemplate);
			m_ruFusionMime->SetTarget(m_ruEntity);

			// If the hierarchy does not have two spine segments, use the deprecated template
			CRuHierarchy_GR2 *hierarchy = RuEntity_FindFirstHierarchy_GR2(m_ruEntity);
			if(hierarchy && hierarchy->GetSubNodeIndexByName("Bip01 Spine2") == -1)
			{
				m_ruFusionMime->SetMimeTemplate(g_ruMimeTemplate_Deprecated);
			}

			RuEntity_SetHierarchyIgnoreVisibilityCheck(m_ruEntity, TRUE);
			StartMotion();

			{
				int color[4] = { 0, 0, 0, 0 };
				SetDyeFXColors(color);
			}			
		}
	}

	if ( m_ruParentEntity )
		m_ruFx2DModel->SetModel(m_ruParentEntity);
	else
		m_ruFx2DModel->SetModel(m_ruEntity);
	//m_ruFx2DModel->SetModelPosition(1024.0f, 768.0f, 1.0f);
	m_ruFx2DModel->SetModelScale(1.0f);
	m_ruFx2DModel->SetModelOffset(m_modelOffset);
	m_ruFx2DModel->SetAlignmentFrames(m_alignment.m_top.c_str(), m_alignment.m_bottom.c_str(), m_alignment.m_bias, m_alignment.m_scale);

	if ( m_maskTexture )
		m_ruFx2DModel->SetMaskingTexture(m_maskTexture);

	if ( m_defaultTexture )
		m_defaultTexture->SetVisible(m_ruEntity == NULL);


	/*
	//在200位置增加參考點
	if( m_ruEntity )
	{
		CRuLinkFrame* linkTemp = ruNEW CRuLinkFrame();
		linkTemp->SetLinkName("linkTemp");

		CRuLinkFrame* linkFrame = ruNEW CRuLinkFrame();
		linkFrame->SetLinkName("p_200");
		linkFrame->SetTranslation( CRuVector3( 0, 200, 0 ) );
		linkTemp->AddChild(linkFrame);
		ruSAFE_RELEASE(linkFrame);

		m_ruEntity->AddChild(linkTemp);
		ruSAFE_RELEASE(linkTemp);

	}
	*/
}

// ----------------------------------------------------------------------------------
void CUiModel::ReleaseEntity()
{
	if (m_ruFx2DModel)
		m_ruFx2DModel->SetModel(NULL);
	ReleaseAttachEntity();					// 釋放所有參考點位置
	
	ruSAFE_RELEASE(m_ruEntity);	
	ruSAFE_RELEASE(m_ruFusionMime);
	ruSAFE_RELEASE(m_ruParentEntity);
}

// ----------------------------------------------------------------------------------
// 附加ACT物件到主要ACT物件參考點上,在相同參考點上只能附加一個ACT物件.
// point 參考點名稱
// actFilename 物件ACT檔案名稱,傳入NULL會釋放原部位名稱
void CUiModel::AttachEntity(const char* point, const char* actFilename, const char* enchantFilename)
{
	if ( point == NULL || m_ruEntity == NULL )
		return;

	// 產生物件
	CRuEntity* entity = g_ruResourceManager->LoadEntity(actFilename);
	CRuEntity* enchantEntity = NULL;
	if ( entity ) {
		if ( RuEntity_FindLinkFrame(entity, "enfx01") && RuEntity_FindLinkFrame(entity, "enfx02") ) {
			enchantEntity = g_ruResourceManager->LoadEntity(enchantFilename);
			if ( enchantEntity ) {
				RuACT_PlayPersistentMotion(enchantEntity);
				RuACT_PlayMotion(enchantEntity, "idle");
				RuACT_PlayMotion(enchantEntity, "stand_idle01");
				entity->AddChild(enchantEntity);
			}
		}
	}	

	if ( entity )
	{
		//RuACT_PlayPersistentMotion(entity);
		RuACT_PlayMotion(entity, "persistent");
		RuACT_PlayMotion(entity, "idle");
	}

	map<string, ModelAttachEntity>::iterator iter = m_attachEntity.find(point);
	if ( iter != m_attachEntity.end() ) {			// 原參考點部份已存在
		ModelAttachEntity& attach = iter->second;

		// 釋放舊物件
		RuEntity_Detach(m_ruEntity, attach.m_entity, point);
		ruSAFE_RELEASE(attach.m_enchantEntity);
		ruSAFE_RELEASE(attach.m_entity);
		
		// 物件不存
		if ( entity == NULL ) {
			m_attachEntity.erase(iter);
			return;
		}

		attach.m_entity = entity;
		attach.m_enchantEntity = enchantEntity;
	} else if ( entity ) {
		m_attachEntity.insert(make_pair(point, ModelAttachEntity(entity, enchantEntity)));
	} else {
		return;
	}
	RuEntity_Attach(m_ruEntity, entity, point);
}

// ----------------------------------------------------------------------------------
void CUiModel::ReleaseAttachEntity()
{
	for ( map<string, ModelAttachEntity>::iterator iter = m_attachEntity.begin(); iter != m_attachEntity.end(); iter++ ) {
		ModelAttachEntity& attach = iter->second;

		if ( m_ruEntity ) RuEntity_Detach(m_ruEntity, attach.m_entity, iter->first.c_str());
		ruSAFE_RELEASE(attach.m_enchantEntity);
		ruSAFE_RELEASE(attach.m_entity);
	}
	m_attachEntity.clear();
}

// ----------------------------------------------------------------------------------
void CUiModel::StartMotion(const char* motion)
{
	if ( m_ruEntity == NULL )
		return;
	
	RuACT_PlayPersistentMotion(m_ruEntity);
	if ( motion == NULL )
	{
		if ( RuACT_HasMotion(m_ruEntity, "stand_idle01", FALSE) )
			motion = "stand_idle01";
		else if ( RuACT_HasMotion(m_ruEntity, "idle", FALSE) )
			motion = "idle";
	}
	RuACT_PlayMotion(m_ruEntity, motion, 0.0f, TRUE);
}
// ----------------------------------------------------------------------------------
void CUiModel::GetPaperdollPart(const char* partName,  char* buff)
{
	if ( m_ruEntity )
	{	
		IRuPaperdoll* ruPaperdoll = RuEntity_FindFirstPaperdoll(m_ruEntity);
		if (ruPaperdoll)
		{
			const char* name= ruPaperdoll->GetComponent(partName);
			strcpy(buff,name) ;

		}
	}
}
// ----------------------------------------------------------------------------------
void CUiModel::SetPaperdollPart(const char* partName, const char* name)
{
	if ( m_ruEntity )
	{
		//if (name == NULL)
		//	name = "";
		//else if (name[0] == 0)
		//{
		//	if (strcmp(partName, "head") == 0)
		//		name = "type01";
		//	else if (strcmp(partName, "hair") == 0)
		//		name = "type01";
		//	else if (strcmp(partName, "torso") == 0)
		//		name = "body000-001";
		//	else if (strcmp(partName, "hand") == 0)
		//		name = "body000-001";
		//	else if (strcmp(partName, "leg") == 0)
		//		name = "body000-001";
		//	else if (strcmp(partName, "foot") == 0)
		//		name = "body000-001";
		//}

		if( name == NULL || name[0] == 0 )
		{
			if (strcmp(partName, "head") == 0)
				name = "type01";
			else if (strcmp(partName, "hair") == 0)
				name = "type01";
			else if (strcmp(partName, "torso") == 0)
				name = "body000-001";
			else if (strcmp(partName, "hand") == 0)
				name = "body000-001";
			else if (strcmp(partName, "leg") == 0)
				name = "body000-001";
			else if (strcmp(partName, "foot") == 0)
				name = "body000-001";
		}

		RuPaperdoll_SetComponent(m_ruEntity, partName, name);
		RuPaperdoll_Build(m_ruEntity);
	}
}

// ----------------------------------------------------------------------------------
void CUiModel::SetPaperdollColor(const char* partName, int color1, int color2)
{
	if ( m_ruEntity == NULL )
		return;

	IRuPaperdoll* ruPaperdoll = RuEntity_FindFirstPaperdoll(m_ruEntity);
	if (ruPaperdoll)
	{
		BOOL layerActivated[4];
		RUCOLOR layerColor[4];		

		if ( strcmp(partName, g_partName[0]) == 0 )
		{
			for ( int i = 0; i < MAX_MODEL_PAPERDOLL_PARTS; i++ )
			{
				ruPaperdoll->GetComponentColors(g_partName[i], layerActivated, layerColor);
				layerActivated[0] = ((color1 & 0xFF000000) == 0xFF000000);
				layerColor[0] = color1;
				ruPaperdoll->SetComponentColors(g_partName[i], layerActivated, layerColor);
			}
		}
		else if ( strcmp(partName, g_partName[1]) == 0 )
		{
			// 檢測髮色
			CUiRGBA color(color1);
			if ( color.m_r < 60 && color.m_g < 60 && color.m_b < 60 )
			{
				color.m_r = 0x4C;
				color.m_g = 0x4C;
				color.m_b = 0x4C;
				color1 = color.m_value;
			}

			for ( int i = 0; i < MAX_MODEL_PAPERDOLL_PARTS; i++ )
			{
				ruPaperdoll->GetComponentColors(g_partName[i], layerActivated, layerColor);
				layerActivated[1] = ((color1 & 0xFF000000) == 0xFF000000);
				layerColor[1] = color1;
				ruPaperdoll->SetComponentColors(g_partName[i], layerActivated, layerColor);
			}
		}
		else if ( ruPaperdoll->GetComponentColors(partName, layerActivated, layerColor) )
		{
			layerActivated[2] = ((color1 & 0xFF000000) == 0xFF000000);
			layerActivated[3] = ((color2 & 0xFF000000) == 0xFF000000);
			layerColor[2] = color1;
			layerColor[3] = color2;
			ruPaperdoll->SetComponentColors(partName, layerActivated, layerColor);
		}
		RuPaperdoll_Build(m_ruEntity);
	}
}

// ----------------------------------------------------------------------------------
void CUiModel::GetPaperdollColor(const char* partName, int* color)
{
	if ( m_ruEntity == NULL )
		return;

	IRuPaperdoll* ruPaperdoll = RuEntity_FindFirstPaperdoll(m_ruEntity);
	if (ruPaperdoll && color)
	{		
		BOOL layerActivated[4];
		RUCOLOR layerColor[4];

		// 先取得基本顏色資料
		ruPaperdoll->GetComponentColors(partName, layerActivated, layerColor);

		if ( strcmp(partName, g_partName[0]) == 0 )
		{
			color[0] = layerColor[0];
		}
		else if ( strcmp(partName, g_partName[1]) == 0 )
		{
			color[0] = layerColor[1];
		}
		else
		{
			color[0] = layerColor[2];
			color[1] = layerColor[3];
		}
	}
}

// ----------------------------------------------------------------------------------
void CUiModel::SetBoneScale(const char* boneName, float scale)
{
	if ( m_ruFusionMime )
	{
		RuMime_SetBoneScale(m_ruFusionMime, boneName, scale);
	}
}
// ----------------------------------------------------------------------------------
// 回傳最大換色階層數
int	CUiModel::GetMaterialCount()
{
	if ( m_ruEntity == NULL )
		return 0;

	// Enumerate all materials
	CRuArrayList<IRuMaterial *> materials;
	RuEntity_EnumerateMaterials(m_ruEntity, materials);	

	return materials.Count();
}
// ----------------------------------------------------------------------------------
// layer [int] 層級編號
// outExists bool[4] 回傳每個顏色層是否有做用
bool CUiModel::CanDyeFXColors(int layer, bool outExists[4])
{
	if ( m_ruEntity == NULL )
		return false;
	
	// Enumerate all materials
	CRuArrayList<IRuMaterial *> materials;
	RuEntity_EnumerateMaterials(m_ruEntity, materials);	

	if ( layer < 0 || layer >= materials.Count() )
		return false;
	
	RuPropertyType propertyValue;
	BOOL dyeFXEnabled = materials[layer]->GetProperty("material:dyeFX", propertyValue) ? boost::get<BOOL>(propertyValue) : FALSE;

	if ( dyeFXEnabled == FALSE || RuMaterial_BuildDyeFXMask(materials[layer]) == FALSE )
		return false;
	
	for ( int i = 0; i < 4; i++ )
		outExists[i] = (RuMaterial_GetDyeFXExists(materials[layer], i + 1) != 0);

	return true;
}
// ----------------------------------------------------------------------------------
void CUiModel::SetDyeFXColors(int colors[4])
{
	if ( m_ruEntity == NULL )
		return;

	// Enumerate all materials
	CRuArrayList<IRuMaterial *> materials;
	RuEntity_EnumerateMaterials(m_ruEntity, materials);

	for(INT32 i = 0; i < materials.Count(); ++i)
	{
		RuPropertyType propertyValue;
		BOOL dyeFXEnabled  = materials[i]->GetProperty("material:dyeFX", propertyValue) ? boost::get<BOOL>(propertyValue) : FALSE;

		// If DyeFX is enabled, build the DyeFX mask and the color map
		if(dyeFXEnabled)
		{
			BOOL layerActivated[4] = { FALSE, FALSE, FALSE, FALSE };
			RUCOLOR layerColor[4] = { 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF };

			for ( int j = 0; j < 4; j++ )
			{
				layerActivated[j] = colors[j] & 0xFF000000;
				layerColor[j] = colors[j];
			}

			if(RuMaterial_BuildDyeFXMask(materials[i]) == FALSE)
			{
				m_dyeFXMaskRequiresLoad = true;
			}

			RuMaterial_SetDyeFXColors(materials[i], layerActivated, layerColor);
		}
	}
}

// ----------------------------------------------------------------------------------
void CUiModel::BuildDyeFXMask()
{
	if ( m_ruEntity == NULL )
		return;

	// Enumerate all materials
	CRuArrayList<IRuMaterial *> materials;
	RuEntity_EnumerateMaterials(m_ruEntity, materials);

	for(INT32 i = 0; i < materials.Count(); ++i)
	{
		RuPropertyType propertyValue;
		BOOL dyeFXEnabled  = materials[i]->GetProperty("material:dyeFX", propertyValue) ? boost::get<BOOL>(propertyValue) : FALSE;

		// If DyeFX is enabled, build the DyeFX mask and the color map
		if(dyeFXEnabled)
		{
			if(RuMaterial_BuildDyeFXMask(materials[i]) == FALSE)
			{
				m_dyeFXMaskRequiresLoad = true;
			}
		}
	}
}

// ----------------------------------------------------------------------------------
void CUiModel::SetTargetBone(const char* targetBoneName)
{
	if ( strcmp(targetBoneName, "HEAD") == 0 ) {
		SetAlignmentFrames("p_top", "p_mouth", 0.7f, 0.8f);
	} else if ( strcmp(targetBoneName, "BODY") == 0 ) {
		SetAlignmentFrames("p_top", "p_down", 0.5f, 0.5f);
	}
	
	/*
	m_ruFx2DModel->ClearTargetBones();
	if ( targetBoneName )
	{
		if ( strcmp(targetBoneName, "HEAD") == 0 )
		{
			m_ruFx2DModel->AddTargetBone("Bip01 Head");
			m_ruFx2DModel->AddTargetBone("Bip01 L Clavicle");
			m_ruFx2DModel->AddTargetBone("Bip01 R Clavicle");
		}
		else if ( strcmp(targetBoneName, "BODY") == 0 )
		{
		}
	}
	*/
}

// ----------------------------------------------------------------------------------
// top		上參考點
// bottom	下參考點
// bias		設定中心點在兩參考點位置中間的比重位置(0.0 ~ 1.0,標準0.5)
// scale	中心點的半徑大小(標準0.5)
void CUiModel::SetAlignmentFrames(const char* top, const char* bottom, float bias, float scale)
{
	m_alignment.m_top = (top != NULL) ? top : "";
	m_alignment.m_bottom = (bottom != NULL) ? bottom : "";
	m_alignment.m_bias = bias;
	m_alignment.m_scale = scale;

	if ( m_ruFx2DModel ) {
		m_ruFx2DModel->SetAlignmentFrames(m_alignment.m_top.c_str(), m_alignment.m_bottom.c_str(), m_alignment.m_bias, m_alignment.m_scale);
	}	
}

// ----------------------------------------------------------------------------------
void CUiModel::SetTargetRotate(float rotateAngel)
{
	CRuQuaternion quaternion;
	quaternion.FromAngleAxis(CRuVector3(0.0f, 1.0f, 0.0f), rotateAngel);
	quaternion.TransformVector(m_cameraPosition);

	if ( m_ruFx2DModel )
		m_ruFx2DModel->SetRelativeCameraPosition(m_cameraPosition);
}

// ----------------------------------------------------------------------------------
void CUiModel::SetRelativeCameraPosition(const CRuVector3 &relativeCamPos)
{
	m_cameraPosition = relativeCamPos;

	if ( m_ruFx2DModel )
		m_ruFx2DModel->SetRelativeCameraPosition(m_cameraPosition);
}

// ----------------------------------------------------------------------------------
void CUiModel::SetMaskFile(const char* maskFile)
{
	ruSAFE_RELEASE(m_maskTexture);
	m_maskTexture = g_ruResourceManager->LoadTexture(maskFile);
	if ( m_ruFx2DModel && m_maskTexture )
		m_ruFx2DModel->SetMaskingTexture(m_maskTexture);
}

// ----------------------------------------------------------------------------------
void CUiModel::SetModelOffset(CRuVector3& offset)
{
	m_modelOffset = offset;
	if ( m_ruFx2DModel )
		m_ruFx2DModel->SetModelOffset(m_modelOffset);
}

// ----------------------------------------------------------------------------------
void CUiModel::SetDefaultTexture(CUiTexture* defaultTexture)
{
	m_defaultTexture = defaultTexture;
}

// ----------------------------------------------------------------------------------
int CUiModel::GetMemSize()
{
	int size = CUiFrame::GetMemSize() + sizeof(CUiModel) - sizeof(CUiFrame);

	if ( m_ruParentEntity )
		size += sizeof(CRuEntity);

	if ( m_ruEntity )
		size += sizeof(CRuEntity);

	if ( m_ruFusionMime )
		size += sizeof(CRuFusion_Mime);

	if ( m_ruFx2DModel )
		size += sizeof(CRuFX2D_Model);

	if ( m_maskTexture )
		size += sizeof(IRuTexture);

	if ( m_defaultTexture )
		size += sizeof(CUiTexture);

	for ( map<string, ModelAttachEntity>::iterator iter = m_attachEntity.begin(); iter != m_attachEntity.end(); iter++ )
	{
		size += sizeof(WCHAR) * (int)iter->first.size();
		size += sizeof(ModelAttachEntity);
	}

	size += sizeof(WCHAR) * (int)m_alignment.m_top.size();
	size += sizeof(WCHAR) * (int)m_alignment.m_bottom.size();

	return size;
}

// ----------------------------------------------------------------------------------
void CUiModel::SetEntityDiffuse(DWORD color)
{
	if ( m_ruEntity )
	{
		CRuParameterBlock *parameterBlock = m_ruEntity->GetParameterBlock();
		UINT64 parameterOut;

		parameterBlock->GetParameter(ruPARAMTYPE_MATERIAL_DIFFUSE, parameterOut);
		if ( parameterOut != (UINT64)color )
			parameterBlock->SetParameter(ruPARAMTYPE_MATERIAL_DIFFUSE, color);
	}
}

// ----------------------------------------------------------------------------------
void CUiModel::SetEntityRotate(const CRuVector3 &axisOfRotation, REAL rotationAngle)
{
	CRuQuaternion quaternion;
	quaternion.FromAngleAxis(axisOfRotation, rotationAngle);

	if ( m_ruEntity ) {
		m_ruEntity->Rotate(quaternion, ruTRANSFORMOP_PRECONCAT);
	}
}

// ----------------------------------------------------------------------------------
void CUiModel::OnPaint(float alpha)
{	
	if ( m_ruEntity )
	{		
		CRuParameterBlock *parameterBlock = m_ruEntity->GetParameterBlock();
		parameterBlock->SetParameter(ruPARAMTYPE_MATERIAL_ALPHA, RuCastFloatToINT64(alpha));
	}

	if ( m_ruFx2DModel)
	{		
		CUiRectangle rect;
		GetRectangle(&rect);
		m_ruFx2DModel->SetViewRect(CRuRectangle(rect.m_left, rect.m_top, rect.m_right, rect.m_bottom));
		m_ruFx2DModel->Render(g_ruUIRenderer);
	}

	CUiFrame::OnPaint(alpha);
}

// ----------------------------------------------------------------------------------
void CUiModel::OnUpdate(float elapsedTime)
{
	if ( m_ruEntity )
	{
		m_ruEntity->Update(elapsedTime);
	}

	if ( m_ruFusionMime )
	{
		m_ruFusionMime->Update(elapsedTime);
	}

	// If the dyeFX mask has not yet been loaded, attempt to build it again
	if(m_dyeFXMaskRequiresLoad)
	{
		BuildDyeFXMask();
	}

	CUiFrame::OnUpdate(elapsedTime);
}

// ----------------------------------------------------------------------------------
void CUiModel::OnShow(lua_State* L)
{
	StartMotion();

	CUiFrame::OnShow(L);
}