#include "../sdk/classes/entity.h"
#include "../sdk/signatures/signatures.h"

#include "../thirdparty/libdetour/libdetour.h"

#include "../features/logs/logs.h"
#include "../features/hook_initializer/initializer.h"

ADD_SIG(CBaseEntity_ShouldDraw, "client.so", "31 C0 80 BF AC 00 00 00 0A")

DETOUR_DECL_TYPE(bool, CBaseEntity_ShouldDraw, CBaseEntity *self);
detour_ctx_t baseentity_shoulddraw;

static bool Hooked_CBaseEntity_ShouldDraw(CBaseEntity *self)
{
	if (self == nullptr)
		return false;

	bool ret;
	DETOUR_ORIG_GET(&baseentity_shoulddraw, ret, CBaseEntity_ShouldDraw, self);

	return ret;
}

static void Hook_CBaseEntity_ShouldDraw()
{
	detour_init(&baseentity_shoulddraw, Sigs::CBaseEntity_ShouldDraw.GetPointer(),
		    (void *)&Hooked_CBaseEntity_ShouldDraw);

	if (!detour_enable(&baseentity_shoulddraw))
		features::logs.Error("Couldn't hook C_BaseEntity::ShouldDraw");
}

MARK_FOR_INIT(Hook_CBaseEntity_ShouldDraw)