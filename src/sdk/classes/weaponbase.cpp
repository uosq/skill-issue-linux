#include "weaponbase.h"

#include "player.h"

CTFWeaponInfo* CTFWeaponBase::m_pWeaponInfo()
{
	/*
	xref: ShotgunPunchAngle
	There is 2 functions, one has a if checking for > 0 and one that doesn't
	go to the one that doesn't have the if checking for > 0
	you'll go to CTFShotgun::UpdatePunchAngles
	you get the offset from
	fVar1 = *(float *)((long)*(int *)((long)this + 0xf70) * 0x40 + *(long *)((long)this + 0xf80) + 0x734);
	0xf80 is m_pWeaponInfo
	0xf70 is m_iWeaponMode
	GetWeaponData() is the 0x40
	m_flPunchAngle is 0x734
	*/

	return *reinterpret_cast<CTFWeaponInfo**>(uintptr_t(this) + 0xf80);
}

const WeaponData_t& CTFWeaponBase::GetWeaponData()
{
	return m_pWeaponInfo()->GetWeaponData(m_iWeaponMode());
}

int CTFWeaponBase::GetWeaponID()
{
	// xref: -use_action_slot_item_server
	// func: EndUseActionSlotItem
	/*
		we want the 0xe18 offset
====>  			iVar3 = (**(code **)(*plVar7 + 0xe18))(plVar7);
		  if (iVar3 != 0x65) goto LAB_01dc047d;
		  uVar5 = FUN_01fdca00(0x40);
		  FUN_01fda880(uVar5,"-use_action_slot_item_server");
	*/

	/*using Fn    = int (*)(void *);
	auto vtable = *reinterpret_cast<void ***>(this);
	auto fn	    = reinterpret_cast<Fn>(vtable[0xE18 / sizeof(void *)]);
	return fn(this);*/

	constexpr int index = 0xE18/sizeof(uintptr_t);
	return vtable_call<index, int>(this);
}

bool CTFWeaponBase::IsInReload()
{
	return m_bInReload() || m_iReloadMode() != 0;
}

int CTFWeaponBase::GetSlot()
{
	return m_pWeaponInfo()->iSlot;
}

EWeaponType CTFWeaponBase::GetWeaponType()
{
	if (GetSlot() == EWeaponSlot::SLOT_MELEE || GetWeaponID() == TF_WEAPON_BUILDER)
		return EWeaponType::MELEE;

	switch (m_iItemDefinitionIndex())
	{
	case Soldier_s_TheBuffBanner:
	case Soldier_s_FestiveBuffBanner:
	case Soldier_s_TheBattalionsBackup:
	case Soldier_s_TheConcheror:
	case Scout_s_BonkAtomicPunch:
	case Scout_s_CritaCola:
		return EWeaponType::UNKNOWN;
	}

	switch (GetWeaponID())
	{
	case TF_WEAPON_PDA:
	case TF_WEAPON_PDA_ENGINEER_BUILD:
	case TF_WEAPON_PDA_ENGINEER_DESTROY:
	case TF_WEAPON_PDA_SPY:
	case TF_WEAPON_PDA_SPY_BUILD:
	case TF_WEAPON_INVIS:
	case TF_WEAPON_BUFF_ITEM:
	case TF_WEAPON_GRAPPLINGHOOK:
	case TF_WEAPON_ROCKETPACK:
		return EWeaponType::UNKNOWN;

	case TF_WEAPON_CLEAVER:
	case TF_WEAPON_ROCKETLAUNCHER:
	case TF_WEAPON_ROCKETLAUNCHER_DIRECTHIT:
	case TF_WEAPON_PARTICLE_CANNON:
	case TF_WEAPON_RAYGUN:
	case TF_WEAPON_FLAMETHROWER:
	case TF_WEAPON_FLAME_BALL:
	case TF_WEAPON_FLAREGUN:
	case TF_WEAPON_FLAREGUN_REVENGE:
	case TF_WEAPON_GRENADELAUNCHER:
	case TF_WEAPON_CANNON:
	case TF_WEAPON_PIPEBOMBLAUNCHER:
	case TF_WEAPON_SHOTGUN_BUILDING_RESCUE:
	case TF_WEAPON_DRG_POMSON:
	case TF_WEAPON_CROSSBOW:
	case TF_WEAPON_SYRINGEGUN_MEDIC:
	case TF_WEAPON_COMPOUND_BOW:
	case TF_WEAPON_JAR:
	case TF_WEAPON_JAR_MILK:
	case TF_WEAPON_JAR_GAS:
	case TF_WEAPON_LUNCHBOX:
		return EWeaponType::PROJECTILE;
	}

	return EWeaponType::HITSCAN;
}

