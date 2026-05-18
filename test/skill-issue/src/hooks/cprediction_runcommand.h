#pragma once

#include "../sdk/classes/player.h"
#include "../sdk/definitions/cusercmd.h"
#include "../sdk/interfaces/interfaces.h"
#include "../sdk/signatures/signatures.h"

ADD_SIG(CPrediction_RunCommand, "client.so",
	"55 48 89 E5 41 57 49 89 CF 41 56 41 55 49 89 D5 41 54 49 89 FC 53 "
	"48 89 F3")

// sig 55 48 89 E5 41 57 49 89 CF 41 56 41 55 49 89 D5 41 54 49 89 FC 53 48 89 F3
// args CPrediction *self,  C_BasePlayer *player, CUserCmd *ucmd, IMoveHelper *moveHelper
// xref: CPrediction::RunCommand

void Hooked_RunCommand(IPrediction *self, CTFPlayer *player, CUserCmd *ucmd, void *moveHelper);

// I should probably just vtable hook it but oh well :p
void Hook_RunCommand(void);