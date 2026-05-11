#include "cbaseviewmodel_calcviewmodelview.h"

#include <string>

#include "../libdetour/libdetour.h"

#include "../features/visuals/viewmodel_aim/viewmodel_aim.h"
#include "../features/visuals/viewmodel_interp/viewmodel_interp.h"
#include "../features/visuals/viewmodel_offset/viewmodel_offset.h"
#include "../features/visuals/norecoil/norecoil.h"
#include "../features/scriptmanager/scriptmanager.h"

DETOUR_DECL_TYPE(void, original_CalcViewModelView, void *thisptr, CBaseEntity *, const Vector &, const QAngle &);
detour_ctx_t calcViewModel_ctx;

void HookedCalcViewModelView(void *thisptr, CBaseEntity *owner, const Vector &eyePosition,
				    const QAngle &eyeAngles)
{
	Vector angle	= eyeAngles;
	Vector position = eyePosition;

	if (owner)
	{
		features::scriptmanager.CallHooks("CalcViewModelView", &position, &angle);

		features::norecoil.RunCalcViewModelView(angle);
		features::viewmodel_interp.Run(angle);
		features::viewmodel_aim.Run(angle);
		features::viewmodel_offset.Run(position, angle);
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