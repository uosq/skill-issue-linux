#pragma once

#include "sdl.h"

#include "../sdk/definitions/d3d9.h"

void InitImGui();
void CleanupImGui();
void RenderImGui();

D3DFORMAT GetBackBufferFormat(IDirect3DDevice9 *device);
HRESULT __stdcall Hooked_Present(IDirect3DDevice9 *pDevice, const RECT *pSourceRect, const RECT *pDestRect, HWND hDestWindowOverride, const RGNDATA *pDirtyRegion);
HRESULT __stdcall Hooked_Reset(IDirect3DDevice9 *pDevice, D3DPRESENT_PARAMETERS *pPresentationParameters);

void *GetModuleBaseAddress(const char *module_name);
bool HookD3D9VTable();
void HookDXVK();