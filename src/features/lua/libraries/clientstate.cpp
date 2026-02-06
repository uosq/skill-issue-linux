#include "../libraries.h"
#include "../../../sdk/definitions/cclientstate.h"

namespace LuaFuncs
{
	namespace clientstate
	{
		const luaL_Reg clientstatelib[]
		{
			{"GetChokedCommands", GetChokedCommands},
			{"GetLastOutgoingCommand", GetLastOutgoingCommand},
			{"GetClientSignonState", GetClientSignonState},
			{"GetDeltaTick", GetDeltaTick},
			{"GetLastCommandAck", GetLastCommandAck},
			{"ForceFullUpdate", ForceFullUpdate},
			{nullptr, nullptr}
		};

		void luaopen_clientstate(lua_State* L)
		{
			lua_newtable(L);
			luaL_setfuncs(L, clientstatelib, 0);
			lua_setglobal(L, "clientstate");
		}

		int GetChokedCommands(lua_State* L)
		{
			if (interfaces::ClientState == nullptr)
			{
				lua_pushnil(L);
				return 1;
			}

			lua_pushinteger(L, static_cast<CClientState*>(interfaces::ClientState)->chokedcommands);
			return 1;
		}

		int GetLastOutgoingCommand(lua_State* L)
		{
			if (interfaces::ClientState == nullptr)
			{
				lua_pushnil(L);
				return 1;
			}

			lua_pushinteger(L, static_cast<CClientState*>(interfaces::ClientState)->lastoutgoingcommand);
			return 1;
		}

		int GetClientSignonState(lua_State* L)
		{
			if (interfaces::ClientState == nullptr)
			{
				lua_pushnil(L);
				return 1;
			}

			lua_pushinteger(L, static_cast<CClientState*>(interfaces::ClientState)->m_nSignonState);
			return 1;
		}

		int GetDeltaTick(lua_State* L)
		{
			if (interfaces::ClientState == nullptr)
			{
				lua_pushnil(L);
				return 1;
			}

			lua_pushinteger(L, static_cast<CClientState*>(interfaces::ClientState)->m_nDeltaTick);
			return 1;
		}

		int GetLastCommandAck(lua_State* L)
		{
			if (interfaces::ClientState == nullptr)
			{
				lua_pushnil(L);
				return 1;
			}

			lua_pushinteger(L, static_cast<CClientState*>(interfaces::ClientState)->last_command_ack);
			return 1;
		}

		int ForceFullUpdate(lua_State *L)
		{
			// xref: Requesting full game update
			if (interfaces::ClientState == nullptr)
				return 0;

			// I should probably add this to the clientstate def
			// But I don't use it anywhere in the code sooo

			using ForceFullUpdateFn = void(*)(CClientState* self);
			static ForceFullUpdateFn original_ForceFullUpdate = reinterpret_cast<ForceFullUpdateFn>(sigscan_module("engine.so", "83 BF B8 01 00 00 FF 74 ? 55"));
			if (original_ForceFullUpdate == nullptr)
				return 0;

			original_ForceFullUpdate(reinterpret_cast<CClientState*>(interfaces::ClientState));
			return 0;
		}
	}
}