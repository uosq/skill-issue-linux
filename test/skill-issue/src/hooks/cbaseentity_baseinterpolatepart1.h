#pragma once

#include "../sdk/signatures/signatures.h"
#include "../sdk/definitions/vector.h"
#include "../sdk/classes/entity.h"

// sig 55 48 89 E5 41 57 49 89 F7 41 56 49 89 CE 41 55 4D 89 CD
// args CBaseEntity* self, float &currentTime, Vector &oldOrigin, QAngle &oldAngles, Vector &oldVel, int &bNoMoreChanges

ADD_SIG(CBaseEntity_BaseInterpolateParte1, "client.so", "55 48 89 E5 41 57 49 89 F7 41 56 49 89 CE 41 55 4D 89 CD");

int Hooked_BaseInterpolatePart1(CBaseEntity *self, float *currentTime, Vector *oldOrigin, QAngle *oldAngles,
				       Vector *oldVel, int *bNoMoreChanges);

void Hook_BaseInterpolatePart1(void);