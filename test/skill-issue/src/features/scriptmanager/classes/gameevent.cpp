#include "../../../sol3/sol.hpp"

#include "../../../sdk/definitions/igameevents.h"

void BindGameEvent(sol::state& lua)
{
	lua.new_usertype<IGameEvent>
	(
		"GameEvent", sol::no_constructor,
		"IsReliable", &IGameEvent::IsReliable,
		"IsLocal", &IGameEvent::IsLocal,
		"IsEmpty", &IGameEvent::IsEmpty,
		"GetBool", &IGameEvent::GetBool,
		"GetInt", &IGameEvent::GetInt,
		"GetFloat", &IGameEvent::GetFloat,
		"SetBool", &IGameEvent::SetBool,
		"SetInt", &IGameEvent::SetInt,
		"SetFloat", &IGameEvent::SetFloat,
		"SetString", &IGameEvent::SetString,
		"GetName", &IGameEvent::GetName,
		"GetString", &IGameEvent::GetString,

		"GetBool", [](IGameEvent* ev, const char* key, sol::optional<bool> def)
		{
			return ev->GetBool(key, def.value_or(false));
		},

		"GetInt", [](IGameEvent* ev, const char* key, sol::optional<int> def)
		{
			return ev->GetInt(key, def.value_or(0));
		},

		"GetFloat", [](IGameEvent* ev, const char* key, sol::optional<float> def)
		{
			return ev->GetFloat(key, def.value_or(0.0f));
		},

		"GetString", [](IGameEvent* ev, const char* key, sol::optional<const char*> def)
		{
			return ev->GetString(key, def.value_or(""));
		}
	);
}