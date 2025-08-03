

#ifndef MING_BASE_TAUTOLISTS_H_
#define MING_BASE_TAUTOLISTS_H_

#include <base\base_Array.h>
namespace base
{

template <class T>
class TAutolists
{
public:
    TAutolists()
    { 
        ms_List.add(static_cast<T>(this));
    }

    virtual ~TAutolists()
    { 
		int index=ms_List.indexOf(static_cast<T>(this));
		ms_List.remove(index);
    }

public:
   
        
private:

    // The actual list and list pointer
	static Array<T>            ms_List;
    

};

// Declarations of static variables
template<class T> Array<T>    TAutolists<T>::ms_List;


}
#endif//MING_BASE_TAUTOLISTS_H_



