#include "esp.h"
#include <immintrin.h>

struct ESP_Data
{
	std::string name;
	int health = 0;
	int maxhealth = 0;
	int buffhealth = 0;

	ESP_Data()
	{
		name = "";
		health = 0;
		maxhealth = 0;
		buffhealth = 0;
	}
};

bool GetESPData(const EntityListEntry& entry, ESP_Data& out)
{
	if (entry.ptr == nullptr)
		return false;

	CBaseEntity* ent = entry.ptr;

	if (entry.flags & EntityFlags::IsPlayer)
	{
		if (ent == EntityList::GetLocal() && !Settings::Misc::thirdperson)
			return false;

		auto* p = static_cast<CTFPlayer*>(ent);
		out.name = p->GetName();
		out.health = p->GetHealth();

		if (auto* res = EntityList::GetPlayerResources())
		{
			int index = ent->GetIndex();
			out.maxhealth = res->m_iMaxHealth(index);
			out.buffhealth = res->m_iMaxBuffedHealth(index);
		}

		return true;
	}

	if (entry.flags & EntityFlags::IsBuilding)
	{
		auto* b = static_cast<CBaseObject*>(ent);
		out.name = b->GetName();
		out.health = b->m_iHealth();
		out.maxhealth = b->m_iMaxHealth();
		out.buffhealth = 0;
		return true;
	}

	out.name = ent->GetClientClass()->networkName;
	return true;
}

namespace ESP
{
	void Init() { FontManager::CreateFont("esp font", "Arial", 16, 400, EFONTFLAG_CUSTOM | EFONTFLAG_ANTIALIAS); }

	Color GetEntityColor(CBaseEntity* entity)
	{
		if (entity == EntityList::m_pAimbotTarget)
			return Settings::Colors::aimbot_target;

		switch (entity->m_iTeamNum())
		{
			case ETeam::TEAM_RED:
				return Settings::Colors::red_team;
			case ETeam::TEAM_BLU:
				return Settings::Colors::blu_team;
			default: break;
		}

		Color defaultColor = {255, 255, 255, 255};
		return defaultColor;
	}

	bool IsValidPlayer(CTFPlayer* pLocal, CBaseEntity* entity)
	{
		if (entity == nullptr)
			return false;

		if (!entity->IsPlayer())
			return false;

		if (!interfaces::CInput->CAM_IsThirdPerson() && entity->GetIndex() == pLocal->GetIndex())
			return false;

		CTFPlayer* player = static_cast<CTFPlayer*>(entity);

		if (!player->IsAlive())
			return false;

		if (player->InCond(TF_COND_CLOAKED) && Settings::ESP::ignorecloaked)
			return false;

		return true;
	}

	bool IsValidBuilding(CTFPlayer* pLocal, CBaseObject* entity)
	{
		if (entity == nullptr)
			return false;

		if (entity->IsDormant())
			return false;

		return true;
	}

	bool IsValidEntity(CTFPlayer* pLocal, const EntityListEntry& entry)
	{
		if (!entry.ptr)
			return false;

		if (entry.flags & EntityFlags::IsPlayer)
			return IsValidPlayer(pLocal, static_cast<CTFPlayer*>(entry.ptr));

		if (entry.flags & EntityFlags::IsBuilding)
			return IsValidBuilding(pLocal, static_cast<CBaseObject*>(entry.ptr));

		return false;
	}

	bool GetEntityBounds(CBaseEntity* ent, Vector& top, Vector& bottom, int& w, int& h)
	{
		Vector origin = ent->GetAbsOrigin();

		if (!helper::engine::WorldToScreen(origin, bottom))
			return false;

		Vector absHead = origin + Vector{0, 0, ent->m_vecMaxs().z};
		if (!helper::engine::WorldToScreen(absHead, top))
			return false;

		h = (bottom - top).Length2D();
		w = ent->IsTeleporter() ? (h * 2.0f) : (h * 0.3f);

		return true;
	}

	void PaintBox(Color color, const Vector& top, const Vector& bottom, int w, int h)
	{
		helper::draw::SetColor(color);
		helper::draw::OutlinedRect(top.x - w, bottom.y - h, bottom.x + w, bottom.y);
	}

	void PaintName(Color color, const Vector& top, int w, int h, const std::string& name)
	{
		int textw, texth;
		helper::draw::GetTextSize(name, textw, texth);
		helper::draw::TextShadow(top.x - (textw*0.5f), top.y - texth - 2, color, name);
	}

	void PaintHealthbar(const Vector &top, const Vector &bottom, int w, int h, int health, int maxhealth, int buffhealth)
	{
		const Color background = {20, 20, 20, 255};
		const Color bar = {100, 255, 100, 255};
		
		constexpr int width = 5;
		constexpr int gap = 3;
		constexpr int barMargin = 1;
		
		const int barRight = top.x - w - gap;
		const int barLeft = barRight - width;
		
		const int x0 = barLeft;
		const int y0 = top.y;
		const int x1 = barRight;
		const int y1 = bottom.y;

		const float healthRatio = std::clamp(static_cast<float>(health)/maxhealth, 0.0f, 1.0f);

		// draw background
		interfaces::Surface->DrawSetColor(background);
		interfaces::Surface->DrawFilledRect(x0 - barMargin, y0 - barMargin, x1 + barMargin, y1 + barMargin);

		// draw health bar
		interfaces::Surface->DrawSetColor(bar);
		interfaces::Surface->DrawFilledRect(x0, y1 - static_cast<int>(h * healthRatio), x1, y1);
	}

	void Run(CTFPlayer* pLocal)
	{
		if (!helper::engine::IsInMatch() || !Settings::ESP::enabled)
			return;

		FontManager::SetFont("esp font");

		for (const auto& entry : EntityList::GetEntities())
		{
			if (!IsValidEntity(pLocal, entry))
				continue;

			CBaseEntity* ent = entry.ptr;

			Vector top, bottom;
			int w, h;
			if (!GetEntityBounds(ent, top, bottom, w, h))
				continue;

			Color color = GetEntityColor(ent);

			if (Settings::ESP::box)
				PaintBox(color, top, bottom, w, h);

			ESP_Data data = {};
			if (!GetESPData(entry, data))
				continue;

			if (Settings::ESP::name)
				PaintName(color, top, w, h, data.name);

			if (Settings::ESP::healthbar && data.maxhealth > 0)
				PaintHealthbar(top, bottom, w, h, data.health, data.maxhealth, data.buffhealth);
		}
	}

};