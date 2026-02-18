#pragma once

#include "../../sdk/interfaces/interfaces.h"
#include "../../sdk/helpers/helper.h"
#include "../../sdk/classes/weaponbase.h"
#include "../../sdk/classes/cbaseobject.h"
#include "../../sdk/definitions/eteam.h"
#include "../entitylist/entitylist.h"
#include "../../settings/settings.h"

namespace ESP
{
	Color GetEntityColor(CBaseEntity* entity);

	bool IsValidPlayer(CTFPlayer* pLocal, CBaseEntity* entity);
	bool IsValidBuilding(CTFPlayer* pLocal, CBaseObject* entity);
	bool IsValidEntity(CBaseEntity* entity);

	bool GetEntityBounds(CBaseEntity* entity, Vector& top, Vector& bottom, int& w, int& h);

	void PaintBox(Color color, const Vector& top, const Vector& bottom, int w, int h);
	void PaintName(Color color, const Vector& top, int w, int h, const std::string& name);
	void PaintHealthbar(const Vector& top, const Vector& bottom, int w, int h, int health, int maxhealth, int buffhealth = 0);

	void Init();
	void Run(CTFPlayer* pLocal);
};