#include "localplayer.h"

#include "../../../settings/settings.h"

#include "../../../sdk/classes/cbaseobject.h"
#include "../../../sdk/classes/ctfrobotdestruction_robot.h"

#include "../../../features/scriptmanager/scriptmanager.h"

static float GetMinimumHeadshotDamage(CBaseEntity* pTarget)
{
	assert(pTarget && "pTarget is null");

	float minimum_damage = 150.0f;

	if (pTarget->IsPlayer())
	{
		CTFPlayer* pPlayer = static_cast<CTFPlayer*>(pTarget);
		minimum_damage = pPlayer->GetHealth();
	}
	else if (pTarget->IsBuilding())
	{
		CBaseObject* pObj = static_cast<CBaseObject*>(pTarget);
		minimum_damage = pObj->m_iHealth();
	}
	else if (pTarget->IsRobot())
	{
		CTFRobotDestruction_Robot* pRobot = static_cast<CTFRobotDestruction_Robot*>(pTarget);
		minimum_damage = pRobot->m_iHealth();
	}

	// minimum is 150
	return std::max(minimum_damage, 150.0f);
}

static int GetMinimumRocketDamage(CBaseEntity* pTarget)
{
	assert(pTarget && "pTarget is null");

	int minimum_damage = 0;

	if (pTarget->IsPlayer())
	{
		CTFPlayer* pPlayer = static_cast<CTFPlayer*>(pTarget);
		minimum_damage = pPlayer->GetHealth();
	}
	else if (pTarget->IsBuilding())
	{
		CBaseObject* pObj = static_cast<CBaseObject*>(pTarget);
		minimum_damage = pObj->m_iHealth();
	}
	else if (pTarget->IsRobot())
	{
		CTFRobotDestruction_Robot* pRobot = static_cast<CTFRobotDestruction_Robot*>(pTarget);
		minimum_damage = pRobot->m_iHealth();
	}

	return minimum_damage;
}

