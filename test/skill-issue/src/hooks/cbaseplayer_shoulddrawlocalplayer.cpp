#include "cbaseplayer_shoulddrawlocalplayer.h"

#include "../libdetour/libdetour.h"

#include "../features/entitylist/entitylist.h"
#include "../features/logs/logs.h"

DETOUR_DECL_TYPE(bool, CBasePlayer_ShouldDrawLocalPlayer, void);
detour_ctx_t entity_shoulddraw;

bool Hooked_CBasePlayer_ShouldDrawLocalPlayer(void)
{
	bool ret;
	DETOUR_ORIG_GET(&entity_shoulddraw, ret, CBasePlayer_ShouldDrawLocalPlayer);

	auto pLocal = features::entities.GetLocal();
	if (pLocal != nullptr && pLocal->IsAlive() && pLocal->InCond(TF_COND_ZOOMED))
		return true;

	return ret;
}

void Hook_CBasePlayer_ShouldDrawLocalPlayer()
{
	detour_init(&entity_shoulddraw, Sigs::CBasePlayer_ShouldDrawLocalPlayer.GetPointer(),
		    (void *)&Hooked_CBasePlayer_ShouldDrawLocalPlayer);

	if (!detour_enable(&entity_shoulddraw))
		features::logs.Error("Couldn't hook C_BasePlayer::ShouldDrawLocalPlayer");
}