#include "projectile.h"

#include <cmath>

#include "../../../sdk/imgui_utils/imgui_utils.h"

#include "../../logs/logs.h"
#include "../../prediction/prediction.h"
#include "../../../sdk/definitions/vphysics_interface.h"

#include "../aimbot.h"

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

	//float flDistance  = (vecTargetPos - vecStartPos).Length();
	//float flTotalTime = flDistance / prjInfo.speed;

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
	float flMaxTime = config::aimbot::max_sim_time.Get();

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

	bool bNoFovLimit = config::aimbot::fov.Get() >= 180.0f;

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

	#if 0
	if (m_pPhysEnv == nullptr)
	{
		m_pPhysEnv = interfaces::Physics->CreateEnvironment();
		m_pPhysEnv->SetGravity(Vec3(0, 0, -800));
		m_pPhysEnv->SetAirDensity(2.0f);
		m_pPhysEnv->ResetSimulationClock();
	}
	#endif

	if (!config::aimbot::key.Get().IsEnabled())
		return;

	bool bVisualsEnabled = config::aimbot::draw_predicted_player_path.Get() || config::aimbot::draw_predicted_player_indicator.Get();

	if (!bVisualsEnabled && !config::aimbot::key.Get().IsActive())
		return;

	static ConVar *sv_gravity = interfaces::Cvar->FindVar("sv_gravity");
	if (sv_gravity == nullptr)
		return features::logs.Error("[CAimbotProjectile::RunMain] sv_gravity is null!");

	if (pLocal == nullptr || pWeapon == nullptr)
		return;

	ProjectileInfo_t prjInfo{};
	if (!pWeapon->GetProjectileInfo(prjInfo))
		return;

	auto vTargets = GetBestTargets(pLocal, pWeapon);
	if (vTargets.empty())
		return;

	Vec3 vecEyePos;// = pLocal->GetEyePos();
	{
		std::vector<Vec3> vPath;
		features::prediction.BeginPrediction(pLocal, TICKS_TO_TIME(1));
		features::prediction.Simulate(vPath);
		features::prediction.EndPrediction();

		vecEyePos = vPath.back() + pLocal->m_vecViewOffset();
	}

	float flPrimeTime = pWeapon->GetWeaponID() == TF_WEAPON_PIPEBOMBLAUNCHER ? 0.7f : 0;

	for (const auto &target : vTargets)
	{
		if (target.entity == nullptr)
			continue;

		float flTime = target.distance / prjInfo.speed;
		if (flTime > config::aimbot::max_sim_time.Get())
			continue;

		flTime += flPrimeTime;
		flTime += TICKS_TO_TIME(1); // we are always 1 tick behind on CreateMove

		Vector vecAimPos{};
		std::vector<Vector> vPath;

		if (target.entity->IsPlayer())
		{
			CTFPlayer *pPlayer = static_cast<CTFPlayer *>(target.entity);

			// try to avoid reallocating a lot of memory
			vPath.reserve(TIME_TO_TICKS(flTime));

			features::prediction.BeginPrediction(pPlayer, flTime);
			features::prediction.Simulate(vPath);
			features::prediction.EndPrediction();

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
			{
				if (!FindVisiblePosToShoot(prjInfo, target.entity, vPath.back(), vecEyePos, 0.0f, vecAimPos))
					continue; // we are a failure

				vecDir = vecAimPos - vecEyePos;
				vecDir.Normalize();
				vecAngle = vecDir.ToAngle();
			}

			m_pTarget                   = target.entity;
			m_vecAimAngle               = vecAngle;
			m_vecAimPos                 = vecAimPos;
			m_vecPath                   = vPath;
			m_flTimeToTarget	    = flTime;
			features::entities.SetAimbotTarget(target.entity);
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
			{
				if (!FindVisiblePosToShoot(prjInfo, target.entity, vPath.back(), vecEyePos, flGravity, vecAimPos))
					continue; // we are fucking failures

				if (!SolveBallisticArc(vecAngle, vecEyePos, vecAimPos, prjInfo.speed, flGravity))
					continue; // math isn't mathing for the new aim position
			}

			m_pTarget                   = target.entity;
			m_vecAimAngle               = vecAngle;
			m_vecAimPos                 = vecAimPos;
			m_vecPath                   = vPath;
			features::entities.SetAimbotTarget(target.entity);
			return;
		}
	}
}

bool CAimbotProjectile::ApplyPlainAim(CTFPlayer* pLocal, CTFWeaponBase* pWeapon, CUserCmd* pCmd, AimbotState& pState)
{
	assert(pLocal && "pLocal is null");
	assert(pWeapon && "pWeapon is null");
	assert(pCmd && "pCmd is null");

	pState.running = true;
	pState.angle = m_vecAimAngle;
	pState.shouldSilent = false;

	pCmd->viewangles = m_vecAimAngle;
	interfaces::Engine->SetViewAngles(m_vecAimAngle);

	bool shooting = false;

	if (config::aimbot::autoshoot.Get())
		shooting = helper::localplayer::Shoot(pLocal, pWeapon, pCmd, m_pTarget);

	return shooting;
}

