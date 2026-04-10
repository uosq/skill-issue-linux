#include "scriptmanager.h"

#include <algorithm>
#include <filesystem>
#include <fstream>

#include "../angelscript/api/globals.h"
#include "../angelscript/api/libraries/hooks/hooks.h"

#define SCRIPT_DIR "./skill-issue/scripts/"

static std::vector<Script> m_Scripts;
static std::vector<std::string> s_vScriptFiles;

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

std::vector<std::string>& ScriptManager::GetAvailableAngelScripts()
{
	return s_vScriptFiles;
}

bool ScriptManager::RefreshAngelScripts()
{
	s_vScriptFiles.clear();

	const std::filesystem::path dir = SCRIPT_DIR;

	if (!std::filesystem::exists(dir) || !std::filesystem::is_directory(dir))
		return false;

	for (const auto &entry : std::filesystem::directory_iterator(dir))
	{
		if (!entry.is_regular_file())
			continue;

		const auto &path = entry.path();

		if (path.extension() == ".as")
			s_vScriptFiles.emplace_back(path.filename().string());
	}

	std::sort(s_vScriptFiles.begin(), s_vScriptFiles.end());
	return true;
}