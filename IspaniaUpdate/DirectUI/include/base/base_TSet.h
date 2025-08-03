
#ifndef MING_BASE_TSET_H_
#define MING_BASE_TSET_H_
#include <base\base_Array.h>
//----------------------------------------------- INCLUDES -----------------------------------------------------------//


//----------------------------------------------- CLASSES ------------------------------------------------------------//

//--------------------------------------------------------------------------------------------------------------------//
//                                                TSet                                                                //
//--------------------------------------------------------------------------------------------------------------------//

namespace base
{
  template <typename CObj> class TSet
  {
  // typedefs

  public:

    typedef Array< CObj >            setType;
  

  // public methods

  public:
                    TSet                (void);
                   ~TSet                (void);

    void            clearSet            (void);
    void            addToSet            (const CObj& Obj);
    bool            removeFromSet       (const CObj& Obj);
    bool            isInSet             (const CObj& Obj);

  // protected data

  protected:

    setType         m_Set;
  };
}

//----------------------------------------------- INLINES ------------------------------------------------------------//

#include  <base\base_TSet.inl>

//--------------------------------------------------------------------------------------------------------------------//

#endif // MING_BASE_TSET_H_
