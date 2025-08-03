// RegionCreator.h: interface for the CRegionCreator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_REGIONCREATOR_H__6A03AB52_413A_41FD_B034_E8F1B613787A__INCLUDED_)
#define AFX_REGIONCREATOR_H__6A03AB52_413A_41FD_B034_E8F1B613787A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CRegionCreator  
{
public:

	CRegionCreator(LPCTSTR name);
	// Methods
	HRGN CreateRegionFromBitmap(HBITMAP hBitmap, COLORREF transparentColor,int &w,int &h);

	static bool g_bLoad;

	void SaveRegion(void);
	bool LoadRegion(void);
	TCHAR  m_name[1024];
	HRGN m_h;
	virtual ~CRegionCreator();
};

#endif // !defined(AFX_REGIONCREATOR_H__6A03AB52_413A_41FD_B034_E8F1B613787A__INCLUDED_)
