#include "core.h"

#include <atomic>

#include "../sdk/signatures/signatures.h"

#include "../features/scriptmanager/scriptmanager.h"
#include "../features/chams/chams.h"
#include "../features/esp/esp.h"
#include "../features/glow/glow.h"
#include "../features/radar/radar.h"
#include "../features/ticks/ticks.h"
#include "../features/visuals/viewmodel_aim/viewmodel_aim.h"
#include "../features/visuals/viewmodel_interp/viewmodel_interp.h"
#include "../features/backtrack/backtrack.h"
#include "../features/materialregistry/reg.h"
#include "../features/playerlist/playerlist.h"
#include "../features/hook_initializer/initializer.h"
#include "../features/network/network.h"

#include "../gui/gui.h"
#include "../hooks.h"

CApp::CApp() : m_bInitialized(false)
{
}

bool CApp::IsInitialized()
{
	return m_bInitialized.load();
}

void CApp::Setup()
{
	// we are fucked
	if (!features::network.init())
		return;

	if (!Sigs_InitAll())
		return;

	Netvars::Setup();

	Settings::InitBinds();
	features::scriptmanager.Init();

	GUI::Init();
	features::ticks.Init();
	MaterialManager::Init();
	features::material_registry.Init();
	features::playerlist.Init();

	features::backtrack.Init();
	features::viewmodel_interp.Init();
	features::viewmodel_aim.Init();
	features::radar.Init();
	features::esp.Init();
	features::glow.Init();
	features::chams.Init();

	VMTHooks::Initialize();

	HookInitManager::get().init_all();

	m_bInitialized.store(true, std::memory_order_release);
}

CApp* gApp{nullptr};