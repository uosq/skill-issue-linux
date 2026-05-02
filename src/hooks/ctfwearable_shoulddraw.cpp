#include "ctfwearable_shoulddraw.h"

#include "../libdetour/libdetour.h"

#include "../features/logs/logs.h"

DETOUR_DECL_TYPE(bool, CTFWearable_ShouldDraw, void *self);
detour_ctx_t ctfwearable_shoulddraw_ctx;

bool Hooked_CTFWearable_ShouldDraw(void *self)
{
	bool ret;
	DETOUR_ORIG_GET(&ctfwearable_shoulddraw_ctx, ret, CTFWearable_ShouldDraw, self);
	return ret;
}

void Hook_CTFWearable_ShouldDraw()
{
	detour_init(&ctfwearable_shoulddraw_ctx, Sigs::CTFWearable_ShouldDraw.GetPointer(),
		    (void *)&Hooked_CTFWearable_ShouldDraw);

	if (!detour_enable(&ctfwearable_shoulddraw_ctx))
		Logs::Error("Couldn't hook CTFWearable::ShouldDraw");
}