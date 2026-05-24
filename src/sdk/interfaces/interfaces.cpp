#include "interfaces.h"

#include <cstdint>
#include <cstdio>

#include "createinterface.h"

#include "../../mem.h"

HCursor cursor				= 0;
AttributeHookValueFn AttributeHookValue = nullptr;

namespace interfaces
{
	IVEngineClient014 *Engine	   = nullptr;
	CHLClient *ClientDLL		   = nullptr;
	IClientMode *ClientMode		   = nullptr;
	ICvar *Cvar			   = nullptr;
	IPanel *VGui			   = nullptr;
	ISurface *Surface		   = nullptr;
	IClientEntityList *EntityList	   = nullptr;
	IEngineVGuiInternal *EngineVGui	   = nullptr;
	IVRenderView *RenderView	   = nullptr;
	IInputSystem *InputSystem	   = nullptr;
	IEngineTrace *EngineTrace	   = nullptr;
	IMaterialSystem *MaterialSystem	   = nullptr;
	CGlobalVars *GlobalVars		   = nullptr;
	class Prediction *Prediction	   = nullptr;
	class CInput *CInput		   = nullptr;
	IVModelRender *ModelRender	   = nullptr;
	IKeyValuesSystem *KeyValuesSystem  = nullptr;
	IVModelInfoClient *ModelInfoClient = nullptr;
	void *ClientState		   = nullptr; // fuck C++
	CBaseHudChat *gHUD		   = nullptr;
	IStudioRender *StudioRender	   = nullptr;
	//CEconNotificationQueue* g_notificationQueue = nullptr;
	IPhysics *Physics = nullptr;
	IPhysicsCollision *PhysicsCollision = nullptr;
	ILocalize* VGuiLocalize = nullptr;
	IDemoPlayer* DemoPlayer = nullptr;
	IDemoRecorder* DemoRecorder = nullptr;
} // namespace interfaces

namespace factories
{
	CreateInterfaceFn engine	 = nullptr;
	CreateInterfaceFn client	 = nullptr;
	CreateInterfaceFn vstdlib	 = nullptr;
	CreateInterfaceFn vgui2		 = nullptr;
	CreateInterfaceFn surface	 = nullptr;
	CreateInterfaceFn enginevgui	 = nullptr;
	CreateInterfaceFn inputsystem	 = nullptr;
	CreateInterfaceFn materialsystem = nullptr;
	CreateInterfaceFn studiorender	 = nullptr;
	CreateInterfaceFn vphysics	 = nullptr;
	//CreateInterfaceFn tier0		 = nullptr;
}; // namespace factories

template <typename T> bool GetInterface(T *&out, CreateInterfaceFn factory, const char *name)
{
	if (out)
		return true;

	out = reinterpret_cast<T *>(factory(name, nullptr));

	if (out == nullptr)
	{
		std::fprintf(stderr, "[!] Skill Issue Error: Couldn't get interface '%s'!\n", name);
		return false;
	}

	return true;
}

template <typename T>
static bool GetGlobalInterface(T*& out, const char* module_name, const char* signature, const char* interface_name, bool is_mov_instruction = false)
{
	uintptr_t sig_address = reinterpret_cast<uintptr_t>(sigscan_module(module_name, signature));
	if (!sig_address)
	{
		std::fprintf(stderr, "[!] Skill Issue Error: Signature for '%s' not found in %s!\n", interface_name, module_name);
		return false;
	}

	uintptr_t resolved_addr = RelToAbs(sig_address);

	if (is_mov_instruction)
		out = reinterpret_cast<T*>(resolved_addr);
	else
		out = *reinterpret_cast<T**>(resolved_addr);

	if (!out)
	{
		std::fprintf(stderr, "[!] Skill Issue Error: Interface '%s' resolved to nullptr!\n", interface_name);
		return false;
	}

	return true;
}

static bool init_factory(const char* dll, CreateInterfaceFn& out)
{
	void* lib = dlopen(dll, RTLD_NOLOAD | RTLD_NOW);
	if (lib == nullptr)
	{
		std::fprintf(stderr, "[!] Skill Issue Error: dll '%s' is nullptr!\n", dll);
		return false;
	}

	void* interface_name_ptr = dlsym(lib, "CreateInterface");
	if (interface_name_ptr == nullptr)
	{
		std::fprintf(stderr, "[!] Skill Issue Error: Couldn't get CreateInterface of dll '%s'\n", dll);
		return false;
	}

	out = reinterpret_cast<CreateInterfaceFn>(interface_name_ptr);
	return true;
}

bool init_factories(void)
{
	if (!init_factory("./bin/linux64/engine.so", factories::engine))
		return false;

	if (!init_factory("./tf/bin/linux64/client.so", factories::client))
		return false;

	if (!init_factory("./bin/linux64/libvstdlib.so", factories::vstdlib))
		return false;

	if (!init_factory("./bin/linux64/vgui2.so", factories::vgui2))
		return false;

	if (!init_factory("./bin/linux64/vguimatsurface.so", factories::surface))
		return false;

	if (!init_factory("./bin/linux64/inputsystem.so", factories::inputsystem))
		return false;

	if (!init_factory("./bin/linux64/materialsystem.so", factories::materialsystem))
		return false;

	if (!init_factory("./bin/linux64/studiorender.so", factories::studiorender))
		return false;

	if (!init_factory("./bin/linux64/vphysics.so", factories::vphysics))
		return false;

	return true;
}

