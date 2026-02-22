#include "colorlua.h"

void LuaColor::CopyFromColor(const Color& in)
{
	this->r = in.r();
	this->g = in.g();
	this->b = in.b();
	this->a = in.a();
}

void LuaColor::WriteToColor(Color& out)
{
	out.SetColor(r, g, b, a);
}

int LuaColor::Set(lua_State *L)
{
	LuaColor* lcolor = static_cast<LuaColor*>(luaL_checkudata(L, 1, LUA_COLOR_METATABLE));

	uint8_t r = luaL_optinteger(L, 2, 0);
	uint8_t g = luaL_optinteger(L, 3, 0);
	uint8_t b = luaL_optinteger(L, 4, 0);
	uint8_t a = luaL_optinteger(L, 5, 0);

	lcolor->r = r;
	lcolor->g = g;
	lcolor->b = b;
	lcolor->a = a;
	return 0;
}

int LuaColor::__index(lua_State *L)
{
	LuaColor* lcolor = static_cast<LuaColor*>(luaL_checkudata(L, 1, LUA_COLOR_METATABLE));
	const char* key = luaL_checkstring(L, 2);
	char letter = key[0];

	switch (letter)
	{
		case 'r':
		{
			lua_pushinteger(L, lcolor->r);
			return 1;
		}

		case 'g':
		{
			lua_pushinteger(L, lcolor->g);
			return 1;
		}

		case 'b':
		{
			lua_pushinteger(L, lcolor->b);
			return 1;
		}

		case 'a':
		{
			lua_pushinteger(L, lcolor->a);
			return 1;
		}

		default: break;
	}

	luaL_error(L, "Invalid field! Expected r, g, b or a. Received '%s'", letter);
}

int LuaColor::__newindex(lua_State *L)
{
	LuaColor* lcolor = static_cast<LuaColor*>(luaL_checkudata(L, 1, LUA_COLOR_METATABLE));
	const char* key = luaL_checkstring(L, 2);
	char letter = key[0];

	switch (letter)
	{
		case 'r':
		{
			uint8_t val = luaL_checkinteger(L, 3);
			lcolor->r = val;
			return 0;
		}

		case 'g':
		{
			uint8_t val = luaL_checkinteger(L, 3);
			lcolor->g = val;
			return 0;
		}

		case 'b':
		{
			uint8_t val = luaL_checkinteger(L, 3);
			lcolor->b = val;
			return 0;
		}

		case 'a':
		{
			uint8_t val = luaL_checkinteger(L, 3);
			lcolor->a = val;
			return 0;
		}

		default: break;
	}

	luaL_error(L, "Invalid field! Expected r, g, b or a. Received '%s'", letter);
}

int LuaColor::__gc(lua_State* L)
{
	LuaColor* lcolor = static_cast<LuaColor*>(luaL_checkudata(L, 1, LUA_COLOR_METATABLE));
	lcolor->~LuaColor();
	return 0;
}

void LuaClasses::Color::open(lua_State *L)
{
	luaL_newmetatable(L, LUA_COLOR_METATABLE);

	lua_pushcfunction(L, LuaColor::__index);
	lua_setfield(L, -2, "__index");

	lua_pushcfunction(L, LuaColor::__newindex);
	lua_setfield(L, -2, "__newindex");

	lua_pushstring(L, LUA_COLOR_METATABLE);
	lua_setfield(L, -2, "__name");

	// lock
	lua_pushboolean(L, 0);
	lua_setfield(L, -2, "__metatable");

	// pop the metatable
	lua_pop(L, 1);

	lua_pushcfunction(L, LuaColor::Color3);
	lua_setglobal(L, "color");

	lua_pop(L, 1);
}

LuaColor* LuaClasses::Color::push(lua_State *L, struct Color &color)
{
	LuaColor* lcolor = static_cast<LuaColor*>(lua_newuserdata(L, sizeof(LuaColor)));
	new (lcolor) LuaColor{};

	lcolor->CopyFromColor(color);

	luaL_getmetatable(L, LUA_COLOR_METATABLE);
	lua_setmetatable(L, -2);

	return lcolor;
}

int LuaColor::Color3(lua_State* L)
{
	LuaColor* lcolor = static_cast<LuaColor*>(lua_newuserdata(L, sizeof(LuaColor)));
	new (lcolor) LuaColor{};

	lcolor->CopyFromColor(Color(0, 0, 0, 0));

	luaL_getmetatable(L, LUA_COLOR_METATABLE);
	lua_setmetatable(L, -2);

	return 1;
}