#include "cviewrender_shoulddrawviewmodel.h"

#include "../sdk/signatures/signatures.h"

#include "../libdetour/libdetour.h"
#include "../settings/settings.h"

#include "../features/entitylist/entitylist.h"
#include "../features/logs/logs.h"

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
ADD_SIG(CViewRender_ShouldDrawViewModel, "client.so", "40 84 F6 74 ? 48 8B 05 ? ? ? ? 8B 50 58")
DETOUR_DECL_TYPE(bool, ShouldDrawViewModel, void* self, bool bDrawviewmodel);

static detour_ctx_t ctx;

bool CViewRender_ShouldDrawViewModel(void* self, bool bDrawviewmodel)
{
	if (Settings::Misc.no_zoom)
	{
		CTFPlayer* pLocal = EntityList::GetLocal();
		if (pLocal && pLocal->InCond(TF_COND_ZOOMED))
			return true;
	}
	
	bool ret = true;
	DETOUR_ORIG_GET(&ctx, ret, ShouldDrawViewModel, self, bDrawviewmodel);
	return ret;
}

void Hook_ShouldDrawViewModel()
{
	detour_init(&ctx, Sigs::CViewRender_ShouldDrawViewModel.GetPointer(), (void*)&CViewRender_ShouldDrawViewModel);
	if (!detour_enable(&ctx))
		return Logs::Error("Couldnt hook CViewRender::ShouldDrawViewModel");
}