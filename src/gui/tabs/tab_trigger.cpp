#include "tab_trigger.h"

#include "../../imgui/imgui.h"
#include "../../settings/settings.h"

void DrawTriggerTab()
{
	gBinds.RenderHotkey("TriggerBot", Settings::Trigger.key);
	ImGui::PushStyleVar(ImGuiStyleVar_Alpha, Settings::Trigger.key->IsEnabled() ? 1.0f : 0.5f);
	{
		ImGui::Checkbox("Hitscan", &Settings::Trigger.hitscan);

		{
			constexpr const char *items[]{"None", "Legit", "Rage"};
			ImGui::Combo("Auto Backstab", &Settings::Trigger.autobackstab, items, 3);
			ImGui::Combo("Auto Airblast", &Settings::Trigger.autoairblast, items, 3);
		}
	}
	ImGui::PopStyleVar();
}