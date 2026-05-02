#include "tab_trigger.h"

#include "../../imgui/imgui.h"
#include "../../settings/settings.h"

#include "../utils/gui_utils.h"

void DrawTriggerTab()
{
	gBinds.RenderHotkey("TriggerBot", Config.trigger.key);
	ImGui::PushStyleVar(ImGuiStyleVar_Alpha, Config.trigger.key->IsEnabled() ? 1.0f : 0.5f);
	{
		ImGui_CheckboxBit("Hitscan", Config.trigger.packed.hitscan);

		{
			constexpr const char *items[]{"None", "Legit", "Rage"};
			int temp = 0;

			temp = Config.trigger.packed.autobackstab;
			ImGui::Combo("Auto Backstab", &temp, items, 3);
			Config.trigger.packed.autobackstab = temp;

			temp = Config.trigger.packed.autoairblast;
			ImGui::Combo("Auto Airblast", &temp, items, 3);
			Config.trigger.packed.autoairblast = temp;
		}
	}
	ImGui::PopStyleVar();
}