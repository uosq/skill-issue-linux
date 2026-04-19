#include "aimbot.h"

#include "../binds/binds.h"
#include "../ticks/ticks.h"
#include "utils/utils.h"

#include "projectile/projectile.h"

namespace Aimbot
{
	AimbotState m_state{};

	Vector GetAngle()
	{
		return m_state.angle;
	}

	bool IsRunning()
	{
		return m_state.running;
	}

	bool ShouldSilent()
	{
		return m_state.shouldSilent;
	}

	void Run(CTFPlayer *pLocal, CTFWeaponBase *pWeapon, CUserCmd *pCmd)
	{
		ClearAimbotState(m_state);

		if (helper::engine::IsConsoleVisible() || helper::engine::IsGameUIVisible() ||
		    helper::engine::IsTakingScreenshot())
			return;

		switch (pWeapon->GetWeaponType())
		{
		case EWeaponType::HITSCAN:
			AimbotHitscan::Run(pLocal, pWeapon, pCmd, m_state);
			break;

		case EWeaponType::PROJECTILE:
		{
			gAimProjectile.RunMain(pLocal, pWeapon);
			gAimProjectile.RunAim(pLocal, pWeapon, pCmd, m_state);

			if (m_state.shouldSilent)
				TickManager::m_bSendPacket = false;

			break;
		}

		case EWeaponType::MELEE:
		{
			AimbotMelee::Run(pLocal, pWeapon, pCmd, m_state);

			if (m_state.shouldSilent)
				TickManager::m_bSendPacket = false;

			break;
		}
		default:
			break;
		}
	}

	void DrawFOVIndicator()
	{
		if (Settings::Aimbot.fov >= 90 || !Settings::Aimbot.draw_fov_indicator)
			return;

		ImDrawList* pDraw = ImGui::GetBackgroundDrawList();
		if (pDraw == nullptr)
			return;

		// float aimFov = DEG2RAD(Settings::Aimbot.fov);
		float aimFov = DEG2RAD(AimbotUtils::GetAimbotFovScaled());
		float camFov = DEG2RAD(CustomFov::GetFov() * 0.5f);

		int w, h;
		helper::draw::GetScreenSize(w, h);

		// Unfortunately my old formula was bad, like very bad and
		// wrong I got this one from Amalgam and it SEEMS to be working
		// fine, but I still need to do more tests
		float radius = tanf(aimFov) / tanf(camFov) * (float)(w) * (4.f / 6.f) / (16.f / 9.f);

		pDraw->AddCircle(ImVec2(w * 0.5f, h * 0.5f), radius, IM_COL32(255, 255, 255, 255));
	}

	void RunPaint()
	{
		if (!Settings::Aimbot.key->IsEnabled())
			return;

		//DrawFOVIndicator();

		gAimProjectile.RunPath();
		gAimProjectile.RunIndicator();
	}
}; // namespace Aimbot