#pragma once

#include "components/window.h"

struct GUI
{
	void Init()
	{
		window.Init();
	}

	void Paint()
	{
		window.Paint();
	}

	private:
	Window window;
};