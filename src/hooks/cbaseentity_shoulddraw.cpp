#include "cbaseentity_shoulddraw.h"

#include "../libdetour/libdetour.h"

#include "../features/logs/logs.h"

DETOUR_DECL_TYPE(bool, CBaseEntity_ShouldDraw, CBaseEntity *self);
detour_ctx_t baseentity_shoulddraw;

bool Hooked_CBaseEntity_ShouldDraw(CBaseEntity *self)
{
	if (self == nullptr)
		return false;

	bool ret;
	DETOUR_ORIG_GET(&baseentity_shoulddraw, ret, CBaseEntity_ShouldDraw, self);

	return ret;
}

void Hook_CBaseEntity_ShouldDraw()
{
	detour_init(&baseentity_shoulddraw, Sigs::CBaseEntity_ShouldDraw.GetPointer(),
		    (void *)&Hooked_CBaseEntity_ShouldDraw);

	if (!detour_enable(&baseentity_shoulddraw))
		Logs::Error("Couldn't hook C_BaseEntity::ShouldDraw");
}