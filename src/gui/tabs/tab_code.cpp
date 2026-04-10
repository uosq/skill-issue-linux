#include "tab_code.h"

#include "../../imgui/imgui.h"
#include "../../imgui/imgui_stdlib.h"

#include "../utils/string_utils.h"

#include "../../features/scriptmanager/scriptmanager.h"
#include "../../features/angelscript/api/globals.h"

#include "../../sdk/interfaces/interfaces.h"

#define SCRIPT_DIR "./skill-issue/scripts/"

static std::string s_SearchString = "";
static bool s_bFirstTimeOpen = true;

struct AccessOption
{
	const char *label;
	int flag;
};

void DrawCodeTab()
{
	auto& vScripts = ScriptManager::GetAvailableAngelScripts();

	if (s_bFirstTimeOpen)
	{
		ScriptManager::RefreshAngelScripts();
		s_bFirstTimeOpen = false;
	}

	ImGui::BeginGroup();

	if (ImGui::BeginTabBar("LuaTab"))
	{
		if (ImGui::BeginTabItem("Scripts"))
		{
			if (ImGui::Button("Refresh"))
				ScriptManager::RefreshAngelScripts();

			ImGui::SameLine();

			ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
			ImGui::InputText("##Search Files", &s_SearchString);

			bool bIsSearchEmpty = s_SearchString.empty();

			if (!vScripts.empty())
			{
				if (ImGui::BeginChild("##ScriptList"))
				{
					int columns = std::max(1, (int)(ImGui::GetContentRegionAvail().x / 250.0f));

					if (ImGui::BeginTable("##ScriptTable", columns))
					{
						for (int i = 0; i < vScripts.size(); i++)
						{
							const std::string &file = vScripts[i];

							if (!bIsSearchEmpty && !ContainsInsensitive(file, s_SearchString))
								continue;

							ImGui::TableNextColumn();

							std::string fullPath = std::string(SCRIPT_DIR) + "/" + file;

							Script &script = ScriptManager::GetOrCreate(fullPath);

							bool wasLoaded = script.loaded;

							if (ImGui::Checkbox(file.c_str(), &wasLoaded))
							{
								if (wasLoaded)
									ScriptManager::Load(script);
								else
									ScriptManager::Unload(script);
							}
						}

						ImGui::EndTable();
					}
					ImGui::EndChild();
				}
			}
			else
			{
				std::string dir = std::string(interfaces::Engine->GetGameDirectory()) + SCRIPT_DIR;
				ImGui::Text("Add scripts in %s", dir.c_str());
			}

			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Restrictions"))
		{
			static const AccessOption options[]{
			    {"Allow Engine Library", ScriptAccessMask::SCRIPT_MASK_ALLOW_ENGINE},
			    {"Allow Client Library", ScriptAccessMask::SCRIPT_MASK_ALLOW_CLIENT},
			    {"Allow ClientState Library", ScriptAccessMask::SCRIPT_MASK_ALLOW_CLIENTSTATE},
			    {"Allow Common Library", ScriptAccessMask::SCRIPT_MASK_ALLOW_COMMON},
			    {"Allow Draw Library", ScriptAccessMask::SCRIPT_MASK_ALLOW_DRAW},
			    {"Allow EntityList Library", ScriptAccessMask::SCRIPT_MASK_ALLOW_ENTITYLIST},
			    {"Allow Hooks Library", ScriptAccessMask::SCRIPT_MASK_ALLOW_HOOKS},
			    {"Allow ImGui Library", ScriptAccessMask::SCRIPT_MASK_ALLOW_IMGUI},
			    {"Allow Input Library", ScriptAccessMask::SCRIPT_MASK_ALLOW_INPUT},
			    {"Allow Materials Library", ScriptAccessMask::SCRIPT_MASK_ALLOW_MATERIALS},
			    {"Allow Render Library", ScriptAccessMask::SCRIPT_MASK_ALLOW_RENDER},
			    {"Allow FileSystem Library", ScriptAccessMask::SCRIPT_MASK_ALLOW_FILESYSTEM},
			    {"Allow Math Library", ScriptAccessMask::SCRIPT_MASK_ALLOW_MATH},
			    {"Allow Array Library", ScriptAccessMask::SCRIPT_MASK_ALLOW_ARRAY},
			    {"Allow Dictionary Library", ScriptAccessMask::SCRIPT_MASK_ALLOW_DICT},
			    {"Allow DateTime Library", ScriptAccessMask::SCRIPT_MASK_ALLOW_DATETIME},
			    {"Allow String Library", ScriptAccessMask::SCRIPT_MASK_ALLOW_STRING},
			    {"Allow GlobalVars Library", ScriptAccessMask::SCRIPT_MASK_ALLOW_GLOBALVARS},
			    {"Allow Binds Library", ScriptAccessMask::SCRIPT_MASK_ALLOW_BINDS},
			};

			int &mask = GetScriptAccessMask();

			int columns = std::max(1, (int)(ImGui::GetContentRegionAvail().x / 250.0f));

			if (ImGui::BeginTable("##fdgidfojg", columns))
			{
				for (int i = 0; i < IM_ARRAYSIZE(options); i++)
				{
					ImGui::TableNextColumn();
					ImGui::CheckboxFlags(options[i].label, &mask, options[i].flag);
				}

				ImGui::EndTable();
			}

			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}

	ImGui::EndGroup();
}