bool CAimbotProjectile::ApplySmoothAssistanceAim(CTFPlayer* pLocal, CTFWeaponBase* pWeapon, CUserCmd* pCmd, AimbotState& pState)
{
	if (config::aimbot::projectile_method.Get() == (int)AimbotMode::ASSISTANCE && pCmd->mousedx == 0 && pCmd->mousedy == 0)
		return false;

	Vec3 viewAngles; /* = */ interfaces::Engine->GetViewAngles(viewAngles);
	Vec3 smoothed = AimbotUtils::GetSmoothedAngle(viewAngles, m_vecAimAngle);

	pState.running = true;
	pState.angle = smoothed;
	pState.shouldSilent = false;

	pCmd->viewangles = smoothed;
	interfaces::Engine->SetViewAngles(smoothed);

	float fov = Math::CalcFov(smoothed, m_vecAimAngle);

	if (fov > 5.0f)
		return false;

	bool shooting = false;

	if (config::aimbot::autoshoot.Get())
		shooting = helper::localplayer::Shoot(pLocal, pWeapon, pCmd, m_pTarget);

	return shooting;
}

bool CAimbotProjectile::ApplySilentAim(CTFPlayer* pLocal, CTFWeaponBase* pWeapon, CUserCmd* pCmd, AimbotState& pState)
{
	pState.running = true;
	pState.angle = m_vecAimAngle;

	bool shooting = false;

	if (config::aimbot::autoshoot.Get())
		shooting = helper::localplayer::Shoot(pLocal, pWeapon, pCmd, m_pTarget);

	if (shooting || helper::localplayer::IsAttacking(pLocal, pWeapon, pCmd))
	{
		pCmd->viewangles = m_vecAimAngle;
		pState.shouldSilent = !IsRightAttack(pWeapon) && pWeapon->m_iItemDefinitionIndex() != Pyro_m_DragonsFury;
		return true;
	}

	return false;
}

bool CAimbotProjectile::ApplyAim(CTFPlayer* pLocal, CTFWeaponBase* pWeapon, CUserCmd* pCmd, AimbotState& pState)
{
	AimbotMode mode = static_cast<AimbotMode>(config::aimbot::projectile_method.Get());
	bool ret = false;

	switch(mode)
	{
        case AimbotMode::PLAIN:
	ret = ApplyPlainAim(pLocal, pWeapon, pCmd, pState);
	break;

        case AimbotMode::SMOOTH:
        case AimbotMode::ASSISTANCE:
	ret = ApplySmoothAssistanceAim(pLocal, pWeapon, pCmd, pState);
	break;

        case AimbotMode::SILENT:
	ret = ApplySilentAim(pLocal, pWeapon, pCmd, pState);
	break;

	case AimbotMode::INVALID:
        case AimbotMode::MAX:
        break;
        }

	return ret;
}

// i know i should just merge them in a single function
// but it makes the code's flow hard to follow
// simple isnt always better but in this case
// it fucking is

// normal weapon
// that is only click and shoot
void CAimbotProjectile::OnGenericWeapons(CTFPlayer* pLocal, CTFWeaponBase* pWeapon, CUserCmd* pCmd, AimbotState& pState)
{
	bool shooting = false;

	if (config::aimbot::autoshoot.Get())
		shooting = helper::localplayer::Shoot(pLocal, pWeapon, pCmd, m_pTarget);

	if (shooting || helper::localplayer::IsAttacking(pLocal, pWeapon, pCmd))
		ApplyAim(pLocal, pWeapon, pCmd, pState);
}

// weapons that can charge
// like sticky bomb launcher and huntsman
void CAimbotProjectile::OnChargeWeapons(CTFPlayer* pLocal, CTFWeaponBase* pWeapon, CUserCmd* pCmd, AimbotState& pState)
{
	bool autoshoot = config::aimbot::autoshoot.Get();
	bool shooting = false;

	if (autoshoot)
		shooting = helper::localplayer::Shoot(pLocal, pWeapon, pCmd, m_pTarget);

	if (shooting || helper::localplayer::IsAttacking(pLocal, pWeapon, pCmd))
		ApplyAim(pLocal, pWeapon, pCmd, pState);
}

// very basic and doesn't work right 100% of the time
void CAimbotProjectile::OnRightClickWeapons(CTFPlayer* pLocal, CTFWeaponBase* pWeapon, CUserCmd* pCmd, AimbotState& pState)
{
	if (config::aimbot::autoshoot.Get())
		pCmd->buttons |= IN_ATTACK2;

	if (pCmd->buttons & IN_ATTACK2)
	{
		ApplyAim(pLocal, pWeapon, pCmd, pState);
		pState.shouldSilent = false; // can't silent with them
	}
}

void CAimbotProjectile::RunAim(CTFPlayer *pLocal, CTFWeaponBase *pWeapon, CUserCmd *pCmd, AimbotState &pState)
{
	if (!config::aimbot::key.Get().IsActive())
		return;

	if (m_pTarget == nullptr || m_vecPath.empty())
		return;

	if (ApplyAim(pLocal, pWeapon, pCmd, pState))
		AimbotUtils::ShootCallback(pCmd, m_pTarget);
}

