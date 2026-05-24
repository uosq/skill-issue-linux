#include "../../../sol3/sol.hpp"

#include "../../../sdk/classes/entity.h"
#include "../../../hooks.h"

#include "drawmodelcontext.h"

using DrawModelExecuteFn = void(*)
(
	IVModelRender *thisptr,
	const DrawModelState_t &state,
	const ModelRenderInfo_t &pInfo,
	matrix3x4 *pCustomBoneToWorld
);

CBaseEntity* DrawModelContext::GetEntity()
{
	if (!valid || entity_index < 0)
		return nullptr;

	IClientEntity* clientEntity = interfaces::EntityList->GetClientEntity(entity_index);

	if (clientEntity == nullptr)
		return nullptr;

	return static_cast<CBaseEntity*>(clientEntity);
}

bool DrawModelContext::IsBacktrack()
{
	return valid && is_backtrack;
}

bool DrawModelContext::IsChams()
{
	return valid && is_chams;
}

bool DrawModelContext::IsGlow()
{
	return valid && is_glow;
}

void DrawModelContext::CallOriginal()
{
	if (!valid) return;

	auto original = VMTHooks::ModelRender.GetOriginal<DrawModelExecuteFn>(19);
	original(thisptr, state, pInfo, pCustomBoneToWorld);
}

void BindDrawModelContext(sol::state& lua)
{
	lua.new_usertype<DrawModelContext>
	(
		"DrawModelContext", sol::no_constructor,

		"GetEntity", &DrawModelContext::GetEntity,
		"IsChams", &DrawModelContext::IsChams,
		"IsGlow", &DrawModelContext::IsGlow,
		"IsBacktrack", &DrawModelContext::IsBacktrack,
		"Execute", &DrawModelContext::CallOriginal
	);
}