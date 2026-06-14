#pragma once

#include "../../sdk/classes/cbaseobject.h"
#include "../../sdk/classes/weaponbase.h"
#include "../../sdk/definitions/eteam.h"
#include "../../sdk/helpers/helper.h"
#include "../../sdk/interfaces/interfaces.h"

#include "../config/config.h"
#include "../entitylist/entitylist.h"

enum class ESPTeamSelectionMode
{
	INVALID = -1,
	ENEMIES,
	TEAMMATES,
	BOTH,
	MAX,
};

enum class HealthMode
{
	INVALID = -1,
	NONE,
	TEXT,
	BAR,
	BOTH,
	MAX
};

enum class ESPConditionFlags
{
	Zoomed	= 1 << 0,
	Bonked	= 1 << 1,
	Ubered	= 1 << 2,
	Jarated = 1 << 3,
};

enum class ESPFont
{
	INVALID = -1,
	TF2BUILD,
	ARIAL,
	COUNT,
};

namespace ESP_Utils
{
	Color GetEntityColor(CBaseEntity *entity);

	bool IsValidPlayer(CTFPlayer *pLocal, CBaseEntity *entity);
	bool IsValidBuilding(CTFPlayer *pLocal, CBaseObject *entity);
	bool IsValidEntity(CTFPlayer *pLocal, const EntityListEntry &entry);
} // namespace ESP_Utils