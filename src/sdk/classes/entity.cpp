#include "entity.h"

#include "../definitions/icollideable.h"

bool CBaseEntity::IsWeapon()
{
	// I think I got every one of them
	switch (GetClassID())
	{
	case ETFClassID::CTFWeaponInvis:
	case ETFClassID::CTFWeaponPDA:
	case ETFClassID::CWeaponMedigun:
	case ETFClassID::CTFWeaponPDA_Spy: // wtf is this?
	case ETFClassID::CTFWeaponBuilder:
	case ETFClassID::CTFWeaponSapper:
	case ETFClassID::CTFMinigun:
	case ETFClassID::CTFSniperRifleClassic:
	case ETFClassID::CTFSniperRifle:
	case ETFClassID::CTFSniperRifleDecap:
	case ETFClassID::CTFBaseProjectile:
	case ETFClassID::CTFBat:
	case ETFClassID::CTFBat_Giftwrap:
	case ETFClassID::CTFBat_Fish:
	case ETFClassID::CTFBat_Wood:
	case ETFClassID::CTFBonesaw:
	case ETFClassID::CTFBottle:
	case ETFClassID::CTFBuffBanner:
	case ETFClassID::CTFCannon:
	case ETFClassID::CTFSMG:
	case ETFClassID::CTFChargedSMG:
	case ETFClassID::CTFCleaver:
	case ETFClassID::CTFClub:
	case ETFClassID::CTFCompoundBow:
	case ETFClassID::CTFCrossbow:
	case ETFClassID::CTFDRGPomson:
	case ETFClassID::CTFFireAxe:
	case ETFClassID::CTFFists:
	case ETFClassID::CTFFlameRocket:
	case ETFClassID::CTFFlameThrower:
	case ETFClassID::CTFWeaponFlameBall:
	case ETFClassID::CTFFlareGun:
	case ETFClassID::CTFFlareGun_Revenge:
	case ETFClassID::CTFGrenadeLauncher:
	case ETFClassID::CTFJar:
	case ETFClassID::CTFJarGas:
	case ETFClassID::CTFJarMilk:
	case ETFClassID::CTFKatana:
	case ETFClassID::CTFKnife:
	case ETFClassID::CTFLaserPointer:
	case ETFClassID::CTFLunchBox_Drink:
	case ETFClassID::CTFMechanicalArm:
	case ETFClassID::CTFParachute:
	case ETFClassID::CTFParachute_Primary:
	case ETFClassID::CTFParachute_Secondary:
	case ETFClassID::CTFPistol:
	case ETFClassID::CTFPistol_Scout:
	case ETFClassID::CTFPistol_ScoutPrimary:
	case ETFClassID::CTFPistol_ScoutSecondary:
	case ETFClassID::CTFPipebombLauncher:
	case ETFClassID::CTFPEPBrawlerBlaster:
	case ETFClassID::CTFRocketLauncher:
	case ETFClassID::CTFRocketLauncher_AirStrike:
	case ETFClassID::CTFRocketLauncher_DirectHit:
	case ETFClassID::CTFRocketLauncher_Mortar:
	case ETFClassID::CTFRocketPack:
	case ETFClassID::CTFShotgun:
	case ETFClassID::CTFShotgun_HWG:
	case ETFClassID::CTFShotgun_Pyro:
	case ETFClassID::CTFShotgun_Revenge:
	case ETFClassID::CTFShotgunBuildingRescue:
	case ETFClassID::CTFShotgun_Soldier:
	case ETFClassID::CTFRevolver:
		return true;
	
	default:
		return false;
	}
	return false;
}

int CBaseEntity::GetIndex()
{
	auto networkable = GetClientNetworkable();
	if (networkable == nullptr) return -1;
	return networkable->entindex();
}

bool CBaseEntity::IsSentry()
{
	return GetClassID() == ETFClassID::CObjectSentrygun;
}

bool CBaseEntity::IsDispenser()
{
	return GetClassID() == ETFClassID::CObjectDispenser;
}

bool CBaseEntity::IsTeleporter()
{
	return GetClassID() == ETFClassID::CObjectTeleporter;
}

bool CBaseEntity::IsPlayer()
{
	return GetClassID() == ETFClassID::CTFPlayer;
}

bool CBaseEntity::IsBuilding()
{
	switch (GetClassID())
	{
	case ETFClassID::CObjectDispenser:
	case ETFClassID::CObjectSentrygun:
	case ETFClassID::CObjectTeleporter:
		return true;
	default:
		break;
	}

	return false;
}

bool CBaseEntity::IsRobot()
{
	return GetClassID() == ETFClassID::CTFRobotDestruction_Robot;
}

Vector CBaseEntity::GetCenter()
{
	return GetAbsOrigin() + (m_vecMaxs() + m_vecMins()) * 0.5f;
}

