#pragma once

#include "../imgui/imgui.h"
#include "../imgui/imgui_stdlib.h"
#include "../settings.h"
#include "../sdk/helpers/helper.h"
#include "../features/antiaim/antiaim.h"
#include "../imgui/TextEditor.h"
#include "../features/lua/api.h"
#include "console.h"
#include "../features/entitylist/entitylist.h"
#include "../sdk/definitions/eteam.h"
#include "../features/aimbot/melee/aimbot_melee.h"
#include "../features/aimbot/utils/utils.h"
#include "../features/triggerbot/triggerbot.h"

static TextEditor editor;

enum TabMenu
{
	TAB_AIMBOT = 0,
	TAB_ESP,
	TAB_MISC,
	TAB_TRIGGER,
	TAB_ANTIAIM,
	TAB_LUA,
	TAB_NETVARS,
	TAB_CONFIG,
};

static void DrawTabButtons(int &tab)
{
	ImGui::BeginGroup();

	if (ImGui::Button("AIMBOT", ImVec2(-1, 0)))
		tab = TAB_AIMBOT;

	if (ImGui::Button("TRIGGER", ImVec2(-1, 0)))
		tab = TAB_TRIGGER;

	if (ImGui::Button("ESP", ImVec2(-1, 0)))
		tab = TAB_ESP;

	if (ImGui::Button("MISC", ImVec2(-1, 0)))
		tab = TAB_MISC;

	if (ImGui::Button("ANTIAIM", ImVec2(-1, 0)))
		tab = TAB_ANTIAIM;

	if (ImGui::Button("PLUTO", ImVec2(-1, 0)))
		tab = TAB_LUA;

	if (ImGui::Button("NETVARS", ImVec2(-1, 0)))
		tab = TAB_NETVARS;

	ImGui::EndGroup();
}

static void DrawAimbotTab()
{
	ImGui::BeginGroup();

	ImGui::Checkbox("Enabled", &g_Settings.aimbot.enabled);
	ImGui::InputText("Key", &g_Settings.aimbot.key);
	ImGui::Checkbox("Autoshoot", &g_Settings.aimbot.autoshoot);
	ImGui::Checkbox("Viewmodel Aim", &g_Settings.aimbot.viewmodelaim);
	ImGui::Checkbox("Draw FOV Indicator", &g_Settings.aimbot.draw_fov_indicator);
	ImGui::Checkbox("Wait For Charge", &g_Settings.aimbot.waitforcharge);
	ImGui::SliderFloat("Fov", &g_Settings.aimbot.fov, 0.0f, 180.0f);
	ImGui::SliderFloat("Max Sim Time", &g_Settings.aimbot.max_sim_time, 0.0f, 5.0f);
	ImGui::SliderFloat("Smoothness", &g_Settings.aimbot.smoothness, 10.0f, 100.0f);

	if (ImGui::BeginCombo("Melee Aimbot", GetMeleeModeName().c_str()))
	{
		if (ImGui::Selectable("None"))
			g_Settings.aimbot.melee = MeleeMode::NONE;

		if (ImGui::Selectable("Legit"))
			g_Settings.aimbot.melee = MeleeMode::LEGIT;

		if (ImGui::Selectable("Rage"))
			g_Settings.aimbot.melee = MeleeMode::RAGE;

		ImGui::EndCombo();
	}

	if (ImGui::BeginCombo("Aimbot Method", AimbotUtils::GetAimbotModeName().c_str()))
	{
		if (ImGui::Selectable("Plain"))
			g_Settings.aimbot.mode = AimbotMode::PLAIN;

		if (ImGui::Selectable("Smooth"))
			g_Settings.aimbot.mode = AimbotMode::SMOOTH;

		if (ImGui::Selectable("Assistance"))
			g_Settings.aimbot.mode = AimbotMode::ASSISTANCE;

		if (ImGui::Selectable("Silent"))
			g_Settings.aimbot.mode = AimbotMode::SILENT;

		if (ImGui::Selectable("pSilent"))
			g_Settings.aimbot.mode = AimbotMode::PSILENT;

		ImGui::EndCombo();
	}

	if (ImGui::BeginCombo("Team Selection", AimbotUtils::GetTeamModeName().c_str()))
	{
		if (ImGui::Selectable("Only Enemy"))
			g_Settings.aimbot.teamMode = TeamMode::ONLYENEMY;

		if (ImGui::Selectable("Only Teammate"))
			g_Settings.aimbot.teamMode = TeamMode::ONLYTEAMMATE;

		if (ImGui::Selectable("Both"))
			g_Settings.aimbot.teamMode = TeamMode::BOTH;

		ImGui::EndCombo();
	}

	ImGui::Separator();

	ImGui::TextUnformatted("Ignore Options");
	ImGui::Checkbox("Cloaked", &g_Settings.aimbot.ignorecloaked);
	ImGui::SameLine();
	ImGui::Checkbox("Ubercharged", &g_Settings.aimbot.ignoreubered);
	ImGui::SameLine();
	ImGui::Checkbox("Hoovy", &g_Settings.aimbot.ignorehoovy);
	ImGui::SameLine();
	ImGui::Checkbox("Bonked", &g_Settings.aimbot.ignorebonked);

	ImGui::EndGroup();
}

