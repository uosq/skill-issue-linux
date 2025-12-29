# pragma once

#include "interfaces/interfaces.h"
#include "../hooks/clientmodeshared_createmove.h"
#include "../hooks/enginevgui_paint.h"
#include "definitions/types.h"
#include "math.h"

namespace SDK
{
	inline void FixMovement(CUserCmd* pCmd, const Vec3& vCurAngle, const Vec3& vTargetAngle)
	{
		bool bCurOOB = fabsf(Math::NormalizeAngle(vCurAngle.x)) > 90.f;
		bool bTargetOOB = fabsf(Math::NormalizeAngle(vTargetAngle.x)) > 90.f;

		Vec3 vMove = { pCmd->forwardmove, pCmd->sidemove * (bCurOOB ? -1 : 1), pCmd->upmove};
		float flSpeed = vMove.Length2D();
		Vec3 vMoveAng = Math::VectorAngles(vMove);

		float flCurYaw = vCurAngle.y + (bCurOOB ? 180.f : 0.f);
		float flTargetYaw = vTargetAngle.y + (bTargetOOB ? 180.f : 0.f);
		float flYaw = DEG2RAD(flTargetYaw - flCurYaw + vMoveAng.y);

		pCmd->forwardmove = cos(flYaw) * flSpeed;
		pCmd->sidemove = sin(flYaw) * flSpeed * (bTargetOOB ? -1 : 1);
	}

	inline void FixMovement(CUserCmd* pCmd, const Vec3& vTargetAngle)
	{
		FixMovement(pCmd, pCmd->viewangles, vTargetAngle);
	}
}