#include "base_pcheader.h"
#include <time.h>


//-----------------------------------------------------------------------------
#ifdef _DEBUG

#endif
namespace base
{


RandomX::RandomX()
{
	time_t t;
	time(&t);
	setSeed( (long)t );
}

RandomX::RandomX( long seed )
{
	setSeed( seed );
}

void RandomX::setSeed( long seed )
{
	m_seed = seed ^ 0x5EECE66D;
}

int RandomX::nextInt()
{
	return next(32);
}

long RandomX::nextLong()
{
	return next(32);
}

float RandomX::nextFloat()
{
	int v = next(32);
	v &= 0x7FFFFFF;
	return float(v) / float(0x8000000);
}

double RandomX::nextDouble()
{
	int v = next(32);
	v &= 0x7FFFFFF;
	return double(v) / double(0x8000000);
}

bool RandomX::nextBoolean()
{
	return 0 != (next(32) & 256);
}

int	RandomX::next( int /*bits*/ )
{
	// Linear congruential generator (LCG):
	// I(k) = ( a * I(k-1) + c ) % m,

	// Minimal standard RNG (Park and Miller 1988):
	const int a = 16807;
	const int c = 0;
	const int m = 2147483647;

	// sample 2 times, use only bits 8-16 from second sample
	m_seed = ( a * m_seed + c ) % m;
	int v = m_seed;
	m_seed = ( a * m_seed + c ) % m;
	v += (m_seed >> 8) & 0xFF;

#ifdef _DEBUG

//	base::Debug::printf( "Random.next: %X", v );
#endif 
	return v;
}


} // base
