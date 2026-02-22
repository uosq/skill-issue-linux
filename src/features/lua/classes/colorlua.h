#pragma once

#include <cstdint>
#include "../../../sdk/definitions/color.h"

#include "../pluto/lua.hpp"

#define LUA_COLOR_METATABLE "Color"

struct LuaColor
{
	uint8_t r, g, b, a;

	LuaColor()
	{
		r = 0;
		g = 0;
		b = 0;
		a = 0;
	}

	void CopyFromColor(const Color& in);
	void WriteToColor(Color& out);

	static int __newindex(lua_State* L);
	static int __index(lua_State* L);
	static int __gc(lua_State* L);

	static int Set(lua_State* L);

	static int Color3(lua_State* L);
};

namespace LuaClasses
{
	namespace Color
	{
		void open(lua_State* L);
		LuaColor* push(lua_State* L, struct Color& color);
	}
}