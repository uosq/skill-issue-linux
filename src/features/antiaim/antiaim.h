#pragma once

#include "../../sdk/interfaces/interfaces.h"
#include "../../sdk/definitions/cusercmd.h"
#include "../../settings/settings.h"
#include "../../sdk/classes/player.h"
#include "../../sdk/classes/weaponbase.h"
#include "../../sdk/helpers/helper.h"
#include "../../sdk/definitions/cclientstate.h"

namespace Antiaim
{
	std::string GetPitchModeName(PitchMode mode);
	std::string GetYawModeName(YawMode mode);
	float GetPitch(CUserCmd* pCmd);
	float GetYaw(CUserCmd* pCmd, YawMode mode);
	void Run(CTFPlayer* pLocal, CTFWeaponBase* pWeapon, CUserCmd* pCmd);
}