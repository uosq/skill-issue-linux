#include "../libraries.h"

namespace LuaFuncs
{
	namespace client
	{
		const luaL_Reg clientlib[]
		{
			{"GetConVar", GetConVar},
			{"SetConVar", SetConVar},
			{"ChatSay", ChatSay},
			{"Command", Command},
			{"IsClassMenuOpen", IsClassMenuOpen},
			{"ClearConsole", ClearConsole},
			//{"ChatPrintf", ChatPrintf}, Why this not work?? :sob:
			//{"Notification", Notification},
			{nullptr, nullptr}
		};

		void luaopen_client(lua_State* L)
		{
			lua_newtable(L);
			luaL_setfuncs(L, clientlib, 0);
			lua_setglobal(L, "client");
		}

		int GetConVar(lua_State* L)
		{
			int nargs = lua_gettop(L);
			if (nargs != 1)
			{
				luaL_error(L, "Expected 1 argument! Received %d", nargs);
				return 0;
			}

			if (!lua_isstring(L, 1))
			{
				luaL_error(L, "Expected 1st argument as string!");
				return 0;
			}

			ConVar* cvar = interfaces::Cvar->FindVar(luaL_checkstring(L, 1));
			if (cvar == nullptr)
			{
				luaL_error(L, "Couldn't find ConVar!");
				return 0;
			}

			lua_pushinteger(L, cvar->GetInt());
			lua_pushnumber(L, cvar->GetFloat());
			lua_pushstring(L, cvar->GetString());
			return 3;
		}

		int SetConVar(lua_State* L)
		{
			int nargs = lua_gettop(L);
			if (nargs != 2)
			{
				luaL_error(L, "Expected 2 arguments! Received %d", nargs);
				return 0;
			}

			if (!lua_isstring(L, 1))
			{
				luaL_error(L, "Expected 1st argument as string!");
				return 0;
			}

			if (lua_isnoneornil(L, 2))
			{
				luaL_error(L, "Argument #2 is nil!");
				return 0;
			}

			ConVar* cvar = interfaces::Cvar->FindVar(luaL_checkstring(L, 1));
			if (cvar == nullptr)
			{
				luaL_error(L, "Couldn't find ConVar!");
				return 0;
			}

			if (lua_isinteger(L, 2))
				cvar->SetValue(static_cast<int>(luaL_checkinteger(L, 2)));
			else if (lua_isnumber(L, 2))
				cvar->SetValue(static_cast<float>(luaL_checknumber(L, 2)));
			else if (lua_isstring(L, 2))
				cvar->SetValue(luaL_checkstring(L, 2));

			return 0;
		}

		int ChatSay(lua_State* L)
		{
			int nargs = lua_gettop(L);
			if (nargs != 1)
			{
				luaL_error(L, "Expected 1 argument! Received %s", nargs);
				return 0;
			}

			const char* text = luaL_checkstring(L, 1);
			if (text == nullptr)
			{
				luaL_error(L, "Text is a null pointer!");
				return 0;
			}

			interfaces::Engine->ClientCmd((std::string("say ") + text).c_str());
			return 0;
		}

		int Command(lua_State* L)
		{
			int nargs = lua_gettop(L);
			if (nargs == 0 || nargs > 2)
			{
				luaL_error(L, "Expected at least 1 argument! Received %s", nargs);
				return 0;
			}

			if (!lua_isstring(L, 1))
			{
				luaL_error(L, "Expected 1st argument as string!");
				return 0;
			}

			if (lua_istrue(L, 2))
				interfaces::Engine->ClientCmd_Unrestricted(luaL_checkstring(L, 1));
			else
				interfaces::Engine->ClientCmd(luaL_checkstring(L, 1));

			return 0;
		}

		int IsClassMenuOpen(lua_State* L)
		{
			static ConVar* _cl_classmenuopen = interfaces::Cvar->FindVar("_cl_classmenuopen");
			if (_cl_classmenuopen == nullptr)
				return 0;

			lua_pushboolean(L, _cl_classmenuopen->GetInt());
			return 1;
		}

		int ChatPrintf(lua_State* L)
		{
			/*int nargs = lua_gettop(L);
			if (nargs == 0)
				return 0;

			for (int i = 1; i <= nargs; i++)
			{
				const char* str = luaL_tolstring(L, i, nullptr);
				if (str) helper::localplayer::ChatPrintf(0, CHAT_FILTER_NONE, str);
			}*/

			return 0;
		}

		/*int Notification(lua_State* L)
		{
			CEconNotification* notification = new CEconNotification();
			notification->SetText(luaL_checkstring(L, 1));
			notification->SetLifetime(999.0f);
			interfaces::g_notificationQueue->AddNotification(notification);
			return 0;
		}*/

		int ClearConsole(lua_State *L)
		{
			interfaces::EngineVGui->ClearConsole();
			return 0;
		}
	}
}