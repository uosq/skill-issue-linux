#pragma once

#include "../settings.h"
#include "framework.h"
#include "../sdk/helpers/fonts.h"

static void DrawMainWindow()
{
	{
		static int lastHomePress = 0;
		interfaces::InputSystem->PollInputState();
		int tick = interfaces::InputSystem->GetButtonPressedTick(KEY_HOME);

		if (tick > lastHomePress)
		{
			settings.menu_open = !settings.menu_open;
			lastHomePress = tick;
			interfaces::Surface->SetCursorAlwaysVisible(settings.menu_open);
		}

		if (!settings.menu_open)
			return;
	}

	static FrameProperties windowFrame;
	windowFrame.x = 960 - 60;
	windowFrame.y = 540 - 50;
	windowFrame.w = 120;
	windowFrame.h = 100;

	fontManager.SetCurrentFont(fontManager.GetCurrentFont());

	GUI_Frame(&windowFrame);
	{
		static ButtonProperties testProps;
		testProps.x = 10;
		testProps.y = 20;
		testProps.w = 100;
		testProps.h = 20;
		testProps.text = "Hello, world!";

		if (GUI_Button(&testProps, &windowFrame))
			interfaces::Cvar->ConsolePrintf("Clicked\n");
	}
}