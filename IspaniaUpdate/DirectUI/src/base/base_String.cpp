#include "base_Pcheader.h"


#include "base_StringTable.h"

#include <base/base_UTFConverter.h>


namespace base
{
//#define GatMemPool() String::stringPool

	
	//struct strinter
	/** String pool used by the String class implementation. */
	//MemoryPool			*stringPool;//(40000,0,"string");

	SP(StringTable) _stringTable;

	void InitStringTable(void )
	{
		if (!_stringTable)
		_stringTable=new StringTable();
	}
	void FinishStringTable(void )
	{
		if (_stringTable)
			_stringTable=0;
	}

	
	StringTable* GatMemPool()
	{
		if (!_stringTable)
			_stringTable=new StringTable();

		return _stringTable;

	}


	/** Temporary buffer used by the String::c_str() implementation. */
	SQChar				cstrBuffer[40960];

	/** Temporary buffer index used by the String::c_str() implementation. */
	int					cstrBufferIndex;

static int allocateString( const SQChar*s, int len )
{
	
	return  GatMemPool()->Add(s,len);
}


static int allocateString( const void* data, int bytes, const IConverter& decoder )
{
	// find out UTF-8 length
	const uint8_t* databytes = reinterpret_cast<const uint8_t*>(data);
	int len = 0;
	UTFConverter encoder( UTFConverter::ENCODING_UTF8 );
	char buf[32];
	for ( int i = 0 ; i < bytes ; )
	{
		int decodedbytes = 1;
		int cp;
		if ( decoder.decode(databytes+i, databytes+bytes, &decodedbytes, &cp) )
		{
			i += decodedbytes;
			int encodedbytes = 0;
			encoder.encode( buf, buf+sizeof(buf), &encodedbytes, cp );
			len += encodedbytes;
		}
		else
		{
			++i;
		}
	}

	// alloc and set UTF-8 string
	int strh = _stringTable->Add((SQChar*)databytes, len );
	if ( len > 0 )
	{
		StringTable::_stringdata* ss = _stringTable->get(strh);
		SQChar*s=ss->_val;

		


		int d = 0;
		for ( int i = 0 ; i < bytes ; )
		{
			int decodedbytes = 1;
			int cp;
			if ( decoder.decode(databytes+i, databytes+bytes, &decodedbytes, &cp) )
			{
				i += decodedbytes;
				int encodedbytes = 0;
				encoder.encode( s+d, s+len, &encodedbytes, cp );
				d += encodedbytes;
			}
			else
			{
				++i;
			}
		}
	}
	
	return strh;
}



String::String():m_h(0)	
{
	//m_h=allocateString(_SC(""),-1 );
}


	String::String( uint32_t h  ):m_h(h)
	{
		if (h)
			_stringTable->ref( h );
	}

String::String( const SQChar* str ,int count ):m_h(0)
{
	if ( str )
	{
		
		
		m_h = allocateString( str,count );
	
		
	}

}
String::String( const SQChar * str ) :
	m_h(0)
{
	if ( str )
	{
		// DEBUG: print String ctor
		//printf( "String('%s')\n", str );
		
			m_h = allocateString( str,-1 );
		
		
	}
}


