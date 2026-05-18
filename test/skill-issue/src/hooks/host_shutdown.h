#pragma once

#include "../sdk/signatures/signatures.h"

ADD_SIG(Host_Shutdown, "engine.so", "80 3D ? ? ? ? 00 0F 85 ? ? ? ? 55 31 F6")

void HookedHost_ShutdownFn(void);
void HookHost_Shutdown();