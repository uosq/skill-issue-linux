#pragma once

#include <cstdint>
#include "../../sdk/classes/entity.h"

enum EntityFlags: uint8_t
{
	IsPlayer = 1,
	IsBuilding = 2,
	IsEnemy = 4,
	IsProjectile = 8,
	IsAlive = 16
};

struct EntityListEntry
{
	CBaseEntity* ptr;
	uint8_t flags;

	EntityListEntry()
	{
		flags = 0;
		ptr = nullptr;
	}
};