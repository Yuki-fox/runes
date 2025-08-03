#pragma once
struct VersionData
{
	int m_version[4];
	//TCHAR  m_country[128];
	//TCHAR  m_path[128];

	VersionData();

	void SetData(LPCTSTR data ,CString &path);

};