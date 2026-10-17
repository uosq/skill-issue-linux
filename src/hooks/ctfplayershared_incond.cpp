#include "../sdk/defs.h"
#include "../sdk/signatures/signatures.h"

#include "../features/misc/misc.h"
#include "../features/logs/logs.h"
#include "../features/hookmanager/hookmanager.hpp"

/*
	First go to the xref 'damage_prevented'
	You'll enter CTFPlayerShared::RemoveCond
	The FIRST function is the CTFPlayerShared::InCond that we want

	example:
	long *plVar8;
	uint *puVar9;
	uint uVar10;
	
======>	cVar3 = InCond();
	if (cVar3 == '\0') {
	return;
	}
	if ((int)eCond < 0x80) {
	if ((int)eCond < 0x60) {
	if ((int)eCond < 0x40) {
*/

INIT_HOOK(InCond, bool, (void* rdi, ETFCond eCond), "client.so", "55 83 FE 1F 48 89 E5 41 54 41 89 F4")
{
	if (eCond == TF_COND_ZOOMED && config::no_scope_overlay::enabled.Get())
		return false;

	return original_InCond(rdi, eCond);
}
