#pragma once

#include "../include/angelscript.h"
#include <string>

namespace API
{
	asIScriptEngine *GetScriptEngine();
	asIScriptContext *GetScriptContext();
	void Initialize();
	void Unitialize();
	bool IsInitialized();
} // api end
