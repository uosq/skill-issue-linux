#include "cgameclient_executestringcommand.h"

#include "../libdetour/libdetour.h"

#include "../sdk/interfaces/interfaces.h"

DETOUR_DECL_TYPE(bool, ExecuteStringCommand, void *self, const char *pCommandString);
detour_ctx_t execstringcmd_ctx;

bool HookedExecuteStringCommand(void *self, const char *pCommandString)
{
	//std::string cmd = pCommandString;

	bool ret;
	DETOUR_ORIG_GET(&execstringcmd_ctx, ret, ExecuteStringCommand, self, pCommandString);

	return ret;
}

void Hook_ExecuteStringCommand()
{
	detour_init(&execstringcmd_ctx, Sigs::CGameClient_ExecuteStringCommand.GetPointer(),
		    (void *)&HookedExecuteStringCommand);
	if (!detour_enable(&execstringcmd_ctx))
	{
		interfaces::Cvar->ConsolePrintf("Failed to hook CGameClient::ExecuteStringCommand\n");
		return;
	}

#ifdef DEBUG
	interfaces::Cvar->ConsolePrintf("CGameClient::ExecuteStringCommand hooked\n");
#endif
}