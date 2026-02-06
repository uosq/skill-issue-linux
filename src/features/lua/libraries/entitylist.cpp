#include "../libraries.h"
#include "../classes.h"
#include "../../../features/entitylist/entitylist.h"

namespace LuaFuncs
{
	namespace entitylist
	{
		const luaL_Reg entitylistlib[] =
		{
			{"GetAtIndex", GetEntityAtIndex},
			{"GetLocalPlayer", GetLocalPlayer},
			{"GetHighestIndex", GetHighestEntityIndex},
			{"GetPlayers", GetPlayers},
			{"GetTeammates", GetTeammates},
			{"GetEnemies", GetEnemies},
			{"GetActiveWeapon", GetActiveWeapon},
			{nullptr, nullptr}
		};

		void luaopen_entitylist(lua_State* L)
		{
			lua_newtable(L);
			luaL_setfuncs(L, entitylistlib, 0);
			lua_setglobal(L, "entities");
		}

		int GetEntityAtIndex(lua_State* L)
		{
			if (!interfaces::Engine->IsInGame() || !interfaces::Engine->IsConnected())
			{
				lua_pushnil(L);
				return 1;
			}

			int entindex = static_cast<int>(luaL_checkinteger(L, 1));
			CBaseEntity* entity = reinterpret_cast<CBaseEntity*>(interfaces::EntityList->GetClientEntity(entindex));
			if (entity == nullptr)
			{
				lua_pushnil(L);
				return 1;
			}

			LuaClasses::EntityLua::push_entity(L, entity);
			return 1;
		}

		int GetLocalPlayer(lua_State* L)
		{
			if (!interfaces::Engine->IsInGame() || !interfaces::Engine->IsConnected())
			{
				lua_pushnil(L);
				return 1;
			}

			CTFPlayer* pLocal = EntityList::m_pLocalPlayer;
			if (pLocal == nullptr)
			{
				lua_pushnil(L);
				return 1;
			}

			LuaClasses::EntityLua::push_entity(L, pLocal);
			return 1;
		}

		int GetHighestEntityIndex(lua_State* L)
		{
			if (!interfaces::Engine->IsInGame() || !interfaces::Engine->IsConnected())
			{
				lua_pushinteger(L, 0);
				return 1;
			}

			lua_pushinteger(L, interfaces::EntityList->GetHighestEntityIndex());
			return 1;
		}

		int GetPlayers(lua_State* L)
		{
			const auto& entities = EntityList::GetEntities();

			lua_newtable(L);

			int index = 1; // lua arrays start at 1

			for (auto& entry : entities)
			{
				if (entry.ptr == nullptr)
					continue;

				if (!(entry.flags & EntityFlags::IsPlayer))
					continue;

				LuaClasses::EntityLua::push_entity(L, entry.ptr);
				lua_rawseti(L, -2, index++);
			}

			return 1; // return the table
		}

		int GetTeammates(lua_State* L)
		{
			const auto& entities = EntityList::GetEntities();

			lua_newtable(L);

			int index = 1; // lua arrays start at 1

			for (auto& entry : entities)
			{
				if (entry.ptr == nullptr)
					continue;

				if (entry.flags & EntityFlags::IsEnemy)
					continue;

				LuaClasses::EntityLua::push_entity(L, entry.ptr);
				lua_rawseti(L, -2, index++);
			}

			return 1; // return the table
		}

		int GetEnemies(lua_State* L)
		{
			const auto& entities = EntityList::GetEntities();

			lua_newtable(L);

			int index = 1; // lua arrays start at 1

			for (auto& entry : entities)
			{
				if (entry.ptr == nullptr)
					continue;

				if (!(entry.flags & EntityFlags::IsEnemy))
					continue;

				LuaClasses::EntityLua::push_entity(L, entry.ptr);
				lua_rawseti(L, -2, index++);
			}

			return 1; // return the table
		}

		int GetActiveWeapon(lua_State* L)
		{
			if (!interfaces::Engine->IsInGame() || !interfaces::Engine->IsConnected())
			{
				lua_pushnil(L);
				return 1;
			}

			CTFPlayer* pLocal = EntityList::m_pLocalPlayer;
			if (pLocal == nullptr)
			{
				lua_pushnil(L);
				return 1;
			}

			CTFWeaponBase* pWeapon = HandleAs<CTFWeaponBase*>(pLocal->GetActiveWeapon());
			if (pWeapon == nullptr)
			{
				lua_pushnil(L);
				return 1;
			}

			LuaClasses::EntityLua::push_entity(L, pWeapon);
			return 1;
		}
	}
}