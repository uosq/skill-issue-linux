#pragma once

#include "../sdk/signatures/signatures.h"

ADD_SIG(CL_CheckForPureServerWhitelist, "engine.so", "83 3D ? ? ? ? 01 7E ? 80 3D ? ? ? ? 00 75")

void Hooked_CL_CheckForPureServerWhitelist(void *&pFilesToReload);
void HookCheckForPure();