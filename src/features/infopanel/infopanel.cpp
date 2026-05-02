#include "infopanel.h"

#include "../../imgui/imgui.h"
#include "../../settings/settings.h"

#include "../aimbot/aimbot.h"
#include "../spectators/spectators.h"
#include "../warp/warp.h"
#include "../radar/radar.h"

static void DrawColoredText(const char* text, bool state)
{
	ImVec4 col = state ? ImVec4(100, 255, 100, 255) : ImVec4(255, 255, 255, 255);
	ImGui::TextColored(col, "%s: %s", text, state ? "ON" : "OFF");
}

template <typename... text>
static void DrawColoredTextEx(const char* fmt, text... txt)
{
	ImGui::TextColored(ImVec4(255, 255, 255, 255), fmt, txt...);
}

void InfoPanel::OnImGui(bool bMenuOpen)
{
	if (!Config.misc.packed.infopanel)
		return;

	int flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse;
	if (!bMenuOpen)
		flags |= ImGuiWindowFlags_NoMove;

	if (ImGui::Begin("Information Panel", nullptr, flags))
	{
		DrawColoredText("Aimbot", Aimbot::IsRunning());
		DrawColoredText("ESP", Config.esp.packed.enabled);
		DrawColoredText("Chams", Config.chams.enabled);
		DrawColoredText("Autostrafe", Config.misc.packed.autostrafe);
		DrawColoredText("Glow", Config.glow.packed.blur > 0);

		int amount = 0;
		DrawColoredText("Spectated", Spectators::IsLocalPlayerSpectated(amount));
		DrawColoredTextEx("Spectator Count: %i", amount);

		DrawColoredText("Warp", Config.warp.key->IsEnabled());
		DrawColoredTextEx("Warp Amount: %i", Warp::m_iShiftAmount);
		DrawColoredTextEx("Warp Ticks: %i", Warp::m_iStoredTicks);

		DrawColoredText("Radar", Config.radar.packed.enabled);
		DrawColoredTextEx("Radar Range: %i", Radar::m_iRange);
		DrawColoredTextEx("Radar Radius: %.3f", Radar::m_flRadius);
	}
	ImGui::End();
}