#include "../../../sol3/sol.hpp"

#include "../../../sdk/interfaces/interfaces.h"
#include "../../../sdk/helpers/engine/engine.h"
#include "../../../sdk/definitions/cgametrace.h"
#include "../../../sdk/definitions/ctracefilters.h"

void BindEngine(sol::state& lua)
{
	sol::table engine = lua.create_named_table("engine");

	engine["play_sound"] = [](const std::string& sound) -> void
	{
		interfaces::Surface->PlaySound(sound.c_str());
	};

	engine["get_max_clients"] = []() -> int
	{
		return interfaces::Engine->GetMaxClients();
	};

	engine["get_viewangles"] = []() -> Vec3
	{
		Vec3 viewangles;
		interfaces::Engine->GetViewAngles(viewangles);
		return viewangles;
	};

	engine["is_taking_screenshot"] = []() -> bool
	{
		return interfaces::Engine->IsTakingScreenshot();
	};

	engine["set_viewangles"] = [](Vec3& vec) -> void
	{
		interfaces::Engine->SetViewAngles(vec);
	};

	engine["is_in_game"] = []() -> bool
	{
		return interfaces::Engine->IsInGame();
	};

	engine["client_cmd"] = [](const std::string& cmd, bool unrestricted = false) -> void
	{
		if (unrestricted)
			interfaces::Engine->ClientCmd_Unrestricted(cmd.c_str());
		else
			interfaces::Engine->ClientCmd(cmd.c_str());
	};

	engine["is_gameui_visible"] = []() -> bool
	{
		return interfaces::EngineVGui->IsGameUIVisible();
	};

	engine["is_console_visible"] = []() -> bool
	{
		return interfaces::EngineVGui->IsConsoleVisible();
	};

	engine["is_connected"] = []() -> bool
	{
		return interfaces::Engine->IsConnected();
	};

	engine["trace_line"] = [](const Vec3& start, const Vec3& end, unsigned int mask, sol::function callback) -> CGameTrace
	{
		if (!callback.valid())
			return {};

		CGameTrace trace {};
		CTraceFilterLua filter {callback};

		helper::engine::Trace(start, end, mask, &filter, &trace);

		return trace;
	};

	engine["trace_hull"] = [](const Vec3& start, const Vec3& end, const Vec3& mins, const Vec3& maxs, unsigned int mask, sol::function callback) -> CGameTrace
	{
		if (!callback.valid())
			return {};

		CGameTrace trace {};
		CTraceFilterLua filter {callback};

		helper::engine::TraceHull(start, end, mins, maxs, mask, &filter, &trace);

		return trace;
	};

	engine["get_screen_size"] = []() -> std::tuple<int, int>
	{
		int width, height;
		interfaces::Engine->GetScreenSize(width, height);
		return std::make_tuple(width, height);
	};
}