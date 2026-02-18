#include "antiaim.h"
#include "../ticks/ticks.h"

namespace Antiaim
{
	std::string GetPitchModeName(PitchMode mode)
	{
		switch (Settings::AntiAim::pitch_mode)
		{
			case static_cast<int>(PitchMode::NONE): return "None";
			case static_cast<int>(PitchMode::UP): return "Up";
			case static_cast<int>(PitchMode::DOWN): return "Down";
			case static_cast<int>(PitchMode::FAKEUP): return "Fake Up";
			case static_cast<int>(PitchMode::FAKEDOWN): return "Fake Down";
			case static_cast<int>(PitchMode::RANDOM): return "Random";
		}

		return "unknown";
	}

	std::string GetYawModeName(YawMode mode)
	{
		switch (mode)
		{
			case YawMode::LEFT: return "Left";
			case YawMode::RIGHT: return "Right";
			case YawMode::BACK: return "Back";
			case YawMode::NONE: return "None";
			case YawMode::FORWARD: return "Forward";
			case YawMode::SPIN_LEFT: return "Spin left";
			case YawMode::SPIN_RIGHT: return "Spin Right";
			case YawMode::JITTER: return "Jitter";
		}

		return "unknown";
	}

	float GetPitch(CUserCmd* pCmd)
	{
		switch (static_cast<PitchMode>(Settings::AntiAim::pitch_mode))
		{
			case PitchMode::NONE: return pCmd->viewangles.x;
			case PitchMode::UP: return -89.0f;
			case PitchMode::DOWN: return 89.0f;
			case PitchMode::FAKEUP: return -271.0f;
			case PitchMode::FAKEDOWN: return 271.0f;
			case PitchMode::RANDOM: return (rand() % 180) * (rand() % 2 ? 1 : -1);
		}

		return 0;
	}

	float GetYaw(CUserCmd* pCmd, YawMode mode)
	{
		float spin_speed = Settings::AntiAim::spin_speed;

		switch (mode)
		{
			case YawMode::LEFT: return pCmd->viewangles.y + 90.0f;
			case YawMode::RIGHT: return pCmd->viewangles.y - 90.0f;
			case YawMode::BACK: return pCmd->viewangles.y - 180.0f;
			case YawMode::NONE: return pCmd->viewangles.y;
			case YawMode::FORWARD: return pCmd->viewangles.y;
			case YawMode::SPIN_LEFT: return pCmd->viewangles.y + (pCmd->tick_count % 360) * spin_speed;
			case YawMode::SPIN_RIGHT: return pCmd->viewangles.y - (pCmd->tick_count % 360) * spin_speed;
			case YawMode::JITTER: return pCmd->viewangles.y + ((pCmd->tick_count % 2) ? 90.0f : -90.0f);
		}

		return 0;
	}

	void Run(CTFPlayer* pLocal, CTFWeaponBase* pWeapon, CUserCmd* pCmd)
	{
		if (!Settings::AntiAim::enabled)
			return;

		if (!pLocal->IsAlive() || pLocal->IsGhost() || pLocal->IsTaunting())
			return;

		if (helper::localplayer::IsAttacking(pLocal, pWeapon, pCmd))
			return;

		constexpr int maxChoke = 2; // fake on 2 ticks, real on 1
		int choke = static_cast<CClientState*>(interfaces::ClientState)->chokedcommands;

		if (choke < maxChoke) TickManager::m_bSendPacket = false; // real
		else TickManager::m_bSendPacket = true; // fake

		bool isFake = TickManager::m_bSendPacket;
		
		if (Settings::AntiAim::pitch_mode != static_cast<int>(PitchMode::NONE))
		{
			float pitch = GetPitch(pCmd);
			pCmd->viewangles.x = pitch;
		}

		int mode = isFake ? Settings::AntiAim::fake_yaw_mode : Settings::AntiAim::real_yaw_mode;

		if (mode != static_cast<int>(YawMode::NONE))
			pCmd->viewangles.y = GetYaw(pCmd, static_cast<YawMode>(mode));
	}
}