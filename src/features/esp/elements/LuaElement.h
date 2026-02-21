#if 0

#pragma once

#include <string>
#include "BaseElement.h"
#include "../../../settings/settings.h"
#include "../../../sdk/helpers/helper.h"

/*
esp.Register("hello world", function(entity, data)
	if entity is valid then
		return true
	end

	return false
end)
*/

class LuaElement : public IBaseElement
{
public:
	bool ShouldDraw(CBaseEntity*, const ESP_Data&) const override
	{
		
	}

	void Draw(Vec2& pos, const ESP_Data&, ESPContext&) const override
	{
		helper::draw::TextShadow(pos.x, pos.y, Color(255, 255, 255, 255), m_text);
	}

	Vec2 GetSize(const ESP_Data& data) const override
	{
		int w, h;
		helper::draw::GetTextSize(m_text, w, h);
		return Vec2(w, h);
	}

	ESP_ALIGNMENT GetAlignment() const override
	{
		return m_alignment;
	}

private:
	std::string m_text;
	int m_shoulddraw_ref;
	ESP_ALIGNMENT m_alignment;
};

#endif