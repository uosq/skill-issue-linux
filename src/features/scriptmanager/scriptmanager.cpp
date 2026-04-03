#include "scriptmanager.h"

#include <filesystem>
#include <fstream>

#include "../angelscript/api/globals.h"
#include "../angelscript/api/libraries/hooks/hooks.h"

static std::vector<Script> m_Scripts;

bool ScriptManager::Load(Script &script)
{
	if (script.loaded)
		return false;

	std::ifstream file(script.path);
	if (!file.is_open())
		return false;

	std::string code, line;
	while (std::getline(file, line))
		code += line + "\n";

	auto engine  = GetScriptEngine();
	auto *module = engine->GetModule(script.path.c_str(), asGM_ALWAYS_CREATE);

	if (module == nullptr)
		return false;

	module->AddScriptSection("main", code.c_str());
	module->SetAccessMask(GetScriptAccessMask());

	if (module->Build() < 0)
	{
		engine->DiscardModule(module->GetName());
		return false;
	}

	auto *func = module->GetFunctionByDecl("void main()");
	if (!func)
	{
		engine->DiscardModule(module->GetName());
		return false;
	}

	script.module = module;
	script.loaded = true;

	// execute main
	auto *ctx = engine->CreateContext();
	ctx->Prepare(func);
	ctx->Execute();
	ctx->Release();

	return true;
}

bool ScriptManager::Unload(Script &script)
{
	if (!script.loaded || !script.module)
		return false;

	auto engine = GetScriptEngine();

	// clean shit
	if (auto *func = script.module->GetFunctionByDecl("void unload()"))
	{
		auto *ctx = engine->CreateContext();
		ctx->Prepare(func);
		ctx->Execute();
		ctx->Release();
	}

	Hooks_RemoveModule(script.module);

	// destroy the module
	engine->DiscardModule(script.module->GetName());

	script.module = nullptr;
	script.loaded = false;

	return true;
}

bool ScriptManager::Reload(Script &script)
{
	if (script.loaded)
		Unload(script);

	return Load(script);
}

Script *ScriptManager::Find(const std::string &path)
{
	for (auto &s : m_Scripts)
	{
		if (s.path == path)
			return &s;
	}
	return nullptr;
}

Script &ScriptManager::GetOrCreate(const std::string &fullPath)
{
	if (auto *s = Find(fullPath))
		return *s;

	Script script;
	script.path = fullPath;
	script.name = std::filesystem::path(fullPath).filename().string();

	m_Scripts.push_back(script);
	return m_Scripts.back();
}