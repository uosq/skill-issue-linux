#include "ienginetrace.h"
#include "bspflags.h"
#include "ctracefilters.h"
#include "../classes/player.h"
#include "../classes/weaponbase.h"
#include "../handle_utils.h"
#include "ihandleentity.h"

#include "../../features/lua/classes.h"
#include "../../features/lua/api.h"
#include "../../gui/console.h"

bool CTraceFilterHitscan::ShouldHitEntity(IHandleEntity* pServerEntity, int nContentsMask)
{
	if (!pServerEntity || pServerEntity == pSkip)
		return false;

	auto pEntity = reinterpret_cast<CBaseEntity*>(pServerEntity);
	if (iTeam == -1) iTeam = pSkip ? pSkip->m_iTeamNum() : 0;
	if (iType != SKIP_CHECK && !vWeapons.empty())
	{
		if (auto pWeapon = pSkip && pSkip->IsPlayer() ? HandleAs<CTFWeaponBase*>(((CTFPlayer*)pSkip)->GetActiveWeapon()) : nullptr)
		{
			int iWeaponID = pWeapon->GetWeaponID();
			bWeapon = std::find(vWeapons.begin(), vWeapons.end(), iWeaponID) != vWeapons.end();
		}
		vWeapons.clear();
	}

	switch (pEntity->GetClassID())
	{
	case ETFClassID::CTFAmmoPack:
	case ETFClassID::CFuncAreaPortalWindow:
	case ETFClassID::CFuncRespawnRoomVisualizer:
	case ETFClassID::CTFReviveMarker: return false;
	case ETFClassID::CTFMedigunShield: return pEntity->m_iTeamNum() != iTeam;
	case ETFClassID::CTFPlayer:
	case ETFClassID::CBaseObject:
	case ETFClassID::CObjectSentrygun:
	case ETFClassID::CObjectDispenser:
	case ETFClassID::CObjectTeleporter: 
		if (iType != SKIP_CHECK && (iWeapon == WEAPON_INCLUDE ? bWeapon : !bWeapon))
			return iType == FORCE_HIT ? true : false;
		return pEntity->m_iTeamNum() != iTeam;

	default: return true;
	}

	return true;
}
TraceType_t CTraceFilterHitscan::GetTraceType() const
{
	return TRACE_EVERYTHING;
}

bool CTraceFilterCollideable::ShouldHitEntity(IHandleEntity* pServerEntity, int nContentsMask)
{
	if (!pServerEntity || pServerEntity == pSkip)
		return false;

	auto pEntity = reinterpret_cast<CBaseEntity*>(pServerEntity);
	if (iTeam == -1) iTeam = pSkip ? pSkip->m_iTeamNum() : 0;
	if (iType != SKIP_CHECK && !vWeapons.empty())
	{
		if (auto pWeapon = pSkip && pSkip->IsPlayer() ? HandleAs<CTFWeaponBase*>(((CTFPlayer*)pSkip)->GetActiveWeapon()) : nullptr)
		{
			int iWeaponID = pWeapon->GetWeaponID();
			bWeapon = std::find(vWeapons.begin(), vWeapons.end(), iWeaponID) != vWeapons.end();
		}
		vWeapons.clear();
	}

	switch (pEntity->GetClassID())
	{
	case ETFClassID::CBaseEntity:
	case ETFClassID::CBaseDoor:
	case ETFClassID::CDynamicProp:
	case ETFClassID::CPhysicsProp:
	case ETFClassID::CPhysicsPropMultiplayer:
	case ETFClassID::CFunc_LOD:
	case ETFClassID::CObjectCartDispenser:
	case ETFClassID::CFuncTrackTrain:
	case ETFClassID::CFuncConveyor:
	case ETFClassID::CTFGenericBomb:
	case ETFClassID::CTFPumpkinBomb: return true;
	case ETFClassID::CFuncRespawnRoomVisualizer:
		if (nContentsMask & CONTENTS_PLAYERCLIP)
			return pEntity->m_iTeamNum() != iTeam;
		break;
	case ETFClassID::CTFMedigunShield:
		if (!(nContentsMask & CONTENTS_PLAYERCLIP))
			return pEntity->m_iTeamNum() != iTeam;
		break;
	case ETFClassID::CTFPlayer:
		if (iPlayer == PLAYER_ALL)
			return true;
		if (iPlayer == PLAYER_NONE)
			return false;
		if (iType != SKIP_CHECK && (iWeapon == WEAPON_INCLUDE ? bWeapon : !bWeapon))
			return iType == FORCE_HIT ? true : false;
		return pEntity->m_iTeamNum() != iTeam;
	case ETFClassID::CBaseObject:
	case ETFClassID::CObjectSentrygun:
	case ETFClassID::CObjectDispenser: return iObject == OBJECT_ALL ? true : iObject == OBJECT_NONE ? false : pEntity->m_iTeamNum() != iTeam;
	case ETFClassID::CObjectTeleporter: return true;
	//case ETFClassID::CTFBaseBoss:
	//case ETFClassID::CTFTankBoss:
	//case ETFClassID::CMerasmus:
	//case ETFClassID::CEyeballBoss:
	//case ETFClassID::CHeadlessHatman:
	//case ETFClassID::CZombie: return bMisc;
	case ETFClassID::CTFGrenadePipebombProjectile: return bMisc; //&& pEntity->As<CTFGrenadePipebombProjectile>()->m_iType() == TF_GL_MODE_REMOTE_DETONATE;

	default: return true;
	}

	return false;
}
TraceType_t CTraceFilterCollideable::GetTraceType() const
{
	return TRACE_EVERYTHING;
}

