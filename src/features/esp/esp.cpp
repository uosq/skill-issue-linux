#include "esp.h"

#include <vector>

#include "../../imgui/imgui.h"

#include "../../sdk/FontManager/fontmanager.h"
#include "../../sdk/definitions/isurface.h"
#include "../../sdk/classes/player.h"
#include "../../sdk/classes/cbaseobject.h"
#include "../../sdk/classes/ctfrobotdestruction_robot.h"
#include "../../sdk/definitions/icollideable.h"

#include "esp_utils.h"

#include "../entitylist/entitylist.h"
#include "../visuals/thirdperson/thirdperson.h"

enum class TextSide
{
	LEFT = 0, RIGHT,
	TOP, BOTTOM
};

struct ESPData
{
	float x, y, w, h;
	Color color;
	int entindex = -1;
	bool valid = false;

	float pad_left = 0.0f;
	float pad_right = 0.0f;
	float pad_bottom = 0.0f;

	float top_y = 0.0f;
	float bottom_y = 0.0f;
	float left_y = 0.0f;
	float right_y = 0.0f;
};

static std::vector<ESPData> s_vData;

void ESP::Init()
{

}

void ESP::OnlevelInitPostEntity()
{
	Reset();
	s_vData.reserve(interfaces::Engine->GetMaxClients());
}

static bool GetEntityBounds(CBaseEntity* pEntity, ESPData& data)
{
	int entindex = pEntity->GetIndex();
	if (entindex < 0) return false;

	Vec3 vecOrigin = pEntity->GetAbsOrigin();
	Vec3 vecMaxs = pEntity->GetCollideable()->OBBMaxs();
	Vec3 vecTop = vecOrigin; vecTop.z += vecMaxs.z;

	Vec2 screenOrigin, screenTop;

	if (!helper::engine::WorldToScreen(vecOrigin, screenOrigin)
	|| !helper::engine::WorldToScreen(vecTop, screenTop))
		return false;

	float h = screenOrigin.y - screenTop.y;
	if (h < 0) h = -h;
	float w = pEntity->IsTeleporter() ? (h * 2.0f) : (h * 0.5f); // teleporter has fucked bounding box

	// center it
	data.color = ESP_Utils::GetEntityColor(pEntity);
	data.entindex = entindex;
	data.x = screenOrigin.x - (w / 2.0f);
	data.y = screenTop.y;
	data.w = w;
	data.h = h;
	data.valid = true;

	return true;
}

static void CalcHealthBarLayout(ESPData& data)
{
	HealthMode mode = static_cast<HealthMode>(Settings::ESP.health);
	if (mode == HealthMode::BAR || mode == HealthMode::BOTH)
	{
		constexpr float BAR_WIDTH = 5.0f;
		constexpr float GAP = 3.0f;
		constexpr float MARGIN = 1.0f;

		data.pad_left += (BAR_WIDTH + GAP + (MARGIN * 2.0f));
	}
}

static void FillTargets(CTFPlayer* pLocal)
{
	constexpr int iVALID_FLAGS = EntityFlags::IsAlive | EntityFlags::IsBuilding | EntityFlags::IsPlayer;

	s_vData.clear();

	int localIndex = pLocal->GetIndex();

	for (const auto& entry : EntityList::GetEntities())
	{
		if (entry.ptr == nullptr)
			continue;

		if (!(entry.flags & iVALID_FLAGS))
			continue;

		if (!ESP_Utils::IsValidEntity(pLocal, entry))
			continue;

		if (entry.ptr->GetIndex() == localIndex
		&& !Thirdperson::IsThirdPerson(pLocal))
			continue;

		ESPData data;
		if (!GetEntityBounds(entry.ptr, data))
			continue;

		CalcHealthBarLayout(data);

		s_vData.emplace_back(data);
	}
}

static int GetEntityHealth(CBaseEntity* pEntity)
{
	if (pEntity->IsPlayer())
	{
		CTFPlayer* pPlayer = static_cast<CTFPlayer*>(pEntity);
		return pPlayer->GetHealth();
	}

	if (pEntity->IsBuilding())
	{
		CBaseObject* pObj = static_cast<CBaseObject*>(pEntity);
		return pObj->m_iHealth();
	}

	if (pEntity->IsRobot())
	{
		CTFRobotDestruction_Robot* pRobot = static_cast<CTFRobotDestruction_Robot*>(pEntity);
		return pRobot->m_iHealth();
	}

	return -1;
}

static int GetEntityMaxHealth(CBaseEntity* pEntity)
{
	if (pEntity->IsPlayer())
	{
		CTFPlayerResource* pResources = EntityList::GetPlayerResources();
		if (!pResources) return -1;

		return pResources->m_iMaxHealth(pEntity->GetIndex());
	}

	if (pEntity->IsBuilding())
	{
		CBaseObject* pObj = static_cast<CBaseObject*>(pEntity);
		return pObj->m_iMaxHealth();
	}

	if (pEntity->IsRobot())
	{
		CTFRobotDestruction_Robot* pRobot = static_cast<CTFRobotDestruction_Robot*>(pEntity);
		return pRobot->m_iMaxHealth();
	}

	return -1;
}

