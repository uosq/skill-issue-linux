#pragma once

#include <cstdint>
#include <optional>
#include <string>

enum TF_GC_TEAM
{
	TF_GC_TEAM_DEFENDERS = 0, // Mann	(used as red in ladder games)
	TF_GC_TEAM_INVADERS = 1, // Machine	(used as blue in ladder games)
	TF_GC_TEAM_BROADCASTER = 2,
	TF_GC_TEAM_SPECTATOR = 3,
	TF_GC_TEAM_PLAYER_POOL = 4,				// players that haven't been assign to a team yet

	TF_GC_TEAM_NOTEAM = 5,
};

enum TFLobbyReadyState
{
	TFLobbyReadyState_UNDECLARED = 0,
	TFLobbyReadyState_ACCEPTED = 1,
	TFLobbyReadyState_DECLINED = 2,
};

class CTFLobbyMember
{
public:
	enum class ConnectState
	{
		INVALID = 0,               // Dummy / sentinel value used for debugging
		RESERVATION_PENDING = 1,   // GC is telling gameserver it wants to add a player
		RESERVED = 2,              // Gameserver has acknowledged reservation
		CONNECTED = 3,             // Is currently connected
		DISCONNECTED = 5           // Was connected, but no longer is
	};

	std::optional<uint64_t>          id; // steamid
	std::optional<TF_GC_TEAM>        team;
	std::optional<ConnectState>      connect_state;
	std::optional<std::string>       name;
	std::optional<TFLobbyReadyState> ready_state;
	std::optional<uint64_t>          party_id;
	std::optional<bool>              squad_surplus;
	std::optional<uint32_t>          badge_level;

	// Skillrating has a specified default value in the proto file
	uint32_t skillrating = 10000;

	std::optional<uint32_t>          last_connect_time;
};