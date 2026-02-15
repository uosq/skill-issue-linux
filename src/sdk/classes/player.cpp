#include "player.h"

bool CTFPlayer::IsAlive()
{
	return m_lifeState() == LIFE_ALIVE;
}

bool CTFPlayer::InCond(ETFCond cond)
{
	switch ((int)(cond/32))
	{
		case 0: return m_nPlayerCond() & (1 << cond) || m_condition_bits() & (1 << cond);
		case 1: return m_nPlayerCondEx() & (1 << (cond - 32));
		case 2: return m_nPlayerCondEx2() & (1 << (cond - 64));
		case 3: return m_nPlayerCondEx3() & (1 << (cond - 96));
		case 4: return m_nPlayerCondEx4() & (1 << (cond - 128));
		default: return false;
	}

	return false;
}

Vector CTFPlayer::GetCenter()
{
	return GetAbsOrigin() + ((m_vecMins() + m_vecMaxs()) * 0.5f);
}

Vector CTFPlayer::GetEyePos()
{
	return GetAbsOrigin() + m_vecViewOffset();
}

bool CTFPlayer::IsGhost()
{
	return InCond(ETFCond::TF_COND_HALLOWEEN_GHOST_MODE);
}

bool CTFPlayer::IsTaunting()
{
	return InCond(ETFCond::TF_COND_TAUNTING);
}

bool CTFPlayer::IsUbercharged()
{
	return InCond(ETFCond::TF_COND_INVULNERABLE);
}

std::string CTFPlayer::GetName()
{
	player_info_t info{};
	if (!interfaces::Engine->GetPlayerInfo(GetIndex(), &info))
		return "";

	return info.name;
}

void CTFPlayer::UpdateClientSideAnimation()
{
	using C_BaseAnimating_UpdateClientSideAnimationFn = void(*)(void*);
	static auto orig = (C_BaseAnimating_UpdateClientSideAnimationFn)sigscan_module("client.so", "80 BF D0 0A 00 00 00 75");
	if (!orig)
		return;

	orig((void*)this);
}

int CTFPlayer::GetWaterLevel()
{
	int level = WL_NotInWater;
	Vector point = {};
	int cont = 0;

	Vector mins = m_vecMins();
	Vector maxs = m_vecMaxs();
	Vector origin = GetAbsOrigin();

	point.x = origin.x + (mins.x + maxs.x) * 0.5f;
	point.y = origin.y + (mins.y + maxs.y) * 0.5f;
	point.z = origin.z + mins.z + 1;

	cont = interfaces::EngineTrace->GetPointContents(point);
	if (cont & MASK_WATER)
	{
		level = WL_Feet;

		point.z = origin.z + (mins.z + maxs.z)*0.5f;
		cont = interfaces::EngineTrace->GetPointContents(point);
		if (cont & MASK_WATER)
		{
			level = WL_Waist;

			point.z = origin.z + m_vecViewOffset().z;
			cont = interfaces::EngineTrace->GetPointContents(point);
			if (cont & MASK_WATER)
				level = WL_Eyes;
		}
	}

	return level;
}

float CTFPlayer::GetEffectiveInvisibilityLevel()
{
	// xref: taunt_attr_player_invis_percent
	using GetEffectiveInvisibilityLevelFn = float(*)(void* thisptr);
	static auto orig = reinterpret_cast<GetEffectiveInvisibilityLevelFn>(sigscan_module("client.so", "55 48 89 E5 41 56 41 55 4C 8D AF 78 1E 00 00 41 54 49 89 FC 4C 89 EF 53 E8"));

	if (orig == nullptr)
		return -1;

	return orig(this);
}

uint8_t CTFPlayer::GetMoveType()
{
	// xref: "Bogus pmove player movetype %i on (%i) 0=cl 1=sv\n
	// the switch condition is it
	// the function is CGameMovement::PlayerMove
	/*
	its thhe 0x214 from cVar5
		cVar5 = *(char *)(lVar7 + 0x214);
		LAB_016fa147:
		switch(cVar5) {
		case '\0':
		break;
		case '\x01':
		case '\x02':
		(**(code **)(*param_1 + 0xb8))(param_1);
		break;
		default:
		DevMsg(1,"Bogus pmove player movetype %i on (%i) 0=cl 1=sv\n",cVar5,0);
		break;
	*/
	return *reinterpret_cast<int*>(uintptr_t(this) + 0x214);
}