#pragma once

#include "../sdk/sdk.h"
#include "../sdk/definitions/ipanel.h"
#include "../sdk/definitions/color.h"
#include <cwchar>
#include <string>
#include "../sdk/classes/entity.h"

//using PaintTraverseFn = void (*)(IPanel* thisptr, VPANEL vguiPanel, bool forceRepaint, bool allowForce);
using PaintTraverseFn = void (*)(IEngineVGui* thisptr, VGuiPanel_t type);
inline PaintTraverseFn originalPaintTraverse = nullptr;

inline HFont font = 0;

inline void HookedEngineVGuiPaint(IEngineVGui* thisptr, VGuiPanel_t type)
{
	originalPaintTraverse(thisptr, type);

	interfaces::surface->DrawSetColor(255, 255, 255, 255);
	interfaces::surface->DrawFilledRect(0, 0, 100, 100);

	interfaces::surface->DrawSetColor(255, 0, 0, 255);
	interfaces::surface->DrawOutlinedCircle(50, 50, 48, 64);

	Color color(255, 0, 0, 255);
	interfaces::surface->DrawSetTextFont(font);
	interfaces::surface->DrawSetTextColor(color);
	interfaces::surface->DrawSetTextPos(100, 100);
	interfaces::surface->DrawPrintText(L"Hello, world!", wcslen(L"Hello, world"));

	if (int maxclients = interfaces::enginetool->GetMaxClients())
	{
		for (int i = 0; i < maxclients; i++)
		{
			IClientEntity* entity = interfaces::entitylist->GetClientEntity(i);
			if (!entity)
				continue;
		}
	}
}

inline void HookEngineVGuiPaint()
{
	font = interfaces::surface->CreateFont();
	interfaces::surface->SetFontGlyphSet(font, "Verdana", 16, 400, 0, 0, 0);

	auto vt = vtable::get(interfaces::enginevgui);
	originalPaintTraverse = vtable::hook(vt, 15, HookedEngineVGuiPaint);
	
	/*
		reminder to me:
		if using windows hooks, always add a +1, as (i think)
		gcc/clang puts the constructor&destructor at the start of the class
	*/

	interfaces::vstdlib->ConsolePrintf("Hooked EngineVGui::Paint!\n");
}