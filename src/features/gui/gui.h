#include <SDL2/SDL.h>

#include "../../sdk/definitions/d3d9.h"
#include "../../sdk/definitions/ipanel.h"

#include "../../thirdparty/imgui/imgui.h"
#include "../../thirdparty/imgui/imgui_impl_dx9.h"

#include "../../features/feature.h"

class CGui
{
public:
	void shutdown();

	// sdl2 shit
	void on_swapwindow(SDL_Window* window);
	void on_pollevent(SDL_Event* event, int ret);
	
	// dxvk shit
	void on_present(IDirect3DDevice9* pDevice, const RECT* pSourceRect, const RECT* pDestRect, HWND hDestWindowOverride, const RGNDATA* pDirtyRegion);
	void on_reset(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters);
	void update_presentation_parameters(D3DPRESENT_PARAMETERS* pPresentationParameters);

	ImFont* arial = nullptr;
	ImFont* tf2_build = nullptr;
	HCursor cursor = 0;

// our gui
private:
	void render_windows();
	void begin_main_window();
	void end_main_window();
	void render_main_window_content();
	bool window_pushed_font = false;

// imgui
private:
	void init_imgui();
	void setup_default_style();

// sdl2
private:
	void sdl_process_event(SDL_Event* event, int ret);
	void sdl_new_frame(SDL_Window* window);
	void init_impl_imgui_sdl2();

// dxvk
private:
	void init_impl_imgui_dxvk();
	void dxvk_new_frame();
	D3DFORMAT get_back_buffer_format(IDirect3DDevice9* device);

// sdl2 backend variables
private:
	SDL_GLContext our_context = nullptr;
	SDL_GLContext game_context = nullptr;
	SDL_Window* game_window = nullptr;

// dxvk backend variables
private:
	IDirect3DDevice9* pd3dDevice = nullptr;
	D3DPRESENT_PARAMETERS d3dpp = {};
	bool dxvk_initialized = false;
};

DECLARE_FEATURE(CGui, gui)