#include "../libraries.h"
#include "../../../settings/settings.h"

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

			lua_pushinteger(L, Settings::Colors::aimbot_target.r());
			lua_setfield(L, -2, "r");

			lua_pushinteger(L, Settings::Colors::aimbot_target.g());
			lua_setfield(L, -2, "g");

			lua_pushinteger(L, Settings::Colors::aimbot_target.b());
			lua_setfield(L, -2, "b");

			lua_pushinteger(L, Settings::Colors::aimbot_target.a());
			lua_setfield(L, -2, "a");

			return 1;
		}

		int GetRedTeamColor(lua_State* L)
		{
			lua_newtable(L);

			lua_pushinteger(L, Settings::Colors::red_team.r());
			lua_setfield(L, -2, "r");

			lua_pushinteger(L, Settings::Colors::red_team.g());
			lua_setfield(L, -2, "g");

			lua_pushinteger(L, Settings::Colors::red_team.b());
			lua_setfield(L, -2, "b");

			lua_pushinteger(L, Settings::Colors::red_team.a());
			lua_setfield(L, -2, "a");

			return 1;
		}

		int GetBluTeamColor(lua_State* L)
		{
			lua_newtable(L);

			lua_pushinteger(L, Settings::Colors::blu_team.r());
			lua_setfield(L, -2, "r");

			lua_pushinteger(L, Settings::Colors::blu_team.g());
			lua_setfield(L, -2, "g");

			lua_pushinteger(L, Settings::Colors::blu_team.b());
			lua_setfield(L, -2, "b");

			lua_pushinteger(L, Settings::Colors::blu_team.a());
			lua_setfield(L, -2, "a");

			return 1;
		}

		int GetWeaponColor(lua_State* L)
		{
			lua_newtable(L);

			lua_pushinteger(L, Settings::Colors::weapon.r());
			lua_setfield(L, -2, "r");

			lua_pushinteger(L, Settings::Colors::weapon.g());
			lua_setfield(L, -2, "g");

			lua_pushinteger(L, Settings::Colors::weapon.b());
			lua_setfield(L, -2, "b");

			lua_pushinteger(L, Settings::Colors::weapon.a());
			lua_setfield(L, -2, "a");

			return 1;
		}
	}
}