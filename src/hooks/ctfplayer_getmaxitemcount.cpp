#include "ctfplayer_getmaxitemcount.h"

DETOUR_DECL_TYPE(int, originalMaxItemCountFn, void *thisptr);
detour_ctx_t GetMaxItemCount_ctx;

int Hooked_GetMaxItemCount(void *thisptr)
{
	if (Settings::Misc.backpack_expander)
		return 4000;

	int ret;
	DETOUR_ORIG_GET(&GetMaxItemCount_ctx, ret, originalMaxItemCountFn, thisptr);

	return ret;
}

void HookCTFPlayerInventory_MaxItemCount()
{
	detour_init(&GetMaxItemCount_ctx, Sigs::CTFPlayer_GetMaxItemCount.GetPointer(),
		    (void *)&Hooked_GetMaxItemCount);
	detour_enable(&GetMaxItemCount_ctx);

#ifdef DEBUG
	constexpr Color_t color{100, 255, 100, 255};
	interfaces::Cvar->ConsoleColorPrintf(color, "CTFPlayerInventory::GetMaxItemCount hooked\n");
#endif
}