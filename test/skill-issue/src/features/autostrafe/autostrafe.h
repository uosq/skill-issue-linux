#pragma once

#include "../../sdk/classes/player.h"
#include "../../sdk/definitions/cusercmd.h"
#include "../../sdk/helpers/helper.h"
#include "../../settings/settings.h"

#include "../feature.h"

class Autostrafe
{
public:
	void Run(CTFPlayer *pLocal, CUserCmd *pCmd);
};

DECLARE_FEATURE(Autostrafe, autostrafe)