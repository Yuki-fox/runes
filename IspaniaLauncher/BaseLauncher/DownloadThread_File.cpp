#include "stdafx.h"
#include "DownloadThread_File.h"
#include "BaseCore.h"
DownloadThreadFile::DownloadThreadFile(BaseCore * core):m_core(core)
{

}

DownloadThreadFile::~DownloadThreadFile(void)
{
}
void DownloadThreadFile::run()
{

	m_core->DownloadThread_File();
}


DownloadThreadFileVCp2p::DownloadThreadFileVCp2p(BaseCore * core):m_core(core)
{

}

DownloadThreadFileVCp2p::~DownloadThreadFileVCp2p(void)
{
}
void DownloadThreadFileVCp2p::run()
{

	m_core->DownloadThread_File2();
}


