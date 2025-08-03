#ifndef MING_PROGRESSBAR_H
#define MING_PROGRESSBAR_H
#pragma once

class UILIB_API CProgressBarUI: public CControlUI
{
public:
	CProgressBarUI(SquirrelObject&so);
	virtual ~CProgressBarUI();

	void SetWidth(int cxWidth);
	void SetHeight(int cyHeight);
	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);


	int GetValue(void);
	void SetValue(int Value);
	void SetMax(int maxValue);
	void DoPaint(HDC hDC, const RECT& rcPaint);
	
	void SetImage(LPCTSTR imageFile);

	LPCTSTR GetClass() const ;
	UINT GetControlFlags() const;
	SIZE EstimateSize(SIZE szAvailable);

protected:
	
	
	 int m_maxValue;
	 int m_Value;
	 HBITMAP m_image;

	 SIZE m_cxyFixed;
};
#endif//MING_PROGRESSBAR_H