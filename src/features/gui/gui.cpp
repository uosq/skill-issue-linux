#include "gui.h"

#include "../../core/core.h"

#include "../../thirdparty/imgui/imgui_impl_opengl3.h"
#include "../../thirdparty/imgui/imgui_impl_sdl2.h"

#include "../../sdk/interfaces/interfaces.h"

#include "../config/config.h"
#include "../binds/binds.h"
#include "../logs/logs.h"
#include "../spectators/spectators.h"
#include "../esp/esp.h"
#include "../aimbot/aimbot.h"
#include "../spyalert/spyalert.h"
#include "../playerlist/playerlist.h"
#include "../radar/radar.h"
#include "../warp/warp.h"
#include "../scriptmanager/scriptmanager.h"
#include "../esp/esp_utils.h"

extern const unsigned int Arial_compressed_size;
extern const unsigned char Arial_compressed_data[668199];

extern void DrawAimbotTab();
extern void DrawESPTab();
extern void DrawMiscTab();
extern void DrawTriggerTab();
extern void DrawCodeTab();
extern void DrawNetvarsTab();
extern void DrawRadarTab();
extern void DrawConfigTab();
extern void DrawLogsTab();
extern void DrawMaterialEditor();

void CGui::init_imgui()
{
	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	setup_default_style();
}

void CGui::init_impl_imgui_sdl2()
{
	ImGui_ImplOpenGL3_Init("#version 100");
	ImGui_ImplSDL2_InitForOpenGL(game_window, nullptr);
}

