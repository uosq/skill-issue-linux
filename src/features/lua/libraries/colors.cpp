#include "../libraries.h"
#include "../../../settings.h"

namespace LuaFuncs
{
	namespace colors
	{
		const luaL_Reg colorslib[]
		{
			{"GetAimbotTargetColor", GetAimbotTargetColor},
			{"GetRedTeamColor", GetRedTeamColor},
			{"GetBluTeamColor", GetBluTeamColor},
			{"GetWeaponColor", GetWeaponColor},
			{nullptr, nullptr}
		};

		void luaopen_colors(lua_State* L)
		{
			lua_newtable(L);
			luaL_setfuncs(L, colorslib, 0);
			lua_setglobal(L, "visuals");
		}

		int GetAimbotTargetColor(lua_State* L)
		{
			lua_newtable(L);

			lua_pushinteger(L, g_Settings.colors.aimbot_target.r());
			lua_setfield(L, -2, "r");

			lua_pushinteger(L, g_Settings.colors.aimbot_target.g());
			lua_setfield(L, -2, "g");

			lua_pushinteger(L, g_Settings.colors.aimbot_target.b());
			lua_setfield(L, -2, "b");

			lua_pushinteger(L, g_Settings.colors.aimbot_target.a());
			lua_setfield(L, -2, "a");

			return 1;
		}

		int GetRedTeamColor(lua_State* L)
		{
			lua_newtable(L);

			lua_pushinteger(L, g_Settings.colors.red_team.r());
			lua_setfield(L, -2, "r");

			lua_pushinteger(L, g_Settings.colors.red_team.g());
			lua_setfield(L, -2, "g");

			lua_pushinteger(L, g_Settings.colors.red_team.b());
			lua_setfield(L, -2, "b");

			lua_pushinteger(L, g_Settings.colors.red_team.a());
			lua_setfield(L, -2, "a");

			return 1;
		}

		int GetBluTeamColor(lua_State* L)
		{
			lua_newtable(L);

			lua_pushinteger(L, g_Settings.colors.blu_team.r());
			lua_setfield(L, -2, "r");

			lua_pushinteger(L, g_Settings.colors.blu_team.g());
			lua_setfield(L, -2, "g");

			lua_pushinteger(L, g_Settings.colors.blu_team.b());
			lua_setfield(L, -2, "b");

			lua_pushinteger(L, g_Settings.colors.blu_team.a());
			lua_setfield(L, -2, "a");

			return 1;
		}

		int GetWeaponColor(lua_State* L)
		{
			lua_newtable(L);

			lua_pushinteger(L, g_Settings.colors.weapon.r());
			lua_setfield(L, -2, "r");

			lua_pushinteger(L, g_Settings.colors.weapon.g());
			lua_setfield(L, -2, "g");

			lua_pushinteger(L, g_Settings.colors.weapon.b());
			lua_setfield(L, -2, "b");

			lua_pushinteger(L, g_Settings.colors.weapon.a());
			lua_setfield(L, -2, "a");

			return 1;
		}
	}
}