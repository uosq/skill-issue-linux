#pragma once

#include "basecomponent.h"

struct IPaintableComponent: public BaseComponent
{
	virtual void Paint() = 0;
};