#pragma once

#include "../../imgui/imgui.h"
#include "../../settings/settings.h"

#include "../entitylist/entitylist.h"
#include "../esp/esp.h"
#include "../feature.h"

class Radar
{
public:
	void DrawContents();
	void DrawHealthbar(ImDrawList *draw, ImVec2 pos, int health, int maxhealth, int iconSize);

	void Init();

	// Call in EngineVGui->Paint
	void Run();

	float GetRadius();
	int GetRange();
	Vec2 WorldToRadar(const Vector &localPos, const Vector &enemyPos, float viewAnglesYaw);
private:
	int m_iRange{0};
	float m_flRadius{0};
};

DECLARE_FEATURE(Radar, radar)