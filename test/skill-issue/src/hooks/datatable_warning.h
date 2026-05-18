#pragma once

#include "../sdk/signatures/signatures.h"

/*
Honestly I don't even need to hook this
But the warnings are annoying as hell
*/

//xref: DataTable warning: %s
// sig 55 49 89 FA 48 89 E5 41 54 48 81 EC D8 10 00 00 84 C0 48 89 B5 48 FF FF FF 48 89 95 50 FF FF FF 48 89 8D 58 FF FF FF 4C 89 85 60 FF FF FF 4C 89 8D 68 FF FF FF 74 ? 0F 29 85 70 FF FF FF 0F 29 4D 80 0F 29 55 90 0F 29 5D A0 0F 29 65 B0 0F 29 6D C0 0F 29 75 D0 0F 29 7D E0 4C 8D A5 40 EF FF FF

ADD_SIG(DataTable_Warning, "engine.so",
	"55 49 89 FA 48 89 E5 41 54 48 81 EC D8 10 00 00 84 C0 48 89 B5 48 "
	"FF FF FF 48 89 95 50 FF FF FF 48 89 8D 58 FF FF FF 4C 89 85 60 FF "
	"FF FF 4C 89 8D 68 FF FF FF 74 ? 0F 29 85 70 FF FF FF 0F 29 4D 80 0F "
	"29 55 90 0F 29 5D A0 0F 29 65 B0 0F 29 6D C0 0F 29 75 D0 0F 29 7D "
	"E0 4C 8D A5 40 EF FF FF")

void Hooked_DataTable_Warning(const char *pInMessage, ...);
void Hook_DataTable_Warning(void);