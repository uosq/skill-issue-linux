#pragma once

#include <mutex>
#include <string>
#include <vector>

#include "../../sdk/definitions/color.h"

#include "../feature.h"

class CTFPlayer;

struct HealthbarBounds
{
	float x, y, w, h;
	float bar_x, bar_y, bar_w, bar_h;
};

struct ESPData
{
	float x, y, w, h;
	Color color;
	int entindex = -1;
	bool valid = false;

	float pad_left = 0.0f;
	float pad_right = 0.0f;
	float pad_bottom = 0.0f;
	float pad_top = 0.0f;

	float top_y = 0.0f;
	float bottom_y = 0.0f;
	float left_y = 0.0f;
	float right_y = 0.0f;

	float text_scale = 1.0f; // used to scale with distance

	std::string name = "";
	std::string className = "";
	std::string weaponName = "";

	int health = -1;
	int maxHealth = -1;

	bool isJarated = false;
	bool isBonked = false;
	bool isUbered = false;
	bool isZoomed = false;

	HealthbarBounds hb;
	bool hasHealthbar = false;

	void ResetOffsets();
};

class ESP
{
public:
	void Init();
	void Reset();

	// Call on FRAME_NET_UPDATE_END
	void OnFrameStageNotify();
	void OnLevelShutdown();
	void OnlevelInitPostEntity();
	void OnImGui();

private:
	void FillTargets(CTFPlayer* pLocal);

	std::mutex m_esp_mutex;
	std::vector<ESPData> m_vec_data;
};

DECLARE_FEATURE(ESP, esp)