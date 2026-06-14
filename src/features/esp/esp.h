#pragma once

#include <mutex>
#include <string>
#include <vector>

#include "../../sdk/definitions/color.h"

#include "../config/config.h"

#include "../feature.h"

BEGIN_CONFIG(esp)

	ADD_CONFIG(enabled, "esp enabled", false)
	ADD_CONFIG(team_selected, "esp team", 0)
	ADD_CONFIG(ignore_cloaked, "esp ignore cloaked", false)
	ADD_CONFIG(health, "esp health", 0)
	ADD_CONFIG(health_bar, "esp health bar", 0)

	ADD_CONFIG(buildings, "esp buildings", false)
	ADD_CONFIG(name, "esp name", false)
	ADD_CONFIG(box, "esp box", false)
	ADD_CONFIG(weapon, "esp weapon", false)
	ADD_CONFIG(class_name, "esp class name", false)
	ADD_CONFIG(medkit, "esp medkit", false)
	ADD_CONFIG(ammopack, "esp ammopack", false)

	ADD_CONFIG(font_selected, "esp font selected", 0)
	ADD_CONFIG(font_size, "esp font size", 0)

	ADD_CONFIG(condition_zoomed, "esp condition zoomed", false)
	ADD_CONFIG(condition_bonked, "esp condition bonked", false)
	ADD_CONFIG(condition_ubered, "esp condition ubered", false)
	ADD_CONFIG(condition_jarated, "esp condition jarated", false)

	ADD_CONFIG(side_healthbar, "esp side healthbar", 0)
	ADD_CONFIG(side_weaponname, "esp side weaponname", 0)
	ADD_CONFIG(side_classname, "esp side classname", 0)
	ADD_CONFIG(side_name, "esp side name", 0)
	ADD_CONFIG(side_jarate, "esp side jarate", 0)
	ADD_CONFIG(side_uber, "esp side uber", 0)
	ADD_CONFIG(side_bonk, "esp side bonk", 0)
	ADD_CONFIG(side_zoom, "esp side zoom", 0)

	ADD_CONFIG(custom_box_rounding, "esp custom box_rounding", 0)
	ADD_CONFIG(custom_healthbar_rounding, "esp custom healthbar_rounding", 0)
	ADD_CONFIG(custom_healthbar_margin, "esp custom healthbar_margin", 0)
	ADD_CONFIG(custom_healthbar_thickness, "esp custom healthbar_thickness", 1)
	ADD_CONFIG(custom_gap, "esp custom gap", 0)
	ADD_CONFIG(custom_text_padding, "esp custom text_padding", 0)

END_CONFIG()

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