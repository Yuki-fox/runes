#pragma once

#include <string>
#include <vector>
#include "../interface.h"


class CCombatMeter;
extern CCombatMeter* g_pCombatMeter;







class CCombatMeter : public CInterfaceBase
{
public:
	CCombatMeter(CInterface* object);
	virtual ~CCombatMeter();

	virtual	void RegisterFunc();
  
	void AddDamage( const char* source, const char* target, const char* skill, const char* type, int damage );
	void AddHeal( const char* source, const char* target, const char* skill, const char* type, int heal );

public:

protected:

protected:


};











