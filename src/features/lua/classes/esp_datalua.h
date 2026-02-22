#pragma once

#include "../pluto/lua.h"
#include "../pluto/lauxlib.h"

#include "../../../sdk/math.h"
#include "../../esp/structs.h"

#define LUA_ESP_DATA_METATABLE "ESP_Data"

struct Lua_ESP_Data
{
	std::string name;
	int health;
	int maxhealth;
	int buffhealth;
	float width, height;

	Vec2 top;
	Vec2 bottom;
	bool valid;

	static int __index(lua_State* L);
	static int __newindex(lua_State* L);

	void CopyFromData(const ESP_Data& in);
	void WriteToData(ESP_Data& out);
};

namespace LuaClasses
{
	namespace ESP_Data
	{
		void open(lua_State* L);
		Lua_ESP_Data* push(lua_State* L, struct ESP_Data& data);
	}
}