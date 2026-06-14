#include <algorithm>
#include <cstdint>

#include "../../../thirdparty/imgui/imgui.h"

#include "../../esp/esp.h"
#include "../../colors/colors.h"
#include "../../glow/glow.h"
#include "../../chams/chams.h"

// normal checkbox
static void DrawCheckboxWithSide(const char* label, bool* v, int& current_side)
{
        ImGui::PushID(label);

        ImGui::Checkbox(label, v);
        ImGui::SameLine();

        if (ImGui::Button("+", ImVec2(22, 0)))
                ImGui::OpenPopup("SidePopup");

        if (ImGui::BeginPopup("SidePopup"))
        {
                ImGui::TextDisabled("Position: %s", label);
                ImGui::Separator();

                int temp = current_side;
                if (ImGui::Selectable("Left",   temp == 0)) temp = 0;
                if (ImGui::Selectable("Right",  temp == 1)) temp = 1;
                if (ImGui::Selectable("Top",    temp == 2)) temp = 2;
                if (ImGui::Selectable("Bottom", temp == 3)) temp = 3;
                
                current_side = temp;

                ImGui::EndPopup();
        }

        ImGui::PopID();
}

static void DrawComboWithSide(const char* label, int* current_item, const char* const items[], int items_count, int& current_side)
{
        ImGui::PushID(label);

        ImGui::Combo(label, current_item, items, items_count);
        ImGui::SameLine();

        if (ImGui::Button("+", ImVec2(22, 0)))
                ImGui::OpenPopup("SidePopup");

        if (ImGui::BeginPopup("SidePopup"))
        {
                ImGui::TextDisabled("Position: %s", label);
                ImGui::Separator();

                int temp = current_side;
                if (ImGui::Selectable("Left",   temp == 0)) temp = 0;
                if (ImGui::Selectable("Right",  temp == 1)) temp = 1;
                if (ImGui::Selectable("Top",    temp == 2)) temp = 2;
                if (ImGui::Selectable("Bottom", temp == 3)) temp = 3;
                
                current_side = temp;

                ImGui::EndPopup();
        }

        ImGui::PopID();
}

static uint32_t DrawSliderWithFlag(const char* label, uint32_t flagValue, int min, int max)
{
	int current = int(flagValue);
	ImGui::SliderInt(label, &current, min, max);
	return uint32_t(current);
}

static void ChangeMenuAccentColor()
{
	const Color& accent = config::colors::menu_accent.Get();
	ImGuiStyle& style = ImGui::GetStyle();

	// Divide by 255.0f to convert 0-255 into 0.0f-1.0f for ImGui math
	float r = accent.r() / 255.0f;
	float g = accent.g() / 255.0f;
	float b = accent.b() / 255.0f;
	float a = accent.a() / 255.0f;

	float h, s, v;
	ImGui::ColorConvertRGBtoHSV(r, g, b, h, s, v);

	float hovered_s = std::clamp(s * 0.88f, 0.0f, 1.0f);
	float hovered_v = std::clamp(v * 1.18f, 0.0f, 1.0f);

	float active_s  = s;
	float active_v  = std::clamp(v * 1.17f, 0.0f, 1.0f);

	float r_base, g_base, b_base;
	ImGui::ColorConvertHSVtoRGB(h, s, v, r_base, g_base, b_base);

	float r_hov, g_hov, b_hov;
	ImGui::ColorConvertHSVtoRGB(h, hovered_s, hovered_v, r_hov, g_hov, b_hov);

	float r_act, g_act, b_act;
	ImGui::ColorConvertHSVtoRGB(h, active_s, active_v, r_act, g_act, b_act);

	// base accent
	style.Colors[ImGuiCol_CheckMark]             = ImVec4(r_base, g_base, b_base, a);
	style.Colors[ImGuiCol_SliderGrab]            = ImVec4(r_base, g_base, b_base, a);
	style.Colors[ImGuiCol_HeaderActive]          = ImVec4(r_base, g_base, b_base, a);
	style.Colors[ImGuiCol_TabActive]             = ImVec4(r_base, g_base, b_base, a);
	style.Colors[ImGuiCol_TabUnfocusedActive]    = ImVec4(r_base, g_base, b_base, a);
	style.Colors[ImGuiCol_TextSelectedBg]        = ImVec4(r_base, g_base, b_base, a);

	// hovered accent
	style.Colors[ImGuiCol_ButtonHovered]         = ImVec4(r_hov, g_hov, b_hov, a);
	style.Colors[ImGuiCol_ButtonActive]          = ImVec4(r_hov, g_hov, b_hov, a);
	style.Colors[ImGuiCol_HeaderHovered]         = ImVec4(r_hov, g_hov, b_hov, a);
	style.Colors[ImGuiCol_TabHovered]            = ImVec4(r_hov, g_hov, b_hov, a);

	// active accent
	style.Colors[ImGuiCol_SliderGrabActive]      = ImVec4(r_act, g_act, b_act, a);
}

static bool CustomColorEdit(const char* label, Color& color)
{
	float col[3]
	{
		color.r() / 255.0f,
		color.g() / 255.0f,
		color.b() / 255.0f
	};

	if (ImGui::ColorEdit3(label, col))
	{
		color.SetColor
		(
			col[0] * 255.0f,
			col[1] * 255.0f,
			col[2] * 255.0f,
			255.0f
		);

		return true;
	}

	return false;
}

