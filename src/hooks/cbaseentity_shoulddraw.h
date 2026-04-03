#pragma once

#include "../libdetour/libdetour.h"
#include "../libsigscan.h"
#include "../settings/settings.h"

#include "../sdk/classes/entity.h"

#include "../features/entitylist/entitylist.h"
#include "../features/logs/logs.h"

#include "../sdk/signatures/signatures.h"

static detour_ctx_t baseentity_shoulddraw;
DETOUR_DECL_TYPE(bool, CBaseEntity_ShouldDraw, CBaseEntity *self);

ADD_SIG(CBaseEntity_ShouldDraw, "client.so", "31 C0 80 BF AC 00 00 00 0A")

static bool Hooked_CBaseEntity_ShouldDraw(CBaseEntity *self)
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

static void Hook_CBaseEntity_ShouldDraw()
{
	detour_init(&baseentity_shoulddraw, Sigs::CBaseEntity_ShouldDraw.GetPointer(),
		    (void *)&Hooked_CBaseEntity_ShouldDraw);

	if (!detour_enable(&baseentity_shoulddraw))
		Logs::Error("Couldn't hook C_BaseEntity::ShouldDraw");
}