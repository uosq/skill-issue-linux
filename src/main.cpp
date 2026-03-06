#include "hooks/enginevgui_paint.h"
#include "core/core.h"

__attribute__((constructor))
void init(void)
{
	gApp.App();
	gApp.StartInterfaces();

	HookEngineVGuiPaint();
}

__attribute__((destructor))
void uninit(void)
{
	Lua::ClosePluto();
}