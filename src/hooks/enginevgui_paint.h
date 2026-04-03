#pragma once

#include "../sdk/classes/entity.h"
#include "../sdk/classes/weaponbase.h"
#include "../sdk/definitions/color.h"
#include "../sdk/definitions/ipanel.h"
#include "../sdk/handle_utils.h"
#include "../sdk/helpers/helper.h"
#include "../settings/settings.h"
#include <cwchar>
#include <string>

#include "../features/aimbot/aimbot.h"
#include "../features/entitylist/entitylist.h"
#include "../features/esp/esp.h"
#include "../features/warp/warp.h"

#include "../features/angelscript/api/api.h"
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

		FontManager::SetFont(ESP::GetFont());

		CTFPlayer *pLocal = EntityList::GetLocal();
		if (pLocal)
		{
			ESP::Run(pLocal);
			Aimbot::RunPaint();
		}

		// compile time
		if (Settings::menu_open)
		{
			Color color = {255, 255, 255, 255};
			helper::draw::TextShadow(10, 10, color, "Skill Issue");
			helper::draw::TextShadow(10, 30, color, "Build date: " __DATE__ " " __TIME__);
		}

		interfaces::Surface->FinishDrawing();
	}
}

inline void HookEngineVGuiPaint()
{
	INSTALL_VTABLE_HOOK(VGuiPaint, interfaces::EngineVGui, 15);

#ifdef DEBUG
	constexpr Color_t color = {100, 255, 100, 255};
	helper::console::ColoredPrint("EngineVGui::Paint hooked\n", color);
#endif
}