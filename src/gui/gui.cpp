#include "gui.h"

#include <filesystem>

TextEditor GUI::editor;
int GUI::tab = 0;
bool GUI::openDeletePopup = false;

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

	if (ImGui::Button("CONFIGS", ImVec2(-1, 0)))
		tab = TAB_CONFIG;

	ImGui::EndGroup();
}

void DrawAimbotTab()
{
	ImGui::BeginGroup();

	ImGui::Checkbox("Enabled", &Settings::Aimbot::enabled);
	ImGui::InputText("Key", &Settings::Aimbot::key);
	ImGui::Checkbox("Autoshoot", &Settings::Aimbot::autoshoot);
	ImGui::Checkbox("Viewmodel Aim", &Settings::Aimbot::viewmodelaim);
	ImGui::Checkbox("Draw FOV Indicator", &Settings::Aimbot::draw_fov_indicator);
	ImGui::Checkbox("Wait For Charge", &Settings::Aimbot::waitforcharge);
	ImGui::SliderFloat("Fov", &Settings::Aimbot::fov, 0.0f, 180.0f);
	ImGui::SliderFloat("Max Sim Time", &Settings::Aimbot::max_sim_time, 0.0f, 5.0f);
	ImGui::SliderFloat("Smoothness", &Settings::Aimbot::smoothness, 0.0f, 100.0f);

	if (ImGui::BeginCombo("Melee Aimbot", AimbotMelee::GetMeleeModeName().c_str()))
	{
		if (ImGui::Selectable("None"))
			Settings::Aimbot::melee = static_cast<int>(MeleeMode::NONE);

		if (ImGui::Selectable("Legit"))
			Settings::Aimbot::melee = static_cast<int>(MeleeMode::LEGIT);

		if (ImGui::Selectable("Rage"))
			Settings::Aimbot::melee = static_cast<int>(MeleeMode::RAGE);

		ImGui::EndCombo();
	}

	if (ImGui::BeginCombo("Aimbot Method", AimbotUtils::GetAimbotModeName().c_str()))
	{
		if (ImGui::Selectable("Plain"))
			Settings::Aimbot::mode = static_cast<int>(AimbotMode::PLAIN);

		if (ImGui::Selectable("Smooth"))
			Settings::Aimbot::mode = static_cast<int>(AimbotMode::SMOOTH);

		if (ImGui::Selectable("Assistance"))
			Settings::Aimbot::mode = static_cast<int>(AimbotMode::ASSISTANCE);

		if (ImGui::Selectable("Silent"))
			Settings::Aimbot::mode = static_cast<int>(AimbotMode::SILENT);

		ImGui::EndCombo();
	}

	if (ImGui::BeginCombo("Team Selection", AimbotUtils::GetTeamModeName().c_str()))
	{
		if (ImGui::Selectable("Only Enemy"))
			Settings::Aimbot::teamMode = static_cast<int>(TeamMode::ONLYENEMY);

		if (ImGui::Selectable("Only Teammate"))
			Settings::Aimbot::teamMode = static_cast<int>(TeamMode::ONLYTEAMMATE);

		if (ImGui::Selectable("Both"))
			Settings::Aimbot::teamMode = static_cast<int>(TeamMode::BOTH);

		ImGui::EndCombo();
	}

	ImGui::Separator();

	ImGui::TextUnformatted("Ignore Options");
	ImGui::Checkbox("Cloaked", &Settings::Aimbot::ignorecloaked);
	ImGui::SameLine();
	ImGui::Checkbox("Ubercharged", &Settings::Aimbot::ignoreubered);
	ImGui::SameLine();
	ImGui::Checkbox("Hoovy", &Settings::Aimbot::ignorehoovy);
	ImGui::SameLine();
	ImGui::Checkbox("Bonked", &Settings::Aimbot::ignorebonked);

	ImGui::Separator();

	ImGui::Checkbox("Hold Minigun Spin", &Settings::Aimbot::hold_minigun_spin);

	ImGui::EndGroup();
}

