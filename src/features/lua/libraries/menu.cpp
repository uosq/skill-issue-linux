#include "../libraries.h"
#include "../../../settings/settings.h"
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
			auto it = Settings::GetOptionMap().find(key);
			if (it == Settings::GetOptionMap().end())
				return 0;

			auto& e = it->second;

			switch (e->GetType())
			{
				case SettingType::BOOL:
				Settings::SetSetting(key, static_cast<bool>(lua_toboolean(L, 2)));
				break;

				case SettingType::INT:
				Settings::SetSetting(key, static_cast<int>(luaL_checkinteger(L, 2)));
				break;

				case SettingType::FLOAT:
				Settings::SetSetting(key, static_cast<float>(luaL_checknumber(L, 2)));
				break;

				case SettingType::STRING:
				Settings::SetSetting(key, static_cast<std::string>(luaL_checkstring(L, 2)));
				break;

				case SettingType::UINT8:
				Settings::SetSetting(key, static_cast<uint8_t>(luaL_checkinteger(L, 2)));
				break;

				case SettingType::NONE:
				break;
			}

			return 0;
		}

		int GetValue(lua_State* L)
		{
			const char* key = luaL_checkstring(L, 1);
			auto it = Settings::GetOptionMap().find(key);
			if (it == Settings::GetOptionMap().end())
				return 0;

			auto& e = it->second;

			switch (e->GetType())
			{
				case SettingType::BOOL:
				{
					bool val = false;
					if (!Settings::GetSetting(key, val))
					{
						lua_pushnil(L);
						return 1;
					}

					lua_pushboolean(L, val);
					return 1;
				}

				case SettingType::INT:
				{
					int val = 0;
					if (!Settings::GetSetting(key, val))
					{
						lua_pushnil(L);
						return 1;
					}

					lua_pushinteger(L, val);
					return 1;
				}

				case SettingType::FLOAT:
				{
					float val = 0;
					if (!Settings::GetSetting(key, val))
					{
						lua_pushnil(L);
						return 1;
					}

					lua_pushnumber(L, val);
					return 1;
				}

				case SettingType::STRING:
				{
					std::string val = "";
					if (!Settings::GetSetting(key, val))
					{
						lua_pushnil(L);
						return 1;
					}

					lua_pushlstring(L, val.c_str(), val.length());
					return 1;
				}

				case SettingType::UINT8:
				{
					uint8_t val = 0;
					if (!Settings::GetSetting(key, val))
					{
						lua_pushnil(L);
						return 1;
					}

					lua_pushinteger(L, static_cast<int>(val));
					return 1;
				}

				case SettingType::NONE:
				break;
			}

			lua_pushnil(L);
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