#include "ctfwearable_shoulddraw.h"

DETOUR_DECL_TYPE(bool, CTFWearable_ShouldDraw, void *self);
detour_ctx_t ctfwearable_shoulddraw_ctx;

bool Hooked_CTFWearable_ShouldDraw(void *self)
{
	if (Settings::Misc.no_zoom)
		return true;

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