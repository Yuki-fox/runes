
#ifndef MING_BASE_TSET_INL_
#define MING_BASE_TSET_INL_

namespace base
{

//--------------------------------------------------------------------------------------------------------------------//
//                                                CONSTRUCTORS / DESTRUCTOR                                           //
//--------------------------------------------------------------------------------------------------------------------//

template <typename CObj> inline TSet<CObj>::TSet()
  {}

template <typename CObj> inline TSet<CObj>::~TSet()
  {}

//--------------------------------------------------------------------------------------------------------------------//
//                                                OPERATIONS                                                          //
//--------------------------------------------------------------------------------------------------------------------//

template <typename CObj> inline void TSet<CObj>::clearSet()
  {
  m_Set.clear();
  }

template <typename CObj> inline void TSet<CObj>::addToSet(const CObj& Obj)
  {
  m_Set.add(Obj);
  }

template <typename CObj> inline bool TSet<CObj>::removeFromSet(const CObj& Obj)
  {
	  int i=  m_Set.indexOf(Obj);
	  if (i!=-1)
		  m_Set.remove(i) ;
  return( i!=-1);
  }

template <typename CObj> inline bool TSet<CObj>::isInSet(const CObj& Obj)
  {
  return(m_Set.indexOf(Obj)!=-1);
  }

//--------------------------------------------------------------------------------------------------------------------//

} // namespace

#endif // _GPG4_TSET_INL_
