#pragma once

#include "../../../../../sdk/definitions/ivmodelrender.h"
#include "../../../include/angelscript.h"

struct DrawModelContext
{
	IVModelRender *thisptr;
	DrawModelState_t state;
	ModelRenderInfo_t pInfo;
	matrix3x4 *pCustomBoneToWorld;
	bool valid = false;
};

void DrawModelContext_RegisterClass(asIScriptEngine *engine);
bool AS_ShouldCallOriginal();