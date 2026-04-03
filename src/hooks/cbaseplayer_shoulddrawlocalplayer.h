#pragma once

#include "../libdetour/libdetour.h"
#include "../libsigscan.h"
#include "../settings/settings.h"

#include "../sdk/classes/entity.h"

#include "../features/entitylist/entitylist.h"
#include "../features/logs/logs.h"

#include "../sdk/signatures/signatures.h"

static detour_ctx_t entity_shoulddraw;
DETOUR_DECL_TYPE(bool, CBasePlayer_ShouldDrawLocalPlayer, void);

ADD_SIG(CBasePlayer_ShouldDrawLocalPlayer, "client.so",
	"55 48 89 E5 41 54 48 83 EC 08 48 8D 05 ? ? ? ? 48 8B 38 48 85 FF 74 "
	"? 48 8B 07 FF 50 38")

static bool Hooked_CBasePlayer_ShouldDrawLocalPlayer(void)
{
	bool ret;
	DETOUR_ORIG_GET(&entity_shoulddraw, ret, CBasePlayer_ShouldDrawLocalPlayer);

	if (auto pLocal = EntityList::GetLocal(); pLocal != nullptr && pLocal->IsAlive())
		return ret || (pLocal->InCond(TF_COND_ZOOMED) && Settings::Misc.no_zoom);

	return ret;
}

static void Hook_CBasePlayer_ShouldDrawLocalPlayer()
{
	detour_init(&entity_shoulddraw, Sigs::CBasePlayer_ShouldDrawLocalPlayer.GetPointer(),
		    (void *)&Hooked_CBasePlayer_ShouldDrawLocalPlayer);

	if (!detour_enable(&entity_shoulddraw))
		Logs::Error("Couldn't hook C_BasePlayer::ShouldDrawLocalPlayer");
}