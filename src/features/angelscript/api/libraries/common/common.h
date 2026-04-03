#pragma once

#include "../../../include/angelscript.h"
#include <string>

void Common_RegisterLibrary(asIScriptEngine *engine);
bool Execute(const std::string &text);

/* Example code
Print("hi");
*/