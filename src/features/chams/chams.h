#pragma once

#include <memory>
#include <unordered_set>
#include <vector>

#include "../../sdk/classes/entity.h"

#include "../entitylist/entitylist.h"
#include "../feature.h"

class Chams
{
public:
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

private:
	void DoAttachmentColorModulation(CBaseEntity* attachment, const Color& orig_color);
	void DrawAttachments(CBaseEntity* entity, int drawflags, const Color& orig_color);
	void DrawEntityAndAttachments(CBaseEntity* entity, int drawflags);

	std::unordered_set<int> hidden_entities;
	bool is_drawing = false;
};

DECLARE_FEATURE(Chams, chams)