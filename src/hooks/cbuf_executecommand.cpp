#include "../sdk/definitions/convar.h"
#include "../sdk/signatures/signatures.h"

#include "../thirdparty/libdetour/libdetour.h"

#include "../features/logs/logs.h"
#include "../features/hook_initializer/initializer.h"

ADD_SIG(Cbuf_ExecuteCommand, "engine.so", "55 48 89 E5 41 54 53 48 8B 1D ? ? ? ? 48 8B 03 48 85 C0")

DETOUR_DECL_TYPE(void, Cbuf_ExecuteCommand, const CCommand& args, cmd_source_t source);
detour_ctx_t ctx;

void Hooked_Cbuf_ExecuteCommand(const CCommand& args, cmd_source_t source)
{
	// always nothing? wtf
	//if (args.ArgC() > 0)
		//interfaces::Cvar->ConsolePrintf("Command: %s\n", args.ArgS());

	DETOUR_ORIG_CALL(&ctx, Cbuf_ExecuteCommand, args, source);
}

void Hook_Cbuf_ExecuteCommand()
{
	detour_init(&ctx, Sigs::Cbuf_ExecuteCommand.GetPointer(), (void*)&Hooked_Cbuf_ExecuteCommand);
	if (!detour_enable(&ctx))
		features::logs.Error("Couldn't hook Cbuf_ExecuteCommand");
}

MARK_FOR_INIT(Hook_Cbuf_ExecuteCommand)