void CBaseEntity::CalcAbsVelocity()
{
	// xref: Main: %s, Cycle: %.2f\n
	// is inside CMultiPlayerAnimState::DebugShowAnimStateForPlayer
	// first function
	using CalcAbsVelocityFn = void (*)(CBaseEntity *self);
	static CalcAbsVelocityFn original =
	    reinterpret_cast<CalcAbsVelocityFn>(sigscan_module("client.so", "F6 87 11 02 00 00 10"));
	original(this);
}

Vector CBaseEntity::EstimateAbsVelocity()
{
	CalcAbsVelocity();

	/*
	The offsets are from CMultiPlayerAnimState::GetOuterAbsVelocity

		C_BaseEntity::CalcAbsVelocity(pLocal);
		*vec = *(undefined4 *)(pLocal + 0x1c8);
		vec[1] = *(undefined4 *)(pLocal + 0x1cc);
		vec[2] = *(undefined4 *)(pLocal + 0x1d0);
		return;
	}
	*/

	Vec3 m_vecAbsVelocity = *reinterpret_cast<Vector*>(reinterpret_cast<uintptr_t>(this) + 0x1c8);
	return m_vecAbsVelocity;
}

bool CBaseEntity::IsProjectile()
{
	switch (GetClassID())
	{
	case ETFClassID::CBaseProjectile:
	case ETFClassID::CBaseGrenade:
	case ETFClassID::CTFWeaponBaseGrenadeProj:
	case ETFClassID::CTFWeaponBaseMerasmusGrenade:
	case ETFClassID::CTFGrenadePipebombProjectile:
	case ETFClassID::CTFStunBall:
	case ETFClassID::CTFBall_Ornament:
	case ETFClassID::CTFProjectile_Jar:
	case ETFClassID::CTFProjectile_Cleaver:
	case ETFClassID::CTFProjectile_JarGas:
	case ETFClassID::CTFProjectile_JarMilk:
	case ETFClassID::CTFProjectile_SpellBats:
	case ETFClassID::CTFProjectile_SpellKartBats:
	case ETFClassID::CTFProjectile_SpellMeteorShower:
	case ETFClassID::CTFProjectile_SpellMirv:
	case ETFClassID::CTFProjectile_SpellPumpkin:
	case ETFClassID::CTFProjectile_SpellSpawnBoss:
	case ETFClassID::CTFProjectile_SpellSpawnHorde:
	case ETFClassID::CTFProjectile_SpellSpawnZombie:
	case ETFClassID::CTFProjectile_SpellTransposeTeleport:
	case ETFClassID::CTFProjectile_Throwable:
	case ETFClassID::CTFProjectile_ThrowableBreadMonster:
	case ETFClassID::CTFProjectile_ThrowableBrick:
	case ETFClassID::CTFProjectile_ThrowableRepel:
	case ETFClassID::CTFBaseRocket:
	case ETFClassID::CTFFlameRocket:
	case ETFClassID::CTFProjectile_Arrow:
	case ETFClassID::CTFProjectile_GrapplingHook:
	case ETFClassID::CTFProjectile_HealingBolt:
	case ETFClassID::CTFProjectile_Rocket:
	case ETFClassID::CTFProjectile_BallOfFire:
	case ETFClassID::CTFProjectile_MechanicalArmOrb:
	case ETFClassID::CTFProjectile_SentryRocket:
	case ETFClassID::CTFProjectile_SpellFireball:
	case ETFClassID::CTFProjectile_SpellLightningOrb:
	case ETFClassID::CTFProjectile_SpellKartOrb:
	case ETFClassID::CTFProjectile_EnergyBall:
	case ETFClassID::CTFProjectile_Flare:
	case ETFClassID::CTFBaseProjectile:
	case ETFClassID::CTFProjectile_EnergyRing:
		return true;
	default:
		return false;
	}

	return false;
}

void CBaseEntity::AddToLeafSystem(RenderGroup_t group)
{
	using AddToLeafSystemFn = void (*)(void *self, RenderGroup_t group);
	static AddToLeafSystemFn original = reinterpret_cast<AddToLeafSystemFn>(sigscan_module("client.so", "55 89 F2 48 89 E5 41 54"));
	original(this, group);
}

bool CBaseEntity::IsEffectsActive(int nEffects)
{
	// xref: CalcAimEntPositions
	/*
	do {
		// we want the 0xa8
		while ((*(byte *)(*(long *)(DAT_02ecc4c0 + lVar5 * 8) + 0xa8) & 1) == 0) {
			lVar5 = lVar5 + 1;
			if (iVar1 <= (int)lVar5) goto LAB_0162bf68;
		}
		FUN_0162b390();
		lVar5 = lVar5 + 1;
	} while ((int)lVar5 < iVar1);
	*/

	//auto& effects = GetEffects();

	// i want to kms :sob:
	return (m_fEffects() & nEffects) != 0;
}

