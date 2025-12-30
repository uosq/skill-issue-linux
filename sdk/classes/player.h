#pragma once

#include "entity.h"
#include "../definitions/types.h"

struct user_cmd;

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

struct player_info {
	char name[32];
	int user_id;
	char guid[33];
	unsigned long friends_id;
	char friends_name[32];
	bool fakeplayer;
	bool ishltv;
	unsigned long custom_files[4];
	unsigned char files_downloaded;
};

enum {
	TF_COND_INVALID                          = -1,
	TF_COND_AIMING                           = 0, // Sniper aiming, Heavy minigun.
	TF_COND_ZOOMED                           = 1,
	TF_COND_DISGUISING                       = 2,
	TF_COND_DISGUISED                        = 3,
	TF_COND_CLOAKED                        = 4, // Spy specific
	TF_COND_INVULNERABLE                     = 5,
	TF_COND_TELEPORTED                       = 6,
	TF_COND_TAUNTING                         = 7,
	TF_COND_INVULNERABLE_WEARINGOFF          = 8,
	TF_COND_STEALTHED_BLINK                  = 9,
	TF_COND_SELECTED_TO_TELEPORT             = 10,
	TF_COND_CRITBOOSTED                      = 11, // DO NOT RE-USE THIS -- THIS IS FOR KRITZKRIEG AND REVENGE CRITS ONLY
	TF_COND_TMPDAMAGEBONUS                   = 12,
	TF_COND_FEIGN_DEATH                      = 13,
	TF_COND_PHASE                            = 14,
	TF_COND_STUNNED                          = 15,
	TF_COND_OFFENSEBUFF                      = 16,
	TF_COND_SHIELD_CHARGE                    = 17,
	TF_COND_DEMO_BUFF                        = 18,
	TF_COND_ENERGY_BUFF                      = 19,
	TF_COND_RADIUSHEAL                       = 20,
	TF_COND_HEALTH_BUFF                      = 21,
	TF_COND_BURNING                          = 22,
	TF_COND_HEALTH_OVERHEALED                = 23,
	TF_COND_URINE                            = 24,
	TF_COND_BLEEDING                         = 25,
	TF_COND_DEFENSEBUFF                      = 26,
	TF_COND_MAD_MILK                         = 27,
	TF_COND_MEGAHEAL                         = 28,
	TF_COND_REGENONDAMAGEBUFF                = 29,
	TF_COND_MARKEDFORDEATH                   = 30,
	TF_COND_NOHEALINGDAMAGEBUFF              = 31,
	TF_COND_SPEED_BOOST                      = 32,
	TF_COND_CRITBOOSTED_PUMPKIN              = 33,
	TF_COND_CRITBOOSTED_USER_BUFF            = 34,
	TF_COND_CRITBOOSTED_DEMO_CHARGE          = 35,
	TF_COND_SODAPOPPER_HYPE                  = 36,
	TF_COND_CRITBOOSTED_FIRST_BLOOD          = 37,
	TF_COND_CRITBOOSTED_BONUS_TIME           = 38,
	TF_COND_CRITBOOSTED_CTF_CAPTURE          = 39,
	TF_COND_CRITBOOSTED_ON_KILL              = 40,
	TF_COND_CANNOT_SWITCH_FROM_MELEE         = 41,
	TF_COND_DEFENSEBUFF_NO_CRIT_BLOCK        = 42,
	TF_COND_REPROGRAMMED                     = 43,
	TF_COND_CRITBOOSTED_RAGE_BUFF            = 44,
	TF_COND_DEFENSEBUFF_HIGH                 = 45,
	TF_COND_SNIPERCHARGE_RAGE_BUFF           = 46,
	TF_COND_DISGUISE_WEARINGOFF              = 47,
	TF_COND_MARKEDFORDEATH_SILENT            = 48,
	TF_COND_DISGUISED_AS_DISPENSER           = 49,
	TF_COND_SAPPED                           = 50,
	TF_COND_INVULNERABLE_HIDE_UNLESS_DAMAGED = 51,
	TF_COND_INVULNERABLE_USER_BUFF           = 52,
	TF_COND_HALLOWEEN_BOMB_HEAD              = 53,
	TF_COND_HALLOWEEN_THRILLER               = 54,
	TF_COND_RADIUSHEAL_ON_DAMAGE             = 55,
	TF_COND_CRITBOOSTED_CARD_EFFECT          = 56,
	TF_COND_INVULNERABLE_CARD_EFFECT         = 57,
	TF_COND_MEDIGUN_UBER_BULLET_RESIST       = 58,
	TF_COND_MEDIGUN_UBER_BLAST_RESIST        = 59,
	TF_COND_MEDIGUN_UBER_FIRE_RESIST         = 60,
	TF_COND_MEDIGUN_SMALL_BULLET_RESIST      = 61,
	TF_COND_MEDIGUN_SMALL_BLAST_RESIST       = 62,
	TF_COND_MEDIGUN_SMALL_FIRE_RESIST        = 63,
	TF_COND_STEALTHED_USER_BUFF              = 64,
	TF_COND_MEDIGUN_DEBUFF                   = 65,
	TF_COND_STEALTHED_USER_BUFF_FADING       = 66,
	TF_COND_BULLET_IMMUNE                    = 67,
	TF_COND_BLAST_IMMUNE                     = 68,
	TF_COND_FIRE_IMMUNE                      = 69,
	TF_COND_PREVENT_DEATH                    = 70,
	TF_COND_MVM_BOT_STUN_RADIOWAVE           = 71,
	TF_COND_HALLOWEEN_SPEED_BOOST            = 72,
	TF_COND_HALLOWEEN_QUICK_HEAL             = 73,
	TF_COND_HALLOWEEN_GIANT                  = 74,
	TF_COND_HALLOWEEN_TINY                   = 75,
	TF_COND_HALLOWEEN_IN_HELL                = 76,
	TF_COND_HALLOWEEN_GHOST_MODE             = 77,
	TF_COND_MINICRITBOOSTED_ON_KILL          = 78,
	TF_COND_OBSCURED_SMOKE                   = 79,
	TF_COND_PARACHUTE_ACTIVE                 = 80,
	TF_COND_BLASTJUMPING                     = 81,
	TF_COND_HALLOWEEN_KART                   = 82,
	TF_COND_HALLOWEEN_KART_DASH              = 83,
	TF_COND_BALLOON_HEAD                     = 84,
	TF_COND_MELEE_ONLY                       = 85,
	TF_COND_SWIMMING_CURSE                   = 86,
	TF_COND_FREEZE_INPUT                     = 87,
	TF_COND_HALLOWEEN_KART_CAGE              = 88,
	TF_COND_DONOTUSE_0                       = 89,
	TF_COND_RUNE_STRENGTH                    = 90,
	TF_COND_RUNE_HASTE                       = 91,
	TF_COND_RUNE_REGEN                       = 92,
	TF_COND_RUNE_RESIST                      = 93,
	TF_COND_RUNE_VAMPIRE                     = 94,
	TF_COND_RUNE_REFLECT                     = 95,
	TF_COND_RUNE_PRECISION                   = 96,
	TF_COND_RUNE_AGILITY                     = 97,
	TF_COND_GRAPPLINGHOOK                    = 98,
	TF_COND_GRAPPLINGHOOK_SAFEFALL           = 99,
	TF_COND_GRAPPLINGHOOK_LATCHED            = 100,
	TF_COND_GRAPPLINGHOOK_BLEEDING           = 101,
	TF_COND_AFTERBURN_IMMUNE                 = 102,
	TF_COND_RUNE_KNOCKOUT                    = 103,
	TF_COND_RUNE_IMBALANCE                   = 104,
	TF_COND_CRITBOOSTED_RUNE_TEMP            = 105,
	TF_COND_PASSTIME_INTERCEPTION            = 106,
	TF_COND_SWIMMING_NO_EFFECTS              = 107,
	TF_COND_PURGATORY                        = 108,
	TF_COND_RUNE_KING                        = 109,
	TF_COND_RUNE_PLAGUE                      = 110,
	TF_COND_RUNE_SUPERNOVA                   = 111,
	TF_COND_PLAGUE                           = 112,
	TF_COND_KING_BUFFED                      = 113,
	TF_COND_TEAM_GLOWS                       = 114,
	TF_COND_KNOCKED_INTO_AIR                 = 115,
	TF_COND_COMPETITIVE_WINNER               = 116,
	TF_COND_COMPETITIVE_LOSER                = 117,
	TF_COND_HEALING_DEBUFF                   = 118,
	TF_COND_PASSTIME_PENALTY_DEBUFF          = 119,
	TF_COND_GRAPPLED_TO_PLAYER               = 120,
	TF_COND_GRAPPLED_BY_PLAYER               = 121,
	TF_COND_PARACHUTE_DEPLOYED               = 122,
	TF_COND_GAS                              = 123,
	TF_COND_BURNING_PYRO                     = 124,
	TF_COND_ROCKETPACK                       = 125,
	TF_COND_LOST_FOOTING                     = 126,
	TF_COND_AIR_CURRENT                      = 127,
	TF_COND_HALLOWEEN_HELL_HEAL              = 128,
	TF_COND_POWERUPMODE_DOMINANT		   = 129,
	TF_COND_IMMUNE_TO_PUSHBACK		   = 130,
	TF_COND_LAST
};

