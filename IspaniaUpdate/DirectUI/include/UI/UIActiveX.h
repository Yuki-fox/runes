#if !defined(AFX_UIACTIVEX_H__20060223_0330_20DB_3F74_0080AD509054__INCLUDED_)
#define AFX_UIACTIVEX_H__20060223_0330_20DB_3F74_0080AD509054__INCLUDED_

#pragma once


/////////////////////////////////////////////////////////////////////////////////////
//

class CActiveXCtrl;
struct IOleObject;


/////////////////////////////////////////////////////////////////////////////////////
//

class UILIB_API CActiveXUI : public CControlUI, public IMessageFilterUI
{
friend CActiveXCtrl;
public:
   CActiveXUI(SquirrelObject&so);
   virtual ~CActiveXUI();

   void Init();

   LPCTSTR GetClass() const;

   bool CreateControl(const CLSID clsid);
   bool CreateControl(LPCTSTR pstrCLSID);
   HRESULT GetControl(const IID iid, LPVOID* ppRet);

   void SetWidth(int cx);
   void SetHeight(int cy);

   void SetPos(RECT rc);
   SIZE EstimateSize(SIZE szAvailable);
   void DoPaint(HDC hDC, const RECT& rcPaint);

   virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

   LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);

   template <class Q>
   HRESULT GetControl(Q** ppUnk)
   {
	   return GetControl(__uuidof(Q), (void**)ppUnk);
   }
     
protected:
   void ReleaseControl();
   bool DelayedControlCreation();

protected:
   CLSID m_clsid;
   bool m_bCreated;
   IOleObject* m_pUnk;
   CActiveXCtrl* m_pControl;
   HWND m_hwndHost;
   SIZE m_szFixed;
};


#endif // !defined(AFX_UIACTIVEX_H__20060223_0330_20DB_3F74_0080AD509054__INCLUDED_)
