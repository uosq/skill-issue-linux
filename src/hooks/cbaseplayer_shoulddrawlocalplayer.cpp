#include "../features/entitylist/entitylist.h"
#include "../features/hookmanager/hookmanager.hpp"

/* CBasePlayer::ShouldDrawLocalPlayer */
INIT_HOOK(ShouldDrawLocalPlayer, bool, (), "client.so", "55 48 89 E5 41 54 48 83 EC 08 48 8D 05 ? ? ? ? 48 8B 38 48 85 FF 74 ? 48 8B 07 FF 50 38")
{
	auto pLocal = features::entities.GetLocal();
	if (pLocal != nullptr && pLocal->IsAlive() && pLocal->InCond(TF_COND_ZOOMED))
		return true;

	return original_ShouldDrawLocalPlayer();
}
