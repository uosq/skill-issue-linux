#pragma once

#include "../sdk/signatures/signatures.h"

ADD_SIG(CTFPlayer_GetMaxItemCount, "client.so",
	"48 8B BF 98 00 00 00 48 85 FF 74 ? 55 BE 07 00 00 00 48 89 E5 E8 ? "
	"? ? ? 48 85 C0 74 ? 83 78 28 01 75 ? 48 8B 40 08 48 8B 10")

int Hooked_GetMaxItemCount(void *thisptr);
void HookCTFPlayerInventory_MaxItemCount();