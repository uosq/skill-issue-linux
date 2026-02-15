#pragma once

#include "entity.h"
#include "../definitions/types.h"
#include "../defs.h"
#include "../definitions/bspflags.h"
#include "playerresource.h"
#include "../definitions/classdata.h"

#define	FL_ONGROUND (1<<0)
#define FL_DUCKING (1<<1)
#define	FL_WATERJUMP (1<<2)
#define FL_ONTRAIN (1<<3)
#define FL_INRAIN (1<<4)
#define FL_FROZEN (1<<5)
#define FL_ATCONTROLS (1<<6)
#define	FL_CLIENT (1<<7)
#define FL_FAKECLIENT (1<<8)
#define	FL_INWATER (1<<9)
#define PLAYER_FLAG_BITS 32

enum {
	CLASS_UNDEFINED = 0,
	CLASS_SCOUT,
	CLASS_SNIPER,
	CLASS_SOLDIER,
	CLASS_DEMOMAN,
	CLASS_MEDIC,
	CLASS_HEAVYWEAPONS,
	CLASS_PYRO,
	CLASS_SPY,
	CLASS_ENGINEER
};

struct PlayerInfo {
	char name[32];
	int UserID;
	char GUID[33];
	unsigned long FriendIDs;
	char FriendNames[32];
	bool isFakePlayer;
	bool isHLTV;
	unsigned long custom_files[4];
	unsigned char files_downloaded;
};

#define	LIFE_ALIVE 0
#define	LIFE_DYING 1
#define	LIFE_DEAD 2
#define LIFE_RESPAWNABLE 3
#define LIFE_DISCARDBODY 4

enum
{
	WL_NotInWater=0,
	WL_Feet,
	WL_Waist,
	WL_Eyes
};

class CTFPlayer : public CBaseEntity {
public:
	NETVAR(GetActiveWeapon, "CBaseCombatCharacter->m_hActiveWeapon", EHANDLE)
	NETVAR(GetNextAttack, "CBaseCombatCharacter->m_flNextAttack", float)
	NETVAR(GetTickBase, "CBasePlayer->m_nTickBase", int)
	NETVAR(GetVelocity, "CBasePlayer->m_vecVelocity[0]", Vector)
	NETVAR(GetFlags, "CBasePlayer->m_fFlags", int)
	NETVAR(GetViewModel, "CBasePlayer->m_hViewModel[0]", EHANDLE)
	NETVAR(GetHealth, "CBasePlayer->m_iHealth", int)
	NETVAR(GetOrigin, "CTFPlayer->m_vecOrigin", Vector)
	NETVAR(m_nForceTauntCam, "CTFPlayer->m_nForceTauntCam", bool)
	NETVAR(m_lifeState, "CBasePlayer->m_lifeState", uint8_t) // 1 byte
	NETVAR(m_nPlayerCond, "CTFPlayer->m_nPlayerCond", int)
	NETVAR(m_nPlayerCondEx, "CTFPlayer->m_nPlayerCondEx", int)
	NETVAR(m_nPlayerCondEx2, "CTFPlayer->m_nPlayerCondEx2", int)
	NETVAR(m_nPlayerCondEx3, "CTFPlayer->m_nPlayerCondEx3", int)
	NETVAR(m_nPlayerCondEx4, "CTFPlayer->m_nPlayerCondEx4", int)
	NETVAR(m_condition_bits, "CTFPlayer->_condition_bits", int)

	// !!! Only has 2 fields ([0], [1]) // is it really only 2 fields? TEST
	NETVAR(m_angEyeAngles, "CTFPlayer->m_angEyeAngles[0]", Vector)

	NETVAR(m_vecViewOffset, "CBasePlayer->m_vecViewOffset[0]", Vector)
	NETVAR(m_flMaxspeed, "CBasePlayer->m_flMaxspeed", float)
	NETVAR(m_flStepSize, "CBasePlayer->m_flStepSize", float)
	NETVAR(m_iDefaultFOV, "CBasePlayer->m_iDefaultFOV", int)
	NETVAR(m_hObserverTarget, "CBasePlayer->m_hObserverTarget", EHANDLE)
	NETVAR(m_iObserverMode, "CBasePlayer->m_iObserverMode", int)
	NETVAR(m_iClass, "CTFPlayer->m_iClass", int)
	NETVAR(m_hGroundEntity, "CBasePlayer->m_hGroundEntity", EHANDLE)
	NETVAR(m_nWaterLevel, "CTFPlayer->m_nWaterLevel", uint8_t)
	NETVAR(m_iFOV, "CBasePlayer->m_iFOV", int)

	NETVAR(m_vecPunchAngle, "CBasePlayer->m_vecPunchAngle", Vector)

	bool IsAlive();
	bool InCond(ETFCond cond);
	Vector GetCenter();
	Vector GetEyePos();
	bool IsGhost();
	bool IsTaunting();
	bool IsUbercharged();
	std::string GetName();
	void UpdateClientSideAnimation();
	int GetWaterLevel();
	float GetEffectiveInvisibilityLevel();
	uint8_t GetMoveType();
};