#include "../libraries.h"
#include "../../../sdk/FontManager/fontmanager.h"

namespace LuaFuncs
{
	namespace fonts
	{
		const luaL_Reg fontmgrlib[]
		{
			{"CreateFont", CreateFont},
			{"SetFont", SetFont},
			{"GetCurrentFontID", GetCurrentFontID},
			{"GetAllFonts", GetAllFonts},
			{nullptr, nullptr}
		};

		void open(lua_State* L)
		{
			lua_newtable(L);
			luaL_setfuncs(L, fontmgrlib, 0);
			lua_setglobal(L, "fonts");
		}

		int CreateFont(lua_State* L)
		{
			const std::string id = luaL_checkstring(L, 1);
			const std::string name = luaL_checkstring(L, 2);
			int height = luaL_checkinteger(L, 3);
			int weight = luaL_checkinteger(L, 4);
			int flags = luaL_optinteger(L, 5, EFONTFLAG_CUSTOM | EFONTFLAG_ANTIALIAS);

			int font = FontManager::CreateFont(id, name, height, weight, flags);
			if (font == 0)
			{
				lua_pushboolean(L, false);
				return 1;
			}

			lua_pushboolean(L, true);
			return 1;
		}

		int SetFont(lua_State* L)
		{
			const std::string id = luaL_checkstring(L, 1);
			int font = FontManager::GetFont(id);
			if (font == 0)
			{
				lua_pushboolean(L, false);
				return 1;
			}

			interfaces::Surface->DrawSetTextFont(font);
			lua_pushboolean(L, true);
			return 1;
		}

		int GetCurrentFontID(lua_State* L)
		{
			std::string id = FontManager::GetCurrentFontID();
			if (id.empty())
			{
				lua_pushnil(L);
				return 1;
			}

			lua_pushlstring(L, id.c_str(), id.length());
			return 1;
		}

		int GetAllFonts(lua_State* L)
		{
			auto fonts = FontManager::GetAllFonts();
			lua_newtable(L);

			int i = 1;
			for (const auto& font : fonts)
			{
				// push font name
				lua_pushstring(L, font.first.c_str());
				lua_pushinteger(L, font.second);
				lua_settable(L, -3);
			}

			return 1;
		}
	}
}