#pragma once

#include "../../../sdk/classes/entity.h"
#include "../structs.h"

class IBaseElement
{
      public:
	virtual ~IBaseElement()									    = default;

	virtual bool ShouldDraw(CBaseEntity *ent, const ESP_Data &data) const			    = 0;
	virtual void Draw(Vec2 &pos, CBaseEntity *ent, const ESP_Data &data, ESPContext &ctx) const = 0;
	virtual Vec2 GetSize(const ESP_Data &data) const					    = 0;
	virtual ESP_ALIGNMENT GetAlignment() const						    = 0;
	virtual Color GetColor(CBaseEntity *ent, const ESP_Data &data) const			    = 0;
};