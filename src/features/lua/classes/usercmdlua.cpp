#include "usercmdlua.h"
#include "../pluto/lauxlib.h"
#include "../../ticks/ticks.h"
#include "vectorlua.h"

LuaUserCmd* LuaClasses::UserCmd::push(lua_State* L, const CUserCmd& pCmd)
{
	LuaUserCmd* lcmd = static_cast<LuaUserCmd*>(lua_newuserdata(L, sizeof(LuaUserCmd)));
	new (lcmd) LuaUserCmd{};

	lcmd->CopyFromUserCmd(pCmd);

	luaL_getmetatable(L, "UserCmd");
	lua_setmetatable(L, -2);

	return lcmd;
}

void LuaClasses::UserCmd::open(lua_State *L)
{
	luaL_newmetatable(L, "UserCmd");

	lua_pushcfunction(L, LuaUserCmd::__index);
	lua_setfield(L, -2, "__index");

	lua_pushcfunction(L, LuaUserCmd::__newindex);
	lua_setfield(L, -2, "__newindex");

	lua_pushstring(L, "UserCmd");
	lua_setfield(L, -2, "__name");

	// lock
	lua_pushboolean(L, 0);
	lua_setfield(L, -2, "__metatable");

	// pop the metatable
	lua_pop(L, 1);
}

void LuaUserCmd::CopyFromUserCmd(const CUserCmd& pCmd)
{
	valid = true;
	command_number = pCmd.command_number;
	tick_count = pCmd.tick_count;
	viewangles = pCmd.viewangles;
	forwardmove = pCmd.forwardmove;
	sidemove = pCmd.sidemove;
	upmove = pCmd.upmove;
	buttons = pCmd.buttons;
	impulse = pCmd.impulse;
	weaponselect = pCmd.weaponselect;
	weaponsubtype = pCmd.weaponsubtype;
	random_seed = pCmd.random_seed;
	server_random_seed = pCmd.server_random_seed;
	mousedx = pCmd.mousedx;
	mousedy = pCmd.mousedy;
	hasbeenpredicted = pCmd.hasbeenpredicted;
	sendpacket = TickManager::m_bSendPacket;
}

void LuaUserCmd::WriteToUserCmd(CUserCmd* pOut)
{
	pOut->command_number = command_number;
	pOut->tick_count = tick_count;
	pOut->viewangles = viewangles;
	pOut->forwardmove = forwardmove;
	pOut->sidemove = sidemove;
	pOut->upmove = upmove;
	pOut->buttons = buttons;
	pOut->impulse = impulse;
	pOut->weaponselect = weaponselect;
	pOut->weaponsubtype = weaponsubtype;
	pOut->random_seed = random_seed;
	pOut->server_random_seed = server_random_seed;
	pOut->mousedx = mousedx;
	pOut->mousedy = mousedy;
	pOut->hasbeenpredicted = hasbeenpredicted;
	TickManager::m_bSendPacket = sendpacket;
}

int LuaUserCmd::__index(lua_State* L)
{
	LuaUserCmd* lcmd = static_cast<LuaUserCmd*>(luaL_checkudata(L, 1, "UserCmd"));
	if (lcmd == nullptr)
	{
		lua_pushnil(L);
		return 1;
	}

	if (!lcmd->valid)
	{
		luaL_error(L, "Invalid UserCmd!");
		return 0;
	}

	const char* key = luaL_checkstring(L, 2);

	if (strcmp(key, "command_number") == 0)
	{
		lua_pushinteger(L, lcmd->command_number);
		return 1;
	}

	if (strcmp(key, "tick_count") == 0)
	{
		lua_pushinteger(L, lcmd->tick_count);
		return 1;
	}

	if (strcmp(key, "viewangles") == 0)
	{
		LuaClasses::Vector3::push(L, lcmd->viewangles); //lua_pushinteger(L, lcmd->viewangles);
		return 1;
	}

	if (strcmp(key, "forwardmove") == 0)
	{
		lua_pushnumber(L, lcmd->forwardmove);
		return 1;
	}

	if (strcmp(key, "sidemove") == 0)
	{
		lua_pushnumber(L, lcmd->sidemove);
		return 1;
	}

	if (strcmp(key, "upmove") == 0)
	{
		lua_pushnumber(L, lcmd->upmove);
		return 1;
	}

	if (strcmp(key, "buttons") == 0)
	{
		lua_pushinteger(L, lcmd->buttons);
		return 1;
	}

	if (strcmp(key, "impulse") == 0)
	{
		lua_pushinteger(L, lcmd->impulse);
		return 1;
	}

	if (strcmp(key, "weaponselect") == 0)
	{
		lua_pushinteger(L, lcmd->weaponselect);
		return 1;
	}

	if (strcmp(key, "weaponsubtype") == 0)
	{
		lua_pushinteger(L, lcmd->weaponsubtype);
		return 1;
	}

	if (strcmp(key, "random_seed") == 0)
	{
		lua_pushinteger(L, lcmd->random_seed);
		return 1;
	}

	if (strcmp(key, "server_random_seed") == 0)
	{
		lua_pushinteger(L, lcmd->server_random_seed);
		return 1;
	}

	if (strcmp(key, "mousedx") == 0)
	{
		lua_pushinteger(L, lcmd->mousedx);
		return 1;
	}

	if (strcmp(key, "mousedy") == 0)
	{
		lua_pushinteger(L, lcmd->mousedy);
		return 1;
	}

	if (strcmp(key, "hasbeenpredicted") == 0)
	{
		lua_pushinteger(L, lcmd->hasbeenpredicted);
		return 1;
	}

	if (strcmp(key, "sendpacket") == 0)
	{
		lua_pushboolean(L, lcmd->sendpacket);
		return 1;
	}

	// didn't find a field
	luaL_error(L, "Couldn't find field %s", key);
	return 0;
}

