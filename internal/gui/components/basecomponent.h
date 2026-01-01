#pragma once

#include "../../sdk/interfaces/interfaces.h"
#include "../../sdk/definitions/color.h"

struct BaseStyle
{
	Color background{46, 52, 64, 255};
	Color textcolor{255, 255, 255, 255};
	Color border{136, 192, 208, 255};
};

struct BaseComponent
{
	BaseStyle style;

	int x = 0, y = 0;
	int width = 0, height = 0;
};

inline bool IsMouseInside(int x, int y, int width, int height)
{
	int mouseX, mouseY;
	interfaces::surface->SurfaceGetCursorPos(mouseX, mouseY);
	return mouseX >= x && mouseX <= (x + width) && mouseY >= y && mouseY <= (y + height);
}