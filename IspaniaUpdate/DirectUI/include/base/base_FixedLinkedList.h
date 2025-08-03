
#ifndef BASE_FIXEDLINKEDLIST_H
#define BASE_FIXEDLINKEDLIST_H

#ifdef _WIN32
#pragma once
#endif
//#include <base\base_base.h>

#include <base\base_Array.h>
namespace base
{

// This is a useful macro to iterate from head to tail in a linked list.
#undef FOR_EACH_LL
#define FOR_EACH_LL( listName, iteratorName ) \
	for( int iteratorName=listName.head(); iteratorName != listName.invalidIndex(); iteratorName = listName.next( iteratorName ) )


//-----------------------------------------------------------------------------
// class FixedLinkedList:
//-----------------------------------------------------------------------------
	

template <typename T,typename I=int> 
class FixedLinkedList
{
public:
	typedef T ElemType_t;
	
	
	// constructor, destructor
	FixedLinkedList( );
	~FixedLinkedList( );

	// gets particular elements
	T&         element( I i );
	T const&   element( I i ) const;
	T&         operator[]( I i );
	T const&   operator[]( I i ) const;

	// Make sure we have a particular amount of memory
	//void ensureCapacity( int num );

	// Memory deallocation
	void purge();

	// Delete all the elements then call purge.
	void purgeAndDeleteElements();
	
	// Insertion methods....
	I	insertBefore( I before );
	I	insertAfter( I after );
	I	addToHead( ); 
	I	addToTail( );

	I	insertBefore( I before, T const& src );
	I	insertAfter( I after, T const& src );
	I	addToHead( T const& src ); 
	I	addToTail( T const& src );

	// Find an element and return its index or invalidIndex() if it couldn't be found.
	I		find( const T &src ) const;
	
	// Look for the element. If it exists, remove it and return true. Otherwise, return false.
	bool	findAndRemove( const T &src );

	// Removal methods
	void	remove( I elem );
	void	removeAll();

	// Allocation/deallocation methods
	// If multilist == true, then list list may contain many
	// non-connected lists, and isInList and Head + Tail are meaningless...
	I		alloc( bool multilist = false );
	void	free( I elem );

	// list modification
	void	linkBefore( I before, I elem );
	void	linkAfter( I after, I elem );
	void	unlink( I elem );
	void	linkToHead( I elem );
	void	linkToTail( I elem );

	// invalid index
	inline static I  invalidIndex()  { return -1; }
	inline static size_t elementSize() { return sizeof(ListElem_t); }

	// list statistics
	int	count() const;

	// Traversing the list
	I  head() const;
	I  tail() const;
	I  previous( I i ) const;
	I  next( I i ) const;

	// Are nodes in the list or valid?
	bool  isValidIndex( I i ) const;
	bool  isInList( I i ) const;
   
protected:
	// What the linked list element looks like
	struct ListElem_t
	{
		T  m_Element;
		I  m_previous;
		I  m_next;

		ListElem_t(){}
		ListElem_t( const ListElem_t&oth ):
			m_Element(oth.m_Element),
			m_previous(oth.m_previous),
			m_next(oth.m_next)
		{
			

		}
	private:
		// No copy constructor for these...
		
	};
	
	// constructs the class
	I allocInternal( bool multilist = false );
	void constructList();
	
	// Gets at the list element....
	ListElem_t& internalElement( I i ) { return m_memory[i]; }
	ListElem_t const& internalElement( I i ) const { return m_memory[i]; }

	// copy constructors not allowed
	FixedLinkedList( const FixedLinkedList<T,I> &list ) { assert(0); }
	   
