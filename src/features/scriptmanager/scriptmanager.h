#pragma once

#include <string>
#include <vector>

#include "../../sol3/sol.hpp"
#include "../feature.h"

struct LuaHook
{
	std::string name;
	sol::protected_function func;
	sol::environment env;
};

struct Script
{
	std::string path;
	std::string name;
	bool loaded = false;
	sol::environment env;
};

class ScriptManager
{
public:
	ScriptManager() = default;
	~ScriptManager() { Shutdown(); }

	void Init();
	void Shutdown();
	sol::state& GetState() { return m_Lua; }

	bool Load(Script &script);
	bool Unload(Script &script);
	bool Reload(Script &script);
	Script *Find(const std::string &path);
	Script &GetOrCreate(const std::string &fullPath);

	const std::vector<std::string>& GetAvailableScripts() const;
	bool RefreshScripts();

	bool AddHook(const std::string &event, const std::string &id, sol::protected_function func, sol::this_environment te);
	bool RemoveHook(const std::string &event, const std::string &id);

	template <typename... Args>
	void CallHooks(const std::string &event, Args&&... args)
	{
		auto it = m_Hooks.find(event);
		if (it == m_Hooks.end() || it->second.empty())
			return;

		for (const auto &hook : it->second)
		{
			if (!hook.func.valid())
				continue;

			auto result = hook.func(std::forward<Args>(args)...);

			if (!result.valid())
			{
				sol::error err = result;
				// Log err.what()
			}
		}
	}

private:
	void RemoveEnvironmentHooks(const sol::environment& env);

	sol::state m_Lua;
	std::vector<Script> m_Scripts;
	std::vector<std::string> m_ScriptFiles;
	std::unordered_map<std::string, std::vector<LuaHook>> m_Hooks;
};

DECLARE_FEATURE(ScriptManager, scriptmanager);