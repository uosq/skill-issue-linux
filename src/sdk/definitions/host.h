#pragma once

#include "cmodel.h"
class CCommonHostState
{
public:
	model_t		*worldmodel;	// cl_entitites[0].model
	struct worldbrushdata_t *worldbrush;
	float		interval_per_tick;		// Tick interval for game
	void SetWorldModel( model_t *pModel );
};