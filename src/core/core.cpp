#include "core.h"

#include "../sdk/interfaces/interfaces.h"
#include "../sdk/signatures/signatures.h"

#include "../features/angelscript/api/api.h"
#include "../features/chams/chams.h"
#include "../features/esp/esp.h"
#include "../features/glow/glow.h"
#include "../features/radar/radar.h"
#include "../features/ticks/ticks.h"
#include "../features/visuals/viewmodel_aim/viewmodel_aim.h"
#include "../features/visuals/viewmodel_interp/viewmodel_interp.h"

#include "../gui/gui.h"

#include "../hooks/calcviewmodelbobhelper.h"
#include "../hooks/cbaseanimating_interpolate.h"
#include "../hooks/cbaseentity_baseinterpolatepart1.h"
#include "../hooks/cbaseplayer_shoulddrawlocalplayer.h"
#include "../hooks/cbaseviewmodel_calcviewmodelview.h"
#include "../hooks/cgameclient_executestringcommand.h"
#include "../hooks/chlclient_levelinitpreentity.h"
#include "../hooks/chlclient_levelpostentity.h"
#include "../hooks/chlclient_levelshutdown.h"
#include "../hooks/cinput_getusercmd.h"
#include "../hooks/cinput_validadeusercmd.h"
#include "../hooks/cinventorymanager_showitemspickedup.h"
#include "../hooks/cl_checkforpureserverwhitelist.h"
#include "../hooks/cl_move.h"
#include "../hooks/clientmodeshared_dopostscreenspaceeffects.h"
#include "../hooks/clientmodeshared_firegameevent.h"
#include "../hooks/clientmodeshared_overrideview.h"
#include "../hooks/cmaterial_uncache.h"
#include "../hooks/cprediction_runcommand.h"
#include "../hooks/ctfplayer_getmaxitemcount.h"
#include "../hooks/ctfwearable_shoulddraw.h"
#include "../hooks/cthirdperson_manager.h"
#include "../hooks/datatable_warning.h"
#include "../hooks/dxvk.h"
#include "../hooks/framestagenotify.h"
#include "../hooks/host_shutdown.h"
#include "../hooks/iclientmode_shoulddrawviewmodel.h"
#include "../hooks/iinputsystem_sleepuntilinput.h"
#include "../hooks/ipanel_paint_traverse.h"
#include "../hooks/istudiorender_forcedmaterialoverride.h"
#include "../hooks/isurface_setcursor.h"
#include "../hooks/modelrender_drawmodelexecute.h"
#include "../hooks/netchan_sendnetmsg.h"
#include "../hooks/sdl.h"
//#include "../hooks/ctfmatchsummary_ontick.h"
//#include "../hooks/cbaseentity_shoulddraw.h"

CApp::CApp() : m_bInitialized(false)
{
}

bool CApp::IsInitialized()
{
	return m_bInitialized;
}

bool CApp::StartInterfaces()
{
	return InitializeInterfaces();
}

void CApp::Setup()
{
	if (!Sigs_InitAll())
		return;

	Netvars::Setup();

	Settings::InitBinds();
	API::Initialize();

	GUI::Init();
	TickManager::Init();
	MaterialManager::Init();
	FontManager::Init();

	Backtrack::Init();
	ViewmodelInterp::Init();
	ViewmodelAim::Init();
	Radar::Init();
	ESP::Init();
	Glow::Init();
	Chams::Init();

	StartHooks();
}

bool CApp::StartHooks()
{
	HookSDL();
	HookDXVK();

	//HookEngineVGuiPaint(); Already initialized
	HookFrameStageNotify();
	HookOverrideView();
	HookCalcViewModelView();
	HookCTFPlayerInventory_MaxItemCount();
	HookPaintTraverse();
	HookCheckForPure();
	HookDrawModelExecute();
	HookDoPostScreenSpaceEffects();
	HookLockCursor();
	HookLevelShutdown();
	HookShowItemsPickedUp();
	HookSendNetMsg();
	HookLevelInitPostEntity();
	HookLevelInitPreEntity();
	HookFireGameEvent();
	HookForcedMaterialOverride();
	HookHost_Shutdown();
	HookCMaterial_Uncache();
	Hook_ExecuteStringCommand();
	Hook_RunCommand();
	Hook_GetUserCmd();
	Hook_ValidateUserCmd();
	HookCL_Move();
	Hook_BaseInterpolatePart1();
	Hook_Interpolate();
	Hook_DataTable_Warning();
	Hook_CThirdPersonManager_Update();
	Hook_SleepUntilInput();
	Hook_CTFWearable_ShouldDraw();
	Hook_ShouldDrawViewModel();
	Hook_CBasePlayer_ShouldDrawLocalPlayer();
	Hook_CalcViewModelBobHelper();
	//Hook_CTFMatchSummary_OnTick();
	//Hook_CBaseEntity_ShouldDraw();

	m_bInitialized = true;
	return true;
}

CApp gApp{};