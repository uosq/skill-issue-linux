#include "cbuf_executecommand.h"

#include "../sdk/definitions/convar.h"

#include "../libdetour/libdetour.h"

#include "../features/logs/logs.h"
//#include "../features/angelscript/api/libraries/hooks/hooks.h"

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
		Logs::Error("Couldn't hook Cbuf_ExecuteCommand");
}