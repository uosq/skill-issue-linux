#pragma once

#include "../libdetour/libdetour.h"
#include "../libsigscan.h"
#include "../settings/settings.h"

#include "../sdk/classes/entity.h"

#include "../features/entitylist/entitylist.h"
#include "../features/logs/logs.h"

#include "../sdk/signatures/signatures.h"

ADD_SIG(CBaseEntity_ShouldDraw, "client.so", "31 C0 80 BF AC 00 00 00 0A")

bool Hooked_CBaseEntity_ShouldDraw(CBaseEntity *self);
void Hook_CBaseEntity_ShouldDraw();