#include "bhop.h"

void Bhop::Reset()
{
	m_bUsedAirJump = false;
	m_bJumpReleased = false;
	m_bLastWantsJump = false;
	m_iJumps = 0;
}

void Bhop::Run(CTFPlayer *pLocal, CUserCmd *pCmd)
{
	if (!config::bhop::enabled.Get() || pLocal->GetWaterLevel() > WL_Feet)
		return;

	bool wantsJump = pCmd->buttons & IN_JUMP;
	bool jumpInThisTick = wantsJump && !m_bLastWantsJump;
	m_bLastWantsJump = wantsJump;

	bool isOnGround = pLocal->GetFlags() & FL_ONGROUND;
	bool isScout = pLocal->m_iClass() == TF_CLASS_SCOUT;

	if (isOnGround)
	{
		m_bUsedAirJump = false;
		m_bJumpReleased = false;

		if (wantsJump)
		{
			if (m_iJumps == 0 || config::bhop::perfect.Get() || (rand() % 101) <= config::bhop::maxchance.Get())
			{
				pCmd->buttons |= IN_JUMP;
				m_iJumps++;
			}
			else
			{
				pCmd->buttons &= ~IN_JUMP;
			}
		}
		else
		{
			m_iJumps = 0;
		}
	}
	else
	{
		if (!wantsJump)
			m_bJumpReleased = true;

		if (isScout && wantsJump && !m_bUsedAirJump && m_bJumpReleased)
		{
			pCmd->buttons |= IN_JUMP;
			m_bUsedAirJump = true;
		}
		else
		{
			pCmd->buttons &= ~IN_JUMP;
		}
	}
}
