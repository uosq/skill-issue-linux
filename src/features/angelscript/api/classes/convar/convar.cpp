#include "convar.h"

#include "../../../../../sdk/definitions/convar.h"

#define CONVAR_CLASSNAME "ConVar"

int GetInt(ConVar *cvar)
{
	if (!cvar)
		return 0;
	return cvar->GetInt();
}

float GetFloat(ConVar *cvar)
{
	if (!cvar)
		return 0;
	return cvar->GetFloat();
}

bool GetBool(ConVar *cvar)
{
	if (!cvar)
		return false;
	return cvar->GetBool();
};

std::string GetString(ConVar *cvar)
{
	if (!cvar)
		return "Invalid ConVar";
	return cvar->GetString();
}

void SetString(ConVar *cvar, const std::string &value)
{
	if (!cvar)
		return;
	cvar->SetValue(value.c_str());
}

void SetFloat(ConVar *cvar, float value)
{
	if (!cvar)
		return;
	cvar->SetValue(value);
}

void SetInt(ConVar *cvar, int value)
{
	if (!cvar)
		return;
	cvar->SetValue(value);
}

void SetBool(ConVar *cvar, bool value)
{
	if (!cvar)
		return;
	cvar->SetValue(value);
}

void ConVar_RegisterClass(asIScriptEngine *engine)
{
	engine->RegisterObjectType(CONVAR_CLASSNAME, 0, asOBJ_REF | asOBJ_NOCOUNT);

	engine->RegisterObjectMethod(CONVAR_CLASSNAME, "int GetInt()", asFUNCTION(GetInt), asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectMethod(CONVAR_CLASSNAME, "float GetFloat()", asFUNCTION(GetFloat), asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectMethod(CONVAR_CLASSNAME, "bool GetBool()", asFUNCTION(GetBool), asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectMethod(CONVAR_CLASSNAME, "string GetString()", asFUNCTION(GetString),
				     asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectMethod(CONVAR_CLASSNAME, "void SetValue(const string &in value)", asFUNCTION(SetString),
				     asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectMethod(CONVAR_CLASSNAME, "void SetValue(float value)", asFUNCTION(SetFloat),
				     asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectMethod(CONVAR_CLASSNAME, "void SetValue(int value)", asFUNCTION(SetInt),
				     asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectMethod(CONVAR_CLASSNAME, "void SetValue(bool value)", asFUNCTION(SetBool),
				     asCALL_CDECL_OBJFIRST);
}