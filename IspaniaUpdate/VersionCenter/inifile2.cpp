#include "stdafx.h"
#include "iniFile2.h"



#pragma warning (disable:4949)
#pragma unmanaged

//**************************************************************************************
//ÀRºA¸ê®Æ
//**************************************************************************************
IniFile2::IniFile2()
{
	_Section=_T("BOOT");
}

bool IniFile2::SetInt(LPCTSTR key,int value )

{
	TCHAR temp[256];
	_stprintf(temp,_T("%d"),value);

	if(	SetStr( key , temp ) == false )
		return false;
	return true; 
}

int		IniFile2::Int(LPCTSTR key )

{
	base::String a=Str(key);
	if (a.length()==0)
		return 0;

	return _tstoi(a);
}

int		IniFile2::IntDef(LPCTSTR key,int def )

{
	base::String a=Str(key);
	if (a.length()==0)
		return def;

	return _tstoi(a);
}


void	IniFile2::SetSection(LPCTSTR section)
{
	_Section=section;

}
base::String 	IniFile2::GetSection(void)
{

	return _Section;
}
float	IniFile2::Float( LPCTSTR key )
{
	base::String a=Str(key);
    if (a.length()==0)
		return 0;

	return (float)_tstof(a);
}

base::String IniFile2::StrDef(LPCTSTR key,LPCTSTR defValue)
{
	base::String a=Str(key);
	if (a.length()==0)
		return defValue;

	return a;
}
float	IniFile2::FloatDef( LPCTSTR key  ,float def  )
{
	base::String a=Str(key);
    if (a.length()==0)
		return def;

	return (float)_tstof(a);
}
bool	IniFile2::SetFloat( LPCTSTR key ,float f )
{
	TCHAR temp[2048];
	_stprintf(temp,_T("%f"),f);

	if(	SetStr( key , temp ) == false )
		return false;

return true;
}

bool IniFile2::SetStr(LPCTSTR key, LPCTSTR value)
{
	TCHAR Buf[2048];

	SI_Error 	Ret;

	
	Ret=_ini.SetValue(_Section,key ,value);
		 
//	SI_UPDATED  =  1,   //!< An existing value was updated
	//	SI_INSERTED =  2, 
		
	
	if( Ret <0 )
	{
		_stprintf( Buf , _T("IniFile : Name = %s not find!") , key );
		OutputDebugString( Buf );
		return false;
	}

	return true;
}
base::String IniFile2::Str(LPCTSTR key)
{
	
	
	LPCTSTR pValue=_ini.GetValue(_Section, key  );

	if(!pValue)
	{
		TCHAR Buf[1024];
		_stprintf( Buf , _T("IniFile : Name = %s not find!"), key );
		OutputDebugString( Buf );
		return _T("");
	}

	return pValue;
}

bool	IniFile2::SetIniFile(LPCTSTR filename)
{
	TCHAR Path[1024];
	
	lstrcpy(Path,filename);
	// if file contain filepath
	if( PathFileExists( filename ) == false )
	{		
		TCHAR tCurrentDirectory[2048];
		GetCurrentDirectory(sizeof(tCurrentDirectory),tCurrentDirectory);
		wsprintf(Path,_T("%s\\%s") ,tCurrentDirectory,filename);	
	}
	return _ini.LoadFile(Path)==SI_OK;


	//return true;
}


void IniFile2::SetUnicode(bool s)
{
_ini.SetUnicode(s);

}
bool IniFile2::SetIniData(LPVOID data,DWORD size)
{

	return _ini.Load((char *)data,size)==SI_OK;


	//return true;
}

void IniFile2::Save(LPCTSTR FileName,bool saveB)
{

	_ini.SaveFile(FileName,saveB);



}
/*
class StringWriter : public CSimpleIni::OutputWriter 
{
	base::Array<char>& m_buff;
public:
	StringWriter(base::Array<char>& buff) : m_buff(buff) { }
	void Write(const char * a_pBuf) 
	{
		m_buff;
	}
private:
	StringWriter(const StringWriter &);             // disable
	StringWriter & operator=(const StringWriter &); // disable
};
*/
void IniFile2::SaveString (std::string& buff)
{

	_ini.Save(buff);



}
#pragma managed