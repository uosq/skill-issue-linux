#pragma once

#include "../sdk/helpers/helper.h"
#include <functional>
#include <string>

struct BaseStyle
{
	// r g b a
	Color background = {40, 40, 40, 255};
	Color text = {255, 255, 255, 255};
	Color border = {136, 192, 208, 255};
};

struct FrameProperties
{
	BaseStyle style;
	int x = 0, y = 0, w = 0, h = 0;
	int oldMouseX = 0, oldMouseY = 0;
	bool dragging = false;
};

struct ButtonProperties: public FrameProperties
{
	std::string text = "";
};

static void _GUIBorder(FrameProperties* props, FrameProperties* frame = nullptr)
{
	int x, y;
	x = props->x;
	y = props->y;

	if (frame != nullptr)
	{
		x += frame->x;
		y += frame->y;
	}

	helper::draw::SetColor(props->style.border);
	helper::draw::OutlinedRect(x - 1, y - 1, x + props->w + 1, y + props->h + 1);
}

static void _GUIBackground(FrameProperties* props, FrameProperties* frame = nullptr)
{
	int x, y;
	x = props->x;
	y = props->y;

	if (frame != nullptr)
	{
		x += frame->x;
		y += frame->y;
	}

	helper::draw::SetColor(props->style.background);
	helper::draw::FilledRect(x, y, x + props->w, y + props->h);
}

static void _GUITextCentered(Color color, int x, int y, std::string text)
{
	if (text.empty())
		return;

	int tw, th, tx, ty;
	helper::draw::GetTextSize(text, tw, th);

	tx = static_cast<int>(x - (tw*0.5f));
	ty = static_cast<int>(y - (th*0.5f));

	helper::draw::TextShadow(tx, ty, color, text);
}

inline bool _IsMouseInside(int x, int y, int width, int height)
{
	int mouseX, mouseY;
	interfaces::Surface->SurfaceGetCursorPos(mouseX, mouseY);
	return mouseX >= x && mouseX <= (x + width) && mouseY >= y && mouseY <= (y + height);
}

inline bool _IsLeftClicking()
{
	static int lastTickPressed = 0;

	// not sure if this is correct
	interfaces::InputSystem->PollInputState();
	int tick = interfaces::InputSystem->GetButtonPressedTick(MOUSE_LEFT);
	//interfaces::Cvar->ConsolePrintf("%d\n", tick);

	if (tick > lastTickPressed)
	{
		lastTickPressed = tick;
		return true;
	}

	return false;
}

inline void _GUIDragFrame(FrameProperties *props, int mouseX, int mouseY)
{
	bool isDown = interfaces::InputSystem->IsButtonDown(MOUSE_LEFT);
	if (!isDown)
	{
		props->dragging = false;
		return;
	}

	if (_IsLeftClicking() && _IsMouseInside(props->x, props->y, props->w, 10))
		props->dragging = true;

	if (props->dragging)
	{
		props->x += (mouseX - props->oldMouseX);
		props->y += (mouseY - props->oldMouseY);
	}
}

static void GUI_Frame(FrameProperties* props)
{
	int x, y, w, h;
	x = props->x;
	y = props->y;
	w = props->w;
	h = props->h;

	int mouseX, mouseY;
	helper::draw::GetMousePosition(x, y);

	_GUIDragFrame(props, mouseX, mouseY);
	_GUIBorder(props);
	_GUIBackground(props);
}

static bool GUI_Button(ButtonProperties* props, FrameProperties* frame = nullptr)
{
	int x, y, w, h;
	x = props->x;
	y = props->y;
	w = props->w;
	h = props->h;

	if (frame != nullptr)
	{
		x += frame->x;
		y += frame->y;
	}

	_GUIBorder(props, frame);
	_GUIBackground(props, frame);

	if (!props->text.empty())
		_GUITextCentered(props->style.text, x + (w*0.5f), y + (h*0.5f), props->text);

	return _IsMouseInside(x, y, w, h) && _IsLeftClicking();
}