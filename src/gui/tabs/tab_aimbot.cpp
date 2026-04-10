#include "tab_aimbot.h"

#include "../../features/binds/binds.h"
#include "../../settings/settings.h"

void DrawAimbotTab()
{
	gBinds.RenderHotkey("Aimbot", Settings::Aimbot.key);
	ImGui::PushStyleVar(ImGuiStyleVar_Alpha, Settings::Aimbot.key->IsEnabled() ? 1.0f : 0.5f);
	{
		// ImGui::InputText("Key", Settings::Aimbot.key,
		// sizeof(Settings::Aimbot.key));

		ImGui::Checkbox("Autoshoot", &Settings::Aimbot.autoshoot);
		ImGui::Checkbox("Viewmodel Aim", &Settings::Aimbot.viewmodelaim);
		ImGui::Checkbox("Draw FOV Indicator", &Settings::Aimbot.draw_fov_indicator);
		ImGui::Checkbox("Wait For Charge", &Settings::Aimbot.waitforcharge);
		ImGui::Checkbox("Draw Path", &Settings::Aimbot.path);
		ImGui::SliderFloat("Fov", &Settings::Aimbot.fov, 0.0f, 180.0f);
		ImGui::SliderFloat("Max Sim Time", &Settings::Aimbot.max_sim_time, 0.0f, 5.0f);
		ImGui::SliderFloat("Smoothness (lower is smoother)", &Settings::Aimbot.smoothness, 0.0f, 100.0f);

		{
			constexpr const char *items[]{
			    "None",
			    "Circle",
			    "Square",
			    "Triangle",
			};
			ImGui::Combo("Indicator Style", &Settings::Aimbot.indicator, items, 4);
		}

		{
			constexpr const char *items[]{"None", "Legit", "Rage"};
			ImGui::Combo("Melee Aimbot", &Settings::Aimbot.melee, items, 3);
		}

		{
			constexpr const char *items[]{
			    "Plain",
			    "Smooth",
			    "Assistance",
			    "Silent",
			};
			ImGui::Combo("Method", &Settings::Aimbot.mode, items, 4);
		}

		{
			constexpr const char *items[]{
			    "Only Enemies",
			    "Only Teammates",
			    "Both",
			};

			ImGui::Combo("Team Selection", &Settings::Aimbot.teamMode, items, 3);
		}

		ImGui::Separator();

		ImGui::TextUnformatted("Ignore Options");
		ImGui::Checkbox("Cloaked", &Settings::Aimbot.ignorecloaked);
		ImGui::SameLine();
		ImGui::Checkbox("Ubercharged", &Settings::Aimbot.ignoreubered);
		ImGui::SameLine();
		ImGui::Checkbox("Hoovy", &Settings::Aimbot.ignorehoovy);
		ImGui::SameLine();
		ImGui::Checkbox("Bonked", &Settings::Aimbot.ignorebonked);

		ImGui::Separator();

		ImGui::Checkbox("Hold Minigun Spin", &Settings::Aimbot.hold_minigun_spin);
	}
	ImGui::PopStyleVar();
}