bool CTFWeaponBase::IsHitscan()
{
	return GetWeaponType() == EWeaponType::HITSCAN;
}

bool CTFWeaponBase::CanPrimaryAttack()
{
	CTFPlayer *owner = HandleAs<CTFPlayer *>(m_hOwnerEntity());
	if (!owner)
		return false;

	float curtime = TICKS_TO_TIME(owner->GetTickBase());
	return m_flNextPrimaryAttack() <= curtime && m_flNextAttack() <= curtime;
}

bool CTFWeaponBase::CanSecondaryAttack()
{
	CTFPlayer *owner = HandleAs<CTFPlayer *>(m_hOwnerEntity());
	if (!owner)
		return false;

	float curtime = TICKS_TO_TIME(owner->GetTickBase());
	return m_flNextSecondaryAttack() && m_flNextAttack() <= curtime;
}

bool CTFWeaponBase::HasPrimaryAmmoForShot()
{
	return m_iClip1() > 0 || m_iClip1() == -1;
}

bool CTFWeaponBase::IsMelee()
{
	return GetSlot() == SLOT_MELEE;
}

bool CTFWeaponBase::CanAmbassadorHeadshot()
{
	if (GetWeaponID() == TF_WEAPON_REVOLVER &&
	    AttributeHookValue(0, "set_weapon_mode", this, nullptr, true))
		return (interfaces::GlobalVars->curtime - m_flLastFireTime()) > 1.0f;
	return false;
}

bool CTFWeaponBase::IsSniperRifle()
{
	switch (GetClassID())
	{
	case ETFClassID::CTFSniperRifleClassic:
	case ETFClassID::CTFSniperRifle:
	case ETFClassID::CTFSniperRifleDecap:
		return true;

	default:
		break;
	}

	return false;
}

bool CTFWeaponBase::IsAmbassador()
{
	return GetClassID() == ETFClassID::CTFRevolver &&
	       (m_iItemDefinitionIndex() == Spy_m_FestiveAmbassador ||
		m_iItemDefinitionIndex() == Spy_m_TheAmbassador);
}

bool CTFWeaponBase::CanHitTeammates()
{
	switch (GetWeaponID())
	{
	case TF_WEAPON_CROSSBOW:
	case TF_WEAPON_LUNCHBOX:
	case TF_WEAPON_JAR_MILK:
	case TF_WEAPON_JAR:
	case TF_WEAPON_MEDIGUN:
	case TF_WEAPON_WRENCH:
		return true;

	default:
		break;
	}

	switch (m_iItemDefinitionIndex())
	{
	case Soldier_t_TheDisciplinaryAction:
	case Sniper_m_TheSydneySleeper:
		return true;

	default:
		break;
	}

	static ConVar *mp_friendlyfire = interfaces::Cvar->FindVar("mp_friendlyfire");
	if (mp_friendlyfire && mp_friendlyfire->GetBool())
		return true;

	return false;
}

bool CTFWeaponBase::CanAirblast()
{
	int iAirblastDisabled =
	    static_cast<int>(AttributeHookValue(0, "airblast_disabled", this, nullptr, true));
	bool bAllowed = iAirblastDisabled == 0;
	return bAllowed;
}

Vector CTFWeaponBase::GetDeflectionSize()
{
	return Vector(128, 128, 64);
}

float CTFWeaponBase::GetSmackDelay()
{
	float delay = GetWeaponData().m_flSmackDelay;
	return delay > 0 ? delay : 0.2f;
}

bool CTFWeaponBase::DoSwingTrace(CGameTrace& trace)
{
	if (!IsMelee()) return false;
	return vtable_call<530, bool, CGameTrace&>(this, trace);
}

float CTFWeaponBase::GetSwingRange()
{
	float range = 48.0f;

	CTFPlayer* pOwner = HandleAs<CTFPlayer*>(m_hOwner());

	if (pOwner)
	{
		if (pOwner->InCond(TF_COND_SHIELD_CHARGE))
		{
			range = 128.0f;
		}
		else
		{
			int is_sword = AttributeHookValue(0, "is_a_sword", this, nullptr, false);
			if (is_sword) range = 72.0f;
		}

		if (pOwner->m_flModelScale() > 1.0f)
			range *= pOwner->m_flModelScale();
	}

	float melee_range_multiplier = AttributeHookValue(1.0f, "melee_range_multiplier", this, nullptr, false);
	range *= melee_range_multiplier;

	return range;
}