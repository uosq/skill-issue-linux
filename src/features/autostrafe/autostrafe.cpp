#include "autostrafe.h"

float GetAirAccelerate()
{
	static ConVar *sv_airaccelerate = interfaces::Cvar->FindVar("sv_airaccelerate");
	return sv_airaccelerate->GetFloat();
}

float GetMaxSpeed()
{
	static ConVar *sv_maxspeed = interfaces::Cvar->FindVar("sv_maxspeed");
	return sv_maxspeed->GetFloat();
}

float GetForwardSpeed()
{
	static ConVar *cl_forwardspeed = interfaces::Cvar->FindVar("cl_forwardspeed");
	return cl_forwardspeed->GetFloat();
}

float GetSideSpeed()
{
	static ConVar *cl_sidespeed = interfaces::Cvar->FindVar("cl_sidespeed");
	return cl_sidespeed->GetFloat();
}

namespace Autostrafe
{
	void Run(CTFPlayer *pLocal, CUserCmd *pCmd)
	{
		if (!Settings::Misc.autostrafe || pLocal->GetWaterLevel() > WL_Feet)
			return;

		bool bIsOnGround = pLocal->GetFlags() & FL_ONGROUND;
		bool bIsOnWater	 = pLocal->GetFlags() & FL_INWATER;
		bool bIsScout	 = pLocal->m_iClass() == TF_CLASS_SCOUT;

		/* could not manage to make it work nicely on scout */
		if (bIsOnGround || bIsOnWater || bIsScout)
			return;

		Vec3 vecVelocity    = pLocal->GetVelocity();
		const float flSpeed = vecVelocity.Length();

		if (flSpeed < 30)
			return;

		float flAirAccelerate = GetAirAccelerate();
		float flMaxSpeed      = GetMaxSpeed();

		float flForwardSpeed  = GetForwardSpeed();
		float flSideSpeed     = GetSideSpeed();

		float flTerminal      = flAirAccelerate / flMaxSpeed * 100.0f / flSpeed;

		if (flTerminal < -1 || flTerminal > 1)
			return;

		float flGoodDelta = acosf(flTerminal);

		Vec3 vecViewAngles;
		interfaces::Engine->GetViewAngles(vecViewAngles);

		float flYaw	      = DEG2RAD(vecViewAngles.y);
		float flVelDirection  = atan2f(vecVelocity.y, vecVelocity.x) - flYaw;
		float flTargetAngle   = atan2f(-pCmd->sidemove, pCmd->forwardmove);
		float flDelta	      = Math::NormalizeRad(flVelDirection - flTargetAngle);

		float flMoveDirection = flDelta < 0 ? flVelDirection + flGoodDelta : flVelDirection - flGoodDelta;

		pCmd->forwardmove     = cosf(flMoveDirection) * flForwardSpeed;
		pCmd->sidemove	      = -sinf(flMoveDirection) * flSideSpeed;
	}
} // namespace Autostrafe
