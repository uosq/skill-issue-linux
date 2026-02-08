#pragma once

#include "../../sdk/interfaces/interfaces.h"
#include "../../sdk/helpers/helper.h"
#include "../../sdk/classes/weaponbase.h"
#include "../../sdk/classes/cbaseobject.h"
#include "../../sdk/definitions/eteam.h"
#include "../entitylist/entitylist.h"
#include "../../settings.h"

namespace ESP
{
	Color GetEntityColor(CBaseEntity* entity);
	bool IsValidPlayer(CTFPlayer* pLocal, CBaseEntity* entity);
	bool IsValidBuilding(CTFPlayer* pLocal, CBaseObject* entity);
	void PaintBox(Color color, Vector top, Vector bottom, int w, int h);
	void PaintName(Color color, Vector top, int w, int h, std::string name);
	void Init();
	void Run(CTFPlayer* pLocal);
};