#pragma once

#include <UI\MinG_Ui.h>

struct Resource;
class CZipResource :public IResource
{
public:
	CZipResource();
	virtual ~CZipResource();

	void AddResource(LPCTSTR ZipPath,char *password);

	void Finish(void);

	//interface
	 SQStream* Create(LPCTSTR file);
	 void Delete(SQStream*stream);

	

	 //data
private:

	

	base::Array<Resource*> m_res;
	

};