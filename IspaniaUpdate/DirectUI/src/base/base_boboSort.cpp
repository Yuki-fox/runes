//#include "StdAfx.h"





#include "base_pcheader.h"




//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


namespace base
{
	

	
	_boboSort::_boboSort(int handle,const String& Name):m_Name(Name)
	{
		m_Handle=handle;

		m_pNext=m_pPren=NULL;		
	}
	int _boboSort::getHandle()
	{
		return m_Handle;
	}
	_boboSort::~_boboSort()
	{
		
	}
	void BoboMagr::delbobo(int handle)
	{
		_boboSort *pNext;
		_boboSort *pObj;
		pNext=m_pFirst;
		while(pNext)
		{
			pObj=pNext;
			pNext=pNext->m_pNext;
			if (pObj->m_Handle==handle)
			{
				delbobo(pObj);
				
				break;
				
			}
			
		}
		
		
	}
	
	
	
	BoboMagr::BoboMagr()
	{
		m_pFirst=m_pTail=NULL;
	}
	BoboMagr::~BoboMagr()
	{
		finish();
	}
	void BoboMagr::finish(void)
	{
		_boboSort *pNext;
		_boboSort *pObj;
		pNext=m_pFirst;
		while(pNext)
		{
			pObj=pNext;
			pNext=pNext->m_pNext;
			
			delete pObj;
			
		}
		m_pFirst=NULL;
		m_pTail=NULL;
		
	}
	int BoboMagr::find(const String& pName)
	{
		int handle=-1;
		_boboSort *pNext;
		_boboSort *pObj;
		pNext=m_pFirst;
		while(pNext)
		{
			pObj=pNext;
			pNext=pNext->m_pNext;
			if(pName==pObj->m_Name)
			{
				//找到了
				handle=pObj->getHandle();
				//放在最高點
				toTop(pObj);
				break;
			}
		}
		return handle;
		
	}
	void BoboMagr::toTop(_boboSort *pObj)
	{
		_boboSort* pNext=pObj->m_pNext;
		_boboSort* pPren=pObj->m_pPren;
		
		//處理下端節點
		if(pNext)
			pNext->m_pPren=pPren;
		else
			m_pTail=pPren; //尾端改變
		
		//處理上端節點
		if(pPren)
			pPren->m_pNext=pNext;
		else
			m_pFirst=pNext; //前端改變 
		
		
		//堆到最高
		if(m_pFirst)
		{
			m_pFirst->m_pPren=pObj;
		}
		else
			m_pTail=pObj;//尾端改變
		
		pObj->m_pPren=NULL;
		
		pObj->m_pNext=m_pFirst;
		m_pFirst=pObj;//前端改變
		
		
		
	}
		void BoboMagr::removeTail()
		{
			_boboSort* pren=0;
			_boboSort* tail=m_pTail;


			if (tail)
			{
				pren=tail->m_pPren;			
				delete tail;
			}
			m_pTail=pren;
			if(pren==NULL)
				m_pFirst=0;

		}
		void  BoboMagr::delbobo(_boboSort* pObj)
		{

			_boboSort* pNext=pObj->m_pNext;
			_boboSort* pPren=pObj->m_pPren;

			//處理下端節點
			if(pNext)
				pNext->m_pPren=pPren;
			else
				m_pTail=pPren; //尾端改變

			//處理上端節點
			if(pPren)
				pPren->m_pNext=pNext;
			else
				m_pFirst=pNext; //前端改變 
			delete pObj;

		}
		_boboSort * BoboMagr::addbobo(int handle,const String &pName)
	{
		_boboSort *pNewObj;
		pNewObj= new _boboSort(handle,pName);
		
		
		//新的節點不能用ToTop
		//堆到最高
		
		
		
		if(m_pFirst)
		{
			
			m_pFirst->m_pPren=pNewObj;
			
		}
		else
			m_pTail=pNewObj;//尾端改變
		
		pNewObj->m_pNext=m_pFirst;
		
		
		m_pFirst=pNewObj;//前端改變
		
		return pNewObj;
		
	}
}