#include "tab_misc.h"

#include "../../imgui/imgui.h"
#include "../../settings/settings.h"

void DrawMiscTab()
{
	gBinds.RenderHotkey("Third Person", Settings::Misc.thirdperson_key);
	ImGui::SliderFloat4("Third Person Offset", Settings::Misc.thirdperson_offset, -100.0f, 100.0f);

	ImGui::Separator();

	ImGui::Checkbox("Spectator List", &Settings::Misc.spectatorlist);
	ImGui::Checkbox("Player List", &Settings::Misc.playerlist);
	ImGui::Checkbox("sv_pure bypass", &Settings::Misc.sv_pure_bypass);
	ImGui::Checkbox("Streamer Mode", &Settings::Misc.streamer_mode);
	ImGui::Checkbox("Bhop", &Settings::Misc.bhop);
	ImGui::Checkbox("Autostrafe", &Settings::Misc.autostrafe);
	ImGui::Checkbox("Backpack Expander", &Settings::Misc.backpack_expander);
	ImGui::Checkbox("Accept Item Drops", &Settings::Misc.accept_item_drop);

	ImGui::Checkbox("No Recoil", &Settings::Misc.norecoil);
	ImGui::Checkbox("No Engine Sleep", &Settings::Misc.no_engine_sleep);

	ImGui::Checkbox("No Scope Overlay", &Settings::Misc.no_scope_overlay);
	ImGui::Checkbox("No Zoom", &Settings::Misc.no_zoom);

	ImGui::Checkbox("Anti AFK", &Settings::Misc.antiafk);
	ImGui::Checkbox("Info Panel", &Settings::Misc.infopanel);

	// ImGui::Checkbox("No Survey", &Settings::Misc.no_survey);

	ImGui::Separator();

	ImGui::Checkbox("Custom Fov Enabled", &Settings::Misc.customfov_enabled);

	ImGui::BeginDisabled(!Settings::Misc.customfov_enabled);
	{
		ImGui::SliderFloat("Custom Fov", &Settings::Misc.customfov, 1.0f, 120.0f);
		ImGui::SliderFloat("Zoomed Fov", &Settings::Misc.zoomedfov, 1.0f, 120.0f);
	}
	ImGui::EndDisabled();

	ImGui::Separator();

	ImGui::Checkbox("No Viewmodel Bob", &Settings::Misc.no_viewmodel_bob);
	ImGui::SliderFloat3("Viewmodel Offset", Settings::Misc.viewmodel_offset, -20, 20.0f);
	ImGui::SliderFloat("Viewmodel Interp", &Settings::Misc.viewmodel_interp, 0.0f, 50.0f);

	ImGui::Separator();

	#if 0
	{
		constexpr const char *items[]{"None", "Last Record Only", "All Records"};
		ImGui::Combo("Backtrack", &Settings::Misc.backtrack, items, 3);
	}
	#endif

	ImGui::Separator();

	ImGui::PushStyleVar(ImGuiStyleVar_Alpha, Settings::AntiAim.warp_key->IsEnabled() ? 1.0f : 0.5f);
	{
		gBinds.RenderHotkey("Warp Key", Settings::AntiAim.warp_key);
		gBinds.RenderHotkey("Warp Recharge Key", Settings::AntiAim.warp_recharge_key);
		ImGui::SliderInt("Warp Speed", &Settings::AntiAim.warp_speed, 1, 24);
	}
	ImGui::PopStyleVar();
}