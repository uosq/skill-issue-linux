#include "convars.h"

#define REG_CONVAR(name) \
	name = interfaces::Cvar->FindVar(#name); \
	if (name == nullptr) \
		return false; \

namespace ConVars
{
	ConVar* host_limitlocal = nullptr;
	ConVar* sv_gravity = nullptr;
	ConVar* tf_flamethrower_size = nullptr;
	ConVar* tf_scout_stunball_base_speed = nullptr;
	ConVar* sv_stopspeed_cvar = nullptr;
	ConVar* sv_friction_cvar = nullptr;
	ConVar* sv_airaccelerate = nullptr;
	ConVar* sv_accelerate = nullptr;
	ConVar* cl_cmdrate = nullptr;
	ConVar* fov_desired = nullptr;
	ConVar* default_fov = nullptr;
	ConVar* sv_maxusrcmdprocessticks = nullptr;
}

bool ConVars::Init(void)
{
	REG_CONVAR(host_limitlocal)
	REG_CONVAR(sv_gravity)
	REG_CONVAR(tf_flamethrower_size)
	REG_CONVAR(tf_scout_stunball_base_speed)
	REG_CONVAR(sv_stopspeed_cvar)
	REG_CONVAR(sv_friction_cvar)
	REG_CONVAR(sv_airaccelerate)
	REG_CONVAR(sv_accelerate)
	REG_CONVAR(cl_cmdrate)
	REG_CONVAR(fov_desired)
	REG_CONVAR(default_fov)
	REG_CONVAR(sv_maxusrcmdprocessticks)

	return true;
}