void CGui::setup_default_style()
{
	ImGuiIO &io = ImGui::GetIO();
	io.ConfigWindowsMoveFromTitleBarOnly = true;

	// fonts
	tf2_build = io.Fonts->AddFontFromFileTTF("tf/resource/tf2build.ttf", 16.0f);
	arial = io.Fonts->AddFontFromMemoryCompressedTTF(Arial_compressed_data, Arial_compressed_size);

	// Fork of Rounded Visual Studio style from ImThemes
	ImGuiStyle &style			     = ImGui::GetStyle();

	style.Alpha				     = 1.0f;
	style.DisabledAlpha			     = 0.6f;
	style.WindowPadding			     = ImVec2(8.0f, 8.0f);
	style.WindowRounding			     = 4.0f;
	style.WindowBorderSize			     = 0.0f;
	style.WindowMinSize			     = ImVec2(32.0f, 32.0f);
	style.WindowTitleAlign			     = ImVec2(0.5f, 0.5f);
	style.WindowMenuButtonPosition		     = ImGuiDir_Left;
	style.ChildRounding			     = 0.0f;
	style.ChildBorderSize			     = 1.0f;
	style.PopupRounding			     = 4.0f;
	style.PopupBorderSize			     = 1.0f;
	style.FramePadding			     = ImVec2(4.0f, 3.0f);
	style.FrameRounding			     = 2.5f;
	style.FrameBorderSize			     = 0.0f;
	style.ItemSpacing			     = ImVec2(8.0f, 4.0f);
	style.ItemInnerSpacing			     = ImVec2(4.0f, 4.0f);
	style.CellPadding			     = ImVec2(4.0f, 2.0f);
	style.IndentSpacing			     = 21.0f;
	style.ColumnsMinSpacing			     = 6.0f;
	style.ScrollbarSize			     = 11.0f;
	style.ScrollbarRounding			     = 2.5f;
	style.GrabMinSize			     = 10.0f;
	style.GrabRounding			     = 2.0f;
	style.TabRounding			     = 3.5f;
	style.TabBorderSize			     = 0.0f;
	style.ColorButtonPosition		     = ImGuiDir_Right;
	style.ButtonTextAlign			     = ImVec2(0.5f, 0.5f);
	style.SelectableTextAlign		     = ImVec2(0.0f, 0.0f);

	style.Colors[ImGuiCol_Text]		     = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_TextDisabled]	     = ImVec4(0.5921569f, 0.5921569f, 0.5921569f, 1.0f);
	style.Colors[ImGuiCol_WindowBg]		     = ImVec4(0.14509805f, 0.14509805f, 0.14901961f, 1.0f);
	style.Colors[ImGuiCol_ChildBg]		     = ImVec4(0.14509805f, 0.14509805f, 0.14901961f, 1.0f);
	style.Colors[ImGuiCol_PopupBg]		     = ImVec4(0.14509805f, 0.14509805f, 0.14901961f, 1.0f);
	style.Colors[ImGuiCol_Border]		     = ImVec4(0.30588236f, 0.30588236f, 0.30588236f, 1.0f);
	style.Colors[ImGuiCol_BorderShadow]	     = ImVec4(0.30588236f, 0.30588236f, 0.30588236f, 1.0f);
	style.Colors[ImGuiCol_FrameBg]		     = ImVec4(0.2f, 0.2f, 0.21568628f, 1.0f);
	style.Colors[ImGuiCol_FrameBgHovered]	     = ImVec4(0.41201305f, 0.41201568f, 0.41201717f, 1.0f);
	style.Colors[ImGuiCol_FrameBgActive]	     = ImVec4(0.5536425f, 0.55364627f, 0.55364805f, 1.0f);
	style.Colors[ImGuiCol_TitleBg]		     = ImVec4(0.14509805f, 0.14509805f, 0.14901961f, 1.0f);
	style.Colors[ImGuiCol_TitleBgActive]	     = ImVec4(0.14509805f, 0.14509805f, 0.14901961f, 1.0f);
	style.Colors[ImGuiCol_TitleBgCollapsed]	     = ImVec4(0.14509805f, 0.14509805f, 0.14901961f, 1.0f);
	style.Colors[ImGuiCol_MenuBarBg]	     = ImVec4(0.2f, 0.2f, 0.21568628f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarBg]	     = ImVec4(0.2f, 0.2f, 0.21568628f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrab]	     = ImVec4(0.32156864f, 0.32156864f, 0.33333334f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(0.3529412f, 0.3529412f, 0.37254903f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4(0.3529412f, 0.3529412f, 0.37254903f, 1.0f);
	style.Colors[ImGuiCol_CheckMark]	     = ImVec4(0.0f, 0.46666667f, 0.78431374f, 1.0f);
	style.Colors[ImGuiCol_SliderGrab]	     = ImVec4(0.0f, 0.46666667f, 0.78431374f, 1.0f);
	style.Colors[ImGuiCol_SliderGrabActive]	     = ImVec4(0.0f, 0.59557945f, 0.91845495f, 1.0f);
	style.Colors[ImGuiCol_Button]		     = ImVec4(0.2f, 0.2f, 0.21568628f, 1.0f);
	style.Colors[ImGuiCol_ButtonHovered]	     = ImVec4(0.11372549f, 0.5921569f, 0.9254902f, 1.0f);
	style.Colors[ImGuiCol_ButtonActive]	     = ImVec4(0.11372549f, 0.5921569f, 0.9254902f, 1.0f);
	style.Colors[ImGuiCol_Header]		     = ImVec4(0.2f, 0.2f, 0.21568628f, 1.0f);
	style.Colors[ImGuiCol_HeaderHovered]	     = ImVec4(0.11372549f, 0.5921569f, 0.9254902f, 1.0f);
	style.Colors[ImGuiCol_HeaderActive]	     = ImVec4(0.0f, 0.46666667f, 0.78431374f, 1.0f);
	style.Colors[ImGuiCol_Separator]	     = ImVec4(0.30588236f, 0.30588236f, 0.30588236f, 1.0f);
	style.Colors[ImGuiCol_SeparatorHovered]	     = ImVec4(0.30588236f, 0.30588236f, 0.30588236f, 1.0f);
	style.Colors[ImGuiCol_SeparatorActive]	     = ImVec4(0.30588236f, 0.30588236f, 0.30588236f, 1.0f);
	style.Colors[ImGuiCol_ResizeGrip]	     = ImVec4(0.14509805f, 0.14509805f, 0.14901961f, 1.0f);
	style.Colors[ImGuiCol_ResizeGripHovered]     = ImVec4(0.2f, 0.2f, 0.21568628f, 1.0f);
	style.Colors[ImGuiCol_ResizeGripActive]	     = ImVec4(0.32156864f, 0.32156864f, 0.33333334f, 1.0f);
	style.Colors[ImGuiCol_Tab]		     = ImVec4(0.14509805f, 0.14509805f, 0.14901961f, 1.0f);
	style.Colors[ImGuiCol_TabHovered]	     = ImVec4(0.11372549f, 0.5921569f, 0.9254902f, 1.0f);
	style.Colors[ImGuiCol_TabActive]	     = ImVec4(0.0f, 0.46666667f, 0.78431374f, 1.0f);
	style.Colors[ImGuiCol_TabUnfocused]	     = ImVec4(0.14509805f, 0.14509805f, 0.14901961f, 1.0f);
	style.Colors[ImGuiCol_TabUnfocusedActive]    = ImVec4(0.0f, 0.46666667f, 0.78431374f, 1.0f);
	style.Colors[ImGuiCol_PlotLines]	     = ImVec4(0.0f, 0.46666667f, 0.78431374f, 1.0f);
	style.Colors[ImGuiCol_PlotLinesHovered]	     = ImVec4(0.11372549f, 0.5921569f, 0.9254902f, 1.0f);
	style.Colors[ImGuiCol_PlotHistogram]	     = ImVec4(0.0f, 0.46666667f, 0.78431374f, 1.0f);
	style.Colors[ImGuiCol_PlotHistogramHovered]  = ImVec4(0.11372549f, 0.5921569f, 0.9254902f, 1.0f);
	style.Colors[ImGuiCol_TableHeaderBg]	     = ImVec4(0.1882353f, 0.1882353f, 0.2f, 1.0f);
	style.Colors[ImGuiCol_TableBorderStrong]     = ImVec4(0.30980393f, 0.30980393f, 0.34901962f, 1.0f);
	style.Colors[ImGuiCol_TableBorderLight]	     = ImVec4(0.22745098f, 0.22745098f, 0.24705882f, 1.0f);
	style.Colors[ImGuiCol_TableRowBg]	     = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_TableRowBgAlt]	     = ImVec4(1.0f, 1.0f, 1.0f, 0.06f);
	style.Colors[ImGuiCol_TextSelectedBg]	     = ImVec4(0.0f, 0.46666667f, 0.78431374f, 1.0f);
	style.Colors[ImGuiCol_DragDropTarget]	     = ImVec4(0.14509805f, 0.14509805f, 0.14901961f, 1.0f);
	style.Colors[ImGuiCol_NavHighlight]	     = ImVec4(0.14509805f, 0.14509805f, 0.14901961f, 1.0f);
	style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.0f, 1.0f, 1.0f, 0.7f);
	style.Colors[ImGuiCol_NavWindowingDimBg]     = ImVec4(0.8f, 0.8f, 0.8f, 0.2f);
	style.Colors[ImGuiCol_ModalWindowDimBg]	     = ImVec4(0.14509805f, 0.14509805f, 0.14901961f, 1.0f);
}

