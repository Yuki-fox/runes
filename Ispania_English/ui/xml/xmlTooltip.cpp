#include "xmltooltip.h"

// ----------------------------------------------------------------------------
CXmlTooltip::CXmlTooltip(CXmlUi* pXmlUi, CXmlLayout* pParent)
	: CXmlFrame(pXmlUi, pParent)
{
	m_type				= eXmlUiType_Tooltip;
}

// ----------------------------------------------------------------------------
CXmlTooltip::~CXmlTooltip()
{
}
