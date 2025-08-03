//////////////////////////////////////////////////////////////////////////
//StringTable
/*
* The following code is based on Lua 4.0 (Copyright 1994-2002 Tecgraf, PUC-Rio.)
* http://www.lua.org/copyright.html#4
* http://www.lua.org/source/4.0.1/src_lstring.c.html
*/

#include "base_Pcheader.h"
#include "base_StringTable.h"

#ifdef UNICODE
	#define CharLen(l) (l<<1)
#else
	#define CharLen(l) l
#endif


namespace base 
{
	
	inline SQHash _hashstr (const SQChar *s, size_t l)
	{
		SQHash h = (SQHash)l;  /* seed */
		size_t step = (l>>5)|1;  /* if string is too long, don't hash all its chars */
		for (; l>=step; l-=step)
			h = h ^ ((h<<5)+(h>>2)+(unsigned short)*(s++));
		return h;
	}
	StringTable::  _stringdata* StringTable::get(int index) const
	  {
		  return ( _stringdata*)m_pool->get(index);

	  }
	StringTable::StringTable():MTRefCount(OBJECT_INITMUTEX)
{
	AllocNodes(4);
	_slotused = 0;

	m_pool=new base::MemoryPool(400000,0,_SC("String"));

	Add(_SC(""),0);
}

StringTable::~StringTable()
{
	unref(0);
	//Remove(0);
	delete []_strings;
	_strings=NULL;
	m_pool=NULL;
}
void StringTable::ref( int  index)
{
//	if (index==0)
	//	index=0;
	synchronized( m_mutex );
	m_pool->ref(index);

}
void StringTable::unref(int index)
{
//	if (index==0)
	//	index=0;
	synchronized( m_mutex );
	if (m_pool->unref(index)==0)
    Remove(index);
}
void StringTable::AllocNodes(SQInteger size)
{
	_numofslots=size;
	//_slotused=0;
	_strings=new int[_numofslots];
	memset(_strings,-1,sizeof(int)*_numofslots);
}

int StringTable::Add(const SQChar *news,SQInteger len)
{
	synchronized( m_mutex );
	if(len<0)
		len = (SQInteger)scstrlen(news);
	
	int size=CharLen(len);
	SQHash h = _hashstr(news,len)&(_numofslots-1);
	int sh;
	_stringdata *s;
	for (sh = _strings[h]; sh!=-1; sh = s->_next)
	{
		s=(_stringdata *)m_pool->get(sh);

		if(s->_len == len && (!memcmp(news,s->_val,size)))
		{
			//if (len==0)
			//	len=0;
			if (len>0)
				m_pool->ref( sh );
			
			return sh; //found
		}
	}


	int handle =m_pool->allocate(size+sizeof(_stringdata));
	_stringdata *t=(_stringdata *)m_pool->get(handle);
	//SQ_MALLOC(rsl(len)+sizeof(SQString));
	
	memcpy(t->_val,news,size);
	t->_val[len] = _SC('\0');
	t->_len = len;
	t->_hash = _hashstr(news,len);
	t->_next = _strings[h];
	_strings[h] = handle;
	_slotused++;
	if (_slotused > _numofslots)  /* too crowded? */
		Resize(_numofslots*2);
	return handle;
}

void StringTable::Resize(SQInteger size)
{
	base::Debug::printf(_SC("StringTable::Resize %d"),size);
	
	SQInteger oldsize=_numofslots;
	int *oldtable=_strings;
	AllocNodes(size);
	for (SQInteger i=0; i<oldsize; i++)
	{
		int sh = oldtable[i];
		while(sh!=-1)
		{

			
			_stringdata* p=(_stringdata *)m_pool->get(sh);
			int next = p->_next;
			SQHash h = p->_hash&(_numofslots-1);
			p->_next = _strings[h];
			_strings[h] = sh;

	//		base::Debug::printf(_SC("%d:handle=%d,%s"),i,sh,p->_val);
			sh = next;
		}
	}
	delete []oldtable;
}

void StringTable::Remove(int index)
{
	int sh;
	_stringdata *s;
	_stringdata *bs;

	_stringdata *prev=NULL;

	bs=(_stringdata*)m_pool->get(index);

	SQHash h = bs->_hash&(_numofslots - 1);

	for (sh = _strings[h]; sh!=-1; )
	{
		s=(_stringdata *)m_pool->get(sh);
		if(s == bs)
		{
			if(prev)
				prev->_next = s->_next;
			else
				_strings[h] = s->_next;
			_slotused--;
			SQInteger slen = s->_len;

			m_pool->unalloc(index);
			
			return;
		}
		prev = s;
		sh = s->_next;
	}
	assert(0);//if this fail something is wrong
}
}