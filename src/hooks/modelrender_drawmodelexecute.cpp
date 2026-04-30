#include "modelrender_drawmodelexecute.h"

#include "../sdk/definitions/ivmodelrender.h"
#include "../sdk/interfaces/interfaces.h"

#include "../hooks.h"

#include "../features/backtrack/backtrack.h"
#include "../features/chams/chams.h"
#include "../features/glow/glow.h"

#include "../features/angelscript/api/classes/drawmodelcontext/drawmodelcontext.h"

#include "../features/angelscript/api/libraries/hooks/hooks.h"

using DrawModelExecuteFn = void(*)(IVModelRender *thisptr,
                                   const DrawModelState_t &state,
                                   const ModelRenderInfo_t &pInfo,
                                   matrix3x4 *pCustomBoneToWorld);

static void DrawModelExecute(IVModelRender* thisptr, const DrawModelState_t &state, const ModelRenderInfo_t &pInfo, matrix3x4 *pCustomBoneToWorld)
{
	auto original = VMTHooks::ModelRender.GetOriginal<DrawModelExecuteFn>(19);

	if (AS_ShouldCallOriginal())
		return original(thisptr, state, pInfo, pCustomBoneToWorld);

	if (interfaces::Engine->IsTakingScreenshot())
		return original(thisptr, state, pInfo, pCustomBoneToWorld);

	if (Backtrack::m_drawing)
		return original(thisptr, state, pInfo, Backtrack::m_current_drawing_record->m_Bones);

	if (!Chams::IsDrawing() && !Glow::m_bRunning)
	{
		float color[3] = {1, 1, 1};
		interfaces::RenderView->SetColorModulation(color);
		interfaces::RenderView->SetBlend(1.0f);
		interfaces::ModelRender->ForcedMaterialOverride(nullptr);
	}

	{
		DrawModelContext ctx;
		ctx.valid	       = true;
		ctx.state	       = state;
		ctx.pCustomBoneToWorld = pCustomBoneToWorld;
		ctx.pInfo	       = pInfo;
		ctx.thisptr	       = thisptr;
		Hooks_CallHooks("DrawModel", [&](asIScriptContext *ctx) { ctx->SetArgObject(0, &ctx); });
	}

	if (Chams::IsDrawing() || Glow::m_bRunning)
		return original(thisptr, state, pInfo, pCustomBoneToWorld);

	if (pInfo.entity_index > 0 && (Chams::ShouldHide(pInfo.entity_index) || Glow::ShouldHide(pInfo.entity_index)))
		return;

	original(thisptr, state, pInfo, pCustomBoneToWorld);
}

void HookDrawModelExecute(void)
{
	VMTHooks::ModelRender.Hook(19, &DrawModelExecute);

#ifdef DEBUG
	constexpr Color_t color = {100, 255, 100, 255};
	helper::console::ColoredPrint("IModelRender::DrawModelExecute hooked\n", color);
#endif
}