bool CTraceFilterWorldAndPropsOnly::ShouldHitEntity(IHandleEntity* pServerEntity, int nContentsMask)
{
	if (!pServerEntity || pServerEntity == pSkip)
		return false;
	if (pServerEntity->GetRefEHandle().GetSerialNumber() == (1 << 15))
		return interfaces::EntityList->GetClientEntity(0) != pSkip;

	auto pEntity = reinterpret_cast<CBaseEntity*>(pServerEntity);
	if (iTeam == -1) iTeam = pSkip ? pSkip->m_iTeamNum() : 0;

	switch (pEntity->GetClassID())
	{
	case ETFClassID::CBaseEntity:
	case ETFClassID::CBaseDoor:
	case ETFClassID::CDynamicProp:
	case ETFClassID::CPhysicsProp:
	case ETFClassID::CPhysicsPropMultiplayer:
	case ETFClassID::CFunc_LOD:
	case ETFClassID::CObjectCartDispenser:
	case ETFClassID::CFuncTrackTrain:
	case ETFClassID::CFuncConveyor: return true;
	case ETFClassID::CFuncRespawnRoomVisualizer: return nContentsMask & CONTENTS_PLAYERCLIP && pEntity->m_iTeamNum() != iTeam;
	default: return false;
	}

	return false;
}
TraceType_t CTraceFilterWorldAndPropsOnly::GetTraceType() const
{
	return TRACE_EVERYTHING_FILTER_PROPS;
}

TraceType_t CTraceFilterLua::GetTraceType() const
{
	return TRACE_EVERYTHING;
}

bool CTraceFilterLua::ShouldHitEntity(IHandleEntity* pServerEntity, int nContentsMask)
{
	luaL_checktype(Lua::m_luaState, 4, LUA_TFUNCTION);
	lua_pushvalue(Lua::m_luaState, 4);

	int callbackRef = luaL_ref(Lua::m_luaState, LUA_REGISTRYINDEX);
	lua_rawgeti(Lua::m_luaState, LUA_REGISTRYINDEX, callbackRef);

	LuaClasses::EntityLua::push_entity(Lua::m_luaState, static_cast<CBaseEntity*>(pServerEntity));
	lua_pushinteger(Lua::m_luaState, nContentsMask);

	bool result = true;

	if (lua_pcall(Lua::m_luaState, 2, 1, 0) != LUA_OK)
	{
		const char* err = lua_tostring(Lua::m_luaState, -1);
		if (err) consoleText += std::string(err) + "\n";
		lua_pop(Lua::m_luaState, 1);
	}
	else
	{
		// read return value
		if (lua_isboolean(Lua::m_luaState, -1))
			result = lua_toboolean(Lua::m_luaState, -1);

		lua_pop(Lua::m_luaState, 1); // pop return value
	}

	luaL_unref(Lua::m_luaState, LUA_REGISTRYINDEX, callbackRef);

	return result;
}
