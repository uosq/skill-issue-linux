#pragma once

#include "../sdk/classes/entity.h"
#include "../sdk/classes/player.h"
#include "../sdk/definitions/ivmodelrender.h"
#include "../sdk/definitions/keyvalues.h"
#include "../sdk/helpers/helper.h"
#include "../sdk/interfaces/interfaces.h"

#include "../features/backtrack/backtrack.h"
#include "../features/chams/chams.h"
#include "../features/glow/glow.h"

#include "../features/angelscript/api/classes/drawmodelcontext/drawmodelcontext.h"

#include "../features/angelscript/api/api.h"
#include "../features/angelscript/api/libraries/hooks/hooks.h"

DECLARE_VTABLE_HOOK(DrawModelExecute, void,
		    (IVModelRender * thisptr, const DrawModelState_t &state, const ModelRenderInfo_t &pInfo,
		     matrix3x4 *pCustomBoneToWorld))
{
	//if (settings.esp.chams && !Chams::m_bRunning && Chams::ShouldHide(pInfo.entity_index))
	//return;

	if (AS_ShouldCallOriginal())
		return originalDrawModelExecute(thisptr, state, pInfo, pCustomBoneToWorld);

	if (interfaces::Engine->IsTakingScreenshot())
		return originalDrawModelExecute(thisptr, state, pInfo, pCustomBoneToWorld);

	if (Backtrack::m_drawing)
		return originalDrawModelExecute(thisptr, state, pInfo, Backtrack::m_current_drawing_record->m_Bones);

	if (!Chams::m_bRunning && !Glow::m_bRunning)
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

	if (Chams::m_bRunning || Glow::m_bRunning)
		return originalDrawModelExecute(thisptr, state, pInfo, pCustomBoneToWorld);

	if (Chams::ShouldHide(pInfo.entity_index) || Glow::ShouldHide(pInfo.entity_index))
		return;

	originalDrawModelExecute(thisptr, state, pInfo, pCustomBoneToWorld);
}

#if 0
static int LuaCallDME(lua_State* L)
{
	originalDrawModelExecute(ctx.thisptr, ctx.state, ctx.pInfo, ctx.pCustomBoneToWorld);
	return 0;
}
#endif

inline void HookDrawModelExecute(void)
{
	INSTALL_VTABLE_HOOK(DrawModelExecute, interfaces::ModelRender, 19);

#ifdef DEBUG
	constexpr Color_t color = {100, 255, 100, 255};
	helper::console::ColoredPrint("IModelRender::DrawModelExecute hooked\n", color);
#endif
}