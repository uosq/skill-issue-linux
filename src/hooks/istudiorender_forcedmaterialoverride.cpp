#include "../features/chams/chams.h"
#include "../features/glow/glow.h"
#include "../features/hookmanager/hookmanager.hpp"

using ForcedMaterialOverrideFn = void (*)(IStudioRender* rdi, IMaterial* mat, OverrideType_t nOverrideType);

INIT_VTABLE_HOOK(ForcedMaterialOverride, void, (IStudioRender* rdi, IMaterial *mat, OverrideType_t nOverrideType), interfaces::StudioRender, 33)
{
	if (features::chams.IsDrawing() || features::glow.IsRunning())
		return;

	original_ForcedMaterialOverride(rdi, mat, nOverrideType);
}