#ifndef MING_BASE_STRING_H
#define MING_BASE_STRING_H

#include <base\base_BaseTypes.h>
#include <base/base_base.h>

//#include <base/base_assert.h>

//#include <base/base_MemoryPool.h>

namespace base
{


	class IConverter;


	/** 
	* Immutable Unicode character base::String. Used encoding is UTF-8.
	* String uses internal memory pool, which allows
	* low-overhead allocation and compact operation to avoid
	* run-time memory fragmentation. Not thread-safe.
	* 
	* @ingroup base
	*/
	class BASE_API String
	{
	public:
		/** Creates an zero length base::String. */
		String();

		/** 
		* Creates a base::String from the 0-terminated UTF-8 code unit sequence.
		* @exception OutOfMemoryException
		*/
		String( const SQChar* str );
		String( uint32_t h  );

		String( const char* str );


		String( const SQChar* str ,int count );

		/** 
		* Creates a base::String from the encoded character sequence.
		* Ignores invalid character sequences.
		* @exception OutOfMemoryException
		*/
		String( const void* data, size_t size, const IConverter& decoder );

		/** 
		* Copy by reference. 
		*/
		String( const String& other );

		///
		~String();

		/** 
		* Copy by reference. 
		*/
		String&		operator=( const String& other );

		/** 
		* Returns number of UTF-8 code units in the base::String. 
		*/
		int			length() const;


		size_t size (void) const {return length(); }
		bool empty (void) const {return length()==0; }

		/**
		* Returns UTF-8 code unit at specified index. 
		*/
		SQChar		charAt( int index ) const;

		SQChar		at( int index ) const{return charAt(index) ;}


		inline SQChar  operator[]( int index )const
		{
			//assert( index >= 0 && index < length() );
			return charAt(index);
		}


		/**
		* Copies characters from this base::String into the destination character array.
		* NOTE: Does not terminate output with zero, see get() for getting full 
		* base::String in zero-terminated form.
		*
		* @param begin Index to the beginning (inclusive) of the substring.
		* @param end Index to the end (exclusive) of the substring.
		*/
		void		getChars( int begin, int end, SQChar* dest ) const;

		/**
		* Copies characters from this base::String into the destination character array
		* and terminates the output with zero.
		* Asserts if buffer not large enough, but terminates it still with zero.
		* @param buf [out] Receives the base::String.
		* @param bufsize Size of the buffer.
		*/
		void		get( char* buf, int bufsize ) const;

		/**
		* Returns 0-terminated UTF-8 data.
		*
		* WARNING: This is convenience method, which uses static buffer
		* to store base::String characters which it returns. So calling the function
		* too many times in the same statement or with too long base::String
		* might result in loss of information in the function return value.
		* Always use get or cpy in 'mission critical' situations and this function
		* only for example in debug output.
		*/
		const SQChar*	c_str() const;

		/**
		* Returns true if the base::String ends with specified substring.
		*/
		bool		endsWith( const String& suffix ) const;

		/**
		* Returns true if the base::String starts with specified substring.
		*/
		bool		startsWith( const String& prefix ) const;

		/**
		* Returns hash code for this base::String.
		*/
		int			hashCode() const;

		/**
		* Returns the first index within this base::String of the specified character.
		*
		* @param ch Character to find.
		* @return Index of the found position or -1 if the character was not found from the base::String.
		*/
		int			indexOf( SQChar ch ) const											{return indexOf(ch,0);}

		/**
		* Returns the first index within this base::String of the specified character.
		* Starts the search from the specified position.
		*
		* @param ch Character to find.
		* @param index The first position to search.
		* @return Index of the found position or -1 if the character was not found from the base::String.
		*/
		int			indexOf( SQChar ch, int index ) const;

		/**
		* Returns the first index within this base::String of the specified substring.
		*
		* @param str Substring to find.
		* @return Index of the found position or -1 if the substring was not found from the base::String.
		*/
		int			indexOf( const String& str ) const									{return indexOf(str,0);}

		/**
		* Returns the first index within this base::String of the specified substring.
		* Starts the search from the specified position.
		*
		* @param str Substring to find.
		* @param index The first position to search.
		* @return Index of the found position or -1 if the substring was not found from the base::String.
		*/
		int			indexOf( const String& str, int index ) const;


		//size_t find( char ch ) const{ return (size_t)indexOf(ch,1);}

		/**
		* Returns the last index within this base::String of the specified character.
		*
		* @param ch Character to find.
		* @return Index of the found position or -1 if the character was not found from the base::String.
		*/
		int			lastIndexOf( SQChar ch ) const;


		void clear(void);

		/**
		* Returns the last index within this base::String of the specified character.
		* Starts the search from the specified position.
		*
		* @param ch Character to find.
		* @param index The last position to search.
		* @return Index of the found position or -1 if the character was not found from the base::String.
		*/
		int			lastIndexOf( SQChar ch, int index ) const;

		/**
		* Returns the last index within this base::String of the specified substring.
		*
		* @param str Substring to find.
		* @return Index of the found position or -1 if the substring was not found from the base::String.
		*/
		int			lastIndexOf( const String& str ) const								{return lastIndexOf(str,length()-1);}

