#include "gui.h"

TextEditor GUI::editor;
int GUI::tab = 0;

void DrawTabButtons(int &tab)
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

	if (ImGui::Button("RADAR", ImVec2(-1, 0)))
		tab = TAB_RADAR;

	if (ImGui::Button("PLUTO", ImVec2(-1, 0)))
		tab = TAB_LUA;

	if (ImGui::Button("NETVARS", ImVec2(-1, 0)))
		tab = TAB_NETVARS;

	ImGui::EndGroup();
}

void DrawAimbotTab()
{
	ImGui::BeginGroup();

	ImGui::Checkbox("Enabled", &Settings::aimbot.enabled);
	ImGui::InputText("Key", &Settings::aimbot.key);
	ImGui::Checkbox("Autoshoot", &Settings::aimbot.autoshoot);
	ImGui::Checkbox("Viewmodel Aim", &Settings::aimbot.viewmodelaim);
	ImGui::Checkbox("Draw FOV Indicator", &Settings::aimbot.draw_fov_indicator);
	ImGui::Checkbox("Wait For Charge", &Settings::aimbot.waitforcharge);
	ImGui::SliderFloat("Fov", &Settings::aimbot.fov, 0.0f, 180.0f);
	ImGui::SliderFloat("Max Sim Time", &Settings::aimbot.max_sim_time, 0.0f, 5.0f);
	ImGui::SliderFloat("Smoothness", &Settings::aimbot.smoothness, 10.0f, 100.0f);

	if (ImGui::BeginCombo("Melee Aimbot", AimbotMelee::GetMeleeModeName().c_str()))
	{
		if (ImGui::Selectable("None"))
			Settings::aimbot.melee = MeleeMode::NONE;

		if (ImGui::Selectable("Legit"))
			Settings::aimbot.melee = MeleeMode::LEGIT;

		if (ImGui::Selectable("Rage"))
			Settings::aimbot.melee = MeleeMode::RAGE;

		ImGui::EndCombo();
	}

	if (ImGui::BeginCombo("Aimbot Method", AimbotUtils::GetAimbotModeName().c_str()))
	{
		if (ImGui::Selectable("Plain"))
			Settings::aimbot.mode = AimbotMode::PLAIN;

		if (ImGui::Selectable("Smooth"))
			Settings::aimbot.mode = AimbotMode::SMOOTH;

		if (ImGui::Selectable("Assistance"))
			Settings::aimbot.mode = AimbotMode::ASSISTANCE;

		if (ImGui::Selectable("Silent"))
			Settings::aimbot.mode = AimbotMode::SILENT;

		ImGui::EndCombo();
	}

	if (ImGui::BeginCombo("Team Selection", AimbotUtils::GetTeamModeName().c_str()))
	{
		if (ImGui::Selectable("Only Enemy"))
			Settings::aimbot.teamMode = TeamMode::ONLYENEMY;

		if (ImGui::Selectable("Only Teammate"))
			Settings::aimbot.teamMode = TeamMode::ONLYTEAMMATE;

		if (ImGui::Selectable("Both"))
			Settings::aimbot.teamMode = TeamMode::BOTH;

		ImGui::EndCombo();
	}

	ImGui::Separator();

	ImGui::TextUnformatted("Ignore Options");
	ImGui::Checkbox("Cloaked", &Settings::aimbot.ignorecloaked);
	ImGui::SameLine();
	ImGui::Checkbox("Ubercharged", &Settings::aimbot.ignoreubered);
	ImGui::SameLine();
	ImGui::Checkbox("Hoovy", &Settings::aimbot.ignorehoovy);
	ImGui::SameLine();
	ImGui::Checkbox("Bonked", &Settings::aimbot.ignorebonked);

	ImGui::Separator();

	ImGui::Checkbox("Hold Minigun Spin", &Settings::aimbot.hold_minigun_spin);

	ImGui::EndGroup();
}