void CGui::sdl_new_frame(SDL_Window* window)
{
	SDL_GL_MakeCurrent(window, our_context);

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	render_windows();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	SDL_GL_MakeCurrent(game_window, game_context);
}

void CGui::sdl_process_event(SDL_Event* event, int ret)
{
	if (ret > 0 && event != nullptr)
	{
		Uint32 windowID = 0;

		if (event->type == SDL_WINDOWEVENT)
			windowID = event->window.windowID;
		if (event->type == SDL_MOUSEMOTION)
			windowID = event->motion.windowID;

		if (windowID != 0)
		{
			SDL_Window* activeWindow = SDL_GetWindowFromID(windowID);
			if (activeWindow) game_window = activeWindow;
		}
	}

	if (game_window && ImGui::GetCurrentContext())
		ImGui_ImplSDL2_ProcessEvent(event);

	if (menu_open.Get())
		event->type = 0;
}

void CGui::on_swapwindow(SDL_Window* window)
{
	if (!gApp->IsInitialized())
		return;

	if (our_context == nullptr || game_context == nullptr)
	{
		game_context = SDL_GL_GetCurrentContext();
		our_context = SDL_GL_CreateContext(window);

		init_imgui();
		init_impl_imgui_sdl2();

		setup_default_style();
	}

	sdl_new_frame(window);
}

