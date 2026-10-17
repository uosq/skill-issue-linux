#include "../features/misc/misc.h"
#include "../features/entitylist/entitylist.h"
#include "../features/hookmanager/hookmanager.hpp"

/*
xref: CViewRender::DrawViewModel

		local_188 = 0;
		if (_g_Telemetry != 0) {
		(**(code **)(_g_Telemetry + 0xa0))
			(_g_Telemetry,&local_188,0,0,0,"viewrender.cpp",0x41e,&DAT_02f68bd0,&DAT_0235722d,
			"DrawViewModels");
		}
		local_160 = _g_Telemetry;
		local_168 = local_188;
======>		bVar3 = ShouldDrawViewModel(this,drawViewmodel);
		bVar4 = FUN_018b53e0();
		local_180 = (long *)(**(code **)(*DAT_03025308 + 0x310))();
		if (local_180 != (long *)0x0) {
*/

INIT_HOOK(CViewRender_ShouldDrawViewModel, bool, (void* rdi, bool bDrawviewmodel), "client.so", "40 84 F6 74 ? 48 8B 05 ? ? ? ? 8B 50 58")
{
	if (config::no_zoom::enabled.Get())
	{
		CTFPlayer* pLocal = features::entities.GetLocal();
		if (pLocal && pLocal->InCond(TF_COND_ZOOMED))
			return true;
	}
	
	return original_CViewRender_ShouldDrawViewModel(rdi, bDrawviewmodel);
}