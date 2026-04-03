#include "client.h"

#include "../../../../../sdk/interfaces/interfaces.h"
#include "../../globals.h"

ConVar *GetConVar(const std::string &convar)
{
	ConVar *cvar = interfaces::Cvar->FindVar(convar.c_str());
	if (!cvar)
		return nullptr;
	return cvar;
}

void RunCommand(const std::string &cmd, bool unrestricted)
{
	if (unrestricted)
		interfaces::Engine->ClientCmd(cmd.c_str());
	else
		interfaces::Engine->ClientCmd_Unrestricted(cmd.c_str());
}

void ClearConsole()
{
	interfaces::EngineVGui->ClearConsole();
}

void Client_RegisterLibrary(asIScriptEngine *engine)
{
	auto defNamespace = engine->GetDefaultNamespace();

	engine->SetDefaultNamespace("Client");
	{
		engine->SetDefaultAccessMask(ScriptAccessMask::SCRIPT_MASK_ALLOW_CLIENT);
		engine->RegisterGlobalFunction("ConVar@ GetConVar(const string &in name)", asFUNCTION(GetConVar),
					       asCALL_CDECL);
		engine->RegisterGlobalFunction("void Command(const string &in text, bool unrestricted = "
					       "false)",
					       asFUNCTION(RunCommand), asCALL_CDECL);
		engine->RegisterGlobalFunction("void ClearConsole()", asFUNCTION(ClearConsole), asCALL_CDECL);
	}

	engine->SetDefaultNamespace(defNamespace);
}