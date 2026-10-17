#include <SDL2/SDL_events.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_video.h>
#include <dlfcn.h>

#include "../thirdparty/imgui/imgui_impl_sdl2.h"

#include "../features/gui/gui.h"
#include "../features/hookmanager/hookmanager.hpp"

INIT_DLSYM_HOOK(SwapWindow, void, (SDL_Window* window), "libSDL2-2.0.so.0", "SDL_GL_SwapWindow")
{
	features::gui.on_swapwindow(window);

	original_SwapWindow(window);
}

INIT_DLSYM_HOOK(PollEvent, int, (SDL_Event* event), "libSDL2-2.0.so.0", "SDL_PollEvent")
{
	int ret = original_PollEvent(event);
	
	if (ret != 0)
		features::gui.on_pollevent(event, ret);

	return ret;
}
