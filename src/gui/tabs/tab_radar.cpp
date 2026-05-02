#include "tab_radar.h"

#include "../../imgui/imgui.h"
#include "../../settings/settings.h"

#include "../utils/gui_utils.h"

void DrawRadarTab()
{
	ImGui_CheckboxBit("Enabled", Config.radar.packed.enabled);
	ImGui_SliderIntBit("Size", Config.radar.packed.size, 1, 300);
	ImGui_SliderIntBit("Icon Size", Config.radar.packed.icon_size, 1, 15);
	ImGui_SliderIntBit("Range", Config.radar.packed.range, 10, 3000);

	ImGui::Separator();
	ImGui_CheckboxBit("Players", Config.radar.packed.players);
	ImGui_CheckboxBit("Projectiles", Config.radar.packed.projectiles);
	// ImGui_CheckboxBit("Objective", Config.radar.packed.objective);
	ImGui_CheckboxBit("Buildings", Config.radar.packed.buildings);
}