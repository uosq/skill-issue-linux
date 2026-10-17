#include "../features/misc/misc.h"
#include "../features/hookmanager/hookmanager.hpp"

INIT_HOOK(GetMaxItemCount, int, (void* rdi), "client.so", "48 8B BF 98 00 00 00 48 85 FF 74 ? 55 BE 07 00 00 00 48 89 E5 E8 ? ? ? ? 48 85 C0 74 ? 83 78 28 01 75 ? 48 8B 40 08 48 8B 10")
{
	if (config::backpack_expander::enabled.Get())
		return 4000;

	return original_GetMaxItemCount(rdi);
}
