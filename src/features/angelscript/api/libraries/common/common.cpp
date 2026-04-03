#include "common.h"

#include "../../../../../sdk/interfaces/interfaces.h"
#include "../../globals.h"

void Print(const std::string &text)
{
	interfaces::Cvar->ConsolePrintf("%s\n", text.c_str());
}

void Warn(const std::string &text)
{
	Color_t color{255, 161, 0, 255};
	interfaces::Cvar->ConsoleColorPrintf(color, "%s\n", text.c_str());
}

bool Execute(const std::string &text)
{
	auto engine	     = GetScriptEngine();

	asIScriptModule *mod = engine->GetModule("code", asGM_ALWAYS_CREATE);

	mod->AddScriptSection("script", text.c_str());
	mod->SetAccessMask(GetScriptAccessMask());
	mod->Build();

	auto ctx  = GetScriptContext();
	auto func = mod->GetFunctionByDecl("void main()");

	if (!func)
	{
		interfaces::Cvar->ConsolePrintf("\"void main()\" function is missing!\n");
		return false;
	}

	ctx->Prepare(func);

	if (ctx->Execute() != asEXECUTION_FINISHED)
		interfaces::Cvar->ConsolePrintf("Execution could not finish!\n");

	ctx->Unprepare();
	return true;
}

void Common_RegisterLibrary(asIScriptEngine *engine)
{
	engine->SetDefaultAccessMask(ScriptAccessMask::SCRIPT_MASK_ALLOW_COMMON);
	engine->RegisterGlobalFunction("void print(const string &in text)", asFUNCTION(Print), asCALL_CDECL);
	engine->RegisterGlobalFunction("void warn(const string &in text)", asFUNCTION(Warn), asCALL_CDECL);
	engine->RegisterGlobalFunction("bool Execute(const string &in text)", asFUNCTION(Execute), asCALL_CDECL);
}