	base::Array<ListElem_t> m_memory;
	I	m_head;
	I	m_tail;
	I	m_firstFree;
	I	m_elementCount;		// The number actually in the list
	//I	m_totalElements;	// The total number of elements	allocated
};
   
   
//-----------------------------------------------------------------------------
// constructor, destructor
//-----------------------------------------------------------------------------
template <typename T,typename I>
FixedLinkedList<T,I>::FixedLinkedList(   ) 
	
{
	constructList();
}

template <typename T,typename I>
FixedLinkedList<T,I>::~FixedLinkedList( ) 
{
	removeAll();
}

template <typename T,typename I>
void FixedLinkedList<T,I>::constructList()
{
	m_head = invalidIndex(); 
	m_tail = invalidIndex();
	
	m_firstFree = -1;
	
	
	//invalidIndex();
	m_elementCount =  0;
}


//-----------------------------------------------------------------------------
// gets particular elements
//-----------------------------------------------------------------------------
template <typename T,typename I>
inline T& FixedLinkedList<T,I>::element( I i )
{
	return m_memory[i].m_Element; 
}

template <typename T,typename I>
inline T const& FixedLinkedList<T,I>::element( I i ) const
{
	return m_memory[i].m_Element; 
}

template <typename T,typename I>
inline T& FixedLinkedList<T,I>::operator[]( I i )        
{ 
	return m_memory[i].m_Element; 
}

template <typename T,typename I>
inline T const& FixedLinkedList<T,I>::operator[]( I i ) const 
{
	return m_memory[i].m_Element; 
}


//-----------------------------------------------------------------------------
// list statistics
//-----------------------------------------------------------------------------
template <typename T,typename I>
inline int FixedLinkedList<T,I>::count() const      
{ 
	return m_memory.size(); 
}


//-----------------------------------------------------------------------------
// Traversing the list
//-----------------------------------------------------------------------------
template <typename T,typename I>
inline I FixedLinkedList<T,I>::head() const  
{ 
	return m_head; 
}

template <typename T,typename I>
inline I FixedLinkedList<T,I>::tail() const  
{ 
	return m_tail; 
}

template <typename T,typename I>
inline I  FixedLinkedList<T,I>::previous( I i ) const  
{ 
	assert( isValidIndex(i) ); 
	return internalElement(i).m_previous; 
}

template <typename T,typename I>
inline I  FixedLinkedList<T,I>::next( I i ) const  
{ 
	assert( isValidIndex(i) ); 
	return internalElement(i).m_next; 
}


//-----------------------------------------------------------------------------
// Are nodes in the list or valid?
//-----------------------------------------------------------------------------
template <typename T,typename I>
inline bool FixedLinkedList<T,I>::isValidIndex( I i ) const  
{ 
	return (i != -1) && ((m_memory[i].m_previous != i) || (m_memory[i].m_next != i));
}

template <typename T,typename I>
inline bool FixedLinkedList<T,I>::isInList( I i ) const
{
	return (i != -1) && (i < count());
}


//-----------------------------------------------------------------------------
// Makes sure we have enough memory allocated to store a requested # of elements
//-----------------------------------------------------------------------------
//template< class T >
//void FixedLinkedList<T,I>::ensureCapacity( int num )
//{
//	m_memory.resize(num);
//}


//-----------------------------------------------------------------------------
// Deallocate memory
//-----------------------------------------------------------------------------
template <typename T,typename I>
void  FixedLinkedList<T,I>::purge()
{
	removeAll();
	//m_memory.purge( );
	m_firstFree =-1;// invalidIndex();
	//m_totalElements = 0;
}


template<typename T,typename I>
void FixedLinkedList<T,I>::purgeAndDeleteElements()
{
	int iNext;
	for( int i=head(); i != invalidIndex(); i=iNext )
	{
		iNext = next(i);
		delete element(i);
	}

	purge();
}


//-----------------------------------------------------------------------------
// Node allocation/deallocation
//-----------------------------------------------------------------------------
template <typename T,typename I>
I FixedLinkedList<T,I>::allocInternal( bool multilist )
{
	I elem;
	if (m_firstFree == -1)
	{
		elem=m_memory.size();
		ListElem_t tmp;
		// Nothing in the free list; add.
		// Since nothing is in the free list, m_totalElements == total # of elements
		// the list knows about.
		m_memory.add(tmp);		
	} 
	else
	{
		elem = m_firstFree;
		m_firstFree = internalElement(m_firstFree).m_next;
	}

	ListElem_t& etmp=m_memory[elem];
	
	if (!multilist)
	{
		etmp.m_next =elem;
		etmp.m_previous =elem;
	}
	else
	{
		etmp.m_next =-1;
		etmp.m_previous =-1;
	}

	return elem;
}

template <typename T,typename I>
I FixedLinkedList<T,I>::alloc( bool multilist )
{
	I elem = allocInternal( multilist );
	//construct( &element(elem) );

	return elem;
}

template <typename T,typename I>
void  FixedLinkedList<T,I>::free( I elem )
{
	assert( isValidIndex(elem) );
	unlink(elem);

	ListElem_t &internalElem = internalElement(elem);
	//destruct( &internalElem.m_Element );
	internalElem.m_next = m_firstFree;
	m_firstFree = elem;
}


//-----------------------------------------------------------------------------
// Insertion methods; allocates and links (uses default constructor)
//-----------------------------------------------------------------------------
template <typename T,typename I>
I FixedLinkedList<T,I>::insertBefore( I before )
{
	// Make a new node
	I   newNode = allocInternal();
	
	// Link it in
	linkBefore( before, newNode );
	
	// Construct the data
	//Construct( &element(newNode) );
	
	return newNode;
}

template <typename T,typename I>
I FixedLinkedList<T,I>::insertAfter( I after )
{
	// Make a new node
	I   newNode = allocInternal();
	
	// Link it in
	linkAfter( after, newNode );
	
	// Construct the data
	//Construct( &element(newNode) );
	
	return newNode;
}

template <typename T,typename I>
inline I FixedLinkedList<T,I>::addToHead( ) 
{ 
	return insertAfter( invalidIndex() ); 
}

template <typename T,typename I>
inline I FixedLinkedList<T,I>::addToTail( ) 
{ 
	return insertBefore( invalidIndex() ); 
}


//-----------------------------------------------------------------------------
// Insertion methods; allocates and links (uses copy constructor)
//-----------------------------------------------------------------------------
template <typename T,typename I>
I FixedLinkedList<T,I>::insertBefore( I before, T const& src )
{
	// Make a new node
	I   newNode = allocInternal();
	
	// Link it in
	linkBefore( before, newNode );
	
	// Construct the data
	
	element(newNode)=src;
	//copyConstruct( &element(newNode), src );
	
	return newNode;
}

template <typename T,typename I>
I FixedLinkedList<T,I>::insertAfter( I after, T const& src )
{
	// Make a new node
	I   newNode = allocInternal();
	
	// Link it in
	linkAfter( after, newNode );
	
	// Construct the data
	element(newNode)=src;
	//copyConstruct( &element(newNode), src );
	
	return newNode;
}

template <typename T,typename I>
inline I FixedLinkedList<T,I>::addToHead( T const& src ) 
{ 
	return insertAfter( invalidIndex(), src ); 
}

template <typename T,typename I>
inline I FixedLinkedList<T,I>::addToTail( T const& src ) 
{ 
	return insertBefore( invalidIndex(), src ); 
}


//-----------------------------------------------------------------------------
// Removal methods
//-----------------------------------------------------------------------------

template<class T, class I>
I FixedLinkedList<T,I>::find( const T &src ) const
{
	for ( I i=head(); i != invalidIndex(); i = next( i ) )
	{
		if ( element( i ) == src )
			return i;
	}
	return i;//nvalidIndex();
}


template<class T, class I>
bool FixedLinkedList<T,I>::findAndRemove( const T &src )
{
	I i = find( src );
	if ( i == invalidIndex() )
	{
		return false;
	}
	else
	{
		remove( i );
		return true;
	}
}


template <typename T,typename I>
void  FixedLinkedList<T,I>::remove( I elem )
{
	free( elem );
}

template <typename T,typename I>
void  FixedLinkedList<T,I>::removeAll()
{
	if (m_memory.size() == 0)
		return;

	// Have to do some convoluted stuff to invoke the destructor on all
	// valid elements for the multilist case (since we don't have all elements
	// connected to each other in a list).

	// Put everything into the free list	
	
	// This doesn't actually deallocate the memory... purge does that
	m_memory.clear();
	
	m_firstFree = -1;
	
	// Clear everything else out
	m_head = invalidIndex(); 
	m_tail = invalidIndex();
	m_elementCount = 0;
}


//-----------------------------------------------------------------------------
// list modification
//-----------------------------------------------------------------------------
template <typename T,typename I>
void  FixedLinkedList<T,I>::linkBefore( I before, I elem )
{
	assert( isValidIndex(elem) );
	
	// unlink it if it's in the list at the moment
	unlink(elem);
	
	ListElem_t& newElem = internalElement(elem);
	
	// The element *after* our newly linked one is the one we linked before.
	newElem.m_next = before;
	
	if (before == invalidIndex())
	{
		// In this case, we're linking to the end of the list, so reset the tail
		newElem.m_previous = m_tail;
		m_tail = elem;
	}
	else
	{
		// Here, we're not linking to the end. Set the prev pointer to point to
		// the element we're linking.
		assert( isInList(before) );
		ListElem_t& beforeElem = internalElement(before);
		newElem.m_previous = beforeElem.m_previous;
		beforeElem.m_previous = elem;
	}
	
	// Reset the head if we linked to the head of the list
	if (newElem.m_previous == invalidIndex())
		m_head = elem;
	else
		internalElement(newElem.m_previous).m_next = elem;
	
	// one more element baby
	++m_elementCount;
}

template <typename T,typename I>
void  FixedLinkedList<T,I>::linkAfter( I after, I elem )
{
	assert( isValidIndex(elem) );
	
	// unlink it if it's in the list at the moment
	if ( isInList(elem) )
		unlink(elem);
	
	ListElem_t& newElem = internalElement(elem);
	
	// The element *before* our newly linked one is the one we linked after
	newElem.m_previous = after;
	if (after == invalidIndex())
	{
		// In this case, we're linking to the head of the list, reset the head
		newElem.m_next = m_head;
		m_head = elem;
	}
	else
	{
		// Here, we're not linking to the end. Set the next pointer to point to
		// the element we're linking.
		assert( isInList(after) );
		ListElem_t& afterElem = internalElement(after);
		newElem.m_next = afterElem.m_next;
		afterElem.m_next = elem;
	}
	
	// Reset the tail if we linked to the tail of the list
	if (newElem.m_next == invalidIndex())
		m_tail = elem;
	else
		internalElement(newElem.m_next).m_previous = elem;
	
	// one more element baby
	++m_elementCount;
}

template <typename T,typename I>
void  FixedLinkedList<T,I>::unlink( I elem )
{
	assert( isValidIndex(elem) );
	if (isInList(elem))
	{
		ListElem_t *pOldElem = &m_memory[elem];
		
		// If we're the first guy, reset the head
		// otherwise, make our previous node's next pointer = our next
		if ( pOldElem->m_previous != 0 )
		{
			m_memory[pOldElem->m_previous].m_next = pOldElem->m_next;
		}
		else
		{
			m_head = pOldElem->m_next;
		}
		
		// If we're the last guy, reset the tail
		// otherwise, make our next node's prev pointer = our prev
		if ( pOldElem->m_next != 0 )
		{
			m_memory[pOldElem->m_next].m_previous = pOldElem->m_previous;
		}
		else
		{
			m_tail = pOldElem->m_previous;
		}
		
		// This marks this node as not in the list, 
		// but not in the free list either
		pOldElem->m_previous = pOldElem->m_next = elem;
		
		// One less puppy
		--m_elementCount;
	}
}

template <typename T,typename I>
inline void FixedLinkedList<T,I>::linkToHead( I elem ) 
{
	linkAfter( invalidIndex(), elem ); 
}

template <typename T,typename I>
inline void FixedLinkedList<T,I>::linkToTail( I elem ) 
{
	linkBefore( invalidIndex(), elem ); 
}

}   
#endif // BASE_FIXEDLINKEDLIST_H