static void DrawImGuiBox(ImDrawList* pDraw, float x, float y, float w, float h, ImU32 color, float rounding = 0.0f, ImDrawFlags flags = 0, float thickness = 1.0f)
{
	ImVec2 min = ImVec2(x, y);
    	ImVec2 max = ImVec2(x + w, y + h);

	pDraw->AddRect(min, max, color, rounding, flags, thickness);
}

static void DrawImGuiBoxFilled(ImDrawList* pDraw, float x, float y, float w, float h, ImU32 color, float rounding = 0.0f, ImDrawFlags flags = 0)
{
	ImVec2 min = ImVec2(x, y);
    	ImVec2 max = ImVec2(x + w, y + h);

	pDraw->AddRectFilled(min, max, color, rounding, flags);
}

static void DrawBox(ImDrawList* pDraw, const ESPData& data)
{
	if (pDraw == nullptr)
		return;

	ImU32 iColor = IM_COL32
	(
		data.color.r(),
		data.color.g(),
		data.color.b(),
		255 // default is 0
	);

	DrawImGuiBox(pDraw, data.x, data.y, data.w, data.h, iColor, 2.0f);
}

static float GetHealthRatio(int health, int maxhealth)
{
	return std::clamp(static_cast<float>(health) / maxhealth, 0.0f, 1.0f);
}

static void GetHealthColor(int health, int maxhealth, int& r, int& g)
{
	float ratio = GetHealthRatio(health, maxhealth);
	r = static_cast<int>((1.0f - ratio) * 255.0f);
	g = static_cast<int>(ratio * 255.0f);
}

static void DrawHealthbar(ImDrawList* pDraw, CBaseEntity* pTarget, ESPData& data)
{
	constexpr int iGAP = 3;
	constexpr int iWIDTH = 5;
	constexpr int iMARGIN = 1;
	constexpr float flROUNDING = 2.0f;

	HealthMode mode = static_cast<HealthMode>(Settings::ESP.health);
	if (mode >= HealthMode::MAX || mode <= HealthMode::INVALID)
		return;

	if (mode != HealthMode::BAR && mode != HealthMode::BOTH)
		return;

	int iHealth = GetEntityHealth(pTarget);
	if (iHealth < 0) return;

	int iMaxHealth = GetEntityMaxHealth(pTarget);
	if (iMaxHealth < 0) return;

	// background
	{
		constexpr ImU32 iBACKGROUND = IM_COL32(20, 20, 20, 255);

		float x = data.x - iWIDTH - iGAP - iMARGIN;
		float y = data.y - iMARGIN;

		DrawImGuiBoxFilled(pDraw, x, y, iWIDTH + (iMARGIN * 2.0f), data.h + (iMARGIN * 2.0f), iBACKGROUND, flROUNDING);
	}

	// bar
	{
		float flHealthRatio = GetHealthRatio(iHealth, iMaxHealth);
		float x = data.x - iWIDTH - iGAP;
		float y = data.y;

		int r = 0, g = 0;
		GetHealthColor(iHealth, iMaxHealth, r, g);

		ImU32 iColor = IM_COL32(r, g, 100, 255);

		float barHeight = data.h * flHealthRatio;
		float yOffset = y + (data.h - barHeight); 

		DrawImGuiBoxFilled(pDraw, x, yOffset, iWIDTH, barHeight, iColor, flROUNDING);
	}
}

void ESP::Reset()
{
	s_vData.clear();
}

int ESP::GetFont()
{
	switch (static_cast<ESPFont>(Settings::ESP.font))
	{
		case ESPFont::TF2BUILD:	return FontManager::GetFont("esp font tf2");
		case ESPFont::ARIAL:	return FontManager::GetFont("esp font arial");
		default: break;
	}

	return FontManager::GetFont("esp font tf2");
}

void ESP::OnLevelShutdown()
{
	Reset();
}

static std::string GetEntityName(CBaseEntity* pEntity)
{
	if (pEntity == nullptr) return "Unknown";

	if (pEntity->IsPlayer())
	{
		CTFPlayer* pPlayer = static_cast<CTFPlayer*>(pEntity);
		return pPlayer->GetName();
	}

	if (pEntity->IsDispenser())	return "Dispenser";
	if (pEntity->IsTeleporter())	return "Teleporter";
	if (pEntity->IsSentry())	return "Sentrygun";

	const char* networkName = pEntity->GetClientClass()->networkName;
	return networkName ? networkName : "Unknown";
}

