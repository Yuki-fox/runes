#ifndef MING_BASE_IFACTORY_H
#define MING_BASE_IFACTORY_H

#include <base\base_singleton.h>

namespace base
{
	class String;
}


#define INTERFACE_IFactory(T) \
	public:\
	virtual	void initial(const base::String& dataPath)##T\
	virtual	void finish(void)##T

#define BASE_IFactory INTERFACE_IFactory(=0;)
#define DERIVD_IFactory INTERFACE_IFactory(;)

namespace base
{

	class IFactory 
	{
	public:

		BASE_IFactory;
	};

}//base
#endif//MING_BASE_IFACTORY_H
