#include "../thirdparty/libdetour/libdetour.h"

#include "../core/core.h"
#include "../sdk/signatures/signatures.h"

#include "../features/entitylist/entitylist.h"
#include "../features/hook_initializer/initializer.h"

ADD_SIG(CBasePlayer_ShouldDrawLocalPlayer, "client.so", "55 48 89 E5 41 54 48 83 EC 08 48 8D 05 ? ? ? ? 48 8B 38 48 85 FF 74 ? 48 8B 07 FF 50 38")

DETOUR_DECL_TYPE(bool, CBasePlayer_ShouldDrawLocalPlayer, void);
detour_ctx_t entity_shoulddraw;

bool Hooked_CBasePlayer_ShouldDrawLocalPlayer(void)
{
	bool ret;
	DETOUR_ORIG_GET(&entity_shoulddraw, ret, CBasePlayer_ShouldDrawLocalPlayer);

	if (gApp->IsInitialized())
	{
		auto pLocal = features::entities.GetLocal();
		if (pLocal != nullptr && pLocal->IsAlive() && pLocal->InCond(TF_COND_ZOOMED))
			return true;
	}

	return ret;
}

void Hook_CBasePlayer_ShouldDrawLocalPlayer()
{
	//detour_init(&entity_shoulddraw, Sigs::CBasePlayer_ShouldDrawLocalPlayer.GetPointer(),
		    //(void *)&Hooked_CBasePlayer_ShouldDrawLocalPlayer);

	//if (!detour_enable(&entity_shoulddraw))
		//features::logs.Error("Couldn't hook C_BasePlayer::ShouldDrawLocalPlayer");
}

MARK_FOR_INIT(Hook_CBasePlayer_ShouldDrawLocalPlayer)