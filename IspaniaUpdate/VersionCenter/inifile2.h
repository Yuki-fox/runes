#ifndef		__INIFILE2_H_2008_6__
#define		__INIFILE2_H_2008_6__

#pragma warning (disable:4786)
#pragma warning (disable:4949)
#include	<base\base_string.h>
//#include	<set>
//#include	<tchar.h>
//#include	<windows.h>


#include "SimpleIni.h"
#pragma unmanaged

//using namespace std;


//#ifdef UNICODE 
//#define CTString  std::wstring
//#else
//#define CTString  std::string
//#endif


class IniFile2 
{
protected:

	base::String          _Section;

	CSimpleIni        _ini;
public:
	IniFile2();

	CSimpleIni&   GetIni() {return _ini; }
	//設定INI檔案 (可負數設定)
	bool	SetIniFile(LPCTSTR FileName);
	bool	SetIniData(LPVOID  data,DWORD size);
	void	SetSection(LPCTSTR section);
	base::String GetSection(void);
	int		Int(LPCTSTR key);
	int		IntDef(LPCTSTR key,int defValue =0);
	base::String	Str(LPCTSTR key);
	base::String	StrDef(LPCTSTR key,LPCTSTR defValue =0);
	float	Float( LPCTSTR key );
	float	FloatDef(LPCTSTR key ,float defValue =0);

	void SetUnicode(bool s);
	void Save(LPCTSTR FileName,bool saveB=true);
	void SaveString (std::string& buff);
	bool	SetInt(LPCTSTR key,int value );


	bool	SetStr(LPCTSTR key, LPCTSTR value);
	bool	SetFloat( LPCTSTR key ,float f );
	
};


#pragma managed
#endif