#pragma once

#include "../pluto/lua.h"
#include "../pluto/lauxlib.h"
#include "../pluto/lualib.h"

#include "../../../sdk/definitions/vector.h"

struct LuaVector: public Vector
{
	// methods
	static int Lua_Init(lua_State* L);
	static int Lua_Set(lua_State* L);
	static int Lua_Normalize(lua_State* L);
	static int Lua_Length(lua_State* L);
	static int Lua_Length2D(lua_State* L);
	static int Lua_LengthSqr(lua_State* L);
	static int Lua_Length2DSqr(lua_State* L);
	static int Lua_Cross(lua_State* L);
	static int Lua_Dot(lua_State* L);
	static int Lua_DistTo(lua_State* L);
	static int Lua_DistTo2D(lua_State* L);
	static int Lua_ToAngle(lua_State* L);
	static int Lua_Angles(lua_State* L);
	static int Lua_ToString(lua_State* L);

	// Lua metamethods
	static int __index(lua_State* L);
	static int __newindex(lua_State* L);
	static int __gc(lua_State* L);
	static int __add(lua_State* L);
	static int __sub(lua_State* L);
	static int __div(lua_State* L);
	static int __eq(lua_State* L);
	static int __mul(lua_State* L);
	static int __unary(lua_State* L);

	constexpr static const luaL_Reg methods[]
	{
		{"Set", Lua_Set},
		{"Normalize", Lua_Normalize},
		{"Length", Lua_Length},
		{"Length2D", Lua_Length2D},
		{"LengthSqr", Lua_LengthSqr},
		{"Length2DSqr", Lua_Length2DSqr},
		{"Cross", Lua_Cross},
		{"Dot", Lua_Dot},
		{"DistTo", Lua_DistTo},
		{"DistTo2D", Lua_DistTo2D},
		{"ToAngle", Lua_ToAngle},
		{"Angles", Lua_Angles},
		{"ToString", Lua_ToString},
		{nullptr, nullptr},
	};

	void WriteToVector(Vector* pOut);
	void CopyFromVector(const Vector& in);
};

namespace LuaClasses
{
	namespace Vector3
	{
		LuaVector* push(lua_State* L, const Vec3& vec);
		void open(lua_State* L);
		int vector3(lua_State* L);
	}
}