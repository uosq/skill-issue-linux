#pragma once

#include "../sdk/signatures/signatures.h"
#include "../sdk/definitions/cbaseanimating.h"

ADD_SIG(CBaseAnimating_Interpolate, "client.so",
	"55 48 89 E5 41 57 41 56 41 55 41 54 53 48 83 EC 68 4C 8B AF B8 07 00 00");

bool Hooked_Interpolate(CBaseAnimating *self, float currentTime);
void Hook_Interpolate(void);