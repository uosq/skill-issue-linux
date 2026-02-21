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

namespace ESP
{
	extern std::vector<std::unique_ptr<IBaseElement>> m_builtinElements;

	Color GetEntityColor(CBaseEntity* entity);

	bool IsValidPlayer(CTFPlayer* pLocal, CBaseEntity* entity);
	bool IsValidBuilding(CTFPlayer* pLocal, CBaseObject* entity);
	bool IsValidEntity(CBaseEntity* entity);

	bool GetEntityBounds(CBaseEntity* entity, Vector& top, Vector& bottom, int& w, int& h);

	void PaintBox(Color color, const Vector& top, const Vector& bottom, int w, int h);

	bool GetData(const EntityListEntry& entry, ESP_Data& out);

	void Init();
	void Run(CTFPlayer* pLocal);
};