		/**
		* Returns the last index within this base::String of the specified substring.
		* Starts the search from the specified position.
		*
		* @param str Substring to find.
		* @param index The last position to search.
		* @return Index of the found position or -1 if the substring was not found from the base::String.
		*/
		int			lastIndexOf( const String& str, int index ) const;

		/**
		* Tests if two base::String regions are equal.
		*
		* @param thisoffset Offset to this base::String.
		* @param other The other base::String to compare.
		* @param otheroffset Offset to other base::String.
		* @param length Length of the sequences to compare.
		*/
		bool		regionMatches( int thisoffset, const String& other, int otheroffset, int length ) const;

		/**
		* Returns a new base::String which has oldchar characters replaced with newchar.
		* @exception OutOfMemoryException
		*/
		String		replace(const String& oldchar,const String& newchar ) const;
		String		replace(const SQChar oldchar,const SQChar newchar ) const;

		/**
		* Returns a new base::String that is a substring of this base::String.
		*
		* @param begin Index to the beginning (inclusive) of the substring.
		* @param end Index to the end (exclusive) of the substring.
		* @exception OutOfMemoryException
		*/
		String		substring( int begin, int end ) const;

		/**
		* Returns a new base::String that is a substring of this base::String.
		*
		* @param begin Index to the beginning (inclusive) of the substring.
		* @exception OutOfMemoryException
		*/
		String		substring( int begin ) const;

		/**
		* Returns a new base::String that has all characters of this base::String converted to lowercase.
		* Doesn't handle locale dependent special casing.
		* @exception OutOfMemoryException
		*/
		String		toLowerCase() const;

		/**
		* Returns a new base::String that has all characters of this base::String converted to uppercase.
		* Doesn't handle locale dependent special casing.
		* @exception OutOfMemoryException
		*/
		String		toUpperCase() const;

		/**
		* Returns a new base::String that is otherwise identical to this base::String 
		* but has whitespace removed from both ends of the base::String.
		* @exception OutOfMemoryException
		*/
		String		trim() const;

		/**
		* Bitwise lecigographical compare between this base::String and other base::String. 
		* @return If this base::String is lexicographically before other then the return value is <0 and if this base::String is after other base::String then >0. If the strings are equal then the return value is 0.
		*/
		int			compareTo( const String& other ) const;

		/**
		* Bitwise lecigographical compare between this base::String and other base::String. 
		* @return If this base::String is lexicographically before other then the return value is <0 and if this base::String is after other base::String then >0. If the strings are equal then the return value is 0.
		*/
		int			compareTo( const SQChar* other ) const;

		/** Concatenate 0-terminated UTF-8 char sequence and String. */
		

		
		operator const SQChar*()const{return c_str();}
		String 	operator+( const SQChar* other ) const;
		operator const SQChar*(){return c_str();}


		/** Concatenate two Strings. */
		String 	operator+( const String& other ) const;

		/** Compare if two strings are equal. */
		bool operator==( const SQChar* other ) const						{return 0==compareTo(other);}

		/** Compare if two strings are equal. */
		bool operator==( const String& other ) const						{return 0==compareTo(other);}

		/** Compare if two strings are inequal. */
		bool operator!=( const SQChar* other ) const						{return 0!=compareTo(other);}

		/** Compare if two strings are inequal. */
		bool operator!=( const String& other ) const						{return 0!=compareTo(other);}

		/** Lexicographical compare. */
		bool operator<( const SQChar* other ) const						{return compareTo(other)<0;}

		/** Lexicographical compare. */
		bool operator<( const String& other ) const						{return compareTo(other)<0;}

		/** 
		* Safe zero-terminated character base::String copy to limited buffer. 
		* @return true if base::String fit to the buffer.
		*/
		static bool cpy( SQChar* buf, int bufsize, const SQChar* sz );

		/** 
		* Safe base::String copy to limited buffer. 
		* @return true if base::String fit to the buffer.
		*/
		static bool cpy( SQChar* buf, int bufsize, const String& str );

		/** 
		* Safe zero-terminated character base::String concatenation to limited buffer. 
		* @return true if base::String fit to the buffer.
		*/
		static bool cat( SQChar* buf, int bufsize, const SQChar* sz );
		static bool cat( SQChar* buf, int bufsize, const String& str );

		//static int toUTF16( uint16_t * buf, int bufsize, const char*  str ,int strsize );
		//static int toUTF16Size( const String& str );

		/** 
		* Safe base::String concatenation to limited buffer. 
		* @return true if base::String fit to the buffer.
		*/
	
		//static MemoryPool *stringPool;

		

	private:
		int m_h;


	};


} // base

BASE_API int UTF8toSQChar(uint8_t*  buf, int bufsize,  SQChar*  str ,int strsize);
BASE_API int SQChartoUTF8(uint8_t*  buf, int bufsize, const SQChar*  str ,int strsize);

/** 
 * Concatenate 0-terminated UTF-8 char sequence and String. 
 * @exception OutOfMemoryException
 */
inline base::String 
	operator+( const SQChar* first, const base::String& second )		{return base::String(first)+second;}

	inline base::String 
		operator+=( base::String& first, const SQChar* second ) 
	{
		base::String tem= first+second;

		first=tem;
		return first;

	}

	inline base::String 
		append( base::String& first, const SQChar* second ,int count=-1) 
	{
		base::String temp= first+base::String(second,count);
		first=temp;
		
		return temp;

	}
#endif // MING_BASE_STRING_H

// MinG
