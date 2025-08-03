#ifndef MING_BASE_BOBOSORT_H
#define MING_BASE_BOBOSORT_H

#pragma once

#include <base\base_base.h>
#include <base\base_String.h>





namespace base
{	
	

	class String;
	struct BASE_API _boboSort
	{		
	public:
		String m_Name;		
		int m_Handle;
		_boboSort* m_pPren;
		_boboSort* m_pNext;		
	public:
		_boboSort(int handle,const String& Name);
		int getHandle();

		virtual ~_boboSort();
	};
	class BASE_API BoboMagr
	{
	public:
		BoboMagr();
		~BoboMagr();
		void finish(void);

		void toTop(_boboSort *pObj);
		int find(const String& pName);
		void delbobo(int handle);
		void delbobo(_boboSort* );
		
		_boboSort * addbobo(int handle,const String& pName);
		void removeTail();
		
		_boboSort *m_pFirst;
		_boboSort *m_pTail;
		
		
	};
}//base
#endif // MING_BASE_BOBOSORT_H
