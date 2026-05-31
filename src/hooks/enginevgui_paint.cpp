#include "../core/core.h"

#include "../sdk/definitions/ienginevgui.h"
#include "../sdk/interfaces/interfaces.h"

#include "../hooks.h"

#include "../features/scriptmanager/scriptmanager.h"
#include "../features/hook_initializer/initializer.h"

using VGuiPaintFn = void (*)(IEngineVGuiInternal *rdi, PaintMode_t paint);

static void VGuiPaint(IEngineVGuiInternal* rdi, PaintMode_t paint)
{
	auto original = VMTHooks::EngineVGui.GetOriginal<VGuiPaintFn>(15);
	original(rdi, paint);

	if (!gApp->IsInitialized() || interfaces::Engine->IsTakingScreenshot())
		return;

	if (paint & PAINT_UIPANELS)
	{
		interfaces::Surface->StartDrawing();

		features::scriptmanager.CallHooks("Draw");

		interfaces::Surface->FinishDrawing();
	}
}

void HookEngineVGuiPaint()
{
	VMTHooks::Initialize();

	VMTHooks::EngineVGui.Hook(15, &VGuiPaint);

#ifdef DEBUG
	constexpr Color_t color = {100, 255, 100, 255};
	helper::console::ColoredPrint("EngineVGui::Paint hooked\n", color);
#endif
}

MARK_FOR_INIT(HookEngineVGuiPaint)