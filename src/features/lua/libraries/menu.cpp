#include "../libraries.h"
#include "../../../settings.h"
#include "../../../gui/console.h"

namespace LuaFuncs
{
	namespace menu
	{
		const luaL_Reg menulib[]
		{
			{"IsOpen", IsOpen},
			{"SetOpen", SetOpen},
			{"SetValue", SetValue},
			{"GetValue", GetValue},
			{"ClearOutput", ClearOutput},
			{nullptr, nullptr}
		};

		int SetValue(lua_State* L)
		{
			const char* key = luaL_checkstring(L, 1);
			auto it = Settings::m_optionMap.find(key);
			if (it == Settings::m_optionMap.end())
				return 0;

			SettingEntry& e = it->second;

			switch (e.type)
			{
				case SettingType::BOOL:
				*static_cast<bool*>(e.ptr) = lua_toboolean(L, 2);
				break;

				case SettingType::INT:
				*static_cast<int*>(e.ptr) = luaL_checkinteger(L, 2);
				break;

				case SettingType::FLOAT:
				*static_cast<float*>(e.ptr) = (float)luaL_checknumber(L, 2);
				break;

				case SettingType::STRING:
				*static_cast<std::string*>(e.ptr) = luaL_checkstring(L, 2);
				break;
			}

			return 0;
		}

		int GetValue(lua_State* L)
		{
			const char* key = luaL_checkstring(L, 1);
			auto it = Settings::m_optionMap.find(key);
			if (it == Settings::m_optionMap.end())
				return 0;

			SettingEntry& e = it->second;

			switch (e.type)
			{
				case SettingType::BOOL:
				lua_pushboolean(L, *reinterpret_cast<bool*>(e.ptr));
				break;

				case SettingType::INT:
				lua_pushinteger(L, *reinterpret_cast<bool*>(e.ptr));
				break;

				case SettingType::FLOAT:
				lua_pushnumber(L, *reinterpret_cast<bool*>(e.ptr));
				break;

				case SettingType::STRING:
				lua_pushstring(L, (reinterpret_cast<std::string*>(e.ptr)->c_str()));
				break;
			}

			return 1;
		}

		void luaopen_menu(lua_State* L)
		{
			lua_newtable(L);
			luaL_setfuncs(L, menulib, 0);
			lua_setglobal(L, "menu");
		}

		int IsOpen(lua_State* L)
		{
			lua_pushboolean(L, Settings::menu_open);
			return 1;
		}

		int SetOpen(lua_State* L)
		{
			int open = luaL_checkinteger(L, 1);
			Settings::menu_open = open;
			interfaces::Surface->SetCursorAlwaysVisible(open);
			return 0;
		}

		// tbh I wanted to name it ClearConsole
		// But client lib already has a function with that name
		int ClearOutput(lua_State* L)
		{
			consoleText.clear();
			return 0;
		}
	}
}