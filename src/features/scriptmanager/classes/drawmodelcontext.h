#pragma once

#include "../../../sdk/definitions/types.h"

class CBaseEntity;
class IVModelRender;
struct DrawModelState_t;
struct ModelRenderInfo_t;

class DrawModelContext
{
public:
	CBaseEntity* GetEntity();
	bool IsBacktrack();
	bool IsChams();
	bool IsGlow();
	void CallOriginal();

	int entity_index;
	bool is_backtrack;
	bool is_glow;
	bool is_chams;

	IVModelRender* thisptr;
	DrawModelState_t &state;
	ModelRenderInfo_t &pInfo;
	matrix3x4* pCustomBoneToWorld;

	bool valid { false };
};