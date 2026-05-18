#include "../../features/binds/binds.h"
#include "../../settings/settings.h"

#include "../utils/gui_utils.h"

static void DrawLeftColumn()
{
	ImGui::TextUnformatted("Targeting");
	{
		constexpr const char *items[]{"Plain", "Smooth", "Assistance", "Silent"};
		int temp = Config.aimbot.packed.aimmode;
		ImGui::Combo("Method", &temp, items, 4);
		Config.aimbot.packed.aimmode = temp;
	}
	{
		constexpr const char *items[]{"None", "Legit", "Rage"};
		int temp = Config.aimbot.packed.meleemode;
		ImGui::Combo("Melee Aimbot", &temp, items, 3);
		Config.aimbot.packed.meleemode = temp;
	}
	{
		constexpr const char *items[]{"Only Enemies", "Only Teammates", "Both"};
		int temp = Config.aimbot.packed.teamselection;
		ImGui::Combo("Team Selection", &temp, items, 3);
		Config.aimbot.packed.teamselection = temp;
	}

	ImGui::Separator();

	ImGui::TextUnformatted("Toggles");
	ImGui_CheckboxBit("Autoshoot", Config.aimbot.packed.autoshoot);
	ImGui_CheckboxBit("ViewModel Aim", Config.aimbot.packed.viewmodelaim);
	ImGui_CheckboxBit("Wait For Charge", Config.aimbot.packed.waitforcharge);
	ImGui_CheckboxBit("Hold Minigun Spin", Config.aimbot.packed.hold_minigun_spin);
	ImGui_CheckboxBit("Swing Prediction", Config.aimbot.packed.swing_pred);
	ImGui_CheckboxBit("Swing Prediction LocalPlayer", Config.aimbot.packed.swing_pred_local);

	if (ImGui::IsItemHovered())
		ImGui::SetTooltip("%s", "Predicts yourself on swing pred");

	ImGui::Separator();

	ImGui::TextUnformatted("Ignore Options");
	ImGui_CheckboxBit("Cloaked", Config.aimbot.packed.ignorecloaked);
	ImGui_CheckboxBit("Ubercharged", Config.aimbot.packed.ignoreubered);
	ImGui_CheckboxBit("Hoovy", Config.aimbot.packed.ignorehoovy);
	ImGui_CheckboxBit("Bonked", Config.aimbot.packed.ignorebonked);
}

static void DrawRightColumn()
{
	ImGui::TextUnformatted("Adjustments");
	ImGui::SliderFloat("Fov", &Config.aimbot.fov, 0.0f, 180.0f);

	ImGui::SliderFloat("Smoothness", &Config.aimbot.smoothness, 0.0f, 100.0f);
	if (ImGui::IsItemHovered())
		ImGui::SetTooltip("Higher is smoother");

	ImGui::SliderFloat("Max Sim Time", &Config.aimbot.max_sim_time, 0.0f, 5.0f);

	ImGui::Separator();

	ImGui::TextUnformatted("Visuals");
	ImGui_CheckboxBit("Draw FOV Indicator", Config.aimbot.packed.draw_fov_indicator);
	ImGui_CheckboxBit("Draw Target Path", Config.aimbot.packed.proj_path);
	{
		constexpr const char *items[]{"None", "Circle", "Square", "Triangle"};
		int temp = Config.aimbot.packed.proj_indicator;
		ImGui::Combo("Indicator Style", &temp, items, 4);
		Config.aimbot.packed.proj_indicator = temp;
	}

	ImGui::Separator();

	ImGui::TextUnformatted("TriggerBot");

	features::binds.RenderHotkey("Key", Config.trigger.key);
	ImGui::PushStyleVar(ImGuiStyleVar_Alpha, Config.trigger.key->IsEnabled() ? 1.0f : 0.5f);
	{
		ImGui_CheckboxBit("Hitscan##Trigger", Config.trigger.packed.hitscan);

		{
			constexpr const char *items[]{"None", "Legit", "Rage"};
			int temp = 0;

			temp = Config.trigger.packed.autobackstab;
			ImGui::Combo("Auto Backstab##Trigger", &temp, items, 3);
			Config.trigger.packed.autobackstab = temp;

			temp = Config.trigger.packed.autoairblast;
			ImGui::Combo("Auto Airblast##Trigger", &temp, items, 3);
			Config.trigger.packed.autoairblast = temp;
		}
	}
	ImGui::PopStyleVar();
}

void DrawAimbotTab()
{
	features::binds.RenderHotkey("Aimbot", Config.aimbot.key);

	ImGui::PushStyleVar(ImGuiStyleVar_Alpha, Config.aimbot.key->IsEnabled() ? 1.0f : 0.5f);
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