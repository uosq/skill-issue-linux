#pragma once

#include "../sdk/definitions/igameevents.h"


#include "../sdk/signatures/signatures.h"

ADD_SIG(ClientModeShared_FireGameEvent, "client.so",
	"55 48 89 E5 41 57 41 56 49 89 FE 41 55 41 54 49 89 F4 53 48 8D 35")

// Ideally instead of a hook i should just get ClientModeShared and use it instead

void Hooked_FireGameEvent(void *self, IGameEvent *event);
void HookFireGameEvent(void);