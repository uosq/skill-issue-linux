#pragma once

#include "basecombatweapon.h"
#include "../handle_utils.h"

class CBaseProjectile: public CBaseAnimating
{
public:
	NETVAR(m_hOriginalLauncher, "CBaseProjectile->m_hOriginalLauncher", EHANDLE);
};

class CBaseGrenade: public CBaseProjectile
{
public:
	NETVAR(m_flDamage, "CBaseGrenade->m_flDamage", float);
	NETVAR(m_DmgRadius, "CBaseGrenade->m_DmgRadius", float);
	NETVAR(m_bIsLive, "CBaseGrenade->m_bIsLive", bool);
	NETVAR(m_hThrower, "CBaseGrenade->m_hThrower", EHANDLE);
	NETVAR(m_vecVelocity, "CBaseGrenade->m_vecVelocity", Vector);
	NETVAR(m_fFlags, "CBaseGrenade->m_fFlags", int);
};

class CTFWeaponBaseGrenadeProj: public CBaseGrenade
{
public:
	NETVAR(m_vInitialVelocity, "CTFWeaponBaseGrenadeProj->m_vInitialVelocity", Vector);
	NETVAR(m_bCritical, "CTFWeaponBaseGrenadeProj->m_bCritical", bool);
	NETVAR(m_iDeflected, "CTFWeaponBaseGrenadeProj->m_iDeflected", int);
	NETVAR(m_vecOrigin, "CTFWeaponBaseGrenadeProj->m_vecOrigin", Vector);
	NETVAR(m_angRotation, "CTFWeaponBaseGrenadeProj->m_angRotation", Vector);
	NETVAR(m_hDeflectOwner, "CTFWeaponBaseGrenadeProj->m_hDeflectOwner", EHANDLE);
};

class CTFGrenadePipebombProjectile: public CTFWeaponBaseGrenadeProj
{
public:
	NETVAR(m_bTouched, "CTFGrenadePipebombProjectile->m_bTouched", bool);
	NETVAR(m_iType, "CTFGrenadePipebombProjectile->m_iType", int);
	NETVAR(m_hLauncher, "CTFGrenadePipebombProjectile->m_hLauncher", EHANDLE);
	NETVAR(m_bDefensiveBomb, "CTFGrenadePipebombProjectile->m_bDefensiveBomb", bool);
};