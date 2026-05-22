#pragma once

#include "../definitions/weaponinfo.h"

#include "../defs.h"
#include "../handle_utils.h"

#include "basecombatcharacter.h"
#include "basecombatweapon.h"
#include "entity.h"

class CTFPlayer;
class CTFGrenadePipebombProjectile;

// source: https://github.com/rei-2/Amalgam/blob/bffae9999cf35a5fbdeb92387b9fae58796b8939/Amalgam/src/SDK/Definitions/Main/CTFWeaponBase.h
// yes, i am pasting a lot of amalgam
// do i care? no, as im not gonna write all ths shit from scratch

typedef unsigned short WEAPON_FILE_INFO_HANDLE;

struct ProjectileInfo_t
{
	float speed	    = 0;
	float gravity	    = 0;
	float primetime	    = 0;
	float damage_radius = 0;
	float lifetime	    = 60.0f;
	bool simple_trace   = false;
	Vector offset{};
	Vector hull{6, 6, 6};
};

struct BobState_t
{
	BobState_t()
	{
		m_flBobTime	= 0;
		m_flLastBobTime = 0;
		m_flLastSpeed	= 0;
		m_flVerticalBob = 0;
		m_flLateralBob	= 0;
	}

	float m_flBobTime;
	float m_flLastBobTime;
	float m_flLastSpeed;
	float m_flVerticalBob;
	float m_flLateralBob;
};

// xref: CHudCrosshair
// Almost all of the functions with 1 parameter is GetTFWpnData
// Then to get the ofset for the handle you do
// go inside GetTFWpnData
// it will call GetFileWeaponInfoFromHandle
// whatever it passes (this + 0xsomething), the 0x123 is the offset to the handle
inline FileWeaponInfo_t *original_GetFileWeaponInfoFromHandle(void *handle)
{
	using GetFileWeaponInfoFromHandleFn = FileWeaponInfo_t *(*)(void *);
	static auto orig = (GetFileWeaponInfoFromHandleFn)sigscan_module("client.so", "66 3B 3D ? ? ? ? 48 8D 05");
	return orig(handle);
}

class CTFWeaponBase : public CBaseCombatWeapon
{
public:
	NETVAR(m_bLowered, "CTFWeaponBase->m_bLowered", bool)
	NETVAR(m_iReloadMode, "CTFWeaponBase->m_iReloadMode", int)
	NETVAR(m_bResetParity, "CTFWeaponBase->m_bResetParity", bool)
	NETVAR(m_bReloadedThroughAnimEvent, "CTFWeaponBase->m_bReloadedThroughAnimEvent", bool)
	NETVAR(m_bDisguiseWeapon, "CTFWeaponBase->m_bDisguiseWeapon", bool)
	NETVAR(m_flLastCritCheckTime, "CTFWeaponBase->m_flLastCritCheckTime", float)
	NETVAR(m_flReloadPriorNextFire, "CTFWeaponBase->m_flReloadPriorNextFire", float)
	NETVAR(m_flLastFireTime, "CTFWeaponBase->m_flLastFireTime", float)
	NETVAR(m_flEffectBarRegenTime, "CTFWeaponBase->m_flEffectBarRegenTime", float)
	NETVAR(m_flObservedCritChance, "CTFWeaponBase->m_flObservedCritChance", float)
	NETVAR(m_flEnergy, "CTFWeaponBase->m_flEnergy", float)
	NETVAR(m_hExtraWearable, "CTFWeaponBase->m_hExtraWearable", EHANDLE)
	NETVAR(m_hExtraWearableViewModel, "CTFWeaponBase->m_hExtraWearableViewModel", EHANDLE)
	NETVAR(m_bBeingRepurposedForTaunt, "CTFWeaponBase->m_bBeingRepurposedForTaunt", bool)
	NETVAR(m_nKillComboClass, "CTFWeaponBase->m_nKillComboClass", int)
	NETVAR(m_nKillComboCount, "CTFWeaponBase->m_nKillComboCount", int)
	NETVAR(m_flInspectAnimEndTime, "CTFWeaponBase->m_flInspectAnimEndTime", float)
	NETVAR(m_nInspectStage, "CTFWeaponBase->m_nInspectStage", int)
	NETVAR(m_iConsecutiveShots, "CTFWeaponBase->m_iConsecutiveShots", int)

	NETVAR_OFFSET(m_flSmackTime, "CTFWeaponBase->m_nInspectStage", float, 28);
	NETVAR_OFFSET(m_flCritTokenBucket, "CTFWeaponBase->m_iReloadMode", float, -244);
	NETVAR_OFFSET(m_nCritChecks, "CTFWeaponBase->m_iReloadMode", int, -240);
	NETVAR_OFFSET(m_nCritSeedRequests, "CTFWeaponBase->m_iReloadMode", int, -236);
	NETVAR_OFFSET(m_iWeaponMode, "CTFWeaponBase->m_iReloadMode", int, -4);
	NETVAR_OFFSET(m_flCritTime, "CTFWeaponBase->m_flLastCritCheckTime", float, -4);
	NETVAR_OFFSET(m_iCurrentSeed, "CTFWeaponBase->m_flLastCritCheckTime", int, 8);
	NETVAR_OFFSET(m_flLastRapidFireCritCheckTime, "CTFWeaponBase->m_flLastCritCheckTime", float, 12);