bool helper::localplayer::ShootInternal(CTFPlayer *pLocal, CTFWeaponBase *pWeapon, CUserCmd* pCmd, CBaseEntity* pTarget)
{
	assert(pLocal && "pLocal is null");
	assert(pWeapon && "pWeapon is null");
	assert(pCmd && "pCmd is null");

	EWeaponType type = pWeapon->GetWeaponType();
	int id = pWeapon->GetWeaponID();
	int def_index = pWeapon->m_iItemDefinitionIndex();

	if (def_index == Soldier_m_TheBeggarsBazooka)
	{
		int clip1 = pWeapon->m_iClip1();

		if (clip1 <= 0)
		{
			pCmd->buttons |= IN_ATTACK;
			return false;
		}

		// max ammo
		// no point in holding it in anymore
		if (clip1 >= 3)
		{
			pCmd->buttons &= ~IN_ATTACK;
			return true;
		}
		
		if (pTarget && Config.aimbot.packed.waitforcharge)
		{
			int damage = pWeapon->GetWeaponData().m_nDamage * clip1;
			int minimum_damage = GetMinimumRocketDamage(pTarget);

			if (damage >= minimum_damage)
			{
				pCmd->buttons &= ~IN_ATTACK;
				return true;
			}

			pCmd->buttons |= IN_ATTACK;
			return false;
		}

		pCmd->buttons &= ~IN_ATTACK;
		return true;
	}

	switch (type)
	{
        case EWeaponType::HITSCAN:
	{
		if (IsAttacking(pLocal, pWeapon, pCmd))
			return true;

		switch (id)
		{
			case TF_WEAPON_REVOLVER:
			{
				if (pWeapon->IsAmbassador() && Config.aimbot.packed.waitforcharge)
				{
					if (!pWeapon->CanAmbassadorHeadshot())
						return false;
				}

				pCmd->buttons |= IN_ATTACK;
				return IsAttacking(pLocal, pWeapon, pCmd);
			}

			case TF_WEAPON_SNIPERRIFLE:
			{
				if (pTarget && pLocal->InCond(TF_COND_ZOOMED) && Config.aimbot.packed.waitforcharge)
				{
					CTFSniperRifle* pRifle = static_cast<CTFSniperRifle*>(pWeapon);
					float damage = pRifle->GetChargedDamage();
					float minimum_damage = GetMinimumHeadshotDamage(pTarget);

					if (damage <= minimum_damage)
						return false;
				}

				pCmd->buttons |= IN_ATTACK;
				return true;
			}

			case TF_WEAPON_SNIPERRIFLE_CLASSIC:
			{
				CTFSniperRifleClassic* pClassic = static_cast<CTFSniperRifleClassic*>(pWeapon);

				bool onground = (pLocal->GetFlags() & FL_ONGROUND);
				float damage = pClassic->GetChargedDamage();
				bool charging = pClassic->m_bCharging();

				if (charging && Config.aimbot.packed.waitforcharge)
				{
					// we can headshot now
					if (damage >= 450.0f && onground)
					{
						pCmd->buttons &= ~IN_ATTACK;
						return true;
					}
					else
					{
						pCmd->buttons |= IN_ATTACK;
						return false;
					}
				}

				// dont care about headshot
				if (damage > 0.0f && onground)
				{
					pCmd->buttons &= ~IN_ATTACK;
					return true;
				}

				// charge
				pCmd->buttons |= IN_ATTACK;
				return false;
				break;
			}

			default:
			{
				pCmd->buttons |= IN_ATTACK;
				return IsAttacking(pLocal, pWeapon, pCmd);
			}
		}

		break;
	}
        case EWeaponType::PROJECTILE:
	{
		switch (id)
		{
			case TF_WEAPON_ROCKETLAUNCHER:
			case TF_WEAPON_ROCKETLAUNCHER_DIRECTHIT:
			case TF_WEAPON_FLAREGUN:
			case TF_WEAPON_FLAREGUN_REVENGE:
			case TF_WEAPON_PARTICLE_CANNON:
			case TF_WEAPON_SYRINGEGUN_MEDIC:
			case TF_WEAPON_RAYGUN:
			case TF_WEAPON_GRENADELAUNCHER:
			{
				pCmd->buttons |= IN_ATTACK;
				return IsAttacking(pLocal, pWeapon, pCmd);
			}

			case TF_WEAPON_COMPOUND_BOW:
			case TF_WEAPON_PIPEBOMBLAUNCHER:
			{
				CTFPipebombLauncher* pLauncher = static_cast<CTFPipebombLauncher*>(pWeapon);
				float chargebegintime = pLauncher->m_flChargeBeginTime();
				float tickbase = TICKS_TO_TIME(pLocal->GetTickBase());
				float charge = chargebegintime > 0 ? tickbase - chargebegintime : 0;

				if (charge > 0)
				{
					pCmd->buttons &= ~IN_ATTACK;
					return true;
				}

				// charge
				pCmd->buttons |= IN_ATTACK;
				return false;
			}

			case TF_WEAPON_CANNON:
			{
				float detonatetime = static_cast<CTFGrenadeLauncher *>(pWeapon)->m_flDetonateTime();

				if (detonatetime > 0)
				{
					pCmd->buttons &= ~IN_ATTACK;
					return true;
				}

				// charge
				pCmd->buttons |= IN_ATTACK;
				return false;
			}

			default:
			{
				pCmd->buttons |= IN_ATTACK;
				return IsAttacking(pLocal, pWeapon, pCmd);
			}
		}

		return false;
	}
        case EWeaponType::MELEE:
	{
		pCmd->buttons |= IN_ATTACK;
		return IsAttacking(pLocal, pWeapon, pCmd);
	}

	case EWeaponType::UNKNOWN:
        default:
	break;
        }

	return false;
}

bool helper::localplayer::Shoot(CTFPlayer *pLocal, CTFWeaponBase *pWeapon, CUserCmd* pCmd, CBaseEntity* pTarget)
{
	bool ret = ShootInternal(pLocal, pWeapon, pCmd, pTarget);

	features::scriptmanager.CallHooks("AimbotShoot", pCmd, pTarget);

	return ret;
}

bool helper::localplayer::CanShoot(CTFPlayer *pLocal, CTFWeaponBase *pWeapon, CUserCmd *pCmd, bool ignoreAttack)
{
	if (!pLocal || !pWeapon)
		return false;

	return (ignoreAttack || (pCmd->buttons & IN_ATTACK)) && pWeapon->CanPrimaryAttack();
}

