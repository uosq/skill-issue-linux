#pragma once

#include "../include/angelscript.h"

enum ScriptAccessMask
{
	SCRIPT_MASK_ALLOW_ENGINE      = 1 << 0,
	SCRIPT_MASK_ALLOW_CLIENT      = 1 << 1,
	SCRIPT_MASK_ALLOW_CLIENTSTATE = 1 << 2,
	SCRIPT_MASK_ALLOW_COMMON      = 1 << 3,
	SCRIPT_MASK_ALLOW_DRAW	      = 1 << 4,
	SCRIPT_MASK_ALLOW_ENTITYLIST  = 1 << 5,
	SCRIPT_MASK_ALLOW_HOOKS	      = 1 << 6,
	SCRIPT_MASK_ALLOW_IMGUI	      = 1 << 7,
	SCRIPT_MASK_ALLOW_INPUT	      = 1 << 8,
	SCRIPT_MASK_ALLOW_MATERIALS   = 1 << 9,
	SCRIPT_MASK_ALLOW_RENDER      = 1 << 10,
	SCRIPT_MASK_ALLOW_FILESYSTEM  = 1 << 11,
	SCRIPT_MASK_ALLOW_MATH	      = 1 << 12,
	SCRIPT_MASK_ALLOW_ARRAY	      = 1 << 13,
	SCRIPT_MASK_ALLOW_DICT	      = 1 << 14,
	SCRIPT_MASK_ALLOW_DATETIME    = 1 << 15,
	SCRIPT_MASK_ALLOW_STRING      = 1 << 16,
};

asIScriptEngine *GetScriptEngine();
asIScriptContext *GetScriptContext();
int &GetScriptAccessMask();