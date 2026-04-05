#pragma once

#include <dlfcn.h>

#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_dx9.h"
#include "../imgui/imgui_impl_sdl2.h"

#include "../libdetour/libdetour.h"

#include "../gui/gui.h"
#include "sdl.h"

#include "../sdk/definitions/d3d9.h"
#include "../sdk/interfaces/interfaces.h"

#include "../features/binds/binds.h"
#include "../features/radar/radar.h"
#include "../features/warp/warp.h"

void InitImGui();
void CleanupImGui();
void RenderImGui();

D3DFORMAT GetBackBufferFormat(IDirect3DDevice9 *device);
HRESULT __stdcall Hooked_Present(IDirect3DDevice9 *pDevice, const RECT *pSourceRect, const RECT *pDestRect, HWND hDestWindowOverride, const RGNDATA *pDirtyRegion);
HRESULT __stdcall Hooked_Reset(IDirect3DDevice9 *pDevice, D3DPRESENT_PARAMETERS *pPresentationParameters);

void *GetModuleBaseAddress(const char *module_name);
bool HookD3D9VTable();
void HookDXVK();