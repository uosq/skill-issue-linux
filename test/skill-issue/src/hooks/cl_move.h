#pragma once

#include "../sdk/signatures/signatures.h"

ADD_SIG(CL_Move, "engine.so", "55 48 89 E5 41 57 41 56 41 55 41 54 53 48 83 EC 78 83 3D ? ? ? ? 01")

// CL_Move(float accumulated_shit, bool bFinalTick) 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 83 EC 78 83 3D ? ? ? ? 01
// CL_SendMove(void) 55 66 0F EF C0 48 89 E5 41 57 41 56 48 8D BD E8 EF FF FF

void HookedCL_Move(float accumulated_extra_samples, bool bFinalTick);
void HookCL_Move(void);