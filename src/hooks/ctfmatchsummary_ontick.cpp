#include "ctfmatchsummary_ontick.h"

detour_ctx_t summary_ontick_ctx;
DETOUR_DECL_TYPE(void, CTFMatchSummary_OnTick, void *self);

void Hooked_CTFMatchSummary_OnTick(void *self)
{
	/*
	the offset is from this lVar7 + 0x818
	if ((*(byte *)(lVar7 + 0x818) & 2) != 0) {
		lVar7 = GCTFGCClientSystem();
		uVar12 = CreateSurveyQuestionPanel(lVar10,lVar7 + 0x808);
		FUN_0217e8a0(uVar12,1,0);
	}
	*/

	// remember, GCTFGCClientSystem is a function that returns a pointer!
	auto gc	      = reinterpret_cast<uintptr_t (*)()>(Sigs::GTFGCClientSystem.GetPointer())();
	auto match_id = reinterpret_cast<uint32_t *>(gc + 0x818);

	// why the fuck did Valve even make this shitty ass survey??
	if (*match_id & 2)
		*match_id &= ~2;

	DETOUR_ORIG_CALL(&summary_ontick_ctx, CTFMatchSummary_OnTick, self);
}

void Hook_CTFMatchSummary_OnTick()
{
	detour_init(&summary_ontick_ctx, Sigs::CTFMatchSummary_OnTick.GetPointer(),
		    (void *)&Hooked_CTFMatchSummary_OnTick);
	if (!detour_enable(&summary_ontick_ctx))
		Logs::Error("Couldn't hook CTFMatchSumary::OnTick");
}