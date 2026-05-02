#pragma once

#include <memory>
#include <vector>

#include "../../sdk/classes/entity.h"

#include "../entitylist/entitylist.h"

namespace Chams
{
	void Init();
	void Reset();

	void OnGameShutdown();
	void OnLevelPostEntity();
	void OnLevelShutdown();
	void OnDoPostScreenSpaceEffects(CTFPlayer* pLocal);

	bool ShouldHide(int entindex);
	bool IsDrawing();

	void ApplyMaterials(CBaseEntity* entity, int drawflags);

	bool IsValidEntity(CTFPlayer* pLocal, const EntityListEntry& entity);

} // end namespace