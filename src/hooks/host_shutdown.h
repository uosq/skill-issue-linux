#pragma once

#include <cstdlib>

#include "../libdetour/libdetour.h"
#include "../settings/settings.h"

#include "../sdk/interfaces/interfaces.h"
#include "../sdk/signatures/signatures.h"

#include "../features/angelscript/api/api.h"
#include "../features/angelscript/api/libraries/hooks/hooks.h"

ADD_SIG(Host_Shutdown, "engine.so", "80 3D ? ? ? ? 00 0F 85 ? ? ? ? 55 31 F6")

void HookedHost_ShutdownFn(void);
void HookHost_Shutdown();