#include "../core/core.h"

#include "../sdk/classes/entity.h"
#include "../sdk/signatures/signatures.h"

#include "../thirdparty/libdetour/libdetour.h"

#include "../features/visuals/viewmodel_aim/viewmodel_aim.h"
#include "../features/visuals/viewmodel_interp/viewmodel_interp.h"
#include "../features/visuals/viewmodel_offset/viewmodel_offset.h"
#include "../features/visuals/norecoil/norecoil.h"
#include "../features/scriptmanager/scriptmanager.h"
#include "../features/hook_initializer/initializer.h"

ADD_SIG(CBaseViewModel_CalcViewModelView, "client.so", "55 48 89 E5 41 57 41 56 41 55 49 89 F5 41 54 49 89 FC 53 48 83 EC 48 8B 41 08")

DETOUR_DECL_TYPE(void, original_CalcViewModelView, void *thisptr, CBaseEntity *, const Vector &, const QAngle &);
detour_ctx_t calcViewModel_ctx;

static void HookedCalcViewModelView(void *thisptr, CBaseEntity *owner, const Vector &eyePosition, const QAngle &eyeAngles)
{
	Vector angle	= eyeAngles;
	Vector position = eyePosition;

	if (owner && gApp->IsInitialized())
	{
		features::scriptmanager.CallHooks("CalcViewModelView", &position, &angle);

		features::norecoil.RunCalcViewModelView(angle);
		features::viewmodel_interp.Run(angle);
		features::viewmodel_aim.Run(angle);
		features::viewmodel_offset.Run(position, angle);
	}

	DETOUR_ORIG_CALL(&calcViewModel_ctx, original_CalcViewModelView, thisptr, owner, position, angle);
}

static void HookCalcViewModelView()
{
	detour_init(&calcViewModel_ctx, Sigs::CBaseViewModel_CalcViewModelView.GetPointer(),
		    (void *)&HookedCalcViewModelView);
	detour_enable(&calcViewModel_ctx);

#ifdef DEBUG
	constexpr Color_t color{100, 255, 100, 255};
	interfaces::Cvar->ConsoleColorPrintf(color, "CalcViewModelView hooked\n");
#endif
}

MARK_FOR_INIT(HookCalcViewModelView)