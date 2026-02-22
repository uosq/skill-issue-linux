#include "esp_datalua.h"
#include "../../../sdk/definitions/fnv.h"
#include "vectorlua.h"

int Lua_ESP_Data::__index(lua_State* L)
{
	Lua_ESP_Data* ldata = static_cast<Lua_ESP_Data*>(luaL_checkudata(L, 1, LUA_ESP_DATA_METATABLE));
	if (!ldata || !ldata->valid)
		luaL_error(L, "Invalid ESP Data!");

	const char* key = luaL_checkstring(L, 2);

	switch (fnv::Hash(key))
	{
		case fnv::HashConst("name"):
		{
			lua_pushlstring(L, ldata->name.c_str(), ldata->name.length());
			return 1;
		}
		
		case fnv::HashConst("health"):
		{
			lua_pushinteger(L, ldata->health);
			return 1;
		}
		
		case fnv::HashConst("maxhealth"):
		{
			lua_pushinteger(L, ldata->maxhealth);
			return 1;
		}
		
		case fnv::HashConst("buffhealth"):
		{
			lua_pushinteger(L, ldata->buffhealth);
			return 1;
		}
		
		case fnv::HashConst("width"):
		{
			lua_pushinteger(L, ldata->width);
			return 1;
		}
		
		case fnv::HashConst("height"):
		{
			lua_pushinteger(L, ldata->height);
			return 1;
		}
		
		case fnv::HashConst("top"):
		{
			Vector vec = Vector(ldata->top.x, ldata->top.y);
			LuaClasses::Vector3::push(L, vec);
			return 1;
		}
		
		case fnv::HashConst("bottom"):
		{
			// bro this is stupid
			// i should make a Vector2 class
			Vector vec = Vector(ldata->bottom.x, ldata->bottom.y);
			LuaClasses::Vector3::push(L, vec);
			return 1;
		}
	}

	luaL_error(L, "Invalid index");
	return 1;
}

int Lua_ESP_Data::__newindex(lua_State* L)
{
	Lua_ESP_Data* ldata = static_cast<Lua_ESP_Data*>(luaL_checkudata(L, 1, LUA_ESP_DATA_METATABLE));
	if (!ldata || !ldata->valid)
		luaL_error(L, "Invalid ESP Data!");

	const char* key = luaL_checkstring(L, 2);

	switch (fnv::Hash(key))
	{
		case fnv::HashConst("name"):
		{
			const char* newName = luaL_checkstring(L, 3);
			ldata->name = newName;
			return 0;
		}
		
		case fnv::HashConst("health"):
		{
			int newhealth = luaL_checkinteger(L, 3);
			ldata->health = newhealth;
			return 0;
		}
		
		case fnv::HashConst("maxhealth"):
		{
			int newmaxhealth = luaL_checkinteger(L, 3);
			ldata->maxhealth = newmaxhealth;
			return 0;
		}
		
		case fnv::HashConst("buffhealth"):
		{
			int buffhealth = luaL_checkinteger(L, 3);
			ldata->buffhealth = buffhealth;
			return 0;
		}
		
		case fnv::HashConst("width"):
		{
			int width = luaL_checkinteger(L, 3);
			ldata->width = width;
			return 0;
		}
		
		case fnv::HashConst("height"):
		{
			int heght = luaL_checkinteger(L, 3);
			ldata->height = heght;
			return 0;
		}
		
		case fnv::HashConst("top"):
		{
			// bro this is stupid
			// i should make a Vector2 class
			LuaVector* lvec = static_cast<LuaVector*>(luaL_checkudata(L, 3, "Vector3"));
			ldata->top = Vec2(lvec->x, lvec->y);
			return 0;
		}
		
		case fnv::HashConst("bottom"):
		{
			// bro this is stupid
			// i should make a Vector2 class
			LuaVector* lvec = static_cast<LuaVector*>(luaL_checkudata(L, 3, "Vector3"));
			ldata->bottom = Vec2(lvec->x, lvec->y);
			return 0;
		}
	}

	luaL_error(L, "Invalid index");
	return 0;
}

void Lua_ESP_Data::CopyFromData(const ESP_Data& in)
{
	this->name = in.name;
	this->health = in.health;
	this->maxhealth = in.maxhealth;
	this->buffhealth = in.buffhealth;
	this->width = in.width;
	this->height = in.height;
	this->top = in.top;
	this->bottom = in.bottom;
}

void Lua_ESP_Data::WriteToData(ESP_Data& out)
{
	out.name = name;
	out.health = health;
	out.maxhealth = maxhealth;
	out.buffhealth = buffhealth;
	out.width = width;
	out.height = height;
	out.top = top;
	out.bottom = bottom;
}

void LuaClasses::ESP_Data::open(lua_State *L)
{
	luaL_newmetatable(L, LUA_ESP_DATA_METATABLE);

	lua_pushcfunction(L, Lua_ESP_Data::__index);
	lua_setfield(L, -2, "__index");

	lua_pushcfunction(L, Lua_ESP_Data::__newindex);
	lua_setfield(L, -2, "__newindex");

	lua_pushstring(L, LUA_ESP_DATA_METATABLE);
	lua_setfield(L, -2, "__name");

	// lock
	lua_pushboolean(L, 0);
	lua_setfield(L, -2, "__metatable");

	// pop the metatable
	lua_pop(L, 1);
}

Lua_ESP_Data* LuaClasses::ESP_Data::push(lua_State *L, struct ESP_Data &data)
{
	Lua_ESP_Data* ldata = static_cast<Lua_ESP_Data*>(lua_newuserdata(L, sizeof(Lua_ESP_Data)));
	new (ldata) Lua_ESP_Data{};

	ldata->CopyFromData(data);
	ldata->valid = true;

	luaL_getmetatable(L, LUA_ESP_DATA_METATABLE);
	lua_setmetatable(L, -2);

	return ldata;
}