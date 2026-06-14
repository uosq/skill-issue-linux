#include "../core/core.h"

#include "../sdk/defs.h"
#include "../sdk/signatures/signatures.h"

#include "../thirdparty/libdetour/libdetour.h"

#include "../features/misc/misc.h"
#include "../features/logs/logs.h"
#include "../features/hook_initializer/initializer.h"

ADD_SIG(CTFPlayerShared_InCond, "client.so", "55 83 FE 1F 48 89 E5 41 54 41 89 F4")

static detour_ctx_t ctx;
DETOUR_DECL_TYPE(bool, CTFPlayerShared_InCond, void*, ETFCond);

static bool Hooked_InCond(void* rdi, ETFCond eCond)
{
	if (gApp->IsInitialized())
	{
		if (eCond == TF_COND_ZOOMED && config::no_scope_overlay::enabled.Get())
			return false;
	}

	bool ret{false};
	DETOUR_ORIG_GET(&ctx, ret, CTFPlayerShared_InCond, rdi, eCond);
	return ret;
}

void Hook_CTFPlayerShared_InCond(void)
{
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

	detour_init(&ctx, Sigs::CTFPlayerShared_InCond.GetPointer(), (void*)&Hooked_InCond);

	if (!detour_enable(&ctx))
		return features::logs.Error("Couldn't hook CTFPlayerShared::InCond");
}

//MARK_FOR_INIT(Hook_CTFPlayerShared_InCond)