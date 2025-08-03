
#ifndef MING_SLIDERBAR_H
#define MING_SLIDERBAR_H
#pragma once

#define THUMB_WIDTH 18
#define THUMB_HEIGHT 20

class UILIB_API CSliderBarUI: public CControlUI
{

public:
	CSliderBarUI(SquirrelObject&so);
	virtual ~CSliderBarUI(){}



	void SetWidth(int cxWidth);
	void SetHeight(int cyHeight);
	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
	bool Activate();



	int GetValue(void);
	void SetValue(int Value);
	void SetRange(int lowRange,int highRange);
	void DoPaint(HDC hDC, const RECT& rcPaint);
	void Event(TEventUI& event);

	LPCTSTR GetClass() const ;
	UINT GetControlFlags() const;
	SIZE EstimateSize(SIZE szAvailable);
	void CalMouse(POINT &MousePos,bool step);
	void Step(bool up,float scale);

protected:
	int m_LowRange;
	int m_HighRange;
	int m_Value;
	RECT m_rcThumb;
	RECT m_rcChannel;
	UINT m_uButtonState;

	 SIZE m_cxyFixed;


};

#endif//MING_SLIDERBAR_H