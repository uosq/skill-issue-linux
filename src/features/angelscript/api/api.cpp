#include "api.h"

#include "../../../sdk/interfaces/interfaces.h"

#define AS_USE_STLNAMES 1
#include "../add_on/datetime/datetime.h"
#include "../add_on/scriptarray/scriptarray.h"
#include "../add_on/scriptdictionary/scriptdictionary.h"
#include "../add_on/scriptfile/scriptfile.h"
#include "../add_on/scriptfile/scriptfilesystem.h"
#include "../add_on/scriptmath/scriptmath.h"
#include "../add_on/scriptmath/scriptmathcomplex.h"
#include "../add_on/scriptstdstring/scriptstdstring.h"
#include "classes/convar/convar.h"
#include "classes/drawmodelcontext/drawmodelcontext.h"
#include "classes/entity/entity.h"
#include "classes/gameevent/event.h"
#include "classes/material/mat.h"
#include "classes/texture/texture.h"
#include "classes/usercmd/usercmd.h"
#include "classes/vector/vector3.h"
#include "classes/vector2/vec2.h"
#include "classes/viewsetup/viewsetup.h"
#include "enums/enums.h"
#include "globals.h"
#include "libraries/client/client.h"
#include "libraries/clientstate/clientstate.h"
#include "libraries/common/common.h"
#include "libraries/draw/draw.h"
#include "libraries/engine/engine.h"
#include "libraries/entitylist/entitylist.h"
#include "libraries/hooks/hooks.h"
#include "libraries/imgui/imgui.h"
#include "libraries/input/input.h"
#include "libraries/materials/materials.h"
#include "libraries/render/render.h"

static bool s_bInitialized = false;

// I don't like singletons, but I couldn't
// think of a better way of doing it
asIScriptContext *API::GetScriptContext()
{
	return ::GetScriptContext();
}

asIScriptEngine *API::GetScriptEngine()
{
	return ::GetScriptEngine();
}

void MessageCallback(const asSMessageInfo *msg, void *param)
{
	const char *type = "ERR";
	if (msg->type == asMSGTYPE_WARNING)
		type = "WARN";
	else if (msg->type == asMSGTYPE_INFORMATION)
		type = "INFO";

	interfaces::Cvar->ConsolePrintf("%s (line: %d, column: %d): %s\n", type, msg->row, msg->col, msg->message);
}

void API::Initialize()
{
	if (s_bInitialized)
		return;

	auto engine = GetScriptEngine();

	engine->SetMessageCallback(asFUNCTION(MessageCallback), 0, asCALL_CDECL);

	{
		engine->SetDefaultAccessMask(SCRIPT_MASK_ALLOW_STRING);
		RegisterStdString(engine);

		engine->SetDefaultAccessMask(SCRIPT_MASK_ALLOW_ARRAY);
		RegisterScriptArray(engine, true);

		engine->SetDefaultAccessMask(SCRIPT_MASK_ALLOW_DICT);
		RegisterScriptDictionary(engine);

		engine->SetDefaultAccessMask(SCRIPT_MASK_ALLOW_MATH);
		RegisterScriptMath(engine);
		RegisterScriptMathComplex(engine);

		engine->SetDefaultAccessMask(SCRIPT_MASK_ALLOW_DATETIME);
		RegisterScriptDateTime(engine);

		engine->SetDefaultAccessMask(SCRIPT_MASK_ALLOW_FILESYSTEM);
		RegisterScriptFile(engine);
		RegisterScriptFileSystem(engine);
	}

	// enums
	Enums_Register(engine);

	// classes
	Vector2_RegisterClass(engine);
	Vector3_RegisterClass(engine);
	Entity_RegisterClass(engine);
	UserCmd_RegisterClass(engine);
	ViewSetup_RegisterClass(engine);
	Material_RegisterClass(engine);
	GameEvent_RegisterClass(engine);
	DrawModelContext_RegisterClass(engine);
	Texture_RegisterClass(engine);
	ConVar_RegisterClass(engine);

	// libraries
	Common_RegisterLibrary(engine);
	Engine_RegisterLibrary(engine);
	ClientState_RegisterLibrary(engine);
	Hooks_RegisterLibrary(engine);
	Draw_RegisterLibrary(engine);
	Materials_RegisterLibrary(engine);
	Interface_EntityList_Register(engine);
	Render_RegisterLibrary(engine);
	Input_RegisterLibrary(engine);
	Client_RegisterLibrary(engine);
	ImGui_RegisterLibrary(engine);

	s_bInitialized = true;
}

void API::Unitialize()
{
	GetScriptContext()->Release();
	GetScriptEngine()->ShutDownAndRelease();

	s_bInitialized = false;
}

bool API::IsInitialized()
{
	return s_bInitialized;
}