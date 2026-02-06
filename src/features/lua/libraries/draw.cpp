#include "../libraries.h"

namespace LuaFuncs
{
	namespace draw
	{
		HFont currentFont = 0;

		const luaL_Reg drawlib[]
		{
			{"Line", Line},
			{"FilledRect", FilledRect},
			{"FilledRectFade", FilledRectFade},
			{"FilledRectFadeFast", FilledRectFadeFast},
			{"OutlinedRect", OutlinedRect},
			{"SetColor", SetColor},
			{"SetTextColor", SetTextColor},
			{"GetTextSize", GetTextSize},
			{"OutlinedCircle", OutlinedCircle},
			{"Text", Text},
			{nullptr, nullptr},
		};

		void luaopen_draw(lua_State* L)
		{
			lua_newtable(L);
			luaL_setfuncs(L, drawlib, 0);
			lua_setglobal(L, "draw");
		}

		int Line(lua_State* L)
		{
			int x0 = luaL_checkinteger(L, 1);
			int y0 = luaL_checkinteger(L, 2);
			int x1 = luaL_checkinteger(L, 3);
			int y1 = luaL_checkinteger(L, 4);

			interfaces::Surface->DrawLine(x0, y0, x1, y1);
			return 0;
		}

		int FilledRect(lua_State* L)
		{
			int x0 = luaL_checkinteger(L, 1);
			int y0 = luaL_checkinteger(L, 2);
			int x1 = luaL_checkinteger(L, 3);
			int y1 = luaL_checkinteger(L, 4);

			interfaces::Surface->DrawFilledRect(x0, y0, x1, y1);
			return 0;
		}

		int FilledRectFade(lua_State* L)
		{
			int x0 = luaL_checkinteger(L, 1);
			int y0 = luaL_checkinteger(L, 2);
			int x1 = luaL_checkinteger(L, 3);
			int y1 = luaL_checkinteger(L, 4);
			int alpha0 = luaL_checkinteger(L, 5);
			int alpha1 = luaL_checkinteger(L, 6);
			bool bHorizontal = luaL_checkinteger(L, 7);

			interfaces::Surface->DrawFilledRectFade(x0, y0, x1, y1, alpha0, alpha1, bHorizontal);
			return 0;
		}

		int FilledRectFadeFast(lua_State* L)
		{
			int x0 = luaL_checkinteger(L, 1);
			int y0 = luaL_checkinteger(L, 2);
			int x1 = luaL_checkinteger(L, 3);
			int y1 = luaL_checkinteger(L, 4);
			int fadeStartPt = luaL_checkinteger(L, 5);
			int fadeEndPt = luaL_checkinteger(L, 6);
			int alpha0 = luaL_checkinteger(L, 7);
			int alpha1 = luaL_checkinteger(L, 8);
			bool bHorizontal = luaL_checkinteger(L, 9);

			interfaces::Surface->DrawFilledRectFastFade(x0, y0, x1, y1, fadeStartPt, fadeEndPt, alpha0, alpha1, bHorizontal);
			return 0;
		}

		int OutlinedRect(lua_State* L)
		{
			int x0 = luaL_checkinteger(L, 1);
			int y0 = luaL_checkinteger(L, 2);
			int x1 = luaL_checkinteger(L, 3);
			int y1 = luaL_checkinteger(L, 4);

			interfaces::Surface->DrawOutlinedRect(x0, y0, x1, y1);
			return 0;
		}

		int SetColor(lua_State* L)
		{
			int r = luaL_checkinteger(L, 1);
			int g = luaL_checkinteger(L, 2);
			int b = luaL_checkinteger(L, 3);
			int a = luaL_optinteger(L, 4, 255);

			interfaces::Surface->DrawSetColor(r, g, b, a);
			return 0;
		}

		int SetTextColor(lua_State* L)
		{
			int r = luaL_checkinteger(L, 1);
			int g = luaL_checkinteger(L, 2);
			int b = luaL_checkinteger(L, 3);
			int a = luaL_optinteger(L, 4, 255);

			interfaces::Surface->DrawSetTextColor(r, g, b, a);
			return 0;
		}

		int GetTextSize(lua_State* L)
		{
			const char* str = luaL_checkstring(L, 1);
			if (str == nullptr)
			{
				lua_pushnil(L);
				return 1;
			}

			// this is stupid
			std::string text(str);
			std::wstring wtext(text.begin(), text.end());
			
			int tw, th;
			interfaces::Surface->GetTextSize(currentFont, wtext.c_str(), tw, th);

			lua_pushnumber(L, tw);
			lua_pushnumber(L, th);
			return 2;
		}

		int OutlinedCircle(lua_State* L)
		{
			int x = luaL_checkinteger(L, 1);
			int y = luaL_checkinteger(L, 2);
			int radius = luaL_checkinteger(L, 3);
			int segments = luaL_checkinteger(L, 4);

			interfaces::Surface->DrawOutlinedCircle(x, y, radius, segments);
			return 0;
		}

		int Text(lua_State* L)
		{
			int x = luaL_checkinteger(L, 1);
			int y = luaL_checkinteger(L, 2);
			const char* str = luaL_checkstring(L, 3);

			std::string text(str);
			std::wstring wtext(text.begin(), text.end()); // this is fucking stupid

			interfaces::Surface->DrawSetTextPos(x, y);
			interfaces::Surface->DrawPrintText(wtext.c_str(), wtext.length());
			return 0;
		}
	}
}