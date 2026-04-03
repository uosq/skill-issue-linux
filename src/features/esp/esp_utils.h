#pragma once

#include "../../sdk/classes/cbaseobject.h"
#include "../../sdk/classes/weaponbase.h"
#include "../../sdk/definitions/eteam.h"
#include "../../sdk/helpers/helper.h"
#include "../../sdk/interfaces/interfaces.h"

#include "../../settings/settings.h"
#include "../entitylist/entitylist.h"

#include "elements/BaseElement.h"
#include "structs.h"

namespace ESP_Utils
{
	Color GetEntityColor(CBaseEntity *entity);

	bool IsValidPlayer(CTFPlayer *pLocal, CBaseEntity *entity);
	bool IsValidBuilding(CTFPlayer *pLocal, CBaseObject *entity);
	bool IsValidEntity(CTFPlayer *pLocal, const EntityListEntry &entry);

	bool GetEntityBounds(CBaseEntity *entity, ESP_Data &out);
} // namespace ESP_Utils