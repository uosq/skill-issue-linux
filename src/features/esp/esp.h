#pragma once

#include "../../sdk/interfaces/interfaces.h"
#include "../../sdk/helpers/helper.h"
#include "../../sdk/classes/weaponbase.h"
#include "../../sdk/classes/cbaseobject.h"
#include "../../sdk/definitions/eteam.h"
#include "../entitylist/entitylist.h"
#include "../../settings.h"

namespace ESP
{
	inline Color GetEntityColor(CBaseEntity* entity)
	{
		if (entity == EntityList::m_pAimbotTarget)
			return g_Settings.colors.aimbot_target;

		switch (entity->m_iTeamNum())
		{
			case ETeam::TEAM_RED:
				return g_Settings.colors.red_team;
			case ETeam::TEAM_BLU:
				return g_Settings.colors.blu_team;
			default: break;
		}

		Color defaultColor = {255, 255, 255, 255};
		return defaultColor;
	}

	inline bool IsValidPlayer(CTFPlayer* pLocal, CBaseEntity* entity)
	{
		if (entity == nullptr)
			return false;

		if (!entity->IsPlayer())
			return false;

		if (!interfaces::CInput->CAM_IsThirdPerson() && entity->GetIndex() == pLocal->GetIndex())
			return false;

		CTFPlayer* player = static_cast<CTFPlayer*>(entity);
		if (!player)
			return false;

		if (!player->IsAlive())
			return false;

		if (player->InCond(TF_COND_CLOAKED) && g_Settings.esp.ignorecloaked)
			return false;

		return true;
	}

	inline bool IsValidBuilding(CTFPlayer* pLocal, CBaseObject* entity)
	{
		if (entity == nullptr)
			return false;

		if (entity->IsDormant())
			return false;

		return true;
	}

	inline void PaintBox(Color color, Vector top, Vector bottom, int w, int h)
	{
		helper::draw::SetColor(color);
		helper::draw::OutlinedRect(top.x - w, bottom.y - h, bottom.x + w, bottom.y);
	}

	inline void PaintName(Color color, Vector top, int w, int h, std::string name)
	{
		int textw, texth;
		helper::draw::GetTextSize(name, textw, texth);
		helper::draw::TextShadow(top.x - (textw*0.5f), top.y - texth - 2, color, name);
	}

	inline void Run(CTFPlayer* pLocal)
	{
		if (!helper::engine::IsInMatch() || !g_Settings.esp.enabled)
			return;

		Color white {255, 255, 255, 255};
		helper::draw::SetFont(fontManager.GetCurrentFont());

		for (auto& entry : EntityList::GetEntities())
		{
			bool isPlayer, isBuilding;
			isPlayer = entry.flags & EntityFlags::IsPlayer;
			isBuilding = entry.flags & EntityFlags::IsBuilding;

			if (!isPlayer && !isBuilding)
				continue;

			CBaseEntity* entity = entry.ptr;
			if (entity == nullptr)
				continue;

			if (isPlayer)
			{
				CTFPlayer* pPlayer = static_cast<CTFPlayer*>(entry.ptr);
				if (!interfaces::CInput->CAM_IsThirdPerson() && entity->GetIndex() == pLocal->GetIndex())
					continue;

				if (!pPlayer->IsAlive())
					continue;
	
				if (pPlayer->InCond(TF_COND_CLOAKED) && g_Settings.esp.ignorecloaked)
					continue;
			}

			Vector origin = entity->GetAbsOrigin();
			Vector feet; // fuck i need to sleep
			if (!helper::engine::WorldToScreen(origin, feet))
				continue;

			Vector head;
			if (!helper::engine::WorldToScreen(origin + Vector{0, 0, entity->m_vecMaxs().z}, head))
				continue;

			int h = (feet - head).Length2D();
			int w = (entity->IsTeleporter()) ? (h * 2.0f) : (h * 0.3);

			Color color = GetEntityColor(entity);

			if (g_Settings.esp.box)
				PaintBox(color, head, feet, w, h);
	
			// name
			if (g_Settings.esp.name)
			{
				if (isPlayer)
					PaintName(white, head, w, h, static_cast<CTFPlayer*>(entity)->GetName());
				else if (isBuilding)
					PaintName(white, head, w, h, static_cast<CBaseObject*>(entity)->GetName());
				else
					PaintName(white, head, w, h, entity->GetClientClass()->networkName);
			}
		}
	}
};