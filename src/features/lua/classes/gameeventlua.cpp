#include "../classes.h"
#include "../pluto/lauxlib.h"

namespace LuaClasses
{
	namespace GameEventLua
	{
		const luaL_Reg methods[]
		{
			{"GetName", GetName},
			{"IsReliable", IsReliable},
			{"IsLocal", IsLocal},
			{"IsEmpty", IsEmpty},
			{"GetBool", GetBool},
			{"GetInt", GetInt},
			{"GetFloat", GetFloat},
			{"GetString", GetString},
			{"SetBool", SetBool},
			{"SetInt", SetInt},
			{"SetFloat", SetFloat},
			{"SetString", SetString},
			{nullptr, nullptr}
		};

		void luaopen_gameevent(lua_State* L)
		{
			luaL_newmetatable(L, "GameEvent");

			luaL_setfuncs(L, methods, 0);

			lua_pushcfunction(L, Index);
			lua_setfield(L, -2, "__index");

			lua_pushcfunction(L, GC);
			lua_setfield(L, -2, "__gc");
		}

		LuaGameEvent* push_gameevent(lua_State* L, IGameEvent* event)
		{
			LuaGameEvent* levent = static_cast<LuaGameEvent*>(lua_newuserdata(L, sizeof(LuaGameEvent)));
			new (levent) LuaGameEvent{event};

			luaL_getmetatable(L, "GameEvent");
			lua_setmetatable(L, -2);

			return levent;
		}

		int Index(lua_State* L)
		{
			luaL_getmetatable(L, "GameEvent");
			lua_pushvalue(L, 2);
			lua_rawget(L, -2);
			return 1;
		}

		int GC(lua_State* L)
		{
			LuaGameEvent* levent = static_cast<LuaGameEvent*>(luaL_checkudata(L, 1, "GameEvent"));
			levent->~LuaGameEvent();
			return 0;
		}

		int GetName(lua_State* L)
		{
			LuaGameEvent* levent = static_cast<LuaGameEvent*>(luaL_checkudata(L, 1, "GameEvent"));
			if (levent->event == nullptr)
			{
				lua_pushnil(L);
				return 1;
			}

			lua_pushstring(L, levent->event->GetName());
			return 1;
		}

		int IsReliable(lua_State* L)
		{
			LuaGameEvent* levent = static_cast<LuaGameEvent*>(luaL_checkudata(L, 1, "GameEvent"));
			if (levent->event == nullptr)
			{
				lua_pushnil(L);
				return 1;
			}

			lua_pushboolean(L, levent->event->IsReliable());
			return 1;
		}

		int IsLocal(lua_State* L)
		{
			LuaGameEvent* levent = static_cast<LuaGameEvent*>(luaL_checkudata(L, 1, "GameEvent"));
			if (levent->event == nullptr)
			{
				lua_pushnil(L);
				return 1;
			}

			lua_pushboolean(L, levent->event->IsLocal());
			return 1;
		}

		int IsEmpty(lua_State* L)
		{
			LuaGameEvent* levent = static_cast<LuaGameEvent*>(luaL_checkudata(L, 1, "GameEvent"));
			if (levent->event == nullptr)
			{
				lua_pushnil(L);
				return 1;
			}

			const char* key = luaL_checkstring(L, 2);
			lua_pushboolean(L, levent->event->IsEmpty(key));
			return 1;
		}
		
		int GetBool(lua_State* L)
		{
			LuaGameEvent* levent = static_cast<LuaGameEvent*>(luaL_checkudata(L, 1, "GameEvent"));
			if (levent->event == nullptr)
			{
				lua_pushnil(L);
				return 1;
			}

			const char* key = luaL_checkstring(L, 2);
			lua_pushboolean(L, levent->event->GetBool(key));
			return 1;
		}

		int GetInt(lua_State* L)
		{
			LuaGameEvent* levent = static_cast<LuaGameEvent*>(luaL_checkudata(L, 1, "GameEvent"));
			if (levent->event == nullptr)
			{
				lua_pushnil(L);
				return 1;
			}

			const char* key = luaL_checkstring(L, 2);
			lua_pushinteger(L, levent->event->GetInt(key));
			return 1;
		}

		int GetFloat(lua_State* L)
		{
			LuaGameEvent* levent = static_cast<LuaGameEvent*>(luaL_checkudata(L, 1, "GameEvent"));
			if (levent->event == nullptr)
			{
				lua_pushnil(L);
				return 1;
			}

			const char* key = luaL_checkstring(L, 2);
			lua_pushnumber(L, levent->event->GetFloat(key));
			return 1;
		}

		int GetString(lua_State* L)
		{
			LuaGameEvent* levent = static_cast<LuaGameEvent*>(luaL_checkudata(L, 1, "GameEvent"));
			if (levent->event == nullptr)
			{
				lua_pushnil(L);
				return 1;
			}

			const char* key = luaL_checkstring(L, 2);
			lua_pushstring(L, levent->event->GetString(key));
			return 1;
		}

		int SetBool(lua_State* L)
		{
			LuaGameEvent* levent = static_cast<LuaGameEvent*>(luaL_checkudata(L, 1, "GameEvent"));
			if (levent->event == nullptr)
			{
				lua_pushnil(L);
				return 1;
			}

			const char* key = luaL_checkstring(L, 2);
			bool val = luaL_checkinteger(L, 3);
			levent->event->SetBool(key, val);
			return 0;
		}

		int SetInt(lua_State* L)
		{
			LuaGameEvent* levent = static_cast<LuaGameEvent*>(luaL_checkudata(L, 1, "GameEvent"));
			if (levent->event == nullptr)
			{
				lua_pushnil(L);
				return 1;
			}

			const char* key = luaL_checkstring(L, 2);
			int val = luaL_checkinteger(L, 3);
			levent->event->SetInt(key, val);
			return 0;
		}

		int SetFloat(lua_State* L)
		{
			LuaGameEvent* levent = static_cast<LuaGameEvent*>(luaL_checkudata(L, 1, "GameEvent"));
			if (levent->event == nullptr)
			{
				lua_pushnil(L);
				return 1;
			}

			const char* key = luaL_checkstring(L, 2);
			float val = luaL_checknumber(L, 3);
			levent->event->SetFloat(key, val);
			return 0;
		}

		int SetString(lua_State* L)
		{
			LuaGameEvent* levent = static_cast<LuaGameEvent*>(luaL_checkudata(L, 1, "GameEvent"));
			if (levent->event == nullptr)
			{
				lua_pushnil(L);
				return 1;
			}

			const char* key = luaL_checkstring(L, 2);
			const char* val = luaL_checkstring(L, 3);
			levent->event->SetString(key, val);
			return 0;
		}
	}
}