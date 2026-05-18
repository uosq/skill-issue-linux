#pragma once

#include "../sdk/signatures/signatures.h"

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

void Hooked_CTFMatchSummary_OnTick(void *self);
void Hook_CTFMatchSummary_OnTick();