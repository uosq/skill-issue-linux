#pragma once

#include "../handle_utils.h"
#include "basecombatweapon.h"
#include "entity.h"

// Grenade Launcher mode (for pipebombs)
enum
{
	TF_GL_MODE_REGULAR = 0,
	TF_GL_MODE_REMOTE_DETONATE,
	TF_GL_MODE_REMOTE_DETONATE_PRACTICE,
	TF_GL_MODE_CANNONBALL,
};


class CBaseProjectile : public CBaseAnimating
{
public:
	NETVAR(m_hOriginalLauncher, "CBaseProjectile->m_hOriginalLauncher", EHANDLE);
};

class CBaseGrenade : public CBaseProjectile
{
public:
	NETVAR(m_flDamage, "CBaseGrenade->m_flDamage", float);
	NETVAR(m_DmgRadius, "CBaseGrenade->m_DmgRadius", float);
	NETVAR(m_bIsLive, "CBaseGrenade->m_bIsLive", bool);
	NETVAR(m_hThrower, "CBaseGrenade->m_hThrower", EHANDLE);
	NETVAR(m_vecVelocity, "CBaseGrenade->m_vecVelocity", Vector);
	NETVAR(m_fFlags, "CBaseGrenade->m_fFlags", int);
};

class CTFWeaponBaseGrenadeProj : public CBaseGrenade
{
public:
	NETVAR(m_vInitialVelocity, "CTFWeaponBaseGrenadeProj->m_vInitialVelocity", Vector);
	NETVAR(m_bCritical, "CTFWeaponBaseGrenadeProj->m_bCritical", bool);
	NETVAR(m_iDeflected, "CTFWeaponBaseGrenadeProj->m_iDeflected", int);
	NETVAR(m_vecOrigin, "CTFWeaponBaseGrenadeProj->m_vecOrigin", Vector);
	NETVAR(m_angRotation, "CTFWeaponBaseGrenadeProj->m_angRotation", Vector);
	NETVAR(m_hDeflectOwner, "CTFWeaponBaseGrenadeProj->m_hDeflectOwner", EHANDLE);
};

class CTFGrenadePipebombProjectile : public CTFWeaponBaseGrenadeProj
{
public:
	NETVAR(m_bTouched, "CTFGrenadePipebombProjectile->m_bTouched", bool);
	NETVAR(m_iType, "CTFGrenadePipebombProjectile->m_iType", int);
	NETVAR(m_hLauncher, "CTFGrenadePipebombProjectile->m_hLauncher", EHANDLE);
	NETVAR(m_bDefensiveBomb, "CTFGrenadePipebombProjectile->m_bDefensiveBomb", bool);
	NETVAR_OFFSET(m_flCreationTime, "CTFGrenadePipebombProjectile->m_iType", float, 4);
	NETVAR_OFFSET(m_flDetonateTime, "CTFWeaponBaseGrenadeProj->m_bCritical", float, -17);

	float GetLiveTime()
	{
		static ConVar *tf_grenadelauncher_livetime = interfaces::Cvar->FindVar("tf_grenadelauncher_livetime");
		float flLiveTime			   = tf_grenadelauncher_livetime->GetFloat();

		AttributeHookValue(flLiveTime, "sticky_arm_time", HandleAs<CBaseEntity *>(m_hLauncher()), nullptr,
				   true);

		// check for powerups here!

		return flLiveTime;
	}

	float GetDamageRadius()
	{
		const float base_radius = AttributeHookValue(m_DmgRadius(), "mult_explosion_radius", this, nullptr, false);
		float radius_mod = 1.0f;

		if (m_iType() == TF_GL_MODE_REMOTE_DETONATE)
		{
			if (!m_bTouched())
			{
				static ConVar* tf_grenadelauncher_livetime = interfaces::Cvar->FindVar("tf_grenadelauncher_livetime");
				static ConVar* tf_sticky_radius_ramp_time = interfaces::Cvar->FindVar("tf_sticky_radius_ramp_time");
				static ConVar* tf_sticky_airdet_radius = interfaces::Cvar->FindVar("tf_sticky_airdet_radius");

				if (tf_grenadelauncher_livetime && tf_sticky_radius_ramp_time && tf_sticky_airdet_radius)
				{
					const float arm_time = tf_grenadelauncher_livetime->GetFloat();
					const float radius_ramp_time = tf_sticky_radius_ramp_time->GetFloat();
					const float creation_time = m_flCreationTime();
					const float cur_time = interfaces::GlobalVars->curtime;
					const float airdet_radius = tf_sticky_airdet_radius->GetFloat();

					radius_mod *= Math::RemapVal(cur_time - creation_time, arm_time, arm_time + radius_ramp_time, airdet_radius, 1.0f);
				}
			}
		}

		return base_radius * radius_mod;
	}

	bool CanExplode()
	{
		return interfaces::GlobalVars->curtime <= m_flDetonateTime();
	}
};