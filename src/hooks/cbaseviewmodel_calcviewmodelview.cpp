#include "../sdk/classes/entity.h"

#include "../features/visuals/viewmodel_aim/viewmodel_aim.h"
#include "../features/visuals/viewmodel_interp/viewmodel_interp.h"
#include "../features/visuals/viewmodel_offset/viewmodel_offset.h"
#include "../features/visuals/norecoil/norecoil.h"
#include "../features/scriptmanager/scriptmanager.h"
#include "../features/hookmanager/hookmanager.hpp"

INIT_HOOK(CalcViewModelView, void, (void *thisptr, CBaseEntity *owner, const Vector &eyePosition, const QAngle &eyeAngles), "client.so", "55 48 89 E5 41 57 41 56 41 55 49 89 F5 41 54 49 89 FC 53 48 83 EC 48 8B 41 08")
{
	Vector angle = eyeAngles;
	Vector position = eyePosition;

	if (owner)
	{
		features::scriptmanager.CallHooks("CalcViewModelView", &position, &angle);

		features::norecoil.RunCalcViewModelView(angle);
		features::viewmodel_interp.Run(angle);
		features::viewmodel_aim.Run(angle);
		features::viewmodel_offset.Run(position, angle);
	}

	original_CalcViewModelView(thisptr, owner, position, angle);
}