void CBaseEntity::AddEffects(uint8_t effects)
{
	//auto addr = reinterpret_cast<uintptr_t>(this) + 0xa8;
	//*reinterpret_cast<uint8_t*>(addr) |= effects;
	m_fEffects() |= effects;
}

void CBaseEntity::RemoveEffects(uint8_t effects)
{
	//aut o addr = reinterpret_cast<uintptr_t>(this) + 0xa8;
	// *reinterpret_cast<uint8_t*>(addr) &= ~effects;
	m_fEffects() &= ~effects;
}

void CBaseEntity::UpdateVisibility()
{
	using UpdateVisibilityFn	   = void (*)(void *self);
	static UpdateVisibilityFn original = reinterpret_cast<UpdateVisibilityFn>(
	    sigscan_module("client.so", "55 48 89 E5 41 54 49 89 FC 53 48 83 EC 10 48 8D 1D ? "
					"? ? ? 48 8B 3B 48 8B 07 FF 90 60 02 00 00"));
	original(this);
}

void CBaseEntity::SetAbsOrigin(const Vec3& absOrigin)
{
	/*
	xref: C_BaseAnimating::BecomeRagdollOnClient failed. pRagdoll:%p bInitBoneArrays:%d bInitAsClient:%d\n

	FUN_01629990(param_1,1);
	  (**(code **)(*param_1 + 0x58))(param_1);
====>  		plVar3 = (long *)CreateRagdollCopy(param_1);

	go inside C_BaseAnimating::CreateRagdollCopy

		uVar5 = (**(code **)(*DAT_02ee4820 + 0x20))(DAT_02ee4820,lVar7);
		cVar1 = (**(code **)(*plVar4 + 1000))(plVar4,uVar5,7);
		if (cVar1 == '\0') {
		(**(code **)(*plVar4 + 0x50))(plVar4);
		return (long *)0x0;
		}
		FUN_0162a770(this,plVar4);
		uVar6 = (**(code **)(*this + 0x58))(this);
======>			SetAbsOrigin(plVar4,uVar6);
		uVar6 = (**(code **)(*this + 0x60))(this);
======>			SetAbsAngles(plVar4,uVar6);
		FUN_01796c50(plVar4,this);
	*/

	using SetAbsOriginFn = void(*)(void* self, const Vec3& absOrigin);
	static SetAbsOriginFn original = (SetAbsOriginFn)Sigs::CBaseEntity_SetAbsOrigin.GetPointer();
	original((void*)this, absOrigin);
}

void CBaseEntity::SetAbsAngles(const Vec3& absAngle)
{
	// to know how to get this, look at SetAbsOrigin above ^

	using SetAbsAnglesFn = void(*)(void* self, const Vec3& absAngle);
	static SetAbsAnglesFn original = (SetAbsAnglesFn)Sigs::CBaseEntity_SetAbsAngles.GetPointer();
	original((void*)this, absAngle);
}

bool CBaseEntity::ShouldCollide(int collisionGroup, int contentsMask)
{
	if ( m_CollisionGroup() == COLLISION_GROUP_DEBRIS )
	{
		if ( ! (contentsMask & CONTENTS_DEBRIS) )
			return false;
	}
	return true;
}

float CBaseEntity::m_flModelScale()
{
	return *reinterpret_cast<float*>(uintptr_t(this) + 0x914);
}

void CBaseEntity::SetCollisionBounds(const Vec3& mins, const Vec3& maxs)
{
	/*

		HOW TO GET

		In 'client.so', search for: disableshadows
		You'll find 2 functions that have this string, get the first one

		In there, it should have a "mins" and "maxs" strings
		Any of them work, you have to get the function after UTIL_StringToVector

		if ((szKeyName == "mins") || (iVar3 = FUN_01fe7d80(szKeyName), iVar3 == 0)) {
		FUN_017b9e60(&local_34,szValue);
======>		CCollisionProperty::SetCollisionBounds(this + 0x12,&local_34,&this[0x13].IClientNetworkable.field_0x4);
		return 1;
		}

		if ((szKeyName == "maxs") || (iVar3 = FUN_01fe7d80(szKeyName), iVar3 == 0)) {
		FUN_017b9e60(&local_34,szValue);
======>		CCollisionProperty::SetCollisionBounds(this + 0x12,&this[0x13].IClientRenderable,&local_34);
		return 1;
		}
	*/

	using SetCollisionBoundsFn = void(*)(ICollideable* rdi, const Vec3& mins, const Vec3& maxs);
	static SetCollisionBoundsFn original = reinterpret_cast<SetCollisionBoundsFn>(sigscan_module("client.so", "55 48 89 E5 41 55 49 89 D5 41 54 49 89 FC 53 48 89 F3 48 83 EC 28 F3 0F 10 47 10"));

	original(this->GetCollideable(), mins, maxs);
}