#pragma once

#include "../../sdk/interfaces/interfaces.h"
#include "../../sdk/helpers/helper.h"
#include "../../sdk/classes/weaponbase.h"
#include "../../sdk/classes/cbaseobject.h"
#include "../../sdk/definitions/eteam.h"

#include "../entitylist/entitylist.h"
#include "../../settings/settings.h"

#include "elements/BaseElement.h"
#include "structs.h"

namespace ESP_Utils
{
	Color GetEntityColor(CBaseEntity* entity);

	bool IsValidPlayer(CTFPlayer* pLocal, CBaseEntity* entity);
	bool IsValidBuilding(CTFPlayer* pLocal, CBaseObject* entity);
	bool IsValidEntity(CTFPlayer* pLocal, const EntityListEntry& entry);

	bool GetEntityBounds(CBaseEntity* entity, ESP_Data& out);
}