#pragma once

#include "../libdetour/libdetour.h"
#include "../features/lua/hookmgr.h"
#include "../features/lua/api.h"
#include "../features/lua/classes.h"

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
        ValidadeUserCmd(self,pppuVar3,from);
        pcVar4 = *(code **)(*self + 0x40);
      }
      else {
        pppuVar3 = &local_88;
      }
*/

inline detour_ctx_t validate_ctx;
DETOUR_DECL_TYPE(void, ValidateUserCmd, void*, CUserCmd*, int);

inline void Hooked_ValidateUserCmd(void* input, CUserCmd* pCmd, int sequence_number)
{

}

inline void Hook_ValidateUserCmd(void)
{
	void* original = sigscan_module("client.so", "55 48 89 E5 41 56 41 89 D6 41 55 49 89 FD 41 54 4C 8D 65 DC");
	detour_init(&validate_ctx, original, (void*)&Hooked_ValidateUserCmd);

	if (!detour_enable(&validate_ctx))
	{
		interfaces::Cvar->ConsolePrintf("Failed to hook CInput::ValidateUserCmd!\n");
		return;
	}

	interfaces::Cvar->ConsolePrintf("CInput::ValidateUserCmd hooked\n");
}