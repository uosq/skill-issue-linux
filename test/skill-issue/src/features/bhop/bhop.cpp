#include "bhop.h"

void Bhop::Reset()
{
	m_bUsedAirJump	= false;
	m_bJumpReleased = false;
}

void Bhop::Run(CTFPlayer *pLocal, CUserCmd *pCmd)
{
	if (!Config.misc.packed.bhop || pLocal->GetWaterLevel() > WL_Feet)
		return;

	bool wantsJump	= pCmd->buttons & IN_JUMP;
	bool isOnGround = pLocal->GetFlags() & FL_ONGROUND;
	bool isScout	= pLocal->m_iClass() == TF_CLASS_SCOUT;

	if (isOnGround)
	{
		m_bUsedAirJump	= false;
		m_bJumpReleased = false;

		if (wantsJump)
			pCmd->buttons |= IN_JUMP;
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
