#pragma once

#include <string>
#include "BaseElement.h"
#include "../../../settings/settings.h"
#include "../../../sdk/helpers/helper.h"
#include "../esp_utils.h"

class NameElement : public IBaseElement
{
public:
	bool ShouldDraw(CBaseEntity*, const ESP_Data&) const override
	{
		return Settings::ESP::name;
	}

	void Draw(Vec2& pos, CBaseEntity* ent, const ESP_Data& data, ESPContext& ctx) const override
	{
		Color color = ESP_Utils::GetEntityColor(ent);
		helper::draw::TextShadow(pos.x, pos.y, color, data.name);
	}

	Vec2 GetSize(const ESP_Data& data) const override
	{
		int w, h;
		helper::draw::GetTextSize(data.name, w, h);
		return Vec2(w, h);
	}

	ESP_ALIGNMENT GetAlignment() const override
	{
		return ESP_ALIGNMENT::TOP;
	}
};