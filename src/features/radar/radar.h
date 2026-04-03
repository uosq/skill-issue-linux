#pragma once

#include "../../imgui/imgui.h"
#include "../../settings/settings.h"
#include "../entitylist/entitylist.h"
#include "../esp/esp.h"

namespace Radar
{
	void DrawContents();
	void DrawHealthbar(ImDrawList *draw, ImVec2 pos, int health, int maxhealth, int iconSize);

	extern int m_iRange;
	extern float m_flRadius;

	void Init();

	// Call in EngineVGui->Paint
	void Run();

	float GetRadius();
	int GetRange();
	Vec2 WorldToRadar(const Vector &localPos, const Vector &enemyPos, float viewAnglesYaw);
}; // namespace Radar