#pragma once

#include <vector>
#include "../../sdk/classes/cbaseobject.h"
#include "../../sdk/classes/projectiles.h"
#include "../../sdk/helpers/helper.h"

#include "structs.h"
#include "../feature.h"

class EntityList
{
public:
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

private:
	void StoreBaseAnimating(CBaseAnimating* pAnimating);

	std::vector<EntityListEntry> s_vecEntities;
	CTFPlayer* s_pLocalPlayer{nullptr};
	CTFPlayerResource* s_pPlayerResource{nullptr};
	CBaseEntity* s_pAimbotTarget{nullptr};
	std::vector<StaticEntity> s_vecStaticEntities;
};

DECLARE_FEATURE(EntityList, entities)