void DrawESPTab()
{
        if (ImGui::BeginTable("##ESPContents", 2, ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_SizingStretchProp))
        {
                ImGui::TableSetupColumn("LeftSide");
                ImGui::TableSetupColumn("RightSide");

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                
                ImGui::TextUnformatted("ESP");
                ImGui::Checkbox("Enabled##ESP", &config::esp::enabled.Get());

                ImGui::BeginDisabled(!config::esp::enabled.Get());
                {
                        // FIXED: Passing the underlying value references cleanly
                        DrawCheckboxWithSide("Name", &config::esp::name.Get(), config::esp::side_name.Get());
                        ImGui::Checkbox("Box", &config::esp::box.Get());
                        ImGui::Checkbox("Ignore Cloaked", &config::esp::ignore_cloaked.Get());
                        ImGui::Checkbox("Buildings", &config::esp::buildings.Get());

                        DrawCheckboxWithSide("Weapon", &config::esp::weapon.Get(), config::esp::side_weaponname.Get());
                        DrawCheckboxWithSide("Class", &config::esp::class_name.Get(), config::esp::side_classname.Get());

                        ImGui::Checkbox("Ammo Pack", &config::esp::ammopack.Get());
                        ImGui::Checkbox("Medkit", &config::esp::medkit.Get());

                        {
                                constexpr const char *items[]{"None", "Text", "Bar", "Both"};
                                // FIXED: No return value wrapping needed anymore
                                DrawComboWithSide("Health##ESP", &config::esp::health.Get(), items, 4, config::esp::side_healthbar.Get());
                        }

                        {
                                constexpr const char *items[]{"Only Enemies", "Only Teammates", "Both"};
                                ImGui::Combo("Team Selection##ESP", &config::esp::team_selected.Get(), items, 3);
                        }
                }
                ImGui::EndDisabled();

                {
                        constexpr const char *items[]{"TF2", "Arial"};
                        ImGui::Combo("Font", &config::esp::font_selected.Get(), items, 2);
                        ImGui::SliderInt("Font Size ##ESP", &config::esp::font_size.Get(), 8, 32);
                }

                ImGui::TableNextColumn();

                ImGui::TextUnformatted("Conditions");
                // FIXED: Direct values references passed straight through
                DrawCheckboxWithSide("Zoom", &config::esp::condition_zoomed.Get(), config::esp::side_zoom.Get());
                DrawCheckboxWithSide("Ubercharge", &config::esp::condition_ubered.Get(), config::esp::side_uber.Get());
                DrawCheckboxWithSide("Jarate", &config::esp::condition_jarated.Get(), config::esp::side_jarate.Get());
                DrawCheckboxWithSide("Bonk", &config::esp::condition_bonked.Get(), config::esp::side_bonk.Get());

                ImGui::Separator();

                ImGui::TextUnformatted("Glow");
                ImGui::Checkbox("Enabled##Glow", &config::glow::enabled.Get());
                ImGui::SliderInt("Stencil##Glow", &config::glow::stencil.Get(), 0, 10);
                ImGui::SliderInt("Blur##Glow", &config::glow::blur.Get(), 0, 10);
                ImGui::Checkbox("Highlight Weapon##Glow", &config::glow::weapon.Get());

                ImGui::Separator();

                ImGui::TextUnformatted("Chams");
                ImGui::Checkbox("Enabled##Chams", &config::chams::enabled.Get());

                // Row 2
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::Separator();

                ImGui::TextUnformatted("Colors");
                CustomColorEdit("RED Team", config::colors::red_team.Get());
                CustomColorEdit("BLU Team", config::colors::blu_team.Get());
                CustomColorEdit("Aimbot Target", config::colors::aimbot_target.Get());
                CustomColorEdit("Weapon", config::colors::weapon.Get());

                if (CustomColorEdit("Menu Accent", config::colors::menu_accent.Get()))
                        ChangeMenuAccentColor();

                CustomColorEdit("Ammo Pack", config::colors::ammopack.Get());
                CustomColorEdit("Medkit", config::colors::healthkit.Get());
                CustomColorEdit("Backtrack", config::colors::backtrack.Get());

                ImGui::TableNextColumn();
                ImGui::Separator();

                ImGui::TextUnformatted("Customization");
                config::esp::custom_box_rounding.Set(DrawSliderWithFlag("Box Roundness", config::esp::custom_box_rounding.Get(), 0, 15));
                config::esp::custom_healthbar_rounding.Set(DrawSliderWithFlag("Health Bar Roundness", config::esp::custom_healthbar_rounding.Get(), 0, 15));
                config::esp::custom_healthbar_margin.Set(DrawSliderWithFlag("Health Bar Margin", config::esp::custom_healthbar_margin.Get(), 0, 15));
                config::esp::custom_healthbar_thickness.Set(DrawSliderWithFlag("Health Bar Thickness", config::esp::custom_healthbar_thickness.Get(), 1, 15));
                config::esp::custom_gap.Set(DrawSliderWithFlag("Health Bar Padding", config::esp::custom_gap.Get(), 0, 15));
                config::esp::custom_text_padding.Set(DrawSliderWithFlag("Text Padding", config::esp::custom_text_padding.Get(), 0, 15));

                ImGui::EndTable();
        }
}