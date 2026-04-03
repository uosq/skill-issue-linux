#pragma once

#include "../definitions/cbaseflex.h"
#include "../definitions/cbasehandle.h"
#include "../netvars/netvar.h"
#include <array>

class C_BaseCombatWeapon;
class C_WeaponCombatShield;

#define BCC_DEFAULT_LOOK_TOWARDS_TOLERANCE 0.9f
#define MAX_WEAPONS 48

class CBaseCombatCharacter : public CBaseFlex
{
      public:
	NETVAR(m_flNextAttack, "CBaseCombatCharacter->m_flNextAttack", float)
	NETVAR(m_hActiveWeapon, "CBaseCombatCharacter->m_hActiveWeapon", EHANDLE)
	inline std::array<EHANDLE, MAX_WEAPONS> &m_hMyWeapons()
	{
		static int nOffset = Netvars::m_netvarMap[fnv::Hash("CBaseCombatCharacter->m_hMyWeapons")];
		return *reinterpret_cast<std::array<EHANDLE, MAX_WEAPONS> *>(uintptr_t(this) + nOffset);
	}
};