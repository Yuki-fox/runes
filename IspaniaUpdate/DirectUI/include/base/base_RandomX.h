#ifndef MING_BASE_RANDOM_H
#define MING_BASE_RANDOM_H

#include <base/base_base.h>
#include <base/base_RefCount.h>


namespace base
{


/** 
 * Linear congruential pseudo-random number generator.
 * Not synchronized.
 *
 * Formula:
 * I(k) = ( a * I(k-1) + c ) % m,
 *
 * Minimal standard RNG (Park and Miller 1988):
 * <ul>
 * <li>a = 16807
 * <li>c = 0
 * <li>m = 2147483647
 * </ul>
 *
 * 
 */
class BASE_API RandomX :	public RefCount
{
public:
	/** Seeds RNG from system clock. */
	RandomX();

	/** Seeds RNG with explicit seed value. */
	explicit RandomX( long seed );

	/** Sets random number generator seed. */
	void	setSeed( long seed );

	/** Returns random int. */
	int		nextInt();

	/** Returns random long. */
	long	nextLong();
	
	/** Returns random float in range [0,1). */
	float	nextFloat();

	/** Returns random float in range [0,1). */
	double	nextDouble();

	/** Returns random bool. */
	bool	nextBoolean();

private:
	long m_seed;

	virtual int	next( int bits );
};


} // base


#endif // MING_BASE_RANDOM_H
