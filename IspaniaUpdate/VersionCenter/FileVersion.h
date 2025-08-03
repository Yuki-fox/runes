#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

//////////////////////////////////////////////////////////////////////
/*
 $History: FileVersion.h $
 * 
 * *****************  Version 3  *****************
 * User: Navis        Date: 6/18/99    Time: 3:35p
 * Updated in $/PJ
 * Standardized History and include format.
 * Implemented Insert Template Wizard.
 * 
 * *****************  Version 2  *****************
 * User: Navis        Date: 4/19/99    Time: 3:29p
 * Updated in $/PJ
 * class to obtain module version info.  Intend to use in About Dialog
 * box.
*/
//////////////////////////////////////////////////////////////////////
// by Manuel Laflamme from CodeGuru site.

class CFileVersion
{ 
// Construction
public: 
    CFileVersion();

// Operations	
public: 
    BOOL    Open(LPCTSTR lpszModuleName);
    void    Close();

	base::String QueryValue(LPCTSTR lpszValueName, DWORD dwLangCharset = 0);
    base::String GetFileDescription()  {return QueryValue(_T("FileDescription")); };
    base::String GetFileVersion()      {return QueryValue(_T("FileVersion"));     };
    base::String GetInternalName()     {return QueryValue(_T("InternalName"));    };
    base::String GetCompanyName()      {return QueryValue(_T("CompanyName"));     }; 
    base::String GetLegalCopyright()   {return QueryValue(_T("LegalCopyright"));  };
    base::String GetOriginalFilename() {return QueryValue(_T("OriginalFilename"));};
    base::String GetProductName()      {return QueryValue(_T("ProductName"));     };
    base::String GetProductVersion()   {return QueryValue(_T("ProductVersion"));  };

    BOOL    GetFixedInfo(VS_FIXEDFILEINFO& vsffi);
    base::String GetFixedFileVersion();
    base::String GetFixedProductVersion();

// Attributes
protected:
    LPBYTE  m_lpVersionData; 
    DWORD   m_dwLangCharset; 

// Implementation
public:
    ~CFileVersion(); 
}; 




