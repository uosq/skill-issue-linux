#include "../libraries.h"
#include "../hookmgr.h"

namespace LuaFuncs
{
	namespace hooks
	{
		const luaL_Reg hooklib[] =
		{
			{"Add", Register},
			{"Remove", Unregister},
			{nullptr, nullptr},
		};

		int Register(lua_State* L)
		{
			const char* name = luaL_checkstring(L, 1);
			const char* id = luaL_checkstring(L, 2);

			luaL_checktype(L, 3, LUA_TFUNCTION);
			lua_pushvalue(L, 3);
			int ref = luaL_ref(L, LUA_REGISTRYINDEX);

			lua_pushboolean(L, LuaHookManager::Add(name, id, ref));
			return 1;
		}

		int Unregister(lua_State* L)
		{
			const char* name = luaL_checkstring(L, 1);
			const char* id = luaL_checkstring(L, 2);

			lua_pushboolean(L, LuaHookManager::Remove(L, name, id));
			return 1;
		}

		void luaopen_hooks(lua_State* L)
		{
			lua_newtable(L);
			luaL_setfuncs(L, hooklib, 0);
			lua_setglobal(L, "hooks");

			//consoleText += "hooks library initialized\n";
		}
	}
}