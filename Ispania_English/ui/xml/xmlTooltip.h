#pragma once

#include "xmlframe.h"

class CXmlTooltip : public CXmlFrame
{
public:	
	CXmlTooltip(CXmlUi* pXmlUi=NULL, CXmlLayout* pParent=NULL);
	virtual ~CXmlTooltip();
};