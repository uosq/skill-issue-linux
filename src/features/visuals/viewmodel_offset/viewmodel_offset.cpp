#include "viewmodel_offset.h"

void ViewmodelOffset::Run(Vector &position, Vector &angle)
{
	Vector offset
	{
		config::viewmodel_offset::offset.Get()[0],
		config::viewmodel_offset::offset.Get()[1],
		config::viewmodel_offset::offset.Get()[2]
	};

	if (offset.IsZero())
		return;

	Vector forward, right, up;
	Math::AngleVectors(angle, &forward, &right, &up);

	position += forward * offset.x;
	position += right * offset.y;
	position += up * offset.z;
}