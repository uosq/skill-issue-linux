#include "aimbot.h"
#include "utils/utils.h"
//#include "../visuals/customfov/customfov.h"
#include "../ticks/ticks.h"

namespace Aimbot
{
	AimbotState m_state = {};

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

	void Run(CTFPlayer* pLocal, CTFWeaponBase* pWeapon, CUserCmd* pCmd)
	{
		ClearAimbotState(m_state);

		if (!Settings::Aimbot::enabled)
			return;

		if (helper::engine::IsConsoleVisible() || helper::engine::IsGameUIVisible() || helper::engine::IsTakingScreenshot())
			return;

		ButtonCode_t key = interfaces::InputSystem->StringToButtonCode(Settings::Aimbot::key.c_str());

		if (key != BUTTON_CODE_INVALID && !interfaces::InputSystem->IsButtonDown(key))
			return;

		switch (pWeapon->GetWeaponType())
		{
			case EWeaponType::HITSCAN:
			{
				AimbotHitscan::Run(pLocal, pWeapon, pCmd, m_state);
			} break;
			
			case EWeaponType::PROJECTILE:
			{
				AimbotProjectile::Run(pLocal, pWeapon, pCmd, m_state);

				if (m_state.shouldSilent)
					TickManager::m_bSendPacket = false;
			} break;

			case EWeaponType::MELEE:
			{
				AimbotMelee::Run(pLocal, pWeapon, pCmd, m_state);

				if (m_state.shouldSilent)
					TickManager::m_bSendPacket = false;
			} break;
			default: break;
		}
	}

	void DrawFOVIndicator()
	{
		if (Settings::Aimbot::fov >= 90 || !Settings::Aimbot::draw_fov_indicator)
			return;

		//float aimFov = DEG2RAD(Settings::Aimbot::fov);
		float aimFov = DEG2RAD(AimbotUtils::GetAimbotFovScaled());
		float camFov = DEG2RAD(CustomFov::GetFov() * 0.5f);

		int w, h;
		helper::draw::GetScreenSize(w, h);

		// Unfortunately my old formula was bad, like very bad and wrong
		// I got this one from Amalgam and it SEEMS to be working fine, but I still need to do more tests
		float radius = tanf(aimFov) / tanf(camFov) * (float)(w) * (4.f / 6.f) / (16.f / 9.f);

		helper::draw::SetColor(255, 255, 255, 255);
		interfaces::Surface->DrawOutlinedCircle((int)(w*0.5f), (int)(h*0.5f), (int)(radius), 64);
	}

	void CleanTargetPath()
	{
		/*static float lastcleartime = 0.0f;
		if (state.targetPath.empty())
			return;

		if (interfaces::GlobalVars && (interfaces::GlobalVars->realtime - lastcleartime) < 5.0f)
			return;

		state.targetPath.clear();
		lastcleartime = interfaces::GlobalVars->realtime;
		*/
	}

	void DrawTargetPath()
	{
		/*
		//CleanTargetPath();

		if (state.targetPath.size() < 2)
			return;

		Vector prevScreen;
		if (!helper::engine::WorldToScreen(state.targetPath[0], prevScreen))
			return;

		helper::draw::SetColor({255, 255, 255, 255});

		for (size_t i = 1; i < state.targetPath.size(); i++)
		{
			Vector currScreen;
			if (!helper::engine::WorldToScreen(state.targetPath[i], currScreen))
				continue;

			interfaces::Surface->DrawLine(prevScreen.x, prevScreen.y, currScreen.x, currScreen.y);
			prevScreen = currScreen;
		}*/
	}

	void RunPaint()
	{
		if (!Settings::Aimbot::enabled)
			return;

		//DrawTargetPath();
		DrawFOVIndicator();
	}
};