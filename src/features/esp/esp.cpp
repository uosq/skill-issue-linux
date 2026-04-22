#include "esp.h"

#include <string>
#include <vector>

#include "../../imgui/imgui.h"

#include "../../sdk/definitions/isurface.h"
#include "../../sdk/classes/player.h"
#include "../../sdk/classes/cbaseobject.h"
#include "../../sdk/classes/ctfrobotdestruction_robot.h"
#include "../../sdk/definitions/icollideable.h"
#include "../../sdk/imgui_utils/imgui_utils.h"
#include "../../sdk/definitions/itemschema.h"

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
	float pad_top = 0.0f;

	float top_y = 0.0f;
	float bottom_y = 0.0f;
	float left_y = 0.0f;
	float right_y = 0.0f;

	float text_scale = 1.0f; // used to scale with distance

	void ResetOffsets();
};

void ESPData::ResetOffsets()
{
	top_y = 0.0f;
	bottom_y = 0.0f;
	left_y = 0.0f;
	right_y = 0.0f;
}

struct HealthbarBounds
{
	float x, y, w, h;
	float bar_x, bar_y, bar_w, bar_h;
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

static float GetHealthBarScale(const ESPData& data)
{
	return data.text_scale * 0.75f;
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

static bool GetHealthbarBounds(CBaseEntity* pTarget, ESPData& data, HealthbarBounds& out)
{
	HealthMode mode = static_cast<HealthMode>(Settings::ESP.health);
	if (mode != HealthMode::BAR && mode != HealthMode::BOTH)
		return false;

	int hp = GetEntityHealth(pTarget);
	int maxHp = GetEntityMaxHealth(pTarget);
	if (hp < 0 || maxHp <= 0)
		return false;

	TextSide side = static_cast<TextSide>(Settings::ESP.sides.healthbar);

	float gap = (float)Settings::ESP.custom.gap;
	float margin = (float)Settings::ESP.custom.healthbar_margin;
	float thickness = (float)Settings::ESP.custom.healthbar_thickness;
	float scale = std::max(GetHealthBarScale(data), 0.1f); // cant see shit with size 0

	float size = thickness * scale;

	if (side == TextSide::LEFT)
	{
		out.bar_x = data.x - gap - margin - size;
		out.bar_y = data.y;
		out.bar_w = size;
		out.bar_h = data.h;
	}
	else if (side == TextSide::RIGHT)
	{
		out.bar_x = data.x + data.w + gap + margin;
		out.bar_y = data.y;
		out.bar_w = size;
		out.bar_h = data.h;
	}
	else if (side == TextSide::TOP)
	{
		out.bar_x = data.x;
		out.bar_y = data.y - gap - margin - size;
		out.bar_w = data.w;
		out.bar_h = size;
	}
	else if (side == TextSide::BOTTOM)
	{
		out.bar_x = data.x;
		out.bar_y = data.y + data.h + gap + margin;
		out.bar_w = data.w;
		out.bar_h = size;
	}
	else
	{
		return false;
	}

	// background
	out.x = out.bar_x - margin;
	out.y = out.bar_y - margin;
	out.w = out.bar_w + (margin * 2.0f);
	out.h = out.bar_h + (margin * 2.0f);

	return true;
}

static void CalcHealthBarLayout(ESPData& data, CBaseEntity* pTarget)
{
	HealthbarBounds hb;
	if (!GetHealthbarBounds(pTarget, data, hb))
		return;

	TextSide side = static_cast<TextSide>(Settings::ESP.sides.healthbar);

	float totalPadding;

	if (side == TextSide::LEFT)
		totalPadding = data.x - hb.x;
	else if (side == TextSide::RIGHT)
		totalPadding = (hb.x + hb.w) - (data.x + data.w);
	else if (side == TextSide::TOP)
		totalPadding = data.y - hb.y;
	else if (side == TextSide::BOTTOM)
		totalPadding = (hb.y + hb.h) - (data.y + data.h);
	else
		return;

	if (side == TextSide::LEFT)
		data.pad_left += totalPadding;
	else if (side == TextSide::RIGHT)
		data.pad_right += totalPadding;
	else if (side == TextSide::TOP)
		data.pad_top += totalPadding;
	else if (side == TextSide::BOTTOM)
		data.pad_bottom += totalPadding;
}

static float GetTextScale(float distance)
{
	constexpr float MAX_DIST = 1000.0f;
	constexpr float MIN_SCALE = 0.6f; // can't see shit if its too small
	constexpr float MAX_SCALE = 1.2f;

	float t = std::clamp(distance / MAX_DIST, 0.0f, 1.0f);
	return MAX_SCALE + (MIN_SCALE - MAX_SCALE) * t;
}

static void FillTargets(CTFPlayer* pLocal)
{
	constexpr int iVALID_FLAGS = EntityFlags::IsAlive | EntityFlags::IsBuilding | EntityFlags::IsPlayer;

	s_vData.clear();

	int localIndex = pLocal->GetIndex();
	Vec3 localOrigin = pLocal->GetAbsOrigin();

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

		data.text_scale = GetTextScale(localOrigin.DistTo(entry.ptr->GetAbsOrigin()));

		CalcHealthBarLayout(data, entry.ptr);

		s_vData.emplace_back(data);
	}
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

	float rounding = (float)Settings::ESP.custom.box_rounding;
	DrawImGuiBox(pDraw, data.x, data.y, data.w, data.h, iColor, rounding);
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
	int iHealth = GetEntityHealth(pTarget);
	if (iHealth < 0) return;

	int iMaxHealth = GetEntityMaxHealth(pTarget);
	if (iMaxHealth < 0) return;

	HealthbarBounds hb;
	if (!GetHealthbarBounds(pTarget, data, hb))
		return;

	// background
	DrawImGuiBoxFilled(
		pDraw,
		hb.x, hb.y,
		hb.w, hb.h,
		IM_COL32(20, 20, 20, 255),
		(float)Settings::ESP.custom.healthbar_rounding
	);

	// bar
	float ratio = GetHealthRatio(iHealth, iMaxHealth);

	int r = 0, g = 0;
	GetHealthColor(iHealth, iMaxHealth, r, g);

	if (hb.bar_h > hb.bar_w) // vertical
	{
		float filled = hb.bar_h * ratio;
		float y = hb.bar_y + (hb.bar_h - filled);

		DrawImGuiBoxFilled
		(
			pDraw, hb.bar_x, y, hb.bar_w, filled,
			IM_COL32(r, g, 100, 255),
			Settings::ESP.custom.healthbar_rounding);
	}
	else // horizontal
	{
		float filled = hb.bar_w * ratio;

		DrawImGuiBoxFilled(pDraw, hb.bar_x, hb.bar_y, filled, hb.bar_h,
			IM_COL32(r, g, 100, 255),
			Settings::ESP.custom.healthbar_rounding);
	}
}

void ESP::Reset()
{
	s_vData.clear();
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
	if (text.length() == 0 || text.empty()) return;

	ImVec2 text_size = ImGui::CalcTextSize(text.c_str());
	text_size.x *= data.text_scale;
	text_size.y *= data.text_scale;

	float padding = (float)Settings::ESP.custom.text_padding;
	float draw_x = 0.0f;
	float draw_y = 0.0f;

	switch(side)
	{
		case TextSide::TOP:
			draw_x = data.x + (data.w / 2.0f) - (text_size.x / 2.0f);
			draw_y = data.y - text_size.y - data.top_y - data.pad_top - padding;
			data.top_y += text_size.y;
			break;

		case TextSide::BOTTOM:
			draw_x = data.x + (data.w / 2.0f) - (text_size.x / 2.0f);
			draw_y = data.y + data.h + data.bottom_y + data.pad_bottom + padding;
			data.bottom_y += text_size.y;
			break;

		case TextSide::LEFT:
			draw_x = data.x - text_size.x - data.pad_left - padding;
			draw_y = data.y + data.left_y;
			data.left_y += text_size.y;
			break;

		case TextSide::RIGHT:
			draw_x = data.x + data.w + data.pad_right + padding;
			draw_y = data.y + data.right_y;
			data.right_y += text_size.y;
			break;
	}

	// shadow
	ImU32 color = IM_COL32(textColor.r(), textColor.g(), textColor.b(), 255);
	ImGui::DrawTextShadow(pDraw, ImGui::GetFont(), text_size.y, ImVec2(draw_x, draw_y), color, text.c_str());
}

static void DrawClass(ImDrawList* pDraw, CBaseEntity* pTarget, ESPData& data)
{
	if (pTarget == nullptr) return;
	if (!Settings::ESP.class_name) return;

	const char* className = pTarget->GetClientClass()->networkName;
	if (className == nullptr) return; // no class? wtf

	if (pTarget->IsPlayer())
	{
		static const char* classes[]
		{
			"Scout", "Sniper",
			"Soldier", "Demoman",
			"Medic", "Heavy",
			"Pyro", "Spy",
			"Engineer",
		};

		CTFPlayer* pPlayer = static_cast<CTFPlayer*>(pTarget);
		int classIdx = pPlayer->m_iClass();

		if (classIdx <= ETFClass::TF_CLASS_UNDEFINED || classIdx >= TF_CLASS_CIVILIAN)
			return; // wtf??

		// m_iClass starts at 1 (0 is undefined)
		className = classes[pPlayer->m_iClass() - 1];
	}

	TextSide side = static_cast<TextSide>(Settings::ESP.sides.classname);
	DrawText(pDraw, className, data, side);
}

static void DrawPlayerConditions(ImDrawList* pDraw, CBaseEntity* pTarget, ESPData& data)
{
	if (pTarget == nullptr || !pTarget->IsPlayer()) return;

	CTFPlayer* pPlayer = static_cast<CTFPlayer*>(pTarget);

	int conds = Settings::ESP.fconditions;
	if (conds == 0) return; 

	if ((conds & (int)ESPConditionFlags::Jarated) && pPlayer->InCond(TF_COND_URINE))
		DrawText(pDraw, "Jarate", data, static_cast<TextSide>(Settings::ESP.sides.jarate), Color(255, 200, 0, 255));

	if ((conds & (int)ESPConditionFlags::Bonked) && pPlayer->InCond(TF_COND_BONKED))
		DrawText(pDraw, "Bonk", data, static_cast<TextSide>(Settings::ESP.sides.bonk));

	if ((conds & (int)ESPConditionFlags::Ubered) && pPlayer->InCond(TF_COND_INVULNERABLE))
		DrawText(pDraw, "Uber", data, static_cast<TextSide>(Settings::ESP.sides.uber), Color(255, 100, 100, 255));

	if ((conds & (int)ESPConditionFlags::Zoomed) && pPlayer->InCond(TF_COND_ZOOMED))
		DrawText(pDraw, "Zoom", data, static_cast<TextSide>(Settings::ESP.sides.zoom));
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

	TextSide side = static_cast<TextSide>(Settings::ESP.sides.healthbar);
	DrawText(pDraw, text, data, side, color);
}

static void DrawWeapon(ImDrawList* pDraw, CBaseEntity* pTarget, ESPData& data)
{
	if (pTarget == nullptr || !pTarget->IsPlayer())
		return;

	CTFPlayer* pPlayer = static_cast<CTFPlayer*>(pTarget);
	CTFWeaponBase* pWeapon = HandleAs<CTFWeaponBase*>(pPlayer->GetActiveWeapon());

	if (pWeapon == nullptr)
		return;

	int iItemDefinitionIndex = pWeapon->m_iItemDefinitionIndex();

	const char* baseName = ItemDefinition_GetBaseName(iItemDefinitionIndex);
	if (baseName == nullptr || baseName[0] == '\0')
		return;

	const char* localizedName = interfaces::VGuiLocalize->FindAsUTF8(baseName);
	if (localizedName == nullptr || localizedName[0] == '\0')
		return;

	TextSide side = static_cast<TextSide>(Settings::ESP.sides.weaponname);
	DrawText(pDraw, localizedName, data, side);
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

	for (ESPData& data : s_vData)
	{
		if (!data.valid) continue;

		// fix not working with references
		data.ResetOffsets();

		CBaseEntity* pTarget = static_cast<CBaseEntity*>(interfaces::EntityList->GetClientEntity(data.entindex));
		if (pTarget == nullptr) continue;

		if (Settings::ESP.box) DrawBox(pDraw, data);
		if (static_cast<HealthMode>(Settings::ESP.health) != HealthMode::NONE)
		{
			DrawHealthbar(pDraw, pTarget, data);
			DrawHealthText(pDraw, pTarget, data);
		}

		if (Settings::ESP.name)
		{
			TextSide side = static_cast<TextSide>(Settings::ESP.sides.name);
			DrawText(pDraw, GetEntityName(pTarget), data, side, data.color);
		}

		DrawPlayerConditions(pDraw, pTarget, data);
		DrawClass(pDraw, pTarget, data);
		DrawWeapon(pDraw, pTarget, data);
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