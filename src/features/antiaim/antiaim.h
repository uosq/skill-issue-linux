#pragma once

#include "../../sdk/classes/player.h"
#include "../../sdk/classes/weaponbase.h"
#include "../../sdk/definitions/cclientstate.h"
#include "../../sdk/definitions/cusercmd.h"
#include "../../sdk/helpers/helper.h"
#include "../../sdk/interfaces/interfaces.h"
#include "../../settings/settings.h"

namespace Antiaim
{
	std::string GetPitchModeName(PitchMode mode);
	std::string GetYawModeName(YawMode mode);
	float GetPitch(CUserCmd *pCmd);
	float GetYaw(CUserCmd *pCmd, YawMode mode);
	void Run(CTFPlayer *pLocal, CTFWeaponBase *pWeapon, CUserCmd *pCmd);
} // namespace Antiaim