#pragma once

#include <string>
#include "BaseElement.h"
#include "../../../settings/settings.h"
#include "../../../sdk/helpers/helper.h"

class HealthTextElement : public IBaseElement
{
public:
	bool ShouldDraw(CBaseEntity*, const ESP_Data&) const override
	{
		return Settings::ESP::healthbar;
	}

	void Draw(Vec2& pos, const ESP_Data& data, ESPContext& ctx) const override
	{
		helper::draw::TextShadow(pos.x, pos.y, Color(255, 255, 255, 255), std::to_string(data.health) + "/" + std::to_string(data.maxhealth));
	}

	Vec2 GetSize(const ESP_Data& data) const override
	{
		int w, h;
		helper::draw::GetTextSize(std::to_string(data.health) + "/" + std::to_string(data.maxhealth), w, h);
		return Vec2(w, h);
	}

	ESP_ALIGNMENT GetAlignment() const override
	{
		return ESP_ALIGNMENT::BOTTOM;
	}
};