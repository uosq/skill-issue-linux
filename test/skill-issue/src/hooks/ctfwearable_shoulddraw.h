#pragma once

// CTFWearable::ShouldDraw(void* self);
// sig 55 48 89 E5 41 57 41 56 41 55 41 54 49 89 FC 53 48 83 EC 18 8B 97 54 07 00 00
// xref: ghost_wearable
// dll: client.so
// There are 2 functions that have the xref
// Get the one that only has 1 parameter

#include "../sdk/signatures/signatures.h"

ADD_SIG(CTFWearable_ShouldDraw, "client.so",
	"55 48 89 E5 41 57 41 56 41 55 41 54 49 89 FC 53 48 83 EC 18 8B 97 "
	"54 07 00 00")

bool Hooked_CTFWearable_ShouldDraw(void *self);
void Hook_CTFWearable_ShouldDraw();