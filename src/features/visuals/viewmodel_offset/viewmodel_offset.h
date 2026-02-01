#pragma once

#include "../../../sdk/interfaces/interfaces.h"
#include "../../../sdk/helpers/helper.h"
#include "../../../settings.h"

class ViewmodelOffset
{
public:
	void Run(Vector& position, Vector& angle);
};

inline void ViewmodelOffset::Run(Vector& position, Vector& angle)
{
	Vector offset = {g_Settings.misc.viewmodel_offset[0], g_Settings.misc.viewmodel_offset[1], g_Settings.misc.viewmodel_offset[2]};
	if (offset.IsZero())
		return;

	Vector forward, right, up;
	Math::AngleVectors(angle, &forward, &right, &up);

	position += forward * offset.x;
	position += right * offset.y;
	position += up * offset.z;
}

inline ViewmodelOffset g_ViewmodelOffset;