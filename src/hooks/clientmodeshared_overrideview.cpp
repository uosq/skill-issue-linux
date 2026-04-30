#include "clientmodeshared_overrideview.h"

#include <string>

#include "../sdk/classes/player.h"

#include "../hooks.h"

#include "../features/entitylist/entitylist.h"
#include "../features/visuals/customfov/customfov.h"
#include "../features/visuals/norecoil/norecoil.h"
#include "../features/visuals/thirdperson/thirdperson.h"

#include "../features/angelscript/api/libraries/hooks/hooks.h"

using OverrideViewFn = void (*)(IClientMode* rdi, CViewSetup *pView);

static void OverrideView(IClientMode * rdi, CViewSetup *pView)
{
	auto original = VMTHooks::ClientMode.GetOriginal<OverrideViewFn>(17);
	original(rdi, pView);

	if (pView == nullptr)
		return;

	Hooks_CallHooks("OverrideView", [&](asIScriptContext *ctx) { ctx->SetArgObject(0, &pView); });

	if (CTFPlayer *pLocal = EntityList::GetLocal(); pLocal != nullptr)
	{
		Thirdperson::OverrideView(pLocal, pView);

		if (!pLocal->IsAlive())
			return;

		NoRecoil::RunOverrideView(pLocal, pView);
		CustomFov::Run(pLocal, pView);
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