#include "../libraries.h"
#include "../../../imgui/imgui.h"

namespace LuaFuncs
{
	namespace ui
	{
		const luaL_Reg uilib[]
		{
			{"Begin", Begin},
			{"Button", Button},
			{"Checkbox", Checkbox},
			{"TextUnformatted", TextUnformatted},
			{"SliderFloat", SliderFloat},
			{"End", End},
			{nullptr, nullptr}
		};

		void luaopen_ui(lua_State* L)
		{
			lua_newtable(L);
			luaL_setfuncs(L, uilib, 0);
			lua_setglobal(L, "ui");
		}
		
		int Begin(lua_State* L)
		{
			const char* name = luaL_checkstring(L, 1);
			if (strcmp(name, "Skill Issue") == 0)
			{
				luaL_error(L, "Can't use the menu's name!");
				return 0;
			}

			int flags = luaL_optinteger(L, 2, 0);

			lua_pushboolean(L, ImGui::Begin(name, nullptr, flags));
			return 1;
		}

		int Button(lua_State* L)
		{
			const char* label = luaL_checkstring(L, 1);
			lua_pushboolean(L, ImGui::Button(label));
			return 1;
		}

		int Checkbox(lua_State* L)
		{
			const char* label = luaL_checkstring(L, 1);
			luaL_checktype(L, 2, LUA_TBOOLEAN);

			bool value = lua_toboolean(L, 2);

			bool changed = ImGui::Checkbox(label, &value);

			lua_pushboolean(L, changed);
			lua_pushboolean(L, value);
			return 2;
		}

		int TextUnformatted(lua_State* L)
		{
			const char* text = luaL_checkstring(L, 1);
			ImGui::TextUnformatted(text);
			return 1;
		}

		int SliderFloat(lua_State* L)
		{
			const char* label = luaL_checkstring(L, 1);
			float value = luaL_checknumber(L, 2);
			float min = luaL_checknumber(L, 3);
			float max = luaL_checknumber(L, 4);
			lua_pushboolean(L, ImGui::SliderFloat(label, &value, min, max));
			lua_pushnumber(L, value);
			return 2;
		}

		int End(lua_State* L)
		{
			ImGui::End();
			return 1;
		}
	}
}