static void DrawESPTab()
{
	ImGui::BeginGroup();

	ImGui::Checkbox("ESP Enabled", &g_Settings.esp.enabled);
	ImGui::Checkbox("Name", &g_Settings.esp.name);
	ImGui::Checkbox("Box", &g_Settings.esp.box);
	ImGui::Checkbox("Ignore Cloakeds", &g_Settings.esp.ignorecloaked);
	ImGui::Checkbox("Buildings", &g_Settings.esp.buildings);
	ImGui::Checkbox("Weapon", &g_Settings.esp.weapon);

	ImGui::Separator();

	ImGui::Checkbox("Chams", &g_Settings.esp.chams);

	ImGui::Separator();

	ImGui::TextUnformatted("Glow");
	ImGui::SliderInt("Stencil", &g_Settings.esp.stencil, 0, 10);
	ImGui::SliderInt("Blur", &g_Settings.esp.blur, 0, 10);

	ImGui::Separator();

	static float red[3] = {g_Settings.colors.red_team.r()/255.0f, g_Settings.colors.red_team.g()/255.0f, g_Settings.colors.red_team.b()/255.0f};
	static float blu[3] = {g_Settings.colors.blu_team.r()/255.0f, g_Settings.colors.blu_team.g()/255.0f, g_Settings.colors.blu_team.b()/255.0f};
	static float target[3] = {g_Settings.colors.aimbot_target.r()/255.0f, g_Settings.colors.aimbot_target.g()/255.0f, g_Settings.colors.aimbot_target.b()/255.0f};
	static float weapon[3] = {g_Settings.colors.weapon.r()/255.0f, g_Settings.colors.weapon.g()/255.0f, g_Settings.colors.weapon.b()/255.0f};

	ImGui::TextUnformatted("Colors");

	if (ImGui::ColorEdit3("RED Team", red))
		g_Settings.colors.red_team.SetColor(red[0]*255.0f, red[1]*255.0f, red[2]*255.0f, 255.0f);

	if (ImGui::ColorEdit3("BLU Team", blu))
		g_Settings.colors.blu_team.SetColor(blu[0]*255.0f, blu[1]*255.0f, blu[2]*255.0f, 255.0f);

	if (ImGui::ColorEdit3("Aimbot Target", target))
		g_Settings.colors.aimbot_target.SetColor(target[0]*255.0f, target[1]*255.0f, target[2]*255.0f, 255.0f);

	if (ImGui::ColorEdit3("Weapon", weapon))
		g_Settings.colors.weapon.SetColor(weapon[0]*255.0f, weapon[1]*255.0f, weapon[2]*255.0f, 255.0f);

	ImGui::EndGroup();
}

static void DrawMiscTab()
{
	ImGui::BeginGroup();

	ImGui::Checkbox("Third person", &g_Settings.misc.thirdperson);
	ImGui::InputText("Third Person Key", &g_Settings.misc.thirdperson_key);

	ImGui::Separator();

	ImGui::Checkbox("Spectator List", &g_Settings.misc.spectatorlist);
	ImGui::Checkbox("Player List", &g_Settings.misc.playerlist);
	ImGui::Checkbox("sv_pure bypass", &g_Settings.misc.sv_pure_bypass);
	ImGui::Checkbox("Streamer Mode", &g_Settings.misc.streamer_mode);
	ImGui::Checkbox("Bhop", &g_Settings.misc.bhop);
	ImGui::Checkbox("Backpack Expander", &g_Settings.misc.backpack_expander);
	ImGui::Checkbox("Accept Item Drops", &g_Settings.misc.accept_item_drop);

	ImGui::Separator();

	ImGui::Checkbox("Custom Fov Enabled", &g_Settings.misc.customfov_enabled);
	ImGui::SliderFloat("Custom Fov", &g_Settings.misc.customfov, 54.0f, 120.0f);

	ImGui::Separator();

	ImGui::SliderFloat3("Viewmodel Offset", g_Settings.misc.viewmodel_offset, -20, 20.0f );
	ImGui::SliderFloat("Viewmodel Interp", &g_Settings.misc.viewmodel_interp, 0.0f, 50.0f);

	ImGui::EndGroup();
}

