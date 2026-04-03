#pragma once

#include "../../../include/angelscript.h"
#include <functional>
#include <string>
#include <vector>

struct ASHook
{
	std::string name;
	asIScriptFunction *func;
	asIScriptModule *mod;
};

void Hooks_RegisterLibrary(asIScriptEngine *engine);

// gets all hooks from event
bool Hooks_GetHooks(const std::string &event, std::vector<ASHook> &out);

// calls all functions from event
void Hooks_CallHooks(const std::string &event, const std::function<void(asIScriptContext *)> &argSetter = nullptr);

// used by the script list
void Hooks_RemoveModule(asIScriptModule *module);