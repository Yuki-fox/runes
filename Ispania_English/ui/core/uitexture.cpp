#include <windows.h>
#include "uitexture.h"

// ----------------------------------------------------------------------------------
CUiTexture::CUiTexture(const char* name, CUiLayout* pParent, CUi* pUi)
	: CUiLayout(name, pParent, pUi)
{
	m_alphaMode			= ALPHA_MODE_BLEND;	
	m_texCoords.clear();

	m_animationSpeed	= 0.0f;
	m_animationTime		= 0.0f;
	m_animationPlayTime	= 0.0f;
	m_rotate			= 0.0f;
	m_reverse			= false;

	m_cooldown			= NULL;
	m_textureMaskImage	= NULL;
}

// ----------------------------------------------------------------------------------
CUiTexture::~CUiTexture()
{
	ruSAFE_RELEASE(m_cooldown);
	UI_SAFE_DELETE(m_textureMaskImage);
}

// ----------------------------------------------------------------------------------
void CUiTexture::Update(float elapsedTime)
{
	m_animationTime += elapsedTime;

	if ( m_animationPlayTime > 0.0f )
		m_animationPlayTime += elapsedTime;

	if ( m_cooldown && m_cooldown->GetCooldownRemaining() > 0.0f )
	{
		m_cooldown->Update(elapsedTime);
	}	
}

// ----------------------------------------------------------------------------------
void CUiTexture::ReloadFile()
{
	m_textureImage.RemoveTexture();
}

// ----------------------------------------------------------------------------------
void CUiTexture::Render(float alpha)
{
	CUiPoint pos = GetPaintPos();
	Render(alpha, pos.m_x, pos.m_y, 1.0f, 1.0f);
}

// ----------------------------------------------------------------------------------
void CUiTexture::Render(float alpha, float wScale, float hScale)
{
	CUiPoint pos = GetPaintPos();
	Render(alpha, pos.m_x, pos.m_y, wScale, hScale);
}

// ----------------------------------------------------------------------------------
void CUiTexture::Render(float alpha, float x, float y, float wScale, float hScale)
{
	if ( !m_isVisible )
		return;

	CRuRectangle tex, dest;
	CUiSize size = GetRealSize();

	// 取得貼圖座標
	if ( !m_texCoords.empty() )
	{
		DWORD texPos = 0;
		if ( m_animationSpeed > 0.0f && m_texCoords.size() > 1 )
		{			
			if ( m_animationPlayTime > 0.0f )
			{
				texPos = (DWORD)(m_animationSpeed * m_animationPlayTime);
				if ( texPos >= m_texCoords.size() )
					texPos = (DWORD)m_texCoords.size() - 1;
			}
			else
			{
				texPos = (DWORD)((m_animationTime + m_animationTime) * m_animationSpeed) % (DWORD)m_texCoords.size();
			}
		}

		tex.m_left		= m_texCoords[texPos].m_left;
		tex.m_top		= m_texCoords[texPos].m_top;
		tex.m_right		= m_texCoords[texPos].m_right * wScale;
		tex.m_bottom	= m_texCoords[texPos].m_bottom * hScale;
	}
	else
		tex.Set( 0.0f, 0.0f, wScale, hScale );

	// 取得目的座標	
	if ( m_reverse )
	{
		dest.m_left		= x + (size.m_x - size.m_x * wScale);
		dest.m_top		= y;
		dest.m_right	= x + size.m_x;
		dest.m_bottom	= y + size.m_y * hScale;
	}
	else
	{
		dest.m_left		= x;
		dest.m_top		= y;
		dest.m_right	= x + size.m_x * wScale;
		dest.m_bottom	= y + size.m_y * hScale;
	}

	CUiRGBA color;
	TextureAlphaMode alphaMode = m_alphaMode;
	color.m_value = m_color.GetColor();
	color.SetAlphaScale(alpha);

	// 是否有遮罩
	if ( m_textureMaskImage )
	{
		m_textureMaskImage->Draw(dest, CRuRectangle(0.0f, 0.0f, 1.0f, 1.0f), ALPHA_MODE_MASK, 0xFFFFFFFF);
		alphaMode = ALPHA_MODE_MASKED;
	}

	if ( m_rotate != 0.0f )
	{
		g_ruUIRenderer->SetRotation(m_rotate);
		m_textureImage.Draw(dest, tex, alphaMode, color.m_value);		// 繪出圖形
		g_ruUIRenderer->SetRotation(0.0f);
	}
	else
	{		
		m_textureImage.Draw(dest, tex, alphaMode, color.m_value);		// 繪出圖形
	}

	if ( m_cooldown && m_cooldown->GetCooldownRemaining() > 0.0f )
	{
		m_cooldown->SetIconSize(size.m_x, size.m_y);
		m_cooldown->SetRenderingOffset(CRuVector3(x, y, 0.0f));
		m_cooldown->Render(g_ruUIRenderer, 0.0f);
	}
}

// ----------------------------------------------------------------------------------
void CUiTexture::SetTexCoords(int count, CRuRectangle* pRectangle)
{
	m_texCoords.clear();
	for (int i = 0; i < count; i++)
		m_texCoords.push_back(pRectangle[i]);
}

// ----------------------------------------------------------------------------------
void CUiTexture::SetTexture(const CUiTexture* texture)
{
	if (texture)
	{
		m_color				= texture->m_color;
		m_textureImage		= texture->m_textureImage;		
		m_alphaMode			= texture->m_alphaMode;
		m_texCoords			= texture->m_texCoords;
		m_animationSpeed	= texture->m_animationSpeed;
	}
}

// ----------------------------------------------------------------------------------
void CUiTexture::StartCooldown(float duration, float remaining)
{
	if ( m_cooldown == NULL )
		m_cooldown = ruNEW CRuFX2D_IconCooldown();
	
	m_cooldown->StartCooldown(duration, min(duration, remaining));
}

// ----------------------------------------------------------------------------------
void CUiTexture::SetMaskFile(const char* fileNameStr)
{
	if ( m_textureMaskImage == NULL )
		m_textureMaskImage = new CTextureImage;
	m_textureMaskImage->SetFileName(fileNameStr);
}

// ----------------------------------------------------------------------------------
// 開始撥放動畫,會停留在最一張
void CUiTexture::PlayAnimation()
{
	m_animationPlayTime = 0.01f;
}

// ----------------------------------------------------------------------------------
// 取得目前動畫撥放進度
float CUiTexture::GetAnimationProgress()
{
	return min(m_animationSpeed * m_animationPlayTime, 1.0f);
}

// ----------------------------------------------------------------------------------
int CUiTexture::GetMemSize()
{
	int size = CUiLayout::GetMemSize() + sizeof(CUiTexture) - sizeof(CUiLayout);

	if ( m_textureImage.GetFileName() )
		size += (int)strlen(m_textureImage.GetFileName()) + 1;

	if ( m_textureMaskImage )
	{
		size += sizeof(CTextureImage);
		if ( m_textureMaskImage->GetFileName() )
			size += (int)strlen(m_textureMaskImage->GetFileName()) + 1;
	}

	if ( m_cooldown )
	{
		size += sizeof(CRuFX2D_IconCooldown);
	}

	size += sizeof(CRuRectangle) * (int)m_texCoords.size() + sizeof(m_texCoords);

	return size;
}