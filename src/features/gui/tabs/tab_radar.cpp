#include "../../../thirdparty/imgui/imgui.h"
#include "../../radar/radar.h"

void DrawRadarTab()
{
	ImGui::Checkbox("Enabled", &config::radar::enabled.Get());
	ImGui::SliderInt("Size", &config::radar::size.Get(), 1, 300);
	ImGui::SliderInt("Icon Size", &config::radar::icon_size.Get(), 1, 15);
	ImGui::SliderInt("Range", &config::radar::range.Get(), 10, 3000);

	ImGui::Separator();
	ImGui::Checkbox("Players", &config::radar::players.Get());
	ImGui::Checkbox("Projectiles", &config::radar::projectiles.Get());
	ImGui::Checkbox("Buildings", &config::radar::buildings.Get());
}