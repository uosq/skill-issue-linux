#include "scriptmanager.h"

#include <algorithm>
#include <filesystem>

#include "../../sol3/lua/lua.hpp"
#include "../../sdk/interfaces/interfaces.h"

#define SCRIPT_DIR "./skill-issue/scripts"

// classes
extern void BindUserCmd(sol::state& lua);
extern void BindVector(sol::state& lua);
extern void BindGameEvent(sol::state& lua);
extern void BindEntity(sol::state& lua);
extern void BindPlayer(sol::state& lua);
extern void BindWeapon(sol::state& lua);
extern void BindWeaponData(sol::state& lua);

// libraries
extern void BindEntities(sol::state& lua);
extern void BindEngine(sol::state& lua);

static void BindLogging(sol::state& lua)
{
	auto argsToString = [&lua](sol::variadic_args va) -> std::string
	{
		std::string output;

		for (auto it = va.cbegin(); it != va.cend(); ++it)
		{
			std::string str = lua["tostring"](*it);
			output += str;

			if (it + 1 != va.cend())
				output += "\t";
		}
		return output;
	};

	lua.set_function("print", [argsToString](sol::variadic_args va) 
	{
		std::string msg = argsToString(va);
		interfaces::Cvar->ConsolePrintf("%s", msg.c_str());
	});

	lua.set_function("warn", [argsToString](sol::variadic_args va)
	{
		std::string msg = argsToString(va);
		Color_t color{255, 166, 0, 255};
		interfaces::Cvar->ConsoleColorPrintf(color, "%s", msg.c_str());
	});

	// i know &lua is not used
	// shut up clang
	lua.set_function("error", [argsToString](sol::variadic_args va)
	{
		std::string msg = argsToString(va);
		throw std::runtime_error(msg); // i know this is strange, but sol will catch this shit and log it for us
	});
}

void ScriptManager::Init()
{
	m_Lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::string, sol::lib::math, sol::lib::table);

	m_Lua.set_exception_handler([](lua_State* L, sol::optional<const std::exception&> maybe_ex, sol::string_view description)
	{
		std::string err_msg = "C++ Exception: ";

		if (maybe_ex)
			err_msg += maybe_ex.value().what();
		else
			err_msg += std::string(description.data(), description.size());

		Color_t color{255, 100, 100, 255};
		interfaces::Cvar->ConsoleColorPrintf(color, "%s", err_msg.c_str());

		return sol::stack::push(L, err_msg);
	});

	BindLogging(m_Lua);

	sol::table hooks_table = m_Lua.create_named_table("hooks");

	hooks_table["add"] = [this](const std::string &event, const std::string &id, sol::protected_function func, sol::this_environment te)
	{
		return this->AddHook(event, id, func, te);
	};

	hooks_table["remove"] = [this](const std::string &event, const std::string &id)
	{
		return this->RemoveHook(event, id);
	};

	// classes
	BindVector(m_Lua);
	BindUserCmd(m_Lua);
	BindGameEvent(m_Lua);
	BindEntity(m_Lua);
	BindPlayer(m_Lua);
	BindWeaponData(m_Lua);
	BindWeapon(m_Lua);

	// libraries
	BindEngine(m_Lua);
	BindEntities(m_Lua);
}

void ScriptManager::Shutdown()
{
	for (auto& script : m_Scripts)
	{
		if (script.loaded)
			Unload(script);
	}

	m_Scripts.clear();
	m_ScriptFiles.clear();
	m_Hooks.clear();

	m_Lua = sol::state();
}

bool ScriptManager::Load(Script &script)
{
	if (script.loaded)
		return false;

	script.env = sol::environment(m_Lua, sol::create, m_Lua.globals());

	auto result = m_Lua.safe_script_file(script.path, script.env, sol::script_pass_on_error);
	if (!result.valid() || result.status() != sol::call_status::ok)
	{
		sol::error err = result;

		Color_t color{255, 100, 100, 255};
		interfaces::Cvar->ConsoleColorPrintf(color, "%s", err.what());

		return false;
	}

	script.loaded = true;
	return true;
}

bool ScriptManager::Unload(Script &script)
{
	if (!script.loaded)
		return false;

	sol::protected_function unload_func = script.env["unload"];
	if (unload_func.valid())
		unload_func();

	RemoveEnvironmentHooks(script.env);

	script.env = sol::environment();
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

const std::vector<std::string>& ScriptManager::GetAvailableScripts() const
{
	return m_ScriptFiles;
}

bool ScriptManager::RefreshScripts()
{
	m_ScriptFiles.clear();
	const std::filesystem::path dir = SCRIPT_DIR;

	if (!std::filesystem::exists(dir) || !std::filesystem::is_directory(dir))
		return false;

	for (const auto &entry : std::filesystem::directory_iterator(dir))
	{
		if (!entry.is_regular_file())
			continue;

		const auto &path = entry.path();
		if (path.extension() == ".lua")
			m_ScriptFiles.emplace_back(path.filename().string());
	}

	std::sort(m_ScriptFiles.begin(), m_ScriptFiles.end());
	return true;
}

bool ScriptManager::AddHook(const std::string &event, const std::string &id, sol::protected_function func, sol::this_environment te)
{
	if (!func.valid())
		return false;

	auto &eventHooks = m_Hooks[event];
	for (const auto &entry : eventHooks)
	{
		if (entry.name == id)
			return false;
	}

	eventHooks.push_back({id, func, te});
	return true;
}

bool ScriptManager::RemoveHook(const std::string &event, const std::string &id)
{
	auto it = m_Hooks.find(event);
	if (it == m_Hooks.end())
		return false;

	auto &vec = it->second;
	for (auto vec_it = vec.begin(); vec_it != vec.end(); ++vec_it)
	{
		if (vec_it->name == id)
		{
			vec.erase(vec_it);
			return true;
		}
	}
	return false;
}

void ScriptManager::RemoveEnvironmentHooks(const sol::environment& env)
{
	for (auto &[event, vec] : m_Hooks)
	{
		for (auto it = vec.begin(); it != vec.end();)
		{
			if (it->env == env)
				it = vec.erase(it);
			else
				it++;
		}
	}
}