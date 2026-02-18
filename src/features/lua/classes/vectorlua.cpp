#include "vectorlua.h"
#include "../../../sdk/math.h"
#include <cstring>

int LuaClasses::Vector3::vector3(lua_State* L)
{
	float x = luaL_optnumber(L, 1, 0.0f);
	float y = luaL_optnumber(L, 2, 0.0f);
	float z = luaL_optnumber(L, 3, 0.0f);

	LuaVector* lvec = static_cast<LuaVector*>(lua_newuserdata(L, sizeof(LuaVector)));
	new (lvec) LuaVector{};

	lvec->Set(x, y, z);

	luaL_getmetatable(L, "Vector3");
	lua_setmetatable(L, -2);

	return 1;
}

LuaVector* LuaClasses::Vector3::push(lua_State *L, const Vec3 &vec)
{
	LuaVector* lvec = static_cast<LuaVector*>(lua_newuserdata(L, sizeof(LuaVector)));
	new (lvec) LuaVector{};

	lvec->CopyFromVector(vec);

	luaL_getmetatable(L, "Vector3");
	lua_setmetatable(L, -2);

	return lvec;
}

void LuaClasses::Vector3::open(lua_State* L)
{
	luaL_newmetatable(L, "Vector3");

	luaL_setfuncs(L, LuaVector::methods, 0);

	lua_pushcfunction(L, LuaVector::__index);
	lua_setfield(L, -2, "__index");

	lua_pushcfunction(L, LuaVector::__newindex);
	lua_setfield(L, -2, "__newindex");

	lua_pushcfunction(L, LuaVector::__add);
	lua_setfield(L, -2, "__add");

	lua_pushcfunction(L, LuaVector::__sub);
	lua_setfield(L, -2, "__sub");

	lua_pushcfunction(L, LuaVector::__div);
	lua_setfield(L, -2, "__div");

	lua_pushcfunction(L, LuaVector::__eq);
	lua_setfield(L, -2, "__eq");

	lua_pushcfunction(L, LuaVector::__gc);
	lua_setfield(L, -2, "__gc");

	lua_pushcfunction(L, LuaVector::__mul);
	lua_setfield(L, -2, "__mul");

	lua_pushcfunction(L, LuaVector::__unary);
	lua_setfield(L, -2, "__unary");

	lua_pushstring(L, "Vector3");
	lua_setfield(L, -2, "__name");

	// lock
	lua_pushboolean(L, 0);
	lua_setfield(L, -2, "__metatable");

	lua_pop(L, 1); // pop table

	lua_pushcfunction(L, vector3);
	lua_setglobal(L, "Vector3");

	lua_pop(L, 1); // pop Vector3 global
}

void LuaVector::CopyFromVector(const Vector& in)
{
	x = in.x;
	y = in.y;
	z = in.z;
}

void LuaVector::WriteToVector(Vector* pOut)
{
	pOut->x = x;
	pOut->y = y;
	pOut->z = z;
}

int LuaVector::__index(lua_State *L)
{
	LuaVector* lvec = static_cast<LuaVector*>(luaL_checkudata(L, 1, "Vector3"));
	if (lvec == nullptr)
	{
		luaL_error(L, "Vector is nullptr!");
		return 0;
	}

	const char* key = luaL_checkstring(L, 2);

	if(strcmp(key, "x") == 0)
	{
		lua_pushnumber(L, lvec->x);
		return 1;
	}

	if(strcmp(key, "y") == 0)
	{
		lua_pushnumber(L, lvec->y);
		return 1;
	}

	if(strcmp(key, "z") == 0)
	{
		lua_pushnumber(L, lvec->z);
		return 1;
	}

	luaL_getmetatable(L, "Vector3");
	lua_pushvalue(L, 2);
	lua_rawget(L, -2);
	return 1;
}

int LuaVector::__newindex(lua_State *L)
{
	LuaVector* lvec = static_cast<LuaVector*>(luaL_checkudata(L, 1, "Vector3"));
	if (lvec == nullptr)
	{
		luaL_error(L, "Vector is nullptr!");
		return 0;
	}

	const char* key = luaL_checkstring(L, 2);

	if(strcmp(key, "x") == 0)
	{
		float value = luaL_checknumber(L, 3);
		lvec->x = value;
		return 0;
	}

	if(strcmp(key, "y") == 0)
	{
		float value = luaL_checknumber(L, 3);
		lvec->y = value;
		return 0;
	}

	if(strcmp(key, "z") == 0)
	{
		float value = luaL_checknumber(L, 3);
		lvec->z = value;
		return 0;
	}

	return 0;
}

int LuaVector::Lua_ToString(lua_State *L)
{
	LuaVector* lvec = static_cast<LuaVector*>(luaL_checkudata(L, 1, "Vector3"));

	// im not sure how many characters should I allocate :/
	char str[128];
	int size = snprintf(str, sizeof(str), "%.3f, %.3f, %.3f", lvec->x, lvec->y, lvec->z);

	lua_pushlstring(L, str, static_cast<size_t>(size));
	return 1;
}

int LuaVector::Lua_Angles(lua_State *L)
{
	LuaVector* lvec = static_cast<LuaVector*>(luaL_checkudata(L, 1, "Vector3"));

	Vector forward, right, up;
	Math::AngleVectors(*lvec, &forward, &right, &up);

	LuaClasses::Vector3::push(L, forward);
	LuaClasses::Vector3::push(L, right);
	LuaClasses::Vector3::push(L, up);
	return 3;
}

int LuaVector::Lua_Cross(lua_State *L)
{
	LuaVector* lvecA = static_cast<LuaVector*>(luaL_checkudata(L, 1, "Vector3"));
	LuaVector* lvecB = static_cast<LuaVector*>(luaL_checkudata(L, 2, "Vector3"));

	LuaClasses::Vector3::push(L, lvecA->Cross(*lvecB));
	return 1;
}

