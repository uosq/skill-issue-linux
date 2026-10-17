#include <thread>
#include <chrono>

#include "features/MaterialManager/materialmanager.h"
#include "features/backtrack/backtrack.h"
#include "features/chams/chams.h"
#include "features/glow/glow.h"
#include "features/hookmanager/hookmanager.hpp"
#include "features/network/network.h"
#include "features/playerlist/playerlist.h"
#include "features/radar/radar.h"
#include "features/scriptmanager/scriptmanager.h"
#include "features/ticks/ticks.h"
#include "features/visuals/viewmodel_aim/viewmodel_aim.h"
#include "features/visuals/viewmodel_interp/viewmodel_interp.h"
#include "sdk/interfaces/interfaces.h"
#include "sdk/netvars/netvar.h"
#include "sdk/signatures/signatures.h"

static void InitThread()
{
	while (true)
	{
		if (init_factories() && init_interfaces() && init_global_interfaces())
			break;

		std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}

	// we're fucked
	if (!features::network.init())
		return;

	if (!Sigs_InitAll())
		return;

	Netvars::Setup();

	features::scriptmanager.Init();

	features::ticks.Init();
	features::materials.Init();
	features::playerlist.Init();

	features::backtrack.Init();
	features::viewmodel_interp.Init();
	features::viewmodel_aim.Init();
	features::radar.Init();
	features::esp.Init();
	features::glow.Init();
	features::chams.Init();

	CHookManager::LoadAllHooks();
}

extern "C" void __attribute__((visibility("default"))) InitSkillIssue()
{
	std::thread init_thread(InitThread);
	init_thread.detach();
}