#include "modelrender_drawmodelexecute.h"

#include "../sdk/definitions/ivmodelrender.h"
#include "../sdk/interfaces/interfaces.h"

#include "../hooks.h"

#include "../features/backtrack/backtrack.h"
#include "../features/chams/chams.h"
#include "../features/glow/glow.h"

#include "../features/scriptmanager/scriptmanager.h"
#include "../features/scriptmanager/classes/drawmodelcontext.h"

using DrawModelExecuteFn = void(*)(IVModelRender *thisptr,
                                   const DrawModelState_t &state,
                                   const ModelRenderInfo_t &pInfo,
                                   matrix3x4 *pCustomBoneToWorld);

static void DrawModelExecute(IVModelRender* thisptr, const DrawModelState_t &state, const ModelRenderInfo_t &pInfo, matrix3x4* pCustomBoneToWorld)
{
	auto original = VMTHooks::ModelRender.GetOriginal<DrawModelExecuteFn>(19);

	if (interfaces::Engine->IsTakingScreenshot())
		return original(thisptr, state, pInfo, pCustomBoneToWorld);

	{
		DrawModelState_t ctx_state = state;
		ModelRenderInfo_t ctx_info = pInfo;

		DrawModelContext ctx
		{
			pInfo.entity_index,
			features::backtrack.IsDrawing(),
			features::glow.IsRunning(),
			features::glow.IsRunning(),

			thisptr, ctx_state, ctx_info, pCustomBoneToWorld,

			true
		};

		features::scriptmanager.CallHooks("DrawModel", ctx);

		ctx.valid = false;
	}

	if (features::backtrack.IsDrawing())
		return original(thisptr, state, pInfo, features::backtrack.GetDrawingRecord()->m_Bones);

	if (!features::chams.IsDrawing() && !features::glow.IsRunning())
	{
		float color[3] = {1, 1, 1};
		interfaces::RenderView->SetColorModulation(color);
		interfaces::RenderView->SetBlend(1.0f);
		interfaces::ModelRender->ForcedMaterialOverride(nullptr);
	}

	features::scriptmanager.CallHooks("DrawModel");

	if (features::chams.IsDrawing() || features::glow.IsRunning())
		return original(thisptr, state, pInfo, pCustomBoneToWorld);

	if (pInfo.entity_index > 0 && (features::chams.ShouldHide(pInfo.entity_index) || features::glow.ShouldHide(pInfo.entity_index)))
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