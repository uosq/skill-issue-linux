#include "../../../sol3/sol.hpp"

#include "../../../sdk/definitions/weaponinfo.h"

void BindWeaponData(sol::state& lua)
{
	lua.new_usertype<WeaponData_t>
	(
		"WeaponInfo", sol::no_constructor,

		"m_nDamage", &WeaponData_t::m_nDamage,
		"m_nBulletsPerShot", &WeaponData_t::m_nBulletsPerShot,
		"m_flRange", &WeaponData_t::m_flRange,
		"m_flSpread", &WeaponData_t::m_flSpread,
		"m_flPunchAngle", &WeaponData_t::m_flPunchAngle,
		"m_flTimeFireDelay", &WeaponData_t::m_flTimeFireDelay,
		"m_flTimeIdle", &WeaponData_t::m_flTimeIdle,
		"m_flTimeIdleEmpty", &WeaponData_t::m_flTimeIdleEmpty,
		"m_flTimeReloadStart", &WeaponData_t::m_flTimeReloadStart,
		"m_flTimeReload", &WeaponData_t::m_flTimeReload,
		"m_bDrawCrosshair", &WeaponData_t::m_bDrawCrosshair,
		"m_iProjectile", &WeaponData_t::m_iProjectile,
		"m_iAmmoPerShot", &WeaponData_t::m_iAmmoPerShot,
		"m_flProjectileSpeed", &WeaponData_t::m_flProjectileSpeed,
		"m_flSmackDelay", &WeaponData_t::m_flSmackDelay,
		"m_bUseRapidFireCrits", &WeaponData_t::m_bUseRapidFireCrits
	);
}