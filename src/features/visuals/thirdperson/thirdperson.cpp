#include "thirdperson.h"

#include "../../../sdk/definitions/cgametrace.h"
#include "../../../sdk/definitions/ctracefilters.h"

#define CAM_HULL_OFFSET 14.0f
#define CAM_UP_OFFSET 25.0f

// Call in FrameStageNotify -> FRAME_NET_UPDATE_END
void Thirdperson::FrameStageNotify(CTFPlayer *pLocal)
{
	if (!pLocal->IsAlive())
		return interfaces::CInput->CAM_ToFirstPerson();

	bool bState = IsThirdPerson(pLocal);
	if (bState)
		interfaces::CInput->CAM_ToThirdPerson();
	else
		interfaces::CInput->CAM_ToFirstPerson();

	pLocal->ThirdPersonSwitch(bState);
}

void Thirdperson::OverrideView(CTFPlayer *pLocal, CViewSetup *pView)
{
	if (!pLocal->IsAlive())
		return interfaces::CInput->CAM_ToFirstPerson();

	bool bState = IsThirdPerson(pLocal);
	if (bState)
		interfaces::CInput->CAM_ToThirdPerson();
	else
		interfaces::CInput->CAM_ToFirstPerson();

	pLocal->ThirdPersonSwitch(bState);

	if (interfaces::CInput->CAM_IsThirdPerson())
	{
		bool bActive	     = Settings::Misc.thirdperson_key->IsActive();
		Vector vecViewAngles = pView->angles;
		Vector vecCamOffset  = GetCameraOffset();

		Vector vecForward, vecRight, vecUp;
		Math::AngleVectors(vecViewAngles, &vecForward, &vecRight, &vecUp);

		float flScale	 = bActive ? Settings::Misc.thirdperson_offset[3] : 1.0f;

		Vector vecOrigin = pView->origin;

		CGameTrace trace{};
		CTraceFilterWorldAndPropsOnly filter{};
		filter.pSkip  = pLocal;

		Vector vecEnd = pLocal->GetEyePos() + (vecForward * vecCamOffset.x * flScale) +
				(vecRight * vecCamOffset.y * flScale) + (vecUp * vecCamOffset.z * flScale);

		Vector vecHullMin, vecHullMax;
		vecHullMin.Set(-CAM_HULL_OFFSET, -CAM_HULL_OFFSET, -CAM_HULL_OFFSET);
		vecHullMax.Set(CAM_HULL_OFFSET, CAM_HULL_OFFSET, CAM_HULL_OFFSET);

		helper::engine::TraceHull(pView->origin, vecEnd, vecHullMin, vecHullMax, MASK_SHOT_HULL, &filter,
					  &trace);

		vecEnd	      = trace.endpos;

		pView->origin = vecEnd;
	}
}

bool Thirdperson::ShouldIgnoreBind(CTFPlayer *pLocal)
{
	return pLocal->IsTaunting() || pLocal->IsGhost() || pLocal->InCond(TF_COND_HALLOWEEN_KART) ||
	       (pLocal->InCond(TF_COND_STUNNED) && (pLocal->m_iStunFlags() & (TF_STUN_CONTROLS | TF_STUN_LOSER_STATE)));
}

bool Thirdperson::IsThirdPerson(CTFPlayer *pLocal)
{
	return Settings::Misc.thirdperson_key->IsActive() || ShouldIgnoreBind(pLocal);
}

Vector Thirdperson::GetCameraOffset()
{
	if (Settings::Misc.thirdperson_key->IsActive())
		return {Settings::Misc.thirdperson_offset[0], Settings::Misc.thirdperson_offset[1],
			Settings::Misc.thirdperson_offset[2]};
	else
		return {-150.0f, 0, CAM_UP_OFFSET};
}