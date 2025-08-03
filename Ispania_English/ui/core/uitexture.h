#pragma once

#include <vector>
#include "Rune Engine/texture/textureimage.h"
#include "uilayout.h"

#define UI_LUA_TYPE_TEXTURE				"_uiLua.Texture"

using namespace std;

// --------------------------------------------------------------------------------------
class CUiLuaTexture
{
public:
	static	void			OpenLib(lua_State* L);
	static	int				SetMetaTable(lua_State* L);
};

// --------------------------------------------------------------------------------------
class CUiTexture : public CUiLayout
{
public:
	CUiTexture(const char* name, CUiLayout* pParent, CUi* pUi);
	virtual ~CUiTexture();

	// virtual function
	virtual	void			Update(float elapsedTime);
	virtual	void			Render(float alpha);
	virtual void			Render(float alpha, float wScale, float hScale);
	virtual void			Render(float alpha, float x, float y, float wScale, float hScale);
	virtual	void			SetFile(const char* fileNameStr)			{ m_textureImage.SetFileName(fileNameStr); }
	virtual void			ReloadFile();
	virtual	void			SetAlphaMode(TextureAlphaMode alphaMode)	{ m_alphaMode = alphaMode; }	
	virtual	void			SetAnimationSpeed( float speed )			{ m_animationSpeed = speed; }	
	virtual	void			ClearTexCoords()							{ m_texCoords.clear(); }
	virtual	void			SetTexCoords(int count, CRuRectangle* pRectangle);	

	virtual	CTextureImage*	GetTextureImage()							{ return &m_textureImage; }	
	virtual	float			GetAnimationSpeed()							{ return m_animationSpeed; }
	virtual	int				GetTexCoordsCount()							{ return (int)m_texCoords.size(); }
	virtual	CRuRectangle	GetTexCoords(int num)						{ return m_texCoords[num]; }
	virtual	TextureAlphaMode	GetAlphaMode()							{ return m_alphaMode; }

	virtual	void			SetTexture(const CUiTexture* texture);
	virtual	void			SetRotate(float angel)						{ m_rotate = angel; }
	virtual	float			GetRotate()									{ return m_rotate; }
	virtual	void			StartCooldown(float duration, float remaining);
	virtual	void			SetMaskFile(const char* fileNameStr);

	virtual void			SetReverseState(bool reverse)				{ m_reverse = reverse; }
	virtual bool			GetReverseState()							{ return m_reverse; }

	// 開始撥放動畫
	virtual void			PlayAnimation();
	virtual float			GetAnimationProgress();

	virtual int				GetMemSize();

protected:
	virtual	void			SetLuaMetaTable(lua_State* l)				{ CUiLuaTexture::SetMetaTable(l); }

protected:
	CTextureImage			m_textureImage;			// 貼圖類別
	CTextureImage*			m_textureMaskImage;		// 遮罩
	TextureAlphaMode		m_alphaMode;			// 混色模式
	CRuFX2D_IconCooldown*	m_cooldown;

	vector<CRuRectangle>	m_texCoords;			// tu,tv軸座標
	float					m_animationSpeed;		// 動畫速度
	float					m_animationTime;		// 動畫處理時間(控制動畫不同步或同步)
	float					m_animationPlayTime;	// 單一循環動畫開始時間
	float					m_rotate;				// 旋轉角度
	bool					m_reverse;				// 相反(statusbar)
};