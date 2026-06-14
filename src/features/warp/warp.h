#pragma once

#include "../../sdk/helpers/helper.h"

#include "../feature.h"
#include "../config/config.h"

BEGIN_CONFIG(warp)

	ADD_CONFIG(key, "warp key", Hotkey{})
	ADD_CONFIG(recharge_key, "warp recharge key", Hotkey{})
	ADD_CONFIG(speed, "warp speed", 0)

END_CONFIG()

enum class WarpState
{
	WAITING = 0,
	RUNNING,
	RECHARGING,
};

class Warp
{
public:
	bool IsValidWeapon(CTFWeaponBase *pWeapon);
	void Reset();
	int GetMaxTicks();
	void RunCreateMove(CTFPlayer *pLocal, CTFWeaponBase *pWeapon, CUserCmd *pCmd);
	void Run_CLMove();
	
	void DrawContents();
	void RunWindow();

	bool IsShifting();
	bool IsRecharging();

	int m_iStoredTicks{0};
	WarpState m_iDesiredState{WarpState::WAITING};
	bool m_bShifting{false};
	bool m_bRecharging{false};
	int m_iShiftAmount{0};
};

DECLARE_FEATURE(Warp, warp)