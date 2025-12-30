#pragma once

#include "../sdk/sdk.h"
#include "../sdk/definitions/ipanel.h"
#include "../sdk/definitions/color.h"
#include <cwchar>
#include <string>
#include "../sdk/classes/entity.h"
#include "../sdk/helpers/helper.h"

//using PaintTraverseFn = void (*)(IPanel* thisptr, VPANEL vguiPanel, bool forceRepaint, bool allowForce);
using PaintTraverseFn = void (*)(IEngineVGui* thisptr, VGuiPanel_t type);
inline PaintTraverseFn originalPaintTraverse = nullptr;

inline HFont font = 0;

inline void HookedEngineVGuiPaint(IEngineVGui* thisptr, VGuiPanel_t type)
{
	originalPaintTraverse(thisptr, type);

	//interfaces::surface->DrawSetColor(40, 40, 40, 255);
	//helper::draw::FilledRect(100, 100, 800, 800);

	HFont font = helper::draw::GetCurrentFont();
	helper::draw::SetFont(font);

	/*int width, height;
	std::wstring text = helper::draw::ConvertStringToWChar("Hello, world!");
	interfaces::surface->GetTextSize(font, text.c_str(), width, height);

	helper::draw::Text(50, 50, (Color){255, 255, 255, 255}, "Hello, world!\n");*/
}

inline void HookEngineVGuiPaint()
{
	auto vt = vtable::get(interfaces::enginevgui);
	originalPaintTraverse = vtable::hook(vt, 15, HookedEngineVGuiPaint);
	
	/*
		reminder to me:
		if using windows hooks, always add a +1, as (i think)
		gcc/clang puts the constructor&destructor at the start of the class
	*/

	Color_t color;
	color.SetRGB(100, 255, 100, 255);
	helper::console::ColoredPrint("EngineVGui::Paint hooked\n", color);
}