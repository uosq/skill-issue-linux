#pragma once

#include "../sdk/signatures/signatures.h"

ADD_SIG(CThirdPersonManager_Update, "client.so",
	"55 48 89 E5 41 54 4C 8D 25 ? ? ? ? 53 48 89 FB 49 8B 04 24 48 85 C0 "
	"74 ? 48 8B 40 38")

void Hooked_CThirdPersonManager_Update();
void Hook_CThirdPersonManager_Update();