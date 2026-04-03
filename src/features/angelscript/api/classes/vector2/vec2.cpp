#include "vec2.h"

#include "../../../../../sdk/definitions/vector.h"

#define VEC2_CLASSNAME "Vector2"

void Vector2_Default(Vec2 *self)
{
	new (self) Vec2();
}

void Vector2_Init(Vec2 *self, float x, float y)
{
	new (self) Vec2(x, y);
}

void Vector2_RegisterClass(asIScriptEngine *engine)
{
	engine->RegisterObjectType(VEC2_CLASSNAME, sizeof(Vec2), asOBJ_VALUE | asOBJ_POD | asGetTypeTraits<Vec2>());

	engine->RegisterObjectBehaviour(VEC2_CLASSNAME, asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(Vector2_Default),
					asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectBehaviour(VEC2_CLASSNAME, asBEHAVE_CONSTRUCT, "void f(float, float)",
					asFUNCTION(Vector2_Init), asCALL_CDECL_OBJFIRST);

	engine->RegisterObjectProperty(VEC2_CLASSNAME, "float x", offsetof(Vec2, x));
	engine->RegisterObjectProperty(VEC2_CLASSNAME, "float y", offsetof(Vec2, y));

	engine->RegisterObjectMethod(VEC2_CLASSNAME, "float Length() const", asMETHOD(Vec2, Length), asCALL_THISCALL);
	engine->RegisterObjectMethod(VEC2_CLASSNAME, "float LengthSqr() const", asMETHOD(Vec2, LengthSqr),
				     asCALL_THISCALL);
	engine->RegisterObjectMethod(VEC2_CLASSNAME, "float DistTo() const", asMETHOD(Vec2, DistTo), asCALL_THISCALL);
	engine->RegisterObjectMethod(VEC2_CLASSNAME, "float DistToSqr() const", asMETHOD(Vec2, DistToSqr),
				     asCALL_THISCALL);
	engine->RegisterObjectMethod(VEC2_CLASSNAME, "float Dot() const", asMETHOD(Vec2, Dot), asCALL_THISCALL);
	engine->RegisterObjectMethod(VEC2_CLASSNAME, "bool IsZero() const", asMETHOD(Vec2, IsZero), asCALL_THISCALL);
}