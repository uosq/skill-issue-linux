#pragma once

#include <cstdint>
#include <string>

class CTFPlayer;

namespace Playerlist
{
	void Initialize();
	void Shutdown();
	void Store();

	void DrawWindow();

	//std::string GetPlayerName(CTFPlayer* pPlayer);
	uint8_t GetPlayerPriority(CTFPlayer* pPlayer);
}