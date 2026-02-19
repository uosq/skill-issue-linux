#include "fakelag.h"

#include "../../settings/settings.h"
#include "../ticks/ticks.h"

bool FakeLag::m_bFakeLagging = false;

void FakeLag::Run()
{
	m_bFakeLagging = false;

	if (!Settings::AntiAim::fakelag_enabled)
		return;

	if (TickManager::m_iChokedCommands >= 21)
		return;

	const uint8_t iGoal = Settings::AntiAim::fakelag_ticks;
	if (TickManager::m_iChokedCommands < iGoal)
	{
		TickManager::m_bSendPacket = false;
		m_bFakeLagging = true;
	}
}