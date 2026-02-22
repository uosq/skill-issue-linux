#include "../libraries.h"
#include "../../esp/esp.h"

namespace LuaFuncs
{
	namespace esp
	{
		const luaL_Reg methods[]
		{
			{"Register", Register},
			{"Unregister", Unregister},
			{"UnregisterAll", UnregisterAll},
			{nullptr, nullptr}
		};

		void open(lua_State* L)
		{
			lua_newtable(L);
			luaL_setfuncs(L, methods, 0);
			lua_setglobal(L, "esp");
		}

		int UnregisterAll(lua_State* L)
		{
			ESP::m_luaElements.clear();
			return 0;
		}

		/*
		esp.Register("id", "text", (int)alignment, function(entity, data)
			return true
		end): bool
		*/
		int Register(lua_State* L)
		{
			const char* id = luaL_checkstring(L, 1);

			for (const auto& element : ESP::m_luaElements)
			{
				const std::string& curID = element->GetID();
				if (curID == id)
				{
					lua_pushboolean(L, 0);
					return 1;
				}
			}

			const char* text = luaL_checkstring(L, 2);
			int alignment = luaL_checkinteger(L, 3);

			luaL_checktype(L, 4, LUA_TFUNCTION);
    			lua_pushvalue(L, 4);
    			int callbackRef = luaL_ref(L, LUA_REGISTRYINDEX);

			ESP::m_luaElements.push_back(std::make_unique<LuaElement>(id, text, callbackRef, static_cast<ESP_ALIGNMENT>(alignment)));

			lua_pushboolean(L, 1);
			return 1;
		}

		/*
		esp.Unregister("id"): bool
		*/
		int Unregister(lua_State* L)
		{
			const char* id = luaL_checkstring(L, 1);

			for (auto it = ESP::m_luaElements.begin(); it != ESP::m_luaElements.end(); it++)
			{
				if (it->get()->GetID() == id)
				{
					ESP::m_luaElements.erase(it);
					lua_pushboolean(L, 1);
					return 1;
				}
			}

			lua_pushboolean(L, 0);
			return 1;
		}
	}
}