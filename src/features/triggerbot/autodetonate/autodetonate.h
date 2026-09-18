#pragma once

#include "../../feature.h"
#include "../../config/config.h"

class CUserCmd;
class CTFPlayer;

BEGIN_CONFIG(autodetonate)
	ADD_CONFIG(enabled, "Enables auto detonate", false)
END_CONFIG()

// i don't like OOP, but i can't rewrite the entire project now

struct AutoDetonate
{
	void Run(CUserCmd* cmd, CTFPlayer* local);
};

DECLARE_FEATURE(AutoDetonate, autodetonate)