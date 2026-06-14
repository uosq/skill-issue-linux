#pragma once

#include "../../thirdparty/imgui/imgui.h"

#include "../config/config.h"
#include "../entitylist/entitylist.h"

#include "../esp/esp.h"
#include "../feature.h"

BEGIN_CONFIG(radar)

	ADD_CONFIG(enabled, "radar enabled", false)
	ADD_CONFIG(size, "radar window size", 50)
	ADD_CONFIG(range, "radar detection range", 2048)
	ADD_CONFIG(icon_size, "radar icon size", 10)
	ADD_CONFIG(buildings, "radar show buildings", false)
	ADD_CONFIG(projectiles, "radar show projectiles", false)
	ADD_CONFIG(players, "radar show players", false)

END_CONFIG()

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