void DrawESPTab()
{
	ImGui::BeginGroup();

	ImGui::Checkbox("ESP Enabled", &Settings::esp.enabled);
	ImGui::Checkbox("Name", &Settings::esp.name);
	ImGui::Checkbox("Box", &Settings::esp.box);
	ImGui::Checkbox("Ignore Cloakeds", &Settings::esp.ignorecloaked);
	ImGui::Checkbox("Buildings", &Settings::esp.buildings);
	ImGui::Checkbox("Weapon", &Settings::esp.weapon);

	ImGui::Separator();

	ImGui::Checkbox("Chams", &Settings::esp.chams);

	ImGui::Separator();

	ImGui::TextUnformatted("Glow");
	ImGui::SliderInt("Stencil", &Settings::esp.stencil, 0, 10);
	ImGui::SliderInt("Blur", &Settings::esp.blur, 0, 10);

	ImGui::Separator();

	static float red[3] = {Settings::colors.red_team.r()/255.0f, Settings::colors.red_team.g()/255.0f, Settings::colors.red_team.b()/255.0f};
	static float blu[3] = {Settings::colors.blu_team.r()/255.0f, Settings::colors.blu_team.g()/255.0f, Settings::colors.blu_team.b()/255.0f};
	static float target[3] = {Settings::colors.aimbot_target.r()/255.0f, Settings::colors.aimbot_target.g()/255.0f, Settings::colors.aimbot_target.b()/255.0f};
	static float weapon[3] = {Settings::colors.weapon.r()/255.0f, Settings::colors.weapon.g()/255.0f, Settings::colors.weapon.b()/255.0f};

	ImGui::TextUnformatted("Colors");

	if (ImGui::ColorEdit3("RED Team", red))
		Settings::colors.red_team.SetColor(red[0]*255.0f, red[1]*255.0f, red[2]*255.0f, 255.0f);

	if (ImGui::ColorEdit3("BLU Team", blu))
		Settings::colors.blu_team.SetColor(blu[0]*255.0f, blu[1]*255.0f, blu[2]*255.0f, 255.0f);

	if (ImGui::ColorEdit3("Aimbot Target", target))
		Settings::colors.aimbot_target.SetColor(target[0]*255.0f, target[1]*255.0f, target[2]*255.0f, 255.0f);

	if (ImGui::ColorEdit3("Weapon", weapon))
		Settings::colors.weapon.SetColor(weapon[0]*255.0f, weapon[1]*255.0f, weapon[2]*255.0f, 255.0f);

	ImGui::EndGroup();
}

void DrawMiscTab()
{
	ImGui::BeginGroup();

	ImGui::Checkbox("Third person", &Settings::misc.thirdperson);
	ImGui::InputText("Third Person Key", &Settings::misc.thirdperson_key);

	ImGui::Separator();

	ImGui::Checkbox("Spectator List", &Settings::misc.spectatorlist);
	ImGui::Checkbox("Player List", &Settings::misc.playerlist);
	ImGui::Checkbox("sv_pure bypass", &Settings::misc.sv_pure_bypass);
	ImGui::Checkbox("Streamer Mode", &Settings::misc.streamer_mode);
	ImGui::Checkbox("Bhop", &Settings::misc.bhop);
	ImGui::Checkbox("Backpack Expander", &Settings::misc.backpack_expander);
	ImGui::Checkbox("Accept Item Drops", &Settings::misc.accept_item_drop);

	ImGui::Checkbox("No Recoil", &Settings::misc.norecoil);

	ImGui::Separator();

	ImGui::Checkbox("Custom Fov Enabled", &Settings::misc.customfov_enabled);
	ImGui::SliderFloat("Custom Fov", &Settings::misc.customfov, 54.0f, 120.0f);

	ImGui::Separator();

	ImGui::SliderFloat3("Viewmodel Offset", Settings::misc.viewmodel_offset, -20, 20.0f );
	ImGui::SliderFloat("Viewmodel Interp", &Settings::misc.viewmodel_interp, 0.0f, 50.0f);

	ImGui::EndGroup();
}

