#pragma once

// Credits to PixyZT on UC
// https://www.unknowncheats.me/forum/team-fortress-2-a/740968-remove-viewmodel-bobbing-redux.html

/*
dll: client.so
how to get:
goto C_BaseViewModel::CalcViewModelView


        (**(code **)(*this + 0x898))(this,&eyePos,&local_5c,&eyeAng);
      }
      goto LAB_015fa4f1;
    }
  }
  plVar6 = (long *)0x0;
LAB_015fa4f1:
  if (*(code **)(*this + 0x8a8) != FUN_015fb1f0) {
we need this offset ===>    (**(code **)(*this + 0x8a8))(this,owner,&eyePos,&local_5c);
  }
  cVar1 = (**(code **)(*(long *)PTR_DAT_02ea6650 + 0x78))();

What you want is the 0x8a8 offset
0x8a8/8 = 277
that is the vftable index on C_TFViewModel
That index is the C_TFViewModel::AddViewModelBob

void C_TFViewModel::AddViewModelBob
               (long *this,undefined8 owner,undefined8 eyePosition,undefined8 eyeAngles)

{
  int iVar1;
  long lVar2;
  
  iVar1 = FUN_015fabe0();
  if (iVar1 == 1) {
    if (*(code **)(*this + 0x1b8) == FUN_015c7110) {
      lVar2 = this[0x11];
    }
    else {
      lVar2 = (**(code **)(*this + 0x1b8))(this);
    }
    if (lVar2 != 0) {
 we want this one ====> CalcViewModelBobHelper(owner,(long)this + 0xcd4);
      FUN_01e5eb70(eyePosition,eyeAngles,(long)this + 0xcd4);
      return;
    }
  }
  return;
}
*/

#include <cstring>

#include "../libdetour/libdetour.h"
#include "../libsigscan.h"

#include "../sdk/classes/player.h"
#include "../sdk/classes/weaponbase.h"
#include "../sdk/signatures/signatures.h"

#include "../features/logs/logs.h"
#include "../settings/settings.h"

DETOUR_DECL_TYPE(float, CalcViewModelBobHelper, CTFPlayer *pPlayer, BobState_t *pBobState);
ADD_SIG(CalcViewModelBobHelper, "client.so", "48 85 F6 0F 84 ? ? ? ? 55 66 0F EF DB")

static detour_ctx_t calcbobhelper;

static float Hooked_CalcViewModelBobHelper(CTFPlayer *pPlayer, BobState_t *pBobState)
{
	if (!Settings::Misc.no_viewmodel_bob)
	{
		DETOUR_ORIG_CALL(&calcbobhelper, CalcViewModelBobHelper, pPlayer, pBobState);
		return 0.0f;
	}

	memset(pBobState, 0, sizeof(BobState_t));
	return 0.0f;
}

static void Hook_CalcViewModelBobHelper()
{
	detour_init(&calcbobhelper, Sigs::CalcViewModelBobHelper.GetPointer(), (void *)&Hooked_CalcViewModelBobHelper);
	if (!detour_enable(&calcbobhelper))
		Logs::Error("Couldn't hook CalcViewModelBobHelper");
}