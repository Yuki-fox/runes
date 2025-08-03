#include "base_pcheader.h"




namespace base
{


LinkedListItemBase::~LinkedListItemBase()
{
	assert( !m_previous && !m_next && "Item still linked in its dtor" );
}

LinkedListBase::LinkedListBase() :
	m_first(0),m_last(0),m_size(0)
{
}

LinkedListBase::~LinkedListBase()
{
	assert( !m_first && "Still items in linked list dtor" );
}

void LinkedListBase::insert( LinkedListItemBase* item )
{
	assert( !item->linked() );

	item->m_next = m_first;
	if ( m_first != 0 )
		m_first->m_previous = item;
	m_first = item;
	if (!m_last)
		m_last=item;

	m_size++;

}
void LinkedListBase::push_front( LinkedListItemBase* item )
{
	if (m_first)
	{
		m_first->linkTo(item);
		m_first=item;

	}
	else
		insert(item);

}
void LinkedListBase::push_back( LinkedListItemBase* item )
{
	if (m_last)
	{
		item->linkTo(m_last);
		m_last=item;

	}
	else
		insert(item);
	
}


void LinkedListBase::remove( LinkedListItemBase* item )
{
	assert( 0 != item );

	LinkedListItemBase *pPren=item->m_previous;
	LinkedListItemBase *pNext=item->m_next;
	item->m_next = item->m_previous = 0;

	if (pPren)
		pPren->m_next=pNext;
	else 
		m_first=pNext;

	if (pNext)
		pNext->m_previous=pPren;
	else 
		m_last=pPren;	
	
	m_size--;
}




} // base

// MinG
