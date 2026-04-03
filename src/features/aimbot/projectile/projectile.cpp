#include "projectile.h"

#include "../../logs/logs.h"
#include "../../prediction/prediction.h"

#include <cmath>

CAimbotProjectile::CAimbotProjectile()
{
	// reserve at least 1 second
	m_vecPath.reserve(67);
	m_pTarget = nullptr;
}

CBaseEntity *CAimbotProjectile::GetCurrentTarget()
{
	return m_pTarget;
}

std::vector<Vector> &CAimbotProjectile::GetPath()
{
	return m_vecPath;
}

float CAimbotProjectile::GetInitialZOffset(CTFWeaponBase *pWeapon, const Vector &vecMaxs)
{
	switch (pWeapon->GetWeaponID())
	{
	case TF_WEAPON_ROCKETLAUNCHER:
	case TF_WEAPON_ROCKETLAUNCHER_DIRECTHIT:
		return 10.0f;

	case TF_WEAPON_COMPOUND_BOW:
		return vecMaxs.z;

	case TF_WEAPON_GRENADELAUNCHER:
		return vecMaxs.z * 0.25f;

	default:
		break;
	}

	return vecMaxs.z * 0.5f;
}

bool CAimbotProjectile::SolveBallisticArc(Vector &outAngle, const Vector p0, const Vector p1, float flSpeed,
					  float flGravity)
{
	Vector diff  = p1 - p0;
	float dx     = diff.Length2D();
	float dy     = diff.z;
	float speed2 = flSpeed * flSpeed;
	float g	     = flGravity;

	float root   = speed2 * speed2 - g * (g * dx * dx + 2 * dy * speed2);
	if (root < 0)
		return false;

	float angle, yaw, pitch;
	angle = atan((speed2 - sqrt(root)) / (g * dx));
	yaw   = RAD2DEG(atan2(diff.y, diff.x));
	pitch = RAD2DEG(-angle);

	outAngle.Set(pitch, yaw);
	return true;
}

// Yes, I know this isn't predicting right
bool CAimbotProjectile::CheckTrajectory(CBaseEntity *pTarget, const Vector vecStartPos, const Vector vecTargetPos,
					const Vector vecAngle, const ProjectileInfo_t &prjInfo, float flGravity)
{
	if (pTarget == nullptr)
		return false;

	float flDistance  = (vecTargetPos - vecStartPos).Length();
	float flTotalTime = flDistance / prjInfo.speed;

	Vector vecVelocity;
	Math::AngleVectors(vecAngle, &vecVelocity);
	vecVelocity *= prjInfo.speed;

	CGameTrace trace;
	CTraceFilterCollideable filter;
	filter.pSkip = pTarget;

	Vector vecMins{-prjInfo.hull.x, -prjInfo.hull.y, -prjInfo.hull.z};
	Vector vecMaxs{prjInfo.hull.x, prjInfo.hull.y, prjInfo.hull.z};

	Vector vecPos	= vecStartPos;

	float flClock	= 0.0f;
	float flDt	= interfaces::GlobalVars->interval_per_tick;
	float flMaxTime = Settings::Aimbot.max_sim_time;

	while (flClock < flMaxTime)
	{
		Vector prevPos = vecPos;

		vecVelocity.z -= flGravity * flDt;
		vecPos += vecVelocity * flDt;

		helper::engine::TraceHull(prevPos, vecPos, vecMins, vecMaxs, MASK_SOLID, &filter, &trace);

		if (trace.fraction < 1.0f)
			return false;

		flClock += flDt;

		// check if we have passed the target pos
		Vector vecToTarget = vecTargetPos - vecPos;
		if (vecToTarget.Dot(vecVelocity) < 0.0f)
			break;
	}

	return true;
}

