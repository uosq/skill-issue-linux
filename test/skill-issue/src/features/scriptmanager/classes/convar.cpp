#include "../../../sol3/sol.hpp"

#include "../../../sdk/definitions/convar.h"

void BindConVar(sol::state& lua)
{
	lua.new_usertype<ConVar>
	(
		"ConVar", sol::no_constructor,

		"GetFloat", [](ConVar* cvar) -> float
		{
			return cvar->GetFloat();
		},

		"GetInt", [](ConVar* cvar) -> int
		{
			return cvar->GetInt();
		},

		"GetString", [](ConVar* cvar) -> const char*
		{
			return cvar->GetString();
		},

		"GetHelpText", [](ConVar* cvar) -> const char*
		{
			return cvar->GetHelpText();
		},

		"IsCommand", [](ConVar* cvar) -> bool
		{
			return cvar->IsCommand();
		},

		"IsFlagSet", [](ConVar* cvar, int flag) -> bool
		{
			return cvar->IsFlagSet(flag);
		},

		"GetName", [](ConVar* cvar) -> const char*
		{
			return cvar->GetName();
		},

		"AddFlags", [](ConVar* cvar, int flags) -> void
		{
			return cvar->AddFlags(flags);
		},

		"SetValue", sol::overload
		(
			[](ConVar* cvar, const char* newvalue) -> void
			{
				cvar->SetValue(newvalue);
			},

			[](ConVar* cvar, float newvalue) -> void
			{
				cvar->SetValue(newvalue);
			},

			[](ConVar* cvar, int newvalue) -> void
			{
				cvar->SetValue(newvalue);
			}
		)
	);
}