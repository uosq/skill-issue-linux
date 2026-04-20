#include "imgui_utils.h"

void ImGui::DrawBoxFilled(ImDrawList* pDraw, const ImVec2& position, const ImVec2& size, ImU32 color, float rounding, ImDrawFlags flags)
{
	pDraw->AddRectFilled(position, ImVec2(position.x + size.x, position.y + size.y), color, rounding, flags);
}

void ImGui::DrawBox(ImDrawList* pDraw, const ImVec2& position, const ImVec2& size, ImU32 color, float rounding, ImDrawFlags flags, float thickness)
{
	pDraw->AddRect(position, ImVec2(position.x + size.x, position.y + size.y), color, rounding, flags, thickness);
}