bool CAimbotProjectile::GetProjectileInfo(ProjectileInfo_t &pOut, CTFPlayer *pLocal, CTFWeaponBase *pWeapon)
{
	if (pLocal == nullptr || pWeapon == nullptr)
		return false;

	bool bDucking	 = pLocal->GetFlags() & FL_DUCKING;
	float flGravity	 = interfaces::Cvar->FindVar("sv_gravity")->GetFloat() / 800;

	int id		 = pWeapon->GetWeaponID();

	int iTickBase	 = pLocal->GetTickBase();
	float flTickBase = TICKS_TO_TIME(iTickBase);

	switch (id)
	{
	case TF_WEAPON_ROCKETLAUNCHER:
	case TF_WEAPON_ROCKETLAUNCHER_DIRECTHIT:
	{
		pOut.hull.Set();
		pOut.speed    = pLocal->InCond(TF_COND_RUNE_PRECISION)
				    ? 3000
				    : AttributeHookValue(1100, "mult_projectile_speed", pWeapon, nullptr, true);
		pOut.offset.x = 23.5f;
		pOut.offset.y = AttributeHookValue(0, "centerfire_projectile", pWeapon, nullptr, true) == 1 ? 0 : 12;
		pOut.offset.z = bDucking ? 8 : -3;
		pOut.damage_radius = id == TF_WEAPON_ROCKETLAUNCHER ? 146 : 44;
		pOut.simple_trace  = true;
		return true;
	}

	case TF_WEAPON_PARTICLE_CANNON:
	case TF_WEAPON_RAYGUN:
	case TF_WEAPON_DRG_POMSON:
	{
		bool bIsCowMangler = id == TF_WEAPON_PARTICLE_CANNON;
		pOut.offset.Set(23.5, 8, bDucking ? 8 : -3);
		pOut.speed	  = bIsCowMangler ? 1100 : 1200;
		pOut.hull	  = bIsCowMangler ? Vector(0, 0, 0) : Vector(1, 1, 1);
		pOut.simple_trace = true;
		return true;
	}

	case TF_WEAPON_GRENADELAUNCHER:
	case TF_WEAPON_CANNON:
	{
		bool bIsCannon = id == TF_WEAPON_CANNON;
		float mortar =
		    bIsCannon ? AttributeHookValue(0.f, "grenade_launcher_mortar_mode", pWeapon, nullptr, true) : 0;
		pOut.speed =
		    AttributeHookValue(pLocal->InCond(TF_COND_RUNE_PRECISION)
					   ? 3000
					   : AttributeHookValue(1200, "mult_projectile_range", pWeapon, nullptr, true),
				       "mult_projectile_range", pWeapon, nullptr, true);
		pOut.gravity = flGravity;
		return true;
	}

	case TF_WEAPON_PIPEBOMBLAUNCHER:
	{
		pOut.offset.Set(16, 8, -6);
		pOut.gravity		  = flGravity;

		float charge		  = 0.0f;
		float m_flChargeBeginTime = ((CTFPipebombLauncher *)pWeapon)->m_flChargeBeginTime();
		if (m_flChargeBeginTime > flTickBase)
			charge = 0.0f;
		else
			charge = flTickBase - m_flChargeBeginTime;

		pOut.speed = AttributeHookValue(
		    Math::RemapVal(0, 0, AttributeHookValue(4.0f, "stickybomb_charge_rate", pWeapon, nullptr, true),
				   900, 2400, true),
		    "mult_projectile_range", pWeapon, nullptr, true);
		return true;
	}

	case TF_WEAPON_FLAREGUN:
	{
		pOut.offset.Set(23.5, 12, bDucking ? 8 : -3);
		pOut.hull.Set(0, 0, 0);
		pOut.speed    = AttributeHookValue(2000, "mult_projectile_speed", pWeapon, nullptr, true);
		pOut.gravity  = 0.01f;
		pOut.lifetime = 0.3 * flGravity;
		return true;
	}

	case TF_WEAPON_FLAREGUN_REVENGE:
	{
		pOut.offset.Set(23.5, 12, bDucking ? 8 : -3);
		pOut.hull.Set(0, 0, 0);
		pOut.speed = 3000;
		return true;
	}

	case TF_WEAPON_COMPOUND_BOW:
	{
		pOut.offset.Set(23.5, 12, -3);
		pOut.hull.Set(1, 1, 1);

		float flchargebegintime = static_cast<CTFPipebombLauncher *>(pWeapon)->m_flChargeBeginTime();
		float charge		= 0.0f;
		if (flchargebegintime > 0)
			charge = TICKS_TO_TIME(pLocal->GetTickBase()) - flchargebegintime;

		pOut.speed    = Math::RemapVal(charge, 0, 1, 1800, 2600);
		pOut.gravity  = Math::RemapVal(charge, 0, 1, 0.5, 0.1) * flGravity;
		pOut.lifetime = 10;
		return true;
	}

	case TF_WEAPON_CROSSBOW:
	case TF_WEAPON_SHOTGUN_BUILDING_RESCUE:
	{
		bool isCrossbow = id == TF_WEAPON_CROSSBOW;
		pOut.offset.Set(23.5, 12, -3);
		pOut.hull     = isCrossbow ? Vector(3, 3, 3) : Vector(1, 1, 1);
		pOut.speed    = 2400;
		pOut.gravity  = flGravity * 0.2;
		pOut.lifetime = 10;
		return true;
	}

	case TF_WEAPON_SYRINGEGUN_MEDIC:
	{
		pOut.offset.Set(16, 6, -8);
		pOut.hull.Set(1, 1, 1);
		pOut.speed   = 1000;
		pOut.gravity = 0.3 * flGravity;
		return true;
	}

	case TF_WEAPON_FLAMETHROWER:
	{
		static ConVar *tf_flamethrower_size = interfaces::Cvar->FindVar("tf_flamethrower_size");
		if (!tf_flamethrower_size)
			return false;

		float flhull = tf_flamethrower_size->GetFloat();
		pOut.offset.Set(40, 5, 0);
		pOut.hull.Set(flhull, flhull, flhull);
		pOut.speed	  = 1000;
		pOut.lifetime	  = 0.285;
		pOut.simple_trace = true;
		return true;
	}

	case TF_WEAPON_FLAME_BALL:
	{
		pOut.offset.Set(3, 7, -9);
		pOut.hull.Set(1, 1, 1);
		pOut.speed	  = 3000;
		pOut.lifetime	  = 0.18;
		pOut.gravity	  = 0;
		pOut.simple_trace = true;
		return true;
	}

	case TF_WEAPON_CLEAVER:
	{
		pOut.offset.Set(16, 8, -6);
		pOut.hull.Set(1, 1, 10); // wtf is this 10?
		pOut.gravity  = 1;
		pOut.lifetime = 2.2;
		return true;
	}

	case TF_WEAPON_BAT_WOOD:
	case TF_WEAPON_BAT_GIFTWRAP:
	{
		static ConVar *tf_scout_stunball_base_speed = interfaces::Cvar->FindVar("tf_scout_stunball_base_speed");
		pOut.speed				    = tf_scout_stunball_base_speed->GetFloat();
		pOut.gravity				    = 1;
		pOut.lifetime				    = flGravity;
		return true;
	}

	case TF_WEAPON_JAR:
	case TF_WEAPON_JAR_MILK:
	{
		pOut.offset.Set(16, 8, -6);
		pOut.speed    = 1000;
		pOut.gravity  = 1;
		pOut.lifetime = 2.2;
		pOut.hull.Set(3, 3, 3);
		return true;
	}

	case TF_WEAPON_JAR_GAS:
	{
		pOut.offset.Set(16, 8, -6);
		pOut.speed    = 2000;
		pOut.gravity  = 1;
		pOut.lifetime = 2.2;
		pOut.hull.Set(3, 3, 3);
		return true;
	}

	case TF_WEAPON_LUNCHBOX:
	{
		pOut.offset.z = -8;
		pOut.hull.Set(17, 17, 7);
		pOut.speed   = 500;
		pOut.gravity = 1 * flGravity;
		return true;
	}
	}

	return false;
}

