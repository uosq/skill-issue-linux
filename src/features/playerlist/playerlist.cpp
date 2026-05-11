#include "playerlist.h"

#include <unordered_map>
#include <filesystem>
#include <shared_mutex> // Add this
#include <mutex>        // Add this
#include <vector>

#include "../../sdk/classes/player.h"

#include "../../simpleini/SimpleIni.h"
#include "../../imgui/imgui.h"
#include "../../settings/settings.h"

#define CACHE_FILE "./skill-issue/players.ini"

void Playerlist::Init()
{
	if (!std::filesystem::exists(CACHE_FILE))
		return;

	CSimpleIniA ini;
	ini.SetUnicode(true);

	SI_Error err = ini.LoadFile(CACHE_FILE);

	if (err != SI_OK)
		return;

	const CSimpleIniA::TKeyVal* section = ini.GetSection("Players");

	if (!section) return;

	std::unique_lock<std::shared_mutex> lock(cache_mutex);

	for (const auto& keyValuePair : *section)
	{
		try
		{
			uint32_t steamID3 = std::stoul(keyValuePair.first.pItem);
			uint8_t priority = static_cast<uint8_t>(std::stoul(keyValuePair.second));

			player_cache[steamID3] = priority;
		}
		catch (const std::exception& e)
		{
			continue;
		}
	}
}

void Playerlist::Shutdown()
{
	std::filesystem::path filePath(CACHE_FILE);
	std::filesystem::create_directories(filePath.parent_path());

	CSimpleIniA ini;
	ini.SetUnicode();

	ini.LoadFile(CACHE_FILE);

	{
		std::shared_lock<std::shared_mutex> lock(cache_mutex);

		for (const auto& [steamID, priority] : player_cache)
		{
			if (priority == 1)
				continue;

			std::string idStr = std::to_string(steamID);
			std::string prioStr = std::to_string(priority);

			ini.SetValue("Players", idStr.c_str(), prioStr.c_str());
		}
	}

	// stop lock here to not mess with IO
	ini.SaveFile(CACHE_FILE);
}

void Playerlist::StorePlayer(CTFPlayer* pPlayer)
{
	if (pPlayer == nullptr)
		return;

	uint32_t steamID3 = pPlayer->GetSteamID3();

	if (steamID3 == 0)
		return;

	{
		std::shared_lock<std::shared_mutex> read_lock(cache_mutex);
		if (player_cache.find(steamID3) != player_cache.end())
			return; // already cached
	}

	std::unique_lock<std::shared_mutex> write_lock(cache_mutex);

	// double check juust in case
	if (player_cache.find(steamID3) == player_cache.end())
		player_cache.insert(std::pair<uint32_t, uint8_t>(steamID3, 1));
}

void Playerlist::AddPlayerToWindow(CTFPlayer* pPlayer)
{
	if (pPlayer == nullptr)
		return;

	uint8_t priority = Playerlist::GetPlayerPriority(pPlayer);
	const std::string& name = pPlayer->GetName();
	uint8_t team = pPlayer->m_iTeamNum();
	uint32_t steamID3 = pPlayer->GetSteamID3();

	std::lock_guard<std::mutex> lock(window_mutex);
        window_list.emplace_back(PlayerData{name, priority, team, steamID3});
}

void Playerlist::ResetWindowList()
{
	std::lock_guard<std::mutex> lock(window_mutex);
	window_list.clear();
}

void Playerlist::Store()
{
	ResetWindowList();

	int max_clients = interfaces::Engine->GetMaxClients();

	for (int i = 1; i <= max_clients; i++)
	{
		CBaseEntity* entity = static_cast<CBaseEntity*>(interfaces::EntityList->GetClientEntity(i));
		if (entity == nullptr)
			continue;

		if (!entity->IsPlayer())
			continue;

		CTFPlayer* pPlayer = static_cast<CTFPlayer*>(entity);
		StorePlayer(pPlayer);
		AddPlayerToWindow(pPlayer);
	}
}

uint8_t Playerlist::GetPlayerPriority(CTFPlayer *pPlayer)
{
	if (pPlayer == nullptr)
		return PLAYERLIST_NORMAL_PRIORITY;

	uint32_t steamID3 = pPlayer->GetSteamID3();

	if (steamID3 == 0)
		return PLAYERLIST_NORMAL_PRIORITY;

	std::shared_lock<std::shared_mutex> read_lock(cache_mutex);

	auto it = player_cache.find(steamID3);

	if (it == player_cache.end())
		return PLAYERLIST_NORMAL_PRIORITY;

	return it->second;
}

void Playerlist::DrawPriority(uint8_t priority)
{
	std::string priority_text = std::to_string(priority);
	ImGui::SameLine();

	float text_width = ImGui::CalcTextSize(priority_text.c_str()).x;
	float right_alignment_x = ImGui::GetWindowWidth() - text_width - ImGui::GetStyle().WindowPadding.x;

	ImGui::SetCursorPosX(right_alignment_x);
	ImGui::TextUnformatted(priority_text.c_str());
}

void Playerlist::SetPlayerPriority(uint32_t steamID3, uint8_t new_priority)
{
        std::unique_lock<std::shared_mutex> write_lock(cache_mutex);
        player_cache[steamID3] = new_priority;
}

void Playerlist::DrawWindow()
{
	if (!Config.misc.packed.playerlist)
		return;

	if (interfaces::Engine->IsTakingScreenshot())
		return;

	std::vector<PlayerData> render_list;
        {
                std::lock_guard<std::mutex> lock(window_mutex);
                render_list = window_list;
        }

	ImGui::SetNextWindowSizeConstraints(ImVec2(150.0f, 0.0f), ImVec2(FLT_MAX, FLT_MAX));

	int flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse;
	if (!Settings::menu_open)
		flags |= ImGuiWindowFlags_NoMove;

	const Color& blu_team_color = Config.colors.blu_team;
	const Color& red_team_color = Config.colors.red_team;

	if (ImGui::Begin("Player List", nullptr, flags))
	{
		for (const auto &entry : render_list)
		{
			const std::string& name = entry.name;
			uint8_t team = entry.team;

			switch (team)
			{
			case TEAM_BLU:
			{
				ImVec4 textColor(blu_team_color.r() / 255.0f, blu_team_color.g() / 255.0f, blu_team_color.b() / 255.0f, 255);
				ImGui::TextColored(textColor, "%s", name.c_str());
				break;
			}

			case TEAM_RED:
			{
				ImVec4 textColor(red_team_color.r() / 255.0f, red_team_color.g() / 255.0f, red_team_color.b() / 255.0f, 255);
				ImGui::TextColored(textColor, "%s", name.c_str());
				break;
			}

			default:
				break;
			}

			uint8_t priority = entry.priority;

			if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
			{
				if (priority < 10)
					priority++;
				else
					priority = 0;

				SetPlayerPriority(entry.steamID3, priority);
			}

			if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
			{
				if (priority == 0)
					priority = 10;
				else
					priority--;

				SetPlayerPriority(entry.steamID3, priority);
			}

			DrawPriority(priority);
		}
	}
	ImGui::End();
}