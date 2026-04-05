#include "cbaseviewmodel_calcviewmodelview.h"

DETOUR_DECL_TYPE(void, original_CalcViewModelView, void *thisptr, CBaseEntity *, const Vector &, const QAngle &);
detour_ctx_t calcViewModel_ctx;

void HookedCalcViewModelView(void *thisptr, CBaseEntity *owner, const Vector &eyePosition,
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

void HookCalcViewModelView()
{
	detour_init(&calcViewModel_ctx, Sigs::CBaseViewModel_CalcViewModelView.GetPointer(),
		    (void *)&HookedCalcViewModelView);
	detour_enable(&calcViewModel_ctx);

#ifdef DEBUG
	constexpr Color_t color{100, 255, 100, 255};
	interfaces::Cvar->ConsoleColorPrintf(color, "CalcViewModelView hooked\n");
#endif
}