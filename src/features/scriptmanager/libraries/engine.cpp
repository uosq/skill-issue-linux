#include "../../../sol3/sol.hpp"

#include "../../../sdk/interfaces/interfaces.h"

void BindEngine(sol::state& lua)
{
	sol::table engine = lua.create_named_table("engine");

	engine["PlaySound"] = [](const std::string& sound) -> void
	{
		interfaces::Surface->PlaySound(sound.c_str());
	};

	engine["GetMaxClients"] = []() -> int
	{
		return interfaces::Engine->GetMaxClients();
	};

	engine["GetViewAngles"] = []() -> Vec3
	{
		Vec3 viewangles;
		interfaces::Engine->GetViewAngles(viewangles);
		return viewangles;
	};

	engine["IsTakingScreenshot"] = []() -> bool
	{
		return interfaces::Engine->IsTakingScreenshot();
	};

	engine["SetViewAngles"] = [](Vec3& vec) -> void
	{
		interfaces::Engine->SetViewAngles(vec);
	};

	engine["IsInGame"] = []() -> bool
	{
		return interfaces::Engine->IsInGame();
	};

	engine["ClientCmd"] = [](const std::string& cmd, bool unrestricted = false) -> void
	{
		if (unrestricted)
			interfaces::Engine->ClientCmd_Unrestricted(cmd.c_str());
		else
			interfaces::Engine->ClientCmd(cmd.c_str());
	};

	engine["IsGameUIVisible"] = []() -> bool
	{
		return interfaces::EngineVGui->IsGameUIVisible();
	};

	engine["IsConsoleVisible"] = []() -> bool
	{
		return interfaces::EngineVGui->IsConsoleVisible();
	};

	engine["IsConnected"] = []() -> bool
	{
		return interfaces::Engine->IsConnected();
	};
}