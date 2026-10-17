#include "../sdk/classes/entity.h"

#include "../features/hookmanager/hookmanager.hpp"

INIT_HOOK(CBaseEntity_ShouldDraw, bool, (CBaseEntity* self), "client.so", "31 C0 80 BF AC 00 00 00 0A")
{
	if (self == nullptr)
		return false;

	return original_CBaseEntity_ShouldDraw(self);
}