static void DrawTriggerTab()
{
	ImGui::BeginGroup();

	ImGui::Checkbox("Trigger Enabled", &g_Settings.triggerbot.enabled);
	ImGui::Checkbox("Hitscan", &g_Settings.triggerbot.hitscan);
	ImGui::InputText("Trigger Key", &g_Settings.triggerbot.key);

	if (ImGui::BeginCombo("Auto Backstab", g_Autobackstab.GetModeName().c_str()))
	{
		if (ImGui::Selectable("None"))
			g_Settings.triggerbot.autobackstab = AutoBackstabMode::NONE;

		if (ImGui::Selectable("Legit"))
			g_Settings.triggerbot.autobackstab = AutoBackstabMode::LEGIT;

		if (ImGui::Selectable("Rage"))
			g_Settings.triggerbot.autobackstab = AutoBackstabMode::RAGE;

		ImGui::EndCombo();
	}

	if (ImGui::BeginCombo("Auto Airblast", g_AutoAirblast.GetModeName().c_str()))
	{
		if (ImGui::Selectable("None"))
			g_Settings.triggerbot.autoairblast = AutoAirblastMode::NONE;

		if (ImGui::Selectable("Legit"))
			g_Settings.triggerbot.autoairblast = AutoAirblastMode::LEGIT;

		if (ImGui::Selectable("Rage"))
			g_Settings.triggerbot.autoairblast = AutoAirblastMode::RAGE;

		ImGui::EndCombo();
	}

	ImGui::EndGroup();
}

static void DrawAntiaimTab()
{
	ImGui::BeginGroup();

	ImGui::Checkbox("Enabled", &g_Settings.antiaim.enabled);

	if (ImGui::BeginCombo("Pitch Mode", Antiaim::GetPitchModeName(g_Settings.antiaim.pitch_mode).c_str()))
	{
		if (ImGui::Selectable("None"))
			g_Settings.antiaim.pitch_mode = PitchMode::NONE;

		if (ImGui::Selectable("Up"))
			g_Settings.antiaim.pitch_mode = PitchMode::UP;

		if (ImGui::Selectable("Down"))
			g_Settings.antiaim.pitch_mode = PitchMode::DOWN;

		if (ImGui::Selectable("Fake Up"))
			g_Settings.antiaim.pitch_mode = PitchMode::FAKEUP;

		if (ImGui::Selectable("Fake Down"))
			g_Settings.antiaim.pitch_mode = PitchMode::FAKEDOWN;

		if (ImGui::Selectable("Random"))
			g_Settings.antiaim.pitch_mode = PitchMode::RANDOM;

		ImGui::EndCombo();
	}

	if (ImGui::BeginCombo("Real Yaw Mode", Antiaim::GetYawModeName(g_Settings.antiaim.real_yaw_mode).c_str()))
	{
		if (ImGui::Selectable("None"))
			g_Settings.antiaim.real_yaw_mode = YawMode::NONE;

		if (ImGui::Selectable("Left"))
			g_Settings.antiaim.real_yaw_mode = YawMode::LEFT;

		if (ImGui::Selectable("Right"))
			g_Settings.antiaim.real_yaw_mode = YawMode::RIGHT;

		if (ImGui::Selectable("Spin Left"))
			g_Settings.antiaim.real_yaw_mode = YawMode::SPIN_LEFT;

		if (ImGui::Selectable("Spin Right"))
			g_Settings.antiaim.real_yaw_mode = YawMode::SPIN_RIGHT;

		if (ImGui::Selectable("Jitter"))
			g_Settings.antiaim.real_yaw_mode = YawMode::JITTER;

		if (ImGui::Selectable("Back"))
			g_Settings.antiaim.real_yaw_mode = YawMode::BACK;

		if (ImGui::Selectable("Forward"))
			g_Settings.antiaim.real_yaw_mode = YawMode::FORWARD;

		ImGui::EndCombo();
	}

	if (ImGui::BeginCombo("Fake Yaw Mode", Antiaim::GetYawModeName(g_Settings.antiaim.fake_yaw_mode).c_str()))
	{
		if (ImGui::Selectable("None"))
			g_Settings.antiaim.fake_yaw_mode = YawMode::NONE;

		if (ImGui::Selectable("Left"))
			g_Settings.antiaim.fake_yaw_mode = YawMode::LEFT;

		if (ImGui::Selectable("Right"))
			g_Settings.antiaim.fake_yaw_mode = YawMode::RIGHT;

		if (ImGui::Selectable("Spin Left"))
			g_Settings.antiaim.fake_yaw_mode = YawMode::SPIN_LEFT;

		if (ImGui::Selectable("Spin Right"))
			g_Settings.antiaim.fake_yaw_mode = YawMode::SPIN_RIGHT;

		if (ImGui::Selectable("Jitter"))
			g_Settings.antiaim.fake_yaw_mode = YawMode::JITTER;

		if (ImGui::Selectable("Back"))
			g_Settings.antiaim.fake_yaw_mode = YawMode::BACK;

		if (ImGui::Selectable("Forward"))
			g_Settings.antiaim.fake_yaw_mode = YawMode::FORWARD;

		ImGui::EndCombo();
	}

	ImGui::SliderFloat("Spin Speed", &g_Settings.antiaim.spin_speed, 0.0f, 10.0f);

	ImGui::EndGroup();
}

