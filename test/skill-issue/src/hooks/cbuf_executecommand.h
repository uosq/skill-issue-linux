#pragma once

#include "../sdk/signatures/signatures.h"

ADD_SIG(Cbuf_ExecuteCommand, "engine.so", "55 48 89 E5 41 54 53 48 8B 1D ? ? ? ? 48 8B 03 48 85 C0")

void Hook_Cbuf_ExecuteCommand();