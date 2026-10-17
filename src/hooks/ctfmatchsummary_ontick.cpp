#if 0

#include <cstdint>

#include "../features/hookmanager/hookmanager.hpp"
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

/*
Just look at the code above ^
*/
ADD_SIG(GTFGCClientSystem, "client.so",
	"E8 ? ? ? ? 4C 8D 6D DC 48 89 C7 E8 ? ? ? ? 4C 89 EF 89 45 DC E8 ? ? "
	"? ? 48 8D 75 D8 4C 89 EA")

INIT_HOOK(CTFMatchSummary_OnTick, void, (void* rdi), "client.so", "E8 ? ? ? ? 4C 8D 6D DC 48 89 C7 E8 ? ? ? ? 4C 89 EF 89 45 DC E8 ? ? ? ? 48 8D 75 D8 4C 89 EA")
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
	auto gc = reinterpret_cast<uintptr_t (*)()>(Sigs::GTFGCClientSystem.GetPointer())();
	auto match_id = reinterpret_cast<uint32_t *>(gc + 0x818);

	// why the fuck did Valve even make this shitty ass survey??
	if (*match_id & 2)
		*match_id &= ~2;

	original_CTFMatchSummary_OnTick(rdi);
}

#endif