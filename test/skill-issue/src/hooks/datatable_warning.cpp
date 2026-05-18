#include "datatable_warning.h"

#include "../sdk/interfaces/interfaces.h"
#include "../libdetour/libdetour.h"

DETOUR_DECL_TYPE(void, DataTable_Warning, const char *pInMessage, ...);
detour_ctx_t warning_ctx;

void Hooked_DataTable_Warning(const char *pInMessage, ...)
{
	/*
	do nothing
	*/
}

void Hook_DataTable_Warning(void)
{
	detour_init(&warning_ctx, Sigs::DataTable_Warning.GetPointer(), (void *)&Hooked_DataTable_Warning);
	if (!detour_enable(&warning_ctx))
	{
		interfaces::Cvar->ConsolePrintf("Failed to hook DataTable_Warning\n");
		return;
	}

#ifdef DEBUG
	interfaces::Cvar->ConsolePrintf("DataTable_Warning hooked\n");
#endif
}