#include <fstream>

#include "../sdk/interfaces/interfaces.h"
#include "../sdk/definitions/d3d9.h"

#include "../thirdparty/imgui/imgui_impl_dx9.h"

#include "../features/gui/gui.h"

#include "../features/hookmanager/hookmanager.hpp"

LPDIRECT3DDEVICE9 d3d_device = nullptr;

// 2 vfunctions after D3D9DeviceEx::Reset
// you find vfunction18 (Present) which calls PresentEx
// but PresentEx and Present are generic as shit so can't reliably sig them
// so use the D3D9SwapChainEx::Present instead
// https://github.com/doitsujin/dxvk/blob/64542839fedb9684a5636660d1d138121ee17eec/src/d3d9/d3d9_device.cpp#L4248
INIT_HOOK(D3D9SwapChainEx_Present, HRESULT, (IDirect3DSwapChain9Ex* swapchain, const RECT* pSourceRect,
    const RECT* pDestRect,
	  HWND hDestWindowOverride,
    const RGNDATA* pDirtyRegion,
	  DWORD dwFlags), "libdxvk_d3d9.so", "41 57 41 56 49 89 D6 41 55 41 54 55 48 89 FD")
{
	if (!d3d_device)
	{
		(void)swapchain->GetDevice(&d3d_device);
		// undo the AddRef from GetDevice
		// https://github.com/doitsujin/dxvk/blob/a9ab43b31d6ee5054a69d31080d43f867bed1d15/src/util/com/com_pointer.h#L17
		// https://github.com/doitsujin/dxvk/blob/a9ab43b31d6ee5054a69d31080d43f867bed1d15/src/d3d9/d3d9_device_child.h#L56
		if (d3d_device) d3d_device->Release();
	}

	if (d3d_device)
	{
		features::gui.on_present(d3d_device, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
	}

	return original_D3D9SwapChainEx_Present(swapchain, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion, dwFlags);
}

// xref: Device reset failed because device still has alive losable resources: Device not reset. Remaining resources:
INIT_HOOK(D3D9DeviceEx_Reset, HRESULT, (LPDIRECT3DDEVICE9 device, D3DPRESENT_PARAMETERS* pPresentationParameters), "libdxvk_d3d9.so", "41 57 41 56 45 31 F6 41 55 41 54 49 89 F4")
{
	features::gui.on_reset(d3d_device, pPresentationParameters);

	HRESULT ret = original_D3D9DeviceEx_Reset(device, pPresentationParameters);

	if (SUCCEEDED(ret))
		features::gui.update_presentation_parameters(pPresentationParameters);

	return ret;
}