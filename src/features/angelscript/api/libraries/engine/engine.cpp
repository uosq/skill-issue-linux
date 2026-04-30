#include "engine.h"

#include "../../../../../sdk/helpers/engine/engine.h"
#include "../../../../../sdk/interfaces/interfaces.h"

#include "../../globals.h"

#include "../../classes/tracefilter/tracefilter.h"

static int GetMaxClients()
{
	return interfaces::Engine->GetMaxClients();
}

static void ClientCmd(const std::string &cmd, bool unrestricted)
{
	if (unrestricted)
		interfaces::Engine->ClientCmd(cmd.c_str());
	else
		interfaces::Engine->ClientCmd_Unrestricted(cmd.c_str());
}

static void PlaySound(const std::string &sound)
{
	interfaces::Surface->PlaySound(sound.c_str());
}

static bool IsInGame()
{
	return interfaces::Engine->IsInGame();
}

static bool IsConnected()
{
	return interfaces::Engine->IsConnected();
}

static Vector GetViewAngles()
{
	Vector viewangles;
	interfaces::Engine->GetViewAngles(viewangles);
	return viewangles;
}

static void SetViewAngles(Vector &viewangle)
{
	interfaces::Engine->SetViewAngles(viewangle);
}

static bool WorldToScreen(Vector worldAbsPos, Vec2 &outPos)
{
	return helper::engine::WorldToScreen(worldAbsPos, outPos);
}

static bool IsGameUIVisible()
{
	return interfaces::EngineVGui->IsGameUIVisible();
}

static bool IsConsoleVisible()
{
	return interfaces::EngineVGui->IsConsoleVisible();
}

static bool IsTakingScreenshot()
{
	return interfaces::Engine->IsTakingScreenshot();
}

static void TraceFn(Vector start, Vector end, unsigned int mask, asIScriptFunction* fn, CGameTrace* pTrace)
{
	ScriptTraceFilter filter(fn);
	helper::engine::Trace(start, end, mask, &filter, pTrace);
}

static void TraceHullFn(Vector start, Vector end, Vector hullmin, Vector hullmax, unsigned int mask, asIScriptFunction* fn, CGameTrace *pTrace)
{
	ScriptTraceFilter filter(fn);
	helper::engine::TraceHull(start, end, hullmin, hullmax, mask, &filter, pTrace);
}

void Engine_RegisterLibrary(asIScriptEngine *engine)
{
	engine->RegisterFuncdef("bool TraceFilterFn(Entity@ ent, int contents)");

	const char *strDefaultNamespace = engine->GetDefaultNamespace();

	engine->SetDefaultNamespace("Engine");
	{
		engine->SetDefaultAccessMask(ScriptAccessMask::SCRIPT_MASK_ALLOW_ENGINE);
		engine->RegisterGlobalFunction("void PlaySound(const string &in sound)", asFUNCTION(PlaySound), asCALL_CDECL);
		engine->RegisterGlobalFunction("void ClientCmd(const string &in cmd, bool unrestricted)", asFUNCTION(ClientCmd), asCALL_CDECL);
		engine->RegisterGlobalFunction("int GetMaxClients()", asFUNCTION(GetMaxClients), asCALL_CDECL);
		engine->RegisterGlobalFunction("bool IsInGame()", asFUNCTION(IsInGame), asCALL_CDECL);
		engine->RegisterGlobalFunction("bool IsConnected()", asFUNCTION(IsConnected), asCALL_CDECL);
		engine->RegisterGlobalFunction("Vector3 GetViewAngles()", asFUNCTION(GetViewAngles), asCALL_CDECL);
		engine->RegisterGlobalFunction("void SetViewAngles(Vector3 &in angle)", asFUNCTION(SetViewAngles), asCALL_CDECL);
		engine->RegisterGlobalFunction("bool WorldToScreen(const Vector3 &in worldPos, Vector2 &out outPos)", asFUNCTION(WorldToScreen), asCALL_CDECL);
		engine->RegisterGlobalFunction("bool IsGameUIVisible()", asFUNCTION(IsGameUIVisible), asCALL_CDECL);
		engine->RegisterGlobalFunction("bool IsConsoleVisible()", asFUNCTION(IsConsoleVisible), asCALL_CDECL);
		engine->RegisterGlobalFunction("bool IsTakingScreenshot()", asFUNCTION(IsTakingScreenshot), asCALL_CDECL);
		engine->RegisterGlobalFunction("void TraceLine(Vector3 start, Vector3 end, uint mask, TraceFilterFn@ filter, Trace &out trace)", asFUNCTION(TraceFn), asCALL_CDECL);
		engine->RegisterGlobalFunction("void TraceHull(Vector3 start, Vector3 end, Vector3 min, Vector3 max, uint mask, TraceFilterFn@ filter, Trace &out trace)", asFUNCTION(TraceHullFn), asCALL_CDECL);
	}

	engine->SetDefaultNamespace(strDefaultNamespace);
}