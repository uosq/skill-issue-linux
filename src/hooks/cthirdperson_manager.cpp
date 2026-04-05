#include "cthirdperson_manager.h"

DETOUR_DECL_TYPE(void, CThirdPersonManager_Update, void *self);

void Hooked_CThirdPersonManager_Update()
{
	/*
	;)
	*/
}

void Hook_CThirdPersonManager_Update()
{
	detour_ctx_t thirdperson_ctx;
	detour_init(&thirdperson_ctx, Sigs::CThirdPersonManager_Update.GetPointer(),
		    (void *)&Hooked_CThirdPersonManager_Update);

	if (!detour_enable(&thirdperson_ctx))
		Logs::Error("Couldn't hook CThirdPersonManager::Update!");
}