#include "viewmodel_offset.h"

void ViewmodelOffset::Run(Vector& position, Vector& angle)
{
	Vector offset = {Settings::misc.viewmodel_offset[0], Settings::misc.viewmodel_offset[1], Settings::misc.viewmodel_offset[2]};
	if (offset.IsZero())
		return;

	Vector forward, right, up;
	Math::AngleVectors(angle, &forward, &right, &up);

	position += forward * offset.x;
	position += right * offset.y;
	position += up * offset.z;
}