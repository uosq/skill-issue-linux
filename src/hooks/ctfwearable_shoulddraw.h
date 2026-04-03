#pragma once

// CTFWearable::ShouldDraw(void* self);
// sig 55 48 89 E5 41 57 41 56 41 55 41 54 49 89 FC 53 48 83 EC 18 8B 97 54 07 00 00
// xref: ghost_wearable
// dll: client.so
// There are 2 functions that have the xref
// Get the one that only has 1 parameter

#include "../libdetour/libdetour.h"
#include "../libsigscan.h"
#include "../settings/settings.h"

#include "../features/logs/logs.h"

#include "../sdk/signatures/signatures.h"

ADD_SIG(CTFWearable_ShouldDraw, "client.so",
	"55 48 89 E5 41 57 41 56 41 55 41 54 49 89 FC 53 48 83 EC 18 8B 97 "
	"54 07 00 00")

DETOUR_DECL_TYPE(bool, CTFWearable_ShouldDraw, void *self);
static detour_ctx_t ctfwearable_shoulddraw_ctx;

static bool Hooked_CTFWearable_ShouldDraw(void *self)
{
	if (Settings::Misc.no_zoom)
		return true;

	bool ret;
	DETOUR_ORIG_GET(&ctfwearable_shoulddraw_ctx, ret, CTFWearable_ShouldDraw, self);
	return ret;
}

static void Hook_CTFWearable_ShouldDraw()
{
	detour_init(&ctfwearable_shoulddraw_ctx, Sigs::CTFWearable_ShouldDraw.GetPointer(),
		    (void *)&Hooked_CTFWearable_ShouldDraw);

	if (!detour_enable(&ctfwearable_shoulddraw_ctx))
		Logs::Error("Couldn't hook CTFWearable::ShouldDraw");
}