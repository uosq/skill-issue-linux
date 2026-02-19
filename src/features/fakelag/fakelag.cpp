#include "fakelag.h"

//#include "../../settings/settings.h"
//#include "../ticks/ticks.h"

bool FakeLag::m_bFakeLagging = false;
int FakeLag::m_iChoked = 0;
int FakeLag::m_iGoal = 0;

void FakeLag::Run(CUserCmd *pCmd)
{
	/*if (!Settings::AntiAim::fakelag_enabled)
	{
		m_iChoked = 0;
		m_iGoal = 0;
		return;
	}

	m_bFakeLagging = false;

	if (TickManager::m_iChokedCommands >= 21)
	{
		m_iChoked = 0;
		m_iGoal = 0;
		return;
	}

	m_iGoal = Settings::AntiAim::fakelag_ticks;

	if (m_iChoked < m_iGoal)
	{
		m_iChoked++;
		TickManager::m_bSendPacket = false;
		m_bFakeLagging = true;
	}*/
}