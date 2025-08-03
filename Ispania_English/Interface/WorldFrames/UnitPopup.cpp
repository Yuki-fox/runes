#include "UnitPopup.h"

CUnitPopup*					g_pUnitPopup = NULL;

// ----------------------------------------------------------------------------
CUnitPopup::CUnitPopup(CInterface* object)
	: CInterfaceBase(object)
{
	g_pUnitPopup = this;
}

// ----------------------------------------------------------------------------
CUnitPopup::~CUnitPopup()
{
	g_pUnitPopup = NULL;
}

// ----------------------------------------------------------------------------
void CUnitPopup::RegisterFunc()
{

}