#pragma once

#include "../../sdk/classes/entity.h"
#include <cstdint>

enum EntityFlags
{
	IsPlayer     = 1,
	IsBuilding   = 1 << 1,
	IsEnemy	     = 1 << 2,
	IsProjectile = 1 << 3,
	IsAlive	     = 1 << 4
};

struct EntityListEntry
{
	CBaseEntity *ptr;
	uint8_t flags;

	EntityListEntry()
	{
		flags = 0;
		ptr   = nullptr;
	}
};