void DrawESPTab()
{
	ImGui::BeginGroup();

	ImGui::Checkbox("ESP Enabled", &Settings::ESP::enabled);
	ImGui::Checkbox("Name", &Settings::ESP::name);
	ImGui::Checkbox("Box", &Settings::ESP::box);
	ImGui::Checkbox("Ignore Cloaked", &Settings::ESP::ignorecloaked);
	ImGui::Checkbox("Buildings", &Settings::ESP::buildings);
	ImGui::Checkbox("Weapon", &Settings::ESP::weapon);
	ImGui::Checkbox("Health", &Settings::ESP::healthbar);

	ImGui::Separator();

	ImGui::Checkbox("Chams", &Settings::ESP::chams);

	ImGui::Separator();

	ImGui::TextUnformatted("Glow");
	ImGui::SliderInt("Stencil", &Settings::ESP::stencil, 0, 10);
	ImGui::SliderInt("Blur", &Settings::ESP::blur, 0, 10);

	ImGui::Separator();

	static float red[3] = {Settings::Colors::red_team.r()/255.0f, Settings::Colors::red_team.g()/255.0f, Settings::Colors::red_team.b()/255.0f};
	static float blu[3] = {Settings::Colors::blu_team.r()/255.0f, Settings::Colors::blu_team.g()/255.0f, Settings::Colors::blu_team.b()/255.0f};
	static float target[3] = {Settings::Colors::aimbot_target.r()/255.0f, Settings::Colors::aimbot_target.g()/255.0f, Settings::Colors::aimbot_target.b()/255.0f};
	static float weapon[3] = {Settings::Colors::weapon.r()/255.0f, Settings::Colors::weapon.g()/255.0f, Settings::Colors::weapon.b()/255.0f};

	ImGui::TextUnformatted("Colors");

	if (ImGui::ColorEdit3("RED Team", red))
		Settings::Colors::red_team.SetColor(red[0]*255.0f, red[1]*255.0f, red[2]*255.0f, 255.0f);

	if (ImGui::ColorEdit3("BLU Team", blu))
		Settings::Colors::blu_team.SetColor(blu[0]*255.0f, blu[1]*255.0f, blu[2]*255.0f, 255.0f);

	if (ImGui::ColorEdit3("Aimbot Target", target))
		Settings::Colors::aimbot_target.SetColor(target[0]*255.0f, target[1]*255.0f, target[2]*255.0f, 255.0f);

	if (ImGui::ColorEdit3("Weapon", weapon))
		Settings::Colors::weapon.SetColor(weapon[0]*255.0f, weapon[1]*255.0f, weapon[2]*255.0f, 255.0f);

	ImGui::EndGroup();
}

void DrawMiscTab()
{
	ImGui::BeginGroup();

	ImGui::Checkbox("Third person", &Settings::Misc::thirdperson);
	ImGui::InputText("Third Person Key", &Settings::Misc::thirdperson_key);

	ImGui::Separator();

	ImGui::Checkbox("Spectator List", &Settings::Misc::spectatorlist);
	ImGui::Checkbox("Player List", &Settings::Misc::playerlist);
	ImGui::Checkbox("sv_pure bypass", &Settings::Misc::sv_pure_bypass);
	ImGui::Checkbox("Streamer Mode", &Settings::Misc::streamer_mode);
	ImGui::Checkbox("Bhop", &Settings::Misc::bhop);
	ImGui::Checkbox("Backpack Expander", &Settings::Misc::backpack_expander);
	ImGui::Checkbox("Accept Item Drops", &Settings::Misc::accept_item_drop);

	ImGui::Checkbox("No Recoil", &Settings::Misc::norecoil);

	ImGui::Separator();

	ImGui::Checkbox("Custom Fov Enabled", &Settings::Misc::customfov_enabled);
	ImGui::SliderFloat("Custom Fov", &Settings::Misc::customfov, 54.0f, 120.0f);

	ImGui::Separator();

	ImGui::SliderFloat3("Viewmodel Offset", Settings::Misc::viewmodel_offset, -20, 20.0f );
	ImGui::SliderFloat("Viewmodel Interp", &Settings::Misc::viewmodel_interp, 0.0f, 50.0f);

	ImGui::EndGroup();
}

