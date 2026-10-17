#include "../features/backtrack/backtrack.h"
#include "../features/chams/chams.h"
#include "../features/glow/glow.h"
#include "../features/scriptmanager/scriptmanager.h"
#include "../features/hookmanager/hookmanager.hpp"

using DoPostScreenSpaceEffectsFn = bool (*)(IClientMode* rdi, CViewSetup* setup);

INIT_VTABLE_HOOK(DoPostScreenSpaceEffects, bool, (IClientMode* rdi, CViewSetup* setup), interfaces::ClientMode, 40)
{
	features::backtrack.DoPostScreenSpaceEffects();
	//Chams::Run();
	features::glow.Run();

	if (auto pLocal = features::entities.GetLocal(); pLocal != nullptr)
		features::chams.OnDoPostScreenSpaceEffects(pLocal);

	features::scriptmanager.CallHooks("DoPostScreenSpaceEffects");

	return original_DoPostScreenSpaceEffects(rdi, setup);
}
