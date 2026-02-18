#pragma once

#include "../../entitylist/entitylist.h"

#include "../pluto/lua.h"
#include "../pluto/lauxlib.h"

struct LuaEntity
{
    CBaseEntity* ent;
};

namespace LuaClasses
{
	namespace EntityLua
	{
		extern const luaL_Reg methods[];

		void luaopen_entity(lua_State* L);
		LuaEntity* push_entity(lua_State* L, CBaseEntity* entity);
		LuaEntity* CheckEntity(lua_State* L, int idx);
		int IsValid(lua_State* L);
		int GC(lua_State* L);
		int GetName(lua_State* L);
		int GetClassName(lua_State* L);
		int GetAbsOrigin(lua_State* L);
		int GetAbsAngles(lua_State* L);
		int GetNetvarInt(lua_State* L);
		int GetNetvarFloat(lua_State* L);
		int GetNetvarVector(lua_State* L);
		int GetNetvarEntity(lua_State* L);
		int GetHealth(lua_State* L);
		int GetMaxHealth(lua_State* L);
		int EQ(lua_State *L);
		int GetMins(lua_State* L);
		int GetMaxs(lua_State* L);
		int GetTeamNumber(lua_State* L);
		int IsWeapon(lua_State* L);
		int IsDormant(lua_State* L);
		int IsAlive(lua_State* L);
		int SetNetvarInt(lua_State* L);
		int SetNetvarFloat(lua_State* L);
		int SetNetvarVector(lua_State* L);
		int SetNetvarEntity(lua_State* L);
		int GetPlayerResources(lua_State* L);
		int IsPlayer(lua_State* L);
		int GetWeaponID(lua_State *L);
		int GetClassID(lua_State* L);
		int GetFirstMoveChild(lua_State* L);
		int GetNextMovePeer(lua_State* L);
		int AttributeHookValueInt(lua_State* L);
		int AttributeHookValueFloat(lua_State* L);
		int GetWeaponType(lua_State* L);
		int IsInReload(lua_State* L);
		int GetSmackTime(lua_State* L);
		int GetIndex(lua_State* L);
		int Predict(lua_State* L);
		int EstimateAbsVelocity(lua_State* L);
		int ShouldDraw(lua_State* L);
		int SetupBones(lua_State* L);
		int GetMoveType(lua_State *L);
	}
}