static void DrawText(ImDrawList* pDraw, const std::string& text, ESPData& data, TextSide side, Color textColor = Color(255, 255, 255, 255))
{
	constexpr float iPADDING = 2.0f;
	if (text.length() == 0 || text.empty()) return;

	//helper::draw::GetTextSize(text, textsize.x, textsize.y);
	ImVec2 textsize = ImGui::CalcTextSize(text.c_str());

	float draw_x = 0.0f;
	float draw_y = 0.0f;

	switch(side)
	{
		case TextSide::TOP:
			draw_x = data.x + (data.w / 2.0f) - (textsize.x / 2.0f);
			draw_y = data.y - textsize.y - data.top_y - iPADDING;
			data.top_y += textsize.y;
			break;

		case TextSide::BOTTOM:
			draw_x = data.x + (data.w / 2.0f) - (textsize.x / 2.0f);
			draw_y = data.y + data.h + data.bottom_y + iPADDING;
			data.bottom_y += textsize.y;
			break;

		case TextSide::LEFT:
			draw_x = data.x - textsize.x - data.pad_left - iPADDING;
			draw_y = data.y + data.left_y;
			data.left_y += textsize.y;
			break;

		case TextSide::RIGHT:
			draw_x = data.x + data.w + data.pad_right + iPADDING;
			draw_y = data.y + data.right_y;
			data.right_y += textsize.y;
			break;
	}

	// shadow
	pDraw->AddText(ImVec2(draw_x + 1, draw_y + 1), IM_COL32(0, 0,0, 255), text.c_str());

	ImU32 color = IM_COL32(textColor.r(), textColor.g(), textColor.b(), 255);
	pDraw->AddText(ImVec2(draw_x, draw_y), color, text.c_str());
}

static void DrawClass(ImDrawList* pDraw, CBaseEntity* pTarget, ESPData& data)
{
	if (pTarget == nullptr) return;
	if (!Settings::ESP.class_name) return;

	const char* className = pTarget->GetClientClass()->networkName;
	if (className == nullptr) return;

	DrawText(pDraw, className, data, TextSide::BOTTOM);
}

static void DrawPlayerConditions(ImDrawList* pDraw, CBaseEntity* pTarget, ESPData& data)
{
	if (pTarget == nullptr || !pTarget->IsPlayer()) return;

	CTFPlayer* pPlayer = static_cast<CTFPlayer*>(pTarget);

	int conds = Settings::ESP.fconditions;
	if (conds == 0) return; // never liked the implicit "if (!conds)"

	if ((conds & (int)ESPConditionFlags::Jarated) && pPlayer->InCond(TF_COND_URINE))
		DrawText(pDraw, "Jarate", data, TextSide::RIGHT, Color(255, 200, 0, 255));

	if ((conds & (int)ESPConditionFlags::Bonked) && pPlayer->InCond(TF_COND_BONKED))
		DrawText(pDraw, "Bonk", data, TextSide::RIGHT);

	if ((conds & (int)ESPConditionFlags::Ubered) && pPlayer->InCond(TF_COND_INVULNERABLE))
		DrawText(pDraw, "Uber", data, TextSide::RIGHT, Color(255, 100, 100, 255));

	if ((conds & (int)ESPConditionFlags::Zoomed) && pPlayer->InCond(TF_COND_ZOOMED))
		DrawText(pDraw, "Zoom", data, TextSide::RIGHT);
}

static void DrawHealthText(ImDrawList* pDraw, CBaseEntity* pTarget, ESPData& data)
{
	HealthMode mode = static_cast<HealthMode>(Settings::ESP.health);
	if (mode >= HealthMode::MAX || mode <= HealthMode::INVALID)
		return;

	if (mode != HealthMode::TEXT && mode != HealthMode::BOTH)
		return;

	int iHealth = GetEntityHealth(pTarget);
	if (iHealth < 0) return;

	int iMaxHealth = GetEntityMaxHealth(pTarget);
	if (iMaxHealth < 0) return;

	std::string text = std::to_string(iHealth) + "/" + std::to_string(iMaxHealth);

	int r = 0, g = 0;
	GetHealthColor(iHealth, iMaxHealth, r, g);
	Color color(r, g, 100, 255);

	DrawText(pDraw, text, data, TextSide::BOTTOM, color);
}

void ESP::OnImGui()
{
	CTFPlayer* pLocal = EntityList::GetLocal();
	if (pLocal == nullptr)
		return;

	if (!interfaces::Engine->IsInGame())
		return;

	if (!interfaces::Engine->IsConnected())
		return;

	ImDrawList* pDraw = ImGui::GetBackgroundDrawList();
	if (pDraw == nullptr)
		return;

	if (s_vData.empty())
		return;

	for (ESPData data : s_vData)
	{
		if (!data.valid) continue;

		CBaseEntity* pTarget = static_cast<CBaseEntity*>(interfaces::EntityList->GetClientEntity(data.entindex));
		if (pTarget == nullptr) continue;

		if (Settings::ESP.box) DrawBox(pDraw, data);
		if (static_cast<HealthMode>(Settings::ESP.health) != HealthMode::NONE)
		{
			DrawHealthbar(pDraw, pTarget, data);
			DrawHealthText(pDraw, pTarget, data);
		}

		if (Settings::ESP.name)
			DrawText(pDraw, GetEntityName(pTarget), data, TextSide::TOP, data.color);

		DrawPlayerConditions(pDraw, pTarget, data);
		DrawClass(pDraw, pTarget, data);
	}
}

void ESP::OnFrameStageNotify(int stage)
{
	if (stage != FRAME_RENDER_START)
		return;

	CTFPlayer* pLocal = EntityList::GetLocal();
	if (pLocal == nullptr)
		return;

	FillTargets(pLocal);
}