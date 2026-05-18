#include "istudiorender_forcedmaterialoverride.h"

#include "../features/chams/chams.h"
#include "../features/glow/glow.h"

#include "../hooks.h"

using ForcedMaterialOverrideFn = void (*)(IStudioRender* rdi, IMaterial* mat, OverrideType_t nOverrideType);

static void ForcedMaterialOverride(IStudioRender* rdi, IMaterial *mat, OverrideType_t nOverrideType)
{
	if (features::chams.IsDrawing() || features::glow.IsRunning())
		return;

	auto original = VMTHooks::StudioRender.GetOriginal<ForcedMaterialOverrideFn>(33);
	original(rdi, mat, nOverrideType);
}

void HookForcedMaterialOverride()
{
	VMTHooks::StudioRender.Hook(33, &ForcedMaterialOverride);

#ifdef DEBUG
	constexpr Color_t color = {100, 255, 100, 255};
	helper::console::ColoredPrint("IStudioRender::ForcedMaterialOverride hooked\n", color);
#endif
}