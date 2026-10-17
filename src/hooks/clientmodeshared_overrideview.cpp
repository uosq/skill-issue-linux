#include "../sdk/classes/player.h"

#include "../features/entitylist/entitylist.h"
#include "../features/visuals/norecoil/norecoil.h"
#include "../features/visuals/thirdperson/thirdperson.h"

#include "../features/scriptmanager/scriptmanager.h"
#include "../features/visuals/customfov/customfov.h"
#include "../features/hookmanager/hookmanager.hpp"

using OverrideViewFn = void (*)(IClientMode* rdi, CViewSetup *pView);

INIT_VTABLE_HOOK(OverrideView, void, (IClientMode* rdi, CViewSetup* pView), interfaces::ClientDLL, 17)
{
	original_OverrideView(rdi, pView);

	if (pView == nullptr)
		return;

	features::scriptmanager.CallHooks("OverrideView", pView);

	if (CTFPlayer *pLocal = features::entities.GetLocal(); pLocal != nullptr)
	{
		features::thirdperson.OverrideView(pLocal, pView);
		features::norecoil.RunOverrideView(pLocal, pView);
		features::customfov.OnOverrideView(pLocal, pView);
	}
}
