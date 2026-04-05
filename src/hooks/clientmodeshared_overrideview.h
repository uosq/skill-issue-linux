#pragma once

#include "../features/entitylist/entitylist.h"
#include "../sdk/classes/entity.h"
#include "../sdk/classes/player.h"
#include "../sdk/helpers/helper.h"
#include "../sdk/interfaces/interfaces.h"
#include "../settings/settings.h"
#include <string>

#include "../features/visuals/customfov/customfov.h"
#include "../features/visuals/norecoil/norecoil.h"
#include "../features/visuals/thirdperson/thirdperson.h"

#include "../features/angelscript/api/api.h"
#include "../features/angelscript/api/libraries/hooks/hooks.h"

void HookOverrideView();