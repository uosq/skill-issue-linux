#include "esp_utils.h"

Color ESP_Utils::GetEntityColor(CBaseEntity *entity)
{
	if (entity == features::entities.GetAimbotTarget())
		return Config.colors.aimbot_target;

	switch(entity->GetClassID())
	{
		case ETFClassID::CBaseAnimating:
		{
			CBaseAnimating* animating = static_cast<CBaseAnimating*>(entity);

			if (animating->IsMedkit())
				return Config.colors.healthkit;
			else if (animating->IsAmmoPack())
				return Config.colors.ammopack;

			break;
		}

		default:
		break;
	}

	switch (entity->m_iTeamNum())
	{
	case ETeam::TEAM_RED:
		return Config.colors.red_team;
	case ETeam::TEAM_BLU:
		return Config.colors.blu_team;
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

	if (player->InCond(TF_COND_CLOAKED) && Config.esp.packed.ignorecloaked)
		return false;

	const ESPTeamSelectionMode mode = static_cast<ESPTeamSelectionMode>(Config.esp.team_selection);

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

	const ESPTeamSelectionMode mode = static_cast<ESPTeamSelectionMode>(Config.esp.team_selection);

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