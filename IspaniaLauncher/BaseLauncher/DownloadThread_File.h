#pragma once
#include "base_Thread.h"
class BaseCore;
class DownloadThreadFile :public base::Thread
{
BaseCore *m_core;
public:
	DownloadThreadFile(BaseCore *);
	virtual ~DownloadThreadFile(void);
	
	 void	run();
};

class DownloadThreadFileVCp2p :public base::Thread
{
	BaseCore *m_core;
public:
	DownloadThreadFileVCp2p(BaseCore *);
	virtual ~DownloadThreadFileVCp2p(void);

	void	run();
};

