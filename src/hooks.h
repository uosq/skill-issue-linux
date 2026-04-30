#pragma once

#include "vmt.h"

namespace VMTHooks
{
	extern VMT Engine;
	extern VMT EngineVGui;
	extern VMT InputSystem;
	extern VMT Client;
	extern VMT ClientMode;
	extern VMT Surface;
	extern VMT ModelRender;
	extern VMT StudioRender;
	extern VMT CInput;
	extern VMT VGui;

	void Initialize();
	void Restore();
}