void DrawTriggerTab()
{
	ImGui::BeginGroup();

	ImGui::Checkbox("Trigger Enabled", &Settings::Triggerbot::enabled);
	ImGui::Checkbox("Hitscan", &Settings::Triggerbot::hitscan);
	ImGui::InputText("Trigger Key", &Settings::Triggerbot::key);

	if (ImGui::BeginCombo("Auto Backstab", AutoBackstab::GetModeName().c_str()))
	{
		if (ImGui::Selectable("None"))
			Settings::Triggerbot::autobackstab = static_cast<int>(AutoBackstabMode::NONE);

		if (ImGui::Selectable("Legit"))
			Settings::Triggerbot::autobackstab = static_cast<int>(AutoBackstabMode::LEGIT);

		if (ImGui::Selectable("Rage"))
			Settings::Triggerbot::autobackstab = static_cast<int>(AutoBackstabMode::RAGE);

		ImGui::EndCombo();
	}

	if (ImGui::BeginCombo("Auto Airblast", AutoAirblast::GetModeName().c_str()))
	{
		if (ImGui::Selectable("None"))
			Settings::Triggerbot::autoairblast = static_cast<int>(AutoBackstabMode::NONE);

		if (ImGui::Selectable("Legit"))
			Settings::Triggerbot::autoairblast = static_cast<int>(AutoBackstabMode::LEGIT);

		if (ImGui::Selectable("Rage"))
			Settings::Triggerbot::autoairblast = static_cast<int>(AutoBackstabMode::RAGE);

		ImGui::EndCombo();
	}

	ImGui::EndGroup();
}

