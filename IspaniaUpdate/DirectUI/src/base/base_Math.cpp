
#include "base_Pcheader.h"



namespace base
{




	
float Math::random()
{
	const float RAND_SCALE = 1.f / ( float(RAND_MAX) + 1.f );
	return RAND_SCALE * float(rand());
}




} // base

// MinG
