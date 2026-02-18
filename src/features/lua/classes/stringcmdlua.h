#pragma once

#include <cstdint>
#include <string>

#include "../pluto/lua.hpp"
#include "../pluto/lauxlib.h"
#include "../pluto/lualib.h"

struct LuaStringCmd
{
	std::string cmd;
	bool valid;

	// methods
	static int Get(lua_State* L);
	static int Set(lua_State* L);

	// metamethods
	static int __gc(lua_State* L);

	constexpr static const luaL_Reg methods[]
	{
		{"Get", Get},
		{"Set", Set},
		{nullptr, nullptr}
	};
};

namespace LuaClasses
{
	namespace StringCmd
	{
		void open(lua_State* L);
		LuaStringCmd* push(lua_State* L, const std::string& pCmd);
	}
}