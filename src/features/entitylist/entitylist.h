#pragma once

#include <vector>
#include "../../sdk/helpers/helper.h"
#include "../../sdk/classes/cbaseobject.h"
#include "../../sdk/classes/projectiles.h"

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

namespace EntityList
{
	extern std::vector<EntityListEntry> m_vecEntities;
	extern CTFPlayer* m_pLocalPlayer;
	extern CTFPlayerResource *m_pPlayerResource;
	extern CBaseEntity* m_pAimbotTarget;

	void Reserve();
	void Clear();
	void Store(); // Call in FrameStageNotify -> FRAME_NET_UPDATE_END
	const std::vector<EntityListEntry>& GetEntities();
	CTFPlayer* GetLocal();
	CTFPlayerResource* GetPlayerResources();
	std::vector<EntityListEntry> GetTeammates();
	std::vector<EntityListEntry> GetEnemies();
}