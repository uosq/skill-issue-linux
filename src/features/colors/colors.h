#pragma once

#include "../config/config.h"

#include "../../sdk/definitions/color.h"

BEGIN_CONFIG(colors)

	ADD_CONFIG(red_team, "red_team", (Color{255, 0, 255, 255}))
	ADD_CONFIG(blu_team, "blu_team", (Color{0, 255, 255, 255}))
	ADD_CONFIG(aimbot_target, "aimbot_target", (Color{255, 255, 255, 255}))
	ADD_CONFIG(weapon, "weapon", (Color{255, 255, 255, 255}))
	ADD_CONFIG(menu_accent, "menu_accent", (Color{0, 150, 255, 255}))
	ADD_CONFIG(healthkit, "healthkit", (Color{255, 1000, 100, 255}))
	ADD_CONFIG(ammopack, "ammopack", (Color{255, 255, 255, 255}))
	ADD_CONFIG(backtrack, "backtrack", (Color{255, 255, 255, 255}))

END_CONFIG()