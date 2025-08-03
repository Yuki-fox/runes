#if !defined(AFX_UICONTAINER_H__20060218_C077_501B_DC6B_0080AD509054__INCLUDED_)
#define AFX_UICONTAINER_H__20060218_C077_501B_DC6B_0080AD509054__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


/////////////////////////////////////////////////////////////////////////////////////
//UI容器界面.

class IContainerUI
{
public:
	//取得子元件
   virtual CControlUI* GetItem(int iIndex) const = 0;
   //取得子元件個數
   virtual int GetCount() const = 0;
   //加入子元件
   virtual bool Add(CControlUI* pControl) = 0;
   //移除子元件
   virtual bool Remove(CControlUI* pControl) = 0;
   //清除所有
   virtual void RemoveAll() = 0;
};


/////////////////////////////////////////////////////////////////////////////////////
//UI容器實作

class UILIB_API CContainerUI : public CControlUI, public IContainerUI
{
public:
   CContainerUI(SquirrelObject&so);
   virtual ~CContainerUI();

public:
   LPCTSTR GetClass() const;
   LPVOID GetInterface(LPCTSTR pstrName);

   CControlUI* GetItem(int iIndex) const;
   int GetCount() const;
   bool Add(CControlUI* pControl);
   bool Remove(CControlUI* pControl);
   void RemoveAll();

   void Event(TEventUI& event);
   void SetVisible(bool bVisible);

   virtual void SetInset(SIZE szInset);
   virtual void SetInset(RECT rcInset);
   virtual void SetPadding(int iPadding);
   virtual void SetWidth(int cx);
   virtual void SetHeight(int cy);
   virtual void SetAutoDestroy(bool bAuto);

   virtual int FindSelectable(int iIndex, bool bForward = true) const;

   void SetPos(RECT rc);
   SIZE EstimateSize(SIZE szAvailable);
   void DoPaint(HDC hDC, const RECT& rcPaint);

   void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

   void SetManager(CPaintManagerUI* pManager, CControlUI* pParent);
   CControlUI* FindControl(FINDCONTROLPROC Proc, LPVOID pData, UINT uFlags);

   virtual int GetScrollPos() const;
   virtual int GetScrollPage() const;
   virtual SIZE GetScrollRange() const;
   virtual void SetScrollPos(int iPos);
   virtual void EnableScrollBar(bool bEnable = true);

protected:
   virtual void ProcessScrollbar(RECT rc, int cyRequired);

protected:
	//base::Array<CControlUI*> m_items;
   CStdPtrArray m_items;
   RECT m_rcInset;
   int m_iPadding;
   SIZE m_cxyFixed;
   bool m_bAutoDestroy;
   bool m_bAllowScrollbars;
   HWND m_hwndScroll;
   int m_iScrollPos;
};


/////////////////////////////////////////////////////////////////////////////////////
//畫布UI

class UILIB_API CCanvasUI : public CContainerUI
{
public:
   CCanvasUI(SquirrelObject&so);
   virtual ~CCanvasUI();

   LPCTSTR GetClass() const;

   bool SetWatermark(UINT iBitmapRes, int iOrientation = HTBOTTOMRIGHT);
   bool SetWatermark(LPCTSTR pstrBitmap, int iOrientation = HTTOPRIGHT);

   void DoPaint(HDC hDC, const RECT& rcPaint);

   void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

protected:
   COLORREF m_clrBack;
   HBITMAP m_hBitmap;
   BITMAP m_BitmapInfo;
   int m_iOrientation;
};


/////////////////////////////////////////////////////////////////////////////////////
//

class UILIB_API CWindowCanvasUI : public CCanvasUI
{
public:
   CWindowCanvasUI(SquirrelObject&so);
     virtual~ CWindowCanvasUI(){}
   LPCTSTR GetClass() const;
};


/////////////////////////////////////////////////////////////////////////////////////
//

class UILIB_API CControlCanvasUI : public CCanvasUI
{
public:
   CControlCanvasUI(SquirrelObject&so);
     virtual~ CControlCanvasUI(){}
   LPCTSTR GetClass() const;
};


/////////////////////////////////////////////////////////////////////////////////////
//

class UILIB_API CWhiteCanvasUI : public CCanvasUI
{
public:
   CWhiteCanvasUI(SquirrelObject&so);
        virtual~ CWhiteCanvasUI(){}
   LPCTSTR GetClass() const;
};


/////////////////////////////////////////////////////////////////////////////////////
//

class UILIB_API CDialogCanvasUI : public CCanvasUI
{
public:
   CDialogCanvasUI(SquirrelObject&so);
       virtual~ CDialogCanvasUI(){}
   LPCTSTR GetClass() const;
};


/////////////////////////////////////////////////////////////////////////////////////
//

class UILIB_API CTabFolderCanvasUI : public CCanvasUI
{
public:
   CTabFolderCanvasUI(SquirrelObject&so);
     virtual~ CTabFolderCanvasUI(){}
   LPCTSTR GetClass() const;
};


/////////////////////////////////////////////////////////////////////////////////////
//

class UILIB_API CVerticalLayoutUI : public CContainerUI
{
public:
   CVerticalLayoutUI(SquirrelObject&so);
    virtual~ CVerticalLayoutUI(){}
   LPCTSTR GetClass() const;
   void SetPos(RECT rc);

protected:
   int m_cyNeeded;
   int m_nPrevItems;
};


/////////////////////////////////////////////////////////////////////////////////////
//

class UILIB_API CHorizontalLayoutUI : public CContainerUI
{
public:
   CHorizontalLayoutUI(SquirrelObject&so);
    virtual~ CHorizontalLayoutUI(){}
   LPCTSTR GetClass() const;
   void SetPos(RECT rc);
};


/////////////////////////////////////////////////////////////////////////////////////
//

class UILIB_API CTileLayoutUI : public CContainerUI
{
public:
   CTileLayoutUI(SquirrelObject&so);
    virtual~ CTileLayoutUI(){}
   LPCTSTR GetClass() const;

   void SetPos(RECT rc);
   void SetColumns(int nCols);

protected:
   int m_nColumns;
   int m_cyNeeded;
};


/////////////////////////////////////////////////////////////////////////////////////
//

class UILIB_API CDialogLayoutUI : public CContainerUI
{
public:
   CDialogLayoutUI(SquirrelObject&so);
    virtual~ CDialogLayoutUI(){}
   LPCTSTR GetClass() const;
   LPVOID GetInterface(LPCTSTR pstrName);

   void SetStretchMode(CControlUI* pControl, UINT uMode);

   void SetPos(RECT rc);
   SIZE EstimateSize(SIZE szAvailable);

protected:
   void RecalcArea();

protected:  
   typedef struct 
   {
      CControlUI* pControl;
      UINT uMode;
      RECT rcItem;
   } STRETCHMODE;

   RECT m_rcDialog;
   RECT m_rcOriginal;
   bool m_bFirstResize;
   CStdValArray m_aModes;
};


#endif // !defined(AFX_UICONTAINER_H__20060218_C077_501B_DC6B_0080AD509054__INCLUDED_)

