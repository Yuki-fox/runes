#include "stdafx.h"

#include "VersionData.h"


VersionData::VersionData()
{



	m_version[0]=1;
	m_version[1]=0;
	m_version[2]=1;
	m_version[3]=0;

}

void VersionData::SetData(LPCTSTR vers ,CString &path)
{

	TCHAR temp[512];

	lstrcpy(temp,(LPCTSTR)vers);
	TCHAR* pVer=_tcstok(temp, _T(".")); 

	if (pVer)
		m_version[0]=_tstoi(pVer);
	pVer=_tcstok(NULL, _T(".")); 
	if (pVer)
		m_version[1]=_tstoi(pVer);

	pVer=_tcstok(NULL, _T(".")); 
	if (pVer)
		m_version[2]=_tstoi(pVer);

	pVer=_tcstok(NULL, _T(".")); 
	if (pVer)
		m_version[3]=_tstoi(pVer);

	pVer=_tcstok(NULL, _T(".")); 


	int len=0;
	char ctemp[512];
	
	path=pVer;

}