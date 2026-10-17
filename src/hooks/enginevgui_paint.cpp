#include "../sdk/definitions/ienginevgui.h"
#include "../sdk/interfaces/interfaces.h"

#include "../features/scriptmanager/scriptmanager.h"
#include "../features/hookmanager/hookmanager.hpp"

using VGuiPaintFn = void (*)(IEngineVGuiInternal *rdi, PaintMode_t paint);

INIT_VTABLE_HOOK(IEngineVGuiInternal_VGuiPaint, void, (IEngineVGuiInternal* rdi, PaintMode_t paint), interfaces::EngineVGui, 15)
{
	original_IEngineVGuiInternal_VGuiPaint(rdi, paint);

	if (interfaces::Engine->IsTakingScreenshot())
		return;

	if (paint & PAINT_UIPANELS)
	{
		interfaces::Surface->StartDrawing();

		features::scriptmanager.CallHooks("Draw");

		interfaces::Surface->FinishDrawing();
	}
}