
#include "StdAfx.h"
#include <UI\UIBase.h>

#ifdef _DEBUG
   #include <shlwapi.h>
   #pragma comment(lib, "shlwapi.lib")
#endif


/////////////////////////////////////////////////////////////////////////////////////
//
//

void UILIB_API __Trace(LPCTSTR pstrFormat, ...)
{
#ifdef _DEBUG
   TCHAR szBuffer[300] = { 0 };
   va_list args;
   va_start(args, pstrFormat);
   ::wvnsprintf(szBuffer, lengthof(szBuffer) - 2, pstrFormat, args);
   _tcscat(szBuffer, _T("\n"));
   va_end(args);
   ::OutputDebugString(szBuffer);
#endif
}

LPCTSTR __TraceMsg(UINT uMsg)
{
#define MSGDEF(x) if(uMsg==x) return L#x
   MSGDEF(WM_SETCURSOR);
   MSGDEF(WM_NCHITTEST);
   MSGDEF(WM_NCPAINT);
   MSGDEF(WM_PAINT);
   MSGDEF(WM_ERASEBKGND);
   MSGDEF(WM_NCMOUSEMOVE);  
   MSGDEF(WM_MOUSEMOVE);
   MSGDEF(WM_MOUSELEAVE);
   MSGDEF(WM_MOUSEHOVER);   
   MSGDEF(WM_NOTIFY);
   MSGDEF(WM_COMMAND);
   MSGDEF(WM_MEASUREITEM);
   MSGDEF(WM_DRAWITEM);   
   MSGDEF(WM_LBUTTONDOWN);
   MSGDEF(WM_LBUTTONUP);
   MSGDEF(WM_LBUTTONDBLCLK);
   MSGDEF(WM_RBUTTONDOWN);
   MSGDEF(WM_RBUTTONUP);
   MSGDEF(WM_RBUTTONDBLCLK);
   MSGDEF(WM_SETFOCUS);
   MSGDEF(WM_KILLFOCUS);  
   MSGDEF(WM_MOVE);
   MSGDEF(WM_SIZE);
   MSGDEF(WM_SIZING);
   MSGDEF(WM_MOVING);
   MSGDEF(WM_GETMINMAXINFO);
   MSGDEF(WM_CAPTURECHANGED);
   MSGDEF(WM_WINDOWPOSCHANGED);
   MSGDEF(WM_WINDOWPOSCHANGING);   
   MSGDEF(WM_NCCALCSIZE);
   MSGDEF(WM_NCCREATE);
   MSGDEF(WM_NCDESTROY);
   MSGDEF(WM_TIMER);
   MSGDEF(WM_KEYDOWN);
   MSGDEF(WM_KEYUP);
   MSGDEF(WM_CHAR);
   MSGDEF(WM_SYSKEYDOWN);
   MSGDEF(WM_SYSKEYUP);
   MSGDEF(WM_SYSCOMMAND);
   MSGDEF(WM_SYSCHAR);
   MSGDEF(WM_VSCROLL);
   MSGDEF(WM_HSCROLL);
   MSGDEF(WM_CHAR);
   MSGDEF(WM_SHOWWINDOW);
   MSGDEF(WM_PARENTNOTIFY);
   MSGDEF(WM_CREATE);
   MSGDEF(WM_NCACTIVATE);
   MSGDEF(WM_ACTIVATE);
   MSGDEF(WM_ACTIVATEAPP);   
   MSGDEF(WM_CLOSE);
   MSGDEF(WM_DESTROY);
   MSGDEF(WM_GETICON);   
   MSGDEF(WM_GETTEXT);
   MSGDEF(WM_GETTEXTLENGTH);   
   static TCHAR szMsg[10];
   ::wsprintf(szMsg, _T("0x%04X"), uMsg);
   return szMsg;
}


/////////////////////////////////////////////////////////////////////////////////////
//
//

CRect::CRect()
{
   left = top = right = bottom = 0;
}

CRect::CRect(const RECT& src)
{
   left = src.left;
   top = src.top;
   right = src.right;
   bottom = src.bottom;
}

CRect::CRect(int iLeft, int iTop, int iRight, int iBottom)
{
   left = iLeft;
   top = iTop;
   right = iRight;
   bottom = iBottom;
}

int CRect::GetWidth() const
{
   return right - left;
}

int CRect::GetHeight() const
{
   return bottom - top;
}

void CRect::Empty()
{
   left = top = right = bottom = 0;
}

void CRect::Join(const RECT& rc)
{
   if( rc.left < left ) left = rc.left;
   if( rc.top < top ) top = rc.top;
   if( rc.right > right ) right = rc.right;
   if( rc.bottom > bottom ) bottom = rc.bottom;
}

