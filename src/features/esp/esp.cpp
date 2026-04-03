#include "esp.h"

#include "elements/BaseElement.h"
#include "elements/BonkCondElement.h"
#include "elements/HealthTextElement.h"
#include "elements/JaratedCondElement.h"
// #include "elements/LuaElement.h"
#include "elements/NameElement.h"
#include "elements/UberedCondElement.h"
#include "elements/ZoomedCondElement.h"

#include "structs.h"

#include "../visuals/thirdperson/thirdperson.h"

namespace ESP
{
	std::vector<std::unique_ptr<IBaseElement>> m_builtinElements = {};
	// std::vector<std::unique_ptr<LuaElement>> m_luaElements = {};

	void Init()
	{
		constexpr int iFONT_FLAGS = EFONTFLAG_CUSTOM | EFONTFLAG_ANTIALIAS;

		FontManager::CreateFont("esp font tf2", "TF2 BUILD", 16, 400, iFONT_FLAGS);
		FontManager::CreateFont("esp font arial", "Arial", 16, 400, iFONT_FLAGS);

		m_builtinElements.reserve(6);

		m_builtinElements.push_back(std::make_unique<NameElement>());
		m_builtinElements.push_back(std::make_unique<HealthTextElement>());
		m_builtinElements.push_back(std::make_unique<ZoomedCondElement>());
		m_builtinElements.push_back(std::make_unique<UberedCondElement>());
		m_builtinElements.push_back(std::make_unique<JaratedCondElement>());
		m_builtinElements.push_back(std::make_unique<BonkedCondElement>());

		// m_luaElements.reserve(5);
	}

	bool GetData(const EntityListEntry &entry, ESP_Data &out)
	{
		if (entry.ptr == nullptr)
			return false;

		CBaseEntity *ent = entry.ptr;

		if (entry.flags & EntityFlags::IsPlayer)
		{
			if (ent == EntityList::GetLocal() && !Thirdperson::IsThirdPerson(static_cast<CTFPlayer *>(ent)))
				return false;

			if (!ESP_Utils::GetEntityBounds(ent, out))
				return false;

			auto *p	   = static_cast<CTFPlayer *>(ent);
			out.name   = p->GetName();
			out.health = p->GetHealth();

			if (auto *res = EntityList::GetPlayerResources())
			{
				int index      = ent->GetIndex();
				out.maxhealth  = res->m_iMaxHealth(index);
				out.buffhealth = res->m_iMaxBuffedHealth(index);
			}

			return true;
		}

		if (entry.flags & EntityFlags::IsBuilding)
		{
			if (!ESP_Utils::GetEntityBounds(ent, out))
				return false;

			auto *b	       = static_cast<CBaseObject *>(ent);
			out.name       = b->GetName();
			out.health     = b->m_iHealth();
			out.maxhealth  = b->m_iMaxHealth();
			out.buffhealth = 0;
			return true;
		}

		out.name =
		    ent->GetClientClass()->networkName != nullptr ? ent->GetClientClass()->networkName : "unknown";
		return false;
	}

	void PaintBox(Color color, const ESP_Data &data)
	{
		helper::draw::SetColor(color);
		helper::draw::OutlinedRect(data.top.x - data.width, data.bottom.y - data.height,
					   data.bottom.x + data.width, data.bottom.y);
	}

	void PaintName(Color color, const Vector &top, int w, int h, const std::string &name)
	{
		int textw, texth;
		helper::draw::GetTextSize(name, textw, texth);
		helper::draw::TextShadow(top.x - (textw * 0.5f), top.y - texth - 2, color, name);
	}

	void PaintHealthbar(const ESP_Data &data)
	{
		const Color background	= {20, 20, 20, 255};

		constexpr int width	= 5;
		constexpr int gap	= 3;
		constexpr int barMargin = 1;

		const int barRight	= data.top.x - data.width - gap;
		const int barLeft	= barRight - width;

		const int x0		= barLeft;
		const int y0		= data.top.y;
		const int x1		= barRight;
		const int y1		= data.bottom.y;

		const float healthRatio = std::clamp(static_cast<float>(data.health) / data.maxhealth, 0.0f, 1.0f);

		// draw background
		interfaces::Surface->DrawSetColor(background);
		interfaces::Surface->DrawFilledRect(x0 - barMargin, y0 - barMargin, x1 + barMargin, y1 + barMargin);

		int r = (1 - healthRatio) * 255;
		int g = healthRatio * 255;

		// draw health bar
		interfaces::Surface->DrawSetColor(Color{r, g, 100, 255});
		interfaces::Surface->DrawFilledRect(x0, y1 - static_cast<int>(data.height * healthRatio), x1, y1);
	}