void DrawTriggerTab()
{
	ImGui::BeginGroup();

	ImGui::Checkbox("Trigger Enabled", &Settings::triggerbot.enabled);
	ImGui::Checkbox("Hitscan", &Settings::triggerbot.hitscan);
	ImGui::InputText("Trigger Key", &Settings::triggerbot.key);

	if (ImGui::BeginCombo("Auto Backstab", AutoBackstab::GetModeName().c_str()))
	{
		if (ImGui::Selectable("None"))
			Settings::triggerbot.autobackstab = AutoBackstabMode::NONE;

		if (ImGui::Selectable("Legit"))
			Settings::triggerbot.autobackstab = AutoBackstabMode::LEGIT;

		if (ImGui::Selectable("Rage"))
			Settings::triggerbot.autobackstab = AutoBackstabMode::RAGE;

		ImGui::EndCombo();
	}

	if (ImGui::BeginCombo("Auto Airblast", AutoAirblast::GetModeName().c_str()))
	{
		if (ImGui::Selectable("None"))
			Settings::triggerbot.autoairblast = AutoAirblastMode::NONE;

		if (ImGui::Selectable("Legit"))
			Settings::triggerbot.autoairblast = AutoAirblastMode::LEGIT;

		if (ImGui::Selectable("Rage"))
			Settings::triggerbot.autoairblast = AutoAirblastMode::RAGE;

		ImGui::EndCombo();
	}

	ImGui::EndGroup();
}

void DrawAntiaimTab()
{
	ImGui::BeginGroup();

	ImGui::Checkbox("Enabled", &Settings::antiaim.enabled);

	if (ImGui::BeginCombo("Pitch Mode", Antiaim::GetPitchModeName(Settings::antiaim.pitch_mode).c_str()))
	{
		if (ImGui::Selectable("None"))
			Settings::antiaim.pitch_mode = PitchMode::NONE;

		if (ImGui::Selectable("Up"))
			Settings::antiaim.pitch_mode = PitchMode::UP;

		if (ImGui::Selectable("Down"))
			Settings::antiaim.pitch_mode = PitchMode::DOWN;

		if (ImGui::Selectable("Fake Up"))
			Settings::antiaim.pitch_mode = PitchMode::FAKEUP;

		if (ImGui::Selectable("Fake Down"))
			Settings::antiaim.pitch_mode = PitchMode::FAKEDOWN;

		if (ImGui::Selectable("Random"))
			Settings::antiaim.pitch_mode = PitchMode::RANDOM;

		ImGui::EndCombo();
	}

	if (ImGui::BeginCombo("Real Yaw Mode", Antiaim::GetYawModeName(Settings::antiaim.real_yaw_mode).c_str()))
	{
		if (ImGui::Selectable("None"))
			Settings::antiaim.real_yaw_mode = YawMode::NONE;

		if (ImGui::Selectable("Left"))
			Settings::antiaim.real_yaw_mode = YawMode::LEFT;

		if (ImGui::Selectable("Right"))
			Settings::antiaim.real_yaw_mode = YawMode::RIGHT;

		if (ImGui::Selectable("Spin Left"))
			Settings::antiaim.real_yaw_mode = YawMode::SPIN_LEFT;

		if (ImGui::Selectable("Spin Right"))
			Settings::antiaim.real_yaw_mode = YawMode::SPIN_RIGHT;

		if (ImGui::Selectable("Jitter"))
			Settings::antiaim.real_yaw_mode = YawMode::JITTER;

		if (ImGui::Selectable("Back"))
			Settings::antiaim.real_yaw_mode = YawMode::BACK;

		if (ImGui::Selectable("Forward"))
			Settings::antiaim.real_yaw_mode = YawMode::FORWARD;

		ImGui::EndCombo();
	}

	if (ImGui::BeginCombo("Fake Yaw Mode", Antiaim::GetYawModeName(Settings::antiaim.fake_yaw_mode).c_str()))
	{
		if (ImGui::Selectable("None"))
			Settings::antiaim.fake_yaw_mode = YawMode::NONE;

		if (ImGui::Selectable("Left"))
			Settings::antiaim.fake_yaw_mode = YawMode::LEFT;

		if (ImGui::Selectable("Right"))
			Settings::antiaim.fake_yaw_mode = YawMode::RIGHT;

		if (ImGui::Selectable("Spin Left"))
			Settings::antiaim.fake_yaw_mode = YawMode::SPIN_LEFT;

		if (ImGui::Selectable("Spin Right"))
			Settings::antiaim.fake_yaw_mode = YawMode::SPIN_RIGHT;

		if (ImGui::Selectable("Jitter"))
			Settings::antiaim.fake_yaw_mode = YawMode::JITTER;

		if (ImGui::Selectable("Back"))
			Settings::antiaim.fake_yaw_mode = YawMode::BACK;

		if (ImGui::Selectable("Forward"))
			Settings::antiaim.fake_yaw_mode = YawMode::FORWARD;

		ImGui::EndCombo();
	}

	ImGui::SliderFloat("Spin Speed", &Settings::antiaim.spin_speed, 0.0f, 10.0f);

	ImGui::Separator();

	ImGui::Checkbox("Warp", &Settings::antiaim.warp_enabled);
	ImGui::SliderInt("Speed", &Settings::antiaim.warp_speed, 1, 24);
	ImGui::InputText("Warp Key", &Settings::antiaim.warp_key);
	ImGui::InputText("Warp Recharge Key", &Settings::antiaim.warp_recharge_key);
	ImGui::InputText("Warp DoubleTap Key", &Settings::antiaim.warp_dt_key);

	ImGui::EndGroup();
}