int LuaUserCmd::__newindex(lua_State* L)
{
	LuaUserCmd* lcmd = static_cast<LuaUserCmd*>(luaL_checkudata(L, 1, "UserCmd"));
	if (lcmd == nullptr)
	{
		luaL_error(L, "UserCmd is nullptr!");
		return 0;
	}

	if (!lcmd->valid)
	{
		luaL_error(L, "Invalid UserCmd!");
		return 0;
	}

	const char* key = luaL_checkstring(L, 2);

	if (strcmp(key, "command_number") == 0)
	{
		int value = luaL_checkinteger(L, 3);
		lcmd->command_number = value;
		return 0;
	}

	if (strcmp(key, "tick_count") == 0)
	{
		int value = luaL_checkinteger(L, 3);
		lcmd->tick_count = value;
		return 0;
	}

	if (strcmp(key, "viewangles") == 0)
	{
		Vector* value = static_cast<Vector*>(luaL_checkudata(L, 3, "Vector3"));
		lcmd->viewangles = *value;
		return 0;
	}

	if (strcmp(key, "forwardmove") == 0)
	{
		int value = luaL_checknumber(L, 3);
		lcmd->forwardmove = value;
		return 0;
	}

	if (strcmp(key, "sidemove") == 0)
	{
		int value = luaL_checknumber(L, 3);
		lcmd->sidemove = value;
		return 0;
	}

	if (strcmp(key, "upmove") == 0)
	{
		int value = luaL_checknumber(L, 3);
		lcmd->upmove = value;
		return 0;
	}

	if (strcmp(key, "buttons") == 0)
	{
		int value = luaL_checkinteger(L, 3);
		lcmd->buttons = value;
		return 0;
	}

	if (strcmp(key, "impulse") == 0)
	{
		int value = luaL_checkinteger(L, 3);
		lcmd->impulse = value;
		return 0;
	}

	if (strcmp(key, "weaponselect") == 0)
	{
		int value = luaL_checkinteger(L, 3);
		lcmd->weaponselect = value;
		return 0;
	}

	if (strcmp(key, "weaponsubtype") == 0)
	{
		int value = luaL_checkinteger(L, 3);
		lcmd->weaponsubtype = value;
		return 0;
	}

	if (strcmp(key, "random_seed") == 0)
	{
		int value = luaL_checkinteger(L, 3);
		lcmd->random_seed = value;
		return 0;
	}

	if (strcmp(key, "server_random_seed") == 0)
	{
		int value = luaL_checkinteger(L, 3);
		lcmd->server_random_seed = value;
		return 0;
	}

	if (strcmp(key, "mousedx") == 0)
	{
		int value = luaL_checkinteger(L, 3);
		lcmd->mousedx = value;
		return 0;
	}

	if (strcmp(key, "mousedy") == 0)
	{
		int value = luaL_checkinteger(L, 3);
		lcmd->mousedy = value;
		return 0;
	}

	if (strcmp(key, "hasbeenpredicted") == 0)
	{
		int value = luaL_checkinteger(L, 3);
		lcmd->hasbeenpredicted = value;
		return 0;
	}

	if (strcmp(key, "sendpacket") == 0)
	{
		if (!lua_isboolean(L, 3))
			return 0;

		int value = lua_toboolean(L, 3);
		lcmd->sendpacket = value;
		return 0;
	}

	return 0;
}