void DrawAntiaimTab()
{
	ImGui::BeginGroup();

	ImGui::Checkbox("Enabled", &Settings::AntiAim::enabled);

	if (ImGui::BeginCombo("Pitch Mode", Antiaim::GetPitchModeName(static_cast<PitchMode>(Settings::AntiAim::pitch_mode)).c_str()))
	{
		if (ImGui::Selectable("None"))
			Settings::AntiAim::pitch_mode = static_cast<int>(PitchMode::NONE);

		if (ImGui::Selectable("Up"))
			Settings::AntiAim::pitch_mode = static_cast<int>(PitchMode::UP);

		if (ImGui::Selectable("Down"))
			Settings::AntiAim::pitch_mode = static_cast<int>(PitchMode::DOWN);

		if (ImGui::Selectable("Fake Up"))
			Settings::AntiAim::pitch_mode = static_cast<int>(PitchMode::FAKEUP);

		if (ImGui::Selectable("Fake Down"))
			Settings::AntiAim::pitch_mode = static_cast<int>(PitchMode::FAKEDOWN);

		if (ImGui::Selectable("Random"))
			Settings::AntiAim::pitch_mode = static_cast<int>(PitchMode::RANDOM);

		ImGui::EndCombo();
	}

	if (ImGui::BeginCombo("Real Yaw Mode", Antiaim::GetYawModeName(static_cast<YawMode>(Settings::AntiAim::real_yaw_mode)).c_str()))
	{
		if (ImGui::Selectable("None"))
			Settings::AntiAim::real_yaw_mode = static_cast<int>(YawMode::NONE);

		if (ImGui::Selectable("Left"))
			Settings::AntiAim::real_yaw_mode = static_cast<int>(YawMode::LEFT);

		if (ImGui::Selectable("Right"))
			Settings::AntiAim::real_yaw_mode = static_cast<int>(YawMode::RIGHT);

		if (ImGui::Selectable("Spin Left"))
			Settings::AntiAim::real_yaw_mode = static_cast<int>(YawMode::SPIN_LEFT);

		if (ImGui::Selectable("Spin Right"))
			Settings::AntiAim::real_yaw_mode = static_cast<int>(YawMode::SPIN_RIGHT);

		if (ImGui::Selectable("Jitter"))
			Settings::AntiAim::real_yaw_mode = static_cast<int>(YawMode::JITTER);

		if (ImGui::Selectable("Back"))
			Settings::AntiAim::real_yaw_mode = static_cast<int>(YawMode::BACK);

		if (ImGui::Selectable("Forward"))
			Settings::AntiAim::real_yaw_mode = static_cast<int>(YawMode::FORWARD);

		ImGui::EndCombo();
	}

	if (ImGui::BeginCombo("Fake Yaw Mode", Antiaim::GetYawModeName(static_cast<YawMode>(Settings::AntiAim::fake_yaw_mode)).c_str()))
	{
		if (ImGui::Selectable("None"))
			Settings::AntiAim::fake_yaw_mode = static_cast<int>(YawMode::NONE);

		if (ImGui::Selectable("Left"))
			Settings::AntiAim::fake_yaw_mode = static_cast<int>(YawMode::LEFT);

		if (ImGui::Selectable("Right"))
			Settings::AntiAim::fake_yaw_mode = static_cast<int>(YawMode::RIGHT);

		if (ImGui::Selectable("Spin Left"))
			Settings::AntiAim::fake_yaw_mode = static_cast<int>(YawMode::SPIN_LEFT);

		if (ImGui::Selectable("Spin Right"))
			Settings::AntiAim::fake_yaw_mode = static_cast<int>(YawMode::SPIN_RIGHT);

		if (ImGui::Selectable("Jitter"))
			Settings::AntiAim::fake_yaw_mode = static_cast<int>(YawMode::JITTER);

		if (ImGui::Selectable("Back"))
			Settings::AntiAim::fake_yaw_mode = static_cast<int>(YawMode::BACK);

		if (ImGui::Selectable("Forward"))
			Settings::AntiAim::fake_yaw_mode = static_cast<int>(YawMode::FORWARD);

		ImGui::EndCombo();
	}

	ImGui::SliderFloat("Spin Speed", &Settings::AntiAim::spin_speed, 0.0f, 10.0f);

	ImGui::Separator();

	ImGui::Checkbox("Warp", &Settings::AntiAim::warp_enabled);
	ImGui::SliderInt("Speed", &Settings::AntiAim::warp_speed, 1, 24);
	ImGui::InputText("Warp Key", &Settings::AntiAim::warp_key);
	ImGui::InputText("Warp Recharge Key", &Settings::AntiAim::warp_recharge_key);
	ImGui::InputText("DoubleTap Key", &Settings::AntiAim::warp_dt_key);

	ImGui::Separator();

	ImGui::Checkbox("Fake Lag Enabled", &Settings::AntiAim::fakelag_enabled);
	ImGui::SliderInt("Ticks", &Settings::AntiAim::fakelag_ticks, 1, 21);

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
			"hooks", "vector3",
			"entities", "draw",
			"render", "materials",
			"client", "bitbuffer",
			"clientstate", "ui",
			"menu", "aimbot",
			"radar", "colors",
			"fonts", "warp",
			"esp", "color"
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
	ImGui::Checkbox("Enabled", &Settings::Radar::enabled);
	ImGui::SliderInt("Size", &Settings::Radar::size, 1, 300);
	ImGui::SliderInt("Icon Size", &Settings::Radar::icon_size, 1, 15);
	ImGui::SliderInt("Range", &Settings::Radar::range, 10, 3000);

	ImGui::Separator();
	ImGui::Checkbox("Players", &Settings::Radar::players);
	ImGui::Checkbox("Projectiles", &Settings::Radar::projectiles);
	//ImGui::Checkbox("Objective", &Settings::Radar::objective);
	ImGui::Checkbox("Buildings", &Settings::Radar::buildings);
}

int GUI::selectedIndex = -1;
std::vector<std::string> GUI::configs;
std::string GUI::newConfigName = "";
bool GUI::firstOpenConfigTab = true;

