#include "enginevgui_paint.h"

#include "../sdk/classes/weaponbase.h"
#include "../sdk/definitions/color.h"
#include "../settings/settings.h"
#include <string>

#include "../features/aimbot/aimbot.h"
#include "../features/entitylist/entitylist.h"
#include "../features/esp/esp.h"

#include "../features/angelscript/api/libraries/hooks/hooks.h"

#include "../core/core.h"

DECLARE_VTABLE_HOOK(VGuiPaint, void, (IEngineVGui * thisptr, PaintMode_t paint))
{
	originalVGuiPaint(thisptr, paint);

	// I don't trust
	// C++'s static initialization
	if (!gApp.IsInitialized())
		return gApp.Setup();

	if (interfaces::Engine->IsTakingScreenshot())
		return;

	if (paint & PAINT_UIPANELS)
	{
		interfaces::Surface->StartDrawing();

		Hooks_CallHooks("Draw");

		interfaces::Surface->FinishDrawing();
	}
}

void HookEngineVGuiPaint()
{
	INSTALL_VTABLE_HOOK(VGuiPaint, interfaces::EngineVGui, 15);

#ifdef DEBUG
	constexpr Color_t color = {100, 255, 100, 255};
	helper::console::ColoredPrint("EngineVGui::Paint hooked\n", color);
#endif
}