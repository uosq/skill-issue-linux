#pragma once

#include "../sdk/signatures/signatures.h"

ADD_SIG(CBasePlayer_ShouldDrawLocalPlayer, "client.so",
	"55 48 89 E5 41 54 48 83 EC 08 48 8D 05 ? ? ? ? 48 8B 38 48 85 FF 74 "
	"? 48 8B 07 FF 50 38")

bool Hooked_CBasePlayer_ShouldDrawLocalPlayer(void);
void Hook_CBasePlayer_ShouldDrawLocalPlayer();