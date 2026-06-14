#include "../../binds/binds.h"
#include "../../config/config.h"

#include "../../aimbot/aimbot.h"
#include "../../triggerbot/triggerbot.h"

static bool AIM_METHOD(const char* name, int& item)
{
	constexpr const char *items[]{"Plain", "Smooth", "Assistance", "Silent"};
	return ImGui::Combo(name, &item, items, 4);
}

static void setup_swingpred_popup()
{
	if (!ImGui::BeginPopup("SwingPredPopup"))
		return;

	{
		ImGui::Checkbox("Predict LocalPlayer", &config::aimbot::swing_melee_local.Get());
	
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip("If we should predict the localplayer\nwhen doing swing prediction");
	}

	{
		int range = static_cast<int>(config::aimbot::swing_melee_range.Get());

		if (ImGui::SliderInt("Range", &range, 0, 100, "%d%%"))
			config::aimbot::swing_melee_range.Get() = static_cast<uint8_t>(range);

		if (ImGui::IsItemHovered())
			ImGui::SetTooltip("Melee's swing range\n0%% = no range, 100%% = full swing range");
	}

	ImGui::EndPopup();
}

static void DrawLeftColumn()
{
	//make_hitscan_popup();

	ImGui::TextUnformatted("Targeting");
	{
		AIM_METHOD("Hitscan Aim Method", config::aimbot::hitscan_method.Get());
		AIM_METHOD("Projectile Aim Method", config::aimbot::projectile_method.Get());
		AIM_METHOD("Melee Aim Method", config::aimbot::melee_method.Get());
	}
	{
		constexpr const char *items[]{"None", "Legit", "Rage"};
		ImGui::Combo("Melee Aimbot Mode", &config::aimbot::melee_mode.Get(), items, 3);
	}
	{
		constexpr const char *items[]{"Only Enemies", "Only Teammates", "Both"};
		int temp = config::aimbot::team_selected.Get();
		ImGui::Combo("Team Selection", &temp, items, 3);
		config::aimbot::team_selected.Get() = temp;
	}

	ImGui::Separator();

	ImGui::TextUnformatted("Toggles");
	ImGui::Checkbox("Autoshoot", &config::aimbot::autoshoot.Get());
	ImGui::Checkbox("ViewModel Aim", &config::aimbot::viewmodel_aim.Get());
	ImGui::Checkbox("Wait For Charge", &config::aimbot::wait_for_charge.Get());
	ImGui::Checkbox("Hold Minigun Spin", &config::aimbot::hold_minigun_spin.Get());

	{
		setup_swingpred_popup();

		ImGui::Checkbox("Swing Prediction", &config::aimbot::swing_pred.Get());

		ImGui::SameLine();

		if (ImGui::Button("+"))
			ImGui::OpenPopup("SwingPredPopup");
	}

	ImGui::Separator();

	ImGui::TextUnformatted("Ignore Options");
	ImGui::Checkbox("Cloaked", &config::aimbot::ignore_cloaked.Get());
	ImGui::Checkbox("Ubercharged", &config::aimbot::ignore_uber.Get());
	ImGui::Checkbox("Hoovy", &config::aimbot::ignore_hoovy.Get());
	ImGui::Checkbox("Bonked", &config::aimbot::ignore_bonked.Get());
}

static void DrawRightColumn()
{
	ImGui::TextUnformatted("Adjustments");
	ImGui::SliderFloat("Fov", &config::aimbot::fov.Get(), 0.0f, 180.0f);

	ImGui::SliderFloat("Smoothness", &config::aimbot::smoothness.Get(), 0.0f, 100.0f);
	if (ImGui::IsItemHovered())
		ImGui::SetTooltip("Higher is smoother");

	ImGui::SliderFloat("Max Sim Time", &config::aimbot::max_sim_time.Get(), 0.0f, 5.0f);
	if (ImGui::IsItemHovered())
		ImGui::SetTooltip("Maximum allowed time to predict");

	ImGui::Separator();

	ImGui::TextUnformatted("Visuals");
	ImGui::Checkbox("Draw FOV Indicator", &config::aimbot::fov_indicator.Get());
	ImGui::Checkbox("Draw Target Path", &config::aimbot::draw_predicted_player_path.Get());
	{
		constexpr const char *items[]{"None", "Circle", "Square", "Triangle"};
		ImGui::Combo("Indicator Style", &config::aimbot::draw_predicted_player_indicator.Get(), items, 4);
	}

	ImGui::Separator();

	ImGui::TextUnformatted("TriggerBot");

	features::binds.RenderHotkey("Key", &config::trigger::key.Get());
	ImGui::PushStyleVar(ImGuiStyleVar_Alpha, config::trigger::key.Get().IsEnabled() ? 1.0f : 0.5f);
	{
		ImGui::Checkbox("Hitscan##Trigger", &config::trigger::hitscan.Get());

		constexpr const char* items[]{"None", "Legit", "Rage"};
		ImGui::Combo("Auto Backstab##Trigger", &config::autobackstab::enabled.Get(), items, 3);
		ImGui::Combo("Auto Airblast##Trigger", &config::autoairblast::enabled.Get(), items, 3);
	}
	ImGui::PopStyleVar();
}

void DrawAimbotTab()
{
	features::binds.RenderHotkey("Aimbot", &config::aimbot::key.Get());

	ImGui::PushStyleVar(ImGuiStyleVar_Alpha, config::aimbot::key.Get().IsEnabled() ? 1.0f : 0.5f);
	{
		if (ImGui::BeginTable("##AimbotContents", 2, ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_SizingStretchProp))
		{
			ImGui::TableSetupColumn("LeftSide");
			ImGui::TableSetupColumn("RightSide");

			ImGui::TableNextRow();

			// left column
			ImGui::TableNextColumn();

			DrawLeftColumn();

			// right column
			ImGui::TableNextColumn();

			DrawRightColumn();

			ImGui::EndTable();
		}
	}
	ImGui::PopStyleVar();
}