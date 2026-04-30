#include "cengineclient_getscreenaspectratio.h"

#include "../vtables.h"
#include "../settings/settings.h"

#include "../sdk/interfaces/interfaces.h"

DECLARE_VTABLE_HOOK(GetScreenAspectRatio, float, (void* self))
{
	if (Settings::Misc.aspectratio > 0)
		return Settings::Misc.aspectratio;

	return originalGetScreenAspectRatio(self);
}

void Hook_CEngineClient_GetScreenAspectRatio()
{
/*
void FUN_01f20050(undefined8 param_1,long *param_2)
{
		undefined4 uVar1;
		code *pcVar2;
		char *pcVar3;
		float fVar4;
		ulong uVar5;
		
======>		fVar4 = (float)(**(code **)(*g_pEngine + 0x2f8))();
		(**(code **)(*param_2 + 0x820))(param_2,0);
		pcVar2 = *(code **)(*param_2 + 0x720);
		if (pcVar2 == FUN_01bb0cb0) {
		(**(code **)(*param_2 + 0x728))();
		}
		else {
		(*pcVar2)(param_2);
*/
/*
0x2f8 / sizeof(void*) = 95
*/

/*
xref: #TF_Comp_Scoreboard_Score_Standard
inside CTFMatchSummary::InitPlayerList( TFSectionedListPanel *pPlayerList, int nTeam )
first variable
*/

	INSTALL_VTABLE_HOOK(GetScreenAspectRatio, interfaces::Engine, 95);
}