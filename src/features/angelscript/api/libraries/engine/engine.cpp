#include "engine.h"

#include "../../../../../sdk/helpers/engine/engine.h"
#include "../../../../../sdk/interfaces/interfaces.h"
#include "../../globals.h"

int GetMaxClients()
{
	return interfaces::Engine->GetMaxClients();
}

void ClientCmd(const std::string &cmd, bool unrestricted)
{
	if (unrestricted)
		interfaces::Engine->ClientCmd(cmd.c_str());
	else
		interfaces::Engine->ClientCmd_Unrestricted(cmd.c_str());
}

bool CON_IsVisible()
{
	return interfaces::Engine->Con_IsVisible();
}

void PlaySound(const std::string &sound)
{
	interfaces::Surface->PlaySound(sound.c_str());
}

bool IsInGame()
{
	return interfaces::Engine->IsInGame();
}

bool IsConnected()
{
	return interfaces::Engine->IsConnected();
}

Vector GetViewAngles()
{
	Vector viewangles;
	interfaces::Engine->GetViewAngles(viewangles);
	return viewangles;
}

void SetViewAngles(Vector &viewangle)
{
	interfaces::Engine->SetViewAngles(viewangle);
}

bool WorldToScreen(Vector worldAbsPos, Vec2 &outPos)
{
	return helper::engine::WorldToScreen(worldAbsPos, outPos);
}

bool IsGameUIVisible()
{
	return interfaces::EngineVGui->IsGameUIVisible();
}

bool IsConsoleVisible()
{
	return interfaces::EngineVGui->IsConsoleVisible();
}

bool IsTakingScreenshot()
{
	return interfaces::Engine->IsTakingScreenshot();
}

void Engine_RegisterLibrary(asIScriptEngine *engine)
{
	const char *strDefaultNamespace = engine->GetDefaultNamespace();

	engine->SetDefaultNamespace("Engine");
	engine->SetDefaultAccessMask(ScriptAccessMask::SCRIPT_MASK_ALLOW_ENGINE);
	engine->RegisterGlobalFunction("void PlaySound(const string &in sound)", asFUNCTION(PlaySound), asCALL_CDECL);
	engine->RegisterGlobalFunction("void ClientCmd(const string &in cmd, bool unrestricted)", asFUNCTION(ClientCmd),
				       asCALL_CDECL);
	engine->RegisterGlobalFunction("int GetMaxClients()", asFUNCTION(GetMaxClients), asCALL_CDECL);
	engine->RegisterGlobalFunction("bool IsInGame()", asFUNCTION(IsInGame), asCALL_CDECL);
	engine->RegisterGlobalFunction("bool IsConnected()", asFUNCTION(IsConnected), asCALL_CDECL);
	engine->RegisterGlobalFunction("Vector3 GetViewAngles()", asFUNCTION(GetViewAngles), asCALL_CDECL);
	engine->RegisterGlobalFunction("void SetViewAngles(Vector3 &in angle)", asFUNCTION(SetViewAngles),
				       asCALL_CDECL);
	engine->RegisterGlobalFunction("bool WorldToScreen(const Vector3 &in "
				       "worldPos, Vector2 &out outPos)",
				       asFUNCTION(WorldToScreen), asCALL_CDECL);
	engine->RegisterGlobalFunction("bool IsGameUIVisible()", asFUNCTION(IsGameUIVisible), asCALL_CDECL);
	engine->RegisterGlobalFunction("bool IsConsoleVisible()", asFUNCTION(IsConsoleVisible), asCALL_CDECL);
	engine->RegisterGlobalFunction("bool IsTakingScreenshot()", asFUNCTION(IsTakingScreenshot), asCALL_CDECL);

	engine->SetDefaultNamespace(strDefaultNamespace);
}