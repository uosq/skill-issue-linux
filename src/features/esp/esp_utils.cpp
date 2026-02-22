#include "esp_utils.h"
#include "structs.h"

Color ESP_Utils::GetEntityColor(CBaseEntity* entity)
{
	if (entity == EntityList::m_pAimbotTarget)
		return Settings::Colors::aimbot_target;

	switch (entity->m_iTeamNum())
	{
		case ETeam::TEAM_RED:
			return Settings::Colors::red_team;
		case ETeam::TEAM_BLU:
			return Settings::Colors::blu_team;
		default: break;
	}

	Color defaultColor = {255, 255, 255, 255};
	return defaultColor;
}

bool ESP_Utils::IsValidPlayer(CTFPlayer* pLocal, CBaseEntity* entity)
{
	if (entity == nullptr)
		return false;

	if (!entity->IsPlayer())
		return false;

	if (!interfaces::CInput->CAM_IsThirdPerson() && entity->GetIndex() == pLocal->GetIndex())
		return false;

	CTFPlayer* player = static_cast<CTFPlayer*>(entity);

	if (!player->IsAlive())
		return false;

	if (player->InCond(TF_COND_CLOAKED) && Settings::ESP::ignorecloaked)
		return false;

	return true;
}

bool ESP_Utils::IsValidBuilding(CTFPlayer* pLocal, CBaseObject* entity)
{
	if (entity == nullptr)
		return false;

	if (entity->IsDormant())
		return false;

	return true;
}

bool ESP_Utils::IsValidEntity(CTFPlayer* pLocal, const EntityListEntry& entry)
{
	if (!entry.ptr)
		return false;

	if (entry.flags & EntityFlags::IsPlayer)
		return IsValidPlayer(pLocal, static_cast<CTFPlayer*>(entry.ptr));

	if (entry.flags & EntityFlags::IsBuilding)
		return IsValidBuilding(pLocal, static_cast<CBaseObject*>(entry.ptr));

	return false;
}
	
bool ESP_Utils::GetEntityBounds(CBaseEntity* ent, ESP_Data& out)
{
	Vector origin = ent->GetAbsOrigin();
	Vector bottom = {};

	if (!helper::engine::WorldToScreen(origin, bottom))
		return false;

	Vector top;
	Vector absHead = origin + Vector{0, 0, ent->m_vecMaxs().z};
	if (!helper::engine::WorldToScreen(absHead, top))
		return false;
	
	float h = (bottom - top).Length2D();
	float w = ent->IsTeleporter() ? (h * 2.0f) : (h * 0.3f);

	out.bottom = Vec2(bottom.x, bottom.y);
	out.height = h;
	out.width = w;
	out.top = Vec2(top.x, top.y);
	return true;
}