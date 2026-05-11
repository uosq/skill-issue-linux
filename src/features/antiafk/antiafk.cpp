#include "antiafk.h"

#include "../../settings/settings.h"

#include "../../sdk/interfaces/interfaces.h"
#include "../../sdk/definitions/cusercmd.h"
#include "../../sdk/defs.h"

#include "../logs/logs.h"

void Antiafk::OnCreateMove(CUserCmd *pCmd)
{
	// mp_idlemaxtime is in minutes
	static ConVar* mp_idlemaxtime = interfaces::Cvar->FindVar("mp_idlemaxtime");
	if (mp_idlemaxtime == nullptr)
		return features::logs.Error("[AntiAFK::OnCreateMove] mp_idlemaxtime is null");

	if (!Config.misc.packed.antiafk)
		return;

	m_afkticks++;

	if (pCmd->buttons != 0 && pCmd->mousedx == 0 && pCmd->mousedy == 0)
		m_afkticks = 0;

	// the localplayer can modify
	// convars any time, gotta make
	// sure they don't make it 0 as
	// it would force us to move forward
	// always
	float flMaxTime = std::max(mp_idlemaxtime->GetFloat() * 0.5f, 0.1f);

	if (TICKS_TO_TIME(m_afkticks) >= (flMaxTime * 60.0f))
	{
		pCmd->forwardmove = 450.0f;
		pCmd->buttons |= IN_FORWARD;
		m_afkticks = 0;
	}
}

void Antiafk::OnLevelShutdown()
{
	m_afkticks = 0;
}