	String::String( const char* str ):m_h(0)
	{
		if (str)
		{
		
		SQChar temp[4096];
		int len=UTF8toSQChar((uint8_t*)str,-1,temp,4096)-1;

		if (len>0)
		m_h = allocateString( temp,len );
		}
	}


String::String( const void* data, size_t size, const IConverter& decoder ) :
	m_h( allocateString(data, size, decoder) )
{
}

String::String( const String& other ) :
	m_h( other.m_h )
{
	if ( other.m_h  )
		_stringTable->ref( other.m_h );
}
void String::clear(void)
{
	if ( m_h)
	{
		_stringTable->unref( m_h );
		
		
	}
	m_h=0;

}
String::~String()
{
	if ( m_h )
		_stringTable->unref( m_h );
}

String& String::operator=( const String& other )
{
	if ( other.m_h)
		GatMemPool()->ref( other.m_h );
	if ( m_h  )
		GatMemPool()->unref( m_h );
	m_h = other.m_h;
	return *this;
}

int String::length() const
{
	return (m_h)  ? _stringTable->get(m_h)->_len : 0;
}

SQChar String::charAt( int index ) const											
{
	StringTable::_stringdata *str=_stringTable->get(m_h);
	assert( index >= 0 && index <str->_len );
	SQChar* s =str->_val;
	return s[index];
}

void String::getChars( int begin, int end, SQChar* dest ) const
{
	StringTable::_stringdata *str=_stringTable->get(m_h);
	assert( begin >= 0 && begin <= str->_len );
	assert( begin >= 0 && begin <= end );
	assert( end <=  str->_len  );

	if ( end > begin )
	{
		SQChar* s =  str->_val;
		memcpy( dest, s+begin, (end-begin)*sizeof(SQChar) );
	}
}

void String::get( char* buf, int bufsize ) const
{
	StringTable::_stringdata *str=_stringTable->get(m_h);
	assert( buf != 0 );
	assert( bufsize > str->_len );

	int len = 0;
	if (  m_h )
	{
		SQChar* s =  str->_val;
		len = str->_len;
		if ( len >= bufsize )
			len = bufsize-1;
		SQChartoUTF8((uint8_t*)buf,bufsize,s,len);
		//memcpy( buf, s, len*sizeof(SQChar) );
	}
	if ( bufsize > 0 )
		buf[len] = 0;
}

const SQChar* String::c_str() const
{
	
	StringTable::_stringdata *str=_stringTable->get(m_h);
	//if (!str)
	//	return _SC("die");
	const int BUFSIZE = sizeof(cstrBuffer)/sizeof(cstrBuffer[0]);
	const SQChar* sz = (m_h != 0 ? str->_val : _SC(""));
	int len = str->_len;
	if ( len >= BUFSIZE )
		len = BUFSIZE-1;

	SQChar* buf = cstrBuffer;
	int& bufp = cstrBufferIndex;
	if ( bufp+len >= BUFSIZE )
		bufp = 0;
	
	assert( bufp+len < BUFSIZE );
	const int bufp0 = bufp;
	bufp += len+1;

	assert( bufp0 >= 0 && bufp0 < BUFSIZE );
	assert( bufp0+len >= 0 && bufp0+len < BUFSIZE );
	if ( len > 0 )
		memcpy( buf+bufp0, sz, len*sizeof(SQChar) );

	buf[bufp0+len] = 0;
	return buf+bufp0;
}

bool String::endsWith( const String& suffix ) const
{
	StringTable::_stringdata *str=_stringTable->get(m_h);
	StringTable::_stringdata *suffixStr=_stringTable->get(suffix.m_h);
	assert( suffixStr->_len > 0 );
	
	int		thislen		= str->_len;
	int		otherlen	= suffixStr->_len;

	if ( otherlen <= thislen )
		return 0 == scstrncmp( str->_val+thislen-otherlen,suffixStr->_val, otherlen );
	else
		return false;
}

bool String::startsWith( const String& prefix ) const
{
	StringTable::_stringdata *str=_stringTable->get(m_h);
	StringTable::_stringdata *suffixStr=_stringTable->get(prefix.m_h);

	assert( suffixStr->_len > 0 );
	
	int		thislen		= str->_len;
	int		otherlen	= suffixStr->_len;

	if ( otherlen <= thislen )
		return 0 == scstrncmp( str->_val,suffixStr->_val, otherlen );
	else
		return false;
}

int String::hashCode() const
{
	StringTable::_stringdata *str=_stringTable->get(m_h);

	return str->_hash;
}

int String::indexOf( SQChar ch, int index ) const
{
	StringTable::_stringdata *str=_stringTable->get(m_h);

	assert( index >= 0 && index < str->_len);

	int thislen =  str->_len;
	SQChar* s =str->_val;
	for ( ; index < thislen ; ++index )
	{
		if ( s[index] == ch )
			return index;
	}
	return -1;
}

int String::indexOf( const String& sustr, int index ) const
{
	StringTable::_stringdata *str=_stringTable->get(m_h);
	StringTable::_stringdata *sstr=_stringTable->get(sustr.m_h);
	assert( index >= 0 );
	assert( sstr->_len > 0 );

	int		thislen		=str->_len;
	int		slen		= sstr->_len;//dorp NULL
	int		lastIndex	= thislen - slen;
	SQChar*	s1			= sstr->_val;
	SQChar*	s2			= str->_val;

	for ( ; index <= lastIndex ; ++index )
	{
		int j = 0;
		for ( ; j < slen ; ++j )
		{
			if ( s1[j] != s2[index+j] )
				break;
		}
		if ( slen == j )
			return index;
	}
	return -1;
}

int String::lastIndexOf( SQChar ch ) const										
{
	StringTable::_stringdata *str=_stringTable->get(m_h);
	int len = str->_len;
	if ( 0 == len )
		return -1;
	else
		return lastIndexOf( ch, len-1 );
}

int String::lastIndexOf( SQChar ch, int index ) const
{
	StringTable::_stringdata *str=_stringTable->get(m_h);
	assert( index >= 0 && index < str->_len);

	SQChar* s =str->_val;
	for ( ; index >= 0 ; --index )
	{
		if ( s[index] == ch )
			return index;
	}
	return -1;
}

int String::lastIndexOf( const String& sustr, int index ) const
{
	StringTable::_stringdata *str=_stringTable->get(m_h);
	StringTable::_stringdata *sstr=_stringTable->get(sustr.m_h);

	assert( index >= 0 && index < str->_len );
	assert( sstr->_len > 0 );

	int		thislen		= str->_len;
	int		slen		= sstr->_len;
	SQChar*	s1			= sstr->_val;
	SQChar*	s2			= str->_val;

	if ( index+slen > thislen )
		index = thislen - slen;

	for ( ; index >= 0 ; --index )
	{
		int j = 0;
		for ( ; j < slen ; ++j )
		{
			if ( s1[j] != s2[index+j] )
				break;
		}
		if ( slen == j )
			return index;
	}
	return -1;
}

bool String::regionMatches( int thisoffset, const String& other, int otheroffset, int length ) const
{
	StringTable::_stringdata *str=_stringTable->get(m_h);
	StringTable::_stringdata *ostr=_stringTable->get(other.m_h);
	assert( other.length() > 0 );
	
	int		thislen		= str->_len;
	int		otherlen	= ostr->_len;
	SQChar*	s1			= str->_val;
	SQChar*	s2			= ostr->_val;

	if ( thisoffset >= 0 && 
		otheroffset >= 0 &&
		thisoffset+length <= thislen &&
		otheroffset+length <= otherlen )
	{
		for ( int c = 0 ; c < length ; ++c )
		{
			if ( s1[thisoffset+c] != s2[otheroffset+c] )
				return false;
		}
		return true;
	}
	return false;
}


String String::replace(const String& oldchar,const String& newchar ) const
{
	StringTable::_stringdata *oldstr=_stringTable->get(oldchar.m_h);
	StringTable::_stringdata *newstr=_stringTable->get(newchar.m_h);
	StringTable::_stringdata *str=_stringTable->get(m_h);


	int thislen = str->_len;
	
	

	SQChar tstr[2048];
	memset(tstr,0,2048*sizeof(SQChar));
	SQChar *pStr=tstr;
	if ( thislen > 0 )
	{
		SQChar temp[1024];
		
		
		SQChar* s = oldstr->_val;
		SQChar* s2 = str->_val;
		SQChar* s1 = newstr->_val;

		int i=0;
		int len=oldstr->_len;
		memset(temp,0,1024*sizeof(SQChar));
		int len1=newstr->_len;
		for ( int ci = 0 ; ci < thislen ; ++ci )
		{
			SQChar c = s2[ci];
			bool match=true;
			for (int l=0;l<len;l++)
			{
				if (s2[ci+l]!=s[l])
				{
					match=false;
					break;
				}				
			}
			if (match) 
			{
				if ((i+len1)>1024)
				
				{
					i=0;
					memcpy(pStr,temp,i*sizeof (SQChar));
					pStr+=i;
					memset(temp,0,1024*sizeof(SQChar));
					
				}							
				memcpy(&temp[i],s1,len1*sizeof(SQChar));
				i+=len1;
			}
			
			else
			{

				if ((i+1)>1024)			
				{
					i=0;
					memcpy(pStr,temp,i*sizeof (SQChar));
					pStr+=i;
					memset(temp,0,1024*sizeof(SQChar));

				}

				temp[i]=c;
				i++;

			}
			
			
			
			
		}
		memcpy(pStr,temp,i*sizeof (SQChar));
		pStr+=i;

		
	}

	return String(tstr);
}

String String::replace(const SQChar oldchar,const SQChar newchar ) const
{
	StringTable::_stringdata *str=_stringTable->get(m_h);

	int thislen = str->_len;
	SQChar tstr[1024];
	if ( thislen > 0 )
	{
		//str.m_h = allocateString( thislen );
		SQChar* s = tstr;
		SQChar* s2 =str->_val;
		
		for ( int i = 0 ; i < thislen ; ++i )
		{
			SQChar c = s2[i];
			if ( c == oldchar )
				c = newchar;
			s[i] = c;
		}
	}

	return String(tstr,thislen);
}

String String::substring( int begin, int end ) const
{
	StringTable::_stringdata *str=_stringTable->get(m_h);
	assert( begin >= 0 && end <= length() && end >= begin );

	SQChar tstr[1024];
	
	int len = end - begin;
	if ( len>0 )
	{
		
		
		memcpy( tstr, str->_val+begin, len*sizeof(SQChar) );
		tstr[len]=0;
	}
	
	return String(tstr,len);
}

String String::substring( int begin ) const
{
	StringTable::_stringdata *str=_stringTable->get(m_h);
	assert( begin >= 0 && begin <= str->_len );
	return substring( begin,  str->_len);
}

String String::toLowerCase() const
{
	StringTable::_stringdata *str=_stringTable->get(m_h);
	int thislen = str->_len;
	SQChar tstr[1024];
	if ( thislen > 0 )
	{
		
	
		SQChar* s0 = str->_val;
		
		for ( int i = 0 ; i < thislen ; ++i )
		{
			SQChar ch = s0[i];
			if ( (unsigned char)ch < (unsigned char)0x80 )
				ch = (char)tolower( ch );
			
			tstr[i] = ch;
		}
	}

	return String(tstr,thislen);
}

String String::toUpperCase() const
{
	StringTable::_stringdata *str=_stringTable->get(m_h);
	int thislen = str->_len;
	SQChar tstr[1024];
	if ( thislen > 0 )
	{
		SQChar* s0 = str->_val;
		
		for ( int i = 0 ; i < thislen ; ++i )
		{
			SQChar ch = s0[i];
			if ( (unsigned char)ch < (unsigned char)0x80 )
                ch = (char)toupper( ch );
			
			tstr[i] = ch;
		}
	}

	return String(tstr,thislen);
}

String String::trim() const
{
	StringTable::_stringdata *str=_stringTable->get(m_h);
	int thislen = str->_len;
	int begin	= 0;
	int end		= thislen;
	SQChar* s0 = str->_val;

	for ( ; begin < thislen ; ++begin )
	{
		SQChar ch = s0[begin];
		if ( (SQChar)ch >= (SQChar)0x80 ||
			!isspace( ch ) )
			break;
	}
	for ( ; end > 0 ; --end )
	{
		SQChar ch = s0[end-1];
		if ( (SQChar)ch >= (SQChar)0x80 ||
			!isspace( ch ) )
			break;
	}
	return substring( begin, end );
}

int String::compareTo( const String& other ) const 
{
	StringTable::_stringdata *str=_stringTable->get(m_h);
	StringTable::_stringdata *ostr=_stringTable->get(other.m_h);
	return scstrcmp(str->_val,ostr->_val);
		
}

int String::compareTo( const SQChar* other ) const 
{
	StringTable::_stringdata *str=_stringTable->get(m_h);
	return scstrcmp( str->_val, other );
}

String String::operator+( const SQChar* other ) const
{
	StringTable::_stringdata *str=_stringTable->get(m_h);
	assert( other );
	
	int len1 = str->_len;
	int len2 = scstrlen(other);
	int len = len1 + len2;
	
	SQChar tstr[1024];
	
	if ( len > 0 )
	{
		
		
		memcpy(tstr,str->_val,len1*sizeof(SQChar));
		memcpy(&tstr[len1],other,len2*sizeof(SQChar));
		tstr[len]=0;
	}

	return String(tstr,len);
}

String String::operator+( const String& other ) const
{
	StringTable::_stringdata *str=_stringTable->get(m_h);
	StringTable::_stringdata *ostr=_stringTable->get(other.m_h);

	int len1 = str->_len;
	int len2 = ostr->_len;
	int len = len1 + len2;
	
	SQChar tstr[1024];
	if ( len > 0 )
	{

		memcpy(tstr,str->_val,len1*sizeof(SQChar));
		memcpy(&tstr[len1],ostr->_val,len2*sizeof(SQChar));
		tstr[len]=0;
	}

	return String(tstr,len);
}

bool String::cpy( SQChar* buf, int bufsize, const SQChar* sz )
{
	assert( bufsize > 0 );
	int bufp = 0;
	while ( 0 != *sz && bufp+1 < bufsize )
		buf[bufp++] = *sz++;
	assert( bufp < bufsize );
	buf[bufp] = 0;
	return *sz == 0;
}

bool String::cpy( SQChar* buf, int bufsize, const String& str )
{
	StringTable::_stringdata *_str=_stringTable->get(str.m_h);
	assert( bufsize > 0 );
	
		return String::cpy( buf, bufsize, _str->_val );
	
	
}

bool String::cat( SQChar* buf, int bufsize, const SQChar* sz )
{
	assert( bufsize > 0 );
	int bufp = scstrlen(buf);
	assert( bufp < bufsize );
	while ( 0 != *sz && bufp+1 < bufsize )
		buf[bufp++] = *sz++;
	assert( bufp < bufsize );
	buf[bufp] = 0;
	return *sz == 0;
}

bool String::cat( SQChar* buf, int bufsize, const String& str )
{
	StringTable::_stringdata *_str=_stringTable->get(str.m_h);
	assert( bufsize > 0 );
	
		return String::cat( buf, bufsize, _str->_val );
	return true;
}

/*
int String::toUTF16( uint16_t * buf, int bufsize, const char*  str ,int strsize)
{


	return MultiByteToWideChar(CP_ACP,0,str,strsize,buf,bufsize);

}
*/
/*
int String::toUTF16Size( const String& str )
{
	int i;
	int WCharCount=0;
	int len=str.length();
	const char *strbuff=str.c_str();
	
	for (i=0;i<len;i++)
	{
		if (strbuff[i]>0xA0)
		{
			i++;
		}
		WCharCount++;

	}


	return WCharCount;

}

*/
} // base
int UTF8toSQChar(uint8_t*  buf, int bufsize,  SQChar*  str ,int strsize)
{


	return MultiByteToWideChar(CP_ACP,0,(LPCSTR)buf,bufsize,str,strsize);

}
int SQChartoUTF8(uint8_t*  buf, int bufsize, const SQChar*  str ,int strsize)
{


	return WideCharToMultiByte(CP_ACP,0,str,strsize,(LPSTR)buf,bufsize,0,0);

}

// MinG
