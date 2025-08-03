/*	see copyright notice in squirrel.h */
#ifndef _SQSTRING_H_
#define _SQSTRING_H_



struct SQString : public SQRefCounted
{
	SQString(){}
	~SQString(){}
public:
	static SQString *Create(const SQChar *, SQInteger len = -1 );
	SQInteger Next(const SQObjectPtr &refpos, SQObjectPtr &outkey, SQObjectPtr &outval);
	void Release();

	int m_h;
	
	//SQSharedState *_sharedstate;
	//int  _next; //chain for the string table
	SQInteger len();
	SQHash hashCode();
	const SQChar* val();
};



#endif //_SQSTRING_H_
