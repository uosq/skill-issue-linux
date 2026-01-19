#pragma once

#include "pluto/lua.hpp"
#include "pluto/lualib.h"

#define REGISTER_LUA_CONSTANT_INT(name, value) \
	lua_pushinteger(L, value); \
	lua_setglobal(L, name);

void luaregister_constants(lua_State* L);