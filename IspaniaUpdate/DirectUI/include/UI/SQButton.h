
#ifndef MING_SQBUTTON_H
#define MING_SQBUTTON_H
#pragma once
enum BUTTON_STATE
{
	BS_NORMAL,
	BS_PUSHED,	
	BS_HOVER,
	BS_DISABLE,
	BS_COUNT

};

class UILIB_API SQButtonUI : public CButtonUI
{
public:
	SQButtonUI(SquirrelObject&so);
	virtual ~SQButtonUI();

	LPCTSTR GetClass() const;
	void SetHeight(int height);
	 
	void DoPaint(HDC hDC, const RECT& rcPaint);
	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
	SIZE EstimateSize(SIZE /*szAvailable*/);

	void SetImage(int state,LPCTSTR image);
protected:
	HBITMAP m_image[BS_COUNT];
	 int m_cyHeight;
};
#endif//MING_SQBUTTON_H