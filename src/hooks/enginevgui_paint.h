#pragma once

#include "../sdk/definitions/ipanel.h"
#include <cwchar>
#include <string>
#include "../sdk/classes/entity.h"
#include "../sdk/helpers/helper.h"
#include "../sdk/definitions/color.h"
#include "../sdk/classes/weaponbase.h"
#include "../sdk/handle_utils.h"
#include "../settings.h"

#include "../features/esp/esp.h"
#include "../features/aimbot/aimbot.h"
#include "../features/visuals/visuals.h"
#include "../features/entitylist/entitylist.h"

DECLARE_VTABLE_HOOK(VGuiPaint, void, (IEngineVGui* thisptr, PaintMode_t paint))
{
	originalVGuiPaint(thisptr, paint);

	if (paint & PAINT_UIPANELS)
	{
		interfaces::Surface->StartDrawing();

		HFont font = helper::draw::GetCurrentFont();
		helper::draw::SetFont(font);
	
		//Color color{255, 255, 255, 255};
		//helper::draw::TextShadow(10, 10, color, "Vapo Linux");
	
		CTFPlayer* pLocal = EntityList::GetLocal();
		if (pLocal)
		{
			ESP::Run(pLocal);
			Aimbot::RunPaint();
		}

		interfaces::Surface->FinishDrawing();
	}
}

inline void HookEngineVGuiPaint()
{
	INSTALL_VTABLE_HOOK(VGuiPaint, interfaces::EngineVGui, 15);

	Color_t color;
	color.SetRGB(100, 255, 100, 255);
	helper::console::ColoredPrint("EngineVGui::Paint hooked\n", color);
}