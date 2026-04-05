#pragma once

#include "../features/entitylist/entitylist.h"
#include "../sdk/helpers/helper.h"
#include "../vtables.h"

#include "../features/angelscript/api/api.h"
#include "../features/angelscript/api/libraries/hooks/hooks.h"

void AS_LevelInitPreEntity_Callback(const char *mapName);
void HookLevelInitPreEntity();