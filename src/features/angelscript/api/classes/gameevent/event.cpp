#include "event.h"

#include "../../../../../sdk/definitions/igameevents.h"

#define GAMEEVENT_CLASSNAME "GameEvent"

void GameEvent_AddRef(IGameEvent *event)
{
}
void GameEvent_Release(IGameEvent *event)
{
}

std::string GetName(IGameEvent *event)
{
	return event ? event->GetName() : "Invalid Event";
}

bool IsReliable(IGameEvent *event)
{
	if (!event)
		return false;
	return event->IsReliable();
}

bool IsLocal(IGameEvent *event)
{
	if (!event)
		return false;
	return event->IsLocal();
}

bool IsEmpty(IGameEvent *event, const std::string &keyName)
{
	if (!event)
		return false;
	return event->IsEmpty(keyName.c_str());
}

// Data access
bool GetBool(IGameEvent *event, const std::string &keyName, bool defaultValue)
{
	if (!event)
		return 0;
	return event->GetBool(keyName.c_str(), defaultValue);
}

int GetInt(IGameEvent *event, const std::string &keyName, int defaultValue)
{
	if (!event)
		return 0;
	return event->GetInt(keyName.c_str(), defaultValue);
}

float GetFloat(IGameEvent *event, const std::string &keyName, float defaultValue)
{
	if (!event)
		return 0;
	return event->GetFloat(keyName.c_str(), defaultValue);
}

std::string GetString(IGameEvent *event, const std::string &keyName, const std::string &defaultValue)
{
	return event ? event->GetString(keyName.c_str(), defaultValue.c_str()) : "Invalid Event";
}

void SetBool(IGameEvent *event, const std::string &keyName, bool value)
{
	if (!event)
		return;
	event->SetBool(keyName.c_str(), value);
}

void SetInt(IGameEvent *event, const std::string &keyName, int value)
{
	if (!event)
		return;
	event->SetInt(keyName.c_str(), value);
}

void SetFloat(IGameEvent *event, const std::string &keyName, float value)
{
	if (!event)
		return;
	event->SetFloat(keyName.c_str(), value);
}

void SetString(IGameEvent *event, const std::string &keyName, const std::string &value)
{
	if (!event)
		return;
	event->SetString(keyName.c_str(), value.c_str());
}

void GameEvent_RegisterClass(asIScriptEngine *engine)
{
	engine->RegisterObjectType(GAMEEVENT_CLASSNAME, 0, asOBJ_REF);

	engine->RegisterObjectBehaviour(GAMEEVENT_CLASSNAME, asBEHAVE_ADDREF, "void f()", asFUNCTION(GameEvent_AddRef),
					asCALL_CDECL_OBJLAST);
	engine->RegisterObjectBehaviour(GAMEEVENT_CLASSNAME, asBEHAVE_RELEASE, "void f()",
					asFUNCTION(GameEvent_Release), asCALL_CDECL_OBJLAST);

	engine->RegisterObjectMethod(GAMEEVENT_CLASSNAME, "string GetName()", asFUNCTION(GetName),
				     asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectMethod(GAMEEVENT_CLASSNAME, "bool IsLocal()", asFUNCTION(IsLocal), asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectMethod(GAMEEVENT_CLASSNAME, "bool IsEmpty( const string &in keyName)",
				     asFUNCTION(IsEmpty), asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectMethod(GAMEEVENT_CLASSNAME, "bool GetBool(const string &in keyName, bool defaultValue)",
				     asFUNCTION(GetBool), asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectMethod(GAMEEVENT_CLASSNAME, "int GetInt(const string &in keyName, int defaultValue)",
				     asFUNCTION(GetInt), asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectMethod(GAMEEVENT_CLASSNAME,
				     "float GetFloat(const string &in keyName, float defaultValue)",
				     asFUNCTION(GetFloat), asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectMethod(GAMEEVENT_CLASSNAME,
				     "string GetString(const string &in keyName, const string &in "
				     "defaultValue)",
				     asFUNCTION(GetString), asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectMethod(GAMEEVENT_CLASSNAME, "void SetBool(const string &in keyName, bool value)",
				     asFUNCTION(SetBool), asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectMethod(GAMEEVENT_CLASSNAME, "void SetInt(const string &in keyName, int value)",
				     asFUNCTION(SetInt), asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectMethod(GAMEEVENT_CLASSNAME, "void SetFloat(const string &in keyName, float value)",
				     asFUNCTION(SetFloat), asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectMethod(GAMEEVENT_CLASSNAME,
				     "void SetString(const string &in keyName, const string &in value)",
				     asFUNCTION(SetString), asCALL_CDECL_OBJFIRST);
}