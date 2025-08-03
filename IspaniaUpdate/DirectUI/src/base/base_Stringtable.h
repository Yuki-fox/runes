#ifndef MING_BASE_STRINGTABLE_H
#define MING_BASE_STRINGTABLE_H
#include <base\base_Singleton.h>
#include <base\base_MTRefCount.h>
namespace base
{

	class StringTable :public Singleton<StringTable>,public MTRefCount
{
public:
	struct _stringdata
	{
		int    _len;
		int    _next;
		SQHash _hash;
		SQChar _val[1];

	};
	StringTable();
	virtual ~StringTable();
	 int Add(const SQChar *,SQInteger len);

	 _stringdata* get(int index)const ;
	void ref( int  );
	 void unref(int );
	 void Remove(int);
private:
	void Resize(SQInteger size);
	void AllocNodes(SQInteger size);
	SP(MemoryPool) m_pool;
	int  *_strings;
	SQUnsignedInteger _numofslots;
	SQUnsignedInteger _slotused;
};


}

#endif//MING_BASE_STRINGTABLE_H
