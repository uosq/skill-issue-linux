#pragma once

#include "../../sdk/classes/cbaseobject.h"
#include "../../sdk/classes/projectiles.h"
#include "../../sdk/helpers/helper.h"
#include "structs.h"
#include <vector>

namespace EntityList
{
	extern std::vector<EntityListEntry> m_vecEntities;
	extern CTFPlayer *m_pLocalPlayer;
	extern CTFPlayerResource *m_pPlayerResource;
	extern CBaseEntity *m_pAimbotTarget;

	void Reserve();
	void Clear();
	void Store(); // Call in FrameStageNotify -> FRAME_NET_UPDATE_END
	const std::vector<EntityListEntry> &GetEntities();
	CTFPlayer *GetLocal();
	CTFPlayerResource *GetPlayerResources();
	std::vector<EntityListEntry> GetTeammates();
	std::vector<EntityListEntry> GetEnemies();
} // namespace EntityList