#include "clientmodeshared_overrideview.h"

DECLARE_VTABLE_HOOK(OverrideView, void, (IClientMode * thisptr, CViewSetup *pView))
{
	originalOverrideView(thisptr, pView);

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
	INSTALL_VTABLE_HOOK(OverrideView, interfaces::ClientMode, 17);

#ifdef DEBUG
	constexpr Color_t color = {100, 255, 100, 255};
	helper::console::ColoredPrint("ClientModeShared::OverrideView hooked\n", color);
#endif
}