void CRect::ResetOffset()
{
   ::OffsetRect(this, -left, -top);
}

void CRect::Normalize()
{
   if( left > right ) { int iTemp = left; left = right; right = iTemp; }
   if( top > bottom ) { int iTemp = top; top = bottom; bottom = iTemp; }
}

void CRect::Offset(int cx, int cy)
{
   ::OffsetRect(this, cx, cy);
}

void CRect::Inflate(int cx, int cy)
{
   ::InflateRect(this, cx, cy);
}

void CRect::Deflate(int cx, int cy)
{
   ::InflateRect(this, -cx, -cy);
}

void CRect::Union(CRect& rc)
{
   ::UnionRect(this, this, &rc);
}


/////////////////////////////////////////////////////////////////////////////////////
//
//

CSize::CSize()
{
   cx = cy = 0;
}

CSize::CSize(const SIZE& src)
{
   cx = src.cx;
   cy = src.cy;
}

CSize::CSize(const RECT rc)
{
   cx = rc.right - rc.left;
   cy = rc.bottom - rc.top;
}

CSize::CSize(int _cx, int _cy)
{
   cx = _cx;
   cy = _cy;
}


/////////////////////////////////////////////////////////////////////////////////////
//
//

CPoint::CPoint()
{
   x = y = 0;
}

CPoint::CPoint(const POINT& src)
{
   x = src.x;
   y = src.y;
}

CPoint::CPoint(int _x, int _y)
{
   x = _x;
   y = _y;
}

CPoint::CPoint(LPARAM lParam)
{
   x = GET_X_LPARAM(lParam);
   y = GET_Y_LPARAM(lParam);
}



/////////////////////////////////////////////////////////////////////////////////////
//
//

CStdPtrArray::CStdPtrArray(int iPreallocSize) : m_ppVoid(NULL), m_nCount(0), m_nAllocated(iPreallocSize)
{
   ASSERT(iPreallocSize>=0);
   if( iPreallocSize > 0 )
	   m_ppVoid = static_cast<LPVOID*>(malloc(iPreallocSize * sizeof(LPVOID)));
}

CStdPtrArray::~CStdPtrArray()
{
   if( m_ppVoid != NULL ) 
	   free(m_ppVoid);
}

void CStdPtrArray::Empty()
{
   if( m_ppVoid != NULL ) 
	   free(m_ppVoid);
   m_ppVoid = NULL;
   m_nCount = m_nAllocated = 0;
}

void CStdPtrArray::Resize(int iSize)
{
   Empty();
   m_ppVoid = static_cast<LPVOID*>(malloc(iSize * sizeof(LPVOID)));
   ::ZeroMemory(m_ppVoid, iSize * sizeof(LPVOID));
   m_nAllocated = iSize;
   m_nCount = iSize;
}

bool CStdPtrArray::IsEmpty() const
{
   return m_nCount == 0;
}

bool CStdPtrArray::add(LPVOID pData)
{
   if( ++m_nCount >= m_nAllocated) 
   {
      m_nAllocated *= 2;
      if( m_nAllocated == 0 ) 
		  m_nAllocated = 11;
      m_ppVoid = static_cast<LPVOID*>(realloc(m_ppVoid, m_nAllocated * sizeof(LPVOID)));
      if( m_ppVoid == NULL ) 
		  return false;
   }
   m_ppVoid[m_nCount - 1] = pData;
   return true;
}

bool CStdPtrArray::InsertAt(int iIndex, LPVOID pData)
{
   if( iIndex == m_nCount ) 
	   return add(pData);
   if( iIndex < 0 || iIndex > m_nCount )
	   return false;
   if( ++m_nCount >= m_nAllocated) 
   {
      m_nAllocated *= 2;
      if( m_nAllocated == 0 ) 
		  m_nAllocated = 11;
      m_ppVoid = static_cast<LPVOID*>(realloc(m_ppVoid, m_nAllocated * sizeof(LPVOID)));
      if( m_ppVoid == NULL ) 
		  return false;
   }
   memmove(&m_ppVoid[iIndex + 1], &m_ppVoid[iIndex], (m_nCount - iIndex - 1) * sizeof(LPVOID));
   m_ppVoid[iIndex] = pData;
   return true;
}

bool CStdPtrArray::SetAt(int iIndex, LPVOID pData)
{
   if( iIndex < 0 || iIndex >= m_nCount )
	   return false;
   m_ppVoid[iIndex] = pData;
   return true;
}

