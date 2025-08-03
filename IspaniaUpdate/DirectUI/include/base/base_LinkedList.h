#ifndef MING_BASE_LINKEDLIST_H
#define MING_BASE_LINKEDLIST_H

#include <base\base_base.h>
namespace base
{


class BASE_API LinkedListItemBase
{
public:
	/**
	 * Returns true if the item is linked to some list.
	 */
	bool	linked() const		{return 0 != m_next || 0 != m_previous;}
	void linkTo( LinkedListItemBase* prev )
	{
		if(prev)
			prev->m_next=this;
		m_previous=prev;
	}

	LinkedListItemBase* m_next; 
	LinkedListItemBase* m_previous;
protected:
	LinkedListItemBase() : m_next(0), m_previous(0) {}
	
	virtual ~LinkedListItemBase();

	friend class LinkedListBase;
	

private:
	LinkedListItemBase( const LinkedListItemBase& );
	LinkedListItemBase& operator=( const LinkedListItemBase& );
};

class BASE_API LinkedListBase
{
public:
	LinkedListBase();
	virtual ~LinkedListBase();
	void push_back(LinkedListItemBase* item);
	void push_front(LinkedListItemBase* item);
	
	
	void	insert( LinkedListItemBase* item );
	void	remove( LinkedListItemBase* item );

protected:
	LinkedListItemBase*		m_first;
	LinkedListItemBase*		m_last;
	int m_size;

private:
	LinkedListBase( const LinkedListBase& );
	LinkedListBase& operator=( const LinkedListBase& );
};

/**
 * Double linked list item.
 * @ingroup base
 */
template <class T> class LinkedListItem :
	public LinkedListItemBase
{
public:
	/**
	 * Returns the next element in the list if any.
	 */
	T*		next() const			{return static_cast<T*>( m_next );}

	/**
	 * Returns the previous element in the list if any.
	 */
	T*		previous() const		{return static_cast<T*>( m_previous );}

	
};

/**
 * Double linked list.
 * @ingroup base
 */
template <class T> class LinkedList :
	public LinkedListBase
{
public:
	/** 
	 * Inserts element to the start of the list.
	 */
	void	insert( T* item )		{LinkedListBase::insert( item );}

	

	void finish() {m_first=0;}
	/** 
	 * Removes element from the list.
	 */
	void	remove( T* item )		{LinkedListBase::remove( item );}

	/**
	 * Returns the first element in the list.
	 */
	T*		first() const			{return static_cast<T*>( m_first );}
	
	/**
	 * Returns the last element in the list.
	 */
	T*		last() const			{return static_cast<T*>( m_last);}
	int    size() const {return m_size;}
};


} // base


#endif // MING_BASE_LINKEDLIST_H

// MinG
