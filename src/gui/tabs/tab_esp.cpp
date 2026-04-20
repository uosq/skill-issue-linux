#include "tab_esp.h"

#include "../../imgui/imgui.h"
#include "../../settings/settings.h"

// normal checkbox
static uint32_t DrawCheckboxWithSide(const char* label, bool* v, uint32_t current_side)
{
	ImGui::PushID(label);

	ImGui::Checkbox(label, v);
	ImGui::SameLine();

	if (ImGui::Button("+", ImVec2(22, 0)))
		ImGui::OpenPopup("SidePopup");

	uint32_t out_side = current_side;
	if (ImGui::BeginPopup("SidePopup"))
	{
		ImGui::TextDisabled("Position: %s", label);
		ImGui::Separator();

		int temp = static_cast<int>(current_side);
		if (ImGui::Selectable("Left", temp == 0)) temp = 0;
		if (ImGui::Selectable("Right", temp == 1)) temp = 1;
		if (ImGui::Selectable("Top", temp == 2)) temp = 2;
		if (ImGui::Selectable("Bottom", temp == 3)) temp = 3;
		out_side = static_cast<uint32_t>(temp);

		ImGui::EndPopup();
	}

	ImGui::PopID();
	return out_side;
}

// for bitwise flags
static uint32_t DrawCheckboxFlagsWithSide(const char* label, int* flags, int flags_value, uint32_t current_side)
{
	ImGui::PushID(label);

	ImGui::CheckboxFlags(label, flags, flags_value);
	ImGui::SameLine();

	if (ImGui::Button("+", ImVec2(22, 0)))
		ImGui::OpenPopup("SidePopup");

	uint32_t out_side = current_side;
	if (ImGui::BeginPopup("SidePopup"))
	{
		ImGui::TextDisabled("Position: %s", label);
		ImGui::Separator();

		int temp = static_cast<int>(current_side);
		if (ImGui::Selectable("Left", temp == 0)) temp = 0;
		if (ImGui::Selectable("Right", temp == 1)) temp = 1;
		if (ImGui::Selectable("Top", temp == 2)) temp = 2;
		if (ImGui::Selectable("Bottom", temp == 3)) temp = 3;
		out_side = static_cast<uint32_t>(temp);

		ImGui::EndPopup();
	}

	ImGui::PopID();
	return out_side;
}

static uint32_t DrawComboWithSide(const char* label, int* current_item, const char* const items[], int items_count, uint32_t current_side)
{
	ImGui::PushID(label);

	ImGui::Combo(label, current_item, items, items_count);
	ImGui::SameLine();

	if (ImGui::Button("+", ImVec2(22, 0)))
		ImGui::OpenPopup("SidePopup");

	uint32_t out_side = current_side;
	if (ImGui::BeginPopup("SidePopup"))
	{
		ImGui::TextDisabled("Position: %s", label);
		ImGui::Separator();

		int temp = static_cast<int>(current_side);
		if (ImGui::Selectable("Left",   temp == 0)) temp = 0;
		if (ImGui::Selectable("Right",  temp == 1)) temp = 1;
		if (ImGui::Selectable("Top",    temp == 2)) temp = 2;
		if (ImGui::Selectable("Bottom", temp == 3)) temp = 3;
		out_side = static_cast<uint32_t>(temp);

		ImGui::EndPopup();
	}

	ImGui::PopID();
	return out_side;
}

void DrawESPTab()
{
	if (ImGui::BeginTable("##ESPContents", 2))
	{
		ImGui::TableSetupColumn("LeftSide");
		ImGui::TableSetupColumn("RightSide");

		ImGui::TableNextRow();

		// left
		ImGui::TableNextColumn();
		ImGui::Checkbox("ESP Enabled", &Settings::ESP.enabled);

		ImGui::BeginDisabled(!Settings::ESP.enabled);
		{
			Settings::ESP.sides.name = DrawCheckboxWithSide("Name", &Settings::ESP.name, Settings::ESP.sides.name);

			ImGui::Checkbox("Box", &Settings::ESP.box);
			ImGui::Checkbox("Ignore Cloaked", &Settings::ESP.ignorecloaked);
			ImGui::Checkbox("Buildings", &Settings::ESP.buildings);

			Settings::ESP.sides.weaponname = DrawCheckboxWithSide("Weapon", &Settings::ESP.weapon, Settings::ESP.sides.weaponname);
			Settings::ESP.sides.classname  = DrawCheckboxWithSide("Class", &Settings::ESP.class_name, Settings::ESP.sides.classname);

			{
				constexpr const char *items[]{"None", "Text", "Bar", "Both"};
				Settings::ESP.sides.healthbar = DrawComboWithSide("Health", &Settings::ESP.health, items, 4, Settings::ESP.sides.healthbar);
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


			ImGui::SliderInt("Font Size", &Settings::ESP.font_size, 8, 32);
		}

		// right
		ImGui::TableNextColumn();

		ImGui::TextUnformatted("Conditions");

		Settings::ESP.sides.zoom   = DrawCheckboxFlagsWithSide("Zoom", &Settings::ESP.fconditions, static_cast<int>(ESPConditionFlags::Zoomed), Settings::ESP.sides.zoom);
		Settings::ESP.sides.uber   = DrawCheckboxFlagsWithSide("Ubercharge", &Settings::ESP.fconditions, static_cast<int>(ESPConditionFlags::Ubered), Settings::ESP.sides.uber);
		Settings::ESP.sides.jarate = DrawCheckboxFlagsWithSide("Jarate", &Settings::ESP.fconditions, static_cast<int>(ESPConditionFlags::Jarated), Settings::ESP.sides.jarate);
		Settings::ESP.sides.bonk   = DrawCheckboxFlagsWithSide("Bonk", &Settings::ESP.fconditions, static_cast<int>(ESPConditionFlags::Bonked), Settings::ESP.sides.bonk);

		ImGui::EndTable();
	} // table end

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