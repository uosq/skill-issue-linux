#include "../../../thirdparty/imgui/imgui.h"

#include "../../triggerbot/triggerbot.h"

#include "../../triggerbot/autoairblast/autoairblast.h"
#include "../../triggerbot/autobackstab/autobackstab.h"
#include "../../triggerbot/autodetonate/autodetonate.h"

void DrawTriggerTab()
{
	features::binds.RenderHotkey("TriggerBot", &config::trigger::key.Get());
	ImGui::PushStyleVar(ImGuiStyleVar_Alpha, config::trigger::key.Get().IsEnabled() ? 1.0f : 0.5f);
	{
		constexpr const char* items[]{"None", "Legit", "Rage"};

		ImGui::Checkbox("Hitscan", &config::trigger::hitscan.Get());

		ImGui::Combo("Auto Backstab", &config::autobackstab::enabled.Get(), items, 3);
		ImGui::Combo("Auto Airblast", &config::autoairblast::enabled.Get(), items, 3);
		ImGui::Checkbox("Auto Detonate", &config::autodetonate::enabled.Get());
	}
	ImGui::PopStyleVar();
}