void CGui::shutdown()
{
	if (ImGui::GetCurrentContext() != nullptr)
	{
		if (dxvk_initialized)
		{
			ImGui_ImplDX9_Shutdown();
			ImGui_ImplSDL2_Shutdown();
		}
		else
		{
			ImGui_ImplOpenGL3_Shutdown();
			ImGui_ImplSDL2_Shutdown();
		}
		ImGui::DestroyContext();
	}

	if (our_context != nullptr)
	{
		SDL_GL_DeleteContext(our_context);
		our_context = nullptr;
	}

	game_window = nullptr;
	game_context = nullptr;
	pd3dDevice = nullptr;
	dxvk_initialized = false;
}

void CGui::on_pollevent(SDL_Event* event, int ret)
{
	sdl_process_event(event, ret);
}

void CGui::init_impl_imgui_dxvk()
{
	if (dxvk_initialized || !pd3dDevice || !game_window)
		return;

	if (ImGui::GetCurrentContext() == nullptr)
		init_imgui();

	ImGui_ImplSDL2_InitForVulkan(game_window);
	ImGui_ImplDX9_Init(pd3dDevice);

	dxvk_initialized = true;
}

D3DFORMAT CGui::get_back_buffer_format(IDirect3DDevice9* device)
{
	IDirect3DSurface9* pBackBuffer = nullptr;
	device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer);

	if (pBackBuffer)
	{
		D3DSURFACE_DESC desc;
		pBackBuffer->GetDesc(&desc);
		pBackBuffer->Release();
		return desc.Format;
	}
	return D3DFMT_UNKNOWN;
}

void CGui::dxvk_new_frame()
{
	if (!pd3dDevice || !dxvk_initialized)
		return;

	static bool checkedFormat        = false;
	static bool needsGammaCorrection = false;

	if (!checkedFormat)
	{
		D3DFORMAT format = get_back_buffer_format(pd3dDevice);
		// D3DFMT_A8R8G8B8 or sRGB variants (common DXVK target formats)
		needsGammaCorrection = (format == 22 || format == 21);
		checkedFormat        = true;
	}

	DWORD oldSRGBState = 0;
	if (needsGammaCorrection)
	{
		pd3dDevice->GetRenderState(D3DRS_SRGBWRITEENABLE, &oldSRGBState);
		pd3dDevice->SetRenderState(D3DRS_SRGBWRITEENABLE, false);
	}

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	render_windows();

	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

	if (needsGammaCorrection)
		pd3dDevice->SetRenderState(D3DRS_SRGBWRITEENABLE, oldSRGBState);
}

void CGui::on_present(IDirect3DDevice9* pDevice, const RECT* pSourceRect, const RECT* pDestRect, HWND hDestWindowOverride, const RGNDATA* pDirtyRegion)
{
	if (!gApp->IsInitialized())
		return;

	if (!pd3dDevice)
		pd3dDevice = pDevice;

	init_impl_imgui_dxvk();
	dxvk_new_frame();
}

void CGui::on_reset(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters)
{
	if (gApp->IsInitialized() && dxvk_initialized)
	{
		ImGui_ImplDX9_Shutdown();
		ImGui_ImplSDL2_Shutdown();
		dxvk_initialized = false;
	}
}

void CGui::update_presentation_parameters(D3DPRESENT_PARAMETERS* pPresentationParameters)
{
	if (!gApp->IsInitialized())
		return;

	d3dpp = *pPresentationParameters;
}

static void update_toggle_menu()
{
	if (ImGui::IsKeyPressed(ImGuiKey_Insert, false) || ImGui::IsKeyPressed(ImGuiKey_F11, false))
	{
		menu_open.Get() = !menu_open.Get();
		interfaces::Surface->SetCursorAlwaysVisible(menu_open.Get());
	}

	if (ImGui::IsKeyPressed(ImGuiKey_Escape, false))
	{
		menu_open.Get() = false;
		interfaces::Surface->SetCursorAlwaysVisible(menu_open.Get());
	}
}

