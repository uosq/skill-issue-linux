#include "../classes.h"
#include "../libraries.h"
#include "../../entitylist/entitylist.h"
#include "../../../settings.h"
#include "../../aimbot/utils/utils.h"
#include "../../aimbot/aimbot.h"

namespace LuaFuncs
{
	namespace aimbot
	{
		const luaL_Reg aimbotlib[]
		{
			{"GetTarget", GetTarget},
			{"SetTarget", SetTarget},
			{"GetMode", GetMode},
			{"SetMode", SetMode},
			{"GetFOV", GetFOV},
			{"SetFOV", SetFOV},
			{"IsEntityValid", IsEntityValid},
			{"IsRunning", IsRunning},
			{"GetKey", GetKey},
			{"SetKey", SetKey},
			{nullptr, nullptr}
		};

		void luaopen_aimbot(lua_State* L)
		{
			lua_newtable(L);
			luaL_setfuncs(L, aimbotlib, 0);
			lua_setglobal(L, "aimbot");
		}

		int GetTarget(lua_State* L)
		{
			CBaseEntity* pTarget = EntityList::m_pAimbotTarget;
			if (pTarget == nullptr)
			{
				lua_pushnil(L);
				return 1;
			}

			LuaClasses::EntityLua::push_entity(L, pTarget);
			return 1;
		}

		int SetTarget(lua_State* L)
		{
			if (lua_isnoneornil(L, 1))
			{
				EntityList::m_pAimbotTarget = nullptr;
				return 0;
			}

			LuaEntity* le = static_cast<LuaEntity*>(luaL_checkudata(L, 1, "Entity"));
			if (le->ent == nullptr)
			{
				EntityList::m_pAimbotTarget = nullptr;
				return 0;
			}

			EntityList::m_pAimbotTarget = le->ent;
			return 0;
		}

		int GetMode(lua_State* L)
		{
			lua_pushinteger(L, int(g_Settings.aimbot.mode));
			lua_pushstring(L, AimbotUtils::GetAimbotModeName().c_str());
			return 2;
		}

		int SetMode(lua_State* L)
		{
			int mode = luaL_checkinteger(L, 1);
			if (int(AimbotMode::INVALID) >= mode || int(AimbotMode::MAX) <= mode )
			{
				luaL_error(L, "Aimbot mode must be in a valid range!");
				return 0;
			}

			g_Settings.aimbot.mode = AimbotMode(mode);
			return 0;
		}

		int GetFOV(lua_State* L)
		{
			lua_pushnumber(L, AimbotUtils::GetAimbotFovScaled());
			return 1;
		}

		int SetFOV(lua_State* L)
		{
			float fov = luaL_checknumber(L, 1);
			if (fov > 180.0f || fov < 0.0f)
			{
				luaL_error(L, "Aimbot FOV must be in a valid range! (0, 180)");
				return 0;
			}

			return 0;
		}

		int IsEntityValid(lua_State* L)
		{
			if (lua_isnoneornil(L, 1))
			{
				lua_pushboolean(L, false);
				return 1;
			}

			LuaEntity* le = static_cast<LuaEntity*>(luaL_checkudata(L, 1, "Entity"));
			if (le->ent == nullptr)
			{
				lua_pushboolean(L, false);
				return 1;
			}

			const auto& pLocal = EntityList::m_pLocalPlayer;
			if (pLocal == nullptr)
			{
				lua_pushboolean(L, false);
				return 1;
			}

			lua_pushboolean(L, AimbotUtils::IsValidEntity(le->ent));
			return 1;
		}

		int IsRunning(lua_State* L)
		{
			lua_pushboolean(L, Aimbot::m_state.running);
			return 1;
		}

		int GetKey(lua_State* L)
		{
			const std::string& key = g_Settings.aimbot.key;
			ButtonCode_t btn = interfaces::InputSystem->StringToButtonCode(key.c_str());

			if (!helper::input::IsButtonValid(btn))
			{
				lua_pushnil(L);
				lua_pushnil(L);
				return 2;
			}

			lua_pushinteger(L, static_cast<lua_Integer>(btn));
			lua_pushstring(L, key.c_str());
			return 2;
		}

		int SetKey(lua_State* L)
		{
			if (lua_isnoneornil(L, 1))
				return 0;

			if (lua_isstring(L, 1))
			{
				const char* str = luaL_checkstring(L, 1);
				ButtonCode_t btn = interfaces::InputSystem->StringToButtonCode(str);

				if (helper::input::IsButtonValid(btn))
					g_Settings.aimbot.key = str;

				return 0;
			}

			if (lua_isinteger(L, 1))
			{
				const int key = luaL_checkinteger(L, 1);
				ButtonCode_t btn = interfaces::InputSystem->VirtualKeyToButtonCode(key);
				
				if (helper::input::IsButtonValid(btn))
					g_Settings.aimbot.key = interfaces::InputSystem->ButtonCodeToString(btn);

				return 0;
			}

			return 0;
		}
	}
}