void CAimbotProjectile::Reset()
{
	if (!m_vecPath.empty())
		m_vecPath.clear();

	if (m_pTarget != nullptr)
		m_pTarget = nullptr;
}

std::vector<PotentialTarget> CAimbotProjectile::GetBestTargets(CTFPlayer *pLocal, CTFWeaponBase *pWeapon)
{
	auto vTargets = AimbotUtils::GetTargets(pWeapon->CanHitTeammates(), pLocal->m_iTeamNum());
	if (vTargets.empty())
		return {};

	float flMaxFov = AimbotUtils::GetAimbotFovScaled();

	std::vector<PotentialTarget> vPotentialTargets{};

	Vector vecLocalCenter = pLocal->GetCenter();
	Vector vecEyePos      = pLocal->GetEyePos();
	Vector vecViewAngles;
	interfaces::Engine->GetViewAngles(vecViewAngles);

	bool bNoFovLimit = Settings::Aimbot.fov >= 180.0f;

	for (const auto &targetEntry : vTargets)
	{
		Vector vecCenter = targetEntry.ptr->GetCenter();
		Vector vecDir	 = (vecLocalCenter - vecCenter);

		float flDistance = vecDir.Normalize();

		// no need to try to aim at someone in another country
		if (flDistance > 4096.0f)
			continue;

		Vector vecAngle = Math::CalcAngle(vecEyePos, vecCenter);
		float flFov	= Math::CalcFov(vecViewAngles, vecAngle);

		if (!bNoFovLimit && flFov > flMaxFov)
			continue;

		vPotentialTargets.emplace_back(PotentialTarget{vecDir, vecCenter, flDistance, flFov, targetEntry.ptr});
	}

	if (vPotentialTargets.empty())
		return {};

	std::sort(vPotentialTargets.begin(), vPotentialTargets.end(),
		  [&](PotentialTarget a, PotentialTarget b) { return a.fov < b.fov; });

	return vPotentialTargets;
}