void CGui::begin_main_window()
{
	cursor = ImGui::GetMouseCursor();

	features::binds.Update();
	update_toggle_menu();

	window_pushed_font = false;

	switch ((ESPFont)config::esp::font_selected.Get())
	{
		case ESPFont::TF2BUILD:
		ImGui::PushFont(tf2_build, config::esp::font_size.Get());
		window_pushed_font = true;
		break;

		case ESPFont::ARIAL:
		ImGui::PushFont(arial, config::esp::font_size.Get());
		window_pushed_font = true;
		break;

		case ESPFont::INVALID:
		case ESPFont::COUNT:
		{
			features::logs.Error("Invalid font!");
			break;
		}
	}
}

void CGui::end_main_window()
{
	if (window_pushed_font)
		ImGui::PopFont();
}

void CGui::render_main_window_content()
{
	if (menu_open.Get() && !helper::engine::IsTakingScreenshot())
	{
		if (ImGui::Begin("Skill Issue", nullptr, ImGuiWindowFlags_NoCollapse))
		{
			if (ImGui::BeginTabBar("MainTabBar"))
			{
				if (ImGui::BeginTabItem("AIMBOT"))
				{
					if (ImGui::BeginChild("MainTabContent"))
					{
						DrawAimbotTab();
					}
					ImGui::EndChild();
					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("ESP"))
				{
					if (ImGui::BeginChild("MainTabContent"))
					{
						DrawESPTab();
					}
					ImGui::EndChild();
					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("MISC"))
				{
					if (ImGui::BeginChild("MainTabContent"))
					{
						DrawMiscTab();
					}
					ImGui::EndChild();
					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("LUA"))
				{
					if (ImGui::BeginChild("MainTabContent"))
					{
						DrawCodeTab();
					}
					ImGui::EndChild();
					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("NETVARS"))
				{
					if (ImGui::BeginChild("MainTabContent"))
					{
						DrawNetvarsTab();
					}
					ImGui::EndChild();
					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("RADAR"))
				{
					if (ImGui::BeginChild("MainTabContent"))
					{
						DrawRadarTab();
					}
					ImGui::EndChild();
					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("CONFIG"))
				{
					if (ImGui::BeginChild("MainTabContent"))
					{
						DrawConfigTab();
					}
					ImGui::EndChild();
					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("LOGS"))
				{
					if (ImGui::BeginChild("MainTabContent"))
					{
						DrawLogsTab();
					}
					ImGui::EndChild();
					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("CHAMS"))
				{
					if (ImGui::BeginChild("MainTabContent"))
					{
						DrawMaterialEditor();
					}
					ImGui::EndChild();
					ImGui::EndTabItem();
				}

				ImGui::EndTabBar();
			}
		}
		ImGui::End();
	}
}

void CGui::render_windows()
{
	// we didnt even start, so give up
	if (!gApp->IsInitialized())
		return;

	ImDrawList* pDraw = ImGui::GetBackgroundDrawList();
	if (pDraw == nullptr)
		return;

	begin_main_window();

	features::scriptmanager.CallHooks("ImGui");

	features::warp.RunWindow();
	features::radar.Run();
	features::spectators.DrawList();
	features::aimbot.OnImGui(pDraw);
	features::esp.OnImGui();
	features::spyalert.OnImGui(pDraw);
	features::playerlist.DrawWindow();
	features::binds.DrawWindow(menu_open.Get());

	render_main_window_content();

	if (menu_open.Get())
	{
		pDraw->AddText(ImVec2(10, 10), IM_COL32(255, 255, 255, 255), "Skill Issue");
		pDraw->AddText(ImVec2(10, 10 + config::esp::font_size.Get()), IM_COL32(255, 255, 255, 255), "Build date: " __DATE__ " " __TIME__);
	}

	end_main_window();
}