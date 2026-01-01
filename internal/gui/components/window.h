#pragma once

#include "basecomponent.h"
#include "paintablecomponent.h"
#include "../../sdk/helpers/helper.h"
#include <vector>
#include <format>

struct WindowHeader
{
	int height = 20;
	std::string title = "Vapo - Linux";
};

struct Window: public IPaintableComponent
{
	WindowHeader header;
	std::vector<BaseComponent> children;

	bool dragging = false;

	void Init()
	{
		width = 300;
		height = 300;
		x = 100;
		y = 100;
	}

	void GetHeaderPos(int &x, int&y)
	{
		x = this->x - 1;
		y = this->y - header.height - 1;
	}

	void Drag(int oldMouseX, int oldMouseY, int mouseX, int mouseY)
	{
		bool isDown = interfaces::inputsystem->IsButtonDown(MOUSE_LEFT);
		if (!isDown)
		{
			dragging = false;
			return;
		}
				
		int hx, hy;
		GetHeaderPos(hx, hy);
		int tickpressed = interfaces::inputsystem->GetButtonPressedTick(MOUSE_LEFT);

		if (tickpressed >= globalvars->tickcount && isDown && IsMouseInside(hx, hy, width, header.height))
		{
			dragging = true;
		}

		if (dragging)
		{
			x += (mouseX - oldMouseX);
			y += (mouseY - oldMouseY);
		}
	}

	void Paint() override
	{
		static int oldMouseX = 0;
		static int oldMouseY = 0;

		int mouseX, mouseY;
		interfaces::surface->SurfaceGetCursorPos(mouseX, mouseY);

		Drag(oldMouseX, oldMouseY, mouseX, mouseY);

		{	// background
			helper::draw::SetColor(style.background);
			helper::draw::FilledRect(x, y, x + width, y + height);
		}

		{	// border
			helper::draw::SetColor(style.border);
			helper::draw::OutlinedRect(x - 1, y - 1, x + width + 1, y + height + 1);
		}

		{	// header
			int hx, hy;
			GetHeaderPos(hx, hy);

			helper::draw::SetColor(style.border);
			helper::draw::FilledRect(hx, hy, hx + width + 2, hy + header.height + 1);

			{	// title
				int textw, texth;
				helper::draw::GetTextSize(header.title, textw, texth);
	
				int textx, texty;
				textx = x + (int)(width*0.5f) - (int)(textw*0.5f);
				texty = hy + (int)(header.height*0.5f) - (int)(texth*0.5f);
				helper::draw::Text(textx, texty, style.textcolor, header.title);
			}
		}

		oldMouseX = mouseX;
		oldMouseY = mouseY;
	}
};