	CTFWeaponInfo* m_pWeaponInfo();

	const WeaponData_t& GetWeaponData();
	int GetWeaponID();

	bool IsInReload();
	int GetSlot();

	EWeaponType GetWeaponType();
	Vector GetDeflectionSize();
	float GetSmackDelay();

	bool IsHitscan();
	bool IsMelee();
	bool IsSniperRifle();
	bool IsAmbassador();

	bool CanPrimaryAttack();
	bool CanSecondaryAttack();
	bool CanAmbassadorHeadshot();
	bool CanHitTeammates();
	bool CanAirblast();

	bool HasPrimaryAmmoForShot();

	bool DoSwingTrace(CGameTrace& trace);
	float GetSwingRange();

	bool GetProjectileInfo(ProjectileInfo_t &pOut);
};

class CTFKnife : public CTFWeaponBase
{
public:
	NETVAR(m_bReadyToBackstab, "CTFKnife->m_bReadyToBackstab", bool);
	NETVAR(m_bKnifeExists, "CTFKnife->m_bKnifeExists", bool);
	NETVAR(m_flKnifeRegenerateDuration, "CTFKnife->m_flKnifeRegenerateDuration", float);
	NETVAR(m_flKnifeMeltTimestamp, "CTFKnife->m_flKnifeMeltTimestamp", float);
};

class CTFMinigun : public CTFWeaponBase
{
public:
	NETVAR(m_iWeaponState, "CTFMinigun->m_iWeaponState", int);
	NETVAR(m_bCritShot, "CTFMinigun->m_bCritShot", bool);
};

class CWeaponMedigun : public CTFWeaponBase
{
public:
	NETVAR(m_hHealingTarget, "CWeaponMedigun->m_hHealingTarget", EHANDLE);
	NETVAR(m_bHealing, "CWeaponMedigun->m_bHealing", bool);
	NETVAR(m_bAttacking, "CWeaponMedigun->m_bAttacking", bool);
	NETVAR(m_bChargeRelease, "CWeaponMedigun->m_bChargeRelease", bool);
	NETVAR(m_bHolstered, "CWeaponMedigun->m_bHolstered", bool);
	NETVAR(m_nChargeResistType, "CWeaponMedigun->m_nChargeResistType", int);
	NETVAR(m_hLastHealingTarget, "CWeaponMedigun->m_hLastHealingTarget", EHANDLE);
	NETVAR(m_flChargeLevel, "CWeaponMedigun->m_flChargeLevel", float);

	//int GetMedigunType();
	//MedigunChargeTypes GetChargeType();
	//medigun_resist_types_t GetResistType();
};

class CTFPipebombLauncher : public CTFWeaponBase
{
public:
	NETVAR(m_iPipebombCount, "CTFPipebombLauncher->m_iPipebombCount", int);
	NETVAR(m_flChargeBeginTime, "CTFPipebombLauncher->m_flChargeBeginTime", float);
	//int GetDetonateType();
};

class CTFSniperRifle : public CTFWeaponBase
{
public:
	NETVAR(m_flChargedDamage, "CTFSniperRifle->m_flChargedDamage", float);

	float GetChargedDamage()
	{
		bool isMachina = m_iItemDefinitionIndex() == Sniper_m_TheMachina;
		float mult     = isMachina ? 1.15f : 1.0f;
		return m_flChargedDamage() * 3 * mult;
	}
};

class CTFGrenadeLauncher : public CTFWeaponBase
{
public:
	NETVAR(m_flDetonateTime, "CTFGrenadeLauncher->m_flDetonateTime", float);
	NETVAR(m_iCurrentTube, "CTFGrenadeLauncher->m_iCurrentTube", int);
	NETVAR(m_iGoalTube, "CTFGrenadeLauncher->m_iGoalTube", int);

	//int GetDetonateType();
};

class CTFSniperRifleClassic : public CTFSniperRifle
{
public:
	NETVAR(m_bCharging, "CTFSniperRifleClassic->m_bCharging", bool);
};

class CTFParticleCannon : public CTFWeaponBase
{
public:
	NETVAR(m_flChargeBeginTime, "CTFParticleCannon->m_flChargeBeginTime", float);
	NETVAR(m_iChargeEffect, "CTFParticleCannon->m_iChargeEffect", int);
};

class CTFFlareGun : public CTFWeaponBase
{
public:
	//int GetFlareGunType();
};

class CTFThrowable : public CTFWeaponBase
{
public:
	NETVAR(m_flChargeBeginTime, "CTFThrowable->m_flChargeBeginTime", float);
};

class CTFGrapplingHook : public CTFWeaponBase
{
public:
	NETVAR(m_hProjectile, "CTFGrapplingHook->m_hProjectile", EHANDLE);
};

class CTFSpellBook : public CTFThrowable
{
public:
	NETVAR(m_flTimeNextSpell, "CTFSpellBook->m_flTimeNextSpell", float);
	NETVAR(m_iSelectedSpellIndex, "CTFSpellBook->m_iSelectedSpellIndex", int);
	NETVAR(m_iSpellCharges, "CTFSpellBook->m_iSpellCharges", int);
	NETVAR(m_bFiredAttack, "CTFSpellBook->m_bFiredAttack", bool);
};