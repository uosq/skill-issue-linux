#include "tab_code.h"

#include "../../imgui/imgui.h"
#include "../../imgui/imgui_stdlib.h"

#include "../utils/string_utils.h"

#include "../../features/scriptmanager/scriptmanager.h"

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
	const auto& vScripts = features::scriptmanager.GetAvailableScripts();

	if (s_bFirstTimeOpen)
	{
		features::scriptmanager.RefreshScripts();
		s_bFirstTimeOpen = false;
	}

	ImGui::BeginGroup();

	if (ImGui::Button("Refresh"))
		features::scriptmanager.RefreshScripts();

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

					Script &script = features::scriptmanager.GetOrCreate(fullPath);

					bool wasLoaded = script.loaded;

					if (ImGui::Checkbox(file.c_str(), &wasLoaded))
					{
						if (wasLoaded)
							features::scriptmanager.Load(script);
						else
							features::scriptmanager.Unload(script);
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

	ImGui::EndGroup();
}