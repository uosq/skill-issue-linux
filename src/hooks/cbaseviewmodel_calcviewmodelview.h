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

inline detour_ctx_t calcViewModel_ctx;
DETOUR_DECL_TYPE(void, original_CalcViewModelView, void *thisptr, CBaseEntity *, const Vector &, const QAngle &);

ADD_SIG(CBaseViewModel_CalcViewModelView, "client.so",
	"55 48 89 E5 41 57 41 56 41 55 49 89 F5 41 54 49 89 FC 53 48 83 EC "
	"48 8B 41 08")

inline void HookedCalcViewModelView(void *thisptr, CBaseEntity *owner, const Vector &eyePosition,
				    const QAngle &eyeAngles)
{
	Vector angle	= eyeAngles;
	Vector position = eyePosition;

	if (owner)
	{
		Hooks_CallHooks("CalcViewModelView", [&](asIScriptContext *ctx) {
			ctx->SetArgObject(0, &position);
			ctx->SetArgObject(1, &angle);
		});

		ViewmodelInterp::Run(angle);
		ViewmodelAim::Run(angle);
		ViewmodelOffset::Run(position, angle);
	}

	DETOUR_ORIG_CALL(&calcViewModel_ctx, original_CalcViewModelView, thisptr, owner, position, angle);
}

inline void HookCalcViewModelView()
{
	detour_init(&calcViewModel_ctx, Sigs::CBaseViewModel_CalcViewModelView.GetPointer(),
		    (void *)&HookedCalcViewModelView);
	detour_enable(&calcViewModel_ctx);

#ifdef DEBUG
	constexpr Color_t color{100, 255, 100, 255};
	interfaces::Cvar->ConsoleColorPrintf(color, "CalcViewModelView hooked\n");
#endif
}