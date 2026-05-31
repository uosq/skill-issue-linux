#include "../thirdparty/libdetour/libdetour.h"

#include "../sdk/interfaces/interfaces.h"
#include "../sdk/signatures/signatures.h"

#include "../features/hook_initializer/initializer.h"

// ExecuteStringCommand
// engine.so 55 48 89 E5 41 57 41 56 4C 8D B5 C0 F9 FF FF
ADD_SIG(CGameClient_ExecuteStringCommand, "engine.so", "55 48 89 E5 41 57 41 56 4C 8D B5 C0 F9 FF FF")

DETOUR_DECL_TYPE(bool, ExecuteStringCommand, void *self, const char *pCommandString);
detour_ctx_t execstringcmd_ctx;

static bool HookedExecuteStringCommand(void *self, const char *pCommandString)
{
	//std::string cmd = pCommandString;

	bool ret;
	DETOUR_ORIG_GET(&execstringcmd_ctx, ret, ExecuteStringCommand, self, pCommandString);

	return ret;
}

static void Hook_ExecuteStringCommand()
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

MARK_FOR_INIT(Hook_ExecuteStringCommand)