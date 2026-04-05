#pragma once

#include "../sdk/classes/entity.h"
#include "../sdk/classes/player.h"
#include "../sdk/helpers/helper.h"
#include "../sdk/interfaces/interfaces.h"
#include "../settings/settings.h"
#include <string>

#include "../features/aimbot/aimbot.h"
#include "../features/visuals/viewmodel_aim/viewmodel_aim.h"
#include "../features/visuals/viewmodel_interp/viewmodel_interp.h"
#include "../features/visuals/viewmodel_offset/viewmodel_offset.h"

#include "../libdetour/libdetour.h"

#include "../features/angelscript/api/api.h"
#include "../features/angelscript/api/libraries/hooks/hooks.h"

#include "../sdk/signatures/signatures.h"

ADD_SIG(CBaseViewModel_CalcViewModelView, "client.so",
	"55 48 89 E5 41 57 41 56 41 55 49 89 F5 41 54 49 89 FC 53 48 83 EC "
	"48 8B 41 08")

void HookedCalcViewModelView(void *thisptr, CBaseEntity *owner, const Vector &eyePosition, const QAngle &eyeAngles);
void HookCalcViewModelView();