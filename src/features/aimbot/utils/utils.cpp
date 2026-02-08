#include "utils.h"

#define ARRAYSIZE(x) sizeof((x))/sizeof((x[0]))

namespace AimbotUtils
{
	bool IsValidEntity(CBaseEntity* entity)
	{
		if (entity == nullptr)
			return false;

		if (entity->IsPlayer())
		{
			auto* player = static_cast<CTFPlayer*>(entity);
			if (player == nullptr)
				return false;

			if (!player->IsAlive())
				return false;

			if (player->IsUbercharged())
				return false;
	
			if (player->IsGhost())
				return false;

			if (Settings::aimbot.ignorecloaked && player->InCond(ETFCond::TF_COND_CLOAKED))
				return false;

			if (Settings::aimbot.ignorebonked && player->InCond(ETFCond::TF_COND_BONKED))
				return false;

			if (Settings::aimbot.ignoreubered && player->InCond(ETFCond::TF_COND_INVULNERABLE))
				return false;

			if (Settings::aimbot.ignorehoovy && player->m_iClass() == ETFClass::TF_CLASS_HEAVYWEAPONS && (player->GetFlags() & FL_DUCKING))
				return false;

			return true;
		}

		if (entity->IsBuilding())
			return true;

		return false;
	}

	// Is this optimized? absolutely fucking not
	// I need to think of a better way
	// I should probably check bones
	bool GetVisiblePoint(Vector& out, CTFPlayer* pLocal, Vector origin, Vector mins, Vector maxs)
	{
		static float points[] = {0.15f, 0.35f, 0.5f, 0.75f, 0.85f};
		static constexpr int points_size = ARRAYSIZE(points);
		CGameTrace trace;
		CTraceFilterWorldAndPropsOnly filter;
		filter.pSkip = pLocal;

		Vector absMax, absMin;
		absMax = origin + maxs;
		absMin = origin + mins;

		Vector eyePos = pLocal->GetAbsOrigin() + pLocal->m_vecViewOffset();

		for (int x = 0; x < points_size; x++)
			for (int y = 0; y < points_size; y++)
				for (int z = 0; z < points_size; z++)
				{
					Vector point;
					point.x = absMin.x + (absMax.x - absMin.x) * points[x];
					point.y = absMin.y + (absMax.y - absMin.y) * points[y];
					point.z = absMin.z + (absMax.z - absMin.z) * points[z];

					helper::engine::Trace(eyePos, point, MASK_SHOT_HULL, &filter, &trace);

					if (trace.fraction == 1.0f)
					{
						out = point;
						return true;
					}
				}

		return false;
	}

	std::string GetAimbotModeName()
	{
		switch(Settings::aimbot.mode)
		{
			case AimbotMode::PLAIN: return "Plain";
			case AimbotMode::SMOOTH: return "Smooth";
			case AimbotMode::ASSISTANCE: return "Assistance";
			case AimbotMode::SILENT: return "Silent";
			default: return "Invalid";
                }
        }

	std::string GetTeamModeName()
	{
		switch(Settings::aimbot.teamMode)
		{
			case TeamMode::ONLYENEMY: return "Only Enemy";
			case TeamMode::ONLYTEAMMATE: return "Only Teammate";
			case TeamMode::BOTH: return "Both";
			default: return "Invalid";
                }
        }

	bool CanDamageWithSniperRifle(CTFPlayer* pLocal, CBaseEntity* pTarget, CTFWeaponBase* pWeapon)
	{
		if (pLocal == nullptr || pTarget == nullptr || pWeapon == nullptr)
			return false;

		if (!pWeapon->IsSniperRifle())
			return true;

		if (pTarget->IsPlayer())
		{
			CTFPlayer* player = static_cast<CTFPlayer*>(pTarget);
			if (player == nullptr)
				return false;

			int health = player->GetHealth();
			return static_cast<CTFSniperRifle*>(pWeapon)->GetChargedDamage() >= health;
		}

		if (pTarget->IsBuilding())
		{
			CBaseObject* obj = static_cast<CBaseObject*>(pTarget);
			if (obj == nullptr)
				return false;

			int health = obj->m_iHealth();
			return static_cast<CTFSniperRifle*>(pWeapon)->GetChargedDamage() >= health;
		}

		return false;
	}

	float GetAimbotFovScaled()
	{
		float cameraFOV = CustomFov::GetFov();

		float radAimbotHalf = DEG2RAD(Settings::aimbot.fov / 2.0f);
		float radPlayerHalf = DEG2RAD(cameraFOV / 2.0f);
		constexpr float radBaseHalf = DEG2RAD(90.0f / 2.0f);

		float scaledRad = atan(tan(radAimbotHalf) * (tan(radPlayerHalf) / tan(radBaseHalf)));

		return RAD2DEG(scaledRad) * 2.0f;
	}

	std::vector<EntityListEntry> GetTargets(const bool& bCanHitTeammates, int localTeam)
	{
		std::vector<EntityListEntry> vecEntities;

		for (const auto& entry : EntityList::GetEntities())
		{
			if (entry.ptr == EntityList::m_pLocalPlayer)
				continue;

			if (!(entry.flags & EntityFlags::IsAlive))
				continue;

			if (entry.ptr == nullptr)
				continue;

			if (!IsValidEntity(entry.ptr))
				continue;

			TeamMode teamMode = Settings::aimbot.teamMode;
			int teamNum = entry.ptr->m_iTeamNum();

			if (!bCanHitTeammates || teamMode == TeamMode::ONLYENEMY)
			{
				if (teamNum == localTeam)
					continue;
			}

			if (bCanHitTeammates && teamMode == TeamMode::ONLYTEAMMATE)
			{
				if (teamNum != localTeam)
					continue;
			}

			vecEntities.emplace_back(entry);
		}

		return vecEntities;
	}
};