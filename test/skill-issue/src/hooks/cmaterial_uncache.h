#pragma once

#include "../sdk/definitions/imaterial.h"
#include "../sdk/signatures/signatures.h"

ADD_SIG(CMaterial_Uncache, "materialsystem.so", "55 48 89 E5 41 56 41 55 41 54 49 89 FC 53 89 F3 48 81 EC 10 01 00 00")

void HookedUncache(IMaterial *mat, bool bPreserveVars);
void HookCMaterial_Uncache(void);