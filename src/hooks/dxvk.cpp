#include <fstream>

#include "../sdk/interfaces/interfaces.h"
#include "../sdk/definitions/d3d9.h"

#include "../thirdparty/imgui/imgui_impl_dx9.h"
#include "../thirdparty/libdetour/libdetour.h"

#include "../features/gui/gui.h"

#include "../features/hook_initializer/initializer.h"

typedef HRESULT(__stdcall *Present_t)(IDirect3DDevice9 *, CONST RECT *, CONST RECT *, HWND, CONST RGNDATA *);
typedef HRESULT(__stdcall *Reset_t)(IDirect3DDevice9 *, D3DPRESENT_PARAMETERS *);

DETOUR_DECL_TYPE(HRESULT, original_Present, IDirect3DDevice9 *, CONST RECT *, CONST RECT *, HWND, CONST RGNDATA *);
DETOUR_DECL_TYPE(HRESULT, original_Reset, IDirect3DDevice9 *, D3DPRESENT_PARAMETERS *);

static detour_ctx_t present_ctx;
static detour_ctx_t reset_ctx;

HRESULT __stdcall Hooked_Present
(
	IDirect3DDevice9 *pDevice,
	const RECT *pSourceRect, const RECT *pDestRect,
	HWND hDestWindowOverride, const RGNDATA *pDirtyRegion
)
{
	features::gui.on_present(pDevice, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);

	HRESULT ret;
	DETOUR_ORIG_GET(&present_ctx, ret, original_Present, pDevice, pSourceRect, pDestRect, hDestWindowOverride,
			pDirtyRegion);
	return ret;
}

HRESULT __stdcall Hooked_Reset(IDirect3DDevice9 *pDevice, D3DPRESENT_PARAMETERS *pPresentationParameters)
{
	features::gui.on_reset(pDevice, pPresentationParameters);

	HRESULT ret;
	DETOUR_ORIG_GET(&reset_ctx, ret, original_Reset, pDevice, pPresentationParameters);

	if (SUCCEEDED(ret))
		features::gui.update_presentation_parameters(pPresentationParameters);

	return ret;
}

void *GetModuleBaseAddress(const char *module_name)
{
	std::ifstream file("/proc/self/maps");
	if (!file.is_open())
		return nullptr;

	std::string line;
	while (std::getline(file, line))
	{
		if (line.find(module_name) != std::string::npos)
		{
			size_t dash_pos = line.find('-');
			if (dash_pos != std::string::npos)
			{
				std::string addr_str = line.substr(0, dash_pos);
				return (void *)std::stoull(addr_str, nullptr, 16);
			}
		}
	}

	return nullptr;
}

bool HookD3D9VTable()
{
	// DXVK creates a D3D9 device, we need to get the vtable
	void *dxvk_base = GetModuleBaseAddress("libdxvk_d3d9.so");
	if (!dxvk_base)
	{
		interfaces::Cvar->ConsolePrintf("DXVK not found\n");
		return false;
	}

#ifdef DEBUG
	interfaces::Cvar->ConsolePrintf("DXVK base: %p\n", dxvk_base);
#endif

	// Create a temporary D3D9 device to get the vtable
	void *d3d9_lib = dlopen("libdxvk_d3d9.so", RTLD_LAZY | RTLD_NOLOAD);
	if (!d3d9_lib)
	{
		interfaces::Cvar->ConsolePrintf("Could not load libdxvk_d3d9.so\n");
		return false;
	}

	typedef IDirect3D9 *(__stdcall * Direct3DCreate9_t)(UINT);
	Direct3DCreate9_t pDirect3DCreate9 = (Direct3DCreate9_t)dlsym(d3d9_lib, "Direct3DCreate9");

	if (!pDirect3DCreate9)
	{
		interfaces::Cvar->ConsolePrintf("Could not find Direct3DCreate9\n");
		return false;
	}

	IDirect3D9 *pD3D = pDirect3DCreate9(D3D_SDK_VERSION);
	if (!pD3D)
	{
		interfaces::Cvar->ConsolePrintf("Direct3DCreate9 failed\n");
		return false;
	}

	// Create a dummy device to get vtable
	D3DPRESENT_PARAMETERS d3dpp    = {};
	d3dpp.Windowed		       = TRUE;
	d3dpp.SwapEffect	       = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat	       = D3DFMT_UNKNOWN;
	d3dpp.BackBufferWidth	       = 1;
	d3dpp.BackBufferHeight	       = 1;
	d3dpp.hDeviceWindow	       = nullptr; // DXVK doesnt require this

	IDirect3DDevice9 *pDummyDevice = nullptr;
	HRESULT hr		       = pD3D->CreateDevice(D3DADAPTER_DEFAULT,
							    D3DDEVTYPE_NULLREF, // Use null reference device for dummy
							    nullptr, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pDummyDevice);

	if (FAILED(hr) || !pDummyDevice)
	{
		interfaces::Cvar->ConsolePrintf("Failed to create dummy device (0x%X)\n", hr);
		pD3D->Release();
		return false;
	}

	void **vtable	       = *(void ***)pDummyDevice;

	void *original_Present = vtable[17];
	void *original_Reset   = vtable[16];

	detour_init(&present_ctx, original_Present, (void *)&Hooked_Present);
	if (!detour_enable(&present_ctx))
	{
		interfaces::Cvar->ConsolePrintf("Failed to hook Present\n");
		pDummyDevice->Release();
		pD3D->Release();
		return false;
	}

	detour_init(&reset_ctx, original_Reset, (void *)&Hooked_Reset);
	if (!detour_enable(&reset_ctx))
	{
		interfaces::Cvar->ConsolePrintf("Failed to hook Reset\n");
		detour_disable(&present_ctx);
		pDummyDevice->Release();
		pD3D->Release();
		return false;
	}

	// Cleanup dummy resources
	pDummyDevice->Release();
	pD3D->Release();

#ifdef DEBUG
	interfaces::Cvar->ConsolePrintf("D3D9/DXVK hooked successfully\n");
#endif
	return true;
}

void HookDXVK()
{
	if (GetModuleBaseAddress("libdxvk_d3d9.so") == nullptr)
		return interfaces::Cvar->ConsolePrintf("DXVK not loaded\n");

	if (!HookD3D9VTable())
		return interfaces::Cvar->ConsolePrintf("Failed to hook DXVK\n");
}

MARK_FOR_INIT(HookDXVK);