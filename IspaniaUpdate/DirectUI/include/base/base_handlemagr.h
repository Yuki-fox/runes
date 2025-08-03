#ifndef MING_BASE_HANDLEMAGR_H
#define MING_BASE_HANDLEMAGR_H
#include <base\base_Array.h>
#include <vector>
#include <cassert>
namespace base
{


template <typename DATA, typename HANDLE>
class HandleMagr
{
private:
	struct Res 
	{
		DATA	   data;
		unsigned int   magic;
		int        ref;
		
	};
    // private types
    typedef base::Array <Res>         UserVec;
  //  typedef base::Array <unsigned int> MagicVec;
    typedef base::Array <unsigned int> FreeVec;

    // private data
    UserVec  m_UserData;     // data we're going to get to
    //MagicVec m_MagicNumbers; // corresponding magic numbers
    FreeVec  m_FreeSlots;    // keeps track of free slots in the db

public:

// Lifetime.

    HandleMagr( void )  {  }
   ~HandleMagr( void )  {  }

// Handle methods.

    // acquisition
    DATA* Acquire( HANDLE& handle );
	void AddRef(HANDLE& handle);
	void Delete(HANDLE& handle);
    DATA* Release( HANDLE&  handle );

	void Clear(void);
    // dereferencing
    DATA*       Dereference( HANDLE& handle );
    const DATA* Dereference( HANDLE& handle ) const;

    // other query
    unsigned int GetUsedHandleCount( void ) const
        {  return ( m_UserData.size() - m_FreeSlots.size() );  }
    bool HasUsedHandles( void ) const
        {  return ( !!GetUsedHandleCount() );  }
};
template <typename DATA, typename HANDLE>
void HandleMagr <DATA, HANDLE> ::Delete(HANDLE& handle)
{
	// which one?
	unsigned int index = handle.GetIndex();

	// make sure it's valid
	assert( index < (unsigned int)m_UserData.size() );

	Res* pRes=&m_UserData[ index ];

	assert( pRes->magic == handle.GetMagic() );
	assert( pRes->ref>0 );
	pRes->ref=0;
	pRes->magic=0;

	m_FreeSlots.add( index );


}
template <typename DATA, typename HANDLE>
void HandleMagr <DATA, HANDLE> ::Clear(void)
{

	m_UserData.clear();
	m_FreeSlots.clear();

}

template <typename DATA, typename HANDLE>
void HandleMagr <DATA, HANDLE> ::AddRef(HANDLE& handle)
{
	// which one?
	unsigned int index = handle.GetIndex();

	// make sure it's valid
	assert( index < (unsigned int)m_UserData.size() );

	Res* pRes=&m_UserData[ index ];

	assert( pRes->magic == handle.GetMagic() );
	assert( pRes->ref>0 );
	pRes->ref++;


}
template <typename DATA, typename HANDLE>
DATA* HandleMagr <DATA, HANDLE> :: Acquire( HANDLE& handle )
{
    // if free list is empty, add a new one otherwise use first one found

    unsigned int index;
	DATA * ret=0;
    if (! m_FreeSlots.size() )
    {
        index = m_UserData.size();
        handle.Init( index );
        m_UserData.add( Res() );
		Res* pRes=&m_UserData[ index ];
		pRes->magic=handle.GetMagic();
		pRes->ref=1; 
		
		ret=&pRes->data;
        //m_MagicNumbers.add( handle.GetMagic() );
    }
    else
    {
        index = m_FreeSlots.last();
        handle.Init( index );
        m_FreeSlots.remove(m_FreeSlots.size()-1);
		Res* pRes=&m_UserData[ index ];
        pRes->magic = handle.GetMagic();
		pRes->ref=1; 
		ret=&pRes->data;

    }
    return ( ret);
}

template <typename DATA, typename HANDLE>
DATA* HandleMagr <DATA, HANDLE> :: Release( HANDLE& handle )
{
    // which one?
    unsigned int index = handle.GetIndex();

    // make sure it's valid
    assert( index < (unsigned int)m_UserData.size() );

	Res* pRes=&m_UserData[ index ];

    assert( pRes->magic == handle.GetMagic() );
	assert( pRes->ref>0 );

	DATA* ret=0;
	
	pRes->ref--;
	if(pRes->ref==0)
	{
		// ok remove it - tag as unused and add to free list
		m_FreeSlots.add( index );
		ret=&pRes->data;
		pRes->magic=0;
	}  

	return ret;
  
}

template <typename DATA, typename HANDLE>
inline DATA* HandleMagr <DATA, HANDLE>
:: Dereference( HANDLE& handle )
{
    if ( handle.IsNull() )  return ( 0 );

    // check handle validity - $ this check can be removed for speed
    // if you can assume all handle references are always valid.
    unsigned int index = handle.GetIndex();
	Res* pRes=&m_UserData[ index ];
    if (   ( index >= (unsigned int)m_UserData.size() )
        || ( pRes->magic != handle.GetMagic() ) )
    {
        // no good! invalid handle == client programming error
        assert( 0 );
        return ( 0 );
    }

    return &pRes->data ;
}

template <typename DATA, typename HANDLE>
inline const DATA* HandleMagr <DATA, HANDLE>
:: Dereference( HANDLE& handle ) const
{
    // this lazy cast is ok - non-const version does not modify anything
    typedef HandleMgr <DATA, HANDLE> ThisType;
    return ( const_cast <ThisType*> ( this )->Dereference( handle ) );
}
}
#endif//MING_BASE_HANDLEMAGR_H
