#pragma once

#include "../../sdk/classes/player.h"
#include "../../sdk/definitions/cusercmd.h"
#include "../../sdk/helpers/helper.h"

#include "../feature.h"
#include "../config/config.h"

BEGIN_CONFIG(autostrafe)
	ADD_CONFIG(enabled, "autostrafe enabled", false)
END_CONFIG()

class Autostrafe
{
public:
	void Run(CTFPlayer *pLocal, CUserCmd *pCmd);
};

DECLARE_FEATURE(Autostrafe, autostrafe)