int LuaVector::Lua_DistTo(lua_State *L)
{
	LuaVector* lvecA = static_cast<LuaVector*>(luaL_checkudata(L, 1, "Vector3"));
	LuaVector* lvecB = static_cast<LuaVector*>(luaL_checkudata(L, 2, "Vector3"));

	lua_pushnumber(L, lvecA->DistTo(*lvecB));
	return 1;
}

int LuaVector::Lua_DistTo2D(lua_State *L)
{
	LuaVector* lvecA = static_cast<LuaVector*>(luaL_checkudata(L, 1, "Vector3"));
	LuaVector* lvecB = static_cast<LuaVector*>(luaL_checkudata(L, 2, "Vector3"));

	lua_pushnumber(L, lvecA->DistTo2D(*lvecB));
	return 1;
}

int LuaVector::Lua_Dot(lua_State *L)
{
	LuaVector* lvecA = static_cast<LuaVector*>(luaL_checkudata(L, 1, "Vector3"));
	LuaVector* lvecB = static_cast<LuaVector*>(luaL_checkudata(L, 2, "Vector3"));

	lua_pushnumber(L, lvecA->Dot(*lvecB));
	return 1;
}

int LuaVector::Lua_Init(lua_State *L)
{
	LuaVector* lvec = static_cast<LuaVector*>(luaL_checkudata(L, 1, "Vector3"));

	float x = luaL_optnumber(L, 2, 0.0f);
	float y = luaL_optnumber(L, 3, 0.0f);
	float z = luaL_optnumber(L, 4, 0.0f);

	lvec->Set(x, y, z);
	return 1;
}

int LuaVector::Lua_Set(lua_State* L)
{
	return Lua_Init(L);
}

int LuaVector::Lua_Length(lua_State *L)
{
	LuaVector* lvec = static_cast<LuaVector*>(luaL_checkudata(L, 1, "Vector3"));
	lua_pushnumber(L, lvec->Length());
	return 1;
}

int LuaVector::Lua_Length2D(lua_State *L)
{
	LuaVector* lvec = static_cast<LuaVector*>(luaL_checkudata(L, 1, "Vector3"));
	lua_pushnumber(L, lvec->Length2D());
	return 1;
}

int LuaVector::Lua_Length2DSqr(lua_State *L)
{
	LuaVector* lvec = static_cast<LuaVector*>(luaL_checkudata(L, 1, "Vector3"));
	lua_pushnumber(L, lvec->Length2DSqr());
	return 1;
}

int LuaVector::Lua_LengthSqr(lua_State *L)
{
	LuaVector* lvec = static_cast<LuaVector*>(luaL_checkudata(L, 1, "Vector3"));
	lua_pushnumber(L, lvec->LengthSqr());
	return 1;
}

int LuaVector::Lua_Normalize(lua_State *L)
{
	LuaVector* lvec = static_cast<LuaVector*>(luaL_checkudata(L, 1, "Vector3"));
	lvec->Normalize();
	return 0;
}

int LuaVector::__add(lua_State *L)
{
	LuaVector* lvecA = static_cast<LuaVector*>(luaL_checkudata(L, 1, "Vector3"));
	LuaVector* lvecB = static_cast<LuaVector*>(luaL_checkudata(L, 2, "Vector3"));

	LuaClasses::Vector3::push(L, (*lvecA) + (*lvecB));
	return 1;
}

int LuaVector::__div(lua_State *L)
{
	LuaVector* lvec = static_cast<LuaVector*>(luaL_checkudata(L, 1, "Vector3"));
	float number = luaL_checknumber(L, 2);

	LuaClasses::Vector3::push(L, (*lvec)/number);
	return 1;
}

int LuaVector::__eq(lua_State *L)
{
	LuaVector* lvecA = static_cast<LuaVector*>(luaL_checkudata(L, 1, "Vector3"));
	LuaVector* lvecB = static_cast<LuaVector*>(luaL_checkudata(L, 2, "Vector3"));

	lua_pushboolean(L, lvecA->x == lvecB->x && lvecA->y == lvecB->y && lvecA->z == lvecB->z);
	return 1;
}

int LuaVector::__gc(lua_State *L)
{
	LuaVector* lvec = static_cast<LuaVector*>(luaL_checkudata(L, 1, "Vector3"));
	lvec->~LuaVector();
	return 0;
}

int LuaVector::__mul(lua_State *L)
{
	LuaVector* lvec = static_cast<LuaVector*>(luaL_checkudata(L, 1, "Vector3"));
	float number = luaL_checknumber(L, 2);

	LuaClasses::Vector3::push(L, (*lvec) * number);
	return 1;
}

int LuaVector::__sub(lua_State *L)
{
	LuaVector* lvecA = static_cast<LuaVector*>(luaL_checkudata(L, 1, "Vector3"));
	LuaVector* lvecB = static_cast<LuaVector*>(luaL_checkudata(L, 2, "Vector3"));

	LuaClasses::Vector3::push(L, (*lvecA) - (*lvecB));
	return 1;
}

int LuaVector::__unary(lua_State *L)
{
	LuaVector* lvec = static_cast<LuaVector*>(luaL_checkudata(L, 1, "Vector3"));
	LuaClasses::Vector3::push(L, Vector(-lvec->x, -lvec->y, -lvec->z));
	return 1;
}

int LuaVector::Lua_ToAngle(lua_State *L)
{
	LuaVector* lvec = static_cast<LuaVector*>(luaL_checkudata(L, 1, "Vector3"));
	LuaClasses::Vector3::push(L, lvec->ToAngle());
	return 1;
}