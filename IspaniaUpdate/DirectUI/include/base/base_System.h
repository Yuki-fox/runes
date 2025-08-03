#ifndef MING_BASE_SYSTEM_H
#define MING_BASE_SYSTEM_H


namespace base
{


/** 
 * System class provides running environment related information.
 * 
 * @ingroup base
 */
class BASE_API System
{
public:


	static void initial(void);
	static void finish(void);


	
	/** 
	 * Returns current system time in milliseconds. 
	 */
	static int currentTimeMillis();
	


private:
	System();
	System( const System& );
	System& operator=( const System& );
};


} // base


#endif // MING_BASE_SYSTEM_H

// MinG
