#include "imgui_utils.h"

void ImGui::DrawBoxFilled(ImDrawList* pDraw, const ImVec2& position, const ImVec2& size, ImU32 color, float rounding, ImDrawFlags flags)
{
	pDraw->AddRectFilled(position, ImVec2(position.x + size.x, position.y + size.y), color, rounding, flags);
}

void ImGui::DrawBox(ImDrawList* pDraw, const ImVec2& position, const ImVec2& size, ImU32 color, float rounding, ImDrawFlags flags, float thickness)
{
	pDraw->AddRect(position, ImVec2(position.x + size.x, position.y + size.y), color, rounding, flags, thickness);
}

void ImGui::DrawTextShadow(ImDrawList* pDraw, const ImVec2& position, ImU32 color, const char* text)
{
	// shadow
	pDraw->AddText(ImVec2(position.x + 1, position.y + 1), IM_COL32(0, 0, 0, 255), text);

	// text
	pDraw->AddText(position, color, text);
}

void ImGui::DrawTextShadow(ImDrawList* pDraw, ImFont* font, float font_size, const ImVec2& position, ImU32 color, const char* text)
{
	// shadow
	pDraw->AddText(font, font_size, ImVec2(position.x + 1, position.y + 1), IM_COL32(0, 0, 0, 255), text);

	// text
	pDraw->AddText(font, font_size, position, color, text);
}