#define	LIFE_ALIVE 0
#define	LIFE_DYING 1
#define	LIFE_DEAD 2
#define LIFE_RESPAWNABLE 3
#define LIFE_DISCARDBODY 4

class Player : public CBaseEntity {
public:
	NETVAR(GetActiveWeapon, "CBaseCombatCharacter->m_hActiveWeapon", C_BaseEntity*)
	NETVAR(GetNextAttack, "CBaseCombatCharacter->m_flNextAttack", float)
	NETVAR(GetTickBase, "CBasePlayer->m_nTickBase", int)
	NETVAR(GetVelocity, "CBasePlayer->m_vecVelocity[0]", Vector)
	NETVAR(GetFlags, "CBasePlayer->m_fFlags", int)
	NETVAR(GetViewModel, "CBasePlayer->m_hViewModel[0]", C_BaseEntity*)
	NETVAR(GetHealth, "CBasePlayer->m_iHealth", int)
	NETVAR(GetOrigin, "CTFPlayer->m_vecOrigin", Vector)
	NETVAR(m_nForceTauntCam, "CTFPlayer->m_nForceTauntCam", bool)
	NETVAR(m_lifeState, "CBasePlayer->m_lifeState", uint8_t) // 1 byte

	bool IsAlive()
	{
		//interfaces::vstdlib->ConsolePrintf("%d\n", m_lifeState());
		return m_lifeState() == LIFE_ALIVE;
	}
};