void CAimbotProjectile::RunMain(CTFPlayer *pLocal, CTFWeaponBase *pWeapon)
{
	Reset();

	if (!Settings::Aimbot.key->IsEnabled())
		return;

	bool bVisualsEnabled = Settings::Aimbot.path || Settings::Aimbot.indicator;

	if (!bVisualsEnabled && !Settings::Aimbot.key->IsActive())
		return;

	static ConVar *sv_gravity = interfaces::Cvar->FindVar("sv_gravity");
	if (sv_gravity == nullptr)
		return Logs::Error("[CAimbotProjectile::RunMain] sv_gravity is null!");

	if (pLocal == nullptr || pWeapon == nullptr)
		return;

	ProjectileInfo_t prjInfo{};
	if (!GetProjectileInfo(prjInfo, pLocal, pWeapon))
		return;

	auto vTargets = GetBestTargets(pLocal, pWeapon);
	if (vTargets.empty())
		return;

	Vector vecEyePos  = pLocal->GetEyePos();

	float flPrimeTime = pWeapon->GetWeaponID() == TF_WEAPON_PIPEBOMBLAUNCHER ? 0.7f : 0;

	for (const auto &target : vTargets)
	{
		if (target.entity == nullptr)
			continue;

		float flTime = target.distance / prjInfo.speed;
		if (flTime > Settings::Aimbot.max_sim_time)
			continue;

		flTime += flPrimeTime;

		Vector vecAimPos{};
		std::vector<Vector> vPath;

		if (target.entity->IsPlayer())
		{
			CTFPlayer *pPlayer = static_cast<CTFPlayer *>(target.entity);

			// try to avoid reallocating a lot of memory
			vPath.reserve(TIME_TO_TICKS(flTime));

			gPrediction.BeginPrediction(pPlayer, flTime);
			gPrediction.Simulate(vPath);
			gPrediction.EndPrediction();

			if (vPath.empty())
				continue;

			vecAimPos = vPath.back();
			vecAimPos.z += GetInitialZOffset(pWeapon, pPlayer->m_vecMaxs());
		}
		else
		{
			vecAimPos = target.center;
			vPath.emplace_back(target.center);
		}

		if (prjInfo.simple_trace)
		{
			Vector vecDir = vecAimPos - vecEyePos;
			vecDir.Normalize();
			Vector vecAngle = vecDir.ToAngle();

			Vector vecForward, vecRight, vecUp;
			Math::AngleVectors(vecAngle, &vecForward, &vecRight, &vecUp);

			Vector vecSpawnPos = vecEyePos + vecForward * prjInfo.offset.x + vecRight * prjInfo.offset.y +
					     vecUp * prjInfo.offset.z;

			if (!CheckTrajectory(target.entity, vecSpawnPos, vecAimPos, vecAngle, prjInfo, 0.0f))
				continue;

			m_pTarget		    = target.entity;
			m_vecAimAngle		    = vecAngle;
			m_vecAimPos		    = vecAimPos;
			m_vecPath		    = vPath;
			EntityList::m_pAimbotTarget = target.entity;
			return;
		}
		else
		{
			float flGravity = sv_gravity->GetFloat() * 0.5f * prjInfo.gravity;

			Vector vecAngle;
			if (!SolveBallisticArc(vecAngle, vecEyePos, vecAimPos, prjInfo.speed, flGravity))
				continue;

			Vector vecForward, vecRight, vecUp;
			Math::AngleVectors(vecAngle, &vecForward, &vecRight, &vecUp);

			Vector vecSpawnPos = vecEyePos + vecForward * prjInfo.offset.x + vecRight * prjInfo.offset.y +
					     vecUp * prjInfo.offset.z;

			if (!CheckTrajectory(target.entity, vecSpawnPos, vecAimPos, vecAngle, prjInfo, flGravity))
				continue;

			m_pTarget		    = target.entity;
			m_vecAimAngle		    = vecAngle;
			m_vecAimPos		    = vecAimPos + Vector{0, 0, GetAimDrop(flGravity, flTime)};
			m_vecPath		    = vPath;
			EntityList::m_pAimbotTarget = target.entity;
			return;
		}
	}
}

