#pragma once

#include "../sdk/classes/entity.h"

#include "../sdk/signatures/signatures.h"

ADD_SIG(CBaseViewModel_CalcViewModelView, "client.so",
	"55 48 89 E5 41 57 41 56 41 55 49 89 F5 41 54 49 89 FC 53 48 83 EC "
	"48 8B 41 08")

void HookedCalcViewModelView(void *thisptr, CBaseEntity *owner, const Vector &eyePosition, const QAngle &eyeAngles);
void HookCalcViewModelView();