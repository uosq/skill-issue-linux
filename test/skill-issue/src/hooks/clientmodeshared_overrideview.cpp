#include "clientmodeshared_overrideview.h"

#include <string>

#include "../sdk/classes/player.h"

#include "../hooks.h"

#include "../features/entitylist/entitylist.h"
#include "../features/visuals/customfov/customfov.h"
#include "../features/visuals/norecoil/norecoil.h"
#include "../features/visuals/thirdperson/thirdperson.h"

#include "../features/scriptmanager/scriptmanager.h"

using OverrideViewFn = void (*)(IClientMode* rdi, CViewSetup *pView);

static void OverrideView(IClientMode* rdi, CViewSetup *pView)
{
	auto original = VMTHooks::ClientMode.GetOriginal<OverrideViewFn>(17);
	original(rdi, pView);

	if (pView == nullptr)
		return;

	features::scriptmanager.CallHooks("OverrideView", pView);

	if (CTFPlayer *pLocal = features::entities.GetLocal(); pLocal != nullptr)
	{
		features::thirdperson.OverrideView(pLocal, pView);
		features::norecoil.RunOverrideView(pLocal, pView);
		features::customfov.Run(pLocal, pView);
	}
}

void HookOverrideView()
{
	VMTHooks::ClientMode.Hook(17, &OverrideView);

#ifdef DEBUG
	constexpr Color_t color = {100, 255, 100, 255};
	helper::console::ColoredPrint("ClientModeShared::OverrideView hooked\n", color);
#endif
}