#include "stdafx.h"
#include "ZipResource.h"

#include <sqlib\sq_stdio.h>

struct Resource
{
	HZIP m_hz; 
 
	base::String m_ResPath;
	Resource();
	void Initial(LPCTSTR ZipPath,char *password);
	SQStream* Create(LPCTSTR fil);
	~Resource();
};
Resource::Resource():m_hz(NULL)
{ 
 
}
void Resource::Initial(LPCTSTR ZipPath,char *password)
{
	TCHAR temp[512];
	wsprintf(temp,_T("%s.res"),ZipPath);
	m_ResPath=ZipPath;
	m_hz = OpenZip( temp ,password );

}


Resource::~Resource()
{
	if(m_hz)
		CloseZip(m_hz);
}
struct MemeStream :public SQStream
{
	MemeStream(SQInteger size) 
	{
		_size = size;
		_allocated = size;
		_buf = new unsigned char [size];
		memset(_buf, 0, _size);
		_ptr = 0;
		_owns = true;
	}
	virtual ~MemeStream()
	{
		delete []_buf;
		
	}
	SQInteger Write(void *buffer, SQInteger size) 
	{
		if(!CanAdvance(size)) {
			GrowBufOf(_ptr + size - _size);
		}
		memcpy(&_buf[_ptr], buffer, size);
		_ptr += size;
		return size;
	}
	SQInteger Read(void *buffer,SQInteger size) 
	{
		SQInteger n = size;
		if(!CanAdvance(size)) {
			if((_size - _ptr) > 0)
				n = _size - _ptr;
			else return 0;
		}
		memcpy(buffer, &_buf[_ptr], n);
		_ptr += n;
		return n;
	}
	bool Resize(SQInteger n) 
	{
		if(!_owns) 
			return false;
		if(n != _allocated) 
		{
			unsigned char *newbuf = new unsigned char [n];
			memset(newbuf,0,n);
			if(_size > n)
				memcpy(newbuf,_buf,n);
			else
				memcpy(newbuf,_buf,_size);
			delete []_buf;
			_buf=newbuf;
			_allocated = n;
			if(_size > _allocated)
				_size = _allocated;
			if(_ptr > _allocated)
				_ptr = _allocated;
		}
		return true;
	}
	bool GrowBufOf(SQInteger n)
	{
		bool ret = true;
		if(_size + n > _allocated) 
		{
			if(_size + n > _size * 2)
				ret = Resize(_size + n);
			else
				ret = Resize(_size * 2);
		}
		_size = _size + n;
		return ret;
	}
	bool CanAdvance(SQInteger n) 
	{
		if(_ptr+n>_size)
			return false;
		return true;
	}
	SQInteger Seek(SQInteger offset, SQInteger origin) 
	{
		switch(origin) 
		{
		case SQ_SEEK_SET:
			if(offset > _size || offset < 0)
				return -1;
		_ptr = offset;
		break;
		case SQ_SEEK_CUR:
		if(_ptr + offset > _size || _ptr + offset < 0) 
			return -1;
		_ptr += offset;
		break;
		case SQ_SEEK_END:
		if(_size + offset > _size || _size + offset < 0) 
			return -1;
		_ptr = _size + offset;
		break;
		default: 
			return -1;
		}
		return 0;
	}
	bool IsValid() 
	{
		return _buf?true:false;
	}
	bool EOS() 
	{
		return _ptr == _size;
	}
	SQInteger Flush() { return 0; }
	SQInteger Tell() { return _ptr; }
	SQInteger Len() { return _size; }
	SQUserPointer GetBuf(){ return _buf; }
private:
	SQInteger _size;
	SQInteger _allocated;
	SQInteger _ptr;
	unsigned char *_buf;
	bool _owns;
};
SQStream* Resource::Create(LPCTSTR file)
{
	ZRESULT	zRet;	
	ZIPENTRY ze; 	
	int index;

	SQStream* ret=NULL;

	TCHAR temp[2048];
	wcscpy(temp,file);
	wcslwr(temp);

	if (m_hz)
	{ 
		zRet=FindZipItem(m_hz,temp,true,&index,&ze);
		if ((zRet==ZR_OK) ||(zRet==ZR_MORE) )
		{			
			DWORD len=ze.unc_size;

			MemeStream *mem=new MemeStream(len);
			if (mem)
			{
				SQUserPointer data=mem-> GetBuf();
				zRet=UnzipItem(m_hz,index,data,len);
				if (zRet==ZR_OK|| zRet==ZR_MORE) 
					ret= mem;
			}


		}
	}
	if (!ret)
	{
		TCHAR temp[512];
		wsprintf(temp,_T("%s/%s"),m_ResPath.c_str(),file);
		SQFILE file=sqstd_fopen(temp,_T("rb"));
		if (file)
		{
			sqstd_fseek(file,0,SQ_SEEK_END);
			int size=sqstd_ftell(file);
			sqstd_fseek(file,0,SQ_SEEK_SET);

			MemeStream *mem=new MemeStream(size);
			if (mem)
			{
				SQUserPointer data=mem-> GetBuf();
				sqstd_fread(data,1,size,file);
				ret= mem;
			}
			sqstd_fclose(file);
		}
	}
	return ret;
}
CZipResource::CZipResource()
{

}

CZipResource::~CZipResource()
{
	Finish();
}
void CZipResource::AddResource(LPCTSTR ZipPath,char *password)
{	

	Resource * zip=new Resource();


	zip->Initial(ZipPath,password);
	m_res.add(zip);
	
	
} 

void CZipResource::Finish(void)
{
	int i;
	for (i=0;i<m_res.size();i++)
	{
		delete m_res[i];
		m_res[i]=NULL;
	}
	m_res.clear();


}
SQStream* CZipResource::Create(LPCTSTR file)
{
	SQStream*ret=NULL;
	int i;
	for (i=0;i<m_res.size();i++)
	{
		ret= m_res[i]->Create(file);
		if (ret)
			break;
	}

	return ret;

}
void CZipResource::Delete(SQStream* stream)
{

	delete stream;
}