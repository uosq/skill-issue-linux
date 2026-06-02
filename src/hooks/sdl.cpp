#include "../core/core.h"

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_video.h>
#include <dlfcn.h>

#include "../sdk/interfaces/interfaces.h"

#include "../thirdparty/libdetour/libdetour.h"
#include "../thirdparty/imgui/imgui_impl_sdl2.h"

#include "../features/gui/gui.h"

#include "../features/hook_initializer/initializer.h"

DETOUR_DECL_TYPE(void, original_SwapWindow, SDL_Window *window);
DETOUR_DECL_TYPE(int, original_PollEvent, SDL_Event *event);
DETOUR_DECL_TYPE(int, original_GetWindowSize, SDL_Window *window, int *w, int *h);

detour_ctx_t swapdetour;
detour_ctx_t polldetour;
detour_ctx_t windowsizedetour;

void Hooked_SwapWindow(SDL_Window *window)
{
	features::gui.on_swapwindow(window);
	DETOUR_ORIG_CALL(&swapdetour, original_SwapWindow, window);
}

int Hooked_PollEvent(SDL_Event *event)
{
	int ret = 0;
	DETOUR_ORIG_GET(&polldetour, ret, original_PollEvent, event);

	if ( !gApp->IsInitialized() || ret == 0 )
		return ret;

	features::gui.on_pollevent(event, ret);

	return ret;
}

void HookSDL()
{
	void *sdl = dlopen("libSDL2-2.0.so.0", RTLD_LAZY | RTLD_NOLOAD);
	if (sdl == nullptr)
		return interfaces::Cvar->ConsolePrintf("Couldn't load SDL2\n");

	void *original_SwapWindow = dlsym(sdl, "SDL_GL_SwapWindow");
	if (original_SwapWindow == nullptr)
		return interfaces::Cvar->ConsolePrintf("Couldn't get SwapWindow\n");

	void *original_PollEvent = dlsym(sdl, "SDL_PollEvent");
	if (original_PollEvent == nullptr)
		return interfaces::Cvar->ConsolePrintf("Couldn't get PollEvent\n");

	detour_init(&swapdetour, original_SwapWindow, (void *)&Hooked_SwapWindow);
	if (!detour_enable(&swapdetour))
		return interfaces::Cvar->ConsolePrintf("Couldn't hook SwapWindow\n");

	detour_init(&polldetour, original_PollEvent, (void *)&Hooked_PollEvent);
	if (!detour_enable(&polldetour))
		return interfaces::Cvar->ConsolePrintf("Couldn't hook PollEvent\n");

#ifdef DEBUG
	interfaces::Cvar->ConsolePrintf("SDL2 hooked\n");
#endif
}

MARK_FOR_INIT(HookSDL)