void RefreshConfigList(const std::string& folder)
{
	GUI::configs.clear();

	if (!std::filesystem::exists(folder))
		std::filesystem::create_directories(folder);

	for (const auto& entry : std::filesystem::directory_iterator(folder))
	{
		if (entry.is_regular_file() && entry.path().extension() == ".txt")
		{
			GUI::configs.push_back(entry.path().stem().string());
		}
	}
}

void DrawConfigTab()
{
	const std::string configFolder = "./skill-issue-configs";

	if (GUI::firstOpenConfigTab)
	{
	    RefreshConfigList(configFolder);
	    GUI::firstOpenConfigTab = false;
	}

	if (ImGui::Button("Refresh"))
	{
		RefreshConfigList(configFolder);
		GUI::selectedIndex = -1;
	}

	ImGui::SameLine();

	float buttonWidth = 120.0f;
	ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - buttonWidth - ImGui::GetStyle().ItemSpacing.x);

	ImGui::InputText("##NewConfigName", &GUI::newConfigName, sizeof(GUI::newConfigName));

	ImGui::SameLine();

	if (ImGui::Button("Create & Save", ImVec2(buttonWidth, 0)))
	{
		if (GUI::newConfigName.length() > 0)
		{
			std::string filename = std::string(GUI::newConfigName) + ".txt";
			std::string fullPath = configFolder + "/" + filename;

			Settings::SaveAll(fullPath);
			RefreshConfigList(configFolder);
			GUI::newConfigName[0] = '\0';
		}
	}

	ImGui::Separator();
    
	if (ImGui::BeginChild("##Configs"))
	{
		// config list
		for (int i = 0; i < GUI::configs.size(); ++i)
		{
			bool isSelected = GUI::selectedIndex == i;

			if (ImGui::Selectable(GUI::configs[i].c_str(), isSelected))
				GUI::selectedIndex = i;

			if (isSelected)
			{
				std::string fullPath = configFolder + "/" + GUI::configs[i] + ".txt";

				if (ImGui::Button("Load"))
					Settings::LoadAll(fullPath);
			
				ImGui::SameLine();
			
				if (ImGui::Button("Save"))
					Settings::SaveAll(fullPath);
		
				ImGui::SameLine();

				if (ImGui::Button("Delete"))
					GUI::openDeletePopup = true;
		
				// this doesn't look good to me, should I change it?
				if (GUI::openDeletePopup)
					ImGui::OpenPopup("ConfirmDelete");
		
				if (ImGui::BeginPopupModal("ConfirmDelete", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
				{
					ImGui::Text("Delete selected config?");
					ImGui::Separator();
		
					if (ImGui::Button("Yes", ImVec2(120, 0)))
					{
						std::filesystem::remove(fullPath);
						RefreshConfigList(configFolder);
						GUI::selectedIndex = -1;
						GUI::openDeletePopup = false;
						ImGui::CloseCurrentPopup();
					}
		
					ImGui::SameLine();
		
					if (ImGui::Button("No", ImVec2(120, 0)))
					{
						GUI::openDeletePopup = false;
						ImGui::CloseCurrentPopup();
					}
		
					ImGui::EndPopup();
				}
			}
		}
	}
    
	ImGui::EndChild();
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
					Color color = Settings::Colors::blu_team;
					ImVec4 textColor(color.r()/255.0f, color.g()/255.0f, color.b()/255.0f, 255);
					ImGui::TextColored(textColor, "%s", name.c_str());
					break;
				}

				case TEAM_RED:
				{
					Color color = Settings::Colors::red_team;
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
				case TAB_CONFIG: DrawConfigTab(); break;
				default: break;
			}
			
			ImGui::EndTable();
		}
	}
	ImGui::End();
}

void GUI::Init()
{
	configs.reserve(5);
	GUI::selectedIndex = -1;
	GUI::newConfigName = "";
	GUI::firstOpenConfigTab = true;
	GUI::openDeletePopup = false;
}