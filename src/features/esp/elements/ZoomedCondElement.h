#pragma once

#include "../../../sdk/helpers/helper.h"
#include "../../../settings/settings.h"
#include "../esp_utils.h"
#include "BaseElement.h"
#include <string>

class ZoomedCondElement : public IBaseElement
{
      public:
	bool ShouldDraw(CBaseEntity *pEnt, const ESP_Data &) const override
	{
		if (!pEnt->IsPlayer())
			return false;

		CTFPlayer *pPlayer = static_cast<CTFPlayer *>(pEnt);
		return pPlayer->InCond(ETFCond::TF_COND_ZOOMED) &&
		       (Settings::ESP.fconditions & static_cast<int>(ESPConditionFlags::Zoomed));
	}

	void Draw(Vec2 &pos, CBaseEntity *ent, const ESP_Data &data, ESPContext &ctx) const override
	{
		Color color = GetColor(ent, data);
		helper::draw::TextShadow(pos.x, pos.y, color, "Zoom");
	}

	Vec2 GetSize(const ESP_Data &data) const override
	{
		int w, h;
		helper::draw::GetTextSize("Zoom", w, h);
		return Vec2(w, h);
	}

	ESP_ALIGNMENT GetAlignment() const override
	{
		return ESP_ALIGNMENT::RIGHT;
	}

	Color GetColor(CBaseEntity *pEnt, const ESP_Data &data) const override
	{
		return Color{255, 255, 255, 255};
	}
};