#pragma once

#include "../../../sdk/classes/player.h"
#include "../../../sdk/classes/weaponbase.h"

#include "../utils/utils.h"

class CAimbotProjectile
{
      public:
	~CAimbotProjectile() = default;
	CAimbotProjectile();

	void RunMain(CTFPlayer *pLocal, CTFWeaponBase *pWeapon);
	void RunAim(CTFPlayer *pLocal, CTFWeaponBase *pWeapon, CUserCmd *pCmd, AimbotState &pState);
	void RunIndicator();
	void RunPath();

	void Reset();
	void ResetIndicator();

	std::vector<Vector> &GetPath();
	CBaseEntity *GetCurrentTarget();

	bool GetProjectileInfo(ProjectileInfo_t &pOut, CTFPlayer *pLocal, CTFWeaponBase *pWeapon);

      protected:
	float GetInitialZOffset(CTFWeaponBase *pWeapon, const Vector &vecMaxs);
	bool SolveBallisticArc(Vector &outAngle, const Vector p0, const Vector p1, float flSpeed, float flGravity);
	bool CheckTrajectory(CBaseEntity *pTarget, const Vector vecStartPos, const Vector vecTargetPos,
			     const Vector vecAngle, const ProjectileInfo_t &prjInfo, float flGravity);
	std::vector<PotentialTarget> GetBestTargets(CTFPlayer *pLocal, CTFWeaponBase *pWeapon);
	float GetAimDrop(float flGravity, float flTimeSeconds);
	void DrawPath(const std::vector<Vector>& vPath);
	bool IsRightAttack(CTFWeaponBase *pWeapon);

      private:
	std::vector<Vector> m_vecPath;
	CBaseEntity *m_pTarget;
	Vector m_vecAimAngle;
	Vector m_vecAimPos;

	Vector m_vecOldIndicatorPos;
	CBaseEntity *m_pOldIndicatorTarget;
	// IPhysicsEnvironment* m_pPhysEnv;
};

extern CAimbotProjectile gAimProjectile;