#pragma once

#include "../sdk/interfaces/interfaces.h"
#include "../sdk/classes/entity.h"
#include "../sdk/classes/player.h"
#include "../sdk/helpers/helper.h"
#include "../settings/settings.h"
#include <string>

#include "../features/aimbot/aimbot.h"
#include "../features/visuals/viewmodel_interp/viewmodel_interp.h"
#include "../features/visuals/viewmodel_aim/viewmodel_aim.h"
#include "../features/visuals/viewmodel_offset/viewmodel_offset.h"

#include "../libdetour/libdetour.h"

#include "../features/angelscript/api/api.h"
#include "../features/angelscript/api/libraries/hooks/hooks.h"

inline detour_ctx_t calcViewModel_ctx;
DETOUR_DECL_TYPE(void, original_CalcViewModelView, void* thisptr, CBaseEntity*, const Vector&, const QAngle&);

static void AS_CalcViewModelView_Callback(Vector& eyePos, Vector& eyeAngles)
{
	std::vector<ASHook> hooks;
	if (!Hooks_GetHooks("CalcViewModelView", hooks))
		return;

	auto engine = API::GetScriptEngine();

	for (const auto& hook : hooks)
	{
		asIScriptContext* ctx = engine->RequestContext();

		ctx->Prepare(hook.func);
		ctx->SetArgObject(0, &eyePos);
		ctx->SetArgObject(1, &eyeAngles);
		ctx->Execute();

		engine->ReturnContext(ctx);
	}
}

inline void HookedCalcViewModelView(void* thisptr, CBaseEntity* owner, const Vector& eyePosition, const QAngle& eyeAngles)
{
	Vector angle = eyeAngles;
	Vector position = eyePosition;

	if (owner)
	{
		AS_CalcViewModelView_Callback(position, angle);

		ViewmodelInterp::Run(angle);
		ViewmodelAim::Run(angle);
		ViewmodelOffset::Run(position, angle);
	}

	DETOUR_ORIG_CALL(&calcViewModel_ctx, original_CalcViewModelView, thisptr, owner, position, angle);
}

inline void HookCalcViewModelView()
{
	void* original = (sigscan_module("client.so", "55 48 89 E5 41 57 41 56 41 55 49 89 F5 41 54 49 89 FC 53 48 83 EC 48 8B 41 08"));
	detour_init(&calcViewModel_ctx, original, (void*)&HookedCalcViewModelView);
	detour_enable(&calcViewModel_ctx);

	#ifdef DEBUG
	constexpr Color_t color{100, 255, 100, 255};
	interfaces::Cvar->ConsoleColorPrintf(color, "CalcViewModelView hooked\n");
	#endif
}