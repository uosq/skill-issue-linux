#include "features/chams/chams.h"
#include "features/esp/esp.h"
#include "features/glow/glow.h"
#include "features/radar/radar.h"
#include "features/visuals/viewmodel_aim/viewmodel_aim.h"
#include "features/visuals/viewmodel_interp/viewmodel_interp.h"
#include "hooks/cgameclient_executestringcommand.h"
#include "hooks/cinput_getusercmd.h"
#include "hooks/cinput_validadeusercmd.h"
#include "hooks/cinventorymanager_showitemspickedup.h"
#include "hooks/cl_move.h"
#include "hooks/clientmodeshared_firegameevent.h"
#include "hooks/cmaterial_uncache.h"
#include "hooks/ctfplayer_getmaxitemcount.h"
#include "hooks/host_shutdown.h"
#include "hooks/istudiorender_forcedmaterialoverride.h"
#include "hooks/isurface_setcursor.h"
#include "hooks/modelrender_drawmodelexecute.h"
#include "hooks/enginevgui_paint.h"
#include "hooks/framestagenotify.h"
#include "hooks/clientmodeshared_overrideview.h"
#include "hooks/cbaseviewmodel_calcviewmodelview.h"
#include "hooks/cl_checkforpureserverwhitelist.h"
#include "hooks/clientmodeshared_dopostscreenspaceeffects.h"
#include "hooks/chlclient_levelshutdown.h"
#include "hooks/ipanel_paint_traverse.h"
#include "hooks/netchan_sendnetmsg.h"
#include "hooks/sdl.h"
#include "hooks/chlclient_levelinitpreentity.h"
#include "hooks/chlclient_levelpostentity.h"
#include "hooks/dxvk.h"

#include "sdk/FontManager/fontmanager.h"
#include "sdk/MaterialManager/materialmanager.h"
#include "sdk/interfaces/interfaces.h"
#include <sys/types.h>
#include <unistd.h>
#include "sdk/netvars/netvar.h"
#include "features/lua/api.h"

__attribute__((constructor))
void init(void)
{
	if (!InitializeInterfaces())
		return;

	MaterialManager::Init();
	FontManager::Init();
	Settings::RegisterOptions();

	ViewmodelInterp::Init();
	ViewmodelAim::Init();
	Radar::Init();
	ESP::Init();
	Glow::Init();
	Chams::Init();

	HookSDL();
	//HookVulkan();
	HookDXVK();

	Netvars::Setup();
	//SetupNetVarsToFile();

	Lua::InitPluto();

	HookEngineVGuiPaint();
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
	Hook_GetUserCmd();
	Hook_ValidateUserCmd();
	HookCL_Move();
	//Hook_CreateMove();
}

__attribute__((destructor))
void uninit(void)
{
	Lua::ClosePluto();
}