bool init_interfaces()
{
	if (!GetInterface(interfaces::Engine, factories::engine, "VEngineClient014"))
		return false;

	if (!GetInterface(interfaces::EngineVGui, factories::engine, "VEngineVGui002"))
		return false;

	if (!GetInterface(interfaces::ClientDLL, factories::client, "VClient017"))
		return false;

	if (!GetInterface(interfaces::Cvar, factories::vstdlib, "VEngineCvar004"))
		return false;

	if (!GetInterface(interfaces::VGui, factories::vgui2, "VGUI_Panel009"))
		return false;

	if (!GetInterface(interfaces::Surface, factories::surface, "VGUI_Surface030"))
		return false;

	if (!GetInterface(interfaces::EntityList, factories::client, "VClientEntityList003"))
		return false;

	if (!GetInterface(interfaces::RenderView, factories::engine, "VEngineRenderView014"))
		return false;

	if (!GetInterface(interfaces::InputSystem, factories::inputsystem, "InputSystemVersion001"))
		return false;

	if (!GetInterface(interfaces::EngineTrace, factories::engine, "EngineTraceClient003"))
		return false;

	if (!GetInterface(interfaces::MaterialSystem, factories::materialsystem, "VMaterialSystem082"))
		return false;

	if (!GetInterface(interfaces::Prediction, factories::client, "VClientPrediction001"))
		return false;

	if (!GetInterface(interfaces::StudioRender, factories::studiorender, "VStudioRender025"))
		return false;

	if (!GetInterface(interfaces::ModelRender, factories::engine, "VEngineModel016"))
		return false;

	if (!GetInterface(interfaces::ModelInfoClient, factories::engine, "VModelInfoClient006"))
		return false;

	if (!GetInterface(interfaces::Physics, factories::vphysics, VPHYSICS_INTERFACE_VERSION))
		return false;

	if (!GetInterface(interfaces::PhysicsCollision, factories::vphysics, VPHYSICS_COLLISION_INTERFACE_VERSION))
		return false;

	if (!GetInterface(interfaces::VGuiLocalize, factories::vgui2, "VGUI_Localize005"))
		return false;

	{
		void *vstdlib = dlopen("./bin/linux64/libvstdlib.so", RTLD_NOLOAD | RTLD_NOW);
		if (!vstdlib)
			return false;

		typedef IKeyValuesSystem *(*KeyValuesSystemFn)();
		KeyValuesSystemFn GetKeyValuesSystem = reinterpret_cast<KeyValuesSystemFn>(dlsym(vstdlib, "KeyValuesSystem"));

		interfaces::KeyValuesSystem = GetKeyValuesSystem();
		if (!interfaces::KeyValuesSystem)
			return false;
	}

	return true;
}

bool init_global_interfaces()
{
	if (!GetGlobalInterface(interfaces::ClientMode, "client.so", "48 8D 05 ? ? ? ? 40 0F B6 F6 48 8B 38", "ClientMode"))
		return false;

	if (!GetGlobalInterface(interfaces::GlobalVars, "client.so", "4C 8D 15 ? ? ? ? 49 8B 02", "GlobalVars"))
		return false;

	AttributeHookValue = reinterpret_cast<AttributeHookValueFn>(sigscan_module("client.so", "55 31 C0 48 89 E5 41 57 41 56 41 55 49 89 F5 41 54 49 89 FC 53 89 CB"));
	if (!AttributeHookValue)
	{
		std::fprintf(stderr, "[!] Skill Issue Error: Failed to find AttributeHookValue signature!\n");
		return false;
	}

	if (!GetGlobalInterface(interfaces::CInput, "client.so", "48 8D 05 ? ? ? ? 48 8B 38 48 8B 07 FF 90 ? ? ? ? 48 8D 15 ? ? ? ? 84 C0", "CInput"))
		return false;

	if (!GetGlobalInterface(interfaces::ClientState, "engine.so", "48 8D 05 ? ? ? ? 4C 8B 40", "ClientState", true))
		return false;

	if (!GetGlobalInterface(interfaces::gHUD, "client.so", "4C 8D 25 ? ? ? ? 53 48 89 FB 4C 89 E7 E8 ? ? ? ?", "gHUD", true))
		return false;

	uintptr_t CEngineClient_vfunction125 = reinterpret_cast<uintptr_t>(sigscan_module("engine.so", "55 48 89 E5 53 48 83 EC 08 48 8D 1D ? ? ? ? 48 8B 3B 48 8B 07 FF 50 20 84 C0 74 ? 48 8B 3B 48 8B 07"));
	if (!CEngineClient_vfunction125)
	{
		std::fprintf(stderr, "[!] Skill Issue Error: CEngineClient_vfunction125 signature broke!\n");
		return false;
	}

	interfaces::DemoRecorder = *reinterpret_cast<IDemoRecorder**>(RelToAbs(CEngineClient_vfunction125 + 0x9));
	if (!interfaces::DemoRecorder)
	{
		std::fprintf(stderr, "[!] Skill Issue Error: DemoRecorder pointer resolved to nullptr!\n");
		return false;
	}

	interfaces::DemoPlayer = *reinterpret_cast<IDemoPlayer**>(RelToAbs(CEngineClient_vfunction125 + 0x26));
	if (!interfaces::DemoPlayer)
	{
		std::fprintf(stderr, "[!] Skill Issue Error: DemoPlayer pointer resolved to nullptr!\n");
		return false;
	}

	return true;
}

bool InitializeInterfaces()
{
	if (!init_factories())
		return false;

	if (!init_interfaces())
		return false;

	if (!init_global_interfaces())
		return false;

	return true;
}