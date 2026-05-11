#pragma once

#include <cstdint>
#include <shared_mutex>
#include <string>
#include <unordered_map>
#include <vector>

#include "../../sdk/definitions/eteam.h"

#include "../feature.h"

#define PLAYERLIST_NORMAL_PRIORITY 1
#define PLAYERLIST_FRIEND_PRIORITY 0

class CTFPlayer;

struct PlayerData
{
	std::string name = "";
	uint8_t priority = PLAYERLIST_NORMAL_PRIORITY;
	uint8_t team = ETeam::TEAM_UNASSIGNED;
	uint32_t steamID3 = 0;
};

class Playerlist
{
public:
	void Init();
	void Shutdown();
	void Store();

	void DrawWindow();

	//std::string GetPlayerName(CTFPlayer* pPlayer);
	uint8_t GetPlayerPriority(CTFPlayer* pPlayer);

private:
	void StorePlayer(CTFPlayer* pPlayer);
	void AddPlayerToWindow(CTFPlayer* pPlayer);
	void ResetWindowList();
	void SetPlayerPriority(uint32_t steamID3, uint8_t new_priority);
	void DrawPriority(uint8_t priority);

	// [player index, priority]
	std::unordered_map<uint32_t, uint8_t> player_cache;
	std::vector<PlayerData> window_list;
	std::shared_mutex cache_mutex;
	std::mutex window_mutex;
};

DECLARE_FEATURE(Playerlist, playerlist)