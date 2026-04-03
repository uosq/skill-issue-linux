#pragma once

#include "../features/logs/logs.h"
#include "../libdetour/libdetour.h"
#include "../libsigscan.h"
#include "../sdk/signatures/signatures.h"

ADD_SIG(CThirdPersonManager_Update, "client.so",
	"55 48 89 E5 41 54 4C 8D 25 ? ? ? ? 53 48 89 FB 49 8B 04 24 48 85 C0 "
	"74 ? 48 8B 40 38")

DETOUR_DECL_TYPE(void, CThirdPersonManager_Update, void *self);

static void Hooked_CThirdPersonManager_Update()
{
	/*
	;)
	*/
}

static void Hook_CThirdPersonManager_Update()
{
	detour_ctx_t thirdperson_ctx;
	detour_init(&thirdperson_ctx, Sigs::CThirdPersonManager_Update.GetPointer(),
		    (void *)&Hooked_CThirdPersonManager_Update);

	if (!detour_enable(&thirdperson_ctx))
		Logs::Error("Couldn't hook CThirdPersonManager::Update!");
}