static void DrawLuaTab()
{
	static bool init = false;
	if (!init)
	{
		const char* keywords[]
		{
			"begin", "switch", "continue", "number",
			"int", "float", "boolean", "bool",
			"function", "table", "userdata", "nil",
			"any", "void", "enum", "export",
			"class", "new", "parent", "try",
			"catch",
		};

		const char* myIdentifiers[]
		{
			"globals", "engine",
			"hooks", "Vector3",
			"entities", "draw",
			"render", "materials",
			"client", "BitBuffer",
			"clientstate", "ui",
			"menu", "aimbot",
			#ifdef TRACY_ENABLE
			"tracy"
			#endif
		};

		auto def = TextEditor::LanguageDefinition::Lua();
		def.mPreprocChar = '$';

		for (auto& k: keywords)
			def.mKeywords.insert(k);

		TextEditor::Identifier id;
		id.mDeclaration = "Custom Library";

		for (auto& k : myIdentifiers)
			def.mIdentifiers.insert(std::make_pair(std::string(k), id));

		editor.SetLanguageDefinition(def);
		editor.SetPalette(TextEditor::GetDarkPalette());
		editor.SetShowWhitespaces(false);
		init = true;
	}

	ImGui::BeginGroup();

	if (ImGui::BeginTabBar("LuaTab"))
	{
		if (ImGui::BeginTabItem("Console Tab"))
		{
			ImVec2 size = ImGui::GetContentRegionAvail();
			size.y -= 30;

			ImGui::InputTextMultiline(
				"##ConsoleText",
				&consoleText,
				size,
				ImGuiInputTextFlags_ReadOnly | ImGuiInputTextFlags_WordWrap
			);

			ImGui::Spacing();

			if (ImGui::Button("Clear"))
				consoleText = "";

			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Editor Tab"))
		{
			editor.Render("Editor", ImVec2(0, -25));
	
			ImGui::Spacing();
	
			if (ImGui::Button("Run Code"))
				Lua::RunCode(editor.GetText());
	
			ImGui::SameLine();
	
			if (ImGui::Button("Clear"))
				editor.SetText("");

			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}

	ImGui::EndGroup();
}

static void DrawParsedNetvarData(const std::vector<NetvarClassEntry>& classes)
{
	for (const auto& cls : classes)
	{
		if (ImGui::TreeNode(cls.className.c_str()))
		{
			if (cls.members.empty())
				ImGui::TextDisabled("No members");
			else
				for (const auto& name : cls.members)
					ImGui::BulletText("%s", name.c_str());

			ImGui::TreePop();
		}
	}
}

static void DrawNetVarsTab()
{
	if (ImGui::BeginChild("NetvarContent"))
		DrawParsedNetvarData(netvarUI);
	ImGui::EndChild();
}

static void DrawSpectatorList()
{
	if (helper::engine::IsTakingScreenshot())
		return;

	ImGui::SetNextWindowSizeConstraints(
        	ImVec2(150.0f, 0.0f),
        	ImVec2(FLT_MAX, FLT_MAX)
    	);

	int flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse;
	if (!g_Settings.menu_open)
		flags |= ImGuiWindowFlags_NoMove;

	ImGui::Begin("Spectator List", nullptr, flags);

	int maxclients = helper::engine::GetMaxClients();
	if (!helper::engine::IsInMatch() || maxclients <= 1)
		return ImGui::End();

	CTFPlayer* pLocal = helper::engine::GetLocalPlayer();
	if (pLocal == nullptr || !pLocal->IsAlive())
		return ImGui::End();

	int localTeam = pLocal->m_iTeamNum();
	int localIndex = pLocal->GetIndex();

	for (const auto& entry : EntityList::GetEntities())
	{
		if (!(entry.flags & EntityFlags::IsPlayer))
			continue;

		CTFPlayer* player = static_cast<CTFPlayer*>(entry.ptr);
		if (player == nullptr)
			continue;

		if (player->IsAlive() || player == pLocal)
			continue;

		if (player->m_iTeamNum() != localTeam)
			continue;

		CTFPlayer* m_hObserverTarget = HandleAs<CTFPlayer*>(player->m_hObserverTarget());
		if (!m_hObserverTarget || m_hObserverTarget->GetIndex() != localIndex)
			continue;

		player_info_t info;
		if (!interfaces::Engine->GetPlayerInfo(player->GetIndex(), &info))
			continue;

		int m_iObserverMode = player->m_iObserverMode();
		bool isfirstperson = m_iObserverMode == OBS_MODE_IN_EYE;

		ImGui::TextColored(isfirstperson ? ImVec4(1.0, 0.5, 0.5, 1.0) : ImVec4(1.0, 1.0, 1.0, 1.0), "%s", player->GetName().c_str());
	}

	ImGui::End();
}

static void DrawPlayerList()
{
	if (interfaces::Engine->IsTakingScreenshot())
		return;

	ImGui::SetNextWindowSizeConstraints(
        	ImVec2(150.0f, 0.0f),
        	ImVec2(FLT_MAX, FLT_MAX)
    	);

	int flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse;
	if (!g_Settings.menu_open)
		flags |= ImGuiWindowFlags_NoMove;

	if (ImGui::Begin("Player List", nullptr, flags))
	{
		for (const auto& entry : EntityList::GetEntities())
		{
			if (!(entry.flags & EntityFlags::IsPlayer))
				continue;

			auto* entity = static_cast<CTFPlayer*>(entry.ptr);
			if (entity == nullptr)
				continue;

			std::string name = entity->GetName();

			switch (entity->m_iTeamNum())
			{
				case TEAM_BLU:
				{
					Color color = g_Settings.colors.blu_team;
					ImVec4 textColor(color.r()/255.0f, color.g()/255.0f, color.b()/255.0f, 255);
					ImGui::TextColored(textColor, "%s", name.c_str());
					break;
				}

				case TEAM_RED:
				{
					Color color = g_Settings.colors.red_team;
					ImVec4 textColor(color.r()/255.0f, color.g()/255.0f, color.b()/255.0f, 255);
					ImGui::TextColored(textColor, "%s", name.c_str());
					break;
				}

				default: break;
			}
		}
	}
	ImGui::End();
}

static void DrawMainWindow()
{
	if (helper::engine::IsTakingScreenshot())
		return;

	static int tab = 0;

	if (ImGui::Begin("Skill Issue", nullptr, ImGuiWindowFlags_NoCollapse))
	{
		if (ImGui::BeginTable("MainTable", 2, 0))
		{
			ImGui::TableSetupColumn("Buttons", ImGuiTableColumnFlags_WidthFixed, 70);
			ImGui::TableSetupColumn("Content", ImGuiTableColumnFlags_WidthStretch);
			
			ImGui::TableNextRow();
			
			ImGui::TableSetColumnIndex(0);
			DrawTabButtons(tab);

			ImGui::TableSetColumnIndex(1);
			switch(tab)
			{
				case TAB_AIMBOT: DrawAimbotTab(); break;
				case TAB_ESP: DrawESPTab(); break;
				case TAB_MISC: DrawMiscTab(); break;
				case TAB_TRIGGER: DrawTriggerTab(); break;
				case TAB_ANTIAIM: DrawAntiaimTab(); break;
				case TAB_LUA: DrawLuaTab(); break;
				case TAB_NETVARS: DrawNetVarsTab(); break;
				default: break;
			}
			
			ImGui::EndTable();
		}
	}
	ImGui::End();
}