#pragma once

#include "../../sdk/classes/entity.h"
#include "../../sdk/definitions/ctracefilters.h"
#include <vector>

struct PredictionResult
{
	std::vector<Vec3> m_vecPath;
	float m_flSeconds;

	const Vec3 &GetLastPosition();
};

class CPrediction
{
      public:
	CPrediction();
	~CPrediction() = default;

	void BeginPrediction(CTFPlayer *pEntity, float flTargetSeconds);
	void EndPrediction();

	bool Simulate(std::vector<Vector> &path);

      protected:
	float GetAirSpeedCap();
	float GetGravity();
	void BeginGravity();
	void EndGravity();
	void Friction();
	bool IsOnGround();
	void StepMove(Vector &vecDestination, CGameTrace &trace);
	int TryPlayerMove(Vector *vecFirstDest = nullptr, CGameTrace *tFirstTrace = nullptr,
			  float flSlideMultiplier = 0.0f);
	void TryTouchGroundInQuadrants(const Vector &start, const Vector &end, unsigned int fMask, ITraceFilter &filter,
				       CGameTrace &pm);
	void TryTouchGround(const Vector &start, const Vector &end, const Vector &mins, const Vector &maxs,
			    unsigned int fMask, ITraceFilter &filter, CGameTrace &pm);
	void StayOnGround();
	int ClipVelocity(Vector &in, Vector &normal, Vector &out, float overbounce, float flRedirectCoeff);
	void TracePlayerBBox(const Vector &start, const Vector &end, unsigned int fMask, ITraceFilter &filter,
			     CGameTrace &trace);
	void SetAbsOrigin(const Vector &in);
	bool CheckWater();
	Vector &GetAbsOrigin();

	void Accelerate(Vector &wishdir, float wishspeed, float accel);
	void AirAccelerate(Vector &wishdir, float wishspeed, float accel);
	void WalkMove();
	void AirMove();

      private:
	CTFPlayer *m_pTarget;

	Vector m_vecAbsOrigin;
	Vector m_vecVelocity;
	Vector m_vecBaseVelocity;
	Vector m_vecMaxs;
	Vector m_vecMins;
	Vector m_vecWishDir;

	float m_flGravity;
	float m_flAccelerate;
	float m_flStepSize;
	float m_flFriction;
	float m_flStopSpeed;
	float m_flBounce;
	float m_flTargetSeconds;
	float m_flMaxSpeed;
	float m_flAirSpeedCap;

	bool m_bAllowAutoMovement;
	bool m_bIsOnGround;
	bool m_bIsStarted;

	CTraceFilterWorldAndPropsOnly m_filter;
};

extern CPrediction gPrediction;