#include "tab_antiaim.h"

#include "../../imgui/imgui.h"
#include "../../settings/settings.h"

void DrawAntiAimTab()
{
	ImGui::Checkbox("Enabled", &Settings::AntiAim.enabled);

	ImGui::BeginDisabled(!Settings::AntiAim.enabled);
	{
		{
			constexpr const char *items[]{"None", "Up", "Down", "Fake Up", "Fake Down", "Random"};
			ImGui::Combo("Pitch Mode", &Settings::AntiAim.pitch_mode, items, 6);
		}

		{
			constexpr const char *items[]{"None",	 "Left",      "Right",	    "Back",
						      "Forward", "Spin Left", "Spin Right", "Jitter"};

			ImGui::Combo("Real Yaw", &Settings::AntiAim.real_yaw_mode, items, 8);
			ImGui::Combo("Fake Yaw", &Settings::AntiAim.fake_yaw_mode, items, 8);
		}

		ImGui::SliderInt("Spin Speed", &Settings::AntiAim.spin_speed, 0, 10);
	}
	ImGui::EndDisabled();

	ImGui::Separator();

	ImGui::Checkbox("Fake Lag Enabled", &Settings::AntiAim.fakelag_enabled);

	ImGui::BeginDisabled(!Settings::AntiAim.fakelag_enabled);
	{
		ImGui::SliderInt("Ticks", &Settings::AntiAim.fakelag_ticks, 1, 21);
	}
	ImGui::EndDisabled();
}