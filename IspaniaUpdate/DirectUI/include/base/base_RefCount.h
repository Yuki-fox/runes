#ifndef BASE_REFCOUNT_H
#define BASE_REFCOUNT_H
#pragma once 
#include <base\base_base.h>
#include <base\base_Assert.h>
#include <base\base_SmartPtr.h>
namespace base
{

	class BASE_API RefCount
	{
	public:
		RefCount	():m_refCount(0){}
		RefCount	( const RefCount& ):m_refCount(0){}
		virtual		~RefCount	(){assert(m_refCount==0);}
		RefCount&	operator=	( const RefCount& )		{return *this;}
		void		addRef		()	{++m_refCount;}
		void		release		()	{if ( --m_refCount == 0 ) delete this;}
		int			references	() const				{return m_refCount;}
	protected:
		int m_refCount;

	};

}//base
#endif//BASE_REFCOUNT_H