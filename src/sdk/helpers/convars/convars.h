#pragma once

#include "../../definitions/convar.h"
#include "../../interfaces/interfaces.h"

namespace ConVars
{
	extern ConVar* host_limitlocal;
	extern ConVar* sv_gravity;
	extern ConVar* tf_flamethrower_size;
	extern ConVar* tf_scout_stunball_base_speed;
	extern ConVar* sv_stopspeed_cvar;
	extern ConVar* sv_friction_cvar;
	extern ConVar* sv_airaccelerate;
	extern ConVar* sv_accelerate;
	extern ConVar* cl_cmdrate;
	extern ConVar* fov_desired;
	extern ConVar* default_fov;
	extern ConVar* sv_maxusrcmdprocessticks;

	bool Init(void);
}