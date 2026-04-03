#pragma once

#include <string>
#include <vector>

#include "../angelscript/include/angelscript.h"

struct Script
{
	std::string path;
	std::string name;
	bool loaded		= false;
	asIScriptModule *module = nullptr;
};

namespace ScriptManager
{
	bool Load(Script &script);
	bool Unload(Script &script);
	bool Reload(Script &script);
	Script *Find(const std::string &path);
	Script &GetOrCreate(const std::string &fullPath);

	std::vector<Script> &GetScripts();
}; // namespace ScriptManager