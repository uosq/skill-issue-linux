#include "esp.h"
#include "elements/BaseElement.h"
#include "elements/HealthTextElement.h"
#include "elements/LuaElement.h"
#include "elements/NameElement.h"
#include "structs.h"

namespace ESP
{
	std::vector<std::unique_ptr<IBaseElement>> m_builtinElements = {};
	std::vector<std::unique_ptr<LuaElement>> m_luaElements = {};

	void Init()
	{
		FontManager::CreateFont("esp font", "Arial", 16, 400, EFONTFLAG_CUSTOM | EFONTFLAG_ANTIALIAS);

		m_builtinElements.reserve(2);
		m_builtinElements.push_back(std::make_unique<NameElement>());
		m_builtinElements.push_back(std::make_unique<HealthTextElement>());

		m_luaElements.reserve(5);
	}

	bool GetData(const EntityListEntry& entry, ESP_Data& out)
	{
		if (entry.ptr == nullptr)
			return false;

		CBaseEntity* ent = entry.ptr;

		if (entry.flags & EntityFlags::IsPlayer)
		{
			if (ent == EntityList::GetLocal() && !Settings::Misc::thirdperson)
				return false;

			if (!ESP_Utils::GetEntityBounds(ent, out))
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
			if (!ESP_Utils::GetEntityBounds(ent, out))
				return false;

			auto* b = static_cast<CBaseObject*>(ent);
			out.name = b->GetName();
			out.health = b->m_iHealth();
			out.maxhealth = b->m_iMaxHealth();
			out.buffhealth = 0;
			return true;
		}

		out.name = ent->GetClientClass()->networkName != nullptr ? ent->GetClientClass()->networkName : "unknown";
		return false;
	}

	void PaintBox(Color color, const ESP_Data& data)
	{
		helper::draw::SetColor(color);
		helper::draw::OutlinedRect(data.top.x - data.width, data.bottom.y - data.height, data.bottom.x + data.width, data.bottom.y);
	}

	void PaintName(Color color, const Vector& top, int w, int h, const std::string& name)
	{
		int textw, texth;
		helper::draw::GetTextSize(name, textw, texth);
		helper::draw::TextShadow(top.x - (textw*0.5f), top.y - texth - 2, color, name);
	}

	void PaintHealthbar(const ESP_Data& data)
	{
		const Color background = {20, 20, 20, 255};
		const Color bar = {100, 255, 100, 255};
		
		constexpr int width = 5;
		constexpr int gap = 3;
		constexpr int barMargin = 1;
		
		const int barRight = data.top.x - data.width - gap;
		const int barLeft = barRight - width;
		
		const int x0 = barLeft;
		const int y0 = data.top.y;
		const int x1 = barRight;
		const int y1 = data.bottom.y;

		const float healthRatio = std::clamp(static_cast<float>(data.health)/data.maxhealth, 0.0f, 1.0f);

		// draw background
		interfaces::Surface->DrawSetColor(background);
		interfaces::Surface->DrawFilledRect(x0 - barMargin, y0 - barMargin, x1 + barMargin, y1 + barMargin);

		// draw health bar
		interfaces::Surface->DrawSetColor(bar);
		interfaces::Surface->DrawFilledRect(x0, y1 - static_cast<int>(data.height * healthRatio), x1, y1);
	}

	void Run(CTFPlayer* pLocal)
	{
		if (!helper::engine::IsInMatch() || !Settings::ESP::enabled)
			return;

		FontManager::SetFont("esp font");

		constexpr int gap = 2;

		for (const auto& entry : EntityList::GetEntities())
		{
			if (!ESP_Utils::IsValidEntity(pLocal, entry))
				continue;

			CBaseEntity* ent = entry.ptr;

			ESP_Data data = {};

			if (!GetData(entry, data))
				continue;

			Color color = ESP_Utils::GetEntityColor(ent);
			if (Settings::ESP::box)
				PaintBox(color, data);

			//if (Settings::ESP::name)
				//PaintName(color, top, w, h, data.name);

			if (Settings::ESP::healthbar && data.maxhealth > 0)
				PaintHealthbar(data);

			ESPContext context = {};

			auto run = [&](const auto& elements)
			{
				for (const auto& element : elements)
				{
					if (element == nullptr || !element->ShouldDraw(ent, data))
						continue;
		
					auto alignment = element->GetAlignment();
					Vec2 size = element->GetSize(data);

					switch(alignment)
					{
						case ESP_ALIGNMENT::TOP:
						{
							Vec2 pos = Vec2(data.top.x - size.x/2.0f, data.top.y - context.topOffset - gap);
							element->Draw(pos, ent, data, context);
							context.topOffset += element->GetSize(data).y;
							break;
						}
						case ESP_ALIGNMENT::LEFT:
						{
							Vec2 pos = Vec2(data.top.x - data.width - size.x - gap, data.top.y + context.verticalLeftOffset + gap);
							element->Draw(pos, ent, data, context);
							context.verticalLeftOffset += element->GetSize(data).y;
							break;
						}
						case ESP_ALIGNMENT::RIGHT:
						{
							Vec2 pos = Vec2(data.top.x + data.width + gap, data.top.y + context.verticalRightOffset + gap);
							element->Draw(pos, ent, data, context);
							context.verticalRightOffset += element->GetSize(data).y;
							break;
						}
						case ESP_ALIGNMENT::BOTTOM:
						{
							Vec2 pos = Vec2(data.top.x - size.x/2.0f, data.top.y + data.height + context.bottomOffset + gap);
							element->Draw(pos, ent, data, context);
							context.bottomOffset += element->GetSize(data).y;
							break;
						}
						case ESP_ALIGNMENT::INVALID:
						case ESP_ALIGNMENT::MAX:
						break;
					}
				}
			};

			run(m_builtinElements);
			run(m_luaElements);
		}
	}
};