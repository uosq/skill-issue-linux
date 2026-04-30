#include "clientmodeshared_dopostscreenspaceeffects.h"

#include "../features/backtrack/backtrack.h"
#include "../features/chams/chams.h"
#include "../features/glow/glow.h"
#include "../features/angelscript/api/libraries/hooks/hooks.h"

#include "../hooks.h"

using DoPostScreenSpaceEffectsFn = bool (*)(IClientMode* rdi, CViewSetup* setup);

static bool DoPostScreenSpaceEffects(IClientMode* rdi, CViewSetup* setup)
{
	Backtrack::DoPostScreenSpaceEffects();
	//Chams::Run();
	Glow::Run();

	if (auto pLocal = EntityList::GetLocal(); pLocal != nullptr)
		Chams::OnDoPostScreenSpaceEffects(pLocal);

	Hooks_CallHooks("DoPostScreenSpaceEffects");

	auto original = VMTHooks::ClientMode.GetOriginal<DoPostScreenSpaceEffectsFn>(40);
	return original(rdi, setup);
}

void HookDoPostScreenSpaceEffects(void)
{
	VMTHooks::ClientMode.Hook(40, &DoPostScreenSpaceEffects);

#ifdef DEBUG
	constexpr Color_t color = {100, 255, 100, 255};
	helper::console::ColoredPrint("ClientMode::DoPostScreenSpaceEffects Hooked\n", color);
#endif
}