void CAimbotProjectile::RunAim(CTFPlayer *pLocal, CTFWeaponBase *pWeapon, CUserCmd *pCmd, AimbotState &pState)
{
	if (!Settings::Aimbot.key->IsActive())
		return;

	if (m_pTarget == nullptr || m_vecPath.empty())
		return;

	if (Settings::Aimbot.autoshoot)
	{
		if (IsRightAttack(pWeapon))
			pCmd->buttons |= IN_ATTACK2;
		else
			pCmd->buttons |= IN_ATTACK;
	}

	if (helper::localplayer::CanShoot(pLocal, pWeapon, pCmd))
	{
		if ((pCmd->buttons & IN_ATTACK))
		{
			int weaponID = pWeapon->GetWeaponID();
			switch (weaponID)
			{
			case TF_WEAPON_COMPOUND_BOW:
			case TF_WEAPON_PIPEBOMBLAUNCHER:
			{
				float flchargebegintime =
				    static_cast<CTFPipebombLauncher *>(pWeapon)->m_flChargeBeginTime();
				// interfaces::Cvar->ConsolePrintf("Charge:
				// %f\n", flchargebegintime);

				float charge = flchargebegintime > 0.f
						   ? TICKS_TO_TIME(pLocal->GetTickBase()) - flchargebegintime
						   : 0.f;
				if (charge > 0.0f)
					pCmd->buttons &= ~IN_ATTACK;
				break;
			}

			default:
				break;
			}
		}
	}

	if (helper::localplayer::IsAttacking(pLocal, pWeapon, pCmd))
	{
		pState.targetPath = m_vecPath;

		pCmd->viewangles  = m_vecAimAngle;
		pState.angle	  = m_vecAimAngle;

		AimbotMode mode	  = static_cast<AimbotMode>(Settings::Aimbot.mode);

		if (mode == AimbotMode::SILENT && !IsRightAttack(pWeapon))
			pState.shouldSilent = true;

		if (mode == AimbotMode::PLAIN)
			interfaces::Engine->SetViewAngles(m_vecAimAngle);
	}
}

void CAimbotProjectile::ResetIndicator()
{
	m_vecOldIndicatorPos.Set(0, 0);
	m_pOldIndicatorTarget = nullptr;
}