void DrawLuaTab()
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
			"radar", "colors",
			"fonts", "warp",
		};

		auto def = TextEditor::LanguageDefinition::Lua();
		def.mPreprocChar = '$';

		for (auto& k: keywords)
			def.mKeywords.insert(k);

		TextEditor::Identifier id;
		id.mDeclaration = "Custom Library";

		for (auto& k : myIdentifiers)
			def.mIdentifiers.insert(std::make_pair(std::string(k), id));

		GUI::editor.SetLanguageDefinition(def);
		GUI::editor.SetPalette(TextEditor::GetDarkPalette());
		GUI::editor.SetShowWhitespaces(false);
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
			GUI::editor.Render("Editor", ImVec2(0, -25));
	
			ImGui::Spacing();
	
			if (ImGui::Button("Run Code"))
				Lua::RunCode(GUI::editor.GetText());
	
			ImGui::SameLine();
	
			if (ImGui::Button("Clear"))
				GUI::editor.SetText("");

			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}

	ImGui::EndGroup();
}

void DrawParsedNetvarData(const std::vector<NetvarClassEntry>& classes)
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

void DrawNetVarsTab()
{
	if (ImGui::BeginChild("NetvarContent"))
		DrawParsedNetvarData(Netvars::m_netvarUIVector);
	ImGui::EndChild();
}

void DrawRadarTab()
{
	ImGui::Checkbox("Enabled", &Settings::radar.enabled);
	ImGui::SliderInt("Size", &Settings::radar.size, 1, 300);
	ImGui::SliderInt("Icon Size", &Settings::radar.icon_size, 1, 15);
	ImGui::SliderInt("Range", &Settings::radar.range, 10, 3000);

	ImGui::Separator();
	ImGui::Checkbox("Players", &Settings::radar.players);
	ImGui::Checkbox("Projectiles", &Settings::radar.projectiles);
	//ImGui::Checkbox("Objective", &Settings::radar.objective);
	ImGui::Checkbox("Buildings", &Settings::radar.buildings);
}

void GUI::RunSpectatorList()
{
	if (helper::engine::IsTakingScreenshot())
		return;

	ImGui::SetNextWindowSizeConstraints(
        	ImVec2(150.0f, 0.0f),
        	ImVec2(FLT_MAX, FLT_MAX)
    	);

	int flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse;
	if (!Settings::menu_open)
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

void GUI::RunPlayerList()
{
	if (interfaces::Engine->IsTakingScreenshot())
		return;

	ImGui::SetNextWindowSizeConstraints(
        	ImVec2(150.0f, 0.0f),
        	ImVec2(FLT_MAX, FLT_MAX)
    	);

	int flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse;
	if (!Settings::menu_open)
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
					Color color = Settings::colors.blu_team;
					ImVec4 textColor(color.r()/255.0f, color.g()/255.0f, color.b()/255.0f, 255);
					ImGui::TextColored(textColor, "%s", name.c_str());
					break;
				}

				case TEAM_RED:
				{
					Color color = Settings::colors.red_team;
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

void GUI::RunMainWindow()
{
	if (helper::engine::IsTakingScreenshot())
		return;

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
				case TAB_RADAR: DrawRadarTab(); break;
				default: break;
			}
			
			ImGui::EndTable();
		}
	}
	ImGui::End();
}