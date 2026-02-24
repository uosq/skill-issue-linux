#include "stringcmdlua.h"

int LuaStringCmd::Get(lua_State *L)
{
	LuaStringCmd* lcmd = static_cast<LuaStringCmd*>(luaL_checkudata(L, 1, "StringCmd"));
	if (!lcmd->valid)
		luaL_error(L, "Invalid StringCmd");

	lua_pushlstring(L, lcmd->cmd.c_str(), lcmd->cmd.length());
	return 1;
}

int LuaStringCmd::Set(lua_State *L)
{
	LuaStringCmd* lcmd = static_cast<LuaStringCmd*>(luaL_checkudata(L, 1, "StringCmd"));
	if (!lcmd->valid)
		luaL_error(L, "Invalid StringCmd");

	lcmd->cmd = luaL_checkstring(L, 2);
	return 0;
}

int LuaStringCmd::__gc(lua_State *L)
{
	LuaStringCmd* lcmd = static_cast<LuaStringCmd*>(luaL_checkudata(L, 1, "StringCmd"));
	lcmd->valid = false;
	lcmd->~LuaStringCmd();
	return 0;
}

namespace LuaClasses
{
	namespace StringCmd
	{
		void open(lua_State* L)
		{
			luaL_newmetatable(L, "StringCmd");

			luaL_setfuncs(L, LuaStringCmd::methods, 0);

			lua_pushvalue(L, -1);
			lua_setfield(L, -2, "__index");

			lua_pushcfunction(L, LuaStringCmd::__gc);
			lua_setfield(L, -2, "__gc");

			lua_pop(L, 1);

			lua_pop(L, 1);
		}

		LuaStringCmd* push(lua_State* L, const std::string& cmd)
		{
			LuaStringCmd* lcmd = static_cast<LuaStringCmd*>(lua_newuserdata(L, sizeof(LuaStringCmd)));
			new (lcmd) LuaStringCmd{cmd, true};

			luaL_getmetatable(L, "StringCmd");
			lua_setmetatable(L, -2);

			return lcmd;
		}
	}
}