#pragma once

#include "../../sdk/definitions/types.h"

extern "C"
{
	#include <lua5.4/lua.h>
	#include <lua5.4/lua.hpp>
	#include <lua5.4/lualib.h>
}

namespace LuaClasses
{
	namespace VectorLua
	{
		extern const luaL_Reg methods[];
		void luaopen_vector(lua_State* L);
		Vector* push_vector(lua_State* L, float x = 0, float y = 0, float z = 0);
		Vector* push_vector(lua_State* L, Vector vec);
		int Vector3(lua_State* L);
		
		int GC(lua_State* L);
		int Index(lua_State* L);
		int NewIndex(lua_State* L);
		int ToString(lua_State* L);

		int Init(lua_State* L);
		int Set(lua_State* L);
		int Normalize(lua_State* L);
		int Length(lua_State* L);
		int Length2D(lua_State* L);
		int LengthSqr(lua_State* L);
		int Length2DSqr(lua_State* L);
		int Cross(lua_State* L);
		int Dot(lua_State* L);
		int DistTo(lua_State* L);
		int DistTo2D(lua_State* L);
		int ToAngle(lua_State* L);
		int Angles(lua_State* L);
	};
}