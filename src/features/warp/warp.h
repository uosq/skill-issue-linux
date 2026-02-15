#pragma once
#include "../../sdk/helpers/helper.h"

enum class WarpState
{
	WAITING = 0,
	RUNNING,
	RECHARGING,
	DT
};

namespace Warp
{
	extern int m_iStoredTicks;
	extern WarpState m_iDesiredState;
	extern bool m_bShifting;
	extern bool m_bRecharging;
	extern int m_iShiftAmount;

	bool IsValidWeapon(CTFWeaponBase* pWeapon);
	void Reset();
	int GetMaxTicks();
	void RunCreateMove(CTFPlayer* pLocal, CTFWeaponBase* pWeapon, CUserCmd* pCmd);
	void Run_CLMove();

	void DrawContents();
	void RunWindow();
}