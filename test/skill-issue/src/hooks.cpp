#include "hooks.h"

#include "sdk/interfaces/interfaces.h"

namespace VMTHooks
{
	VMT Engine;
	VMT EngineVGui;
	VMT InputSystem;
	VMT Client;
	VMT ClientMode;
	VMT Surface;
	VMT ModelRender;
	VMT StudioRender;
	VMT CInput;
	VMT VGui;
}

void VMTHooks::Initialize()
{
	Engine.Setup(interfaces::Engine);
	EngineVGui.Setup(interfaces::EngineVGui);
	InputSystem.Setup(interfaces::InputSystem);
	Client.Setup(interfaces::ClientDLL);
	ClientMode.Setup(interfaces::ClientMode);
	Surface.Setup(interfaces::Surface);
	ModelRender.Setup(interfaces::ModelRender);
	CInput.Setup(interfaces::CInput);
	VGui.Setup(interfaces::VGui);
}

void VMTHooks::Restore()
{
	Engine.Restore();
	EngineVGui.Restore();
	InputSystem.Restore();
	Client.Restore();
	ClientMode.Restore();
	Surface.Restore();
	ModelRender.Restore();
	StudioRender.Restore();
	CInput.Restore();
	VGui.Restore();
}