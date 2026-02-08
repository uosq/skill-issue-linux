#include "../classes.h"

namespace LuaClasses
{
	namespace StringCmdLua
	{
		const luaL_Reg methods[]
		{
			{"Get", Get},
			{"Set", Set},
			{nullptr, nullptr}
		};

		void open(lua_State* L)
		{
			luaL_newmetatable(L, "StringCmd");
			luaL_setfuncs(L, methods, 0);

			lua_pushvalue(L, -1);
			lua_setfield(L, -2, "__index");

			lua_pushcfunction(L, GC);
			lua_setfield(L, -2, "__gc");

			lua_pop(L, 1);
		}

		LuaStringCmd* push(lua_State* L, std::string cmd)
		{
			LuaStringCmd* lcmd = static_cast<LuaStringCmd*>(lua_newuserdata(L, sizeof(LuaStringCmd)));
			new (lcmd) LuaStringCmd{cmd, true};

			luaL_getmetatable(L, "StringCmd");
			lua_setmetatable(L, -2);

			return lcmd;
		}

		int Get(lua_State* L)
		{
			LuaStringCmd* lcmd = static_cast<LuaStringCmd*>(luaL_checkudata(L, 1, "StringCmd"));
			if (!lcmd->valid)
			{
				luaL_error(L, "Invalid object!");
				return 0;
			}

			lua_pushlstring(L, lcmd->cmd.c_str(), lcmd->cmd.length());
			return 1;
		}

		int Set(lua_State* L)
		{
			LuaStringCmd* lcmd = static_cast<LuaStringCmd*>(luaL_checkudata(L, 1, "StringCmd"));
			if (!lcmd->valid)
			{
				luaL_error(L, "Invalid object!");
				return 0;
			}

			std::string newCmd = luaL_checkstring(L, 2);
			lcmd->cmd = newCmd;
			return 0;
		}

		int GC(lua_State* L)
		{
			LuaStringCmd* lcmd = static_cast<LuaStringCmd*>(luaL_checkudata(L, 1, "StringCmd"));
			lcmd->valid = false;
			lcmd->cmd.clear();
			lcmd->~LuaStringCmd();
			return 0;
		}
	}
}