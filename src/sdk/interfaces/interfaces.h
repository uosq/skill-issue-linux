#pragma once

#include "createinterface.h"
#include <dlfcn.h>

#include "../../libsigscan.h"

#include "../definitions/ceconnotificationqueue.h"
#include "../definitions/cglobalvars.h"
#include "../definitions/cvar.h"
#include "../definitions/enginetool.h"
#include "../definitions/hudbasechat.h"
#include "../definitions/icliententitylist.h"
#include "../definitions/iclientmode.h"
#include "../definitions/ienginetrace.h"
#include "../definitions/ienginevgui.h"
#include "../definitions/igamemovement.h"
#include "../definitions/iinput.h"
#include "../definitions/iinputsystem.h"
#include "../definitions/ikeyvaluessystem.h"
#include "../definitions/imaterialsystem.h"
#include "../definitions/iprediction.h"
#include "../definitions/isurface.h"
#include "../definitions/ivengineclient.h"
#include "../definitions/ivmodelinfoclient.h"
#include "../definitions/ivmodelrender.h"
#include "../definitions/ivrenderview.h"
#include "../definitions/vphysics_interface.h"
#include "../definitions/weaponinfo.h"
#include "../definitions/ilocalize.h"
#include "../definitions/demo.h"
#include "../definitions/cclientstate.h"

using AttributeHookValueFn = float (*)(float defaultValue, const char *attribName, CBaseEntity *pEntity, void *, bool);
extern AttributeHookValueFn AttributeHookValue;

extern HCursor cursor;

namespace interfaces
{
	extern IVEngineClient014 *Engine;
	extern CHLClient *ClientDLL;
	extern IClientMode *ClientMode;
	extern ICvar *Cvar;
	extern IPanel *VGui;
	extern ISurface *Surface;
	extern IClientEntityList *EntityList;
	extern IEngineVGuiInternal *EngineVGui;
	extern IVRenderView *RenderView;
	extern IInputSystem *InputSystem;
	extern IEngineTrace *EngineTrace;
	extern IMaterialSystem *MaterialSystem;
	extern CGlobalVars *GlobalVars;
	extern Prediction *Prediction;
	extern CInput *CInput;
	extern IVModelRender *ModelRender;
	extern IKeyValuesSystem *KeyValuesSystem;
	extern IVModelInfoClient *ModelInfoClient;
	extern CClientState *ClientState;
	extern CBaseHudChat *gHUD;
	#if 0
	extern IGameMovement* GameMovement;
	extern CEconNotificationQueue* g_notificationQueue;
	#endif

	extern IStudioRender *StudioRender;
	extern IPhysics *Physics;
	extern IPhysicsCollision* PhysicsCollision;
	extern ILocalize* VGuiLocalize;
	extern IDemoPlayer* DemoPlayer;
	extern IDemoRecorder* DemoRecorder;
} // namespace interfaces

bool init_factories();
bool init_interfaces();
bool init_global_interfaces();