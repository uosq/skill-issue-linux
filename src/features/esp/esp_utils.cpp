#include "esp_utils.h"

Color ESP_Utils::GetEntityColor(CBaseEntity *entity)
{
	if (entity == EntityList::m_pAimbotTarget)
		return Settings::Colors.aimbot_target;

	switch (entity->m_iTeamNum())
	{
	case ETeam::TEAM_RED:
		return Settings::Colors.red_team;
	case ETeam::TEAM_BLU:
		return Settings::Colors.blu_team;
	default:
		break;
	}

	Color defaultColor = {255, 255, 255, 255};
	return defaultColor;
}

bool ESP_Utils::IsValidPlayer(CTFPlayer *pLocal, CBaseEntity *entity)
{
	if (entity == nullptr)
		return false;

	if (!entity->IsPlayer())
		return false;

	if (!interfaces::CInput->CAM_IsThirdPerson() && entity->GetIndex() == pLocal->GetIndex())
		return false;

	CTFPlayer *player = static_cast<CTFPlayer *>(entity);

	if (!player->IsAlive())
		return false;

	if (player->InCond(TF_COND_CLOAKED) && Settings::ESP.ignorecloaked)
		return false;

	const ESPTeamSelectionMode mode = static_cast<ESPTeamSelectionMode>(Settings::ESP.team_selection);

	bool bIsEnemy			= pLocal->m_iTeamNum() != entity->m_iTeamNum();

	if (bIsEnemy && mode == ESPTeamSelectionMode::TEAMMATES)
		return false;

	if (!bIsEnemy && mode == ESPTeamSelectionMode::ENEMIES)
		return false;

	return true;
}

bool ESP_Utils::IsValidBuilding(CTFPlayer *pLocal, CBaseObject *entity)
{
	if (entity == nullptr)
		return false;

	if (entity->IsDormant())
		return false;

	const ESPTeamSelectionMode mode = static_cast<ESPTeamSelectionMode>(Settings::ESP.team_selection);

	if (mode >= ESPTeamSelectionMode::MAX || mode <= ESPTeamSelectionMode::INVALID)
		return false;

	bool bIsEnemy = pLocal->m_iTeamNum() != entity->m_iTeamNum();

	if (bIsEnemy && mode == ESPTeamSelectionMode::TEAMMATES)
		return false;

	if (!bIsEnemy && mode == ESPTeamSelectionMode::ENEMIES)
		return false;

	return true;
}

bool ESP_Utils::IsValidEntity(CTFPlayer *pLocal, const EntityListEntry &entry)
{
	if (!entry.ptr)
		return false;

	if (entry.flags & EntityFlags::IsPlayer)
		return IsValidPlayer(pLocal, static_cast<CTFPlayer *>(entry.ptr));

	if (entry.flags & EntityFlags::IsBuilding)
		return IsValidBuilding(pLocal, static_cast<CBaseObject *>(entry.ptr));

	return false;
}