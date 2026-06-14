#include "../../../thirdparty/imgui/imgui.h"

#include "../../misc/misc.h"
#include "../../nopush/nopush.h"
#include "../../antiafk/antiafk.h"
#include "../../spyalert/spyalert.h"
#include "../../backtrack/backtrack.h"
#include "../../spectators/spectators.h"
#include "../../warp/warp.h"
#include "../../autostrafe/autostrafe.h"
#include "../../bhop/bhop.h"
#include "../../visuals/norecoil/norecoil.h"
#include "../../visuals/thirdperson/thirdperson.h"
#include "../../visuals/customfov/customfov.h"
#include "../../visuals/viewmodel_interp/viewmodel_interp.h"
#include "../../visuals/viewmodel_offset/viewmodel_offset.h"
#include "../../playerlist/playerlist.h"

static void setup_norecoil_popup()
{
	if (ImGui::BeginPopup("NoRecoilPopup"))
	{
		{
			ImGui::SliderInt("Scaled", &config::norecoil::scale.Get(), 0, 100);

			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("How much recoil should we see\n0%% = no recoil, 100%% = full recoil");
		}

		ImGui::Checkbox("Ignore Spectators", &config::norecoil::ignore_spectators.Get());

		if (ImGui::IsItemHovered())
			ImGui::SetTooltip("If no recoil should appear to spectators");

		ImGui::EndPopup();
	}
}

void DrawMiscTab()
{
	if (ImGui::BeginTable("##MiscContents", 2, ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_SizingStretchProp))
	{
		ImGui::TableSetupColumn("LeftSide");
		ImGui::TableSetupColumn("RightSide");

		ImGui::TableNextRow();

		// lef column
		ImGui::TableNextColumn();

		//ImGui::TextUnformatted("Safe Mode");
		//ImG::i_Checkbox("Enabled##Safe Mode", );

		ImGui::Separator();

		ImGui::TextUnformatted("General");
		ImGui::Checkbox("Spectator List", &config::spectators::enabled.Get());
		ImGui::Checkbox("Player List", &config::playerlist::enabled.Get());
		ImGui::Checkbox("sv_pure bypass", &config::sv_pure_bypass::enabled.Get());
		ImGui::Checkbox("Streamer Mode", &config::streamer_mode::enabled.Get());
		ImGui::Checkbox("Bhop", &config::bhop::enabled.Get());
		ImGui::Checkbox("Autostrafe", &config::autostrafe::enabled.Get());
		ImGui::Checkbox("Backpack Expander", &config::backpack_expander::enabled.Get());
		ImGui::Checkbox("Accept Item Drops", &config::accept_item_drop::enabled.Get());

		{
			setup_norecoil_popup();
			ImGui::Checkbox("No Recoil", &config::norecoil::enabled.Get());

			ImGui::SameLine();

			if (ImGui::Button("+"))
				ImGui::OpenPopup("NoRecoilPopup");
		}

		ImGui::Checkbox("No Push", &config::nopush::enabled.Get());
		ImGui::Checkbox("No Engine Sleep", &config::no_engine_sleep::enabled.Get());
		ImGui::Checkbox("No Scope Overlay", &config::no_scope_overlay::enabled.Get());
		ImGui::Checkbox("No Zoom", &config::no_zoom::enabled.Get());
		ImGui::Checkbox("Anti AFK", &config::antiafk::enabled.Get());
		ImGui::Checkbox("Spy Alert", &config::spyalert::enabled.Get());
		// Im::ui_Checkbox("No Survey", );

		ImGui::Separator();

		ImGui::TextUnformatted("Backtrack");
		ImGui::Checkbox("Enabled##Backtrack", &config::backtrack::enabled.Get());

		{
			constexpr const char *items[]{"None", "Last Record Only", "All Records"};
			ImGui::Combo("Mode##Backtrack", &config::backtrack::draw_mode.Get(), items, 3);
		}

		// right column
		ImGui::TableNextColumn();

		ImGui::TextUnformatted("Third Person");
		features::binds.RenderHotkey("Key", &config::thirdperson::key.Get());
		ImGui::SliderFloat4("Offset", config::thirdperson::offset.Get().data(), -100.0f, 100.0f);

		ImGui::Separator();

		ImGui::TextUnformatted("Field of View");
		ImGui::Checkbox("Custom Fov Enabled", &config::customfov::enabled.Get());

		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, config::customfov::enabled.Get() ? 1.0f : 0.5f);
		{
			ImGui::SliderFloat("Custom Fov", &config::customfov::unzoomed.Get(), 1.0f, 120.0f);
			ImGui::SliderFloat("Zoomed Fov", &config::customfov::zoomed.Get(), 1.0f, 120.0f);
		}
		ImGui::PopStyleVar();

		ImGui::Separator();

		ImGui::TextUnformatted("Viewmodel");
		ImGui::Checkbox("No Viewmodel Bob", &config::viewmodel_bob::enabled.Get());
		ImGui::SliderFloat3("Viewmodel Offset", config::viewmodel_offset::offset.Get().data(), -20.0f, 20.0f);
		ImGui::SliderFloat("Viewmodel Interp", &config::viewmodel_interp::enabled.Get(), 0.0f, 50.0f);

		ImGui::Separator();

		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, config::warp::key.Get().IsEnabled() ? 1.0f : 0.5f);
		{
			ImGui::TextUnformatted("Warp");
			features::binds.RenderHotkey("Key", &config::warp::key.Get());
			features::binds.RenderHotkey("Recharge Key", &config::warp::recharge_key.Get());
			ImGui::SliderInt("Speed##Warp", &config::warp::speed.Get(), 1, 24);
		}
		ImGui::PopStyleVar();

		ImGui::Separator();

		ImGui::TextUnformatted("Screen");
		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, config::aspect_ratio::value.Get() > 0 ? 1.0f : 0.5f);
		{
			ImGui::SliderFloat("Aspect Ratio", &config::aspect_ratio::value.Get(), 0.0f, 5.0f);
		}
		ImGui::PopStyleVar();

		ImGui::EndTable();
	}
}