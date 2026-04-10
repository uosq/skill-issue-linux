#include "tab_radar.h"

#include "../../imgui/imgui.h"
#include "../../settings/settings.h"

void DrawRadarTab()
{
	ImGui::Checkbox("Enabled", &Settings::Radar.enabled);
	ImGui::SliderInt("Size", &Settings::Radar.size, 1, 300);
	ImGui::SliderInt("Icon Size", &Settings::Radar.icon_size, 1, 15);
	ImGui::SliderInt("Range", &Settings::Radar.range, 10, 3000);

	ImGui::Separator();
	ImGui::Checkbox("Players", &Settings::Radar.players);
	ImGui::Checkbox("Projectiles", &Settings::Radar.projectiles);
	// ImGui::Checkbox("Objective", &Settings::Radar.objective);
	ImGui::Checkbox("Buildings", &Settings::Radar.buildings);
}