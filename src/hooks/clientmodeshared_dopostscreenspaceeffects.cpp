#include "clientmodeshared_dopostscreenspaceeffects.h"

#include "../sdk/interfaces/interfaces.h"

#include "../features/backtrack/backtrack.h"
#include "../features/chams/chams.h"
#include "../features/glow/glow.h"
#include "../features/angelscript/api/libraries/hooks/hooks.h"

DECLARE_VTABLE_HOOK(DoPostScreenSpaceEffects, bool, (IClientMode* thisptr, CViewSetup* setup))
{
	Backtrack::DoPostScreenSpaceEffects();
	//Chams::Run();
	Glow::Run();

	if (auto pLocal = EntityList::GetLocal(); pLocal != nullptr)
		Chams::OnDoPostScreenSpaceEffects(pLocal);

	Hooks_CallHooks("DoPostScreenSpaceEffects");
	return originalDoPostScreenSpaceEffects(thisptr, setup);
}

void HookDoPostScreenSpaceEffects(void)
{
	INSTALL_VTABLE_HOOK(DoPostScreenSpaceEffects, interfaces::ClientMode, 40);

#ifdef DEBUG
	constexpr Color_t color = {100, 255, 100, 255};
	helper::console::ColoredPrint("ClientMode::DoPostScreenSpaceEffects Hooked\n", color);
#endif
}