	void Run(CTFPlayer *pLocal)
	{
		if (!helper::engine::IsInMatch() || !Settings::ESP.enabled)
			return;

		if (interfaces::Engine->IsTakingScreenshot())
			return;

		constexpr int iGAP = 2;

		FontManager::SetFont(GetFont());

		for (const auto &entry : EntityList::GetEntities())
		{
			if (!ESP_Utils::IsValidEntity(pLocal, entry))
				continue;

			ESP_Data data{};
			if (!GetData(entry, data))
				continue;

			CBaseEntity *ent = entry.ptr;

			Color color	 = ESP_Utils::GetEntityColor(ent);

			if (Settings::ESP.box)
				PaintBox(color, data);

			{ // Health bar
				auto mode = static_cast<HealthMode>(Settings::ESP.health);
				if (data.maxhealth > 0 && (mode == HealthMode::BAR || mode == HealthMode::BOTH))
					PaintHealthbar(data);
			}

			ESPContext context{};
			context.topOffset += 16.0f;

			auto run = [&](const auto &elements) {
				for (const auto &element : elements)
				{
					if (element == nullptr || !element->ShouldDraw(ent, data))
						continue;

					auto alignment = element->GetAlignment();
					Vec2 size      = element->GetSize(data);

					switch (alignment)
					{
					case ESP_ALIGNMENT::TOP:
					{
						Vec2 pos = Vec2(data.top.x - size.x / 2.0f,
								data.top.y - context.topOffset - iGAP);
						element->Draw(pos, ent, data, context);
						context.topOffset += element->GetSize(data).y;
						break;
					}
					case ESP_ALIGNMENT::LEFT:
					{
						Vec2 pos = Vec2(data.top.x - data.width - size.x - iGAP,
								data.top.y + context.verticalLeftOffset + iGAP);
						element->Draw(pos, ent, data, context);
						context.verticalLeftOffset += element->GetSize(data).y;
						break;
					}
					case ESP_ALIGNMENT::RIGHT:
					{
						Vec2 pos = Vec2(data.top.x + data.width + iGAP,
								data.top.y + context.verticalRightOffset + iGAP);
						element->Draw(pos, ent, data, context);
						context.verticalRightOffset += element->GetSize(data).y;
						break;
					}
					case ESP_ALIGNMENT::BOTTOM:
					{
						Vec2 pos = Vec2(data.top.x - size.x / 2.0f,
								data.top.y + data.height + context.bottomOffset + iGAP);
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
			// run(m_luaElements);
		}
	}

	const char *GetHealthMode()
	{
		switch (static_cast<HealthMode>(Settings::ESP.health))
		{
		case HealthMode::TEXT:
			return "Text";
		case HealthMode::BAR:
			return "Bar";
		case HealthMode::BOTH:
			return "Both";
		default:
			return "Invalid";
		}
	}

	const char *GetTeamMode()
	{
		switch (static_cast<ESPTeamSelectionMode>(Settings::ESP.team_selection))
		{
		case ESPTeamSelectionMode::ENEMIES:
			return "Only Enemies";
		case ESPTeamSelectionMode::TEAMMATES:
			return "Only Teammates";
		case ESPTeamSelectionMode::BOTH:
			return "Both";
		default:
			return "Invalid";
		}
	}

	int GetFont()
	{
		switch (static_cast<ESPFont>(Settings::ESP.font))
		{
		case ESPFont::TF2BUILD:
			return FontManager::GetFont("esp font tf2");
		case ESPFont::ARIAL:
			return FontManager::GetFont("esp font arial");
		default:
			break;
		}

		return FontManager::GetFont("esp font tf2");
	}
}; // namespace ESP