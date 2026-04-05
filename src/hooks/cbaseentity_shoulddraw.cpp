#include "cbaseentity_shoulddraw.h"

DETOUR_DECL_TYPE(bool, CBaseEntity_ShouldDraw, CBaseEntity *self);
detour_ctx_t baseentity_shoulddraw;

bool Hooked_CBaseEntity_ShouldDraw(CBaseEntity *self)
{
	if (self == nullptr)
		return false;

	/*if (self->GetClassID() == ETFClassID::CTFViewModel)
		if (auto pLocal = EntityList::GetLocal(); pLocal != nullptr && pLocal->IsAlive() && pLocal->InCond(TF_COND_ZOOMED))
			return Settings::Misc.no_zoom;
	*/

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