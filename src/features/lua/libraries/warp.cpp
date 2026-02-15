#include "../libraries.h"
#include "../../warp/warp.h"

namespace LuaFuncs
{
	namespace warp
	{
		const luaL_Reg warplib[]
		{
			{ "IsShifting", IsShifting },
			{ "IsRecharging", IsRecharging },
			{ "GetMaxTicks", GetMaxTicks },
			{ "GetTicks", GetTicks },
			{ nullptr, nullptr }
		};

		void open(lua_State* L)
		{
			lua_newtable(L);
			luaL_setfuncs(L, warplib, 0);
			lua_setglobal(L, "warp");
		}

		int IsShifting(lua_State* L)
		{
			lua_pushboolean(L, Warp::m_bShifting);
			return 0;
		}

		int IsRecharging(lua_State* L)
		{
			lua_pushboolean(L, Warp::m_bRecharging);
			return 0;
		}

		int GetTicks(lua_State* L)
		{
			lua_pushinteger(L, Warp::m_iStoredTicks);
			return 1;
		}

		int GetMaxTicks(lua_State* L)
		{
			lua_pushinteger(L, Warp::GetMaxTicks());
			return 1;
		}
	}
}