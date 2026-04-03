#include "core/core.h"
#include "hooks/enginevgui_paint.h"

__attribute__((constructor)) void init(void)
{
	gApp.StartInterfaces();

	HookEngineVGuiPaint();
}