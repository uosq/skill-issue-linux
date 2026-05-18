#include "utils.h"

#define ARRAYSIZE(x) sizeof((x)) / sizeof((x[0]))

namespace AimbotUtils
{
	bool IsValidEntity(CBaseEntity *entity)
	{
		if (entity == nullptr)
			return false;

		if (entity->IsPlayer())
		{
			auto *player = static_cast<CTFPlayer *>(entity);
			if (player == nullptr)
				return false;

			if (!player->IsAlive())
				return false;

			if (player->IsUbercharged())
				return false;

			if (player->IsGhost())
				return false;

			if (Config.aimbot.packed.ignorecloaked && player->InCond(ETFCond::TF_COND_CLOAKED))
				return false;

			if (Config.aimbot.packed.ignorebonked && player->InCond(ETFCond::TF_COND_BONKED))
				return false;

			if (Config.aimbot.packed.ignoreubered && player->InCond(ETFCond::TF_COND_INVULNERABLE))
				return false;

			if (Config.aimbot.packed.ignorehoovy && player->m_iClass() == ETFClass::TF_CLASS_HEAVYWEAPONS &&
			    (player->GetFlags() & FL_DUCKING))
				return false;

			return true;
		}

		if (entity->IsBuilding())
		{
			CBaseObject* obj = static_cast<CBaseObject*>(entity);
			return obj->m_iHealth() > 0;
		}

		return false;
	}

	// Is this optimized? absolutely fucking not
	// I need to think of a better way
	// I should probably check bones
	bool GetVisiblePoint(Vector &out, CTFPlayer *pLocal, Vector origin, Vector mins, Vector maxs)
	{
		static float points[]		 = {0.15f, 0.35f, 0.5f, 0.75f, 0.85f};
		static constexpr int points_size = ARRAYSIZE(points);
		CGameTrace trace;
		CTraceFilterWorldAndPropsOnly filter;
		filter.pSkip = pLocal;

		Vector absMax, absMin;
		absMax	      = origin + maxs;
		absMin	      = origin + mins;

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
		switch (static_cast<AimbotMode>(Config.aimbot.packed.aimmode))
		{
		case AimbotMode::PLAIN:
			return "Plain";
		case AimbotMode::SMOOTH:
			return "Smooth";
		case AimbotMode::ASSISTANCE:
			return "Assistance";
		case AimbotMode::SILENT:
			return "Silent";
		default:
			return "Invalid";
		}
	}

	std::string GetTeamModeName()
	{
		switch (static_cast<TeamMode>(Config.aimbot.packed.teamselection))
		{
		case TeamMode::ONLYENEMY:
			return "Only Enemy";
		case TeamMode::ONLYTEAMMATE:
			return "Only Teammate";
		case TeamMode::BOTH:
			return "Both";
		default:
			return "Invalid";
		}
	}

	bool CanDamageWithSniperRifle(CTFPlayer *pLocal, CBaseEntity *pTarget, CTFWeaponBase *pWeapon)
	{
		if (pLocal == nullptr || pTarget == nullptr || pWeapon == nullptr)
			return false;

		if (!pWeapon->IsSniperRifle())
			return true;

		if (pTarget->IsPlayer())
		{
			CTFPlayer *player = static_cast<CTFPlayer *>(pTarget);
			if (player == nullptr)
				return false;

			int health = player->GetHealth();
			return static_cast<CTFSniperRifle *>(pWeapon)->GetChargedDamage() >= health;
		}

		if (pTarget->IsBuilding())
		{
			CBaseObject *obj = static_cast<CBaseObject *>(pTarget);
			if (obj == nullptr)
				return false;

			int health = obj->m_iHealth();
			return static_cast<CTFSniperRifle *>(pWeapon)->GetChargedDamage() >= health;
		}

		return false;
	}

	float GetFovScaled(float flFov)
	{
		float cameraFOV		    = features::customfov.GetFov();

		float radAimbotHalf	    = DEG2RAD(flFov / 2.0f);
		float radPlayerHalf	    = DEG2RAD(cameraFOV / 2.0f);
		constexpr float radBaseHalf = DEG2RAD(90.0f / 2.0f);

		float scaledRad		    = atan(tan(radAimbotHalf) * (tan(radPlayerHalf) / tan(radBaseHalf)));

		return RAD2DEG(scaledRad) * 2.0f;
	}

	float GetAimbotFovScaled()
	{
		return GetFovScaled(Config.aimbot.fov);
	}

