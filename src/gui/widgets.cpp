#include "widgets.h"

#include "../imgui/imgui.h"
#include "../settings/settings.h"

bool ImGui::SafeCheckbox(const char *text, bool* v)
{
	ImGui::BeginDisabled(Config.misc.packed.safemode);
	bool ret = ImGui::Checkbox(text, v);
	ImGui::EndDisabled();

	return ret;
}