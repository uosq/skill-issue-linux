#include "tab_netvars.h"

#include "../../imgui/imgui.h"
#include "../../imgui/imgui_stdlib.h"

#include "../../sdk/netvars/netvar.h"

#include "../utils/string_utils.h"

static std::string s_Search = "";

void DrawParsedNetvarData(const std::vector<NetvarClassEntry> &classes)
{
	bool bIsSearchEmpty = s_Search.empty();

	for (const auto &cls : classes)
	{
		if (!bIsSearchEmpty && !ContainsInsensitive(cls.className, s_Search))
			continue;

		if (ImGui::TreeNode(cls.className.c_str()))
		{
			if (cls.members.empty())
				ImGui::TextDisabled("No members");
			else
				for (const auto &name : cls.members)
					ImGui::BulletText("%s", name.c_str());

			ImGui::TreePop();
		}
	}
}

void DrawNetvarsTab()
{
	float maxSize = ImGui::GetContentRegionAvail().x;

	ImGui::SetNextItemWidth(maxSize);
	ImGui::InputText("##SearchBar", &s_Search);

	if (ImGui::BeginChild("NetvarContent"))
		DrawParsedNetvarData(Netvars::m_netvarUIVector);

	ImGui::EndChild();
}