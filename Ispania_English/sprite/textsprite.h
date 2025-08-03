#pragma	  once

class CTextSprite : public IBaseSprite
{
public:
	CTextSprite();
	virtual ~CTextSprite();

	virtual	void					Update(float elapsedTime);
	virtual void					Render();
	virtual	void					ReceiveDestruct(IBaseSprite* pSprite);

	virtual	void					SetText(const char* text, float scale, int color);
protected:
	CRuEntity_Widget_Label*			m_pRuWidget;				// 3D¤å¦rª«¥ó
	float							m_updateTime;
};