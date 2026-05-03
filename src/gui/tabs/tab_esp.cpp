#include "tab_esp.h"

#include "../../imgui/imgui.h"
#include "../../settings/settings.h"

#include "../utils/gui_utils.h"

#define ImGui_CheckboxWithSideBit(label, bitfield, side_bitfield) \
[&]() \
{ \
	bool temp = (bitfield); \
	side_bitfield = DrawCheckboxWithSide(label, &temp, side_bitfield); \
	if (temp != (bitfield)) bitfield = temp; \
}()

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

static uint32_t DrawSliderWithFlag(const char* label, uint32_t flagValue, int min, int max)
{
	int current = int(flagValue);
	ImGui::SliderInt(label, &current, min, max);
	return uint32_t(current);
}

void DrawESPTab()
{
	if (ImGui::BeginTable("##ESPContents", 2, ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_SizingStretchProp))
	{
		ImGui::TableSetupColumn("LeftSide");
		ImGui::TableSetupColumn("RightSide");

		// row 1
		ImGui::TableNextRow();

		ImGui::TableNextColumn();
		ImGui::TextUnformatted("ESP");
		ImGui_CheckboxBit("Enabled##ESP", Config.esp.packed.enabled);

		ImGui::BeginDisabled(!Config.esp.packed.enabled);
		{
			ImGui_CheckboxWithSideBit("Name", Config.esp.packed.name, Config.esp.sides.name);
			ImGui_CheckboxBit("Box", Config.esp.packed.box);
			ImGui_CheckboxBit("Ignore Cloaked", Config.esp.packed.ignorecloaked);
			ImGui_CheckboxBit("Buildings", Config.esp.packed.buildings);

			ImGui_CheckboxWithSideBit("Weapon", Config.esp.packed.weapon, Config.esp.sides.weaponname);
			ImGui_CheckboxWithSideBit("Class", Config.esp.packed.class_name, Config.esp.sides.classname);

			{
				constexpr const char *items[]{"None", "Text", "Bar", "Both"};
				Config.esp.sides.healthbar = DrawComboWithSide("Health##ESP", &Config.esp.health, items, 4, Config.esp.sides.healthbar);
			}

			{
				constexpr const char *items[]{"Only Enemies", "Only Teammates", "Both"};
				ImGui::Combo("Team Selection##ESP", &Config.esp.team_selection, items, 3);
			}
		}
		ImGui::EndDisabled();

		{
			constexpr const char *items[]{"TF2", "Arial"};

			int temp_font = Config.esp.font.selected;
			if (ImGui::Combo("Font", &temp_font, items, 2))
				Config.esp.font.selected = temp_font;

			ImGui_SliderIntBit("Font Size##ESP", Config.esp.font.size, 8, 32);
		}

		ImGui::TableNextColumn();

		ImGui::TextUnformatted("Conditions");
		ImGui_CheckboxWithSideBit("Zoom", Config.esp.conditions.zoomed, Config.esp.sides.zoom);
		ImGui_CheckboxWithSideBit("Ubercharge", Config.esp.conditions.ubered, Config.esp.sides.uber);
		ImGui_CheckboxWithSideBit("Jarate", Config.esp.conditions.jarated, Config.esp.sides.jarate);
		ImGui_CheckboxWithSideBit("Bonk", Config.esp.conditions.bonked, Config.esp.sides.bonk);

		ImGui::Separator();

		ImGui::TextUnformatted("Glow");
		ImGui_CheckboxBit("Enabled##Glow", Config.glow.packed.enabled);
		ImGui_SliderIntBit("Stencil##Glow", Config.glow.packed.stencil, 0, 10);
		ImGui_SliderIntBit("Blur##Glow", Config.glow.packed.blur, 0, 10);
		ImGui_CheckboxBit("Highlight Weapon##Glow", Config.glow.packed.weapon);

		ImGui::Separator();

		ImGui::TextUnformatted("Chams");
		ImGui::Checkbox("Enabled##Chams", &Config.chams.enabled);

		// row 2
		ImGui::TableNextRow();

		ImGui::TableNextColumn();
		ImGui::Separator();

		ImGui::TextUnformatted("Colors");

		float red[3] = {Config.colors.red_team.r() / 255.0f, Config.colors.red_team.g() / 255.0f, Config.colors.red_team.b() / 255.0f};
		float blu[3] = {Config.colors.blu_team.r() / 255.0f, Config.colors.blu_team.g() / 255.0f, Config.colors.blu_team.b() / 255.0f};
		float target[3] = {Config.colors.aimbot_target.r() / 255.0f, Config.colors.aimbot_target.g() / 255.0f, Config.colors.aimbot_target.b() / 255.0f};
		float weapon[3] = {Config.colors.weapon.r() / 255.0f, Config.colors.weapon.g() / 255.0f, Config.colors.weapon.b() / 255.0f};

		if (ImGui::ColorEdit3("RED Team", red))
			Config.colors.red_team.SetColor(red[0] * 255.0f, red[1] * 255.0f, red[2] * 255.0f, 255.0f);

		if (ImGui::ColorEdit3("BLU Team", blu))
			Config.colors.blu_team.SetColor(blu[0] * 255.0f, blu[1] * 255.0f, blu[2] * 255.0f, 255.0f);

		if (ImGui::ColorEdit3("Aimbot Target", target))
			Config.colors.aimbot_target.SetColor(target[0] * 255.0f, target[1] * 255.0f, target[2] * 255.0f, 255.0f);

		if (ImGui::ColorEdit3("Weapon", weapon))
			Config.colors.weapon.SetColor(weapon[0] * 255.0f, weapon[1] * 255.0f, weapon[2] * 255.0f, 255.0f);

		ImGui::TableNextColumn();
		ImGui::Separator();

		ImGui::TextUnformatted("Customization");

		Config.esp.custom.box_rounding = DrawSliderWithFlag("Box Roundness", Config.esp.custom.box_rounding, 0, 15);
		Config.esp.custom.healthbar_rounding = DrawSliderWithFlag("Health Bar Roundness", Config.esp.custom.healthbar_rounding, 0, 15);
		Config.esp.custom.healthbar_margin = DrawSliderWithFlag("Health Bar Margin", Config.esp.custom.healthbar_margin, 0, 15);
		Config.esp.custom.healthbar_thickness = DrawSliderWithFlag("Health Bar Thickness", Config.esp.custom.healthbar_thickness, 1, 15);
		Config.esp.custom.gap = DrawSliderWithFlag("Health Bar Padding", Config.esp.custom.gap, 0, 15);
		Config.esp.custom.text_padding = DrawSliderWithFlag("Text Padding", Config.esp.custom.text_padding, 0, 15);

		ImGui::EndTable();
	} // table end
}