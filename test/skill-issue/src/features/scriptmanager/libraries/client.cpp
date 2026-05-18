#include "../../../sol3/sol.hpp"

#include "../../../sdk/interfaces/interfaces.h"
#include "../../../sdk/helpers/engine/engine.h"

void BindClient(sol::state& lua)
{
	sol::table client = lua.create_named_table("client");

	client["get_player_info"] = [](int index) -> std::optional<player_info_t>
	{
		// 0 is CWorld
		if (index <= 0 || index > interfaces::Engine->GetMaxClients())
			return std::nullopt;

		player_info_t pi;

		if (!interfaces::Engine->GetPlayerInfo(index, &pi))
			return std::nullopt;

		return pi;
	};

	client["world_to_screen"] = &helper::engine::WorldToScreen;

	client["get_convar"] = [](const char* name) -> std::optional<ConVar*>
	{
		if (name == nullptr || name[0] == '\0')
			return std::nullopt;

		ConVar* cvar = interfaces::Cvar->FindVar(name);

		if (cvar == nullptr)
			return std::nullopt;

		return cvar;
	};
}