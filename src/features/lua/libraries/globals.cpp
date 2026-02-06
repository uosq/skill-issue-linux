#include "../libraries.h"

namespace LuaFuncs
{
	namespace globalvars
	{
		const luaL_Reg globalvarslib[] =
		{
			{"TickCount", TickCount},
			{"TickInterval", TickInterval},
			{"CurTime", CurTime},
			{"AbsoluteFrameTime", AbsoluteFrameTime},
			{"FrameTime", FrameTime},
			{"RealTime", RealTime},
			{"MaxClients", MaxClients},
			{nullptr, nullptr},
		};

		void luaopen_globalvars(lua_State* L)
		{
			lua_newtable(L);
			luaL_setfuncs(L, globalvarslib, 0);
			lua_setglobal(L, "globals");

			//consoleText += "globals library initialized\n";
		}

		int TickCount(lua_State* L)
		{
			lua_pushinteger(L, interfaces::GlobalVars != nullptr ? interfaces::GlobalVars->tickcount : 0);
			return 1;
		}

		int TickInterval(lua_State* L)
		{
			lua_pushnumber(L, interfaces::GlobalVars != nullptr ? interfaces::GlobalVars->interval_per_tick : 0);
			return 1;
		}

		int CurTime(lua_State* L)
		{
			lua_pushnumber(L, interfaces::GlobalVars != nullptr ? interfaces::GlobalVars->curtime : 0);
			return 1;
		}

		int AbsoluteFrameTime(lua_State* L)
		{
			lua_pushnumber(L, interfaces::GlobalVars != nullptr ? interfaces::GlobalVars->absolute_frametime : 0);
			return 1;
		}

		int FrameTime(lua_State* L)
		{
			lua_pushnumber(L, interfaces::GlobalVars != nullptr ? interfaces::GlobalVars->frametime : 0);
			return 1;
		}

		int RealTime(lua_State* L)
		{
			lua_pushnumber(L, interfaces::GlobalVars != nullptr ? interfaces::GlobalVars->realtime : 0);
			return 1;
		}

		int MaxClients(lua_State* L)
		{
			lua_pushnumber(L, interfaces::GlobalVars != nullptr ? interfaces::GlobalVars->max_clients : 0);
			return 1;
		}
	}
}