#pragma once

// ExecuteStringCommand
// engine.so 55 48 89 E5 41 57 41 56 4C 8D B5 C0 F9 FF FF

#include "../sdk/signatures/signatures.h"

ADD_SIG(CGameClient_ExecuteStringCommand, "engine.so", "55 48 89 E5 41 57 41 56 4C 8D B5 C0 F9 FF FF")

bool HookedExecuteStringCommand(void *self, const char *pCommandString);
void Hook_ExecuteStringCommand();