void CAimbotProjectile::ResetIndicator()
{
	m_vecOldIndicatorPos.Set(0, 0);
	m_pOldIndicatorTarget = nullptr;
}

void CAimbotProjectile::RunIndicator(ImDrawList* pDraw)
{
	if (!config::aimbot::key.Get().IsEnabled())
		return;

	if (interfaces::Engine->IsTakingScreenshot())
		return ResetIndicator();

	if (config::aimbot::draw_predicted_player_indicator.Get() == static_cast<int>(AimbotIndicatorStyle::NONE))
		return ResetIndicator();

	CTFPlayer *pLocal = features::entities.GetLocal();
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
		constexpr int iSIZE = 5;

		switch (static_cast<AimbotIndicatorStyle>(config::aimbot::draw_predicted_player_indicator.Get()))
		{
		case AimbotIndicatorStyle::NONE:
			break;

		case AimbotIndicatorStyle::CIRCLE:
		{
			pDraw->AddCircleFilled(ImVec2(screenPos.x, screenPos.y), (float)iSIZE, IM_COL32(255, 255, 255, 255));
			break;
		}
		case AimbotIndicatorStyle::SQUARE:
		{
			ImGui::DrawBoxFilled(pDraw, ImVec2(screenPos.x, screenPos.y), ImVec2(iSIZE*2.0f, iSIZE*2.0f), IM_COL32(255, 255, 255, 255), 2.0f);
			break;
		}
		case AimbotIndicatorStyle::TRIANGLE:
		{
			ImVec2 p1, p2, p3;
			p1 = {screenPos.x - iSIZE, screenPos.y + iSIZE};
			p2 = {screenPos.x, screenPos.y - iSIZE};
			p3 = {screenPos.x + iSIZE, screenPos.y + iSIZE};

			pDraw->AddTriangleFilled(p1, p2, p3, IM_COL32(255, 255, 255, 255));
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

void CAimbotProjectile::RunPath(ImDrawList* pDraw)
{
	if (!config::aimbot::draw_predicted_player_path.Get() || m_pTarget == nullptr || m_vecPath.empty())
		return;

	DrawPath(pDraw, m_vecPath);
}

void CAimbotProjectile::DrawPath(ImDrawList* pDraw, const std::vector<Vector> &vPath)
{
	for (size_t i = 1; i < vPath.size(); i++)
	{
		Vec2 vecPrevScreen, vecCurrScreen;

		bool bIsPreviousVisible = helper::engine::WorldToScreen(vPath[i - 1], vecPrevScreen);
		bool bIsCurrentVisible	= helper::engine::WorldToScreen(vPath[i], vecCurrScreen);

		if (bIsPreviousVisible && bIsCurrentVisible)
			pDraw->AddLine(ImVec2(vecPrevScreen.x, vecPrevScreen.y), ImVec2(vecCurrScreen.x, vecCurrScreen.y), IM_COL32(255, 255, 255, 255));
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

// this is not good
bool CAimbotProjectile::FindVisiblePosToShoot(const ProjectileInfo_t& prjInfo,
	CBaseEntity* pTarget, const Vec3& predictedPos, const Vec3& shootPos, float flGravity, Vec3& out)
{
	if (pTarget == nullptr)
		return false;

	// they are already in another country
	// no need to bother with them
	if (shootPos.DistTo(predictedPos) > 2000.0f)
		return false;

	Vec3 mins = pTarget->m_vecMins();
	Vec3 maxs = pTarget->m_vecMaxs();

	constexpr int GRID_POINTS = 4;

	float stepX = (maxs.x - mins.x) / (GRID_POINTS - 1);
	float stepY = (maxs.y - mins.y) / (GRID_POINTS - 1);
	float stepZ = (maxs.z - mins.z) / (GRID_POINTS - 1);

	constexpr float HITBOX_SCALE = 0.85f;

	for (int x = 0; x < GRID_POINTS; x++)
	{
		for (int y = 0; y < GRID_POINTS; y++)
		{
			for (int z = 0; z < GRID_POINTS; z++)
			{
				Vec3 offset
				(
					mins.x + (stepX * x),
					mins.y + (stepY * y),
					mins.z + (stepZ * z)
				);

				offset *= HITBOX_SCALE;

				Vec3 testAimPos = predictedPos + offset;

				Vec3 vecAngle;

				if (!SolveBallisticArc(vecAngle, shootPos, testAimPos, prjInfo.speed, flGravity))
					continue;

				Vec3 vecForward, vecRight, vecUp;
				Math::AngleVectors(vecAngle, &vecForward, &vecRight, &vecUp);

				Vec3 vecSpawnPos = shootPos + vecForward * prjInfo.offset.x +
						   vecRight * prjInfo.offset.y +
						   vecUp * prjInfo.offset.z;

				if (CheckTrajectory(pTarget, vecSpawnPos, testAimPos, vecAngle, prjInfo, flGravity))
				{
					out = testAimPos;
					return true;
				}
			}
		}
	}

	// shit
	return false;
}
