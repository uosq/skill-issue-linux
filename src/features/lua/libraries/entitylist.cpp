#include "../libraries.h"
#include "../classes/entitylua.h"
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
			{"GetByClassID", GetByClassID},
			{"GetEntityByID", GetEntityByID},
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

		// entities.GetEntitiesByID(217) -- 217 = stickies
		int GetByClassID(lua_State* L)
		{
			int id = luaL_checkinteger(L, 1);

			lua_newtable(L);

			for (int i = 1; i <= interfaces::EntityList->GetHighestEntityIndex(); i++)
			{
				CBaseEntity* pEntity = static_cast<CBaseEntity*>(interfaces::EntityList->GetClientEntity(i));
				if (pEntity == nullptr)
					continue;

				if (static_cast<int>(pEntity->GetClassID()) != id)
					continue;

				LuaClasses::EntityLua::push_entity(L, pEntity);
				lua_rawseti(L, -2, i);
			}

			return 1;
		}

		int GetEntityByID(lua_State *L)
		{
			int iUserID = luaL_checkinteger(L, 1);

			for (int i = 1; i <= interfaces::GlobalVars->max_clients; i++)
			{
				CTFPlayer* pPlayer = static_cast<CTFPlayer*>(interfaces::EntityList->GetClientEntity(i));
				if (pPlayer == nullptr || !pPlayer->IsPlayer())
					continue;

				player_info_t pInfo;
				if (!interfaces::Engine->GetPlayerInfo(i, &pInfo))
					continue;

				if (pInfo.userID == iUserID)
				{
					LuaClasses::EntityLua::push_entity(L, pPlayer);
					return 1;
				}
			}

			lua_pushnil(L);
			return 1;
		}
	}
}