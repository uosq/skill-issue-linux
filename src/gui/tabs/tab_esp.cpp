#include "tab_esp.h"

#include "../../imgui/imgui.h"
#include "../../settings/settings.h"

void DrawESPTab()
{
	if (ImGui::BeginTable("##ESPContents", 2))
	{
		ImGui::TableSetupColumn("LeftSide");
		ImGui::TableSetupColumn("RightSide");

		ImGui::TableNextRow();

		ImGui::TableNextColumn();
		ImGui::Checkbox("ESP Enabled", &Settings::ESP.enabled);
		ImGui::BeginDisabled(!Settings::ESP.enabled);
		{
			ImGui::Checkbox("Name", &Settings::ESP.name);
			ImGui::Checkbox("Box", &Settings::ESP.box);
			ImGui::Checkbox("Ignore Cloaked", &Settings::ESP.ignorecloaked);
			ImGui::Checkbox("Buildings", &Settings::ESP.buildings);
			ImGui::Checkbox("Weapon", &Settings::ESP.weapon);

			{
				constexpr const char *items[]{"None", "Text", "Bar", "Both"};
				ImGui::Combo("Health", &Settings::ESP.health, items, 4);
			}

			{
				constexpr const char *items[]{"Only Enemies", "Only Teammates", "Both"};
				ImGui::Combo("Team Selection", &Settings::ESP.team_selection, items, 3);
			}
		}
		ImGui::EndDisabled();
		{
			constexpr const char *items[]{"TF2", "Arial"};
			ImGui::Combo("Font", &Settings::ESP.font, items, 2);
		}

		ImGui::TableNextColumn();

		ImGui::TextUnformatted("Conditions");

		ImGui::CheckboxFlags("Zoom", &Settings::ESP.fconditions, static_cast<int>(ESPConditionFlags::Zoomed));
		ImGui::CheckboxFlags("Ubercharge", &Settings::ESP.fconditions,
				     static_cast<int>(ESPConditionFlags::Ubered));
		ImGui::CheckboxFlags("Jarate", &Settings::ESP.fconditions,
				     static_cast<int>(ESPConditionFlags::Jarated));
		ImGui::CheckboxFlags("Bonk", &Settings::ESP.fconditions, static_cast<int>(ESPConditionFlags::Bonked));

		ImGui::EndTable();
	} // table end

	ImGui::Separator();

	ImGui::Checkbox("Chams", &Settings::ESP.chams);
	ImGui::SliderFloat("Chams Alpha", &Settings::ESP.chams_alpha, 0.0f, 1.0f);

	ImGui::Separator();

	ImGui::TextUnformatted("Glow");
	ImGui::SliderInt("Stencil", &Settings::ESP.stencil, 0, 10);
	ImGui::SliderInt("Blur", &Settings::ESP.blur, 0, 10);

	ImGui::Separator();

	float red[3] = {Settings::Colors.red_team.r() / 255.0f, Settings::Colors.red_team.g() / 255.0f, Settings::Colors.red_team.b() / 255.0f};
	float blu[3] = {Settings::Colors.blu_team.r() / 255.0f, Settings::Colors.blu_team.g() / 255.0f, Settings::Colors.blu_team.b() / 255.0f};
	float target[3] = {Settings::Colors.aimbot_target.r() / 255.0f, Settings::Colors.aimbot_target.g() / 255.0f, Settings::Colors.aimbot_target.b() / 255.0f};
	float weapon[3] = {Settings::Colors.weapon.r() / 255.0f, Settings::Colors.weapon.g() / 255.0f, Settings::Colors.weapon.b() / 255.0f};

	ImGui::TextUnformatted("Colors");

	if (ImGui::ColorEdit3("RED Team", red))
		Settings::Colors.red_team.SetColor(red[0] * 255.0f, red[1] * 255.0f, red[2] * 255.0f, 255.0f);

	if (ImGui::ColorEdit3("BLU Team", blu))
		Settings::Colors.blu_team.SetColor(blu[0] * 255.0f, blu[1] * 255.0f, blu[2] * 255.0f, 255.0f);

	if (ImGui::ColorEdit3("Aimbot Target", target))
		Settings::Colors.aimbot_target.SetColor(target[0] * 255.0f, target[1] * 255.0f, target[2] * 255.0f, 255.0f);

	if (ImGui::ColorEdit3("Weapon", weapon))
		Settings::Colors.weapon.SetColor(weapon[0] * 255.0f, weapon[1] * 255.0f, weapon[2] * 255.0f, 255.0f);
}