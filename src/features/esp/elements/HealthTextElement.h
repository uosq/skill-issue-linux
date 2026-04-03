#pragma once

#include "../../../sdk/helpers/helper.h"
#include "../../../settings/settings.h"
#include "BaseElement.h"
#include <string>

class HealthTextElement : public IBaseElement
{
      public:
	bool ShouldDraw(CBaseEntity *, const ESP_Data &) const override
	{
		auto mode = static_cast<HealthMode>(Settings::ESP.health);

		if (mode >= HealthMode::MAX || mode <= HealthMode::INVALID)
			return false;

		return mode == HealthMode::TEXT || mode == HealthMode::BOTH;
	}

	void Draw(Vec2 &pos, CBaseEntity *ent, const ESP_Data &data, ESPContext &ctx) const override
	{
		helper::draw::TextShadow(pos.x, pos.y, GetColor(ent, data),
					 std::to_string(data.health) + "/" + std::to_string(data.maxhealth));
	}

	Vec2 GetSize(const ESP_Data &data) const override
	{
		int w, h;
		helper::draw::GetTextSize(std::to_string(data.health) + "/" + std::to_string(data.maxhealth), w, h);
		return Vec2(w, h);
	}

	ESP_ALIGNMENT GetAlignment() const override
	{
		return ESP_ALIGNMENT::BOTTOM;
	}

	Color GetColor(CBaseEntity *pEnt, const ESP_Data &data) const override
	{
		if (data.health <= 0 || data.maxhealth <= 0)
			return Color{255, 255, 255, 255};

		float ratio = static_cast<float>(data.health) / static_cast<float>(data.maxhealth);

		int r	    = (1 - ratio) * 255;
		int g	    = ratio * 255;

		return Color{r, g, 100, 255};
	}
};