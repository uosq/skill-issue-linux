#include "../sdk/interfaces/interfaces.h"

#include "../features/hookmanager/hookmanager.hpp"

// sig 55 48 89 E5 41 56 41 89 D6 41 55 49 89 FD 41 54 4C 8D 65 DC
// client.so
// how to get it:
// its in CInput::WriteUsercmdDeltaToBuffer
/*
	if (from != -1) {
		if (pcVar4 == FUN_017333b0) {
			pppuVar3 = (undefined ***)(self[0x21] + (long)(from % 0x5a) * 0x48);
			if (from == *(int *)(pppuVar3 + 1)) {
				if (pppuVar3 == (undefined ***)0x0) goto LAB_017370a0;
LAB_01736f98:
==>     ValidadeUserCmd(self,pppuVar3,from);
				pcVar4 = *(code **)(*self + 0x40);
			}
			else {
				pppuVar3 = &local_88;
			}
*/

INIT_HOOK(ValidateUserCmd, void, (void *input, CUserCmd *pCmd, int sequence_number), "client.so", "55 48 89 E5 41 56 41 89 D6 41 55 49 89 FD 41 54 4C 8D 65 DC")
{
	// sometimes the best way is to just shut the fuck up
}
