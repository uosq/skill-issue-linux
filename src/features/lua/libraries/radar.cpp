#include "../libraries.h"
#include "../classes.h"
#include "../../radar/radar.h"

namespace LuaFuncs
{
	namespace radar
	{
		const luaL_Reg radarlib[]
		{
			{"GetRange", GetRange},
			{"GetRadius", GetRadius},
			{"WorldToRadar", WorldToRadar},
			{nullptr, nullptr}
		};

		void luaopen_radar(lua_State* L)
		{
			lua_newtable(L);
			luaL_setfuncs(L, radarlib, 0);
			lua_setglobal(L, "radar");
		}

		int GetRange(lua_State* L)
		{
			lua_pushinteger(L, Radar::GetRange());
			return 1;
		}

		int GetRadius(lua_State* L)
		{
			lua_pushnumber(L, Radar::GetRadius());
			return 1;
		}

		int WorldToRadar(lua_State* L)
		{
			Vector* localPos = static_cast<Vector*>(luaL_checkudata(L, 1, "Vector3"));
			if (localPos == nullptr)
			{
				luaL_error(L, "Argument #1: expected Vector3, received nullptr");
				return 1;
			}

			Vector* enemyPos = static_cast<Vector*>(luaL_checkudata(L, 2, "Vector3"));
			if (enemyPos == nullptr)
			{
				luaL_error(L, "Argument #2: expected Vector3, received nullptr");
				return 1;
			}

			float viewAnglesYaw = 0;
			if (lua_isnoneornil(L, 3))
			{
				Vector viewAngles;
				interfaces::Engine->GetViewAngles(viewAngles);
				viewAnglesYaw = viewAngles.y;
			}
			else
			{
				viewAnglesYaw = luaL_checknumber(L, 3);
			}

			Vec2 pos = Radar::WorldToRadar(*localPos, *enemyPos, viewAnglesYaw - 90.0f);
			Vector ret = {pos.x, pos.y, 0.0f}; // dont implicitly convert
			LuaClasses::VectorLua::push_vector(L, ret);
			return 1;
		}
	}
}