void CAimbotProjectile::RunIndicator()
{
	if (!Settings::Aimbot.key->IsEnabled())
		return;

	if (interfaces::Engine->IsTakingScreenshot())
		return ResetIndicator();

	if (Settings::Aimbot.indicator == static_cast<int>(AimbotIndicatorStyle::NONE))
		return ResetIndicator();

	CTFPlayer *pLocal = EntityList::GetLocal();
	if (pLocal == nullptr)
		return ResetIndicator();

	CTFWeaponBase *pWeapon = HandleAs<CTFWeaponBase *>(pLocal->GetActiveWeapon());
	if (pWeapon == nullptr)
		return ResetIndicator();

	if (m_pTarget == nullptr)
		return ResetIndicator();

	if (m_pOldIndicatorTarget != m_pTarget)
	{
		m_pOldIndicatorTarget = m_pTarget;
		m_vecOldIndicatorPos  = m_vecAimPos;
	}

	m_vecOldIndicatorPos = m_vecOldIndicatorPos.Lerp(m_vecAimPos, interfaces::GlobalVars->frametime * 10.0f);

	Vec2 screenPos;
	if (helper::engine::WorldToScreen(m_vecOldIndicatorPos, screenPos))
	{
		const int iSize = 5;

		interfaces::Surface->DrawSetColor(255, 255, 255, 255);

		switch (static_cast<AimbotIndicatorStyle>(Settings::Aimbot.indicator))
		{
		case AimbotIndicatorStyle::NONE:
			break;

		case AimbotIndicatorStyle::CIRCLE:
		{
			interfaces::Surface->DrawOutlinedCircle((int)screenPos.x, (int)screenPos.y, iSize, 68);
			break;
		}
		case AimbotIndicatorStyle::SQUARE:
		{
			interfaces::Surface->DrawFilledRect(screenPos.x - iSize, screenPos.y - iSize,
							    screenPos.x + iSize, screenPos.y + iSize);
			break;
		}
		case AimbotIndicatorStyle::TRIANGLE:
		{
			Vec2 p1, p2, p3;
			p1 = {screenPos.x - iSize, screenPos.y + iSize};
			p2 = {screenPos.x, screenPos.y - iSize};
			p3 = {screenPos.x + iSize, screenPos.y + iSize};

			// left point
			interfaces::Surface->DrawLine(p1.x, p1.y, p2.x, p2.y);

			// top center point
			interfaces::Surface->DrawLine(p2.x, p2.y, p3.x, p3.y);

			// right point
			interfaces::Surface->DrawLine(p1.x, p1.y, p3.x, p3.y);
			break;
		}

		default:
			break;
		}
	}
}

float CAimbotProjectile::GetAimDrop(float flGravity, float flTimeSeconds)
{
	return flGravity * flTimeSeconds * flTimeSeconds;
}

void CAimbotProjectile::RunPath()
{
	if (!Settings::Aimbot.path || m_pTarget == nullptr || m_vecPath.empty())
		return;

	interfaces::Surface->DrawSetColor(255, 255, 255, 255);
	DrawPath(m_vecPath);
}

void CAimbotProjectile::DrawPath(const std::vector<Vector> &vPath)
{
	for (int i = 1; i < vPath.size(); i++)
	{
		Vec2 vecPrevScreen, vecCurrScreen;

		bool bIsPreviousVisible = helper::engine::WorldToScreen(vPath[i - 1], vecPrevScreen);
		bool bIsCurrentVisible	= helper::engine::WorldToScreen(vPath[i], vecCurrScreen);

		if (bIsPreviousVisible && bIsCurrentVisible)
			interfaces::Surface->DrawLine(vecPrevScreen.x, vecPrevScreen.y, vecCurrScreen.x,
						      vecCurrScreen.y);
	}
}

bool CAimbotProjectile::IsRightAttack(CTFWeaponBase *pWeapon)
{
	switch (pWeapon->GetWeaponID())
	{
	case TF_WEAPON_LUNCHBOX:
	case TF_WEAPON_BAT_WOOD:
	case TF_WEAPON_BAT_GIFTWRAP:
		return true;

	default:
		return false;
	}

	return false;
}

CAimbotProjectile gAimProjectile{};