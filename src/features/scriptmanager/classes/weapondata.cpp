#include "../../../sol3/sol.hpp"

#include "../../../sdk/definitions/weaponinfo.h"

void BindWeaponData(sol::state& lua)
{
	lua.new_usertype<WeaponData_t>
	(
		"WeaponData", sol::no_constructor,

		"damage", &WeaponData_t::m_nDamage,
		"bullets_per_shot", &WeaponData_t::m_nBulletsPerShot,
		"range", &WeaponData_t::m_flRange,
		"spread", &WeaponData_t::m_flSpread,
		"punchangle", &WeaponData_t::m_flPunchAngle,
		"time_fire_delay", &WeaponData_t::m_flTimeFireDelay,
		"time_idle", &WeaponData_t::m_flTimeIdle,
		"time_idle_empty", &WeaponData_t::m_flTimeIdleEmpty,
		"time_reload_start", &WeaponData_t::m_flTimeReloadStart,
		"time_reload", &WeaponData_t::m_flTimeReload,
		"draw_crosshair", &WeaponData_t::m_bDrawCrosshair,
		"projectile", &WeaponData_t::m_iProjectile,
		"ammo_per_shot", &WeaponData_t::m_iAmmoPerShot,
		"projectile_speed", &WeaponData_t::m_flProjectileSpeed,
		"smack_delay", &WeaponData_t::m_flSmackDelay,
		"uses_rapid_fire_crits", &WeaponData_t::m_bUseRapidFireCrits
	);
}