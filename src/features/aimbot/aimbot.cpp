#include "aimbot.h"

#include "../binds/binds.h"
#include "../ticks/ticks.h"
#include "melee/aimbot_melee.h"
#include "utils/utils.h"

#include "projectile/projectile.h"

static void DrawFOVIndicator(ImDrawList* pDraw)
{
	if (Config.aimbot.fov >= 90 || !Config.aimbot.packed.draw_fov_indicator)
		return;

	float aimFov = DEG2RAD(AimbotUtils::GetAimbotFovScaled());
	float camFov = DEG2RAD(features::customfov.GetFov() * 0.5f);

	int w, h;
	helper::draw::GetScreenSize(w, h);

	// Unfortunately my old formula was bad, like very bad and
	// wrong I got this one from Amalgam and it SEEMS to be working
	// fine, but I still need to do more tests
	float radius = tanf(aimFov) / tanf(camFov) * (float)(w) * (4.f / 6.f) / (16.f / 9.f);

	pDraw->AddCircle(ImVec2(w * 0.5f, h * 0.5f), radius, IM_COL32(255, 255, 255, 255));
}

Vector Aimbot::GetAngle()
{
	return m_state.angle;
}

bool Aimbot::IsRunning()
{
	return m_state.running;
}

bool Aimbot::ShouldSilent()
{
	return m_state.shouldSilent;
}

void Aimbot::Run(CTFPlayer *pLocal, CTFWeaponBase *pWeapon, CUserCmd *pCmd)
{
	ClearAimbotState(m_state);

	if (helper::engine::IsConsoleVisible() || helper::engine::IsGameUIVisible() ||
	    helper::engine::IsTakingScreenshot())
		return;

	switch (pWeapon->GetWeaponType())
	{
	case EWeaponType::HITSCAN:
		m_hitscan.Run(pLocal, pWeapon, pCmd, m_state);
		break;

	case EWeaponType::PROJECTILE:
	{
		m_projectile.RunMain(pLocal, pWeapon);
		m_projectile.RunAim(pLocal, pWeapon, pCmd, m_state);

		if (m_state.shouldSilent)
			features::ticks.GetSendPacket() = false;

		break;
	}

	case EWeaponType::MELEE:
	{
		m_melee.Run(pLocal, pWeapon, pCmd, m_state);

		if (m_state.shouldSilent)
			features::ticks.GetSendPacket() = false;

		break;
	}
	default:
		break;
	}
}

void Aimbot::OnImGui(ImDrawList* pDraw)
{
	if (!Config.aimbot.key->IsEnabled())
		return;

	DrawFOVIndicator(pDraw);
	m_projectile.RunPath(pDraw);
	m_projectile.RunIndicator(pDraw);
}

AimbotState& Aimbot::GetState()
{
	return m_state;
}

void Aimbot::Reset()
{
	m_projectile.Reset();
	m_projectile.ResetIndicator();
}