	std::vector<EntityListEntry> GetTargets(const bool &bCanHitTeammates, int localTeam)
	{
		std::vector<EntityListEntry> vecEntities;

		for (const auto &entry : features::entities.GetEntities())
		{
			if (entry.ptr == features::entities.GetLocal())
				continue;
		
			if (!(entry.flags & EntityFlags::IsAlive))
				continue;
			
			if (entry.ptr == nullptr)
				continue;

			if (!IsValidEntity(entry.ptr))
				continue;

			TeamMode teamMode = static_cast<TeamMode>(Config.aimbot.packed.teamselection);
			int teamNum	  = entry.ptr->m_iTeamNum();

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
}; // namespace AimbotUtils

// Am I doing this right?
bool AimbotUtils::RebuildAnimationMatrix(CTFPlayer* pPlayer, const Vector& predictedOrigin, const Vector& predictedVelocity, float predictedTime, matrix3x4* outBones)
{
        if (!pPlayer || !outBones)
                return false;

        Vector backupOrigin = pPlayer->GetAbsOrigin();
        Vector backupVelocity = pPlayer->GetVelocity();
        float backupSimTime = pPlayer->m_flSimulationTime();
        float backupAnimTime = pPlayer->m_flAnimTime();
        
        int backupFlags = pPlayer->GetFlags();
        int backupEFlags = pPlayer->m_iEFlags();
        int backupEffects = pPlayer->m_fEffects();

        float backupCurTime = interfaces::GlobalVars->curtime;
        float backupFrameTime = interfaces::GlobalVars->frametime;
        int backupTickCount = interfaces::GlobalVars->tickcount;

	bool backupClientSideAnim = pPlayer->m_bClientSideAnimation();

	auto backupPoses = pPlayer->m_flPoseParameter();

        interfaces::GlobalVars->curtime = predictedTime;
        interfaces::GlobalVars->frametime = interfaces::GlobalVars->interval_per_tick;
        interfaces::GlobalVars->tickcount = TIME_TO_TICKS(predictedTime);

        pPlayer->SetAbsOrigin(predictedOrigin);
        pPlayer->GetVelocity() = predictedVelocity;
        pPlayer->m_flSimulationTime() = predictedTime;
        pPlayer->m_flAnimTime() = predictedTime;

        pPlayer->m_bClientSideAnimation() = true;
        pPlayer->m_iEFlags() &= ~EFL_DIRTY_ABSVELOCITY;
	pPlayer->m_fEffects() |= EF_NOINTERP;

        pPlayer->InvalidateBoneCache();
        pPlayer->UpdateClientSideAnimation();

        bool bSuccess = pPlayer->SetupBones(outBones, MAXSTUDIOBONES, BONE_USED_BY_ANYTHING, predictedTime);

        pPlayer->SetAbsOrigin(backupOrigin);
        pPlayer->GetVelocity() = backupVelocity;
        pPlayer->m_flSimulationTime() = backupSimTime;
        pPlayer->m_flAnimTime() = backupAnimTime;
        
        pPlayer->GetFlags() = backupFlags;
        pPlayer->m_iEFlags() = backupEFlags;
        pPlayer->m_fEffects() = backupEffects;

	pPlayer->m_flPoseParameter() = backupPoses;

	pPlayer->m_bClientSideAnimation() = backupClientSideAnim;

        interfaces::GlobalVars->curtime = backupCurTime;
        interfaces::GlobalVars->frametime = backupFrameTime;
        interfaces::GlobalVars->tickcount = backupTickCount;

        pPlayer->InvalidateBoneCache();

        return bSuccess;
}

Vec3 AimbotUtils::GetSmoothedAngle(const Vec3& viewAngles, const Vec3& targetDir)
{
	Vector delta = targetDir - viewAngles;
	delta.x = Math::NormalizeAngle(delta.x);
	delta.y = Math::NormalizeAngle(delta.y);
	
	float smoothFactor = std::max(1.0f, Config.aimbot.smoothness);
	Vector stepAngle = delta / smoothFactor;

	return viewAngles + stepAngle;
}

CBaseEntity* AimbotUtils::LookingAtEntity(CTFPlayer* pLocal, const Vec3& viewAngles)
{
	CGameTrace trace {};
	CTraceFilterHitscan filter {};
	filter.pSkip = pLocal;

	Vec3 forward; /* = */ Math::AngleVectors(viewAngles, &forward);
	Vec3 shoot_pos = pLocal->GetEyePos();

	helper::engine::Trace(shoot_pos, shoot_pos + (forward * 8192), MASK_SHOT | CONTENTS_HITBOX, &filter, &trace);

	return trace.m_pEnt;
}