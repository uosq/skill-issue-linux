#include "istudiorender_forcedmaterialoverride.h"

#include "../sdk/interfaces/interfaces.h"

#include "../features/chams/chams.h"
#include "../features/glow/glow.h"

DECLARE_VTABLE_HOOK(ForcedMaterialOverride, void,
		    (IStudioRender * thisptr, IMaterial *mat, OverrideType_t nOverrideType))
{
	if (Chams::m_bRunning || Glow::m_bRunning)
		return;

	originalForcedMaterialOverride(thisptr, mat, nOverrideType);
}

void HookForcedMaterialOverride()
{
	INSTALL_VTABLE_HOOK(ForcedMaterialOverride, interfaces::StudioRender, 33);

#ifdef DEBUG
	constexpr Color_t color = {100, 255, 100, 255};
	helper::console::ColoredPrint("IStudioRender::ForcedMaterialOverride hooked\n", color);
#endif
}