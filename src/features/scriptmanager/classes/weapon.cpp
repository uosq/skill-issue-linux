#include "../../../sol3/sol.hpp"

#include "../../../sdk/classes/weaponbase.h"

void BindWeapon(sol::state& lua)
{
	lua.new_usertype<CTFWeaponBase>
	(
		"Player", sol::no_constructor,

		sol::base_classes, sol::bases<CBaseEntity>(),

		"GetID", &CTFWeaponBase::GetWeaponID,
		"IsInReload", &CTFWeaponBase::IsInReload,
		"CanPrimaryAttack", &CTFWeaponBase::CanPrimaryAttack,
		"CanSecondaryAttack", &CTFWeaponBase::CanSecondaryAttack,
		"IsMelee", &CTFWeaponBase::IsMelee,
		"IsHitscan", &CTFWeaponBase::IsHitscan,
		"CanHitTeammates", &CTFWeaponBase::CanHitTeammates,
		"GetDeflectionSize", &CTFWeaponBase::GetDeflectionSize,
		"GetSwingRange", &CTFWeaponBase::GetSwingRange,

		"GetType", [](CTFWeaponBase* self)
		{
			return static_cast<int>(self->GetWeaponType());
		},

		"GetSmackTime", [](CTFWeaponBase* self) -> float
		{
			return self->m_flSmackTime();
		},

		"GetChargeBeginTime", [](CTFWeaponBase* self) -> float
		{
			int weaponID = self->GetWeaponID();

			switch(weaponID)
			{
				case TF_WEAPON_PIPEBOMBLAUNCHER:
				return static_cast<CTFPipebombLauncher*>(self)->m_flChargeBeginTime();

				case TF_WEAPON_PARTICLE_CANNON:
				return static_cast<CTFParticleCannon*>(self)->m_flChargeBeginTime();

				case TF_WEAPON_CLEAVER:
				case TF_WEAPON_BAT_GIFTWRAP:
				case TF_WEAPON_BAT_WOOD:
				case TF_WEAPON_JAR:
				case TF_WEAPON_JAR_MILK:
				case TF_WEAPON_THROWABLE:
				return static_cast<CTFThrowable*>(self)->m_flChargeBeginTime();

				default:
				break;
			}

			return 0;
		},

		"GetData", [](CTFWeaponBase* self) -> const WeaponData_t&
		{
			return self->GetWeaponData();
		},

		"GetMode", [](CTFWeaponBase* self) -> int
		{
			return self->m_iWeaponMode();
		}
	);
}