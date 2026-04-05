#pragma once

#include "../sdk/classes/entity.h"
#include "../sdk/classes/player.h"
#include "../sdk/helpers/helper.h"
#include "../sdk/interfaces/interfaces.h"

#include "../features/backtrack/backtrack.h"
#include "../features/chams/chams.h"
#include "../features/glow/glow.h"
#include "../features/visuals/thirdperson/thirdperson.h"

#include "../features/angelscript/api/api.h"
#include "../features/angelscript/api/libraries/hooks/hooks.h"

void HookDoPostScreenSpaceEffects(void);