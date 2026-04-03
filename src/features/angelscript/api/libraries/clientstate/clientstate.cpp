#include "clientstate.h"

#include "../../globals.h"

#include "../../../../../sdk/definitions/cclientstate.h"
#include "../../../../../sdk/interfaces/interfaces.h"
#include "../../../../logs/logs.h"

int GetChokedCommands()
{
	return static_cast<CClientState *>(interfaces::ClientState)->chokedcommands;
}

int GetLastOutgoingCommand()
{
	return static_cast<CClientState *>(interfaces::ClientState)->lastoutgoingcommand;
}

int GetLastCommandAck()
{
	return static_cast<CClientState *>(interfaces::ClientState)->last_command_ack;
}

int GetClientSignonState()
{
	return static_cast<CClientState *>(interfaces::ClientState)->m_nSignonState;
}

int GetDeltaTick()
{
	return static_cast<CClientState *>(interfaces::ClientState)->m_nDeltaTick;
}

void ForceFullUpdate()
{
	// xref: Requesting full game update
	if (interfaces::ClientState == nullptr)
		return Logs::Error("ClientState is null!");

	// I should probably add this to the clientstate def
	// But I don't use it anywhere in the code sooo

	using ForceFullUpdateFn = void (*)(CClientState *self);
	static ForceFullUpdateFn original_ForceFullUpdate =
	    reinterpret_cast<ForceFullUpdateFn>(sigscan_module("engine.so", "83 BF B8 01 00 00 FF 74 ? 55"));
	if (original_ForceFullUpdate == nullptr)
		return Logs::Error("ForceFullUpdate is null!");

	original_ForceFullUpdate(reinterpret_cast<CClientState *>(interfaces::ClientState));
}

void ClientState_RegisterLibrary(asIScriptEngine *engine)
{
	auto defNamespace = engine->GetDefaultNamespace();

	engine->SetDefaultNamespace("ClientState");
	{
		engine->SetDefaultAccessMask(ScriptAccessMask::SCRIPT_MASK_ALLOW_CLIENTSTATE);
		engine->RegisterGlobalFunction("int GetChokedCommands()", asFUNCTION(GetChokedCommands), asCALL_CDECL);
		engine->RegisterGlobalFunction("int GetLastOutgoingCommand()", asFUNCTION(GetLastOutgoingCommand),
					       asCALL_CDECL);
		engine->RegisterGlobalFunction("int GetLastCommandAck()", asFUNCTION(GetLastCommandAck), asCALL_CDECL);
		engine->RegisterGlobalFunction("int GetClientSignonState()", asFUNCTION(GetClientSignonState),
					       asCALL_CDECL);
		engine->RegisterGlobalFunction("int GetDeltaTick()", asFUNCTION(GetDeltaTick), asCALL_CDECL);
		engine->RegisterGlobalFunction("void ForceFullUpdate()", asFUNCTION(ForceFullUpdate), asCALL_CDECL);
	}

	engine->SetDefaultNamespace(defNamespace);
}