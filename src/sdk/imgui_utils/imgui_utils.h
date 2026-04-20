#pragma once

#include "../../imgui/imgui.h"

namespace ImGui
{
	void DrawBoxFilled(ImDrawList* pDraw, const ImVec2& position, const ImVec2& size, ImU32 color, float rounding = 0.0f, ImDrawFlags flags = 0);
	void DrawBox(ImDrawList* pDraw, const ImVec2& position, const ImVec2& size, ImU32 color, float rounding = 0.0f, ImDrawFlags flags = 0, float thickness = 1.0f);
}
