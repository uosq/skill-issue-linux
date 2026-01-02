#pragma once

#include "../sdk/definitions/ipanel.h"
#include "../sdk/interfaces/interfaces.h"
#include "../sdk/helpers/helper.h"

#include "../settings.h"

//#include "../gui/gui.h"

using PaintTraverseFn = void (*)(IPanel* thisptr, VPANEL vguiPanel, bool forceRepaint, bool allowForce);
inline PaintTraverseFn originalPaintTraverse = nullptr;

//inline WindowContext windowContext;
//inline BaseStyle style;

static inline float lastSettingsUpdate = 0.0f;

inline void HookedPaintTraverse(IPanel* thisptr, VPANEL vguiPanel, bool forceRepaint, bool allowForce)
{
	originalPaintTraverse(thisptr, vguiPanel, forceRepaint, allowForce);
	
	float currenttime = globalvars->realtime;
	// is this a good way of doing it? absolutely fucking not
	// but im lazy and dont want to hook sdl and vulkan
	if (currenttime - lastSettingsUpdate > 1.0f)
	{
		if (auto response = cli.Get("/"))
		{
			nlohmann::json j = nlohmann::json::parse(response->body);
			settings = j.get<Settings>();
			//helper::console::Print("Updated settings!\n");
		}
		lastSettingsUpdate = currenttime;
	}

	/*const char* name = interfaces::vgui->GetName(vguiPanel);
	std::string panelName = name;

	if (panelName != "MatSystemTopPanel")
		return;

	helper::draw::SetFont(fontManager.GetCurrentFont());
	GUI_Window(windowContext, style, "Hello, world!");
	*/
}

inline void HookPaintTraverse()
{
	//GUI_InitWindow(windowContext, 10, 10, 200, 200);

	void** vt = vtable::get(interfaces::vgui);
	originalPaintTraverse = vtable::hook(vt, 42, &HookedPaintTraverse);

	helper::console::ColoredPrint("IPanel::PaintTraverse hooked\n", (Color_t){100, 255, 100, 255});
}