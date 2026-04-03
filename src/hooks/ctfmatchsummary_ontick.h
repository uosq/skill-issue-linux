#pragma once

#include "../features/logs/logs.h"
#include "../libdetour/libdetour.h"
#include "../sdk/signatures/signatures.h"
#include "../settings/settings.h"
#include <cstdint>

/*
xref: Announcer.SummaryScreenWinners

  if (g_pGameRules != 0) {
    bVar14 = *(byte *)(g_pGameRules + 0x5bd);
  }
  switch(*(undefined4 *)(param_1 + 0xd38)) {
  case 0:
    bVar16 = *(byte *)(lVar8 + 0x69);
    lVar7 = GCTFGCClientSystem();
    if ((*(byte *)(lVar7 + 0x818) & 2) != 0) {
      lVar7 = GCTFGCClientSystem();
====> uVar12 = CreateSurveyQuestionPanel(lVar10,lVar7 + 0x808);
      FUN_0217e8a0(uVar12,1,0);
    }
    *(undefined4 *)(param_1 + 0xd38) = 1;
*/
ADD_SIG(CTFMatchSummary_OnTick, "client.so", "55 48 89 E5 41 57 41 56 49 89 FE 41 55 4C 8D 6F 40 41 54 4C 89 EF")

/*
Just look at the code above ^
*/
ADD_SIG(GTFGCClientSystem, "client.so",
	"E8 ? ? ? ? 4C 8D 6D DC 48 89 C7 E8 ? ? ? ? 4C 89 EF 89 45 DC E8 ? ? "
	"? ? 48 8D 75 D8 4C 89 EA")

static detour_ctx_t summary_ontick_ctx;
DETOUR_DECL_TYPE(void, CTFMatchSummary_OnTick, void *self);

static void Hooked_CTFMatchSummary_OnTick(void *self)
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
	if (Settings::Misc.no_survey && (*match_id & 2))
		*match_id &= ~2;

	DETOUR_ORIG_CALL(&summary_ontick_ctx, CTFMatchSummary_OnTick, self);
}

static void Hook_CTFMatchSummary_OnTick()
{
	detour_init(&summary_ontick_ctx, Sigs::CTFMatchSummary_OnTick.GetPointer(),
		    (void *)&Hooked_CTFMatchSummary_OnTick);
	if (!detour_enable(&summary_ontick_ctx))
		Logs::Error("Couldn't hook CTFMatchSumary::OnTick");
}