bool CStdPtrArray::remove(int iIndex)
{
   if( iIndex < 0 || iIndex >= m_nCount )
	   return false;
   if( iIndex < --m_nCount ) 
	   ::CopyMemory(m_ppVoid + iIndex, m_ppVoid + iIndex + 1, (m_nCount - iIndex) * sizeof(LPVOID));
   return true;
}

int CStdPtrArray::Find(LPVOID pData) const
{
   for( int i = 0; i < m_nCount; i++ ) 
   {
	   if( m_ppVoid[i] == pData ) 
		   return i;
   }
   return -1;
}

int CStdPtrArray::size() const
{
   return m_nCount;
}

LPVOID* CStdPtrArray::GetData()
{
   return m_ppVoid;
}

LPVOID CStdPtrArray::GetAt(int iIndex) const
{
   if( iIndex < 0 || iIndex >= m_nCount ) 
	   return NULL;
   return m_ppVoid[iIndex];
}

LPVOID CStdPtrArray::operator[] (int iIndex) const
{
   ASSERT(iIndex>=0 && iIndex<m_nCount);
   return m_ppVoid[iIndex];
}


/////////////////////////////////////////////////////////////////////////////////////
//
//

CStdValArray::CStdValArray(int iElementSize, int iPreallocSize /*= 0*/) : 
   m_pVoid(NULL), 
   m_nCount(0), 
   m_iElementSize(iElementSize), 
   m_nAllocated(iPreallocSize)
{
   ASSERT(iElementSize>0);
   ASSERT(iPreallocSize>=0);
   if( iPreallocSize > 0 )
	   m_pVoid = static_cast<LPBYTE>(malloc(iPreallocSize * m_iElementSize));
}

CStdValArray::~CStdValArray()
{
   if( m_pVoid != NULL ) free(m_pVoid);
}

void CStdValArray::Empty()
{   
   m_nCount = 0;  // NOTE: We keep the memory in place
}

bool CStdValArray::IsEmpty() const
{
   return m_nCount == 0;
}

bool CStdValArray::Add(LPCVOID pData)
{
   if( ++m_nCount >= m_nAllocated) 
   {
      m_nAllocated *= 2;
      if( m_nAllocated == 0 ) 
		  m_nAllocated = 11;
      m_pVoid = static_cast<LPBYTE>(realloc(m_pVoid, m_nAllocated * m_iElementSize));
      if( m_pVoid == NULL ) 
		  return false;
   }
   ::CopyMemory(m_pVoid + ((m_nCount - 1) * m_iElementSize), pData, m_iElementSize);
   return true;
}

bool CStdValArray::Remove(int iIndex)
{
   if( iIndex < 0 || iIndex >= m_nCount ) 
	   return false;
   if( iIndex < --m_nCount )
	   ::CopyMemory(m_pVoid + (iIndex * m_iElementSize), m_pVoid + ((iIndex + 1) * m_iElementSize), (m_nCount - iIndex) * m_iElementSize);
   return true;
}

int CStdValArray::size() const
{
   return m_nCount;
}

LPVOID CStdValArray::GetData()
{
   return static_cast<LPVOID>(m_pVoid);
}

LPVOID CStdValArray::GetAt(int iIndex) const
{
   if( iIndex < 0 || iIndex >= m_nCount )
	   return NULL;
   return m_pVoid + (iIndex * m_iElementSize);
}

LPVOID CStdValArray::operator[] (int iIndex) const
{
   ASSERT(iIndex>=0 && iIndex<m_nCount);
   return m_pVoid + (iIndex * m_iElementSize);
}


/////////////////////////////////////////////////////////////////////////////////////
//
//

CWaitCursor::CWaitCursor()
{
   m_hOrigCursor = ::SetCursor(::LoadCursor(NULL, IDC_WAIT));
}

CWaitCursor::~CWaitCursor()
{
   ::SetCursor(m_hOrigCursor);
}




void ProcessResourceTokens(base::String& v)
{
	/*
   // Replace string-tokens: %{nnn}  where nnn is a resource string identifier
	base::String temp;
   int iPos = v.indexOf('%');
   while( iPos >= 0 ) 
   {
      if( v[iPos + 1] == '{' ) 
	  {
         int iEndPos = iPos + 2;
         while( isdigit(GetAt(iEndPos)) ) 
			 iEndPos++;
         if( GetAt(iEndPos) == '}' ) 
		 {
            base::String sTemp = base::String::RES((UINT)_ttoi(m_pstr + iPos + 2));
           temp=v.replace(Mid(iPos, iEndPos - iPos + 1), sTemp);
         }
      }
      iPos =v. indexOf('%', iPos + 1);
   }
   */
}
