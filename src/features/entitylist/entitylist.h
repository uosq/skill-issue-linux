#pragma once

#include "../../sdk/classes/cbaseobject.h"
#include "../../sdk/classes/projectiles.h"
#include "../../sdk/helpers/helper.h"
#include "structs.h"
#include <vector>

namespace EntityList
{
	void Reserve(); // Call in LevelInitPostEntity
	void Clear();
	void Store(); // Call in FrameStageNotify -> FRAME_NET_UPDATE_END

	const std::vector<EntityListEntry>& GetEntities();
	const std::vector<StaticEntity>& GetStaticEntities();

	CTFPlayer *GetLocal();
	CTFPlayerResource *GetPlayerResources();
	std::vector<EntityListEntry> GetTeammates();
	std::vector<EntityListEntry> GetEnemies();

	CBaseEntity* GetAimbotTarget();
	void SetAimbotTarget(CBaseEntity* pEntity = nullptr);
} // namespace EntityList