bool helper::localplayer::IsAttacking(CTFPlayer *pLocal, CTFWeaponBase *pWeapon, CUserCmd *pCmd)
{
	if (pLocal == nullptr || pWeapon == nullptr)
		return false;

	switch (pWeapon->GetWeaponType())
	{
	case EWeaponType::HITSCAN:
	{
		int id = pWeapon->GetWeaponID();
		if (id == TF_WEAPON_SNIPERRIFLE_CLASSIC)
			return pWeapon->CanPrimaryAttack() && !(pCmd->buttons & IN_ATTACK) &&
			       static_cast<CTFSniperRifleClassic *>(pWeapon)->m_flChargedDamage() >
				   0.0f;

		if (id == TF_WEAPON_MINIGUN)
		{
			int state = static_cast<CTFMinigun *>(pWeapon)->m_iWeaponState();
			if (state == AC_STATE_FIRING || state == AC_STATE_SPINNING)
				return pWeapon->CanPrimaryAttack() &&
				       pWeapon->HasPrimaryAmmoForShot() && (pCmd->buttons & IN_ATTACK);
			return false;
		}

		return pWeapon->HasPrimaryAmmoForShot() && CanShoot(pLocal, pWeapon, pCmd);
	}

	case EWeaponType::PROJECTILE:
	{
		int id = pWeapon->GetWeaponID();
		if (id == TF_WEAPON_COMPOUND_BOW || id == TF_WEAPON_PIPEBOMBLAUNCHER)
		{
			float flchargebegintime =
			    static_cast<CTFPipebombLauncher *>(pWeapon)->m_flChargeBeginTime();
			float charge = flchargebegintime > 0.f
					   ? TICKS_TO_TIME(pLocal->GetTickBase()) - flchargebegintime
					   : 0.f;
			return charge > 0.0f && !(pCmd->buttons & IN_ATTACK);
		}

		if (pWeapon->m_iItemDefinitionIndex() == Soldier_m_TheBeggarsBazooka)
			return pWeapon->CanPrimaryAttack() && pWeapon->m_iClip1() > 0 &&
			       !(pCmd->buttons & IN_ATTACK);

		if (id == TF_WEAPON_CANNON)
		{
			/*float mortar = AttributeHookValue(0.0f, "grenade_launcher_mortar_mode", pWeapon,
							  nullptr, true);
			if (mortar == 0.0f)
				return CanShoot(pLocal, pWeapon, pCmd);*/

			float detonatetime = static_cast<CTFGrenadeLauncher *>(pWeapon)->m_flDetonateTime();
			//float charge = detonatetime > 0.0f ? mortar - detonatetime -
								 //TICKS_TO_TIME(pLocal->GetTickBase())
							   //: 0.0f;
			//charge	     = Math::RemapVal(charge, 0.0f, mortar, 0.0f, 1.0f);
			//return charge == 1.0f || (!(pCmd->buttons & IN_ATTACK) && charge > 0.0f);
			return detonatetime > 0 && !(pCmd->buttons & IN_ATTACK);
		}

		if (id == TF_WEAPON_LUNCHBOX)
			return pCmd->buttons & IN_ATTACK2;

		return CanShoot(pLocal, pWeapon, pCmd);
	}

	case EWeaponType::MELEE:
	{
		int id = pWeapon->GetWeaponID();
		if (id == TF_WEAPON_KNIFE)
			return CanShoot(pLocal, pWeapon, pCmd);

		return pWeapon->m_flSmackTime() != -1.0f &&
		       TICKS_TO_TIME(pLocal->GetTickBase()) >= pWeapon->m_flSmackTime();
	}

	case EWeaponType::UNKNOWN:
		break;
	}

	return false;
}

void helper::localplayer::ChatPrintf(int iPlayerIndex, int iFilter, const char *fmt)
{
	// xref: Console
	using ChatPrintfFn	     = void (*)(CBaseHudChat *, int, int, const char *);
	static ChatPrintfFn original = reinterpret_cast<ChatPrintfFn>(sigscan_module("client.so", "55 48 89 E5 41 57 41 56 41 55 49 89 FD 41 54 41 89 F4 53 89 D3 48 89 CA"));
	original(interfaces::gHUD, iPlayerIndex, iFilter, fmt);
}
