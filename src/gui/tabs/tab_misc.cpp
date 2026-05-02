#include "tab_misc.h"

#include "../../imgui/imgui.h"
#include "../../settings/settings.h"

#include "../utils/gui_utils.h"

void DrawMiscTab()
{
	if (ImGui::BeginTable("##MiscContents", 2, ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_SizingStretchProp))
	{
		ImGui::TableSetupColumn("LeftSide");
		ImGui::TableSetupColumn("RightSide");

		ImGui::TableNextRow();

		// lef column
		ImGui::TableNextColumn();

		ImGui::TextUnformatted("General");
		ImGui_CheckboxBit("Spectator List", Config.misc.packed.spectatorlist);
		ImGui_CheckboxBit("Player List", Config.misc.packed.playerlist);
		ImGui_CheckboxBit("sv_pure bypass", Config.misc.packed.sv_pure_bypass);
		ImGui_CheckboxBit("Streamer Mode", Config.misc.packed.streamer_mode);
		ImGui_CheckboxBit("Bhop", Config.misc.packed.bhop);
		ImGui_CheckboxBit("Autostrafe", Config.misc.packed.autostrafe);
		ImGui_CheckboxBit("Backpack Expander", Config.misc.packed.backpack_expander);
		ImGui_CheckboxBit("Accept Item Drops", Config.misc.packed.accept_item_drop);
		ImGui_CheckboxBit("No Recoil", Config.misc.packed.norecoil);
		ImGui_CheckboxBit("No Engine Sleep", Config.misc.packed.no_engine_sleep);
		ImGui_CheckboxBit("No Scope Overlay", Config.misc.packed.no_scope_overlay);
		ImGui_CheckboxBit("No Zoom", Config.misc.packed.no_zoom);
		ImGui_CheckboxBit("Anti AFK", Config.misc.packed.antiafk);
		ImGui_CheckboxBit("Info Panel", Config.misc.packed.infopanel);
		ImGui_CheckboxBit("Spy Alert", Config.misc.packed.spyalert);
		// ImGui_CheckboxBit("No Survey", Config.misc.packed.no_survey);

		ImGui::Separator();

		ImGui::TextUnformatted("Backtrack");
		ImGui_CheckboxBit("Enabled##Backtrack", Config.backtrack.packed.enabled);

		{
			constexpr const char *items[]{"None", "Last Record Only", "All Records"};
			int temp = Config.backtrack.packed.draw_mode;
			ImGui::Combo("Mode##Backtrack", &temp, items, 3);
			Config.backtrack.packed.draw_mode = temp;
		}

		// right column
		ImGui::TableNextColumn();

		ImGui::TextUnformatted("Third Person");
		gBinds.RenderHotkey("Key", Config.misc.thirdperson_key);
		ImGui::SliderFloat4("Offset", Config.misc.thirdperson_offset, -100.0f, 100.0f);

		ImGui::Separator();

		ImGui::TextUnformatted("Field of View");
		ImGui_CheckboxBit("Custom Fov Enabled", Config.misc.packed.customfov_enabled);

		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, Config.misc.packed.customfov_enabled ? 1.0f : 0.5f);
		{
			ImGui::SliderFloat("Custom Fov", &Config.misc.customfov, 1.0f, 120.0f);
			ImGui::SliderFloat("Zoomed Fov", &Config.misc.zoomedfov, 1.0f, 120.0f);
		}
		ImGui::PopStyleVar();

		ImGui::Separator();

		ImGui::TextUnformatted("Viewmodel");
		ImGui_CheckboxBit("No Viewmodel Bob", Config.misc.packed.no_viewmodel_bob);
		ImGui::SliderFloat3("Viewmodel Offset", Config.misc.viewmodel_offset, -20.0f, 20.0f);
		ImGui::SliderFloat("Viewmodel Interp", &Config.misc.viewmodel_interp, 0.0f, 50.0f);

		ImGui::Separator();

		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, Config.warp.key->IsEnabled() ? 1.0f : 0.5f);
		{
			ImGui::TextUnformatted("Warp");
			gBinds.RenderHotkey("Key", Config.warp.key);
			gBinds.RenderHotkey("Recharge Key", Config.warp.recharge_key);
			ImGui_SliderIntBit("Speed##Warp", Config.warp.packed.speed, 1, 24);
		}
		ImGui::PopStyleVar();

		ImGui::Separator();

		ImGui::TextUnformatted("Screen");
		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, Config.misc.aspectratio > 0 ? 1.0f : 0.5f);
		{
			ImGui::SliderFloat("Aspect Ratio", &Config.misc.aspectratio, 0.0f, 5.0f);
		}
		ImGui::PopStyleVar();

		ImGui::EndTable();
	}
}