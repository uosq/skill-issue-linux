#include "drawmodelcontext.h"

#include "../../../../chams/chams.h"
#include "../../../../glow/glow.h"

#define DMC_CLASSNAME "DrawModelContext"

bool AS_Calling_DrawModelExecute = false;

bool AS_ShouldCallOriginal()
{
	return AS_Calling_DrawModelExecute;
}

int GetEntityIndex(DrawModelContext *ctx)
{
	if (!ctx || !ctx->valid)
		return -1;

	return ctx->pInfo.entity_index;
}

bool IsChamsModel(DrawModelContext *ctx)
{
	if (!ctx || !ctx->valid)
		return false;

	return Chams::m_bRunning;
}

bool IsGlowModel(DrawModelContext *ctx)
{
	if (!ctx || !ctx->valid)
		return false;

	return Glow::m_bRunning;
}

void CallOriginal(DrawModelContext *ctx)
{
	if (!ctx || !ctx->valid)
		return;

	AS_Calling_DrawModelExecute = true;
	interfaces::ModelRender->DrawModelExecute(ctx->state, ctx->pInfo, ctx->pCustomBoneToWorld);
	AS_Calling_DrawModelExecute = false;
}

void DrawModelContext_RegisterClass(asIScriptEngine *engine)
{
	engine->RegisterObjectType(DMC_CLASSNAME, 0, asOBJ_REF | asOBJ_NOCOUNT);
	engine->RegisterObjectMethod(DMC_CLASSNAME, "int GetEntityIndex()", asFUNCTION(GetEntityIndex),
				     asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectMethod(DMC_CLASSNAME, "bool IsChams() const", asFUNCTION(IsChamsModel),
				     asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectMethod(DMC_CLASSNAME, "bool IsGlow() const", asFUNCTION(IsGlowModel),
				     asCALL_CDECL_OBJFIRST);
	engine->RegisterObjectMethod(DMC_CLASSNAME, "void Execute()", asFUNCTION(CallOriginal), asCALL_CDECL_OBJFIRST);
}