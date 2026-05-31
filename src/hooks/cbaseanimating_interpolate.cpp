#include "../thirdparty/libdetour/libdetour.h"

#include "../sdk/signatures/signatures.h"
#include "../sdk/definitions/cbaseanimating.h"

#include "../features/warp/warp.h"
#include "../features/entitylist/entitylist.h"

#include "../core/core.h"

#include "../features/hook_initializer/initializer.h"

ADD_SIG(CBaseAnimating_Interpolate, "client.so", "55 48 89 E5 41 57 41 56 41 55 41 54 53 48 83 EC 68 4C 8B AF B8 07 00 00");

DETOUR_DECL_TYPE(bool, Interpolate, CBaseEntity *self, float currentTime);
static detour_ctx_t interpolate_ctx;

static bool Hooked_Interpolate(CBaseAnimating *self, float currentTime)
{
	if (!gApp->IsInitialized() || self == nullptr)
	{
		bool retVal;
		DETOUR_ORIG_GET(&interpolate_ctx, retVal, Interpolate, self, currentTime);
		return retVal;
	}

	if (features::warp.IsShifting() || features::warp.IsRecharging())
	{
		auto pLocal = features::entities.GetLocal();
		if (pLocal && pLocal == self)
			return true;
	}

	bool retVal;
	DETOUR_ORIG_GET(&interpolate_ctx, retVal, Interpolate, self, currentTime);
	return retVal;
}

static void Hook_Interpolate(void)
{
	//xref: C_BaseAnimating::Interpolate
	//void* original = sigscan_module("client.so", "55 48 89 E5 41 57 41 56 41 55 41 54 53 48 83 EC 68 4C 8B AF B8 07 00 00");

	detour_init(&interpolate_ctx, Sigs::CBaseAnimating_Interpolate.GetPointer(), (void *)&Hooked_Interpolate);
	if (!detour_enable(&interpolate_ctx))
	{
		interfaces::Cvar->ConsolePrintf("Failed to hook CBaseAnimating::Interpolate\n");
		return;
	}

#ifdef DEBUG
	interfaces::Cvar->ConsolePrintf("CBaseAnimating::Interpolate hooked\n");
#endif
}

MARK_FOR_INIT(Hook_Interpolate)