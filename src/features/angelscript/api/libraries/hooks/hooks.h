#pragma once

#include "../../../include/angelscript.h"
#include <functional>
#include <string>
#include <vector>

struct ASHook
{
	std::string name;
	asIScriptFunction* func;
};

void Hooks_RegisterLibrary(asIScriptEngine* engine);
bool Hooks_GetHooks(const std::string& event, std::vector<ASHook>& out);
void Hooks_CallHooks(const std::string& name, const std::function<void(asIScriptContext*)>& argSetter = nullptr);