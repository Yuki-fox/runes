#ifndef MING_BASE_MEMORYPOOL_H
#define MING_BASE_MEMORYPOOL_H

#include <base/base_Base.h>

#include <base/base_RefCount.h>
#include <base/base_assert.h>

#include <base/base_Array.h>
//#include <base/base_RTTI.h>
namespace base
{


/**
 * Handle based memory pool with compact support.
 * Note: Not thread safe.
 * @ingroup base
 */
class  BASE_API MemoryPool :	public RefCount
{
	// DECLARE_RTTI;
public:
	/** 
	 * Creates pool with specified initial size and maximum size. 
	 * @param size Initial size.
	 * @param maxsize Maximum pool size. If 0 then the pool can grow without limits.
	 * @param name Name of the pool. Used for debugging.
	 */
	MemoryPool( int size, int maxsize, const SQChar* name );

	///
	virtual ~MemoryPool();

	/** 
	 * Allocates a memory block from the pool. 
	 * Returned handle has reference count of 1.
	 * @param bytes Number of bytes to allocate.
	 * @return Handle to the block.
	 * @exception OutOfMemoryException
	 */
	int				allocate( int bytes );

	/** 
	 * Adds reference to memory block handle. 
	 */
	void			ref( int handle )				{(reinterpret_cast<BlockHeader*>(m_blocks[handle])-1)->refs += 1; 
	//if (handle==0)
	//	handle=0;
	}

	/** 
	 * Releases reference to memory block handle. 
	 */
	int			unref( int handle )				{ return (reinterpret_cast<BlockHeader*>(m_blocks[handle])-1)->refs -= 1; }

	/** 
	 * Returns memory associated with handle. 
	 * Does not alter memory block handle reference count.
	 */
	char*			get( int handle ) 				{return m_blocks[handle];}
	
	/** 
	 * Compacts used memory pool.
	 */
	void			compact();

	/** 
	 * Returns number of allocated blocks. 
	 */
	int				blocksAllocated() const;

	/** 
	 * Returns number of allocated bytes (including gaps). 
	 */
	int				bytesAllocated() const			{return m_next;}

	/** 
	 * Returns size of the memory pool. 
	 */
	int				size() const					{return m_mem.size();}

	/** 
	 * Dump all contents of memory pool to debug output. 
	 * Warning: very large overhead. 
	 */
	void			dump() const;
void			unalloc( int handle );
private:
	friend class Handle;

	struct BlockHeader
	{
		int refs;
		int size;
	};

	class HandleSorter
	{
	public:
		HandleSorter( MemoryPool* mp )				: m_mp(mp) {}
		bool operator()( int a, int b ) const		{return m_mp->get(a) < m_mp->get(b);}
			
	private:
		MemoryPool* m_mp;
	};
	
	Array<char>		m_mem;
	Array<char*>	m_blocks;
	Array<int>		m_freed;
	int				m_maxSize;
	int				m_next;
	Array<int>		m_gcbuf;
	SQChar			m_name[32]; // DEBUG
	int				m_maxgctime; // DEBUG

	
	void 			resize( int newsize );
	
	MemoryPool( const MemoryPool& );
	MemoryPool& operator=( const MemoryPool& );
};


} // base


#endif // MING_BASE_MEMORYPOOL_H

// MinG
